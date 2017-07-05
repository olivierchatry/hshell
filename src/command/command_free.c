#include <stdlib.h>
#include <hshell.h>

void command_free(command_t *command) {
	if (command->redirect)
		free(command->redirect);
	ARRAY_FREE_EACH(command->argv);
	ARRAY_EACH(command->commands, command_free);
	ARRAY_FREE_EACH(command->commands);
}

void command_chain_free(command_chain_t *chain) {
	ARRAY_FREE(chain->line);
	command_free(&chain->root);
}
