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
extern struct Main gMain;
#define gMain gMain

/* Save mirror (0x02011C20). */
struct SaveData {
    u32 unk0;
    u8 flags4;                      /* +0x04 bit 7: Shift-JIS text mode */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

extern void (*IntrTable[16])(void); /* IntrTable */

struct Timer2State {
    u8 filler0[0x82C];
    u16 counter;                    /* +0x82C incremented by Timer2Intr */
};
extern struct Timer2State gLinkBuf;

struct ScrollReg {
    vu16 *reg;
    u16 mask;
    u16 pad;
};
extern const struct ScrollReg gBgHofsRegs[4]; /* HOFS registers, mask bits 4-7 */
extern const struct ScrollReg gBgVofsRegs[4]; /* VOFS registers, mask bits 8-11 */

void TextDrawSjisNumber(u32 a, u32 b, u16 c);
void TextDrawLatinNumber(u32 a, u32 b, u16 c);
void UpdateSaveChecksum(void);
void WriteSram(const void *src, void *dst, u32 size);
u32 VerifySram(const void *src, void *dst, u32 size);
void SoundVBlank(void);
void SoundMain(void);
u16 Random(void);
void ReadKeys(void);
void CopyDoubleWords(void *dst, const void *src, u32 size);
void MemCopy16(void *dst, const void *src, u32 size);
void StrCat(char *dst, const char *src);
void StrCatNumber(char *dst, s32 n);
void ClearBlend(void);
void FlushOamBuffer(void);
void SaveGame(void);
u16 CB_MainMenu(void);
void GamepakIntr(void);
void Timer2Intr(void);
void VBlankIntr(void);
void SoundDma1Intr(void);
u16 CB_License(void);
void ReadSram(const void *src, void *dst, u32 size);
void SoundInit(u32 a);
void ResetBgScroll(void);
void SetBrightnessWhite(void);
void SetSeEnabled(u32 a);
void SetBgmEnabled(u32 a);
void SetTextModeLatin(void);
void DebugHook_Nop(void);
void FrameSyncUpdate(void);
void MainLoop(void);
void GameInit(void);
extern vu16 gMain_intrCheck;

void TextDrawNumber(u32 a, u32 b, u16 c)
{
    /* Keep both coordinates in the callee-saved registers used by the ROM. */
    __asm__ __volatile__("" : : : "r3");
    if (gSaveData.flags4 & 0x80)
        TextDrawSjisNumber(a, b, c);
    else
        TextDrawLatinNumber(a, b, c);
}
extern u8 gTextCanvas[];      /* text bitmap; +0x10000 width, +0x10001 height (tiles) */

/* Converts the 1-byte-per-pixel bitmap at 0x02000000 into 4bpp tiles at
 * dst; pixels of value 0 take the background nibble from bits 0-3 of bg. */
void TextCanvasToTiles(u16 *dst, u16 bg)
{
    s32 i, next;

    /* FAKEMATCH: the do-while(0) wrapper swaps the fill/row-counter registers (r4/r5) */
    do {
        bg &= 0xF;
        bg |= bg << 4;
        bg |= bg << 8;
    } while (0);
    for (i = 0; i < gTextCanvas[0x10000] * gTextCanvas[0x10001]; i = next) {
        const u8 *src = gTextCanvas + i * 64;
        s32 j;

        next = i + 1;
        for (j = 15; j >= 0; j--) {
            *dst = bg;
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

void ReadKeys(void)
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
void MemClear16(void *dstp, s32 size)
{
    u16 *dst = dstp;

    size = (size + 1) / 2;
    while (size != 0) {
        *dst++ = 0;
        size--;
    }
}
void MemCopy16(void *dstp, const void *srcp, u32 size)
{
    u16 *dst = dstp;
    const u16 *src = srcp;

    size = (size + 1) / 2;
    while (size != 0) {
        *dst++ = *src++;
        size--;
    }
}

void CopyDoubleWords(void *dstp, const void *srcp, u32 size)
{
    const struct OamEntry *src = srcp;
    struct OamEntry *dst = dstp;

    size = (size + 7) >> 3;
    while (size != 0) {
        *dst++ = *src++;
        size--;
    }
}
void StrCopy(char *dst, const char *src)
{
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
}
void StrCat(char *dst, const char *src)
{
    while (*dst != 0) {
        dst++;
    }
    while (*src != 0) {
        *dst++ = *src++;
    }
    *dst = 0;
}
void StrCatNumberFullwidth(char *dst, s32 n)
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
    StrCat(dst, (char *)&buf[i + 1]);
}
void StrCatNumber(char *dst, s32 n)
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
    StrCat(dst, &buf[i + 1]);
}
s32 StrLen(const char *s)
{
    s32 n = 0;

    while (*s != 0) {
        n++;
        s++;
    }
    return n;
}
s32 StrLenWide(const u16 *s)
{
    s32 n = 0;

    while (*(const u8 *)s != 0) {
        s++;
        n++;
    }
    return n;
}
void FormatStr(char *dst, const char *fmt, const char *arg)
{
    while (*fmt != 0) {
        if (*fmt == '%' && fmt[1] == 's') {
            *dst = 0;
            fmt += 2;
            StrCat(dst, arg);
            StrCat(dst, fmt);
            return;
        }
        *dst++ = *fmt++;
    }
}
void FormatInt(char *dst, const char *fmt, s32 arg)
{
    while (*fmt != 0) {
        if (*fmt == '%' && fmt[1] == 'd') {
            *dst = 0;
            fmt++;
            StrCatNumber(dst, arg);
            fmt++;
            StrCat(dst, fmt);
            return;
        }
        *dst++ = *fmt++;
    }
}
s32 RoundTo10(s32 x)
{
    return (x + 5) / 10 * 10;
}
s32 HalveRoundUp(s32 x)
{
    x = x * 5;
    x += 5;
    return x / 10;
}
s32 HalveRoundDown(s32 x)
{
    x = x * 5;
    x += 4;
    return x / 10;
}
void SaveGame(void)
{
    s32 i;

    UpdateSaveChecksum();
    i = 0;
    do {
        WriteSram(&gSaveData, (void *)SRAM, 0x2170);
        if (VerifySram(&gSaveData, (void *)SRAM, 0x2170) == 0) {
            break;
        }
        i++;
    } while (i <= 31);
}
void SetMainCallback(u16 (*cb)(void))
{
    SaveGame();
    gMain.vblankCallbackEarly = NULL;
    gMain.vblankCallback = NULL;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    IntrTable[1] = NULL;
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
u32 SaveAndResetSceneState(void)
{
    SaveGame();
    gMain.vblankCallbackEarly = NULL;
    gMain.vblankCallback = NULL;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    IntrTable[1] = NULL;
    REG_IME = 1;
    gMain.seq4858 = 0;
    gMain.seq4859 = 0;
    gMain.seq485A = 0;
    gMain.seq485B = 0;
    return 1;
}
extern const u16 gSystemFontPal[];
extern const u16 gSystemTiles[];

void LoadSystemGfx(void)
{
    MemCopy16((void *)BG_PLTT, gSystemFontPal, 0x20);
    *(u16 *)BG_PLTT = 0;
    MemCopy16((void *)(VRAM + 0x4000), gSystemTiles, 0x200);
    MemCopy16((void *)OBJ_PLTT, gSystemFontPal, 0x20);
    MemCopy16((void *)OBJ_VRAM0, gSystemTiles, 0x200);
    *(u16 *)OBJ_PLTT = 0;
}
void VBlankIntr(void)
{
    gMain.intrCheck |= 1;
    gMain.vblankCounter++;
    gMain.lagCounter++;
    if (gMain.vblankCallbackEarly != NULL) {
        gMain.vblankCallbackEarly();
    }
    SoundVBlank();
    if (gMain.vblankCallback != NULL) {
        gMain.vblankCallback();
    }
    gMain.vblankCounter8++;
}
void Timer2Intr(void)
{
    REG_TM2CNT_L = 0xF400;
    REG_TM2CNT_H |= 0xC3;
    gLinkBuf.counter++;
}
void GamepakIntr(void)
{
    while (1) {
    }
}
void ResetBgHofs(void)
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
void ResetBgVofs(void)
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
void ResetBgScroll(void)
{
    ResetBgHofs();
    ResetBgVofs();
}
void SetBrightnessFull(u16 bldcnt)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = bldcnt;
    REG_BLDY = gMain.brightness;
}
void ClearBlend(void)
{
    gMain.brightness = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
}
u32 FadeBrightnessDown(u16 bldcnt)
{
    u32 ret;

    if (gMain.brightness > 2) {
        gMain.brightness -= 2;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        ClearBlend();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = bldcnt;
        ret = 0;
    }
    return ret;
}
u32 FadeBrightnessUp(u16 bldcnt)
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
u32 FadeAlphaDown(u16 bldcnt)
{
    u32 ret;

    if (gMain.brightness > 2) {
        gMain.brightness -= 2;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        ClearBlend();
        ret = 1;
    } else {
        REG_BLDALPHA = gMain.brightness + ((0x1F - gMain.brightness) << 8);
        REG_BLDCNT = bldcnt;
        ret = 0;
    }
    return ret;
}
u32 FadeAlphaUp(u16 bldcnt)
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
void SetBrightnessBlack(void)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = 0x3FFF;
    REG_BLDY = gMain.brightness;
}
void SetBrightnessWhite(void)
{
    gMain.brightness = 0x1F;
    REG_BLDCNT = 0x3FBF;
    REG_BLDY = gMain.brightness;
}
u32 FadeToBlack(s32 step)
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
u32 FadeFromBlack(s32 step)
{
    u32 ret;

    if (gMain.brightness > step) {
        gMain.brightness -= step;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        ClearBlend();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x3FFF;
        ret = 0;
    }
    return ret;
}
u32 FadeToWhite(s32 step)
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
u32 FadeFromWhite(s32 step)
{
    u32 ret;

    if (gMain.brightness > step) {
        gMain.brightness -= step;
    } else {
        gMain.brightness = 0;
    }
    if (gMain.brightness == 0) {
        ClearBlend();
        ret = 1;
    } else {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x3FBF;
        ret = 0;
    }
    return ret;
}
void FlushOamBuffer(void)
{
    s32 i;
    u32 *p;
    u8 *q;

    if (gMain.vblankFlags & 1) {
        CopyDoubleWords((void *)OAM, gMain.oam, 0x400);
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
void FrameSyncUpdate(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gMain.vblankFlags & gBgHofsRegs[i].mask) {
            *gBgHofsRegs[i].reg = gMain.bgHofs[i];
        }
        if (gMain.vblankFlags & gBgVofsRegs[i].mask) {
            *gBgVofsRegs[i].reg = gMain.bgVofs[i];
        }
    }
    if (gMain.vblankFlags & 2) {
        for (i = 0; i < 8; i++) {
            CopyDoubleWords((void *)(VRAM + i * 0x800), gMain.bgMapBuffer[i], 0x800);
        }
    }
    FlushOamBuffer();
    ReadKeys();
    SoundMain();
    Random();
}
void DebugHook_Nop(void)
{
}
void MainLoop(void)
{
    while (1) {
        gMain.intrCheck &= 0xFFFE;
        while (!(gMain.intrCheck & 1)) {
        }
        FrameSyncUpdate();
        gMain.lagCounter = 0;
        if (gMain.callback()) {
            SetMainCallback(CB_MainMenu);
        }
        DebugHook_Nop();
        gMain.frameCounter++;
        gMain.frameCounter8++;
    }
}
void GameInit(void)
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
    IntrTable[0] = NULL;
    IntrTable[1] = NULL;
    IntrTable[2] = VBlankIntr;
    IntrTable[3] = NULL;
    IntrTable[4] = NULL;
    IntrTable[5] = NULL;
    IntrTable[6] = Timer2Intr;
    IntrTable[7] = NULL;
    IntrTable[8] = NULL;
    IntrTable[9] = SoundDma1Intr;
    IntrTable[10] = NULL;
    IntrTable[11] = NULL;
    IntrTable[12] = NULL;
    IntrTable[13] = GamepakIntr;
    IntrTable[14] = NULL;
    IntrTable[15] = NULL;
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
    gMain.callback = CB_License;
    gMain.vblankCallback = NULL;
    SoundInit(0);
    REG_TM2CNT_L = 0xF400;
    REG_TM2CNT_H |= 0xC3;
    gMain.vblankFlags = 3;
    ResetBgScroll();
    SetBrightnessWhite();
    SetSeEnabled(1);
    SetBgmEnabled(1);
    SetTextModeLatin();
}
void AgbMain(void)
{
    GameInit();
    MainLoop();
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
    u16 recv[4];                /* +0xAE4 SIOMULTI copy (alias of gSioMultiRecv) */
    u32 saved;                  /* +0xAEC */
    u8 filler0AF0[0xAFC - 0xAF0];
    s32 i;                      /* +0xAFC */
};
extern struct LinkSio gLinkSio;
extern u16 gSioMultiRecv[4]; /* SIOMULTI0-3 snapshot (overlaps LinkSio.recv) */

void LinkSerialIntr(void)
{
    struct LinkSio *link;

    *(unsigned long long *)gLinkSio.recv = *(volatile unsigned long long *)0x04000120;
    if (gLinkSio.recv[0] == 0xFEFE && gLinkSio.state > 9) {
        gLinkSio.state = -3;
    } else if (gLinkSio.state >= 0) {
        gLinkSio.i = 0;
        do {
            gLinkSio.rxBuf[gLinkSio.i][gLinkSio.state] = gLinkSio.recv[gLinkSio.i];
            gLinkSio.i++;
        } while (gLinkSio.i <= 1);
        if (gLinkSio.state == 9) {
            u16 (*t)[12];

            gLinkSio.saved = (u32)gLinkSio.rxDone;
            t = gLinkSio.rxDone;
            gLinkSio.rxDone = gLinkSio.rxBuf;
            gLinkSio.rxBuf = t;
            gLinkSio.dataReady = 1;
        }
    }
    link = &gLinkSio;
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
extern const u16 gSineTable128[];

/* Sets the affine parameters of OAM group `idx` from a rotation angle
 * (low 7 bits, 0x80 = full turn) and a scale in the top nibble. */
void SetOamMatrixPacked(u16 idx, u16 angle)
{
    u8 *oam = (u8 *)gMain.oam;
    u16 a = gSineTable128[angle & 0x7F];
    u16 b = gSineTable128[(angle + 0x20) & 0x7F];
    u16 c = gSineTable128[(angle + 0x40) & 0x7F];
    u32 t;
    s32 scale;

    oam += idx << 5;
    t = angle;
    asm("" : "+r"(t)); /* FAKEMATCH: hides angle's zero-extension from cse so >>12 is not folded into >>28 */
    scale = t >> 12;
    if (scale <= 7) {
        a = (s16)a / (scale + 1);
        b = (s16)b / (scale + 1);
        c = (s16)c / (scale + 1);
    } else {
        a = (s16)a * (scale - 8);
        b = (s16)b * (scale - 8);
        c = (s16)c * (scale - 8);
    }
    ((u16 *)oam)[3] = b;
    ((u16 *)oam)[7] = a;
    ((u16 *)oam)[11] = c;
    ((u16 *)oam)[15] = b;
}
