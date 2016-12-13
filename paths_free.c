#include "hshell.h"
#include "array.h"
#include "hlib.h"

void paths_free(shell_t *shell) {
	ARRAY_EACH(shell->paths, free);
	ARRAY_FREE(shell->paths);
	if (shell->paths_string) {
		free(shell->paths_string);
		shell->paths_string = NULL;
	}	
}