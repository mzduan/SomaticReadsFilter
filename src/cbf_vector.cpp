//
// Created by 段明哲 on 2021/5/29.
//

#include "cbf_vector.h"

void cbfvec::set(uint64_t idx,uint8_t data)
{
    uint64_t off=idx * _unit_size;
    bits[off >> 3] = (bits[off >> 3] & (~(mask << (off & 0x07U)))) | (data << (off & 0x07U));
    if((off & 0x07U) + _unit_size > 8)
    {
        bits[(off >> 3) + 1] =(bits[(off >> 3) + 1] & (~(mask >> (8 - (off & 0x07U))))) | (data >> (8 - (off & 0x07U)));
    }
}
uint8_t cbfvec::get(uint64_t idx)
{
    uint64_t off;
    uint8_t dat;
    off = (idx * _unit_size);
    dat = (bits[off >> 3] >> (off & 0x07U));
    if((off & 0x07U) + _unit_size > 8)
    {
        dat |= bits[(off >> 3) + 1] << (8 - (off & 0x07U));
    }
    return dat & mask;
}