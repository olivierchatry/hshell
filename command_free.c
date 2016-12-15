#include <stdlib.h>
#include "hshell.h"

void command_tree_free(struct command_tree_s* command) {
	ARRAY_FREE_EACH(command->argv);
	ARRAY_EACH(command->child, command_tree_free);
	ARRAY_FREE_EACH(command->child);
}

void command_free(command_t *command) {
	ARRAY_FREE(command->line);	
	ARRAY_EACH(command->tree, command_tree_free);
	ARRAY_FREE_EACH(command->tree);
}