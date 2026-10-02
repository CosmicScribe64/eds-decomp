#include "global.h"

#include "gba.h"

/* gMain: the big system struct at 0x03000040 (see wiki ram-map). */
struct Main {
    u32 rngState;                       /* 0x0000 */
    u16 heldKeys;                       /* 0x0004 */
    u16 newKeys;                        /* 0x0006 */
    u16 prevKeys;                       /* 0x0008 */
    u16 keyRepeatTimer;                 /* 0x000A */
    u8 intrMainBuf[0x400];              /* 0x000C */
    u16 intrCheck;                      /* 0x040C */
    u16 vblankFlags;                    /* 0x040E */
    u16 (*callback)(void);              /* 0x0410 */
    void (*vblankCallback)(void);       /* 0x0414 */
    void (*vblankCallbackEarly)(void);  /* 0x0418 */
    u16 bgMapBuffer[8][0x400];          /* 0x041C */
    u16 unk441C;                        /* 0x441C */
    u16 unk441E;                        /* 0x441E */
    u16 bgVofs[4];                      /* 0x4420 */
    u16 bgHofs[4];                      /* 0x4428 */
    u8 oamBuffer[0x400];                /* 0x4430 */
    u8 oamCount;                        /* 0x4830 */
    u8 unk4831;                         /* 0x4831 */
    u8 brightness;                      /* 0x4832 */
    u8 unk4833;                         /* 0x4833 */
    u16 unk4834;                        /* 0x4834 */
    u16 hblankScroll[16];               /* 0x4836 */
    u8 unk4856;                         /* 0x4856 */
    u8 seqIndexCampaign;                /* 0x4857 */
    u8 seqState0;                       /* 0x4858 */
    u8 seqIndex1;                       /* 0x4859 */
    u8 seqState1;                       /* 0x485A */
    u8 seqState2;                       /* 0x485B */
    u16 currentBgm;                     /* 0x485C */
    u16 frameCounter;                   /* 0x485E */
    u8 frameCounter8;                   /* 0x4860 */
    u8 vblankCounter8;                  /* 0x4861 */
    u16 unk4862;                        /* 0x4862 */
    u16 vblankCounter;                  /* 0x4864 */
    u16 lagCounter;                     /* 0x4866 */
    u16 lastSeFrame;                    /* 0x4868 */
    u8 filler486A[0x4878 - 0x486A];     /* 0x486A */
    u8 seqIndexTop;                     /* 0x4878 */
    u8 unk4879;                         /* 0x4879 */
    u8 unk487A;                         /* 0x487A */
};

/* gTitleState at 0x0201527C */
struct TitleState {
    u16 scroll;                         /* 0x0: BG3 scroll, decremented each frame */
    u16 savePresent : 1;                 /* 0x2 bit 0 */
    u16 continueSelected : 1;            /* 0x2 bit 1 */
};

extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
extern struct TitleState gUnk_0201527C;
#define gTitleState gUnk_0201527C
/* gSaveData at 0x02011C20 (0x2170-byte save image) */
struct SaveData {
    u8 filler0[0x2150];
    u16 days;                           /* 0x2150: in-game calendar day count (hypothesis) */
    u8 filler2152[0x215E - 0x2152];
    u16 unk215E;                        /* 0x215E: unlock counter (hypothesis) */
    u16 unk2160;                        /* 0x2160 */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

extern u16 (*const gUnk_0819879C[])(void);
extern const u8 gUnk_087D01F4[];
extern const u8 gUnk_087C056C[];
/* IWRAM 0x03000000: interrupt vectors (hypothesis: +4 = HBlank callback) */
struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);
};
extern struct IntrVectors gUnk_03000000;
u16 sub_08075AE4(u16 step);
u16 sub_080057BC(void);
s32 sub_080753CC(const u8 *str); /* StrLen */
void sub_0807501C(s32 x, s32 y, u16 color, const u8 *str); /* DrawText (hypothesis) */
void sub_08075114(void *dest, u16 b);
void sub_08074B08(u8 a, u8 b);
extern const u8 gUnk_087D292C[];

/* Starting-deck card pool (0x08198744, 11 entries). See [[deck-lists]]. */
struct DeckPool {
    const u16 *cards;
    u32 count:10;       /* cards in the pool */
    u32 take0:5;        /* copies drawn for starting choice 0 */
    u32 take1:5;        /* ... choice 1 */
    u32 take2:5;        /* ... choice 2 */
};
extern const struct DeckPool gUnk_08198744[];
extern const u16 gUnk_08623DF4[]; /* card ID to card index */
extern const u8 gUnk_080813E4[];
s32 sub_08076F9C(void); /* Random */
void sub_080774EC(u16 card);
void sub_0801A7DC(const u8 *fmt, u32 arg);
void sub_0801A7E8(void);

/* Converts a card ID (0..1999 and 2000+) to a card index; 0xFFFF maps to 0. Inlined wherever it appears. */
static inline u16 CardIdToIndex(u16 id)
{
    if (id == 0xFFFF)
        return 0;
    if (id < 2000) {
        /* FAKEMATCH: keep the initialized lookup mask and its working copy
         * separate, then add the table to the already scaled byte offset. */
        register u32 mask asm("r3") = 0x7FF;
        register u32 copy asm("r1") = mask;
        u32 off;
        register const u16 *table asm("r4");

        asm("" : : "r"(mask));
        off = (id & copy) * 2;
        table = gUnk_08623DF4;
        off += (u32)table;
        return *(const u16 *)off;
    }
    {
        u32 off = ((id - 2000) & 0x7FF) * 2;
        /* FAKEMATCH: the alternate-ID path uses a separate table scratch. */
        register const u16 *table asm("r3");

        table = gUnk_08623DF4;
        off += (u32)table;
        return *(const u16 *)off + 1;
    }
}
extern const u8 gUnk_080813F0[];
void sub_0807326C(u16 a, u16 b, u16 c, const void *img);

u32 sub_080042D8(u32 year, u32 month, u32 day); /* day of week */
u32 sub_08004280(u32 year);
u32 sub_08004358(u32 year, u32 month, u32 day);
u32 sub_08004494(u32 a, u32 b, u32 c);
/* Unpacked date */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};
void sub_080047F4(struct Date *date, u16 days);
extern const u8 gUnk_08198628[]; /* days per month */
void sub_08004F2C(void);
void sub_08004FA4(void);
void sub_08004FD8(void);
void sub_08072FAC(u16 mapBase, u16 palIdx, u16 tileBase, const void *img);
void sub_08073498(void);
void sub_08073574(void);
void sub_08075278(void *dst, u32 size);
void sub_080757AC(void);
void sub_080759F4(void);
void sub_08075A30(void);
u16 sub_08075A6C(u16 step);
u16 sub_08075B58(u16 step);
u16 sub_08075BD0(u16 step);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
u32 sub_08077034(void);
void sub_08077AEC(u16 id);
void sub_08077B54(u16 id);
void sub_08077BCC(void);

/*
 * Returns a bitmask of restrictions/flags for the calendar entry (arg0, arg1, arg2),
 * which is a year, a month and a day-like value. Starts from sub_08004358()'s flags and
 * ORs in bits per case, then checks surrounding days via sub_08004494 (day-info) and
 * the save-data unlock counters.
 */
static inline u32 WeekOfMonth(u32 year, u32 month, u32 day)
{
    sub_080042D8(year, month, 1);
    sub_080042D8(year, month, day);
    return (day - 1) / 7 + 1;
}

u32 sub_080044E4(u32 year, u32 month, u32 day)
{
    u32 bit20, bit21;
    u32 flags = sub_08004358(year, month, day);

    switch (month - 2) {
    case 0:
        if (day == 0xE)
            flags |= 0x40000;
        break;
    case 1:
        if (day == 0xE)
            flags |= 0x80000;
        break;
    case 4:
        if (day == 0x1C)
            flags |= 0x8000;
        if (WeekOfMonth(year, month, day) == 1 && sub_080042D8(year, month, day) == 6)
            flags |= 0x10000000;
        if (WeekOfMonth(year, month, day - 1) == 1 && sub_080042D8(year, month, day - 1) == 6
            && gSaveData.unk2160 != 0)
            flags |= 0x20000000;
        break;
    case 8:
        if (day == 0x1F)
            flags |= 0x10000;
        break;
    case 9:
        if (sub_080042D8(year, month, day) == 0) {
            switch (WeekOfMonth(year, month, day)) {
            case 1:
                flags |= 0x1000000;
                break;
            case 2:
                if (gSaveData.unk215E != 0)
                    flags |= 0x2000000;
                break;
            case 3:
                if (gSaveData.unk215E > 1)
                    flags |= 0x4000000;
                break;
            case 4:
                if (gSaveData.unk215E > 2)
                    flags |= 0x8000000;
                break;
            }
        }
        break;
    case 10:
        if (day == 0x18)
            flags |= 0x20000;
        break;
    }
    if (sub_080042D8(year, month, day) == 6) {
        u32 v = WeekOfMonth(year, month, day);
        if (v == 2 || v == 4)
            flags |= 0x400000;
    }
    /* The shared exit keeps flags at one return use, which lets month win r6 over flags. */
    if ((sub_08004494(year, month, day) << 16) != 0)
        goto end;
    bit20 = 0;
    bit21 = 0;
    if (sub_080042D8(year, month, day) == 2
        && (year > 0x7D1 || month > 1 || day > 2))
        bit20 = 1;
    if (sub_080042D8(year, month, day) == 1
        && (sub_08004494(year, month, day + 1) << 16) != 0)
        bit20 = 1;
    if (sub_080042D8(year, month, day) == 6
        && (sub_08004494(year, month, day + 2) << 16) != 0
        && (sub_08004494(year, month, day + 3) << 16) != 0)
        bit20 = 1;
    if (bit20 != 0)
        flags |= 0x100000;
    if (day == 0x15)
        bit21 = 1;
    if (day == 0x14 && (sub_08004494(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (day == 0x13 && (sub_08004494(year, month, 0x14) << 16) != 0
        && (sub_08004494(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (day == 0x12 && (sub_08004494(year, month, 0x13) << 16) != 0
        && (sub_08004494(year, month, 0x14) << 16) != 0
        && (sub_08004494(year, month, 0x15) << 16) != 0)
        bit21 = 1;
    if (bit21 != 0)
        flags |= 0x200000;
end:
    return flags;
}

/* Converts a day count (day 0 = 2001-01-01) into a packed date plus weekday.
 * Day 36524 (2100-02-29, not a leap day) is skipped. */
void sub_080047F4(struct Date *date, u16 days)
{
    u16 year, month;
    s32 monthLen = 31;

    if (days > 36523)
        days++;
    year = days / 1461 * 4;
    days = days % 1461;
    year += days / 365 + 2001;
    if (days == 1460) {
        days = 365;
        year--;
    } else {
        days = days % 365;
    }
    month = 0;
    while (days >= monthLen) {
        days -= monthLen;
        month++;
        monthLen = gUnk_08198628[month];
        if (month == 1)
            monthLen += sub_08004280(year);
    }
    date->year = year;
    date->month = month + 1;
    date->day = days + 1;
    date->weekday = sub_080042D8(year, month + 1, days + 1);
    if (date->day == 0)
        date->day++;
}
void sub_08004914(struct Date *date)
{
    sub_080047F4(date, gSaveData.days);
}

u32 sub_08004930(u32 a, u32 b, u32 c)
{
    sub_080042D8(a, b, 1);
    sub_080042D8(a, b, c);
    return (c - 1) / 7 + 1;
}

/* Builds the starting deck for `choice` (choice % 3) by shuffling each of the 11 pools
 * and adding the first take<n> cards of each. The scene supplies choice 0..2. */
void sub_0800495C(s32 choice)
{
    u16 buf[64];
    const struct DeckPool *pool = gUnk_08198744;
    u32 p;
    s32 i, n;

    for (p = 0; p <= 10; pool++, p++) {
        const u16 *src = pool->cards;

        for (i = 0; i < pool->count; i++)
            buf[i] = src[i];
        for (i = 0; i < pool->count * 4; i++) {
            s32 a = sub_08076F9C() % pool->count;
            s32 b = sub_08076F9C() % pool->count;
            u16 t = buf[a];
            buf[a] = buf[b];
            buf[b] = t;
        }
        switch (choice % 3) {
        case 0:
            n = pool->take0;
            break;
        case 1:
            n = pool->take1;
            break;
        case 2:
            n = pool->take2;
            break;
        }
        for (i = 0; i < n; i++) {
            u16 id = buf[i % pool->count];
            u16 idx = CardIdToIndex(id);

            if (idx)
                sub_080774EC(idx);
            else
                sub_0801A7DC(gUnk_080813E4, id);
        }
    }
    sub_0801A7E8();
}

/* HBlank handler: wavy BG1 horizontal scroll */
void sub_08004ABC(void)
{
    REG_BG1HOFS = gMain.hblankScroll[(REG_VCOUNT + gMain.frameCounter) & 0xF];
}

/* License step 0: License_InitVideo */
u16 sub_08004AF8(void)
{
    switch (gMain.seqState0) {
    default:
        return 1;
    case 0:
        sub_08075A30();
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 1:
        gMain.vblankFlags = 3;
        sub_08073574();
        sub_080757AC();
        REG_BG0CNT = 0x84;
        REG_BG1CNT = 0x105;
        REG_BG2CNT = 0x206;
        REG_BG3CNT = 0x307;
        *(vu16 *)0x05000000 = 0xFFFF;
        gMain.seqState0++;
        return 0;
    }
}
/* License step 1: draws the centred notice text 0x080813F0 (drawn twice, offset, as an outline) and holds it. */
#if 0 /* NONMATCHING: only the outline loop nest differs. The target strength-reduces x+i+j into a
       * j-loop giv (r5, copied to r8) and keeps x in sl; here x+i is hoisted and j is added per iteration. */
u16 sub_08004B84(void)
{
    s32 x, i, j, k;

    switch (gMain.seqState0) {
    case 0:
        sub_08073498();
        sub_08074B08(0x20, 3);
        x = (240 - sub_080753CC(gUnk_080813F0) * 9) / 2;
        for (i = 1; i >= 0; i--)
            for (j = 0; j <= 1; j++)
                for (k = 0; k <= 0; k++)
                    sub_0807501C(x + i + j, k + i, i == 1 ? 0x100F : 0x1008, gUnk_080813F0);
        sub_08075114((void *)0x06004400, 0);
        for (i = 0; i < 96; i++)
            gMain.bgMapBuffer[1][0x120 + i] = i + 0x20;
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x200;
        if (!sub_08075BD0(1))
            return 0;
        return 0;
        gMain.seqState0++;
    case 2:
        if (gMain.seqIndex1++ < 120)
            return 0;
        gMain.seqIndex1 = 0;
        gMain.seqState0++;
        return 0;
    default:
        if (!sub_08075B58(1))
            break;
        REG_DISPCNT &= ~0x200;
        return 1;
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080044E4", sub_08004B84); /* 0x08004B84 size 0x138 */
/* License step 2: shows the logo image 0x087D01F4, holds it 120 frames, fades out. */
u16 sub_08004CBC(void)
{
    switch (gMain.seqState0) {
    case 0:
        sub_08073498();
        sub_08072FAC(0, 0, 0x20, gUnk_087D01F4);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x100;
        if (!sub_08075BD0(1))
            return 0;
        gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ < 120)
            return 0;
        gMain.seqIndex1 = 0;
        gMain.seqState0++;
        return 0;
    default:
        if (!sub_08075B58(1))
            break;
        REG_DISPCNT &= ~0x100;
        return 1;
    }
    return 0;
}
/* License step 3: second logo (0x087D292C), then hands over to the title screen. */
u16 sub_08004D60(void)
{
    switch (gMain.seqState0) {
    case 0:
        sub_08073498();
        sub_08072FAC(0, 0, 0x20, gUnk_087D292C);
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT |= 0x100;
        if (sub_08075BD0(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (gMain.seqIndex1++ >= 120) {
            gMain.seqIndex1 = 0;
            gMain.seqState0++;
        }
        return 0;
    case 3:
        if (sub_08075A6C(1)) {
            REG_DISPCNT &= ~0x100;
            gMain.seqState0++;
        }
        return 0;
    default:
        gMain.vblankCallbackEarly = NULL;
        gMain.vblankCallback = NULL;
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~2;
        gUnk_03000000.hblankCallback = NULL;
        REG_IME = 1;
        gMain.seqIndexTop = 0;
        gMain.unk4879 = 0;
        gMain.unk487A = 0;
        gMain.seqIndexCampaign = 0;
        gMain.seqState0 = 0;
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        gMain.callback = sub_080057BC;
        return 0;
    }
}

/* CB_License: runs the step table at 0x0819879C */
u16 sub_08004EAC(void)
{
    u16 (*step)(void) = gUnk_0819879C[gMain.seqIndexTop];

    if (step != NULL) {
        if (step()) {
            gMain.seqIndexTop++;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* Title VBlank callback: scroll BG3 diagonally */
void sub_08004F08(void)
{
    gTitleState.scroll--;
    REG_BG3VOFS = gTitleState.scroll >> 2;
    REG_BG3HOFS = gTitleState.scroll;
}

/*
 * Draws the "New Game" / "Continue" labels (a 64x32 + 32x32 sprite each).
 * The unselected option uses the tiles 12 further on (the dimmed version).
 */
void sub_08004F2C(void)
{
    s32 i;

    for (i = 0; i <= 1; i++) {
        s32 x = 0x18 + i * 0x70;
        s32 t = i * 0x80 + 0x200;
        u16 tile = t;
        if (gTitleState.continueSelected != i)
            tile += 12;
        sub_080761F0(x | (0x68 << 16), 0x40C0, tile);
        sub_080761F0((0x58 + i * 0x70) | (0x68 << 16), 0x80, tile + 8);
    }
}

void sub_08004FA4(void)
{
    REG_DISPCNT = 0;
    REG_BG0CNT = 0x84;
    REG_BG1CNT = 0x105;
    REG_BG2CNT = 0x206;
    REG_BG3CNT = 0x307;
}

/* Title step: sets up the title screen. Copies the palettes/text/logo images, fills a
 * 4x4-tile pattern into the IWRAM tile map, halves the brightness of palette entries
 * 0xC0..0xCF, copies the HBlank scroll table and installs the VBlank/HBlank callbacks. */
extern u8 gUnk_03004876[];
extern const u8 gUnk_0822C300[];
extern const u8 gUnk_08081408[];
extern const u8 gUnk_08081414[];
extern const u8 gUnk_08198830[];
extern const u8 gUnk_087BDAA8[];
extern const u8 gUnk_087C1DCC[];
extern const u8 gUnk_087C0CD4[];
extern const u8 gUnk_0867DFCC[];
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_08075294(void *dst, const void *src, u32 size);
void sub_08004F08(void);
void sub_08004ABC(void);
#if 0 /* NONMATCHING: the 4x4 tile-map fill loop's register allocation differs. agbcc
       * splits the running tile value into several literals instead of the target's single
       * r1 chain (ldr r1,=0xC389; 15x add r1,#1) and hoists the last values; also the
       * tile-map base ends up in r3 instead of r4. Address shape is otherwise exact. */
void sub_08004FD8(void)
{
    s32 x, y, i;
    s32 t;

    sub_080752B0((void *)0x05000200, gUnk_0822C300, 0x20);
    sub_08074B08(0x20, 0x10);
    sub_0807501C(9, 9, 0x100F, gUnk_08081408);
    sub_0807501C(8, 8, 0x1007, gUnk_08081408);
    sub_0807501C(0x73, 0xB, 0xC01, gUnk_08081408);
    sub_0807501C(0x72, 0xA, 0xC0D, gUnk_08081408);
    sub_0807501C(1, 0x29, 0x100F, gUnk_08081414);
    sub_0807501C(0, 0x28, 0x1007, gUnk_08081414);
    sub_0807501C(0x69, 0x2B, 0xC01, gUnk_08081414);
    sub_0807501C(0x68, 0x2A, 0xC0D, gUnk_08081414);
    sub_08075114((void *)0x06014000, 0);
    sub_08075294((void *)0x05000000, gUnk_0822C300, 0x20);
    *(s16 *)0x05000000 = 0;
    sub_08072FAC(0x20, 0x10, 0x10, gUnk_087BDAA8);
    sub_0807326C(0x409, 0xA0, 0x2B8, gUnk_087C1DCC);
    sub_0807326C(0x809, 0xB0, 0x310, gUnk_087C0CD4);
    sub_0807326C(0xC00, 0xC0, 0x388, gUnk_0867DFCC);
    for (y = 0; y <= 0x1F; y += 4) {
        for (x = 0; x <= 0x1F; x += 4) {
            s32 o = (y << 5) + x;

            gMain.bgMapBuffer[3][o] = 0xC388;
            t = 0xC389;
            gMain.bgMapBuffer[3][o + 1] = t;
            gMain.bgMapBuffer[3][o + 2] = ++t;
            gMain.bgMapBuffer[3][o + 3] = ++t;
            gMain.bgMapBuffer[3][o + 0x20] = ++t;
            gMain.bgMapBuffer[3][o + 0x21] = ++t;
            gMain.bgMapBuffer[3][o + 0x22] = ++t;
            gMain.bgMapBuffer[3][o + 0x23] = ++t;
            gMain.bgMapBuffer[3][o + 0x40] = ++t;
            gMain.bgMapBuffer[3][o + 0x41] = ++t;
            gMain.bgMapBuffer[3][o + 0x42] = ++t;
            gMain.bgMapBuffer[3][o + 0x43] = ++t;
            gMain.bgMapBuffer[3][o + 0x60] = ++t;
            gMain.bgMapBuffer[3][o + 0x61] = ++t;
            gMain.bgMapBuffer[3][o + 0x62] = ++t;
            gMain.bgMapBuffer[3][o + 0x63] = ++t;
        }
    }
    for (i = 0; i <= 0xF; i++) {
        u32 v = ((u16 *)0x05000180)[i];

        ((u16 *)0x05000180)[i] = ((v & 0x7C00) >> 1 & 0x7C00) | ((v & 0x1F) >> 1 & 0x1F)
                               | ((v & 0x3E0) >> 1 & 0x3E0);
    }
    sub_08075294(gMain.hblankScroll, gUnk_08198830, 0x20);
    gMain.vblankCallback = sub_08004F08;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    gUnk_03000000.hblankCallback = sub_08004ABC;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080044E4", sub_08004FD8); /* 0x08004FD8 size 0x2A4 */
/* Title step 0: Title_Init */
u16 sub_0800527C(void)
{
    switch (gMain.seqState0) {
    case 0:
        sub_08075278(&gTitleState, 4);
        gTitleState.savePresent = sub_08077034();
        gTitleState.continueSelected = gTitleState.savePresent;
        gMain.seqState0++;
        return 0;
    case 1:
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 2:
        sub_080759F4();
        sub_08073574();
        sub_080757AC();
        sub_08004FA4();
        gMain.vblankFlags = 3;
        gMain.seqState0++;
        return 0;
    }
    return 1;
}
/* Title step 1: Title_Setup */
u16 sub_08005310(void)
{
    switch (gMain.seqState0) {
    default:
        sub_08004FD8();
        sub_08077B54(0);
        return 1;
    case 0:
        REG_DISPCNT = 0;
        gMain.seqState0++;
        return 0;
    case 1:
        sub_080759F4();
        sub_08073574();
        sub_080757AC();
        sub_08004FA4();
        gMain.vblankFlags = 3;
        gMain.seqState0++;
        return 0;
    }
}
/* Title step 2: fade in, then drop the HBlank handler and draw the menu labels. */
u16 sub_08005368(void)
{
    switch (gMain.seqState0) {
    case 0:
        REG_DISPCNT = 0x600;
        gMain.seqState0++;
    case 1:
        if (!(gMain.frameCounter & 3) && sub_08075AE4(1))
            gMain.seqState0++;
        return 0;
    case 2:
        if (sub_08075B58(4)) {
            REG_IME = 0;
            REG_IE &= ~2;
            gUnk_03000000.hblankCallback = NULL;
            REG_IME = 1;
            REG_IME = 0;
            REG_IE &= ~2;
            REG_IME = 1;
            REG_DISPCNT |= 0x1900;
            sub_0807326C(0xA20, 0x90, 0x284, gUnk_087C056C);
            sub_08004F2C();
            gMain.seqState0++;
        }
        return 0;
    default:
        sub_08004F2C();
        return sub_08075BD0(1);
    }
}

/* Title step 4: Title_FadeOut */
u16 sub_0800545C(void)
{
    sub_08004F2C();
    if (sub_08075A6C(4)) {
        gMain.vblankCallback = NULL;
        return 1;
    }
    return 0;
}

/* Title step 3: Title_HandleInput */
u16 sub_0800548C(void)
{
    sub_08004F2C();
    if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT)) {
        if (gTitleState.savePresent) {
            gTitleState.continueSelected = 1 - gTitleState.continueSelected;
            sub_08077AEC(0);
        } else {
            sub_08077AEC(3);
        }
    }
    if (gMain.newKeys & A_BUTTON) {
        sub_08077AEC(1);
        sub_08077BCC();
        return 1;
    }
    return 0;
}
