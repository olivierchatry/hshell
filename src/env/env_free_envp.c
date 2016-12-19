#include <hshell.h>

void env_free_envp(shell_t *shell) {
	ARRAY_FREE_EACH(shell->envp);
}