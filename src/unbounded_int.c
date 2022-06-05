#include "unbounded_int.h"

/*
 * Frees all the memory allocated to the given unbounded integer
 */
void free_ubint(ubint ui){
	digit* current;
	digit* next;

	current  = NULL;
	next     = NULL;

	current = ui.first;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	ui.first = NULL;
	ui.last  = NULL;
}

/*
 * Returns an unbounded integer from the integer represented by s.
 * If s does not represent an integer or an error occured, the unbounded
 * integer returned will have '*' as sign.
 */
ubint str_to_ubint(const char* s){
	int    i;
	int    start;
	int    len;
	ubint  ret;
	digit* cur;
	digit* prev;

	i         = 0;
	start     = 0;
	ret.first = NULL;
	ret.last  = NULL;
	cur       = NULL;
	prev      = NULL;
	len       = strlen(s);

	if (is_int(s) < 0)
		goto error;

	if (s[0] == '-') {
		ret.sign = '-';
		ret.len  = len - 1;
		start    = 1;
	} else {
		ret.sign = '+';
		ret.len  = len;
		start    = 0;
	}

	ret.first = malloc(sizeof (struct digit));
	if (ret.first == NULL) {
		perror("malloc");
		return UB_ERR;
	}

	ret.first->val  = s[start];
	ret.last        = ret.first;
	ret.first->next = NULL;
	ret.first->prev = NULL;
	prev            = ret.first;

	for (i = start + 1; i < len; i++) {
		cur = malloc(sizeof (struct digit));
		if (cur == NULL) {
			perror("malloc");
			goto error;
		}

		cur->val   = s[i];
		cur->prev  = prev;
		prev->next = cur;
		cur->next  = NULL;
		prev       = cur;
	}

	ret.last       = prev;
	ret.last->next = NULL;

	return ret;

 error:
	fprintf(stderr, "Could not create unbounded integer from [%s]\n", s);
	free_ubint(ret);
	
	return UB_ERR;
}

/*
 * Returns an unbounded integer from the integer represented by i.
 * If an error occured, returns an unbounded integer with '*' as sign value.
 */
ubint ll_to_ubint(long long i){
	char str[256]; /* Plenty of bytes */

	if (snprintf(str, 256, "%lld", i) < 0)
		goto error;

	return str_to_ubint(str);

 error:
	fprintf(stderr, "Could not create unbounded integer from [%lld]\n", i);

	return UB_ERR;
}
