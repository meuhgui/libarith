#include "utils.h"

/*
 * Returns 0 if the string s is an integer (all characters are digits, with the
 * exception of the first character that might be a '-' sign).
 * Returns -1 otherwise.
 */
int is_int(const char* s){
	int i;
	int len;

	if (s == NULL)
		return -1;

	len = strlen(s);

	for (i = 0; i < len; i++) {
		if (s[i] < 48 || s[i] > 57) {
			if (i == 0 && s[i] == '-' && len > 1)
				continue;
			else
				return -1;
		}
	}

	return 0;
}
