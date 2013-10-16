#include "quad.h"

/*
 * Convert (unsigned) quad to double.
 * This is exactly like floatdidf.c except that negatives never occur.
 */
double
__floatunsdidf(u_quad_t x)
{
	double d;
	union uu u;

	u.uq = x;
	d = (double)u.ul[H] * (((int)1 << (INT_BITS - 2)) * 4.0);
	d += u.ul[L];
	return (d);
}
