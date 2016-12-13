#include <unistd.h>
#include <stdlib.h>
#include "hshell.h"

#define COMMAND_GET_READ_BUFFER_SIZE 4096

static void command_concat(command_t* command, char to_concat) {
	if (command->line_size >= command->line_buffer_size) {
		char* next;
		int 	i;
		next = malloc(command->line_buffer_size + COMMAND_GET_READ_BUFFER_SIZE);
		for (i = 0; i < command->line_buffer_size; ++i) {
			next[i] = command->line[i];
		}
		if (command->line_buffer_size) {
			free(command->line);
		}
		command->line = next;
		command->line_buffer_size += COMMAND_GET_READ_BUFFER_SIZE;
	}
	command->line[command->line_size++] = to_concat;
}

int command_get(command_t* command, int fd_from) {
	int		inhib_next = 0;
	int		error = 0;
	char	read_buffer = 1;

	while (error == 0 && read_buffer) {
		if (command->line_size > GET_COMMAND_MAXIMUM_CMD_SIZE) {
			error = ERR_GET_COMMAND_TO_BIG; 
		} else {
			if (read(fd_from, &read_buffer, 1) == -1) {
				error = ERR_GET_COMMAND_READ;
			}	else {
				if (read_buffer == '\\') {
					inhib_next = 1;
				} else {
					if (read_buffer == '\n') {
						if (!inhib_next) {
							read_buffer = 0;
						}
					} 
					inhib_next = 0;
				}
				command_concat(command, read_buffer);		
				if (command->line == NULL) {
					error = ERR_GET_COMMAND_MEMORY;
				}
			}
		}
	}
	return error;
}
