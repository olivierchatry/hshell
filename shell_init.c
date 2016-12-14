#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "hshell.h"
#include "hlib.h"

static shell_t* global_shell;

void signal_interrupt() {
	const char* cancel_string = "";
	write(global_shell->cancel_pipe[1], cancel_string, 1);
}

void shell_init(shell_t* shell, int argc, char** argv, char **envp) {
	int index = 1;

	shell->state = SHELL_STATE_INIT;
	ARRAY_INIT(shell->env_keys);
	ARRAY_INIT(shell->env_values);
	ARRAY_INIT(shell->paths);
	ARRAY_INIT(shell->envp);
	shell->paths_string = NULL;
	shell->exit = 0;
	shell->exit_code = 0;
	while (index < argc) {
		if (hstrcmp(argv[index], "--test") == 0) {
			hprintf("test");
		}
		index++;
	}
	while (*envp) {
		env_add(shell, *envp++);
	}
	shell->state = SHELL_STATE_RUN;
	env_hook(shell, "");
	global_shell = shell;
	signal(SIGINT, signal_interrupt);
	pipe(shell->cancel_pipe);
}
