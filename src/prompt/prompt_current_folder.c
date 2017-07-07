#include <hshell.h>
#include "utils/hlib.h"

/**
 * prompt_current_folder - Retrieves the basename of the current directory
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_current_folder(shell_t *shell, char id)
{
	const char *pwd = env_get(shell, "PWD");

	if (hstrcmp(pwd, util_get_home()) == 0)
	{
		ARRAY_ADD(shell->prompt, '~', PROMPT_BUFFER_SIZE);
	}
	else
	{
		int len = hstrlen(pwd) - 1;
		int prev = len;

		while (len && pwd[len] != '/')
		{
			len--;
		}
		if (prev == len)
		{
			len--;
		}
		ARRAY_CAT(shell->prompt, (pwd + len + 1), prev - len, PROMPT_BUFFER_SIZE);
	}
	UNUSED(id);
}

/**
 * prompt_current_folder_full - Expands the full current directory path
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_current_folder_full(shell_t *shell, char id)
{
	const char *pwd = env_get(shell, "PWD");
	const char *home = env_get(shell, "HOME");
	int len = 0;

	if (home == NULL)
		home = util_get_home();
	if (hstrncmp(pwd, home, hstrlen(home)) == 0)
	{
		ARRAY_ADD(shell->prompt, '~', PROMPT_BUFFER_SIZE);
		len = hstrlen(home);
	}
	ARRAY_CAT(shell->prompt, (pwd + len),
		hstrlen(pwd) - len, PROMPT_BUFFER_SIZE);
	UNUSED(id);
}
