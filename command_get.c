#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"
#include "hlib.h"
#include <errno.h>

static int command_wait(int fd) {
	fd_set 	rds;

	FD_ZERO(&rds);
	FD_SET(fd, &rds);
	if (select(fd + 1, &rds, NULL, NULL, NULL) > 0) {
		if (FD_ISSET(fd, &rds)) {
			return 1;
		} 
	}
	return 0;
}


int command_get(shell_t *shell, command_chain_t *chain, int fd_from) {
	char	read_buffer[LINE_BUFFER_SIZE];
	char	*temp_read_buffer;
	char 	ate = 1;

	while (ate) {
		if (chain->line_size > COMMAND_GET_MAXIMUM_CMD_SIZE) {
			return ERR_GET_COMMAND_TO_BIG;
		}
		if (command_wait(fd_from)) {
			int count = read(fd_from, read_buffer, LINE_BUFFER_SIZE);
			if (count == -1) {
				return ERR_GET_COMMAND_READ;
			}
			if (count == 0 && chain->line_size == 0) {
				return ERR_GET_COMMAND_EOF;
			}
			temp_read_buffer = read_buffer;
			while (count--) {
				ARRAY_ADD(shell->line, *temp_read_buffer++, LINE_BUFFER_SIZE);
				if (shell->line == NULL) {
					return ERR_GET_COMMAND_MEMORY;
				}
			}
			ate = ARRAY_LAST(shell->line) != '\n';
		} else {
			hprintf("\n");
			ate = 0;
			ARRAY_FREE(chain->line);
		}
	}
	ARRAY_ADD(shell->line, 0, LINE_BUFFER_SIZE);
	ARRAY_POP(shell->line);
	UNUSED(shell);
	return OK;
}
