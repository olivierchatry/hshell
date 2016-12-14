#include "hshell.h"
#include "hlib.h"

struct builtin_s {
	char	*name;
	void	(*fct)(shell_t *shell, command_t *command);
};


void builtin_env(shell_t *shell, command_t *cmd) {
	char** envp = shell->envp;
	while (*envp) {
		hprintf(*envp);
		hprintf("\n");
		envp++;
	}
	UNUSED(cmd);
}

void builtin_setenv(shell_t *shell, command_t *cmd) {
	if (cmd->argv_size > 3) {
		env_set(shell, cmd->argv[1], cmd->argv[2]);
	} else {
		hprintf("missing argument\n");
	}
}

void builtin_exit(shell_t* shell, command_t* cmd) {
	shell->exit = 1;
	if (cmd->argv_size > 2) {
		shell->exit_code = atoi(cmd->argv[1]);
	}
}

void builtin_unsetenv(shell_t *shell, command_t *cmd) {
	if (cmd->argv_size > 2) {
		env_remove(shell, cmd->argv[1]);
	} else {
		hprintf("missing argument\n");
	}
}

static struct builtin_s s_builtins[] = {
	{"exit", builtin_exit},
	{"env", builtin_env},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{NULL, NULL}
};


int command_builtins(shell_t *shell, command_t *command) {
	int index = 0;
	while (s_builtins[index].name) {
		if (hstrcmp(s_builtins[index].name, command->argv[0]) == 0) {
			s_builtins[index].fct(shell, command);
			return 1;
		}
		index++;
	}
	return 0;
}