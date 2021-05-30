//
// Created by 段明哲 on 2021/5/29.
//

#ifndef SOMATICREADSFILTER_HASHSET_H
#define SOMATICREADSFILTER_HASHSET_H
#include <iostream>
#include <functional>
#define is_entity_null(flags, idx)    ((flags)[(idx)>>4]>>(((idx)&0x0f)<<1)&0x01) //低位为1，表示对应的哈希数组中的位置没有元素
#define is_entity_del(flags, idx)     ((flags)[(idx)>>4]>>(((idx)&0x0f)<<1)&0x02) //高位为1，表示对应的哈希数组中该位置不能使用了
#define exists_entity(flags, idx)     (!((flags)[(idx)>>4]>>(((idx)&0x0f)<<1)&0x03)) //00时返回true，表示对应的哈希数组中该位置已经有元素了
#define set_entity_null(flags, idx)   ((flags)[(idx)>>4] |= (0x01u<<(((idx)&0x0f)<<1))) //低位置1,表示该位置又可以使用了
#define set_entity_del(flags, idx)    ((flags)[(idx)>>4] |= (0x02u<<(((idx)&0x0f)<<1)))
#define clear_entity_null(flags, idx) ((flags)[(idx)>>4] &= ~(0x01u<<(((idx)&0x0f)<<1))) //低位置0，放元素
#define clear_entity_del(flags, idx)  ((flags)[(idx)>>4] &= ~(0x02u<<(((idx)&0x0f)<<1)))

static const uint64_t sys_prime_list[61] =
{
    0x7LLU, 0xfLLU, 0x1fLLU, 0x43LLU, 0x89LLU,
    0x115LLU, 0x22dLLU, 0x45dLLU, 0x8bdLLU, 0x1181LLU,
    0x2303LLU, 0x4609LLU, 0x8c17LLU, 0x1183dLLU, 0x2307bLLU,
    0x460fdLLU, 0x8c201LLU, 0x118411LLU, 0x230833LLU, 0x461069LLU,
    0x8c20e1LLU, 0x11841cbLLU, 0x2308397LLU, 0x461075bLLU, 0x8c20ecbLLU,
    0x11841da5LLU, 0x23083b61LLU, 0x461076c7LLU, 0x8c20ed91LLU, 0x11841db31LLU,
    0x23083b673LLU, 0x461076d1bLLU, 0x8c20eda41LLU, 0x11841db48dLLU, 0x23083b6937LLU,
    0x461076d27fLLU, 0x8c20eda50dLLU, 0x11841db4a59LLU, 0x23083b694ebLLU, 0x461076d29f1LLU,
    0x8c20eda5441LLU, 0x11841db4a887LLU, 0x23083b69511fLLU, 0x461076d2a2c1LLU, 0x8c20eda54591LLU,
    0x11841db4a8b55LLU, 0x23083b69516c1LLU, 0x461076d2a2da5LLU, 0x8c20eda545b55LLU, 0x11841db4a8b6b5LLU,
    0x23083b69516d91LLU, 0x461076d2a2db3bLLU, 0x8c20eda545b69dLLU, 0x11841db4a8b6d5dLLU, 0x23083b69516daf5LLU,
    0x461076d2a2db5edLLU, 0x8c20eda545b6c5fLLU, 0x11841db4a8b6d8ebLLU, 0x23083b69516db1ffLLU, 0x461076d2a2db643fLLU,
    0x8c20eda545b6c8f3LLU
};
static inline uint64_t hashset_find_prime(uint64_t n)
{
    uint32_t i;
    i = 0;
    while(i < 60 && n > sys_prime_list[i]) i ++;
    return sys_prime_list[i];
}
static inline uint64_t Jenkins_hash_64(uint64_t key)
{
    key += ~(key << 32);
    key ^= (key >> 22);
    key += ~(key << 13);
    key ^= (key >> 8);
    key += (key << 3);
    key ^= (key >> 15);
    key += ~(key << 27);
    key ^= (key >> 31);
    return key;
}
template<typename T>
class hash_set
{
private:
    T *array;
    uint32_t *flags;
    size_t size;
    size_t count;
    size_t max;
    float load_factor;
    std::function<uint64_t(T)> hash_func;
    std::function<bool(T,T)> equal_func;
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
        array=(T*)realloc(array,n*sizeof(T));
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
            T key=array[i];
            set_entity_del(old_flags,i);
            while(true)
            {
                size_t new_pos=this->hash_func(key)%this->size;
                while(!is_entity_null(this->flags, new_pos))
                {
                    new_pos = (new_pos + 1) % this->size;
                }
                clear_entity_null(this->flags, new_pos);
                if(new_pos<old_size && exists_entity(old_flags,new_pos))
                {
                    T tmp=key;
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
    hash_set(size_t size,float load_factor,std::function<uint64_t(T)> hash_func,std::function<bool(T,T)> equal_func)
    {
        this->hash_func=hash_func;
        this->equal_func=equal_func;
        this->size=hashset_find_prime(size);
        this->load_factor=load_factor;
        this->array= (T*)calloc(this->size,sizeof(T));
        this->flags=(uint32_t*)malloc((this->size+15)/16*4);
        this->max=this->size*load_factor;
        memset(this->flags, 0x55, (this->size + 15) / 16 * 4);
        this->count=0;
    }
    void insert(T& key)
    {
        uint64_t hash_code=this->hash_func(key);
//        uint64_t hash_code=Jenkins_hash_64(key);
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
                if(this->equal_func(this->array[pos],key))
//                if(this->array[pos]==key)
                {
                    return; //此时哈希表中已经存在该元素了
                }
                //否则，继续循环找下一个位置
            }
            pos++;
            pos=pos%this->size;
        }
    }
    bool exist(T& key)
    {
        uint64_t hash_code=this->hash_func(key);
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
//            else if(this->array[pos]==key)
            else if(this->equal_func(this->array[pos],key))
            {
                return true;
            }
            pos++;
            pos=pos%this->size;
        }
    }
    T* get(T& key)
    {
        uint64_t hash_code=this->hash_func(key);
        size_t pos=hash_code%this->size;
        while(true)
        {
            if(is_entity_null(this->flags,pos))
            {
                return nullptr;
            }
            if(is_entity_del(this->flags,pos))  //该位置的元素被删除过
            {
                continue;
            }
            else if(this->equal_func(this->array[pos],key))
            {
                return &this->array[pos];
            }
            pos++;
            pos=pos%this->size;
        }
    }
    void remove(const T& key)
    {
        uint64_t hash_code=this->hash_func(key);
//        uint64_t hash_code=Jenkins_hash_64(key);
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
//            else if(this->array[pos]==key)
            else if(this->equal_func(this->array[pos],key))
            {
                set_entity_del(flags,pos);
                this->count--;
                return;
            }
            pos++;
            pos=pos%this->size;
        }
    }
    size_t dump(const char* file_name)
    {
        size_t n;
        FILE *fout=fopen(file_name,"w+");
        n=sizeof(size_t)*fwrite(&this->size,sizeof(size_t),1,fout);
        n+=sizeof(size_t)*fwrite(&this->count,sizeof(size_t),1,fout);
        n+=sizeof(size_t)*fwrite(&this->load_factor,sizeof(size_t),1,fout);

        n+=sizeof(T)*fwrite(this->array,sizeof(T),size,fout);
        n+=sizeof(uint32_t)*fwrite(this->flags,sizeof(uint32_t),(this->size + 15) / 16,fout);


        fclose(fout);
        return n;

    }
    void load(const char* file_name)
    {
        FILE *fin=fopen(file_name,"r");

        fread(&this->size,sizeof(size_t),1,fin);
        fread(&this->count,sizeof(size_t),1,fin);
        fread(&this->load_factor,sizeof(size_t),1,fin);
        this->max=this->size*this->load_factor;
        this->array=(T*)malloc(size*sizeof(T));
        fread(&this->array,sizeof(size_t),size,fin);
        this->flags=(uint32_t*)malloc((size + 15)/16 * 4);
        fread(&this->flags,sizeof(uint32_t),(size + 15)/16 * 4,fin);
        fclose(fin);
    }
    void reset()
    {
        this->count=0;
        memset(this->flags, 0x55, (this->size + 15) / 16 * 4);
    }
    void clear()
    {
        free(this->array);
        free(this->flags);
    }


};



#endif //SOMATICREADSFILTER_HASHSET_H
