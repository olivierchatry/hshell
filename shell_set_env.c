#include "hshell.h"
#include "hlib.h"

char* shell_env_set(shell_t *shell, const char *key, const char *value) {
	int index = shell_env_get_index(shell, key);
	char* vvalue = hstrdup(value);
	if (index == -1) {
		ARRAY_ADD(shell->env_keys, hstrdup(key), ENV_BUFFER_SIZE);
		ARRAY_ADD(shell->env_values, vvalue, ENV_BUFFER_SIZE);
	} else {
		shell->env_values[index] = hstrdup(vvalue);
	}
	return vvalue;
}
