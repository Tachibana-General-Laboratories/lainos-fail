#include <string.h>

void *memchr(const void *s, int c, size_t n) {
	const unsigned char *p = (const unsigned char *) s;

	while (n--) {
		if (*p == (unsigned char) c) {
			return (void *) p;
		}

		++p;
	}

	return NULL;
}

int memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *p1 = (const unsigned char *) s1;
	const unsigned char *p2 = (const unsigned char *) s2;

	while (n--) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}

		++p1;
		++p2;
	}

	return 0;
}

void *memcpy(void *restrict s1, const void *restrict s2, size_t n) {
	char *dest = (char *) s1;
	const char *src = (const char *) s2;

	while (n--) {
		*dest++ = *src++;
	}

	return s1;
}

void *memmove(void *s1, const void *s2, size_t n) {
	char *dest = (char *) s1;
	const char *src = (const char *) s2;

	if (dest <= src) {
		while (n--) {
			*dest++ = *src++;
		}
	} else {
		src += n;
		dest += n;

		while (n--) {
			*--dest = *--src;
		}
	}

	return s1;
}

void *memset(void *s, int c, size_t n) {
	unsigned char *p = (unsigned char *) s;

	while (n--) {
		*p++ = (unsigned char) c;
	}

	return s;
}

char *strcat(char *restrict s1, const char *restrict s2) {
	char *rc = s1;

	if (*s1) {
		while (*++s1);
	}

	while ((*s1++ = *s2++));

	return rc;
}

char *strchr(const char *s, int c) {
	do {
		if (*s == (char) c) {
			return (char *) s;
		}
	} while (*s++);

	return NULL;
}

int strcmp(const char *s1, const char *s2) {
	while ((*s1) && (*s1 == *s2)) {
		++s1;
		++s2;
	}

	return (*(unsigned char *)s1 - * (unsigned char *)s2);
}

char *strcpy(char *restrict s1, const char *restrict s2) {
	char *rc = s1;

	while ((*s1++ = *s2++));

	return rc;
}

size_t strcspn(const char *s1, const char *s2) {
	size_t len = 0;
	const char *p;

	while (s1[len]) {
		p = s2;

		while (*p) {
			if (s1[len] == *p++) {
				return len;
			}
		}

		++len;
	}

	return len;
}

size_t strlen(const char *s) {
	size_t rc = 0;

	while (s[rc]) {
		++rc;
	}

	return rc;
}

char *strncat(char *restrict s1, const char *restrict s2, size_t n) {
	char *rc = s1;

	while (*s1) {
		++s1;
	}

	while (n && (*s1++ = *s2++)) {
		--n;
	}

	if (n == 0) {
		*s1 = '\0';
	}

	return rc;
}

int strncmp(const char *s1, const char *s2, size_t n) {
	while (*s1 && n && (*s1 == *s2)) {
		++s1;
		++s2;
		--n;
	}

	if (n == 0) {
		return 0;
	} else {
		return (*(unsigned char *)s1 - * (unsigned char *)s2);
	}
}

char *strncpy(char *restrict s1, const char *restrict s2, size_t n) {
	char *rc = s1;

	while ((n > 0) && (*s1++ = *s2++)) {
		/* Cannot do "n--" in the conditional as size_t is unsigned and we have
		   to check it again for >0 in the next loop below, so we must not risk
		   underflow.
		*/
		--n;
	}

	/* Checking against 1 as we missed the last --n in the loop above. */
	while (n-- > 1) {
		*s1++ = '\0';
	}

	return rc;
}

size_t strnlen(const char *s, size_t maxlen) {
	for (size_t len = 0; len != maxlen; len++) {
		if (s[len] == '\0') {
			return len;
		}
	}

	return maxlen;
}

char *strpbrk(const char *s1, const char *s2) {
	const char *p1 = s1;
	const char *p2;

	while (*p1) {
		p2 = s2;

		while (*p2) {
			if (*p1 == *p2++) {
				return (char *) p1;
			}
		}

		++p1;
	}

	return NULL;
}

char *strrchr(const char *s, int c) {
	size_t i = 0;

	while (s[i++]);

	do {
		if (s[--i] == (char) c) {
			return (char *) s + i;
		}
	} while (i);

	return NULL;
}

size_t strspn(const char *s1, const char *s2) {
	size_t len = 0;
	const char *p;

	while (s1[ len ]) {
		p = s2;

		while (*p) {
			if (s1[len] == *p) {
				break;
			}

			++p;
		}

		if (! *p) {
			return len;
		}

		++len;
	}

	return len;
}

char *strstr(const char *s1, const char *s2) {
	const char *p1 = s1;
	const char *p2;

	while (*s1) {
		p2 = s2;

		while (*p2 && (*p1 == *p2)) {
			++p1;
			++p2;
		}

		if (! *p2) {
			return (char *) s1;
		}

		++s1;
		p1 = s1;
	}

	return NULL;
}

char *strtok(char *restrict s1, const char *restrict s2) {
	static char *tmp = NULL;
	const char *p = s2;

	if (s1 != NULL) {
		/* new string */
		tmp = s1;
	} else {
		/* old string continued */
		if (tmp == NULL) {
			/* No old string, no new string, nothing to do */
			return NULL;
		}

		s1 = tmp;
	}

	/* skipping leading s2 characters */
	while (*p && *s1) {
		if (*s1 == *p) {
			/* found seperator; skip and start over */
			++s1;
			p = s2;
			continue;
		}

		++p;
	}

	if (! *s1) {
		/* no more to parse */
		return (tmp = NULL);
	}

	/* skipping non-s2 characters */
	tmp = s1;

	while (*tmp) {
		p = s2;

		while (*p) {
			if (*tmp == *p++) {
				/* found seperator; overwrite with '\0', position tmp, return */
				*tmp++ = '\0';
				return s1;
			}
		}

		++tmp;
	}

	/* parsed to end of string */
	tmp = NULL;
	return s1;
}

