#include <hshell.h>
#include "utils/hlib.h"

command_t *command_clone(command_t *command) {
	int				index;
	command_t *new = malloc(sizeof(command_t));
	ARRAY_INIT(new->argv);
	for (index = 0; index < command->argv_size; ++index) {
		char	*argv = command->argv[index];
		ARRAY_ADD(new->argv, argv ? hstrdup(argv) : NULL, command->argv_buffer_size);
	}
	ARRAY_INIT(new->commands);
	for (index = 0; index < command->commands_size; ++index) {
		command_t	*child = command->commands[index];
		child = child ? command_clone(child) : child;
		ARRAY_ADD(new->commands, child, new->commands_buffer_size);
	}
	new->op = command->op;
	new->alias = command->alias;
	new->redirect = command->redirect;
	new->redirect_type = command->redirect_type;
	return new;
}
