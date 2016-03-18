#ifndef EXAMPLE4_H
#define EXAMPLE4_H

extern int readline(int fd, char *buf, int nbytes);
extern int copyfile(int fromfd, int tofd);
extern int do_simplecopy(void);

#endif
