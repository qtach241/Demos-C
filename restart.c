#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "restart.h"

/* These restart functions call their non-restart versions and restart themselves if interrupted by a signal. */

pid_t r_wait(int *stat_loc)
{
    pid_t retval;

    /* Only break out of this loop if wait returns non-error, and the function has not been interrupted. */
    while (((retval = wait(stat_loc)) == -1) && (errno == EINTR));
    return retval;
}

ssize_t r_read(int fd, void *buf, size_t size)
{
    ssize_t retval;

    while (retval = read(fd, buf, size), retval == -1 && errno == EINTR);
    return retval;
}

ssize_t r_write(int fd, void *buf, size_t size)
{
    char *bufp;
    size_t bytestowrite;
    ssize_t byteswritten;
    size_t totalbytes;

    for (bufp = buf, bytestowrite = size, totalbytes = 0;
         bytestowrite > 0;
         bufp += byteswritten, bytestowrite -= byteswritten)
    {
        byteswritten = write(fd, bufp, bytestowrite);
        if ((byteswritten == -1) && (errno != EINTR))
            return -1;
        if (byteswritten == -1)
            byteswritten = 0;
        totalbytes += byteswritten;
    }
    return totalbytes;
}

/*
The readblock function is a version of read that continues reading until the
requested number of bytes is read or an error occurs. Useful for reading structures.
*/
ssize_t readblock(int fd, void *buf, size_t size) 
{
    char *bufp;
    size_t bytestoread;
    ssize_t bytesread;
    size_t totalbytes;

    for (bufp = buf, bytestoread = size, totalbytes = 0;
         bytestoread > 0;
         bufp += bytesread, bytestoread -= bytesread)
    {
        bytesread = read(fd, bufp, bytestoread);
        if ((bytesread == 0) && (totalbytes == 0))
            return 0;
        if (bytesread == 0)
        {
            errno = EINVAL;
            return -1;
        }
        if ((bytesread) == -1 && (errno != EINTR))
            return -1;
        if (bytesread == -1)
            bytesread = 0;
        totalbytes += bytesread;
    }
    return totalbytes;
}

int r_close(int fd)
{
    int retval;

    while (retval = close(fd), retval == -1 && errno == EINTR);
    return retval;
}
