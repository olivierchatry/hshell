#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"

static int command_wait(shell_t *shell, int fd) {
	fd_set rds;
	
	FD_ZERO(&rds);
	FD_SET(fd, &rds);
	FD_SET(shell->cancel_pipe[0], &rds);
	
	if (select(2, &rds, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(fd, &rds)) {
			return 1;
		}
	}

	return 0;
}

int command_get(shell_t *shell, command_t *command, int fd_from) {
	int		inhib_next = 0;
	int		count;
	char	read_buffer[LINE_BUFFER_SIZE];
	char	*temp_read_buffer;
	char 	ate = 1;
	
	while (ate) {
		if (command->line_size > COMMAND_GET_MAXIMUM_CMD_SIZE) {
			return ERR_GET_COMMAND_TO_BIG; 
		} 

		if (command_wait(shell, fd_from)) {
			count = read(fd_from, read_buffer, LINE_BUFFER_SIZE);
			if (count == -1) {
				return ERR_GET_COMMAND_READ;
			}
			temp_read_buffer = read_buffer;
			while (count-- && ate) {
				ate = *temp_read_buffer++; 
				if (ate == '\\') {
					inhib_next = 1;
				} else {
					if (ate == '\n' && !inhib_next) {
						ate = 0;
					}				
					inhib_next = 0;
				}
				ARRAY_ADD(command->line, ate, LINE_BUFFER_SIZE);
				if (command->line == NULL) {
					return ERR_GET_COMMAND_MEMORY;
				}
			}
		} else {
			const char *newline = "\n";
			write(0, newline, 1);
			ate = 0;
			ARRAY_FREE(command->line);
		}
	}
	return OK;
}
