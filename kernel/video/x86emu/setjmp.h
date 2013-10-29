#ifndef SETJMP_H
#define SETJMP_H SETJMP_H

#define	_JBLEN	10

typedef long jmp_buf[_JBLEN];

int setjmp (jmp_buf);
void longjmp (jmp_buf, int);

#endif
