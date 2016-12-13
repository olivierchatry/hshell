#include "hshell.h"
#include "array.h"

void env_free_envp(shell_t *shell) {
	ARRAY_EACH(shell->envp, free);
	ARRAY_FREE(shell->envp);
}