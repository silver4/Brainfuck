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
            retval = srealloc(retval, (new = i + strlen(buffer)));
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
    Code* retval = NULL;
    char* const text = load(source);
    if(text != NULL)
    {
        if((retval = calloc(1, sizeof(Code))) != NULL)
        {
            strip(text);
            retval->loop.side = 1;
            retval->text = text;
        }
        else
        {
            free(text);
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
            Code* code = init(source);
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
    for(code->i = 0; code->text[code->i] != '\0'; code->i += code->loop.side)
    {
        if(!code->loop.skip)
        {
            switch(code->text[code->i])
            {
                case '+':
                    ++code->memory.data[code->memory.cur];
                    break;
                case '-':
                    --code->memory.data[code->memory.cur];
                    break;
                case '>':
                    ++code->memory.cur;
                    break;
                case '<':
                    --code->memory.cur;
                    break;
                case '.':
                    putchar(code->memory.data[code->memory.cur]);
                    break;
                case ',':
                    code->memory.data[code->memory.cur] = getchar();
                    break;
                default:
                    break;
            }
        }
        /*Switch is useless for less than 3 cases*/
        if(code->text[code->i] == '[')
        {
            ++code->loop.level;
            if(code->loop.skip)
            {
                if(code->loop.current == code->loop.level)
                {
                    code->loop.skip = !(code->memory.data[code->memory.cur]);
                    code->loop.side = 1;
                }
            }
            else
            {
                code->loop.skip = !(code->memory.data[code->memory.cur]);
                code->loop.current = code->loop.level;
            }
        }
        else if(code->text[code->i] == ']')
        {
            if(code->loop.current == code->loop.level)
            {
                if(code->loop.skip)
                {
                    code->loop.skip = 0;
                }
                else if((code->loop.skip = code->memory.data[code->memory.cur]))
                {
                    code->loop.side = -1;
                    code->loop.current = code->loop.level;
                }
            }
            --code->loop.level;
        }
    }
}

void quit(Code* const code)
{
    free(code->text);
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
