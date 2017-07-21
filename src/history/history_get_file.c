#include <hshell.h>
#include "utils/hlib.h"

/**
 * history_get_file - Locate history file
 *
 * Return: Path to the history file
 */
char *history_get_file(void)
{
	const char	*home;
	const char	*history = SHELL_HISTORY_FILE;
	char		*path = NULL;

	home = util_get_home();
	if (home)
	{
		path = malloc(hstrlen(home) + hstrlen(history) + 2);
		if (path)
		{
			hstrcpy(path, home);
			hstrcat(path, "/");
			hstrcat(path, history);
		}
	}

	return (path);
}
