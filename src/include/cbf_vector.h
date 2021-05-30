//
// Created by 段明哲 on 2021/5/29.
//

#ifndef SOMATICREADSFILTER_CBF_VECTOR_H
#define SOMATICREADSFILTER_CBF_VECTOR_H
#include <iostream>

class cbfvec
{
private:
    uint8_t *bits;
    uint8_t _unit_size;  //每个单元格占多少位
    uint64_t _cap;  //单元格的数量
    uint32_t mask;
public:
    cbfvec(uint64_t cap,uint8_t unit_size):_cap(cap),_unit_size(unit_size)
    {
        bits= (uint8_t*)malloc((_cap * _unit_size + 7) / 8);
        mask=(1U << _unit_size) - 1U;
    }
    void clear(){free(bits);}
    void set(uint64_t idx,uint8_t data);
    uint8_t get(uint64_t idx);
};


#endif //SOMATICREADSFILTER_CBF_VECTOR_H
