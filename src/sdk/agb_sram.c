/*
 * agb_sram.c: Nintendo AGB SDK "AgbSram" library, version 1.12 (non-fast variant).
 *
 * Yu-Gi-Oh! The Eternal Duelist Soul (USA, AY5E)
 *
 *   .text   0x0807ED04 - 0x0807EE98  (0x194 bytes, Thumb)
 *   .rodata 0x08087FB4 - 0x08087FD0  (0x1C bytes)
 *
 *   function          address     size   status
 *   ReadSram_Core     0x0807ED04  0x24   MATCHING  (static; copied to the stack by ReadSram)
 *   ReadSram          0x0807ED28  0x64   MATCHING
 *   WriteSram         0x0807ED8C  0x40   MATCHING
 *   VerifySram_Core   0x0807EDCC  0x30   MATCHING  (static; copied to the stack by VerifySram)
 *   VerifySram        0x0807EDFC  0x64   MATCHING
 *   WriteSramEx       0x0807EE60  0x38   MATCHING  (linked but never called by the game)
 *
 * Build: cpp | agbcc -mthumb-interwork -O1 | arm-none-eabi-as -mcpu=arm7tdmi
 *        (-O1, not -O2. At -O2 the prologues/loops differ and the function-pointer
 *        call goes through _call_via_r4 instead of _call_via_r3. old_agbcc -O1 gives
 *        identical output.)
 * External: _call_via_r3 (libgcc _call_via_rX.o, 0x0807EEA4).
 * Verified: object linked with .text at 0x0807ED04 and .rodata at 0x08087FB4 and
 *           compared byte-for-byte against baserom (0 differing bytes in both sections).
 *
 * .rodata notes
 *   0x08087FB4  "SRAM_V112\0" + 2 bytes of 0 padding   (AgbSramLibVer, 10 bytes, align 4)
 *   0x08087FC0  .word ReadSram_Core   (0x0807ED05)  \  dead constant-pool entries (.LC0/.LC1,
 *   0x08087FC4  .word ReadSram        (0x0807ED29)   | .LC2/.LC3) that agbcc -O1 emits into
 *   0x08087FC8  .word VerifySram_Core (0x0807EDCD)   | .rodata for the function addresses used
 *   0x08087FCC  .word VerifySram      (0x0807EDFD)  /  in ReadSram/VerifySram. The code itself
 *                                                      loads them from the .text literal pools.
 *   These four words come out of the compiler automatically; they must not be written as
 *   data in C. They only land in the right place if this object's .rodata is linked at
 *   0x08087FB4, between the game's rodata and the table at 0x08087FD0.
 *
 * Matching notes
 *   - Func_src must be assigned in two statements (load, then ^= 1). A single
 *     `(u16 *)((u32)ReadSram_Core ^ 1)` makes CSE reuse the register holding
 *     ReadSram_Core for the size computation. The ROM reloads the address from the
 *     literal pool instead.
 *   - v1.12 differs from the "fast" SRAM_F_V10x library (e.g. fireemblem8u
 *     src/agb_sram.c). There is no SetSramFastFunc and no ReadSramFast/VerifySramFast
 *     pointers. ReadSram/VerifySram copy their _Core routine into a stack buffer
 *     (u16[0x40] / u16[0x60]) on every call and run it from there. Neither _Core
 *     routine touches WAITCNT; the wrappers set it (SRAM 8 wait) before copying.
 */
#include "agb_sram.h"

const char AgbSramLibVer[] = "SRAM_V112";

static void ReadSram_Core(u8 *src, u8 *dst, u32 size)
{
    while (size--)
        *dst++ = *src++;
}

void ReadSram(u8 *src, u8 *dst, u32 size)
{
    u16 *Func_src, *Func_dst;
    u16 readSram_Work[0x40];
    u16 func_size;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;       // SRAM wait: 8 cycles
    Func_src = (u16 *)ReadSram_Core;
    Func_src = (u16 *)((u32)Func_src ^ 1);          // clear the Thumb bit
    Func_dst = readSram_Work;
    func_size = ((u32)ReadSram - (u32)ReadSram_Core) >> 1;
    while (func_size != 0)
    {
        *Func_dst++ = *Func_src++;
        func_size--;
    }
    ((void (*)(u8 *, u8 *, u32))((u32)readSram_Work + 1))(src, dst, size);
}

void WriteSram(u8 *src, u8 *dst, u32 size)
{
    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    while (size--)
        *dst++ = *src++;
}

static u32 VerifySram_Core(u8 *src, u8 *tgt, u32 size)
{
    while (size--)
    {
        if (*tgt++ != *src++)
            return (u32)(tgt - 1);
    }
    return 0;
}

u32 VerifySram(u8 *src, u8 *tgt, u32 size)
{
    u16 *Func_src, *Func_dst;
    u16 verifySram_Work[0x60];
    u16 func_size;

    REG_WAITCNT = (REG_WAITCNT & 0xfffc) | 3;
    Func_src = (u16 *)VerifySram_Core;
    Func_src = (u16 *)((u32)Func_src ^ 1);
    Func_dst = verifySram_Work;
    func_size = ((u32)VerifySram - (u32)VerifySram_Core) >> 1;
    while (func_size != 0)
    {
        *Func_dst++ = *Func_src++;
        func_size--;
    }
    return ((u32 (*)(u8 *, u8 *, u32))((u32)verifySram_Work + 1))(src, tgt, size);
}

u32 WriteSramEx(u8 *src, u8 *dst, u32 size)
{
    u8 i;
    u32 errorAdr;

    // write + verify, up to SRAM_RETRY_MAX (3) attempts
    for (i = 0; i < 3; i++)
    {
        WriteSram(src, dst, size);
        errorAdr = VerifySram(src, dst, size);
        if (errorAdr == 0)
            break;
    }
    return errorAdr;
}
