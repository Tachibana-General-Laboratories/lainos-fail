#include "quad.h"

/*
 * Add two quads.  This is trivial since a one-bit carry from a single
 * u_int addition x+y occurs if and only if the sum x+y is less than
 * either x or y (the choice to compare with x or y is arbitrary).
 */
quad_t
__adddi3(quad_t a, quad_t b)
{
	union uu aa, bb, sum;

	aa.q = a;
	bb.q = b;
	sum.ul[L] = aa.ul[L] + bb.ul[L];
	sum.ul[H] = aa.ul[H] + bb.ul[H] + (sum.ul[L] < bb.ul[L]);
	return (sum.q);
}
