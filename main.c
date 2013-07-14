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

char* strip(char const* text)
{
    char* retval = NULL;
    int quit = 0;
    size_t i;
    for(i = 0; !quit; ++i)
    {
        retval = srealloc(retval, i + 1);
        if(!(quit = (retval == NULL)))
        {
            text = strpbrk(text, "+-><.,[]");
            retval[i] = (quit = (text == NULL))? '\0': *text;
            ++text;
        }
    }
    return retval;
}

Code* init(char const* const text)
{
    Code* retval = calloc(1, sizeof(Code));
    if(retval != NULL)
    {
        retval->loop.side = 1;
        retval->text = malloc(strlen(text) + 1);
        if(retval->text != NULL)
        {
            strcpy(retval->text, text);
        }
    }
    return retval;
}

int main(int argc,char** argv)
{
    int i, retval = EXIT_FAILURE;
    for(i = 1; i < argc; ++i)
    {
        FILE* source = fopen(argv[i], "r");
        if(source != NULL)
        {
            char* const text = load(source);
            fclose(source);
            if(text != NULL)
            {
                char* const stripped = strip(text);
                free(text);
                if(stripped != NULL)
                {
                    Code* code = init(stripped);
                    free(stripped);
                    if(code != NULL)
                    {
                        exec(code);
                        quit(code);
                        retval = EXIT_SUCCESS;
                    }
                }
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
