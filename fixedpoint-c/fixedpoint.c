#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "macros.h"

int main (int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	double x;
	double y;
	R128 r128_x;
	R128 r128_y;
	R128 r128_r;
	R128 r128_m;

	int ret;
	char bufa[256];
	char bufm[256];

	(void) ret;

	// basic sanity
	x = -2.118018798719000910681;
	r128FromFloat(&r128_r, x);
	R128_TEST_FLEQ(r128_r, x);
	y = r128ToFloat(&r128_r);
	R128_TEST_FLFLEQ(x, y);


	ret = r128ToStringf(bufa, sizeof(bufa), "%#.18f", &r128_r);
	printf("print string : %s\n", bufa);
	printf("r.hi : %llu\n", r128_r.hi);
	printf("r.lo : %llu\n", r128_r.lo);
	//extern int r128ToString(char *dst, size_t dstSize, const R128 *v);

	printf("r: %lld\n", r128ToInt(&r128_r));



	x = -2.118018798719000910681;
	r128FromFloat(&r128_x, x);
	printf("x = %.18f (-2.118018798719000910681)\n", x);
	printf("x.hi : %llu (0x%llx)\n", r128_x.hi, r128_x.hi);
	printf("x.lo : %llu (0x%llx)\n", r128_x.lo, r128_x.lo);

	y = -2.918018798719000910681;
	r128FromFloat(&r128_y, y);
	printf("y = %.18f (-2.918018798719000910681)\n", y);
	printf("y.hi : %llu (0x%llx)\n", r128_y.hi, r128_y.hi);
	printf("y.lo : %llu (0x%llx)\n", r128_y.lo, r128_y.lo);

	r128Mul(&r128_m, &r128_x, &r128_y);
	printf("x * y = %f\n", x * y);
	printf("m.hi : %llu (0x%llx)\n", r128_m.hi, r128_m.hi);
	printf("m.lo : %llu (0x%llx)\n", r128_m.lo, r128_m.lo);

	ret = r128ToStringf(bufm, sizeof(bufm), "x * y = %#.18f", &r128_m);
	printf("print multip : %s\n", bufm);

	printf("\n");

	x = 2.118018798719000910681;
	r128FromFloat(&r128_x, x);
	printf("x = %.18f (2.118018798719000910681)\n", x);
	printf("x.hi : %llu (0x%llx)\n", r128_x.hi, r128_x.hi);
	printf("x.lo : %llu (0x%llx)\n", r128_x.lo, r128_x.lo);

	y = 2.918018798719000910681;
	r128FromFloat(&r128_y, y);
	printf("y = %.18f (2.918018798719000910681)\n", y);
	printf("y.hi : %llu (0x%llx)\n", r128_y.hi, r128_y.hi);
	printf("y.lo : %llu (0x%llx)\n", r128_y.lo, r128_y.lo);

	r128Mul(&r128_m, &r128_x, &r128_y);
	printf("x * y = %f\n", x * y);
	printf("m.hi : %llu (0x%llx)\n", r128_m.hi, r128_m.hi);
	printf("m.lo : %llu (0x%llx)\n", r128_m.lo, r128_m.lo);

	ret = r128ToStringf(bufm, sizeof(bufm), "x * y = %#.18f", &r128_m);
	printf("print multip : %s\n", bufm);


	printf("\n");
	

	x = 3.1;
	r128FromFloat(&r128_x, x);
	if (r128_x.hi & 0x8000000000000000) {
		printf("(-)\n");
	}
	printf("x = %.18f (3.1)\n", x);
	printf("x.hi : %llu (0x%llx)\n", r128_x.hi, r128_x.hi);
	printf("x.lo : %llu (0x%llx)\n", r128_x.lo, r128_x.lo);

	y = -3.1;
	r128FromFloat(&r128_y, y);
	if (r128_y.hi & 0x8000000000000000) {
		printf("(-)\n");
	}
	printf("y = %.18f (-3.1)\n", x);
	printf("y.hi : %llu (0x%llx)\n", r128_y.hi, r128_y.hi);
	printf("y.lo : %llu (0x%llx)\n", r128_y.lo, r128_y.lo);

	printf("y.hi : %llu (0x%llx) [converted]\n", 0xFFFFFFFFFFFFFFFF - r128_y.hi, 0xFFFFFFFFFFFFFFFF - r128_y.hi);
	printf("y.lo : %llu (0x%llx) [converted]\n", 0xFFFFFFFFFFFFFFFF - r128_y.lo + 1, 0xFFFFFFFFFFFFFFFF - r128_y.lo + 1);
	printf("y.hi : %llu (0x%llx) [converted]\n", ~r128_y.hi, ~r128_y.hi);
	printf("y.lo : %llu (0x%llx) [converted]\n", ~r128_y.lo + 1, ~r128_y.lo + 1);
	printf("\n");

	x = 3.9;
	r128FromFloat(&r128_x, x);
	if (r128_x.hi & 0x8000000000000000) {
		printf("(-)\n");
	}
	printf("x = %.18f (3.9)\n", x);
	printf("x.hi : %llu (0x%llx)\n", r128_x.hi, r128_x.hi);
	printf("x.lo : %llu (0x%llx)\n", r128_x.lo, r128_x.lo);

	y = -3.9;
	r128FromFloat(&r128_y, y);
	if (r128_y.hi & 0x8000000000000000) {
		printf("(-)\n");
	}
	printf("y = %.18f (-3.9)\n", x);
	printf("y.hi : %llu (0x%llx)\n", r128_y.hi, r128_y.hi);
	printf("y.lo : %llu (0x%llx)\n", r128_y.lo, r128_y.lo);

	printf("y.hi : %llu (0x%llx) [converted]\n", 0xFFFFFFFFFFFFFFFF - r128_y.hi, 0xFFFFFFFFFFFFFFFF - r128_y.hi);
	printf("y.lo : %llu (0x%llx) [converted]\n", 0xFFFFFFFFFFFFFFFF - r128_y.lo + 1, 0xFFFFFFFFFFFFFFFF - r128_y.lo + 1);

	return 0;
}
