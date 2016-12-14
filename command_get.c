#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"
#include "hlib.h"
#include <errno.h>

static int command_wait(shell_t *shell, int fd) {
	fd_set rds;
	
	FD_ZERO(&rds);
	FD_SET(fd, &rds);
	FD_SET(shell->cancel_pipe[0], &rds);
	
	if (select(shell->cancel_pipe[0] + 1, &rds, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(fd, &rds)) {
			return 1;
		}
	}

	return 0;
}

int command_copy_reminder(shell_t *shell, char *read_buffer) {
	int have = shell->command_reminder != NULL;
	int index = 0;
	if (have) {
		while (shell->command_reminder[index]) {
			read_buffer[index] = shell->command_reminder[index];
			index++;
		}
		free(shell->command_reminder);
		shell->command_reminder = NULL;
	}
	read_buffer[index] = 0;
	return have;
}

int command_get(shell_t *shell, command_t *command, int fd_from) {
	int		inhib_next = 0;
	int		count;
	char	read_buffer[LINE_BUFFER_SIZE];
	char	*temp_read_buffer;
	char 	ate = 1;

	command_copy_reminder(shell, read_buffer);	
	count = hstrlen(read_buffer);
	while (ate) {
		if (command->line_size > COMMAND_GET_MAXIMUM_CMD_SIZE) {
			return ERR_GET_COMMAND_TO_BIG; 
		} 
		if (count || command_wait(shell, fd_from)) {
			if (!count) {
				count = read(fd_from, read_buffer, LINE_BUFFER_SIZE);
			}
			if (count == -1) {
				return ERR_GET_COMMAND_READ;
			}
			if (count == 0) {
				return ERR_GET_COMMAND_EOF;
			}
			temp_read_buffer = read_buffer;

			while (count-- && ate) {
				ate = *temp_read_buffer++; 
				if (ate == '\n' && !inhib_next) {
					if (count > 1) {
						shell->command_reminder = hstrndup(temp_read_buffer, count - 1);
					}
					ate = 0;
				}				

				inhib_next = ate == '\\';
				ARRAY_ADD(command->line, ate, LINE_BUFFER_SIZE);
				if (command->line == NULL) {
					return ERR_GET_COMMAND_MEMORY;
				}
			}
		} else {
			hprintf("\n");
			ate = 0;
			ARRAY_FREE(command->line);
		}
	}
	return OK;
}
