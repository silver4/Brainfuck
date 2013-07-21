/*
Copyright (C) 2012, Alexis Pierru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "main.h"

char* load(FILE* const source)
{
    char buffer[63], *retval = NULL;
    int quit = 0;
    size_t i, new;
    for(i = 1; !quit; i = new)
    {
        if(!(quit = (fgets(buffer, sizeof(buffer), source) == NULL)))
        {
            retval = srealloc(retval, new = i + strlen(buffer));
            if(!(quit = (retval == NULL)))
            {
                strcpy(retval + i - 1, buffer);
            }
        }
    }
    return retval;
}

void strip(char* text)
{
    char* tmp = text;
    while((text = strpbrk(text, "+-><[].,")) != NULL)
    {
        *tmp++ = *text++;
    }
    *tmp = '\0';
}

Code* init(FILE* const source)
{
    Code* retval = malloc(sizeof(Code));
    if(retval != NULL)
    {
        retval->memory.data = calloc(30000, sizeof(char));
        retval->text.data = load(source);
        if(retval->memory.data != NULL && retval->text.data != NULL)
        {
            strip(retval->text.data);
            retval->memory.i = 0;
        }
        else
        {
            quit(retval);
            retval = NULL;
        }
    }
    return retval;
}

int main(int argc, char** argv)
{
    int i, retval = EXIT_FAILURE;
    for(i = 1; i < argc; ++i)
    {
        FILE* const source = fopen(argv[i], "r");
        if(source != NULL)
        {
            Code* const code = init(source);
            fclose(source);
            if(code != NULL)
            {
                exec(code);
                quit(code);
                retval = EXIT_SUCCESS;
            }
        }
    }
    return retval;
}

void exec(Code* const code)
{
    int skip = 0, side = 1, level = 0, current = -1;

    for(code->text.i = 0;
        code->text.data[code->text.i] != '\0';
        code->text.i += side)
    {
        if(!skip)
        {
            switch(code->text.data[code->text.i])
            {
                case '+':
                    ++code->memory.data[code->memory.i];
                    break;
                case '-':
                    --code->memory.data[code->memory.i];
                    break;
                case '>':
                    ++code->memory.i;
                    break;
                case '<':
                    --code->memory.i;
                    break;
                case '.':
                    putchar(code->memory.data[code->memory.i]);
                    break;
                case ',':
                    code->memory.data[code->memory.i] = getchar();
                    break;
                default:
                    break;
            }
        }
        /*Switch is useless for less than 3 cases*/
        if(code->text.data[code->text.i] == '[')
        {
            if(current == level)
            {
                skip = 0;
                side = 1;
            }
            else if(!skip)
            {
                skip = !code->memory.data[code->memory.i];
                ++current;
            }
            ++level;
        }
        else if(code->text.data[code->text.i] == ']')
        {
            --level;
            if(current == level)
            {
                if((skip = code->memory.data[code->memory.i]))
                {
                    side = -1;
                }
                else
                {
                    --current;
                }
            }
        }
    }
}

void quit(Code* const code)
{
    free(code->text.data);
    free(code->memory.data);
    free(code);
}

void* srealloc(void* const pointer, size_t const size)
{
    void* const retval = realloc(pointer, size);
    if(retval == NULL)
    {
        free(pointer);
    }
    return retval;
}
