#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include "hshell.h"
#include "hlib.h"

static int			g_cancel_pipe_fd;

void signal_interrupt() {
	const char	*cancel_string = "";
	write(g_cancel_pipe_fd, cancel_string, 1);
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
	shell->command_reminder = NULL;
	shell->child_exit_code = 0;
	shell->paths_string = NULL;
	shell->exit = 0;
	shell->exit_code = 0;
	shell->is_tty = isatty(0); 
	shell->history = NULL;
	shell->history_size = 0;
	shell->history_write_index = 0;
	shell->history_count = 0;

	history_init(shell, 4096);
	
	/*while (index < argc) {
		if (hstrcmp(argv[index], "--test") == 0) {
			hprintf("test");
		}
		index++;
	}*/
	while (*envp) {
		env_add(shell, *envp++);
	}
	shell->state = SHELL_STATE_RUN;
	env_hook(shell, "");	
	g_cancel_pipe_fd = shell->cancel_pipe[1];
	signal(SIGINT, signal_interrupt);
	pipe(shell->cancel_pipe);
}
