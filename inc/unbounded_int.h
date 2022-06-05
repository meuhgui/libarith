#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

struct digit {
	struct digit* next; /* pointer to the next digit */
	char          val;  /* value of this digit */
	struct digit* prev; /* pointer to the previous digit */
};

struct unbounded_int {
	char          sign;  /* sign of the integer : '+' or '-'. '*' for error */
	size_t        len;   /* length of the list that represents the integer */
	struct digit* first; /* pointer to the first digit of the integer */
	struct digit* last;  /* pointer to the last digit of the integer */
};

typedef struct digit digit;
typedef struct unbounded_int ubint;

static const ubint UB_ERR = {'*', 0, NULL, NULL}; /* Returned on error */

/*
 * Frees all the memory allocated to the given unbounded integer
 */
void free_ubint(ubint ui);

/*
 * Returns an unbounded integer from the integer represented by s.
 * If s does not represent an integer or an error occured, the unbounded
 * integer returned will have '*' as sign.
 */
ubint str_to_ubint(const char* s);

/*
 * Returns an unbounded integer from the integer represented by i.
 * If an error occured, returns an unbounded integer with '*' as sign value.
 */
ubint ll_to_ubint(long long i);

#endif /* UNBOUNDED_INT_H */
