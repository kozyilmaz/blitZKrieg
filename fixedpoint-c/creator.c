#include <stdio.h>
#include <stdlib.h>

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

int main (void)
{
	double d = randfrom(1,-1);
	printf("d=%lf\n", d);
	return 0;
}