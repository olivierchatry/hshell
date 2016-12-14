#include <unistd.h>

#include "hshell.h"
#include "hlib.h"

void shell_getcwd(shell_t* shell) {
	int 	buffer_size = 1024;
	char* buffer = malloc(buffer_size);

	if (shell->previous_pwd) {
		free(shell->previous_pwd);
	}

	shell->previous_pwd = env_get(shell, "PWD");
	
	while (getcwd(buffer, buffer_size) == NULL) {
		buffer_size += 1024;
		if (buffer) {
			free(buffer);
		}
		buffer = malloc(buffer_size);
	}
	env_set(shell, "PWD", buffer);
	free(buffer);
}