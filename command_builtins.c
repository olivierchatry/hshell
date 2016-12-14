#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#include "hshell.h"
#include "hlib.h"

struct builtin_s {
	char	*name;
	void	(*fct)(shell_t *shell, command_tree_t *command, int *status);
};


void builtin_alias(shell_t *shell, command_tree_t *cmd, int *status) {
	int index;
	if (cmd->argv_size <= 2 || hstrcmp(cmd->argv[1], "-p") == 0) {
		for (index = 0; index < shell->alias_keys_size; ++index) {
			hprintf(shell->alias_keys[index]);
			hprintf("=");
			hprintf(shell->alias_values[index]);
			hprintf("\n");
		}
	} else {
		for (index = 1; index < cmd->argv_size - 1; ++index) {
			alias_add(shell, cmd->argv[index]);
		}
	}
	UNUSED(status);
}

void builtin_env(shell_t *shell, command_tree_t *cmd, int *status) {
	char	**envp = shell->envp;
	char	*env_filter = cmd->argv_size > 2 ? cmd->argv[1] : NULL; 
	int		env_filter_len = env_filter ? hstrlen(env_filter) : 0;
	while (*envp) {
		if ( (env_filter_len == 0) || (hstrncmp(env_filter, *envp, env_filter_len) == 0)) {
			hprintf(*envp);
			hprintf("\n");
		}
		envp++;
	}
	UNUSED(status)
}

void builtin_setenv(shell_t *shell, command_tree_t *cmd, int *status) {
	if (cmd->argv_size > 3) {
		env_set(shell, cmd->argv[1], cmd->argv[2]);
	} else {
		hprintf("missing argument\n");
		*status = -1;
	}
}

void builtin_exit(shell_t* shell, command_tree_t* cmd, int *status) {
	shell->exit = 1;
	if (cmd->argv_size > 2) {
		shell->exit_code = hatoi(cmd->argv[1]);
	}
	UNUSED(status)
}

void builtin_unsetenv(shell_t *shell, command_tree_t *cmd, int *status) {
	if (cmd->argv_size > 2) {
		env_remove(shell, cmd->argv[1]);
	} else {
		hprintf("missing argument\n");
		*status = -1;
	}
}

void builtin_cd(shell_t *shell, command_tree_t *cmd, int *status) {
	const char* path = NULL;
	if (cmd->argv_size > 2) {
		path = cmd->argv[1]; 
		if (hstrcmp(path, "-") == 0) {
			path = env_get(shell, "OLDPWD");
		}
	} else {
		struct passwd *pws = getpwuid(geteuid());
		if (pws) {
			path = pws->pw_dir;
		}
	}
	*status = -1;
	if (path) {
		*status = chdir(path);
	}
	shell_getcwd(shell);
}

static struct builtin_s s_builtins[] = {
	{"exit", builtin_exit},
	{"env", builtin_env},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{"cd", builtin_cd},
	{"alias", builtin_alias},
	{NULL, NULL}
};

int command_builtins(shell_t *shell, command_tree_t *command, int *status) {
	int index = 0;
	*status = 0;
	while (s_builtins[index].name) {
		if (hstrcmp(s_builtins[index].name, command->argv[0]) == 0) {
			s_builtins[index].fct(shell, command, status);
			return 1;
		}
		index++;
	}
	return 0;
}