#ifndef __HSHELL_H__
#define __HSHELL_H__

#define GET_COMMAND_MAXIMUM_CMD_SIZE 4096*4096
#include "array.h"

struct command_s {
	char* 	ARRAY(line);
	char** 	ARRAY(argv);
};

typedef struct command_s command_t;

#define OK 0

#define ERR_GET_COMMAND_TO_BIG	1
#define ERR_GET_COMMAND_READ		2
#define ERR_GET_COMMAND_MEMORY	3

int		command_get(command_t* command, int fd_from);
void	command_free(command_t* command);
void 	command_init(command_t* command);

#endif