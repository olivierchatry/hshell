#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_hostname - Expands the hostname
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_hostname(shell_t *shell, char id)
{
	const char *hostname;

	if (id == 'H')
		hostname = util_get_hostname(true);
	else
		hostname = util_get_hostname(false);

	if (hostname)
		ARRAY_CAT(shell->prompt, hostname, hstrlen(hostname), PROMPT_BUFFER_SIZE);
}
