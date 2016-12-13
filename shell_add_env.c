#include "hshell.h"
#include "hlib.h"

void	shell_env_add(shell_t* shell, char* env) {
	const char *delim = "= ";
	char *saveptr;
	char *key = hstrtok_r(env, delim, &saveptr);
	char *value = hstrtok_r(NULL, delim, &saveptr);
	if (key) {
		if (value) {
			shell_env_set(shell, key, value);
		} else {
			shell_env_remove(shell, key);
		}
	}
}
