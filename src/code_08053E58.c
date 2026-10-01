#include "global.h"

/* Menu block 0x0201AE60 (see code_0804EFF0) */
struct Ui {
    u8 u0[0xA];
    u16 w0A;        /* +0xA */
    u8 u0C[2];
    u16 h;          /* +0xE */
    u8 u10[4];
    u16 sel;        /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;       /* +0x22 */
    u8 timer;       /* +0x23 */
};
extern struct Ui gUnk_0201AE60;
struct MainKeys { u8 u0[6]; u16 keys; u8 u8_[0x485E - 8]; u16 frameCounter; };
extern struct MainKeys gUnk_03000040;
extern u16 gUnk_0300489E;   /* gMain.frameCounter */
/* Duel global 0x020192E0 */
struct Duel {
    u8 u0[0x1B50];
    u8 b1B50;
    u8 pad;
    u16 h1B52[5];       /* +0x1B52 candidate card ids */
    u8 u1B5C[0x1B62 - 0x1B5C];
    u8 step;            /* +0x1B62 */
    u8 pad2;
    u16 sel;            /* +0x1B64 */
};
extern u8 gUnk_020192E0[];
#define DUEL (*(struct Duel *)gUnk_020192E0)
extern const u16 gUnk_081A4424[];
extern const u8 gUnk_08086350[];
int sub_08062140(u16 a);
void sub_08077AEC(u16 se);
/* A card instance word in the duel lists (see code_08008A1C). */
struct DuelCardBits {
    u32 id : 12;
    u32 unk12 : 20;
};
struct DuelZone {
    u32 card;               /* +0: struct DuelCard word */
    u8 unk4[0x94 - 4];
};
/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;           /* +2 */
    u8 unk3;
    u8 numGrave;            /* +4 */
    u8 unk5[7];
    u8 b0C_0 : 5;
    u8 flag0C_5 : 1;        /* +0xC bit 5 (hypothesis: the player's monsters are "sealed") */
    u8 b0C_6 : 2;
    u8 unkD[0x28 - 0xD];
    struct DuelZone zones[11];      /* +0x28 */
    u32 hand[80];                  /* +0x684 */
    u32 deck[80];                  /* +0x7C4 */
    u32 grave[80];                  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};
extern struct DuelPlayer gUnk_020192E4[2];
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
extern u32 gUnk_02019BE8[];
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
#define ZONE(p, i) (*(u32 *)(((p) & 1) * 0xD64 + (i) * 0x94 + (u32)gUnk_0201930C))
#define GRAVE(p, i) (*(u32 *)(0x02019BE8 + ((p) & 1) * 0xD64 + (i) * 4))
#define CARD_ID(w) (((struct DuelCardBits *)&(w))->id)
#define CARD_STATS(id) (*(gUnk_08621DE0 + ((id) & 0x7FF)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
int sub_0800C8BC(int player, int zone);
int sub_0800CAF0(int player, int zone);
/* Duel action record at 0x0201CF90 (see code_08055EB0): only the fields read here. */
struct ActRec {
    u32 player : 1;
    u32 zone5 : 5;
    u32 zone8 : 8;
    u32 f14 : 1;        /* bit 14: a card is attached (hypothesis) */
    u32 f15 : 1;
    u32 f16 : 3;
    u32 f19 : 3;
    u32 pad22 : 3;
    u32 f25 : 1;
    u32 f26 : 1;
    u32 pad27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 pad30 : 1;
    u32 cardId : 16;    /* bits 31-46 (straddles the word boundary) */
    u32 pad47 : 17;
    u32 card8;          /* +8 */
    u16 h0C;            /* +0xC */
    u16 lo5 : 5;        /* +0xE bits 0-4 */
    u16 step : 7;       /* +0xE bits 5-11: step of the action */
    u16 hi4 : 4;
};
void sub_08017FF4(int a, int b);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_0801FBCC(u32 a, int b);
void sub_080467B0(int player);
void sub_08018ED8(int player, int zone, int a, int b);
void sub_08018544(int player, int zone, int a);
u16 sub_080576BC(u16 id, int a);
extern const u8 gUnk_08086370[];
void sub_08054770(void);
u16 sub_0805487C(void);
extern struct ActRec gUnk_0201CF90;
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, u32 affine);
void sub_0805F074(u16 id, int a);
void sub_0805ED9C(void);
int sub_08076F9C(void);
void sub_080602A4(u16 a, u16 b, u16 c, const u8 *d);
void sub_08060308(u16 a, void (*b)(void), u16 (*c)(void));
int sub_08008524(int player, u16 number);
int sub_08008794(int player, u16 number);
int sub_080086CC(int player, u16 number);
int sub_0800A2A8(int player, u16 number);
int sub_08008A1C(int player);
int sub_0800966C(u16 id);
int sub_0800756C(u16 cardNo);
int sub_08008668(int player);
int sub_08047170(int player);
int sub_0802CFA0(int player, u16 id, int a);
int sub_08008860(int player);
int sub_08054198(int player, u16 id);
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE_C(id) ((CARD_STATS_C(id) & 0x1F00000) >> 20)
/* Monster level used by the prompts: 0 for Magic/Trap/Ticket, 10 for type 0x18, else stats bits 25-28. */
static inline int GetCardLevel(u16 id)
{
    switch ((int)CARD_TYPE_C(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS_C(id) & 0x1E000000) >> 25;
    }
}
/* Card category: 3 for card 1910, 1 for 1911-1912, 7 Magic, 8 Trap, 9 Ticket, else the monster kind (see code_080619E8). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER_C(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE_C(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (CARD_STATS_C(id) & 0xC0000) >> 18;
    }
}
int sub_08008AF8(int player, int a);
u16 sub_08053EF8(void);
void sub_08053E58(void);

void sub_08053E58(void)
{
    int y = (gUnk_0201AE60.b21 - gUnk_0201AE60.h + 2) * 8;
    int i = 0;
    u32 base = (u32)&DUEL;
    int x = 0x28;
    u32 off = 0x1B52;
    u16 *cards;
    u16 *fc;

    cards = (u16 *)(base + off);

    fc = &gUnk_0300489E;
    do {
        u16 id = *cards;
        u16 pulse = ((const u16 *)0x081A4424)[(*fc >> 1) & 0xF];
        u32 yx;
        u16 tile;
        if (i != gUnk_0201AE60.sel)
            pulse = 0x100;
        yx = ((u32)y << 16) | (u32)x;
        tile = sub_08062140(id) + 0x1000;
        sub_08076714(yx, 0x80, tile, (u32)pulse << 16);
        x += 0x20;
        cards++;
    } while (++i <= 4);
}


/* Key callback: left/right cycle the selection (5 entries), A confirms. */
u16 sub_08053EF8(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *st = &u->state;
    if (*st == 0) {
        sub_0805F074(DUEL.h1B52[u->sel], 1);
        (*st)++;
        return 0;
    }
    if (gUnk_03000040.keys & 0x20)
        u->sel = u->sel + 4;
    else if (gUnk_03000040.keys & 0x10)
        u->sel = u->sel + 1;
    else
        goto check_a;
    u->sel = u->sel % 5;
    sub_0805ED9C();
    sub_0805F074(DUEL.h1B52[u->sel], 1);
    return 0;
check_a:
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
/* Candidate pick step: CPU (or forced random) picks one of the 5 candidates; the human gets a menu. */
int sub_08053F98(void)
{
    struct Duel *d = &DUEL;
    int idx;
    /* FAKEMATCH: retain the initialized step pointer across menu setup. */
    register u8 *st asm("r5");
    if (d->b1B50 & 4) {
        idx = sub_08076F9C() % 5;
        goto store;
    }
    {
        /* FAKEMATCH: initialized base/offset copies retain the ADD order. */
        register u32 base asm("r4") = (u32)d;
        register u32 off asm("r0") = 0x1B62;
        asm("" : "+r"(off));
        st = (u8 *)(base + off);
    }
    if (*st != 0) {
        idx = gUnk_0201AE60.sel;
    store:
        {
            u32 off = (u32)idx << 1;
            u32 startOff = 0x1B52;
            u32 range = (u32)d + startOff;
            d->sel = *(u16 *)(off + range);
        }
        return 1;
    }
    sub_080602A4(0x206, 0x213, 0xB, gUnk_08086350);
    sub_08060308(5, sub_08053E58, sub_08053EF8);
    (*st)++;
    return 0;
}


/* Usability test for a card chain: needs card numbers 0x2E1, 0x2F4, 0x320 on the player's side. */
int sub_08054028(int p)
{
    int a = 0, b = 0, c = 0;
    int ok = 1;
    int cnt = 0;
    u16 n = 0x58A;
    if (sub_08008524(0, n) > 0)
        ok = 0;
    if (sub_08008524(1, n) > 0)
        ok = 0;
    if (p != 0 && sub_0800A2A8(p, 0x34D) == 0)
        return 0;
    if (sub_080086CC(p, 0x2E1) && ok) {
        a = 1;
        cnt++;
    }
    if (sub_0800A2A8(p, 0x2E1))
        a = 1;
    if (a == 0)
        return 0;
    if (sub_080086CC(p, 0x2F4) && ok) {
        b = 1;
        cnt++;
    }
    if (sub_0800A2A8(p, 0x2F4))
        b = 1;
    if (b == 0)
        return 0;
    if (sub_080086CC(p, 0x320) && ok) {
        c = 1;
        cnt++;
    }
    if (sub_0800A2A8(p, 0x320))
        c = 1;
    if (c == 0)
        return 0;
    if (sub_08008A1C(p) == 0 && cnt == 0)
        return 0;
    return 1;
}
int sub_08054130(int p)
{
    int f = 0;
    u16 n = 0x58A;
    if (sub_08008524(0, n) > 0 || sub_08008524(1, n) > 0)
        return 0;
    if (sub_08008794(p, 0x582))
        f = 1;
    if (sub_08008794(p, 0x584))
        f = 1;
    if (f != 0 && sub_08008AF8(p, -1) > 1)
        return 1;
    return 0;
}
/* Do the player's field (if `sealed`) or graveyard hold enough matching monsters (1-3) for the ritual/fusion-like card `id`? */
int sub_08054198(int player, u16 id)
{
    int need = 1;
    int mode = 0;
    int i;
    int flag;
    /* FAKEMATCH: retain the initialized mode across the opponent-card test. */
    asm("" : "+r"(mode));
    flag = gUnk_020192E4[player & 1].flag0C_5;
    if (sub_08008524(1 - player, 0x5E7) > 0 && flag == 0)
        return 0;
    switch (CARD_NUMBER_C(id)) {
    case 0x5EA:
        need = 3;
        if (flag) {
            for (i = 0; i <= 4; i++) {
                if (ZONE(player, i) << 20 != 0 && sub_0800C8BC(player, i) == 3) {
                    if (--need == 0)
                        return 1;
                }
            }
        } else {
            for (i = 0; i < gUnk_020192E4[player & 1].numGrave; i++) {
                /* FAKEMATCH: preserve the initialized grave-base load. */
                register u32 addr asm("r0") = (u32)gUnk_02019BE8;
                u32 off = (player & 1) * 0xD64 + i * 4;
                u16 cid;

                cid = CARD_ID(*(u32 *)(off + addr));
                if (CARD_TYPE_C(cid) == 3) {
                    if (--need == 0)
                        return 1;
                }
            }
        }
        return 0;
    case 0x5EB:
        need = 2;
        mode = 1;
        break;
    case 0x5EC:
        mode = 4;
        break;
    case 0x5ED:
        mode = 3;
        break;
    case 0x5EE:
        mode = 5;
        break;
    case 0x5EF:
        mode = 6;
        break;
    }
    if (flag) {
        for (i = 0; i <= 4; i++) {
            if (ZONE(player, i) << 20 != 0 && sub_0800CAF0(player, i) == mode) {
                if (--need == 0)
                    return 1;
            }
        }
    } else {
        for (i = 0; i < gUnk_020192E4[player & 1].numGrave; i++) {
            /* FAKEMATCH: each initialized grave base stays in the load scratch. */
            register u32 addr asm("r0") = (u32)gUnk_02019BE8;
            u32 off = (player & 1) * 0xD64 + i * 4;
            u16 cid;
            u32 st;

            cid = CARD_ID(*(u32 *)(off + addr));
            st = CARD_STATS_C(cid);
            if (((st & 0x1F00000) >> 20) <= 0x14 && (st >> 29) == mode) {
                if (--need == 0)
                    return 1;
            }
        }
    }
    return 0;
}

/* Can the player use card `id` (a monster-or-not candidate of a prompt) right now? Nothing for empty slots, unsafe cards,
   fusion / ritual monsters, then per-card-number conditions; other cards depend on their level. */
/* Return the zero-extended halfword as a word, as the ROM callers consume it. */
int sub_08054398(int player, u16 id)
{
    u16 number;
    int lvl;
    if (id == 0)
        return 0;
    if (sub_0800966C(id) != 0)
        return 0;
    if (CARD_TYPE_C(id) > 0x14)
        return 0;
    if (GetCardSubtype(id) == 3)
        return 0;
    if (GetCardSubtype(id) == 2)
        return 0;
    number = CARD_NUMBER_C(id);
    if (sub_0800756C(number) != 0 && sub_08008668(player) == 0)
        return 0;
    switch (number) {
    case 0x5EA:
    case 0x5EB:
    case 0x5EC:
    case 0x5ED:
    case 0x5EE:
    case 0x5EF:
        if (sub_08047170(player) != 0)
            goto summon_check;
        return 0;
    case 0x2E5:
    case 0x3E:
    case 0x187:
    case 0x4B2:
        return 0;
    case 0x37:
    case 0x38:
    case 0x42:
    case 0x170:
        if (sub_08047170(player) == 0)
            return 0;
        return (u16)(sub_0802CFA0(player, id, 1));
    case 0x4E2:
        if (gUnk_020192E4[player & 1].handCount == 1 && sub_08008A1C(player) > 0)
            return 1;
        if (sub_08008AF8(player, -1) > 1)
            return 1;
        return 0;
    case 0x546:
        if (sub_08008860(player) + 1 < sub_08008860(1 - player) && sub_08008A1C(player) > 0)
            return 1;
        if (sub_08008AF8(player, -1) > 0)
            return 1;
        return 0;
    case 0x175:
        if (sub_08047170(player) == 0)
            return 0;
        if (sub_08008AF8(player, -1) <= 2)
            return 0;
        if (sub_080086CC(player, 0x172) == 0)
            return 0;
        if (sub_080086CC(player, 0x173) == 0)
            return 0;
        if (sub_080086CC(player, 0x174) == 0)
            return 0;
        return 1;
    case 0x34D:
        if (sub_08047170(player) == 0)
            return 0;
        return (u16)(sub_08054028(player));
    summon_check:
        if (sub_08008A1C(player) == 0 && !gUnk_020192E4[player & 1].flag0C_5)
            return 0;
        return (u16)(sub_08054198(player, id));
    case 0x4E9:
        return (u16)(sub_08054130(player));
    default:
        lvl = GetCardLevel(id);
        /* Keep the initialized level at the original switch join; no instructions. */
        __asm__("" : : "r"(lvl));
        switch (lvl) {
        case 5:
        case 6:
            if (sub_08008AF8(player, -1) <= 0)
                return 0;
            return 1;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            if (sub_08008A1C(player) > 0)
                return 1;
            return 0;
        default:
            if (sub_08008AF8(player, -1) > 1)
                return 1;
            return 0;
        }
    }
}

/* Draw callback of the yes/no prompt: two card sprites (the action record's card, or a blank tile) that pulse when selected. */
void sub_08054770(void)
{
    int y = gUnk_0201AE60.w0A * 8 + 0x20;
    u32 yx1, yx2;
    u16 tile1, tile2;
    u32 aff1, aff2;
    y -= (gUnk_0201AE60.h + gUnk_0201AE60.w0A - gUnk_0201AE60.b21 + 2) * 8;
    yx1 = (y << 16) | 0x40;
    tile1 = sub_08062140(gUnk_0201CF90.cardId) | 0x1000;
    if (gUnk_0201AE60.sel == 0)
        aff1 = gUnk_081A4424[(gUnk_03000040.frameCounter & 0x1E) >> 1] << 16;
    else
        aff1 = 0x01000000;
    sub_08076714(yx1, 0x80, tile1, aff1);
    yx2 = (y << 16) | 0x90;
    if (gUnk_0201CF90.f14)
        tile2 = sub_08062140(gUnk_0201CF90.cardId) | 0x1000;
    else
        tile2 = 0x40;
    if (gUnk_0201AE60.sel != 0)
        aff2 = (gUnk_081A4424[(gUnk_03000040.frameCounter & 0x1E) >> 1] << 16) | 0x20;
    else
        aff2 = 0x01000020;
    sub_08076714(yx2, 0x80, tile2, aff2);
}
/* Key callback of a two-entry yes/no prompt: L/R toggle, A confirms; then a 60-tick flash before returning 1. */
u16 sub_0805487C(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *st = &u->state;
    int s = *st;
    unsigned short t = s; /* FAKEMATCH: the short copy keeps the state byte in r2 and `s + 1` in r2 */
    switch (t) {
    case 1:
        if ((&gUnk_0201AE60)->timer <= 0x3B)
            (&gUnk_0201AE60)->timer++;
        else
            *st = s + 1;
        return 0;
    case 2:
        return 1;
    default:
        if (gUnk_03000040.keys & 0x30) {
            sub_08077AEC(0);
            (&gUnk_0201AE60)->sel = 1 - (&gUnk_0201AE60)->sel;
        }
        if (gUnk_03000040.keys & 1) {
            sub_08077AEC(1);
            (&gUnk_0201AE60)->state = 1;
            (&gUnk_0201AE60)->timer = 0;
        }
        return 0;
    }
}
/* Step machine of the action record at 0x0201CF90: step 0 announces it (messages 0xC4/0x80C4), 1 / 2 handle the zone
   (messages 0x71 / 0x90), then the step counter is advanced. Returns 1 when done. */
int sub_08054900(void)
{
    switch (gUnk_0201CF90.step) {
    case 0: {
        u16 msg;
        if (gUnk_0201CF90.f25)
            sub_08017FF4(gUnk_0201CF90.f28, gUnk_0201CF90.f16);
        if (gUnk_0201CF90.f26)
            sub_08017FF4(gUnk_0201CF90.f29, gUnk_0201CF90.f19);
        msg = gUnk_0201CF90.player ? 0x80C4 : 0xC4;
        {
            int id = gUnk_0201CF90.cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)&gUnk_0201CF90 >> 6;
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower));
                lower &= gUnk_0201CF90.zone5;
                mask = lower;
            }
            packed |= mask;
            packed |= (gUnk_0201CF90.f14 | gUnk_0201CF90.f15 << 1) << 8;
            sub_0801EC58(msg, id, packed, 0);
        }
        gUnk_0201CF90.step++;
        return 0;
    }
    case 1:
        sub_08024134(gUnk_0201CF90.player, 0, gUnk_0201CF90.zone5);
        if (!gUnk_0201CF90.f14) {
            gUnk_0201CF90.step = 10;
            return 0;
        }
        sub_0801EC58(0x71, gUnk_0201CF90.cardId, 1, 0);
        gUnk_0201CF90.step++;
        return 0;
    case 2: {
        /* FAKEMATCH: keep the player extraction in its original two scratches. */
        register u32 playerBits asm("r1") = (u32)*(u8 *)&gUnk_0201CF90 << 31;
        register u32 player asm("r3") = playerBits >> 31;
        u32 zone = gUnk_0201CF90.zone5;
        u16 msg;
        u32 a, t;
        if ((*(u32 *)(player * 0xD64 + zone * 0x94 + (u32)gUnk_0201930C) << 20) == 0)
            return 1;
        msg = 0x90;
        if (player)
            msg = 0x8090;
        sub_0801EC58(msg, zone, gUnk_0201CF90.h0C, 0);
        sub_080467B0(gUnk_0201CF90.player);
        switch (*((gUnk_0201CF90.cardId & 0x7FF) + gUnk_08622AB4)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            a = gUnk_0201CF90.player << 31;
            t = (gUnk_0201CF90.zone5 << 16) | 0x0A400000;
            sub_0801FBCC(a | t | gUnk_0201CF90.cardId, gUnk_0201CF90.player | (gUnk_0201CF90.zone5 << 8));
            break;
        case 0x31C:
            sub_08018ED8(gUnk_0201CF90.player, gUnk_0201CF90.zone5, 0, 0);
            break;
        case 0x45E:
        case 0x585:
            sub_08018544(gUnk_0201CF90.player, gUnk_0201CF90.zone5, 1);
            break;
        }
        gUnk_0201CF90.step++;
        return 0;
    }
    default:
        return 1;
    }
}

/* Step machine of the yes/no prompt for the action record at 0x0201CF90: step 0 asks (CPU decides with sub_080576BC, the
   human gets the prompt 0x207/0x30F with the callbacks sub_08054770 / sub_0805487C), 1 stores the answer in the record,
   2 / 3 / 4 announce and run the card (see sub_08054900). Returns 1 when done. */
static inline u16 ActionBNumber(u16 id)
{
    u32 off = (id & 0x7FF) * 2;
    /* FAKEMATCH: this initialized table address uses the original r3 scratch. */
    register const u16 *base asm("r3") = gUnk_08622AB4;
    off += (u32)base;
    return *(const u16 *)off;
}

int sub_08054B60(void)
{
    int step = gUnk_0201CF90.step;
    struct ActRec *r = &gUnk_0201CF90;

    switch (step) {
    case 0:
        if (r->player) {
            gUnk_0201AE60.sel = sub_080576BC(r->cardId, 0);
        } else {
            sub_080602A4(0x207, 0x30F, 0xB, gUnk_08086370);
            sub_08060308(5, sub_08054770, sub_0805487C);
        }
        r->step++;
        return 0;
    case 1: {
        u16 n;
        gUnk_0201CF90.f15 = gUnk_0201AE60.sel;
        if (gUnk_0201CF90.f15)
            gUnk_0201CF90.f14 = 0;
        else
            gUnk_0201CF90.f14 = 1;
        n = 0x47F;
        if (sub_08008524(0, n) != 0 || sub_08008524(1, n) != 0)
            gUnk_0201CF90.f14 = 1;
        goto next_global;
    }
    case 2: {
        u16 msg;
        if (r->f25)
            sub_08017FF4(r->player, r->f16);
        if (r->f26)
            sub_08017FF4(r->player, r->f19);
        msg = r->player ? 0x80C4 : 0xC4;
        {
            int id = r->cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)r >> 6;
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower));
                lower &= r->zone5;
                mask = lower;
            }
            packed |= mask;
            packed |= (r->f14 | r->f15 << 1) << 8;
            sub_0801EC58(msg, id, packed, 0);
        }
        r->step++;
        return 0;
    }
    case 3: {
        /* FAKEMATCH: preserve an initialized pointer copy before the calls. */
        register struct ActRec *copy asm("r5") = r;
        struct ActRec *r2;
        u16 msg;
        asm("" : : "r"(copy));
        r2 = copy;
        sub_08024134(r2->player, 0, r2->zone5);
        msg = r2->player ? 0x8090 : 0x90;
        sub_0801EC58(msg, r2->zone5, r2->h0C, 0);
        if (!r2->f14) {
            r2->step = 10;
            return 0;
        }
        sub_0801EC58(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    }
    case 4: {
        u32 a, t;
        sub_080467B0(gUnk_0201CF90.player);
        switch (ActionBNumber(gUnk_0201CF90.cardId)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            a = gUnk_0201CF90.player << 31;
            t = (gUnk_0201CF90.zone5 << 16) | 0x0A400000;
            sub_0801FBCC(a | t | gUnk_0201CF90.cardId, gUnk_0201CF90.player | (gUnk_0201CF90.zone5 << 8));
            break;
        case 0x31C:
            sub_08018ED8(gUnk_0201CF90.player, gUnk_0201CF90.zone5, 0, 0);
            break;
        case 0x45E:
        case 0x585:
            sub_08018544(gUnk_0201CF90.player, gUnk_0201CF90.zone5, 1);
            break;
        }
    next_global:
        gUnk_0201CF90.step++;
        return 0;
    }
    default:
        return 1;
    }
}

