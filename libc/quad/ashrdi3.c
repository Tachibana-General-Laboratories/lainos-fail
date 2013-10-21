#include "quad.h"

/*
 * Shift a (signed) quad value right (arithmetic shift right).
 */
quad_t
__ashrdi3(quad_t a, qshift_t shift) {
	union uu aa;

	if (shift == 0) {
		return (a);
	}

	aa.q = a;

	if (shift >= INT_BITS) {
		int s;

		/*
		 * Smear bits rightward using the machine's right-shift
		 * method, whether that is sign extension or zero fill,
		 * to get the `sign word' s.  Note that shifting by
		 * INT_BITS is undefined, so we shift (INT_BITS-1),
		 * then 1 more, to get our answer.
		 */
		/* LINTED inherits machine dependency */
		s = (aa.sl[H] >> (INT_BITS - 1)) >> 1;
		/* LINTED inherits machine dependency*/
		aa.ul[L] = aa.sl[H] >> (shift - INT_BITS);
		aa.ul[H] = s;
	} else {
		aa.ul[L] = (aa.ul[L] >> shift) |
				   (aa.ul[H] << (INT_BITS - shift));
		/* LINTED inherits machine dependency */
		aa.sl[H] >>= shift;
	}

	return (aa.q);
}
