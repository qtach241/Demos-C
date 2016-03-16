#include "example3.h"

enum
{
    BADKEY = -1,
    outputPID = 0,
    outputIDs,
    simplefork,
    twoprocs,
    badprocessID,
    simplechain,
    LISTKEYS,
    NUMKEYS
};

typedef struct key_s
{
    int key_value;
    char *key_string;
    char *key_description;
} key_t;

extern int string_to_key(char *s);
extern int list_keys(void);
extern int execute_program(int argc, char **argv);
