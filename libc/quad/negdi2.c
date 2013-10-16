#include "quad.h"

/*
 * Return -a (or, equivalently, 0 - a), in quad.  See subdi3.c.
 */
quad_t
__negdi2(quad_t a)
{
	union uu aa, res;

	aa.q = a;
	res.ul[L] = -aa.ul[L];
	res.ul[H] = -aa.ul[H] - (res.ul[L] > 0);
	return (res.q);
}
