/*
 * main (0x080750E0-0x08076144): the system core (wiki/functions/main-c.md).
 *
 * Small memory/string helpers (MemClear16, MemCopy16, CopyDoubleWords, StrCopy/StrCat, the
 * number formatters, FormatStr/FormatInt), key input (ReadKeys), the SRAM save (SaveGame),
 * scene switching (SetMainCallback), the VBlank/Timer2/Gamepak IRQ handlers, the brightness
 * and alpha fades, the OAM flush (FlushOamBuffer), the per-frame sync (FrameSyncUpdate),
 * MainLoop, GameInit/AgbMain, the link serial IRQ (LinkSerialIntr) and the OAM affine setter
 * (SetOamMatrixPacked). This unit defines most of the helpers declared in util.h.
 */
#include "global.h"
#include "gba.h"                    /* REG_* registers, SRAM/EWRAM/IWRAM/VRAM/OAM, BG_PLTT/OBJ_PLTT/OBJ_VRAM0 */
#include "link.h"                   /* struct LinkSio gLinkSio, struct LinkBuf gLinkBuf (timer2Ticks), gSioMultiRecv */
#include "util.h"                   /* MemClear16/MemCopy16/CopyDoubleWords, StrCopy/StrCat/FormatStr/FormatInt, Random */

/* One 8-byte OAM entry, as staged in gMain.oam before the flush to hardware OAM. */
struct OamEntry {
    u32 w0;
    u32 w1;
};

/*
 * gMain, the game's main state (0x03000040). Local view of include/main.h, kept for matching
 * (build/readability/issues/main.md): main.h declares callback as void (*)(void) but MainLoop
 * tests its return value (the scene callbacks return nonzero when done), intrCheck must stay
 * vu16 for the volatile discard read in MainLoop, and oam is an array of 8-byte entries here
 * rather than main.h's byte buffer. Field names and offsets are main.h's.
 */
struct Main {
    u32 rngState;                   /* +0x0000 */
    u16 heldKeys;                   /* +0x0004 */
    u16 newKeys;                    /* +0x0006 */
    u16 prevKeys;                   /* +0x0008 */
    u16 keyRepeatTimer;             /* +0x000A */
    u8 intrMainBuf[0x400];          /* +0x000C: IntrMain is copied here */
    vu16 intrCheck;                 /* +0x040C: bit 0: VBlank happened */
    u16 vblankFlags;                /* +0x040E */
    u16 (*callback)(void);          /* +0x0410: scene; returns nonzero when done */
    void (*vblankCallback)(void);   /* +0x0414 */
    void (*vblankCallbackEarly)(void); /* +0x0418 */
    u16 bgMapBuffer[8][0x400];      /* +0x041C */
    u16 unk441C;                    /* +0x441C */
    u16 unk441E;                    /* +0x441E */
    u16 bgVofs[4];                  /* +0x4420 */
    u16 bgHofs[4];                  /* +0x4428 */
    struct OamEntry oam[128];       /* +0x4430 */
    u8 oamCount;                    /* +0x4830 */
    u8 affineCount;                 /* +0x4831 */
    u8 brightness:6;                /* +0x4832 bits 0..5: fade level 0..0x1F */
    u8 brightnessFlags:2;           /* +0x4832 bits 6..7 */
    u8 unk4833;                     /* +0x4833 */
    s16 hblankY;                    /* +0x4834 */
    u16 hblankScroll[16];           /* +0x4836 */
    u8 unk4856;                     /* +0x4856 */
    u8 seqIndexCampaign;            /* +0x4857 */
    u8 seqState0;                   /* +0x4858 */
    u8 seqIndex1;                   /* +0x4859 */
    u8 seqState1;                   /* +0x485A */
    u8 seqState2;                   /* +0x485B */
    u16 currentBgm;                 /* +0x485C */
    u16 frameCounter;               /* +0x485E */
    u8 frameCounter8;               /* +0x4860 */
    u8 vblankCounter8;              /* +0x4861 */
    u16 lastVcount;                 /* +0x4862 */
    u16 vblankCounter;              /* +0x4864 */
    u16 lagCounter;                 /* +0x4866 */
    u16 lastSeFrame;                /* +0x4868 */
    u8 unk486A[6];                  /* +0x486A */
    u8 unk4870[8];                  /* +0x4870: bitfields and rewardPack in main.h */
    u8 seqIndexTop;                 /* +0x4878 */
    u8 seq4879;                     /* +0x4879 */
    u8 seq487A;                     /* +0x487A */
};
extern struct Main gMain;           /* 0x03000040 */

/*
 * The save image (0x02011C20). Local view of include/save.h, kept for matching
 * (build/readability/issues/main.md): this unit reads the +0x04 byte whole (flags4), where
 * save.h splits it into the language:7 / sjisText:1 bitfields. Only the fields this unit
 * touches are declared.
 */
struct SaveData {
    u32 unk0;
    u8 flags4;                      /* +0x04: bit 7 = sjisText (Shift-JIS text mode) */
};
extern struct SaveData gSaveData;   /* 0x02011C20 */

extern void (*IntrTable[16])(void); /* the SDK interrupt handler table */

struct ScrollReg {
    vu16 *reg;
    u16 mask;
    u16 pad;
};
extern const struct ScrollReg gBgHofsRegs[4]; /* 0x081A7764: HOFS registers, mask bits 4-7 */
extern const struct ScrollReg gBgVofsRegs[4]; /* 0x081A7784: VOFS registers, mask bits 8-11 */

/*
 * Cross-unit functions with no shared-header declaration usable here, plus this unit's own
 * functions called before their definitions. TextDrawSjisNumber/TextDrawLatinNumber go
 * through 3-argument views (text.h declares a 4th parameter the ROM call does not pass) and
 * the sound API has no prototypes in sound.h; see build/readability/issues/main.md.
 */
void TextDrawSjisNumber(u32 a, u32 b, u16 c);
void TextDrawLatinNumber(u32 a, u32 b, u16 c);
void UpdateSaveChecksum(void);
void WriteSram(const void *src, void *dst, u32 size);
u32 VerifySram(const void *src, void *dst, u32 size);
void SoundVBlank(void);
void SoundMain(void);
void ReadKeys(void);
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

void TextDrawNumber(u32 a, u32 b, u16 c)
{
    /* Matching: the empty barrier clobbering r3 keeps a and b in the callee-saved
     * registers r4/r5, as in the ROM. */
    __asm__ __volatile__("" : : : "r3");
    if (gSaveData.flags4 & 0x80)
        TextDrawSjisNumber(a, b, c);
    else
        TextDrawLatinNumber(a, b, c);
}
extern u8 gTextCanvas[];      /* 0x02000000: text bitmap; +0x10000 width, +0x10001 height (tiles) */

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

/* Key state plus D-pad auto-repeat: a held D-pad bit re-fires as a new key
 * once keyRepeatTimer passes 0x14 frames. */
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
    gMain.seqIndexTop = 0;
    gMain.seq4879 = 0;
    gMain.seq487A = 0;
    gMain.seqIndexCampaign = 0;
    gMain.seqState0 = 0;
    gMain.seqIndex1 = 0;
    gMain.seqState1 = 0;
    gMain.seqState2 = 0;
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
    gMain.seqState0 = 0;
    gMain.seqIndex1 = 0;
    gMain.seqState1 = 0;
    gMain.seqState2 = 0;
    return 1;
}
extern const u16 gSystemFontPal[];  /* 0x0822C300 */
extern const u16 gSystemTiles[];    /* 0x0822C320 */

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
    gLinkBuf.timer2Ticks++;
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
        gMain.affineCount = 0;
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

    /* DMA3 zero-fills of EWRAM and IWRAM from the stack zero (the bare dma[2] read waits
     * for each transfer, as in the ROM). */
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
    *(u32 *)0x03007FFC = (u32)gMain.intrMainBuf; /* the SDK IRQ vector slot */
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
/* LinkSerialIntr (below) is the serial IRQ handler of the SIO driver: struct LinkSio and
 * gLinkSio come from link.h; only the fields the IRQ touches are used here. */
void LinkSerialIntr(void)
{
    struct LinkSio *link;

    /* Snapshot SIOMULTI0-3 (0x04000120) as one 64-bit copy; going through gLinkSio lets the
     * compiler relate the state and base literals, as in the ROM. */
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
/* Matching: util.h declares gSineTable128 (0x081A77A8) as const s16 (signed loads); this unit
 * reads the table with unsigned loads (ldrh), so it keeps a u16 view under an alias symbol
 * (build/readability/issues/main.md). */
extern const u16 gSineTable128U16[] asm("gSineTable128");

/* Sets the affine parameters of OAM group `idx` from a rotation angle
 * (low 7 bits, 0x80 = full turn) and a scale in the top nibble. */
void SetOamMatrixPacked(u16 idx, u16 angle)
{
    u8 *oam = (u8 *)gMain.oam;
    u16 a = gSineTable128U16[angle & 0x7F];
    u16 b = gSineTable128U16[(angle + 0x20) & 0x7F];
    u16 c = gSineTable128U16[(angle + 0x40) & 0x7F];
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
