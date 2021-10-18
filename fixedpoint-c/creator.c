#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
		d = randfrom(-1, 1);
		printf("d[%d] = %.18f\n", i, d);
	}
	return 0;
}