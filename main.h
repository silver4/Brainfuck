#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    struct
    {
        int skip, side;
        int unsigned level, current;
    }loop;
    struct
    {
        char data[30000];
        size_t cur;
    }memory;
    char* text;
    size_t i;
}Code;

Code* create(char const* const);
int main(void);
void exec(Code* const);
void quit(Code* const);

#endif