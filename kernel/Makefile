CFLAGS += -I ../include/sys
ASFLAGS += -I ../include/sys

all: bootblock kernel
OBJS = \
	console/console.o\
	console/cga.o\
	console/kbd.o\
	console/uart.o\
	syscall/syscall.o\
	syscall/sysfile.o\
	syscall/sysproc.o\
	fs/bio.o\
	fs/file.o\
	fs/fs.o\
	fs/pipe.o\
	video/vbe.o\
	video/x86emu/setjmp.o\
	video/x86emu/x86emu.o\
	video/x86emu/x86emu_util.o\
	video/x86emu/divdi3.o\
	video/x86emu/moddi3.o\
	video/x86emu/udivdi3.o\
	video/x86emu/umoddi3.o\
	video/x86emu/qdivrem.o\
	exec.o\
	ide.o\
	ioapic.o\
	kalloc.o\
	lapic.o\
	log.o\
	main.o\
	mp.o\
	picirq.o\
	proc.o\
	spinlock.o\
	string.o\
	swtch.o\
	timer.o\
	trapasm.o\
	trap.o\
	vectors.o\
	vm.o\

# Бутлоадер
bootblock: bootasm.S bootmain.c
	$(CC) $(CFLAGS) -fno-pic -O -nostdinc -I. -c bootmain.c
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I. -c bootasm.S
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o bootblock.o bootasm.o bootmain.o
	#$(OBJDUMP) -S bootblock.o > bootblock.asm
	$(OBJCOPY) -S -O binary -j .text bootblock.o bootblock
	./sign.pl bootblock

entryother: entryother.S
	$(CC) $(CFLAGS) -fno-pic -nostdinc -I. -c entryother.S
	$(LD) $(LDFLAGS) -N -e start -Ttext 0x7000 -o bootblockother.o entryother.o
	$(OBJCOPY) -S -O binary -j .text bootblockother.o entryother
	#$(OBJDUMP) -S bootblockother.o > entryother.asm

initcode: initcode.S
	$(CC) $(CFLAGS) -nostdinc -I. -c initcode.S
	$(LD) $(LDFLAGS) -N -e start -Ttext 0 -o initcode.out initcode.o
	$(OBJCOPY) -S -O binary initcode.out initcode
	#$(OBJDUMP) -S initcode.o > initcode.asm

# Ядро
kernel: $(OBJS) entry.o entryother initcode kernel.ld
	$(LD) $(LDFLAGS) -T kernel.ld -o kernel entry.o $(OBJS) -b binary initcode entryother
	#$(OBJDUMP) -S kernel > kernel.asm
	$(OBJDUMP) -t kernel | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernel.sym


# kernelmemfs is a copy of kernel that maintains the
# disk image in memory instead of writing to a disk.
# This is not so useful for testing persistent storage or
# exploring disk buffering implementations, but it is
# great for testing the kernel on real hardware without
# needing a scratch disk.

# Для файловой системы в памяти
#MEMFSOBJS = $(filter-out ide.o,$(OBJS)) memide.o
#kernelmemfs: $(MEMFSOBJS) entry.o entryother initcode $(FS_IMG)
#	$(LD) $(LDFLAGS) -Ttext 0x100000 -e main -o kernelmemfs entry.o  $(MEMFSOBJS) -b binary initcode entryother $(FS_IMG)
#	$(OBJDUMP) -S kernelmemfs > kernelmemfs.asm
#	$(OBJDUMP) -t kernelmemfs | sed '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > kernelmemfs.sym

tags: $(OBJS) entryother.S _init
	etags *.S *.c

vectors.S: vectors.pl
	perl vectors.pl > vectors.S

-include *.d

clean: 
	rm -f $(OBJS) *.o *.d *.asm *.sym vectors.S bootblock entryother \
	initcode initcode.out kernel kernelmemfs

