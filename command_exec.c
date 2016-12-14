#include "hshell.h"
#include "hlib.h"
#include <unistd.h>
#include <sys/wait.h>

void command_exec(shell_t* shell, command_t *command) {
	int status = 0;
	command_tree_t **tree = command->tree;
	
	while (*tree) {
		command_tree_t *cmd = *tree;
		if (!command_builtins(shell, cmd, &status)) {
			int pid = fork();
			if (pid) {
				waitpid(pid, &status, 0);
			} else {
				char* exec_path = paths_expand(shell, cmd->argv[0]);
				execve(exec_path, cmd->argv, shell->envp);
				hprintf("command not found.\n");
				free(exec_path);
				shell_free(shell);
				command_free(command);
				_exit(EXIT_FAILURE);	
			}
		}
		
		if (cmd->op == SHELL_OP_OR) {
			if (status == 0) {
				while (*tree && (*tree)->op == SHELL_OP_OR) {
					tree++;
				}
			}
		} else if (cmd->op == SHELL_OP_AND) {
			if (status != 0) {
				while (*tree && (*tree)->op == SHELL_OP_AND) {
					tree++;
				}
			}
		}
		tree++;
	}
}
