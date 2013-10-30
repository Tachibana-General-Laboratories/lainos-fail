CFLAGS +=  -I ../include -I ../include/libc

SOURCES_QUAD = $(wildcard $(addsuffix /*.c, quad ))
QUAD = $(patsubst %.c, %.o, $(SOURCES_QUAD) )

LIBC = \
	libc/errno.o \
	libc/rand.o \
	libc/ctype.o \
	libc/search.o \
	libc/malloc.o \
	libc/stdlib.o \
	libc/string.o \

all: quad.a libc.a

quad.a: $(QUAD)
	$(AR) rc $@ $^
	$(RANLIB) $@ 

libc.a: $(QUAD) $(LIBC)
	$(AR) rc $@ $^
	$(RANLIB) $@ 

clean:
	rm -rf $(QUAD) quad.a
	rm -rf $(LIBC) libc.a

