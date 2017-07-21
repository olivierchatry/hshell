#include <hshell.h>
#include "utils/hlib.h"


void env_rebuild_envp(shell_t *shell)
{
	int index;

	env_free_envp(shell);
	for (index = 0; index < shell->env_keys_size; ++index)
	{
		int	size = hstrlen(shell->env_keys[index]);
		char	*env;

		size += hstrlen(shell->env_values[index]);
		size += 2;
		env = hcalloc(size);
		hstrcpy(env, shell->env_keys[index]);
		hstrcat(env, "=");
		hstrcat(env, shell->env_values[index]);
		ARRAY_ADD(shell->envp, env, ENV_BUFFER_SIZE);
	}
	ARRAY_ADD(shell->envp, NULL, ENV_BUFFER_SIZE);
}
