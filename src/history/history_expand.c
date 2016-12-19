#include <hshell.h>
#include "utils/hlib.h"

char* history_find(shell_t *shell, char *beg, char *end) {
	int		count 	= end - beg;
	int		offset 	= -1;
	char*	ret = NULL;
	if (hisnumber(beg, count)) {
		offset = (hatoin(beg, count) - 1);		
		if (offset >= 0 && offset < shell->history_count) {
			int start = shell->history_write_index;
			while (!shell->history[start]) {
				start = (start + 1) % shell->history_size;
			}
			offset = (start + offset) % shell->history_size;	
		}
	} else {
		int start	= (shell->history_write_index - 1 + shell->history_size) % shell->history_size;
		int stop	= shell->history_write_index;
		while ( (start != stop) && (offset < 0)) {				
			if (hstrncmp(shell->history[start], beg, count) == 0) {
				offset = start;
			}
			start = (start - 1 + shell->history_size) % shell->history_size;
		}
	}

	if (offset >= 0 && offset < shell->history_size) {
		ret = shell->history[offset];
	}
	return ret;
}

void history_expand(shell_t *shell, command_chain_t* chain) {
	char	*ARRAY(new);
	char	*line = chain->line;
	char	*start = NULL;
	int		count = hstrlen(line) + 1;

	ARRAY_INIT(new);
	while(count--) {
		char at = *line;
		if ((at == '!') && !start) {
			start = line + 1;
		} else {
			if ( (at == ' ' || at == '\t' || at == 0 || at == '\n') && start) {
				char* hist = history_find(shell, start, line);
				while (hist && *hist) {
					ARRAY_ADD(new, *hist++, LINE_BUFFER_SIZE);
				}				
				start = NULL;
			}
			if (!start) {
				ARRAY_ADD(new, at, LINE_BUFFER_SIZE);
			}
		}	
		line++;
	}
	ARRAY_FREE(chain->line);
	ARRAY_SET(chain->line, new);
}