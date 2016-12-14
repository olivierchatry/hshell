#ifndef __HSHELL_H__
#define __HSHELL_H__

#include <stdlib.h>
#include "array.h"

#define UNUSED(x) (void)x;


#define COMMAND_GET_MAXIMUM_CMD_SIZE 4096*4096

#define ENV_BUFFER_SIZE	1024
#define LINE_BUFFER_SIZE 4096
#define PATH_BUFFER_SIZE 1024
#define ENV_EXPAND_BUFFER_SIZE 1024


#define SHELL_STATE_NONE		0
#define SHELL_STATE_INIT		1
#define SHELL_STATE_RUN			2
#define SHELL_STATE_CANCEL	4

#define SHELL_OP_NONE				0
#define SHELL_OP_AND				1
#define SHELL_OP_OR					2

struct command_tree_s {
	char 									**ARRAY(argv);
	struct command_tree_s	**ARRAY(child);
	int										op;
};

typedef struct command_tree_s command_tree_t;

struct command_s {
	char						*ARRAY(line);
	command_tree_t	**ARRAY(tree);
};

typedef struct command_s command_t;

struct shell_s {
	char	**ARRAY(env_keys);
	char	**ARRAY(env_values);
	char	**ARRAY(alias_keys);
	char	**ARRAY(alias_values);
	int		exit;
	int		exit_code;
	char	**ARRAY(paths);
	char*	paths_string;
	char	**ARRAY(envp);
	int 	state;
	int		cancel_pipe[2];
};

typedef struct shell_s shell_t;


#define OK 0

#define ERR_GET_COMMAND_TO_BIG	1
#define ERR_GET_COMMAND_READ		2
#define ERR_GET_COMMAND_MEMORY	3

int		command_get(shell_t *shell, command_t *command, int fd_from);
void	command_free(command_t *command);
void	command_init(command_t *command);
void	command_split(command_t *command);
void	command_exec(shell_t *shell, command_t *command);
int		command_builtins(shell_t *shell, command_tree_t *command, int *status);
void	command_lexer(command_t *command);
void	command_expand(shell_t *shell, command_t* command);

void	shell_init(shell_t* shell, int argc, char** argv, char** envp);
void	shell_free(shell_t* shell);
void 	shell_getcwd(shell_t* shell);

char	*alias_get(shell_t *shell, const char *key);
char	*alias_set(shell_t *shell, char *alias, char *value);
int		alias_get_index(shell_t *shell, const char *key);
void	alias_add(shell_t *shell, char* env);

void	env_add(shell_t* shell, char* env);
int		env_remove(shell_t *shell, const char* key);
char	*env_get(shell_t *shell, const char *key);
char* env_get_n(shell_t *shell, const char *key, int n);
int		env_get_index(shell_t *shell, const char *key);
int		env_get_index_n(shell_t *shell, const char *key, int n);
char	*env_set(shell_t *shell, const char *key, const char *value);
void	env_hook(shell_t *shell, const char *key);
void	env_rebuild_envp(shell_t *shell);
void	env_free_envp(shell_t *shell);
char	*env_expand(shell_t *shell, const char *str);

void	paths_parse(shell_t* shell);
char	*paths_expand(shell_t *shell, const char* value);
void	paths_free(shell_t *shell);

#endif