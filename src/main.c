#include "hshell.h"
#include "hlib.h"

int main(int argc, char **argv, char **envp) {
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	shell_getcwd(&shell);
	history_load(&shell);
	while (shell.exit == 0) {
		command_chain_t chain;
		
		prompt_print(&shell);
		command_init(&chain);	
		if (command_get(shell.fd, &shell) == ERR_GET_COMMAND_EOF) {
			shell.exit = shell.line_size == 0;
			ARRAY_RESET(shell.line);
		} else {
			if (shell.line) {
				chain.line = hstrdup(shell.line);
				if (shell.is_tty) {
					history_expand(&shell, &chain);
					history_add(&shell, chain.line);
				}
				command_remove_comment(&chain);
				command_expand(&shell, &chain);
				if (command_lexer(&chain) == 1) {
					alias_expand(&shell, &chain);
					command_remove_quote(&chain);
					command_exec(&shell, &chain);
					ARRAY_RESET(shell.line);
				}
			}
		}
		command_chain_free(&chain);
	}
	history_save(&shell);
	shell_free(&shell);
	return shell.exit_code;
}