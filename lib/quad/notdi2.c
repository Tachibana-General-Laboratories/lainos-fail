#include "quad.h"

/*
 * Return ~a.  For some reason gcc calls this `one's complement' rather
 * than `not'.
 */
quad_t
__one_cmpldi2(quad_t a) {
	union uu aa;

	aa.q = a;
	aa.ul[0] = ~aa.ul[0];
	aa.ul[1] = ~aa.ul[1];
	return (aa.q);
}
