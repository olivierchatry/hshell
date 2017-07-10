#include "hshell.h"
#include "utils/hlib.h"

#include <stdio.h>
/**
 * print_chain - DEBUG: Prints out the chain of commands in details
 * @chain: Pointer to the chain to be printed
 */
void print_chain(command_chain_t *chain)
{
	int i, j;

	printf("Command chain:\n");
	for (i = 0; chain->root.commands && chain->root.commands[i]; i++)
	{
		command_t *cmd;

		printf("[%02d]:", i);
		cmd = chain->root.commands[i];
		for (j = 0; cmd->argv && cmd->argv[j]; j++)
		{
			if (j)
				printf(",");
			printf("%s", cmd->argv[j]);
		}
		printf("\n");
		printf("OP: %d\n", cmd->op);
		printf("IO: %d\n", cmd->redirect_type);
	}
	printf("\n");
}

/**
 * command_run - Runs a command line
 * @shell: Shell structure
 * @line: Command line to be run
 *
 * Return: 1 on success, 0 on failure
 */
int command_run(shell_t *shell, const char *line)
{
	command_chain_t chain;

	command_init(&chain);
	chain.line = hstrdup(line);
	if (shell->is_tty)
	{
		history_expand(shell, &chain);
		history_add(shell, chain.line);
	}
	command_remove_comment(&chain);
	command_expand(shell, &chain);
	if (command_lexer(&chain) == 1)
	{
		command_remove_quote(&chain);
		/* DEGUB */
		/*print_chain(&chain);*/
		command_exec(shell, &chain);
		command_chain_free(&chain);
		return (1);
	}
	command_chain_free(&chain);
	return (0);
}
