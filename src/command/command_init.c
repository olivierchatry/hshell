#include <stdlib.h>
#include <hshell.h>

/**
 * command_init - Initializes command structure
 * @chain: Pointer to the first element of the command chain to initialize
 */
void command_init(command_chain_t *chain)
{
	ARRAY_INIT(chain->line);
	ARRAY_INIT(chain->root.argv);
	ARRAY_INIT(chain->root.commands);
	chain->root.op = SHELL_OP_ROOT;
	chain->root.redirect = NULL;
	chain->root.redirect_type = 0;
}
