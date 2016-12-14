#include <unistd.h>
#include "hshell.h"
#include "hlib.h"

static const char *env_skip_any(const char* str) {
	char		*delims=" \t\n&|;$\\/'\"";
	char		inhib = 0;

	while (*str && (inhib || (hstrchr(delims, *str) == NULL))) {
		str++;
	}
	return str;
}

char	*env_expand(shell_t *shell, const char *str) {
	char	*ARRAY(expanded);
	ARRAY_INIT(expanded);
	int inhib = 0;
	while (*str) {
		int found = 0;
		if (!inhib && *str == '$') {
			const char *start = str + 1;
			const char *end = start + 1;
			const char *env = NULL;
			char temp[64];

			if (*start == '$' || *start == '?') {
				sprintf(temp, "%u", *start == '?' ? shell->child_exit_code : getpid());
				env = temp;
			} else {
				end = env_skip_any(start);
				env = env_get_n(shell, start, end - start);
			}

			if (env) {
				while (*env) {
					ARRAY_ADD(expanded, *env, ENV_EXPAND_BUFFER_SIZE);
					env++;
				}
				str = end;
				found = 1;
			} 
		}  
		if (!found) {
			ARRAY_ADD(expanded, *str, ENV_EXPAND_BUFFER_SIZE);			
			inhib = *str == '\\';
			str++;
		}
	}
	ARRAY_ADD(expanded, 0, ENV_EXPAND_BUFFER_SIZE);			
	return expanded;
}
