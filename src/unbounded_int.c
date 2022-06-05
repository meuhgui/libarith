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

/*
 * Returns a string of length ub.len that contains
 * the value represented by the given ub.
 * Returns NULL on error.
 */
char* ubint_to_str(ubint ub){
	int    i;
	int    len;
	char*  res;
	digit* cur;

	res = NULL;
	cur = NULL;

	if (ub.sign == '*')
		goto error;

	if (ub.sign == '-')
		len = ub.len + 2; /* +2 for sign and '\0' */
	else
		len = ub.len + 1; /* +1 for '\0' */

	res = malloc(len);
	if (res == NULL) {
		perror("malloc");
		goto error;
	}

	memset(res, '\0', len);

	cur = ub.first;
	i   = 0;

	while (cur != NULL && i < len - 1) {
		res[i] = cur->val;
		cur = cur->next;
		i++;
	}

	return res;

 error:
	fprintf(stderr, "Could not convert unbounded integer to string\n");
	free(res);

	return NULL;
}
