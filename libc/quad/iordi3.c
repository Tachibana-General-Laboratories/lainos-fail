#include "quad.h"

/*
 * Return a | b, in quad.
 */
quad_t
__iordi3(quad_t a, quad_t b)
{
	union uu aa, bb;

	aa.q = a;
	bb.q = b;
	aa.ul[0] |= bb.ul[0];
	aa.ul[1] |= bb.ul[1];
	return (aa.q);
}
