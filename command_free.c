#include <stdlib.h>
#include "hshell.h"

void command_tree_free(struct command_tree_s* command) {
	ARRAY_EACH(command->argv, free)
	ARRAY_FREE(command->argv);
	ARRAY_EACH(command->child, command_tree_free);
	ARRAY_EACH(command->child, free);
	ARRAY_FREE(command->child);
}

void command_free(command_t *command) {
	ARRAY_FREE(command->line);	
	ARRAY_EACH(command->tree, command_tree_free);
	ARRAY_EACH(command->tree, free);
	ARRAY_FREE(command->tree);
}