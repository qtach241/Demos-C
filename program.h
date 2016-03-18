#ifndef PROGRAM_H
#define PROGRAM_H

enum
{
    BADKEY = -1,
    OUTPUTPID = 0,
    OUTPUTIDS,
    SIMPLEFORK,
    TWOPROCS,
    BADPROCESSID,
    SIMPLECHAIN,
    SIMPLEFAN,
    FANWAIT,
    PARENTWAITPID,
    FANWAITMSG,
    CHAINWAITMSG,
    EXECLS,
    EXECCMD,
    EXECCMDARGV,
    RUNBACK,
    SIMPLECOPY,
    COPYFILE,
    MONITORFORK,
    PARENTPIPE,
    SYNCFAN,
    LISTKEYS,
    NUMKEYS
};

typedef struct keycode_s
{
    int key_value;
    char *key_string;
    char *key_description;
} keycode_t;

extern int string_to_key(char *s);
extern int list_keys(void);
extern int execute_program(int argc, char **argv);

#endif
