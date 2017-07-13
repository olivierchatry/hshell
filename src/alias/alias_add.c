#include <hshell.h>
#include "utils/hlib.h"

void alias_add(shell_t *shell, char *env)
{
	const char *delim = "= ";
	char *value;
	char *key = hstrtok_r(env, delim, &value);

	if (key)
	{
		if (!value)
		{
			value = "";
		}
		alias_set(shell, key, value);
	}
}
