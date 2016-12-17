#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hshell.h"
#include "hlib.h"

void prompt_print(shell_t *shell) {
	const char	*prompt = env_get(shell, "PS1");
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


int main(int argc, char **argv, char **envp) {
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	shell_getcwd(&shell);
	history_load(&shell);
	while (shell.exit == 0) {
		command_chain_t chain;
		
		if (shell.is_tty) {
			prompt_print(&shell);
		}
		command_init(&chain);	
		if (command_get(&shell, &chain, shell.fd) == ERR_GET_COMMAND_EOF) {
			shell.exit = 1;
		} else {
			if (chain.line) {
				if (shell.is_tty) {
					history_expand(&shell, &chain);
					history_add(&shell, chain.line);
				}
				command_remove_comment(&chain);
				command_expand(&shell, &chain);
				command_lexer(&chain);
				alias_expand(&shell, &chain);
				command_remove_quote(&chain);
				command_exec(&shell, &chain);
			}
		}
		command_chain_free(&chain);
	}
	history_save(&shell);
	shell_free(&shell);
	return shell.exit_code;
}