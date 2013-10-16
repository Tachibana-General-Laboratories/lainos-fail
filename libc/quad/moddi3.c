#include "quad.h"

/*
 * Return remainder after dividing two signed quads.
 *
 * XXX	we assume a % b < 0 iff a < 0, but this is actually machine-dependent.
 */
quad_t
__moddi3(quad_t a, quad_t b)
{
	u_quad_t ua, ub, ur;
	int neg = 0;

	ua = a;
	ub = b;

	if (a < 0)
		ua = -ua, neg ^= 1;
	if (b < 0)
		ub = -ub;
	(void)__qdivrem(ua, ub, &ur);
	if (neg)
		ur = -ur;
	return (ur);
}
