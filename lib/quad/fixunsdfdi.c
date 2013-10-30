#include "quad.h"

#define	ONE_FOURTH	((int)1 << (INT_BITS - 2))
#define	ONE_HALF	(ONE_FOURTH * 2.0)
#define	ONE		(ONE_FOURTH * 4.0)

/*
 * Convert double to (unsigned) quad.
 * Not sure what to do with negative numbers---for now, anything out
 * of range becomes UQUAD_MAX.
 */
u_quad_t
__fixunsdfdi(double x) {
	union uu t;
	unsigned int tmp;

	if (x < 0) {
		return (UQUAD_MAX);    /* ??? should be 0?  ERANGE??? */
	}

#ifdef notdef				/* this falls afoul of a GCC bug */

	if (x >= UQUAD_MAX) {
		return (UQUAD_MAX);
	}

#else					/* so we wire in 2^64-1 instead */

	if (x >= 18446744073709551615.0) {	/* XXX */
		return (UQUAD_MAX);
	}

#endif
	/*
	 * Now we know that 0 <= x <= 18446744073709549568.  The upper
	 * limit is one ulp less than 18446744073709551615 tested for above.
	 * Dividing this by 2^32 will *not* round irrespective of any
	 * rounding modes (except if the result is an IEEE denorm).
	 * Furthermore, the quotient will fit into a 32-bit integer.
	 */
	tmp = x / ONE;
	t.ul[L] = (unsigned int)(x - tmp * ONE);
	t.ul[H] = tmp;
	return (t.uq);
}
