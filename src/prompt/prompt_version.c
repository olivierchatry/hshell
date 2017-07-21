#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_version - Expands shell version
 * @shell: Shell structure
 * @prompt: Structure containing the expanded string
 * @id: Prompt special character identifier
 */
void prompt_version(shell_t *shell, prompt_t *prompt, char id)
{
	ARRAY_CAT(prompt->prompt, SHELL_VERSION, hstrlen(SHELL_VERSION),
		PROMPT_BUFFER_SIZE);
	UNUSED(shell);
	UNUSED(id);
}
