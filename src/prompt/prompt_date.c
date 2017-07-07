#include <time.h>

#include <hshell.h>

/**
 * prompt_date - Expands today's date
 * @shell: Shell structure
 * @id: Prompt special character identifier
 */
void prompt_date(shell_t *shell, char id)
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char *days[7] = {
		"Sun ", "Mon ", "Tue ", "Wed ", "Thu ", "Fri ", "Sat "
	};
	char *months[12] = {
		"Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ",
		"Jul ", "Aug ", "Sep ", "Oct ", "Nov ", "Dec "
	};
	int day = tm.tm_mday;

	UNUSED(id);
	ARRAY_CAT(shell->prompt, days[tm.tm_wday], 4, PROMPT_BUFFER_SIZE);
	ARRAY_CAT(shell->prompt, months[tm.tm_mon], 4, PROMPT_BUFFER_SIZE);
	if (day >= 10)
		ARRAY_ADD(shell->prompt, (day / 10) + '0', PROMPT_BUFFER_SIZE);
	ARRAY_ADD(shell->prompt, (day % 10) + '0', PROMPT_BUFFER_SIZE);
}
