#include "unbounded_int.h"

static const ubint UB_ERR = {'*', 0, NULL, NULL}; /* Returned on error */

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
 * Returns 0 if the string s is an integer (all characters are digits, with the
 * exception of the first character that might be a '-' sign).
 * Returns -1 otherwise.
 */
static int is_int(const char* s){
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

/*
 * Returns s (which is an integer) without heading 0s.
 * If s is a negative integer, 0s following the '-' sign are
 * also removed.
 * Returns NULL on error.
 */
static char* remove_heading_zeros(const char* s){
	int   i;
	int   count;
	int   start;
	int   len;
	char* res;

	res   = NULL;
	start = 0;
	count = 0;
	len   = 0;

	if (is_int(s) < 0)
		goto error;

	len = strlen(s);

	if (s[0] == '-')
		start = 1;
	else
		start = 0;

	for (i = start; i < len; i++) {
		if (s[i] != '0')
			break;
		else
			count++;
	}

	if (count == len - start) { /* all zeros */
		res = malloc(2);
		memset(res, '\0', 2);
		strcpy(res, "0");
		return res;
	}

	res = malloc(len + start - count + 1); /* +1 for '\0', +start for sign */
	if (res == NULL) {
		perror("malloc");
		goto error;
	}
	
	if (s[0] == '-') {
		res[0] = '-';
		strcpy(res + 1, s + i);
	} else {
		strcpy(res, s + i);
	}

	return res;

 error:
	fprintf(stderr, "Could not remove heading zeros\n");
	free(res);
	return NULL;
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
	char*  tmp;

	i         = 0;
	start     = 0;
	ret.first = NULL;
	ret.last  = NULL;
	cur       = NULL;
	prev      = NULL;

	if (is_int(s) < 0)
		goto error;

	tmp = remove_heading_zeros(s);
	len = strlen(tmp);

	if (tmp[0] == '-') {
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

	ret.first->val  = tmp[start];
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

		cur->val   = tmp[i];
		cur->prev  = prev;
		prev->next = cur;
		cur->next  = NULL;
		prev       = cur;
	}

	ret.last       = prev;
	ret.last->next = NULL;

	free(tmp);

	return ret;

 error:
	fprintf(stderr, "Could not create unbounded integer from [%s]\n", tmp);
	free(tmp);
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
	if (ub.sign == '-') {
		i = 1;
		res[0] = '-';
	} else {
		i = 0;
	}

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

/*
 * Compares the two unbounded integers a and b.
 * If a or b have '*' as sign value, result is not predictable.
 * Returns an integer less than, equal to, or greater than zero if a
 * is found, respectively, to be less than, to match, or be greater than b.
 */
int compare(ubint a, ubint b){
	digit* a_cur;
	digit* b_cur;

	if (a.sign == '-' && b.sign == '+')
		return -1;

	if (a.sign == '+' && b.sign == '-')
		return 1;

	if (a.len > b.len)
		return ((a.sign == '-') ? -1 : 1);

	if (a.len < b.len)
		return ((a.sign == '-') ? 1 : -1);

	/* a.len == b.len && a.sign == b.sign */
	a_cur = a.first;
	b_cur = b.first;

	while (a_cur != NULL || b_cur != NULL) {
		if (a_cur->val < b_cur->val)
			return ((a.sign = '-') ? -1 : 1);
		if (a_cur->val > b_cur->val)
			return ((a.sign = '-') ? 1 : -1);
		a_cur = a_cur->next;
		b_cur = b_cur->next;
	}

	return 0;
}

/*
 * Compares the unbounded integer a to the long long value b
 * Returns an integer less than, equal to, or greater than zero if a
 * is found, respectively, to be less than, to match, or be greater than b.
 */
int compare_ll(ubint a, long long b){
	ubint b_ubint;

	b_ubint = ll_to_ubint(b);

	return compare(a, b_ubint);
}


/*
 * Removes heading zeros off of the unbounded integer a.
 * Returns NULL on error;
 * This function should be used with unbouned integers that are results of
 * computations of valid unbounded integers.
 */
static ubint* remove_heading_zeros_ubint(ubint* a){
	int    count;
	digit* tmp;

	if (a->sign == '*')
		goto error;

	count = 0;
	tmp   = a->first;

	while (tmp != NULL && tmp->val != '0') {
		a->first = tmp->next;
		tmp = tmp->next;
		free(tmp->prev);
		tmp->prev = NULL;
		count++;
	}

	a->len -= count;

	return a;
	
 error:
	fprintf(stderr, "Could not remove heading zeros\n");
	free_ubint(*a);
	return NULL;
}

/*
 * Returns the sum of the two unbounded integers a and b.
 * Both a and b must be positive. If they are not, returns
 * an unbounded integer with sign '*'.
 */
static ubint positive_sum(ubint a, ubint b){
	int    a_val;
	int    b_val;
	int    i;
	int    longest;
	int    len;
	char   c;
	int    r;
	digit* cur;
	digit* tmp;
	digit* pa;
	digit* pb;
	ubint  res;

	if (a.sign == '*' || b.sign == '*' || a.sign == '-' || b.sign == '-')
		goto error;

	res.first = NULL;
	res.last  = NULL;
	cur       = NULL;
	tmp       = NULL;
	res.sign  = '+';
	longest   = (a.len > b.len) ? a.len : b.len;
	pa        = a.last;
	pb        = b.last;
	r         = 0;
	len       = 0;

	for (i = 0; i < longest; i++) {
		if (pa != NULL) 
			a_val = pa->val - '0';
		else
			a_val = 0;

		if (pb != NULL)
			b_val = pb->val - '0';
		else
			b_val = 0;

		c = (char) ((a_val + b_val + r) % 10 + '0');
		r = (a_val + b_val + r) / 10;

		if (i == 0) {
			res.last = malloc(sizeof (struct digit));
			if (res.last == NULL) {
				perror("malloc");
				goto error;
			}

			res.last->val  = c;
			res.last->next = NULL;
			tmp            = res.last;
		} else {
			cur = malloc(sizeof (struct digit));
			if (cur == NULL) {
				perror("malloc");
				goto error;
			}

			cur->val  = c;
			cur->next = tmp;
			tmp->prev = cur;
			res.first = cur;
			tmp       = cur;
		}

		len++;
		if (pa != NULL)
			pa = pa->prev;
		if (pb != NULL)
			pb = pb->prev;
	}

	res.len = len;

	return *remove_heading_zeros_ubint(&res);

 error:
	fprintf(stderr, "Could not compute sum of positive unbounded integers\n");
	free(cur);
	free_ubint(res);
	return UB_ERR;
}

/*
 * Returns the difference of the two unbounded integers a and b.
 * Both a and b must be positive and such that a >= b. Otherwise,
 * returns an unbounded integer with '*' as sign.
 */
ubint positive_difference(ubint a, ubint b){
	int    a_val;
	int    b_val;
	int    i;
	int    longest;
	int    len;
	int    c_tmp;
	int    r;
	char   c;
	digit* cur;
	digit* tmp;
	digit* pa;
	digit* pb;
	ubint  res;

	if (a.sign == '*' || a.sign == '-' || b.sign == '*' || b.sign == '-')
		goto error;

	if (compare(a, b) <= 0) /* subject says a >= b and a > b... */
		goto error;

	res.first = NULL;
	res.last  = NULL;
	cur       = NULL;
	tmp       = NULL;
	res.sign  = '+';
	longest   = (a.len > b.len) ? a.len : b.len;
	pa        = a.last;
	pb        = b.last;
	r         = 0;
	len       = 0;

	for (i = 0; i < longest; i++) {
		if (pa != NULL) 
			a_val = pa->val - '0';
		else
			a_val = 0;

		if (pb != NULL)
			b_val = pb->val - '0';
		else
			b_val = 0;

		c_tmp = (a_val - b_val + r);
		c     = ((c_tmp >= 0) ? c_tmp : c_tmp + 10) + '0';
		r     = ((c_tmp >= 0) ? 0 : -1);

		if (i == 0) {
			res.last = malloc(sizeof (struct digit));
			if (res.last == NULL) {
				perror("malloc");
				goto error;
			}

			res.last->val  = c;
			res.last->next = NULL;
			tmp            = res.last;
		} else {
			cur = malloc(sizeof (struct digit));
			if (cur == NULL) {
				perror("malloc");
				goto error;
			}

			cur->val  = c;
			cur->next = tmp;
			tmp->prev = cur;
			res.first = cur;
			tmp       = cur;
		}

		len++;
		if (pa != NULL)
			pa = pa->prev;
		if (pb != NULL)
			pb = pb->prev;
	}

	res.len = len;

	return *remove_heading_zeros_ubint(&res);

 error:
	fprintf(stderr, "Could not compute sum of positive unbounded integers\n");
	free(cur);
	free_ubint(res);
	return UB_ERR;
}
