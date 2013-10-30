#include "quad.h"

/*
 * Return 0, 1, or 2 as a <, =, > b respectively.
 * Both a and b are considered signed---which means only the high word is
 * signed.
 */
int
__cmpdi2(quad_t a, quad_t b) {
	union uu aa, bb;

	aa.q = a;
	bb.q = b;
	return (aa.sl[H] < bb.sl[H] ? 0 : aa.sl[H] > bb.sl[H] ? 2 :
			aa.ul[L] < bb.ul[L] ? 0 : aa.ul[L] > bb.ul[L] ? 2 : 1);
}
