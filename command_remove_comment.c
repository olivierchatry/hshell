#include "hshell.h"

void command_remove_comment(command_chain_t *chain) {
	char* line = chain->line;
	while (*line) {
		if (*line == '#') {
			*line = 0;
		} else {
			line++;
		}
	}
}