#include <hshell.h>

void command_remove_comment(command_chain_t *chain) {
	char	*line = chain->line;
	int		in = 0;
	int		inhib = 0;
	for (;*line; ++line) {
		char at = *line;
		switch (at) {
			case '#': 
				in = inhib == 0;
				break;
			case '"': 
			case '\'':
				if (!inhib) {
					if ((inhib == at)) {
						inhib = 0;
					}
				} else {
					inhib = at;
				}
				break;
			case '\n':
				in = 0;
				break;
		}
		if (in) {
			*line = ' ';
		}
	}
}