#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <float.h>

#include "macros.h"

#define RANGE_MIN (-1000)
#define RANGE_MAX (1000)
#define SAMPLE_SIZE (10)
#define SENSITIVITY (0.0000001)

#define HEADER_MAX_STRLEN (64)
#define HEADER_NAME "r128_test_data.h"

struct unpacked {
	uint64_t lo;
	uint64_t hi;
};

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

	struct unpacked samples_unpacked[SAMPLE_SIZE];

	struct unpacked multiplication_unpacked[SAMPLE_SIZE][SAMPLE_SIZE];

	double d;
	R128 r128_double;
	R128 r128_a;
	R128 r128_b;
	R128 r128_c;

	char buf[64];

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

	// convert double to r128 64x64 format
	for (i = 0; i < SAMPLE_SIZE; i++) {
		r128FromFloat(&r128_double, samples[i]);
		// sanity check
		R128_TEST_FLEQ(r128_double, samples[i]);
		// samples unpacked
		samples_unpacked[i].lo = r128_double.lo;
		samples_unpacked[i].hi = r128_double.hi;
		printf("samples_unpacked[%d] = lo:%lu hi:%lu\n", i, samples_unpacked[i].lo, samples_unpacked[i].hi);

	}
	// create multiplication matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		r128_a.lo = samples_unpacked[i].lo;
		r128_a.hi = samples_unpacked[i].hi;
		for (j = 0; j < SAMPLE_SIZE; j++) {
			r128_b.lo = samples_unpacked[j].lo;
			r128_b.hi = samples_unpacked[j].hi;
			r128Mul(&r128_c, &r128_a, &r128_b);
			multiplication_unpacked[i][j].lo = r128_c.lo;
			multiplication_unpacked[i][j].hi = r128_c.hi;
		}
	}
	// create division matrix - with regular double 
	for (i = 0; i < SAMPLE_SIZE; i++) {
		r128_a.lo = samples_unpacked[i].lo;
		r128_a.hi = samples_unpacked[i].hi;
		for (j = 0; j < SAMPLE_SIZE; j++) {
			r128_b.lo = samples_unpacked[j].lo;
			r128_b.hi = samples_unpacked[j].hi;
			r128Div(&r128_c, &r128_a, &r128_b);
			division_unpacked[i][j].lo = r128_c.lo;
			division_unpacked[i][j].hi = r128_c.hi;
		}
	}


	for (i = 0; i < SAMPLE_SIZE; i++) {
		r128_double.lo = samples_unpacked[i].lo;
		r128_double.hi = samples_unpacked[i].hi;
		R128_TEST_FLEQ(r128_double, samples[i]);
	}


	//		r128ToStringf(buf, sizeof(buf), "%#.18f", &r128_c);
	//		d = r128ToFloat(&r128_c);
	//		bool isEqual = fabs(d - multiplication[i][j]) <= FLT_EPSILON;
	//		printf("multiplication[%d][%d] = %s %.18f x %.18f (%s)\n", i, j, buf, multiplication[i][j], d, (isEqual == true) ? "EQUAL" : "NOT EQUAL");
	//		R128_TEST_FLEQ(r128_c, multiplication[i][j]);
	(void) multiplication_unpacked;

	// close file
	fclose(fp);
	// return 
	return 0;

//err1:	fclose(fp);
err0:
	return -1;
}


