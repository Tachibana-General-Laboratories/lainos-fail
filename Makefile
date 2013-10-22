###
# Далее идут опции
###

# Эти переменные используются при сборке
export CC = $(TOOLPREFIX)gcc
export AS = $(TOOLPREFIX)gas
export LD = $(TOOLPREFIX)ld
export RANLIB = $(TOOLPREFIX)ranlib
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

# Опции эмуляции
ifndef QEMU
QEMU := qemu-system-i386
endif
ifndef CPUS
CPUS := 2
endif
ifndef QEMUEXTRA
QEMUEXTRA := -m 512
endif

# Опции форматирования кода
ASTYLEFLAGS = --style=attach --indent=force-tab \
	--break-blocks --align-pointer=name --add-brackets \
	--unpad-paren --pad-oper --pad-header

KERNEL_IMG = kernel.img
FS_IMG = fs.img
FS_PATH = fs
#XV6MEMFS_IMG = xv6memfs.img

###
# Далее идут цели компиляции
###

.PHONY: all img clean format apps kernel fs_bin libc mkfs

# Главная цель
all: img
# Очистка
clean:
	$(MAKE) -C apps clean
	$(MAKE) -C kernel clean
	$(MAKE) -C libc clean
	$(MAKE) -C mkfs clean
	rm -f .gdbinit mkfs $(KERNEL_IMG) $(FS_IMG)
	rm -rf $(FS_PATH)/bin/* $(FS_PATH)/init $(FS_PATH)/sh
# Форматирует весь код
format:
	@echo "Astyle code formatter"
	astyle $(ASTYLEFLAGS) -r "*.c"
	astyle $(ASTYLEFLAGS) -r "*.h"
	find -name "*.orig" -delete
# Сборка *.img
img: $(KERNEL_IMG) $(FS_IMG)

# Ядро
kernel:
	$(MAKE) -C kernel bootblock
	$(MAKE) -C kernel kernel
# img с ядром внутри
$(KERNEL_IMG): kernel
	dd if=/dev/zero of=$(KERNEL_IMG) count=10000
	dd if=kernel/bootblock of=$(KERNEL_IMG) conv=notrunc
	dd if=kernel/kernel of=$(KERNEL_IMG) seek=1 conv=notrunc

# Приложения
apps: libc
	$(MAKE) -C apps
# Библиотека C
libc:
	$(MAKE) -C libc
# Утилита, которая делает fs.img
mkfs:
	$(MAKE) -C mkfs

# Файловая система
$(FS_IMG): mkfs fs_bin
	mkfs/mkfs $(FS_IMG) $(FS_PATH)
# подготавливает fs/bin
fs_bin: apps
	rm -rf $(FS_PATH)/bin/*
	cp -f -t $(FS_PATH)/bin $(wildcard apps/_*)
	cd $(FS_PATH)/bin && rename 's/_//' *
	mv -f $(FS_PATH)/bin/init $(FS_PATH)
	mv -f $(FS_PATH)/bin/sh $(FS_PATH)

# Для файловой системы в памяти
# Не работает в данынй момент
#$(XV6MEMFS_IMG): xv6/bootblock xv6/kernelmemfs
#	dd if=/dev/zero of=$(XV6MEMFS_IMG) count=10000
#	dd if=xv6/bootblock of=$(XV6MEMFS_IMG) conv=notrunc
#	dd if=xv6/kernelmemfs of=$(XV6MEMFS_IMG) seek=1 conv=notrunc

###
# Далее идут эмуляторы
###

# Пока не робит
#bochs: img
#	if [ ! -e .bochsrc ]; then ln -s dot-bochsrc .bochsrc; fi
#	bochs -q

# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

QEMUOPTS = -hdb $(FS_IMG) $(KERNEL_IMG) -smp $(CPUS) $(QEMUEXTRA)

qemu: img
	$(QEMU) -serial mon:stdio $(QEMUOPTS)

# qemu без графики
qemu-nox: img
	$(QEMU) -nographic $(QEMUOPTS)

# Для файловой системы в памяти
#qemu-memfs: $(XV6MEMFS_IMG)
#	$(QEMU) $(XV6MEMFS_IMG) -smp $(CPUS)

.gdbinit: gdbinit.tmpl
	sed "s/localhost:1234/localhost:$(GDBPORT)/" < $^ > $@

# qemu с отладкой
qemu-gdb: img .gdbinit
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -serial mon:stdio $(QEMUOPTS) -S $(QEMUGDB)

# qemu с отладкой без графики
qemu-nox-gdb: img .gdbinit
	@echo "*** Now run 'gdb'." 1>&2
	$(QEMU) -nographic $(QEMUOPTS) -S $(QEMUGDB)

