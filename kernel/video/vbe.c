#include "x86emu/x86emu.h"
#include "bios.h"
#include "vbe.h"

#include "types.h"
typedef unsigned short ushort;
#include "memlayout.h"
#include "spinlock.h"
#include "fs.h"
#include "file.h"
#include "defs.h"

struct x86emu emulator;

void VBE_BiosInit(void)
{
    memset(&emulator, 0, sizeof(emulator));
    x86emu_init_default(&emulator);
    emulator.mem_base = (char *)0;
    emulator.mem_base = (char *)P2V(0);
    emulator.mem_size = BIOS_SIZE;
}

void VBE_BiosInterrupt( BIOS_REGS *p_regs, u8 num )
{
    memcpy(&(emulator.x86), p_regs, sizeof(BIOS_REGS));
    x86emu_exec_intr(&emulator, num);
    memcpy(p_regs, &(emulator.x86), sizeof(BIOS_REGS));
}

//ulong vbe_lfb_addr = 0;
unsigned long long vbe_lfb_addr = 0;
ulong vbe_selected_mode = 0;
ulong vbe_bytes = 0;

VbeInfoBlock *VBE_GetGeneralInfo()
{
    BIOS_REGS regs;
    memset(&regs, 0, sizeof(BIOS_REGS));
    regs.ECX = 0;
    regs.EAX = 0x4f00;
    regs.ES = VBE_BIOS_INFO_OFFSET >> 4;
    regs.EDI = 0x0;
    VBE_BiosInterrupt(&regs, 0x10);
    if (regs.EAX != 0x4f)
        return NULL;
    return (VbeInfoBlock *)(VBE_BIOS_INFO_OFFSET);
}

ModeInfoBlock *VBE_GetModeInfo( ulong mode )
{
    BIOS_REGS regs;
    memset(&regs, 0, sizeof(BIOS_REGS));
    regs.ECX = mode;
    regs.EAX = 0x4f01;
    regs.ES = VBE_BIOS_MODE_INFO_OFFSET >> 4;
    regs.EDI = 0x0;
    VBE_BiosInterrupt(&regs, 0x10);
    if (regs.EAX != 0x4f)
        return NULL;
    return (ModeInfoBlock *)(VBE_BIOS_MODE_INFO_OFFSET);
}

int VBE_SetMode( ulong mode )
{
    BIOS_REGS regs;
    memset(&regs, 0, sizeof(BIOS_REGS));
    if (mode >= 0x100)
    {
        regs.EBX = mode;
        regs.EAX = 0x4f02;
    }
    else
    {
        regs.EAX = mode;
    }
    VBE_BiosInterrupt(&regs, 0x10);
    return (regs.EAX == 0x4f);
}


int VBE_Setup(int w, int h)
{
    uint32_t m = 0;

    KPRINTF("VBE: test started\n");
    VBE_BiosInit();
    memset(P2V((char *)VBE_BIOS_INFO_OFFSET), 0, sizeof(VbeInfoBlock));
    memset(P2V((char *)VBE_BIOS_MODE_INFO_OFFSET), 0, sizeof(ModeInfoBlock));

    VbeInfoBlock *p_info = P2V(VBE_GetGeneralInfo());
    int vbe_support = (p_info != NULL);
    if (vbe_support == 0)
    {
        KPRINTF("VBE: not supported\n");
        return 0;
    }

    vbe_support = (p_info->VbeVersion >= 0x200);
    uint8_t * sign = p_info->VbeSignature.SigChr;
    /*
        vbe_support = vbe_support && (sign[0] == 'V');
        vbe_support = vbe_support && (sign[1] == 'E');
        vbe_support = vbe_support && (sign[2] == 'S');
        vbe_support = vbe_support && (sign[3] == 'A');
    */
    if (vbe_support == 0)
    {
        KPRINTF("VBE: not supported %x %s\n", p_info->VbeVersion, sign);
        return 0;
    }


    //Try to find  mode
    int found = 0;
    for (m = 0x0; m < 0x200; m++)
    {
        ModeInfoBlock *p_m_info = P2V(VBE_GetModeInfo(m));
//        ModeInfoBlock *p_m_info = VBE_GetModeInfo(m);
        if ((p_m_info != NULL) && (p_m_info != P2V(NULL)))
        {
            KPRINTF("VBE: %x %dx%dx%d at %x -> %x\n", m,
                    p_m_info->XResolution,
                    p_m_info->YResolution,
                    p_m_info->BitsPerPixel,
                    p_m_info->PhysBasePtr);

            if (p_m_info->PhysBasePtr != 0
                    && p_m_info->XResolution == w
                    && p_m_info->YResolution == h
                    && (p_m_info->BitsPerPixel == 24 || p_m_info->BitsPerPixel == 32))
            {
                found = 1;
                vbe_selected_mode = m;
                vbe_lfb_addr = p_m_info->PhysBasePtr;
                vbe_bytes = p_m_info->BitsPerPixel / 8;
                KPRINTF("VBE: FOUND GOOD %dx%dx%d -> %x at %x\n", w, h, vbe_bytes, vbe_selected_mode, vbe_lfb_addr);
            }
        }
    }
    return found;
}




#define DEV_VBE 3

struct {
	struct spinlock lock;
	uint size;
    uint w;
    uint h;
    uint bytes;
	void *video;
} vbe;

int
vberead(struct inode *ip, char *dst, int off, int n) {
	int size = (n < vbe.size)? n: vbe.size;

	iunlock(ip);
	acquire(&vbe.lock);

	memcpy(dst, vbe.video+off, size);

	release(&vbe.lock);
	ilock(ip);

	return size;
}

int
vbewrite(struct inode *ip, char *buf, int off, int n) {
	int size = (n < vbe.size)? n: vbe.size;

	iunlock(ip);
	acquire(&vbe.lock);

	memcpy(vbe.video+off, buf, size);

	release(&vbe.lock);
	ilock(ip);

	return size;
}




void test_vbe(void) {
    //int x = 0, y = 0, w= 800, h = 600;
    int x = 0, y = 0, w= 1024, h = 768;

    if (!VBE_Setup(w, h))
        return;

    if (!VBE_SetMode(vbe_selected_mode | 0x4000))
        return;

	vbe.size = w*h*vbe_bytes;
    vbe.video = vbe_lfb_addr;

	initlock(&vbe.lock, "vbe");

	devsw[DEV_VBE].write = vbewrite;
	devsw[DEV_VBE].read = vberead;

    //double cRe, cIm;
    //double newRe, newIm, oldRe, oldIm;
    //double zoom = 1, moveX = 0, moveY = 0;
    int color;
    int maxIterations = 300;

    //cRe = -0.7;
    //cIm = 0.27015;

    for(y = 0; y < h; y++)
        for(x = 0; x < w; x++)
        {
            color = 0xCC0000;

            // Draw pixel
            int * pixel = (char *)vbe_lfb_addr + y * w * vbe_bytes + x * vbe_bytes + 0;
            *(int *)pixel = color & 0xFFFFFF;
        }
}
