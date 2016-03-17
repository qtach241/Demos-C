#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "program.h"

int main(int argc, char *argv[])
{
    int i;

    if( argc > 1 )
    {
        printf("Arguments supplied: ");
        for (i = 1; i < argc; i++)
        {
            printf("'%s' ", argv[i]);
        }
        printf("\r\n");

        if (execute_program(argc, argv) == 0)
        {
            printf("Program returned success! (Process: %ld)\r\n", (long)getpid());
        }
        else
        {
            printf("Program returned failure\r\n");
        }
    }
    else
    {
        printf("At least one argument expected\r\n");
    }
    return 0;
}
