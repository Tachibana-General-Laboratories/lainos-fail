#include "quad.h"

#define	ONE_FOURTH	((int)1 << (INT_BITS - 2))
#define	ONE_HALF	(ONE_FOURTH * 2.0)
#define	ONE		(ONE_FOURTH * 4.0)

/*
 * Convert float to (unsigned) quad.  We do most of our work in double,
 * out of sheer paranoia.
 *
 * Not sure what to do with negative numbers---for now, anything out
 * of range becomes UQUAD_MAX.
 *
 * N.B.: must use new ANSI syntax (sorry).
 */
u_quad_t
__fixunssfdi(float f)
{
	double x, toppart;
	union uu t;

	if (f < 0)
		return (UQUAD_MAX);	/* ??? should be 0?  ERANGE??? */
#ifdef notdef				/* this falls afoul of a GCC bug */
	if (f >= UQUAD_MAX)
		return (UQUAD_MAX);
#else					/* so we wire in 2^64-1 instead */
	if (f >= 18446744073709551615.0)	/* XXX */
		return (UQUAD_MAX);
#endif
	x = f;
	/*
	 * Get the upper part of the result.  Note that the divide
	 * may round up; we want to avoid this if possible, so we
	 * subtract `1/2' first.
	 */
	toppart = (x - ONE_HALF) / ONE;
	/*
	 * Now build a u_quad_t out of the top part.  The difference
	 * between x and this is the bottom part (this may introduce
	 * a few fuzzy bits, but what the heck).  With any luck this
	 * difference will be nonnegative: x should wind up in the
	 * range [0..UINT_MAX].  For paranoia, we assume [INT_MIN..
	 * 2*UINT_MAX] instead.
	 */
	t.ul[H] = (unsigned int)toppart;
	t.ul[L] = 0;
	x -= (double)t.uq;
	if (x < 0) {
		t.ul[H]--;
		x += UINT_MAX;
	}
	if (x > UINT_MAX) {
		t.ul[H]++;
		x -= UINT_MAX;
	}
	t.ul[L] = (u_int)x;
	return (t.uq);
}
