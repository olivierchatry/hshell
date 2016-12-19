#include "stdlib.h"
#include <hshell.h>
#include "utils/hlib.h"

void	prompt_print(shell_t *shell) {
	const char* prompt;
	if (shell->is_tty) {
		if (shell->line_size) {
			prompt = ">";
		} else {
			prompt = env_get(shell, "PS1");
			if (!prompt) {
				prompt = "+[\\u@\\h \\W]\\$ ";
			} 
		}
		prompt_expand(shell, prompt);
		hprintf(shell->prompt);
		fflush(stdout);
	}
}
