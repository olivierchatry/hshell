#include "hshell.h"

void shell_env_remove(shell_t *shell, const char* key) {
	int index = shell_env_get_index(shell, key);
	if (index != -1) {
		free(shell->env_keys[index]);
		free(shell->env_values[index]);
		ARRAY_REMOVE(shell->env_keys, index);
		ARRAY_REMOVE(shell->env_values, index);
		
	}
}
