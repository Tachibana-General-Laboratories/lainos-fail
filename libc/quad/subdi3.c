#include "quad.h"

/*
 * Subtract two quad values.  This is trivial since a one-bit carry
 * from a single u_int difference x-y occurs if and only if (x-y) > x.
 */
quad_t
__subdi3(quad_t a, quad_t b)
{
	union uu aa, bb, diff;

	aa.q = a;
	bb.q = b;
	diff.ul[L] = aa.ul[L] - bb.ul[L];
	diff.ul[H] = aa.ul[H] - bb.ul[H] - (diff.ul[L] > aa.ul[L]);
	return (diff.q);
}
