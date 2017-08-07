#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <hshell.h>
#include "utils/hlib.h"

/**
 * handle_in_redirection - Handles stdin redirection
 * @shell: Shell structure
 * @status: Command exit status to be set
 * @file: Path to the file to redirect to stdin
 *
 * Return: 1 on succes, 0 on failure
 */
int handle_in_redirection(shell_t *shell, int *status, char *file)
{
	int fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		if (errno == EACCES)
			hperror(shell, NULL, "cannot open %s: Permission denied\n", file);
		else if (errno == ENOENT)
			hperror(shell, NULL, "cannot open %s: No such file\n", file);
		else
			hperror(shell, NULL, "cannot open %s: Error\n", file);
		*status = 2;
		return (0);
	}
	shell->saved_std[0] = dup(STDIN_FILENO);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		perror("hsh: dup2");
		*status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

/**
 * handle_heredoc - Handle the Here Document (<<) I/O redirection
 * @shell: Shell structure
 * @cmd: Command structure
 * @status: Command status code to be set
 *
 * Return: 1 on success, 0 on failure
 */
int handle_heredoc(shell_t *shell, command_t *cmd, int *status)
{
	char *line = NULL;
	size_t size;
	ssize_t ret;
	int fd;
	char *ps2 = env_get(shell, "PS2");

	fd = open(HEREDOC_BUFFER_FILE, O_WRONLY | O_CREAT | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
	if (fd == -1)
	{
		perror("hsh: open");
		*status = 1;
		return (0);
	}
	while (42)
	{
		if (shell->is_tty)
		{
			if (ps2)
			{
				ps2 = prompt_expand(shell, ps2);
				write(1, ps2, hstrlen(ps2));
				free(ps2);
			}
			else
			{
				write(1, "> ", 2);
			}
		}
		ret = getline(&line, &size, stdin);
		if (ret <= 0)
			break;
		if (hstrcmp(line, cmd->redirect) == 0 ||
			(hstrncmp(line, cmd->redirect, hstrlen(cmd->redirect)) == 0 &&
			line[hstrlen(cmd->redirect)] == '\n'))
		{
			free(line);
			break;
		}
		write(fd, line, ret);
		free(line);
		line = NULL;
	}
	if (ret == -1 && line == NULL)
	{
		perror("hsh: getline");
		close(fd);
		*status = 1;
		return (0);
	}
	close(fd);
	return (handle_in_redirection(shell, status, HEREDOC_BUFFER_FILE));
}

/**
 * handle_out_redirections - Handles stdout redirections
 * @shell: Shell structure
 * @cmd: Command structure
 * @status: Command exit status to be set
 *
 * Return: 1 on succes, 0 on failure
 */
int handle_out_redirections(shell_t *shell, command_t *cmd, int *status)
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
	shell->saved_std[1] = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("hsh: dup2");
		*status = 1;
		return (0);
	}
	close(fd);
	return (1);
}

/**
 * handle_pipe - Handles pipe redirections
 * @shell: Shell structure
 * @cmd: Command structure
 * @status: Command exit status to be set
 *
 * Return: 1 on succes, 0 on failure
 */
int handle_pipe(shell_t *shell, command_t *cmd, int *status)
{
	UNUSED(cmd);
	if (pipe(shell->pipefd) == -1)
	{
		perror("hsh: pipe");
		*status = 1;
		return (0);
	}
	return (1);
}

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
	switch (cmd->redirect_type)
	{
	case SHELL_OP_REDIRECT_OUT:
	case SHELL_OP_REDIRECT_OUT_CONCAT:
		return (handle_out_redirections(shell, cmd, status));
	case SHELL_OP_REDIRECT_IN:
		return (handle_in_redirection(shell, status, cmd->redirect));
	case SHELL_OP_REDIRECT_IN_HEREDOC:
		return (handle_heredoc(shell, cmd, status));
	case SHELL_OP_REDIRECT_PIPE_OUT:
		return (handle_pipe(shell, cmd, status));
	default:
		return (1);
	}
}
