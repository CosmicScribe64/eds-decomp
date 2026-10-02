#include "global.h"

int sub_0802CFA0(int player, u16 id, u16 x);
void sub_08019894(int player, u16 dmg, u16 a, u16 b);
void sub_080197E0(int player, u16 id);
void sub_080199E0(int player, int n);
void sub_08022784(int player, int a, int b);
int sub_0800A430(int player, int zone);
void sub_08017AB4(int player, u16 a, u16 b, u16 c);
void sub_08018ED8(int player, int zone, int a, int b);
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(u16 a, u16 b, int c, const char *text);
void sub_08060308(int a, int b, int c);
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);
void sub_080197C0(int player, u16 id);
int sub_0801A130(int player, int a);
u32 sub_0800A2A8(int player, int zone);
extern u8 gUnk_020192E0[];
extern const char gUnk_08085B7C[];
extern const u16 gUnk_08623E66[];
extern const char gUnk_0822C720[];
extern const u16 gUnk_08624730[];
extern u8 gUnk_02017FB0[];
struct Unk0201AE60 { u8 pad[0x14]; u16 v14; };
extern struct Unk0201AE60 gUnk_0201AE60;
struct Unk02015EE8 { u8 b0; u8 link : 1; };
extern struct Unk02015EE8 gUnk_02015EE8;
/* Duel progress word at 0x020192E0+0x1B16: bits 1-8 = step. */
struct StepW { u16 lo : 1; u16 step : 8; u16 hi : 7; u8 pad[6]; };
#define STEP (((struct StepW *)(gUnk_020192E0 + 0x1B16))->step)
#define STEP0 (((struct StepW *)(e + 0x1B16))->step)
/* Per-player flag byte at 0x020192E4+8 (stride 0xD64). */
struct PlF { u8 pad[8]; u8 f0 : 1; s8 f1 : 1; u8 rest : 6; u8 pad2[0xD64 - 9]; };
extern struct PlF gUnk_020192E4[];
struct ZoneF6 { u8 pad[6]; u8 b6; u8 pad7[0x94 - 7]; };

void sub_0801FBCC(u32 event, int arg);

/* Battle state at 0x02018450 (see code_0801CE68). */
struct BSide { u8 raw; u8 f1; u16 cardId; u8 pad4[6]; u16 damage; };
struct BState {
    u16 lo : 2;
    u16 bit2 : 1;       /* +0 bit 2 */
    u16 mid : 3;
    u16 atkSlot : 3;
    u16 defSlot : 3;    /* +0 bits 9-11 */
    u16 hi : 4;
    u16 cardId;         /* +2 */
    u8 pad4[4];
    struct BSide side[2];   /* +8, 12 bytes each */
};
extern struct BState gUnk_02018450;
/* View of a side at (byte) offset i*12 from the start of the battle state (fields at +8 ...). */
struct BSideV { u8 pad[8]; u8 raw; u8 f1; u16 cardId; u8 pad4[6]; u16 damage; };
#define BSV(i) ((struct BSideV *)((u8 *)&gUnk_02018450 + (i) * 12))

/* Battle resolution step machine (hypothesis: applies per-card effects after damage; steps 0-2 damage,
 * 10-12 a card-name prompt / link handshake, default clears flags and sends per-side messages). */
#define CB_BS gUnk_02018450
#define CB_SIDE(i) gUnk_02018450.side[i]
#define CB_KEY(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CB_PF(i) (gUnk_020192E4[(i) & 1])
/* Duel state views. The step bitfield is read through a struct pointer so the base symbol is loaded into a
 * pseudo and 0x1B16 is added separately (ROM: ldr sym; ldr 0x1B16; add), not as one sym+0x1B16 literal. */
struct CbDuel { u32 unk0; struct PlF pl[2]; u8 pad[0x1B16 - 4 - 2 * 0xD64]; u16 lo : 1; u16 step : 8; u16 hi : 7; };
struct CbDuel4 { u8 pad[0x1B12]; u16 lo : 1; u16 step : 8; u16 hi : 7; };
#define CB_DUEL ((struct CbDuel *)gUnk_020192E0)
#define CB_STEP (CB_DUEL->step)
struct CbLink { u8 pad[0x450]; u8 b0 : 1; s8 b1 : 1; u8 rest : 6; };
#define CB_LINK ((struct CbLink *)gUnk_02017FB0)
struct CbZone { u8 pad[6]; u8 b6; };
int sub_0804CB58(int p)
{
    char buf[256];
    switch (CB_STEP) {
    case 0:
        if (CB_SIDE(p).damage != 0 && !CB_PF(p).f0 && !CB_PF(p).f1) {
            sub_08019894(p, CB_SIDE(p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            switch (CB_KEY(CB_SIDE(1 - p).cardId)) {
            case 0x71:
                sub_080197E0(1 - p, CB_SIDE(1 - p).cardId);
                sub_08022784(p, 1, 1);
                break;
            case 0xDB:
                sub_080197E0(1 - p, CB_SIDE(1 - p).cardId);
                sub_080199E0(1 - p, 1);
                break;
            case 0x20A:
                sub_080197E0(1 - p, CB_SIDE(1 - p).cardId);
                sub_080199E0(p, 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = ((1 - p) & 1) << 31;
                    u32 b = CB_BS.defSlot << 16;
                    b |= 0x1C400000;
                    ev |= b;
                    ev |= CB_SIDE(1 - p).cardId;
                    sub_0801FBCC(ev, CB_SIDE(p).damage | ((((u8)p & 15) | CB_BS.atkSlot << 4) | (((u8)(1 - p) & 15) | CB_BS.defSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                int k = CB_KEY(CB_SIDE(p).cardId);
                if ((k == 0x2DA || k == 0x536)
                    && sub_0800A430(p, CB_BS.atkSlot) != 0xFFFF && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1)
                    sub_08019894(1 - p, CB_SIDE(p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            }
        }
        CB_STEP++;
        goto ret0;
    case 1:
        if (CB_SIDE(1 - p).damage != 0 && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1
            && sub_0800A2A8(1 - p, 0x39) != 0) {
            /* Same field as CB_STEP, addressed from the player-flag base so CSE reuses that register
             * (ROM: r4 + 0x1B12). */
            ((struct CbDuel4 *)gUnk_020192E4)->step = 10;
            goto ret0;
        }
        CB_STEP++;
        goto ret0;
    case 2:
        if (CB_SIDE(1 - p).damage != 0 && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1) {
            sub_08019894(1 - p, CB_SIDE(1 - p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            switch (CB_KEY(CB_SIDE(p).cardId)) {
            case 0x71:
                sub_080197E0(p, CB_SIDE(p).cardId);
                sub_08022784(1 - p, 1, 1);
                break;
            case 0xDB:
                sub_080197E0(p, CB_SIDE(p).cardId);
                sub_080199E0(p, 1);
                break;
            case 0x20A:
                sub_080197E0(p, CB_SIDE(p).cardId);
                sub_080199E0(1 - p, 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = (p & 1) << 31;
                    u32 b = CB_BS.atkSlot << 16;
                    b |= 0x1A400000;
                    ev |= b;
                    ev |= CB_SIDE(p).cardId;
                    sub_0801FBCC(ev, CB_SIDE(1 - p).damage | ((((u8)(1 - p) & 15) | CB_BS.defSlot << 4) | (((u8)p & 15) | CB_BS.atkSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                int k = CB_KEY(CB_SIDE(1 - p).cardId);
                if ((k == 0x2DA || k == 0x536)
                    && sub_0800A430(1 - p, CB_BS.defSlot) != 0xFFFF && !CB_PF(p).f0 && !CB_PF(p).f1)
                    sub_08019894(p, CB_SIDE(1 - p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            }
        }
        CB_STEP++;
        goto ret0;
    case 10:
        if (p != 0) {
            /* Cast-constant name table: reloaded into the next rotation register (r5), as in the ROM. */
            sub_080753F4(buf, gUnk_08085B7C, (const char *)0x0822C720 + (gUnk_08623E66[0] << 6));
            sub_080602A4(0x204, 0x915, 0xB, buf);
            sub_08060308(1, 0, 0);
            CB_STEP++;
        } else if (!gUnk_02015EE8.link) {
            gUnk_0201AE60.v14 = 1;
            CB_STEP++;
        } else {
            sub_0802297C(0xF057, gUnk_08623E66[0], 0, 0);
            CB_LINK->b1 = 0;
        }
        CB_STEP++;
    /* FAKEMATCH: every `return 0` jumps to this one label after case 10's STEP++, which places the shared
     * return-0 block between case 10 and case 11 as in the ROM. */
    ret0:
        return 0;
    case 11:
        if ((int)(gUnk_02017FB0[0x450] << 30) >= 0)
            goto ret0;
        gUnk_0201AE60.v14 = *(u16 *)(gUnk_02017FB0 + 0x45A);
        CB_STEP++;
        goto ret0;
    case 12:
        if (gUnk_0201AE60.v14 != 0) {
            sub_080197C0(1 - p, gUnk_08623E66[0]);
            if (sub_0801A130(1 - p, 0x39) != 0)
                CB_SIDE(1 - p).damage = 0;
        }
        CB_STEP = 2;
        goto ret0;
    default:
        CB_DUEL->pl[0].f0 = 0;
        CB_DUEL->pl[1].f0 = 0;
        if (CB_KEY(CB_SIDE(1 - p).cardId) == 0x4B1 && (int)(CB_SIDE(1 - p).raw << 28) >= 0) {
            int side = (1 - p) & 1;
            int s1 = CB_BS.defSlot * 0x94 + side * 0xD64;
            if (((struct CbZone *)(s1 + (int)(gUnk_020192E0 + 0x2C)))->b6 & 1) {
                sub_080197E0(1 - p, CB_SIDE(1 - p).cardId);
                sub_08018ED8(1 - p, CB_BS.defSlot, 0, 0);
            }
        }
        {
            int i;
            struct BState *bs;
            const u16 *k;
            /* bs/k are loop-hoisted bases after i = 0 (ROM order); the side byte is read through BSV() so its
             * base is not merged with bs. */
            for (i = 0, bs = &gUnk_02018450, k = gUnk_08624730; i < 2; i++) {
                u8 f = BSV(i)->raw;
                if ((int)(f << 25) < 0 && (int)(f << 28) >= 0)
                    sub_08017AB4(p, k[0], (u8)i | (i == p ? bs->atkSlot : bs->defSlot) << 8, 3);
            }
        }
        return 1;
    }
}

/* Battle hook: when bit 2 is set and the card at +2 is usable for (1-player), queue event 0x91 with the defender slot. Always returns 1. */
int sub_0804D298(int player)
{
    int p;
    u32 r;
    if (gUnk_02018450.bit2) {
        p = 1 - player;
        r = (u16)sub_0802CFA0(p, gUnk_02018450.cardId, 0);
        if (((const u16 *)0x08622AB4)[gUnk_02018450.cardId & 0x7FF] == 0x2FA) {
            if ((int)(gUnk_02018450.side[p].raw << 28) < 0)
                r = 0;
        }
        if (r != 0) {
            u32 ev = ((1 - player) & 1) << 31;
            u32 b = gUnk_02018450.defSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= gUnk_02018450.cardId;
            sub_0801FBCC(ev, 0);
        }
    }
    return 1;
}

u16 sub_0804A998(int player);
void sub_08042AB0(int player, int kind, u32 arg);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
int sub_0800C8BC(int player, int zone);
void sub_08017F98(int player, int zone);
void sub_08018664(int player, int zone, u16 *args);
void sub_08018690(int arg0, int player, int zone, u16 *args);
/* Battle zone copy (0x94 bytes): +0/+2 the card word as two u16 args, +3 bit 0, bits 1-3 (a slot), bit 4. */
struct D3Zone { u16 w0; u8 b2; u8 f3_0 : 1; u8 slot : 3; u8 f3_4 : 1; u8 f3_5 : 3; u8 pad[0x94 - 4]; };
/* Battle state at 0x02018450 with the zone copies at +0x20 and the two pending (player | slot << 8) words. */
struct D3Battle {
    u16 lo : 1;
    u16 direct : 1;     /* +0 bit 1 */
    u16 mid : 4;
    u16 atkSlot : 3;
    u16 defSlot : 3;
    u16 hi : 4;
    u16 cardId;
    u8 pad4[4];
    struct BSide side[2];       /* +0x8 */
    struct D3Zone zones[2];     /* +0x20 */
    u8 pad148[8];
    u16 pend0;                  /* +0x150: attacker side (p | atkSlot << 8), 0xFFFF = none */
    u16 pend1;                  /* +0x152: defender side ((1 - p) | defSlot << 8) */
};
#define D3 (*(struct D3Battle *)&gUnk_02018450)
/* Player flag byte +8 (stride 0xD64). Indexed through a cast pointer to a struct holding the array, so the base
 * 0x020192E4 is loaded before the index is computed (ROM order); a plain array index loads it last. */
struct D3PF { u8 pad[8]; u8 f0 : 1; u8 f1 : 1; u8 f2 : 1; u8 f3 : 1; u8 rest : 4; u8 pad2[0xD64 - 9]; };
struct D3Duel { struct D3PF pl[2]; };
#define D3PF(i) (((struct D3Duel *)gUnk_020192E4)->pl[(i) & 1])
/* Field zone at 0x0201930C + off; the base is gUnk_020192E0 + 0x2C so CSE reuses the 0x020192E0 register from
 * the step test where one is live (ROM: adds r1, r7, #0; adds r1, #0x2C). */
struct D3DZone { u32 w0; u16 w4; u8 b6; };
#define D3ZONE(off) ((struct D3DZone *)((off) + (int)(gUnk_020192E0 + 0x2C)))
/* Battle step after damage: per-card effects for the attacker's card (switch 1), the attacker side's destroyed
 * handling (block 1), the defender's card (switch 2) and the defender side's destroyed handling (block 2); then
 * advances the step. Returns 1 without work for a direct attack, or when a step is already set (re-sending the
 * pending words through sub_08042AB0). */
int sub_0804D320(int p)
{
    int k;
    int done;
    /* One function-scope pointer for both zone copies: its refs/live length put it ahead of 1 - p in global
     * allocation (ROM: z in r5, 1 - p in r6 in block 2); a block-local z loses r5 to 1 - p. */
    u16 *z;

    if (D3.direct)
        return 1;
    if (CB_STEP) {
        if (*(s32 *)&D3.pend0 != -1)
            sub_08042AB0(1 - p, 0x13, D3.pend1 << 16 | D3.pend0);
        return 1;
    }
    switch (CB_KEY(D3.side[p].cardId)) {
    case 0xFF:
        {
            u32 ev = p << 31;
            u32 b = D3.atkSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= D3.side[p].cardId;
            sub_0801FBCC(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x188:
    case 0x18E:
        if (!sub_0804A998(p)) {
            sub_080197E0(p, D3.side[p].cardId);
            sub_0801EC58(p != 1 ? 0x8095 : 0x95, D3.defSlot, 5, 0);
        }
        break;
    case 0x199:
        if (!sub_0804A998(p))
            sub_080197E0(p, D3.side[p].cardId);
        break;
    case 0x5F3:
        if ((int)(D3.side[1 - p].raw << 28) < 0) {
            int side = p & 1;
            int s1 = D3.atkSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->w0 << 20 != 0) {
                sub_080197E0(p, D3.side[p].cardId);
                sub_08017AB4(p, (u8)p | D3.atkSlot << 8, (u8)p | D3.atkSlot << 8, 0x10C);
            }
        }
        break;
    }
    D3.pend0 = 0xFFFF;
    if ((int)(D3.side[p].raw << 28) < 0) {
        done = 0;
        if (CB_KEY(D3.side[1 - p].cardId) == 0x2F9)
            D3PF(1 - p).f3 = 1;
        k = CB_KEY(D3.side[p].cardId);
        if ((k == 0x2DA || k == 0x536) && sub_0800A430(p, D3.atkSlot) != 0xFFFF) {
            z = (u16 *)&D3.zones[p];
            sub_08017F98(p, D3.atkSlot);
            sub_0801EC58(p ? 0x80A4 : 0xA4, D3.atkSlot, z[0], z[1]);
            done = 1;
        }
        if (CB_KEY(D3.side[1 - p].cardId) != 0xFF && CB_KEY(D3.side[p].cardId) != 0xFF) {
            if (!done) {
                /* A switch, not `k >= 0x4E6 && k <= 0x4E8`: the ROM reloads the key and tests both bounds. */
                switch (CB_KEY(D3.side[p].cardId)) {
                case 0x4E6:
                case 0x4E7:
                case 0x4E8:
                    sub_080197C0(p, D3.side[p].cardId);
                    sub_08017AB4(p, D3.side[p].cardId, (u8)(1 - p) | D3.defSlot << 8, 9);
                    break;
                }
                switch (CB_KEY(D3.side[1 - p].cardId)) {
                case 0x52F:
                    D3.zones[p].f3_0 = 1;
                    D3.zones[p].slot = D3.defSlot;
                    break;
                case 0x53C:
                    D3.zones[p].f3_4 = 1;
                    break;
                }
                sub_08018690(1 - p, p, D3.atkSlot, (u16 *)&D3.zones[p]);
                D3.pend0 = (u8)p | D3.atkSlot << 8;
            }
        } else if (!done)
            sub_08018664(p, D3.atkSlot, (u16 *)&D3.zones[p]);
    }
    switch (CB_KEY(D3.side[1 - p].cardId)) {
    case 0xFF:
        {
            u32 ev = ((1 - p) & 1) << 31;
            u32 b = D3.atkSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= D3.side[1 - p].cardId;
            sub_0801FBCC(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x419:
        if (!sub_0804A998(p) && (int)(D3.side[p].raw << 28) >= 0) {
            u32 ev = ((1 - p) & 1) << 31;
            u32 b = D3.defSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= D3.side[1 - p].cardId;
            sub_0801FBCC(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x189:
        if (!sub_0804A998(p) && sub_0800C8BC(p, D3.atkSlot) != 7) {
            sub_080197E0(1 - p, D3.side[1 - p].cardId);
            sub_0801EC58(p ? 0x8095 : 0x95, D3.atkSlot, 3, 0);
        }
        break;
    case 0x261:
        if (!sub_0804A998(p) && sub_0800C8BC(p, D3.atkSlot) != 2) {
            sub_080197E0(1 - p, D3.side[1 - p].cardId);
            sub_0801EC58(p ? 0x8097 : 0x97, D3.atkSlot, 1, 0);
        }
        break;
    case 0x4B1:
        if ((int)(D3.side[1 - p].raw << 28) >= 0) {
            int side = (1 - p) & 1;
            int s1 = D3.defSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->b6 & 1) {
                sub_080197E0(1 - p, D3.side[1 - p].cardId);
                sub_0801EC58(p != 1 ? 0x807E : 0x7E, D3.defSlot, 0, 0);
            }
        }
        break;
    case 0x5F3:
        if ((int)(D3.side[p].raw << 28) < 0) {
            int side = (1 - p) & 1;
            int s1 = D3.defSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->w0 << 20 != 0) {
                sub_080197E0(1 - p, D3.side[1 - p].cardId);
                sub_08017AB4(1 - p, (u8)(1 - p) | D3.defSlot << 8, (u8)(1 - p) | D3.defSlot << 8, 0x10C);
            }
        }
        break;
    }
    D3.pend1 = 0xFFFF;
    if ((int)(D3.side[1 - p].raw << 28) < 0) {
        done = 0;
        if (CB_KEY(D3.side[p].cardId) == 0x2F9)
            D3PF(p).f3 = 1;
        k = CB_KEY(D3.side[1 - p].cardId);
        if ((k == 0x2DA || k == 0x536) && sub_0800A430(1 - p, D3.defSlot) != 0xFFFF) {
            z = (u16 *)&D3.zones[1 - p];
            sub_08017F98(1 - p, D3.defSlot);
            sub_0801EC58(p != 1 ? 0x80A4 : 0xA4, D3.defSlot, z[0], z[1]);
            done = 1;
        }
        if (CB_KEY(D3.side[1 - p].cardId) != 0xFF && CB_KEY(D3.side[p].cardId) != 0xFF) {
            if (!done) {
                switch (CB_KEY(D3.side[1 - p].cardId)) {
                case 0x4E6:
                case 0x4E7:
                case 0x4E8:
                    /* (sic) player p, as in the ROM */
                    sub_080197C0(p, D3.side[1 - p].cardId);
                    sub_08017AB4(1 - p, D3.side[1 - p].cardId, (u8)p | D3.atkSlot << 8, 9);
                    break;
                }
                switch (CB_KEY(D3.side[p].cardId)) {
                case 0x52F:
                    D3.zones[1 - p].f3_0 = 1;
                    D3.zones[1 - p].slot = D3.atkSlot;
                    break;
                case 0x53C:
                    D3.zones[1 - p].f3_4 = 1;
                    break;
                }
                sub_08018690(1 - p, 1 - p, D3.defSlot, (u16 *)&D3.zones[1 - p]);
                D3.pend1 = (u8)(1 - p) | D3.defSlot << 8;
            }
        } else if (!done)
            sub_08018664(1 - p, D3.defSlot, (u16 *)&D3.zones[1 - p]);
    }
    CB_STEP++;
    return 0;
}
