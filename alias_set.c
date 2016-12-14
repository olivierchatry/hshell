#include "hshell.h"
#include "hlib.h"

char* alias_set(shell_t *shell, char *alias, char *value) {
	int index = alias_get_index(shell, alias);
	
	char* vvalue = hstrdup(value);
	if (index == -1) {
		ARRAY_ADD(shell->alias_keys, hstrdup(alias), ENV_BUFFER_SIZE);
		ARRAY_ADD(shell->alias_values, vvalue, ENV_BUFFER_SIZE);
	} else {
		if (shell->alias_values[index]) {
			free(shell->alias_values[index]);
		}
		shell->alias_values[index] = vvalue;
	}
	return vvalue;
}