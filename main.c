#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "hshell.h"

#define ARGV_BUFFER_SIZE 64

void command_split(command_t* command) {
	const char* delim = " \t";
	char*				saveptr;
	char* 			token = strtok_r(command->line, delim, &saveptr);

	ARRAY_ADD(command->argv, token, ARGV_BUFFER_SIZE);

	while (token) {
		token = strtok_r(NULL, delim, &saveptr);
		ARRAY_ADD(command->argv, token, ARGV_BUFFER_SIZE);
	}
}

void command_exec(command_t* command) {
	int pid = fork();
	if (pid) {
		int status;
		wait(&status);
	} else {
		execvp(command->argv[0], command->argv);		
	}
}

int main() {
	while (1) {
		command_t command;

		command_init(&command);	
		command_get(&command, 0);
		command_split(&command);
		command_exec(&command);
		printf("command : %s\n", command.line);
		command_free(&command);
	}
}