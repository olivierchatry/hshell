
#include <hshell.h>
#include "utils/hlib.h"

int env_get_index_n(shell_t *shell, const char *key, int n)
{
	int index = 0;

	while (index < shell->env_keys_size)
	{
		if (hstrncmp(shell->env_keys[index], key, n) == 0)
		{
			return (index);
		}
		index++;
	}
	return (-1);
}

int env_get_index(shell_t *shell, const char *key)
{
	int index = 0;

	while (index < shell->env_keys_size)
	{
		if (hstrcmp(shell->env_keys[index], key) == 0)
		{
			return (index);
		}
		index++;
	}
	return (-1);
}
