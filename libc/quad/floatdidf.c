#include "quad.h"

/*
 * Convert (signed) quad to double.
 */
double
__floatdidf(quad_t x) {
	double d;
	union uu u;
	int neg;

	/*
	 * Get an unsigned number first, by negating if necessary.
	 */
	if (x < 0) {
		u.q = -x, neg = 1;
	} else {
		u.q = x, neg = 0;
	}

	/*
	 * Now u.ul[H] has the factor of 2^32 (or whatever) and u.ul[L]
	 * has the units.  Ideally we could just set d, add INT_BITS to
	 * its exponent, and then add the units, but this is portable
	 * code and does not know how to get at an exponent.  Machine-
	 * specific code may be able to do this more efficiently.
	 */
	d = (double)u.ul[H] * (((int)1 << (INT_BITS - 2)) * 4.0);
	d += u.ul[L];

	return (neg ? -d : d);
}
