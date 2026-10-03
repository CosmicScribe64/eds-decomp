#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a code like 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
 */

/* struct DuelCard, struct DuelZone, struct DuelZonesPlayer and gDuelZones come from duel.h. */
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

/* gDuelPlayers (struct DuelPlayer[2], with .handCount etc.) comes from duel.h. */

/* gDuelDecks (0x020192E4 + 0x7C4) is gDuelPlayers[player].deck (struct DuelCard[80]) from duel.h. */
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

/* Card-list viewer at 0x0201D810 (see card_list_viewer struct ListView); fields used here. */
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
void AiPickCardListEntry(u16 id);
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
/* gDuel is struct DuelState in duel.h, which only covers up to the players (size 0x1ACC).
 * EffectReverseTrapResolve reads the flag byte at +0x1ACD, just past that range, so keep a unit-local tail view. */
struct DuelStateTail {
    u8 unk0[0x1ACD];
    u8 flags1ACD;
};
extern struct DuelStateTail gUnk_020192E0Tail asm("gDuel");
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


int EffectDragonSeekerResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;

        if (EffectDragonSeekerCheck(ref, tz << 8 | tp)) {
            DestroyFieldCardByEffect(tp, tz);
            OnCardDestroyedByEffect(ref->player, tp, tz);
        }
    }
    return 0;
}
int EffectDestroyTargetResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tz;
        int tp;
        int p;
        int tzi; /* FAKEMATCH: an int copy of the u8 tz makes `tzi <= 4` a signed compare (bgt) */
        struct DuelZone *z;
        int id;

        tp = (u8)ref->targets[0];
        tz = ref->targets[0] >> 8;
        p = tp & 1;
        z = ZB(p, tz);
        id = CARD_ID(CARD_WORD(z->card));

        if (id) {
            switch (CARD_NUMBER(ref->id)) {
            case 0x3FF:
            case 0x4BB:
                if (CARD_NUMBER(id) == 0x4B1 && (ZFLAGS(z) & 3) == 1) {
                    DuelCmd_Push(tp ? 0x807F : 0x7F, tz, 0, 0);
                    sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
                    return 0;
                }
                break;
            }
            tzi = tz;
            {
                /* FAKEMATCH: the volatile read forces the second ref->id load (otherwise CSE'd
                 * with the switch's CARD_NUMBER read); putting the 0x7FF mask in a local makes
                 * its load precede that volatile ldrh, as in the ROM. */
                u32 m = 0x7FF;
                u16 vid = *(volatile u16 *)ref;
                int ct = (((const u32 *)0x08621DE0)[m & vid] & 0x1F00000) >> 20;

                if (ct == 0x16 && tzi <= 4 && IsZoneTargetable(tp, tz) == 0)
                    return 0;
            }
            if (ref->player != tp)
                DuelCmd_Push(tp ? 0x808B : 0x8B, tz, 1, 0);
            DestroyFieldCardByEffect(tp, tz);
            OnCardDestroyedByEffect(ref->player, tp, tz);
        }
    }
    return 0;
}
int EffectReturnTargetToHandResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)))
            ReturnFieldCardToHand(tp, tz, 0);
    }
    return 0;
}
int EffectNeedleWormResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        SendTopDeckCardsToGraveyard(1 - ref->player, 5, 1);
    return 0;
}
int EffectPatrolRoboResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);
        int id = CARD_ID(CARD_WORD(z->card));

        if (id && tp != ref->player && !(ZFLAGS(z) & 2)) {
            DuelCmd_Push(tp ? 0x807F : 0x7F, tz, 0, 0);
            ShowCardDetail(tp, id);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            DuelCmd_Push(tp ? 0x807F : 0x7F, tz, 0, 0);
        }
    }
    return 0;
}
int EffectWeatherReportResolve(struct CardRef *ref)
{
    int found = 0;
    int i;

    for (i = 5; i <= 9; i++) {
        int p = (1 - ref->player) & 1;
        struct DuelZone *z = ZB(p, i);
        u16 id = CARD_ID(CARD_WORD(z->card));
        int p2 = (1 - ref->player) & 1;
        struct DuelZone *z2 = ZB(p2, i);

        if ((ZFLAGS(z2) & 2) && id && CARD_NUMBER(id) == 0x15B) {
            DestroyFieldCard(1 - ref->player, i, 1);
            found = 1;
        }
    }
    if (found)
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8047 : 0x47, 1, 0, 0);
    return 0;
}
int EffectGreenkappaResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int i;

        for (i = 0; i < ref->numTargets; i++) {
            int tp = (u8)ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2))
                DestroyFieldCard(tp, tz, 1);
        }
    }
    return 0;
}
int EffectMorphingJarResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct DuelPlayer *pl = gDuelPlayers;

            if (pl[ref->player].handCount != 0) {
                DiscardHandCard(ref->player, 0, 0, 1);
                return 0x80;
            }
            return 0x7F;
        }
        case 0x7F: {
            struct DuelPlayer *pl = gDuelPlayers;
            int opp = (1 - ref->player) & 1;

            if (pl[opp].handCount != 0) {
                DiscardHandCard(1 - ref->player, 0, 1, 1);
                return 0x7F;
            }
            return 0x7E;
        }
        default:
            DrawCards(ref->player, 5);
            DrawCards(1 - ref->player, 5);
        }
    }
    return 0;
}
int EffectPenguinSoldierResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i < ref->numTargets && i <= 1; i++) {
            u8 tp = ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)))
                ReturnFieldCardToHand(tp, tz, 0);
        }
    }
    return 0;
}
int EffectHirosShadowScoutResolve(struct CardRef *ref)
{
    /* FAKEMATCH: the redundant outer & 1 puts movs #1 after the bit extraction */
    int count = gDuelPlayers[((ref->player & 1) ^ 1) & 1].handCount;

    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 2; i++) {
            int p = (1 - ref->player) & 1;

            if (i < gDuelPlayers[p].deckCount) {
                u16 p2 = (1 - ref->player) & 1;
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[p2].deck[i]));
                int id2 = id;

                DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x8061 : 0x61, 1, 1, 0);
                if (CARD_TYPE(id) == 0x16) {
                    ShowDestroyedCard(ref->player, id);
                    DiscardHandCard(1 - ref->player, count, 1, 1);
                } else {
                    ShowRevealedCard(ref->player, id2);
                    count++;
                }
            }
        }
    }
    return 0;
}


int EffectInvaderOfTheThroneResolve(struct CardRef *ref)
{
    __asm__("" : : : "r8");
    if (!ref->skip4 && ref->numTargets == 1) {
        int pl = ref->player;
        int rz = ref->zone;
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;

        if (pl != tp) {
            int p = pl & 1;
            struct DuelZone *z = ZB(p, rz);

            if (CARD_ID(CARD_WORD(z->card))) {
                int p2 = 1 & tp;
                struct DuelZone *z2 = ZB(p2, tz);

                if (CARD_ID(CARD_WORD(z2->card)))
                    SwapFieldCards(pl, rz << 8 | pl, tp | tz << 8);
            }
        }
    }
    return 0;
}
int EffectKunaiWithChainResolve(struct CardRef *ref)
{
    if (ref->kind == 0x10) {
        int tp = (u8)ref->pos;
        int tz = ref->pos >> 8;
        int p = 1 & tp;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)) && !(1 & ZFLAGS(z)) && ref->player != tp)
            ChangeBattlePosition(tp, tz, 0, 0);
    }
    if (ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && ref->player == tp)
            EquipCard(ref->player, ref->player | (ref->zone << 8), ref->targets[0]);
        else
            DestroyFieldCard(ref->player, ref->zone, 1);
    }
    return 0;
}
int EffectDestroyAllByTypeResolve(struct CardRef *ref)
{
    int q; /* FAKEMATCH: extra copy of p feeds EffectDestroyByTypeCheck and fixes scheduling */
    if (!ref->skip4) {
        int side;

        for (side = 0; side <= 1; side++) {
            int p;
            u8 pu;
            int i;

            if (side)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (i = 0, pu = (q = p); i <= 4; i++) {
                q = pu;
                if (EffectDestroyByTypeCheck(ref, (u8)i << 8 | q)) {
                    DestroyFieldCard(p, i, 1);
                    OnCardDestroyedByEffect(ref->player, p, i);
                }
            }
        }
    }
    return 0;
}
int GetZoneCardAtk(int player, int zone);
static inline u32 CardAttack32390(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((CARD_STATS(id) >> 9) & 0x1FF) * 10;
}
/* Two-step effect on the opponent's cards: step 0x7F walks the hand one card per call, step 0x80 the five
 * monster zones; cards with ATK over 1500 are destroyed (hypothesis from the calls). */
/* Printed ATK: 0 for types 21-23, 4000 for type 24, else the stats field * 10. The u16 return keeps the
 * 0x80 loop's register order (faceDown r5, id r6). */
static inline u16 MonsterAtk32390(u16 id)
{
    u32 type = ((CARD_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((CARD_STATS(id) >> 9) & 0x1FF) * 10;
}
int EffectCrushCardResolve(struct CardRef *ref)
{
    int opp = 1 - ref->player;
    int i;
    u32 type;

    if (ref->skip4)
        return 0;
    switch (EFF_PHASE) {
    case 0x80:
        for (i = 0; i <= 4; i++) {
            int p = opp & 1;
            struct DuelZone *z = ZB(p, i);
            u16 id = CARD_ID(CARD_WORD(z->card));
            if (id != 0) {
                u32 faceDown = ((u32)ZFLAGS(z) << 30) >> 31;
                DuelCmd_Push(ref->player ? 0x8008 : 8, opp, i << 8, 0);
                if (faceDown == 0) {
                    u32 atk;
                    DuelCmd_Push(opp ? 0x807F : 0x7F, i, 0, 0);
                    atk = MonsterAtk32390(id);
                    if (atk <= 1499) {
                        ShowRevealedCard(opp, id);
                        DuelCmd_Push(opp ? 0x807F : 0x7F, i, 0, 0);
                    } else {
                        ShowDestroyedCard(opp, id);
                        DestroyFieldCardByEffect(opp, i);
                        OnCardDestroyedByEffect(ref->player, opp, i);
                    }
                } else if (GetZoneCardAtk(opp, i) > 1499) {
                    DestroyFieldCardByEffect(opp, i);
                    OnCardDestroyedByEffect(ref->player, opp, i);
                }
            }
        }
        EFF_SIDE = 0;
        return 0x7F;
    case 0x7F:
        if (EFF_SIDE < gDuelPlayers[opp & 1].handCount) {
            u8 side;
            u32 id;
            u32 raw = EFF_SIDE;
            side = raw;
            asm("" : "+r"(raw)); /* FAKEMATCH: load into r0, copy to r2, index from r0 (cf. AiStrategyCyberStein) */
            id = CARD_ID(CARD_WORD(gDuelPlayers[opp & 1].hand[raw]));
            DuelCmd_Push(ref->player ? 0x8008 : 8, opp, (side << 8) | 0xB, 0);
            type = CARD_TYPE(id);
            if (type <= 0x14) {
                u32 atk;
                switch ((int)type) {
                case 21:
                case 22:
                case 23:
                    atk = 0;
                    break;
                case 24:
                    atk = 4000;
                    break;
                default:
                    atk = ((CARD_STATS(id) << 14) >> 23) * 10;
                    break;
                }
                if (atk > 1499) {
                    ShowDestroyedCard(opp, id);
                    DiscardHandCard(opp, EFF_SIDE, 1, 1);
                    return 0x7F;
                }
            }
            /* FAKEMATCH: int-typed call (u32 id, no u16 narrowing) keeps opp's live range short enough for r4 */
            ((void (*)(int, int))ShowRevealedCard)(opp, id);
            EFF_SIDE++;
            return 0x7F;
        }
        return 0x7E;
    default:
        DuelCmd_Push(opp ? 0x8069 : 0x69, 3, 0, 0);
        return 0;
    }
}
int EffectHarpiesFeatherDusterResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 5; i <= 10; i++) {
            int p = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(p, i);

            if (CARD_ID(CARD_WORD(z->card)))
                DestroyFieldCard(1 - ref->player, i, 5);
        }
    }
    return 0;
}
/* Two-step effect on one target: step 0x80 needs a card with flags & 3 == 1 there (FlipFieldCard, then
 * step 0x7F); step 0x7F calls ShowRevealedCard + FlipFieldCard when GetZoneCardDef > 2000, otherwise
 * ShowDestroyedCard, maybe the 0x1640 event (Chain_AddPending) and the usual DestroyFieldCardByEffect/OnCardDestroyedByEffect finish. */
int EffectAcidTrapHoleResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        /* FAKEMATCH: the ROM keeps p in r8 and the 0x5FA constant in r7; as a plain pseudo p outranks
         * the constant in global allocation (2*5/54 vs 3/18) and takes r7. */
        register int p asm("r8");
        struct DuelZone *z;
        int id;

        p = tp;
        asm("" : "+r"(p)); /* FAKEMATCH: keeps the copy p = tp separate from the and (ROM: mov r8,r4 first) */
        p &= 1;
        z = ZB(p, tz);
        id = CARD_ID(CARD_WORD(z->card));

        switch (EFF_PHASE) {
        case 0x80:
            if (id && (ZFLAGS(z) & 3) == 1) {
                FlipFieldCard(tp, tz, 0);
                return 0x7F;
            }
            break;
        case 0x7F:
            if (GetZoneCardDef(tp, tz) > 2000) {
                /* FAKEMATCH: call with an int id; the unit's u16 prototype adds a narrowing that
                 * reorders the argument moves */
                ((void (*)(int, int))ShowRevealedCard)(tp, id);
                FlipFieldCard(tp, tz, 0);
            } else {
                ShowDestroyedCard(tp, id);
                if (HasFlipEffect(CARD_NUMBER(id), 0) != 0 && CountActiveCardsOnField(0, 0x5FA) == 0
                    && CountActiveCardsOnField(1, 0x5FA) == 0) {
                    /* Separate statements: in one expression fold moves the constant next to p << 31. */
                    u32 hi = (u32)p << 31;
                    u32 ev = (tz & 0x1F) << 16 | 0x16400000;

                    Chain_AddPending(hi | ev | id, 0);
                }
                DestroyFieldCardByEffect(tp, tz);
                OnCardDestroyedByEffect(ref->player, tp, tz);
            }
            break;
        }
    }
    return 0;
}
int EffectReverseTrapResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x801C : 0x1C;
        int f = 1 & ~(gUnk_020192E0Tail.flags1ACD >> 5);

        DuelCmd_Push(msg, f, 0, 0);
    }
    return 0;
}
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
/* Sweep effect keyed by the triggering card number: face-up spells (type 0x15) on the player's side are
 * re-activated through FlipFieldCard/ShowRevealedCard, other cards are removed with DestroyFieldCard (hypothesis). */
int EffectFakeTrapResolve(struct CardRef *ref, struct CardRef *src)
{
    int z;
    u16 id; /* function scope, shared by every case: its loop-weighted refs give it r5 in case A too */

    if (ref->skip4 || src == NULL || src->player == ref->player)
        return 0;
    switch (CARD_NUMBER(src->id)) {
    case 0x53:
    case 0xDF:
    case 0x3EC:
    case 0x437:
    case 0x46E:
    case 0x46F: {
        u8 tp = src->targets[0];
        u32 tz = src->targets[0] >> 8;
        id = CARD_ID(CARD_WORD(ZB2(tp & 1, tz)->card));
        if (id == 0)
            return 0;
        if (tp == ref->player && tz == ref->zone)
            return 0;
        if (CARD_TYPE(id) != 0x15)
            return 0;
        if (!ZB2(tp & 1, tz)->flag6_1) {
            FlipFieldCard(tp, tz, 0);
            ShowRevealedCard(ref->player, id);
            FlipFieldCard(tp, tz, 0);
        }
        if (CARD_TYPE(src->id) > 0x14)
            DuelCmd_Push(ref->player ? 0x80B0 : 0xB0, 1, 0, 0);
        return 0;
    }
    case 0x29F:
    case 0x426:
        for (z = 5; z <= 10; z++) {
            id = CARD_ID(CARD_WORD(ZB2(ref->player & 1, z)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!ZB2(ref->player & 1, z)->flag6_1) {
                        FlipFieldCard(ref->player, z, 0);
                        ShowRevealedCard(ref->player, id);
                        FlipFieldCard(ref->player, z, 0);
                    }
                } else {
                    DestroyFieldCard(ref->player, z, 5);
                }
            }
        }
        if (CARD_TYPE(src->id) > 0x14)
            DuelCmd_Push(ref->player ? 0x80B0 : 0xB0, 1, 0, 0);
        return 0;
    case 0x425:
        for (z = 5; z <= 10; z++) {
            id = CARD_ID(CARD_WORD(ZB2(ref->player & 1, z)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!ZB2(ref->player & 1, z)->flag6_1) {
                        FlipFieldCard(ref->player, z, 0);
                        ShowRevealedCard(ref->player, id);
                        FlipFieldCard(ref->player, z, 0);
                    }
                } else {
                    DestroyFieldCard(ref->player, z, 1);
                }
            }
        }
        for (z = 5; z <= 10; z++) {
            if (CARD_WORD(ZB2((1 - ref->player) & 1, z)->card) << 20)
                DestroyFieldCard(ref->player, z, 1);
        }
        DuelCmd_Push(ref->player ? 0x80B0 : 0xB0, 1, 0, 0);
        return 0;
    case 0x42B:
        for (z = 0; z <= 10; z++) {
            id = CARD_ID(CARD_WORD(ZB2(ref->player & 1, z)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!ZB2(ref->player & 1, z)->flag6_1) {
                        FlipFieldCard(ref->player, z, 0);
                        ShowRevealedCard(ref->player, id);
                        FlipFieldCard(ref->player, z, 0);
                    }
                } else {
                    DestroyFieldCard(ref->player, z, 1);
                }
            }
        }
        for (z = 0; z <= 10; z++) {
            if (CARD_WORD(ZB2((1 - ref->player) & 1, z)->card) << 20)
                DestroyFieldCard(ref->player, z, 1);
        }
        DuelCmd_Push(ref->player ? 0x80B0 : 0xB0, 1, 0, 0);
        return 0;
    }
    return 0;
}
