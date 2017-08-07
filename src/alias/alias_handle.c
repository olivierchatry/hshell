#include <hshell.h>
#include "utils/hlib.h"

void alias_handle(shell_t *shell, char *str)
{
	const char *delim = "= ";
	char *value;
	char *key = NULL;
	int index;

	if (hstrchr(str, '='))
	{
		key = hstrtok_r(str, delim, &value);
		if (key)
		{
			if (!value)
			{
				value = "";
			}
			alias_set(shell, key, value);
		}
	}
	else
	{
		for (index = 0; index < shell->alias_keys_size; ++index)
		{
			if (hstrcmp(shell->alias_keys[index], str) == 0)
			{
				hprintf("%s='%s'\n", shell->alias_keys[index],
					shell->alias_commands[index]->line);
				break;
			}
		}
	}
}
