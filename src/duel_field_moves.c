#include "global.h"
#include "duel.h"

/*
 * Duel field-zone effects: per-zone "card entered / left / flipped" handlers
 * that queue duel events (DuelCmd_Push) and re-apply field bonuses.
 * See wiki/functions/code-080184d8.md.
 */

/* Zone arrays of both players: 0x0201930C + (p & 1) * 0xD64 + zone * 0x94
 * (player+0x28; zones 0-4 monsters, 5-9 magic/trap, 10 field). */
#define ZONE(p, z) (&gDuelPlayers[(p) & 1].zones[z])
/* Same address written as a raw integer (keeps the base inside loops). */
#define ZONE_AT(p, z) ((struct DuelZone *)((z) * 0x94 + ((p) & 1) * 0xD64 + 0x0201930C))
/* A zone's card ID, read through a struct DuelCard pointer (whole-word load). */
#define ZONE_CARD_ID(zp) (((struct DuelCard *)(zp))->id)

extern const u16 gCardIdToNumber[];   /* maps card ID to card number */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern const u16 gCardNumberToId[];   /* maps card key to card number (see duel_setup) */

/* Hand card words at 0x02019968 = 0x020192E4 + 0x684, 80 per player. */
struct HandRow {
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow gDuelHands[2];

/* Scratch card word / event globals used by SendBattleDestroyedCardToGraveyard. */
extern u16 gUnk_02017ECA;
extern const u32 gCardStats[];   /* maps card ID to card data word */
struct Unk02017A40 { u8 pad[0x48A]; u16 w48A; };
extern struct Unk02017A40 gChain;
struct Unk02018450 { u16 w0; u8 b1; };
extern struct Unk02018450 gBattle;

/* Event message ids have bit 15 set when they concern player 1. */
#define EVT(p, id) ((p) ? (0x8000 | (id)) : (id))

void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void DestroyLinkedCards(int player, int zone, int arg);
void SendFieldCardToGrave(int player, int zone, u16 arg, int arg3);
void DestroyFieldCard(int player, int zone, u16 arg);
void ReturnFieldCardToDeck(int player, int zone);
int CountMonsters(int player);
u16 FindMonsterLinkedToCard(int player, int zone);
void ShowDestroyedCard(int player, int cardId);

/* Helpers used by the zone-event handlers below. */
u32 HasFlipEffect(u16 cardNo, u16 flag);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CanActivateEffectOfCard(int player, u32 id, int x);
void Chain_AddPending(u32 event, int arg);
void QueueRemoveLinksToZone(int player, int zone);
void sub_080197C0(int player, u16 arg);
void ShowCardEffect(int player, u16 id);
void LoseLifePoints(int player, int lp);
void GainLifePoints(int player, int lp);
void DrawCards(int player, int n);
void LoseLpOnSendToGraveyard(int player, int arg);
void EventResponse_Request(int player, int kind, u32 arg);

void DestroyFaceUpCardsByNumber(int player, u16 cardNo)
{
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *z = ZONE_AT(player, i);
        u16 id = ZONE_CARD_ID(z);
        if (id != 0 && z->flag6_1 && CARD_NUMBER(id) == cardNo)
            DestroyFieldCard(player, i, 1);
    }
}
void DestroyFieldCard(int player, int zone, u16 arg)
{
    u32 id;
    u32 cardId;
    int i;

    id = ZONE_CARD_ID(ZONE(player, zone));
    cardId = id;

    SendFieldCardToGrave(player, zone, arg, arg != 0);
    if (id == 0)
        return;
    switch (CARD_NUMBER(id)) {
    case 0x5F8:
    case 0x605:
    case 0x606:
    case 0x607:
    case 0x608:
        break;
    default:
        return;
    }
    ShowDestroyedCard(player, cardId);
    for (i = 5; i <= 9; i++) {
        if (i != zone) {
            u32 id2 = ZONE_CARD_ID(ZONE(player, i));
            if (id2 != 0) {
                switch (CARD_NUMBER(id2)) {
                case 0x5F8:
                case 0x605:
                case 0x606:
                case 0x607:
                case 0x608:
                    SendFieldCardToGrave(player, i, arg, 0);
                    break;
                }
            }
        }
    }
}
void DestroyPlayerMonsters(int player, u16 arg)
{
    int i;

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZONE_AT(player, i);
        if (ZONE_CARD_ID(z) != 0)
            DestroyFieldCard(player, i, arg);
    }
}
void BanishBattleDestroyedCard(int player, int zone, u16 *args)
{
    DuelCmd_Push(EVT(player, 0x7D), args[0], args[1], 0);
    DestroyLinkedCards(player, zone, 1);
}
/* Battle state at 0x02018450 seen as u16 bitfields (padded past 4 bytes so agbcc
 * reads atkSlot with ldrh and defSlot with ldrb [+1], as the ROM does). */
struct Unk02018450b {
    u16 unk0_0:6;
    u16 atkSlot:3;      /* bits 6-8 */
    u16 defSlot:3;      /* bits 9-11 */
    u16 unk0_12:4;
    u16 cardId;
    u8 pad4[4];
};
#define gBattle18450 (*(struct Unk02018450b *)&gBattle)
/* A card entered zone `zone` from the hand (args = its card word). Locked play
 * (card 0x453 on either side): just report it. Card 0x1DE summons directly.
 * Otherwise set the word's 0x20 byte bit, announce 0x7C, flag monsters, then
 * dispatch per card number. */
void SendBattleDestroyedCardToGraveyard(int arg0, int player, int zone, u16 *args)
{
    u16 id;
    u32 id2;
    u32 owner;
    u32 w;

    if (zone > 4)
        return;
    if (CountFaceUpMonstersByNumber(0, 0x453) > 0 || CountFaceUpMonstersByNumber(1, 0x453) > 0) {
        BanishBattleDestroyedCard(player, zone, args);
        return;
    }
    gUnk_02017ECA = 0x13;
    owner = ((struct DuelCard *)args)->owner;
    id = ((struct DuelCard *)args)->id;
    if (CARD_NUMBER(id) == 0x1DE) {
        ShowCardEffect(player, id);
        DuelCmd_Push(EVT(player, 0x6A), args[0], args[1], 0);
        DestroyLinkedCards(player, zone, 1);
        return;
    }
    ((u8 *)args)[2] |= 0x20;
    DuelCmd_Push(EVT(player, 0x7C), args[0], args[1], 0);
    if (((((const u32 *)0x08621DE0)[((struct DuelCard *)args)->id & 0x7FF] & 0x1F00000) >> 20) <= 0x14)
        gDuelPlayers[player & 1].flagB_3 = 1;
    w = *(u32 *)args;
    id2 = (w << 20) >> 20;
    switch (CARD_NUMBER(id2)) {
    case 0x2F: case 0x12F: case 0x136: case 0x138: case 0x139: case 0x140:
    case 0x23D: case 0x414: case 0x454: case 0x456:
    case 0x45A: case 0x45B: case 0x45D: case 0x45F: case 0x460: case 0x463:
    case 0x4D9: case 0x4DA: case 0x4E9: case 0x57D:
        {
            u32 ev = owner << 31;
            u32 t, hi;
            ev |= (0x3F & gChain.w48A) << 25;
            /* FAKEMATCH: the ROM loads 0xFFFF between the two shifts of the
             * re-read card ID, so the extraction is split around it. Reusing
             * `w` (the dispatch word) for the attacker position makes its first
             * set non-constant, so local-alloc does not double its live length
             * and it takes r3 before ev. */
            t = *(u32 *)args << 20;
            w = 0xFFFF;
            t = (t >> 20) | 0x600000;
            ev |= t;
            if (player == gDuel.linkSkip)
                w = gDuel.linkSkip | gBattle18450.atkSlot << 8;
            else
                hi = 0; /* FAKEMATCH: dead store; flow deletes it only after cse2, so
                         * CSE does not carry the turn bit past the join and the
                         * ROM's re-read of +0x1B12 is kept. */
            if (player == 1 - gDuel.linkSkip)
                hi = ((u8)(1 - gDuel.linkSkip) | gBattle18450.defSlot << 8) << 16;
            else
                hi = 0xFFFF0000;
            Chain_AddPending(ev, hi | w);
        }
        break;
    case 0x1CD:
        if (!(ZONE(player, zone)->unk7 & 0x20)) {
            DuelCmd_Push(EVT(owner, 0x73), id2, 1, 0);
            LoseLifePoints(owner, 0x1388);
        }
        break;
    case 0x45C:
        if (!(ZONE(player, zone)->unk7 & 0x20)) {
            u32 ev = ((((struct DuelCard *)args)->owner & 1) << 31) | ((0x3F & gUnk_02017ECA) << 25);
            u32 t = id2 | 0x600000;
            Chain_AddPending(ev | t, 0);
        }
        break;
    case 0x5EA:
        if (arg0 == player) {
            /* other units declare the ID parameter as int */
            ((void (*)(int, int))sub_080197C0)(player, id2);
            DuelCmd_Push(EVT(player, 0x4C), 1, 0, 0);
        }
        break;
    case 0x2D9:
    case 0x534:
        QueueRemoveLinksToZone(player, zone);
        break;
    }
    LoseLpOnSendToGraveyard(player, 1);
    DestroyLinkedCards(player, zone, 1);
}
void BanishFieldCard(int player, int zone, u16 arg)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if (arg != 0)
        DuelCmd_Push(EVT(player, 0x7B), zone, 1, 0);
    else
        DuelCmd_Push(EVT(player, 0x7A), zone, 1, 0);
    if (zone <= 4)
        DestroyLinkedCards(player, zone, 1);
    if (CARD_NUMBER(id) == 0x48A)
        DuelCmd_Push(EVT(player, 0x8F), zone, 0, 0);
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        DuelCmd_Push(EVT(player, 0x11), 0, 0, 0);
}
/* A card left a zone: if its number is 0x780-0x7CF re-apply field bonuses,
 * otherwise announce 0x80; on the monster rows also notify the zone-event
 * queue; card 0x447 hands its destruction target over. */
void ReturnFieldCardToHand(int player, int zone, u16 arg2)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if ((u16)(CARD_NUMBER(id) - 0x780) < 0x50)
        DestroyFieldCard(player, zone, 1);
    else
        DuelCmd_Push(EVT(player, 0x80), zone, arg2, 0);
    if (zone <= 4) {
        DestroyLinkedCards(player, zone, 0);
        EventResponse_Request(1 - gDuel.linkSkip, 0x1B, (u16)((u8)player | ((u8)zone << 8)));
    }
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        DuelCmd_Push(EVT(player, 0x11), 0, 0, 0);
    if (CARD_NUMBER(id) == 0x447) {
        u32 r = FindMonsterLinkedToCard(player, zone);
        u16 target = r;
        if (target != 0xFFFF && !(ZONE(player, zone)->unk8C[5] & 8))
            DestroyFieldCard((u8)target, r >> 8 & 0xFF, 1);
    }
}
void ReturnFieldCardToDeck(int player, int zone)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if ((u16)(CARD_NUMBER(id) - 0x780) < 0x50)
        DestroyFieldCard(player, zone, 1);
    else
        DuelCmd_Push(EVT(player, 0x81), zone, 0, 0);
    if (zone <= 4)
        DestroyLinkedCards(player, zone, 0);
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        DuelCmd_Push(EVT(player, 0x11), 0, 0, 0);
    if (CARD_NUMBER(id) == 0x447) {
        u32 r = FindMonsterLinkedToCard(player, zone);
        u16 target = r;
        if (target != 0xFFFF && !(ZONE(player, zone)->unk8C[5] & 8))
            DestroyFieldCard((u8)target, r >> 8 & 0xFF, 1);
    }
}
void ReturnAllMonstersToDeck(int player, u16 arg)
{
    int pass;
    int p;
    int i;

    for (pass = 0; pass <= 1; pass++) {
        if (pass == 0)
            p = 1 - player;
        else
            p = player;
        for (i = 0; i <= 4; i++)
            ReturnFieldCardToDeck(p, i);
        if (arg != 0 && CountMonsters(p) > 0)
            DuelCmd_Push(EVT(p, 0x60), 1, 0, 0);
    }
}
/* A monster was summoned/flipped into `zone`: announce event 0x7F, then either the
 * "flip" message 0x90 (if not yet face-up) and possibly a special summon, or the
 * re-apply call. */
void FlipFieldCard(int player, int zone, u16 arg)
{
    int p = player & 1;
    int zoneBytes = zone * 0x94;
    struct DuelZone *z;
    u16 id;

    __asm__("" : : "r"(p));
    z = (struct DuelZone *)(zoneBytes + p * 0xD64 + 0x0201930C);
    id = ZONE_CARD_ID(z);

    if (id == 0)
        return;
    DuelCmd_Push(EVT(player, 0x7F), zone, 0, 0);
    if (zone <= 4) {
        if (!z->flag6_1) {
            DuelCmd_Push(EVT(player, 0x90), zone, 2, 0);
            if (arg != 0 && HasFlipEffect(CARD_NUMBER(id), 0) != 0
                && CanActivateEffectOfCard(player, id, 0) != 0
                && CountActiveCardsOnField(0, 0x5FA) == 0 && CountActiveCardsOnField(1, 0x5FA) == 0) {
                u32 ev = p << 31;
                u32 ev2 = ((zone & 0x1F) << 16) | 0x16400000;

                Chain_AddPending(ev | ev2 | id, 0);
            }
        } else {
            DestroyLinkedCards(player, zone, 1);
        }
    }
}
/* A monster flipped/summoned face-* in zone `zone`: announce 0x7E; for some
 * cards with a set flag (or a key-monster) and later if arg2/arg3 ask for it,
 * hand the battle effect to the other player. */
void ChangeBattlePosition(int player, int zone, u16 arg2, u16 arg3)
{
    int one = 1;
    int p = player & one;
    int zoneBytes = zone * 0x94;
    s16 playerBytes = p * 0xD64;
    struct DuelZone *z;
    u32 id;

    /* The masked player's offset fits in an s16. The asm statement keeps `one`
     * live, matching the lifetime the ROM gives it. */
    __asm__("" : : "r"(one));
    z = (struct DuelZone *)(zoneBytes + playerBytes + 0x0201930C);
    id = ZONE_CARD_ID(z);

    if (zone > 4)
        return;
    if (id == 0)
        return;
    DuelCmd_Push(EVT(player, 0x7E), zone, arg2, 0);
    if (z->flag6_0) {
        if (CARD_NUMBER(id) == 0x5E && CanActivateEffectOfCard(player, id, 0) != 0) {
            u32 ev = (player & 1) << 31;
            u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
            Chain_AddPending(ev | ev2 | id, 0);
        }
    } else {
        switch (CARD_NUMBER(id)) {
        case 0x77:
        case 0xA1:
        case 0x1F0:
            if (CanActivateEffectOfCard(player, id, 0) != 0) {
                u32 ev = (player & 1) << 31;
                u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
                Chain_AddPending(ev | ev2 | id, 0);
            }
            break;
        }
    }
    if (arg3 == 0)
        return;
    if (arg2 == 0)
        return;
    if (ZONE(player, zone)->flag6_1)
        return;
    if (HasFlipEffect(CARD_NUMBER(id), 0) == 0)
        return;
    if (CountActiveCardsOnField(0, 0x5FA) != 0)
        return;
    if (CountActiveCardsOnField(1, 0x5FA) != 0)
        return;
    {
        u32 ev = (player & 1) << 31;
        u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
        Chain_AddPending(ev | ev2 | id, 0);
    }
}
/* Move between two zones (`arg1`/`arg2` = player | zone << 8): if the source holds a
 * card and the target is empty, announce 0x82; across players, card 0x1E3/0x222
 * with +7 bit 0x20 hands its effect to the other player (0x92). Each zone is read
 * through ZONE() in place: the array form keeps the zone base in a shared register. */
void MoveFieldCard(int player, u16 arg1, u16 arg2)
{
    int p1 = (u8)arg1;
    int zone1 = arg1 >> 8;
    int p2 = (u8)arg2;
    int zone2 = arg2 >> 8;
    u32 id;

    if (ZONE_CARD_ID(ZONE(p1, zone1)) == 0)
        return;
    if (ZONE_CARD_ID(ZONE(p2, zone2)) != 0)
        return;
    DuelCmd_Push(EVT(p1, 0x82), arg1, arg2, 0);
    if (p1 == p2)
        return;
    id = ZONE_CARD_ID(ZONE(p1, zone1));
    switch (CARD_NUMBER(id)) {
    case 0x1E3:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, id);
            LoseLifePoints(p2, 0x7D0);
            DuelCmd_Push(EVT(p2, 0x92), zone2, 0, 0);
            return;
        }
        break;
    case 0x222:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, id);
            GainLifePoints(p1, 0xBB8);
            DuelCmd_Push(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    }
}
/* Two zones (arg1/arg2 = player|zone<<8): if both hold a card, announce 0x84;
 * then for card 0x1E3/0x222 in either zone with the 0x20 flag bit, hand its
 * flip effect to the other player. */
void SwapFieldCards(int player, u16 arg1, u16 arg2)
{
    int p1 = (u8)arg1;
    int zone1 = arg1 >> 8;
    int p2 = (u8)arg2;
    int zone2 = arg2 >> 8;
    u16 id1 = ZONE_CARD_ID(ZONE(p1, zone1));
    u32 id2 = ZONE_CARD_ID(ZONE(p2, zone2));

    if (id1 == 0)
        return;
    if (id2 == 0)
        return;
    DuelCmd_Push(EVT(player, 0x84), arg1, arg2, 0);
    switch (CARD_NUMBER(id1)) {
    case 0x1E3:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, ZONE_CARD_ID(ZONE(p1, zone1)));
            LoseLifePoints(p2, 0x7D0);
            DuelCmd_Push(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    case 0x222:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, ZONE_CARD_ID(ZONE(p1, zone1)));
            GainLifePoints(p1, 0xBB8);
            DuelCmd_Push(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    }
    switch (CARD_NUMBER(id2)) {
    case 0x1E3:
        if (ZONE(p2, zone2)->unk7 & 0x20) {
            sub_080197C0(p2, ZONE_CARD_ID(ZONE(p2, zone2)));
            LoseLifePoints(p1, 0x7D0);
            DuelCmd_Push(EVT(p1, 0x92), zone1, 0, 0);
        }
        break;
    case 0x222:
        if (ZONE(p2, zone2)->unk7 & 0x20) {
            sub_080197C0(p2, ZONE_CARD_ID(ZONE(p2, zone2)));
            GainLifePoints(p2, 0xBB8);
            DuelCmd_Push(EVT(p1, 0x92), zone1, 0, 0);
        }
        break;
    }
}
void ReturnHandCardToDeck(int player, int arg1, u16 arg2)
{
    DuelCmd_Push(EVT(player, 0xC3), arg1, arg2, 0);
}
/* Hand-row card used/leaves: announce 0xC0 (0xC1 while a 0x453 lock is up),
 * then per card key hand control to the other player or queue an effect. */
void DiscardHandCard(int player, int arg1, u16 arg2, u16 arg3)
{
    u32 id;
    int n;
    u16 cardNo;

    if (CountFaceUpMonstersByNumber(0, 0x453) > 0 || CountFaceUpMonstersByNumber(1, 0x453) > 0) {
        DuelCmd_Push(EVT(player, 0xC1), arg1, arg3, 0);
        return;
    }
    DuelCmd_Push(EVT(player, 0xC0), arg1, arg3, 0);
    {
        int p = player & 1;
        int indexBytes = arg1 * 4;
        u32 offset = p * 0xD64;
        u32 word;

        word = *(u32 *)(indexBytes + offset + 0x02019968);
        id = (word << 20) >> 20;
    }
    switch (CARD_NUMBER(id)) {
    case 0x215:
        if (arg2 != 0) {
            DuelCmd_Push(EVT(player, 0x73), id, 1, 0);
            LoseLifePoints(1 - player, 1000);
        }
        break;
    case 0x1CE:
        if (arg2 != 0) {
            DuelCmd_Push(EVT(player, 0x73), id, 1, 0);
            DrawCards(player, 2);
        }
        break;
    case 0x4DA:
        { u32 ev = (player & 1) << 31;
          u32 ev2 = ((0x1F & player) << 16) | 0x3A600000;
          Chain_AddPending(ev | ev2 | id, 0); }
        break;
    }
    n = CountActiveCardsOnField(1 - player, cardNo = 0x40E);
    if (n > 0) {
        DuelCmd_Push(EVT(player, 0x73), ((const u16 *)0x08623DF4)[cardNo], 1, 0);
        LoseLifePoints(player, n * 500);
    }
    LoseLpOnSendToGraveyard(player, 1);
}
