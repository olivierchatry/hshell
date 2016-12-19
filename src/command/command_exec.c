#include <hshell.h>
#include "utils/hlib.h"
#include <unistd.h>
#include <sys/wait.h>

static void command_exec_child(shell_t *shell, command_chain_t *chain, command_t *cmd) {
	char *exec_path = path_expand(shell, cmd->argv[0]);
	if (exec_path) {
		execve(exec_path, cmd->argv, shell->envp);
	}
	hprintf("%s: command not found.\n", exec_path ? exec_path : cmd->argv[0]);
	free(exec_path);
	shell_free(shell);
	command_chain_free(chain);
	_exit(EXIT_FAILURE);	
}

void command_exec(shell_t *shell, command_chain_t *chain) {
	int 			status = 0;
	command_t **commands = chain->root.commands;
	
	while (*commands) {
		command_t *cmd = *commands;
		if (cmd->argv_size > 1) {
			if (!command_builtins(shell, cmd, &status)) {
				int pid = fork();
				if (pid) {
					waitpid(pid, &status, 0);
					shell->child_exit_code = status;
				} else {
					command_exec_child(shell, chain, cmd);
				}
			}
			
			if ( ((status == 0) && (cmd->op == SHELL_OP_OR)) || ((status != 0) && (cmd->op == SHELL_OP_AND))) {
				while (*commands && (*commands)->op == cmd->op) {
					commands++;
				}
			}
		}
		commands++;			
	}
}
