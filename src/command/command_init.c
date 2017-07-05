#include <stdlib.h>
#include <hshell.h>

void command_init(command_chain_t *chain) {
	ARRAY_INIT(chain->line);
	ARRAY_INIT(chain->root.argv);
	ARRAY_INIT(chain->root.commands);
	chain->root.op = SHELL_OP_ROOT;
	chain->root.redirect = NULL;
}
