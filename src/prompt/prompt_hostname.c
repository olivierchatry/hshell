#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_hostname - Expands the hostname
 * @shell: Shell structure
 * @prompt: Structure containing the expanded string
 * @id: Prompt special character identifier
 */
void prompt_hostname(shell_t *shell, prompt_t *prompt, char id)
{
	const char *hostname;

	if (id == 'H')
		hostname = util_get_hostname(true);
	else
		hostname = util_get_hostname(false);

	if (hostname)
		ARRAY_CAT(prompt->prompt, hostname, hstrlen(hostname), PROMPT_BUFFER_SIZE);
	UNUSED(shell);
}
