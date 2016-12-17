#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

#include "hshell.h"
#include "hlib.h"

void signal_interrupt() {
}

void shell_init(shell_t *shell, int argc, char **argv, char **envp) {
	UNUSED(argc);
	UNUSED(argv);
	
	shell->state = SHELL_STATE_INIT;
	ARRAY_INIT(shell->env_keys);
	ARRAY_INIT(shell->env_values);
	ARRAY_INIT(shell->alias_keys);
	ARRAY_INIT(shell->alias_commands);
	ARRAY_INIT(shell->paths);
	ARRAY_INIT(shell->envp);
	ARRAY_INIT(shell->line);
	ARRAY_ALLOCATE(shell->line, LINE_BUFFER_SIZE);

	shell->child_exit_code = 0;
	shell->paths_string = NULL;
	shell->exit = 0;
	shell->exit_code = 0;
	shell->is_tty = isatty(0); 
	shell->history = NULL;
	shell->history_size = 0;
	shell->history_write_index = 0;
	shell->history_count = 0;
	shell->fd = 0;
	history_init(shell, 4096);
	
	if (argc > 1) {
		shell->is_tty = 0;
		shell->fd = open(argv[1], O_RDONLY, 0); 
		if (shell->fd <= 0) {
			shell->exit = 1;
			shell->exit_code = -1;
		}
	}

	while (*envp) {
		env_add(shell, *envp++);
	}
	shell->state = SHELL_STATE_RUN;
	env_hook(shell, "");	
	signal(SIGINT, signal_interrupt);
}
