#include <stdlib.h>
#include <unistd.h>

#include <hshell.h>
#include "utils/hlib.h"

/**
 * util_get_hostname - Retrieve the host name
 * @full: Should return the full hostname?
 *
 * Return: The hostname
 */
const char *util_get_hostname(bool full)
{
	static char	hostname[1024];
	char		*saveptr;

	hostname[1023] = '\0';
	gethostname(hostname, 1023);
	if (full == false)
		return (hstrtok_r(hostname, ".", &saveptr));
	return (hostname);
}
