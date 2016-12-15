#include "hshell.h"

void command_expand(shell_t *shell, command_chain_t* chain) {
	char* expanded;
	
	expanded = env_expand(shell, chain->line);
	if (expanded) {
		free(chain->line);
		chain->line = expanded;
	}
}