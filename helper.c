#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

/* Function to create an argument array from a string of tokens */
int makeargv(const char *s, const char *delimiters, char ***argvp)
{
    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    /* Validate Parameters */
    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL))
    {
        errno = EINVAL;
        return -1;
    }

    *argvp = NULL;
    snew = s + strspn(s, delimiters); /* snew is start of actual string, dropping the delimiter. */
    if ((t = (char*)malloc(strlen(snew) + 1)) == NULL)
        return -1;

    strcpy(t, snew);

    numtokens = 0;
    if (strtok(t, delimiters) != NULL) /* Count the number of tokens in the string */
        for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++);

    /* Create argument array for ptrs to the tokens */
    if ((*argvp = (char**)malloc((numtokens + 1)*sizeof(char*))) == NULL)
    {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }

    /* Insert pointers to tokens into the argument array */
    if (numtokens == 0)
    {
        free(t);
    }
    else
    {
        strcpy(t, snew);
        **argvp = strtok(t, delimiters);
        for (i = 1; i < numtokens; i++)
            *((*argvp) + i) = strtok(NULL, delimiters);
    }
    *((*argvp) + numtokens) == NULL; /* Put in the final NULL pointer */
    
    return numtokens;
}

void freemakeargv(char **argv)
{
    if (argv == NULL)
        return;
    if (*argv != NULL)
        free(*argv);
    free(argv);
}

