#include <stdlib.h>
#include <unistd.h>

int hstrcmp(const char *a, const char *b) {
	while (*a && (*a == *b)) {
		a++, b++;
	}
	return *a - *b;
}

int hstrlen(const char* str) {
	int	count = 0;
	while (str[count]) {
		count++;
	}
	return count;
}

char *hstrdup(const char *str) {
	char* dup = malloc(hstrlen(str) + 1);
	char* tmp = dup;
	while (*str) {
		*tmp++ = *str++;
	}
	*tmp++ = 0;
	return dup;
}

int hprintf(const char* str, ...) {
	int count = 0;
	while (*str) {
		count += write(1, str++, 1);
	}
	return count;
}

const char*	hstrchr(const char *str, int c) {
	while(*str) {
		if (*str == c) {
			return str;
		}
		str++;
	}
	return NULL;
}

char*	hstrcpy(char* dest, const char* src) {
	char* temp = dest;
	while (*src) {
		*dest++ = *src++;
	}
	*dest = 0;
	return temp;
}

char* hstrcat(char* dest, const char* src) {
	while (*dest) {
		dest++;
	}
	return hstrcpy(dest, src);
}

char* hstrtok_r(char *str, const char *delim, char **saveptr) {
	char* token = NULL;
	if (!str) {
		str = *saveptr;
	}
	while (*str && hstrchr(delim, *str)) {
		str++;
	}
	if (*str) {
		token = str;
	}
	while (*str && !hstrchr(delim, *str)) {
		str++;
	}
	*saveptr=str + (*str != 0);
	*str=0;
	return token;
}
