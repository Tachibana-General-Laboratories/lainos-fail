#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "defs.h"
#include "x86.h"
#include "elf.h"

int
execve(char *path, char **argv, char **env) {
	char *s, *last;
	int i, off;
	uint argc, envc, sz, sp, ustack[4 + MAXARG*2 + 2];
	struct elfhdr elf;
	struct inode *ip;
	struct proghdr ph;
	pde_t *pgdir, *oldpgdir;

	if ((ip = namei(path)) == 0) {
		return -1;
	}

	ilock(ip);
	pgdir = 0;

	// Check ELF header
	if (readi(ip, (char *)&elf, 0, sizeof(elf)) < sizeof(elf)) {
		goto bad;
	}

	if (elf.magic != ELF_MAGIC) {
		goto bad;
	}

	if ((pgdir = setupkvm()) == 0) {
		goto bad;
	}

	// Load program into memory.
	sz = 0;

	for (i = 0, off = elf.phoff; i < elf.phnum; i++, off += sizeof(ph)) {
		if (readi(ip, (char *)&ph, off, sizeof(ph)) != sizeof(ph)) {
			goto bad;
		}

		if (ph.type != ELF_PROG_LOAD) {
			continue;
		}

		if (ph.memsz < ph.filesz) {
			goto bad;
		}

		if ((sz = allocuvm(pgdir, sz, ph.vaddr + ph.memsz)) == 0) {
			goto bad;
		}

		if (loaduvm(pgdir, (char *)ph.vaddr, ip, ph.off, ph.filesz) < 0) {
			goto bad;
		}
	}

	iunlockput(ip);
	ip = 0;

	// Allocate two pages at the next page boundary.
	// Make the first inaccessible.  Use the second as the user stack.
	sz = PGROUNDUP(sz);

	if ((sz = allocuvm(pgdir, sz, sz + 2 * PGSIZE)) == 0) {
		goto bad;
	}

	clearpteu(pgdir, (char *)(sz - 2 * PGSIZE));
	sp = sz;

	/*
	 * Далее заполняется стек
	 */

	ustack[0] = 0xffffffff;  // fake return PC

	// Добавляем содержимое env
	// Push argument strings, prepare rest of stack in ustack.
	for (envc = 0; env[envc]; envc++) {
		if (i >= MAXARG) {
			goto bad;
		}

		sp = (sp - (strlen(env[envc]) + 1)) & ~3;

		if (copyout(pgdir, sp, env[envc], strlen(env[envc]) + 1) < 0) {
			goto bad;
		}

		ustack[5 + envc] = sp;
	}

	ustack[3] = sp - (envc + 1) * 4; // env pointer

	ustack[5 + envc] = 0;
	sp -= (5 + envc + 1) * 4;

	// Ахтунг! Здесь именно 4.
	if (copyout(pgdir, sp, ustack, (4 + envc + 1) * 4) < 0) {
		goto bad;
	}















	// Добавляем содержимое argv
	// Push argument strings, prepare rest of stack in ustack.
	for (argc = 0; argv[argc]; argc++) {
		if (argc >= MAXARG) {
			goto bad;
		}

		sp = (sp - (strlen(argv[argc]) + 1)) & ~3;

		if (copyout(pgdir, sp, argv[argc], strlen(argv[argc]) + 1) < 0) {
			goto bad;
		}

		ustack[4 + argc] = sp;
	}

	ustack[1] = argc;
	ustack[2] = sp - (argc + 1) * 4; // argv pointer

	ustack[4 + argc] = 0;
	sp -= (4 + argc + 1) * 4;

	if (copyout(pgdir, sp, ustack, (4 + argc + 1) * 4) < 0) {
		goto bad;
	}

/* Закончили заполнять стек */

	// Save program name for debugging.
	for (last = s = path; *s; s++)
		if (*s == '/') {
			last = s + 1;
		}

	safestrcpy(proc->name, last, sizeof(proc->name));

	// Commit to the user image.
	oldpgdir = proc->pgdir;
	proc->pgdir = pgdir;
	proc->sz = sz;
	proc->tf->eip = elf.entry;  // main
	proc->tf->esp = sp;
	switchuvm(proc);
	freevm(oldpgdir);
	return 0;

bad:

	if (pgdir) {
		freevm(pgdir);
	}

	if (ip) {
		iunlockput(ip);
	}

	return -1;
}
