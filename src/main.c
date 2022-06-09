#include <stdio.h>
#include "unbounded_int.h"

int main(){
	char* sa = "-23";
	char* sb = "3";

	ubint uba = str_to_ubint(sa);
	ubint ubb = str_to_ubint(sb);

	ubint sum = mult_ubint(uba, ubb);

	char* res = ubint_to_str(sum);

	if (res != NULL)
		printf("%s\n", res);

	free(res);
	free_ubint(uba);
	free_ubint(ubb);
	free_ubint(sum);

	return 0;
}
