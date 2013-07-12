#include "main.h"

/*Don't work O.o*/
char* load(FILE* const source)
{
    char buffer[63], *retval = NULL;
    int quit = 0;
    size_t i, new;
    for(i = 0; !quit; i = new)
    {
        if(!(quit = (fgets(buffer, sizeof(buffer), source) == NULL)))
        {
            char* const tmp = realloc(retval, (new = i + strlen(buffer) + 1));
            if(tmp != NULL)
            {
                retval = tmp;
                strcpy(retval + i, buffer);
            }
            else
            {
                free(retval);
                retval = NULL;
                quit = 1;
            }
        }
    }
    return retval;
}

Code* create(char const* const text)
{
    Code* retval = calloc(1, sizeof(Code));
    if(retval != NULL)
    {
        retval->loop.side = 1;
        if((retval->text = malloc(strlen(text) + 1)) != NULL)
        {
            strcpy(retval->text, text);
        }
        else
        {
            free(retval);
            retval = NULL;
        }
    }
    return retval;
}

int main(int argc,char** argv)
{
    int i, retval = EXIT_SUCCESS;
    for(i = 1; i < argc; ++i)
    {
        FILE* source = fopen(argv[i], "r");
        if(!(error = (source == NULL)))
        {
            char* text = load(source);
            if(text != NULL)
            {
                Code* code = init(text);
                if(!(error = (code == NULL)))
                {
                    exec(code);
                    quit(code);
                }
                free(text);
            }
            fclose(source);
        }
        if(error)
        {
            perror("Error");
            retval = EXIT_FAILURE;
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
