#ifndef STDDEF_H
#define STDDEF_H

typedef long long          ptrdiff_t;
typedef unsigned long long size_t;
typedef unsigned int       wchar_t;

#define NULL 0
#define offsetof(type, member) ((size_t) &(((type *) 0)->member))

#endif

