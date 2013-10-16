#ifndef LIMITS_H
#define LIMITS_H LIMITS_H

/* TODO: Defined to 1 as multibyte characters are not supported yet. */
#define MB_LEN_MAX 1

#define LLONG_MAX  0x7fffffffffffffffLL
#define LLONG_MIN  (-0x7fffffffffffffffLL - 1LL)
#define ULLONG_MAX 0xffffffffffffffffULL

#define CHAR_BIT    8
#define CHAR_MAX    0ff
#define CHAR_MIN    0
#define SCHAR_MIN   (-0x7f - 1)
#define SCHAR_MAX   0x7f
#define UCHAR_MAX   0xff

#define SHRT_MAX      0x7fff
#define SHRT_MIN      (-0x7fff - 1)
#define USHRT_MAX     0xffff

#define INT_MAX   0x7fffffff
#define INT_MIN   (-0x7fffffff - 1)
#define UINT_MAX  0xffffffffU

#define LONG_MAX   INT_MAX
#define LONG_MIN   INT_MIN
#define ULONG_MAX  UINT_MAX

#endif

