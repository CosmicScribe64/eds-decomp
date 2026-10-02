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
#if 0 /* NONMATCHING (score 8): BYTE-EXACT: enabling this C gives check.py 11/11, unit bytes MATCH (0x1484);
       * wf.py apply refuses only because nm st_size is 0xDA vs table 0xDC (2 bytes trailing .align padding after bx r1),
       * so flip to C once wf.py ignores padding-only size deltas. Keys: int side = i & 1 at the top of the inner body
       * (longer life makes loop pass 1 hoist it before the (u8)(1-i) chain); (u8)i written inline as the first OR
       * operand of pos (conditional, so PRE does not hoist i<<24 ahead; loop pass 2 hoists it last); int i = 0 with for
       * (; ...) and the shared found: return label are required; no asm barrier needed. Earlier failures: u8 iu = i at
       * loop top is PRE-hoisted first; iu inside the b2 block is never hoisted; for (i = 0; ...) or a literal 0x0201930C
       * base break it. */
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
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E240); /* 0x0804E240 size 0xDC */

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

#if 0 /* NONMATCHING: the ROM keeps the constant 1 in a register and ANDs it with a re-extracted bit 1 (PS[1 & who]); gcc simplifies the AND away, and the who/flag registers differ */
/* Multi-step routine keyed on the player bit 1 of the duel state byte at
 * 0x020192E0+0x1B12 and a step byte at 0x0201AF00. If that player's flag (byte
 * 9 bit 2) is set, it is cleared and the routine ends (returns 1). Otherwise
 * step 0 sends message 0x50 (0x8050), step 1 finishes with sub_080199E0(1, 1)
 * for the local side or runs sub_08024134, step 2 sets a bit at
 * 0x0201CFB0+0x808, and step 4 clears 0x0201CFB0+0x85C and finishes with
 * sub_080199E0(0, 1). Other steps advance on the A-ish key (0x100) when
 * sub_0804A1C8() is 0. */
int sub_0804E420(void)
{
    u8 *e4 = (u8 *)gUnk_020192E4;
#define WHO (((struct SB *)(e4 + 0x1B0E))->who)
    int wh = WHO;
    s8 b = *(e4 + 0x1B0E);
    u8 *st;
    int z = ((struct PS9 *)e4)[wh].f2;
    if (z != 0) {
        ((struct PS9 *)e4)[1 & wh].f2 = 0;
        return 1;
    }
    st = e4 + 0x1B1C;
    switch (*st) {
    case 0:
        sub_0801EC58((b & 2) ? 0x8050 : 0x50, 0, 0, 0);
        (*st)++;
        return 0;
    case 1:
        if ((b & 2) != 0) {
            sub_080199E0(1, 1);
            return 1;
        }
        sub_08024134(WHO, 0xD, 0);
        (*st)++;
        return 0;
    case 2:
        gUnk_0201CFB0[0x808] |= 8;
        (*st)++;
        return 0;
    case 4:
        sub_080199E0(0, 1);
        *(u32 *)(gUnk_0201CFB0 + 0x85C) = z;
        return 1;
    default:
        if (sub_0804A1C8() == 0 && (gUnk_03000040.keys & 0x100) != 0)
            gUnk_020192E0[0x1B20]++;
        return 0;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E420); /* 0x0804E420 size 0x118 */
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
#if 0 /* NONMATCHING: same structure; register allocation differs (ROM keeps e in r7, base in r6, hoists the constant 1 into r8 and 0x7FF into ip) */
/* Three-step routine on the step byte 0x020192E0+0x1B22 with a zone counter at +0x1B23. Step 0: needs the player's counter (0x020192E4 halfword) > 0x1F3 and sub_08008A1C(1-p) != 0, then scans zones counter..4 for a face-down card with key 0x228 (advancing the step, twice for player 1 which also sets 0x0201AE60+0x14); step 1 prints a card-name prompt; step 2 sends message 0x43 (0x8043) and sub_08019078 for the found zone, then advances the counter and finally the step. */
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
        st = e + 0x1B23;
        for (; *st <= 4; (*st)++) {
            u8 *zb = e + 0x2C;
            int s1 = *st * 0x94 + base;
            struct DuelZone *zn = (struct DuelZone *)(s1 + (int)zb);
            u16 id = ID(zn);
            if (id != 0 && (zn->flags6 & 2) != 0 && ((const u16 *)0x08622AB4)[id & 0x7FF] == 0x228) {
                (*(e + 0x1B22))++;
                if (player != 0) {
                    gUnk_0201AE60.h14 = 1;
                    (*(e + 0x1B22))++;
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
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E5B4); /* 0x0804E5B4 size 0x1CC */
#if 0 /* NONMATCHING: same structure; the ROM keeps zone base in sl, the
       * constant 1 in r7 and 0x7FF in r6, steps the zone pointer (add
       * r3,#0x94) while still counting i up in loop 1; this build gets either
       * a mul per iteration or a counted-down loop. */
/* Two passes over the face-down monster zones of `player`: the first sets `found` when a monster of level <= 3 with byte +8 bit 0 clear exists (cleared again unless card 0x52A is present on either side; then message 0x73 is sent), the second destroys such monsters (sub_08018544) when found, and sends message 0xA6 (0x80A6) for each other face-down zone. */
void sub_0804E780(int player)
{
    int found = 0;
    s8 i;
    struct DuelZoneL *zp = (struct DuelZoneL *)((player & 1) * 0x0201930C + 0xD64);
    struct DuelZoneL *zn = zp;
    for (i = 0; i <= 4; i++, zn++) {
        int id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            u32 lv;
            int t = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            switch (t) {
            case 0x15:
            case 0x16:
            case 0x17:
                lv = 0;
                break;
            case 0x18:
                lv = 10;
                break;
            default:
                lv = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
                break;
            }
            if (lv <= 3 && (zn->b8 & 1) == 0)
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
        struct DuelZoneL *zn = (struct DuelZoneL *)(i * 0x94 + (player & 1) * 0xD64 + 0x0201930C);
        u16 id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            u32 lv;
            int t = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            switch (t) {
            case 0x15:
            case 0x16:
            case 0x17:
                lv = 0;
                break;
            case 0x18:
                lv = 10;
                break;
            default:
                lv = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
                break;
            }
            if (lv <= 3 && (zn->b8 & 1) == 0 && found != 0) {
                sub_08018544(player, i, 1);
            } else {
                u32 msg = 0xA6;
                if (player != 0)
                    msg = 0x80A6;
                sub_0801EC58(msg, (u16)i, 1, 0);
            }
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E780); /* 0x0804E780 size 0x1C8 */
#if 0 /* NONMATCHING: 0x6C0 bytes versus the ROM's 0x6A8. The source was
       * audited against the assembly in full; the remaining differences are
       * the frame size (4 vs 8) and register lifetimes. */
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
extern const u16 gUnk_08622AB4[], gUnk_08623DF4[];
extern const u16 gUnk_086249C8[], gUnk_08624A0C[];
extern u32 gUnk_02019BE8[];
struct E948State {
    u8 pad[0x1B12]; u8 flags; u8 pad2[0x1B20-0x1B13];
    u8 step, zone, effectStep, effectZone;
    u8 pad3[0x1B64-0x1B24]; u16 choice;
};
struct E948Player { u8 pad[2]; u8 handCount, deckCount, graveCount; u8 rest[0xD64-5]; };
#define E948 ((struct E948State *)gUnk_020192E0)
#define E948_PS ((struct E948Player *)gUnk_020192E4)
#define E948_ZONE(p,z) ((struct DuelZone *)((z)*0x94+((p)&1)*0xD64+0x0201930C))
int sub_0804E948(void)
{
    u16 player = ((u32)E948->flags << 30) >> 31;
    switch (E948->step) {
    case 0:
        sub_0801EC58(player ? 0x8055 : 0x55, 0, 0, 0);
        E948->effectStep = 0;
        E948->effectZone = 0;
        E948->step++;
        /* The ROM runs the next state in the same call. */
    case 1:
        if ((u16)sub_0804E5B4(player)) {
            sub_0801EC58(player ? 0x8047 : 0x47, 0, 0, 0);
            sub_0804E538(player);
            E948->step++;
        }
        return 0;
    case 2:
        sub_0804E780(player);
        E948->step++;
        return 0;
    case 3:
        {
            int i, off;
            int opponent = 1-player;
            for (i=0, off=0; i<=4; i++, off+=0x94) {
                struct DuelZone *z=(struct DuelZone *)(off+(opponent&1)*0xD64+0x0201930C);
                if ((z->w0 << 20) && (z->flags6 & 2)) {
                    u32 index=sub_0800AA40(player,i,0x60C);
                    if (index>=0) {
                        u16 *link=(u16 *)(off+(player&1)*0xD64+0x0201930C+10+index*2);
                        s16 who=*(u8 *)link;
                        u32 slot=*link>>8;
                        struct DuelZone *linked=E948_ZONE(who,slot);
                        int count=linked->flags6 & 0x3C;
                        if (count==8 && !(count & ((u8 *)linked)[0x91])) {
                            sub_08018AE8(who,slot,0);
                            sub_08018544(opponent,i,1);
                            sub_08046CB0(player,opponent,i);
                        }
                    }
                }
            }
            E948->step++;
            E948->zone=5;
            return 0;
        }
    case 4:
        {
            int opponent=1-player;
            while (E948->zone<=9) {
                struct DuelZone *z=E948_ZONE(opponent,E948->zone);
                u8 id=ID(z);
                if (id && (z->flags6 & 2)) {
                    switch (gUnk_08622AB4[id & 0x7FF]) {
                    case 0x15B:
                        if ((((u32)z->flags6<<26)>>28)<=1)
                            sub_0801EC58(player!=1 ? 0x808A : 0x8A,E948->zone,1,0);
                        else
                            sub_08018544(opponent,E948->zone,1);
                        E948->zone++;
                        return 0;
                    case 0x4CE:
                        if (!(z->flags6 & 0x3C))
                            sub_0801EC58(player!=1 ? 0x808A : 0x8A,E948->zone,1,0);
                        else
                            sub_08018544(opponent,E948->zone,1);
                        E948->zone++;
                        return 0;
                    case 0x5F8:
                        if (sub_08008C6C(opponent)>=0 && !(((u8 *)E948_ZONE(opponent,E948->zone))[0x91] & 8)) {
                            sub_0801EC58(player!=1 ? 0x808A : 0x8A,E948->zone,1,0);
                            sub_08046D3C(opponent,E948->zone);
                            E948->zone++;
                            return 0;
                        }
                        break;
                    }
                }
                E948->zone++;
            }
            E948->step++;
            return 0;
        }
    case 5:
        {
            int opponent=1-player;
            if (sub_080086CC(opponent,0x5EF) && sub_080088A4(player,1,0)>0) {
                sub_08022678(opponent,15,0x5EF,0);
                E948->step++;
            } else E948->step=10;
            return 0;
        }
    case 6:
        if (E948->choice) {
            s16 opponent=1-player;
            int index=sub_0800842C(opponent,0x5EF);
            sub_0801FBCC(((u32)(opponent&1)<<31) | ((index&31)<<16) | 0x06400000 | gUnk_08623DF4[0x5EF],0);
        }
        E948->step=10;
        return 0;
    case 10:
        E948->zone=0;
        do {
            if (sub_0800A78C(player,E948->zone,0x60C)) {
                u16 index=sub_0800AA40(player,E948->zone,0x60C);
                u16 *link=(u16 *)((u8 *)E948_ZONE(player,E948->zone)+10+index*2);
                u8 who=*(u8 *)link;
                u32 slot=*link>>8;
                struct DuelZone *linked=E948_ZONE(who,slot);
                if (!(((u8 *)linked)[0x91] & 8)) {
                    if (!(linked->flags6 & 0x3C)) {
                        sub_0801EC58(who ? 0x808A : 0x8A,slot,1,0);
                    } else {
                        sub_080197E0(player,gUnk_08624A0C[0]);
                        sub_08018AE8(who,slot,0);
                        E948->step++;
                        return 0;
                    }
                }
            }
            E948->zone++;
        } while (E948->zone<=4);
        E948->step=20;
        return 0;
    case 11:
        sub_08018544(player,E948->zone,1);
        E948->zone++;
        E948->step=10;
        return 0;
    case 20:
        {
            int i;
            for (i=0; i<E948_PS[player].graveCount; i++) {
                u32 card=*(u32 *)((u8 *)gUnk_02019BE8+player*0xD64+i*4);
                if ((s32)(card<<8)<0)
                    sub_0801EC58(player ? 0x80D2 : 0xD2,card,card>>16,0);
            }
            E948->step++;
            return 0;
        }
    case 21:
        if (((u8 *)gUnk_020192E4)[(player&1)*0xD64+12] & 0x10) {
            sub_0801EC58(player ? 0x804C : 0x4C,0,0,0);
            if (sub_080088A4(1-player,1,0)>0)
                sub_0801FBCC(((player&1)<<31) | (0x26600000 | gUnk_086249C8[0]),0);
        }
        E948->step++;
        return 0;
    case 22:
        if (((u8 *)gUnk_020192E4)[(player^1)*0xD64+12] & 0x10) {
            sub_0801EC58(player!=1 ? 0x804C : 0x4C,0,0,0);
            if (sub_080088A4(player,1,0)>0)
                sub_0801FBCC(((player^1)<<31) | (0x26600000 | gUnk_086249C8[0]),0);
        }
        E948->step++;
        return 0;
    default:
        if (sub_08008524(0,0x593)<=0 && sub_08008524(1,0x593)<=0) {
            u32 hand=E948_PS[player].handCount;
            if (hand>6) sub_0802272C(player,hand-6,0,0);
        }
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804DB6C", sub_0804E948); /* 0x0804E948 size 0x6A8 */
