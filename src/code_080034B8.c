#include "global.h"

#include "main.h"

#include "gba.h"

/* gMain (struct Main) comes from the shared main.h header. */
#define gMain gUnk_03000040

/* Record scene state (0x0201F814). */
struct Record {
    u16 unk0_0:1;
    u16 unk0_1:2;
    u16 unk0_3:10;
    u16 unk1_5:3; /* bits 13-15 */
    u16 unk2;
};
extern struct Record gUnk_0201F814;
#define gRecord gUnk_0201F814

/* Per-index counters in the save mirror (gSaveData + 0x20D0 + idx*4), viewed two ways
 * so agbcc emits the ROM's `ldrh` for the low field and `ldr` for the middle one. */
struct Card2A { u16 a : 11; u16 rest : 5; u16 hi; };
struct Card2B { u32 lo : 11; u32 b : 11; u32 c : 10; };
struct Card2Rec  { u8 pad0[0x20D0]; struct Card2A a; };
struct Card2RecB { u8 pad0[0x20D0]; struct Card2B b; };
struct Card2C { u16 lo; u16 pad : 6; u16 c : 10; };
struct Card2RecC { u8 pad0[0x20D0]; struct Card2C c; };
extern u8 gUnk_02011C20[];
extern u16 gUnk_08198618[];

/* 0x0201F7E0: Campaign opponent-select state (shared with code_08002388.c). */
struct OpponentSelect {
    u8 page : 3;        /* bits 0-2 */
    u16 cursor : 3;     /* bits 3-5: selected slot */
    u16 unk6 : 3;       /* bits 6-8 */
    u16 unk9 : 4;       /* bits 9-12 */
    u32 unk13 : 4;      /* bits 13-16 */
    u32 unk17 : 15;
    u16 cursorX[8];     /* +0x04 */
    u16 cursorY[8];     /* +0x14 */
    s32 unk24;          /* +0x24 */
    s32 unk28;          /* +0x28 */
    s32 targetX;        /* +0x2C */
    s32 targetY;        /* +0x30 */
};
extern struct OpponentSelect gUnk_0201F7E0;
#define gSel gUnk_0201F7E0
struct Pos16 { s16 x; s16 y; };
extern struct Pos16 gUnk_081983AC[5];
extern const u8 *gUnk_081984CC[3]; /* "Win", "Lose", "Draw" */
struct Bob { u16 x; u16 y; };
extern struct Bob gUnk_081983C0[];
/* Record scroll offsets [parity][direction - 1][step]; indexed as a real 3-D array so
 * sub_08003C78 keeps the (direction - 1) unfolded. */
extern u16 gUnk_08198508[2][2][16];
extern const u8 *gUnk_081985A0[];
extern const u8 *gUnk_08198604[];
extern const u8 gUnk_087E52A4[], gUnk_087E5CF4[], gUnk_087E77B4[];
s32 sub_080753CC(const u8 *str); /* StrLen */
void sub_080762D0(u32 yx, u16 shapeSize, u16 attr2);
void sub_080763D0(u32 yx, u16 shapeSize, u16 attr2);
void sub_08003020(s32 x, s32 y);
void sub_080030FC(void);

extern u16 gUnk_02015ED8; /* gMainMenuCursor */
#define gMainMenuCursor gUnk_02015ED8

typedef u16 (*StepFunc)(void);
extern StepFunc gUnk_081984D8[]; /* main menu launch table */
extern StepFunc gUnk_081984F4[]; /* main menu steps */
extern StepFunc gUnk_08198588[]; /* record steps */
extern const u8 gUnk_08198628[]; /* days per month */
extern const u8 gUnk_087DE858[], gUnk_087DE878[], gUnk_087D4B24[];
extern const u8 gUnk_087E2878[], gUnk_087E2A78[], gUnk_087E2C78[], gUnk_087E2E78[];
extern const u8 gUnk_087E3078[], gUnk_087E3278[], gUnk_087E3478[], gUnk_087E35B8[], gUnk_087E4280[];

u16 sub_08075A6C(u16 step); /* FadeToBlack */
u16 sub_08075AE4(u16 step); /* FadeFromBlack */
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void sub_08077AEC(u16 id); /* PlaySE */
void sub_08077BCC(void);   /* FadeOutBGM */
void sub_080754F8(StepFunc cb); /* SetMainCallback */
void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_08075294(void *dst, const void *src, u32 size); /* MemCopy16 */
void sub_080752B0(void *dst, const void *src, u32 size); /* CopyDoubleWords */
void sub_080757AC(void); /* ResetBgScroll */
void sub_080759F4(void); /* SetBrightnessBlack */
void sub_08073574(void); /* ResetVideo */
u16 sub_08072FAC(u16 mapBase, u16 palIdx, u16 tileBase, const void *img); /* LoadBgImage */
void sub_0807332C(u32 a, u32 b, u32 c, u32 d, const void *e);
void sub_08077B54(u16 id); /* PlayBGMNoTrack */
u16 sub_08063BAC(void);
u16 sub_08063C14(void);
u16 sub_08063C7C(void);
u16 sub_08063CE4(void);

void sub_08003850(void);
u16 sub_08003ED4(void);
u16 sub_08003EEC(void);
void sub_08003F34(void);
void sub_08003F88(void);
void sub_0800406C(void);
void sub_080040E4(s32 a, s32 b);
void sub_08073500(u16 row, u16 col, u16 w, u16 h);
s32 sub_08063DAC(u16 id);
u32 sub_08004280(u32 year);
s32 sub_080042D8(u32 year, u32 month, u32 day);
u32 sub_08004358(u32 year, u32 month, u32 day);

/* Opponent-select cursor: shift the 8-position trail back, aim it at the
 * selected slot (easing over 15 frames, else bobbing on idle), draw the trail
 * sprites and then the portrait frame. */
#if 0 /* NONMATCHING: the logic and instruction sequence essentially match
       * (size 0x1A8 vs 0x1B4), but agbcc's register map differs (ROM: slot r3,
       * &gSel r6, &pos r5, flag r9, slot*4 sl; built: slot r3, &gSel r4, &pos
       * r6, flag r8, slot*4 r9) and the interpolation temporaries are
       * scheduled in a different order. */
void sub_080034B8(s32 slot, u16 flag)
{
    struct OpponentSelect *sel = &gSel;
    u16 *p;
    s8 i;

    p = &sel->cursorX[6];
    i = 7;
    do {
        p[1] = p[0];
        p[9] = p[8];
        p--;
    } while (--i);
    sel->targetX = gUnk_081983AC[slot].x + 0x10;
    sel->targetY = gUnk_081983AC[slot].y;
    if (slot != sel->unk6) {
        sel->unk6 = slot;
        sel->unk9 = 0xF;
        sel->unk13 = 0;
        sel->unk24 = sel->cursorX[0];
        sel->unk28 = sel->cursorY[0];
    }
    if (sel->unk9) {
        s32 t = sel->unk9;
        s16 dx = (sel->unk24 - sel->targetX) * t;
        s16 dy = (sel->unk28 - sel->targetY) * t;
        sel->cursorX[0] = sel->targetX + dx / 16;
        sel->cursorY[0] = sel->targetY + dy / 16;
        sel->unk9 = t - 1;
    } else {
        sel->cursorX[0] = sel->targetX + gUnk_081983C0[(gMain.frameCounter >> 1) & 0x1F].x;
        sel->cursorY[0] = sel->targetY + gUnk_081983C0[(gMain.frameCounter >> 1) & 0x1F].y;
    }
    for (i = 0; i <= 7; i++) {
        if (i == 0) {
            if (flag == 0) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            sub_080762D0((sel->cursorY[0] << 16) | sel->cursorX[0], 0x4080, 0x104);
        } else if (flag == 0) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = 0x808;
            sub_080763D0((sel->cursorY[i] << 16) | sel->cursorX[i], 0x4080, 0x104);
        }
    }
    sub_08003020(gUnk_081983AC[slot].x, gUnk_081983AC[slot].y);
    sub_080030FC();
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080034B8", sub_080034B8); /* 0x080034B8 size 0x1B4 */
/* Draw a clamped 0..99 number as (up to) two decimal digit sprites at (x, y). */
void sub_0800366C(s32 x, s32 y, s32 value)
{
    s32 v = value;

    if (v > 99)
        v = 99;
    if (v < 0)
        v = 0;
    sub_080761F0((x + 8) | (y << 16), 0, ((v % 10) + 0x280) | 0x3000);
    v /= 10;
    if (v > 0)
        sub_080761F0(x | (y << 16), 0, ((v % 10) + 0x280) | 0x3000);
    else
        sub_080761F0(x | (y << 16), 0, 0x32A0);
}
/* Record screen: draw a duelist's win/loss/draw labels and counters, centred at the top. */
void sub_080036FC(u16 duelist)
{
    s32 x;
    s32 total;
    const u8 **p;
    s32 i;
    u32 y;

    total = 2;
    p = gUnk_081984CC;
    i = 2;
    do {
        total += 0x10 + sub_080753CC(*p) * 5;
        p++;
        i--;
    } while (i >= 0);
    x = 0x78 - total / 2;
    if ((u16)(duelist - 1) > 0x17)
        return;
    y = 0x270000;
    sub_080761F0(x | y, 0x4080, 0x328A);
    x += sub_080753CC(gUnk_081984CC[0]) * 5;
    {
        u8 *saveBase = (u8 *)gUnk_02011C20;
        struct Card2Rec *w;

        w = (struct Card2Rec *)(saveBase + duelist * 4);
        sub_0800366C(x, 0x28, w->a.a);
    }
    x += 0x11;
    sub_080761F0(x | y, 0x4080, 0x328E);
    x += sub_080753CC(gUnk_081984CC[1]) * 5;
    {
        struct Card2RecB *w = (struct Card2RecB *)((u8 *)gUnk_02011C20 + duelist * 4);
        sub_0800366C(x, 0x28, w->b.b);
    }
    x += 0x11;
    sub_080761F0(x | y, 0x4080, 0x3292);
    x += sub_080753CC(gUnk_081984CC[2]) * 5;
    {
        struct Card2RecC *w = (struct Card2RecC *)((u8 *)gUnk_02011C20 + duelist * 4);
        sub_0800366C(x, 0x28, w->c.c);
    }
    sub_080761F0(0x000A0048, 0x4080, (gSel.cursor * 0x40 + 0x2C0) | 0x3000);
    sub_080761F0(0x000A0068, 0x4080, (gSel.cursor * 0x40 + 0x2C4) | 0x3000);
    sub_080761F0(0x000A0088, 0x4080, (gSel.cursor * 0x40 + 0x2C8) | 0x3000);
}
/* MainMenu_DrawItems: "MENU" header plus 7 rows of 4 sprites; the row under the cursor uses tiles +0x10. */
void sub_08003850(void)
{
    s32 i;
    u32 x;

    sub_080761F0(0x000A0038, 0x4080, 0);
    sub_080761F0(0x000A0058, 0x4080, 4);
    sub_080761F0(0x000A0078, 0x4080, 8);
    sub_080761F0(0x000A0098, 0x4080, 12);
    for (i = 0, x = 0x38; i <= 6; i++) {
        u16 tile = i * 0x40 + 0x40;
        if (gMainMenuCursor == i)
            tile += 0x10;
        sub_080761F0(((i * 16 + 0x1D) << 16) | x, 0x4080, tile);
        sub_080761F0(((i * 16 + 0x1D) << 16) | 0x58, 0x4080, tile + 4);
        sub_080761F0(((i * 16 + 0x1D) << 16) | 0x78, 0x4080, tile + 8);
        sub_080761F0(((i * 16 + 0x1D) << 16) | 0x98, 0x4080, tile + 12);
    }
}

/* MainMenu_Init: sub-state machine that clears DISPCNT, resets video and BGM,
 * and loads graphics. */
u16 sub_08003920(void)
{
    struct Main *main = &gMain;
    u8 *state = &main->seqState1;

    switch (*state) {
    case 0:
        REG_DISPCNT = 0;
        gMainMenuCursor %= 7;
        break;
    case 1:
        sub_080759F4();
        sub_08073574();
        sub_080757AC();
        REG_BG1CNT = 0x84;
        main->vblankFlags = 3;
        sub_08077B54(3);
        break;
    default:
        sub_080752B0((void *)OBJ_PLTT, gUnk_087DE858, 0x20);
        sub_080752B0((void *)OBJ_VRAM0, gUnk_087DE878, 0x4000);
        sub_08072FAC(0, 0, 0x20, gUnk_087D4B24);
        return 1;
    }
    (*state)++;
    return 0;
}

/* Main menu step 1: turn on BG1 and OBJ, draw the items, fade in. */
u16 sub_080039C0(void)
{
    REG_DISPCNT = 0x1200;
    sub_08003850();
    return sub_08075AE4(1);
}

/* MainMenu_HandleInput: Up/Down move the cursor (wrapping over 7 items), A confirms. */
u16 sub_080039E0(void)
{
    sub_08003850();
    if (gMain.newKeys & DPAD_UP) {
        gMainMenuCursor += 6;
        gMainMenuCursor %= 7;
        sub_08077AEC(0);
    }
    if (gMain.newKeys & DPAD_DOWN) {
        gMainMenuCursor += 8;
        gMainMenuCursor %= 7;
        sub_08077AEC(0);
    }
    if (gMain.newKeys & A_BUTTON) {
        sub_08077AEC(1);
        sub_08077BCC();
        return 1;
    }
    return 0;
}

/* MainMenu_Launch: fade out, then switch to the scene picked by the cursor. */
u16 sub_08003A58(void)
{
    sub_08003850();
    if (sub_08075A6C(2)) {
        gMain.step488A = 0;
        sub_080754F8(gUnk_081984D8[gMainMenuCursor]);
    }
    return 0;
}

/* CB_MainMenu: step runner over gUnk_081984F4, index gMain.seqIndex1. */
u16 sub_08003AA4(void)
{
    StepFunc step = gUnk_081984F4[(u8)gMain.seqIndex1];
    if (step != NULL) {
        if (step()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* Record step 0: clear the record state, reset video, set up BG0-3. */
u16 sub_08003AF4(void)
{
    sub_08075278(&gRecord, 4);
    sub_080759F4();
    sub_080757AC();
    sub_08073574();
    REG_MOSAIC = 0;
    REG_DISPCNT = 0;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x4105;
    REG_BG2CNT = 0x4306;
    REG_BG3CNT = 0x0507;
    gMain.vblankFlags = 0x63;
    return 1;
}

/* Record step 1: load palettes/tiles and BG images, draw both panels, reset the view state. */
u16 sub_08003B64(void)
{
    sub_08075294((void *)0x05000220, gUnk_087E2878, 0x20);
    sub_08075294((void *)0x06010000, gUnk_087E2C78, 0x200);
    sub_08075294((void *)0x05000200, gUnk_087E2A78, 0x20);
    sub_08075294((void *)0x06010400, gUnk_087E2E78, 0x200);
    sub_08075294((void *)0x06010800, gUnk_087E3078, 0x200);
    sub_08075294((void *)0x05000000, gUnk_087E3278, 0x20);
    sub_08075294((void *)0x06004080, gUnk_087E3478, 0x140);
    sub_0807332C(0, 0, 0x10, 0x10, gUnk_087E4280);
    sub_0807332C(5, 0, 0x20, 0x80, gUnk_087E35B8);
    sub_080040E4(0, 0);
    sub_080040E4(1, 1);
    gRecord.unk0_0 = 0;
    gRecord.unk0_1 = 0;
    gRecord.unk0_3 = 0;
    gRecord.unk1_5 = 0;
    return 1;
}

u16 sub_08003C58(void)
{
    sub_0800406C();
    REG_DISPCNT = 0x1F00;
    return sub_08075AE4(2);
}

/* Record-screen input: when a card is selected, animate/scroll it; otherwise
 * LEFT/RIGHT (with R/L) move the selection and confirm. Returns 1 to leave. */
/* sub_08003EEC/sub_08003ED4 are defined later in the unit; the original called them
 * undeclared (implicit int), so their u16 results are tested without narrowing. */
typedef int (*IntFn_08003C78)(void);
u16 sub_08003C78(void)
{
    sub_0800406C();
    if (gRecord.unk0_1) {
        if (gRecord.unk0_3) {
            gRecord.unk0_3--;
            gMain.bgHofs[1] = gUnk_08198508[gRecord.unk0_0][gRecord.unk0_1 - 1][gRecord.unk0_3];
            gMain.bgHofs[2] = gUnk_08198508[gRecord.unk0_0][gRecord.unk0_1 - 1][gRecord.unk0_3];
        } else {
            switch (gRecord.unk0_1) {
            case 1:
                gRecord.unk1_5++;
                break;
            case 2:
                gRecord.unk1_5--;
                break;
            }
            gRecord.unk0_1 = 0;
            gRecord.unk0_0 = 1 - gRecord.unk0_0;
            if (gRecord.unk0_0) {
                gMain.bgHofs[1] = 0x100;
                gMain.bgHofs[2] = 0x100;
            } else {
                gMain.bgHofs[1] = 0;
                gMain.bgHofs[2] = 0;
            }
        }
    } else {
        if (gMain.newKeys & 0x110) {
            if (((IntFn_08003C78)sub_08003EEC)()) {
                sub_080040E4(gRecord.unk1_5 + 1, 1 - gRecord.unk0_0);
                gRecord.unk0_1 = 1;
                gRecord.unk0_3 = 0x10;
                sub_08077AEC(0);
                return 0;
            }
            sub_08077AEC(3);
        }
        if (gMain.newKeys & 0x220) {
            if (((IntFn_08003C78)sub_08003ED4)()) {
                sub_080040E4(gRecord.unk1_5 - 1, 1 - gRecord.unk0_0);
                gRecord.unk0_1 = 2;
                gRecord.unk0_3 = 0x10;
                sub_08077AEC(0);
                return 0;
            }
            sub_08077AEC(3);
        }
    }
    if (gMain.newKeys & 3) {
        sub_08077AEC(2);
        return 1;
    }
    return 0;
}

u16 sub_08003E80(void)
{
    sub_0800406C();
    return sub_08075A6C(2);
}

/* CB_Record: step runner over gUnk_08198588, index gMain.seqState2. */
u16 sub_08003E94(void)
{
    StepFunc step = gUnk_08198588[(u8)gMain.seqState2];
    if (step != NULL) {
        if (step())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}

u16 sub_08003ED4(void)
{
    if (gRecord.unk1_5)
        return 1;
    return 0;
}

u16 sub_08003EEC(void)
{
    switch (gRecord.unk1_5) {
    case 0:
        return sub_08063BAC();
    case 1:
        return sub_08063C14();
    case 2:
        return sub_08063C7C();
    case 3:
        return sub_08063CE4();
    }
    return 0;
}

void sub_08003F34(void)
{
    u32 frame = (gMain.frameCounter >> 3) & 3;
    if (sub_08003ED4())
        sub_080761F0(0x00200010, 0x40, frame * 4 + 0x20);
    if (sub_08003EEC())
        sub_080761F0(0x002000E0, 0x40, frame * 4 + 0x22);
}

/* Record scene: draw the up/down arrows left of each of the 4-5 shown cards. */
struct WF3F88Rec { u32 wins : 11; u32 losses : 11; u32 draws : 10; };
struct WF3F88Save { u8 pad0[0x20D0]; struct WF3F88Rec rec[32]; };
#define WF3F88Save (*(struct WF3F88Save *)gUnk_02011C20)

void sub_08003F88(void)
{
    s32 i, n;
    u32 frame, y;
    s32 diff, d;

    if (gRecord.unk0_1)
        return;
    n = (gRecord.unk1_5 <= 3) ? 5 : 4;
    frame = (gMain.frameCounter >> 3) & 7;
    for (i = 0; i < n; i++) {
        diff = WF3F88Save.rec[gRecord.unk1_5 * 5 + i + 1].wins - WF3F88Save.rec[gRecord.unk1_5 * 5 + i + 1].losses;
        d = (u32)diff >> 31;
        if (diff > 0)
            d = -1;
        sub_080761F0(((i * 24 + 0x2B) << 16) | (0x84 + d * 0x30), 0x4000, gUnk_08198618[frame] + 0x1000);
    }
}

void sub_0800406C(void)
{
    sub_08003F88();
    sub_08003F34();
}

/* Writes a 3-digit decimal number into BG map buffer `bg` at tile (x, y); digit tiles start at 4. */
void sub_0800407C(s32 bg, s32 x, s32 y, s32 value)
{
    s32 i;

    x += 2;
    for (i = 0; i < 3; i++) {
        gMain.bgMapBuffer[bg][(u16)x + (u16)y * 32] = value % 10 + 4;
        value /= 10;
        x--;
    }
}
/* Draw the opponent-record panels: a 4- or 5-row block (a = page), each row's
 * portrait plus the win/loss/draw counters, or an empty placeholder. */
#if 0 /* NONMATCHING: logic matches, but agbcc keeps `a` in sl and spills/rematerialises several loop variables, producing a 0x28-byte frame vs the ROM's 0x1C and a different register map (ROM: b r5, tileBase r7, y r8, row r9, sl/sl, col sp18) */
void sub_080040E4(s32 a, s32 b)
{
    u16 tileBase = b * 0xC0;
    s32 i;
    s16 n;
    u16 sp10;
    s32 sp14;
    u32 col;
    s32 y;
    s32 yRow;
    s32 row;
    u16 sl;

    sub_08073500((u16)(b + 1), 0, 0x20, 0x20);
    sub_08073500((u16)(b + 3), 0, 0x20, 0x20);
    sub_0807332C((u16)(b + 3), 0x63, 0x30, tileBase + 0xA0,
                 (a <= 3) ? gUnk_087E52A4 : gUnk_087E5CF4);
    sp14 = a * 4;
    sub_0807332C((u16)(b + 1), 0x63, 0x40, tileBase + 0xC0, gUnk_08198604[a]);
    n = 4;
    if (a <= 3)
        n = 5;
    i = 0;
    if (n > 0) {
        sp10 = (u16)(b + 1) << 16;
        col = 0x630000;
        y = tileBase + 0xE0;
        sl = 0x40000;
        row = 5;
        yRow = (b * 5 + 5) * 0x10;
        do {
            u16 k = (u16)(sp14 + a + i);
            u16 id = k + 1;

            if (sub_08063DAC(id) != 0) {
                struct Card2Rec *rec = (struct Card2Rec *)((u8 *)gUnk_02011C20 + id * 4);
                struct Card2RecB *recb = (struct Card2RecB *)((u8 *)gUnk_02011C20 + id * 4);
                struct Card2RecC *recc = (struct Card2RecC *)((u8 *)gUnk_02011C20 + id * 4);

                sub_0807332C((u16)(b + 1), (sl >> 16) * 0x20 + 4, yRow, y, gUnk_081985A0[k]);
                sub_0800407C(b + 1, 0xD, row, rec->a.a);
                sub_0800407C(b + 1, 0x13, row, recc->c.c);
                sub_0800407C(b + 1, 0x19, row, recb->b.b);
            } else {
                sub_0807332C((u16)(b + 1), (sl >> 16) * 0x20 + 4, yRow, y, gUnk_087E77B4);
                sub_08073500((u16)(sp10 >> 16), col >> 16, 8, 1);
            }
            col += 0x600000;
            y += 0xC;
            sl += 0x30000;
            row += 3;
            yRow += 0x10;
            i++;
        } while (i < n);
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080034B8", sub_080040E4); /* 0x080040E4 size 0x19C */

/* Leap year test (Gregorian). */
u32 sub_08004280(u32 year)
{
    if ((year & 3) != 0 || (year % 100 == 0 && year % 400 != 0))
        return 0;
    return 1;
}

/* Days in month (1-based), with Feb +1 in leap years. */
u32 sub_080042B4(u32 year, u32 month)
{
    u32 days = gUnk_08198628[month - 1];
    if (month == 2)
        days += sub_08004280(year);
    return days;
}

/* Day of the week (0 = Sunday) for a date, counting days since 2000-01-01 (a Saturday). */
s32 sub_080042D8(u32 year, u32 month, u32 day)
{
    u32 m;
    s32 days = day - 1;
    u32 yearInCycle = year & 3;

    if (year >= 2000)
        year -= 2000;
    days += (year / 4) * 1461;
    days += 1 + yearInCycle * 365;
    if ((year & 3) == 0)
        days--;
    for (m = 1; m < month; m++) {
        days += gUnk_08198628[m - 1];
        if (m == 2)
            days += sub_08004280(year + 2000);
    }
    return (days + 6) % 7;
}

/* Holiday bit mask for a date (Japanese public holidays of ~2000-2002 plus two extra days). */
u32 sub_08004358(u32 year, u32 month, u32 day)
{
    u32 flags = 0;

    switch (month) {
    case 1:
        if (day == 1)
            flags |= 1;
        sub_080042D8(year, month, 1);
        sub_080042D8(year, month, day);
        if ((day - 1) / 7 == 1 && sub_080042D8(year, month, day) == 1)
            flags |= 0x800; /* Coming of Age Day: 2nd Monday */
        break;
    case 2:
        if (day == 11)
            flags |= 2;
        if (day == 24)
            flags |= 0x2000;
        break;
    case 4:
        if (day == 29)
            flags |= 4;
        break;
    case 5:
        switch (day) {
        case 3:
            flags |= 8;
            break;
        case 4:
            flags |= 0x10;
            break;
        case 5:
            flags |= 0x20;
            break;
        }
        break;
    case 7:
        if (day == 20)
            flags |= 0x40;
        if (day == 7)
            flags |= 0x4000;
        break;
    case 9:
        if (day == 15)
            flags |= 0x80;
        break;
    case 10:
        sub_080042D8(year, month, 1);
        sub_080042D8(year, month, day);
        if ((day - 1) / 7 == 1 && sub_080042D8(year, month, day) == 1)
            flags |= 0x1000; /* Health and Sports Day: 2nd Monday */
        break;
    case 11:
        switch (day) {
        case 3:
            flags |= 0x100;
            break;
        case 23:
            flags |= 0x200;
            break;
        }
        break;
    case 12:
        if (day == 23)
            flags |= 0x400;
        break;
    }
    return flags;
}

/* TRUE if the date is a "red" day: Sunday, a holiday, or the Monday after a holiday. */
u32 sub_08004494(u32 year, u32 month, u32 day)
{
    if (sub_080042D8(year, month, day) == 0
        || (sub_08004358(year, month, day) & 0x7FFF)
        || (sub_080042D8(year, month, day) == 1 && (sub_08004358(year, month, day - 1) & 0x7FFF)))
        return 1;
    return 0;
}
