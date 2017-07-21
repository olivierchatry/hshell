#include <hshell.h>
#include "utils/hlib.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

/**
 * util_get_user - Retrieve the user's name
 *
 * Return: User name
 */
const char *util_get_user(void)
{
	const char *user = NULL;
	struct passwd *pws;

	pws = getpwuid(geteuid());
	if (pws)
	{
		user = pws->pw_name;
	}
	return (user);
}
