#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_user - Expands user name
 * @shell: Shell structure
 * @prompt: Structure containing the expanded string
 * @id: Prompt special character identifier
 */
void prompt_user(shell_t *shell, prompt_t *prompt, char id)
{
	const char *user = util_get_user();

	if (user)
		ARRAY_CAT(prompt->prompt, user, hstrlen(user), PROMPT_BUFFER_SIZE);
	UNUSED(shell);
	UNUSED(id);
}
