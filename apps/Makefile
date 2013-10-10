CFLAGS += -I ../include
ASFLAGS += -I ../include

BUILD_DIR = ../build/apps

# Объявляем те проги, которые нам нужны
UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_stressfs\
	_usertests\
	_wc\
	_zombie\

ULIB = lib/ulib.o lib/usys.o lib/printf.o lib/umalloc.o

# Далее идёт магия
_%: %.o $(ULIB)
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o $@ $^
	$(OBJDUMP) -S $@ > $*.asm
	$(OBJDUMP) -t $@ | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $*.sym

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
	$(LD) $(LDFLAGS) -N -e main -Ttext 0 -o _forktest forktest.o lib/ulib.o lib/usys.o
	$(OBJDUMP) -S _forktest > forktest.asm

clean: 
	rm -f *.o *.d *.asm *.sym $(UPROGS)

