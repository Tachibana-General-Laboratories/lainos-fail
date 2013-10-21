#include "quad.h"

/*
 * Divide two unsigned quads.
 */
u_quad_t
__udivdi3(u_quad_t a, u_quad_t b) {

	return (__qdivrem(a, b, (u_quad_t *)0));
}
