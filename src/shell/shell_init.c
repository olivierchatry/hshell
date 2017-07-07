#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "hshell.h"
#include "utils/hlib.h"

/**
 * signal_interrupt - Catch SIGINT (CTRL+C)
 * @signal: Signal number
 */
void signal_interrupt(int signal)
{
	UNUSED(signal);
}

/**
 * shell_set_fd - Opens a file if passed as a parameter
 * @shell: Shell structure
 * @argc: Arguments counter
 * @argv: Arguments vector
 */
void shell_set_fd(shell_t *shell, int argc, char *argv[])
{
	shell->fd = 0;
	if (argc > 1)
	{
		shell->is_tty = 0;
		shell->fd = open(argv[1], O_RDONLY, 0);
		if (shell->fd <= 0)
		{
			shell->exit = true;
			shell->exit_code = -1;
		}
	}
}

/**
 * shell_init - Initializes shell data
 * @shell: Pointer to the shell structure to initialize
 * @argc: Arguments counter from 'main'
 * @argv: Arguments vector from 'main'
 * @envp: Environment from 'main'
 */
void shell_init(shell_t *shell, int argc, char *argv[], char *envp[])
{
	shell->state = SHELL_STATE_INIT;
	ARRAY_INIT(shell->env_keys);
	ARRAY_INIT(shell->env_values);
	ARRAY_INIT(shell->alias_keys);
	ARRAY_INIT(shell->alias_commands);
	ARRAY_INIT(shell->paths);
	ARRAY_INIT(shell->envp);
	ARRAY_INIT(shell->line);
	ARRAY_INIT(shell->prompt);
	ARRAY_ALLOCATE(shell->line, LINE_BUFFER_SIZE);
	shell->child_exit_code = 0;
	shell->paths_string = NULL;
	shell->exit = false;
	shell->exit_code = 0;
	shell->is_tty = isatty(0);
	shell->history = NULL;
	shell->history_size = 0;
	shell->history_write_index = 0;
	shell->history_count = 0;
	shell->saved_std[0] = -1;
	shell->saved_std[1] = -1;
	shell->saved_std[2] = -1;
	history_init(shell, 4096);
	shell_set_fd(shell, argc, argv);
	while (*envp)
	{
		env_add(shell, *envp++);
	}
	shell->state = SHELL_STATE_RUN;
	env_hook(shell, "");
	signal(SIGINT, signal_interrupt);
}
