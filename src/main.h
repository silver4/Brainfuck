#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    struct
    {
        char* data;
        size_t i;
    }memory, text;
}Code;

char* load(FILE* const);
void strip(char* const);
Code* init(FILE* const);
int main(int, char**);
void exec(Code* const);
void quit(Code* const);
void* srealloc(void*, size_t const);

#endif
