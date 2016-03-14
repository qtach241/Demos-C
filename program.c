#include <stdio.h>
#include <string.h>
#include "program.h"

static key_t key_lut[] = 
{
	{ "outputPID", outputPID },
	{ "simplefork", simplefork }
};

int string_to_key(char *s)
{
	int i;
	for (i = 0; i < NUMKEYS; i++)
	{
		key_t *key = key_lut + i*sizeof(key_t);
		if (strcmp(key->key_string, s) == 0)
			return key->key_value;
	}
	return BADKEY;
}

int execute_program(char *s)
{
	switch(string_to_key(s))
	{
	case outputPID:
		return do_outputPID();
	case simplefork:
		return do_simplefork();
	case BADKEY:
		return BADKEY;
	}
}
