#include "hshell.h"

void command_expand(shell_t *shell, command_t* command) {
	char* expanded;
	
	expanded = env_expand(shell, command->line);
	if (expanded) {
		free(command->line);
		command->line = expanded;
	}
}