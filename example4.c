#include <errno.h>
#include <unistd.h>
#include "example4.h"

/* The readline function returns the next line from a file. */
int readline(int fd, char *buf, int nbytes)
{
    int numread = 0;
    int retval;

    while (numread < nbytes - 1)
    {
        retval = read(fd, buf + numread, 1);
        if ((retval == -1) && (errno == EINTR))
            continue;
        if ((retval == 0) && (numread == 0))
            return 0;
        if (retval == 0)
            break;
        if (retval == -1)
            return -1;
        numread++;
        if (buf[numread-1] == '\n')
        {
            buf[numread] = '\0';
            return numread;
        }
    }
    errno = EINVAL;
    return -1;
}

/* The copyfile function copies a file from fromfd to tofd. */
int copyfile(int fromfd, int tofd)
{
    char *bp;
    char buf[BLKSIZE];
    int bytesread, byteswritten;
    int totalbytes = 0;

    while(1)
    {
        while ((bytesread = read(fromfd, buf, BLKSIZE)) == -1) && (errno == EINTR));

        if (bytesread <= 0)
            break;

        bp = buf;

        while (bytesread > 0)
        {
            while(((byteswritten = write(tofd, bp, bytesread)) == -1) && (errno == EINTR));

            if (byteswritten <= 0)
                break;
            totalbytes += byteswritten;
            bytesread -= byteswritten;
            bp += byteswritten;
        }
        if (byteswritten == -1)
            break;
    }
    return totalbytes;
}

int do_simplecopy(void)
{
    int numbytes;

    numbytes = copyfile(STDIN_FILENO, STDOUT_FILENO);
    fprintf(stderr, "Number of bytes copied: %d\r\n", numbytes);
    return 0;
}
