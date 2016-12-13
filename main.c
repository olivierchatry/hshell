#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "hshell.h"
#include "hlib.h"

void prompt_print(shell_t *shell) {
	const char* prompt = env_get(shell, "PS1");
	if (!prompt) {
		prompt = getuid() == 0 ? "#" : ">";
	}
	hprintf(prompt);
}


int main(int argc, char** argv, char** envp) {
	shell_t shell;

	shell_init(&shell, argc, argv, envp);
	while (shell.exit == 0) {
		command_t command;

		prompt_print(&shell);
		command_init(&command);	
		command_get(&command, 0);
		command_split(&command);
		command_exec(&shell, &command);

		command_free(&command);
	}
	shell_free(&shell);
	return shell.exit_code;
}