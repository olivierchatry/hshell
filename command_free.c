#include <stdlib.h>
#include "hshell.h"

void command_tree_free(struct command_s* command) {
	ARRAY_FREE_EACH(command->argv);
	ARRAY_EACH(command->children, command_tree_free);
	ARRAY_FREE_EACH(command->children);
}

void command_free(command_chain_t *chain) {
	ARRAY_FREE(chain->line);	
	ARRAY_EACH(chain->commands, command_tree_free);
	ARRAY_FREE_EACH(chain->commands);
}