#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "restart.h"

/* Restart versions of traditional functions */

pid_t r_wait(int *stat_loc)
{
    pid_t retval;

    /* Only break out of this loop if wait returns non-error, and the function has not been interrupted. */
    while (((retval = wait(stat_loc)) == -1) && (errno == EINTR));
    return retval;
}
