//
// Created by 段明哲 on 2021/5/29.
//


#include "bio_string.h"

int bio_string::_string_size_roundup_power2(unsigned int v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}
bio_string::bio_string(int cap)
{
    _size=0;
    capacity=(cap&0x1)? cap : cap + 1;
    _str=(char*)malloc(sizeof(char) * (capacity + 1));
    _str[0]=0;
}
bio_string::~bio_string()
{
    free(_str);
}
void bio_string::encap_string(int inc)
{
    if (inc + _size > capacity)
    {
        if (inc + _size < 0xFFFFFFF)
        {
            capacity = _string_size_roundup_power2(inc + _size) + 1;
        }
        else
        {
            capacity += (((inc + _size) - capacity + 0xFFFFFFF - 1) / 0xFFFFFFF) * 0xFFFFFFF + 1;
        }
        _str = (char *) realloc(_str, capacity + 1);
    }
}
void bio_string::append_string(const char *src, int offlen)
{
    encap_string(offlen);
    memcpy(_str + _size, src, offlen);
    _size += offlen;
    _str[_size] = 0;
}
void bio_string::clear_string()
{
    _size = 0;
    _str[0] = 0;
}
bio_string& bio_string::operator=(const bio_string &s)
{
    if(this!=&s)
    {
        clear_string();
        append_string(s.str(),s.size());
        return *this;
    }
    else
    {
        return *this;
    }
}
const char& bio_string::operator[](int pos)
{
    return _str[pos];
}
bio_string& bio_string::operator+(const bio_string &s)
{
    this->append_string(s.str(),s.size());
    return *this;
}