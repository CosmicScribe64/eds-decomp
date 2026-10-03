#include "global.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step code such as 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
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
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
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

/* 0x020192E4 + 0x5F0: a card word in each player's state (stride 0xD64), hypothesis: a "set" spell/trap slot */
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

/* Player life points: gDuelPlayers[p].lifePoints (u16 at +0, stride 0xD64). */
struct PlayerLP {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 count904;
    u8 fusionCount;
    u8 unk6[0x28 - 6];
    struct DuelZone zones[11];      /* +0x28 (0x0201930C) */
    u8 unk5F4[0x684 - 0x28 - 11 * 0x94];
    struct DuelCard hand[80];       /* +0x684 */
    u8 filler[0xD64 - 0x684 - 80 * 4];
};
extern struct PlayerLP gDuelPlayers[2];

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
/* Duel screen state at 0x0201CFB0: the word at +0x82C is passed to ReturnHandCardToDeck. */
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
extern u8 gDuel[];   /* duel global state, byte view */
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
/* Duel global state at 0x020192E0 (byte view gDuel); fields used in this unit. */
struct DuelGlobals {
    u8 unk0[0x1B12];
    u8 b0 : 1;      /* +0x1B12 bit 1: a player index (hypothesis) */
    u8 b1 : 1;
    u8 rest : 6;
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 w1B64;
};
#define DG ((struct DuelGlobals *)gDuel)
void DuelPrompt_PostRandomDiscard(int player, int a, int b);
void DuelPrompt_PostDiscard();
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
    u32 w824;
    u32 w828;
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
int GetGraveyardCardById(int player, int id, int *out);
int GetZoneCardAttribute(int player, int zone);
int FindTrapInHand(int player);
int FindNonFieldMagicInHand(int player);
int FindFreeSpellTrapZone(int player);
extern const u8 gStrDustTornadoSetPrompt[];
extern const u8 gStrDustTornadoSelectCards[];
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gMain;
int IsCardInGraveyard(int player, u32 *packed);
/* 0x0201CF90: a byte whose bits 1-5 are used (hypothesis: a selected position) */
extern u8 gSummonAction;
extern const u8 gStrSenjuAddRitualMonsterPrompt[];
extern const u8 gStrSonicBirdAddRitualMagicPrompt[];
extern const u8 gStrRitualSearchSelectCard[];
int CanSpecialSummon(int player);
extern const u8 gStrRecruiterNoCardsInDeck[];
extern const u8 gStrRecruiterSummonPrompt[];
extern const u8 gStrRecruiterSelectMonster[];
struct S15F00 { u8 unk0[0x1B22]; u16 listPos; };    /* saved list position (hypothesis) */
#define S15F00 ((struct S15F00 *)gAiWork)
int RemoveDeckCardByNumber(int player, int number, int *out);
void CopyDuelCard(void *dst, const void *src);
extern const u8 gStrGiantGermSummonPrompt[];
extern const u8 gStrSameNameSetPrompt[];
struct EffState544 {    /* 0x02017A40 view with a card word at +0x544 */
    u8 unk0[0x544];
    u32 card;
};
#define ES544 ((struct EffState544 *)gChain)
int EffectPainfulChoicePrepare(struct CardRef *ref, int a, int b);
void DuelPrompt_PostData(int player, int a, u16 *ids, int n);
void sub_08019820(int player);
void ShowDestroyedCard(int player, int id);
void Chain_AddPending(u32 a, u32 b);
extern const u8 gStrPainfulChoiceSelect5[];
extern const u8 gStrPainfulChoiceCardsRemaining[];
extern u32 gUnk_02017F84[];     /* 5 card words (hypothesis: revealed/drawn cards) */
struct EffStateCards {          /* 0x02017A40 view: card words at +0x544 */
    u8 unk0[0x544];
    u32 cards[8];
};
#define ESC ((struct EffStateCards *)gChain)
int IsSpecialSummonOnly(int id);
#define EFF_CNT gChain[0x3E2]
struct PF {     /* player state at 0x020192E4, stride 0xD64; fields used here */
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 unk4[0x7C4 - 4];
    u32 deck[80];
    u8 filler[0xD64 - 0x7C4 - 80 * 4];
};
#define PF ((struct PF *)gDuelPlayers)

int IsMonsterZoneFree(int player, int zone);
void ReturnFieldCardToDeck(int player, int zone);
int CountMonstersFiltered(int player, int a, int b);
u16 EffectFairysHandMirrorResolve(struct CardRef *ref, int arg);
int Random(void);
extern const u8 gStrCoinTossMenu[];
extern const u8 gStrSelectDeckMonsterToSummon[];
#define CARD_TYPE11(w) ((((const u32 *)0x08621DE0)[(u32)(w) << 21 >> 21] & 0x1F00000) >> 20)
int __modsi3(int a, int b);
extern const u8 gStrSelectHandMonster[];
extern const u8 gStrSelectHandMagicTrap[];
extern const u8 gStrSelectAnotherHandMagicTrap[];
extern u8 gUnk_02017E22[];
int EffectHandMonsterAndTwoCardsPrepare(struct CardRef *ref, struct CardRef *card, int a);
extern const u8 gStrSelectGraveMonsterToDeck[];
extern const u8 gStrSelectGraveMonsterToHand[];
extern const u8 gStrPayToReviveNextStandbyPrompt[];
extern const u16 gUnk_086248EE[];
/* Monster level as the game computes it: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
#define CARD_LEVEL(id, r)                                         switch ((int)CARD_TYPE(id)) {                                 case 0x15:                                                    case 0x16:                                                    case 0x17:                                                        r = 0;                                                        break;                                                    case 0x18:                                                        r = 10;                                                       break;                                                    default:                                                          r = (CARD_STATS(id) & 0x1E000000) >> 25;                      break;                                                    }
struct ListViewH { u8 unk0[0xC]; struct { u16 lo; u16 hi; } c[0x80]; };
#define LVH ((struct ListViewH *)&gCardListView)
void BanishTopDeckCards(int player, int a);
extern const u8 gStrSelectGraveMagicToDeck[];
struct ZoneB1 { u8 unk0; u8 lo : 6; u8 f6 : 1; u8 hi : 1; };   /* byte +1 of a zone: bit 6 = a temporary mark (hypothesis) */
struct EffState3E8 { u8 unk0[0x3E8]; u16 w[2]; };
#define ES3E8 ((struct EffState3E8 *)gChain)
extern const u8 gStrPlaceOnDeckTopPrompt[];
extern const u16 gUnk_086243E8[];
void FlipFieldCard(int player, int zone, int a);
int IsEffectMonster(int id);
extern const u8 gStrEquipFromGraveyardPrompt[];
int CountGraveyardCardsByNumber(int player, int number);
void TriggerForcedRequisition(int player, u16 a);
struct EffStateH { u8 unk0[0x544]; u16 lo; u16 hi; };
#define ESH ((struct EffStateH *)gChain)
extern const u8 gStrSelectEquipTarget[];
extern const u16 gUnk_086247C8[];
int FindDeckCardByNumber(int player, int a, int b);
int CanPlaceSpellTrapCard(int player, u16 a);
void EquipCard(int player, u16 a, u16 b);

int EffectOwnSkullOrThunderCheck(struct CardRef *ref, u16 pos);
extern const u16 gUnk_086243C8[];
extern const u8 gStrAddFromDeckToHandPrompt[];
extern const u8 gStrSelectStatsSourceMonster[];



int EffectBanishOwnMonsterUntilEndPhaseResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;

            if (CARD_ID(CARD_WORD(ZB2(tp & n, tz)->card)))
                DuelCmd_Push(tp ? 0x80A9 : 0xA9, tz, 1, 0);
        }
    }
    return 0;
}
int EffectBlockTwoMonsterZonesResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int i;
        u16 *t = ref->targets;

        for (i = 0; i <= 1; t++, i++) {
            int tz = *t >> 8;

            if (!CARD_ID(CARD_WORD(ZB2(1 & *(u8 *)t, tz)->card)))
                QueueAddZoneLink(ref->player, ref->player | ref->zone << 8, *t, 2);
        }
    }
    return 0;
}
int EffectNegateMagicUnlessDiscardResolve(struct CardRef *ref, struct CardRef *card)
{
    if (!ref->skip4) {
        u8 *es = gChain;
        int ph = es[0x3E0];

        switch ((short)ph) { /* FAKEMATCH: the (short) cast changes no semantics and gives the ROM's r2/`adds r0,r2,#0` codegen. */
        case 0x80:
            if (gDuelPlayers[(1 - ref->player) & 1].handCount == 0)
                return 0x7E;
            DuelPrompt_Post(1 - ref->player, 0x11, 0, 0);
            return 0x7F;
        case 0x7F:
            if (DG->w1B64 != 0)
                return 0;
            es[0x3E0] = ph - 1;
        case 0x7E:
            if (card == 0)
                return 0;
            if (CARD_TYPE(card->id) == 0x16)
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int EffectDiscardMagicDamageResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        LoseLifePoints(1 - ref->player, 500);
    return 0;
}
int EffectFlipSetMonsterToAttackResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->kind == 8) {
        int tp = (u8)ref->pos;
        int tz = ref->pos >> 8;

        if (tp != ref->player) {
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 1) && !(ZFLAGS(z) & 2))
                ChangeBattlePosition(tp, tz, 1, 0);
        }
    }
    return 0;
}
int EffectPlaceParasiteParacideOnDeckResolve(struct CardRef *ref)
{
    char buf[0x80];

    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            if (1 & ((u8 *)ref)[2]) {
                gTextBox.flag14 = 1;
                return 0x7F;
            }
            FormatStr(buf, gStrPlaceOnDeckTopPrompt, gCardNames + gUnk_086243E8[0] * 64);
            TextBoxOpen(0x206, 0x613, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        } else {
            u16 *w;
            register u8 *state __asm__("r1");
            register int offset __asm__("r2");

            if (gTextBox.flag14 == 0)
                return 0;
            state = gChain;
            offset = 0x3E8;
            /* Preserve the ROM base/offset registers and add operand order. */
            __asm__("" : "+r"(offset), "+r"(state));
            w = (u16 *)(state + offset);
            if (RemoveDeckCardByNumber(ref->player, 0x2FA, (int *)w) != -1) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x806A : 0x6A, w[0], w[1], 0);
            }
        }
    }
    return 0;
}
int EffectChangeTargetPositionResolve(struct CardRef *ref)
{
    int n = 7 & ((u8 *)ref)[0xA];

    if (n == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;

        if (CARD_ID(CARD_WORD(ZB2(tp & n, tz)->card)))
            ChangeBattlePosition(tp, tz, 0, 0);
    }
    return 0;
}
int EffectDestroySetEffectMonsterResolve(struct CardRef *ref)
{
    u16 cardId;
    int n = 7 & ((u8 *)ref)[0xA];

    if (n == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & n;
        struct DuelZone *z = ZB(p, tz);
        int id = CARD_ID(CARD_WORD(z->card));
        u16 id2 = id;

        if (id > 0 && (ZFLAGS(z) & 3) == 1) {
            id2 = id; /* Narrow the id before the calls, as in the ROM. */
            FlipFieldCard(tp, tz, 0);
            if (IsEffectMonster(id2)) {
                ShowDestroyedCard(tp, id2);
                ((struct ZoneB1 *)z)->f6 = 1;
                DestroyFieldCardByEffect(tp, tz);
                OnCardDestroyedByEffect(ref->player, tp, tz);
                ((struct ZoneB1 *)z)->f6 = 0;
            } else {
                ShowRevealedCard(tp, cardId = id2);
                FlipFieldCard(tp, tz, 0);
            }
        }
    }
    return 0;
} /* 0x0803A9C8 size 0xB8 */
int EffectDestroyOpponentLevel4Resolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;
        u32 lvl;

        for (i = 0; i <= 4; i++) {
            u16 id = CARD_ID(CARD_WORD(ZB2((1 - ref->player) & 1, i)->card));

            if ((ZB2((1 - ref->player) & 1, i)->flag6_1) && id != 0) {
                CARD_LEVEL(id, lvl);
                if (lvl == 4) {
                    DestroyFieldCardByEffect(1 - ref->player, i);
                    OnCardDestroyedByEffect(ref->player, 1 - ref->player, i);
                }
            }
        }
    }
    return 0;
}
int EffectPayToReviveNextStandbyResolve(struct CardRef *ref)
{
    char buf[0x80];

    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            FormatStr(buf, gStrPayToReviveNextStandbyPrompt, gCardNames + gUnk_086248EE[0] * 64);
            TextBoxOpen(0x206, 0x613, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        } else {
            if (gTextBox.flag14 == 0)
                return 0;
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8043 : 0x43, 1000, 0, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x804B : 0x4B, 1, 0, 0);
        }
    }
    return 0;
}
/* CardRef.targets really has three slots (+0xC, +0xE, +0x10); indexing through this view keeps the base
 * pointer first in the address add, as in the ROM. */
struct Tgt3AC34 { u16 v[3]; };
/* Card word i of player p's list at 0x02019968, with the base added last as in the ROM. */
#define HW_AC34(p, i) (*(u32 *)((p) * 0xD64 + (i) * 4 + (u32)gDuelHands))
int EffectHandRouletteSummonResolve(struct CardRef *ref, struct CardRef *card)
{
    int i;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (EffectHandMonsterAndTwoCardsPrepare(ref, card, 0) == 0)
                return 0;
            TextBoxOpen(0x206, 0x613, 0xB, gStrSelectHandMonster);
        ret7F:
            return 0x7F;
        case 0x7F: {
            int id;
            int p;

            if (DuelCursor_PickTarget(1) == 0)
                goto ret7F;
            p = ref->player & 1; /* the `& 1` leaves the constant 1 in a register that the later `1 & byte` reuses */
            id = CARD_ID(HW_AC34(p, DSV->w82C));
            if (CARD_TYPE(id) <= 0x14 && IsSpecialSummonOnly(id) == 0) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (u8)DSV->w82C << 8 | 0xB, 0);
                ref->targets[0] = DSV->w82C;
                return 0x7E;
            }
            PlaySE(3);
            goto ret7F;
        }
        case 0x7E:
            TextBoxOpen(0x206, 0x613, 0xB, gStrSelectHandMagicTrap);
        ret7D:
            return 0x7D;
        case 0x7D:
            if (DuelCursor_PickTarget(1) == 0)
                goto ret7D;
            if (CARD_TYPE11(HW_AC34(ref->player & 1, DSV->w82C)) > 0x14) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (u8)DSV->w82C << 8 | 0xB, 0);
                ref->targets[1] = DSV->w82C;
                return 0x7C;
            }
            PlaySE(3);
            goto ret7D;
        case 0x7C:
            TextBoxOpen(0x206, 0x613, 0xB, gStrSelectAnotherHandMagicTrap);
        ret7B:
            return 0x7B;
        case 0x7B:
            if (DuelCursor_PickTarget(1) == 0)
                goto ret7B;
            if (CARD_TYPE11(HW_AC34(ref->player & 1, DSV->w82C)) > 0x14 && DSV->w82C != ref->targets[1]) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (u8)DSV->w82C << 8 | 0xB, 0);
                ref->targets[2] = DSV->w82C;
                EFF_SIDE = 0x10;
                EFF_CNT = 0;
                return 0x7A;
            }
            PlaySE(3);
            goto ret7B;
        case 0x7A: {
            struct Tgt3AC34 *t = (struct Tgt3AC34 *)ref->targets;

            do {
                i = Random() % 3;
            } while (i == gUnk_02017E22[0]);
            EFF_CNT = i;
            if (EFF_SIDE != 0) {
                EFF_SIDE--;
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8009 : 9, 0xB, t->v[EFF_CNT], 0);
                return 0x7A;
            }
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (u8)t->v[EFF_CNT] << 8 | 0xB, 0);
            return 0x79;
        }
        case 0x79: {
            u8 *ex;

            for (i = 0, ex = gUnk_02017E22; i <= 2; i++) { /* ex set after i = 0, like a hoisted invariant */
                if (i != *ex)
                    DiscardHandCard(ref->player, ref->targets[i], 0, 0);
            }
            return 0x78;
        }
        case 0x78:
            if (EFF_CNT == 0) {
                u32 *row = gDuelHands[ref->player & 1].w;
                u32 *w = row + ref->targets[0]; /* separate row temp: (p * 0xD64 + base) + t * 4 order */

                ShowRevealedCard(ref->player, CARD_ID(*w));
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80C2 : 0xC2, ((u16 *)w)[0], ((u16 *)w)[1], 0);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80CA : 0xCA, 0, 0, 0);
                QueueSpecialSummonChoosePosition(ref->player, w, 1, 0);
            } else {
                ShowDestroyedCard(ref->player, CARD_ID(HW_AC34(ref->player, ref->targets[EFF_CNT])));
                DiscardHandCard(ref->player, ref->targets[EFF_CNT], 0, 1);
            }
            return 0x77;
        }
    }
    return 0;
}
int EffectCurseFaceUpMagicResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tz = ref->targets[0] >> 8;
            int p = n & *(u8 *)&ref->targets[0];
            struct DuelZone *z = ZB(p, tz);
            int id = CARD_ID(CARD_WORD(z->card));

            if (id && (ZFLAGS(z) & 2) && CARD_TYPE(id) == 0x16) {
                QueueAddZoneLink(ref->player, ref->player | ref->zone << 8, ref->targets[0], 2);
                return 0;
            }
        }
        ((u8 *)ref)[4] |= 8;
    }
    return 0;
}
int EffectCoinTossZeroAttackerResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                TextBoxOpen(0x206, 0x613, 0xB, gStrCoinTossMenu);
                TextBoxSetMenu(2, 0, 0);
            } else {
                gTextBox.flag14 = Random() & 1;
            }
            return 0x7F;
        case 0x7F: {
            u8 r = Random() & 1;

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gTextBox.flag14, r, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            if (r == gTextBox.flag14)
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x803A : 0x3A, 0, 0, 0);
            return 0xA;
        }
        }
    }
    return 0;
}
int EffectRedirectAttackResolve(struct CardRef *ref)
{
    u8 targetCount;

    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        targetCount = n;
        if (targetCount == 1) {
            int tz = ref->targets[0] >> 8;
            int p = n & *(u8 *)&ref->targets[0];
            struct DuelZone *z = ZB(p, tz);
            int id = CARD_ID(CARD_WORD(z->card));

            if ((ZFLAGS(z) & 2) && id && CARD_NUMBER(id) == 0x57D) {
                int b = ((u8 *)ref)[2];

                DuelCmd_Push((n & b) ? 0x8008 : 8, ref->player, tz << 8, 0);
                DuelCmd_Push((n & ((u8 *)ref)[2]) ? 0x8038 : 0x38, ref->targets[0], 1, 0);
            }
        }
    }
    return 0;
}
int EffectRecoverGraveMonsterResolve(struct CardRef *ref)
{
    switch (EFF_PHASE) {
    case 0x80:
        if (CollectEffectTargets(ref->player, 0x58D, 0) == 0)
            return 0;
        switch (CARD_NUMBER(ref->id)) {
        case 0x596:
        case 0x599:
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectGraveMonsterToDeck);
            return 0x7F;
        case 0x58D:
        case 0x5A4:
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectGraveMonsterToHand);
            return 0x7F;
        }
        return 0;
    case 0x7F:
        CardListView_Open(ref->player, -1, 0x58D, 0);
        return 0x7E;
    case 0x7E:
        switch (CARD_NUMBER(ref->id)) {
        case 0x596:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D0 : 0xD0, (u32)LVH->c[gCardListView.row + gCardListView.top].lo << 20 >> 20, 0, 0);
            return 0x78;
        case 0x599:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D1 : 0xD1, (u32)LVH->c[gCardListView.row + gCardListView.top].lo << 20 >> 20, 0, 0);
            return 0x78;
        case 0x5A4: {
            u16 *w = (u16 *)&gCardListView.cards[gCardListView.row + gCardListView.top];

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D2 : 0xD2, w[0], w[1], 0);
            return 0x78;
        }
        case 0x58D: {
            u16 *w = (u16 *)&gCardListView.cards[gCardListView.row + gCardListView.top];

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80DB : 0xDB, w[0], w[1], 0);
            return 0x78;
        }
        }
        return 0;
    }
    return 0;
}
int EffectTributeTwoMonstersResolve(struct CardRef *ref)
{
    if (!ref->skip4 && CountActiveCardsOnField(0, 0x58A) <= 0 && CountActiveCardsOnField(1, 0x58A) <= 0 && ref->numTargets == 2) {
        int i;
        register u16 *targets __asm__("r3");
        u16 *t;

        i = 0;
        targets = ref->targets;
        for (; i <= 1; i++) {
            register int off __asm__("r0") = i * 2;
            u16 *e;
            int tp, tz;

            /* FAKEMATCH: initialized offset constraint retains indexed checks
             * with the r3 base. The later call loop uses its own walking pointer;
             * targets is no longer needed once that pointer is assigned. */
            asm("" : "+r"(off));
            e = (u16 *)((u8 *)targets + off);
            tp = *(u8 *)e;
            tz = *e >> 8;

            if (tp != ref->player)
                return 0;
            if (!CARD_ID(CARD_WORD(ZB2(1 & tp, tz)->card)))
                return 0;
        }
        t = targets;
        for (i = 0; i <= 1; t++, i++)
            TributeMonster((u8)*t, *t >> 8);
        switch (CARD_NUMBER(ref->id)) {
        case 0x598:
            LoseLifePoints(1 - ref->player, 1200);
            break;
        case 0x5A2:
            GainLifePoints(ref->player, 1000);
            break;
        }
    }
    return 0;
}
int EffectBanishDeckTopDamageResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        BanishTopDeckCards(ref->player, 3);
        LoseLifePoints(1 - ref->player, 800);
    }
    return 0;
}
int EffectDamageOpponent800Resolve(struct CardRef *ref)
{
    if (!ref->skip4)
        LoseLifePoints(1 - ref->player, 800);
    return 0;
}
int EffectTributeRecoverGraveMagicResolve(struct CardRef *ref)
{
    switch (EFF_PHASE) {
    case 0x80:
        if (CollectEffectTargets(ref->player, 0x59F, 0) == 0)
            return 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectGraveMagicToDeck);
        return 0x7F;
    case 0x7F:
        CardListView_Open(ref->player, -1, 0x59F, 0);
        return 0x7E;
    case 0x7E:
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D1 : 0xD1, (u32)LVH->c[gCardListView.row + gCardListView.top].lo << 20 >> 20, 0, 0);
        return 0x78;
    }
    return 0;
}
