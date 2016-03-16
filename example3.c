#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int do_outputPID(void)
{
    printf("PID  = %ld\r\n", (long)getpid());
    printf("PPID = %ld\r\n", (long)getppid());
    return 0;
}

int do_outputIDs(void)
{
    printf("Real UID      = %5ld\r\n", (long)getuid());
    printf("Effective UID = %5ld\r\n", (long)geteuid());
    printf("Real GID      = %5ld\r\n", (long)getgid());
    printf("Effective GID = %5ld\r\n", (long)getegid());
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

int do_twoprocs(void)
{
    pid_t childpid;

    childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0)
        printf("I am child %ld, my parent is %ld\r\n", (long)getpid(), (long)getppid());
    else
        printf("I am parent %ld, my child is %ld\r\n", (long)getpid(), (long)childpid);
    return 0;
}

/*
The parent sets the mypid value to its process ID before the fork. When fork executes, the
child gets a copy of the process address space, including all variables. Since the child does not
reset mypid, the value of mypid for the child mismatches the value returned by getpid.
*/
int do_badprocessID(void)
{
    pid_t childpid;
    pid_t mypid;

    mypid = getpid();
    childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0)
        printf("I am child %ld, PID = %ld\r\n", (long)getpid(), (long)mypid);
    else
        printf("I am parent %ld, PID = %ld\r\n", (long)getpid(), (long)mypid);
    return 0;
}
