#include "hshell.h"
#include "hlib.h"

void history_init(shell_t *shell, int size) {	
	char  **history = hcalloc(sizeof(char*) * size);
	int		history_size = size;
	int		history_write_index = 0;
	
	if (shell->history) {
		int end = shell->history_write_index;
		int start = (end + 1) % shell->history_size;
		
		while (start != end) {
			if (history[history_write_index]) {
				free(history[history_write_index]);
			}
			history[history_write_index] =  shell->history[start];
			start = (start + 1) % shell->history_size;
			history_write_index = (history_write_index + 1) & history_size;
		}
		free (shell->history);
	}
	
	shell->history = history;
	shell->history_size = history_size;
	shell->history_write_index = history_write_index;
}