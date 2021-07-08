//
// Created by 段明哲 on 2021/5/29.
//

#ifndef SOMATICREADSFILTER_BIT_VECTOR_H
#define SOMATICREADSFILTER_BIT_VECTOR_H

#include <iostream>
#include <string.h>
#define ONES_STEP_4 0x1111111111111111ULL
#define ONES_STEP_8 0x0101010101010101ULL
class bitvec
{
private:
    uint64_t *bits;
    uint64_t idx;
    uint64_t cap;
    uint64_t one_count;
    uint64_t zero_count;

public:
    bitvec(uint64_t cap);
    ~bitvec();
    void encap(uint64_t num);
    void record_one();
    void record_zero();
    uint64_t get(uint64_t index);
    void clear();
    uint64_t size(){return idx;}
    uint64_t zero_counts(){return zero_count;}
    uint64_t one_counts(){return one_count;}
};






#endif //SOMATICREADSFILTER_BIT_VECTOR_H
