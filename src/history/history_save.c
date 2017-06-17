#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <hshell.h>
#include "utils/hlib.h"

/**
 * history_save - Saves shell commands history to a file
 * @shell: Shell data structure
 */
void history_save(shell_t *shell)
{
	char	*path;

	if (shell->history_count > 0)
	{
		path = history_get_file();
		if (path)
		{
			int fd = open(path, O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

			if (fd != -1)
			{
				int start	= shell->history_write_index;
				int stop	= (shell->history_write_index - 1 + shell->history_size) % shell->history_size;

				while (start != stop)
				{
					if (shell->history[start])
					{
						write(fd, shell->history[start], hstrlen(shell->history[start]));
						write(fd, "\n", 1);
					}
					start = (start + 1) % shell->history_size;
				}
				close(fd);
			}
		}
		free(path);
	}

}
