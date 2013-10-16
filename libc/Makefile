CFLAGS += -I ../include/libc

OBJS = string.o

all: libc.a

libc.a: $(OBJS)
	$(AR) -rv $@ $^
	$(RANLIB) $@ 

clean:
	rm -rf $(OBJS) libc.a

