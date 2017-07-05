#include <string.h>
#include <hshell.h>
#include "utils/hlib.h"

struct token_s
{
	char	*token;
	int	len;
	int	id;
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

static token_t s_tokens[] = {
	{"&&",	2, SHELL_OP_AND},
	{"||",	2, SHELL_OP_OR},
	{";",	1, SHELL_OP_NONE},
	{"\n",	1, SHELL_OP_NONE},
	/*
	 * IMPORTANT
	 * ">>" token needs to be BEFORE ">"
	 * Otherwise, the lexer will detect two times the ">" token instead
	 * of 1 time ">>"
	 *
	 * Same goes for "<<" and "<"
	 */
	{">>",	2, SHELL_OP_REDIRECT_OUT_CONCAT},
	{">",	1, SHELL_OP_REDIRECT_OUT},
	{"<<",	2, SHELL_OP_REDIRECT_IN_UNTIL},
	{"<",	1, SHELL_OP_REDIRECT_IN},
	{NULL,	0, SHELL_OP_NONE}
};

static const char *command_skip_space(const char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
	{
		str++;
	}
	return (str);
}

static const char *command_skip_any(const char *str, char *quote)
{
	char	*delims = TOKEN_SPACE " \n&|;<>";
	char	inhib = 0;

	while (*str && (inhib || *quote || (hstrchr(delims, *str) == NULL)))
	{
		if (!inhib && ((*quote && (*str == *quote)) || (*str == '"' || *str == '\'')))
		{
			*quote = *quote ? 0 : *str;
		}
		inhib = *str == '\\';
		str++;
	}
	return (str);
}

static token_t *command_find_token(const char *str)
{
	token_t	*tokens = s_tokens;

	while (tokens->token && (strncmp(str, tokens->token, tokens->len) != 0))
	{
		tokens++;
	}
	return (tokens);
}

/**
 * command_create - Creates a command structure
 *
 * Return: Pointer to the created structure
 */
command_t *command_create(void)
{
	command_t *cmd = malloc(sizeof(command_t));

	ARRAY_INIT(cmd->argv);
	ARRAY_INIT(cmd->commands);
	cmd->op = SHELL_OP_NONE;
	cmd->alias = NULL;
	cmd->redirect_type = 0;
	cmd->redirect = NULL;
	return (cmd);
}

int command_lexer(command_chain_t *chain)
{
	const char	*line = chain->line;
	command_t	*cmd = command_create();
	char		quote = 0;
	int		error = 0;

	while (*line && !error)
	{
		const char	*start = command_skip_space(line);
		token_t		*token = command_find_token(start);
		const char	*end = line;

		if (token->token)
		{
			end = start + token->len;
			switch (token->id)
			{
			case SHELL_OP_OR:
			case SHELL_OP_AND:
			case SHELL_OP_NONE:
				cmd->op = token->id;
				ARRAY_ADD(cmd->argv, NULL, ARGV_BUFFER_SIZE);
				ARRAY_ADD(chain->root.commands, cmd, COMMAND_BUFFER_SIZE);
				cmd = command_create();
				break;
			case SHELL_OP_REDIRECT_OUT:
			case SHELL_OP_REDIRECT_OUT_CONCAT:
			case SHELL_OP_REDIRECT_IN:
			case SHELL_OP_REDIRECT_IN_UNTIL:
				cmd->redirect_type = token->id;
				break;
			}
		}
		else
		{
			end = command_skip_any(start, &quote);
			if (end - start > 0)
			{
				if (cmd->redirect_type == SHELL_OP_REDIRECT_OUT ||
					cmd->redirect_type == SHELL_OP_REDIRECT_OUT_CONCAT ||
					cmd->redirect_type == SHELL_OP_REDIRECT_IN ||
					cmd->redirect_type == SHELL_OP_REDIRECT_IN_UNTIL)
				{
					cmd->redirect = hstrndup(start, end - start);
				}
				else
				{
					ARRAY_ADD(cmd->argv, hstrndup(start, end - start), ARGV_BUFFER_SIZE);
				}
			}
			else if (*end)
			{
				end++;
			}
		}
		line = end;
	}
	if (error)
	{
		hprintf("parsing error @ %s", line);
	}
	ARRAY_ADD(cmd->argv, NULL, ARGV_BUFFER_SIZE);
	ARRAY_ADD(chain->root.commands, cmd, 2);
	ARRAY_ADD(chain->root.commands, NULL, 1);
	return (!quote);
}
