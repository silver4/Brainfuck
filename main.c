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

void strip(char* const text)
{
    size_t i, cursor = 0;
    char current;
    for(i = 0; (current = text[i]) != '\0'; ++i)
    {
        char const* const instruction = "+-><[].,";
        char tmp;
        size_t j;
        for(j = 0; (tmp = instruction[j]) != '\0'; ++j)
        {
            if(current == tmp)
            {
                text[cursor] = tmp;
                ++cursor;
            }
        }
    }
    text[cursor] = '\0';
}

Code* init(FILE* const source)
{
    Code* retval = malloc(sizeof(Code));
    if(retval != NULL)
    {
        retval->text.data = load(source);
        retval->memory.data = calloc(30000, sizeof(char));
        if(retval->text.data != NULL && retval->memory.data != NULL)
        {
            strip(retval->text.data);
            retval->text.i = 0;
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

int main(int argc,char** argv)
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
    Loop loop = {0, 1, 0, 0};
    for(code->text.i = 0;
        code->text.data[code->text.i] != '\0';
        code->text.i += loop.side)
    {
        if(!loop.skip)
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
            if(loop.skip)
            {
                if(loop.current == loop.level)
                {
                    loop.skip = !code->memory.data[code->memory.i];
                    loop.side = 1;
                }
            }
            else
            {
                loop.skip = !code->memory.data[code->memory.i];
                loop.current = loop.level;
            }
            ++loop.level;
        }
        else if(code->text.data[code->text.i] == ']')
        {
            --loop.level;
            if(loop.current == loop.level)
            {
                if((loop.skip = code->memory.data[code->memory.i]))
                {
                    loop.side = -1;
                }
                else
                {
                    --loop.current;
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
