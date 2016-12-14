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

static struct builtin_s s_builtins[] = {
	{"env", builtin_env},
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