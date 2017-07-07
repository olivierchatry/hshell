#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_shell - Expands shell name
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_shell(shell_t *shell, char id)
{
	ARRAY_CAT(shell->prompt, SHELL_NAME, hstrlen(SHELL_NAME), PROMPT_BUFFER_SIZE);
	UNUSED(id);
}
