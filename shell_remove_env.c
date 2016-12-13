#include "hshell.h"

void shell_remove_env(shell_t *shell, const char* key) {
	int index = shell_get_env_index(shell, key);
	if (index != -1) {
		free(shell->env_key[index]);
		free(shell->env_value[index]);
		ARRAY_REMOVE(shell->env_key, index);
		ARRAY_REMOVE(shell->env_value, index);
		
	}
}
