#include <stdio.h>
#include "utils.h"
#include "unbounded_int.h"

int main(){
	
	ubint ub1;
	char* s1;

	long long i2;
	ubint ub2;

	s1 = "-456839482348023240237456203475620347569238467590274352908746290347";
	i2 = 4223372036854775807;

	ub1 = str_to_ubint(s1);
	ub2 = ll_to_ubint(i2);

	free_ubint(ub1);
	free_ubint(ub2);

	return 0;
}
