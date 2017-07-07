#include <hshell.h>
#include "utils/hlib.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * util_get_home - Retrieves the user's home directory
 *
 * Return: User's home directory path
 */
const char *util_get_home(void)
{
	const char *path = NULL;
	struct passwd *pws;

	pws = getpwuid(geteuid());
	if (pws)
	{
		path = pws->pw_dir;
	}
	return (path);
}
