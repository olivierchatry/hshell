#include "hshell.h"
#include "hlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define TEMP_BUFFER_SIZE 	4096

char	*paths_expand(shell_t *shell, const char* value) {
	struct stat stat;
	char*				temp = NULL;
	int					temp_size = 0;
	int					value_size;
	int 				found = 0;

	value_size = hstrlen(value);
	if (lstat(value, &stat) == -1) {
		int index;
		for (index = 0; (index < shell->paths_size) && !found; ++index) {
			const char* path = shell->paths[index];
			if (path) {
				int path_size = hstrlen(shell->paths[index]);
				if (path_size + value_size >= temp_size) {
					temp_size += TEMP_BUFFER_SIZE;
					free(temp);
					temp = malloc(temp_size);
				}
				hstrcpy(temp, path);
				if (path[path_size - 1] != '/') {
					hstrcat(temp, "/");				
				}
				hstrcat(temp, value);
				found = lstat(temp, &stat) == 0; 
			}
		}
	} 
	
	if (!found) {
		temp = hstrdup(value);
	}

	return temp;
}
