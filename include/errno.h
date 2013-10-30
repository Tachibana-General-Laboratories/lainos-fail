#ifndef ERRNO_H
#define ERRNO_H ERRNO_H

extern int *_errno_func(void);
#define errno (*_errno_func())

/* C errno values */
#define ERANGE 1
#define EDOM   2
#define EILSEQ 3

/* C++11/POSIX errno values */
/* ...take a DEEP breath... */
#define E2BIG 4
#define ECONNRESET 5
#define EISCONN 6
#define ENOENT 7
#define ENOTRECOVERABLE 8
#define EROFS 9
#define EACCES 10
#define EDEADLK 11
#define EISDIR 12
#define ENOEXEC 13
#define ENOTSOCK 14
#define ESPIPE 15
#define EADDRINUSE 16
#define EDESTADDRREQ 17
#define ELOOP 18
#define ENOLCK 19
#define ENOTSUPP 20
#define ESRCH 21
#define EADDRNOTAVAIL 22
#define EMFILE 23
#define ENOLINK 24
#define ENOTTY 25
#define ETIME 26
#define EAFNOSUPPORT 27
#define EEXIST 28
#define EMLINK 29
#define ENOMEM 30
#define ENXIO 31
#define ETIMEDOUT 32
#define EAGAIN 33
#define EFAULT 34
#define EMSGSIZE 35
#define ENOMSG 36
#define EOPNOTSUPP 37
#define ETXTBSY 38
#define EALREADY 39
#define EFBIG 40
#define ENAMETOOLONG 41
#define ENOPROTOOPT 42
#define EOVERFLOW 43
#define EWOULDBLOCK EAGAIN
#define EBADF 44
#define EHOSTUNREACH 45
#define ENETDOWN 46
#define ENOSPC 47
#define EOWNERDEAD 48
#define EXDEV 49
#define EBADMSG 50
#define EIDRM 51
#define ENETRESET 52
#define ENOSR 53
#define EPERM 54
#define EBUSY 55
#define ENETUNREACH 56
#define ENOSTR 57
#define EPIPE 58
#define ECANCELED 59
#define EINPROGRESS 60
#define ENFILE 61
#define ENOSYS 62
#define EPROTO 63
#define ECHILD 64
#define EINTR 65
#define ENOBUFS 66
#define ENOTCONN 67
#define EPROTONOSUPPORT 68
#define ECONNABORTED 69
#define EINVAL 70
#define ENODATA 71
#define ENOTDIR 72
#define EPROTOTYPE 73

#endif

