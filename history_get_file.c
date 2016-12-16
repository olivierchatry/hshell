#include "hshell.h"
#include "hlib.h"

char	*history_get_file() {
	const char	*home;
	const char	*history = ".simple_shell_history";
	char				*path = NULL;

	home = util_get_home();
	if (home) {
		path = malloc(hstrlen(home) + hstrlen(history) + 2);
		if (path) {
			hstrcpy(path, home);
			hstrcat(path, "/");
			hstrcat(path, history);
		}
	}

	return path;
}