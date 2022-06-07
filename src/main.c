#include <stdio.h>
#include "unbounded_int.h"

int main(){
	char* sa = "-38";
	char* sb = "29";

	ubint uba = str_to_ubint(sa);
	ubint ubb = str_to_ubint(sb);

	ubint sum = sum_ubint(uba, ubb);

	char* res = ubint_to_str(sum);

	printf("%s\n", res);

	free(res);
	free_ubint(uba);
	free_ubint(ubb);
	free_ubint(sum);

	return 0;
}
