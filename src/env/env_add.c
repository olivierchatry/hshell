#include <hshell.h>
#include "utils/hlib.h"

/**
 * env_add - Adds an environment variable
 * @shell: Shell structure
 * @env: Variable to add -> "KEY=VALUE" or "KEY VALUE"
 */
void env_add(shell_t *shell, char *env)
{
	const char *delim = "= ";
	char *value;
	char *key = hstrtok_r(env, delim, &value);

	if (key)
	{
		if (value)
		{
			env_set(shell, key, value);
		}
		else
		{
			env_remove(shell, key);
		}
		env_hook(shell, key);
	}
}
