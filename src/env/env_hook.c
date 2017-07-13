#include <hshell.h>
#include "utils/hlib.h"

struct hook_s
{
	const char	*key;
	void		(*fct)(shell_t *);
};

static struct hook_s s_hooks[] = {
	{"PATH", path_parse},
	{NULL, env_rebuild_envp},
	{NULL, NULL}
};

void env_hook(shell_t *shell, const char *key)
{
	int index = 0;

	while (s_hooks[index].fct)
	{
		const char	*other_key = s_hooks[index].key;

		if (((other_key == NULL) && (shell->state == SHELL_STATE_RUN))
			|| (other_key && (hstrcmp(s_hooks[index].key, key) == 0)))
		{
			(*s_hooks[index].fct)(shell);
		}
		index++;
	}
}
