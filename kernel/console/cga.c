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
	char *crt; // CGA memory
} cga;

int
cgaread(struct inode *ip, char *dst, int off, int n) {
	int size = (n < cga.size)? n: cga.size;

	iunlock(ip);
	acquire(&cga.lock);

	memcpy(dst, cga.crt+off, size);

	release(&cga.lock);
	ilock(ip);

	return size;
}

int
cgawrite(struct inode *ip, char *buf, int off, int n) {
	int size = (n < cga.size)? n: cga.size;

	iunlock(ip);
	acquire(&cga.lock);

	memcpy(cga.crt+off, buf, size);

	release(&cga.lock);
	ilock(ip);

	return size;
}


void
cgainit(void) {
	cga.size = 80*25;
	cga.crt = (char *)P2V(0xb8000); // CGA memory

	initlock(&cga.lock, "cga");

	devsw[DEV_CGA].write = cgawrite;
	devsw[DEV_CGA].read = cgaread;
}

