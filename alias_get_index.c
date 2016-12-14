#include "hshell.h"
#include "hlib.h"

int alias_get_index(shell_t *shell, const char *key) {
	int index = 0;
	while ( index < shell->alias_keys_size ) {
		if (hstrcmp(shell->alias_keys[index], key) == 0) {
			return index;
		}
		index++;
	}
	return -1;
}
