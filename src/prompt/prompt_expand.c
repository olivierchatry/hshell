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
 * @prompt: Structure containing the expanded string
 * @at: Character to search a handler for
 */
void prompt_cases(shell_t *shell, prompt_t *prompt, char at)
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
		(*handler)(shell, prompt, at);
	}
	else
	{
		ARRAY_ADD(prompt->prompt, '\\', PROMPT_BUFFER_SIZE);
		ARRAY_ADD(prompt->prompt, at, PROMPT_BUFFER_SIZE);
	}
}

/**
 * prompt_expand - Expands prompt PS1/PS2/PS3/PS4
 * @shell: Shell structure
 * @ps: PS1/2/3/4 to be expanded
 *
 * Return: Expanded prompt
 */
char *prompt_expand(shell_t *shell, const char *ps)
{
	int		state = 0;
	prompt_t	prompt;
	char		*ret;

	/*ARRAY_RESET(prompt.prompt);*/
	ARRAY_INIT(prompt.prompt);
	while (*ps)
	{
		char at = *ps;

		if (state)
		{
			prompt_cases(shell, &prompt, at);
			state = 0;
		}
		else
		{
			state = *ps == '\\';
			if (!state)
			{
				ARRAY_ADD(prompt.prompt, at, PROMPT_BUFFER_SIZE);
			}
		}
		ps++;
	}
	ARRAY_ADD(prompt.prompt, 0, PROMPT_BUFFER_SIZE);
	ret = hstrdup(prompt.prompt);
	ARRAY_FREE(prompt.prompt);
	return (ret);
}
