#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "example4.h"
#include "restart.h"
#include "helper.h"

/*
The readline function returns the next line from a file.
*/
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

/* 
The copyfile function copies a file from fromfd to tofd. Notice that the write statement specifies 
the buffer by a pointer, bp, rather than by a fixed address such as buf. If the previous write operation 
did not output all of buf, the next write operation must start from the end of the previous output. 
*/
#ifdef USEDEP
int copyfile(int fromfd, int tofd)
{
    char *bp;
    char buf[BLKSIZE];
    int bytesread, byteswritten;
    int totalbytes = 0;

    fprintf(stderr, "Warning: Using deprecated version of copyfile\r\n");

    while(1)
    {
        /* Restarts the read if interrupted. */
        while (((bytesread = read(fromfd, buf, BLKSIZE)) == -1) && (errno == EINTR));

        if (bytesread <= 0)
            break;

        bp = buf;

        while (bytesread > 0)
        {
            /* Restarts the write if interrupted. */
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
#else
/* 
Using the restart versions of read and write simplify copyfile greatly. Can drop the scratch variables
and the bp pointer as r_read and r_write handle interruptions internally.
*/
int copyfile(int fromfd, int tofd)
{
    char buf[BLKSIZE];
    int bytesread, byteswritten;
    int totalbytes = 0;

    while(1)
    {
        if ((bytesread = r_read(fromfd, buf, BLKSIZE)) <= 0)
            break;
        if ((byteswritten = r_write(tofd, buf, bytesread)) == -1)
            break;
        totalbytes += byteswritten;
    }
    return totalbytes;
}
#endif

int do_simplecopy(void)
{
    int numbytes;

    numbytes = copyfile(STDIN_FILENO, STDOUT_FILENO);
    fprintf(stderr, "Number of bytes copied: %d\r\n", numbytes);
    return 0;
}

/*
The readwrite function reads from one file descriptor and writes all the bytes read to another file
descriptor.
*/
int readwrite(int fromfd, int tofd)
{
    char buf[BLKSIZE];
    int bytesread;

    if ((bytesread = r_read(fromfd, buf, BLKSIZE)) == -1)
        return -1;
    if (bytesread == 0)
        return 0;
    if (r_write(tofd, buf, bytesread) == -1)
        return -1;

    return bytesread;
}

/*
The readwriteblock function copies a fixed number of bytes from one file descriptor to another.
*/
int readwriteblock(int fromfd, int tofd, char *buf, int size)
{
    int bytesread;

    bytesread = readblock(fromfd, buf, size);
    if (bytesread != size) /* Can only be 0 or -1 */
        return bytesread;

    return r_write(tofd, buf, size);
}

/*
Open a file, copy contents to a destination file. Creates a destination file if one doesn't exist with
the name passed in.
*/
int do_copyfile(int argc, char *argv[])
{
    int bytes;
    int fromfd, tofd;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s %s <from file> <to file>\r\n", argv[0], argv[1]);
        return 1;
    }

    if ((fromfd = open(argv[2], READ_FLAGS)) == -1)
    {
        perror("Failed to open input file");
        return 1;
    }

    if ((tofd = open(argv[3], WRITE_FLAGS, WRITE_PERMS)) == -1)
    {
        perror("Failed to create output file");
        return 1;
    }

    bytes = copyfile(fromfd, tofd);
    printf("%d bytes copied from %s to %s\r\n", bytes, argv[2], argv[3]);

    return 0; /* The return closes the file. */
}

/*
Monitors two files by forking a child process.
*/
int do_monitorfork(int argc, char *argv[])
{
    int bytesread;
    int childpid;
    int fd, fd1, fd2;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s %s <file1> <file2>\r\n", argv[0], argv[1]);
        return 1;
    }

    if ((fd1 = open(argv[2], O_RDONLY)) == -1)
    {
        fprintf(stderr, "Failed to open file %s:%s\r\n", argv[2], strerror(errno));
        return 1;
    }
    if ((fd2 = open(argv[3], O_RDONLY)) == -1)
    {
        fprintf(stderr, "Failed to open file %s:%s\r\n", argv[2], strerror(errno));
        return 1;
    }

    if ((childpid = fork()) == -1)
    {
        perror("Failed to create child process");
        return 1;
    }

    if (childpid > 0)
        fd = fd1;
    else
        fd = fd2;

    bytesread = copyfile(fd, STDOUT_FILENO);
    fprintf(stderr, "Bytes read: %d\r\n", bytesread);
    return 0;
}
