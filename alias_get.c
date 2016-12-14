#include "hshell.h"

char* alias_get(shell_t *shell, const char *key) {
	int index = alias_get_index(shell, key);
	if (index != -1) {
		return shell->alias_values[index];
	} 
	return NULL;
}
