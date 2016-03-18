#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "example6.h"
#include "restart.h"
#include "helper.h"

#define BUFSIZE 10

/*
A program in which a parent writes a string to a pipe and the child reads the string. The
program does not check for I/O errors. Since read calls are not atomic, no guarantee that entire
message gets written.
*/
int do_parentwritepipe(void)
{
    char bufin[BUFSIZE] = "empty";
    char bufout[] = "hello";
    int bytesin;
    pid_t childpid;
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("Failed to create the pipe");
        return 1;
    }

    bytesin = strlen(bufin);
    childpid = fork();
    if (childpid == -1)
    {
        perror("Failed to fork");
        return 1;
    }

    if (childpid)
        write(fd[1], bufout, strlen(bufout)+1);
    else
        bytesin = read(fd[0], bufin, BUFSIZE);

    fprintf(stderr, "[%ld]:my bufin is {%.*s}, my bufout is {%s}\r\n",
                    (long)getpid(), bytesin, bufin, bufout);
    return 0;
}
