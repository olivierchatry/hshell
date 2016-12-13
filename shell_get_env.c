#include "hshell.h"

char* shell_get_env(shell_t *shell, const char *key) {
	int index = shell_get_env_index(shell, key);
	if (index != -1) {
		return shell->env_value[index];
	} 
	return NULL;
}
