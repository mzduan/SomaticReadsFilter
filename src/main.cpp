#include <iostream>
#include "fasta_reader.h"
#include "hashset.h"
#include "bit_vector.h"
#include "cbf.h"
#include "samtools/sam.h"
using namespace std;

//要使用存储kmer_t的hash set，需要定义equal函数和hash函数
typedef struct
{
    uint64_t kmer;
    uint64_t cnt1:16;
    uint64_t cnt2:16;
} kmer_t;
bool kmer_equal(kmer_t k1,kmer_t k2)
{
    return k1.kmer==k2.kmer;
}
uint64_t kmer_hash(kmer_t key)
{
    return Jenkins_hash_64(key.kmer);
}

//要使用存储uint64_t的hash set，需要定义equal函数和hash函数
bool uint64_equal(uint64_t k1,uint64_t k2)
{
    return k1==k2;
}
uint64_t uint64_hash(uint64_t key)
{
    return Jenkins_hash_64(key);
}
int main()
{

    const char* ref_file="/Users/duan/Downloads/data/ref/v38/chr20.fa";
    const char* tumor_bam_file="/Users/duan/Desktop/results/somaticSV/bam/CCS/0.5/somatic_bam_chr20/sim.srt.bam";
    const char* normal_bam_file="/Users/duan/Desktop/results/somaticSV/bam/CCS/0.5/germline_bam_chr20/sim.srt.bam";
    const char* somatic_output_file="/Users/duan/Desktop/somatic.bam";



    //part1: 根据reference构建哈希表
    FastaReader reader(ref_file);
    genome g;
    int kmer_len=31;

    uint64_t kmask = 0xFFFFFFFFFFFFFFFFLLU >> ((32-kmer_len)*2);
    uint64_t ref_kmer=0;
    hash_set<uint64_t> ref_hash(1023,0.67,uint64_hash,uint64_equal);
    std::cout<<"对参考基因组构建hash set"<<std::endl;
    while(reader.read_genome(g))
    {
        int len=g.sequence.size();
        for(int i=0;i<kmer_len;i++)
        {
            ref_kmer=(ref_kmer<<2) |  base_bit_table[(int)g.sequence[i]];
        }
        for(int i=0;i<=len-kmer_len;i++)
        {
            ref_kmer= ((ref_kmer<<2) |  base_bit_table[(int)g.sequence[i]]) & kmask;
            uint64_t rev_key=dna_rev_seq(ref_kmer,kmer_len);
            if(ref_kmer>rev_key)
            {
                ref_kmer=rev_key;
            }
            ref_hash.insert(ref_kmer);
        }
    }

    //part2: 扫描tumor bam,构建bit vector
    bitvec bv(100000);
    bamFile tumor_bam;
    tumor_bam=bam_open(tumor_bam_file,"r");
    bam_header_t *tumor_header = bam_header_read(tumor_bam);

    samfile_t *somatic_out;
    somatic_out=samopen(somatic_output_file, "wb", tumor_header);

    bam_header_destroy(tumor_header);
    uint64_t off = bgzf_tell(tumor_bam);  //记录bam文件中alignments开始的位置

    bam1_t *b = (bam1_t*)calloc(1, sizeof(bam1_t));
    std::cout<<"扫描tumor bam,构建bit vector"<<std::endl;
    while (bam_read1(tumor_bam, b) >= 0)
    {
        uint32_t seq_len=b->core.l_qseq;
        if(seq_len<kmer_len+3) continue;
        uint64_t read_kmer=0;
        uint8_t *seq=bam1_seq(b);
        for(int i=0;i<kmer_len-1;i++)
        {
            read_kmer=(read_kmer<<2) |  bit4_bit_table[bam1_seqi(seq, i)];
        }
        for(int i=0;i<=seq_len-kmer_len;i++)
        {
            read_kmer=((read_kmer<<2) | bit4_bit_table[bam1_seqi(seq, i+kmer_len-1)])  & kmask;
            uint64_t rev_key=dna_rev_seq(read_kmer,kmer_len);
            if(read_kmer>rev_key)
            {
                read_kmer=rev_key;
            }
            if(ref_hash.exist(read_kmer))
            {
                bv.record_one();
            }
            else
            {
                bv.record_zero();
            }
        }
    }

    ref_hash.clear();
    bam_seek(tumor_bam,off,SEEK_SET);
    //part3:,根据bit vector构建cbf，挑选出出现次数大于3的kmer，用于剔除测序错误，剩下的kmer存于新的hash表中


    std::cout<<"扫描tumor bam,根据bit vector构建cbf,剔除测序错误(出现次数大于等于3)"<<std::endl;
    count_bloom_filter cbf(1024*1024*1024,2,3);
    hash_set<kmer_t> read_hash(1023,0.67,kmer_hash,kmer_equal);

    uint64_t idx=0;

    while (bam_read1(tumor_bam, b) >= 0)
    {
        uint32_t seq_len=b->core.l_qseq;
        if(seq_len<kmer_len+3) continue;
        uint64_t read_kmer=0;
        uint8_t *seq=bam1_seq(b);
        for(int i=0;i<kmer_len-1;i++)
        {
            read_kmer=(read_kmer<<2) |  bit4_bit_table[bam1_seqi(seq, i)];
        }
        for(int i=0;i<=seq_len-kmer_len;i++)
        {
            read_kmer= ((read_kmer<<2) | bit4_bit_table[bam1_seqi(seq, i+kmer_len-1)]) & kmask;
            uint64_t rev_key=dna_rev_seq(read_kmer,kmer_len);
            if(read_kmer>rev_key)
            {
                read_kmer=rev_key;
            }
            //构造kmer_t对象，拿它到read hash set里去找
            kmer_t KMER;
            KMER.kmer=read_kmer;
            if(!bv.get(idx))  //只存在于tumor中的kmer，但有可能是测序错误产生，将其存入count bloom filter中
            {
                int occ=cbf.get(KMER.kmer,sizeof(uint64_t));
                if(occ+1>=3)  //算上这次遇见的，该k-mer在cbf中已经出现超过了3次，则认为他是比较可靠的somatic kmer
                {
                    //将该KMER结构加入到read_hash中
                    if(read_hash.exist(KMER))
                    {
                        kmer_t *existed_KMER=read_hash.get(KMER);
                        existed_KMER->cnt1++;
                    }
                    else
                    {
                        KMER.cnt1=occ+1;
                        KMER.cnt2=0;
                        read_hash.insert(KMER);
                    }
                }
                else
                {
                    cbf.put(read_kmer,sizeof(uint64_t));
                }
            }
        }
    }
    bv.clear();
    cbf.clear();

    //目前内存中只有一个read hash，记录了去掉测序错误的，没有出现在reference中的kmer
    //part4: 扫描normal bam,更新read hash中的cnt2

    std::cout<<"扫描normal bam,记录k-mer在normal bam中出现的次数"<<std::endl;
    bamFile normal_bam;
    normal_bam=bam_open(normal_bam_file,"r");
    bam_header_t *normal_header = bam_header_read(tumor_bam);
    bam_header_destroy(normal_header);


    while (bam_read1(normal_bam, b) >= 0)
    {
        uint32_t seq_len=b->core.l_qseq;
        if(seq_len<kmer_len+3) continue;
        uint64_t read_kmer=0;
        uint8_t *seq=bam1_seq(b);
        for(int i=0;i<kmer_len-1;i++)
        {
            read_kmer=(read_kmer<<2) |  bit4_bit_table[bam1_seqi(seq, i)];
        }
        for(int i=0;i<=seq_len-kmer_len;i++)
        {
            read_kmer= ((read_kmer<<2) | bit4_bit_table[bam1_seqi(seq, i+kmer_len-1)] ) & kmask;
            uint64_t rev_key=dna_rev_seq(read_kmer,kmer_len);
            if(read_kmer>rev_key)
            {
                read_kmer=rev_key;
            }
            kmer_t KMER;
            KMER.kmer=read_kmer;
            if(read_hash.exist(KMER))
            {
                kmer_t *existed_KMER=read_hash.get(KMER);
                existed_KMER->cnt2++;
            }
            else
            {
                continue;
            }
        }
    }
    //part5: 最后扫描一遍tumor bam,根据read hash中cnt1和cnt2的比值，判断此次遍历时枚举的kmer是否为somatic kmer,如果是，则将该条alignment输出
    bam_seek(tumor_bam,off,SEEK_SET);
    std::cout<<"输出somatic bam..."<<std::endl;
    while (bam_read1(tumor_bam, b) >= 0)
    {
        uint32_t seq_len=b->core.l_qseq;
        if(seq_len<kmer_len+3) continue;
        uint64_t read_kmer=0;
        uint8_t *seq=bam1_seq(b);
        for(int i=0;i<kmer_len-1;i++)
        {
            read_kmer=(read_kmer<<2) |  bit4_bit_table[bam1_seqi(seq, i)];
        }
        for(int i=0;i<=seq_len-kmer_len;i++)
        {
            read_kmer=((read_kmer<<2) | bit4_bit_table[bam1_seqi(seq, i+kmer_len-1)]) & kmask;
            uint64_t rev_key=dna_rev_seq(read_kmer,kmer_len);
            if(read_kmer>rev_key)
            {
                read_kmer=rev_key;
            }
            kmer_t KMER;
            KMER.kmer=read_kmer;
            if(read_hash.exist(KMER))
            {
                kmer_t *existed_KMER=read_hash.get(KMER);
                int tumor_cnt=existed_KMER->cnt1;
                int normal_cnt=existed_KMER->cnt2;
                if((float)normal_cnt/(tumor_cnt+normal_cnt) <0.01 )
                {
                    //输出该条alignment
                    samwrite(somatic_out, b);
                    break;
                }
            }
            else
            {
                continue;
            }
        }
    }
    read_hash.clear();
    bam_close(tumor_bam);
    bam_close(normal_bam);
    samclose(somatic_out);
    return 0;
}