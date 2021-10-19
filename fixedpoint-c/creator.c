#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "macros.h"

#define RANGE_MIN (-1000)
#define RANGE_MAX (1000)

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

int main (void)
{
	int i;
	double d;

	srand(time(NULL));

	for (i = 0; i < 10; i++) {
		d = randfrom(RANGE_MIN, RANGE_MAX);
		printf("d[%d] = %.18f\n", i, d);
	}


	R128 r128_double;
	r128FromFloat(&r128_double, d);
	R128_TEST_FLEQ(r128_double, d);

	return 0;
}