#include "hshell.h"
#include "hlib.h"
#include <unistd.h>
#include <sys/wait.h>

void command_exec(shell_t* shell, command_t *command) {
	if (command->argv[0]) {
		if (hstrcmp(command->argv[0], "exit") == 0) {
			shell->exit = 1;
			if (command->argv_size > 2) {
				shell->exit_code = atoi(command->argv[1]);
			}
		} else {
			int pid = fork();
			if (pid) {
				int status;
				waitpid(pid, &status, 0);
			} else {
				if (!command_builtins(shell, command)) {
					char* exec_path = paths_expand(shell, command->argv[0]);
					execve(exec_path, command->argv, shell->envp);
					_exit(EXIT_FAILURE);	
				} else {
					_exit(0);				
				}
			}
		}
	}
}
