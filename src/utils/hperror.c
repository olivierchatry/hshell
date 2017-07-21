#include <stdarg.h>
#include <stdio.h>

#include "hshell.h"

/**
 * hperror - Prints an error on stderr
 * @shell: Shell structure
 * @command: Command name
 * @msg: Message to be printed
 *
 * Return: Total number of bytes written
 */
int hperror(shell_t *shell, const char *command, const char *msg, ...)
{
	va_list args;
	int ret;

	va_start(args, msg);
	ret = fprintf(stderr, "%s: %d: %s: ",
		shell->shell_program, shell->line_no, command);
	ret += vfprintf(stderr, msg, args);
	fflush(stderr);
	va_end(args);

	return (ret);
}
