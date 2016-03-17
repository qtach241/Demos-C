#include <stdio.h>
#include <string.h>
#include "program.h"

static key_t key_lut[] = 
{
    { outputPID,     "outputPID",     "Output Process ID and Parent Process ID" },
    { outputIDs,     "outputIDs",     "Output User ID and Group ID" },
    { simplefork,    "simplefork",    "Demonstrate fork() function" },
    { twoprocs,      "twoprocs",      "Test PID after fork()" },
    { badprocessID,  "badprocessID",  "PID mismatch gotcha" },
    { simplechain,   "simplechain",   "Create chain of n processes" },
    { simplefan,     "simplefan",     "Create fan of n processes" },
    { fanwait,       "fanwait",       "Like simplefan but only prints after all processes exit" },
    { parentwaitpid, "parentwaitpid", "See page 101 example 3.18" },
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
    case outputPID:        return do_outputPID();
    case outputIDs:        return do_outputIDs();
    case simplefork:       return do_simplefork();
    case twoprocs:         return do_twoprocs();
    case badprocessID:     return do_badprocessID();
    case simplechain:      return do_simplechain(argc, argv);
    case simplefan:        return do_simplefan(argc, argv);
    case fanwait:          return do_fanwait(argc, argv);
    case parentwaitpid:    return do_parentwaitpid();

    case LISTKEYS:         return list_keys();

    case BADKEY:
        printf("Invalid Key!\r\n");
        return BADKEY;
    }
    return -1;
}
