#ifndef EXAMPLE3_H
#define EXAMPLE3_H

extern int do_outputPID(void);
extern int do_outputIDs(void);
extern int do_simplefork(void);
extern int do_twoprocs(void);
extern int do_badprocessID(void);
extern int do_simplechain(int argc, char **argv);
extern int do_simplefan(int argc, char **argv);
extern int do_fanwait(int argc, char **argv);
extern int do_parentwaitpid(void);
extern int do_fanwaitmsg(int argc, char **argv);
extern int do_chainwaitmsg(int argc, char **argv);
extern void show_return_status(void);
extern int do_execls(void);
extern int do_execcmd(int argc, char **argv);
extern int do_execcmdargv(int argc, char **argv);
extern int do_runback(int argc, char **argv);

#endif
