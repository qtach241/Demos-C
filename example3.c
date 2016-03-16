#include <stdio.h>
#include <unistd.h>

int do_outputPID(void)
{
    printf("PID = %ld\r\n", (long)getpid());
    printf("PPID = %ld\r\n", (long)getppid());
    return 0;
}

int do_simplefork(void)
{
    int x;

    x = 0;
    fork();
    x = 1;
    printf("PID = %ld, x = %d\r\n", (long)getpid(), x);
    return 0;
}
