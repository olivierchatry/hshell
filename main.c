#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hshell.h"
#include "hlib.h"

void prompt_print(shell_t *shell) {
	const char* prompt = env_get(shell, "PS1");
	if (!prompt) {
		prompt = getuid() == 0 ? "# " : "$ ";
	} 
	hprintf(prompt);
	/*else {
		char	*ARRAY(expanded);
		int		len = hstrlen(prompt);
		ARRAY_INIT(expanded);
		while (*prompt) {
			char add = *prompt; 
			if (add == '\\') {
				switch (*(prompt + 1)) {
					case 'u':
						break;
					case 'h':
						break;
					case 'W':
						break;
					case '$':
						break;
				}
			}
		}
		ARRAY_FREE(expanded);
	}*/
}


int main(int argc, char** argv, char** envp) {
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	shell_getcwd(&shell);
	
	while (shell.exit == 0) {
		command_t command;

		prompt_print(&shell);
		command_init(&command);	
		command_get(&shell, &command, 0);
		if (command.line) {
			command_expand(&shell, &command);
			command_lexer(&command);
			command_exec(&shell, &command);
		}
		command_free(&command);
	}
	shell_free(&shell);
	return shell.exit_code;
}