#ifndef __HSHELL_H__
#define __HSHELL_H__

#include <stdlib.h>
#include "array.h"

#define COMMAND_GET_MAXIMUM_CMD_SIZE 4096*4096

#define ENV_BUFFER_SIZE	1024
#define LINE_BUFFER_SIZE 4096
#define PATH_BUFFER_SIZE 1024

struct command_s {
	char* 	ARRAY(line);
	char** 	ARRAY(argv);
};

typedef struct command_s command_t;

struct shell_s {
	char	**ARRAY(env_keys);
	char	**ARRAY(env_values);
	int		exit;
	char	**ARRAY(paths);
	char*	paths_string;
};

typedef struct shell_s shell_t;


#define OK 0

#define ERR_GET_COMMAND_TO_BIG	1
#define ERR_GET_COMMAND_READ		2
#define ERR_GET_COMMAND_MEMORY	3

int		command_get(command_t *command, int fd_from);
void	command_free(command_t *command);
void	command_init(command_t *command);
void	command_split(command_t *command);
void	command_exec(command_t *command);

void	shell_init(shell_t* shell, int argc, char** argv, char** envp);
void	shell_free(shell_t* shell);

void	env_add(shell_t* shell, char* env);
void	env_remove(shell_t *shell, const char* key);
char	*env_get(shell_t *shell, const char *key);
int		env_get_index(shell_t *shell, const char *key);
char	*env_set(shell_t *shell, const char *key, const char *value);

void	paths_parse(shell_t* shell);
const char	*paths_expand(shell_t *shell, const char* value);

#endif