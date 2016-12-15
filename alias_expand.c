#include "hshell.h"
#include "hlib.h"

void alias_expand_r(shell_t* shell, command_t* parent) {
	int				index;
	
	for (index = 0; index < parent->commands_size - 1;) {
		command_t				*command = parent->commands[index];
		command_chain_t	*alias = NULL;
		if (command->argv_size > 1) {
			alias = alias_get(shell, command->argv[0]);
		}
		if (alias && command->alias != alias) {
			command_t	**ARRAY(new);
			int					copy;

			ARRAY_INIT(new);
			for (copy = 0; copy < index; ++copy) {
				ARRAY_ADD(new, parent->commands[copy], COMMAND_BUFFER_SIZE);
			}
			
			if (alias->root.commands_size > 1) {
				command_t *replace;
				int 			argv;
				for (copy = 0; copy < alias->root.commands_size - 1; ++copy) {
					replace = command_clone(alias->root.commands[copy]);
					ARRAY_ADD(new, replace, COMMAND_BUFFER_SIZE);
				}
				// last item on the alias get the replaced command op.
				replace->op = command->op;
				ARRAY_POP(replace->argv); // pop NULL
				for (argv = 1; argv < command->argv_size - 1; ++argv) {
					ARRAY_ADD(replace->argv, hstrdup(command->argv[argv]), ARGV_BUFFER_SIZE);
				}
				ARRAY_ADD(replace->argv, NULL, ARGV_BUFFER_SIZE);
			}
			for (copy = index + 1; copy < parent->commands_size; ++copy) {
				ARRAY_ADD(new, parent->commands[copy], COMMAND_BUFFER_SIZE);
			}
			// free the command we have replaced
			command_free(command);
			free(command);
			// swap the command chain with the new one we just created
			free(parent->commands);

			parent->commands = new;
			parent->commands_buffer_size = new_buffer_size;
			parent->commands_size = new_size;
		} else {
			++index;
		}
	}	
}

void alias_expand(shell_t* shell, command_chain_t* chain) {
	alias_expand_r(shell, &(chain->root));
} 