#include "quad.h"

/*
 * Convert float to (signed) quad.
 * We clamp anything that is out of range.
 *
 * N.B.: must use new ANSI syntax (sorry).
 */
quad_t
__fixsfdi(float x) {
	if (x < 0)
		if (x <= QUAD_MIN) {
			return (QUAD_MIN);
		} else {
			return ((quad_t) - (u_quad_t) - x);
		}
	else if (x >= QUAD_MAX) {
		return (QUAD_MAX);
	} else {
		return ((quad_t)(u_quad_t)x);
	}
}
