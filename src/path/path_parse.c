#include <hshell.h>
#include "utils/hlib.h"

void path_parse(shell_t *shell) {
	path_free(shell);
	
	char	*path = env_get(shell, "PATH");
	if (path) {
		shell->paths_string = hstrdup(path);
		const char *delim = ":;";
		char *token;
		char *saveptr;
		
		token = hstrtok_r(shell->paths_string, delim, &saveptr);
		while (token) {
			ARRAY_ADD(shell->paths, hstrdup(token), PATH_BUFFER_SIZE);
			token = hstrtok_r(NULL, delim, &saveptr);
		}	
	}
}