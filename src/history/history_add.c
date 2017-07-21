#include <hshell.h>
#include "utils/hlib.h"

/**
 * history_add_r - Adds an entry in the shell history
 * @shell: Shell structure
 * @line: Command line (contains a single line)
 */
void history_add_r(shell_t *shell, const char *line)
{
	int current_index;
	char *current;

	current_index = ((shell->history_write_index - 1) +
		shell->history_size) % (shell->history_size);
	current = shell->history[current_index];
	if ((current == NULL) || (hstrcmp(current, line) != 0))
	{
		free(shell->history[shell->history_write_index]);
		shell->history[shell->history_write_index] = hstrdup(line);
		shell->history_write_index = (shell->history_write_index + 1) %
			(shell->history_size);
		shell->history_count++;
	}
	if (shell->history_count > shell->history_size)
	{
		shell->history_count = shell->history_size;
	}
}

/**
 * history_add - Adds an entry in the shell history
 * @shell: Shell structure
 * @line: Command line (may contain multiple lines)
 */
void history_add(shell_t *shell, const char *line)
{
	char	*token;
	char	*saveptr;
	char	*tmp = hstrdup(line);

	token = hstrtok_r(tmp, "\n", &saveptr);
	while (token)
	{
		history_add_r(shell, token);
		token = hstrtok_r(NULL, "\n", &saveptr);
	}
	free(tmp);
}
