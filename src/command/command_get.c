#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>
#include <hshell.h>
#include "utils/hlib.h"
#include <errno.h>

// this is for handling signal interrupt. Select exit when it receive a signal.
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

int command_get(int fd, shell_t *shell) {
	char	read_buffer[LINE_BUFFER_SIZE];
	char 	more = 1;

	while (more) {
		if ( (more = command_wait(fd)) ) {
			int count = read(fd, read_buffer, LINE_BUFFER_SIZE);
			if (count <= 0) {
				return count;
			}
			ARRAY_CAT(shell->line, read_buffer, count, LINE_BUFFER_SIZE);
			if (shell->line == NULL) {
				return ERR_GET_COMMAND_MEMORY;
			}
			more = ARRAY_LAST(shell->line) != '\n';
		} else {
			hprintf("\n");
			ARRAY_RESET(shell->line);
		}
	}
	// this will make sure we have a 0 at the end of the buffer, while
	// it will not be taken into account (POP).
	ARRAY_ADD(shell->line, 0, LINE_BUFFER_SIZE);
	ARRAY_POP(shell->line);
	return OK;
}
