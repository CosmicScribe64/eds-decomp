#include "global.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step state (0x7E or 0x7F). See wiki/functions/code-0803c838.md.
 */

struct DuelCard {
    u32 id : 12;
    u32 unk12 : 20;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flag6_0 : 1;
    u8 flag6_1 : 1;
    u8 counter6 : 4;
    u8 unk6_6 : 2;
    u8 unk7[0x94 - 7];
};
#define ZFLAGS(z) (((u8 *)(z))[6])
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
/* Same address, but with the player term first (the ROM adds the terms in this order in some places). */
#define ZBP(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already handled/skipped (hypothesis) */
    u8 flag4_3 : 1;     /* +0x04 bit 3 */
    u8 unk4_4 : 4;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[2];     /* +0x0C: low byte player, high byte zone */
};

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern u8 gChain[];
#define EFF_PHASE gChain[0x3E0]  /* 0x7D-0x80: step of a multi-step effect (hypothesis) */
#define EFF_SIDE gChain[0x3E1]
extern u8 gDuel[];   /* duel global state, byte view */

void DuelPrompt_Post(int a, int b, int c, int d);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void DestroyFieldCardByEffect(int player, int zone);
void OnCardDestroyedByEffect(int player, int a, int b);
int Random(void);
int CollectEffectTargets(int player, int number, int b);
void QueueSpecialSummonChoosePosition(int player, u32 *card, int a, int b);
extern u16 gCardListViewCards[];   /* = gCardListView.cards[0] (list viewer, see card_list_viewer) */
void ShowCardDetail(int player, int id);
int __modsi3(int a, int b);
void QueueAddZoneLink(int player, int a, u16 b, u16 c);
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gTextBox;
extern char gStrCoinTossSelection[];
void TextBoxOpen(int a, int b, int c, char *s);
void TextBoxSetMenu(int a, int b, int c);
int EffectCanTributeOpponentMonsterPrepare(struct CardRef *ref, int a, int b);
u32 DuelCursor_PickTarget(u32 keys);
int IsSpecialSummonOnly(u16 id);
int FindFreeMonsterZone(int player);
int CountTributableMonsters(int player, int exclude);
int IsTributableMonster(int player, int zone);
void PlaySE(u16 id);
void TributeMonster(int player, int zone);
void QueueNormalSummonChoosePosition(int player, int a, int b, int c);
void MemCopy16(void *dest, const void *src, u32 size);
int EffectCatapultTurtleResolve(void *ref, int a);
int EffectTheLittleSwordsmanOfAileResolve(void *ref, int a);
extern const u32 gCardStats[];
extern const u16 gCardIdToNumber[];
extern char gStrPromptTributeUse[];
extern char gStrSelectHighLevelMonster[];
extern char gStrSelectSecondTribute[];
extern char gStrSelectEffectMonster[];
/* 0x020192E4: per-player duel state, only the parts used here (stride 0xD64). */
struct PlayerState {
    u16 lifePoints;
    u8 handCount;       /* +2 */
    u8 deckCount;       /* +3 */
    u8 pad4[3];
    u8 unk7_0 : 3;
    u8 flag7_3 : 1;     /* +7 bit 3 */
    u8 unk7_4 : 4;
    u8 unk8;
    u8 pad9[3];
    u8 flagsC;          /* +0xC */
    u8 padD[0xD64 - 0xD];
};
extern struct PlayerState gDuelPlayers[2];
struct HandRow {        /* 0x02019968 = 0x020192E4 + 0x684: hand card words */
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow gDuelHands[2];
/* Duel screen state at 0x0201CFB0: +0x824.. hold the selection */
struct DuelScreen {
    u8 pad0[0x824];
    u32 w824;           /* low half: player-ish selection (hypothesis) */
    u32 w828;           /* low byte: column */
    u32 idx82C;         /* +0x82C: selected index (low byte: row) */
};
extern struct CardRef gChainProxyLink;
extern struct DuelScreen gDuelScreen;

/* Monster level: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
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
void MoveFieldCard(int player, u16 a, u16 b);

void EquipCard(int player, u16 a, u16 b);
int EffectSpecialSummonedMonsterCheck(struct CardRef *ref, int a);
void ChangeBattlePosition(int a, int b, int c, int d);
void ReturnFieldCardToHand();
/* Card-list viewer at 0x0201D810 (see card_list_viewer struct ListView). */
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
};
extern struct ListView gCardListView;
extern char gStrBanishOpponentGraveMonsterQuestion[];
extern char gStrBanishAnotherOpponentGraveMonsterQuestion[];
extern char gStrSelectOpponentGraveMonsterToBanish[];
void CardListView_Open(int player, int area, int a2, int a3);
extern u8 gSummonAction;    /* bits 1-5: a zone position (hypothesis) */
int CountActiveCardsOnField(int player, u16 number);
extern char gStrSelectFusionMaterialToAddToHand[];
int EffectTailorOfTheFickleCheck(struct CardRef *ref, u16 pos);
int IsValidEquipTarget(int a, int b, int c, int d);
void MoveEquipCard(u16 a, u16 b);
void DestroyFieldCard();
int EffectFaceUpFusionMonsterCheck(struct CardRef *ref, u16 pos);
extern char gStrSummonFusionMaterialsQuestion[];
#define EFF_W542 (*(u16 *)&gChain[0x542])
int EffectReturnBanishedToGravePrepare(struct CardRef *ref, int a, int b);
extern char gStrSelectBanishedCardToReturnToGrave[];
extern const u16 gFusionRecipes2[];
extern const u16 gFusionRecipes3[];
u16 IsFusionSubstitute(u16 n);
u16 FindFusionMaterialSlot(int player, u16 a, u16 b, u16 c);
u16 GetFusionSlotCardId(int player, u16 pos);
int CountMonsters(int player);
int CheckFusionRecipe2(int a, int b, int c);
int CheckFusionRecipe3(u16 a, u16 b, u16 c, u16 d);
struct CardRef20 {
    struct CardRef r;
    u32 extra;
};
u16 CanActivateEffect(struct CardRef *ref, int a, int b);
void sub_080197C0(int player, int id);
void Chain_AddPending(u32 a, int b);
void ReturnFieldCardToDeck(int player, int zone);
void ShowDestroyedCard(int player, int id);
void ShowRevealedCard(int player, int id);
void FlipFieldCard();
extern char gStrBanishAnotherGraveCardQuestion[];
extern char gStrBanishGraveCardQuestion[];
extern char gStrSelectGraveCardToBanishForAtk[];
#define EFF_CNT gChain[0x3E2]
extern u8 gDuelCtrl[];
int CanSpecialSummon(int player);
int CountFreeMonsterZones(int player);
struct PosWord {
    u32 lo : 12;
    u32 flag12 : 1;     /* bit 12: player (hypothesis) */
    u32 rest : 19;
};
int CountGraveyardCardsByNumber(int player, u16 number);
int FindFreeSpellTrapZone(int player);
int FindFreeMonsterZone(int player);
int GetGraveyardCardById(int player, u16 id, u16 *out);
extern u16 gUnk_02017F84[];
extern char gStrDesignateOwnMonsterToTribute[];
extern char gStrSelectFusionToSummonForTribute[];
extern char gStrSelectOpponentMonsterToChangePosition[];



struct EffState838 {
    u8 unk0[0x3E0];
    u8 phase;       /* +0x3E0 */
    u8 side;        /* +0x3E1 */
    u8 unk3E2[0x542 - 0x3E2];
    u16 w542;       /* +0x542 */
};
#define ES838 ((struct EffState838 *)gChain)
union ViewCard838 {
    struct {
        u32 lo : 20;
        u32 flag20 : 1;
        u32 hi : 11;
    } b;
    u16 h[2];
};
struct ListView838 {
    u8 unk0[0xC];
    union ViewCard838 cards[0x80];    /* +0x0C: card words */
};
#define LV838 ((struct ListView838 *)&gCardListView)
int EffectSplitFusionResolve(struct CardRef *ref)
{
    int tp = (u8)ref->targets[0];
    int tz = ref->targets[0] >> 8;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int n = 7 & ((u8 *)ref)[0xA];
            int a, m;

            if (n != 1)
                return 0;
            if (EffectFaceUpFusionMonsterCheck(ref, ref->targets[0]) == 0)
                return 0;
            ReturnFieldCardToDeck(tp, tz);
            ES838->w542 = CARD_ID(CARD_WORD(ZBP(tp & n, tz)->card));
            a = CountFreeMonsterZones(ref->player);
            m = CollectEffectTargets(ref->player, 0x60A, ES838->w542);
            if (m == 0)
                return 0;
            if (a < m)
                return 0;
            if (n & ((u8 *)ref)[2])
                return 0;
            return 0x7F;
        }
        case 0x7F:
            TextBoxOpen(0x206, 0x613, 0xB, gStrSummonFusionMaterialsQuestion);
            TextBoxSetMenu(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gTextBox.flag14 == 0)
                return 0;
            EFF_SIDE = CollectEffectTargets(ref->player, 0x60A, ES838->w542);
            return 0x7D;
        case 0x7D:
            if (EFF_SIDE == 0)
                return 0;
            EFF_SIDE--;
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, ((union ViewCard838 *)gCardListViewCards)[EFF_SIDE].h[0], ((union ViewCard838 *)gCardListViewCards)[EFF_SIDE].h[1], 0);
            return 0x7C;
        case 0x7C:
            LV838->cards[EFF_SIDE].b.flag20 = 0;
            QueueSpecialSummonChoosePosition(ref->player, (u32 *)&LV838->cards[EFF_SIDE], 1, 0x20);
            return 0x7D;
        }
    }
    return 0;
}
int EffectReturnBanishedToGraveResolve(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (EffectReturnBanishedToGravePrepare(ref, arg, 0) == 0)
                return 0;
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (CollectEffectTargets(ref->player, 0x60D, 0) == 0)
                return 0;
            TextBoxOpen(0x206, 0x613, 0xB, gStrSelectBanishedCardToReturnToGrave);
            return 0x7E;
        case 0x7E:
            CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D: {
            u16 *cw = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.row];

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80DE : 0xDE, cw[0], cw[1], 0);
            EFF_SIDE--;
            if (EFF_SIDE != 0)
                return 0x7F;
            return 0xA;
        }
        }
    }
    return 0;
}
int EffectBanishCostFromFieldResolve(struct CardRef *ref)
{
    u8 *base = (u8 *)gDuelPlayers;
    u8 *ps = base + ref->player * 0xD64;

    ps[0xC] |= 0x20;
    return 0;
}
u16 IsFusionSubstitute(u16 n)
{
    switch (n) {
    case 0x6C:
    case 0x101:
    case 0x10C:
    case 0x281:
        return 1;
    default:
        return 0;
    }
}
int CheckFusionRecipe2(int a, int b, int c)
{
    const u16 *e = gFusionRecipes2;
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u16 z = CARD_NUMBER(c);

    if (IsFusionSubstitute(y) != 0 && IsFusionSubstitute(z) != 0)
        return 0;
again:
    if (*(u32 *)e == 0x03E703E7 && e[2] == 0x3E7)
        return 0;
    if (e[0] == x) {
        if (y == e[1] && z == e[2])
            return 1;
        if (y == e[2] && z == e[1])
            return 1;
        if (IsFusionSubstitute(y) != 0 && (z == e[1] || z == e[2]))
            return 1;
        if (IsFusionSubstitute(z) != 0 && (y == e[1] || y == e[2]))
            return 1;
    }
    e += 4;
    goto again;
}
int CheckFusionRecipe3(u16 a, u16 b, u16 c, u16 d)
{
    const u16 *e = gFusionRecipes3;
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u16 z = CARD_NUMBER(c);
    u16 w = CARD_NUMBER(d);

    if (IsFusionSubstitute(y) != 0 && IsFusionSubstitute(z) != 0)
        return 0;
    if (IsFusionSubstitute(y) != 0 && IsFusionSubstitute(w) != 0)
        return 0;
    if (IsFusionSubstitute(z) != 0 && IsFusionSubstitute(w) != 0)
        return 0;
again:
    if (*(u32 *)e == 0x03E703E7 && e[2] == 0x3E7)
        return 0;
    if (e[0] == x) {
        if (y == e[1]) {
            if (z == e[2] && w == e[3])
                return 1;
            if (z == e[3] && w == e[2])
                return 1;
            if (IsFusionSubstitute(z) != 0 && (w == e[2] || w == e[3]))
                return 1;
            if (IsFusionSubstitute(w) != 0 && (z == e[2] || z == e[3]))
                return 1;
            return 0;
        }
        if (y == e[2]) {
            if (z == e[1] && w == e[3])
                return 1;
            if (z == e[3] && w == e[1])
                return 1;
            if (IsFusionSubstitute(z) != 0 && (w == e[1] || w == e[3]))
                return 1;
            if (IsFusionSubstitute(w) != 0 && (z == e[1] || z == e[3]))
                return 1;
            return 0;
        }
        if (y == e[3]) {
            if (z == e[1] && w == e[2])
                return 1;
            if (z == e[2] && w == e[1])
                return 1;
            if (IsFusionSubstitute(z) != 0 && (w == e[1] || w == e[2]))
                return 1;
            if (IsFusionSubstitute(w) != 0 && (z == e[1] || z == e[2]))
                return 1;
            return 0;
        }
        if (IsFusionSubstitute(y) != 0) {
            if (z == e[1] && (w == e[2] || w == e[3]))
                return 1;
            if (z == e[2] && w == e[3])
                return 1;
            return 0;
        }
    }
    e += 4;
    goto again;
}
int IsMaterialOfFusion(int a, int b)
{
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u32 i;
    const u16 *e;

    if (x > 0x7CF)
        x -= 0x7D0;
    if (y > 0x7CF)
        y -= 0x7D0;
    for (i = 0, e = gFusionRecipes2; i <= 0x34; e += 4, i++) {
        if (x == e[0] && (e[1] == y || e[2] == y))
            return 1;
    }
    for (i = 0, e = gFusionRecipes3; i <= 3; e += 4, i++) {
        if (x == e[0] && (e[1] == y || e[2] == y || e[3] == y))
            return 1;
    }
    if (IsFusionSubstitute(y) != 0)
        return 1;
    return 0;
}
/* Hand card word; the player term written first makes agbcc compute i * 4 first, as the ROM does. */
#define HAND_WORD_CE90(p, i) (*(u32 *)((p) * 0xD64 + (i) * 4 + (u32)gDuelHands))
u16 FindFusionMaterialSlot(int player, u16 a, u16 b, u16 c)
{
    int i;
    u16 id; /* one id for all four scans: per-loop locals put it in r0/r1 instead of r2 */

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZB(player & 1, i);
        id = CARD_ID(CARD_WORD(z->card));

        if (id != 0) {
            if (CARD_NUMBER(id) == a || CARD_NUMBER(id) == a + 0x7D0) {
                u16 pos = i + 0x4000;

                if (b != pos && c != pos)
                    return pos;
            }
        }
    }
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        id = CARD_ID(HAND_WORD_CE90(player & 1, i));

        if (CARD_NUMBER(id) == a || CARD_NUMBER(id) == a + 0x7D0) {
            u16 pos = i + (int)0xFFFF8000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZB(player & 1, i);
        id = CARD_ID(CARD_WORD(z->card));

        if (id != 0 && IsFusionSubstitute(CARD_NUMBER(id)) != 0) {
            u16 pos = i + 0x4000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        id = CARD_ID(HAND_WORD_CE90(player & 1, i));

        if (id != 0 && IsFusionSubstitute(CARD_NUMBER(id)) != 0) {
            u16 pos = i + (int)0xFFFF8000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    return 0xFFFF;
}
u16 GetFusionSlotCardId(int player, u16 pos)
{
    if (pos & 0x8000) {
        int pa = 1 & player;
        u16 off = (pos & 0xFFF) * 4 + pa * 0xD64;
        return CARD_ID(*(u32 *)(off + (u32)gDuelHands));
    } else if (pos & 0x4000) {
        int pa = 1 & player;
        u32 off = (pos & 0xFFF) * 0x94 + pa * 0xD64;
        return CARD_ID(*(u32 *)(off + (u32)gDuelZones));
    }
    return 0;
}

int CheckFusionRecipe(u16 a, u16 b, u16 c, u16 d)
{
    int r;

    if (d != 0)
        r = CheckFusionRecipe3(a, b, c, d);
    else
        r = CheckFusionRecipe2(a, b, c);
    if ((u16)r == 0)
        return 0;
    return 1;
}
static inline int FusKind_D0E8(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 0x776:
        return 3;
    case 0x777:
    case 0x778:
        return 1;
    }
    switch ((int)CARD_TYPE(id)) {
    case 0x16:
        return 7;
    case 0x15:
        return 8;
    case 0x17:
        return 9;
    }
    return (CARD_STATS(id) & 0xC0000) >> 18;
}
#define FUS_WILD(p) IsFusionSubstitute(CARD_NUMBER(GetFusionSlotCardId(player, (p))))
int FindFusionMaterials(int player, u16 id, u16 *out)
{
    const u16 *e;
    u16 num;
    u32 i;

    if (CARD_TYPE(id) > 0x14)
        return 0;
    if (FusKind_D0E8(id) != 2)
        return 0;
    num = CARD_NUMBER(id);
    if (num > 0x7CF)
        num -= 0x7D0;

    for (i = 0, e = gFusionRecipes2; i <= 0x34; e += 4, i++) {
        if (num == e[0]) {
            u16 a = e[1];
            u16 b = e[2];
            out[1] = FindFusionMaterialSlot(player, a, 0xFFFF, 0xFFFF);
            out[0] = FindFusionMaterialSlot(player, b, out[1], 0xFFFF);
            if (out[0] == 0xFFFF)
                return 0;
            if (out[1] == 0xFFFF)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[1]) != 0)
                return 0;
            if ((out[0] & 0x8000) && (out[1] & 0x8000)) {
                if (CountMonsters(player) == 5)
                    return 0;
            }
            return 1;
        }
    }
    for (i = 0, e = gFusionRecipes3; i <= 3; e += 4, i++) {
        if (num == e[0]) {
            u16 a = e[1];
            u16 b = e[2];
            u16 c = e[3];
            out[2] = FindFusionMaterialSlot(player, a, 0xFFFF, 0xFFFF);
            out[1] = FindFusionMaterialSlot(player, b, out[2], 0xFFFF);
            out[0] = FindFusionMaterialSlot(player, c, out[2], out[1]);
            if (out[0] == 0xFFFF)
                return 0;
            if (out[1] == 0xFFFF)
                return 0;
            if (out[2] == 0xFFFF)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[1]) != 0)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[2]) != 0)
                return 0;
            if (FUS_WILD(out[1]) != 0 && FUS_WILD(out[2]) != 0)
                return 0;
            if ((out[0] & 0x8000) && (out[1] & 0x8000) && (out[2] & 0x8000)) {
                if (CountMonsters(player) == 5)
                    return 0;
            }
            return 1;
        }
    }
    return 0;
}
#undef FUS_WILD
struct FusList_3D3D0 {
    u8 pad[0x502];
    u32 cnt : 2;
    u32 rest : 6;
    u8 pad2;
    u16 list[3];
};
#define FL_3D3D0 ((struct FusList_3D3D0 *)gChain)

int IsPendingFusionMaterial(u16 num)
{
    int i;

    for (i = 0; i < FL_3D3D0->cnt; i++) {
        if (FL_3D3D0->list[i] != 0) {
            int y = CARD_NUMBER(FL_3D3D0->list[i]);
            int x = CARD_NUMBER(num);

            if (x > 0x7CF)
                x -= 0x7D0;
            if (y > 0x7CF)
                y -= 0x7D0;
            if (y == x)
                return 1;
        }
    }
    return 0;
}
struct FusList_3D460 {
    u8 pad[0x502];
    u8 cnt : 2;
    u8 rest : 6;
    u8 pad2;
    u16 list[3];
};
#define FL_3D460 ((struct FusList_3D460 *)gChain)

void RemovePendingFusionMaterial(u16 num)
{
    int i;

    for (i = 0; i < FL_3D460->cnt; i++) {
        if (FL_3D460->list[i] != 0) {
            int y = CARD_NUMBER(FL_3D460->list[i]);
            int x = CARD_NUMBER(num);

            if (x > 0x7CF)
                x -= 0x7D0;
            if (y > 0x7CF)
                y -= 0x7D0;
            if (y == x) {
                FL_3D460->list[i] = 0;
                return;
            }
        }
    }
    if (IsFusionSubstitute(CARD_NUMBER(num)) == 0)
        return;
    for (i = 0; i < FL_3D460->cnt; i++) {
        if (FL_3D460->list[i] != 0 && IsFusionSubstitute(CARD_NUMBER(FL_3D460->list[i])) != 0) {
            FL_3D460->list[i] = 0;
            return;
        }
    }
}
struct Fus57C {
    u8 pad0[0x3E0];
    u8 phase;           /* +0x3E0 */
    u8 pad3E1[0x500 - 0x3E1];
    u16 target;         /* +0x500: card id of the fusion result (hypothesis) */
    u8 cnt : 2;         /* +0x502 bits 0-1: number of ingredients */
    u8 cur : 2;         /* +0x502 bits 2-3 */
    u8 hi : 4;          /* +0x502 bits 4-7 */
    u8 pad503;
    u16 ids[3];         /* +0x504: ingredient card ids */
    u16 pos[3];         /* +0x50A: ingredient positions (0x8000 hand / 0x4000 field | index) */
};
#define F57C ((struct Fus57C *)gChain)
int EffectPolymerizationPrepare(struct CardRef *ref, int a, int b);
int AiPickCardListEntry(u16 id);
int SendFusionMaterialToGrave(int player, int zone);
void BanishFieldCard(int player, int zone, u16 arg);
extern char gStrSelectFusionMonsterToSummon[];
extern char gStrSelectTwoFusionMaterials[];
extern char gStrSelectThreeFusionMaterials[];
#define HAND_57C(p, i) (*(u32 *)((p) * 0xD64 + (i) * 4 + (u32)gDuelHands))
#define ZONE_57C(p, i) (*(u32 *)((p) * 0xD64 + (i) * 0x94 + (u32)gDuelZones))
struct ViewCard57C {
    u16 id : 12;
    u16 hi : 4;
    u16 h1;
};
struct ListView57C {
    u8 unk0[5];
    u8 row : 2;
    u8 unk5_2 : 6;
    u16 top;
    u8 unk8[4];
    struct ViewCard57C cards[0x80];
};
#define LV57C ((struct ListView57C *)&gCardListView)
int EffectPolymerizationResolve(struct CardRef *ref, int arg)
{
    if (ref->skip4)
        return 0;
    switch (F57C->phase) {
    case 0x80:
        if (EffectPolymerizationPrepare(ref, arg, 0) == 0)
            return 0;
        if (1 & ((u8 *)ref)[2]) {
            int r = AiPickCardListEntry(ref->id);
            if (r < 0)
                return 0;
            gCardListView.row = 0;
            gCardListView.top = r;
            return 0x7E;
        }
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectFusionMonsterToSummon);
        return 0x7F;
    case 0x7F:
        CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
        return 0x7E;
    case 0x7E:
        F57C->target = LV57C->cards[LV57C->top + LV57C->row].id;
        if ((u16)FindFusionMaterials(ref->player, F57C->target, F57C->pos) == 0)
            return 0;
        switch (CARD_NUMBER(F57C->target)) {
        case 0x00E: case 0x01D: case 0x024: case 0x02A: case 0x044: case 0x055: case 0x05B:
        case 0x072: case 0x084: case 0x0A7: case 0x0BC: case 0x0D6: case 0x0D8: case 0x0DE:
        case 0x0E5: case 0x10F: case 0x125: case 0x171: case 0x180: case 0x184: case 0x198:
        case 0x1AA: case 0x1B4: case 0x1C9: case 0x1D0: case 0x1D2: case 0x1D6: case 0x1D8:
        case 0x1E1: case 0x1E6: case 0x1EE: case 0x1FB: case 0x1FC: case 0x208: case 0x212:
        case 0x214: case 0x220: case 0x233: case 0x23B: case 0x251: case 0x264: case 0x268:
        case 0x269: case 0x27E: case 0x2C2: case 0x2C8: case 0x32C: case 0x4D9: case 0x536:
        case 0x57C: case 0x5A5: case 0x5F6: case 0x7DE: case 0x814:
            F57C->cnt = 2;
            break;
        case 0x17B: case 0x1B9: case 0x234:
            F57C->cnt = 3;
            break;
        }
        if (1 & ((u8 *)ref)[2])
            return 0x64;
        switch (F57C->cnt) {
        case 2:
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectTwoFusionMaterials);
            break;
        case 3:
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectThreeFusionMaterials);
            break;
        }
        F57C->hi = 0;
        F57C->cur = F57C->cnt;
        {
            int i;
            for (i = 0; i < F57C->cnt; i++) {
                u16 id = 0;
                /* FAKEMATCH: the do-while(0) puts the hand read one loop level deeper, which
                 * weights the hoisted hand base's refs so it wins r9 as in the ROM */
                if (F57C->pos[i] & 0x8000)
                    do { id = CARD_ID(HAND_57C(1 & ref->player, F57C->pos[i] & 0xFF)); } while (0);
                if (F57C->pos[i] & 0x4000)
                    id = CARD_ID(ZONE_57C(1 & ref->player, F57C->pos[i] & 0xFF));
                F57C->ids[i] = id;
            }
        }
        F57C->cur--;
        return 0x7D;
    case 0x7D: {
        u16 keys = DuelCursor_PickTarget(0xF1);
        u16 id;
        int ok;
        struct DuelScreen *ds;

        switch (gDuelScreen.w828) {
        case 0xB:
            id = CARD_ID(HAND_57C(ref->player, gDuelScreen.idx82C));
            break;
        case 0:
            id = CARD_ID(ZONE_57C(ref->player, gDuelScreen.idx82C));
            break;
        default:
            id = 0;
            break;
        }
        ok = 0;
        if (id != 0) {
            ok = (u16)IsPendingFusionMaterial(id) != 0;
            if (IsFusionSubstitute(CARD_NUMBER(id)) != 0) {
                int h = F57C->hi;
                ok = 0;
                if (h == 0)
                    ok = 1;
            }
        }
        if (ok == 0 || keys == 0)
            return 0x7D;
        RemovePendingFusionMaterial(id);
        if (IsFusionSubstitute(CARD_NUMBER(id)) != 0)
            F57C->hi++;
        ds = &gDuelScreen;
        switch (ds->w828) {
        case 0xB:
            DuelCmd_Push(CARD_NUMBER(ref->id) != 0x60B ? ((1 & ((u8 *)ref)[2]) ? 0x80CC : 0xCC)
                                                       : ((1 & ((u8 *)ref)[2]) ? 0x80CD : 0xCD),
                         ds->idx82C, 0, 0);
            break;
        case 0:
            if (CARD_NUMBER(ref->id) != 0x60B)
                SendFusionMaterialToGrave(ref->player, ds->idx82C);
            else
                BanishFieldCard(ref->player, ds->idx82C, 1);
            break;
        }
        {
            int c = F57C->cur;
            if (c != 0) {
                F57C->cur = c - 1;
                return 0x7D;
            }
        }
        return 0x63;
    }
    case 0x64: {
        u16 *pp;
        int c = F57C->cnt;

        if (c != 0) {
            F57C->cnt = c - 1;
            pp = &F57C->pos[F57C->cnt];
            if (*pp & 0x8000)
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80CC : 0xCC, *pp & 0xFF, 0, 0);
            else if (*pp & 0x4000)
                SendFusionMaterialToGrave(ref->player, *pp & 0xFF);
            return 0x64;
        }
        return 0x63;
    }
    case 0x63: {
        u16 *cw;

        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80CA : 0xCA, 0, 0, 0);
        cw = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.row];
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80DC : 0xDC, cw[0], cw[1], 0);
        return 0x62;
    }
    case 0x62:
        QueueSpecialSummonChoosePosition(ref->player, &gCardListView.cards[gCardListView.top + gCardListView.row], 1, 1);
        return 0x61;
    }
    return 0;
}
