// convert to intel byte order
ushort
xshort(ushort x) {
	ushort y;
	uchar *a = (uchar *)&y;
	a[0] = x;
	a[1] = x >> 8;
	return y;
}

uint
xint(uint x) {
	uint y;
	uchar *a = (uchar *)&y;
	a[0] = x;
	a[1] = x >> 8;
	a[2] = x >> 16;
	a[3] = x >> 24;
	return y;
}

void
wsect(uint sec, void *buf) {
	if (lseek(fsfd, sec * 512L, 0) != sec * 512L) {
		perror("lseek");
		exit(1);
	}

	if (write(fsfd, buf, 512) != 512) {
		perror("write");
		exit(1);
	}
}

uint
i2b(uint inum) {
	return (inum / IPB) + 2;
}

void
winode(uint inum, struct dinode *ip) {
	char buf[512];
	uint bn;
	struct dinode *dip;

	bn = i2b(inum);
	rsect(bn, buf);
	dip = ((struct dinode *)buf) + (inum % IPB);
	*dip = *ip;
	wsect(bn, buf);
}

void
rinode(uint inum, struct dinode *ip) {
	char buf[512];
	uint bn;
	struct dinode *dip;

	bn = i2b(inum);
	rsect(bn, buf);
	dip = ((struct dinode *)buf) + (inum % IPB);
	*ip = *dip;
}

void
rsect(uint sec, void *buf) {
	if (lseek(fsfd, sec * 512L, 0) != sec * 512L) {
		perror("lseek");
		exit(1);
	}

	if (read(fsfd, buf, 512) != 512) {
		perror("read");
		exit(1);
	}
}

uint
ialloc(ushort type) {
	uint inum = freeinode++;
	struct dinode din;

	memset(&din, 0, sizeof(din));
	din.type = xshort(type);
	din.nlink = xshort(1);
	din.size = xint(0);
	winode(inum, &din);
	return inum;
}

void
balloc(int used) {
	uchar buf[512];
	int i;

	printf("balloc: first %d blocks have been allocated\n", used);
	assert(used < 512 * 8);
	memset(buf, 0, 512);

	for (i = 0; i < used; i++) {
		buf[i / 8] = buf[i / 8] | (0x1 << (i % 8));
	}

	printf("balloc: write bitmap block at sector %zu\n", ninodes / IPB + 3);
	wsect(ninodes / IPB + 3, buf);
}

#define min(a, b) ((a) < (b) ? (a) : (b))

void
iappend(uint inum, void *xp, int n) {
	char *p = (char *)xp;
	uint fbn, off, n1;
	struct dinode din;
	char buf[512];
	uint indirect[NINDIRECT];
	uint x;

	rinode(inum, &din);

	off = xint(din.size);

	while (n > 0) {
		fbn = off / 512;
		assert(fbn < MAXFILE);

		if (fbn < NDIRECT) {
			if (xint(din.addrs[fbn]) == 0) {
				din.addrs[fbn] = xint(freeblock++);
				usedblocks++;
			}

			x = xint(din.addrs[fbn]);
		} else {
			if (xint(din.addrs[NDIRECT]) == 0) {
				// printf("allocate indirect block\n");
				din.addrs[NDIRECT] = xint(freeblock++);
				usedblocks++;
			}

			// printf("read indirect block\n");
			rsect(xint(din.addrs[NDIRECT]), (char *)indirect);

			if (indirect[fbn - NDIRECT] == 0) {
				indirect[fbn - NDIRECT] = xint(freeblock++);
				usedblocks++;
				wsect(xint(din.addrs[NDIRECT]), (char *)indirect);
			}

			x = xint(indirect[fbn - NDIRECT]);
		}

		n1 = min(n, (fbn + 1) * 512 - off);
		rsect(x, buf);
		memcpy(buf + off - (fbn * 512), p, n1);
		wsect(x, buf);
		n -= n1;
		off += n1;
		p += n1;
	}

	din.size = xint(off);
	winode(inum, &din);
}
