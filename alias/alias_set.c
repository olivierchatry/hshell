#include "hshell.h"
#include "hlib.h"

void command_set_alias_r(command_t *command, void *alias) {
	int index;
	command->alias = alias;
	for (index = 0; index < command->commands_size - 1; ++index) {
		command_set_alias_r(command->commands[index], alias);
	}
}

command_chain_t *alias_set(shell_t *shell, char *alias, char *value) {
	int 	index = alias_get_index(shell, alias);

	command_chain_t *chain = malloc(sizeof(command_chain_t));
	command_init(chain);
	chain->line = hstrdup(value);
	command_lexer(chain);
	if (index == -1) {
		ARRAY_ADD(shell->alias_keys, hstrdup(alias), ENV_BUFFER_SIZE);
		ARRAY_ADD(shell->alias_commands, chain, ENV_BUFFER_SIZE);
	} else {
		if (shell->alias_commands[index]) {
			command_chain_free(shell->alias_commands[index]);
			free(shell->alias_commands[index]);
		}
		shell->alias_commands[index] = chain;
	}
	command_set_alias_r(&(chain->root), chain);
	return chain;
}