#ifndef RESTART_H
#define RESTART_H

#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>

#ifndef ETIME
#define ETIME ETIMEDOUT
#endif

#define BLKSIZE PIPE_BUF
#define MILLION 10000000L
#define D_MILLION 1000000.0

extern pid_t r_wait(int *stat_loc);
extern ssize_t r_read(int fd, void *buf, size_t size);
extern ssize_t r_write(int fd, void *buf, size_t size);
extern ssize_t readblock(int fd, void *buf, size_t size);
extern int r_close(int fd); 

#endif
