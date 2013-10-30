#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0
typedef unsigned int size_t;

typedef unsigned long ulong;
typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef unsigned long long uint64_t;
typedef unsigned long long u_int64_t;
typedef unsigned long uint32_t;
typedef unsigned long u_int32_t;
typedef unsigned short uint16_t;
typedef unsigned short u_int16_t;
typedef unsigned char uint8_t;
typedef unsigned char u_int8_t;

typedef  long __int32_t;
typedef unsigned long __uint32_t;

typedef unsigned long long u_quad_t;    /* quads */
typedef long long quad_t;
typedef quad_t * qaddr_t;
//typedef unsigned long u_int;
//typedef unsigned long uint;

typedef long long int64_t;
typedef long int32_t;
typedef short int16_t;
typedef char int8_t;


#define _QUAD_HIGHWORD 1
#define _QUAD_LOWWORD 0

#define __BEGIN_DECLS
#define __END_DECLS
#define __dead
#define __far

#define __HI(x) *(1+(int*)&x)
#define __LO(x) *(int*)&x

#define __P(a) a
#define CHAR_BIT        8       	/* max # of bits in a "char" */

#define EXTRACT_WORDS(i0, i1, x) \
    i0 =  __HI(x); \
    i1 =  __LO(x);

#define INSERT_WORDS(x, i0, i1) \
    __HI(x) = i0; \
    __LO(x) = i1;

#define _BEGIN_STD_C
#define _END_STD_C

#define _EXFUN(a,b) a b

#endif
