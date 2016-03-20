#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "token_ring.h"

/*
Create a ring of processes.
*/
int ring(int argc, char *argv[])
{
    pid_t childpid;    /* indicates process should spawn another */
    int error;         /* return value from dup2 call */
    int fd[2];         /* file descriptors returned by pipe */
    int i;             /* number of this process (starting with 1) */
    int nprocs;        /* total number of processes in ring */

    /* Validate parameters */
    if ( (argc != 3) || ((nprocs = atoi(argv[2])) <= 0) )
    {
        fprintf(stderr, "Usage: %s %s <num procs>\r\n", argv[0], argv[1]);
        return 1;
    }

    /* Coonnect stdin to stdout via a pipe */
    if (pipe(fd) == -1)
    {
        perror("Failed to create starting pipe");
        return 1;
    }
    if ((dup2(fd[0], STDIN_FILENO) == -1) || (dup2(fd[1], STDOUT_FILENO) == -1))
    {
        perror("Failed to connect pipe");
        return 1;
    }
    if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
    {
        perror("Failed to close extra descriptors");
        return 1;
    }

    /* Create the remaining processes */
    for (i =1; i < nprocs; i++)
    {
        if (pipe(fd) == -1)
        {
            fprintf(stderr, "[%ld]:Failed to create pipe %d: %s\r\n",
                            (long)getpid(), i, strerror(errno));
            return 1;
        }
        if ((childpid = fork()) == -1)
        {
            fprintf(stderr, "[%ld]:Failed to create child %d: %s\r\n",
                            (long)getpid(), i, strerror(errno));
            return 1;
        }
        if (childpid > 0)
            error = dup2(fd[1], STDOUT_FILENO);
        else
            error = dup2(fd[0], STDIN_FILENO);

        if (error == -1)
        {
            fprintf(stderr, "[%ld]:Failed to dup pipes for iteration %d: %s\r\n",
                            (long)getpid(), i, strerror(errno));
            return 1;
        }
        if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
        {
            fprintf(stderr, "[%ld]:Failed to close extra descriptors %d: %s\r\n",
                            (long)getpid(), i, strerror(errno));
            return 1;
        }
        if (childpid)
            break;
    }
    fprintf(stderr, "This is process %d with ID %ld and PID %ld\r\n",
                    i, (long)getpid(), (long)getppid());
    return 0;
}
