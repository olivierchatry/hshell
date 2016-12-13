#include "hshell.h"
#include "hlib.h"

void shell_init(shell_t* shell, int argc, char** argv, char **envp) {
	int index = 1;

	ARRAY_INIT(shell->env_key);
	ARRAY_INIT(shell->env_value);
	shell->exit = 0;
	while (index < argc) {
		if (hstrcmp(argv[index], "--test") == 0) {
			hprintf("test");
		}
	}
	while (*envp) {
		shell_add_env(shell, *envp++);
	}
}
