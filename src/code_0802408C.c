#include "global.h"
#include "gba.h"

/* Card location descriptor (4 bytes) passed to the card-move animation sub_080242C4. */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4 */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;          /* +0x000 bit 0 */
    u8 cursorOn:1;      /* +0x000 bit 1: draw the field cursor sprite */
    u8 active:1;        /* +0x000 bit 2: duel screen running (per-frame update enabled) */
    u8 unk0_3:5;
    u8 unk1[3];
    u8 scroll;          /* +0x004: current field scroll (copied to gMain+0x4422/0x4424) */
    u8 scrollFrom;      /* +0x005 */
    u8 scrollTo;        /* +0x006 */
    u8 scrollSteps:4;   /* +0x007 bits 0-3: interpolation steps left (4 = start) */
    u8 unk7_4:4;
    u8 tiles[0x800];    /* +0x008: tile buffer copied to VRAM 0x060091C0 */
    u16 tilesDirty:1;   /* +0x808 bit 0 */
    u16 flag808_1:1;    /* +0x808 bit 1: call sub_0805ED78 after the tile copy */
    u16 cursorDone:1;   /* +0x808 bit 2: cursor move finished (sub_0805F96C is called) */
    u16 cursorShow:1;   /* +0x808 bit 3 */
    u16 cursorFlipA:1;  /* +0x808 bit 4: OAM attr1 0x40 (hypothesis) */
    u16 cursorFlipB:1;  /* +0x808 bit 5: OAM attr2 0x30 (hypothesis: palette) */
    u16 cursorSteps:4;  /* +0x808 bits 6-9: interpolation steps left */
    u16 unk808_10:6;
    u8 filler80A[2];
    s32 cursorX;        /* +0x80C */
    s32 cursorY;        /* +0x810 */
    s32 cursorFromX;    /* +0x814 */
    s32 cursorFromY;    /* +0x818 */
    s32 cursorToX;      /* +0x81C */
    s32 cursorToY;      /* +0x820 */
    s32 player824;      /* +0x824 */
    s32 zone828;        /* +0x828 */
    s32 idx82C;         /* +0x82C */
    u8 animActive:1;    /* +0x830 bit 0: card animation pending */
    u8 animKind:7;      /* +0x830 bits 1-7: 1..5, dispatched by sub_080243CC */
    u8 filler831[3];
    u32 animArg;        /* +0x834: card id and other fields */
    u8 animStep;        /* +0x838 */
    u8 animTimer;       /* +0x839 */
    u16 animArg83A;     /* +0x83A */
    u16 animArg83C;     /* +0x83C */
    u8 filler83E[2];
    struct CardLoc from; /* +0x840 */
    struct CardLoc to;   /* +0x844 */
    u8 unk848[1];        /* +0x848 (= 0x0201D7F8): sub-object passed to sub_08076xxx */
};

extern struct DuelScreen gUnk_0201CFB0;
#define sScreen gUnk_0201CFB0

extern const u16 gUnk_081A43A4[][16];
struct ZoneAnimEntry { u32 unk0; u32 unk4; };
extern const struct ZoneAnimEntry gUnk_081A42A4[][16];

void sub_0805D58C(void);
void sub_0805D708(void);
void sub_0805D848(void);
void sub_0805DA1C(void);
void sub_0805DB90(void);
s32 sub_0807B4D0(s32 a, s32 b);     /* 8.8 fixed-point multiply */

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[6];
    u16 opts;           /* +0x6: options (bit 7, bits 8-14, bit 15) */
    u8 unk8[3];
    u8 step;            /* +0xB: sequence step, index into gUnk_08198FAC */
};
extern struct DuelMsg gUnk_02017A30;
extern u16 (*const gUnk_08198FAC[])(void);

/* One animated token of the toss animation (12 bytes). */
struct TossSlot {
    u8 timer;           /* +0x0: frames until next anim frame */
    u8 frame;           /* +0x1: animation frame 0-7 */
    u8 state;           /* +0x2: 1 = moving */
    u8 unk3;
    s16 y;              /* +0x4: height (8.8) */
    u16 t;              /* +0x6: time parameter */
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
};

struct Toss {
    struct TossSlot slots[8];   /* +0x00 */
    u8 count;           /* +0x60: number of tokens (opts bits 8-14) */
    u8 next;            /* +0x61: next token to launch */
    u8 mode;            /* +0x62: 0 or 4 (opts bit 15) */
    u8 unk63;           /* +0x63: 0 or 10 (opts bit 7) */
    u8 landed;          /* +0x64: tokens that landed with result 0 */
    u8 done;            /* +0x65: set when sub_080251C8 reports all finished */
};

/* Overlay work area at 0x02015280 (0xC58 bytes, fields used here). */
struct Work15280 {
    u8 unk0[0x618];
    struct {
        u16 unk0;
        u16 unk2;
        u8 unk4[0x14];
    } unk618[3];                /* +0x618: passed to sub_0807B534 */
    u8 filler660[0xAAC - 0x660];
    struct Toss toss;           /* +0xAAC (0x68 bytes) */
    struct {
        u8 unk0;
        u8 unk1;
        u8 unk2;
        u8 unk3;
    } unkB14[4];                /* +0xB14: [0] is passed to sub_08027CB8/sub_08027CDC, [1] to sub_08027D1C */
    u8 unkB24;
    u8 unkB25;
    u8 unkB26;
    u8 unkB27;
    u16 unkB28;
    u16 unkB2A;
    struct {
        u16 unk0;
        u16 unk2;
    } unkB2C[2];
    u16 unkB34;
    u8 unkB36;
    u8 unkB37;
    u16 unkB38;
    u16 unkB3A;
    u8 unkB3C;
    u8 unkB3D;
    u16 unkB3E;
    u16 timer;                  /* +0xB40: frame counter; the toss ends after 0x140 frames */
    u8 unkB42;
    u8 fillerB43[0xB48 - 0xB43];
    u8 unkB48[6];               /* +0xB48: sub_0807883C / sub_080787F4 */
    u8 unkB4E;                  /* +0xB4E: 2 = done, 3 = set BLDCNT */
    u8 unkB4F;
    u8 objB50[0xC54 - 0xB50];   /* +0xB50: sub_08025258/sub_080252D4/sub_08025344 */
    u8 phase;                   /* +0xC54: 0 = launch all, 1 = launch one every 16 frames */
    u8 delay;                   /* +0xC55 */
    u8 launched;                /* +0xC56 */
    u8 fillerC57;
};
extern struct Work15280 gUnk_02015280;
#define gWork gUnk_02015280
void sub_08024D48(struct Toss *t);
/* Toss screen graphics (ROM). */
extern const u8 gUnk_086A12EC[];
extern const u8 gUnk_086AA8EC[];
extern const u8 gUnk_086AFA28[];
extern const u8 gUnk_086B2148[];
extern const u8 gUnk_086AFA48[];
extern const u8 gUnk_086AFC48[];
extern const u8 gUnk_086AFE48[];
extern const u8 gUnk_086B0048[];
extern const u8 gUnk_086B0248[];
extern const u8 gUnk_086B0448[];
extern const u8 gUnk_086B0648[];
extern const u8 gUnk_086B0848[];
extern const u8 gUnk_086B0A48[];
extern const u8 gUnk_086B0C48[];
extern const u8 gUnk_086B0E48[];
extern const u8 gUnk_086B1048[];
extern const u8 gUnk_086B1248[];
extern const u8 gUnk_086B1448[];
extern const u8 gUnk_086B1648[];
extern const u8 gUnk_086B1848[];
extern const u8 gUnk_086B1A48[];
extern const u8 gUnk_086B1C48[];
extern const u8 gUnk_086B1E48[];
extern const u8 gUnk_086B1EC8[];
extern const u8 gUnk_086B1F48[];
extern const u8 gUnk_086B1FC8[];
extern const u8 gUnk_086B2048[];
extern const u8 gUnk_086B20C8[];
void sub_08025258(void *p);
void sub_080252D4(void *p);
void sub_0807883C(void *p);
u32 sub_080251C8(struct Toss *t, u8 n);
void sub_08077AEC(u32 se);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
void sub_0802515C(struct TossSlot *s, u8 n, u8 mode);
void sub_08025108(struct TossSlot *s, u8 n);
void sub_0807A298(void *p);
void sub_08027CB8(void *p);
void sub_08027CDC(void *p);
void sub_08027D1C(void *p);
void sub_08024DD4(struct TossSlot *s, u8 n);
void sub_08024EC8(struct TossSlot *s, u8 n, u32 unused, u8 *count);
void sub_08024FBC(struct TossSlot *s, u8 n);
void sub_08025054(struct TossSlot *s, u8 n, u8 mode);


/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 unk0[0x40E];
    u16 unk40E;         /* +0x40E */
    u8 filler410[0x4422 - 0x410];
    u16 bgScroll4422;   /* +0x4422 */
    u16 bgScroll4424;   /* +0x4424 */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_0807A2EC(void *p);
void sub_0807B534(void *p);
void sub_0807B4A8(u32 a);
void sub_08025344(void *p);

extern const u16 gUnk_08081F80[];   /* toss token tile per anim frame */
extern const u16 gUnk_08081F90[];   /* result tile (+5: second set) */
void sub_0807B6B8(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);

extern u8 gUnk_02015DD0[];
void sub_08025200(u8 x, u8 y, void *work);

/* Second tile buffer at 0x0201AE60 (fields used here). */
struct TileBuf {
    u8 unk0_0:1;
    u8 dirty:1;         /* +0x00 bit 1 */
    u8 unk0_2:6;
    u8 filler1[0x24 - 1];
    u8 tiles[0x1B00];   /* +0x24: copied to VRAM 0x06009AE0 */
};
extern struct TileBuf gUnk_0201AE60;
extern const u16 gUnk_081A429C[];   /* interpolation weights (8.8), indexed by steps left */
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_0805ED78(void);
void sub_0805F96C(void);
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

void sub_0802408C(u32 bg);
void sub_080240A8(u32 player, u32 zone);
void sub_080240D4(u32 x, u32 y);
void sub_08024134(s32 player, s32 zone, s32 idx);
u32 sub_080623AC(u32 player, u32 a, u32 b);
void sub_0807695C(u32 a, void *obj);
void sub_080769DC(void *obj);
void sub_08076A20(s16 a, s16 b, void *obj, u16 c);
void sub_08076BEC(s16 a, s16 b, void *obj, u16 c);
void sub_08076DAC(u32 a, void *obj, u16 b, u16 c);


void sub_0802408C(u32 bg)
{
    sScreen.scrollFrom = sScreen.scroll;
    sScreen.scrollTo = bg;
    sScreen.scrollSteps = 4;
}
void sub_080240A8(u32 player, u32 zone)
{
    u16 v = gUnk_081A43A4[player][zone];
    if (sScreen.scrollTo != (u8)v)
        sub_0802408C(v);
}
/* Start moving the field cursor from its current position to (x, y) over 4 frames. */
void sub_080240D4(u32 x, u32 y)
{
    sScreen.cursorFromX = sScreen.cursorX;
    sScreen.cursorFromY = sScreen.cursorY;
    sScreen.cursorToX = x;
    sScreen.cursorToY = y;
    sScreen.cursorSteps = 4;
}
void sub_08024134(s32 player, s32 zone, s32 idx)
{
    s32 i;

    sScreen.player824 = player;
    sScreen.zone828 = zone;
    sScreen.idx82C = idx;
    if (sScreen.zone828 == 0) {
        if (idx == 10) {
            sScreen.zone828 = idx;
            sScreen.idx82C = zone;
        }
        if (sScreen.idx82C > 4) {
            sScreen.zone828 = 5;
            sScreen.idx82C = sScreen.idx82C - 5;
        }
    }
    if (zone == 0 || zone == 5)
        i = zone + idx;
    else
        i = zone;
    sub_080240D4(sub_080623AC(player, zone, idx), gUnk_081A42A4[player][i].unk4);
    sub_080240A8(sScreen.player824, sScreen.zone828);
}
void sub_080241C4(void)
{
    sub_08024134(sScreen.player824, sScreen.zone828, sScreen.idx82C);
}

void sub_080241F0(u32 a)
{
    sub_0807695C(a, sScreen.unk848);
    sScreen.cursorOn = 0;
}

void sub_08024218(void)
{
    sub_080769DC(sScreen.unk848);
}

void sub_08024228(s16 a, s16 b, u16 c)
{
    sub_08076A20(a, b, sScreen.unk848, c);
}

void sub_08024248(s16 a, s16 b, u16 c)
{
    sub_08076BEC(a, b, sScreen.unk848, c);
}

void sub_08024268(u32 a, u16 b, u16 c)
{
    sub_08076DAC(a, sScreen.unk848, b, c);
}

void sub_08024288(u16 kind, u32 arg)
{
    sScreen.animKind = kind;
    sScreen.animArg = arg;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void sub_080242C4(u16 id, struct CardLoc *from, struct CardLoc *to)
{
    sScreen.animKind = 3;
    sScreen.animArg = id;
    sScreen.from = *from;
    sScreen.to = *to;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void sub_0802432C(struct CardLoc *from, struct CardLoc *to)
{
    sScreen.animKind = 4;
    sScreen.from = *from;
    sScreen.to = *to;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void sub_08024380(struct CardLoc *from, u32 arg, u32 a, u32 b)
{
    sScreen.animKind = 5;
    sScreen.animArg = arg;
    sScreen.from = *from;
    sScreen.animArg83A = a;
    sScreen.animArg83C = b;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

u16 sub_080243CC(void)
{
    if (sScreen.animActive) {
        switch (sScreen.animKind) {
        case 1:
            sub_0805D58C();
            return 1;
        case 2:
            sub_0805D708();
            return 1;
        case 3:
            sub_0805D848();
            return 1;
        case 4:
            sub_0805DA1C();
            return 1;
        case 5:
            sub_0805DB90();
            return 1;
        }
    }
    return 0;
}
u32 sub_08024440(void)
{
    u32 busy = 0;

    if (sScreen.active) {
        if (sScreen.tilesDirty) {
            sub_080752B0((void *)0x060091C0, sScreen.tiles, 0x800);
            if (sScreen.flag808_1) {
                sub_0805ED78();
                sScreen.flag808_1 = 0;
            }
            sScreen.tilesDirty = 0;
        }
        if (sScreen.active) {
            if (gUnk_0201AE60.dirty) {
                sub_080752B0((void *)0x06009AE0, gUnk_0201AE60.tiles, 0x1B00);
                gUnk_0201AE60.dirty = 0;
            }
            if (sScreen.active) {
                int steps = sScreen.scrollSteps;
                if (steps > 0) {
                    s32 d = sScreen.scrollTo - sScreen.scrollFrom;
                    sScreen.scrollSteps = steps - 1;
                    d *= gUnk_081A429C[sScreen.scrollSteps];
                    d /= 256;
                    sScreen.scroll = sScreen.scrollFrom + d;
                    busy = 1;
                }
                gMain.bgScroll4422 = sScreen.scroll;
                gMain.bgScroll4424 = sScreen.scroll;
            }
        }
    }
    if (sScreen.cursorDone) {
        sScreen.cursorDone = 0;
        sub_0805F96C();
    }
    if (sScreen.cursorSteps) {
        sScreen.cursorX = sScreen.cursorToX - sScreen.cursorFromX;
        sScreen.cursorY = sScreen.cursorToY - sScreen.cursorFromY;
        sScreen.cursorSteps--;
        sScreen.cursorX *= gUnk_081A429C[sScreen.cursorSteps];
        sScreen.cursorY *= gUnk_081A429C[sScreen.cursorSteps];
        sScreen.cursorX /= 256;
        sScreen.cursorY /= 256;
        sScreen.cursorX += sScreen.cursorFromX;
        sScreen.cursorY += sScreen.cursorFromY;
        busy = 1;
        if (sScreen.cursorSteps == 0)
            sScreen.cursorDone = 1;
    }
    {
        s32 y = sScreen.cursorY - sScreen.scroll;
        if (y >= 0 && y < 160) {
            u16 pal = sScreen.cursorFlipB ? 0x30 : 0;
            u32 flip = sScreen.cursorFlipA ? 0x40 : 0;
            if (sScreen.cursorOn && sScreen.cursorShow)
                sub_08076714((sScreen.cursorX + 8) | (y << 16), 0x80, pal, flip | 0x1000000);
        }
    }
    if (sub_080243CC())
        busy = 1;
    return busy;
}
u32 sub_080246A8(void)
{
    sub_08075278(&gUnk_02015280, sizeof(gUnk_02015280));
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    sub_0807A2EC(&gUnk_02015280);
    sub_0807B534(gUnk_02015280.unk618);
    sub_0807B4A8(8);
    gUnk_02015280.timer = 0;
    gUnk_02015280.unkB42 = 0;
    gUnk_02015280.unkB27 = 0xFF;
    sub_08025344(gUnk_02015280.objB50);
    return 1;
}
u32 sub_08024744(void)
{
    vu16 zero;
    vu16 zero2;
    u8 i;

    sub_080787F4(0, -0x180, 0, gWork.unkB48);
    CpuSet(gUnk_086A12EC, (void *)0x06000000, 0x4B00);
    CpuSet(gUnk_086AA8EC, (void *)0x05000000, 0x100);
    CpuSet(gUnk_086AFA28, (void *)0x05000200, 0x10);
    zero = 0;
    CpuSet((void *)&zero, (void *)0x06014000, 0x01000010);
    CpuSet(gUnk_086AFA48, (void *)0x06014020, 0x100);
    CpuSet(gUnk_086AFC48, (void *)0x06014220, 0x100);
    CpuSet(gUnk_086AFE48, (void *)0x06014420, 0x100);
    CpuSet(gUnk_086B0048, (void *)0x06014620, 0x100);
    CpuSet(gUnk_086B0248, (void *)0x06014820, 0x100);
    CpuSet(gUnk_086B0448, (void *)0x06014A20, 0x100);
    CpuSet(gUnk_086B0648, (void *)0x06014C20, 0x100);
    CpuSet(gUnk_086B0848, (void *)0x06014E20, 0x100);
    CpuSet(gUnk_086B0A48, (void *)0x06015020, 0x100);
    CpuSet(gUnk_086B0C48, (void *)0x06015220, 0x100);
    CpuSet(gUnk_086B0E48, (void *)0x06015420, 0x100);
    CpuSet(gUnk_086B1048, (void *)0x06015620, 0x100);
    CpuSet(gUnk_086B1248, (void *)0x06015820, 0x100);
    CpuSet(gUnk_086B1448, (void *)0x06015A20, 0x100);
    CpuSet(gUnk_086B1648, (void *)0x06015C20, 0x100);
    CpuSet(gUnk_086B1848, (void *)0x06015E20, 0x100);
    CpuSet(gUnk_086B1A48, (void *)0x06016020, 0x100);
    CpuSet(gUnk_086B1C48, (void *)0x06016220, 0x100);
    CpuSet(gUnk_086B1E48, (void *)0x060164A0, 0x40);
    CpuSet(gUnk_086B1EC8, (void *)0x06016520, 0x40);
    CpuSet(gUnk_086B1F48, (void *)0x060165A0, 0x40);
    CpuSet(gUnk_086B1FC8, (void *)0x06016620, 0x40);
    CpuSet(gUnk_086B2048, (void *)0x060166A0, 0x40);
    CpuSet(gUnk_086B20C8, (void *)0x06016720, 0x40);
    CpuSet(gUnk_086B2148, (void *)0x05000220, 0x10);
    zero2 = 0;
    CpuSet((void *)&zero2, (void *)0x06016420, 0x01000040);
    for (i = 0; i < 3; i++) {
        gWork.unk618[i].unk0 = 0;
        gWork.unk618[i].unk2 = 0;
    }
    sub_08024D48(&gWork.toss);
    for (i = 0; i < 4; i++) {
        gWork.unkB14[i].unk1 = 0;
        gWork.unkB14[i].unk0 = 0;
        gWork.unkB14[i].unk2 = 0;
    }
    for (i = 0; i < 2; i++)
        gWork.unkB2C[i].unk0 = 0;
    gWork.unkB3C = 0;
    gWork.unkB3D = 0;
    gWork.unkB24 = 0xFF;
    gWork.unkB25 = 0;
    gWork.unkB28 = 0;
    gWork.unkB36 = 0xFA;
    gWork.unkB37 = 0;
    gWork.unkB38 = 0;
    gWork.unkB3A = 0;
    gWork.timer = 0;
    REG_DISPCNT = 0x1F44;
    return 1;
}
u32 sub_08024AA0(void)
{
    sub_08025258(gWork.objB50);
    sub_080252D4(gWork.objB50);
    sub_0807883C(gWork.unkB48);
    if (gWork.unkB4E == 2)
        return 1;
    if (gWork.unkB4E == 3)
        REG_BLDCNT = 0x1040;
    if (gWork.toss.done == 0 && sub_080251C8(&gWork.toss, gWork.toss.count) == 0) {
        gWork.toss.done = 1;
        gWork.delay = 100;
    }
    switch (gWork.phase) {
    case 0:
        switch (gWork.toss.done) {
        case 0:
            gWork.toss.slots[gWork.toss.next].state = 1;
            gWork.toss.next++;
            if (gWork.toss.next == gWork.toss.count + 1)
                gWork.toss.next = gWork.toss.count;
            else
                sub_08077AEC(30);
            break;
        case 1:
            sub_080787F4(0, 0x180, 0, gWork.unkB48);
            break;
        }
        break;
    case 1:
        if (--gWork.delay == 0xFF) {
            gWork.delay = 15;
            switch (gWork.toss.done) {
            case 0:
                gWork.toss.slots[gWork.toss.next].state = 1;
                gWork.toss.next++;
                if (gWork.toss.next == gWork.toss.count + 1)
                    gWork.toss.next = gWork.toss.count;
                if (gWork.launched++ < gWork.toss.count)
                    sub_08077AEC(30);
                break;
            case 1:
                sub_080787F4(0, 0x180, 0, gWork.unkB48);
                break;
            }
        }
        break;
    }
    sub_08024DD4(gWork.toss.slots, gWork.toss.count);
    sub_08024EC8(gWork.toss.slots, gWork.toss.count, gWork.timer, &gWork.toss.landed);
    sub_08024FBC(gWork.toss.slots, gWork.toss.count);
    sub_0802515C(gWork.toss.slots, gWork.toss.count, gWork.toss.mode);
    sub_08025108(gWork.toss.slots, gWork.toss.count);
    sub_08025054(gWork.toss.slots, gWork.toss.count, gWork.toss.mode);
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    sub_08027CB8(&gWork.unkB14[0]);
    sub_08027CDC(&gWork.unkB14[0]);
    sub_08027D1C(&gWork.unkB14[1]);
    if (gWork.timer++ > 0x140)
        return 1;
    return 0;
}
u32 sub_08024CB8(void)
{
    if (gUnk_08198FAC[gUnk_02017A30.step]) {
        if (gUnk_08198FAC[gUnk_02017A30.step]())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

u32 sub_08024CF0(void)
{
    if (gUnk_02017A30.step == 1) {
        gUnk_02015280.phase = 1;
        gUnk_02015280.delay = 30;
    }
    if (gUnk_08198FAC[gUnk_02017A30.step]) {
        if (gUnk_08198FAC[gUnk_02017A30.step]())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

void sub_08024D48(struct Toss *t)
{
    u8 i;

    for (i = 0; i < 8; i++) {
        t->slots[i].timer = 3;
        t->slots[i].frame = 0;
        t->slots[i].state = 0;
        t->slots[i].y = 0;
        t->slots[i].t = 0;
        t->slots[i].unkA = 0;
        t->slots[i].unk9 = 0;
        t->slots[i].unk8 = 6;
    }
    t->next = 0;
    t->landed = 0;
    t->done = 0;
    t->count = 3;
    t->mode = 0;
    t->unk63 = 2;
    t->count = (gUnk_02017A30.opts & 0x7F00) >> 8;
    t->mode = (gUnk_02017A30.opts >> 13) & 4;
    if (gUnk_02017A30.opts & 0x80)
        t->unk63 = 0;
    else
        t->unk63 = 10;
}

void sub_08024DD4(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].state == 1) {
            if (s[i].timer == 0) {
                s[i].timer = 3;
                if (++s[i].frame == 8)
                    s[i].frame = 0;
            } else {
                s[i].timer--;
            }
        }
    }
}

void sub_08024E24(struct TossSlot *s, u8 n, u8 mask, u8 *count)
{
    u8 i;

    for (i = 0; i < n; i++) {
        u8 st = s[i].state;
        if (st == 1) {
            s[i].t += 40;
            s[i].y = sub_0807B4D0(0x3000, s[i].t) - sub_0807B4D0(0x500, sub_0807B4D0(s[i].t, s[i].t));
            if (s[i].y < 0) {
                u8 f;
                s[i].y = 0;
                s[i].t = 0;
                s[i].state++;
                f = (st & mask) ? 0 : 4;
                s[i].frame = f;
                if (f == 0)
                    (*count)++;
            }
        }
    }
}

void sub_08024EC8(struct TossSlot *s, u8 n, u32 unused, u8 *count)
{
    u8 i;

    for (i = 0; i < n; i++) {
        u8 st = s[i].state;
        if (st == 1) {
            s[i].t += 40;
            s[i].y = sub_0807B4D0(0x3000, s[i].t) - sub_0807B4D0(0x500, sub_0807B4D0(s[i].t, s[i].t));
            if (s[i].y < 0) {
                s[i].y = 0;
                s[i].t = 0;
                s[i].state++;
                if ((gUnk_02017A30.opts >> i) & st)
                    s[i].frame = 4;
                else
                    s[i].frame = 0;
                if (s[i].frame == 0)
                    (*count)++;
            } else if ((s[i].t & 0xF) == 0) {
                sub_08025200((i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8), gUnk_02015DD0);
            }
        }
    }
}
void sub_08024FBC(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        int tile = gUnk_08081F80[s[i].frame] + 0x200;
        sub_0807B6B8(0, tile, (i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8),
                     32, 32, 4, 0, 0x200, 0, 0, 0, &gUnk_02015280);
    }
}
void sub_08025054(struct TossSlot *s, u8 n, u8 mode)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].unkA == 1) {
            int tile = gUnk_08081F90[s[i].unk9 + (mode != 4 ? 5 : 0)] + 0x200;
            sub_0807B6B8(0, tile, (i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8),
                         32, 32, 4, 0, 0x200, 0, 0, 0, &gUnk_02015280);
        }
    }
}
