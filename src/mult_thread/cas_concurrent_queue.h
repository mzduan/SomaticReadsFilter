//
// Created by 段明哲 on 2021/4/29.
//

#ifndef CONCURRENT_HASH_SET_CAS_LIST_H
#define CONCURRENT_HASH_SET_CAS_LIST_H
#include <atomic>
#include <vector>
#include <unistd.h>
class uint64_cas_concurrent_queue
{
public:
    std::atomic<int> front;
    std::atomic<int> back;
    std::atomic<int> size;
    const int capacity;
    std::vector<uint64_t> array;
public:
    uint64_cas_concurrent_queue(int c=1000):capacity(c),size(0),front(0),back(0),array(capacity,0){}
    bool is_full()
    {
        return size.load()==capacity;
    }
    bool is_empty()
    {
        return size.load()==0;
    }
    void push(uint64_t key)
    {
        int b=back.load();
        while(true)
        {
            if(b==-1 || is_full())
            {
                b=back.load();
                continue;
            }
            if(back.compare_exchange_weak(b, -1))
            {
                if(is_full())
                {
                    int flag=-1;
                    back.compare_exchange_weak(flag,b);
                    continue;
                }
                break;
            }
        }
        array[b]=key;
        ++size;
        int flag=-1;
        back.compare_exchange_weak(flag,(b+1)%capacity);
    }
    uint64_t pop()
    {
        uint64_t data;
        int f=front.load();
        while(true)
        {
            if(f==-1 || is_empty())
            {
                f=front.load();
                continue;
            }
            if(front.compare_exchange_weak(f,-1))
            {
                if(is_empty())
                {
                    int flag=-1;
                    front.compare_exchange_weak(flag,f);
                    continue;
                }
                break;
            }
        }
        uint64_t ret=array[f];
        if((ret>>62&1)==0)
        {
            --size;
            data=ret;
            int flag=-1;
            front.compare_exchange_weak(flag,(f+1)%capacity);
            return data;
        }
        else
        {
            data=ret;
            int flag=-1;
            front.compare_exchange_weak(flag,f%capacity);
            return data;
        }
    }
};
#endif //CONCURRENT_HASH_SET_CAS_LIST_H
