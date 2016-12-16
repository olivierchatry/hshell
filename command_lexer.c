#include <string.h>
#include "hshell.h"
#include "hlib.h"

struct token_s {
	char *token;
	int  	len;
	int		id;
};

typedef struct token_s token_t;


/*
** for later

#define OP_NONE	0
#define OP_PARENTHESIS_OPEN	1
#define OP_PARENTHESIS_CLOSE 2
#define OP_BACKGROUND 5
#define OP_COMMAND_SEPERATOR 6
#define OP_SUB_COMMAND 7
#define OP_REDIRECT_OUT 8
#define OP_REDIRECT_IN 9
#define OP_REDIRECT_OUT_CONCAT 10
	{"(" , 1, OP_PARENTHESIS_OPEN},
	{")" , 1, OP_PARENTHESIS_CLOSE},
	{"&",  1, OP_BACKGROUND}, 
	{"`",  1, OP_SUB_COMMAND},
	{">>",  1, OP_REDIRECT_OUT_CONCAT},
	{">",  1, OP_REDIRECT_OUT},
	{"<",  1, OP_REDIRECT_IN},
*/

static 	token_t s_tokens[] = {
	{"&&", 2, SHELL_OP_AND},
	{"||", 2, SHELL_OP_OR},
	{";",	 1, SHELL_OP_NONE},
	{NULL, 0, SHELL_OP_NONE}
};

static const char *command_skip_space(const char *str) {
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n')) {
		str++;
	}
	return str;
}

static const char *command_skip_any(const char *str) {
	char		*delims=" \t\n&|;";
	char		inhib = 0;
	char		quote = 0;

	while (*str && (inhib || quote || (hstrchr(delims, *str) == NULL))) {
		inhib = *str == '\\';
		if (*str == '"') {
			quote = !quote;
		}
		str++;
	}
	return str;
}

static token_t *command_find_token(const char *str) {
	token_t	*tokens = s_tokens;
	while (tokens->token && (strncmp(str, tokens->token, tokens->len) != 0) ) {
		tokens++;
	}
	return tokens;
}

command_t	*command_create() {
	command_t *cmd = malloc(sizeof(command_t));
	ARRAY_INIT(cmd->argv);
	ARRAY_INIT(cmd->commands);
	cmd->op = SHELL_OP_NONE;
	cmd->alias = NULL;
	return cmd;
}

void command_lexer(command_chain_t *chain) {
	const char	*line = chain->line;
	command_t		*cmd = command_create();
	
	while (*line) {
		const char	*start = command_skip_space(line);
		token_t			*token = command_find_token(start);
		const char	*end;
		if (token->token) {
			end = start + token->len;
			switch (token->id) {
				case SHELL_OP_OR:
				case SHELL_OP_AND:
				case SHELL_OP_NONE:
					if (cmd) {
						cmd->op = token->id;
						ARRAY_ADD(cmd->argv, NULL, ARGV_BUFFER_SIZE);
						ARRAY_ADD(chain->root.commands, cmd, COMMAND_BUFFER_SIZE);
						cmd = command_create();
					}
					break;
			}
		} else {
			end = command_skip_any(start);
			if (end-start > 0) {
				ARRAY_ADD(cmd->argv, hstrndup(start, end-start), ARGV_BUFFER_SIZE);
			} else if (*end) {
				end++;
			}
		}		
		line = end;
	}
	ARRAY_ADD(cmd->argv, NULL, 64);
	ARRAY_ADD(chain->root.commands, cmd, 2);
	ARRAY_ADD(chain->root.commands, NULL, 1);
}
