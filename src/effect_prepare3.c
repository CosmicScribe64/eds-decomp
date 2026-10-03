#include "global.h"

/*
 * Duel effect-condition predicates with the signature (struct CardRef *ref, ?, u16 flag),
 * each returning a bool. See wiki/functions/code-0802fb64.md.
 */

/* A card instance word as stored in the duel state. */
struct DuelCard {
    u32 id : 12;        /* card ID (index into gCardStats / gCardIdToNumber); 0 = none */
    u32 unk12 : 20;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flag6_0 : 1;         /* +0x06 bit 0 */
    u8 flag6_1 : 1;         /* +0x06 bit 1: face-up (hypothesis) */
    u8 counter6 : 4;        /* +0x06 bits 2-5: per-turn counter */
    u8 unk6_6 : 2;
    u8 unk7_0 : 5;
    u8 flag7_5 : 1;         /* +0x07 bit 5 */
    u8 unk7_6 : 2;
    u8 unk8[2];
    u16 links[32];          /* +0x0A: low byte = player, high byte = zone */
    u16 linkKinds[32];      /* +0x4A: low byte = kind of link i */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[0x94 - 0x8C];
};

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

/* The zones addressed from their own base (player + 0x28). */
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); p is player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

/* Card reference (0x14 bytes, see duel_piles / duel_stat_queries). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 (hypothesis: effect kind) */
    u16 unk4;
    u16 pos;            /* +0x06: player (low byte) | zone << 8 of a target */
    u16 unk8;           /* +0x08: high byte = a zone/count limit checked by EffectAttackResponsePrepare (hypothesis) */
    u8 fillerA[0xC - 0xA];
    u16 posC;           /* +0x0C: second target position (player | zone << 8), see CanRedirectEffectToZone */
    u16 unkE;           /* +0x0E */
    u8 fillerF[0x14 - 0x10];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Per-player duel state (0xD64 bytes, two at 0x020192E4); only the fields used here. */
struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 countB84;                    /* +0x006 */
    u8 unk7;
    u8 unk8_0 : 4;
    u8 flag8_4 : 1;                 /* +0x008 bit 4 */
    u8 flag8_5 : 1;                 /* +0x008 bit 5 */
    u8 unk8_6 : 2;
    u8 unk9_0 : 5;
    u8 flag9_5 : 1;                 /* +0x009 bit 5 */
    u8 unk9_6 : 2;
    u8 unkA;
    u8 unkB_0 : 3;
    u8 flagB_3 : 1;                 /* +0x00B bit 3 */
    u8 unkB_4 : 4;
    u8 unkC[0x28 - 0xC];
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0x904 - 0x7C4];
    struct DuelCard list904[80];    /* +0x904 (hypothesis: graveyard) */
    struct DuelCard fusionDeck[80]; /* +0xA44 (hypothesis) */
    u8 unkB84[0xD64 - 0xB84];
};
extern struct DuelPlayer gDuelPlayers[2];

/* Duel state at 0x020192E0: the two players start at +4. */
struct DuelGlobal {
    u8 unk0[4];
    struct DuelPlayer players[2];   /* +0x004 */
    u8 unk1acc[0x1B12 - 0x1ACC];
    u8 byte1B12;                    /* bit 1 side/turn player, bits 2-4 phase (hypotheses) */
};
extern struct DuelGlobal gDuel;
extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */
/* Card ID as an 11-bit field (lsl #21; lsr #21). */
#define CARD_ID11(w) (((w) << 21) >> 21)

/* Monster level as the game computes it: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }

int CountMonsters(int player);
int CountFreeMonsterZones(int player);
int CountActiveCardsOnField(int player, u16 number);
int CountFaceUpMonstersByNumber(int player, u16 number);
int CountGraveyardCardsByNumber(int player, u16 number);
int CanSpecialSummon(int player);
int IsToonMonster(u16 number);
int GetZoneCardAtk(int player, int zone);   /* ATK of the card in a zone */
int GetZoneCardDef(int player, int zone);
int FindFusionMaterials(int player, u16 id, void *buf);
int CountActiveCardsOnField2(int player, u16 number);
int CollectEffectTargets(int player, int number, int b);
int CanNormalSummon(int player);
int CanSummonFromHand(int player, u16 id);
int IsSpecialSummonOnly(u16 id);
int CountTributableMonsters(int player, int zone);
int CountMonstersFiltered(int player, int a, int b);
int CountRedirectTargets(u16 number, int player, struct CardRef *ref);
int HasFaceUpToonWorld(int player);
u16 EffectCallOfTheHauntedPrepare(struct CardRef *ref, int a, u16 flag);
u16 CanRedirectEffectToZone(u16 number, struct CardRef *ref, int a, int b);
int CountHandMonsters(int player);
int GetFaceUpFieldMagicNumber(void);
u16 CanEffectTargetZone(struct CardRef *ref, int a, int b);
int FindFreeMonsterZone(int player);
u16 EffectDarkHolePrepare(struct CardRef *ref, struct CardRef *tgt, int c);
int CountSpellTrapsFiltered(int player, int a, int b, int c);
u32 EffectTailorOfTheFickleCheck(struct CardRef *ref, u16 pos);
int IsValidEquipTarget(int a, int b, int c, int d);
int FindMonsterLinkedToCard(int a, int b);
int CountGraveyardMonsters(int player);
void DuelCmd_Push(int a, u16 b, u16 c, int d);
void DestroyFieldCard(int a, int b, int c);
void ChangeBattlePosition(int a, int b, int c, int d);
int GetZoneCardType(int player, int zone);
void QueueAddZoneLink(int player, u16 a, u16 b, int c);
void sub_08019820(int player, u16 id);
void CardListView_Open(int player, int area, int a2, int a3);
int AddDeckCardToHand(int player, u16 number);
int HalveRoundUp(int a);
void LoseLifePoints(int player, int amount);
int TributeMonster(int player, int zone);
void BanishFieldCard(int player, int zone, int c);
void EquipCard(int player, u16 pos, u16 c);
u16 EffectEquipResolve(struct CardRef *ref);
int EffectTributeTargetChainB(struct CardRef *ref);
void FormatStr(char *dst, const char *fmt, const char *arg);
void TextBoxOpen(int a, int b, int c, char *s);
void TextBoxSetMenu(int a, int b, int c);
/* Duel card-list viewer at 0x0201D810 (see card_list_viewer). */
struct ListView {
    u8 flags0;
    u8 step;
    u8 state;
    u8 unk3;
    u8 unk4;
    u8 row : 2;         /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 top;            /* +6 */
    u8 unk8[4];
    u32 cards[1];       /* +0xC */
};
extern struct ListView gCardListView;
struct Unk02015F00 {
    u8 pad[0x1B22];
    u16 savedTop;       /* +0x1B22: saved list position */
};
extern struct Unk02015F00 gAiWork;
extern char gStrNoDeckCardsToAdd[];
extern char gStrSelectDeckMonsterToAdd[];
int AiPickCardListEntry(u16 id);
void CopyDuelCard(void *dst, void *src);
int DeckReorder_Run(int player);
void DuelLink_SendDeck(int player);
/* Effect scratch state at 0x02017E20 (hypothesis: one struct; fields found through CSE'd offsets). */
struct Unk02017E20 {
    u8 counter;                 /* +0x000 */
    u8 pad1[0x15C - 1];
    u32 pad0 : 12;              /* +0x15C: four 8-bit fields starting at bit 12 */
    u32 f1 : 8;
    u32 f2 : 8;
    u32 f3 : 8;
    u32 f4 : 8;
    u32 saved[5];               /* +0x164: copy of the deck words (+0x7C4 of a player) */
};
extern struct Unk02017E20 gChainEffectWork;
extern u8 gChain[];               /* duel/effect scratch; +0x3E0 = effect step (0x7D..0x80), +0x3E5 flag */
struct Unk0201AE60 { u8 pad[0x14]; u16 v14; };
extern struct Unk0201AE60 gTextBox;
extern const char gStrTributeToReturnToDeckPrompt[];
extern const char gStrPayLpToReturnToDeckPrompt[];
extern const char gCardNames[];
extern u16 gUnk_08624052;
int CountActiveCardsOnFieldExcept(int player, u16 number, int c);


int EffectDestroyAllOnSummonPrepare(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag != 0)
        return 0;
    switch (ref->kind) {
    case 5:
    case 6:
    case 7:
        return EffectDarkHolePrepare(ref, tgt, 0);
    }
    return 0;
}
int EffectRedirectAttackPrepare(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt == 0 && ref->kind == 0x10) {
        int player = (u8)ref->unk8;
        int zone = ref->unk8 >> 8;
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) != 0 && player == ref->player && CountFaceUpMonstersByNumber(player, 0x57D) > 0)
            return 1;
    }
    return 0;
}
int EffectTributeRecoverGraveMonsterPrepare(struct CardRef *ref)
{
    if (CountActiveCardsOnField(0, 0x58A) > 0 || CountActiveCardsOnField(1, 0x58A) > 0
        || CollectEffectTargets(ref->player, 0x58D, 0) <= 0)
        return 0;
    return 1;
}
int EffectTributeTwoMonstersPrepare(struct CardRef *ref)
{
    if (CountTributableMonsters(ref->player, -1) > 1)
        return 1;
    return 0;
}
int EffectTributeRecoverGraveMagicPrepare(struct CardRef *ref)
{
    if (CountActiveCardsOnField(0, 0x58A) > 0 || CountActiveCardsOnField(1, 0x58A) > 0
        || CollectEffectTargets(ref->player, 0x59F, 0) <= 0)
        return 0;
    return 1;
}
int EffectHasTwoHandCardsPrepare(struct CardRef *ref)
{
    if (gDuelPlayers[ref->player].handCount <= 1)
        return 0;
    return 1;
}
int EffectHasHandCardPrepare(struct CardRef *ref, int a, u16 flag)
{
    if (flag != 0)
        return 0;
    if (gDuelPlayers[ref->player].handCount != 0)
        return 1;
    return 0;
}
int EffectTwoSpellTrapsOnFieldPrepare(void)
{
    int n = CountSpellTrapsFiltered(0, 0, 0, 1);
    n += CountSpellTrapsFiltered(1, 0, 0, 1);
    return n > 1;
}
int EffectTributeNegateMagicPrepare(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt != 0 && CARD_TYPE(tgt->id) == 0x16 && CountActiveCardsOnField(0, 0x58A) <= 0
        && CountActiveCardsOnField(1, 0x58A) <= 0 && CARD_NUMBER(tgt->id) != 0x603)
        return 1;
    return 0;
}
int EffectMoveEquipPrepare(struct CardRef *ref, int a, u16 flag)
{
    int i, z, j, k;

    if (flag != 0)
        return 0;
    for (i = 0; i <= 1; i++) {
        int p;

        for (z = 0; z <= 4; z++) {
            struct DuelZone *zn;

            p = i & 1;
            zn = ZB(p, z);

            if (CARD_ID(CARD_WORD(zn->card)) != 0 && (ZFLAGS(zn) & 2)) {
                for (j = 0; j <= 1; j++) {
                    for (k = 5; k <= 9; k++) {
                        u8 ok = EffectTailorOfTheFickleCheck(ref, (u8)j | ((u8)k << 8)) != 0;

                        if (IsValidEquipTarget(j, k, i, z) == 0)
                            ok = 0;
                        if (FindMonsterLinkedToCard(j, k) == (u16)((u8)i | ((u8)z << 8)))
                            ok = 0;
                        if (ok)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int EffectSwitchAttackerPrepare(struct CardRef *ref)
{
    if (ref->kind != 0x10)
        return 0;
    if (((u8 *)ref)[6] == ref->player)
        return 0;
    if (CountMonstersFiltered(1 - ref->player, 1, 0) <= 1)
        return 0;
    return 1;
}
int EffectBanishGraveToDestroyPrepare(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH: a 64-bit temp stops agbcc from hoisting the stats-table address out of the loop */
    int n;
    int i, j;
    int lvl;

    if (flag != 0 || tgt != 0)
        return 0;
    if (CountActiveCardsOnField(1 - ref->player, 0x5E7) > 0)
        return 0;
    n = CountGraveyardMonsters(ref->player);
    if (n == 0)
        return 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);
            u16 id = CARD_ID(CARD_WORD(z->card));

            if ((ZFLAGS(z) & 2) && id != 0) {
                u32 stats = ((const u32 *)(u32)base)[id & 0x7FF];

                switch ((int)((stats & 0x1F00000) >> 20)) {
                case 0x15:
                case 0x16:
                case 0x17:
                    lvl = 0;
                    break;
                case 0x18:
                    lvl = 10;
                    break;
                default:
                    lvl = (CARD_STATS(id) & 0x1E000000) >> 25;
                    break;
                }
                if (lvl <= n)
                    return 1;
            }
        }
    }
    return 0;
}
int EffectRepelledAttackerPrepare(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && ref->kind == 0xE) {
        if ((ref->unk8 & 0xF) != ref->player) {
            u16 hi = ref->unk8 >> 8;
            int q = hi & 0xF;
            int zone = hi >> 4;
            int p = q & 1;
            struct DuelZone *z = ZB(p, zone);

            return z->flag6_0;
        }
    }
    return 0;
}
int EffectReturnBanishedToGravePrepare(struct CardRef *ref)
{
    if (CountFaceUpMonstersByNumber(0, 0x453) <= 0 && CountFaceUpMonstersByNumber(1, 0x453) <= 0 && CollectEffectTargets(ref->player, 0x60D, 0) > 4)
        return 1;
    return 0;
}
void DestroyFieldCardByEffect(int player, int zone)
{
    int p = 1 & player;
    struct DuelZone *first;
    struct DuelZone *z;
    u16 id;

    first = ZB(p, zone);
    /* FAKEMATCH: preserve the first address in r0 without an index copy. */
    __asm__("" : : "r"(first));
    id = CARD_ID(CARD_WORD(first->card));

    if (id != 0) {
        switch (CARD_NUMBER(id)) {
        case 0x2F:
        case 0x23D:
        case 0x4D9:
        case 0x4E9:
            DuelCmd_Push(player ? 0x8090 : 0x90, zone, 1, 0);
            p = 1 & player;
            /* FAKEMATCH: keep the recomputed player mask in r2. */
            __asm__("" : : "r"(p));
            z = ZB(p, zone);
            ((u8 *)z)[1] |= 0x40;
        }
        DestroyFieldCard(player, zone, 1);
    }
}
int EffectDragonPiperResolve(struct CardRef *ref)
{
    int i, j;

    if (((u8 *)ref)[4] & 4)
        return 0;
    for (i = 0; i <= 1; i++) {
        for (j = 5; j <= 9; j++) {
            struct DuelZone *z = ZB(i & 1, j);
            int id = CARD_ID(CARD_WORD(z->card));

            if (id > 0 && (ZFLAGS(z) & 2) && CARD_NUMBER(id) == 0x148)
                DestroyFieldCard(i, j, 1);
        }
    }
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && (ZFLAGS(z) & 3) == 3 && GetZoneCardType(i, j) == 1)
                ChangeBattlePosition(i, j, 0, 0);
        }
    }
    return 0;
}
int EffectSanganResolve(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    switch (gChain[0x3E0]) {
    case 0x80:
        if (CollectEffectTargets(ref->player, CARD_NUMBER(ref->id), 0) == 0) {
            if (((u8 *)ref)[2] & 1)
                return 0;
            TextBoxOpen(0x205, 0x914, 0xB, gStrNoDeckCardsToAdd);
            return 0x64;
        }
        if (((u8 *)ref)[2] & 1) {
            AiPickCardListEntry(ref->id);
            gCardListView.row = 0;
            gCardListView.top = gAiWork.savedTop;
            return 0x7E;
        }
        TextBoxOpen(0x205, 0x914, 0xB, gStrSelectDeckMonsterToAdd);
        return 0x7F;
    case 0x7F:
        CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
        return 0x7E;
    case 0x7E:
        sub_08019820(ref->player, CARD_ID(gCardListView.cards[gCardListView.top + gCardListView.row]));
        if (AddDeckCardToHand(ref->player, ((const u16 *)0x08622AB4)[CARD_ID11(gCardListView.cards[gCardListView.top + gCardListView.row])]) != 0)
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
        return 0x64;
    }
    return 0;
}
int EffectCastleOfDarkIllusionsResolve(struct CardRef *ref)
{
    int i, j;

    if (((u8 *)ref)[4] & 4)
        return 0;
    DuelCmd_Push((((u8 *)ref)[2] & 1) ? 0x8092 : 0x92, ref->zone, 0, 0);
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && (ZFLAGS(z) & 2) && GetZoneCardType(i, j) == 2)
                QueueAddZoneLink(ref->player, ref->player | (ref->zone << 8), (u8)i | ((u8)j << 8), 2);
        }
    }
    return 0;
}
int EffectReaperOfTheCardsResolve(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id == 0)
            return 0;
        if (ZFLAGS(z) & 2) {
            if (CARD_TYPE(id) == 0x15)
                DestroyFieldCard(player, zone, 1);
            return 0;
        }
        DuelCmd_Push(player ? 0x807F : 0x7F, zone, 0, 0);
        sub_08019820(player, id);
        if (CARD_TYPE(id) == 0x15)
            DestroyFieldCard(player, zone, 1);
        else
            DuelCmd_Push(player ? 0x807F : 0x7F, zone, 0, 0);
    }
    return 0;
}
int EffectCatapultTurtleResolve(struct CardRef *ref)
{
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        int amount;

        if (CARD_ID(CARD_WORD(z->card)) == 0)
            return 0;
        amount = 0;
        switch (CARD_NUMBER(ref->id)) {
        case 0x58:
            amount = HalveRoundUp(GetZoneCardAtk(player, zone));
            break;
        case 0x1FF:
            amount = 500;
            break;
        }
        if (TributeMonster(player, zone) != 0)
            LoseLifePoints(1 - ref->player, amount);
    }
    return 0;
}
int EffectMaskOfDarknessResolve(struct CardRef *ref)
{
    u32 id;

    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 2)
        return 0;
    if (CountGraveyardCardsByNumber(ref->player, CARD_NUMBER(id = (u32)(ref->posC << 20) >> 20)) > 0) {
        sub_08019820(ref->player, id);
        {
            int player = ((u8 *)ref)[2] & 1;
            register int snd __asm__("r3") = 0xD2;

            if (player)
                snd = 0x80D2;
            {
                u16 pos = ref->posC;
                int zero = 0;

                /* FAKEMATCH: prepare r1/r2 before copying the sound id to r0. */
                __asm__("" : : "r"(pos), "r"(zero) : "r0");
                DuelCmd_Push(snd, pos, zero, 0);
            }
        }
    }
    return 0;
}
int EffectTaintedWisdomResolve(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    DuelCmd_Push((((u8 *)ref)[2] & 1) ? 0x8060 : 0x60, 0, 0, 0);
    return 0;
}
struct Unk02017FB0_30620 {
    u8 filler0[0x304];
    u32 unk304:8;
    u32 dirtyHand:1;
    u32 dirtyDeck:1;
    u32 unk305_2:22;
};
extern struct Unk02017FB0_30620 gLinkState;
extern u8 gDuelCtrl[];
/* The effect scratch at 0x02017A40 viewed as a struct. */
struct Effect30620 {
    u8 pad0[0x3E0];
    u8 step;                    /* +0x3E0 */
    u8 pad3E1[0x53C - 0x3E1];
    u32 pad0_ : 12;             /* +0x53C */
    u32 f1 : 8;
    u32 f2 : 8;
    u32 f3 : 8;
    u32 f4 : 8;
    u32 saved[5];               /* +0x544 */
};
#define EFF30620 (*(struct Effect30620 *)gChain)
int EffectBigEyeResolve(struct CardRef *ref)
{
    int i;

    switch (gChain[0x3E0]) {
    case 0x80:
        if (gDuelPlayers[ref->player].deckCount <= 4)
            return 0;
        for (i = 0; i <= 4; i++)
            CopyDuelCard(&EFF30620.saved[i], (u8 *)gDuelPlayers[ref->player].unk7C4 + i * 4);
        EFF30620.f1 = 0;
        EFF30620.f2 = 0;
        EFF30620.f3 = 0;
        EFF30620.f4 = 0;
        EFF30620.step -= 1;
    case 0x7F:
        if (DeckReorder_Run(ref->player) != 0)
            return 0x7E;
        return 0x7F;
    case 0x7E:
        for (i = 0; i <= 4; i++)
            CopyDuelCard((u8 *)gDuelPlayers[ref->player & 1].unk7C4 + i * 4, &EFF30620.saved[i]);
        if (1 & gDuelCtrl[1]) {
            DuelLink_SendDeck(ref->player);
        r7d:
            return 0x7D;
        }
        return 0;
    case 0x7D:
        if (!gLinkState.dirtyDeck)
            goto r7d;
        return 0;
    }
    return 0;
}
int EffectPenguinKnightResolve(struct CardRef *ref)
{
    DuelCmd_Push((((u8 *)ref)[2] & 1) ? 0x80D6 : 0xD6, 1, 0, 0);
    DuelCmd_Push((((u8 *)ref)[2] & 1) ? 0x8060 : 0x60, 1, 0, 0);
    return 0;
}
int EffectDimensionalWarriorResolve(struct CardRef *ref)
{
    int p1 = (u8)ref->pos;
    int z1 = ref->pos >> 8;
    int p2 = (u8)ref->unk8;
    int z2 = ref->unk8 >> 8;

    BanishFieldCard(p1, z1, 0);
    BanishFieldCard(p2, z2, 0);
    return 0;
}
int EffectTheLittleSwordsmanOfAileResolve(struct CardRef *ref)
{
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) == 0)
            return 0;
        if (TributeMonster(player, zone) != 0)
            QueueAddZoneLink(ref->player, ref->id, ref->player | (ref->zone << 8), 3);
    }
    return 0;
}
int EffectPrincessOfTsurugiResolve(struct CardRef *ref)
{
    int n = CountSpellTrapsFiltered(1 - ref->player, 0, 0, 0);

    if (!(((u8 *)ref)[4] & 4) && n > 0)
        LoseLifePoints(1 - ref->player, n * 500);
    return 0;
}
u16 EffectEquipResolve(struct CardRef *ref)
{
    int p = 1 & ref->player;
    struct DuelZone *z = ZB(p, ref->zone);

    if (CARD_ID(CARD_WORD(z->card)) != 0) {
        int step = ((u8 *)ref)[0xA] & 7;

        if (step == 1) {
            EquipCard(ref->player, ref->player | (ref->zone << 8), ref->posC);
        } else if (CARD_NUMBER(ref->id) == 0x3C2 && step == 2) {
            EquipCard(ref->player, ref->player | (ref->zone << 8), ref->posC);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8087 : 0x87, ref->zone, ref->unkE, 0);
        }
    }
    return 0;
}
u16 EffectAxeOfDespairResolve(struct CardRef *ref)
{
    char buf[0x100];

    if ((((u8 *)ref)[2] & 0xE) != 6)
        return EffectEquipResolve(ref);
    switch (gChain[0x3E0]) {
    case 0x80:
        if (((u8 *)ref)[2] & 1)
            return 0;
        if (CountTributableMonsters(ref->player, -1) == 0)
            return 0x64;
        FormatStr(buf, gStrTributeToReturnToDeckPrompt, gCardNames + (gUnk_08624052 << 6));
        TextBoxOpen(0x205, 0x914, 0xB, buf);
        TextBoxSetMenu(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gTextBox.v14 == 0)
            return 0x64;
    {
        int offset = 0x3E5;

        gChain[offset] = 0;
        /* FAKEMATCH: retain the initialized offset through the store so
         * the zero value uses r0 and the offset uses r2, as in the ROM. */
        __asm__("" : : "r"(offset));
    }
    s7e:
        return 0x7E;
    case 0x7E:
        if (EffectTributeTargetChainB(ref) == 0)
            goto s7e;
        return 0x7D;
    case 0x7D:
    {
        int player = (u8)ref->posC;
        u16 pos = ref->posC;
        int zone;

        zone = pos >> 8;
        /* FAKEMATCH: preserve the initialized position in r3 and reserve
         * r2 through argument extraction, reproducing the ROM registers. */
        __asm__("" : : "r"(pos) : "r2");

        if (TributeMonster(player, zone) != 0)
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D0 : 0xD0, ref->id, 0, 0);
        return 0x64;
    }
    }
    return 0;
}
u16 EffectBlackPendantResolve(struct CardRef *ref)
{
    u8 b2 = ((u8 *)ref)[2];
    /* Reserve the original scratch register without emitting code. */
    __asm__("" : : : "r1");
    if ((b2 & 0xE) != 6)
        return EffectEquipResolve(ref);
    if (!(((u8 *)ref)[4] & 4))
        LoseLifePoints(1 - ref->player, 500);
    return 0;
}
static inline u32 PlayerBitFromModeByte(u8 byte)
{
    register u32 shifted __asm__("r0") = (u32)byte << 31;

    /* FAKEMATCH: retain the ROM's lsl/lsr extraction rather than an and. */
    __asm__("" : : "r"(shifted));
    return shifted >> 31;
}

u16 EffectHornOfLightResolve(struct CardRef *ref)
{
    char buf[0x100];
    register u8 b2 __asm__("r3") = ((u8 *)ref)[2];

    if ((b2 & 0xE) != 6)
        return EffectEquipResolve(ref);
    switch (gChain[0x3E0]) {
    case 0x80:
        if (gDuelPlayers[PlayerBitFromModeByte(b2)].lifePoints <= 0x1F3)
            return 0;
        FormatStr(buf, gStrPayLpToReturnToDeckPrompt, gCardNames + (ref->id << 6));
        TextBoxOpen(0x205, 0x914, 0xB, buf);
        TextBoxSetMenu(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gTextBox.v14 != 0) {
            DuelCmd_Push((1 & b2) ? 0x8043 : 0x43, 500, 1, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D0 : 0xD0, ref->id, 0, 0);
        }
        return 0x64;
    }
    return 0;
}
u16 EffectHornOfTheUnicornResolve(struct CardRef *ref)
{
    u8 b2 = ((u8 *)ref)[2];

    __asm__("" : : : "r1");
    if ((b2 & 0xE) != 6)
        return EffectEquipResolve(ref);
    {
        register int player __asm__("r0") = b2 & 1;
        register int msg __asm__("r3") = 0xD0;

        if (player)
            msg = 0x80D0;
        DuelCmd_Push(msg, ref->id, 0, 0);
    }
    return 0;
}
