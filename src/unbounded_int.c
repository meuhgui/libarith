#include "unbounded_int.h"

/*
 * Returns a ubint from the integer represented by s.
 * If s does not represent an integer or an error occured, the ubint returned
 * will have '*' as sign.
 */
ubint str_to_ubint(const char* s){
	ubint a;

	a.sign = 's';

	return a;
}
