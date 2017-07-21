#include <stdlib.h>
#include <hshell.h>

/**
 * command_free - Frees a command structure
 * @command: Pointer to the structure to be freed
 */
void command_free(command_t *command)
{
	if (command->redirect)
		free(command->redirect);
	ARRAY_FREE_EACH(command->argv);
	ARRAY_EACH(command->commands, command_free);
	ARRAY_FREE_EACH(command->commands);
}

/**
 * command_chain_free - Frees a chain of command
 * @chain: Pointer to the command chain to be freed
 */
void command_chain_free(command_chain_t *chain)
{
	ARRAY_FREE(chain->line);
	command_free(&chain->root);
}
