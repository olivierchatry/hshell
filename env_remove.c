#include "hshell.h"

int env_remove(shell_t *shell, const char* key) {
	int index = env_get_index(shell, key);
	if (index != -1) {
		free(shell->env_keys[index]);
		free(shell->env_values[index]);
		ARRAY_REMOVE(shell->env_keys, index);
		ARRAY_REMOVE(shell->env_values, index);		
		env_hook(shell, key);
		return 1;
	}
	return 0;
}
