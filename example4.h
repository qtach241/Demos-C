#ifndef EXAMPLE4_H
#define EXAMPLE4_H

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include "restart.h"

#define READ_FLAGS    (O_RDONLY)
#define WRITE_FLAGS   (O_WRONLY | O_CREAT | O_EXCL )
#define WRITE_PERMS   (S_IRUSR | S_IWUSR)

extern int readline(int fd, char *buf, int nbytes);
extern int copyfile(int fromfd, int tofd);
extern int do_simplecopy(void);
extern int readwrite(int fromfd, int tofd);
extern int do_copyfile(int argc, char **argv);

#endif
