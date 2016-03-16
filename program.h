#include "example3.h"

enum
{
    BADKEY = -1,
    outputPID = 0,
    simplefork,
    NUMKEYS
};

typedef struct key_s
{
    char key_string[];
    int key_value;
} key_t;

extern int string_to_key(char *s);
extern int execute_program(char *s);
