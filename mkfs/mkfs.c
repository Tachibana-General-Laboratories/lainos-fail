#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>

#define stat xv6_stat  // avoid clash with host struct stat
#define dirent xv6_dirent  // avoid clash with host struct stat
#include "include/sys/types.h"
#include "include/sys/fs.h"
#include "include/sys/stat.h"
#include "include/sys/param.h"
#undef stat
#undef dirent

//int nblocks = 985 + _add;
int nlog = LOGSIZE;
int ninodes = 200;
int size = 1024 * 64;

#define BLOCK_SIZE BSIZE

int fsfd;
struct superblock sb;
char zeroes[512];
uint freeblock;
uint usedblocks;
uint bitblocks;
uint freeinode = 1;
uint root_inode;

#include "fsutil.c"

void mkfs() {
	char buf[BLOCK_SIZE];
	int i;

	bitblocks = size / (512 * 8) + 1;
	usedblocks = ninodes / IPB + 3 + bitblocks;
	freeblock = usedblocks;

	int nblocks = size - nlog - freeblock;

	sb.size = xint(size);
	sb.nblocks = xint(nblocks); // so whole disk is size sectors
	sb.ninodes = xint(ninodes);
	sb.nlog = xint(nlog);

	printf(" used %d (bit %d ninode %zu) free %u log %u total %d of %d (nblocks %d)\n", usedblocks,
		   bitblocks, ninodes / IPB + 1, freeblock, nlog, nblocks + usedblocks + nlog, size, nblocks);

	assert(nblocks + usedblocks + nlog == size);

	for (i = 0; i < nblocks + usedblocks + nlog; i++) {
		wsect(i, zeroes);
	}

	memset(buf, 0, sizeof(buf));
	memmove(buf, &sb, sizeof(sb));
	wsect(1, buf);

	return 0;
}

int
add_dir(DIR *cur_dir, int cur_inode, int parent_inode) {
	int r;
	int child_inode;
	int cur_fd, child_fd;
	struct xv6_dirent de;
	struct dinode din;
	struct dirent dir_buf;
	struct dirent *entry;
	struct stat st;
	int bytes_read;
	char buf[BLOCK_SIZE];
	int off;

	memset(&de, 0, sizeof(de));
	de.inum = xshort(cur_inode);
	strcpy(de.name, ".");
	iappend(cur_inode, &de, sizeof(de));

	memset(&de, 0, sizeof(de));
	de.inum = xshort(parent_inode);
	strcpy(de.name, "..");
	iappend(cur_inode, &de, sizeof(de));

	if (cur_dir == NULL) {
		return 0;
	}

	cur_fd = dirfd(cur_dir);

	if (cur_fd == -1) {
		perror("add_dir");
		exit(EXIT_FAILURE);
	}

	if (fchdir(cur_fd) != 0) {
		perror("add_dir");
		return -1;
	}

	while (true) {
		r = readdir_r(cur_dir, &dir_buf, &entry);

		if (r != 0) {
			perror("add_dir");
			return -1;
		}

		if (entry == NULL) {
			break;
		}

		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		if (entry->d_name[0] == '.') {
			continue;
		}

		printf("entry %s\t", entry->d_name);

		child_fd = open(entry->d_name, O_RDONLY);

		if (child_fd == -1) {
			perror("open");
			return -1;
		}

		r = fstat(child_fd, &st);

		if (r != 0) {
			perror("stat");
			return -1;
		}

		if (S_ISDIR(st.st_mode)) {
			printf("DIR\n");

			child_inode = ialloc(T_DIR);
			r = add_dir(fdopendir(child_fd), child_inode, cur_inode);

			if (r != 0) {
				return r;
			}

			if (fchdir(cur_fd) != 0) {
				perror("chdir");
				return -1;
			}
		} else {
			printf("FILE\n");

			bytes_read = 0;
			child_inode = ialloc(T_FILE);
			memset(&de, 0, sizeof(de));

			while ((bytes_read = read(child_fd, buf, sizeof(buf))) > 0) {
				iappend(child_inode, buf, bytes_read);
			}
		}

		close(child_fd);

		de.inum = xshort(child_inode);
		strncpy(de.name, entry->d_name, DIRSIZ);

		if (de.name[0] == '_') {
			de.name[0] = '.';
		}

		iappend(cur_inode, &de, sizeof(de));
	}

	// fix size of inode cur_dir
	rinode(cur_inode, &din);
	off = xint(din.size);
	off = ((off / BSIZE) + 1) * BSIZE;
	din.size = xint(off);
	winode(cur_inode, &din);

	return 0;
}

int
main(int argc, char *argv[]) {
	int r;
	DIR *root_dir;

	if (argc < 2) {
		fprintf(stderr, "Usage: mkfs fs.img files...\n");
		exit(1);
	}

	assert((512 % sizeof(struct dinode)) == 0);
	assert((512 % sizeof(struct xv6_dirent)) == 0);

	fsfd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0666);

	if (fsfd < 0) {
		perror(argv[1]);
		exit(1);
	}

	mkfs();

	root_dir = opendir(argv[2]);

	root_inode = ialloc(T_DIR);
	assert(root_inode == ROOTINO);

	r = add_dir(root_dir, root_inode, root_inode);

	if (r != 0) {
		exit(EXIT_FAILURE);
	}

	balloc(usedblocks);
	exit(0);
}

