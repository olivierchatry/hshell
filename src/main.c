#include "hshell.h"
#include "utils/hlib.h"

/**
 * main - Program entry point
 * @argc: Arguments counter
 * @argv: Arguments vector
 * @envp: Environment
 *
 * Return: EXIT_SUCCESS or error code
 */
int main(int argc, char *argv[], char *envp[])
{
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	shell_getcwd(&shell);
	history_load(&shell);
	shellrc_load(&shell);
	while (shell.exit == false)
	{
		prompt_print(&shell);
		if (command_get(shell.fd, &shell) == ERR_GET_COMMAND_EOF)
		{
			shell.exit = shell.line_size == 0;
			ARRAY_RESET(shell.line);
		}
		else if (shell.line)
		{
			command_run(&shell, shell.line);
			ARRAY_RESET(shell.line);
		}
	}
	history_save(&shell);
	shell_free(&shell);
	return (shell.exit_code);
}
