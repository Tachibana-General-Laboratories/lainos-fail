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

# Некий хак. Пусть будет.
CFLAGS += $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
# FreeBSD ld wants ``elf_i386_fbsd''
LDFLAGS += -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)

# Опции форматирования кода
ASTYLEFLAGS := --style=attach --indent=force-tab --break-blocks --align-pointer=name --add-brackets --unpad-paren --pad-oper --pad-header

XV6_IMG = build/xv6.img
FS_IMG = build/fs.img
XV6MEMFS_IMG = build/xv6memfs.img

.PHONY: all clean format apps

all: 

clean:
	$(MAKE) -C apps clean
	$(MAKE) -C xv6 clean

kernel: xv6/bootblock xv6/kernel
	$(MAKE) -C xv6

xv6/bootblock:
	$(MAKE) -C xv6 bootblock
xv6/kernel:
	$(MAKE) -C xv6 kernel

# Утилита, которая делает fs.img
mkfs: mkfs.c include/fs.h
	gcc -Werror -Wall -o mkfs mkfs.c

UPROGS = $(notdir $(shell find -name "_*"))
apps:
	$(MAKE) -C apps


$(FS_IMG): mkfs apps
	cd apps && ../mkfs ../$(FS_IMG) $(UPROGS)

$(XV6_IMG): xv6/bootblock xv6/kernel $(FS_IMG)
	dd if=/dev/zero of=$(XV6_IMG) count=10000
	dd if=xv6/bootblock of=$(XV6_IMG) conv=notrunc
	dd if=xv6/kernel of=$(XV6_IMG) seek=1 conv=notrunc

$(XV6MEMFS_IMG): xv6/bootblock xv6/kernelmemfs
	dd if=/dev/zero of=$(XV6MEMFS_IMG) count=10000
	dd if=xv6/bootblock of=$(XV6MEMFS_IMG) conv=notrunc
	dd if=xv6/kernelmemfs of=$(XV6MEMFS_IMG) seek=1 conv=notrunc

# Форматирует весь код
format:
	@echo "Astyle code formatter"
	astyle $(ASTYLEFLAGS) -r "*.c"
	astyle $(ASTYLEFLAGS) -r "*.h"
	find -name "*.orig" -delete

