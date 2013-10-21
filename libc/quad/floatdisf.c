#include "quad.h"

/*
 * Convert (signed) quad to float.
 */
float
__floatdisf(quad_t x) {
	float f;
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
	 * has the units.  Ideally we could just set f, add INT_BITS to
	 * its exponent, and then add the units, but this is portable
	 * code and does not know how to get at an exponent.  Machine-
	 * specific code may be able to do this more efficiently.
	 *
	 * Using double here may be excessive paranoia.
	 */
	f = (double)u.ul[H] * (((int)1 << (INT_BITS - 2)) * 4.0);
	f += u.ul[L];

	return (neg ? -f : f);
}
