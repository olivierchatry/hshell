#include "hshell.h"
#include "hlib.h"

void	shell_add_env(shell_t* shell, char* env) {
	const char* delim = "=";
	char* saveptr;
	char* key = hstrtok_r(env, delim, &saveptr);
	char* value = hstrtok_r(NULL, delim, &saveptr);
	if (key) {
		if (value) {
			shell_set_env(shell, key, value);
		} else {
			shell_remove_env(shell, key);
		}
	}
}
