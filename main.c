#include <stdio.h>
#include <string.h>
#include "program.h"

int main(int argc, char *argv[])
{
    if( argc == 2 )
    {
        printf("The argument supplied is %s\n", argv[1]);
        if (execute_program(argv[1]) == 0)
        {
            printf("Program returned success!\n");
        }
        else
        {
            printf("Program returned failure\n");
        }
    }
    else if( argc > 2 )
    {
        printf("Too many arguments supplied.\n");
    }
    else
    {
        printf("One argument expected.\n");
    }
	return 0;
}
