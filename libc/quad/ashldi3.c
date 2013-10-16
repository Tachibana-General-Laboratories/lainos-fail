#include "quad.h"

/*
 * Shift a (signed) quad value left (arithmetic shift left).
 * This is the same as logical shift left!
 */
quad_t
__ashldi3(quad_t a, qshift_t shift)
{
	union uu aa;

	if (shift == 0)
		return(a);
	aa.q = a;
	if (shift >= INT_BITS) {
		aa.ul[H] = aa.ul[L] << (shift - INT_BITS);
		aa.ul[L] = 0;
	} else {
		aa.ul[H] = (aa.ul[H] << shift) |
		    (aa.ul[L] >> (INT_BITS - shift));
		aa.ul[L] <<= shift;
	}
	return (aa.q);
}
