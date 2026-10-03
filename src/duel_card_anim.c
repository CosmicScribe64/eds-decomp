#include "global.h"

#define IO32(off) (*(vu32 *)(0x04000000 + (off)))
#define IO16(off) (*(vu16 *)(0x04000000 + (off)))
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
struct PickBits { u32 f0 : 1; u32 f1 : 4; u32 f2 : 9; u32 rest : 18; };
/* Block at 0x0201D7E4 (step machine that scrolls a picked zone into view). */
struct Pick {
    u32 w0;             /* +0 */
    u8 step;            /* +4 */
    u8 pad5;
    s16 x;              /* +6 */
    s16 y;              /* +8 */
    u16 padA;
    u32 bits;           /* +0xC packed position (struct PickBits) */
    u32 bits2;          /* +0x10 */
    u8 pad14[0x1E - 0x14];
    u16 h1E;
};
extern struct Pick gDuelAnimArgs;
#define PICK_BITS (*(struct PickBits *)&gDuelAnimArgs.bits)
#define PICK_BITS2 (*(struct PickBits *)&gDuelAnimArgs.bits2)
extern const u16 gDuelAnimLerpWeights[];
extern const u16 gBounceScaleCurve[];
struct Sel {
    u8 pl;              /* +0 */
    u8 zone;            /* +1 */
    u16 h;              /* +2: low byte = flag, high byte = row */
};
struct ScrFlags {
    u8 pad[0x830];
    u8 f0 : 1;
    u8 rest : 7;
    u8 pad831[3];
    struct Sel sel;     /* +0x834 */
    u8 step;            /* +0x838 */
    u8 cnt;             /* +0x839 */
};
extern struct ScrFlags gDuelScreen;
extern u8 gDuelZones[];
extern const u16 gCardFlipTiles[];
void PlaySE(u16 se);
void ClearZoneTiles(int a, int b);
int GetCardIconObjTile(int id);
void AddAffineSprite(u32 yx, u16 a, u16 attr, u32 c);
#define CARD_ID(w) (((w) << 20) >> 20)
struct MainBig { u8 pad0[4]; u16 held; u8 pad[0x4862 - 6]; u16 vcount; };
extern struct MainBig gMain;
struct Blk18450 { u8 pad[0x15D]; u8 k; };
extern struct Blk18450 gBattle;
extern const u32 gBattleSceneOpenBgX[];
extern const u32 gBattleSceneOpenBgY[];
extern const u16 gBattleSceneOpenBgPA[];
#define K gBattle.k
#define H gMain.vcount
int GetAreaX(int a, int b, int c);
int GetAreaY(int a, int b, int c);
void DuelCursor_Select(u32 player, u32 a, u32 b);
void DuelSprAnim_Load(u32 a);
void DuelSprAnim_Draw(int a, int b, int c);
void LoadDuelUiGfx(void);
void BattleScene_DrawAtk(int a, int x, int y, u32 val);
void BattleScene_DrawDef(int a, int x, int y, u32 val);
void BattleScene_DrawSmallNumber(int x, int y, int val);
void BattleScene_DrawBigNumber(int x, int y, int val, int k);
int Random(void);
void MemCopy16(u32 dst, const void *src, u32 n);
extern const u8 gCardArtPalettes[];
extern const u16 gCardArtGfx[];

extern const u16 gCardRotateAngles[];
/* Card slot record: 0x94 bytes per zone, 0xD64 bytes per player side, at 0x0201930C. */
struct Zone58C { u32 w; u8 b4; u8 b5; u8 f0 : 1; u8 fb : 1; u8 rest : 6; u8 pad[0x94 - 7]; };
struct Side58C { struct Zone58C z[23]; u8 pad[0xD64 - 23 * 0x94]; };
/* gCardFlipTiles as [set][frame] and gCardRotateAngles as [flag][frame]: real 2D arrays give the
   ROM's (frame*2 + set*48) + base address order. */
extern const u16 gAnimFrames_081A4474[][24];
extern const u16 gAnimScale_081A44D4[][10];
void DuelAnim_UpdateChangePosition(void)
{
    struct ScrFlags *sc = &gDuelScreen;
    struct Sel *q = &sc->sel;
    u8 pl, zone;
    int z;
    u8 flag;
    int side;
    int fb;
    int a, b;
    int row;
    int id;
    u8 *step;
    pl = q->pl;
    /* Loading zone through an int temporary shortens zone's live range by one insn before
       combine, so zone is allocated before pl (zone r6, pl r7 as in the ROM). */
    z = q->zone;
    zone = z;
    flag = *(u8 *)&q->h;
    side = pl & 1;
    /* Pointer arithmetic (symbol last) puts the base literal after the offset, as in the ROM. */
    fb = ((struct Zone58C *)(side * 0xD64 + zone * 0x94 + (u32)gDuelZones))->fb;
    row = q->h >> 8;
    id = CARD_ID(((struct Side58C *)gDuelZones)[side].z[zone].w);
    step = &sc->step;
    switch (*step) {
    case 0:
        PlaySE(6);
        ClearZoneTiles(pl, zone);
        sc->cnt = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gDuelScreen.cnt <= 9) {
            u16 attr;
            a = GetAreaX(pl, 0, zone);
            b = GetAreaY(pl, 0, zone);
            attr = gAnimFrames_081A4474[fb][0];
            if (row != 0)
                attr = gAnimFrames_081A4474[fb][(gDuelScreen.cnt * 24) / 10];
            if (attr & 0x1000) {
                attr &= 0xEFFF;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int k = GetCardIconObjTile(id) + 0x1000;
                    int s = (s16)attr + k;
                    attr = s;
                }
            }
            AddAffineSprite((b << 16) | a, 0x80, attr | 0x400,
                         0x1000000 | gAnimScale_081A44D4[flag][gDuelScreen.cnt]);
            gDuelScreen.cnt++;
            if (gDuelScreen.cnt <= 9)
                return;
        }
        /* fall through */
    default:
        gDuelScreen.f0 = 0;
        break;
    }
}
/* Card slot record: 0x94 bytes per zone, 0xD64 bytes per player side, at 0x0201930C. */
struct Zone708 { u32 w; u8 pad[0x94 - 4]; };
struct Side708 { struct Zone708 z[23]; u8 pad[0xD64 - 23 * 0x94]; };
/* Same table as gCardFlipTiles, as [row][frame]: a real 2D array gives the ROM's (frame*2 + row*48) + base. */
extern const u16 gAnimFrames_081A4474[][24];
void DuelAnim_UpdateFlip(void)
{
    struct ScrFlags *sc = &gDuelScreen;
    struct Sel *q = &sc->sel;
    u8 pl, zone;
    u8 flag;
    int row;
    int id;
    u8 *step;
    pl = q->pl;
    zone = q->zone;
    flag = *(u8 *)&q->h;
    row = q->h >> 8;
    id = CARD_ID(((struct Side708 *)gDuelZones)[pl & 1].z[zone].w);
    step = &sc->step;
    switch (*step) {
    case 0:
        PlaySE(6);
        ClearZoneTiles(pl, zone);
        sc->cnt = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gDuelScreen.cnt <= 0x17) {
            int a = GetAreaX(pl, 0, zone);
            int b = GetAreaY(pl, 0, zone);
            /* FAKEMATCH: `pl = row` (row <= 0xFF) reuses pl as the row index; it keeps pl live
               past the second call, which puts pl in r7 and zone in r6 as in the ROM. */
            u16 attr = gAnimFrames_081A4474[pl = row][gDuelScreen.cnt];
            if (attr & 0x1000) {
                attr &= 0xEFFF;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int k = GetCardIconObjTile(id) + 0x1000;
                    int s = (s16)attr + k;
                    attr = s;
                }
            }
            AddAffineSprite((b << 16) | a, 0x80, attr | 0x400, flag ? 0x1000020 : 0x1000000);
            gDuelScreen.cnt++;
            if (gDuelScreen.cnt <= 0x17)
                return;
        }
        /* fall through */
    default:
        gDuelScreen.f0 = 0;
        break;
    }
}
void DuelAnim_UpdateMoveCard(void)
{
    struct PickBits *src = &PICK_BITS;
    struct PickBits *dst = &PICK_BITS2;
    if (*((u8 *)dst - 12) == 0) {
        if ((*(u8 *)src & 0x1E) == 0x1A && (*(u8 *)dst & 0x1E) == 0x16)
            PlaySE(14);
        else
            PlaySE(7);
    }
    if (gDuelScreen.step <= 15) {
        int x = GetAreaX(src->f0, src->f1, src->f2);
        int y = GetAreaY(src->f0, src->f1, src->f2);
        int x2 = GetAreaX(dst->f0, dst->f1, dst->f2);
        int y2 = GetAreaY(dst->f0, dst->f1, dst->f2);
        int dx, dy;
        u16 tile = 0x40;
        int flip = 0;
        u16 t;
        int old;
        dx = x2 - x;
        dy = y2 - y;
        t = gDuelAnimLerpWeights[gDuelScreen.step];
        dx *= t;
        dy *= t;
        dx /= 256;
        dy /= 256;
        if (((u8 *)dst)[1] & 0x80)
            tile = GetCardIconObjTile(*(u32 *)&gDuelScreen.sel) + 0x1000;
        if ((u8)(((u8 *)src)[1] & 0x40) == (u8)(((u8 *)dst)[1] & 0x40)) {
            if ((u8)(((u8 *)dst)[1] & 0x40)) flip = 0x20;
        } else if ((u8)(((u8 *)dst)[1] & 0x40)) {
            flip = gDuelScreen.step * 2;
        } else {
            flip = 0x20 - gDuelScreen.step * 2;
        }
        AddAffineSprite((x + dx) | ((y + dy) << 16), 0x80, tile + 0x400,
                     ((u32)gBounceScaleCurve[gDuelScreen.step] << 16) | flip);
        old = gDuelScreen.step;
        gDuelScreen.step = old + 1;
        if ((gMain.held & 2) || (1 & *(u8 *)&gDuelScreen)) {
            if (gDuelScreen.step <= 11)
                gDuelScreen.step = old + 4;
        }
    }
    if (gDuelScreen.step == 16)
        gDuelScreen.f0 = 0;
}
void DuelAnim_UpdateSwapCards(void)
{
    struct PickBits *pb = &PICK_BITS;
    struct ScrFlags *screen;
    u8 *step = (u8 *)pb - 8;
    if (*step == 0)
        PlaySE(7);
    if (*step <= 0xF) {
        int a1 = GetAreaX(pb[0].f0, pb[0].f1, pb[0].f2);
        int b1 = GetAreaY(pb[0].f0, pb[0].f1, pb[0].f2);
        int a2 = GetAreaX(pb[1].f0, pb[1].f1, pb[1].f2);
        int b2 = GetAreaY(pb[1].f0, pb[1].f1, pb[1].f2);
        int dx1 = a2 - a1;
        int dy1 = b2 - b1;
        int dx2 = a1 - a2;
        int dy2 = b1 - b2;
        u16 t = gDuelAnimLerpWeights[*step];
        u8 old;
        dx1 *= t;
        dy1 *= t;
        dx1 /= 256;
        dy1 /= 256;
        dx2 *= t;
        dy2 *= t;
        dx2 /= 256;
        dy2 /= 256;
        /* Stage the screen address before drawing; read its flag afterward. */
        screen = &gDuelScreen;
        AddAffineSprite((a1 + dx1) | ((b1 + dy1) << 16), 0x80, 0x440, (u32)gBounceScaleCurve[*step] << 16);
        AddAffineSprite((a2 + dx2) | ((b2 + dy2) << 16), 0x80, 0x440, (u32)gBounceScaleCurve[*step] << 16);
        old = *step;
        *step = old + 1;
        if ((gMain.held & 2) || (1 & *(u8 *)screen)) {
            if (*step <= 0xB)
                *step = old + 4;
        }
    }
    if (gDuelScreen.step == 0x10)
        gDuelScreen.f0 = 0;
}

void DuelAnim_UpdateZoneEffect(void)
{
    int a = GetAreaX(PICK_BITS.f0, PICK_BITS.f1, PICK_BITS.f2);
    int b = GetAreaY(PICK_BITS.f0, PICK_BITS.f1, PICK_BITS.f2);
    u8 *step = &gDuelAnimArgs.step;
    switch (*step) {
    case 0:
        DuelCursor_Select(0, 0, 0);
        (*step)++;
        break;
    case 1: {
        u32 *w = &gDuelAnimArgs.w0;
        DuelSprAnim_Load(*w);
        (*step)++;
        break;
    }
    case 2: {
        s16 *px = &gDuelAnimArgs.x;
        s16 *py;
        int t = a + *px;
        py = &gDuelAnimArgs.y;
        DuelSprAnim_Draw(t, b + *py, 1);
        if (gDuelAnimArgs.h1E == 0)
            (*step)++;
        break;
    }
    default:
        LoadDuelUiGfx();
        gDuelScreen.f0 = 0;
        break;
    }
}
struct WarpTbl32 { u32 a[16][160]; };
struct WarpTbl16 { u16 a[16][160]; };
#define WA (((const struct WarpTbl32 *)gBattleSceneOpenBgX)->a)
#define WB (((const struct WarpTbl32 *)gBattleSceneOpenBgY)->a)
#define WC (((const struct WarpTbl16 *)gBattleSceneOpenBgPA)->a)
void BattleScene_HBlank(void)
{
    H = *(vu16 *)0x04000006;
    IO32(0x28) = WA[K][H];
    IO32(0x2C) = WB[K][H];
    IO16(0x20) = WC[K][H];
    IO32(0x38) = WA[K][H];
    IO32(0x3C) = WB[K][H];
    IO16(0x30) = WC[K][H];
}
void BattleScene_ResetBgAffine(void)
{
    IO32(0x28) = 0;
    IO32(0x2C) = 0;
    IO16(0x20) = 0x100;
    IO32(0x38) = 0;
    IO32(0x3C) = 0;
    IO16(0x30) = 0x100;
}
void BattleScene_DrawSmallNumber(int x, int y, int val)
{
    int n = val;
    int base = 0x2020;
    x += 0x14;
    if (n == 0) {
        AddSprite((y << 16) | x, 0, base);
    } else {
        do {
            AddSprite(x | (y << 16), 0, (u16)(n % 10 + base));
            n /= 10;
            x -= 4;
        } while (n != 0);
    }
}
void BattleScene_DrawBigNumber(int x, int y, int val, int k)
{
    int n = val;
    u16 base = 0x302E + k * 0x30;
    x += 0x50;
    if (n == 0) {
        AddSprite((y << 16) | x, 0x40, base);
    } else {
        do {
            AddSprite(x | (y << 16), 0x40, (u16)(base + (n % 10) * 4));
            n /= 10;
            x -= 0x10;
        } while (n != 0);
    }
}
void BattleScene_DrawAtk(int a, int x, int y, u32 val)
{
    x += a * 0x78;
    x += 0x47;
    y += 0x7E;
    AddSprite((y << 16) | x, 0x4000, 0x202A);
    BattleScene_DrawSmallNumber(x + 4, y, val);
}
void BattleScene_DrawDef(int a, int x, int y, u32 val)
{
    x += a * 0x78;
    x += 0x47;
    y += 0x86;
    AddSprite((y << 16) | x, 0x4000, 0x202C);
    BattleScene_DrawSmallNumber(x + 4, y, val);
}
void BattleScene_DrawValues(u16 mask, u32 *vals, u16 flag)
{
    int i;
    u32 *p;
    int sh;
    for (i = 0, p = vals, sh = 0; i <= 1; p++, sh += 8, i++) {
        if (flag == 0 || (mask & (2 << sh)) == 0) {
            if (((int)mask >> sh) & 1)
                BattleScene_DrawDef(i, 0, 0, *p);
            else
                BattleScene_DrawAtk(i, 0, 0, *p);
        }
    }
}
void BattleScene_DrawDamage(int a, int b, u16 c)
{
    int x = a * 0x68 + 8;
    int k = a;
    if (c != 0)
        k = Random() & 3;
    BattleScene_DrawBigNumber(x, 0x40, b, k);
}
/* Portrait loader: copies portrait b's 64-colour palette to palette slot d >> 4, unpacks its
 * 720 x 3 halfwords of packed 6-bit pixels (8 pixels per 3 halfwords) to one pixel per byte at
 * VRAM 0x06004000 + a * 0x4000 + c * 32, then adds the palette base (d & 0xFF) to all 0xB40
 * halfwords. The ROM tables are integer addresses (as in LoadCardPicture) so that both bases are
 * rematerialized by reload, which sets the ROM's reload-register rotation. `(s1 & 0xFC) * 64`
 * (not `<< 6`) keeps that term out of the u16 narrowing, so its mask ties to the output. */
void BattleScene_LoadCardArt(int a, u16 b, u16 c, u16 d)
{
    const u16 *src;
    u16 *dst;
    u16 *p;
    int n;
    u32 i;
    u16 m6, m12;

    MemCopy16(0x05000000 + (d >> 4) * 0x20, (const void *)(0x08608360 + b * 0x80), 0x80);
    src = (const u16 *)(0x082A6500 + b * 0x10E0);
    {
        u32 addr = a << 14;
        addr += 0x06004000;
        addr += c << 5;
        dst = (u16 *)addr;
    }
    m6 = 0x3F;
    m12 = 0xFC0;
    for (n = 720; n != 0; n--) {
        u16 s0 = src[0];
        u32 s1 = src[1];
        u32 s2 = src[2];
        u16 t, x;
        dst[0] = (s0 & m6) | ((s0 & m12) << 2);
        dst[1] = (s0 >> 12) | ((s1 & 3) << 4) | ((s1 & 0xFC) * 64);
        t = s1 >> 8;
        dst[2] = (t & m6) | (((t >> 6) | ((s2 & 0xF) << 2)) << 8);
        x = s2 >> 4;
        dst[3] = (x & m6) | ((x & m12) << 2);
        src += 3;
        dst += 4;
    }
    {
        u32 addr = a << 14;
        addr += 0x06004000;
        addr += c << 5;
        p = (u16 *)addr;
    }
    for (i = 0; i <= 0xB3F; i++) {
        *p = (*p & 0x3F3F) + ((u8)d << 8 | (u8)d);
        p++;
    }
}
void BattleScene_LoadCardFrame(int a, u16 *p, u16 y, u16 b)
{
    int off = *p * 2;
    u16 *cnt = (u16 *)((u8 *)p + (off + 8));
    u16 *src = (u16 *)((u8 *)p + (off + 16));
    u32 addr = a << 14;
    u16 *dst;
    u16 i;
    u16 v;
    addr += 0x06004000;
    addr += y << 5;
    dst = (u16 *)addr;
    for (i = 0; i < (*cnt << 5); i++) {
        u16 w = *src;
        v = w;
        /* Keep the loaded word separate from the pixel accumulator. */
        __asm__ __volatile__("" : : "r"(w));
        if (w & 0xFF00)
            v += (u8)b << 8;
        if (v & 0xFF)
            v += (u8)b;
        *dst = v;
        dst++;
        src++;
    }
    MemCopy16(0x05000000 + (b >> 4) * 0x20, p + 4, 0x40);
}
void BattleScene_SetCardArtMap(int a, int x, int y, int v)
{
    u16 *dst = (u16 *)((a << 11) + 0x06000000);
    int row;
    v /= 2;
    dst += x / 2;
    dst += y * 16;
    for (row = 0; row < 10; row++) {
        int j;
        u16 *p;
        if (x & 1) {
            *dst = (u8)v << 8;
            v++;
            p = dst + 1;
            for (j = 0; j < 4; j++) {
                p[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
        } else {
            for (j = 0; j < 4; j++) {
                dst[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
            dst[4] = (u8)v;
            v++;
        }
        dst += 16;
    }
}
void BattleScene_SetCardFrameMap(int a, int x, int y, int v)
{
    u16 *dst = (u16 *)((a << 11) + 0x06000000);
    int row;
    int j;
    v /= 2;
    dst += x / 2;
    dst += y * 16;
    for (row = 0; row < 4; row++) {
        u16 *p;
        if (x & 1) {
            *dst = (u8)v << 8;
            v++;
            p = dst + 1;
            for (j = 0; j < 6; j++) {
                p[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                dst[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
            dst[6] = (u8)v;
            v++;
        }
        dst += 16;
    }
    for (row = 0; row < 10; row++) {
        if (x & 1) {
            dst[0] |= (u8)v << 8;
            v++;
            dst[1] |= (u8)v;
            v++;
            dst[6] |= (u8)v | (u8)(v + 1) << 8;
            v += 2;
        } else {
            dst[0] |= (u8)v | (u8)(v + 1) << 8;
            v += 2;
            dst[5] |= (u8)v << 8;
            v++;
            dst[6] |= (u8)v;
            v++;
        }
        dst += 16;
    }
    for (row = 0; row < 4; row++) {
        u16 *p;
        if (x & 1) {
            *dst = (u8)v << 8;
            v++;
            p = dst + 1;
            for (j = 0; j < 6; j++) {
                p[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                dst[j] = (u8)v | (u8)(v + 1) << 8;
                v += 2;
            }
            dst[6] = (u8)v;
            v++;
        }
        dst += 16;
    }
}
extern u8 gBattleScene[];
extern const u16 gCardFrameTrapGfx[];
extern const u16 gCardFrameMagicGfx[];
extern const u16 gCardFrameTicketGfx[];
extern const u16 gCardFrameEffectGfx[];
extern const u16 gCardFrameFusionGfx[];
extern const u16 gCardFrameRitualGfx[];
extern const u16 gCardFrameNormalGfx[];
extern const u16 gCardStatDigitsPal[];
extern const u16 gCardStatDigitsGfx[];
extern const u16 gLpDigitsPal[];
extern const u16 gLpDigitsGfx[];
extern void (*IntrTable[16])(void);
void MemClear16(void *dst, u32 size);
void CopyDoubleWords(void *dst, const void *src, u32 size);
void ClearBgMapBuffers(void);
void ResetBgScroll(void);
void BattleScene_SetCardFrameMap(int a, int x, int y, int v);
void BattleScene_HBlank(void);
struct E3B8Bits { u8 f0 : 1; u8 f1 : 1; u8 f2 : 1; u8 rest : 5; };
struct E3B8Main { u8 pad[0x40E]; u16 f40E; };
#define E3B8_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define E3B8_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define E3B8_TYPE(id) ((E3B8_STATS(id) & 0x1F00000) >> 20)
static inline int E3B8_Subtype(u16 id)
{
    switch (E3B8_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)E3B8_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (E3B8_STATS(id) & 0xC0000) >> 18;
    }
}
static inline const u16 *E3B8_Portrait(u16 id)
{
    switch ((int)E3B8_TYPE(id)) {
    case 21:
        return gCardFrameTrapGfx;
    case 22:
        return gCardFrameMagicGfx;
    case 23:
        return gCardFrameTicketGfx;
    }
    switch (E3B8_Subtype(id)) {
    case 1:
        return gCardFrameEffectGfx;
    case 2:
        return gCardFrameFusionGfx;
    case 3:
        return gCardFrameRitualGfx;
    default:
        return gCardFrameNormalGfx;
    }
}

void BattleScene_Init(u16 a, u16 b)
{
    MemClear16(gBattleScene, 0xC);
    ((struct E3B8Bits *)&gDuelScreen)->f1 = 0;
    ((struct E3B8Bits *)&gDuelScreen)->f2 = 0;
    *(vu16 *)0x04000000 = 0x42;
    ((struct E3B8Main *)&gMain)->f40E = 1;
    *(vu16 *)0x04000050 = 0;
    *(vu16 *)0x0400000C = 0x4084;
    *(vu16 *)0x0400000E = 0x4188;
    ClearBgMapBuffers();
    ResetBgScroll();
    CopyDoubleWords((void *)0x05000240, gCardStatDigitsPal, 0x20);
    CopyDoubleWords((void *)0x06010400, gCardStatDigitsGfx, 0x1C0);
    CopyDoubleWords((void *)0x05000260, gLpDigitsPal, 0x20);
    CopyDoubleWords((void *)0x060105C0, gLpDigitsGfx, 0x1800);
    MemClear16((void *)0x06004000, 0x4000);
    MemClear16((void *)0x06000000, 0x800);
    if (a != 0) {
        BattleScene_LoadCardArt(0, a, 0x14C, 0x40);
        BattleScene_SetCardArtMap(0, 3, 5, 0x14C);
        BattleScene_LoadCardFrame(0, (u16 *)E3B8_Portrait(a), 0x2C, 0x80);
        BattleScene_SetCardFrameMap(0, 1, 1, 0x2C);
    }
    MemClear16((void *)0x06008000, 0x4000);
    MemClear16((void *)0x06000800, 0x800);
    if (b != 0) {
        BattleScene_LoadCardArt(1, b, 0x14C, 0xA0);
        BattleScene_SetCardArtMap(1, 0x12, 5, 0x14C);
        BattleScene_LoadCardFrame(1, (u16 *)E3B8_Portrait(b), 0x2C, 0xE0);
        BattleScene_SetCardFrameMap(1, 0x10, 1, 0x2C);
    }
    gBattle.k = 0;
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 &= 0xFFFD;
    IntrTable[1] = BattleScene_HBlank;
    *(vu16 *)0x04000208 = 1;
    *(vu16 *)0x04000208 = 0;
    *(vu16 *)0x04000200 |= 2;
    *(vu16 *)0x04000208 = 1;
}
