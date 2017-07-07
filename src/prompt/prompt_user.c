#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_user - Expands user name
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_user(shell_t *shell, char id)
{
	const char *user = util_get_user();

	if (user)
		ARRAY_CAT(shell->prompt, user, hstrlen(user), PROMPT_BUFFER_SIZE);
	UNUSED(id);
}
