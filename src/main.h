/*
Copyright (C) 2012, Alexis Pierru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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
void strip(char*);
Code* init(FILE* const);
int main(int, char**);
void exec(Code* const);
void quit(Code* const);
void* srealloc(void* const, size_t const);

#endif
