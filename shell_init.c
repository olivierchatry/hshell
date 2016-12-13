#include "hshell.h"
#include "hlib.h"

void shell_init(shell_t* shell, int argc, char** argv, char **envp) {
	int index = 1;

	ARRAY_INIT(shell->env_keys);
	ARRAY_INIT(shell->env_values);
	shell->exit = 0;
	while (index < argc) {
		if (hstrcmp(argv[index], "--test") == 0) {
			hprintf("test");
		}
	}
	while (*envp) {
		env_add(shell, *envp++);
	}
}
