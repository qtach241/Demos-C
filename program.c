#include <stdio.h>
#include <string.h>
#include "program.h"
#include "example3.h"
#include "example4.h"
#include "example6.h"
#include "token_ring.h"

static keycode_t key_lut[] = 
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
    { EXECCMDARGV,   "execcmdargv",   "Like execcmd but with comma delimited arguments" },
    { RUNBACK,       "runback",       "Create a child process to run a background command" },
    { SIMPLECOPY,    "simplecopy",    "Copy a file from stdin to stdout" },
    { COPYFILE,      "copyfile",      "Generic version of simplecopy" },
    { MONITORFORK,   "monitorfork",   "Copy two files to STDOUT using fork" },
    { PARENTPIPE,    "parentpipe",    "Parent writes string to pipe, child reads" },
    { SIMPLEREDIR,   "simpleredir",   "Create pipe to run 'ls -l | sort -n'" },
    { SYNCFAN,       "syncfan",       "Create fan of processes and sync echo to stderr" },
    { PCFIFO,        "pcfifo",        "Parent reads what child has written to named pipe" },
    { PIPESERVER,    "pipeserver",    "Read what is written to FIFO and write to stdout" },
    { PIPECLIENT,    "pipeclient",    "Write an informative message to FIFO" },
    { RING,          "ring",          "Create a token ring of n processes" },
    { LISTKEYS,      "list",          "List all valid keys" }
};

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
    case RUNBACK:          return do_runback(argc, argv);
    case SIMPLECOPY:       return do_simplecopy();
    case COPYFILE:         return do_copyfile(argc, argv);
    case MONITORFORK:      return do_monitorfork(argc, argv);
    case PARENTPIPE:       return do_parentwritepipe();
    case SYNCFAN:          return do_synchronizefan(argc, argv);
    case SIMPLEREDIR:      return do_simpleredirect();
    case PCFIFO:           return do_parentchildfifo(argc, argv);
    case PIPESERVER:       return do_pipeserver(argc, argv);
    case PIPECLIENT:       return do_pipeclient(argc, argv);
    case RING:             return ring(argc, argv);

    case LISTKEYS:         return list_keys();

    case BADKEY:
        printf("Invalid Key!\r\n");
        return BADKEY;
    }
    return -1;
}

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

