#include <unistd.h>
#include "hshell.h"
#include "array.h"

void shell_free(shell_t* shell) {
	ARRAY_EACH(shell->env_keys, free);
	ARRAY_EACH(shell->env_values, free);
	ARRAY_FREE(shell->env_keys);
	ARRAY_FREE(shell->env_values);
	ARRAY_EACH(shell->alias_keys, free);
	ARRAY_EACH(shell->alias_values, free);
	ARRAY_FREE(shell->alias_keys);
	ARRAY_FREE(shell->alias_values);
	paths_free(shell);
	env_free_envp(shell);
	close(shell->cancel_pipe[0]);
	close(shell->cancel_pipe[1]);
}
