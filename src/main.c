#include <stdio.h>
#include "unbounded_int.h"

int main(){
	char* sa = "23";
	char* sb = "197";

	ubint uba = str_to_ubint(sa);
	ubint ubb = str_to_ubint(sb);

	free_ubint(uba);
	free_ubint(ubb);

	return 0;
}
