#include <stdio.h>
#include <string.h>
#include "program.h"

static key_t key_lut[] = 
{
    { "outputPID", outputPID },
    { "simplefork", simplefork }
};

int string_to_key(char *s)
{
    printf("string_to_key start\r\n");
    int i;
    for (i = 0; i < NUMKEYS; i++)
    {
        key_t *key = key_lut + i*sizeof(key_t);
        if (strcmp(key->key_string, s) == 0)
            return key->key_value;
    }
    printf("string_to_key exit\r\n");
    return BADKEY;
}

int execute_program(char *s)
{
    printf("execute_program\r\n");
    switch(string_to_key(s))
    {
    case outputPID:
        printf("do_outputPID();\r\n");
        //return do_outputPID();
        break;
    case simplefork:
        printf("do_simplefork();\r\n");
        //return do_simplefork();
        break;
    case BADKEY:
        printf("BADKEYr\n");
        return BADKEY;
        break;
    }
    return -1;
}
