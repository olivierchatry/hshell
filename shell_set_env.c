#include "hshell.h"
#include "hlib.h"

char* shell_set_env(shell_t *shell, const char *key, const char *value) {
	int index = shell_get_env_index(shell, key);
	char* vvalue = hstrdup(value);
	if (index == -1) {
		ARRAY_ADD(shell->env_key, hstrdup(key), ENV_BUFFER_SIZE);
		ARRAY_ADD(shell->env_value, vvalue, ENV_BUFFER_SIZE);
	} else {
		shell->env_value[index] = hstrdup(vvalue);
	}
	return vvalue;
}
