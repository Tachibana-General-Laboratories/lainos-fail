#include "types.h"
#include "defs.h"
#include "param.h"
#include "traps.h"
#include "spinlock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

#define BACKSPACE 0x100
#define CRTPORT 0x3d4

#define DEV_CGA 2

struct {
	struct spinlock lock;
	uint size;
} cga;

static ushort *crt = (ushort *)P2V(0xb8000); // CGA memory

void
cgaputc(int c) {
	int pos;

	// Cursor position: col + 80*row.
	outb(CRTPORT, 14);
	pos = inb(CRTPORT + 1) << 8;
	outb(CRTPORT, 15);
	pos |= inb(CRTPORT + 1);

	if (c == '\n') {
		pos += 80 - pos % 80;
	} else if (c == BACKSPACE) {
		if (pos > 0) {
			--pos;
		}
	} else {
		crt[pos++] = (c & 0xff) | 0x0700;    // black on white
	}

	if ((pos / 80) >= 24) { // Scroll up.
		memmove(crt, crt + 80, sizeof(crt[0]) * 23 * 80);
		pos -= 80;
		memset(crt + pos, 0, sizeof(crt[0]) * (24 * 80 - pos));
	}

	outb(CRTPORT, 14);
	outb(CRTPORT + 1, pos >> 8);
	outb(CRTPORT, 15);
	outb(CRTPORT + 1, pos);
	crt[pos] = ' ' | 0x0700;
}

int
cgaread(struct inode *ip, char *dst, int off, int n) {
	int size = (n < cga.size)? n: cga.size;

	iunlock(ip);
	acquire(&cga.lock);

	memcpy(dst, crt+off, size);

	release(&cga.lock);
	ilock(ip);

	return size;
}

int
cgawrite(struct inode *ip, char *buf, int off, int n) {
	int size = (n < cga.size)? n: cga.size;

	iunlock(ip);
	acquire(&cga.lock);

	memcpy(crt+off, buf, size);

	release(&cga.lock);
	ilock(ip);

	return size;
}


void
cgainit(void) {
	cga.size = 80*25;
	//cga.crt = (char *)P2V(0xb8000); // CGA memory

	initlock(&cga.lock, "cga");

	devsw[DEV_CGA].write = cgawrite;
	devsw[DEV_CGA].read = cgaread;
}

