CFLAGS +=  -I ../include -I ../include/libc

OBJS = \
	   rand.o \
	   ctype.o \
	   search.o \
	   malloc.o \
	   stdlib.o \
	   string.o \

SOURCES_QUAD = $(wildcard $(addsuffix /*.c, quad ))
QUAD = $(patsubst %.c, %.o, $(SOURCES_QUAD) )

all: libc.a

libc.a: $(OBJS) $(QUAD) tss_errno/errno.o
	$(AR) rc $@ $^
	$(RANLIB) $@ 

clean:
	rm -rf $(OBJS) $(QUAD) tss_errno/errno.o libc.a

