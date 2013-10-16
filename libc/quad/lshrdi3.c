#include "quad.h"

/*
 * Shift an (unsigned) quad value right (logical shift right).
 */
quad_t
__lshrdi3(quad_t a, qshift_t shift)
{
	union uu aa;

	if (shift == 0)
		return(a);
	aa.q = a;
	if (shift >= INT_BITS) {
		aa.ul[L] = aa.ul[H] >> (shift - INT_BITS);
		aa.ul[H] = 0;
	} else {
		aa.ul[L] = (aa.ul[L] >> shift) |
		    (aa.ul[H] << (INT_BITS - shift));
		aa.ul[H] >>= shift;
	}
	return (aa.q);
}
