#include <stddef.h>
#include <string.h>
#include <stdlib.h>

extern char **environ;

struct stat;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int *);
int write(int, void *, int);
int read(int, void *, int);
int close(int);
int kill(int);

int execve(const char *, char **, char **);
#define exec(pa, arg) execve(pa,arg,environ)

int open(const char *, int);
int mknod(const char *, short, short);
int unlink(const char *);
int fstat(int fd, struct stat *);
int link(const char *, const char *);
int mkdir(const char *);
int chdir(const char *);
int dup(int);
int getpid(void);
char *sbrk(int);
int sleep(int);
int uptime(void);

// ulib.c
int stat(char *, struct stat *);
void printf(int, const char *, ...);
char *gets(char *, int max);

char *getenv(const char *name);
int putenv(char *string);
int setenv(const char *name, const char *value, int overwrite);
int unsetenv(const char *name);

