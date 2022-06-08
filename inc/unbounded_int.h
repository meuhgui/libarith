#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

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

/*
 * Frees all the memory allocated to the given unbounded integer
 */
extern void free_ubint(ubint ui);

/*
 * Returns an unbounded integer from the integer represented by s.
 * If s does not represent an integer or an error occured, the unbounded
 * integer returned will have '*' as sign.
 */
extern ubint str_to_ubint(const char* s);

/*
 * Returns an unbounded integer from the integer represented by i.
 * If an error occured, returns an unbounded integer with '*' as sign value.
 */
extern ubint ll_to_ubint(long long i);

/*
 * Returns a string of length ub.len that contains
 * the value represented by the given ub.
 * Returns NULL on error.
 */
extern char* ubint_to_str(ubint ub);

/*
 * Compares the two unbounded integers a and b.
 * Returns an integer less than, equal to, or greater than zero if a
 * is found, respectively, to be less than, to match, or be greater than b.
 */
extern int compare(ubint a, ubint b);

/*
 * Compares the unbounded integer a to the long long value b
 * Returns an integer less than, equal to, or greater than zero if a
 * is found, respectively, to be less than, to match, or be greater than b.
 */
extern int compare_ll(ubint a, long long b);

/*
 * Returns the sum of the unbounded integers a and b.
 * If an error occured, an unbounded integer of sign '*' is returned.
 */
extern ubint sum_ubint(ubint a, ubint b);

/*
 * Returns the difference of the unbounded integers a and b.
 * If an error occured, an unbounded integer of sign '*' is returned.
 */
extern ubint dif_ubint(ubint a, ubint b);

#endif /* UNBOUNDED_INT_H */
