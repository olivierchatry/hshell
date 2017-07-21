#include <unistd.h>

#include <hshell.h>
#include "utils/hlib.h"

/**
 * shell_getcwd - Get process current directory
 * @shell: Shell structure
 */
void shell_getcwd(shell_t *shell)
{
	int	buffer_size = 1024;
	char	*buffer = malloc(buffer_size);

	while (getcwd(buffer, buffer_size) == NULL)
	{
		buffer_size += 1024;
		free(buffer);
		buffer = malloc(buffer_size);
	}
	env_set(shell, "PWD", buffer);
	free(buffer);
}
