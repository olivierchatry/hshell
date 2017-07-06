#include <hshell.h>
#include "utils/hlib.h"

#include <stdio.h>
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
 * main - Program entry point
 * @argc: Arguments counter
 * @argv: Arguments vector
 * @envp: Environment
 *
 * Return: EXIT_SUCCESS or error code
 */
int main(int argc, char **argv, char **envp)
{
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	shell_getcwd(&shell);
	history_load(&shell);
	while (shell.exit == 0)
	{
		command_chain_t chain;

		prompt_print(&shell);
		command_init(&chain);
		if (command_get(shell.fd, &shell) == ERR_GET_COMMAND_EOF)
		{
			shell.exit = shell.line_size == 0;
			ARRAY_RESET(shell.line);
		}
		else
		{
			if (shell.line)
			{
				chain.line = hstrdup(shell.line);
				if (shell.is_tty)
				{
					history_expand(&shell, &chain);
					history_add(&shell, chain.line);
				}
				command_remove_comment(&chain);
				command_expand(&shell, &chain);
				if (command_lexer(&chain) == 1)
				{
					/* DEGUB PRINT */
					/*print_chain(&chain);*/
					command_remove_quote(&chain);
					command_exec(&shell, &chain);
					ARRAY_RESET(shell.line);
				}
			}
		}
		command_chain_free(&chain);
	}
	history_save(&shell);
	shell_free(&shell);
	return (shell.exit_code);
}
