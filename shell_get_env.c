#include "hshell.h"

char* shell_env_get(shell_t *shell, const char *key) {
	int index = shell_env_get_index(shell, key);
	if (index != -1) {
		return shell->env_values[index];
	} 
	return NULL;
}
