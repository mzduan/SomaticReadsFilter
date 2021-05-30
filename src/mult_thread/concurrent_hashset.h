//
// Created by 段明哲 on 2021/4/25.
//

#ifndef CONCURRENT_HASH_SET_CONCURRENT_HASHSET_H
#define CONCURRENT_HASH_SET_CONCURRENT_HASHSET_H

#include <iostream>
#include <mutex>
#include <atomic>
#include "hashset.h"

class uint64_segment
{
private:
    uint64_t *array;
    uint32_t *flags;
    size_t size;
    size_t count;
    size_t max;
    float load_factor;
    std::mutex mtx;
    void encap(int num)
    {
        if(count+num<=max)
        {
            return;
        }
        int n=size;
        while(n*load_factor<count+num)
        {
            n=hashset_find_prime(n*2);
        }
        array=(uint64_t*)realloc(array,n*sizeof(uint64_t));
        if(array == NULL)
        {
            fprintf(stderr, "-- Out of memory --\n");
            abort();
        }
        uint32_t *new_flags = (uint32_t*)malloc((n+15)/16 * 4); //新的flags
        memset(new_flags, 0x55, (n+15)/16 * 4);

        uint32_t *old_flags=this->flags;
        size_t old_size=size;
        this->flags=new_flags;
        this->size=n;
        this->max=this->size*load_factor;
        for(int i=0;i<old_size;i++)
        {
            if(!exists_entity(old_flags, i))
            {
                continue;
            }
            uint64_t key=array[i];
            set_entity_del(old_flags,i);
            while(true)
            {
                size_t new_pos=Jenkins_hash_64(key)%this->size;
                while(!is_entity_null(this->flags, new_pos))
                {
                    new_pos = (new_pos + 1) % this->size;
                }
                clear_entity_null(this->flags, new_pos);
                if(new_pos<old_size && exists_entity(old_flags,new_pos))
                {
                    uint64_t tmp=key;
                    key=this->array[new_pos];
                    this->array[new_pos]=tmp;
                    set_entity_del(old_flags,new_pos);
                }
                else
                {
                    this->array[new_pos]=key;
                    break;
                }

            }
        }
        free(old_flags);
    }

public:
    uint64_segment(size_t size,float load_factor)
    {
        this->size=hashset_find_prime(size);
        this->load_factor=load_factor;
        this->array= (uint64_t*)calloc(this->size,sizeof(uint64_t));
        this->flags=(uint32_t*)malloc((this->size+15)/16*4);
        this->max=this->size*load_factor;
        memset(this->flags, 0x55, (this->size + 15) / 16 * 4);
        this->count=0;
    }
    ~uint64_segment()
    {
        free(this->array);
        free(this->flags);
    }
    void insert(uint64_t key,uint64_t hash_code)
    {
        std::unique_lock<std::mutex> lck (mtx);
        encap(1);
        size_t pos=hash_code%this->size;
        size_t tmp=this->size; //当遇到del标记时，改变此值，防止值直接插入到标记为del的位置，导致被插入两次
        while(true)
        {
            if(is_entity_null(flags,pos))
            {
                if(tmp==this->size)
                {
                    clear_entity_null(flags,pos);
                }
                else
                {
                    pos=tmp;
                    clear_entity_del(flags,pos);
                }
                array[pos]=key;
                count++;
                return;
            }
            else if(is_entity_del(flags,pos))
            {
                if(tmp==this->size)
                {
                    tmp=pos;
                }
            }
            else
            {
                if(this->array[pos]==key)
                {
                    return; //此时哈希表中已经存在该元素了
                }
                //否则，继续循环找下一个位置
            }
            pos++;
            pos=pos%this->size;
        }
    }
    bool exist(uint64_t key,uint64_t hash_code)
    {
        std::unique_lock<std::mutex> lck (mtx);
        size_t pos=hash_code%this->size;
        while(true)
        {
            if(is_entity_null(this->flags,pos))
            {
                return false;
            }
            else if(is_entity_del(this->flags,pos))  //该位置的元素被删除过
            {
                continue;
            }
            else if(this->array[pos]==key)
            {
                return true;
            }
            pos++;
            pos=pos%this->size;
        }

    }
    void remove(uint64_t key,uint64_t hash_code)
    {
        std::unique_lock<std::mutex> lck (mtx);
        size_t pos=hash_code%this->size;
        while(true)
        {
            if(is_entity_null(this->flags,pos))
            {
                return;
            }
            else if(is_entity_del(this->flags,pos))
            {
                continue;
            }
            else if(this->array[pos]==key)
            {
                set_entity_del(flags,pos);
                this->count--;
                return;
            }
            pos++;
            pos=pos%this->size;
        }
    }
};


class uint64_concurrent_hash_set
{
private:
    uint64_segment **segs;
    int seg_counts;
public:
    uint64_concurrent_hash_set()
    {
        this->seg_counts=8;
        segs=new uint64_segment*[seg_counts];
        for(int i=0;i<seg_counts;i++)
        {
            segs[i]=new uint64_segment(1023,0.67);
        }
    }
    int get_segment_id(uint64_t hashcode)
    {
        return (hashcode>>61)&0x7;
    }
    bool exist(uint64_t key)
    {
        uint64_t hash_code=Jenkins_hash_64(key);
        int segment_id=get_segment_id(hash_code);
        return this->segs[segment_id]->exist(key,hash_code);
    }
    void insert(uint64_t key)
    {
        uint64_t hash_code=Jenkins_hash_64(key);
        int segment_id=get_segment_id(hash_code);
        this->segs[segment_id]->insert(key,hash_code);
    }
    void remove(uint64_t key)
    {
        uint64_t hash_code=Jenkins_hash_64(key);
        int segment_id=get_segment_id(hash_code);
        this->segs[segment_id]->remove(key,hash_code);
    }
    ~uint64_concurrent_hash_set()
    {
        for(int i=0;i<seg_counts;i++)
        {
            delete segs[i];
        }
        delete[] segs;
    }

};

#endif //CONCURRENT_HASH_SET_CONCURRENT_HASHSET_H
