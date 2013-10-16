#include "quad.h"

/*
 * Convert double to (signed) quad.
 * We clamp anything that is out of range.
 */
quad_t
__fixdfdi(double x)
{
	if (x < 0)
		if (x <= QUAD_MIN)
			return (QUAD_MIN);
		else
			return ((quad_t)-(u_quad_t)-x);
	else
		if (x >= QUAD_MAX)
			return (QUAD_MAX);
		else
			return ((quad_t)(u_quad_t)x);
}
