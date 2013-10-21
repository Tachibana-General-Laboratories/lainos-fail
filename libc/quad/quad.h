/*
 * Quad arithmetic.
 *
 * This library makes the following assumptions:
 *
 *  - The type long long (aka quad_t) exists.
 *
 *  - A quad variable is exactly twice as long as `int'.
 *
 *  - The machine's arithmetic is two's complement.
 *
 * This library can provide 128-bit arithmetic on a machine with 128-bit
 * quads and 64-bit ints, for instance, or 96-bit arithmetic on machines
 * with 48-bit ints.
 */

#include <stdint.h>
#include <limits.h>

/* Quad constants must be written as expressions; #define GCC_ME_HARDER. */
#define	UQUAD_MAX	((u_quad_t)0-1)	/* max value for a uquad_t */
/* max value for a quad_t */
#define	QUAD_MAX	((quad_t)(UQUAD_MAX >> 1))
#define	QUAD_MIN	(-QUAD_MAX-1)	/* min value for a quad_t */

typedef unsigned long long u_quad_t;    /* quads */
typedef long long quad_t;
typedef quad_t *qaddr_t;
typedef unsigned long u_int;
typedef unsigned long uint;

#define _QUAD_HIGHWORD 1
#define _QUAD_LOWWORD 0

/*
 * Depending on the desired operation, we view a `long long' (aka quad_t) in
 * one or more of the following formats.
 */
union uu {
	quad_t	q;		/* as a (signed) quad */
	u_quad_t uq;		/* as an unsigned quad */
	int	sl[2];		/* as two signed ints */
	u_int	ul[2];		/* as two unsigned ints */
};

/*
 * Define high and low parts of a quad_t.
 */
#define	H		_QUAD_HIGHWORD
#define	L		_QUAD_LOWWORD

/*
 * Total number of bits in a quad_t and in the pieces that make it up.
 * These are used for shifting, and also below for halfword extraction
 * and assembly.
 */
#define	QUAD_BITS	(sizeof(quad_t) * CHAR_BIT)
#define	INT_BITS	(sizeof(int) * CHAR_BIT)
#define	HALF_BITS	(sizeof(int) * CHAR_BIT / 2)

/*
 * Extract high and low shortwords from longword, and move low shortword of
 * longword to upper half of long, i.e., produce the upper longword of
 * ((quad_t)(x) << (number_of_bits_in_int/2)).  (`x' must actually be u_int.)
 *
 * These are used in the multiply code, to split a longword into upper
 * and lower halves, and to reassemble a product as a quad_t, shifted left
 * (sizeof(int)*CHAR_BIT/2).
 */
#define	HHALF(x)	((u_int)(x) >> HALF_BITS)
#define	LHALF(x)	((u_int)(x) & (((int)1 << HALF_BITS) - 1))
#define	LHUP(x)		((u_int)(x) << HALF_BITS)

typedef unsigned int	qshift_t;

quad_t __adddi3(quad_t, quad_t);
quad_t __anddi3(quad_t, quad_t);
quad_t __ashldi3(quad_t, qshift_t);
quad_t __ashrdi3(quad_t, qshift_t);
int __cmpdi2(quad_t, quad_t);
quad_t __divdi3(quad_t, quad_t);
quad_t __fixdfdi(double);
quad_t __fixsfdi(float);
u_quad_t __fixunsdfdi(double);
u_quad_t __fixunssfdi(float);
double __floatdidf(quad_t);
float __floatdisf(quad_t);
double __floatunsdidf(u_quad_t);
quad_t __iordi3(quad_t, quad_t);
quad_t __lshldi3(quad_t, qshift_t);
quad_t __lshrdi3(quad_t, qshift_t);
quad_t __moddi3(quad_t, quad_t);
quad_t __muldi3(quad_t, quad_t);
quad_t __negdi2(quad_t);
quad_t __one_cmpldi2(quad_t);
u_quad_t __qdivrem(u_quad_t, u_quad_t, u_quad_t *);
quad_t __subdi3(quad_t, quad_t);
int __ucmpdi2(u_quad_t, u_quad_t);
u_quad_t __udivdi3(u_quad_t, u_quad_t);
u_quad_t __umoddi3(u_quad_t, u_quad_t);
quad_t __xordi3(quad_t, quad_t);
