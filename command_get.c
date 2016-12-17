#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"
#include "hlib.h"
#include <errno.h>

static int command_wait(shell_t *shell, int fd) {
	fd_set 	rds;
	int			nfds = fd;
	
	FD_ZERO(&rds);
	FD_SET(fd, &rds);
	FD_SET(shell->cancel_pipe[0], &rds);
	if (shell->cancel_pipe[0] > nfds) {
		nfds = shell->cancel_pipe[0];
	}
	if (select(nfds + 1, &rds, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(fd, &rds)) {
			return 1;
		}
	}

	return 0;
}

int command_copy_reminder(shell_t *shell, char *read_buffer) {
	int have = shell->command_reminder != NULL;
	int index = 0;
	if (!have) {
		return 0;
	}
	while (shell->command_reminder[index]) {
		read_buffer[index] = shell->command_reminder[index];
		index++;
	}
	free(shell->command_reminder);
	shell->command_reminder = NULL;
	read_buffer[index] = 0;
	return index + 1;
}

int command_get(shell_t *shell, command_chain_t *chain, int fd_from) {
	int		inhib_next = 0;
	int		count;
	char	read_buffer[LINE_BUFFER_SIZE];
	char	*temp_read_buffer;
	char 	ate = 1;

	count = command_copy_reminder(shell, read_buffer);
	while (ate) {
		if (chain->line_size > COMMAND_GET_MAXIMUM_CMD_SIZE) {
			return ERR_GET_COMMAND_TO_BIG;
		}
		if (count || command_wait(shell, fd_from)) {
			int good = (count = count) || (count = read(fd_from, read_buffer, LINE_BUFFER_SIZE));
			if (count == -1) {
				return ERR_GET_COMMAND_READ;
			}
			if (good == 0 && chain->line_size == 0) {
				return ERR_GET_COMMAND_EOF;
			}
			temp_read_buffer = read_buffer;

			while (count-- && ate) {
				ate = *temp_read_buffer++;
				if (!ate || ((ate == '\n') && !inhib_next)) {
					shell->command_reminder = hstrndup(temp_read_buffer, count);
					ate = 0;
				}
				inhib_next = ate == '\\';
				ARRAY_ADD(chain->line, ate, LINE_BUFFER_SIZE);
				if (chain->line == NULL) {
					return ERR_GET_COMMAND_MEMORY;
				}
			}
		} else {
			hprintf("\n");
			ate = 0;
			ARRAY_FREE(chain->line);
		}
	}
	return OK;
}
