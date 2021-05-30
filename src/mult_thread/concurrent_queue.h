//
// Created by 段明哲 on 2021/4/28.
//

#ifndef CONCURRENT_HASH_SET_KMER_QUEUE_H
#define CONCURRENT_HASH_SET_KMER_QUEUE_H
#include <iostream>
#include <mutex>
class uint64_concurrent_queue
{
private:
    uint64_t *array;
    int size;
    int max_size;
    int front;
    int back;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
public:
    uint64_concurrent_queue(int max_size=10000)
    {
        this->max_size=max_size;
        array=new uint64_t[max_size];
        size=0;
        front=0;
        back=0;
    }
    ~uint64_concurrent_queue()
    {
        delete[] array;
    }
    int getsize()
    {
        std::unique_lock<std::mutex> lck (mtx);
        return size;
    }
    void put(uint64_t kmer)
    {
        std::unique_lock<std::mutex> lck (mtx);
        while(size==max_size)  //缓冲区满了
        {
            not_full.wait(lck); //等待不满这一条件发生，此时进程进入阻塞状态
        }
        back = back% max_size;
        array[back] = kmer;
        not_empty.notify_all();
        size++;
        back++;
    }
    uint64_t pop()
    {
        std::unique_lock<std::mutex> lck (mtx);
        while(size==0)  //缓冲区为空
        {
            not_empty.wait(lck);
        }
        front = front % max_size;
        uint64_t ret= array[front];
        not_full.notify_all();
        if( (ret>>62&1)==0) //没有读到结束标志
        {
            size--;
            front++;
        }
        return ret;
    }

};



#endif //CONCURRENT_HASH_SET_KMER_QUEUE_H
