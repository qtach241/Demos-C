#ifndef HELPER_H
#define HELPER_H

extern int makeargv(const char *s, const char *delimiters, char ***argvp);
extern int freemakeargv(char **argv);

#endif
