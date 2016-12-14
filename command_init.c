#include <stdlib.h>
#include "hshell.h"

void command_init(command_t *command) {
	ARRAY_INIT(command->line);
	ARRAY_INIT(command->tree);
}