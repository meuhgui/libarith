#ifndef UNBOUNDED_INT_H
#define UNBOUNDED_INT_H

#include <stdlib.h>

struct digit {
	struct digit* next; /* pointer to the next digit */
	char val;           /* value of this digit */
	struct digit* prev; /* pointer to the previous digit */
};

struct unbounded_int {
	char sign;           /* sign of the integer : '+' or '-'. '*' for error */
	size_t len;          /* length of the list that represents the integer */
	struct digit* first; /* pointer to the first digit of the integer */
	struct digit* last;  /* pointer to the last digit of the integer */
};

typedef struct digit digit;
typedef struct unbounded_int ubint;

#endif /* UNBOUNDED_INT_H */
