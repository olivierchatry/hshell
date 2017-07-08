#include <hshell.h>
#include "utils/hlib.h"

/**
 * shellrc_get_file - Locate rc file
 *
 * Return: Path to the rc file
 */
char *shellrc_get_file(void)
{
	const char	*home;
	const char	*rc = SHELL_RUNCOM_FILE;
	char		*path = NULL;

	home = util_get_home();
	if (home)
	{
		path = malloc(hstrlen(home) + hstrlen(rc) + 2);
		if (path)
		{
			hstrcpy(path, home);
			hstrcat(path, "/");
			hstrcat(path, rc);
		}
	}

	return (path);
}


/**
 * shellrc_load - Loads shell run commands file
 * @shell: Shell structure
 */
void shellrc_load(shell_t *shell)
{
	char *path;
	char *content;

	path = shellrc_get_file();
	content = util_read_file(path);
	if (content)
	{
		char	*saveptr;
		char	*token;

		token = hstrtok_r(content, "\n", &saveptr);
		while (token)
		{
			command_run(shell, token);
			token = hstrtok_r(NULL, "\n", &saveptr);
		}
	}
	free(content);
	free(path);
}
