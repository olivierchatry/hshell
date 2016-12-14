#include <string.h>
#include "hshell.h"
#include "hlib.h"

#define ARGV_BUFFER_SIZE 64

void command_split(command_t *command) {
	const char* delim = " \t\n";
	char*				saveptr;	
	char* 			token;

	token = hstrtok_r(command->line, delim, &saveptr);
	ARRAY_ADD(command->argv, token, ARGV_BUFFER_SIZE);

	while (token) {
		token = hstrtok_r(NULL, delim, &saveptr);
		ARRAY_ADD(command->argv, token, ARGV_BUFFER_SIZE);
	}
}
