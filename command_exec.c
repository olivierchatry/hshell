#include "hshell.h"
#include <unistd.h>
#include <sys/wait.h>

void command_exec(command_t* command) {
	int pid = fork();
	if (pid) {
		int status;
		wait(&status);
	} else {
		execvp(command->argv[0], command->argv);		
	}
}
