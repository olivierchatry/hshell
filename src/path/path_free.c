#include <hshell.h>
#include "utils/hlib.h"

void paths_free(shell_t *shell) {
	ARRAY_FREE_EACH(shell->paths);
	if (shell->paths_string) {
		free(shell->paths_string);
		shell->paths_string = NULL;
	}	
}