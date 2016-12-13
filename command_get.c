#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"


int command_get(command_t *command, int fd_from) {
	int		inhib_next = 0;
	int		count;
	char	read_buffer[LINE_BUFFER_SIZE];
	char	*temp_read_buffer;
	char 	ate = 1;
	
	while (ate) {
		if (command->line_size > COMMAND_GET_MAXIMUM_CMD_SIZE) {
			return ERR_GET_COMMAND_TO_BIG; 
		} 
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
	}
	return OK;
}
