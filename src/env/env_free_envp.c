#include <hshell.h>

/**
 * env_free_envp - Deallocates envp
 * @shell: Shell structure
 */
void env_free_envp(shell_t *shell)
{
	ARRAY_FREE_EACH(shell->envp);
}
