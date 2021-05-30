//
// Created by 段明哲 on 2021/5/29.
//

#ifndef SOMATICREADSFILTER_CBF_H
#define SOMATICREADSFILTER_CBF_H

#include "cbf_vector.h"
class count_bloom_filter
{
private:
    cbfvec* bits;
    size_t _cap;  //有多少个单元格
    uint64_t *hash_codes; //存储哈希函数计算得到的hash值
    uint32_t n_seed ;
    uint8_t _unit_size;
    uint8_t *cnts;  //存储出现次数
public:
    count_bloom_filter(size_t size, uint8_t uint_size, uint32_t n_seed);
    uint8_t put(const uint64_t key, uint32_t len);
    uint8_t get(const uint64_t key, uint32_t len);
    void clear(){this->bits->clear();free(bits);free(cnts);};

};
static int cbf_total_seeds=20;
static const uint32_t cbf_seeds[20] =
{
    100663319ul,  201326611ul,  402653189ul,  805306457ul,  1610612741ul,
    3145739ul,    6291469ul,    12582917ul,   25165843ul,   50331653ul,
    98317ul,      196613ul,     393241ul,     786433ul,     1572869ul,
    3079ul,       6151ul,       12289ul,      24593ul,      49157ul
};
static inline uint64_t MurmurHash64A(const void * key, int len, uint32_t seed)
{
    const uint64_t m = 0xc6a4a7935bd1e995LLU;
    const int r = 47;
    uint64_t h = seed ^ (len * m);
    const uint64_t * data = (const uint64_t *)key;
    const uint64_t * end = data + (len/8);
    while(data != end)
    {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }
    const unsigned char * data2 = (const unsigned char*)data;

    switch(len & 7){
        case 7: h ^= ((uint64_t)data2[6]) << 48;
        case 6: h ^= ((uint64_t)data2[5]) << 40;
        case 5: h ^= ((uint64_t)data2[4]) << 32;
        case 4: h ^= ((uint64_t)data2[3]) << 24;
        case 3: h ^= ((uint64_t)data2[2]) << 16;
        case 2: h ^= ((uint64_t)data2[1]) << 8;
        case 1: h ^= ((uint64_t)data2[0]);
            h *= m;
    };
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return h;
}

#endif //SOMATICREADSFILTER_CBF_H
