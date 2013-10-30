#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <user.h>
#include <sys/x86.h>


char *
gets(char *buf, int max) {
	int i, cc;
	char c;

	for (i = 0; i + 1 < max;) {
		cc = read(0, &c, 1);

		if (cc < 1) {
			break;
		}

		buf[i++] = c;

		if (c == '\n' || c == '\r') {
			break;
		}
	}

	buf[i] = '\0';
	return buf;
}

int
stat(char *n, struct stat *st) {
	int fd;
	int r;

	fd = open(n, O_RDONLY);

	if (fd < 0) {
		return -1;
	}

	r = fstat(fd, st);
	close(fd);
	return r;
}

static char **last_environ;

char *getenv(const char *name) {
	const size_t len = strlen(name);
	char **ep;

	if (environ != NULL)
		for (ep = environ; *ep; ep++)
			if (!strncmp(*ep, name, len) && (*ep)[len] == '=') {
				return &(*ep)[len + 1];
			}

	return 0;
}

int setenv(const char *name, const char *value, int overwrite) {
	register char **ep = 0;
	register size_t size;
	const size_t namelen = strlen(name);
	const size_t vallen = strlen(value) + 1;

	size = 0;

	if (environ != NULL) {
		for (ep = environ; *ep != NULL; ++ep)
			if (!strncmp(*ep, name, namelen) && (*ep)[namelen] == '=') {
				break;
			} else {
				++size;
			}
	}

	if (environ == NULL || *ep == NULL) {
		char **new_environ;

		if (environ == last_environ && environ != NULL)
			// We allocated this space; we can extend it.
		{
			new_environ = (char **) realloc(last_environ, (size + 2) * sizeof(char *));
		} else {
			new_environ = (char **) malloc((size + 2) * sizeof(char *));
		}

		if (new_environ == NULL) {
			return -1;
		}

		new_environ[size] = (char *) malloc(namelen + 1 + vallen);

		if (new_environ[size] == NULL) {
			free((char *) new_environ);
			return -1;
		}

		if (environ != last_environ) {
			memcpy((char *) new_environ, (char *) environ, size * sizeof(char *));
		}

		memcpy(new_environ[size], name, namelen);
		new_environ[size][namelen] = '=';
		memcpy(&new_environ[size][namelen + 1], value, vallen);

		new_environ[size + 1] = NULL;

		last_environ = environ = new_environ;
	} else if (overwrite) {
		size_t len = strlen(*ep);

		if (len + 1 < namelen + 1 + vallen) {
			// The existing string is too short; malloc a new one.
			char *new_string = (char *) malloc(namelen + 1 + vallen);

			if (new_string == NULL) {
				return -1;
			}

			*ep = new_string;
		}

		memcpy(*ep, name, namelen);
		(*ep)[namelen] = '=';
		memcpy(&(*ep)[namelen + 1], value, vallen);
	}

	return 0;
}

int unsetenv(const char *name) {
	const int len = strlen(name);
	char **ep;

	for (ep = environ; *ep; ++ep)
		if (!strncmp(*ep, name, len) && (*ep)[len] == '=') {
			// Found it.  Remove this pointer by moving later ones back.
			char **dp = ep;

			do {
				dp[0] = dp[1];
			} while (*dp++);

			// Continue the loop in case NAME appears again.
		}
}

// FIXME: fail set. Why?
#define alloca(size)   __builtin_alloca (size)
int putenv(char *string) {
	return -1;// ну чтоб не портил ничего
	char *name_end = strchr(string, '=');

	if (name_end) {
		char *name = (char *) alloca(name_end - string + 1);
		memcpy(name, string, name_end - string);
		name[name_end - string] = '\0';
		return setenv(name, name_end + 1, 1);
	}

	unsetenv(string);
	return 0;
}

