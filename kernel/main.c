#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "x86.h"

static void startothers(void);
static void mpmain(void)  __attribute__((noreturn));
extern pde_t *kpgdir;
extern char end[]; // first address after kernel loaded from ELF file

#define BOOT_STEP(function, message...) \
({ \
    KPRINTF("" #function " " message "\n"); \
    function; \
})

// Bootstrap processor starts running C code here.
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
int
main(void) {
	BOOT_STEP(kinit1(end, P2V(4 * 1024 * 1024)), "// phys page allocator");
	BOOT_STEP(kvmalloc(), "// kernel page table");
	BOOT_STEP(mpinit(), "// collect info about this machine");
	BOOT_STEP(lapicinit(), "");
	BOOT_STEP(seginit(), "// set up segments");
	KPRINTF("\ncpu%d: starting xv6\n\n", cpu->id);
	BOOT_STEP(picinit(), "// interrupt controller");
	BOOT_STEP(ioapicinit(), "// another interrupt controller");
	BOOT_STEP(consoleinit(), "// I/O devices & their interrupts");
	BOOT_STEP(uartinit(), "// serial port");
	BOOT_STEP(pinit(), "// process table");
	BOOT_STEP(tvinit(), "// trap vectors");
	BOOT_STEP(binit(), "// buffer cache");
	BOOT_STEP(fileinit(), "// file table");
	BOOT_STEP(iinit(), "// inode cache");
	BOOT_STEP(ideinit(), "// disk");

	if (!ismp) {
		BOOT_STEP(timerinit(), "// uniprocessor timer");
	}

	BOOT_STEP(startothers(), "// start other processors");
	BOOT_STEP(kinit2(P2V(4 * 1024 * 1024), P2V(PHYSTOP)), "// must come after startothers()");
	BOOT_STEP(userinit(), "// first user process");
	BOOT_STEP(test_vbe(), "// VBE!!!!!!!!11111111111");
	BOOT_STEP(mpmain(), "// Finish setting up this processor in mpmain");
}

// Other CPUs jump here from entryother.S.
static void
mpenter(void) {
	switchkvm();
	seginit();
	lapicinit();
	mpmain();
}

// Common CPU setup code.
static void
mpmain(void) {
	KPRINTF("cpu%d: starting\n", cpu->id);
	idtinit();       // load idt register
	xchg(&cpu->started, 1); // tell startothers() we're up
	scheduler();     // start running processes
}

pde_t entrypgdir[];  // For entry.S

// Start the non-boot (AP) processors.
static void
startothers(void) {
	extern uchar _binary_entryother_start[], _binary_entryother_size[];
	uchar *code;
	struct cpu *c;
	char *stack;

	// Write entry code to unused memory at 0x7000.
	// The linker has placed the image of entryother.S in
	// _binary_entryother_start.
	code = p2v(0x7000);
	memmove(code, _binary_entryother_start, (uint)_binary_entryother_size);

	for (c = cpus; c < cpus + ncpu; c++) {
		if (c == cpus + cpunum()) { // We've started already.
			continue;
		}

		// Tell entryother.S what stack to use, where to enter, and what
		// pgdir to use. We cannot use kpgdir yet, because the AP processor
		// is running in low  memory, so we use entrypgdir for the APs too.
		stack = kalloc();
		*(void **)(code - 4) = stack + KSTACKSIZE;
		*(void **)(code - 8) = mpenter;
		*(int **)(code - 12) = (void *) v2p(entrypgdir);

		lapicstartap(c->id, v2p(code));

		// wait for cpu to finish mpmain()
		while (c->started == 0)
			;
	}
}

// Boot page table used in entry.S and entryother.S.
// Page directories (and page tables), must start on a page boundary,
// hence the "__aligned__" attribute.
// Use PTE_PS in page directory entry to enable 4Mbyte pages.
__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
	// Map VA's [0, 4MB) to PA's [0, 4MB)
	[0] = (0) | PTE_P | PTE_W | PTE_PS,
	// Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
	[KERNBASE >> PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

//PAGEBREAK!
// Blank page.

