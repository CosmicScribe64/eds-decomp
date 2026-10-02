#include "global.h"

void sub_0801EC58(u16 cmd, u16 arg2, u16 arg4, u16 arg6);
void sub_08018AE8(int player, int zone, int a);
struct DuelZone { u32 w0; u8 unk4; u8 unk5; u8 flags6; u8 unk7[0x94 - 7]; };
#define ID(z) (((z)->w0 << 20) >> 20)
extern u8 gUnk_020192E0[];
struct AE60 { u8 unk0[0x14]; u16 h14; };
extern struct AE60 gUnk_0201AE60;
extern const char gUnk_08085BD8[];
extern const u16 gUnk_08624244[];
extern const char gUnk_0822C720[];
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
struct DuelZoneF { u32 w0; u8 pad4[0x8C - 4]; u8 b8c; u8 pad8d[0x94 - 0x8D]; };
void sub_08018544(int player, int zone, int a);
struct DuelZoneL { u32 w0; u8 pad4[2]; u8 flags6; u8 pad7; u8 b8; u8 pad9[0x94 - 9]; };
extern const u16 gUnk_08624848[];
struct SB { u8 b0 : 1; u8 who : 1; u8 rest : 6; u8 pad[7]; };
struct PS9 { u8 pad[9]; u8 f0 : 2; u8 f2 : 1; u8 rest : 5; u8 pad2[0xD64 - 10]; };
extern struct PS9 gUnk_020192E4[];
void sub_08024134(int a, int b, int c);
int sub_0804A1C8(void);
void sub_080199E0(int player, int n);
struct BHdr { u8 b0; u8 lo : 1; u8 defSlot : 3; u8 hi : 4; u8 pad[6]; };
extern struct BHdr gUnk_02018450;
extern const u16 gUnk_086249F8[];
int sub_08021628(void);
void sub_08046B54(int player, int zone);
int sub_08008A1C(int player);
int sub_08008A44(int player);
int sub_08008524(int player, u16 number);
int sub_0800A78C(int player, int zone, u16 number);
void sub_08019078(int player, u16 a, u16 b);
void sub_080197E0(int player, u16 id);
extern u8 gUnk_0201CFB0[];
struct MainK { u8 pad0[6]; u16 keys; u8 pad8[0x485E - 8]; u16 anim; };
extern struct MainK gUnk_03000040;
extern const u16 gUnk_081A4424[];
void sub_08076714(u32 a, int b, int c, int d);
extern int (*const volatile gUnk_0819D1D8[])(int);
/* Adjacent duel steps: word +0x1B14 bits 9-16, then halfword +0x1B16
 * bits 1-8. The latter is bits 17-24 relative to +0x1B14, not an alias. */
struct StepWord { u32 lo : 9; u32 step : 8; u32 hi : 15; };
struct StepHalf { u16 lo : 1; u16 step : 8; u16 hi : 7; u8 pad[6]; };

/* Step of a battle effect for card 0x602 or the defender zone's card: if the defender zone (1-player) holds a card and the player's list count is positive and sub_0800A78C(q, def, 0x4DB) holds, send sub_08019078; if card 0x602 is present on either side announce it and finish (return 1), otherwise reset the step and return 0. */
int sub_0804DB6C(int player)
{
    int found = 0;
    int q, qb;
    u8 *e;
    struct DuelZone *zn;
    if (sub_08021628() != 0)
        return 1;
    q = 1 - player;
    sub_08046B54(q, gUnk_02018450.defSlot);
    qb = q & 1;
    zn = (struct DuelZone *)(gUnk_02018450.defSlot * 0x94 + qb * 0xD64 + 0x0201930C);
    if ((zn->w0 << 20) != 0 && sub_08008A1C(player) > 0 && sub_0800A78C(q, gUnk_02018450.defSlot, 0x4DB) != 0) {
        int t = q;
        u32 pos;
        /* FAKEMATCH: preserve the initialized player copy before narrowing;
         * q remains live as the first call argument. Emits no instructions. */
        __asm__("" : "+r"(t));
        pos = (u8)t | gUnk_02018450.defSlot << 8;
        sub_08019078(q, pos, (u8)player | (u8)sub_08008A44(player) << 8);
    }
    if (sub_08008524(player, 0x602) != 0 || sub_08008524(1 - player, 0x602) != 0)
        found = 1;
    if (found == 0) {
        e = gUnk_020192E0;
        ((struct StepWord *)(e + 0x1B14))->step = 1;
        ((struct StepHalf *)(e + 0x1B16))->step = 0;
        return 0;
    }
    sub_080197E0(player, gUnk_086249F8[0]);
    return 1;
}
#if 0 /* NONMATCHING: 0x574 bytes versus the ROM's 0x5B8. The source was
       * audited against the assembly in full; the remaining differences are
       * RAM views and register lifetimes. */
void sub_08007558(void *, const void *);
int sub_08008C6C(int);
int sub_0800A9C8(int, int, u16);
void sub_08018ED8(int, int, int, int);
void sub_08022678(int, int, u16, int);
void sub_08046C6C(int, int, int);
void sub_08056094(int, void *, int, int);
extern u8 gUnk_02015EE8[];
extern u32 gUnk_02019BE8[];
extern const u16 gUnk_08624730[];
extern const u16 gUnk_0862486C[];
extern const u16 gUnk_08622AB4[];
#define DC_STEP(e) (((struct StepHalf *)((e) + 0x1B16))->step)
#define DC_PLAYER(e) (((e)[0x1B17] >> 1) | (((e)[0x1B18] & 1) << 7))
#define DC_INDEX(e) (((struct StepHalf *)((e) + 0x1B18))->step)
#define DC_NUMBER(id) (((const u16 *)gUnk_08622AB4)[(u16)(id) & 0x7FF])
#define DC_CARD(e) ((u16 *)((e) + 0x908 + (DC_PLAYER(e) & 1) * 0xD64 + DC_INDEX(e) * 4))
int sub_0804DC88(int player)
{
    u8 *e = gUnk_020192E0;
    s8 p, i;
    switch (DC_STEP(e)) {
    case 0:
        for (i = 0; i <= 4; i++) {
            struct DuelZone *z = (struct DuelZone *)(i * 0x94 + (player & 1) * 0xD64 + 0x0201930C);
            u32 id = ID(z);
            if (id && DC_NUMBER(id) == 0x540 && (*(u16 *)((u8 *)z + 6) & 0x2003) == 2) {
                sub_080197E0(player, id);
                sub_08018ED8(player, i, 0, 0);
            }
            for (p = 0; p <= 1; p++) {
                if (sub_0800A9C8(p, i, 0x49E)) {
                    sub_080197E0(p, gUnk_08624730[0]);
                    sub_08018544(p, i, 1);
                }
            }
        }
        DC_STEP(gUnk_020192E0)++;
        return 0;
    case 1:
        for (p = 0; p <= 1; p++) {
            for (i = 0; i < ((u8 *)gUnk_020192E4)[(p & 1) * 0xD64 + 4]; i++) {
                u32 card = *(u32 *)((u8 *)gUnk_02019BE8 + (p & 1) * 0xD64 + i * 4);
                if ((s32)(card << 7) < 0) {
                    int q = 1 - p;
                    struct DuelZone *z = (struct DuelZone *)(((card << 4) >> 29) * 0x94 + (q & 1) * 0xD64 + 0x0201930C);
                    u32 id = ID(z);
                    int special = 0;
                    if ((z->flags6 & 2) && id && sub_08008C6C(q) >= 0 && DC_NUMBER(id) == 0x52F)
                        special = 1;
                    sub_08046C6C(p, i, special);
                    return 0;
                }
            }
        }
        DC_STEP(e)++;
        return 0;
    case 2:
        for (p = 0; p <= 1; p++) {
            for (i = 0; i < ((u8 *)gUnk_020192E4)[(p & 1) * 4 + 0xD64]; i++) {
                u32 card = *(u32 *)((u8 *)gUnk_02019BE8 + (p & 1) * 0xD64 + i * 4);
                if ((s32)(card << 3) < 0) {
                    e[0x1B17] = (e[0x1B17] & 1) | (((u16)p & 0x7F) << 1);
                    e[0x1B18] = (e[0x1B18] & ~1) | (((u16)p >> 7) & 1);
                    DC_INDEX(e) = i;
                    DC_STEP(e) = 5;
                    return 0;
                }
            }
        }
        DC_STEP(e)++;
        return 0;
    case 5:
        if (sub_08008A44(1 - DC_PLAYER(e)) == -1) {
            u16 msg = 0xDA;
            if (DC_PLAYER(e))
                msg = 0x80DA;
            sub_0801EC58(msg, DC_INDEX(e), 1, 0);
            DC_STEP(e) = 2;
            return 0;
        }
        sub_080197E0(player, gUnk_0862486C[0]);
        DC_STEP(e)++;
        return 0;
    case 6:
        if (DC_PLAYER(e) && !(gUnk_02015EE8[1] & 1)) {
            *(u16 *)(e + 0x1B64) = 1;
            DC_STEP(e) = 8;
            return 0;
        }
        sub_08022678(1 - DC_PLAYER(e), 0x12, (*(u32 *)DC_CARD(e) << 20) >> 20, 0);
        DC_STEP(e)++;
        return 0;
    case 7:
        if (*(u16 *)(e + 0x1B64)) {
            u16 *card = DC_CARD(e);
            int msg = 0xD3;
            if (DC_PLAYER(e))
                msg = 0x80D3;
            sub_0801EC58(msg, card[0], card[1], 0);
            sub_08007558(e + 0x1B1C, card);
            return 0;
            DC_STEP(e)++;
        } else {
            int msg = 0xDA;
            if (DC_PLAYER(e))
                msg = 0x80DA;
            DC_STEP(e) = 2;
            sub_0801EC58(msg, DC_INDEX(e), 1, 0);
            return 0;
        }
    case 8:
        e[0x1B1F] &= ~0x10;
        sub_08056094(1 - DC_PLAYER(e), e + 0x1B1C, 1, 0x20);
        DC_STEP(e) = 2;
        return 0;
    default:
        {
            u8 *ps = (u8 *)gUnk_020192E4 + (player & 1) * 0xD64;
            if (ps[8] & 0x40) {
                u8 msg = 0x47;
                if (player)
                    msg = 0x8047;
                sub_0801EC58(msg, 0, 0, 0);
                ((struct StepWord *)((u8 *)gUnk_020192E4 + 0x1B10))->step = 0;
                ((struct StepHalf *)((u8 *)gUnk_020192E4 + 0x1B12))->step = 0;
                return 0;
            }
            ps[9] |= 0x10;
            return 1;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804DC88); /* 0x0804DC88 size 0x5B8 */
struct E240Flags { u8 b0:1; u8 b1:1; u8 b2:1; u8 rest:5; };
/* Find the first occupied zone with flag +0x8C bit 1 or 2. */
int sub_0804E240(int player)
{
    int i = 0, j;
    u8 *zb = (u8 *)0x0201930C;
    for (; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            int side = i & 1;
            struct DuelZoneF *zn = (struct DuelZoneF *)(j * 0x94 + side * 0xD64 + (int)zb);
            u8 opp = 1 - i;
            if ((zn->w0 << 20) != 0) {
                if (((struct E240Flags *)&zn->b8c)->b1) {
                    sub_08018544(i, j, 1);
                found:
                    return 0;
                }
                if (((struct E240Flags *)&zn->b8c)->b2) {
                    int r = sub_08008A44(1 - i);
                    u16 msg = 0xA2;
                    u32 pos;
                    if (player != 0)
                        msg = 0x80A2;
                    pos = (u8)i | (u8)j << 8;
                    sub_0801EC58(msg, pos, 0, 0);
                    if (r >= 0) {
                        sub_08019078(player, pos, (u8)r << 8 | opp);
                        goto found;
                    }
                    sub_08018544(i, j, 1);
                    goto found;
                }
            }
        }
    }
    return 1;
}

int sub_0804E31C(int player)
{
    int (*const volatile *table)(int) = gUnk_0819D1D8;
    u8 *e = gUnk_020192E0;
    struct StepWord *step = (struct StepWord *)(e + 0x1B14);
    u32 packed = *(u32 *)step << 15;
    if (table[packed >> 24] != 0) {
        /* FAKEMATCH: keep the initialized shifted step live across the null
         * check, so the callback index is extracted again. No instructions. */
        __asm__("" : "+r"(packed));
        if ((u16)table[packed >> 24](player) != 0) {
            ((struct StepHalf *)(e + 0x1B16))->step = 0;
            step->step++;
        }
        return 0;
    } else {
        int flag = e[0x1B12] & 2;
        int msg = 0x54;
        if (flag)
            msg = 0x8054;
        sub_0801EC58(msg, 0, 0, 0);
        return 1;
    }
}

void sub_0804E3BC(void)
{
}
/* Queue a sprite/effect draw whose first argument depends on how far the list at 0x0201CFB0+0x810 has advanced; the last argument comes from an animation table indexed by bits 1-4 of a gMain halfword. */
void sub_0804E3C0(void)
{
    u8 *e = gUnk_0201CFB0;
    u8 *m;
    const u16 *t;
    int d = *(int *)(e + 0x810) - e[4];
    u32 v = 0x002800A0;
    if (d <= 0x47)
        v = 0x007000A0;
    sub_08076714(v, 0x40C0, 0xF364, (t = gUnk_081A4424, m = (u8 *)&gUnk_03000040, t[(*(u16 *)(m + 0x485E) >> 1) & 0xF] << 16));
}

/* Duel screen fields at 0x0201CFB0 (same layout as struct DuelScreen in code_08012C4C). */
struct E420Screen {
    u8 filler0[0x808];
    u8 unk808_0 : 3;
    u8 busy : 1;
    u8 unk808_4 : 4;
    u8 filler809[0x85C - 0x809];
    u32 unk85C;
};
/* Multi-step routine on the step byte at 0x020192E4+0x1B1C (0x0201AE00), keyed on
 * the active player (bit 1 of the duel flags byte at 0x020192E4+0x1B0E). If that
 * player's byte-9 bit 2 is set, it is cleared and the routine ends (returns 1).
 * Otherwise step 0 sends message 0x50 (0x8050), step 1 finishes with
 * sub_080199E0(1, 1) when bit 1 is set or runs sub_08024134(who, 0xD, 0), step 2
 * sets the screen busy bit, step 4 clears the screen word at +0x85C and finishes
 * with sub_080199E0(0, 1). Other steps advance on key 0x100 when sub_0804A1C8()
 * is 0. Both player indexes are written `who & 1`: CSE shares the constant 1, and
 * combine folds the AND only in the first block, as in the ROM. The early return
 * in the default case keeps the per-case step increments from being cross-jumped. */
int sub_0804E420(void)
{
    u8 *e4 = (u8 *)gUnk_020192E4;
    struct SB *sb = (struct SB *)(e4 + 0x1B0E);
    u8 *st;
    int z = gUnk_020192E4[sb->who & 1].f2;
    if (z != 0) {
        gUnk_020192E4[sb->who & 1].f2 = 0;
        return 1;
    }
    st = e4 + 0x1B1C;
    switch (*st) {
    case 0:
        sub_0801EC58((*(u8 *)sb & 2) ? 0x8050 : 0x50, 0, 0, 0);
        (*st)++;
        return 0;
    case 1:
        if ((*(u8 *)sb & 2) != 0) {
            sub_080199E0(1, 1);
            return 1;
        }
        sub_08024134(sb->who, 0xD, 0);
        (*st)++;
        return 0;
    case 2:
        ((struct E420Screen *)gUnk_0201CFB0)->busy = 1;
        (*st)++;
        return 0;
    case 4:
        ((struct E420Screen *)gUnk_0201CFB0)->unk85C = 0;
        sub_080199E0(0, 1);
        return 1;
    default:
        if (sub_0804A1C8() != 0 || (gUnk_03000040.keys & 0x100) == 0)
            return 0;
        gUnk_020192E0[0x1B20]++;
        return 0;
    }
}
/* For every monster zone (0-4) of `player` holding a face-down card (flags6 bit 1) whose card key is 0x16: send message 0x73 (0x8073 for player 1) and run sub_08018AE8 on it. */
void sub_0804E538(int player)
{
    int i;
    for (i = 0; i <= 4; i++) {
        int s1 = i * 0x94 + (player & 1) * 0xD64;
        struct DuelZone *zn = (struct DuelZone *)(s1 + 0x0201930C);
        u32 id = ID(zn);
        u32 n = id;
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if (((const u16 *)0x08622AB4)[(u16)id & 0x7FF] == 0x16) {
                u32 msg = 0x73;
                if (player != 0)
                    msg = 0x8073;
                sub_0801EC58(msg, n, 1, 0);
                sub_08018AE8(player, i, 0);
            }
        }
    }
}
/* Three-step routine on the step byte 0x020192E0+0x1B22 with a zone counter at +0x1B23. Step 0: needs the player's counter (0x020192E4 halfword) > 0x1F3 and sub_08008A1C(1-p) != 0, then scans zones counter..4 for a face-down card with key 0x228 (advancing the step, twice for player 1 which also sets 0x0201AE60+0x14); step 1 prints a card-name prompt; step 2 sends message 0x43 (0x8043) and sub_08019078 for the found zone, then advances the counter and finally the step.
 * The loop reads the duel state through the global (not the local e) and recomputes player & 1 at the top of its body: loop.c then hoists the constant 1 first (it later serves the h14 store) and CSE keeps the step value across the h14 store. */
int sub_0804E5B4(int player)
{
    char buf[0x80];
    u8 *e = gUnk_020192E0;
    u8 *st = e + 0x1B22;
    switch (*st) {
    case 0: {
        u8 *pe = e + 4;
        s16 base = (player & 1) * 0xD64;
        if (*(u16 *)(base + (int)pe) <= 0x1F3)
            return 1;
        if (sub_08008A1C(1 - player) == 0)
            return 1;
        for (; gUnk_020192E0[0x1B23] <= 4; gUnk_020192E0[0x1B23]++) {
            int side = player & 1;
            int s1 = gUnk_020192E0[0x1B23] * 0x94 + side * 0xD64;
            struct DuelZone *zn = (struct DuelZone *)(s1 + (int)(gUnk_020192E0 + 0x2C));
            u16 id = ID(zn);
            if (id != 0 && (zn->flags6 & 2) != 0 && ((const u16 *)0x08622AB4)[id & 0x7FF] == 0x228) {
                gUnk_020192E0[0x1B22]++;
                if (player != 0) {
                    gUnk_0201AE60.h14 = 1;
                    gUnk_020192E0[0x1B22]++;
                }
                return 0;
            }
        }
        return 1;
    }
    case 1:
        sub_080753F4(buf, gUnk_08085BD8, gUnk_0822C720 + (gUnk_08624244[0] << 6));
        sub_080602A4(0x206, 0x712, 0xB, buf);
        sub_08060308(1, 0, 0);
        (*st)++;
        return 0;
    case 2:
        if (gUnk_0201AE60.h14 != 0) {
            u32 msg = 0x43;
            if (player != 0)
                msg = 0x8043;
            sub_0801EC58(msg, 500, 1, 0);
            sub_08019078(player, (u8)player | e[0x1B23] << 8, (u8)(1 - player) | (u8)sub_08008A44(1 - player) << 8);
        }
        {
            u8 *g = gUnk_020192E0;
            (*(g + 0x1B23))++;
            if (*(g + 0x1B23) <= 4)
                *(g + 0x1B22) = 0;
            else
                (*(g + 0x1B22))++;
        }
        return 0;
    default:
        return 1;
    }
}
/* Two passes over the face-down monster zones of `player`: the first sets `found` when a monster of level <= 3 with byte +8 bit 0 clear exists (cleared again unless card 0x52A is present on either side; then message 0x73 is sent), the second destroys such monsters (sub_08018544) when found, and sends message 0xA6 (0x80A6) for each other face-down zone. */
extern u8 gUnk_0201930C[];
static inline int Level_E780(u16 id)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}
/* Zone addresses go through the symbol gUnk_0201930C so GCSE keeps one copy of it in sl.
 * Loop 1 computes the zone address twice, player term first: loop.c hoists the 0x94 only
 * in its second pass, so the zone pointer is strength-reduced while i still counts up
 * (a hand-stepped pointer gets a reversed counter). */
void sub_0804E780(int player)
{
    int found = 0;
    int i;
    for (i = 0; i <= 4; i++) {
        struct DuelZoneL *zn = (struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gUnk_0201930C);
        u32 id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)Level_E780(id) <= 3 && (((struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gUnk_0201930C))->b8 & 1) == 0)
                found = 1;
        }
    }
    if (sub_08008524(0, 0x52A) == 0 && sub_08008524(1, 0x52A) == 0)
        found = 0;
    if (found != 0) {
        u16 msg = 0x73;
        if (player != 0)
            msg = 0x8073;
        sub_0801EC58(msg, gUnk_08624848[0], 1, 0);
    }
    for (i = 0; i <= 4; i++) {
        struct DuelZoneL *zn = (struct DuelZoneL *)(i * 0x94 + (player & 1) * 0xD64 + (int)gUnk_0201930C);
        u16 id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)Level_E780(id) <= 3 && (((struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gUnk_0201930C))->b8 & 1) == 0 && found != 0) {
                sub_08018544(player, i, 1);
            } else {
                u32 msg = 0xA6;
                if (player != 0)
                    msg = 0x80A6;
                sub_0801EC58(msg, i, 1, 0);
            }
        }
    }
}
#if 0 /* NONMATCHING (score 190): NONMATCHING: score 190 (from 715). Rewritten in the style of the matched
       * sibling sub_0804FC4C (code_0804EFF0): typed externs gE9PS_020192E4[] (array of 0xD64 player structs, so the base
       * loads first), links read as values (u16 link = Z->links[idx]; who = (u8)link as u32 in case 10; slot = link >>
       * 8) which gives the ROM's (pz + 10) + idx*2, linked zone via a separate row pointer in case 3 (row = side*0xD64 +
       * base; lz = &row[slot]), link zone written side-first in case 3, case 4 side = other & 1 as its own statement and
       * flags loaded inside the condition (f = z->f6), signed 1-bit field for flagsC bit 4, (s32)(card << 8) < 0, local
       * table pointer t + (kind = 0x26600000 | t[0]) for the event word, player != 1 message tests. Cases 0-6 match
       * except case 4's 0x4CE and (ROM ties the result to the 0x3C register; f as u16 fixes it but adds a copy).
       * Remaining: case 10 hoists the step address (base+0x1B20) into r8 and the const 1 into sl, where the ROM hoists
       * 0x94/0xD64 and forms the in-loop step++ as (base+0x2C)+0x1AF4; case 20 and default allocation; case 21/22 tails
       * cross-jump. */
int sub_0800842C(int, u16);
int sub_080086CC(int, u16);
int sub_080088A4(int, int, int);
int sub_08008C6C(int);
int sub_0800AA40(int, int, u16);
void sub_0801FBCC(u32, int);
void sub_08022678(int, int, u16, int);
void sub_0802272C(int, int, int, int);
void sub_08046CB0(int, int, int);
void sub_08046D3C(int, u8);
int sub_0804E5B4(int);
void sub_0804E780(int);
extern const u16 gUnk_086249C8[], gUnk_08624A0C[];
struct E9Zone {
    u32 card;
    u8 unk4, unk5;
    u8 f6;
    u8 unk7;
    u16 unk8;
    u16 links[0x43];
    u8 unk90;
    u8 b91;
    u8 unk92[2];
};
struct E9Player {
    u16 life; u8 handCount; u8 pad3; u8 graveCount; u8 pad5[7];
    u8 c0 : 4; s8 c4 : 1; u8 c5 : 3; u8 padD[0x904 - 0xD]; u32 grave[(0xD64 - 0x904) / 4];
};
extern struct E9Player gE9PS_020192E4[];
struct E9State {
    u32 header; struct E9Player players[2]; u8 pad1ACC[0x1B12 - 0x1ACC]; u8 flags; u8 pad13[0x1B20 - 0x1B13];
    u8 step, zone, cursor, subcursor; u8 pad24[0x1B64 - 0x1B24]; u16 choice;
};
#define E9 ((struct E9State *)gUnk_020192E0)
#define E9_E gUnk_020192E0
#define E9_STEP (E9->step)
#define E9_ZONE (E9->zone)
#define E9_PS gE9PS_020192E4
#define E9_ID(z) (((z)->card << 20) >> 20)
#define E9_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
static inline u16 E9CardId(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= 0x7CF) return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
int sub_0804E948(void)
{
    u32 player = ((u32)gUnk_020192E0[0x1B12] << 30) >> 31;
    u32 kind;
    switch (gUnk_020192E0[0x1B20]) {
    case 0: {
        sub_0801EC58(player ? 0x8055 : 0x55, 0, 0, 0);
        E9_E[0x1B22] = 0;
        E9_E[0x1B23] = 0;
        E9_E[0x1B20]++;
    }
    case 1:
        if ((u16)sub_0804E5B4(player)) {
            u16 msg = 0x47;
            if (player) msg = 0x8047;
            sub_0801EC58(msg, 0, 0, 0);
            sub_0804E538(player);
            E9_STEP++;
        }
        return 0;
    case 2:
        sub_0804E780(player);
        E9_STEP++;
        return 0;
    case 3: {
        int i;
        for (i = 0; i <= 4; i++) {
            int other = 1 - player;
            struct E9Zone *z = (struct E9Zone *)(i * 0x94 + (other & 1) * 0xD64 + (u32)gUnk_0201930C);
            if (E9_ID(z) && (z->f6 & 2)) {
                int idx = sub_0800AA40(player, i, 0x60C);
                if (idx >= 0) {
                    u16 link = ((struct E9Zone *)((player & 1) * 0xD64 + i * 0x94 + (u32)gUnk_0201930C))->links[idx];
                    u8 who = link;
                    u32 slot = link >> 8;
                    struct E9Zone *row = (struct E9Zone *)((who & 1) * 0xD64 + (u32)gUnk_0201930C);
                    struct E9Zone *lz = &row[slot];
                    if ((lz->f6 & 0x3C) == 8 && !(lz->b91 & 8)) {
                        sub_08018AE8(who, slot, 0);
                        sub_08018544(other, i, 1);
                        sub_08046CB0(player, other, i);
                    }
                }
            }
        }
        E9_STEP++;
        E9_ZONE = 5;
        return 0;
    }
    case 4:
        for (; E9_ZONE <= 9; E9_ZONE++) {
            int other = 1 - player;
            int side = other & 1;
            struct E9Zone *z = (struct E9Zone *)(E9_ZONE * 0x94 + side * 0xD64 + E9_E + 0x2C);
            u32 id = E9_ID(z);
            u8 f;
            if (id && ((f = z->f6) & 2)) {
                switch (E9_NUMBER(id)) {
                case 0x15B:
                    if (((u32)f << 26) >> 28 <= 1) {
                        u16 msg = 0x8A;
                        if (player != 1) msg = 0x808A;
                        sub_0801EC58(msg, E9_ZONE, 1, 0);
                    } else
                        sub_08018544(other, E9_ZONE, 1);
                    E9_ZONE++;
                    return 0;
                case 0x4CE:
                    if (!(f & 0x3C)) {
                        u16 msg = 0x8A;
                        if (player != 1) msg = 0x808A;
                        sub_0801EC58(msg, E9_ZONE, 1, 0);
                    } else
                        sub_08018544(other, E9_ZONE, 1);
                    E9_ZONE++;
                    return 0;
                case 0x5F8:
                    if (sub_08008C6C(other) >= 0
                        && !(((struct E9Zone *)(side * 0xD64 + E9_ZONE * 0x94 + E9_E + 0x2C))->b91 & 8)) {
                        u16 msg = 0x8A;
                        if (player != 1) msg = 0x808A;
                        sub_0801EC58(msg, E9_ZONE, 1, 0);
                        sub_08046D3C(other, E9_ZONE);
                        E9_ZONE++;
                        return 0;
                    }
                    break;
                }
            }
        }
        E9_STEP++;
        return 0;
    case 5: {
        int other = 1 - player;
        if (sub_080086CC(other, 0x5EF) && sub_080088A4(player, 1, 0) > 0) {
            sub_08022678(other, 15, 0x5EF, 0);
            E9_STEP++;
        } else
            E9_STEP = 10;
        return 0;
    }
    case 6:
        if (E9->choice) {
            int other = 1 - player;
            u32 kind;
            int index = sub_0800842C(other, 0x5EF);
            sub_0801FBCC(((u32)(other & 1) << 31) | (((index & 31) << 16) | (kind = 0x6400000)) | E9CardId(0x5EF), 0);
        }
        E9_STEP = 10;
        return 0;
    case 10:
        E9_ZONE = 0;
        do {
            if (sub_0800A78C(player, E9_ZONE, 0x60C)) {
                u16 idx = sub_0800AA40(player, E9_ZONE, 0x60C);
                u16 link = ((struct E9Zone *)(E9_ZONE * 0x94 + (player & 1) * 0xD64 + E9_E + 0x2C))->links[idx];
                u32 who = (u8)link;
                u32 slot = link >> 8;
                struct E9Zone *lz = (struct E9Zone *)(slot * 0x94 + (who & 1) * 0xD64 + E9_E + 0x2C);
                if (!(lz->b91 & 8)) {
                    if (!(lz->f6 & 0x3C)) {
                        u16 msg = 0x8A;
                        if (who) msg = 0x808A;
                        sub_0801EC58(msg, slot, 1, 0);
                    } else {
                        sub_080197E0(player, gUnk_08624A0C[0]);
                        sub_08018AE8(who, slot, 0);
                        E9_STEP++;
                        return 0;
                    }
                }
            }
            E9_ZONE++;
        } while (E9_ZONE <= 4);
        E9_STEP = 20;
        return 0;
    case 11:
        sub_08018544(player, E9_ZONE, 1);
        E9_ZONE++;
        E9_STEP = 10;
        return 0;
    case 20: {
        int i;
        for (i = 0; i < E9_PS[player & 1].graveCount; i++) {
            u32 card = E9_PS[player & 1].grave[i];
            if ((s32)(card << 8) < 0) {
                u16 msg = 0xD2;
                if (player) msg = 0x80D2;
                sub_0801EC58(msg, card, card >> 16, 0);
            }
        }
        E9_STEP++;
        return 0;
    }
    case 21:
        if (E9_PS[player & 1].c4 < 0) {
            u16 msg = 0x4C;
            if (player) msg = 0x804C;
            sub_0801EC58(msg, 0, 0, 0);
            if (sub_080088A4(1 - player, 1, 0) > 0) {
                const u16 *t = gUnk_086249C8;
                sub_0801FBCC(((player & 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        E9_STEP++;
        return 0;
    case 22:
        if (E9_PS[player ^ 1].c4 < 0) {
            u16 msg = 0x4C;
            if (player != 1) msg = 0x804C;
            sub_0801EC58(msg, 0, 0, 0);
            if (sub_080088A4(player, 1, 0) > 0) {
                const u16 *t = gUnk_086249C8;
                sub_0801FBCC(((player ^ 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        E9_STEP++;
        return 0;
    default:
        if (sub_08008524(0, 0x593) <= 0 && sub_08008524(1, 0x593) <= 0) {
            u32 hand = E9_PS[player].handCount;
            if (hand > 6)
                sub_0802272C(player, hand - 6, 0, 0);
        }
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E948); /* 0x0804E948 size 0x6A8 */
