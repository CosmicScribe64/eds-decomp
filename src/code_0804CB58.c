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

INCLUDE_ASM("asm/nonmatching/code_0804CB58", sub_0804D320); /* 0x0804D320 size 0x84C */
