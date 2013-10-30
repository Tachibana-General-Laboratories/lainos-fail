#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <user.h>
#include <sys/param.h>

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef ptrdiff_t Align;

union header {
	struct {
		union header *ptr;
		size_t size;
	} s;
	Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

static Header *morecore(size_t nu) {
	char *p;
	Header *hp;

	if (nu < 4096) {
		nu = 4096;
	}

	p = sbrk(nu * sizeof(Header));

	if (p == (char *) - 1) {
		return 0;
	}

	hp = (Header *)p;
	hp->s.size = nu;
	free((void *)(hp + 1));
	return freep;
}

void *malloc(size_t nbytes) {
	Header *p, *prevp;
	size_t nunits;

	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;

	if ((prevp = freep) == 0) {
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}

	for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
		if (p->s.size >= nunits) {
			if (p->s.size == nunits) {
				prevp->s.ptr = p->s.ptr;
			} else {
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}

			freep = prevp;
			return (void *)(p + 1);
		}

		if (p == freep)
			if ((p = morecore(nunits)) == 0) {
				return 0;
			}
	}
}

void *calloc(size_t nmemb, size_t size) {
	void *rc = malloc(nmemb * size);

	if (rc != NULL) {
		memset(rc, 0, nmemb * size);
	}

	return rc;
}

void free(void *ap) {
	Header *bp, *p;

	bp = (Header *)ap - 1;

	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
		if (p >= p->s.ptr && (bp > p || bp < p->s.ptr)) {
			break;
		}

	if (bp + bp->s.size == p->s.ptr) {
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else {
		bp->s.ptr = p->s.ptr;
	}

	if (p + p->s.size == bp) {
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else {
		p->s.ptr = bp;
	}

	freep = p;
}

void *realloc(void *ptr, size_t size) {
	Header *bp;

	bp = (Header *)ptr - 1;

	void *ret_ptr = malloc(size);
	// FIXME: доделать (не уверен в правильности s.size)
	// по идее нужно всё циклом обходить
	ret_ptr = memmove(ret_ptr, ptr, (size < bp->s.size) ? size : bp->s.size);

	free(ptr);

	return ret_ptr;
}

