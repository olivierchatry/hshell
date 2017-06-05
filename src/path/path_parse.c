#include <hshell.h>
#include "utils/hlib.h"

void path_parse(shell_t *shell) {
	char	*path;

	path_free(shell);
	path = env_get(shell, "PATH");
	if (path) {
		const char *delim = ":;";
		char *token;
		char *saveptr;

		shell->paths_string = hstrdup(path);
		if (hstrchr(delim, *shell->paths_string)) {
			ARRAY_ADD(shell->paths, hstrdup("."), PATH_BUFFER_SIZE);			
		}
		token = hstrtok_r(shell->paths_string, delim, &saveptr);
		while (token) {
			ARRAY_ADD(shell->paths, hstrdup(token), PATH_BUFFER_SIZE);
			token = hstrtok_r(NULL, delim, &saveptr);
		}
	}
}
