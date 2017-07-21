#include <hshell.h>

void command_remove_comment(command_chain_t *chain)
{
	char	*line = chain->line;
	int		in = 0;
	int		inhib = 0;
	int		in_word = 0;

	for (; *line; ++line)
	{
		char at = *line;

		switch (at)
		{
		case '#':
			in = in || (inhib == 0 && in_word == 0);
			break;
		case '"':
		case '\'':
			if (!inhib)
				inhib = (inhib != at);
			else
				inhib = at;
			break;
		case '\n':
			in = 0;
			in_word = 0;
			break;
		case ' ':
		case '\t':
			in_word = 0;
			break;
		default:
			in_word = 1;
			break;
		}
		if (in)
		{
			*line = ' ';
		}
	}
}
