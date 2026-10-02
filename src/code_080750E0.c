#include "global.h"

#include "gba.h"

/*
 * System core: key input, save, main-callback switching, VBlank/Timer2 IRQ
 * handlers, brightness fades, OAM flush, the per-frame sync, MainLoop and
 * GameInit/AgbMain, plus small memory/string helpers. See
 * wiki/functions/code-080750e0.md.
 */

struct OamEntry {
    u32 w0;
    u32 w1;
};

/* System state (0x03000040). */
struct Main {
    u32 rngState;                   /* +0x000 */
    u16 heldKeys;                   /* +0x004 */
    u16 newKeys;                    /* +0x006 */
    u16 prevKeys;                   /* +0x008 */
    u16 keyRepeatTimer;             /* +0x00A */
    u8 intrMainBuf[0x400];          /* +0x00C RAM copy of IntrMain */
    vu16 intrCheck;                 /* +0x40C bit 0: VBlank happened */
    u16 vblankFlags;                /* +0x40E */
    u16 (*callback)(void);          /* +0x410 scene; returns nonzero when done */
    void (*vblankCallback)(void);   /* +0x414 */
    void (*vblankCallbackEarly)(void); /* +0x418 */
    u16 bgMapBuffer[8][0x400];      /* +0x41C */
    u16 unk441C[2];
    u16 bgVofs[4];                  /* +0x4420 */
    u16 bgHofs[4];                  /* +0x4428 */
    struct OamEntry oam[128];       /* +0x4430 */
    u8 oamCount;                    /* +0x4830 */
    u8 oamCount2;                   /* +0x4831 */
    u8 brightness:6;                /* +0x4832 fade level 0..0x1F */
    u8 unk4832_6:2;
    u8 filler4833[0x4857 - 0x4833];
    u8 seq4857;
    u8 seq4858;
    u8 seq4859;
    u8 seq485A;
    u8 seq485B;
    u8 filler485C[2];
    u16 frameCounter;               /* +0x485E */
    u8 frameCounter8;               /* +0x4860 */
    u8 vblankCounter8;              /* +0x4861 */
    u8 filler4862[2];
    u16 vblankCounter;              /* +0x4864 */
    u16 lagCounter;                 /* +0x4866 */
    u8 filler4868[0x4878 - 0x4868];
    u8 seq4878;
    u8 seq4879;
    u8 seq487A;
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

/* Save mirror (0x02011C20). */
struct SaveData {
    u32 unk0;
    u8 flags4;                      /* +0x04 bit 7: Shift-JIS text mode */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

extern void (*gUnk_03000000[16])(void); /* IntrTable */

struct Timer2State {
    u8 filler0[0x82C];
    u16 counter;                    /* +0x82C incremented by Timer2Intr */
};
extern struct Timer2State gUnk_030049D0;

struct ScrollReg {
    vu16 *reg;
    u16 mask;
    u16 pad;
};
extern const struct ScrollReg gUnk_081A7764[4]; /* HOFS registers, mask bits 4-7 */
extern const struct ScrollReg gUnk_081A7784[4]; /* VOFS registers, mask bits 8-11 */

void sub_08075050(u32 a, u32 b, u16 c);
void sub_0807509C(u32 a, u32 b, u16 c);
void sub_08077080(void);
void WriteSram(const void *src, void *dst, u32 size);
u32 VerifySram(const void *src, void *dst, u32 size);
void sub_0807E3B0(void);
void sub_0807E554(void);
u16 sub_08076F9C(void);
void sub_08075228(void);
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_08075294(void *dst, const void *src, u32 size);
void sub_080752E8(char *dst, const char *src);
void sub_08075370(char *dst, s32 n);
void sub_080757F4(void);
void sub_08075C44(void);
void sub_080754BC(void);
u16 sub_08003AA4(void);
void sub_08075740(void);
void sub_0807570C(void);
void sub_0807569C(void);
void sub_0807E324(void);
u16 sub_08004EAC(void);
void ReadSram(const void *src, void *dst, u32 size);
void sub_0807D578(u32 a);
void sub_080757AC(void);
void sub_08075A30(void);
void sub_08077A74(u32 a);
void sub_08077AB0(u32 a);
void sub_080770DC(void);
void sub_08075D6C(void);
void sub_08075CB4(void);
void sub_08075D70(void);
void sub_08075DF4(void);
extern vu16 gUnk_0300044C;

void sub_080750E0(u32 a, u32 b, u16 c)
{
    /* Keep both coordinates in the callee-saved registers used by the ROM. */
    __asm__ __volatile__("" : : : "r3");
    if (gSaveData.flags4 & 0x80)
        sub_08075050(a, b, c);
    else
        sub_0807509C(a, b, c);
}
extern u8 gUnk_02000000[];      /* text bitmap; +0x10000 width, +0x10001 height (tiles) */

/* Converts the 1-byte-per-pixel bitmap at 0x02000000 into 4bpp tiles at
 * dst; pixels of value 0 take the background nibble from bits 0-3 of bg. */
#if 0 /* NONMATCHING: register allocation only (target: fill r4, row counter r5, byte temp r6; build r5/r6/r4) */
void sub_08075114(u16 *dst, u16 bg)
{
    u16 fill = (bg << 16 & 0xF0000) >> 16;
    s32 i, next;

    fill |= fill << 4;
    fill |= fill << 8;
    for (i = 0; i < gUnk_02000000[0x10000] * gUnk_02000000[0x10001]; i = next) {
        const u8 *src = gUnk_02000000 + i * 64;
        s32 j;

        next = i + 1;
        for (j = 15; j >= 0; j--) {
            *dst = fill;
            if (src[0] != 0) {
                *dst &= 0xFFF0;
                *dst |= src[0];
            }
            if (src[1] != 0) {
                *dst &= 0xFF0F;
                *dst |= src[1] << 4;
            }
            if (src[2] != 0) {
                *dst &= 0xF0FF;
                *dst |= src[2] << 8;
            }
            if (src[3] != 0) {
                *dst &= 0x0FFF;
                *dst |= src[3] << 12;
            }
            dst++;
            src += 4;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080750E0", sub_08075114); /* 0x08075114 size 0x114 */

void sub_08075228(void)
{
    u32 keys = (u16)~REG_KEYINPUT;
    u32 newKeys = keys & ~gMain.heldKeys;

    gMain.newKeys = newKeys;
    gMain.heldKeys = keys;
    if (keys != gMain.prevKeys) {
        gMain.keyRepeatTimer = 0;
        gMain.prevKeys = keys;
    } else {
        u16 t = gMain.keyRepeatTimer;

        gMain.keyRepeatTimer = t + 1;
        if ((u16)(t + 1) > 0x14) {
            gMain.keyRepeatTimer = t - 1;
            newKeys |= keys & 0xF0;
            gMain.newKeys = newKeys;
        }
    }
}
void sub_08075278(void *dstp, s32 size)
{
    u16 *dst = dstp;

    size = (size + 1) / 2;
    while (size != 0) {
        *dst++ = 0;
        size--;
    }
}
void sub_08075294(void *dstp, const void *srcp, u32 size)
{
    u16 *dst = dstp;
    const u16 *src = srcp;

    size = (size + 1) / 2;
    while (size != 0) {
        *dst++ = *src++;
        size--;
    }
}

void sub_080752B0(void *dstp, const void *srcp, u32 size)
{
    const struct OamEntry *src = srcp;
    struct OamEntry *dst = dstp;

    size = (size + 7) >> 3;
    while (size != 0) {
        *dst++ = *src++;
        size--;
    }
}
void sub_080752D0(char *dst, const char *src)
{
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
}
void sub_080752E8(char *dst, const char *src)
{
    while (*dst != 0) {
        dst++;
    }
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
}
void sub_08075308(char *dst, s32 n)
{
    u16 buf[12];
    s32 i, j;

    for (j = 0; j < 11; j++) {
        buf[j] = 0x4F82;
    }
    buf[11] = 0;
    i = 10;
    while (n > 0 && i > 0) {
        buf[i] = (u8)(n % 10 + 0x4F) << 8 | 0x82;
        n /= 10;
        i--;
    }
    sub_080752E8(dst, (char *)&buf[i + 1]);
}
void sub_08075370(char *dst, s32 n)
{
    char buf[12];
    s32 i, j;

    for (j = 0; j < 11; j++) {
        buf[j] = '0';
    }
    buf[11] = 0;
    i = 10;
    while (n > 0 && i > 0) {
        buf[i] = n % 10 + '0';
        n /= 10;
        i--;
    }
    sub_080752E8(dst, &buf[i + 1]);
}
s32 sub_080753CC(const char *s)
{
    s32 n = 0;

    while (*s != 0) {
        n++;
        s++;
    }
    return n;
}
s32 sub_080753E0(const u16 *s)
{
    s32 n = 0;

    while (*(const u8 *)s != 0) {
        s++;
        n++;
    }
    return n;
}
void sub_080753F4(char *dst, const char *fmt, const char *arg)
{
    while (*fmt != 0) {
        if (*fmt == '%' && fmt[1] == 's') {
            *dst = 0;
            fmt += 2;
            sub_080752E8(dst, arg);
            sub_080752E8(dst, fmt);
            return;
        }
        *dst++ = *fmt++;
    }
}
void sub_08075434(char *dst, const char *fmt, s32 arg)
{
    while (*fmt != 0) {
        if (*fmt == '%' && fmt[1] == 'd') {
            *dst = 0;
            fmt++;
            sub_08075370(dst, arg);
            fmt++;
            sub_080752E8(dst, fmt);
            return;
        }
        *dst++ = *fmt++;
    }
}
s32 sub_08075474(s32 x)
{
    return (x + 5) / 10 * 10;
}
s32 sub_0807548C(s32 x)
{
    x = x * 5;
    x += 5;
    return x / 10;
}
s32 sub_080754A4(s32 x)
{
    x = x * 5;
    x += 4;
    return x / 10;
}
void sub_080754BC(void)
{
    s32 i;

    sub_08077080();
    i = 0;
    do {
        WriteSram(&gSaveData, (void *)SRAM, 0x2170);
        if (VerifySram(&gSaveData, (void *)SRAM, 0x2170) == 0) {
            break;
        }
        i++;
    } while (i <= 31);
}
void sub_080754F8(u16 (*cb)(void))
{
    sub_080754BC();
    gMain.vblankCallbackEarly = NULL;
    gMain.vblankCallback = NULL;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    gUnk_03000000[1] = NULL;
    REG_IME = 1;
    gMain.seq4878 = 0;
    gMain.seq4879 = 0;
    gMain.seq487A = 0;
    gMain.seq4857 = 0;
    gMain.seq4858 = 0;
    gMain.seq4859 = 0;
    gMain.seq485A = 0;
    gMain.seq485B = 0;
    gMain.callback = cb;
}
u32 sub_080755A0(void)
{
    sub_080754BC();
    gMain.vblankCallbackEarly = NULL;
    gMain.vblankCallback = NULL;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    gUnk_03000000[1] = NULL;
    REG_IME = 1;
    gMain.seq4858 = 0;
    gMain.seq4859 = 0;
    gMain.seq485A = 0;
    gMain.seq485B = 0;
    return 1;
}
extern const u16 gUnk_0822C300[];
extern const u16 gUnk_0822C320[];

void sub_08075630(void)
{
    sub_08075294((void *)BG_PLTT, gUnk_0822C300, 0x20);
    *(u16 *)BG_PLTT = 0;
    sub_08075294((void *)(VRAM + 0x4000), gUnk_0822C320, 0x200);
    sub_08075294((void *)OBJ_PLTT, gUnk_0822C300, 0x20);
    sub_08075294((void *)OBJ_VRAM0, gUnk_0822C320, 0x200);
    *(u16 *)OBJ_PLTT = 0;
}
void sub_0807569C(void)
{
    gMain.intrCheck |= 1;
    gMain.vblankCounter++;
    gMain.lagCounter++;
    if (gMain.vblankCallbackEarly != NULL) {
        gMain.vblankCallbackEarly();
    }
    sub_0807E3B0();
    if (gMain.vblankCallback != NULL) {
        gMain.vblankCallback();
    }
    gMain.vblankCounter8++;
}
void sub_0807570C(void)
{
    REG_TM2CNT_L = 0xF400;
    REG_TM2CNT_H |= 0xC3;
    gUnk_030049D0.counter++;
}
void sub_08075740(void)
{
    while (1) {
    }
}
void sub_08075744(void)
{
    struct Main *m = &gMain;
    u16 zero = 0;
    s32 i = 3;

    do {
        m->bgHofs[i] = zero;
        i--;
    } while (i >= 0);
    {
        u16 z = 0;
        vu16 *reg = &REG_BG0HOFS;

        *reg = z;
        reg += 2;
        *reg = z;
        reg += 2;
        *reg = z;
        reg += 2;
        *reg = z;
    }
}
void sub_08075778(void)
{
    struct Main *m = &gMain;
    u16 zero = 0;
    s32 i = 3;

    do {
        m->bgVofs[i] = zero;
        i--;
    } while (i >= 0);
    {
        u16 z = 0;
        vu16 *reg = &REG_BG0VOFS;

        *reg = z;
        reg += 2;
        *reg = z;
        reg += 2;
        *reg = z;
        reg += 2;
        *reg = z;
    }
}
void sub_080757AC(void)
{
    sub_08075744();
    sub_08075778();
}
void sub_080757BC(u16 bldcnt)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = bldcnt;
    REG_BLDY = gMain.brightness;
}
void sub_080757F4(void)
{
    gMain.brightness = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
}
u32 sub_0807581C(u16 bldcnt)
{
    u32 ret;

    if (gMain.brightness > 2) {
        gMain.brightness -= 2;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        sub_080757F4();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = bldcnt;
        ret = 0;
    }
    return ret;
}
u32 sub_0807588C(u16 bldcnt)
{
    REG_BLDCNT = bldcnt;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += 2;
        if (gMain.brightness > 0x1F) {
            gMain.brightness = 0x1F;
        }
    }
    {
        vu16 *bldy = &REG_BLDY;

        *bldy = gMain.brightness;
        if (gMain.brightness <= 0x1E) {
            return 0;
        }
        return 1;
    }
}
u32 sub_080758FC(u16 bldcnt)
{
    u32 ret;

    if (gMain.brightness > 2) {
        gMain.brightness -= 2;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        sub_080757F4();
        ret = 1;
    } else {
        REG_BLDALPHA = gMain.brightness + ((0x1F - gMain.brightness) << 8);
        REG_BLDCNT = bldcnt;
        ret = 0;
    }
    return ret;
}
u32 sub_0807597C(u16 bldcnt)
{
    REG_BLDCNT = bldcnt;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += 2;
        if (gMain.brightness > 0x1F) {
            gMain.brightness = 0x1F;
        }
    }
    {
        vu16 *alpha = &REG_BLDALPHA;

        *alpha = (gMain.brightness << 8) + 0x1F - gMain.brightness;
        if (gMain.brightness <= 0x1E) {
            return 0;
        }
        return 1;
    }
}
void sub_080759F4(void)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = 0x3FFF;
    REG_BLDY = gMain.brightness;
}
void sub_08075A30(void)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = 0x3FBF;
    REG_BLDY = gMain.brightness;
}
u32 sub_08075A6C(s32 step)
{
    REG_BLDCNT = 0x3FFF;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F) {
            gMain.brightness = 0x1F;
        }
    }
    {
        vu16 *bldy = &REG_BLDY;

        *bldy = gMain.brightness;
        if (gMain.brightness <= 0x1E) {
            return 0;
        }
        return 1;
    }
}
u32 sub_08075AE4(s32 step)
{
    u32 ret;

    if (gMain.brightness > step) {
        gMain.brightness -= step;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        sub_080757F4();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x3FFF;
        ret = 0;
    }
    return ret;
}
u32 sub_08075B58(s32 step)
{
    REG_BLDCNT = 0x3FBF;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F) {
            gMain.brightness = 0x1F;
        }
    }
    {
        vu16 *bldy = &REG_BLDY;

        *bldy = gMain.brightness;
        if (gMain.brightness <= 0x1E) {
            return 0;
        }
        return 1;
    }
}
u32 sub_08075BD0(s32 step)
{
    u32 ret;

    if (gMain.brightness > step) {
        gMain.brightness -= step;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        sub_080757F4();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x3FBF;
        ret = 0;
    }
    return ret;
}
void sub_08075C44(void)
{
    s32 i;
    u32 *p;
    u8 *q;

    if (gMain.vblankFlags & 1) {
        sub_080752B0((void *)OAM, gMain.oam, 0x400);
        gMain.oamCount = 0;
        gMain.oamCount2 = 0;
        for (i = 0; i < 128; i++) {
            q = (u8 *)&gMain + i * 8;
            p = (u32 *)&gMain.oam[i];
            *p++ = 0;
            *p = 0;
            q[0x4435] |= 0xC;
        }
    }
}
void sub_08075CB4(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gMain.vblankFlags & gUnk_081A7764[i].mask) {
            *gUnk_081A7764[i].reg = gMain.bgHofs[i];
        }
        if (gMain.vblankFlags & gUnk_081A7784[i].mask) {
            *gUnk_081A7784[i].reg = gMain.bgVofs[i];
        }
    }
    if (gMain.vblankFlags & 2) {
        for (i = 0; i < 8; i++) {
            sub_080752B0((void *)(VRAM + i * 0x800), gMain.bgMapBuffer[i], 0x800);
        }
    }
    sub_08075C44();
    sub_08075228();
    sub_0807E554();
    sub_08076F9C();
}
void sub_08075D6C(void)
{
}
void sub_08075D70(void)
{
    while (1) {
        gMain.intrCheck &= 0xFFFE;
        while (!(gMain.intrCheck & 1)) {
        }
        sub_08075CB4();
        gMain.lagCounter = 0;
        if (gMain.callback()) {
            sub_080754F8(sub_08003AA4);
        }
        sub_08075D6C();
        gMain.frameCounter++;
        gMain.frameCounter8++;
    }
}
void sub_08075DF4(void)
{
    u32 zero = 0;
    vu32 *dma = &REG_DMA3SAD;

    dma[0] = (u32)&zero;
    dma[1] = EWRAM;
    dma[2] = 0x85010000;
    dma[2];
    zero = 0;
    dma[0] = (u32)&zero;
    dma[1] = IWRAM;
    dma[2] = 0x85001E80;
    dma[2];
    ReadSram((void *)SRAM, &gSaveData, 0x2170);
    gUnk_03000000[0] = NULL;
    gUnk_03000000[1] = NULL;
    gUnk_03000000[2] = sub_0807569C;
    gUnk_03000000[3] = NULL;
    gUnk_03000000[4] = NULL;
    gUnk_03000000[5] = NULL;
    gUnk_03000000[6] = sub_0807570C;
    gUnk_03000000[7] = NULL;
    gUnk_03000000[8] = NULL;
    gUnk_03000000[9] = sub_0807E324;
    gUnk_03000000[10] = NULL;
    gUnk_03000000[11] = NULL;
    gUnk_03000000[12] = NULL;
    gUnk_03000000[13] = sub_08075740;
    gUnk_03000000[14] = NULL;
    gUnk_03000000[15] = NULL;
    dma[0] = 0x080000FC;
    dma[1] = (u32)gMain.intrMainBuf;
    dma[2] = 0x80000200;
    dma[2];
    *(u32 *)0x03007FFC = (u32)gMain.intrMainBuf;
    REG_IME = 0;
    REG_IE = 1;
    REG_IE |= 0x200;
    REG_IE |= 0x400;
    REG_IE |= 0x2000;
    REG_IE |= 0x20;
    REG_DISPSTAT = 8;
    REG_DISPSTAT |= 0x10;
    REG_IME = 1;
    REG_WAITCNT = 0x4014;
    gMain.callback = sub_08004EAC;
    gMain.vblankCallback = NULL;
    sub_0807D578(0);
    REG_TM2CNT_L = 0xF400;
    REG_TM2CNT_H |= 0xC3;
    gMain.vblankFlags = 3;
    sub_080757AC();
    sub_08075A30();
    sub_08077A74(1);
    sub_08077AB0(1);
    sub_080770DC();
}
void AgbMain(void)
{
    sub_08075DF4();
    sub_08075D70();
}
/* Link-cable SIO state (0x03005B60); only the fields the serial IRQ uses. */
struct LinkSio {
    u8 filler0[0xA1E];
    u8 master;                  /* +0xA1E this unit drives the transfer clock */
    u8 filler0A1F[2];
    u8 dataReady;               /* +0xA21 set when a full packet was swapped in */
    u8 filler0A22[0xA2C - 0xA22];
    s32 state;                  /* +0xA2C slot in the packet, -3..10 */
    u16 (*rxBuf)[12];           /* +0xA30 current receive rows (one per player) */
    u16 (*rxDone)[12];          /* +0xA34 previous (completed) rows */
    u8 filler0A38[4];
    u16 txBuf[10];              /* +0xA3C words sent one per transfer */
    u8 filler0A50[0xAE4 - 0xA50];
    u16 recv[4];                /* +0xAE4 SIOMULTI copy (alias of gUnk_03006644) */
    u32 saved;                  /* +0xAEC */
    u8 filler0AF0[0xAFC - 0xAF0];
    s32 i;                      /* +0xAFC */
};
extern struct LinkSio gUnk_03005B60;
extern u16 gUnk_03006644[4]; /* SIOMULTI0-3 snapshot (overlaps LinkSio.recv) */

void sub_08075F74(void)
{
    struct LinkSio *link;

    *(unsigned long long *)gUnk_03005B60.recv = *(volatile unsigned long long *)0x04000120;
    if (gUnk_03005B60.recv[0] == 0xFEFE && gUnk_03005B60.state > 9) {
        gUnk_03005B60.state = -3;
    } else if (gUnk_03005B60.state >= 0) {
        gUnk_03005B60.i = 0;
        do {
            gUnk_03005B60.rxBuf[gUnk_03005B60.i][gUnk_03005B60.state] = gUnk_03005B60.recv[gUnk_03005B60.i];
            gUnk_03005B60.i++;
        } while (gUnk_03005B60.i <= 1);
        if (gUnk_03005B60.state == 9) {
            u16 (*t)[12];

            gUnk_03005B60.saved = (u32)gUnk_03005B60.rxDone;
            t = gUnk_03005B60.rxDone;
            gUnk_03005B60.rxDone = gUnk_03005B60.rxBuf;
            gUnk_03005B60.rxBuf = t;
            gUnk_03005B60.dataReady = 1;
        }
    }
    link = &gUnk_03005B60;
    if (link->state <= 10) {
        link->state++;
    }
    if (link->master) {
        REG_TM3CNT_H = 0;
    }
    if (link->state <= 9) {
        if (link->state >= 0) {
            vu16 *siocnt = &REG_SIOCNT;

            siocnt[1] = link->txBuf[link->state];
        }
        if (link->master) {
            REG_SIOCNT |= 0x80;
            REG_TM3CNT_H = 0xC0;
        }
    }
}
extern const u16 gUnk_081A77A8[];

/* Sets the affine parameters of OAM group `idx` from a rotation angle
 * (low 7 bits, 0x80 = full turn) and a scale in the top nibble. */
#if 0 /* NONMATCHING: angle param: target extends angle in place in r1 and scale = r1>>12 (the build folds to >>28 from the unextended copy in r3); second/third table values swapped in r5/r6 */
void sub_0807609C(u16 idx, u16 angle)
{
    u8 *oam = (u8 *)gMain.oam;
    u16 a = gUnk_081A77A8[angle & 0x7F];
    u16 c = gUnk_081A77A8[(angle + 0x40) & 0x7F];
    u16 b = gUnk_081A77A8[(angle + 0x20) & 0x7F];
    s32 scale;

    oam += idx << 5;
    scale = angle >> 12;
    if (scale <= 7) {
        s16 d = scale + 1;

        a = (s16)a / d;
        b = (s16)b / d;
        c = (s16)c / d;
    } else {
        u16 m = scale - 8;

        a = (s16)a * m;
        b = (s16)b * m;
        c = (s16)c * m;
    }
    ((u16 *)oam)[3] = b;
    ((u16 *)oam)[7] = a;
    ((u16 *)oam)[11] = c;
    ((u16 *)oam)[15] = b;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080750E0", sub_0807609C); /* 0x0807609C size 0xA8 */
