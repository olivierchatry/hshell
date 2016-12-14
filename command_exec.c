#include "hshell.h"
#include "hlib.h"
#include <unistd.h>
#include <sys/wait.h>

static void command_exec_child(shell_t *shell, command_t *command, command_tree_t *cmd) {
	char* alias = alias_get(shell, cmd->argv[0]);
	char* exec_path = paths_expand(shell, alias ? alias : cmd->argv[0]);
	execve(exec_path, cmd->argv, shell->envp);
	hprintf("command not found.\n");
	free(exec_path);
	shell_free(shell);
	command_free(command);
	_exit(EXIT_FAILURE);	
}

void command_exec(shell_t *shell, command_t *command) {
	int status = 0;
	command_tree_t **tree = command->tree;
	
	while (*tree) {
		command_tree_t *cmd = *tree;
		if (cmd->argv_size > 1) {
			if (!command_builtins(shell, cmd, &status)) {
				int pid = fork();
				if (pid) {
					waitpid(pid, &status, 0);
					shell->child_exit_code = status;
				} else {
					command_exec_child(shell, command, cmd);
				}
			}
			
			if ( ((status == 0) && (cmd->op == SHELL_OP_OR)) || ((status != 0) && (cmd->op == SHELL_OP_AND))) {
				while (*tree && (*tree)->op == cmd->op) {
					tree++;
				}
			}
		}
		tree++;			
	}
}
