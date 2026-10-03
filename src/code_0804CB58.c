#include "global.h"

int CanActivateEffectOfCard(int player, u16 id, u16 x);
void InflictBattleDamage(int player, u16 dmg, u16 a, u16 b);
void ShowCardEffect(int player, u16 id);
void DrawCards(int player, int n);
void DuelPrompt_PostRandomDiscard(int player, int a, int b);
int FindAbsorbedMonsterLink(int player, int zone);
void QueueAddZoneLink(int player, u16 a, u16 b, u16 c);
void ChangeBattlePosition(int player, int zone, int a, int b);
void FormatStr(char *dst, const char *fmt, const char *arg);
void TextBoxOpen(u16 a, u16 b, int c, const char *text);
void TextBoxSetMenu(int a, int b, int c);
u16 DuelLink_SendMessage(u16 a, u16 b, u16 c, u16 d);
void sub_080197C0(int player, u16 id);
int DiscardHandCardByNumber(int player, int a);
u32 CountHandCardsByNumber(int player, int zone);
extern u8 gDuel[];
extern const char gStrAskKuribohFmt[];
extern const u16 gUnk_08623E66[];
extern const char gCardNames[];
extern const u16 gUnk_08624730[];
extern u8 gLinkState[];
struct Unk0201AE60 { u8 pad[0x14]; u16 v14; };
extern struct Unk0201AE60 gTextBox;
struct Unk02015EE8 { u8 b0; u8 link : 1; };
extern struct Unk02015EE8 gDuelCtrl;
/* Duel progress word at 0x020192E0+0x1B16: bits 1-8 = step. */
struct StepW { u16 lo : 1; u16 step : 8; u16 hi : 7; u8 pad[6]; };
#define STEP (((struct StepW *)(gDuel + 0x1B16))->step)
#define STEP0 (((struct StepW *)(e + 0x1B16))->step)
/* Per-player flag byte at 0x020192E4+8 (stride 0xD64). */
struct PlF { u8 pad[8]; u8 f0 : 1; s8 f1 : 1; u8 rest : 6; u8 pad2[0xD64 - 9]; };
extern struct PlF gDuelPlayers[];
struct ZoneF6 { u8 pad[6]; u8 b6; u8 pad7[0x94 - 7]; };

void Chain_AddPending(u32 event, int arg);

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
extern struct BState gBattle;
/* View of a side at (byte) offset i*12 from the start of the battle state (fields at +8 ...). */
struct BSideV { u8 pad[8]; u8 raw; u8 f1; u16 cardId; u8 pad4[6]; u16 damage; };
#define BSV(i) ((struct BSideV *)((u8 *)&gBattle + (i) * 12))

/* Battle resolution step machine (hypothesis: applies per-card effects after damage; steps 0-2 damage,
 * 10-12 a card-name prompt / link handshake, default clears flags and sends per-side messages). */
#define CB_BS gBattle
#define CB_SIDE(i) gBattle.side[i]
#define CB_KEY(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CB_PF(i) (gDuelPlayers[(i) & 1])
/* Duel state views. The step bitfield is read through a struct pointer so the base symbol is loaded into a
 * pseudo and 0x1B16 is added separately (ROM: ldr sym; ldr 0x1B16; add), not as one sym+0x1B16 literal. */
struct CbDuel { u32 unk0; struct PlF pl[2]; u8 pad[0x1B16 - 4 - 2 * 0xD64]; u16 lo : 1; u16 step : 8; u16 hi : 7; };
struct CbDuel4 { u8 pad[0x1B12]; u16 lo : 1; u16 step : 8; u16 hi : 7; };
#define CB_DUEL ((struct CbDuel *)gDuel)
#define CB_STEP (CB_DUEL->step)
struct CbLink { u8 pad[0x450]; u8 b0 : 1; s8 b1 : 1; u8 rest : 6; };
#define CB_LINK ((struct CbLink *)gLinkState)
struct CbZone { u8 pad[6]; u8 b6; };
int BattleStage_InflictDamage(int p)
{
    char buf[256];
    switch (CB_STEP) {
    case 0:
        if (CB_SIDE(p).damage != 0 && !CB_PF(p).f0 && !CB_PF(p).f1) {
            InflictBattleDamage(p, CB_SIDE(p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            switch (CB_KEY(CB_SIDE(1 - p).cardId)) {
            case 0x71:
                ShowCardEffect(1 - p, CB_SIDE(1 - p).cardId);
                DuelPrompt_PostRandomDiscard(p, 1, 1);
                break;
            case 0xDB:
                ShowCardEffect(1 - p, CB_SIDE(1 - p).cardId);
                DrawCards(1 - p, 1);
                break;
            case 0x20A:
                ShowCardEffect(1 - p, CB_SIDE(1 - p).cardId);
                DrawCards(p, 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = ((1 - p) & 1) << 31;
                    u32 b = CB_BS.defSlot << 16;
                    b |= 0x1C400000;
                    ev |= b;
                    ev |= CB_SIDE(1 - p).cardId;
                    Chain_AddPending(ev, CB_SIDE(p).damage | ((((u8)p & 15) | CB_BS.atkSlot << 4) | (((u8)(1 - p) & 15) | CB_BS.defSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                int k = CB_KEY(CB_SIDE(p).cardId);
                if ((k == 0x2DA || k == 0x536)
                    && FindAbsorbedMonsterLink(p, CB_BS.atkSlot) != 0xFFFF && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1)
                    InflictBattleDamage(1 - p, CB_SIDE(p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            }
        }
        CB_STEP++;
        goto ret0;
    case 1:
        if (CB_SIDE(1 - p).damage != 0 && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1
            && CountHandCardsByNumber(1 - p, 0x39) != 0) {
            /* Same field as CB_STEP, addressed from the player-flag base so CSE reuses that register
             * (ROM: r4 + 0x1B12). */
            ((struct CbDuel4 *)gDuelPlayers)->step = 10;
            goto ret0;
        }
        CB_STEP++;
        goto ret0;
    case 2:
        if (CB_SIDE(1 - p).damage != 0 && !CB_PF(1 - p).f0 && !CB_PF(1 - p).f1) {
            InflictBattleDamage(1 - p, CB_SIDE(1 - p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            switch (CB_KEY(CB_SIDE(p).cardId)) {
            case 0x71:
                ShowCardEffect(p, CB_SIDE(p).cardId);
                DuelPrompt_PostRandomDiscard(1 - p, 1, 1);
                break;
            case 0xDB:
                ShowCardEffect(p, CB_SIDE(p).cardId);
                DrawCards(p, 1);
                break;
            case 0x20A:
                ShowCardEffect(p, CB_SIDE(p).cardId);
                DrawCards(1 - p, 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = (p & 1) << 31;
                    u32 b = CB_BS.atkSlot << 16;
                    b |= 0x1A400000;
                    ev |= b;
                    ev |= CB_SIDE(p).cardId;
                    Chain_AddPending(ev, CB_SIDE(1 - p).damage | ((((u8)(1 - p) & 15) | CB_BS.defSlot << 4) | (((u8)p & 15) | CB_BS.atkSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                int k = CB_KEY(CB_SIDE(1 - p).cardId);
                if ((k == 0x2DA || k == 0x536)
                    && FindAbsorbedMonsterLink(1 - p, CB_BS.defSlot) != 0xFFFF && !CB_PF(p).f0 && !CB_PF(p).f1)
                    InflictBattleDamage(p, CB_SIDE(1 - p).damage, (u8)p | CB_BS.atkSlot << 8, (u8)(1 - p) | CB_BS.defSlot << 8);
            }
        }
        CB_STEP++;
        goto ret0;
    case 10:
        if (p != 0) {
            /* Cast-constant name table: reloaded into the next rotation register (r5), as in the ROM. */
            FormatStr(buf, gStrAskKuribohFmt, (const char *)0x0822C720 + (gUnk_08623E66[0] << 6));
            TextBoxOpen(0x204, 0x915, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            CB_STEP++;
        } else if (!gDuelCtrl.link) {
            gTextBox.v14 = 1;
            CB_STEP++;
        } else {
            DuelLink_SendMessage(0xF057, gUnk_08623E66[0], 0, 0);
            CB_LINK->b1 = 0;
        }
        CB_STEP++;
    /* FAKEMATCH: every `return 0` jumps to this one label after case 10's STEP++, which places the shared
     * return-0 block between case 10 and case 11 as in the ROM. */
    ret0:
        return 0;
    case 11:
        if ((int)(gLinkState[0x450] << 30) >= 0)
            goto ret0;
        gTextBox.v14 = *(u16 *)(gLinkState + 0x45A);
        CB_STEP++;
        goto ret0;
    case 12:
        if (gTextBox.v14 != 0) {
            sub_080197C0(1 - p, gUnk_08623E66[0]);
            if (DiscardHandCardByNumber(1 - p, 0x39) != 0)
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
            if (((struct CbZone *)(s1 + (int)(gDuel + 0x2C)))->b6 & 1) {
                ShowCardEffect(1 - p, CB_SIDE(1 - p).cardId);
                ChangeBattlePosition(1 - p, CB_BS.defSlot, 0, 0);
            }
        }
        {
            int i;
            struct BState *bs;
            const u16 *k;
            /* bs/k are loop-hoisted bases after i = 0 (ROM order); the side byte is read through BSV() so its
             * base is not merged with bs. */
            for (i = 0, bs = &gBattle, k = gUnk_08624730; i < 2; i++) {
                u8 f = BSV(i)->raw;
                if ((int)(f << 25) < 0 && (int)(f << 28) >= 0)
                    QueueAddZoneLink(p, k[0], (u8)i | (i == p ? bs->atkSlot : bs->defSlot) << 8, 3);
            }
        }
        return 1;
    }
}

/* Battle hook: when bit 2 is set and the card at +2 is usable for (1-player), queue event 0x91 with the defender slot. Always returns 1. */
int BattleStage_TriggerFlipEffect(int player)
{
    int p;
    u32 r;
    if (gBattle.bit2) {
        p = 1 - player;
        r = (u16)CanActivateEffectOfCard(p, gBattle.cardId, 0);
        if (((const u16 *)0x08622AB4)[gBattle.cardId & 0x7FF] == 0x2FA) {
            if ((int)(gBattle.side[p].raw << 28) < 0)
                r = 0;
        }
        if (r != 0) {
            u32 ev = ((1 - player) & 1) << 31;
            u32 b = gBattle.defSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= gBattle.cardId;
            Chain_AddPending(ev, 0);
        }
    }
    return 1;
}

u16 IsBattleEffectBlocked(int player);
void EventResponse_Request(int player, int kind, u32 arg);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
int GetZoneCardType(int player, int zone);
void DestroyAbsorbedMonsters(int player, int zone);
void BanishBattleDestroyedCard(int player, int zone, u16 *args);
void SendBattleDestroyedCardToGraveyard(int arg0, int player, int zone, u16 *args);
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
#define D3 (*(struct D3Battle *)&gBattle)
/* Player flag byte +8 (stride 0xD64). Indexed through a cast pointer to a struct holding the array, so the base
 * 0x020192E4 is loaded before the index is computed (ROM order); a plain array index loads it last. */
struct D3PF { u8 pad[8]; u8 f0 : 1; u8 f1 : 1; u8 f2 : 1; u8 f3 : 1; u8 rest : 4; u8 pad2[0xD64 - 9]; };
struct D3Duel { struct D3PF pl[2]; };
#define D3PF(i) (((struct D3Duel *)gDuelPlayers)->pl[(i) & 1])
/* Field zone at 0x0201930C + off; the base is gDuel + 0x2C so CSE reuses the 0x020192E0 register from
 * the step test where one is live (ROM: adds r1, r7, #0; adds r1, #0x2C). */
struct D3DZone { u32 w0; u16 w4; u8 b6; };
#define D3ZONE(off) ((struct D3DZone *)((off) + (int)(gDuel + 0x2C)))
/* Battle step after damage: per-card effects for the attacker's card (switch 1), the attacker side's destroyed
 * handling (block 1), the defender's card (switch 2) and the defender side's destroyed handling (block 2); then
 * advances the step. Returns 1 without work for a direct attack, or when a step is already set (re-sending the
 * pending words through EventResponse_Request). */
int BattleStage_DestroyMonsters(int p)
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
            EventResponse_Request(1 - p, 0x13, D3.pend1 << 16 | D3.pend0);
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
            Chain_AddPending(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x188:
    case 0x18E:
        if (!IsBattleEffectBlocked(p)) {
            ShowCardEffect(p, D3.side[p].cardId);
            DuelCmd_Push(p != 1 ? 0x8095 : 0x95, D3.defSlot, 5, 0);
        }
        break;
    case 0x199:
        if (!IsBattleEffectBlocked(p))
            ShowCardEffect(p, D3.side[p].cardId);
        break;
    case 0x5F3:
        if ((int)(D3.side[1 - p].raw << 28) < 0) {
            int side = p & 1;
            int s1 = D3.atkSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->w0 << 20 != 0) {
                ShowCardEffect(p, D3.side[p].cardId);
                QueueAddZoneLink(p, (u8)p | D3.atkSlot << 8, (u8)p | D3.atkSlot << 8, 0x10C);
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
        if ((k == 0x2DA || k == 0x536) && FindAbsorbedMonsterLink(p, D3.atkSlot) != 0xFFFF) {
            z = (u16 *)&D3.zones[p];
            DestroyAbsorbedMonsters(p, D3.atkSlot);
            DuelCmd_Push(p ? 0x80A4 : 0xA4, D3.atkSlot, z[0], z[1]);
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
                    QueueAddZoneLink(p, D3.side[p].cardId, (u8)(1 - p) | D3.defSlot << 8, 9);
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
                SendBattleDestroyedCardToGraveyard(1 - p, p, D3.atkSlot, (u16 *)&D3.zones[p]);
                D3.pend0 = (u8)p | D3.atkSlot << 8;
            }
        } else if (!done)
            BanishBattleDestroyedCard(p, D3.atkSlot, (u16 *)&D3.zones[p]);
    }
    switch (CB_KEY(D3.side[1 - p].cardId)) {
    case 0xFF:
        {
            u32 ev = ((1 - p) & 1) << 31;
            u32 b = D3.atkSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= D3.side[1 - p].cardId;
            Chain_AddPending(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x419:
        if (!IsBattleEffectBlocked(p) && (int)(D3.side[p].raw << 28) >= 0) {
            u32 ev = ((1 - p) & 1) << 31;
            u32 b = D3.defSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= D3.side[1 - p].cardId;
            Chain_AddPending(ev, ((u8)p | D3.atkSlot << 8) | ((u8)(1 - p) | D3.defSlot << 8) << 16);
        }
        break;
    case 0x189:
        if (!IsBattleEffectBlocked(p) && GetZoneCardType(p, D3.atkSlot) != 7) {
            ShowCardEffect(1 - p, D3.side[1 - p].cardId);
            DuelCmd_Push(p ? 0x8095 : 0x95, D3.atkSlot, 3, 0);
        }
        break;
    case 0x261:
        if (!IsBattleEffectBlocked(p) && GetZoneCardType(p, D3.atkSlot) != 2) {
            ShowCardEffect(1 - p, D3.side[1 - p].cardId);
            DuelCmd_Push(p ? 0x8097 : 0x97, D3.atkSlot, 1, 0);
        }
        break;
    case 0x4B1:
        if ((int)(D3.side[1 - p].raw << 28) >= 0) {
            int side = (1 - p) & 1;
            int s1 = D3.defSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->b6 & 1) {
                ShowCardEffect(1 - p, D3.side[1 - p].cardId);
                DuelCmd_Push(p != 1 ? 0x807E : 0x7E, D3.defSlot, 0, 0);
            }
        }
        break;
    case 0x5F3:
        if ((int)(D3.side[p].raw << 28) < 0) {
            int side = (1 - p) & 1;
            int s1 = D3.defSlot * 0x94 + side * 0xD64;
            if (D3ZONE(s1)->w0 << 20 != 0) {
                ShowCardEffect(1 - p, D3.side[1 - p].cardId);
                QueueAddZoneLink(1 - p, (u8)(1 - p) | D3.defSlot << 8, (u8)(1 - p) | D3.defSlot << 8, 0x10C);
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
        if ((k == 0x2DA || k == 0x536) && FindAbsorbedMonsterLink(1 - p, D3.defSlot) != 0xFFFF) {
            z = (u16 *)&D3.zones[1 - p];
            DestroyAbsorbedMonsters(1 - p, D3.defSlot);
            DuelCmd_Push(p != 1 ? 0x80A4 : 0xA4, D3.defSlot, z[0], z[1]);
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
                    QueueAddZoneLink(1 - p, D3.side[1 - p].cardId, (u8)p | D3.atkSlot << 8, 9);
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
                SendBattleDestroyedCardToGraveyard(1 - p, 1 - p, D3.defSlot, (u16 *)&D3.zones[1 - p]);
                D3.pend1 = (u8)(1 - p) | D3.defSlot << 8;
            }
        } else if (!done)
            BanishBattleDestroyedCard(1 - p, D3.defSlot, (u16 *)&D3.zones[1 - p]);
    }
    CB_STEP++;
    return 0;
}
