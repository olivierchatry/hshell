#include <unistd.h>

#include "hshell.h"
#include "hlib.h"

void shell_getcwd(shell_t *shell) {
	int 	buffer_size = 1024;
	char	*buffer = malloc(buffer_size);
	char	*current = env_get(shell, "PWD");
	env_set(shell, "OLDPWD", current);	
	while (getcwd(buffer, buffer_size) == NULL) {
		buffer_size += 1024;
		free(buffer);
		buffer = malloc(buffer_size);
	}
	env_set(shell, "PWD", buffer);
	free(buffer);
}