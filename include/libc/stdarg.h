#ifndef STDARG_H
#define STDARG_H

#define va_start(args, firstarg)    __builtin_va_start(args, firstarg)
#define va_arg(args, type)          __builtin_va_arg(args, type)
#define va_copy(args2, args1)       __builtin_va_copy(args2, args1)
#define va_end(args)                __builtin_va_end(args)

typedef __builtin_va_list va_list;

#endif

