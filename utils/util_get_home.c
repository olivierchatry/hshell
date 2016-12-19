#include "hshell.h"
#include "hlib.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

const char *util_get_home() {
	const char		*path = NULL;
	struct passwd *pws;
	
	pws = getpwuid(geteuid());
	if (pws) {
		path = pws->pw_dir;
	}
	return path;
}
