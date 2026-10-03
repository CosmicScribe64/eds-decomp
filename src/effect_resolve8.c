#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a step state (0x7E/0x7F). See wiki/functions/code-080383f0.md.
 */

/* struct DuelCard / DuelZone / DuelZonesPlayer come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
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
/* gDuel is declared as struct DuelState in duel.h; below +0x1ACC it is addressed as bytes. */

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
/* Local view for EffectTributeOpponentMonsterResolve (non-matching draft, under `#if 0`). The canonical struct DuelPlayer
 * declares its byte at +8 as a plain u8 (unk8), but this draft reads and writes bit 4 of it
 * (the canonical field differs: unk8 as a u8 versus flag8_4 as bit 4), so the unit keeps a
 * bitfield view for that one byte. */
struct PlayerStateLocal {
    u16 lifePoints;
    u8 handCount;       /* +2 */
    u8 deckCount;       /* +3 */
    u8 pad4[4];
    u8 unk8_0 : 4;
    u8 flag8_4 : 1;     /* +8 bit 4 */
    u8 unk8_5 : 3;
    u8 pad9[0xD64 - 9];
};
extern struct PlayerStateLocal gPlayerState[2] asm("gDuelPlayers");
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

int EffectSealOfTheAncientsResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 10; i++) {
            int pp = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(pp, i);

            if (CARD_WORD(z->card) << 20 != 0) {
                struct DuelZone *z2 = ZB((1 - ref->player) & 1, i);

                if ((2 & ZFLAGS(z2)) == 0) {
                    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, 1 - ref->player, (u8)i << 8, 0);
                    DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x807F : 0x7F, i, 0, 0);
                    ShowCardDetail(ref->player, CARD_ID(CARD_WORD(ZBP((1 - ref->player) & 1, i)->card)));
                    DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x807F : 0x7F, i, 0, 0);
                }
            }
        }
    }
    return 0;
}
int EffectDiceResolve(struct CardRef *ref)
{
    int r = Random() % 6 + 1;

    if (!ref->skip4) {
        int msg;
        int pl;
        int i;
        u8 plb;

        switch (CARD_NUMBER(ref->id)) {
        case 0x4B3:
            pl = ref->player;
            msg = 0xE2;
            break;
        case 0x4B4:
            pl = 1 - ref->player;
            msg = 0xE3;
            break;
        }
        /* The assignment in the arm keeps 0x8000 an SImode constant (msg | 0x8000 would be
         * narrowed to u16) and gives an if/else, so CSE does not share the first "1 &" test. */
        DuelCmd_Push(ref->player ? (msg |= 0x8000) : msg, r, 0, 0);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);

        for (i = 0; i <= 4; i++) {
            struct DuelZone *z = ZB(pl & 1, i);

            if ((2 & ZFLAGS(z)) && CARD_WORD(z->card) << 20) {
                plb = pl; /* hoisted by loop.c after pl & 1, as in the ROM */
                QueueAddZoneLink(ref->player, ref->id, (u8)i << 8 | plb, (u8)r << 8 | 3);
            }
        }
    }
    return 0;
}
int EffectExchangeResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            DuelPrompt_Post(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F: {
            u8 *g = (u8 *)&gDuel;

            ref->targets[0] = *(u16 *)(g + 0x1B64);
            DuelPrompt_Post(1 - ref->player, 6, 0, 0);
            return 0x7E;
        }
        case 0x7E: {
            u8 *g = (u8 *)&gDuel;
            u16 *p = (u16 *)(g + 0x1B64);

            ref->targets[1] = *p;
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80C7 : 0xC7, *p, ref->targets[0], 0);
            return 0x64;
        }
        }
    }
    return 0;
}
struct CoinListView {
    u8 unk0[5];
    u8 row : 2;
    u8 unk5_2 : 6;
    u16 top;
    u8 unk8[4];
    u32 cards[0x80];
    u16 kinds[0x80];
    u16 count;
};
extern struct CoinListView gCardListView;
int CountFreeMonsterZones(int player);
int CountActiveCardsOnField(int player, u16 number);
int CountFaceUpMonstersByNumber(int player, u16 number);
int GetZoneCardAtk(int player, int zone);
int HalveRoundDown(int value);
void LoseLifePoints(int player, int amount);
void FormatStr(char *dest, const char *fmt, const char *arg);
void CardListView_Open(int player, int index, int number, int arg);
int AddDeckCardToHand(int player, u16 number);
extern char gStrPromptTributeToSpecialSummonFmt[];
extern char gStrPromptSummonFromHandOrDeck[];
extern char gStrTimeWizardSelectTributeFmt[];
extern char gStrSelectMagicFromDeck[];
extern const char gCardNames[][0x40];
extern const u16 gUnk_08623E38;
extern const u16 gUnk_08624758;

int EffectTimeWizardResolve(struct CardRef *ref)
{
    char text[0x100];
    char intermediate[0x100];
    int i;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                TextBoxOpen(0x206, 0x613, 0xB, gStrCoinTossSelection);
                TextBoxSetMenu(2, 0, 0);
            } else {
                gTextBox.flag14 = Random() & 1;
            }
            return 0x7F;
        case 0x7F: {
            u8 random = Random() & 1;

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gTextBox.flag14, random, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            if (random == gTextBox.flag14) {
                for (i = 0; i <= 4; i++) {
                    int p = (1 - ref->player) & 1;
                    struct DuelZone *z = ZB(p, i);

                    if (CARD_WORD(z->card) << 20) {
                        DestroyFieldCardByEffect(1 - ref->player, i);
                        OnCardDestroyedByEffect(ref->player, 1 - ref->player, i);
                    }
                }
                if (CountFreeMonsterZones(ref->player) <= 0)
                    goto retA;
                if (CollectEffectTargets(ref->player, 0xF, 0) <= 0)
                    goto retA;
                if (CountActiveCardsOnField(0, 0x58A) != 0 || CountActiveCardsOnField(1, 0x58A) != 0)
                    goto retA;
                if (CountFaceUpMonstersByNumber(ref->player, 0x22) == 0 && CountFaceUpMonstersByNumber(ref->player, 0x4BA) == 0 && CountFaceUpMonstersByNumber(ref->player, 0x7F2) == 0)
                    goto retA;
                return 0x78;
            } else {
                int total = 0;

                for (i = 0; i <= 4; i++) {
                    int p = ref->player;
                    struct DuelZone *z = ZB(p, i);

                    if (CARD_WORD(z->card) << 20) {
                        total += GetZoneCardAtk(ref->player, i);
                        DestroyFieldCardByEffect(ref->player, i);
                    }
                }
                LoseLifePoints(ref->player, HalveRoundDown(total));
                goto retA;
            }
        }
        case 0x78:
            FormatStr(intermediate, gStrPromptTributeToSpecialSummonFmt, gCardNames[gUnk_08623E38]);
            FormatStr(text, intermediate, gCardNames[gUnk_08624758]);
            TextBoxOpen(0x206, 0x613, 0xB, text);
            TextBoxSetMenu(1, 0, 0);
            return 0x77;
        case 0x77: {
            int hasOne;
            int hasTwo;

            if (gTextBox.flag14 == 0)
                goto retA;
            i = 0;
            hasOne = 0;
            hasTwo = 0;
            for (; i < gCardListView.count; i++) {
                if (gCardListView.kinds[i] != 1) {
                    if (gCardListView.kinds[i] == 2)
                        hasTwo = 1;
                } else {
                    hasOne = 1;
                }
            }
            if (hasOne) {
                if (hasTwo) {
                    TextBoxOpen(0x206, 0x613, 0xB, gStrPromptSummonFromHandOrDeck);
                    TextBoxSetMenu(2, 0, 0);
                    return 0x76;
                }
                ref->targets[0] = 1;
                return 0x6E;
            }
            if (hasTwo) {
                ref->targets[0] = 2;
                return 0x6E;
            }
            return 0xA;
        }
        case 0x76:
            switch (gTextBox.flag14) {
            case 0:
                ref->targets[0] = 1;
                break;
            case 1:
                ref->targets[0] = 2;
                break;
            }
            return 0x6E;
        case 0x6E:
            FormatStr(text, gStrTimeWizardSelectTributeFmt, ((const char (*)[0x40])0x0822C720)[gUnk_08623E38]);
            TextBoxOpen(0x206, 0x613, 0xB, text);
        ret6D:
            return 0x6D;
        case 0x6D:
            if (DuelCursor_PickTarget(0xE0) != 0) {
                int player = gDuelScreen.w824;
                int zone = gDuelScreen.w828 + gDuelScreen.idx82C;
                int pp = player & 1;
                struct DuelZone *z = ZB(pp, zone);
                int number = ((const u16 *)0x08622AB4)[CARD_ID11(CARD_WORD(z->card))];

                switch (number) {
                case 0x22:
                case 0x4BA:
                case 0x7F2:
                    TributeMonster(player, zone);
                    return 0x64;
                default:
                    PlaySE(3);
                    goto ret6D;
                }
            }
            goto ret6D;
        case 0x64: {

            for (i = 0; i < gCardListView.count; i++) {
                if (gCardListView.kinds[i] == ref->targets[0]) {
                    u32 *card = &gCardListView.cards[i];

                    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80C2 : 0xC2, ((u16 *)card)[0], ((u16 *)card)[1], 0);
                    QueueSpecialSummonChoosePosition(ref->player, card, 1, 1);
                    return 0x63;
                }
            }
        retA:
            return 0xA;
        }
        case 0x63:
            return 0x62;
        case 0x62:
            if (CollectEffectTargets(ref->player, 0x4B2, 0) > 0) {
                TextBoxOpen(0x206, 0x613, 0xB, gStrSelectMagicFromDeck);
                return 0x61;
            }
            goto retA;
        case 0x61:
            CardListView_Open(ref->player, -1, 0x4B2, 0);
            return 0x60;
        case 0x60: {
            int p = ref->player;
            u32 card = gCardListView.cards[gCardListView.top + gCardListView.row];

            if (AddDeckCardToHand(p, ((const u16 *)0x08622AB4)[CARD_ID11(card)]))
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            goto retA;
        }
        default:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
        }
    }
    return 0;
}
int EffectGoddessOfWhimResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                TextBoxOpen(0x206, 0x613, 0xB, gStrCoinTossSelection);
                TextBoxSetMenu(2, 0, 0);
            } else {
                gTextBox.flag14 = Random() & 1;
            }
            return 0x7F;
        case 0x7F: {
            u8 r = Random() & 1;

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gTextBox.flag14, r, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            QueueAddZoneLink(ref->player, ref->id, ref->player | ref->zone << 8, r == gTextBox.flag14 ? 0x103 : 3);
            return 0xA;
        }
        default:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            break;
        }
    }
    return 0;
}
int EffectBarrelDragonResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int mask = 0;
        int cnt = 0;
        int i;

        for (i = 0; i <= 2; i++) {
            if ((Random() & 1) == 0)
                cnt++;
            else
                mask |= 1 << i;
        }
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80E1 : 0xE1, mask, 0, 0);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
        if (cnt > 1) {
            DestroyFieldCardByEffect(1 - ref->player, ref->targets[0] >> 8);
            OnCardDestroyedByEffect(ref->player, 1 - ref->player, ref->targets[0] >> 8);
        }
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
    }
    return 0;
}
int EffectSummonDarkMagicianFromDeckResolve(struct CardRef *ref)
{
    u16 *cw = gCardListViewCards;

    if (ref->skip4) {
        if (!ref->flag4_3)
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
    } else {
        switch (EFF_PHASE) {
        case 0x80:
            if (CollectEffectTargets(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                break;
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, cw[0], cw[1], 0);
            return 0x7F;
        case 0x7F:
            QueueSpecialSummonChoosePosition(ref->player, (u32 *)cw, 1, 0);
            return 0x7E;
        default:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
            break;
        }
    }
    return 0;
}
int EffectDestroyAndGiveMonsterResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        u8 tp1 = ref->targets[0];
        int tz1 = ref->targets[0] >> 8;
        u8 tp2 = ref->targets[1];
        int tz2 = ref->targets[1] >> 8;

        switch (EFF_PHASE) {
        case 0x80: {
            int pa = tp1 & 1;
            struct DuelZone *za = ZB(pa, tz1);

            if (CARD_WORD(za->card) << 20 != 0) {
                int pb = tp2 & 1;
                struct DuelZone *zb = ZB(pb, tz2);

                if (CARD_WORD(zb->card) << 20 != 0) {
                    if (tp1 != ref->player && tp2 == ref->player) {
                        DestroyFieldCardByEffect(tp1, tz1);
                        OnCardDestroyedByEffect(ref->player, tp1, tz1);
                        return 0x7F;
                    }
                }
            }
            break;
        }
        case 0x7F:
            MoveFieldCard(ref->player, ref->targets[1], ref->targets[0]);
            break;
        }
    }
    return 0;
}
struct EffBuf {
    u8 pad[0x4E4];
    struct CardRef ref;
};
#define ZB28(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + ((u32)gPlayerState + 0x28)))
static inline u8 CardType_08038FB8(u16 id)
{
    return CARD_TYPE(id);
}
static inline u8 CardLevel_08038FB8(u16 id)
{
    u8 lvl;

    CARD_LEVEL(id, lvl);
    return lvl;
}
#define HANDW(p, i) (*(u32 *)((p) * 0xD64 + (i) * 4 + (u32)gDuelHands))

int EffectTributeOpponentMonsterResolve(struct CardRef *ref, int arg)
{
    char text[0x80]; /* unused: the ROM reserves 0x80 stack bytes and never touches them */

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (EffectCanTributeOpponentMonsterPrepare(ref, arg, 0) != 0)
                return 0x7F;
            break;
        case 0x7F: {
            int found = 0;
            int has;
            int i;

            if (!gPlayerState[1 & ref->player].flag8_4) {
                for (i = 0; i < gPlayerState[1 & ref->player].handCount; i++) {
                    u16 id = CARD_ID(HANDW(ref->player, i));

                    if (CardType_08038FB8(id) <= 0x14 && IsSpecialSummonOnly(id) == 0) {
                        if (CardLevel_08038FB8(id) > 4) {
                            if (CardLevel_08038FB8(id) <= 6) {
                                if (FindFreeMonsterZone(ref->player) != -1)
                                    found = 1;
                            } else {
                                if (CountTributableMonsters(ref->player, -1) > 0)
                                    found = 1;
                            }
                        }
                    }
                }
            }
            if (gPlayerState[ref->player].flag8_4)
                found = 0;
            has = 0;
            for (i = 0; i <= 4; i++) {
                int pp = 1 & ref->player;
                struct DuelZone *z = ZB28(pp, i);
                u16 id = CARD_ID(CARD_WORD(z->card));

                if (id != 0) {
                    struct DuelZone *z2 = ZB28(1 & ref->player, i);

                    if ((2 & ZFLAGS(z2)) != 0) {
                        switch (CARD_NUMBER(id)) {
                        case 0x58:
                        case 0x105:
                        case 0x1FF:
                            has = 1;
                            break;
                        }
                    }
                }
            }
            if (found != 0) {
                if (has == 0) {
                ret78:
                    return 0x78;
                }
            } else {
                if (has != 0)
                    goto ret64;
                break;
            }
            TextBoxOpen(0x204, 0x717, 0xB, gStrPromptTributeUse);
            TextBoxSetMenu(2, 0, 0);
            return 0x7E;
        }
        case 0x7E:
            if (gTextBox.flag14 == 0)
                goto ret78;
        ret64:
            return 0x64;
        case 0x78:
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectHighLevelMonster);
            gPlayerState[ref->player].flag8_4 = 1;
        ret77:
            return 0x77;
        case 0x77: {
            int idx;
            u16 id;

            if (DuelCursor_PickTarget(1) == 0)
                goto ret77;
            idx = gDuelScreen.idx82C;
            id = CARD_ID(HANDW(ref->player, idx));
            if (CardType_08038FB8(id) <= 0x14 && IsSpecialSummonOnly(id) == 0) {
                if (CardLevel_08038FB8(id) > 4) {
                    if (CardLevel_08038FB8(id) <= 6) {
                        TributeMonster(ref->targets[0], 0);
                        QueueNormalSummonChoosePosition(ref->player, idx, FindFreeMonsterZone(ref->player), 0);
                        return 0x6E;
                    }
                    TextBoxOpen(0x206, 0x712, 0xB, gStrSelectSecondTribute);
                    ref->targets[1] = idx;
                ret76:
                    return 0x76;
                }
            }
            PlaySE(3);
            goto ret77;
        }
        case 0x76:
            if (DuelCursor_PickTarget(0xF0) == 0)
                goto ret76;
            if (IsTributableMonster(ref->player, gDuelScreen.idx82C) != 0) {
                TributeMonster(ref->targets[0], 0);
                TributeMonster(ref->player, gDuelScreen.idx82C);
                QueueNormalSummonChoosePosition(ref->player, ref->targets[1], gDuelScreen.idx82C, 0);
                return 0x6E;
            }
            PlaySE(3);
            goto ret76;
        case 0x64:
            TextBoxOpen(0x206, 0x411, 0xB, gStrSelectEffectMonster);
        ret63:
            return 0x63;
        case 0x63: {
            int p7;
            int pos;

            if (DuelCursor_PickTarget(0xE0) == 0)
                goto ret63;
            p7 = gDuelScreen.w824;
            pos = gDuelScreen.w828 + gDuelScreen.idx82C;
            switch (((const u16 *)0x08622AB4)[CARD_ID11(CARD_WORD(ZBP(1 & p7, pos)->card))]) {
            case 0x58:
            case 0x105:
            case 0x1FF:
                PlaySE(1);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, (u16)gDuelScreen.w824,
                             (u8)gDuelScreen.w828 | (u8)gDuelScreen.idx82C << 8, 0);
                MemCopy16(&gChainProxyLink, ref, 0x14);
                gChainProxyLink.id = CARD_ID(CARD_WORD(ZBP(1 & p7, pos)->card));
                gChainProxyLink.zone = pos;
                return 0x62;
            }
            PlaySE(3);
            goto ret63;
        }
        case 0x62:
            switch (CARD_NUMBER(((struct EffBuf *)gChain)->ref.id)) {
            case 0x58:
            case 0x1FF:
                EffectCatapultTurtleResolve(&gChainProxyLink, 0);
                return 0x61;
            case 0x105:
                EffectTheLittleSwordsmanOfAileResolve(&((struct EffBuf *)gChain)->ref, 0);
                return 0x61;
            }
            break;
        }
    }
    return 0;
}
