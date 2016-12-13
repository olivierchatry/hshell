#include <stdlib.h>
#include "hshell.h"

void command_free(command_t* command) {
	ARRAY_FREE(command->argv);
	ARRAY_FREE(command->line);	
}