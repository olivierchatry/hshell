#include "hshell.h"
#include "hlib.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const char	*paths_expand(shell_t *shell, const char* value) {
	struct stat stat;
	if (lstat(value, &stat) == -1) {
		const char* path = hstrdup(env_get(shell, "PATH"));
		const char* delim = ":;";
		hstrtok_r(path, )
	}
	return value;
}
