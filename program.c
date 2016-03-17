#include <stdio.h>
#include <string.h>
#include "program.h"
#include "example3.h"

static key_t key_lut[] = 
{
    { OUTPUTPID,     "outputPID",     "Output Process ID and Parent Process ID" },
    { OUTPUTIDS,     "outputIDs",     "Output User ID and Group ID" },
    { SIMPLEFORK,    "simplefork",    "Demonstrate fork() function" },
    { TWOPROCS,      "twoprocs",      "Test PID after fork()" },
    { BADPROCESSID,  "badprocessID",  "PID mismatch gotcha" },
    { SIMPLECHAIN,   "simplechain",   "Create chain of n processes" },
    { SIMPLEFAN,     "simplefan",     "Create fan of n processes" },
    { FANWAIT,       "fanwait",       "Like simplefan but only prints after all processes exit" },
    { PARENTWAITPID, "parentwaitpid", "See page 101 example 3.18" },
    { FANWAITMSG,    "fanwaitmsg",    "Fork processes in a fan and print msg when finished" },
    { CHAINWAITMSG,  "chainwaitmsg",  "Fork processes in a chain and print msg when finished" },
    { EXECLS,        "execls",        "Create a child process to run ls -l" },
    { EXECCMD,       "execcmd",       "Create a child process to run any command" },
    { EXECCMDARGV,   "execcmdargv",   "Like execcmd but with hyphen delimited arguments" },
    { LISTKEYS,      "list",          "List all valid keys" }
};

int string_to_key(char *s)
{
    int i;
    for (i = 0; i < NUMKEYS; i++)
    {
        if (strcmp(key_lut[i].key_string, s) == 0)
            return key_lut[i].key_value;
    }
    return BADKEY;
}

int list_keys(void)
{
    int i;
    for (i = 0; i < NUMKEYS; i++)
    {
        printf("[%s] %s\r\n", key_lut[i].key_string, key_lut[i].key_description);
    }
    return 0;
}

int execute_program(int argc, char **argv)
{
    switch(string_to_key(argv[1]))
    {
    case OUTPUTPID:        return do_outputPID();
    case OUTPUTIDS:        return do_outputIDs();
    case SIMPLEFORK:       return do_simplefork();
    case TWOPROCS:         return do_twoprocs();
    case BADPROCESSID:     return do_badprocessID();
    case SIMPLECHAIN:      return do_simplechain(argc, argv);
    case SIMPLEFAN:        return do_simplefan(argc, argv);
    case FANWAIT:          return do_fanwait(argc, argv);
    case PARENTWAITPID:    return do_parentwaitpid();
    case FANWAITMSG:       return do_fanwaitmsg(argc, argv);
    case CHAINWAITMSG:     return do_chainwaitmsg(argc, argv);
    case EXECLS:           return do_execls();
    case EXECCMD:          return do_execcmd(argc, argv);
    case EXECCMDARGV:      return do_execcmdargv(argc, argv);

    case LISTKEYS:         return list_keys();

    case BADKEY:
        printf("Invalid Key!\r\n");
        return BADKEY;
    }
    return -1;
}
