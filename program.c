#include <stdio.h>
#include <string.h>
#include "program.h"

static key_t key_lut[] = 
{
    { outputPID, "outputPID", "Output Process ID and Parent Process ID" },
    { outputIDs, "outputIDs", "Output User ID and Group ID" },
    { simplefork, "simplefork", "Demo fork();" },
    { LISTKEYS, "list", "List all valid keys" }
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

int do_listkeys(void)
{
    int i;
    for (i = 0; i < NUMKEYS; i++)
    {
        printf("[%s] %s\r\n", key_lut[i].key_string, key_lut[i].key_description);
    }
    return 0;
}

int execute_program(char *s)
{
    switch(string_to_key(s))
    {
    case outputPID:    return do_outputPID();
    case outputIDs:    return do_outputIDs();
    case simplefork:   return do_simplefork();
    case LISTKEYS:     return list_keys();

    case BADKEY:
        printf("Invalid Key!\r\n");
        return BADKEY;
    }
    return -1;
}
