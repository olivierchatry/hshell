#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_shell - Expands shell name
 * @shell: Shell structure
 * @prompt: Structure containing the expanded string
 * @id: Prompt special character identifier
 */
void prompt_shell(shell_t *shell, prompt_t *prompt, char id)
{
	ARRAY_CAT(prompt->prompt, SHELL_NAME, hstrlen(SHELL_NAME),
		PROMPT_BUFFER_SIZE);
	UNUSED(shell);
	UNUSED(id);
}
