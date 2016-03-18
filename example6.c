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
        write(fd[1], bufout, strlen(bufout)+1); /* Parent code. */
    else 
        bytesin = read(fd[0], bufin, BUFSIZE); /* Child code. */

    fprintf(stderr, "[%ld]:my bufin is {%.*s}, my bufout is {%s}\r\n",
                    (long)getpid(), bytesin, bufin, bufout);
    return 0;
}

/*
The parent creates a pipe before creating n-1 children. After creating all its children, the
parent writes n characters to the pipe. Each process, including the parent, reads a character
from the pipe before proceeding to output its information to standard error. Since the read
from the pipe blocks until there is something to read, each child waits until the parent writes
to the pipe, thereby providing a synchronization point called a barrier. None of the processes
can do any writing to standard error until all of the processes have been created.
*/
int do_synchronizefan(int argc, char *argv[])
{
    char buf[] = "g";
    pid_t childpid = 0;
    int fd[2];
    int i, n;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s %s <num proc>\r\n", argv[0], argv[1]);
        return 1;
    }

    n = atoi(argv[2]);
    if (pipe(fd) == -1)
    {
        perror("Failed to create the synchronization pipe");
        return 1;
    }

    for (i = 1; i < n; i++)
        if ((childpid = fork()) <= 0)
            break; /* Child breaks, parent continues. */

    /* Parent writes to pipe after all chidren created. */
    if (childpid > 0)
    {
        sleep(5); /* wait 5 seconds */
        for (i = 0; i < n; i++)
            if (r_write(fd[1], buf, 1) != 1)
                perror("Failed to write the synchronization characters");
    }
    /* All processes read from pipe, children who try to read before parent writes will block. */
    if (r_read(fd[0], buf, 1) != 1)
        perror("Failed to read synchronization characters");

    fprintf(stderr, "i:%d process ID:%ld parent ID:%ld child ID:%ld\r\n",
            i, (long)getpid(), (long)getppid(), (long)childpid);

    return (childpid == -1);
}

/*
Execute the equivalent of "ls -l | sort -n +4" shell command.
*/
int do_simpleredirect(void)
{
    pid_t childpid;
    int fd[2];

    if ((pipe(fd) == -1) || ((childpid = fork()) == -1))
    {
        perror("Failed to setup pipeline");
        return 1;
    }

    if (childpid == 0)
    {
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("Failed to redirect stdout of ls");
        else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
            perror("Failed to close extra pipe descriptors on ls");
        else
        {
            execl("/bin/ls", "ls", "-l", NULL);
            perror("Failed to exec ls");
        }
        return 1;
    }
    if (dup2(fd[0], STDIN_FILENO) == -1)
        perror("Failed to redirect stdin of sort");
    else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
        perror("Failed to close extra pipe file descriptors on sort");
    else
    {
        execlp("sort", "sort", "-n", NULL);
        perror("Failed to exec sort");
    }
    return 1;
}
