//
// Created by 段明哲 on 2021/5/29.
//

#ifndef SOMATICREADSFILTER_BIO_STRING_H
#define SOMATICREADSFILTER_BIO_STRING_H

#include <iostream>
class bio_string
{
private:
    char *_str;
    int _size;
    int capacity;
public:
    bio_string(int cap);
    ~bio_string();
    void encap_string(int inc);
    void append_string(const char *src, int offlen);
    void clear_string();
    bio_string& operator=(const bio_string &s);
    const char& operator[](int pos);
    bio_string& operator+(const bio_string &s);
    static int _string_size_roundup_power2(unsigned int v);
    int size() const {return _size;};
    const char* str() const {return _str;};

};

#endif //SOMATICREADSFILTER_BIO_STRING_H
