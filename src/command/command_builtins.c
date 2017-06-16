#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#include "hshell.h"
#include "utils/hlib.h"

struct builtin_s {
	char	*name;
	void	(*fct)(shell_t *shell, command_t *command, int *status);
};

struct err_handler_s {
	int code;
	const char *str;
};

void builtin_alias(shell_t *shell, command_t *cmd, int *status) {
	int index;
	if (cmd->argv_size <= 2 || hstrcmp(cmd->argv[1], "-p") == 0) {
		for (index = 0; index < shell->alias_keys_size; ++index) {
			hprintf("%s=%s\n", shell->alias_keys[index], shell->alias_commands[index]->line);
		}
	} else {
		for (index = 1; index < cmd->argv_size - 1; ++index) {
			alias_add(shell, cmd->argv[index]);
		}
	}
	UNUSED(status);
}

void builtin_env(shell_t *shell, command_t *cmd, int *status) {
	char	**envp = shell->envp;
	char	*env_filter = cmd->argv_size > 2 ? cmd->argv[1] : NULL; 
	int		env_filter_len = env_filter ? hstrlen(env_filter) : 0;
	while (*envp) {
		if ( (env_filter_len == 0) || (hstrncmp(env_filter, *envp, env_filter_len) == 0)) {
			hprintf("%s\n", *envp);
		}
		envp++;
	}
	UNUSED(status);
}

void builtin_setenv(shell_t *shell, command_t *cmd, int *status) {
	if (cmd->argv_size > 3) {
		env_set(shell, cmd->argv[1], cmd->argv[2]);
	} else {
		hprintf("missing argument\n");
		*status = -1;
	}
}

void builtin_exit(shell_t *shell, command_t *cmd, int *status) {
	shell->exit = 1;
	if (cmd->argv_size > 2) {
		if (hisnumber(cmd->argv[1], hstrlen(cmd->argv[1]))) {
			shell->exit_code = hatoi(cmd->argv[1]);
		} else {
			hprint_error("exit", "illegal number\n");
			shell->exit = 0;
			*status = 2;
		}
	}
	UNUSED(status);
}

void builtin_unsetenv(shell_t *shell, command_t *cmd, int *status) {
	if (cmd->argv_size > 2) {
		env_remove(shell, cmd->argv[1]);
	} else {
		hprintf("missing argument\n");
		*status = -1;
	}
}

void builtin_cd(shell_t *shell, command_t *cmd, int *status) {
	const char *path = NULL;
	const struct err_handler_s errors[] = {
		{EACCES, "Permission denied"},
		{ELOOP, "Too many symbolic links"},
		{ENOENT, "No such directory"},
		{ENOTDIR, "Not a directory"},
		{0, NULL}
	};

	if (cmd->argv_size > 2) {
		path = cmd->argv[1]; 
		if (hstrcmp(path, "-") == 0) {
			path = env_get(shell, "OLDPWD");
		}
	} else {
		path = env_get(shell, "HOME");
	}
	*status = -1;
	if (path) {
		*status = chdir(path) * -2;
		if (*status == 0) {
			char    *current = env_get(shell, "PWD");
			env_set(shell, "OLDPWD", current);
		} else {
			int i;
			char resolved = 0;
			for (i = 0; errors[i].str != NULL; i++) {
				if (errno == errors[i].code) {
					hprint_error("cd", "%s\n", errors[i].str);
					resolved = 1;
					break;
				}
			}
			if (!resolved) {
				/* chdir: Default failure message */
				hprint_error("cd", "cannot cd to %s\n", path);
			}
		}
	}
	shell_getcwd(shell);
}

void builtin_history(shell_t *shell, command_t *cmd, int *status) {
	int index 	= (shell->history_write_index + 1) % shell->history_size;
	int offset 	= 1;
	while (index != shell->history_write_index) {
		if (shell->history[index]) {
			hprintf("%4d:  %s\n", offset, shell->history[index]);
			offset++;
		}
		index = (index + 1) % shell->history_size;
	}
	UNUSED(status);
	UNUSED(cmd);
}

static struct builtin_s s_builtins[] = {
	{"exit", builtin_exit},
	{"env", builtin_env},
	{"setenv", builtin_setenv},
	{"unsetenv", builtin_unsetenv},
	{"cd", builtin_cd},
	{"alias", builtin_alias},
	{"history", builtin_history},

	{NULL, NULL}
};

int command_builtins(shell_t *shell, command_t *command, int *status) {
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
