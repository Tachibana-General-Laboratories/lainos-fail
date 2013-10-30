#ifndef _BIOS_H
#define _BIOS_H

#define BIOS_SIZE 0x100000
#define BIOS_HIGH_BASE 0xC0000
#define BIOS_HIGH_SIZE (0x100000 - 0xC0000)
#define BIOS_BDA_BASE 0x9fc00
#define BIOS_BDA_SIZE 0x400
#define VBE_BIOS_INFO_OFFSET      0x70000
#define VBE_BIOS_MODE_INFO_OFFSET 0x80000

typedef struct _BIOS_REGS
{
    u16 CS;
    u16 DS;
    u16 ES;
    u16 FS;
    u16 GS;
    u16 SS;
    u32 EFLAGS;
    u32 EAX;
    u32 EBX;
    u32 ECX;
    u32 EDX;
    u32 ESP;
    u32 EBP;
    u32 ESI;
    u32 EDI;
    u32 EIP;
} BIOS_REGS;

void VBE_BiosInit(void);
void VBE_BiosInterrupt( BIOS_REGS *p_regs, u8 num );

#endif
