#include "global.h"
#include "duel.h"

/*
 * Duel-state helpers: per-player card lists (hand, graveyard, ...) and the
 * per-zone "link" lists of the field zones. See wiki/functions/code-08009a68.md.
 *
 * The card/zone/player structures and gDuelPlayers / gDuelZones come from
 * include/duel.h (canonical layouts and field names).
 */

/* Byte at 0x020192E4 + 0x1B0E (= 0x020192E0 + 0x1B12, "flags1B12" in code_0800C894). */
struct DuelFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;            /* compared with a player number (hypothesis: turn player / host side) */
    u8 rest : 6;
    u8 filler1[4];          /* keeps the struct larger than 4 bytes so the bitfield is read with ldrb */
};
#define DUEL_FLAGS (*(struct DuelFlags *)((u8 *)gUnk_020192E4_u9 + 0x1B0E))

/*
 * Local view used by IsHandRevealed only: the canonical struct DuelPlayer declares +0x9 as a plain
 * `u8 unk9`, but that function reads its bit 0 as a bitfield (the ROM emits `lsl #31`, whereas
 * `unk9 & 1` emits `and #1`). Full size so that [player & 1] still strides 0xD64.
 */
struct DuelPlayerUnk9 {
    u8 unk0[9];
    u8 unk9_0 : 1;
    u8 unk9_1 : 7;
    u8 rest[0xD64 - 0xA];
};
extern struct DuelPlayerUnk9 gUnk_020192E4_u9[2] asm("gDuelPlayers");

/* The same zones addressed from their own base (player + 0x28); struct DuelZonesPlayer is in duel.h. */
#define ZONE(p, z) (gDuelZones[(p) & 1].zones[z])
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1. */
#define ZB(p, z) ((struct DuelZone *)((u8 *)gDuelZones + (z) * 0x94 + (p) * 0xD64))
/* Card reference passed to EffectEquipTargetCheck (0x14 bytes on the stack; same struct as in code_0800C894). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 unk2_10 : 6;
    u8 filler4[0x14 - 0x4];
};

extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
/* Card ID from a card word: bits 0-11 (compiles to lsl #20; lsr #20). */
#define CARD_ID(w) (((w) << 20) >> 20)

/*
 * ROM tables indexed through integer-constant pointers: GCC then reloads the
 * table address at every use (inside loops too) instead of hoisting/CSEing it,
 * which is what the ROM does. Same bytes as gCardStats[] / gCardIdToNumber[].
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Card numbers 1920-1999 are tokens. */
#define IS_TOKEN(id) ((u16)(CARD_NUMBER(id) - 1920) < 80)
#define CARD_KIND(id) ((CARD_STATS(id) & 0xC0000) >> 18)
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)   /* Magic/Trap subtype (2 = Field) */

void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
int CountActiveCardsOnField(int player, u16 number);
int CountActiveCardsOnField2(int player, u16 number);
int EffectEquipTargetCheck(struct CardRef *card, u16 pos);
void DestroyFieldCard(int player, int zone, u16 arg);
void AddCardToFusionDeck(int owner, struct DuelCard *card);
int CountOtherFaceUpSameNameMonsters(int a, int b);
int RemoveGraveyardCardAt(int player, int idx);
int RemoveBanishedCardAt(int player, int idx);

/* Card category (same inline as in code_08006878): 3/1 for card numbers 1910/1911-1912,
 * 7 Magic, 8 Trap, 9 Ticket, else the monster kind (0 normal, 1 effect, 2 fusion, 3 ritual). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return CARD_KIND(id);
    }
}

/* Remove hand[idx], shifting the rest down (inlined into its callers). */
static inline void RemoveHandCard(int player, int idx)
{
    int i;

    gDuelPlayers[player & 1].handCount--;
    for (i = idx; i < gDuelPlayers[player & 1].handCount; i++)
        CopyDuelCard(&gDuelPlayers[player & 1].hand[i], &gDuelPlayers[player & 1].hand[i + 1]);
}

/* Remove entry idx from graveyard, shifting the rest down. */
int RemoveGraveyardCardAt(int player, int idx)
{
    int i;

    if (idx < gDuelPlayers[player & 1].graveCount) {
        gDuelPlayers[player & 1].graveCount--;
        for (i = idx; i < gDuelPlayers[player & 1].graveCount; i++)
            CopyDuelCard(&gDuelPlayers[player & 1].graveyard[i], &gDuelPlayers[player & 1].graveyard[i + 1]);
        return 1;
    }
    return 0;
}
/* Copy graveyard[idx] to *out, then remove it from the list. */
int TakeGraveyardCardAt(int player, int idx, struct DuelCard *out)
{
    int i;

    if (idx < gDuelPlayers[player & 1].graveCount) {
        CopyDuelCard(out, &gDuelPlayers[player & 1].graveyard[idx]);
        gDuelPlayers[player & 1].graveCount--;
        for (i = idx; i < gDuelPlayers[player & 1].graveCount; i++)
            CopyDuelCard(&gDuelPlayers[player & 1].graveyard[i], &gDuelPlayers[player & 1].graveyard[i + 1]);
        return 1;
    }
    return 0;
}
/* Remove the first graveyard entry equal to *card. */
u16 RemoveCardFromGraveyard(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].graveyard[i];
        if (CARD_WORD(*card) == CARD_WORD(*entry))
            return RemoveGraveyardCardAt(player, i);
    }
    return 0;
}
/* Remove the first graveyard entry with card ID `id`. */
u16 RemoveGraveyardCardById(int player, u16 id)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        if (CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i])) == id)
            return RemoveGraveyardCardAt(player, i);
    }
    return 0;
}
static inline u16 GetGraveyardCardId(int player, int idx)
{
    struct DuelCard *card = &gDuelPlayers[player & 1].graveyard[idx];

    return card->id;
}
/* Find the first graveyard entry with card ID `id` and copy it to *out. */
int GetGraveyardCardById(int player, u16 id, struct DuelCard *out)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].graveyard[i];
        if (GetGraveyardCardId(player, i) == id) {
            CopyDuelCard(out, entry);
            return 1;
        }
    }
    return 0;
}
/* Return 1 if graveyard contains a card equal to *card. */
int IsCardInGraveyard(int player, struct DuelCard *card)
{
    struct DuelCard *source = card;
    int i = 0;
    u8 *root = (u8 *)gDuelPlayers;
    u32 offset = (player & 1) * 0xD64;
    /* FAKEMATCH: the initialized count uses r2 until copied into the loop bound. */
    register int count asm("r2") = *((u8 *)(offset + (u32)root) + 4);

    if (i < count) {
        u8 *listbase = root + 0x904;
        int bound = count;
        u32 want;
        struct DuelCard *entry;

        want = CARD_WORD(*source);
        entry = (struct DuelCard *)(offset + (u32)listbase);
        do {
            if (want == CARD_WORD(*entry))
                return 1;
            entry++;
            i++;
        } while (i < bound);
    }
    return 0;
}
/* Count graveyard entries whose card number is `number`. */
int CountGraveyardCardsByNumber(int player, u16 number)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            count++;
    }
    return count;
}
/* Remove entry idx from listB84 (and its parallel arrCC4), shifting the rest down. */
int RemoveBanishedCardAt(int player, int idx)
{
    if (idx < gDuelPlayers[player & 1].countB84) {
        int i = idx;

        gDuelPlayers[player & 1].countB84--;
        i++; /* FAKEMATCH: no-op pair that shifts the index copy's scheduling */
        i--;
        for (; i < gDuelPlayers[player & 1].countB84; i++) {
            CopyDuelCard(&gDuelPlayers[player & 1].listB84[i], gDuelPlayers[player & 1].listB84 + i + 1);
            gDuelPlayers[player & 1].arrCC4[i] = gDuelPlayers[player & 1].arrCC4[i + 1];
        }
        return 1;
    }
    return 0;
}
/* Remove the first listB84 entry equal to *card. */
u16 RemoveCardFromBanished(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].countB84; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].listB84[i];
        if (CARD_WORD(*card) == CARD_WORD(*entry))
            return RemoveBanishedCardAt(player, i);
    }
    return 0;
}
/* Return the card type, preserving the inline lookup's narrow ID argument. */
static inline int GetCardType(u16 id)
{
    return CARD_TYPE(id);
}
/* Count graveyard entries whose card type is a monster type (<= 20). */
int CountGraveyardMonsters(int player)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        if ((u32)GetCardType(CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i]))) <= 20)
            count++;
    }
    return count;
}
/* Count hand entries whose card type is a monster type (<= 20). */
int CountHandMonsters(int player)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        if ((u32)GetCardType(CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]))) <= 20)
            count++;
    }
    return count;
}
/* Add *card to the end of the hand; fusion monsters (subtype 2) go to AddCardToFusionDeck instead. Tokens are dropped. */
void AddCardToHand(int player, struct DuelCard *card)
{
    int count = gDuelPlayers[player & 1].handCount;
    struct DuelCard *slot = &gDuelPlayers[player & 1].hand[count];

    /* FAKEMATCH: keep card in r4 and the hand-offset scratch in r3; emits no instructions. */
    asm volatile ("" : : "r"(card) : "r3");

    if (card->id == 0 || IS_TOKEN(card->id))
        return;
    if (GetCardSubtype(card->id) == 2) {
        AddCardToFusionDeck(card->owner, card);
    } else {
        CopyDuelCard(slot, card);
        gDuelPlayers[player & 1].handCount++;
    }
}

/* Clear the card ID of hand[idx] (if idx is in range). */
void ClearHandCardAt(int player, int idx)
{
    int count = gDuelPlayers[player & 1].handCount;
    struct DuelCard *card = &gDuelPlayers[player & 1].hand[idx];

    if (idx < count)
        card->id = 0;
}
/* Read a hand card as the complete instance word. */
static inline u32 GetHandCardWord(int player, int idx)
{
    struct DuelCard *card = &gDuelPlayers[player & 1].hand[idx];

    return CARD_WORD(*card);
}
/* Remove the first hand card equal to *card, shifting the rest down; 1 if found. */
int RemoveCardFromHand(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        if (GetHandCardWord(player, i) == CARD_WORD(*card)) {
            RemoveHandCard(player, i);
            return 1;
        }
    }
    return 0;
}
/* Compact the hand: remove every empty (ID 0) entry. */
void CompactHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount;) {
        if (CARD_ID(GetHandCardWord(player, i)) == 0)
            RemoveHandCard(player, i);
        else
            i++;
    }
}
/* Return the index of the first hand card of type 21 (Trap), or -1. */
int FindTrapInHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));
        if (id != 0 && CARD_TYPE(id) == 21)
            return i;
    }
    return -1;
}
/* Return the index of the first hand card of type 22 (Magic), or -1. */
int FindMagicInHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));
        if (id != 0 && CARD_TYPE(id) == 22)
            return i;
    }
    return -1;
}
/* Masked table accessors used only by FindNonFieldMagicInHand (see FAKEMATCH note there). */
#define CARD_STATS_M(id, m) (((const u32 *)0x08621DE0)[(id) & (m)])
#define CARD_TYPE_M(id, m) ((CARD_STATS_M(id, m) & 0x1F00000) >> 20)
#define CARD_SUBTYPE_M(id, m) ((CARD_STATS_M(id, m) & 0xE0000) >> 17)
/* Return the index of the first hand Magic card that is not a Field card (subtype 2), or -1. */
int FindNonFieldMagicInHand(int player)
{
    int i;
    u32 mask;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        mask = 0x7FF; /* FAKEMATCH: local mask so the 0x7FF load lands one instruction later */
        if (id != 0 && CARD_TYPE_M(id, mask) == 22 && CARD_SUBTYPE_M(id, mask) != 2)
            return i;
    }
    return -1;
}
/* Count hand entries whose card number is `number`. */
int CountHandCardsByNumber(int player, u16 number)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            count++;
    }
    return count;
}
/* Return the index of the first hand entry whose card number is `number`, or -1. */
int FindHandCardByNumber(int player, u16 number)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            return i;
    }
    return -1;
}
/* 1 if the player's +9 flag is set, or card 1093 is on the opponent's side, or card 1121 on either side,
 * or (when DUEL_FLAGS.bit1 == player) card 1152 on either side. */
int IsHandRevealed(int player)
{
    int opponent;

    if (gUnk_020192E4_u9[player & 1].unk9_0)
        return 1;
    opponent = 1 - player;
    if (CountActiveCardsOnField2(opponent, 1093) > 0 || CountActiveCardsOnField2(player, 1121) > 0 || CountActiveCardsOnField2(opponent, 1121) > 0)
        return 1;
    if (player == DUEL_FLAGS.bit1) {
        if (CountActiveCardsOnField2(player, 1152) > 0 || CountActiveCardsOnField2(opponent, 1152) > 0)
            return 1;
    }
    return 0;
}
/* If card number 1178 is present (either player) and CountOtherFaceUpSameNameMonsters(a, b) > 0, return its total count; else 0. */
int CountAquaChorusBoosts(int a, int b)
{
    int count = CountActiveCardsOnField(0, 1178) + CountActiveCardsOnField(1, 1178);

    if (count > 0 && CountOtherFaceUpSameNameMonsters(a, b) > 0)
        return count;
    return 0;
}
/* Return the first link of zone (player, zone) whose kind is 5, or 0xFFFF. */
u16 FindAbsorbedMonsterLink(int player, int zone)
{
    int i;
    struct DuelZone *z;

    i = 0;
    player &= 1;
    z = (struct DuelZone *)((u8 *)gDuelZones + (zone * 0x94 + player * 0xD64));
    for (; i < z->numLinks; i++) {
        u16 link = z->links[i];
        if ((u8)z->linkKinds[i] == 5)
            return link;
    }
    return 0xFFFF;
}
/* For each kind-1 link of zone (player, zone) whose linked card is one of a fixed set of card
 * numbers, call EffectEquipTargetCheck(linked card, (player, zone)); if that returns 0, DestroyFieldCard(linked zone). */
void DestroyInvalidEquips(int player, int zone)
{
    struct CardRef ref;
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        u16 link = ZB(player & 1, zone)->links[i];

        if ((u8)ZB(player & 1, zone)->linkKinds[i] == 1) {
            int lp = (u8)link;
            u16 lz = link >> 8;
            u16 id = CARD_ID(CARD_WORD(ZB(lp & 1, lz)->card));

            switch (CARD_NUMBER(id)) {
            case 300: case 301: case 302:
            case 304: case 305: case 306: case 307: case 308: case 309:
            case 311:
            case 314: case 315: case 316:
            case 318:
            case 321: case 322: case 323: case 324: case 325: case 326: case 327:
            case 651:
            case 653:
            case 667:
            case 962:
            case 1012: case 1013:
            case 1046: case 1047:
            case 1182:
            case 1314:
            case 1422:
            case 1540:
            case 1550:
                ref.player = lp;
                ref.zone = lz;
                ref.id = id;
                if (EffectEquipTargetCheck(&ref, (u8)player | ((u8)zone << 8)) == 0)
                    DestroyFieldCard(lp, lz, 1);
                break;
            }
        }
    }
}
/* Magic/Trap subtype (stats bits 17-19) of a Magic or Trap card, else 0. */
static inline int GetMagicSubtype(u16 id)
{
    u32 stats = CARD_STATS(id);

    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}
/* Count links of zone (player, zone): kind 5 always counts; kind 1 (link to another zone) counts
 * when the linked card exists and (!needMagic || it is a Magic card) or (!needSubtype3 || its
 * Magic/Trap subtype is 3). */
int CountZoneEquips(int player, int zone, u16 needMagic, u16 needSubtype3)
{
    int count = 0;
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        u16 link = ZB(player & 1, zone)->links[i];
        switch ((u8)ZB(player & 1, zone)->linkKinds[i]) {
        case 1: {
            int ok = 0;
            int lp = (u8)link;
            u16 id = CARD_ID(CARD_WORD(ZB(lp & 1, link >> 8)->card));

            if (id == 0)
                break;
            if (!needMagic || CARD_TYPE(id) == 22)
                ok = 1;
            if (!needSubtype3 || GetMagicSubtype(id) == 3)
                ok = 1;
            if (ok)
                count++;
            break;
        }
        case 5:
            count++;
            break;
        }
    }
    return count;
}
/* Like CountActiveZoneLinksFromCard, but the linked zone's +0x91 bit 3 only disqualifies the link for card
 * numbers 348, 1058 and 1244. The linked zone number is a local (lz) shared by both lookups;
 * lp is a u8 local (not int), which keeps loop.c from strength-reducing linkKinds[i] in its first pass. */
int CountZoneLinksFromCard(int player, int zone, u16 number)
{
    int i;
    int count = 0;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        u16 link = ZB(player & 1, zone)->links[i];
        u8 kind = ZB(player & 1, zone)->linkKinds[i];
        u8 lp = link;
        int lz = link >> 8;
        u16 id = CARD_ID(CARD_WORD(ZB(lp & 1, lz)->card));
        int valid = 1;

        switch (number) {
        case 348:
        case 1058:
        case 1244:
            if (ZB(lp & 1, lz)->unk8C[5] & 8)
                valid = 0;
            break;
        }
        if (valid) {
            switch (kind) {
            case 1:
            case 2:
            case 10:
                if (CARD_NUMBER(id) == number)
                    count++;
                break;
            case 3:
                if (CARD_NUMBER(link) == number)
                    count++;
                break;
            }
        }
    }
    return count;
}
/* Count links of zone (player, zone) that refer to card number `number`: kinds 1, 2 and 10 link to
 * another zone (low byte player, high byte zone; skipped if that zone's +0x91 bit 3 is set),
 * kind 3 holds a card ID directly. */
int CountActiveZoneLinksFromCard(int player, int zone, u16 number)
{
    int count = 0;
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        u16 link = ZB(player & 1, zone)->links[i];
        u8 kind = ZB(player & 1, zone)->linkKinds[i];
        int lp = (u8)link;
        u16 id = CARD_ID(CARD_WORD(ZB(lp & 1, link >> 8)->card));
        int valid = 1;

        if (ZB(lp & 1, link >> 8)->unk8C[5] & 8)
            valid = 0;
        if (valid) {
            switch (kind) {
            case 1:
            case 2:
            case 10:
                if (gCardIdToNumber[id & 0x7FF] == number)
                    count++;
                break;
            case 3:
                if (gCardIdToNumber[link & 0x7FF] == number)
                    count++;
                break;
            }
        }
    }
    return count;
}
/* 1 if zone (player, zone) has a kind-3 link (link = card ID) whose card number is `number`.
 * The count is read through ZB (zone offset first); the body forms the zone pointer from a
 * separate player pointer, so its address ((player + base) + zone) is not CSE'd with the count's. */
int HasZoneCardEffectLink(int player, int zone, u16 number)
{
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        struct DuelZonesPlayer *pl = &gDuelZones[player & 1];
        struct DuelZone *z = &pl->zones[zone];
        u16 link = z->links[i];

        if (z->linkKinds[i] == 3 && CARD_NUMBER(link) == number)
            return 1;
    }
    return 0;
}
/* Index of the first link of zone (player, zone) whose card has card number `number`
 * (kinds 1/2: linked zone's card; kind 3: the link is a card ID), or -1. */
int FindZoneLinkFromCard(int player, int zone, u16 number)
{
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        u16 link = ZB(player & 1, zone)->links[i];
        u8 kind = ZB(player & 1, zone)->linkKinds[i];
        int lp = (u8)link;
        u16 id = CARD_ID(CARD_WORD(ZB(lp & 1, link >> 8)->card));

        switch (kind) {
        case 1:
        case 2:
            if (CARD_NUMBER(id) == number)
                return i;
            break;
        case 3:
            if (CARD_NUMBER(link) == number)
                return i;
            break;
        }
    }
    return -1;
}
