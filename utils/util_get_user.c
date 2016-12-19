#include "hshell.h"
#include "hlib.h"

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

const char *util_get_user() {
	const char		*user = NULL;
	struct passwd *pws;
	
	pws = getpwuid(geteuid());
	if (pws) {
		user = pws->pw_name;
	}
	return user;
}
