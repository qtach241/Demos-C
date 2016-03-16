#include <stdio.h>
#include <string.h>
#include "program.h"

int main(int argc, char *argv[])
{
    int i;

    if( argc > 1 )
    {
        printf("Arguments supplied: ");
        for (int i = 1; i <= argc; i++)
            printf("'%s' \r\n", argv[i]);

        if (execute_program(argc, argv) == 0)
        {
            printf("Program returned success!\r\n");
        }
        else
        {
            printf("Program returned failure\r\n");
        }
    }
    else
    {
        printf("Expect at least one argument\r\n");
    }
    return 0;
}
