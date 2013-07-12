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

char* load(FILE* const);
Code* init(char const* const);
int main(int, char**);
void exec(Code* const);
void quit(Code* const);

#endif
