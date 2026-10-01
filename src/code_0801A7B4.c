#include "global.h"
#include "gba.h"
#include "main.h"
#include "duel.h"

/*
 * Duel screen helpers: duel-screen state init, empty debug stubs, fade-out / exit handlers,
 * trunk bookkeeping and the duel-result screens.  See wiki/functions/code-0801a7b4.md.
 */

/* Save image at 0x02011C20: per-card trunk entries from +0x08 (4 bytes per card ID). */
struct TrunkEntry {
    u16 count : 10;         /* copies owned (hypothesis) */
    u16 unk10 : 6;
    u16 unk2;
};
/* Win / loss / draw record against one opponent (save +0x20D0 + 4 * opponent). */
struct DuelRecord {
    u32 wins : 11;
    u32 losses : 11;
    u32 draws : 10;
};

struct SaveData {
    u8 filler0[8];
    struct TrunkEntry trunk[0x800]; /* +0x08, indexed by card ID */
    u8 filler2008[0x20D0 - 0x2008];
    struct DuelRecord records[32];  /* +0x20D0, indexed by opponent (1-30) */
    u16 unk2150;            /* +0x2150 */
    u8 filler2152[0x2158 - 0x2152];
    u16 unk2158;            /* +0x2158: opponent index (hypothesis: last opponent) */
    u8 filler215A[0x215E - 0x215A];
    u16 unk215E;            /* +0x215E */
    u16 unk2160;            /* +0x2160 */
    u16 unk2162;
    u16 unk2164;            /* +0x2164: bit 0 = pending event (text 350) */
};
extern struct SaveData gUnk_02011C20;

typedef u16 (*StepFunc)(void);

/* Unpacked date (sub_08004914 output). */
struct Date {
    u32 year : 12;
    u32 month : 4;
    u32 day : 5;
};

/* gMain (0x03000040) is struct Main from main.h. This unit's +0x4888 split (bits 1 and
 * 2-3 as separate fields) differs from main.h's unk4888_0:4, which changes the generated
 * code, so the two functions that touch it (sub_0801AE2C, sub_0801B75C) use this unit-
 * specific view; every other function uses the canonical struct Main. */
struct MainFlags0801A7B4 {
    u8 filler0[0x4857];
    u8 seqIndexCampaign;    /* +0x4857 */
    u8 filler4858[0x4870 - 0x4858];
    u8 unk4870_0 : 1;       /* +0x4870 bit 0 */
    u8 opponent : 5;        /* +0x4870 bits 1-5 */
    u8 unk4870_6 : 2;
    u8 filler4871[0x487C - 0x4871];
    u32 events;             /* +0x487C */
    u8 filler4880[0x4888 - 0x4880];
    u8 unk4888_0 : 1;       /* +0x4888 */
    u8 unk4888_1 : 1;
    u8 unk4888_2 : 2;
    u8 counter4888 : 2;     /* bits 4-5; canonical name */
    u8 unk4888_6 : 2;
    s8 score;               /* +0x4889 */
    u16 unk488A_0 : 4;      /* +0x488A */
    u16 step488A : 8;
    u16 unk488A_12 : 4;
};
extern struct MainFlags0801A7B4 gMainFlags asm("gUnk_03000040");
#define gMain gUnk_03000040

/* Duel screen state at 0x020185B8. */
struct DuelScreen {
    u32 unk0;               /* +0x00 */
    u8 unk4;                /* +0x04: bit 0 = player, bits 1+ = state */
    u8 unk5;                /* +0x05 */
};

extern struct DuelScreen gUnk_020185B8;

struct Unk03000000 {
    u32 unk0;
    u32 unk4;               /* +0x04: HBlank handler (hypothesis) */
};
extern struct Unk03000000 gUnk_03000000;

/* Duel state at 0x020192E0 and the player struct at 0x020192E4 come from duel.h. */
/* Byte view of the duel flags at +0x1B12: duel.h splits that byte into 1/1/3/1/2-bit
 * fields, but the unit tests it with a byte mask (& 0x20), which needs this byte view. */
struct DuelFlagByte {
    u8 filler0[0x1B12];
    u8 unk1B12;
};
#define DUEL_FLAGS_1B12 (((struct DuelFlagByte *)&gUnk_020192E0)->unk1B12)

/* Card number to card ID table (gCardNumberToId). */
extern const u16 gUnk_08623DF4[];
/* 60 card numbers (0xFFFF-terminated?) at 0x08081A6C. */
extern const u16 gUnk_08081A6C[];

/* Card number to card ID (0xFFFF maps to 0; numbers >= 2000 map to ID(no - 2000) + 1). */
static inline u16 CardNumberToId(u16 no)
{
    if (no == 0xFFFF)
        return 0;
    if (no <= 1999)
        return ((const u16 *)0x08623DF4)[no & 0x7FF];
    return ((const u16 *)0x08623DF4)[(no - 2000) & 0x7FF] + 1;
}

#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

u16 sub_08075A6C(u16 step);     /* FadeToBlack */
void sub_0807255C(void);
u16 sub_08029DCC(void);
u32 sub_08001AE4(void);         /* text-box runner; nonzero when finished */
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);
void sub_08077BCC(void);        /* FadeOutBGM */
void sub_08001C10(u16 textId);  /* StartDialogue */
void sub_08077498(u16 id);
void sub_0800817C(void);        /* LoadPlayerDeckFromSave */
void sub_08023228(void);
int sub_0801F744(void);
void sub_08007E68(int player, int n);   /* Deck_Shuffle */
void sub_08074554(void);
void sub_080759F4(void);
void sub_08073574(void);
void sub_080757AC(void);
void sub_0807326C(u16 a, u16 b, u16 c, const void *img);
u16 sub_08075AE4(u16 step);     /* FadeFromBlack */
void sub_08022A9C(int a);
void sub_08022BFC(int a);
void sub_08004914(struct Date *out);
u32 sub_080044E4(u32 year, u32 month, s32 day);   /* calendar event flags for a date */
void sub_08077B24(u16 song);    /* PlayBGM */
void sub_080754F8(StepFunc cb); /* SetMainCallback */
int sub_08063BAC(void);
int sub_08063C14(void);
int sub_08063C7C(void);
int sub_08076F9C(void);         /* Random */
void sub_08072510(void);
void sub_08075278(void *dst, u32 size);
/* 0x02015EE8: only byte +1 bit 0 is used here. */
/* Duel-intro / result dialogue state at 0x02017FB0 (bits at +0x304..+0x306). */
struct DuelDialogue {
    u8 filler0[0x304];
    u32 unk304_0 : 2;       /* +0x304 */
    u32 unk304_2 : 1;       /* bit 2 */
    u32 unk304_3 : 6;
    u32 unk304_9 : 1;       /* +0x305 bit 1 */
    u32 unk304_10 : 1;
    u32 unk304_11 : 1;      /* +0x305 bit 3 */
    u32 unk304_12 : 2;
    u32 unk304_14 : 1;      /* +0x305 bit 6 */
    u32 unk304_15 : 1;
    u32 unk304_16 : 1;      /* +0x306 bit 0 */
    u32 unk304_17 : 15;
};
extern struct DuelDialogue gUnk_02017FB0;
extern u8 gUnk_0201CFB0;        /* bit 0: fast mode (see code_08013CDC) */
extern const u8 gUnk_086893D8[];

extern StepFunc const gUnk_08198E7C[];

struct Unk02015EE8 {
    u8 unk0;
    u8 unk1_0 : 1;
    u8 unk1_1 : 7;
};
extern struct Unk02015EE8 gUnk_02015EE8;

/* Reset the acting player's duel-screen state by storing the command word, the player bit and
 * a zeroed byte. `player & ~player` is a byte-match idiom for the constant 0 (agbcc then emits
 * the `movs r0,#0` after the first strb, as the ROM does). */
void sub_0801A7B4(u32 arg, u16 player)
{
    gUnk_020185B8.unk0 = arg;
    gUnk_020185B8.unk4 = player & 1;
    gUnk_020185B8.unk5 = player & ~player;
}
void sub_0801A7CC(void)
{
}
void sub_0801A7D0(void)
{
}
void sub_0801A7D4(void)
{
}
void sub_0801A7D8(void)
{
}
/* Debug printf, compiled out (many callers pass a format string). */
void sub_0801A7DC(const char *fmt, ...)
{
}
void sub_0801A7E4(void)
{
}
void sub_0801A7E8(void)
{
}
void sub_0801A7EC(void)
{
}
void sub_0801A7F0(void)
{
}
/* Duel setup sequence (hypothesis): load the player's deck (needs >= 40 cards, else state 10). */
int sub_0801A7F4(void)
{
    switch (gMain.seqIndex1) {
    case 0:
        sub_0800817C();
        if (gUnk_020192E4[0].deckCount < 40) {
            gMain.seqState0 = 10;
            break;
        }
        gMain.seqIndex1++;
        /* fall through */
    case 1:
        gMain.unk488A_0 = 0;
        sub_08072510();
        sub_08075278((void *)0x02017FB0, 0x494);
        gUnk_020192E0.linkError = 0;
        gMain.seqIndex1++;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        break;
    default:
        sub_0807255C();
        return 1;
    }
    return 0;
}
u16 sub_0801A8A4(void)
{
    gUnk_020192E0.linkError = 0;
    return sub_08029DCC();
}
/* Duel intro sequence (hypothesis): load and shuffle the deck, set up the screen, show the intro
 * dialogue (waits on flags in 0x02017FB0) and fade out. */
int sub_0801A8CC(void)
{
    switch (gMain.seqIndex1) {
    case 0:
        sub_08072510();
        gMain.seqIndex1++;
        return 0;
    case 1:
        if (sub_0801F744()) {
            gUnk_02015EE8.unk1_0 = 1;
            gMain.seqIndex1++;
        }
        return 0;
    case 2:
        sub_0800817C();
        sub_08007E68(0, 4);
        gMain.seqIndex1++;
        return 0;
    case 3:
        sub_08074554();
        REG_DISPCNT = 0;
        REG_MOSAIC = 0;
        REG_BLDCNT = 0;
        REG_BLDY = 0;
        gMain.vblankFlags = 2;
        sub_080759F4();
        sub_08073574();
        sub_080757AC();
        gMain.seqIndex1++;
        return 0;
    case 4:
        sub_0807326C(0, 0, 0x100, gUnk_086893D8);
        if (gUnk_0201CFB0 & 1)
            sub_0802297C(0xEE03, 0, 0, 0);
        gMain.seqIndex1++;
        return 0;
    case 5:
        REG_DISPCNT = 0x100;
        if (sub_08075AE4(4)) {
            if (gMain.unk4870_0)
                return 1;
            gMain.seqIndex1++;
        }
        return 0;
    case 6:
        if (gUnk_02017FB0.unk304_2) {
            sub_0802297C(0xF001, 0, 0, 0);
            sub_08022A9C(0);
            gMain.seqIndex1++;
        }
        return 0;
    case 7:
        if (gUnk_02017FB0.unk304_9) {
            sub_0802297C(0xF012, 0, 0, 0);
            gMain.seqIndex1++;
        }
        return 0;
    case 8:
        if (gUnk_02017FB0.unk304_14) {
            sub_08022BFC(0);
            gMain.seqIndex1++;
        }
        return 0;
    case 9:
        if (gUnk_02017FB0.unk304_11) {
            sub_0802297C(0xF014, 0, 0, 0);
            gMain.seqIndex1++;
        }
        return 0;
    case 10:
        if (gUnk_02017FB0.unk304_16)
            gMain.seqIndex1++;
        return 0;
    case 11:
        if (sub_08075A6C(4))
            gMain.seqIndex1++;
        return 0;
    default:
        gUnk_02017FB0.unk304_2 = 0;
        return 1;
    }
}
int sub_0801ABA0(void)
{
    if (sub_08075A6C(8)) {
        gUnk_02015EE8.unk1_0 = 0;
        sub_0807255C();
        return 1;
    }
    return 0;
}
/* Leave the duel. Fade out, stop BGM, disable the HBlank interrupt and its handler, and start text 320. */
int sub_0801ABCC(void)
{
    sub_0802297C(0xEE00, 0, 0, 0);
    if (sub_08075A6C(4)) {
        sub_08077BCC();
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~2;
        gUnk_03000000.unk4 = 0;
        REG_IME = 1;
        gMain.vblankCallback = 0;
        sub_08001C10(320);
        return 1;
    }
    return 0;
}
u16 sub_0801AC48(void)
{
    if (!(gMain.frameCounter & 7))
        sub_0802297C(0xEE00, 0, 0, 0);
    return sub_08001AE4();
}
int sub_0801AC7C(void)
{
    sub_0807255C();
    return 1;
}
int sub_0801AC88(void)
{
    switch (gMain.step488A) {
    case 0:
        sub_08001C10(401);
        gMain.step488A++;
        break;
    case 1:
        if ((u16)sub_08001AE4()) {
            gMain.step488A++;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    default:
        return 1;
    }
    return 0;
}
/* Run the duel-flow step function table at 0x08198E7C (index gMain.seqState0) until a NULL entry. */
int sub_0801AD18(void)
{
    if (gUnk_08198E7C[gMain.seqState0] != NULL) {
    if (gUnk_02015EE8.unk1_0)
        sub_08023228();
    if (gUnk_08198E7C[gMain.seqState0]()) {
        gMain.seqState0++;
        gMain.step488A = 0;
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
    }
    if ((DUEL_FLAGS_1B12 & 0x20) && (u8)(gMain.seqState0 - 1) <= 2) {
        gMain.seqState0 = 6;
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
    }
    return 0;
    }
    return 1;
}
/* Give one copy of every card (IDs 1-820) that is valid and not yet owned (hypothesis). */
void sub_0801ADE0(void)
{
    int i;
    for (i = 1; i <= 820; i++) {
        if (CARD_NUMBER(i) < 0xFFFF && gUnk_02011C20.trunk[i].count == 0)
            sub_08077498(i);
    }
}
extern const u16 gUnk_080819F6[];   /* BGM per opponent */
extern const u16 gUnk_08081AE4[];
extern const u16 gUnk_08081B16[];
extern const u16 gUnk_08081B48[];
extern const u16 gUnk_08081B7A[];
extern const u16 gUnk_08081BAE[];
extern const u16 gUnk_08081BE2[];
extern const u16 gUnk_08081C16[];
extern const u16 gUnk_08081C42[];
extern const u16 gUnk_08081C76[];

/* Pre-duel dialogue for the opponent picked in gMainFlags.opponent (hypothesis): chooses the
 * text by event flags, opponent record and special dates, sets the duel variant in
 * gMainFlags.unk488A_0 and starts the opponent's BGM. Returns 1 when done, 0 to wait. */
int sub_0801AE2C(void)
{
    u32 opp = gMainFlags.opponent;
    u16 text;
    u32 flags;

    gMainFlags.unk488A_0 = 0;
    flags = gMainFlags.events;
    if (flags == 0x800000) {
        switch (opp) {
        case 11:
            sub_08001C10(11002);
            break;
        case 12:
            sub_08001C10(12002);
            break;
        case 13:
            sub_08001C10(13002);
            break;
        case 14:
            sub_08001C10(14002);
            break;
        case 15:
            sub_08001C10(15002);
            break;
        }
    } else if (opp - 1 <= 29) {
    text = gUnk_08081AE4[opp];
    if (gMainFlags.unk4888_2 == 3) {
        if (flags & 0xF000000) {
            sub_08001C10(gUnk_08081BE2[opp]);
            return 1;
        } else if (flags & 0x400000) {
            sub_08001C10(gUnk_08081BAE[opp]);
            return 1;
        } else if (flags & 0x30000000) {
            sub_08001C10(gUnk_08081C16[opp]);
            return 1;
        }
        sub_08001C10(gUnk_08081B7A[opp]);
        if (gMainFlags.counter4888)
            return 0;
    } else {
        if (gUnk_02011C20.records[opp].wins + gUnk_02011C20.records[opp].losses + gUnk_02011C20.records[opp].draws == 0) {
            if (opp == 22)
                sub_0801ADE0();
        } else if (opp == gUnk_02011C20.unk2158) {
            text = gUnk_08081B16[opp];
        } else {
            text = gUnk_08081B48[opp];
        }
    }

#define PICK(bit, id, variant)                  \
    {                                           \
        sub_08001C10(id);                       \
        gMainFlags.unk4888_2 = 3;               \
        gMainFlags.events = bit;                \
        gMainFlags.unk488A_0 = variant;         \
        sub_08077B24(0x30);                     \
        return 1;                               \
    }
    if ((gMainFlags.events & 1) && opp == 20)
        PICK(1, 20003, 13)
    else if ((gMainFlags.events & 0x800) && opp == 10)
        PICK(0x800, 10003, 3)
    else if ((gMainFlags.events & 2) && opp == 6)
        PICK(2, 6002, 2)
    else if ((gMainFlags.events & 4) && opp == 8)
        PICK(4, 8002, 1)
    else if ((gMainFlags.events & 8) && opp == 5)
        PICK(8, 5003, 6)
    else if ((gMainFlags.events & 0x10) && opp == 4)
        PICK(0x10, 4003, 2)
    else if ((gMainFlags.events & 0x20) && opp == 1)
        PICK(0x20, 1003, 6)
    else if ((gMainFlags.events & 0x40) && opp == 9)
        PICK(0x40, 9002, 5)
    else if ((gMainFlags.events & 0x80) && opp == 12)
        PICK(0x80, 12006, 13)
    else if ((gMainFlags.events & 0x1000) && opp == 3)
        PICK(0x800, 3003, 4)    /* original bug: stores 0x800, not 0x1000 */
    else if ((gMainFlags.events & 0x100) && opp == 7)
        PICK(0x100, 7002, 12)
    else if ((gMainFlags.events & 0x200) && opp == 2)
        PICK(0x200, 2003, 7)
    else if ((gMainFlags.events & 0x400) && opp == 16) {
        sub_08001C10(16002);
        gMainFlags.unk4888_2 = 3;
        gMainFlags.events = 0x400;
        gMainFlags.unk488A_0 = sub_08076F9C() % 7 + 7;
        sub_08077B24(0x30);
        return 1;
    } else if ((gMainFlags.events & 0x8000) && opp == 22) {
        sub_08001C10(22002);
        gMainFlags.unk4888_2 = 3;
        gMainFlags.events = 0x8000;
        gMainFlags.unk488A_0 = sub_08076F9C() % 13 + 1;
        return 1;
    } else if ((gMainFlags.events & 0x10000) && opp == 11) {
        sub_08001C10(11006);
        gMainFlags.unk4888_2 = 3;
        gMainFlags.events = 0x10000;
        gMainFlags.unk488A_0 = sub_08076F9C() % 13 + 1;
        sub_08077B24(0x30);
        return 1;
    } else if ((gMainFlags.events & 0x40000) && (opp == 2 || opp == 10)) {
        sub_08001C10(gUnk_08081C76[opp]);
        gMainFlags.unk4888_2 = 3;
        gMainFlags.events = 0x40000;
        gMainFlags.unk488A_0 = sub_08076F9C() % 6 + 1;
        sub_08077B24(0x30);
        return 1;
    } else if ((gMainFlags.events & 0x80000) && (opp == 1 || opp == 3 || opp == 4 || opp == 5)) {
        sub_08001C10(gUnk_08081C76[opp]);
        gMainFlags.unk4888_2 = 3;
        gMainFlags.events = 0x80000;
        gMainFlags.unk488A_0 = sub_08076F9C() % 6 + 1;
        sub_08077B24(0x30);
        return 1;
    } else if (opp != 21) {
        u32 f = gMainFlags.events;
        if (f & 0x20000)
            PICK(0x20000, gUnk_08081C42[opp], 7)
        else if (f & 0x2000)
            PICK(0x2000, gUnk_08081B7A[opp], 4)
        else if (f & 0x4000)
            PICK(0x4000, gUnk_08081B7A[opp], 3)
    }
#undef PICK
    gMainFlags.events = 0;
    sub_08077B24(gUnk_080819F6[gMainFlags.opponent]);
    sub_08001C10(text);
    }
    return 1;
}
/* Nonzero if the player owns at least n of the 60 cards listed at 0x08081A6C. */
u16 sub_0801B640(int n)
{
    u32 i = 0;
    int count = 0;
    for (; i < 60; i++) {
        if (gUnk_02011C20.trunk[CardNumberToId(gUnk_08081A6C[i])].count != 0)
            count++;
    }
    return count >= n;
}
/* Pick a random card from the 60-entry list at 0x08081A6C that the player owns (trunk count != 0). */
u16 sub_0801B6D4(void)
{
    u16 id;
    do {
        u32 r = (u32)sub_08076F9C() % 60;
        id = CardNumberToId(gUnk_08081A6C[r]);
    } while (gUnk_02011C20.trunk[id].count == 0);
    return id;
}
extern const u16 gUnk_08081A28[];
extern const u16 gUnk_08081A50[];
extern const u16 gUnk_08081A58[];
extern const u16 gUnk_08081A62[];

/* Start-of-day calendar events (hypothesis): check today's date for special events (tournament
 * invitations etc.), pick the event's opponent/text and start the matching dialogue and BGM.
 * Returns 1 when nothing (more) is to be shown. */
int sub_0801B75C(void)
{
    struct Date date;
    u32 events;

    switch (gMainFlags.step488A) {
    case 0:
        sub_0800817C();
        if (gUnk_020192E4[0].deckCount < 40) {
            gMainFlags.seqIndexCampaign = 10;
            return 0;
        }
        gMainFlags.step488A++;
        /* fall through */
    case 1:
        sub_08004914(&date);
        events = sub_080044E4(date.year, date.month, date.day);
        if (date.month == 12 && date.day == 31) {
            gUnk_02011C20.unk215E = 0;
            gUnk_02011C20.unk2160 = 0;
        }
        gMainFlags.unk4888_1 = 0;
        gMainFlags.unk4888_2 = 1;
        gMainFlags.counter4888 = 0;
        gMainFlags.score = 0;
        if (events & 0x1000000) {
            gMainFlags.opponent = gUnk_08081A28[sub_08076F9C() % 5];
            sub_08001C10(0xC9);
            sub_08077B24(0x31);
            gMainFlags.events = 0x1000000;
            gMainFlags.step488A = 9;
            return 0;
        } else if (events & 0x2000000) {
            gMainFlags.opponent = gUnk_08081A28[sub_08076F9C() % 5 + 5];
            sub_08001C10(0xCB);
            sub_08077B24(0x31);
            gMainFlags.events = 0x2000000;
            gMainFlags.step488A = 9;
            return 0;
        } else if (events & 0x4000000) {
            gMainFlags.opponent = gUnk_08081A28[sub_08076F9C() % 5 + 10];
            sub_08001C10(0xCD);
            sub_08077B24(0x31);
            gMainFlags.events = 0x4000000;
            gMainFlags.step488A = 9;
            return 0;
        } else if (events & 0x8000000) {
            gMainFlags.opponent = gUnk_08081A28[sub_08076F9C() % 5 + 15];
            gMainFlags.events = 0x8000000;
            sub_08001C10(0xCF);
            gMainFlags.step488A = 9;
            sub_08077B24(0x32);
            return 0;
        } else if (events & 0x10000000) {
            gMainFlags.opponent = gUnk_08081A50[sub_08076F9C() & 3];
            gMainFlags.events = 0x10000000;
            sub_08001C10(0x2BD);
            gMainFlags.step488A = 9;
            gUnk_02011C20.unk2160 = 0;
            sub_08077B24(0x33);
            return 0;
        } else if (events & 0x20000000) {
            gMainFlags.opponent = gUnk_08081A58[sub_08076F9C() % 5u];
            gMainFlags.events = 0x20000000;
            sub_08001C10(0x2BE);
            gMainFlags.step488A = 9;
            sub_08077B24(0x33);
            return 0;
        } else if (events & 0x400000) {
            int n = 1;
            if (sub_08063BAC())
                n = 2;
            if (sub_08063C14())
                n++;
            if (sub_08063C7C())
                n++;
            n *= 5;
            gMainFlags.opponent = gUnk_08081A28[sub_08076F9C() % n];
            sub_08001C10(500);
            gMainFlags.events = 0x400000;
            gMainFlags.step488A = 9;
            sub_08077B24(0x30);
            return 0;
        } else if (sub_0801B640(5) && gUnk_02011C20.unk2150 != 0 && (u16)(gUnk_02011C20.unk2150 % 60) == 0) {
            sub_08001C10(900);
            sub_08077B24(0x1A);
            gMainFlags.events = 0x800000;
            gMainFlags.step488A = 7;
            return 0;
        } else if (gUnk_02011C20.unk2164 & 1) {
            gUnk_02011C20.unk2164 &= ~1;
            sub_08001C10(350);
            gMainFlags.step488A = 10;
            return 0;
        } else {
            gMainFlags.events = events;
            return 1;
        }
    case 7:
        if (!sub_08001AE4())
            return 0;
        gMainFlags.opponent = gUnk_08081A62[sub_08076F9C() % 5];
        sub_08077B24(0x1B);
        gMainFlags.unk4888_1 = 1;
        return 1;
    case 8:
        sub_080754F8(0);
        return 0;
    case 9:
        if (!sub_08001AE4())
            return 0;
        gMainFlags.unk4888_1 = 1;
        gMainFlags.unk4888_2 = 3;
        return 1;
    case 10:
        if (!sub_08001AE4())
            return 0;
        return 1;
    }
    return 1;
}
