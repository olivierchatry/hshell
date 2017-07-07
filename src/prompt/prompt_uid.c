#include <unistd.h>

#include "hshell.h"
#include "utils/hlib.h"

/**
 * prompt_uid - Expands root prompt# or user prompt$
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_uid(shell_t *shell, char id)
{
	const char *prompt = getuid() == 0 ? "#" : "$";

	ARRAY_CAT(shell->prompt, prompt, 1, PROMPT_BUFFER_SIZE);
	UNUSED(id);
}
