#include <string.h>
#include "hshell.h"
#include "hlib.h"

struct token_s {
	char* token;
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

static const char *command_skip_any(const char* str) {
	char		*delims=" \t\n&|;";
	char		inhib = 0;
	while (*str && (inhib || (hstrchr(delims, *str) == NULL))) {
		inhib = *str == '\\';
		str++;
	}
	return str;
}

static token_t *command_find_token(const char* str) {
	token_t* tokens = s_tokens;
	while (tokens->token && (strncmp(str, tokens->token, tokens->len) != 0) ) {
		tokens++;
	}
	return tokens;
}

command_tree_t	*command_tree_create() {
	command_tree_t *cmd_tree = malloc(sizeof(command_tree_t));
	ARRAY_INIT(cmd_tree->argv);
	ARRAY_INIT(cmd_tree->child);
	cmd_tree->op = SHELL_OP_NONE;
	return cmd_tree;
}

void command_lexer(command_t* command) {
	const char			*line = command->line;
	command_tree_t	*cmd_tree = command_tree_create();
	
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
					if (cmd_tree) {
						cmd_tree->op = token->id;
						ARRAY_ADD(cmd_tree->argv, NULL, 64);
						ARRAY_ADD(command->tree, cmd_tree, 16);
						cmd_tree = command_tree_create();
					}
					break;
			}
		} else {
			end = command_skip_any(start);
			if (end-start > 0) {
				ARRAY_ADD(cmd_tree->argv, hstrndup(start, end-start), 64);
			} else {
				end++;
			}
		}		
		line = end;
	}
	ARRAY_ADD(cmd_tree->argv, NULL, 64);
	ARRAY_ADD(command->tree, cmd_tree, 2);
	ARRAY_ADD(command->tree, NULL, 1);
}