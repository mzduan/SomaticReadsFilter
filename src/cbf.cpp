//
// Created by 段明哲 on 2021/5/30.
//

#include "cbf.h"



count_bloom_filter::count_bloom_filter(size_t cap, uint8_t unit_size, uint32_t n_seed)
{
    this->_cap=cap;
    this->n_seed=n_seed;
    this->_unit_size=unit_size;
    this->hash_codes = (uint64_t*)malloc(sizeof(uint64_t) * n_seed);
    this->cnts  = (uint8_t *)malloc(sizeof(uint8_t) * n_seed);
    this->bits=new cbfvec(_cap,_unit_size);
}
uint8_t count_bloom_filter::put(const uint64_t key, uint32_t len)
{
    uint8_t min=0xFFU;
    uint32_t flag=0;
    for(int i=0;i<n_seed;i++)
    {
        hash_codes[i]=MurmurHash64A(&key, len, cbf_seeds[(i) % cbf_total_seeds]) % _cap;
        cnts[i]=bits->get(hash_codes[i]);
        if(cnts[i]<min)
        {
            min=cnts[i];
            flag = 1U << i;
        }
        else if(cnts[i]==min)
        {
            flag |= 1U << i;
        }
    }
    if(min>=(1U << _unit_size) - 1)  //超过了每个单元格所能表示的最大值
    {
        return min;
    }
    min++;
    for(int i=0;i < n_seed; i++)
    {
        if((flag >> i) & 0x01)
        {
            bits->set(hash_codes[i],min);
        }
    }
    return min;

}
uint8_t count_bloom_filter::get(const uint64_t key, uint32_t len)
{
    uint8_t min=0xFFU;
    for(int i=0;i<n_seed;i++)
    {
        hash_codes[i] = MurmurHash64A(&key, len, cbf_seeds[i % cbf_total_seeds]) % _cap;
        cnts[i] = bits->get(hash_codes[i]);
        if(cnts[i] < min)
        {
            min = cnts[i];
        }
    }
    return min;
}