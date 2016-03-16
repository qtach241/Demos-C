#include <stdio.h>
#include <string.h>
#include "program.h"

int main(int argc, char *argv[])
{
    if( argc == 2 )
    {
        printf("Argument supplied: '%s'\r\n", argv[1]);
        if (execute_program(argv[1]) == 0)
        {
            printf("Program returned success!\r\n");
        }
        else
        {
            printf("Program returned failure\r\n");
        }
    }
    else if( argc > 2 )
    {
        printf("Too many arguments\r\n");
    }
    else
    {
        printf("One argument expected\r\n");
    }
    return 0;
}
