#include <stdlib.h>
#include <unistd.h>
#include "hlib.h"

const char* util_get_hostname() {
	static char hostname[1024];
	char				*saveptr;

	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	return hstrtok_r(hostname, ".", &saveptr);
}
