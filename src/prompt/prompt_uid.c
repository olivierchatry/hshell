#include <unistd.h>

#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_uid - Expands root prompt# or user prompt$
 * @shell: Shell structure
 * @prompt: Structure containing the expanded string
 * @id: Prompt special character identifier
 */
void prompt_uid(shell_t *shell, prompt_t *prompt, char id)
{
	const char *p = getuid() == 0 ? "#" : "$";

	ARRAY_CAT(prompt->prompt, p, 1, PROMPT_BUFFER_SIZE);
	UNUSED(shell);
	UNUSED(id);
}
