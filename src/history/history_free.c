#include <hshell.h>

/**
 * history_free - Deallocates history
 * @shell: Shell structure
 */
void history_free(shell_t *shell)
{
	int index;

	for (index = 0; index < shell->history_size; ++index)
	{
		if (shell->history[index])
		{
			free(shell->history[index]);
		}
	}
	free(shell->history);
	shell->history = NULL;
	shell->history_size = 0;
	shell->history_write_index = 0;
}
