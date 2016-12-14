#include "hshell.h"

void command_remove_comment(command_t *command) {
	char* line = command->line;
	while (*line) {
		if (*line == '#') {
			*line = 0;
		} else {
			line++;
		}
	}
}