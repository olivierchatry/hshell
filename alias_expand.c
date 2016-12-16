#include "hshell.h"
#include "hlib.h"

static command_t **alias_expand_command(shell_t *shell, command_t *command, char *used) {
	int 				found = 1;
	int 				index;
	command_t		**ARRAY(expanded);
	
	ARRAY_INIT(expanded);
	ARRAY_ADD(expanded, command, COMMAND_BUFFER_SIZE);

	for (index = 0; index < shell->alias_keys_size; ++index) {
		used[index] = 0;
	}

	while (found) {
		found = 0;
		for (index = 0; index < shell->alias_keys_size; ++index) {
			if (used[index] == 0) {
				command_t	**ARRAY(processing);
				int 			cmd_index;
				
				ARRAY_INIT(processing);
				for (cmd_index = 0; cmd_index < expanded_size; ++cmd_index) {
					command_t *cmd = expanded[cmd_index]; 
					if (cmd->argv_size > 1) {
						if (hstrcmp(cmd->argv[0], shell->alias_keys[index]) == 0) {
							int cpy_index;
							used[index] = found = 1;
							command_chain_t *alias = shell->alias_commands[index];
							for (cpy_index = 0; cpy_index < alias->root.commands_size - 1; ++cpy_index) {
								ARRAY_ADD(processing, alias->root.commands[cpy_index], COMMAND_BUFFER_SIZE);						
							}
						} else {
							ARRAY_ADD(processing, cmd, COMMAND_BUFFER_SIZE);
						}
					} 
				}
				free(expanded);
				ARRAY_SET(expanded, processing);
			}
		}
	}
	ARRAY_ADD(expanded, NULL, COMMAND_BUFFER_SIZE);
	return expanded;
}

void alias_expand_r(shell_t *shell, command_t *parent, char *used) {
	int				index;
	command_t	**ARRAY(new);

	ARRAY_INIT(new);
	for (index = 0; index < parent->commands_size - 1; ++index) {
		int					cpy_index;
		command_t		*command = parent->commands[index];
		command_t		**expanded;
		command_t 	*replace = NULL;

		// alias_expand_r(shell, command, used);
		expanded = alias_expand_command(shell, command, used);
		for (cpy_index = 0; expanded[cpy_index]; ++cpy_index) {
			replace = expanded[cpy_index] == command ? command : command_clone(expanded[cpy_index]);
			ARRAY_ADD(new, replace, COMMAND_BUFFER_SIZE);
		}
		free(expanded);
		if (replace && replace != command) {
			int argv;
			replace->op = command->op;
			ARRAY_POP(replace->argv); // pop NULL
			for (argv = 1; argv < command->argv_size - 1; ++argv) {
				ARRAY_ADD(replace->argv, hstrdup(command->argv[argv]), ARGV_BUFFER_SIZE);
			}			
			ARRAY_ADD(replace->argv, NULL, ARGV_BUFFER_SIZE);
			command_free(command);
			free(command);
		}
	}
	free(parent->commands);
	ARRAY_ADD(new, NULL, COMMAND_BUFFER_SIZE);
	ARRAY_SET(parent->commands, new);
}

void alias_expand(shell_t *shell, command_chain_t *chain) {
	if (shell->alias_keys_size) {
		char	*used = malloc(shell->alias_keys_size);
		alias_expand_r(shell, &(chain->root), used);
		free(used);
	}
} 