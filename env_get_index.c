
#include "hshell.h"
#include "hlib.h"

int env_get_index(shell_t *shell, const char *key) {
	int index = 0;
	while ( index < shell->env_keys_size ) {
		if (hstrcmp(shell->env_keys[index], key) == 0) {
			return index;
		}
		index++;
	}
	return -1;
}
