#include "quad.h"

/*
 * Return 0, 1, or 2 as a <, =, > b respectively.
 * Neither a nor b are considered signed.
 */
int
__ucmpdi2(u_quad_t a, u_quad_t b) {
	union uu aa, bb;

	aa.uq = a;
	bb.uq = b;
	return (aa.ul[H] < bb.ul[H] ? 0 : aa.ul[H] > bb.ul[H] ? 2 :
			aa.ul[L] < bb.ul[L] ? 0 : aa.ul[L] > bb.ul[L] ? 2 : 1);
}
