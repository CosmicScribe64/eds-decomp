#include "global.h"

#include "gba.h"

/*
 * Password scene (CB 0x0807CC28, step table 0x081A7970) and Card Trading
 * scene (CB 0x0807D348, step table 0x081A79A4). See wiki/functions/code-0807c7c8.md.
 */

/* Main state at 0x03000040: only the fields used here. */
struct Main {
    u8 filler0[6];
    u16 keyNew;                 /* +0x06 newly pressed keys */
    u8 filler8[0x40E - 8];
    u16 unk40E;
    u8 filler410[4];
    u32 unk414;
    u8 filler418[0x85C - 0x418];
    u16 tilemap[0x60];          /* +0x85C */
    u8 filler91C[0x442C - 0x91C];
    u16 unk442C;
    u8 filler442E[0x4859 - 0x442E];
    u8 step;                    /* +0x4859 scene step index */
    u8 sub;                     /* +0x485A sub-step */
    u8 sub2;                    /* +0x485B */
    u8 filler485C[0x16];
    u16 unk4872;
    u8 unk4874_0:2;
};

/* Password / trade scene state at 0x0201F780. */
struct PwState {
    u16 code;                   /* +0x00 */
    u16 a:1;                    /* +0x02 */
    u16 b:1;
    u16 c:7;                    /* bits 2-8 */
    u16 d:4;                    /* bits 9-12 */
    u16 e:3;
    u8 filler4[0xC];
    u8 link[0x4];               /* +0x10 link sync tx entry */
    u8 linkRx[2];               /* +0x14 link rx entry */
    u16 peerData;               /* +0x16 data received from the peer */
    u8 link18[6];
    u16 timer;                  /* +0x1E */
    u16 card;                   /* +0x20 */
    u8 unk22;
    u8 filler23[5];
};

/* Link / password menu state at 0x0201F7B0 (size 0x18), see code_0807B6B8. */
struct LinkState {
    u8 digits[8];               /* +0x00 */
    u8 pw[8];                   /* +0x08 */
    u8 slot:4;                  /* +0x10 selected digit slot */
    u8 cursor:4;                /* +0x10 cursor entry */
    u8 a:6;                     /* +0x11 */
    u32 b:6;
    u16 c:12;                   /* +0x12 (>> 4) frame counter */
    u16 card;                   /* +0x14 */
    u8 filler16[2];
};

/* Save mirror at 0x02011C20: per-card entries of 4 bytes starting at +0xA. */
struct SaveCard {
    u8 filler0[2];
    u8 flag0:1;
    u8 flag1:1;
    u8 filler[1];
};
struct SaveMirror {
    u8 filler0[8];
    struct SaveCard card[1];
};

extern struct Main gMain;
extern struct LinkState gPassword;
extern struct SaveMirror gSaveData;
int Random(void);
void MemClear16(void *dst, u32 size);
void MemCopy16(void *dest, const void *src, u32 size);
void PlaySE(int);
void Password_DrawDigits(void);
void ClearBgMapBuffers(void);
void ResetVideo(void);
void LoadSystemGfx(void);
void SetBrightnessBlack(void);
void ResetBgScroll(void);
void LoadBgImage4bppMap1(u32 a, u32 b, u32 c, const void *d);
int StrLenWide(const void *p);
void TextDrawString(int x, int y, u16 attr, const void *str);
void TextCanvasToTiles(void *dest, u16 v);
void TextCanvasInit(u8 a, u8 b);
struct Hblank {
    u32 filler0;
    void *cb;
};
extern struct Hblank IntrTable;
extern const u8 gCardTradingButtonsPal[], gCardTradingButtonsDimPal[], gCardTradingCardPal[];
extern const u8 gCardTradingButtonsGfx[], gCardTradingButtonsDimGfx[], gCardTradingCardGfx[];
extern const u8 gSystemFontPal[], gCardNames[][64], gCardTradingBgImage[];
void AddSprite(u32 a, u32 b, u16 c);
void AddAffineSprite(u32 a, u16 b, u16 c, u32 d);
struct Pair16 {
    u16 lo;
    u16 hi;
};
extern const u32 gPasswordCardSlideHofs[];
extern const struct Pair16 gPasswordArrowFrames[];
void Password_DrawKeyCursor(void);
void Password_DrawCard(u16 id);
extern struct PwState gCardTrading;
u16 FadeToBlack(int);
u16 FadeFromBlack(int);
u16 FadeToBlack(int);
u16 FadeFromBlack(int);
void CardTrading_DrawMenu(int a, u16 b, int c);
u32 CB_CardTrading(void);
void CardDetail_Init(u16 card, int a, int b);
u16 CardDetail_Run(void);
u16 TradeCardSelect_Run(void);
void LinkSyncStart(u8 *);
u32 LinkSyncStep(u16 id, u16 data, void *p);
u32 LinkSyncClose(void *a, void *b);
void AddCardToTrunk(u16);
void RemoveCardFromTrunk(u16);
void SaveGame(void);
extern const u16 gCardIdToNumber[];
extern const u16 gCardNumberToId[];
void DebugPrintf(const void *);
void DebugPrintFlush(void);
extern const u8 gStrDebugThrowItInNow[];
extern u16 (*const gPasswordSteps[])(void);
extern u16 (*const gCardTradingSteps[])(void);

/* Second view of the a/b flag pair as one 2-bit value. */
struct PwPair {
    u16 filler0;
    u16 ab:2;
    u16 rest:14;
    u8 filler4[0x24];
};

u32 Password_RollAndCheck(void)
{
    struct LinkState *s = &gPassword;

    s->a = 0x20;
    s->b = 0x20;
    Password_DrawDigits();
    Password_DrawKeyCursor();
    if (gMain.keyNew & 1)
        s->c = 0xB4;
    if (s->c++ <= 0xB3) {
                int i;
        u32 base;

        i = 0;
        base = (u32)s;

        for (; i < 8; i++)
            *(u8 *)(i + base) = Random() % 10;
        gPassword.cursor = Random() % 10;
        gPassword.slot = Random() & 7;
        if ((gPassword.c & 0xF) == 8)
            PlaySE(0x27);
    } else {
        s->c = 0;
        s->a = 0;
        s->b = 0;
        if (s->card == 0) {
            MemClear16(s, 8);
            gMain.step += 3;
        } else if (gSaveData.card[s->card].flag1 == 0) {
            MemCopy16(s, s->pw, 8);
            Password_DrawCard(s->card);
            gMain.unk442C = 0;
            REG_BG2HOFS = 0;
            return 1;
        } else {
            MemClear16(s, 8);
            gMain.step += 6;
        }
    }
    return 0;
}

u32 Password_RevealAndGiveCard(void)
{
    if (gMain.keyNew & 1)
        gPassword.c = 0x12C;
    if (gPassword.c <= 0x12B) {
        gPassword.a++;
        if ((gPassword.a & 0x1F) == 0x1F)
            PlaySE(0x29);
        if ((gPassword.a & 0x1F) <= 0x1C)
            Password_DrawDigits();
        if (gPassword.c <= 0x1F) {
            gMain.unk442C = gPasswordCardSlideHofs[gPassword.c];
            AddSprite(0x1C0070, 0, gPasswordArrowFrames[gPassword.c].lo);
            AddSprite(0x4C0070, 0, gPasswordArrowFrames[gPassword.c].lo);
            AddSprite(0x7C0070, 0, gPasswordArrowFrames[gPassword.c].lo);
        }
        gPassword.c++;
        return 0;
    }
    switch (gMain.sub) {
    case 0:
        Password_DrawDigits();
        if (gMain.keyNew & 3)
            gMain.sub++;
        return 0;
    case 1:
        Password_DrawDigits();
        if (FadeToBlack(4)) {
            gSaveData.card[gPassword.card].flag1 = 1;
            AddCardToTrunk(gPassword.card);
            CardDetail_Init(gPassword.card, 0, 0);
            gMain.sub++;
        }
        return 0;
    case 2:
        if (CardDetail_Run())
            gMain.sub++;
        return 0;
    default:
        return 1;
    }
}
u32 Password_ShowError(void)
{
    if (gMain.keyNew & 1)
        gPassword.c = 0x12C;
    if (gPassword.c <= 0x12B) {
        if (gPassword.c <= 0xB3) {
            gPassword.a++;
            if ((gPassword.a & 0x1F) == 0x1F)
                PlaySE(0x28);
        }
        if (gPassword.a & 0x20) {
            AddSprite(0x80018, 0x40, 0x10CA);
            AddSprite(0x80028, 0x4080, 0x10CC);
        }
        if (!(gMain.keyNew & 8)) {
            gPassword.c++;
            return 0;
        }
    }
    return 1;
}

u16 Password_FadeOut(void)
{
    return FadeToBlack(2);
}
u32 Password_ShowUsed(void)
{
    if (gMain.keyNew & 1)
        gPassword.c = 0x12C;
    if (gPassword.c <= 0x12B) {
        if (gPassword.c <= 0xB3) {
            gPassword.a++;
            if ((gPassword.a & 0x1F) == 0x1F)
                PlaySE(0x28);
        }
        if (gPassword.a & 0x20)
            AddSprite(0x80028, 0x4080, 0x110B);
        if (!(gMain.keyNew & 8)) {
            gPassword.c++;
            return 0;
        }
    }
    return 1;
}
/* Password scene callback: runs the current step of the table. */
u32 CB_Password(void)
{
    u16 (*fn)(void) = gPasswordSteps[gMain.step];

    if (fn) {
        if (fn()) {
            gMain.step++;
            gMain.sub = 0;
            gMain.sub2 = 0;
        }
        return 0;
    }
    return 1;
}
/* Copies `rows` rows of `width` tiles into VRAM 0x06010000 (stride 0x400 bytes). */
void CardTrading_LoadObjTiles(const void *src, int tile, int width, int rows)
{
    u8 *dst = (u8 *)0x06010000 + tile * 32;
    int i;

    for (i = 0; i < rows; i++) {
        MemCopy16(dst, src, width * 32);
        dst += 0x400;
        src = (const u8 *)src + width * 32;
    }
}
void CardTrading_DrawMenu(int a, u16 b, int c)
{
    int i, j, limit;

    if (b) {
        limit = 2;
        a &= 1;
    } else {
        limit = 1;
        a = 0;
    }
    i = 0;
    while (i < limit) {
        int iy, next;
        u32 yx;

        j = 0;
        iy = i << 7;
        next = i + 1;
        yx = (i << 21) + 0x300000;
        for (; j < 4; j++) {
            int on = 0;
            int x, y;

            if (i == a)
                on = 1;
            x = 0;
            if (on == 0)
                x = 0x10;
            y = 0;
            if (on == 0)
                y = 0x1000;
            AddSprite(yx | (j << 5), 0x80, (x + j * 4 + iy) | y);
        }
        i = next;
    }
    if (b) {
        AddAffineSprite(0x380098, 0x80, ((c & 0xC) + 0x100) | 0x2000, ((c << 21) + 0x1000000) | gCardTrading.c);
        if (c == 0)
            gCardTrading.c = gCardTrading.c + 1;
    }
}
u32 CardTrading_ClearState(void)
{
    MemClear16(&gCardTrading, 0x28);
    return 1;
}
u32 CardTrading_InitVideo(void)
{
    int len;
    u8 w;
    int half;
    int extent;
    int x;

    gMain.unk40E = 3;
    REG_DISPCNT = 0;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x105;
    ResetVideo();
    ClearBgMapBuffers();
    LoadSystemGfx();
    REG_MOSAIC = 0;
    SetBrightnessBlack();
    ResetBgScroll();
    gMain.unk414 = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= ~2;
    IntrTable.cb = 0;
    REG_IME = 1;
    MemCopy16((void *)0x05000200, gCardTradingButtonsPal, 0x20);
    MemCopy16((void *)0x05000220, gCardTradingButtonsDimPal, 0x20);
    MemCopy16((void *)0x05000240, gCardTradingCardPal, 0x20);
    CardTrading_LoadObjTiles(gCardTradingButtonsGfx, 0, 0x10, 8);
    CardTrading_LoadObjTiles(gCardTradingButtonsDimGfx, 0x10, 0x10, 8);
    CardTrading_LoadObjTiles(gCardTradingCardGfx, 0x100, 0x10, 8);
    MemCopy16((void *)0x05000000, gSystemFontPal, 0x20);
    LoadBgImage4bppMap1(0, 0x10, 0x200, gCardTradingBgImage);
    if (gCardTrading.code != 0) {
        int i;

        len = StrLenWide(gCardNames[gCardTrading.code]);
        w = 12;
        if (len > 0x12)
            w = 10;
        TextCanvasInit(0x20, 3);
        extent = (len * w) >> 1;
        x = 0x78 - extent;
        half = w >> 1;
        TextDrawString(x, 0xD - half, (w << 8) | 8, gCardNames[gCardTrading.code]);
        TextDrawString(0x77 - extent, 0xC - half, (w << 8) | 7, gCardNames[gCardTrading.code]);
        TextCanvasToTiles((void *)0x06005000, 0);
        for (i = 0; i < 0x60; i++)
            gMain.tilemap[i] = 0x80 + i;
    }
    return 1;
}

u32 CardTrading_FadeIn(void)
{
    CardTrading_DrawMenu(gCardTrading.b, gCardTrading.a, gCardTrading.d);
    REG_DISPCNT |= 0x1300;
    return FadeFromBlack(2);
}
u32 CardTrading_HandleInput(void)
{
    struct PwState *st = &gCardTrading;

    CardTrading_DrawMenu(st->b, st->a, st->d);
    if (gMain.keyNew & 2) {
        PlaySE(2);
        gMain.step = 4;
    } else if (gMain.keyNew & 1) {
        if (((struct PwPair *)st)->ab != 3)
            gMain.step = 6;
        else
            gMain.step = 8;
        PlaySE(1);
    } else if (gMain.keyNew & 0xC0) {
        if (st->a) {
            PlaySE(0);
            st->b = 1 - st->b;
        } else {
            PlaySE(3);
        }
    }
    return 0;
}
u32 CardTrading_UnusedReturnFalse(void)
{
    return 0;
}
u32 CardTrading_FadeOut(void)
{
    CardTrading_DrawMenu(gCardTrading.b, gCardTrading.a, gCardTrading.d);
    if (FadeToBlack(2) != 0) {
        REG_DISPCNT = 0;
        return 1;
    }
    return 0;
}
u32 CardTrading_SelectCard(void)
{
    if (TradeCardSelect_Run()) {
        if (gMain.unk4872 != 0) {
            gCardTrading.code = gMain.unk4872;
            gCardTrading.a = 1;
            gCardTrading.b = 1;
        } else {
            gCardTrading.code = 0;
            gCardTrading.a = 0;
            gCardTrading.b = 0;
        }
        gMain.step = 1;
        gMain.sub = 0;
        gMain.sub2 = 0;
    }
    return 0;
}
u32 CardTrading_ThrowCard(void)
{
    struct PwState *st = &gCardTrading;

    CardTrading_DrawMenu(st->b, st->a, st->d);
    if (st->c != 0x60) {
        if (st->c <= 0x5B)
            st->c += 4;
    } else {
        if (st->d > 14) {
            LinkSyncStart(st->link);
            DebugPrintf(gStrDebugThrowItInNow);
            DebugPrintFlush();
            st->timer = 0x200;
            return 1;
        }
        st->d++;
    }
    return 0;
}
u32 CardTrading_ReverseThrow(void)
{
    CardTrading_DrawMenu(gCardTrading.b, gCardTrading.a, gCardTrading.d);
    if (gCardTrading.d > 1) {
        gCardTrading.d--;
        return 0;
    }
    return 1;
}
u32 CardTrading_Exchange(void)
{
    CardTrading_DrawMenu(gCardTrading.b, gCardTrading.a, gCardTrading.d);
    if (LinkSyncStep(gCardTrading.unk22, ((const u16 *)0x08622AB4)[gCardTrading.code & 0x7FF], gCardTrading.link)) {
        u16 id;
        u32 n;

        LinkSyncClose(gCardTrading.filler4, gCardTrading.linkRx);
        id = gCardTrading.peerData;
        if (id == 0xFFFF)
            n = 0;
        else if (id < 2000)
            n = ((const u16 *)0x08623DF4)[id & 0x7FF];
        else
            n = ((const u16 *)0x08623DF4)[(id - 2000) & 0x7FF] + 1;
        gCardTrading.card = n;
        AddCardToTrunk(gCardTrading.card);
        RemoveCardFromTrunk(gCardTrading.code);
        SaveGame();
        return 1;
    }
    if (--gCardTrading.timer == 0)
        gMain.step = 0xE;
    return 0;
}
u32 CardTrading_ShowReceivedCard(void)
{
    switch (gMain.sub) {
    case 0:
        CardDetail_Init(gCardTrading.card, 0, 0);
        gMain.sub++;
        break;
    case 1:
        if (CardDetail_Run())
            gMain.sub++;
        break;
    default:
        gCardTrading.code = 0;
        gCardTrading.b = 0;
        gCardTrading.a = 0;
        gCardTrading.d = 0;
        gMain.step = 1;
        break;
    }
    return 0;
}
/* Card Trading scene callback. */
u32 CB_CardTrading(void)
{
    gCardTrading.unk22 = 0x50;
    gMain.unk4874_0 = 1;
    {
        u16 (*fn)(void) = gCardTradingSteps[gMain.step];

        if (fn) {
            if (fn()) {
                gMain.step++;
                gMain.sub = 0;
                gMain.sub2 = 0;
            }
            return 0;
        }
        return 1;
    }
}
u16 CardTrading_UnusedCallbackWrapper(void)
{
    return CB_CardTrading();
}
