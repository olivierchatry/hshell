#include "hshell.h"
#include "hlib.h"

char	*env_set(shell_t *shell, const char *key, const char *value) {
	int		index = env_get_index(shell, key);
	char	*vvalue = hstrdup(value);
	if (index == -1) {
		ARRAY_ADD(shell->env_keys, hstrdup(key), ENV_BUFFER_SIZE);
		ARRAY_ADD(shell->env_values, vvalue, ENV_BUFFER_SIZE);
	} else {
		if (shell->env_values[index]) {
			free(shell->env_values[index]);
		}
		shell->env_values[index] = vvalue;
	}
	env_hook(shell, key);
	return vvalue;
}
