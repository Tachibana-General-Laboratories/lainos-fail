# Эти переменные используются при сборке
export CC = $(TOOLPREFIX)gcc
export AS = $(TOOLPREFIX)gas
export LD = $(TOOLPREFIX)ld
export OBJCOPY = $(TOOLPREFIX)objcopy
export OBJDUMP = $(TOOLPREFIX)objdump

export ASFLAGS = -m32 -gdwarf-2 -Wa,-divide
export CFLAGS = \
	-std=gnu99 -ffreestanding -ggdb3 -m32 -nostdlib \
	-nostdinc -mno-red-zone -mno-mmx \
	-mno-sse -mno-sse2 -mno-sse3 -mno-3dnow -ggdb3 \
	-Wall -Wextra -Wmissing-include-dirs -Wshadow \
	-Wwrite-strings -Wredundant-decls -Wnested-externs \
	-Winline
export LDFLAGS = -nostdlib -z nodefaultlib -z max-page-size=0x1000
export LDLIBS =

ifndef QEMU
QEMU := qemu
endif

# Некий хак. Пусть будет.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
# FreeBSD ld wants ``elf_i386_fbsd''
LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)

# Опции форматирования кода
ASTYLEFLAGS := --style=attach --indent=force-tab --break-blocks --align-pointer=name --add-brackets --unpad-paren --pad-oper --pad-header

XV6_IMG = xv6.img
FS_IMG = fs.img
FS_PATH = fs
#XV6MEMFS_IMG = xv6memfs.img

.PHONY: all clean format apps kernel

all: $(XV6_IMG)

clean:
	$(MAKE) -C apps clean
	$(MAKE) -C kernel clean
	rm -f .gdbinit mkfs $(XV6_IMG) $(FS_IMG)
	rm -rf $(FS_PATH)/bin/* $(FS_PATH)/init $(FS_PATH)/sh

apps:
	$(MAKE) -C apps

kernel: kernel/bootblock kernel/kernel

kernel/bootblock:
	$(MAKE) -C kernel bootblock
kernel/kernel:
	$(MAKE) -C kernel kernel

# Утилита, которая делает fs.img
mkfs: mkfs.c include/fs.h
	gcc -Wall -o mkfs mkfs.c

UPROGS = $(wildcard apps/_*)
# подготавливает fs/bin
fs_bin: apps
	rm -rf $(FS_PATH)/bin/*
	cp -f -t $(FS_PATH)/bin $(UPROGS)
	cd $(FS_PATH)/bin && rename 's/_//' *
	mv -f $(FS_PATH)/bin/init $(FS_PATH)
	mv -f $(FS_PATH)/bin/sh $(FS_PATH)

$(FS_IMG): mkfs fs_bin
	./mkfs $(FS_IMG) $(FS_PATH)

# img с ядром внутри
$(XV6_IMG): kernel
	dd if=/dev/zero of=$(XV6_IMG) count=10000
	dd if=kernel/bootblock of=$(XV6_IMG) conv=notrunc
	dd if=kernel/kernel of=$(XV6_IMG) seek=1 conv=notrunc

# Для файловой системы в памяти
# Не работает в данынй момент
#$(XV6MEMFS_IMG): xv6/bootblock xv6/kernelmemfs
#	dd if=/dev/zero of=$(XV6MEMFS_IMG) count=10000
#	dd if=xv6/bootblock of=$(XV6MEMFS_IMG) conv=notrunc
#	dd if=xv6/kernelmemfs of=$(XV6MEMFS_IMG) seek=1 conv=notrunc

# Форматирует весь код
format:
	@echo "Astyle code formatter"
	astyle $(ASTYLEFLAGS) -r "*.c"
	astyle $(ASTYLEFLAGS) -r "*.h"
	find -name "*.orig" -delete

# Далее идут эмуляторы

bochs : $(FS_IMG) $(XV6_IMG)
	if [ ! -e .bochsrc ]; then ln -s dot-bochsrc .bochsrc; fi
	bochs -q

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)
ifndef CPUS
CPUS := 2
endif
QEMUOPTS = -hdb $(FS_IMG) $(XV6_IMG) -smp $(CPUS) -m 512 $(QEMUEXTRA)

qemu: $(FS_IMG) $(XV6_IMG)
	$(QEMU) -serial mon:stdio $(QEMUOPTS)

# qemu без графики
qemu-nox: $(FS_IMG) $(XV6_IMG)
	$(QEMU) -nographic $(QEMUOPTS)

# Для файловой системы в памяти
#qemu-memfs: $(XV6MEMFS_IMG)
#	$(QEMU) $(XV6MEMFS_IMG) -smp $(CPUS)

.gdbinit: gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

# qemu с отладкой
qemu-gdb: $(FS_IMG) $(XV6_IMG) .gdbinit
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

# qemu с отладкой без графики
qemu-nox-gdb: $(FS_IMG) $(XV6_IMG) .gdbinit
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)


