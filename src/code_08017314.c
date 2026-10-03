#include "global.h"

/*
 * Duel effect helpers around zone links and the effect-message queue
 * (DuelCmd_Push). See wiki/functions/code-08017314.md.
 */

struct DuelCard {
    u32 id : 12;        /* card ID (index into gCardStats / gCardIdToNumber); 0 = none */
    u32 owner : 1;
    u32 unk13 : 5;
    u32 unk18 : 1;
    u32 unk19 : 13;
};
#define CARD(word) (*(struct DuelCard *)&(word))

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    u32 card;               /* +0x00: struct DuelCard */
    u8 filler4[2];
    u8 flags6;              /* +0x06: bit 1 = face up (hypothesis) */
    u8 flags7;              /* +0x07: bit 5 tested by SendFusionMaterialToGrave */
    u8 filler8[2];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkInfo[32];       /* +0x4A: low byte = link kind */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 unk91_0 : 3;         /* +0x91 */
    u8 unk91_3 : 1;
    u8 unk91_4 : 4;
    u8 filler92[2];
};

struct DuelPlayer {
    u8 unk0[0xB];
    u8 unkB;                        /* +0x00B: bit 3 set when a monster enters (hypothesis) */
    u8 unkC[0x1C];
    struct DuelZone zones[11];      /* +0x028 */
    u8 filler684[0xD64 - 0x684];
};
extern struct DuelPlayer gDuelPlayers[2];

#define PLAYER(p) (gDuelPlayers[(p) & 1])
#define ZONE(p, z) (gDuelPlayers[(p) & 1].zones[z])
/* Zone address from a precomputed t = player & 1: base + (zone * 0x94 + t * 0xD64). */
#define ZONE_T(t, z) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + ((z) * 0x94 + (t) * 0xD64)))
#define ZONE_PTR_ZP(p, z) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + ((z) * 0x94 + ((p) & 1) * 0xD64)))
#define ZONE_PTR(p, z) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (((p) & 1) * 0xD64 + (z) * 0x94)))
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void EventResponse_Request(int player, int kind, u32 arg);
u16 FindMonsterLinkedToCard(int player, int zone);
void DestroyFieldCard(int player, int zone, u16 arg);
int FindFreeMonsterZone(int player);
void RemoveZoneLink(u16 loc, u16 a, u16 b);
void MoveFieldCard(int player, u16 loc, u16 other);
void QueueRemoveZoneLink(int player, u16 loc, u16 b, u16 c);
void EquipCard(int player, u16 loc, u16 b);
void QueueRemoveLinksToZone(int player, int zone);
void DestroyLinkedCards(int player, int zone, u16 arg);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountActiveCardsOnField(int player, u16 cardNo);
void Chain_AddPending(u32 event, int arg);
void LoseLpOnSendToGraveyard(int player, int arg);
void LoseLifePoints(int player, int arg);
void RemoveTargetLinksTo(int player, int zone, int otherPlayer, int otherZone);

void UpdateMonsterControl(int player, int zone, u16 link0)
{
    int i, j;
    int owner;
    int t = player & 1;
    struct DuelZone *z = ZONE_T(t, zone);

    if (CARD(z->card).id == 0)
        return;
    owner = player;
    for (i = z->numLinks; i > 0; i--) {
        u16 l;
        u8 kind;
        int lp, lz;
        struct DuelZone *zp = ZONE_T(t, zone);
        j = i - 1;
        l = zp->links[j];
        kind = zp->linkInfo[j];
        lp = (u8)zp->links[j];
        lz = l >> 8;
        if (kind == 1) {
            int lt = lp & 1;
            struct DuelZone *lzp = ZONE_T(lt, lz);
            u16 id = CARD(lzp->card).id;
            if (id != 0 && (lzp->flags6 & 2)) {
                switch (CARD_NUMBER(id)) {
                case 1068: {
                    int f = lzp->unk91_3;
                    if (l == link0)
                        f = 1;
                    owner = lp;
                    if (f)
                        owner = 1 - owner;
                    break;
                }
                case 1514:
                    owner = lp;
                    if (l == link0)
                        owner = 1 - owner;
                    break;
                }
            }
        }
    }
    if (owner != player) {
        int r = FindFreeMonsterZone(owner);
        if (r == -1) {
            RemoveZoneLink((u8)player | ((u8)zone << 8), link0, 1);
            DestroyFieldCard(player, zone, 1);
        } else {
            MoveFieldCard(player, (u8)player | ((u8)zone << 8), (u8)owner | ((u8)r << 8));
        }
    }
}

struct Unk02017A40_17460 { u8 pad[0x48A]; u16 w48A; };
extern struct Unk02017A40_17460 gChain;
struct DuelCard17460 {
    u32 id : 12;
    u32 owner : 1;
    u32 unk13 : 1;
    u32 unk14 : 3;
    u32 unk17 : 15;
};
/* Player view for this function: indexing the struct array through a pointer cast
 * loads the 0x020192E4 base before the index (get_inner_reference), as the ROM does. */
struct DuelPlayer17460 {
    u8 unk0[0xB];
    u8 unkB_0 : 3;
    u8 unkB_3 : 1;
    u8 unkB_4 : 4;
    u8 unkC[2];
    u16 unkE[11];           /* +0x0E: per-zone value passed to GainLifePoints by card 954 */
    u8 unk24[0xD64 - 0x24];
};
struct DuelPlayers17460 { struct DuelPlayer17460 p[2]; };
#define PLAYERS17460 ((struct DuelPlayers17460 *)gDuelPlayers)
void DestroyFaceUpCardsByNumber(int player, u16 cardNo);
u16 FindMonsterWithLinkTo(int player, int zone);
void SendFieldCardToGrave(int player, int zone, u16 arg, u16 arg3);
int CountActiveCardsOnFieldExcept(int player, u16 number, int zone);
int IsToonMonster(u16 number);
void GainLifePoints(int player, int lp);

/* Zone pointer with `p & 1` evaluated first, then zone * 0x94, then t * 0xD64. */
#define ZONE_PZ(p, z) ({ int _t = (p) & 1; ZONE_T(_t, z); })

/* Card leaves (player, zone): per-card leave effects (333, 1162, 478/1042, 1160/1095/1068/1514),
 * the leave event (0x79) and EventResponse_Request notification by card type, then (if arg3) the
 * re-trigger effects (Chain_AddPending events, 461, 954, 1116, 1162) and DestroyLinkedCards. */
void SendFieldCardToGrave(int player, int zone, u16 arg, u16 arg3)
{
    int t = player & 1;
    struct DuelZone *z = ZONE_T(t, zone);
    u32 id = CARD(z->card).id;
    struct DuelCard17460 *card;
    int i;
    u16 r;
    u16 number;

    if (id == 0)
        return;
    card = (struct DuelCard17460 *)&ZONE(player, zone).card;
    if (arg3 != 0) {
        switch (CARD_NUMBER(id)) {
        case 333:
            if ((z->flags6 & 2) && zone == 10) {
                DestroyFaceUpCardsByNumber(player, 0x58F);
                DestroyFaceUpCardsByNumber(1 - player, 0x58F);
            }
            break;
        case 1162:
            DuelCmd_Push(player ? 0x808F : 0x8F, zone, 0, 0);
            break;
        }
        /* zone (not 10): CSE substitutes the known value and keeps t*0xD64 + 0x5C8 apart from the base */
        if (zone == 10 && (ZONE_PTR(player, zone)->flags6 & 2))
            DuelCmd_Push(player ? 0x8011 : 0x11, 0, 0, 0);
    }
    if (CountFaceUpMonstersByNumber(0, 1107) > 0 || CountFaceUpMonstersByNumber(1, 1107) > 0) {
        DuelCmd_Push(player ? 0x807A : 0x7A, zone, arg, 0);
        DestroyLinkedCards(player, zone, 1);
        return;
    }
    if (CARD_TYPE(id) <= 20)
        PLAYERS17460->p[player & 1].unkB_3 = 1;
    if (CARD_NUMBER(id) == 478 || CARD_NUMBER(id) == 1042) {
        DuelCmd_Push(player ? 0x8073 : 0x73, id, 1, 0);
        DuelCmd_Push(player ? 0x8081 : 0x81, zone, arg, 0);
        if (CARD_TYPE(id) <= 20 && zone <= 4)
            DestroyLinkedCards(player, zone, 1);
        return;
    }
    DuelCmd_Push(player ? 0x8079 : 0x79, zone, arg, 0);
    switch (CARD_TYPE(id)) {
    case 0x16:
        EventResponse_Request(1 - player, 0x14, (u16)((u8)player | ((u8)zone << 8)));
        break;
    case 0x15:
        EventResponse_Request(1 - player, 0x15, (u16)((u8)player | ((u8)zone << 8)));
        break;
    default:
        EventResponse_Request(1 - player, 0x1E, (u16)((u8)player | ((u8)zone << 8)));
        break;
    }
    switch (CARD_NUMBER(id)) {
    case 1160:
        r = FindMonsterWithLinkTo(player, zone);
        if (r != 0xFFFF && arg3 != 0) {
            RemoveZoneLink(r, (u8)player | ((u8)zone << 8), 1);
            SendFieldCardToGrave((u8)r, r >> 8, 1, 1);
        }
        break;
    case 1095:
        r = FindMonsterWithLinkTo(player, zone);
        if (r != 0xFFFF && arg3 != 0 && !ZONE_PZ(player, zone)->unk91_3) {
            RemoveZoneLink(r, (u8)player | ((u8)zone << 8), 2);
            SendFieldCardToGrave((u8)r, r >> 8, 1, 1);
        }
        break;
    case 1068:
    case 1514:
        r = FindMonsterWithLinkTo(player, zone);
        if (r != 0xFFFF)
            UpdateMonsterControl((u8)r, r >> 8, (u8)player | ((u8)zone << 8));
        break;
    }
    LoseLpOnSendToGraveyard(player, 1);
    if (arg3 == 0)
        return;
    number = CARD_NUMBER(id);
    switch (number) {
    case 47:
    case 573:
    case 1241:
    case 1257:
        if ((*(u32 *)card & 0x1C000) || zone > 4)   /* mask test, not the unk14 bitfield read */
            Chain_AddPending((card->owner << 31) | 0x3C600000 | id, 0);
        break;
    case 1242:
        Chain_AddPending((card->owner << 31) | 0x3C600000 | id, 0);
        break;
    case 303:
    case 310:
    case 312:
    case 313:
    case 320:
        Chain_AddPending((card->owner << 31) | (gChain.w48A == 0x13 ? 0x26600000 : 0x28600000) | id, 0);
        break;
    case 461:
        if (!(ZONE_PZ(player, zone)->flags7 & 0x20)) {
            DuelCmd_Push(player ? 0x8073 : 0x73, id, 1, 0);
            LoseLifePoints(player, 5000);
        }
        break;
    case 954:
        if ((ZONE_PZ(player, zone)->flags6 & 2) && !ZONE_PZ(player, zone)->unk91_3) {
            DuelCmd_Push(player ? 0x8073 : 0x73, id, 1, 0);
            if (CountActiveCardsOnFieldExcept(player, number, zone) == 0) {
                for (i = 0; i <= 4; i++) {
                    u16 id2 = CARD(ZONE_PTR(player, i)->card).id;
                    if (id2 != 0 && IsToonMonster(CARD_NUMBER(id2)))
                        SendFieldCardToGrave(player, i, 1, 1);
                }
                for (i = 0; i <= 4; i++) {
                    u16 id2 = CARD(ZONE_PTR(1 - player, i)->card).id;
                    if (id2 != 0 && IsToonMonster(CARD_NUMBER(id2)))
                        SendFieldCardToGrave(1 - player, i, 1, 1);
                }
            }
            /* read twice (CSE'd): a single local changes the register choice */
            if (PLAYERS17460->p[player & 1].unkE[zone] != 0) {
                GainLifePoints(player, PLAYERS17460->p[player & 1].unkE[zone]);
                DuelCmd_Push(player ? 0x80B3 : 0xB3, zone, 0, 0);
            }
        }
        break;
    case 1116:
        if (!(ZONE_PZ(player, zone)->flags7 & 0x20))
            Chain_AddPending(((card->owner & 1) << 31) | 0x3C600000 | id, 0);
        break;
    case 1162:
        DuelCmd_Push(player ? 0x808F : 0x8F, zone, 0, 0);
        break;
    }
    DestroyLinkedCards(player, zone, 1);
}

void QueueAddZoneLink(int player, u16 a, u16 b, u16 c)
{
    DuelCmd_Push(player ? 0x8085 : 0x85, a, b, c);
}

void QueueRemoveZoneLink(int player, u16 a, u16 b, u16 c)
{
    DuelCmd_Push(player ? 0x8086 : 0x86, a, b, c);
}

void EquipCard(int player, u16 from, u16 to)
{
    int fp = (u8)from;
    int fz = from >> 8;
    int tp = (u8)to;
    int tz = to >> 8;
    u16 id = CARD(ZONE_PTR(tp, tz)->card).id;

    DuelCmd_Push(player ? 0x8083 : 0x83, from, to, 0);
    if (CARD_NUMBER(id) == 1351) {
        DuelCmd_Push(tp ? 0x8073 : 0x73, id, 1, 0);
        DestroyFieldCard(fp, fz, 1);
        if (CARD_NUMBER(CARD(ZONE_PTR(fp, fz)->card).id) == 1160)
            DestroyFieldCard(tp, tz, 1);
    } else {
        EventResponse_Request(1 - player, 0x19, from | (to << 16));
    }
}


void MoveEquipCard(u16 loc, u16 arg)
{
    u8 player = loc;
    u16 x = FindMonsterLinkedToCard(player, loc >> 8);
    QueueRemoveZoneLink(player, loc, x, 1);
    EquipCard(player, loc, arg);
}

void QueueRemoveLinksToZone(int player, int zone)
{
    int p, z, i;
    for (p = 0; p < 2; p++) {
        for (z = 0; z < 11; z++) {
            if (CARD(ZONE_PTR(p, z)->card).id != 0) {
                for (i = 0; i < ZONE_PTR(p, z)->numLinks; i++) {
                    u16 l = ZONE_PTR(player, zone)->links[p];
                    u8 kind = ZONE_PTR(player, zone)->linkInfo[p];
                    switch (kind) {
                    case 1:
                    case 2:
                        QueueRemoveZoneLink(player, (u8)player | ((u8)zone << 8), l, kind);
                        break;
                    }
                }
            }
        }
    }
}

void RemoveTargetLinksTo(int player, int zone, int otherPlayer, int otherZone)
{
    /* Preserve the ROM's counter allocation; this is an initialized local. */
    register int i asm("r5") = 0;
    for (; i < ZONE_PTR(player, zone)->numLinks; i++) {
        int to = (player & 1) * 0xD64;
        struct DuelZone *z = (struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (zone * 0x94 + to));
        int offset = i * 2;
        u16 *link = (u16 *)((u8 *)z + 0xA);
        u8 *info;
        u16 l;
        int lp;
        int lz;
        int kind;
        link = (u16 *)((u8 *)link + offset);
        info = (u8 *)z + 0x4A;
        info += offset;
        l = *link;
        lp = *(u8 *)link;
        lz = l >> 8;
        kind = *info;
        if (kind == 2 && lp == otherPlayer && lz == otherZone)
            QueueRemoveZoneLink(otherPlayer, l, (u8)player | ((u8)zone << 8), 2);
    }
}

void DestroyLinkedCards(int player, int zone, u16 arg)
{
    int i;
    int z;

    for (i = 0; i < ZONE_PTR(player, zone)->numLinks; i++) {
        int t = player & 1;
        struct DuelZone *zp = ZONE_T(t, zone);
        int offset = i * 2;
        u16 *link = (u16 *)((u8 *)zp + 0xA);
        u8 *info;
        int lp, lz;
        u16 id;
        int f;
        link = (u16 *)((u8 *)link + offset);
        info = (u8 *)zp + 0x4A;
        info += offset;
        lp = (u8)*link;
        lz = *link >> 8;
        id = CARD(ZONE_PTR(lp, lz)->card).id;
        f = 0;

        switch (*info) {
        case 1:
        case 10:
            if (id != 0)
                f = 1;
            break;
        case 2:
            switch (CARD_NUMBER(id)) {
            case 348:
            case 1095:
            case 1417:
                if (!ZONE_PTR(lp, lz)->unk91_3)
                    f = arg;
                break;
            case 1244:
                if (!ZONE_PTR(lp, lz)->unk91_3)
                    f = 1;
                break;
            }
            break;
        case 5:
        case 6:
            f = 1;
            break;
        }
        if (f)
            DestroyFieldCard(lp, lz, arg != 0);
    }
    if (zone <= 4) {
        for (i = 0; i <= 1; i++)
            for (z = 0; z <= 4; z++)
                RemoveTargetLinksTo(i, z, player, zone);
    }
}


/* For every kind-5 link of the zone, call DestroyFieldCard(linkPlayer, linkZone, 1). */
void DestroyAbsorbedMonsters(int player, int zone)
{
    int i = 0;
    if (i < ZONE_PTR(player, zone)->numLinks) {
        struct DuelZone *z = ZONE_PTR(player, zone);
        u16 *count = &z->numLinks;
        do {
            int offset = i * 2;
            u16 *link = (u16 *)((u8 *)z + 0xA);
            u8 *info;
            int lp;
            int lz;
            int kind;
            link = (u16 *)((u8 *)link + offset);
            info = (u8 *)z + 0x4A;
            info += offset;
            lp = *(u8 *)link;
            lz = *link >> 8;
            kind = *info;
            if (kind == 5)
                DestroyFieldCard(lp, lz, 1);
            i++;
        } while (i < *count);
    }
}

int TributeMonster(int player, int zone)
{
    int t = player & 1;
    int zo = zone * 0x94;
    int to = t * 0xD64;
    u32 id = CARD(*(struct DuelCard *)((u8 *)gDuelPlayers[0].zones + to + zo)).id;

    if (zone > 4)
        return 0;
    if (CountActiveCardsOnField(0, 1418) > 0 || CountActiveCardsOnField(1, 1418) > 0)
        return 0;
    if (id != 0) {
        struct DuelZone *z = &gDuelPlayers[player & 1].zones[zone];
        if (CountFaceUpMonstersByNumber(0, 1107) > 0 || CountFaceUpMonstersByNumber(1, 1107) > 0) {
            DuelCmd_Push(player ? 0x807A : 0x7A, zone, 1, 0);
            DestroyLinkedCards(player, zone, 1);
            return;
        }
        DuelCmd_Push(player ? 0x8093 : 0x93, zone, 0, 0);
        if (CARD_TYPE(id) <= 20)
            { u8 *pl = (u8 *)&PLAYER(player); pl[0xB] |= 8; }
        LoseLpOnSendToGraveyard(player, 1);
        switch (CARD_NUMBER(id)) {
        case 47:
        case 573:
        case 1242:
        case 1257:
            Chain_AddPending((CARD(z->card).owner << 31) | 0x3C600000 | id, 0);
            break;
        case 303:
        case 310:
        case 312:
        case 313:
        case 320:
            Chain_AddPending((CARD(z->card).owner << 31) | 0x28600000 | id, 0);
            break;
        case 461:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20)) {
                DuelCmd_Push(player ? 0x8073 : 0x73, id, 1, 0);
                LoseLifePoints(player, 5000);
            }
            break;
        case 1116:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20))
                Chain_AddPending(((CARD(z->card).owner & 1) << 31) | 0x3C600000 | id, 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case 729:
        case 1332:
            QueueRemoveLinksToZone(player, zone);
        }
        DestroyLinkedCards(player, zone, 0);
        EventResponse_Request(1 - player, 0x1E, (u16)((u8)player | ((u8)zone << 8)));
        return 1;
    }
    return 0;
}
/* A monster was placed in (player, zone): queue the "summoned" event, set the player's +0x0B bit 3
 * for monsters, fire the card's on-summon effects, then re-evaluate the zone's links.
 * Returns 1 if handled, 0 for non-monster zones / empty zones. */
int SendFusionMaterialToGrave(int player, int zone)
{
    int t = player & 1;     /* must be a separate local for the ROM's evaluation order */
    struct DuelZone *z = ZONE_T(t, zone);
    u32 id = CARD(z->card).id;

    if (zone > 4)
        return 0;
    if (id != 0) {
        struct DuelCard *card = (struct DuelCard *)&ZONE(player, zone).card;
        if (CountFaceUpMonstersByNumber(0, 1107) > 0 || CountFaceUpMonstersByNumber(1, 1107) > 0) {
            DuelCmd_Push(player ? 0x807A : 0x7A, zone, 1, 0);
            DestroyLinkedCards(player, zone, 1);
            return;
        }
        DuelCmd_Push(player ? 0x80A5 : 0xA5, zone, 0, 0);
        if (CARD_TYPE(id) <= 20) {
            u8 *pl = (u8 *)&PLAYER(player);
            pl[0xB] |= 8;
        }
        LoseLpOnSendToGraveyard(player, 1);
        switch (CARD_NUMBER(id)) {
        case 47:
        case 573:
        case 1242:
        case 1257:
            Chain_AddPending((card->owner << 31) | 0x3C600000 | id, 0);
            /* fall through */
        case 303:
        case 310:
        case 312:
        case 313:
        case 320:
            Chain_AddPending((card->owner << 31) | 0x28600000 | id, 0);
            break;
        case 461:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20)) {
                DuelCmd_Push(player ? 0x8073 : 0x73, id, 1, 0);
                LoseLifePoints(player, 5000);
            }
            break;
        case 1116:
            if (!(z->flags7 & 0x20))
                Chain_AddPending(((card->owner & 1) << 31) | 0x3C600000 | id, 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case 729:
        case 1332:
            QueueRemoveLinksToZone(player, zone);
        }
        DestroyLinkedCards(player, zone, 0);
        return 1;
    }
    return 0;
}
