#include <stdio.h>
#include "unbounded_int.h"

int main(){
	char* sa = "38";
	char* sb = "29";

	ubint uba = str_to_ubint(sa);
	ubint ubb = str_to_ubint(sb);

	ubint dif = positive_difference(uba, ubb);

	char* res = ubint_to_str(dif);

	printf("%s", res);

	free(res);
	free_ubint(uba);
	free_ubint(ubb);
	free_ubint(dif);

	return 0;
}
