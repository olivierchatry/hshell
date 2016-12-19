#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hshell.h"
#include "hlib.h"


static void	prompt_current_folder(shell_t *shell) {
	const char *pwd = env_get(shell, "PWD");
	if (hstrcmp(pwd, util_get_home()) == 0) {
		ARRAY_ADD(shell->prompt, '~', PROMPT_BUFFER_SIZE);
	} else {
		int len = hstrlen(pwd) - 1;
		int prev = len;
		while (len && pwd[len] != '/') {
			len--;
		}
		if ( prev == len ) {
			len --;
		}
		ARRAY_CAT(shell->prompt, (pwd + len + 1), prev - len, PROMPT_BUFFER_SIZE);
 	}
}

void	prompt_expand(shell_t* shell, const char* prompt) {
	int	state = 0;
	ARRAY_RESET(shell->prompt);

	while (*prompt) {
		char at = *prompt;
		if (state) {
			const char* add = NULL;
			switch (at) {
			case 'u':
				add = util_get_user();
				break;
			case 'h':
				add = util_get_hostname();
				break;
			case 'W':
				prompt_current_folder(shell);
				break;
			case '$':
				add = getuid() == 0 ? "#" : "$";
				break;
			default:
				ARRAY_ADD(shell->prompt, '\\', PROMPT_BUFFER_SIZE);
				ARRAY_ADD(shell->prompt, at, PROMPT_BUFFER_SIZE);
			}
			if (add) {
				ARRAY_CAT(shell->prompt, add, hstrlen(add), PROMPT_BUFFER_SIZE);
			}
			state = 0;
		} else {
			state = *prompt == '\\';
			if (!state) {
				ARRAY_ADD(shell->prompt, at, PROMPT_BUFFER_SIZE);
			}
		}
		prompt++;
	}
	ARRAY_ADD(shell->prompt, 0, PROMPT_BUFFER_SIZE);
}
