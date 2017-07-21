#include <hshell.h>
#include "utils/hlib.h"

#define ISQUOTE(c) ((c == '"') || (c == '\''))

void command_remove_quote_str(char *str)
{
	while (*str)
	{
		if (ISQUOTE(*str))
		{
			char *mmove = str;

			while (*(mmove))
			{
				*mmove = *(mmove + 1);
				mmove++;
			}
		}
		else
		{
			str++;
		}
	}
}

void command_remove_quote_r(command_t *command)
{
	ARRAY_EACH(command->argv, command_remove_quote_str);
	ARRAY_EACH(command->commands, command_remove_quote_r);
}

void command_remove_quote(command_chain_t *chain)
{
	command_remove_quote_r(&chain->root);
}
