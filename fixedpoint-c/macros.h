#ifndef __R128_MACROS_H__
#define __R128_MACROS_H__

#if defined(__cplusplus)
extern "C" {
#endif

//#define R128_ASSERT(x)
//#define R128_STDC_ONLY
#define R128_IMPLEMENTATION
#include "r128.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define PRINT_FAILURE(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__)

#define R128_TEST_EQ(v1, v2) do { \
   if ((v1).lo != (v2).lo || (v1).hi != (v2).hi) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: Got 0x%08x%08x.%08x%08x, expected 0x%08x%08x.%08x%08x\n", \
         __FILE__, __LINE__, R128_R3(&(v1)), R128_R2(&(v1)), R128_R1(&(v1)), R128_R0(&(v1)), \
         R128_R3(&(v2)), R128_R2(&(v2)), R128_R1(&(v2)), R128_R0(&(v2))); \
      assert(0); \
   }\
} while(0)

#define R128_TEST_STRSTREQ(s1, s2) do { \
   if (strcmp(s1, s2)) { \
      PRINT_FAILURE("%s(%d): TEST FAILED: \"%s\" != \"%s\"\n", \
         __FILE__, __LINE__, s1, s2); \
      assert(0); \
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
   R128_TEST_STRSTREQ(bufr, buff); \
} while(0)

#define R128_TEST_FLFLEQ(f1, f2) do { \
   char buf1[64], buf2[64]; \
   sprintf(buf1, "%.18f", (double)(f1)); \
   sprintf(buf2, "%.18f", (double)(f2)); \
   R128_TEST_STRSTREQ(buf1, buf2); \
} while(0)


#if defined(__cplusplus)
}
#endif

#endif /* __R128_MACROS_H__ */
