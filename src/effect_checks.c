#include "global.h"
#include "duel.h"
#include "main.h"

/*
 * Duel field-target checks (can a card in (player, zone) be selected / affected?)
 * (continued from card_list_viewer).  See wiki/functions/code-0802bad0.md.
 *
 * Uses the shared duel.h layouts (DuelCard/DuelZone/DuelZonesPlayer/DuelPlayer).
 */

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); p is player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

/* Card reference (0x14 bytes, see duel_piles / duel_stat_queries). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 unk2_10 : 6;
    u16 unk4;
    u16 unk6;
    u8 filler8[0x14 - 0x8];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

int GetZoneCardType(int player, int zone);
u16 CanCardTargetZone(u16 id, int player, int zone);
int CountActiveCardsOnField(int player, u16 number);
int CollectEffectTargets(int player, int a, int b);
int IsCardLinkedToMonster(int player, int zone);
int CountValidEquipTargets(int player, int zone);
int CountZoneEquips(int player, int zone, int a, int b);
u16 IsZoneTargetable(int player, int zone);

/* Level-like value (hypothesis) into v: 0 for types 21-23, 10 for type 24, else stat bits 25-28. */
#define CARD_LEVEL(v, id)                                  \
    switch ((int)CARD_TYPE(id)) {                          \
    case 21:                                               \
    case 22:                                               \
    case 23:                                               \
        v = 0;                                             \
        break;                                             \
    case 24:                                               \
        v = 10;                                            \
        break;                                             \
    default:                                               \
        v = (CARD_STATS(id) & 0x1E000000) >> 25;           \
        break;                                             \
    }

void TributeMonster(int player, int zone);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
int HalveRoundDown(u16 v);
void TextBoxOpen(u16 a, u16 b, int c, const char *text);
int DuelCursor_PickTarget(u32 mask);
void PlaySE(u16 id);
void DuelPrompt_PostDiscardCost(int player, int a, int b, int c);

/* Output of GetZoneCardStats (0xC bytes): card id, type | attribute << 5, and a value at +4. */
struct CardInfo {
    u16 id;
    u8 typeAttr;        /* +0x02: bits 5-7 = attribute */
    u8 unk3;
    int value;          /* +0x04 */
    int unk8;
};
void GetZoneCardStats(int player, int index, struct CardInfo *out);

/* gMain (0x03000040) comes from main.h. */
extern u8 gChain[];
extern u8 gDuelScreen[];
extern const char gStrCrushCardTributePrompt[];
extern const char gStrSelectTributeMonster[];

int EffectDragonSeekerCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    u16 a = GetZoneCardType(player, zone);
    int p;
    struct DuelZone *z;

    if (zone > 4)
        return 0;
    p = player & 1;
    z = ZB(p, zone);
    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2) || !CanCardTargetZone(ref->id, player, zone))
        return 0;
    return a == 1;
}

int EffectTargetableMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int ret;
    int p;
    struct DuelZone *z;

    if (zone > 4 || !CanCardTargetZone(ref->id, player, zone))
        return 0;
    ret = 0;
    p = player & 1;
    z = ZB(p, zone);
    if (CARD_WORD(z->card) << 20)
        ret = zone = 1; /* FAKEMATCH (found by decomp-permuter): reusing `zone` keeps ret in r3 */
    return ret;
}

int EffectPatrolRoboCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (player != ref->player) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2))
            return 1;
    }
    return 0;
}

int EffectDestroyByTypeCheck(struct CardRef *ref, u16 pos)
{
    u16 refId = ref->id;
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZonesPlayer *pz = &gDuelZones[player & 1];
    struct DuelZone *z = &pz->zones[zone];
    u16 id = CARD_ID(CARD_WORD(z->card));
    u16 a = GetZoneCardType(player, zone);
    int p;

    p = player & 1;
    if (zone > 4)
        return 0;
    /* The zone address is recomputed with the player term first (the ROM's add order). */
    if (CARD_ID(CARD_WORD(((struct DuelZone *)(p * 0xD64 + zone * 0x94 + (u32)gDuelZones))->card))
        && CanCardTargetZone(ref->id, player, zone) && IsZoneTargetable(player, zone) && (ZFLAGS(z) & 2) && id
        && refId) {
        switch (CARD_NUMBER(refId)) {
        case 0x28C:
            if (a == 0xF)
                return 1;
            break;
        case 0x28F:
            if (CountZoneEquips(player, zone, 0, 0) > 0)
                return 1;
            break;
        case 0x293:
            if (a == 7)
                return 1;
            break;
        case 0x295:
            if (a == 0xA)
                return 1;
            break;
        case 0x296:
            if (a == 6)
                return 1;
            break;
        case 0x297:
            if (a == 8)
                return 1;
            break;
        case 0x40C:
            if (a == 0x12)
                return 1;
            break;
        case 0x40D:
            if (a == 3)
                return 1;
            break;
        }
    }
    return 0;
}
int EffectAcidTrapHoleCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (CountActiveCardsOnField(0, 0x58A) <= 0 && CountActiveCardsOnField(1, 0x58A) <= 0 && zone <= 4) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2) && (ZFLAGS(z) & 1))
            return 1;
    }
    return 0;
}

int EffectTargetableFaceUpMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= 4) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && CanCardTargetZone(ref->id, player, zone))
            return (ZFLAGS(z) >> 1) & one;
    }
    return 0;
}

int EffectDarknessApproachesCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int one = 1;
    int p = player & one;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    u16 copy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r1 so `0x7FF & id` masks into r1 */

    copy = id;
    if (zone <= 4 && id != 0 && (u16)(CARD_NUMBER(copy) - 0x780) > 0x4F && CanCardTargetZone(ref->id, player, zone))
        return (ZFLAGS(z) >> 1) & one;
    return 0;
}

int EffectMagicArmShieldCheck(struct CardRef *ref, u16 pos)
{
    register u16 normalized __asm__("r0") = pos;
    int player;
    int zone;
    u16 copy;

    /* FAKEMATCH: narrow the initialized position in r0 before retaining
     * its copy in r7, preserving the ROM's separate narrowing/copy. */
    __asm__("" : : "r"(normalized));
    copy = normalized;
    player = (u8)copy;
    zone = pos >> 8;

    if (zone <= 4 && ref->player != player) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && CanCardTargetZone(ref->id, player, zone) && copy != ref->unk6)
            return (ZFLAGS(z) >> 1) & one;
    }
    return 0;
}
int EffectOpponentFaceUpMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= 4 && ref->player != player) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)))
            return (ZFLAGS(z) >> 1) & one;
    }
    return 0;
}
int EffectRemoveTrapCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    u16 copy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r2 so `0x7FF & id` masks into r0 */
    int t;

    copy = id;
    if (id && (u32)(zone - 5) <= 5 && (ZFLAGS(z) & 2)) {
        t = CARD_TYPE(copy);
        return t == 21;
    }
    return 0;
}
int EffectBlockAttackCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= 4 && player != ref->player) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && CanCardTargetZone(ref->id, player, zone))
            return one & ~ZFLAGS(z);
    }
    return 0;
}
int EffectSnatchStealCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= 4 && ref->player != player) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && CanCardTargetZone(ref->id, player, zone)
            && CARD_NUMBER(CARD_ID(CARD_WORD(z->card))) != 0x547)
            return (ZFLAGS(z) >> 1) & one;
    }
    return 0;
}
int EffectTailorOfTheFickleCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone > 4) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (ZFLAGS(z) & 2) {
            int id = CARD_ID(CARD_WORD(z->card));

            if (id) {
                switch (CARD_NUMBER(id)) {
                default:
                    return 0;
                case 0x12C ... 0x13C:
                case 0x13E:
                case 0x140 ... 0x147:
                case 0x28A:
                case 0x28B:
                case 0x28D:
                case 0x290:
                case 0x291:
                case 0x29B:
                case 0x3C2:
                case 0x3F4:
                case 0x3F5:
                case 0x412:
                case 0x416:
                case 0x417:
                case 0x422:
                case 0x424:
                case 0x49E:
                case 0x58B:
                case 0x58C:
                case 0x58E:
                case 0x5A8 ... 0x5AA:
                case 0x604:
                case 0x60C:
                case 0x60E:
                {
                    int p2 = player & 1;
                    struct DuelZone *z2 = ZB(p2, zone);
                    u32 stats = CARD_STATS(CARD_ID(CARD_WORD(z2->card)));
                    int t = (stats & 0x1F00000) >> 20;
                    int v;

                    switch (t) {
                    case 21:
                    case 22:
                        v = (stats & 0xE0000) >> 17;
                        break;
                    default:
                        v = 0;
                        break;
                    }
                    if (v == 3 && IsCardLinkedToMonster(player, zone) && CountValidEquipTargets(player, zone) > 1)
                        return 1;
                    break;
                }
                }
            }
        }
    }
    return 0;
}


int EffectDustTornadoCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (ref->player != player && (u32)(zone - 5) <= 5) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)))
            return 1;
    }
    return 0;
}
int EffectAnySpellTrapCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone >= 5 && zone <= 10) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)))
            return 1;
    }
    return 0;
}
int EffectNoblemanOfCrossoutCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone <= 4) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)))
            return one & ~(ZFLAGS(z) >> 1);
    }
    return 0;
}
int EffectNoblemanOfExterminationCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone >= 5 && zone <= 9) {
        int one = 1;
        int p = player & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)))
            return one & ~(ZFLAGS(z) >> 1);
    }
    return 0;
}
int EffectOwnSkullOrThunderCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    u16 copy; /* FAKEMATCH (found by decomp-permuter): the extra copy keeps the id live in r4 so `0x7FF & id` masks into r4 */

    copy = id;
    if (player == ref->player && zone <= 4 && (ZFLAGS(z) & 2) && id
        && CanCardTargetZone(ref->id, player, zone)
        && (CARD_NUMBER(copy) == 0x15 || GetZoneCardType(player, zone) == 0x13))
        return 1;
    return 0;
}
int EffectTributeForInsectCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));

    if (player == ref->player && zone <= 4 && id
        && CountActiveCardsOnField(0, 0x58A) <= 0 && CountActiveCardsOnField(1, 0x58A) <= 0) {
        int level;

        CARD_LEVEL(level, id);
        /* FAKEMATCH: retain the initialized result through the shared test
         * instead of jump-threading constant switch arms. No instructions. */
        __asm__("" : "+r"(level));
        if (level != 0) {
            int pl = ref->player;
            int level2;

            CARD_LEVEL(level2, id);
            {
                int count = level2 + 1;
                /* FAKEMATCH: the initialized r0 copy keeps argument 0
                 * before the constant argument 1 in the original call. */
                register int arg0 __asm__("r0") = pl;
                if (CollectEffectTargets(arg0, 0x526, count) > 0)
                    return 1;
            }
        }
    }
    return 0;
}



int EffectTargetableOwnMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));

    if (CanCardTargetZone(ref->id, player, zone) && player == ref->player && zone <= 4 && id)
        return 1;
    return 0;
}
int EffectFaceUpMagicCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if ((u32)(zone - 5) <= 5) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);
        int id = CARD_ID(CARD_WORD(z->card));

        if (id && (ZFLAGS(z) & 2) && CARD_TYPE(id) == 22)
            return 1;
    }
    return 0;
}
int EffectOpponentSpellTrapCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if (zone > 4) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && ref->player != player)
            return 1;
    }
    return 0;
}
int EffectSpecialSummonedMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    u32 w = CARD_WORD(z->card);
    int id = CARD_ID(w);

    if (zone <= 4 && id && (ZFLAGS(z) & 2))
        return (w & 0x10000) >> 16;
    return 0;
}
int EffectFaceDownSpellTrapCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;

    if ((u32)(zone - 5) <= 4) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2))
            return 1;
    }
    return 0;
}
int EffectFaceUpFusionMonsterCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    int v;

    if (zone <= 4 && id && (ZFLAGS(z) & 2) && CanCardTargetZone(ref->id, player, zone)) {
        int number;

        if (CARD_TYPE(id) > 20)
            return 0;
        number = CARD_NUMBER(id);
        switch (number) {
        case 0x776:
            v = 3;
            break;
        case 0x777:
        case 0x778:
            v = 1;
            break;
        default:
            switch ((int)CARD_TYPE(id)) {
            case 22:
                v = 7;
                break;
            case 21:
                v = 8;
                break;
            case 23:
                v = 9;
                break;
            default:
                v = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        if (v == 2)
            return 1;
    }
    return 0;
}
int EffectPayLifePointsChainA(struct CardRef *ref)
{
    int v = 0;

    switch (CARD_NUMBER(ref->id)) {
    case 0x3F9:
    case 0x489:
        v = 500;
        break;
    case 0x488:
        v = 800;
        break;
    case 0x191:
    case 0x406:
    case 0x42E:
    case 0x42F:
    case 0x49F:
        v = 1000;
        break;
    case 0x1F9:
        v = 3000;
        break;
    case 0x1A3:
        v = 5000;
        break;
    case 0x404:
        v = HalveRoundDown(gDuelPlayers[ref->player].lifePoints);
        break;
    }
    if (v > 0)
        DuelCmd_Push(ref->player ? 0x8043 : 0x43, v, 1, 0);
    return 1;
}
int EffectTributeSelfChainA(struct CardRef *ref, u16 pos)
{
    TributeMonster(ref->player, ref->zone);
    return 1;
}
int EffectJigenBakudanChainA(struct CardRef *ref, u16 pos)
{
    if (ref->unk2_10 == 2)
        TributeMonster(ref->player, ref->zone);
    return 1;
}
int EffectCrushCardChainA(struct CardRef *ref)
{
    struct CardInfo info;
    u8 *base = gChain;
    u8 *step = base + 0x3E4;

    switch (*step) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrCrushCardTributePrompt);
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(0xF0)) {
            int pl = ref->player;
            u8 *cbase = gDuelScreen;
            int *cursor = (int *)(cbase + 0x82C);

            GetZoneCardStats(pl, *cursor, &info);
            if (info.value > 1000 || (info.typeAttr & 0xE0) != 0x40) {
                PlaySE(3);
            } else {
                TributeMonster(ref->player, *cursor);
                return 1;
            }
        }
        if (gMain.newKeys & 2)
            gChain[0x3E4]--;
        return 0;
    }
    return 0;
}
int EffectToonWorldChainA(struct CardRef *ref, u16 pos)
{
    DuelCmd_Push(ref->player ? 0x8043 : 0x43, 1000, 1, 0);
    DuelCmd_Push(ref->player ? 0x80B3 : 0xB3, ref->zone, 0, 0);
    return 1;
}
int EffectDiscardCostChainA(struct CardRef *ref)
{
    int v = 0;

    switch (CARD_NUMBER(ref->id)) {
    case 0x405:
    case 0x3FF:
        v = 1;
        break;
    case 0x430:
        v = 2;
        break;
    case 0x42B:
        v = 5;
        break;
    }
    if (v > 0)
        DuelPrompt_PostDiscardCost(ref->player, v, 0, 0);
    return 1;
}
int EffectTributeChosenMonsterChainA(struct CardRef *ref)
{
    u8 *base = gChain;
    u8 *step = base + 0x3E4;

    switch (*step) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectTributeMonster);
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(0xF0)) {
            u8 *cbase = gDuelScreen;

            TributeMonster(*(int *)(cbase + 0x824), *(int *)(cbase + 0x82C));
            return 1;
        }
        return 0;
    }
    return 0;
}
int EffectPayHalfLifePointsChainA(struct CardRef *ref)
{
    DuelCmd_Push(ref->player ? 0x8043 : 0x43, HalveRoundDown(gDuelPlayers[ref->player & 1].lifePoints), 1, 0);
    return 1;
}
