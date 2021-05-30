#include <iostream>
#include "fasta_reader.h"
#include "hashset.h"
#include "concurrent_hashset.h"
#include "concurrent_queue.h"
#include "cas_concurrent_queue.h"
#include <thread>
using namespace std;
bool uint64_equal(uint64_t k1,uint64_t k2)
{
    return k1==k2;
}
uint64_t uint64_hash(uint64_t key)
{
    return Jenkins_hash_64(key);
}

void print_time()
{
    char str[50];
    time_t now = time(NULL);
    strftime(str, 50, "%x %X", localtime(&now));
    cout << str << endl; //输出开始时间
}
void run(uint64_concurrent_hash_set &hashset,uint64_cas_concurrent_queue& q)
{
    uint64_t kmer;
    while(true)
    {
        kmer=q.pop();
        if(kmer>>62&1)  //遇到结束标志，线程退出
        {
            return;
        }
        hashset.insert(kmer);
    }
}
int main()
{

    /* 测试单线程版本哈希表
    print_time();
    const char* ref_file="/Users/duan/Downloads/data/ref/v38/chr20.fa";
    FastaReader reader(ref_file);
    genome g;
    int kmer_len=32;
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
            ref_kmer=((ref_kmer<<2) |  base_bit_table[(int)g.sequence[i]]) & kmask;
            uint64_t rev_key=dna_rev_seq(ref_kmer,kmer_len);
            if(ref_kmer>rev_key)
            {
                ref_kmer=rev_key;
            }
            ref_hash.insert(ref_kmer);
        }
    }

    const char* hashset_out="/Users/duan/Desktop/chr20_hashset.out";
    ref_hash.dump(hashset_out);

    ref_hash.clear();

    print_time();
    return 0;


     */

    /* 构建多线程版本哈希表，借助线程安全的队列
    print_time();
    const char* ref_file="/Users/duan/Downloads/data/ref/v38/chr20.fa";
    FastaReader reader(ref_file);
    genome g;
    int kmer_len=31;
    uint64_t kmask = 0xFFFFFFFFFFFFFFFFLLU >> ((32-kmer_len)*2);
    uint64_t ref_kmer=0;
    uint64_concurrent_hash_set ref_hash;
    uint64_concurrent_queue q;


    std::thread worker1(run,std::ref(ref_hash),std::ref(q));
    std::thread worker2(run,std::ref(ref_hash),std::ref(q));

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
            ref_kmer=((ref_kmer<<2) |  base_bit_table[(int)g.sequence[i]]) & kmask;
            uint64_t rev_key=dna_rev_seq(ref_kmer,kmer_len);
            if(ref_kmer>rev_key)
            {
                ref_kmer=rev_key;
            }
            q.put(ref_kmer);
        }
    }
    q.put(1llu<<62); //放入结束标志
    worker1.join();
    worker2.join();
    print_time();
    return 0;


     */

    print_time();
    const char* ref_file="/Users/duan/Downloads/data/ref/v38/chr20.fa";
    FastaReader reader(ref_file);
    genome g;
    int kmer_len=31;
    uint64_t kmask = 0xFFFFFFFFFFFFFFFFLLU >> ((32-kmer_len)*2);
    uint64_t ref_kmer=0;
    uint64_concurrent_hash_set ref_hash;
    uint64_cas_concurrent_queue q;


    std::thread worker1(run,std::ref(ref_hash),std::ref(q));
    std::thread worker2(run,std::ref(ref_hash),std::ref(q));
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
            ref_kmer=((ref_kmer<<2) |  base_bit_table[(int)g.sequence[i]]) & kmask;
            uint64_t rev_key=dna_rev_seq(ref_kmer,kmer_len);
            if(ref_kmer>rev_key)
            {
                ref_kmer=rev_key;
            }
            q.push(ref_kmer);
        }
    }
    q.push(1llu<<62); //放入结束标志
    worker1.join();
    worker2.join();
    print_time();
    return 0;

}