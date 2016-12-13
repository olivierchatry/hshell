#include "hshell.h"
#include "hlib.h"

void	env_add(shell_t* shell, char* env) {
	const char *delim = "= ";
	char *saveptr;
	char *key = hstrtok_r(env, delim, &saveptr);
	char *value = hstrtok_r(NULL, delim, &saveptr);
	if (key) {
		if (value) {
			env_set(shell, key, value);
		} else {
			env_remove(shell, key);
		}
	}
}
