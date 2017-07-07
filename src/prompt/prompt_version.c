#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_version - Expands shell version
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_version(shell_t *shell, char id)
{
	ARRAY_CAT(shell->prompt, SHELL_VERSION, hstrlen(SHELL_VERSION),
		PROMPT_BUFFER_SIZE);
	UNUSED(id);
}
