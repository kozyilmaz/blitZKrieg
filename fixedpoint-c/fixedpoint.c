//#define R128_ASSERT(x)
#define R128_STDC_ONLY
#define R128_IMPLEMENTATION

#include "r128.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


#define PRINT_FAILURE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#define R128_TEST_EQ(v1, v2) do { \
   if ((v1).lo != (v2).lo || (v1).hi != (v2).hi) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v1)), R128_R2(&(v1)), R128_R1(&(v1)), R128_R0(&(v1)), \
         R128_R3(&(v2)), R128_R2(&(v2)), R128_R1(&(v2)), R128_R0(&(v2))); \
   }\
} while(0)

#define R128_TEST_STRSTREQ(s1, s2) do { \
   if (strcmp(s1, s2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: \"%s\" != \"%s\"\n", \
         __FILE__, __LINE__, s1, s2); \
   } \
} while(0)

#define R128_TEST_STREQ(r, s) do { \
   R128 rs; \
   r128FromString(&rs, (s), NULL); \
   R128_TEST_EQ(r, rs); \
} while(0)

#define R128_TEST_FLEQ(r, f) do { \
   char bufr[64], buff[64]; \
   r128ToStringf(bufr, sizeof(bufr), "%#.18f", &(r));\
   sprintf(buff, "%#.18f", (double)(f)); \
   printf("compare r128-f : %s <> %s\n", bufr, bufr); \
   R128_TEST_STRSTREQ(bufr, buff); \
} while(0)

#define R128_TEST_FLFLEQ(f1, f2) do { \
   char buf1[64], buf2[64]; \
   sprintf(buf1, "%.18f", (double)(f1)); \
   sprintf(buf2, "%.18f", (double)(f2)); \
   printf("compare floats : %s <> %s\n", buf1, buf2); \
   R128_TEST_STRSTREQ(buf1, buf2); \
} while(0)


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
	printf("x.hi : %llu\n", r128_x.hi);
	printf("x.lo : %llu\n", r128_x.lo);

	y = -2.918018798719000910681;
	r128FromFloat(&r128_y, y);
	printf("y.hi : %llu\n", r128_y.hi);
	printf("y.lo : %llu\n", r128_y.lo);

	r128Mul(&r128_m, &r128_x, &r128_y);
	printf("m.hi : %llu\n", r128_m.hi);
	printf("m.lo : %llu\n", r128_m.lo);

	ret = r128ToStringf(bufm, sizeof(bufm), "%#.18f", &r128_m);
	printf("print multip : %s\n", bufm);

	return 0;
}
