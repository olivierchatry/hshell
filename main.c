#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hshell.h"


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