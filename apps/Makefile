CFLAGS += -I ../include
ASFLAGS += -I ../include

CFLAGS += -I ../include/libc
ASFLAGS += -I ../include/libc

BUILD_DIR = ../build/apps

LDLIBS += ../lib/libc.a
LDFLAGS += -T lib/link.ld

# Объявляем те проги, которые нам нужны
UPROGS=\
	_cat\
	_echo\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_wc\
	_zombie\
	_libctests\
	_forktest\
	_stressfs\
	_usertests\
	_libctests\

ULIB = lib/_start.o lib/usys.o lib/ulib.o lib/printf.o

# Далее идёт магия
_%: %.o $(ULIB)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Prevent deletion of intermediate files, e.g. cat.o, after first build, so
# that disk image changes after first build are persistent until clean.  More
# details:
# http://www.gnu.org/software/make/manual/html_node/Chained-Rules.html
#.PRECIOUS: %.o

-include *.d

all: $(UPROGS)

_forktest: forktest.o $(ULIB)
	# forktest has less library code linked in - needs to be small
	# in order to be able to max out the proc table.
	$(LD) $(LDFLAGS) -o _forktest forktest.o lib/_start.o lib/ulib.o lib/usys.o $(LDLIBS)

clean: 
	rm -f *.o *.d *.asm *.sym $(UPROGS) $(ULIB)

