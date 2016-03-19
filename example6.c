#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "example6.h"
#include "restart.h"
#include "helper.h"

#define BUFSIZE 10
#define FIFOSIZE 256
#define FIFO_PERM (S_IRUSR | S_IWUSR)
#define FIFOARG 1
#define FIFO_PERMS (S_IRWXU | S_IWGRP | S_IWOTH)

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
Execute the equivalent of "ls -l | sort -n" shell command.
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

/*
The parent reads what its child has written to a named pipe.
*/
int do_parentchildfifo(int argc, char *argv[])
{
    pid_t childpid;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s %s <pipe name>\r\n", argv[0], argv[1]);
        return 1;
    }

    if (mkfifo(argv[2], FIFO_PERM) == -1)
    {
        if (errno != EEXIST)
        {
            fprintf(stderr, "[%ld]:failed to create named pipe %s: %s\r\n",
                            (long)getpid(), argv[2], strerror(errno));
            return 1;
        }
    }
    if ((childpid = fork()) == -1)
    {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0)
        return dofifochild(argv[2], "Zip Zap Zippity Zoo");
    else
        return dofifoparent(argv[2]);
}

/*
The child writes to the pipe and returns.
*/
int dofifochild(const char *fifoname, const char *idstring)
{
    char buf[FIFOSIZE];
    int fd;
    int rval;
    ssize_t strsize;

    fprintf(stderr, "[%ld]:(Child) about to open FIFO %s...\r\n",
                    (long)getpid(), fifoname);
    while (((fd = open(fifoname, O_WRONLY)) == -1) && (errno == EINTR));
    if (fd == -1)
    {
        fprintf(stderr, "[%ld]:Failed to open named pipe %s for write: %s\r\n",
                        (long)getpid(), fifoname, strerror(errno));
        return 1;
    }
    rval = snprintf(buf, FIFOSIZE, "[%ld]:%s\r\n", (long)getpid(), idstring);
    if (rval < 0)
    {
        fprintf(stderr, "[%ld]:Failed to make the string:\r\n", (long)getpid());
        return 1;
    }

    strsize = strlen(buf) + 1;
    fprintf(stderr, "[%ld]:About to write...\r\n", (long)getpid());
    rval = r_write(fd, buf, strsize);
    if (rval != strsize)
    {
        fprintf(stderr, "[%ld]:Failed to write to pipe: %s\r\n",
                        (long)getpid(), strerror(errno));
        return 1;
    }
    fprintf(stderr, "[%ld]:Finishing...\r\n", (long)getpid());
    return 0;
}

/*
The parent reads what was written to the named pipe.
*/
int dofifoparent(const char *fifoname)
{
    char buf[FIFOSIZE];
    int fd;
    int rval;

    fprintf(stderr, "[%ld]:(Parent) about to open FIFO %s...\r\n",
                    (long)getpid(), fifoname);
    while (((fd = open(fifoname, O_RDONLY)) == -1) && (errno == EINTR)) ;
    if (fd == -1) 
    {
        fprintf(stderr, "[%ld]:Failed to open named pipe %s for read: %s\r\n",
                        (long)getpid(), fifoname, strerror(errno));
        return 1;
    }

    fprintf(stderr, "[%ld]:About to read...\r\n", (long)getpid());

    rval = r_read(fd, buf, FIFOSIZE);
    if (rval == -1) 
    {
        fprintf(stderr, "[%ld]:Failed to read from pipe: %s\r\n",
                        (long)getpid(), strerror(errno));
        return 1;
    }
    fprintf(stderr, "[%ld]:Read %.*s\r\n", (long)getpid(), rval, buf);
    return 0;
}

/*
The program reads what is written to a named pipe and writes it to standard output.
*/
int do_pipeserver(int argc, char *argv[])
{
    int requestfd;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s %s <fifo name> [> <log file>]\r\n", argv[0], argv[1]);
        return 1;
    }

    if ((mkfifo(argv[2], FIFO_PERMS) == -1) && (eerno != EEXIST))
    {
        perror("Server failed to create FIFO");
        return 1;
    }

    if ((requestfd = open(argv[2], O_RDWR)) == -1)
    {
        perror("Server failed to open its FIFO");
        return 1;
    }
    copyfile(requestfd, STDOUT_FILENO);
    return 1;
}
