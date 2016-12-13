#include "hshell.h"
#include "hlib.h"

int shell_get_env_index(shell_t *shell, const char *key) {
	int index = 0;
	while ( index < shell->env_key_size ) {
		if (hstrcmp(shell->env_key[index], key) == 0) {
			return index;
		}
		index++;
	}
	return -1;
}
