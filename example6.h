#ifndef EXAMPLE6_H
#define EXAMPLE6_H

extern int do_parentwritepipe(void);
extern int do_synchronizefan(int argc, char **argv);
extern int do_simpleredirect(void);
extern int do_parentchildfifo(int argc, char **argv);
extern int dofifochild(const char *fifoname, const char *idstring);
extern int dofifoparent(const char *fifoname);
extern int do_pipeserver(int argc, char **argv);
extern int do_pipeclient(int argc, char **argv);

#endif
