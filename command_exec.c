#include "hshell.h"
#include "hlib.h"
#include <unistd.h>
#include <sys/wait.h>

void command_exec(shell_t* shell, command_t *command) {
	if (command->argv[0]) {
		if (!command_builtins(shell, command)) {
			int pid = fork();
			if (pid) {
				int status;
				waitpid(pid, &status, 0);
			} else {
				char* exec_path = paths_expand(shell, command->argv[0]);
				execve(exec_path, command->argv, shell->envp);
				_exit(EXIT_FAILURE);	
			}
		}
	}
}
