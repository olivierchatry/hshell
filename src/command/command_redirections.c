#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <hshell.h>
#include "utils/hlib.h"

/**
 * command_redirections - Handles I/O redirections before executing command
 * @shell: Shell structure
 * @cmd: Command structure
 * @status: Command status code to be set
 *
 * Return: 1 on success, 0 on failure
 */
int command_redirections(shell_t *shell, command_t *cmd, int *status)
{
	if (cmd->redirect_type != SHELL_OP_REDIRECT_OUT &&
		cmd->redirect_type != SHELL_OP_REDIRECT_OUT_CONCAT &&
		cmd->redirect_type != SHELL_OP_REDIRECT_IN &&
		cmd->redirect_type != SHELL_OP_REDIRECT_IN_UNTIL)
	{
		return (1);
	}

	if (cmd->redirect_type == SHELL_OP_REDIRECT_OUT ||
		cmd->redirect_type == SHELL_OP_REDIRECT_OUT_CONCAT)
	{
		int fd;
		int flags = O_WRONLY | O_CREAT;
		mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

		if (cmd->redirect_type == SHELL_OP_REDIRECT_OUT)
			flags |= O_TRUNC;
		else
			flags |= O_APPEND;
		fd = open(cmd->redirect, flags, mode);
		if (fd == -1)
		{
			perror("hsh: open");
			*status = 1;
			return (0);
		}
		shell->saved_stdout = dup(STDOUT_FILENO);
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			perror("hsh: dup2");
			*status = 1;
			return (0);
		}
		close(fd);
	}
	else if (cmd->redirect_type == SHELL_OP_REDIRECT_IN)
	{
		int fd;

		fd = open(cmd->redirect, O_RDONLY);
		if (fd == -1)
		{
			perror("hsh: open");
			*status = 1;
			return (0);
		}
		shell->saved_stdin = dup(STDIN_FILENO);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("hsh: dup2");
			*status = 1;
			return (0);
		}
		close(fd);
	}
	return (1);
}
