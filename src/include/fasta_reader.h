//
// Created by 段明哲 on 2021/4/26.
//
#ifndef FASTA_READER
#define FASTA_READER

#include <iostream>
#include "bio_string.h"
static const uint8_t base_bit_table[256] =
{
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  3, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  3, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4
};
static inline uint64_t dna_rev_seq(uint64_t seq, uint8_t seq_size)
{
    seq = ~seq;
    seq = ((seq & 0x3333333333333333LLU)<< 2) | ((seq & 0xCCCCCCCCCCCCCCCCLLU)>> 2);
    seq = ((seq & 0x0F0F0F0F0F0F0F0FLLU)<< 4) | ((seq & 0xF0F0F0F0F0F0F0F0LLU)>> 4);
    seq = ((seq & 0x00FF00FF00FF00FFLLU)<< 8) | ((seq & 0xFF00FF00FF00FF00LLU)>> 8);
    seq = ((seq & 0x0000FFFF0000FFFFLLU)<<16) | ((seq & 0xFFFF0000FFFF0000LLU)>>16);
    seq = ((seq & 0x00000000FFFFFFFFLLU)<<32) | ((seq & 0xFFFFFFFF00000000LLU)>>32);
    return seq >> (64 - (seq_size<<1));
}
static const uint8_t bit4_bit_table[16] = { 4, 0, 1, 4,  2, 4, 4, 4,  3, 4, 4, 4,  4, 4, 4, 4 };
class genome
{
public:
    bio_string header;
    bio_string sequence;
    genome():header(81),sequence(81)
    {
    }
    void reset()
    {
        header.clear_string();
        sequence.clear_string();
    }

};
class FastaReader
{
private:
    FILE *file;
    char* filename;
    char *buffer;
    int size;  //缓冲区已有元素的大小
    int capacity; //缓冲区容量
    bio_string line; //读取一行数据
    char line_breaker;
    int ptr;  //缓冲区ptr以前的数据已被处理，ptr以后的未被处理
    int roll_ptr; //回滚用，用来处理多读的 '>......'
    bool terminate_flag=false;
public:
    FastaReader(const char* file_name);
    ~FastaReader();
    int read_line();
    bool read_genome(genome &g);
};

#endif