#include "global.h"
#include "main.h"

#include "gba.h"

/* Calendar screen (0x08002388-0x08002930) and Campaign opponent-select
 * screen (0x08002940-0x08003298). */

typedef u16 (*StepFunc)(void);

/* struct Main (gMain, 0x03000040) comes from include/main.h. */
#define gMain gUnk_03000040

/* 0x0201F7E0: Campaign opponent-select state (0x34 bytes). */
struct OpponentSelect {
    u8 page:3;      /* bits 0-2: page (row of gUnk_0819834C) */
    u16 cursor:3;    /* bits 3-5: selected slot on the page (0-4) */
    u16 unk6:3;     /* bits 6-8 */
    u16 unk9:4;     /* bits 9-12 */
    u32 unk13:4;    /* bits 13-16 */
    u32 unk17:15;
    u16 cursorX[8]; /* +0x04 */
    u16 cursorY[8]; /* +0x14 */
    u8 filler24[0x10];
};

extern struct OpponentSelect gUnk_0201F7E0;

struct Pos16 {
    s16 x;
    s16 y;
};

extern StepFunc gUnk_08198338[];                /* Calendar steps */
extern u16 gUnk_0819834C[];               /* opponent (duelist) id, [page * 5 + slot] */
extern StepFunc gUnk_08198380[];                /* opponent-select steps */
extern struct Pos16 gUnk_081983AC[5];     /* slot screen positions */

extern const u8 gUnk_0871B650[];                /* opponent-select OBJ palette */
extern const u8 gUnk_0871B850[];                /* opponent-select OBJ tiles */

#define gSel gUnk_0201F7E0

/* Calendar scene state at 0x0201F7D0 (same layout as code_08001364.c). */
struct Calendar {
    u16 unk0;               /* +0x00 anchor date */
    u16 date;               /* +0x02 current date */
    u32 shownEvents;        /* +0x04 event mask currently drawn */
    u16 blink:2;            /* +0x08 bits 0-1: setup/animation state (3 = steady) */
    u16 secondHalf:1;       /* +0x08 bit 2: showing weeks 3+ of the month */
    u16 page:1;             /* +0x08 bit 3: displayed Mode-4 frame (DISPCNT bit 4) */
    u16 weekday:3;          /* +0x08 bits 4-6: cursor column (day of week) */
    u16 week:3;             /* +0x08 bits 7-9: cursor row (week within the page) */
};
extern struct Calendar gUnk_0201F7D0;
#define gCalendar gUnk_0201F7D0

/* Unpacked date (sub_080047F4 output). */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};

/* Save image at 0x02011C20 (only the in-game day counter). */
struct SaveData {
    u8 filler0[0x2150];
    u16 days;               /* +0x2150: in-game calendar day count */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

extern const u8 gUnk_087F3D18[];
extern const u8 gUnk_0822C300[];
extern const u8 gUnk_087EA718[];
extern const u8 gUnk_087F3F18[];
extern const u8 gUnk_087F4118[];
extern const u8 gUnk_087F7DF8[];
extern const u8 gUnk_087F7E18[];
extern const u8 gUnk_087F5DD8[];
extern const u8 gUnk_087F4D18[];
extern const u8 gUnk_087F4DD8[];

void sub_08002094(void);
void sub_0800217C(u16 date);
void sub_08002220(void);
void sub_080047F4(struct Date *date, u16 days);
s32 sub_080042B4(u32 year, u32 month);
s32 sub_080042D8(u32 year, u32 month, s32 day);
u32 sub_080044E4(u32 year, u32 month, s32 day);

struct Bitmap {
    const u16 *pal;     /* 256-colour palette */
    const u8 *bitmap;   /* 240x160 8bpp mode-4 bitmap */
};

extern struct Bitmap gUnk_08198440[5];          /* page background per page */
extern const u8 *gUnk_08198468[];               /* opponent names, [page * 5 + slot] */
extern const u8 *gUnk_081984CC[3];              /* "Win", "Lose", "Draw" */
extern const u8 gUnk_080813B8[];                /* "[ Unknown ]" */
extern const u8 gUnk_0871C850[];
extern const u8 gUnk_0871CA50[];
extern const u8 gUnk_0871CB90[];

s32 sub_080753CC(const u8 *str);    /* StrLen */
void sub_0807501C(s32 x, s32 y, u16 color, const u8 *str); /* DrawText (hypothesis) */
void sub_08074B38(s32 a, s32 b, u16 c, s32 d);
void sub_08075114(void *dest, u16 b);

void sub_080034B8(s32 slot, u16 flag);
void sub_080036FC(u16 duelist);
s32 sub_08063DAC(u16 duelist);
u16 sub_08063BAC(void);
u16 sub_08063C14(void);
u16 sub_08063C7C(void);
u16 sub_08063CE4(void);
void sub_08076348(u32 yx, u16 shape, u16 tile, u16 flip);
void sub_080762D0(u32 yx, u16 shape, u16 tile);
void sub_080761F0(u32 yx, u16 shape, u16 tile);
void sub_08075294(void *dest, const void *src, u32 size); /* MemCopy16 */
void sub_080759F4(void);    /* SetBrightnessBlack */
void sub_080757AC(void);    /* ResetBgScroll */
void sub_08073574(void);    /* ResetVideo */
void sub_08077AEC(u16 id);  /* PlaySE */
void sub_08003174(s32 scroll);
void sub_0800323C(u32 slot);
void sub_08003298(s32 page);
u16 sub_08075A6C(u8 step);  /* FadeToBlack */
u16 sub_08075AE4(u8 step);  /* FadeFromBlack */
void sub_080754F8(StepFunc cb); /* SetMainCallback */
u16 sub_08003AA4(void);     /* main menu callback */
void sub_08002388(void);

/* Calendar step 0: draw the month grid (year digits, day numbers, event
 * icons) and the cursor. */
#if 0 /* NONMATCHING: semantics identical, but agbcc spills rowY to the stack
       * (24-byte frame vs 20) and picks different callee-saved registers for
       * day/col/cell/events, so the whole body and literal pool shift. */
void sub_08002388(void)
{
    struct Date d0, d1;
    s32 year, x, digits;
    s32 day, cell, col, daysInMonth;
    u16 pal, iconX, rowY, dayY, iconY;

    sub_08002220();
    sub_080047F4(&d0, gCalendar.date);
    sub_080047F4(&d1, gCalendar.unk0);

    /* Year, right to left in 4 decimal digits. */
    year = d0.year;
    digits = 3;
    x = 0;
    do {
        sub_080761F0((0xE0 - x) | 0x100000, 0, (year % 10) + 0x6240);
        year = year / 10;
        x += 6;
        digits--;
    } while (digits >= 0);

    day = 1;
    cell = sub_080042D8(d0.year, d0.month, 1);
    col = cell;
    daysInMonth = sub_080042B4(d0.year, d0.month);
    if (gCalendar.secondHalf) {
        while (cell <= 20) {
            cell++;
            day++;
            col = (col + 1) % 7;
        }
        cell = 0;
    }
    if (daysInMonth < day)
        return;
    if (cell > 20)
        return;

    iconY = 0x300000;
    dayY = 0x280000;
    rowY = 0;
    iconX = col << 5;
    do {
        u32 events = sub_080044E4(d0.year, d0.month, day);
        s32 iconX2 = iconX + 0x18;

        pal = 0;
        if (col == 0)
            pal = 1;
        else if (col == 6)
            pal = 2;
        sub_080761F0((iconX + 0x10) | dayY, 0, (pal << 5) + 0x6200 + day);
        if (events & 0x100000) {
            sub_080762D0(iconX2 | iconY, 0x40, 0x172);
            iconX2 += 8;
        }
        if (events & 0x200000) {
            sub_080762D0(((rowY + 0x30) << 16) | iconX2, 0x40, 0x174);
            iconX2 += 8;
        }
        if (events & 0x3F400000) {
            sub_080762D0(iconX2 | ((rowY + 0x30) << 16), 0x40, 0x170);
        }
        if (d0.year == d1.year && d0.month == d1.month && day == d1.day) {
            sub_080762D0((iconX + 0xD) | ((rowY + 0x25) << 16), 0x80, 0x17C);
        }
        iconX += 0x20;
        col++;
        if (col > 6) {
            iconX = 0;
            col = 0;
            iconY += 0x180000;
            dayY += 0x180000;
            rowY += 0x18;
        }
        day++;
        cell++;
    } while (day <= daysInMonth && cell <= 20);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08002388", sub_08002388); /* 0x08002388 size 0x1F4 */

/* Calendar step 1: clear the calendar state, set up video and load graphics. */
/* DMA fill, then wait, as two separate blocks with their own register
 * pointers, like the SDK's DmaSet and DmaWait macros. */
u16 sub_0800257C(void)
{
    {
        vu16 zero = 0;
        vu32 *dma = (vu32 *)0x040000D4;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gCalendar;
        dma[2] = 0x81000006;
        dma[2];
    }
    {
        vu32 *dmaRegs = (vu32 *)0x040000D4;

        while (dmaRegs[2] & 0x80000000)
            ;
    }
    gCalendar.unk0 = gSaveData.days;
    gCalendar.date = gSaveData.days;
    sub_080759F4();
    sub_080757AC();
    sub_08073574();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = 3;
    REG_BG1CNT = 3;
    REG_BG2CNT = 3;
    REG_BG3CNT = 3;
    REG_DISPCNT = 4;
    gMain.vblankFlags = 1;
    sub_08075294((void *)0x05000000, gUnk_087F3D18, 0x1E0);
    sub_08075294((void *)0x050001E0, gUnk_0822C300, 0x20);
    sub_08075294((void *)0x06000000, gUnk_087EA718, 0x9600);
    sub_08075294((void *)0x0600A000, gUnk_087EA718, 0x9600);
    sub_08075294((void *)0x050002C0, gUnk_087F3F18, 0x20);
    sub_08075294((void *)0x06014000, gUnk_087F4118, 0xC00);
    sub_08075294((void *)0x05000300, gUnk_087F7DF8, 0x20);
    sub_08075294((void *)0x06015400, gUnk_087F7E18, 0x800);
    sub_08075294((void *)0x050002E0, gUnk_087F5DD8, 0x20);
    gCalendar.blink = 0;
    sub_08075294((void *)0x05000200, gUnk_087F4D18, 0xC0);
    sub_08075294((void *)0x06015C00, gUnk_087F4DD8, 0x1000);
    sub_0800217C(gCalendar.date);
    sub_08002094();
    return 1;
}


u16 sub_08002704(void)
{
    sub_08002388();
    REG_DISPCNT = 0x1F04;
    return sub_08075AE4(4);
}

/* Calendar step 2: handle date (R/L), cursor (d-pad) and confirm input. */
u16 sub_08002728(void)
{
    struct Date d;

    sub_08002388();
    if (gCalendar.blink == 3) {
        if (gMain.newKeys & R_BUTTON) {
            sub_080047F4(&d, gCalendar.date);
            gCalendar.date -= d.day;
            gCalendar.date = gCalendar.date + sub_080042B4(d.year, d.month) + 1;
            gCalendar.blink &= ~3;
            sub_08002094();
            sub_08077AEC(0);
        }
        if (gMain.newKeys & L_BUTTON) {
            sub_080047F4(&d, gCalendar.date);
            if (gCalendar.date > 30) {
                gCalendar.date -= d.day;
                gCalendar.blink &= ~3;
                sub_08002094();
                sub_08077AEC(0);
                sub_080047F4(&d, gCalendar.date);
            }
        }
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        gCalendar.weekday = (gCalendar.weekday + 1) % 7;
        sub_08002094();
        sub_08077AEC(0);
    }
    if (gMain.newKeys & DPAD_LEFT) {
        gCalendar.weekday = (gCalendar.weekday + 6) % 7;
        sub_08002094();
        sub_08077AEC(0);
    }
    if (gMain.newKeys & DPAD_UP) {
        if (gCalendar.week != 0) {
            gCalendar.week = (gCalendar.week - 1) & 7;
            sub_08002094();
            sub_08077AEC(0);
        } else if (gCalendar.secondHalf) {
            gCalendar.secondHalf = 0;
            gCalendar.week = 2;
            sub_08002094();
            sub_08077AEC(0);
        } else {
            sub_08077AEC(3);
        }
    }
    if (gMain.newKeys & DPAD_DOWN) {
        if (gCalendar.week <= 1) {
            gCalendar.week = (gCalendar.week + 1) & 7;
            sub_08002094();
            sub_08077AEC(0);
        } else if (!gCalendar.secondHalf) {
            gCalendar.secondHalf = 1;
            gCalendar.week = 0;
            sub_08002094();
            sub_08077AEC(0);
        } else {
            sub_08077AEC(3);
        }
    }
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        sub_08077AEC(2);
        return 1;
    }
    return 0;
}

u16 sub_08002930(void)
{
    return sub_08075A6C(4);
}

/* Calendar scene main callback: step runner over gUnk_08198338. */
u16 sub_08002940(void)
{
    StepFunc step = gUnk_08198338[gMain.seqState2];

    if (step != NULL) {
        if (step())
            gMain.seqState2++;
        return 0;
    }
    return 1;
}

/* Opponent-select step 0: clear the state, set up video and load graphics. */
u16 sub_08002980(void)
{
    {
        vu16 zero = 0;
        vu32 *dma = (vu32 *)0x040000D4;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gSel;
        dma[2] = 0x8100001A;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
    }
    sub_080759F4();
    sub_080757AC();
    sub_08073574();
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0CNT = 3;
    REG_BG1CNT = 3;
    REG_BG2CNT = 3;
    REG_BG3CNT = 3;
    REG_DISPCNT = 4;
    gMain.vblankFlags = 1;
    sub_08075294((void *)0x05000200, gUnk_0871B650, 0x60);
    sub_08075294((void *)0x06014000, gUnk_0871B850, 0x1000);
    sub_0800323C(gSel.cursor);
    sub_08003298(gSel.page);
    return 1;
}
/* Opponent-select step 1: draw the screen and fade in. */
u16 sub_08002A48(void)
{
    sub_080034B8(gSel.cursor, 1);
    sub_08003174(0);
    sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
    REG_DISPCNT = 0x1F04;
    return sub_08075AE4(4);
}
u16 sub_08002AA0(void)
{
    u16 next;

    sub_080034B8(gSel.cursor, 0);
    sub_08003174(0);
    sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);

    if (gMain.newKeys & DPAD_LEFT) {
        if (gSel.cursor < 4)
            gSel.cursor++;
        else
            gSel.cursor = 0;
        if (gSel.page == 4 && gSel.cursor == 0)
            gSel.cursor = 1;
        sub_08077AEC(0);
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        if (gSel.cursor != 0)
            gSel.cursor--;
        else
            gSel.cursor = 4;
        if (gSel.page == 4 && gSel.cursor == 0)
            gSel.cursor = 4;
        sub_08077AEC(0);
    }
    if (gMain.newKeys & L_BUTTON) {
        if (gSel.page != 0) {
            sub_08077AEC(0);
            gMain.seqIndex1 = 5;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gMain.newKeys & R_BUTTON) {
        next = 0;
        switch (gSel.page) {
        case 0:
            next = sub_08063BAC();
            break;
        case 1:
            next = sub_08063C14();
            break;
        case 2:
            next = sub_08063C7C();
            break;
        case 3:
            next = sub_08063CE4();
            break;
        }
        if (next) {
            sub_08077AEC(0);
            gMain.seqIndex1 = 6;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gMain.newKeys & A_BUTTON) {
        if (sub_08063DAC(gUnk_0819834C[gSel.page * 5 + gSel.cursor])) {
            gMain.opponent = gUnk_0819834C[gSel.page * 5 + gSel.cursor];
            sub_08077AEC(1);
            return 1;
        }
        sub_08077AEC(3);
    }
    if (gMain.newKeys & B_BUTTON) {
        sub_08077AEC(1);
        gMain.seqIndex1 = 8;
    }
    return 0;
}
/* Opponent-select step 3: draw the screen and fade out. */
u16 sub_08002CE8(void)
{
    sub_080034B8(gSel.cursor, 0);
    sub_08003174(0);
    sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
    return sub_08075A6C(4);
}

/* Set BG2X (affine reference point) to x * 2 pixels (20.8 fixed). */
void sub_08002D34(s32 x)
{
    vu16 *reg = (vu16 *)0x04000028;
    u32 v = x << 9;

    *reg++ = v;
    *reg = (v >> 16) & 0xFFF;
}

u16 sub_08002D48(void)
{
    switch (gMain.seqState1) {
    default:
        sub_080034B8(gSel.cursor, 1);
        sub_08002D34(gMain.brightness);
        sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
        if (sub_08075AE4(3)) {
            sub_08002D34(0);
            gMain.seqIndex1 = 2;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        sub_080034B8(gSel.cursor, 1);
        sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
        sub_08002D34(-gMain.brightness);
        if (sub_08075A6C(3)) {
            sub_08003174(-gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        sub_08003174(-gMain.brightness);
        return 0;
    case 1:
        sub_08003298(gSel.page - 1);
        gMain.seqState1++;
        return 0;
    }
    sub_08003174(gMain.brightness);
    return 0;
}
u16 sub_08002E80(void)
{
    switch (gMain.seqState1) {
    default:
        sub_080034B8(gSel.cursor, 1);
        sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
        sub_08002D34(-gMain.brightness);
        if (sub_08075AE4(3)) {
            sub_08002D34(0);
            gMain.seqIndex1 = 2;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    case 0:
        sub_080034B8(gSel.cursor, 1);
        sub_080036FC(gUnk_0819834C[gSel.page * 5 + gSel.cursor]);
        sub_08002D34(gMain.brightness);
        if (sub_08075A6C(3)) {
            sub_08003174(gMain.brightness);
            gMain.seqState1++;
            return 0;
        }
        sub_08003174(gMain.brightness);
        return 0;
    case 1:
        sub_08003298(gSel.page + 1);
        gMain.seqState1++;
        return 0;
    }
    sub_08003174(-gMain.brightness);
    return 0;
}

u16 sub_08002FBC(void)
{
    sub_080754F8(sub_08003AA4);
    return 0;
}

/* Campaign opponent-select runner over gUnk_08198380. */
u16 sub_08002FD0(void)
{
    StepFunc step = gUnk_08198380[gMain.seqIndex1];

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

/* Draw a 64x64 frame from four mirrored corner pieces at (x, y). */
void sub_08003020(s32 x, s32 y)
{
    sub_08076348(x | (y << 16), 0x4080, 0x100, 0);
    sub_08076348(x | ((y + 0x10) << 16), 0x40, 0x120, 0);
    sub_08076348((x + 0x20) | (y << 16), 0x4080, 0x100, 0x1000);
    sub_08076348((x + 0x30) | ((y + 0x10) << 16), 0x40, 0x120, 0x1000);
    sub_08076348(x | ((y + 0x30) << 16), 0x4080, 0x100, 0x2000);
    sub_08076348(x | ((y + 0x20) << 16), 0x40, 0x120, 0x2000);
    sub_08076348((x + 0x20) | ((y + 0x30) << 16), 0x4080, 0x100, 0x3000);
    sub_08076348((x + 0x30) | ((y + 0x20) << 16), 0x40, 0x120, 0x3000);
}
/* Draw the page-scroll arrows: left if not on the first page,
 * right if the next page is unlocked. */
void sub_080030FC(void)
{
    u16 next;

    if (gSel.page != 0)
        sub_080762D0(0x00600000, 0x40, 0x12C);
    next = 0;
    switch (gSel.page) {
    case 0:
        next = sub_08063BAC();
        break;
    case 1:
        next = sub_08063C14();
        break;
    case 2:
        next = sub_08063C7C();
        break;
    case 3:
        next = sub_08063CE4();
        break;
    }
    if (next)
        sub_080762D0(0x006000E0, 0x40, 0x12E);
}
/* Draw the portrait frames of the unlocked opponents on the current page,
 * shifted left by 2*scroll pixels. */
void sub_08003174(s32 scroll)
{
    s32 start = 0;
    s32 i;

    if (gSel.page > 3)
        start = 1;
    for (i = start; i <= 4; i++) {
        if (sub_08063DAC(gUnk_0819834C[gSel.page * 5 + i]) == 0) {
            s32 x = gUnk_081983AC[i].x - scroll * 2;
            s32 y = gUnk_081983AC[i].y;

            sub_08076348(x | (y << 16), 0x80, 0x108, 0);
            sub_08076348((x + 0x20) | (y << 16), 0x80, 0x108, 0x1000);
            sub_08076348(x | ((y + 0x20) << 16), 0x80, 0x108, 0x2000);
            sub_08076348((x + 0x20) | ((y + 0x20) << 16), 0x80, 0x108, 0x3000);
        }
    }
}
/* Move the cursor to `slot` and reset its animation. */
void sub_0800323C(u32 slot)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        gSel.cursorX[i] = gUnk_081983AC[slot].x + 0x10;
        gSel.cursorY[i] = gUnk_081983AC[slot].y;
    }
    gSel.unk6 = slot;
    gSel.unk9 = 0;
    gSel.unk13 = 0;
}
void sub_08003298(s32 page)
{
    s32 count;
    s32 i;

    count = 4;
    if (page <= 3)
        count = 5;
    sub_08002D34(0);
    sub_08075294((void *)0x05000000, gUnk_08198440[page].pal, 0x200);
    sub_08075294((void *)0x06000000, gUnk_08198440[page].bitmap, 0x9600);
    sub_08075294((void *)0x0600A000, gUnk_08198440[page].bitmap, 0x9600);
    gSel.page = page;
    if (page == 4 && gSel.cursor == 0) {
        gSel.cursor = 4;
        sub_0800323C((u16)gSel.cursor);
    }
    sub_08074B38(0x20, 10, 0, 0);
    for (i = 0; i < count; i++) {
        s32 slot = i;

        if (count != 5)
            slot = i + 1;
        if (sub_08063DAC(gUnk_0819834C[page * 5 + slot]) == 0) {
            sub_0807501C(0x31 - sub_080753CC(gUnk_080813B8) * 5 / 2, slot * 16 + 1, 0xA05, gUnk_080813B8);
            sub_0807501C(0x30 - sub_080753CC(gUnk_080813B8) * 5 / 2, slot * 16, 0xA01, gUnk_080813B8);
        } else {
            const u8 *name = gUnk_08198468[page * 5 + slot];
            s32 w = sub_080753CC(name) * 5 / 2;

            sub_0807501C(0x31 - w, slot * 16 + 1, 0xA05, name);
            sub_0807501C(0x30 - w, slot * 16, 0xA01, name);
        }
    }
    sub_08075114((void *)0x06015800, 0);
    sub_08074B38(0x20, 2, 0, 0);
    for (i = 0; i <= 2; i++) {
        const u8 *label = gUnk_081984CC[i];

        sub_0807501C(0x51 + i * 0x20, 1, (u8)(i + 6) | 0xA00, label);
        sub_0807501C(0x50 + i * 0x20, 0, (u8)(i + 2) | 0xA00, label);
    }
    sub_08075114((void *)0x06015000, 0);
    sub_08075294((void *)0x05000260, gUnk_0871C850, 0x20);
    sub_08075294((void *)0x06015000, gUnk_0871CA50, 0x140);
    sub_08075294((void *)0x06015400, gUnk_0871CB90, 0x140);
}
