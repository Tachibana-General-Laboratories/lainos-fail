#include "quad.h"

/*
 * Divide two signed quads.
 * ??? if -1/2 should produce -1 on this machine, this code is wrong
 */
quad_t
__divdi3(quad_t a, quad_t b)
{
	u_quad_t ua, ub, uq;
	int neg = 0;

	ua = a;
	ub = b;

	if (a < 0)
		ua = -ua, neg ^= 1;
	if (b < 0)
		ub = -ub, neg ^= 1;

	uq = __qdivrem(ua, ub, (u_quad_t *)0);
	if (neg)
		uq = - uq;
	return uq;
}
