#include "hshell.h"
#include "array.h"

void shell_free(shell_t* shell) {
	ARRAY_EACH(shell->env_key, free);
	ARRAY_EACH(shell->env_value, free);
	ARRAY_FREE(shell->env_key);
	ARRAY_FREE(shell->env_value);
}
