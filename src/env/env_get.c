#include <hshell.h>

char	*env_get_n(shell_t *shell, const char *key, int n)
{
	int index = env_get_index_n(shell, key, n);

	if (index != -1)
	{
		return (shell->env_values[index]);
	}
	return (NULL);
}

char	*env_get(shell_t *shell, const char *key)
{
	int index = env_get_index(shell, key);

	if (index != -1)
	{
		return (shell->env_values[index]);
	}
	return (NULL);
}
