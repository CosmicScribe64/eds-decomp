#include "global.h"

/*
 * Duel card-effect executors: int f(struct CardRef *ref), returning 0 (or 0x7F/0x80/0x92 style codes).
 * See wiki/functions/code-08031bc8.md.
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


int EffectDiscardHandsAndRedrawResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            for (i = 0; i <= 1; i++) {
                int p;
                int k;

                if (i)
                    p = 1 - ref->player;
                else
                    p = ref->player;
                ref->targets[p] = gDuelPlayers[p & 1].handCount;
                for (k = 0; k < gDuelPlayers[p & 1].handCount; k++) {
                    if (((u32)CARD_WORD(gDuelPlayers[p & 1].hand[k]) << 19 >> 31) != p)
                        ref->targets[p]--;
                    DiscardHandCard(p, 0, i, 1);
                }
            }
            for (i = 0; i <= 1; i++) {
                int p;

                if (i)
                    p = 1 - ref->player;
                else
                    p = ref->player;
                DrawCards(p, ref->targets[p]);
            }
            return 0x7F;
        }
        case 0x7F:
            TriggerForcedRequisition(ref->player, ref->targets[ref->player]);
            return 0x7E;
        case 0x7E:
            TriggerForcedRequisition(1 - ref->player, ref->targets[1 - ref->player]);
            return 0x7D;
        }
    }
    return 0;
}
int EffectSummonTokensInFreeZonesResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 4; i++) {
            if (IsMonsterZoneFree(ref->player, i))
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80A3 : 0xA3, (u8)i | ref->zone << 8, 1, 0);
        }
    }
    return 0;
}
int EffectAddGazelleFromDeckResolve(struct CardRef *ref)
{
    char buf[0x80];

    switch (EFF_PHASE) {
    case 0x80:
        if (CollectEffectTargets(ref->player, 0x4D8, 0) == 0)
            return 0;
        if (1 & ((u8 *)ref)[2])
            goto step7E;
        FormatStr(buf, gStrAddFromDeckToHandPrompt, gCardNames + gUnk_086243C8[0] * 64);
        TextBoxOpen(0x206, 0x712, 0xB, buf);
        TextBoxSetMenu(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gTextBox.flag14 == 0)
            return 0;
    step7E:
        return 0x7E;
    case 0x7E:
        AddDeckCardToHand(ref->player, 0x2EA);
        return 0x7D;
    }
    return 0;
}
/* The card-name table is addressed as an integer constant (like CARD_NUMBER): the const_int stays in the add and
 * reload loads it into the rotating reload register (r0 in case 0x7F, r3 in 0x7E), which the extern symbol does not. */
#define NAMES_0822C720 ((const u8 *)0x0822C720)
int EffectEquipSelfFromGraveyardResolve(struct CardRef *ref)
{
    char buf[0x80];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            if (CountGraveyardCardsByNumber(ref->player, 0x4DA) == 0)
                return 0;
            if (FindFreeSpellTrapZone(ref->player) == -1)
                return 0;
            for (i = 0; i <= 1; i++) {
                int j;

                for (j = 0; j <= 4; j++) {
                    struct DuelZone *z = ZB2(i & 1, j);

                    if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                        goto ret7F;
                }
            }
            return 0;
        }
        case 0x7F:
            FormatStr(buf, gStrEquipFromGraveyardPrompt, NAMES_0822C720 + ref->id * 64);
            TextBoxOpen(0x206, 0x712, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gTextBox.flag14 == 0)
                return 0;
            FormatStr(buf, gStrSelectEquipTarget, NAMES_0822C720 + ref->id * 64);
            TextBoxOpen(0x206, 0x712, 0xB, buf);
        ret7D:
            return 0x7D;
        case 0x7D: {
            u32 a;
            u32 c;
            int r;
            u16 *w;

            if (DuelCursor_PickTarget(0xE000E0) != 0) {
                a = DSV->w824;
                c = DSV->w828 + DSV->w82C;
                r = FindFreeSpellTrapZone(ref->player);
                PlaySE(1);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, (u16)DSV->w824, (u8)DSV->w828 | (u8)DSV->w82C << 8, 0);
                GetGraveyardCardById(ref->player, ref->id, (int *)(w = &ESH->lo));
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, w[0], w[1], 0);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8077 : 0x77, (u8)r | 0x100, w[0], w[1]);
                EquipCard(ref->player, ref->player | (u8)r << 8, (u8)a | (u8)c << 8);
                return 0x64;
            }
            if (!(gMain.h6 & 2))
                goto ret7D;
        ret7F:
            return 0x7F;
        }
        }
    }
    return 0;
}
int EffectSummonFourTokensResolve(struct CardRef *ref)
{
    if (!ref->skip4 && CountFreeMonsterZones(ref->player) > 3) {
        int n;
        int i;

        for (i = 0, n = 0; i <= 4 && n <= 3; i++) {
            if (IsMonsterZoneFree(ref->player, i)) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80A3 : 0xA3, (u8)i | ref->zone << 8, 2, 0);
                n++;
            }
        }
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
    }
    return 0;
}
int EffectGainOpponentMonsterStatsResolve(struct CardRef *ref)
{
    switch (EFF_PHASE) {
    case 0x80:
        if (CountMonstersFiltered(1 - ref->player, 1, 0) == 0)
            return 0;
        TextBoxOpen(0x206, 0x613, 0xB, gStrSelectStatsSourceMonster);
    ret7F:
        return 0x7F;
    case 0x7F:
        if (DuelCursor_PickTarget(0xE00000) == 0)
            goto ret7F;
    {
        int praw = DSV->w824;
        int zone;
        int id;

        zone = DSV->w82C;
        id = (*(u32 *)((u32)gDuelZones + zone * 0x94 + (praw & 1) * 0xD64) << 20) >> 20;
        /* FAKEMATCH: finish the initialized id extraction before loading
         * ref->player, preserving the ROM's selection-read schedule. */
        __asm__("" : : "r"(id));

        QueueAddZoneLink(ref->player, id, ref->player | ref->zone << 8, 8);
    }
        return 0x78;
    }
    return 0;
}
#define EFF_FN (*(u8 (**)(struct CardRef *, int))(gChain + 0x4F8))
int EffectReflectPlayerMagicResolve(struct CardRef *ref, struct CardRef *card)
{
    if (ref->skip4)
        return 0;
    if (EFF_PHASE == 0x80) {
        MemCopy16(&ES->cur, card, 0x14);
        { int np = 1 - ES->cur.player; ES->cur.player = np; }
        EFF_FN = gCardEffects[FindCardEffect(ES->cur.id)].fn;
    }
    switch (CARD_NUMBER(card->id)) {
    case 0x151 ... 0x159:
    case 0x3C8:
    case 0x3EE:
    case 0x3EF:
    case 0x3F2:
    case 0x401:
    case 0x40F:
    case 0x42E:
    case 0x42F:
    case 0x439:
        {
            u8 *es = (u8 *)0x02017A40;

            es[0x3E0] = (*(u8 (**)(struct CardRef *, int))(es + 0x4F8))((struct CardRef *)(es + 0x4E4), 0);
            if (es[0x3E0] == 0) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
                return 0;
            }
            return es[0x3E0];
        }
    }
    return 0;
}
int EffectDestroyWeakerDefenseResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1 && EffectOwnSkullOrThunderCheck(ref, ref->targets[0])) {
        int tp = (u8)ref->targets[0];
        int v = GetZoneCardAtk(tp, ref->targets[0] >> 8);
        int j;
        int p;

        j = 0;
        p = 1 - tp;
        for (; j <= 4; j++) {
            struct DuelZone *z = ZB(p & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && GetZoneCardDef(p, j) < v && IsZoneTargetable(p, j)) {
                DestroyFieldCardByEffect(p, j);
                OnCardDestroyedByEffect(ref->player, p, j);
            }
        }
    }
    return 0;
}
int EffectTakeControlOfMachineResolve(struct CardRef *ref)
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

                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && r != -1 && GetZoneCardType(tp, tz) == 7) {
                    MoveFieldCard(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                    QueueAddZoneLink(ref->player, ref->id, ref->player | (u8)r << 8, 3);
                }
            }
        }
    }
    return 0;
}
int EffectEachPlayerDiscardsResolve(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct PlayerLP *pl = gDuelPlayers;

            if (pl[ref->player].handCount != 0)
                DuelPrompt_PostDiscard(ref->player, 1, 0, 0);
            return 0x7F;
        }
        case 0x7F: {
            struct PlayerLP *pl = gDuelPlayers;

            if (pl[(1 - ref->player) & 1].handCount != 0)
                DuelPrompt_PostDiscard(1 - ref->player, 1, 0, 1);
            return 0x7E;
        }
        }
    }
    return 0;
}
struct EffStateW542 { u8 unk0[0x542]; u16 w542; };
#define ESW ((struct EffStateW542 *)gChain)
int EffectEquipFromDeckAndSwitchControlResolve(struct CardRef *ref)
{
    char buf[0x80];

    switch (EFF_PHASE) {
    case 0x80: {
        int i;
        int num;

        if (FindDeckCardByNumber(ref->player, num = 0x4EA, 0x3E7) == -1)
            return 0;
        if (!CanPlaceSpellTrapCard(ref->player, ((const u16 *)0x08623DF4)[num]))
            return 0;
        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB2(i & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                    goto found;
            }
        }
        return 0;
    }
    case 0x7F:
        FormatStr(buf, gStrSelectEquipTarget, gCardNames + gUnk_086247C8[0] * 64);
        TextBoxOpen(0x206, 0x613, 0xB, buf);
    ret7E:
        return 0x7E;
    case 0x7E: {
        int p;
        int r;
        u32 a;
        u32 b;
        int pos;
        u16 *w;

        if (DuelCursor_PickTarget(0xE000E0) == 0)
            goto ret7E;
        p = ref->player;
        r = FindFreeSpellTrapZone(p);
        a = DSV->w824;
        b = DSV->w82C;
        DuelCmd_Push(p ? 0x8077 : 0x77, (u8)r | 0x100, (w = &ESH->lo)[0], w[1]);
        EquipCard(ref->player, p | (u8)r << 8, pos = (u8)a | (u8)b << 8);
        ESW->w542 = pos;
        return 0x7D;
    }
    case 0x7D: {
        int p2 = 1 - (u8)ESW->w542;
        int t = FindFreeMonsterZone(p2);

        if (t >= 0)
            MoveFieldCard(ref->player, ESW->w542, (u8)p2 | (u8)t << 8);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        return 0x7C;
    }
    }
    goto ret0;
found:
    RemoveDeckCardByNumber(ref->player, 0x4EA, (int *)gUnk_02017F84);
    return 0x7F;
ret0:
    return 0;
}
int EffectReturnSelfToDeckResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        ReturnFieldCardToDeck(ref->player, ref->zone);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
    }
    return 0;
}
int EffectCoinTossDrawToFiveResolve(struct CardRef *ref)
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
            int r = Random() & 1;

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gTextBox.flag14, r, 0);
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            if (r == gTextBox.flag14) {
                if (gDuelPlayers[1 & ref->player].handCount <= 4)
                    DrawCards(ref->player, 5 - gDuelPlayers[1 & ref->player].handCount);
            } else {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8046 : 0x46, 1, 0, 0);
            }
            return 0xA;
        }
        }
    }
    return 0;
}
int EffectDestroyFieldMagicsResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            if (CARD_ID(CARD_WORD(gDuelFieldZone[i & 1].card)))
                DestroyFieldCard(i, 10, 1);
        }
    }
    return 0;
}
int EffectLockMagicTrapResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x804A : 0x4A, 2, 0, 0);
        DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x804A : 0x4A, 2, 0, 0);
    }
    return 0;
}
int EffectDoubleMachineAtkResolve(struct CardRef *ref)
{
    int i;

    for (i = 0; i <= 4; i++) {
        if (CARD_ID(CARD_WORD(ZB2(1 & ref->player, i)->card)) && (ZB2(1 & ref->player, i)->flag6_1)
            && GetZoneCardType(ref->player, i) == 7 && IsZoneTargetable(ref->player, i))
            QueueAddZoneLink(ref->player, ref->id, ref->player | (u8)i << 8, 3);
    }
    return 0;
}
int EffectReturnMonsterAndRedrawResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;

            if (CARD_ID(CARD_WORD(gDuelPlayers[tp & n].zones[tz].card))) {
                int count;
                int i;

                ReturnFieldCardToDeck(tp, tz);
                count = gDuelPlayers[n = ref->player & n].handCount; /* FAKEMATCH: reuse the dead target count. */
                for (i = 0; i < count; i++)
                    ReturnHandCardToDeck(ref->player, 0, 1);
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
                DrawCards(ref->player, count);
            }
        }
    }
    return 0;
} /* 0x0803A410 size 0xA8 */
u16 EffectRedirectTargetResolve(struct CardRef *ref, int arg)
{
    if (ref->skip4)
        return 0;
    if (ref->kind == 0x10) {
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (ref->targets[0] >> 8) << 8, 0);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8038 : 0x38, ref->targets[0], 1, 0);
        return 0;
    }
    return EffectFairysHandMirrorResolve(ref, arg);
}
int EffectSummonInsectFromDeckResolve(struct CardRef *ref)
{
    u32 *card = &gCardListView.cards[gCardListView.row + gCardListView.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (ref->numTargets == 1 && CollectEffectTargets(ref->player, 0x526, ref->targets[0])) {
                TextBoxOpen(0x206, 0x712, 0xB, gStrSelectDeckMonsterToSummon);
                return 0x7F;
            }
            break;
        case 0x7F:
            CardListView_Open(ref->player, -1, 0x526, ref->targets[0]);
            return 0x7E;
        case 0x7E:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7D;
        case 0x7D:
            QueueSpecialSummonChoosePosition(ref->player, &gCardListView.cards[gCardListView.row + gCardListView.top], 0, 0x20);
            return 0x7C;
        case 0x7C:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
