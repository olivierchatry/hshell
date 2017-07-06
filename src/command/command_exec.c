#include <hshell.h>
#include "utils/hlib.h"
#include <unistd.h>
#include <sys/wait.h>

static void command_exec_child(shell_t *shell, command_chain_t *chain, command_t *cmd) {
	char *exec_path = path_expand(shell, cmd->argv[0]);

	if (exec_path)
	{
		execve(exec_path, cmd->argv, shell->envp);
	}
	hprintf("%s: command not found.\n", exec_path ? exec_path : cmd->argv[0]);
	free(exec_path);
	shell_free(shell);
	command_chain_free(chain);
	_exit(127);
}

void restore_std_fd(shell_t *shell)
{
	if (shell->saved_std[0] != -1)
	{
		/* Restore stdin */
		dup2(shell->saved_std[0], STDIN_FILENO);
		close(shell->saved_std[0]);
		shell->saved_std[0] = -1;
	}
	if (shell->saved_std[1] != -1)
	{
		/* Restore stdout */
		dup2(shell->saved_std[1], STDOUT_FILENO);
		close(shell->saved_std[1]);
		shell->saved_std[1] = -1;
	}
	if (shell->saved_std[2] != -1)
	{
		/* Restore stderr */
		dup2(shell->saved_std[2], STDERR_FILENO);
		close(shell->saved_std[2]);
		shell->saved_std[2] = -1;
	}
	unlink(HEREDOC_BUFFER_FILE);
}

void command_exec(shell_t *shell, command_chain_t *chain)
{
	int	status = 0;
	int	index = 0;

	while (chain->root.commands[index])
	{
		command_t *cmd;

		alias_expand(shell, chain);
		cmd = chain->root.commands[index];
		if (cmd && (cmd->argv_size > 1))
		{
			if (!command_redirections(shell, cmd, &status))
			{
				shell->child_exit_code = status;
			}
			else if (!command_builtins(shell, cmd, &status))
			{
				int pid = fork();

				if (pid)
				{
					waitpid(pid, &status, 0);
					shell->child_exit_code = WEXITSTATUS(status);
				}
				else
				{
					command_exec_child(shell, chain, cmd);
				}
			}
			else
			{
				shell->child_exit_code = status;
			}
			shell->exit_code = shell->child_exit_code;
			restore_std_fd(shell);

			if (((status == 0) && (cmd->op == SHELL_OP_OR)) || ((status != 0) && (cmd->op == SHELL_OP_AND)))
			{
				while (chain->root.commands[index] && chain->root.commands[index]->op == cmd->op)
				{
					index++;
				}
			}
		}
		index += cmd != NULL;
	}
}
