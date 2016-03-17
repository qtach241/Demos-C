#ifndef HELPER_H
#define HELPER_H

extern int makeargv(const char *s, const char *delimiters, char ***argvp);
extern void freemakeargv(char **argv);

#endif
