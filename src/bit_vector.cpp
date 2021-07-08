//
// Created by 段明哲 on 2021/5/29.
//
#include "bit_vector.h"

using namespace std;
bitvec::bitvec(uint64_t cap)
{
    if(cap==0)
    {
        this->cap=64*8;
    }
    idx=0;
    this->cap = (((cap + 63) / 64) + 7) / 8 * 64 * 8;
    bits= (uint64_t*)malloc(this->cap/ 8);
    memset(bits, 0, this->cap / 8);
    one_count=0;
    zero_count=0;
}
bitvec::~bitvec()
{
    if(bits!=nullptr)
    {
        free(bits);
    }
}
void bitvec::encap(uint64_t num)
{
    uint64_t tmp_cap;
    tmp_cap = this->cap;
    if(idx + num < this->cap) return;
    while(idx + num >= this->cap)
    {
        if(this->cap < 1024 * 1024 * 8)
        {
            this->cap <<= 1;
        }
        else
        {
            this->cap += 1024 * 1024 * 8;
        }
    }
    bits = (uint64_t*)realloc(bits, this->cap / 8);
    memset((uint8_t*)this->bits + tmp_cap / 8, 0, (this->cap - tmp_cap) / 8);
}
void bitvec::record_one()
{
    encap(1);
    bits[idx>>6] |= 1LLU << (idx&0x3FU);
    idx++;
    one_count++;
}
void bitvec::record_zero()
{
    encap(1);
    bits[idx>>6] &= ~(1LLU << (idx&0x3FU));
    idx++;
    zero_count++;
}
uint64_t bitvec::get(uint64_t index)
{
    return (bits[idx>>6] >> (idx&0x3FU)) & 0x01LLU;
}
void bitvec::clear()
{
    free(bits);
    bits=nullptr;
}
