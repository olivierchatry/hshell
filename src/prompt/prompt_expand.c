#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <hshell.h>
#include "utils/hlib.h"

typedef struct prompt_case_s
{
	char c;
	prompt_handler_t handler;
} prompt_case_t;

/**
 * prompt_cases - Expands prompt special characters
 * @shell: Shell structure
 * @at: Character to search a handler for
 */
void prompt_cases(shell_t *shell, char at)
{
	prompt_case_t prompt_handlers[] = {
		{'d', &prompt_date},
		{'H', &prompt_hostname},
		{'h', &prompt_hostname},
		{'s', &prompt_shell},
		{'u', &prompt_user},
		{'v', &prompt_version},
		{'W', &prompt_current_folder},
		{'w', &prompt_current_folder_full},
		{'$', &prompt_uid},
		{0, NULL}
	};
	prompt_handler_t handler;
	int i;

	for (i = 0; prompt_handlers[i].c; i++)
	{
		if (prompt_handlers[i].c == at)
		{
			handler = prompt_handlers[i].handler;
			break;
		}
	}

	if (handler)
	{
		(*handler)(shell, at);
	}
	else
	{
		ARRAY_ADD(shell->prompt, '\\', PROMPT_BUFFER_SIZE);
		ARRAY_ADD(shell->prompt, at, PROMPT_BUFFER_SIZE);
	}
}

/**
 * prompt_expand - Expands prompt PS1
 * @shell: Shell structure
 * @prompt: PS1
 */
void prompt_expand(shell_t *shell, const char *prompt)
{
	int	state = 0;

	ARRAY_RESET(shell->prompt);
	while (*prompt)
	{
		char at = *prompt;

		if (state)
		{
			prompt_cases(shell, at);
			state = 0;
		}
		else
		{
			state = *prompt == '\\';
			if (!state)
			{
				ARRAY_ADD(shell->prompt, at, PROMPT_BUFFER_SIZE);
			}
		}
		prompt++;
	}
	ARRAY_ADD(shell->prompt, 0, PROMPT_BUFFER_SIZE);
}
