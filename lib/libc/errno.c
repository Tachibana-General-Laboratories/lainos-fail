#include "errno.h"

/* Temporary */
static int _errno = 0;

int *_errno_func() {
	return &_errno;
}

