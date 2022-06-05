#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>

/*
 * Returns 0 if the string s is an integer (all characters are digits, with the
 * exception of the first character that might be a '-' sign).
 * Returns -1 otherwise.
 */
int is_int(const char* s);

#endif /* UTILS_H */
