#include "main.h"

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

int main(void)
{
    Code* code = create("++++++++++[>+++++++>++++++++++>+++>+<\
                         <<<-]>++.>+.+++++++..+++.>++.<<++++++\
                         +++++++++.>.+++.------.--------.>+.>.");
    int retval = EXIT_FAILURE;
    if(code != NULL)
    {
        exec(code);
        quit(code);
        retval = EXIT_SUCCESS;
    }
    else
    {
        perror("Error");
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
    /*OMG it was hard !*/
    free(code->text);
    free(code);
}
