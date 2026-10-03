#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a code like 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
 *
 * Shared duel structs/globals (struct DuelCard/DuelZone/DuelZonesPlayer, the gDuel /
 * gDuelPlayers / gDuelZones externs) come from include/duel.h. ZFLAGS is a raw byte view
 * of the zone flags byte at +0x06 (canonical struct DuelZone.flag6_0/flag6_1/counter6).
 */

#define ZFLAGS(z) (((u8 *)(z))[6])
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already negated/skipped (hypothesis) */
    u8 unk4_3 : 5;
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
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern const u16 gCardIdToNumber[];

/* Effect-resolution state at 0x02017A40 (byte view; only two bytes used here). */
extern u8 gChain[];
#define EFF_PHASE gChain[0x3E0]  /* 0x7E / 0x7F / 0x80: step of a multi-step effect (hypothesis) */
#define EFF_SIDE gChain[0x3E1]   /* player currently being processed */

/* 0x020192E4 + 0x5F0: a card word in each player's state (stride 0xD64), possibly a "set" spell/trap slot (hypothesis) */
struct PlayerCard5F0 {
    struct DuelCard card;
    u8 filler[0xD64 - 4];
};
extern struct PlayerCard5F0 gDuelFieldZone[2];

int ReturnGraveyardCardToHand(int player, u16 no);
void GainLifePoints(int player, int lp);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int IsZoneTargetable(int player, int zone);
void DestroyFieldCardByEffect(int player, int zone);
void OnCardDestroyedByEffect(int player, int a, int b);
void QueueAddZoneLink(int player, u16 a, u16 b, u16 c);
u32 GetFieldMagicIndex(u16 cardNo);
void EventResponse_Request(int player, int kind, u32 arg);
void ChangeBattlePosition(int player, int zone, int a, int b);
int GetZoneCardType(int player, int zone);
void sub_080197C0(int player, u16 id);
void FlipFieldCard(int player, int zone, int a);
void ShowRevealedCard(int player, u16 id);
void ApplyKotodama(void);
void DestroyFieldCard(int player, int zone, int a);
void LoseLifePoints(int player, int lp);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
u32 DuelCursor_PickTarget(u32 keys);
void ReturnHandCardToDeck(int player, int arg1, u16 arg2);
extern const u8 gStrNeedleBallPayLpPrompt[];
extern const u8 gStrYadoKaruReturnPrompt[];
extern const u8 gStrYadoKaruSelectPrompt[];

/* 0x020192E4 + 0x7C4: deck card words (80 entries) of each player, stride 0xD64. */
struct PlayerDeck {
    u32 deck[80];
    u8 filler[0xD64 - 80 * 4];
};
extern struct PlayerDeck gDuelDecks[2];
/* u16 at 0x0201AE60+0x14 (nonzero = flag; hypothesis: a "count/flag" of the current effect) */
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gTextBox;
/* Duel screen state at 0x0201CFB0. The word at +0x82C is passed to ReturnHandCardToDeck. */
struct DuelScreen82C {
    u8 unk0[0x82C];
    u32 unk82C;
};
extern struct DuelScreen82C gDuelScreen;

/* Card-list viewer at 0x0201D810 (see code_0802AAC0 struct ListView); fields used here. */
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
    u16 kinds[0x80];    /* +0x20C: per-entry kind (hypothesis) */
};
extern struct ListView gCardListView;
extern u8 gAiWork[];   /* u16 at +0x1B22 is the saved list position (hypothesis) */
extern const u8 gStrElegantEgotistSelectPrompt[];
int CountActiveCardsOnField(int player, u16 number);
int CollectEffectTargets(int player, int number, int b);
int CountFreeMonsterZones(int player);
int AiPickCardListEntry(u16 id);
void CardListView_Open(int player, int area, int a2, int a3);
void QueueSpecialSummonChoosePosition(int player, u32 *card, int a, int b);
void QueueSpecialSummon(int player, u32 *card, int a, int b, int c);
void LoseLpOnSendToGraveyard(int player, int a);
int AddDeckCardToHand(int player, u16 number);
void FormatStr(void *dst, const void *a, const void *b);
extern const u8 gStrThunderDragonAddPromptFmt[];
extern const u16 gCardNumberToId[];
extern const u8 gCardNames[];
extern const u8 gStrCyberSteinSelectPrompt[];
extern const u8 gStrGaleDograSelectPrompt[];
void TributeMonster(int player, int zone);
int EffectBlastJugglerCheck(struct CardRef *ref, u16 pos);
int EffectDragonSeekerCheck(struct CardRef *ref, u16 pos);
void ReturnFieldCardToHand(int player, int zone, int a);
void SendTopDeckCardsToGraveyard(int player, int a, int b);
void ShowCardDetail(int player, int id);
int EffectDestroyByTypeCheck(struct CardRef *ref, u16 pos);
void SwapFieldCards(int player, u16 pos1, u16 pos2);
void EquipCard(int player, u16 a, u16 b);
void ChangeBattlePosition(int player, int zone, int a, int b);
void ShowDestroyedCard(int player, int id);
int GetZoneCardDef(int player, int zone);
int HasFlipEffect(u16 number, int a);
void Chain_AddPending(u32 a, u32 b);
void FlipFieldCard(int player, int zone, int a);
void DiscardHandCard(int player, int a, int b, int c);
void DrawCards(int player, int a);


int CountMonsters(int player);
int CountTributableMonsters(int player, int a);
void DuelPrompt_PostTribute(int player);
int EffectSolemnJudgmentPrepare(struct CardRef *ref, int a, int b);
int DuelPrompt_Post(int player, int a, int b, int c);
/* Local view of gDuel (canonical struct DuelState in duel.h covers only +0x0000..+0x1ACC:
 * the u32 and the two players). This unit reads fields past that (+0x1B12 bit 1, +0x1B64 u16), so it
 * keeps a unit-local view; see wiki/functions/code-08035198.md. */
struct DuelGlobals {
    u8 unk0[0x1B12];
    u8 b0 : 1;      /* +0x1B12 bit 1: a player index (hypothesis) */
    u8 b1 : 1;
    u8 rest : 6;
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 w1B64;
};
#define DG ((struct DuelGlobals *)&gDuel)
void DuelPrompt_PostRandomDiscard(int player, int a, int b);
void DuelPrompt_PostDiscard(int player, int a, int b, int c);
int EffectTailorOfTheFickleCheck(struct CardRef *ref, u16 pos);
int IsValidEquipTarget(int p1, int z1, int p2, int z2);
int FindMonsterLinkedToCard(int player, int zone);
void MoveEquipCard(u16 pos);
void *MemCopy16(void *dst, const void *src, u32 n);
int FindCardEffect(u16 id);
/* Effect-resolution state at 0x02017A40, larger view. */
struct EffState {
    u8 unk0[0x3E0];
    u8 phase;       /* +0x3E0 */
    u8 side;        /* +0x3E1 */
    u8 unk3E2[0x4E4 - 0x3E2];
    struct CardRef cur;     /* +0x4E4: working copy of the CardRef being executed */
    u8 unk4F4[4];   /* +0x4F4: reserved tail of the 0x14-byte working copy */
    u8 (*fn)(struct CardRef *, int);    /* +0x4F8: executor picked from gCardEffects */
    u8 unk4FC[0x542 - 0x4FC];
    u16 w542;       /* +0x542: saved value (a stat) for the 0x78 step of EffectAttackResponseResolve */
};
#define ES ((struct EffState *)gChain)
struct EffEntry {       /* 0x18 bytes each at 0x0819A9D4 (hypothesis: effect table) */
    u32 unk0;
    u8 (*fn)(struct CardRef *, int);
    u32 unk8[4];
};
extern struct EffEntry gCardEffects[];
int FindFreeMonsterZone(int player);
void MoveFieldCard(int player, u16 a, u16 b);
int EffectTheCheerfulCoffinPrepare(struct CardRef *ref, int a, int b);
void PlaySE(int a);
extern const u8 gStrCheerfulCoffinDiscardPrompt[];
extern const u8 gStrCheerfulCoffinSelectMonster[];
/* Duel screen state at 0x0201CFB0, larger view (see struct DuelScreen82C). */
struct DuelScreenView {
    u8 unk0[0x824];
    u16 w824;
    u8 unk826[2];
    u8 b828;
    u8 unk829[3];
    u32 w82C;
};
#define DSV ((struct DuelScreenView *)&gDuelScreen)
/* 0x020192E4 + 0x684: a list of card words per player (stride 0xD64). */
struct PlayerWords {
    u32 w[0xD64 / 4];
};
extern struct PlayerWords gDuelHands[2];
int EffectLastWillPrepare(struct CardRef *ref, int a, int b);
int GetZoneCardAtk(int player, int zone);
void FormatInt(char *dst, const void *fmt, ...);
extern const u8 gStrWidespreadRuinTiePrompt[];
int EffectTheCheerfulCoffinResolve(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (EFF_SIDE == 0)
                return 0;
            if (EffectTheCheerfulCoffinPrepare(ref, arg, 0) == 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrCheerfulCoffinDiscardPrompt);
            TextBoxSetMenu(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gTextBox.flag14 == 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrCheerfulCoffinSelectMonster);
            return 0x7D;
        case 0x7D:
            if (DuelCursor_PickTarget(1) != 0) {
                int p = ref->player & 1;
                struct DuelScreenView *ds = DSV;
                u32 *sel = &ds->w82C;
                if (CARD_TYPE(CARD_ID11(*(u32 *)((u32)gDuelHands + *sel * 4 + p * 0xD64))) <= 0x14) {
                    PlaySE(1);
                    DuelCmd_Push(ref->player ? 0x8008 : 8, ds->w824, ds->b828 | (u8)*sel << 8, 0);
                    DiscardHandCard(ref->player, *sel, 1, 1);
                    EFF_SIDE--;
                    return 0x7F;
                }
                PlaySE(3);
            }
            return 0x7D;
        }
    }
    return 0;
}
int EffectCallOfTheDarkResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (((u8 *)z)[7] & 0x40) {
                    DestroyFieldCardByEffect(p, j);
                    OnCardDestroyedByEffect(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
int EffectChangeOfHeartResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int r = FindFreeMonsterZone(ref->player);

            if (tp != ref->player) {
                int p = tp & n;
                struct DuelZone *z = ZB(p, tz);
                int id = CARD_ID(CARD_WORD(z->card));

                if (id && r != -1) {
                    if (CARD_NUMBER(id) == 0x4B1 && (ZFLAGS(z) & 3) == 1) {
                        DuelCmd_Push(tp ? 0x807F : 0x7F, tz, 0, 0);
                        sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
                        return 0;
                    } else {
                        MoveFieldCard(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                        QueueAddZoneLink(ref->player, ref->id, ref->player | (u8)r << 8, 3);
                    }
                }
            }
        }
    }
    return 0;
}
int EffectSolemnJudgmentResolve(struct CardRef *ref, u16 *idp)
{
    if (!ref->skip4) {
        if (idp) {
            int type = CARD_TYPE(*idp);

            switch (type) {
            case 0x15:
            case 0x16:
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
            }
        } else if (EFF_PHASE == 0x80) {
            if (EffectSolemnJudgmentPrepare(ref, 0, 0)) {
                DuelCmd_Push((u8)ref->pos ? 0x8091 : 0x91, ref->pos >> 8, 7, 0);
                return 0x7F;
            }
        } else {
            u8 tp = ref->pos;
            int tz = ref->pos >> 8;

            DestroyFieldCard(tp, tz, 1);
        }
    }
    return 0;
}
int EffectHornOfHeavenResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            DuelCmd_Push((u8)ref->pos ? 0x8091 : 0x91, ref->pos >> 8, 7, 0);
            return 0x7F;
        } else {
            u8 tp = ref->pos;
            int tz = ref->pos >> 8;

            DestroyFieldCard(tp, tz, 1);
        }
    }
    return 0;
}
int EffectJustDessertsResolve(struct CardRef *ref)
{
    int n = CountMonsters(1 - ref->player);

    if (!ref->skip4 && n > 0)
        LoseLifePoints(1 - ref->player, n * 500);
    return 0;
}
int EffectFusionSageResolve(struct CardRef *ref)
{
    if (AddDeckCardToHand(ref->player, 0x3EB) || AddDeckCardToHand(ref->player, 0x40A))
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
    return 0;
}
int EffectBlockAttackResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            u8 tp = ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int p = tp & n;
            struct DuelZone *z = ZB(p, tz);

            if (!(ZFLAGS(z) & n))
                ChangeBattlePosition(tp, tz, 1, 0);
        }
    }
    return 0;
}
int EffectTheSternMysticResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2)) {
                    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, p, (u8)j << 8, 0);
                    DuelCmd_Push(p ? 0x807F : 0x7F, j, 0, 0);
                    ShowCardDetail(ref->player, CARD_ID(CARD_WORD(z->card)));
                    DuelCmd_Push(p ? 0x807F : 0x7F, j, 0, 0);
                }
            }
        }
    }
    return 0;
}
int EffectWallOfIllusionResolve(struct CardRef *ref)
{
    u8 tp = ref->pos;
    int tz = ref->pos >> 8;

    ReturnFieldCardToHand(tp, tz, 0);
    return 0;
}
int EffectLastWillResolve(struct CardRef *ref, int arg)
{
    u32 *card = &gCardListView.cards[gCardListView.row + gCardListView.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (EffectLastWillPrepare(ref, arg, 0)) {
                CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
                return 0x7F;
            }
            break;
        case 0x7F:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7E;
        case 0x7E:
            QueueSpecialSummonChoosePosition(ref->player, &gCardListView.cards[gCardListView.row + gCardListView.top], 1, 0);
            return 0x7D;
        case 0x7D:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int EffectWabokuResolve(struct CardRef *ref)
{
    if (DG->b1 != ref->player)
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8036 : 0x36, 1, 1, 0);
    return 0;
}
int EffectAttackResponseResolve(struct CardRef *ref)
{
    u8 tp = ref->pos;
    int tz = ref->pos >> 8;

    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x420: {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(tp & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 1)) {
                    DestroyFieldCardByEffect(tp, j);
                    OnCardDestroyedByEffect(ref->player, tp, j);
                }
            }
            break;
        }
        case 0x44A: {
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                GainLifePoints(ref->player, GetZoneCardAtk(tp, tz));
            break;
        }
        case 0x4BE: {
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2)) {
                DuelCmd_Push(tp ? 0x803B : 0x3B, tz, 0, 0);
                LoseLifePoints(tp, GetZoneCardAtk(tp, tz));
            }
            break;
        }
        case 0x587: {
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                QueueAddZoneLink(ref->player, ref->id, ref->pos, 3);
            break;
        }
        default:
            if (CARD_NUMBER(ref->id) == 0x2AD) {
                switch (EFF_PHASE) {
                case 0x80: {
                    int best = -1;
                    int bestZone = -1;
                    int count = 0;
                    int j;
                    char buf[0x80];

                    for (j = 0; j <= 4; j++) {
                        struct DuelZone *z = ZB(1 & tp, j);

                        if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 3) == 2) {
                            int v = GetZoneCardAtk(tp, j);

                            if (v == best)
                                count++;
                            if (v > best) {
                                best = v;
                                bestZone = j;
                                count = 1;
                            }
                        }
                    }
                    if (bestZone < 0)
                        break;
                    if (count == 1 || (1 & ((u8 *)ref)[2])) {
                        DestroyFieldCardByEffect(tp, bestZone);
                        OnCardDestroyedByEffect(ref->player, tp, bestZone);
                    } else {
                        FormatInt(buf, gStrWidespreadRuinTiePrompt, best);
                        TextBoxOpen(0x204, 0x817, 0xB, buf);
                        ES->w542 = best;
                        EFF_PHASE = 0x78;
                    ret78:
                        return 0x78;
                    }
                    break;
                }
                case 0x78:
                    if (DuelCursor_PickTarget(0xF0 << (tp << 4)) == 0)
                        goto ret78;
                    if (GetZoneCardAtk(tp, DSV->w82C) != ES->w542)
                        goto fail78;
                    DestroyFieldCardByEffect(tp, DSV->w82C);
                    OnCardDestroyedByEffect(ref->player, tp, DSV->w82C);
                    break;
                fail78:
                    PlaySE(3);
                    goto ret78;
                default:
                    return 0;
                }
            }
        }
    }
    return 0;
}
int EffectShareThePainResolve(struct CardRef *ref)
{
    if (!ref->skip4 && EFF_PHASE == 0x80) {
        if (CountTributableMonsters(1 - ref->player, -1)) {
            DuelPrompt_PostTribute(1 - ref->player);
            return 0x7F;
        }
    }
    return 0;
}
int EffectHeavyStormResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 1 - ref->player;
            EFF_PHASE--;
        case 0x7F: {
            int j;

            for (j = 5; j <= 10; j++) {
                int side = EFF_SIDE;
                struct DuelZone *z = &gDuelZones[(u8)side & 1].zones[j];

                if (CARD_ID(CARD_WORD(z->card))) {
                    DestroyFieldCard(side, j, j);
                    return 0x7F;
                }
            }
            {
                /* FAKEMATCH: the side pointer pinned to r0 */
                register u8 *p asm("r0");
                u8 *b = gChain;
                int sd;

                p = b + 0x3E1;
                *p = 1 - *p;
                sd = ref->player;
                if (*(volatile u8 *)p == sd)
                    return 0x7F;
            }
        }
        }
    }
    return 0;
}
int EffectCurseOfFiendResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                if (CARD_ID(CARD_WORD(z->card)))
                    ChangeBattlePosition(i, j, 1, 1);
            }
        }
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8048 : 0x48, 1, 0, 0);
    }
    return 0;
}
int EffectUpstartGoblinResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        DrawCards(ref->player, 1);
        GainLifePoints(1 - ref->player, 1000);
    }
    return 0;
}
int EffectFinalDestinyResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && (j > 4 || IsZoneTargetable(p, j))) {
                    DestroyFieldCardByEffect(p, j);
                    OnCardDestroyedByEffect(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
int EffectSnatchStealResolve(struct CardRef *ref)
{
    int p = ref->player;
    int zone = ref->zone;

    if (!(8 & ((u8 *)ref)[4])) {
        struct DuelZone *z = ZB(p, zone);
        int id = CARD_ID(CARD_WORD(z->card));

        int n;

        if (id && (ZFLAGS(z) & 2) && CARD_NUMBER(id) == 0x42C) {
            n = 7 & ((u8 *)ref)[0xA];
            if (n == 1) {
                int tp = (u8)ref->targets[0];
                int tz = ref->targets[0] >> 8;
                int r = FindFreeMonsterZone(ref->player);

                if (tp != ref->player) {
                    int p2 = tp & n;
                    struct DuelZone *z2 = ZB(p2, tz);

                    if (CARD_ID(CARD_WORD(z2->card)) && (ZFLAGS(z2) & 2)) {
                        EquipCard(ref->player, ref->player | ref->zone << 8, tp | tz << 8);
                        if (!ref->skip4 && r != -1)
                            MoveFieldCard(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                    }
                }
            }
        }
    }
    return 0;
}
int EffectConfiscationResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            DuelPrompt_Post(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F:
            DiscardHandCard(1 - ref->player, DG->w1B64, 1, 1);
            return 0x64;
        }
    }
    return 0;
}
int EffectDelinquentDuoResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            DuelPrompt_PostRandomDiscard(1 - ref->player, 1, 1);
            return 0x7F;
        case 0x7F:
            DuelPrompt_PostDiscard(1 - ref->player, 1, 0, 1);
            return 0x7E;
        }
    }
    return 0;
}
int EffectDarknessApproachesResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            u8 tp = ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int p = tp & n;
            struct DuelZone *z = ZB(p, tz);

            if (ZFLAGS(z) & 2)
                FlipFieldCard(tp, tz, 0);
        }
    }
    return 0;
}
/* Executor view of ES->fn: the callbacks return a full int; the caller keeps only the low byte. */
typedef int (*EffFn36030)(struct CardRef *, int);
/* Dispatcher: at phase 0x80 copies ref into ES->cur, takes id/player from card and picks the
 * card's executor from gCardEffects; then runs it and stores its step in EFF_PHASE. A null
 * executor or a zero step prints message 0xB0. */
int EffectFairysHandMirrorResolve(struct CardRef *ref, struct CardRef *card)
{
    u8 *p;

    if (EFF_PHASE == 0x80) {
        MemCopy16(&ES->cur, ref, 0x14);
        ES->cur.id = card->id;
        ES->cur.player = card->player;
        *(EffFn36030 *)&ES->fn = (EffFn36030)gCardEffects[FindCardEffect(card->id)].fn;
        if (*(EffFn36030 *)&ES->fn == 0) {
            /* FAKEMATCH: dead store; it keeps the phase pointer from being shared with the
             * setup block, so the base is reloaded from the pool and ref stays in r7. */
            p = 0;
            goto fail;
        }
    }
    {
        struct EffState *e = ES;
        int step = (*(EffFn36030 *)&e->fn)(&e->cur, 0);

        p = &e->phase;
        *p = step;
        if (*p != 0)
            goto success;
    }
fail:
    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
    return 0;
success:
    return *p;
}
int EffectTailorOfTheFickleResolve(struct CardRef *ref)
{
    u8 tp0 = ref->targets[0];
    int tz0 = ref->targets[0] >> 8;
    u8 tp1 = ref->targets[1];
    int tz1 = ref->targets[1] >> 8;

    if (!ref->skip4 && ref->numTargets == 2) {
        if (EffectTailorOfTheFickleCheck(ref, ref->targets[0]) && IsValidEquipTarget(tp0, tz0, tp1, tz1)) {
            if (FindMonsterLinkedToCard(tp0, tz0) != ref->targets[1])
                MoveEquipCard(ref->targets[0]);
        }
    }
    return 0;
}
int EffectTheForcefulSentryResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            DuelPrompt_Post(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F:
            ReturnHandCardToDeck(1 - ref->player, DG->w1B64, 1);
            DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
