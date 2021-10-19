#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "macros.h"

#define RANGE_MIN (-1000)
#define RANGE_MAX (1000)
#define SAMPLE_SIZE (10)

#define HEADER_MAX_STRLEN (64)
#define HEADER_NAME "r128_test_data.h"

/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
	double range = (max - min);
	double div = RAND_MAX / range;
	return min + (rand() / div);
}


//int disp[2][4] = {
//    {10, 11, 12, 13},
//    {14, 15, 16, 17}
//};

int main(int argc, char** argv)
{
	int i;
	int j;
	FILE *fp = NULL;
	size_t len = 0;
	char name[HEADER_MAX_STRLEN];

	double samples[SAMPLE_SIZE];
	double multiplication[SAMPLE_SIZE][SAMPLE_SIZE];
	double division[SAMPLE_SIZE][SAMPLE_SIZE];
	double addition[SAMPLE_SIZE][SAMPLE_SIZE];
	double subtraction[SAMPLE_SIZE][SAMPLE_SIZE];

	// file name check
	if (argc == 1) {
		strcpy(name, HEADER_NAME);
		printf("using default header name: %s\n", name);
	} else if (argc == 2) {
		if (argv[1] == NULL) {
			printf("header name null\n");
			goto err0;
		}
		len = strnlen(argv[1], HEADER_MAX_STRLEN);
		if (len <= 0 || len == HEADER_MAX_STRLEN) {
			printf("header name too long\n");
			goto err0;
		}
		strcpy(name, argv[1]);
		printf("using header name: %s\n", name);
	} else {
		printf("invalid use, must be 'creator [FILE NAME]'\n");
		goto err0;
	}

	// open file
	fp = fopen(name, "w");
	if (!fp) {
		perror("open");
		goto err0;
	}

	// seed randomness
	srand(time(NULL));

	// create test set
	for (i = 0; i < SAMPLE_SIZE; i++) {
		samples[i] = randfrom(RANGE_MIN, RANGE_MAX);
		printf("d[%d] = %.18f\n", i, samples[i]);
	}
	// create multiplication matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		for (j = 0; j < SAMPLE_SIZE; j++) {
			multiplication[i][j] = samples[i] * samples[j];
			printf("multiplication[%d][%d] = %.18f\n", i, j, multiplication[i][j]);
		}
	}
	// create division matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		for (j = 0; j < SAMPLE_SIZE; j++) {
			division[i][j] = samples[i] / samples[j];
			printf("division[%d][%d] = %.18f\n", i, j, division[i][j]);
		}
	}
	// create addition matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		for (j = 0; j < SAMPLE_SIZE; j++) {
			addition[i][j] = samples[i] + samples[j];
			printf("addition[%d][%d] = %.18f\n", i, j, addition[i][j]);
		}
	}
	// create subtraction matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		for (j = 0; j < SAMPLE_SIZE; j++) {
			subtraction[i][j] = samples[i] - samples[j];
			printf("subtraction[%d][%d] = %.18f\n", i, j, subtraction[i][j]);
		}
	}

	R128 r128_double;
	for (i = 0; i < SAMPLE_SIZE; i++) {
		r128FromFloat(&r128_double, samples[i]);
		R128_TEST_FLEQ(r128_double, samples[i]);
	}

	// close file
	fclose(fp);
	// return 
	return 0;

//err1:	fclose(fp);
err0:
	return -1;
}


