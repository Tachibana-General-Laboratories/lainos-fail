#include "quad.h"

/*
 * Return remainder after dividing two unsigned quads.
 */
u_quad_t
__umoddi3(u_quad_t a, u_quad_t b)
{
	u_quad_t r;

	(void)__qdivrem(a, b, &r);
	return (r);
}
