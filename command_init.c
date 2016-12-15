#include <stdlib.h>
#include "hshell.h"

void command_init(command_chain_t *chain) {
	ARRAY_INIT(chain->line);
	ARRAY_INIT(chain->commands);
}