#include <unistd.h>
#include "hshell.h"
#include "array.h"

void shell_free(shell_t  *shell) {
	ARRAY_FREE_EACH(shell->env_keys);
	ARRAY_FREE_EACH(shell->env_values);

	ARRAY_FREE_EACH(shell->alias_keys);
	
	ARRAY_EACH(shell->alias_commands, command_chain_free);
	ARRAY_FREE_EACH(shell->alias_commands);

	paths_free(shell);
	env_free_envp(shell);
	close(shell->cancel_pipe[0]);
	close(shell->cancel_pipe[1]);
	if (shell->command_reminder) {
		free(shell->command_reminder);
	}
	history_free(shell);
}
