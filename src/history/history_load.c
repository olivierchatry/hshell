
#include <hshell.h>
#include "utils/hlib.h"

/**
 * history_load - Loads shell commands history
 * @shell: Shell structure
 */
void history_load(shell_t *shell)
{
	char				*path;
	char				*content;

	path = history_get_file();
	content = util_read_file(path);
	if (content)
	{
		char	*saveptr;
		char	*token;

		token = hstrtok_r(content, "\n", &saveptr);
		while (token)
		{
			history_add(shell, token);
			token = hstrtok_r(NULL, "\n", &saveptr);
		}
	}
	free(content);
	free(path);
}
