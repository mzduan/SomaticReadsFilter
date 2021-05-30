//
// Created by 段明哲 on 2021/5/29.
//

#include "fasta_reader.h"

FastaReader::FastaReader(const char* file_name):line(81)
{
    this->filename=(char*)malloc(sizeof(char)* (strlen(file_name)+1));
    strcpy(this->filename,file_name);
    this->file=fopen(filename,"r");
    size=0;
    capacity=16*1024;
    line_breaker='\n';
    ptr=0;
    roll_ptr=0;
    this->buffer=(char*)malloc(capacity+2);
}
FastaReader::~FastaReader()
{
    if(file)
    {
        fclose(file);
    }
    free(this->filename);
    free(this->buffer);
}
int FastaReader::read_line()
{
    line.clear_string();
    int state=0;
    int last_ptr=ptr;
    while(!terminate_flag)
    {
        if(last_ptr<size) //缓冲区里还有数据，解析直到遇到'\n'
        {
            while(last_ptr<size)
            {
                if(buffer[last_ptr++]==line_breaker){state=1;break;}
            }
            if(state=1){break;}
        }
        else
        {
            if(ptr)
            {
                memmove(buffer,buffer+ptr,size-ptr);
                last_ptr-=ptr;
                size-=ptr;
                ptr=0;
            }
            if(size==capacity)  //分配的容量不足以容下一行数据
            {
                capacity += 4 * 1024;
                buffer = (char*)realloc(buffer, capacity + 2);
            }
            int n=fread(buffer+size,sizeof(char),capacity-size,file);
            if(n==0)
            {
                terminate_flag=true;
            }
            else
            {
                size+=n;
            }
        }
    }
    if(state==1) //读到一行了
    {
        line.append_string(buffer+ptr,last_ptr-ptr-state);
        roll_ptr=ptr;
        ptr=last_ptr;
    }
    return state;
}
bool FastaReader::read_genome(genome &g)
{
    int state=0;
    while(read_line())
    {
        if(line[0]=='>')
        {
            if(state) //读到了下一个genome上，需要回滚
            {
                ptr=roll_ptr;
                break;
            }
            g.reset();
            state=1;
            g.header=g.header+line;
        }
        else
        {
            state=2;
            g.sequence=g.sequence+line;
        }
    }
    return state==2;
}

