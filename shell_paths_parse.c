#include "hshell.h"
#include "hlib.h"

void shell_paths_parse(shell_t* shell) {
	ARRAY_EACH(shell->paths, free);
	ARRAY_FREE(shell->paths);
	if (shell->paths_string) {
		free(shell->paths_string);
		shell->paths_string = NULL;
	}

	char* path = shell_env_get(shell, "PATH");
	if (path) {
		shell->paths_string = hstrdup(path);
		const char *delim = ":;";
		char *token;
		char *saveptr;
		
		token = hstrtok_r(path, delim, &saveptr);
		while (token) {
			ARRAY_ADD(shell->paths, hstrdup(token), PATH_BUFFER_SIZE);
		}
		
	}
}