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
    u8 filler[0xD64 - 6];
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
int EffectGiantTrunadeResolve(struct CardRef *ref)
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
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)))
                    ReturnFieldCardToHand(p, j, 9);
            }
        }
    }
    return 0;
}
int EffectPainfulChoiceResolve(struct CardRef *ref, int arg)
{
    u16 ids[16];
    char buf[0x80];
    int i;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (EffectPainfulChoicePrepare(ref, arg, 0) == 0)
                return 0;
            TextBoxOpen(0x206, 0x512, 0xB, gStrPainfulChoiceSelect5);
            EFF_SIDE = 5;
            return 0x7F;
        case 0x7F:
            CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7E;
        case 0x7E: {
            u32 *card;

            EFF_SIDE--;
            card = &gCardListView.cards[gCardListView.row + gCardListView.top];
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            CopyDuelCard(&ESC->cards[EFF_SIDE], card);
            if (EFF_SIDE != 0) {
                FormatInt(buf, gStrPainfulChoiceCardsRemaining, EFF_SIDE);
                TextBoxOpen(0x206, 0x512, 0xB, buf);
                return 0x7F;
            }
            return 0x7D;
        }
        case 0x7D:
            for (i = 0; i <= 4; i++)
                ids[i] = CARD_ID(gUnk_02017F84[i]);
            DuelPrompt_PostData(1 - ref->player, 12, ids, 5);
            return 0x7C;
        case 0x7C: {
            int first = 1;

            for (i = 0; i <= 4; i++) {
                u32 *pw = &gUnk_02017F84[i];
                u16 *ph = (u16 *)pw;

                if (CARD_ID(*pw) == DG->w1B64 && first != 0) {
                    /* the unit declares sub_08019820(int); the real one is (int player, u16 id) */
                    ((void (*)(int, u16))sub_08019820)(ref->player, CARD_ID(*pw));
                    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80CB : 0xCB, ph[0], ph[1], 0);
                    first = 0;
                } else {
                    u32 w;
                    u32 id;
                    u32 num;

                    ShowDestroyedCard(ref->player, CARD_ID(*pw));
                    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D7 : 0xD7, ph[0], ph[1], 0);
                    w = *pw;
                    id = CARD_ID(w);
                    /* an int-width temporary keeps the compare in SImode, so loop.c does not hoist 0x4DA */
                    num = CARD_NUMBER(id);
                    if (num == 0x4DA)
                        Chain_AddPending(0x3C600000 | (((w << 19) >> 31) & 1) << 31 | id, 0);
                }
            }
            return 0x64;
        }
        default:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        }
    }
    return 0;
}
int EffectTimeSealResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        DuelCmd_Push(!(1 & ((u8 *)ref)[2]) ? 0x8044 : 0x44, 0, 0, 0);
    return 0;
}
int EffectGraverobberResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int out;
        int opp = 1 - ref->player;
        int id = (u32)ref->targets[0] << 20 >> 20;

        if (GetGraveyardCardById(opp, id, &out))
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D5 : 0xD5, id, 0, 0);
    }
    return 0;
}
int EffectGiftOfTheMysticalElfResolve(struct CardRef *ref)
{
    int count = 0;
    struct DuelZone **zonePtr; /* FAKEMATCH: taking the local address preserves register allocation. */

    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                zonePtr = &z;
                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(*zonePtr) & 2))
                    count++;
            }
        }
        if (count > 0)
            GainLifePoints(ref->player, count * 300);
    }
    return 0;
} /* 0x08036570 size 0x80 */
static inline u32 EffectHandWord(int player, int index)
{
    int idx4 = index * 4;
    int poff = player * 0xD64;

    return *(u32 *)(idx4 + poff + (u32)gDuelHands);
}
int EffectDustTornadoResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int n = 7 & ((u8 *)ref)[0xA];

            if (n == 1) {
                u8 tp = ref->targets[0];
                int tz = ref->targets[0] >> 8;
                int p = tp & n;
                struct DuelZone *z = ZB(p, tz);

                if (CARD_ID(CARD_WORD(z->card))) {
                    DestroyFieldCard(tp, tz, 1);
                    return 0x7F;
                }
            }
            break;
        }
        case 0x7F: {
            int r;

            if (gDuelPlayers[ref->player].handCount == 0) {
                /* FAKEMATCH: an empty repeated test ends the extended block, so
                   the player bit is shifted again for FindTrapInHand. */
                if (gDuelPlayers[ref->player].handCount) {
                }
                break;
            }
            r = FindTrapInHand(ref->player);
            {
                int fail = -1;

                /* Compiler hint: retain r across the second check and rematerialize -1 later. */
                __asm__("" : "+r"(fail));
                if (r == fail && FindNonFieldMagicInHand(ref->player) == r)
                    break;
            }
            if (FindFreeSpellTrapZone(ref->player) == -1)
                break;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDustTornadoSetPrompt);
            TextBoxSetMenu(1, 0, 0);
            return 0x7E;
        }
        case 0x7E:
            if (gTextBox.flag14 == 0)
                break;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDustTornadoSelectCards);
            return 0x7D;
        case 0x7D:
            if (DuelCursor_PickTarget(1) != 0) {
                int p = ref->player;
                u32 w = EffectHandWord(p, DSV->w82C);
                u16 id = CARD_ID(w);
                int kind;
                int t;

                if (CARD_TYPE(id) <= 0x14)
                    goto fail;
                t = CARD_TYPE(id);
                switch (t) {
                case 0x15:
                case 0x16:
                    kind = (CARD_STATS(id) & 0xE0000) >> 17;
                    break;
                default:
                    kind = 0;
                }
                if (kind == 2)
                    goto fail;
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80C5 : 0xC5, id,
                             (DSV->w82C & 0xF) << 4 | (FindFreeSpellTrapZone(ref->player) & 0xF), 0);
                return 0x64;
            fail:
                PlaySE(3);
            }
            if (gMain.h6 & 2)
                return 0x7F;
            return 0x7D;
        }
    }
    return 0;
}

#define ZB2_367E4(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
int EffectReviveFromGraveyardResolve(struct CardRef *ref)
{
    u8 skip = 4 & ((u8 *)ref)[4];

    if (!skip && ref->numTargets == 2) {
        switch (EFF_PHASE) {
        case 0x80: {
            int pp = 1 & ref->player;
            struct DuelZone *z = ZB(pp, ref->zone);
            u32 packed;
            int j;

            if (CARD_WORD(z->card) << 20 == 0)
                return 0;
            packed = ref->targets[1] << 16 | ref->targets[0];
            if (IsCardInGraveyard(ref->player, &packed) == 0)
                return 0;
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, ref->targets[0], ref->targets[1], 0);
            QueueSpecialSummon(ref->player, &packed, 1, 0, 0x20);
            EFF_SIDE = 0;
            for (j = 0; j <= 4; j++) {
                if (CARD_ID(CARD_WORD(ZB2_367E4(1 & ref->player, j)->card)) == ref->targets[0]
                    && (ZB2_367E4(1 & ref->player, j)->unk7[0] & 0x80))
                    EFF_SIDE++;
            }
            return 0x7F;
        }
        case 0x7F:
            switch (CARD_NUMBER(ref->id)) {
            case 0x447:
                QueueAddZoneLink(ref->player, ref->player | ref->zone << 8,
                             ref->player | ((u32)(gSummonAction << 26) >> 27) << 8, 2);
                break;
            case 0x488:
                EquipCard(ref->player, ref->player | ref->zone << 8,
                             ref->player | ((u32)(gSummonAction << 26) >> 27) << 8);
                break;
            }
            return 0x64;
        }
    }
    return 0;
}
int EffectSolomonsLawbookResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8045 : 0x45, 0, 0, 0);
    return 0;
}
int EffectEarthshakerResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
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

                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && GetZoneCardAttribute(p, j) == ref->targets[0]) {
                    DestroyFieldCardByEffect(p, j);
                    OnCardDestroyedByEffect(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
/* Player state at 0x020192E4 (stride 0xD64) as a real array, so the base address is
 * loaded before the index (a `((struct PF *)gDuelPlayers)[i]` cast loads it after). */
struct PFA {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 unk4[0x7C4 - 4];
    u32 deck[80];
    u8 filler[0xD64 - 0x7C4 - 80 * 4];
};
extern struct PFA gPF_020192E4[2];
/* The (u8) narrowing of an int makes the QImode AND take the mask register as its
 * first operand, so the constant 1 stays a QImode pseudo (shared by both player
 * lookups) and `1 - EFF_SIDE` below loads a fresh 1. */
static inline int EffSideIndex(int p)
{
    return (u8)p & 1;
}
/* Level-like value of a card: 0 for types 0x15-0x17, 10 for 0x18, else stat bits 25-28. */
static inline u32 EffCardLevel(int type, int id)
{
    switch (type) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 0xA;
    default:
        return (CARD_STATS(id) & 0x1E000000) >> 25;
    }
}
int EffectCyberJarResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            for (i = 0; i <= 1; i++) {
                int j;

                for (j = 0; j <= 4; j++) {
                    DestroyFieldCardByEffect(i, j);
                    OnCardDestroyedByEffect(ref->player, i, j);
                }
            }
            EFF_SIDE = DG->b1;
            EFF_CNT = 5;
            return 0x7F;
        }
        case 0x7F: {
            u32 *deck;
            u16 *h;
            u32 w;
            int id;

            if (gPF_020192E4[EffSideIndex(EFF_SIDE)].deckCount == 0)
                return 0x78;
            deck = gPF_020192E4[EffSideIndex(EFF_SIDE)].deck;
            h = (u16 *)deck;    /* second copy of the pointer, kept in r8 for the 0x7E message */
            DuelCmd_Push(EFF_SIDE ? 0x8061 : 0x61, 1, 1, 0);
            ShowRevealedCard(EFF_SIDE, CARD_ID(*deck));
            w = *deck;
            if (((w << 19) >> 31) != EFF_SIDE && (int)(w << 14) < 0 && CARD_NUMBER(CARD_ID11(w)) == 0x2FA) {
                if (CountFreeMonsterZones(1 - EFF_SIDE) > 0) {
                    DuelCmd_Push(EFF_SIDE ? 0x80C2 : 0xC2, h[0], h[1], 0);
                    CopyDuelCard(&ESC->cards[0], deck);
                    return 0x7D;
                }
                DiscardHandCard(EFF_SIDE, gPF_020192E4[EffSideIndex(EFF_SIDE)].handCount, 0, 1);
            } else {
                u32 type;

                id = CARD_ID(*deck);
                type = CARD_TYPE(id);
                if (type <= 0x14 && EffCardLevel(type, id) <= 4 && IsSpecialSummonOnly(id) == 0) {
                    DuelCmd_Push(EFF_SIDE ? 0x80C2 : 0xC2, h[0], h[1], 0);
                    CopyDuelCard(&ESC->cards[0], deck);
                    return 0x7E;
                }
            }
            return 0x7C;
        }
        case 0x7E:
            QueueSpecialSummonChoosePosition(EFF_SIDE, &ESC->cards[0], 0, 0);
            return 0x7C;
        case 0x7D:
            QueueSpecialSummonChoosePosition(1 - EFF_SIDE, &ESC->cards[0], 1, 0);
            return 0x7C;
        case 0x7C:
            if (--EFF_CNT == 0) {
                EFF_SIDE = 1 - EFF_SIDE;
                EFF_CNT = 5;
                /* FAKEMATCH: the ROM rereads EFF_SIDE after storing EFF_CNT */
                asm volatile("" ::: "memory");
                if (EFF_SIDE == DG->b1)
                    return 0x78;
            }
            return 0x7F;
        }
    }
    return 0;
}
int EffectSpecialSummonFromDeckResolve(struct CardRef *ref)
{
    u32 *card = &gCardListView.cards[gCardListView.row + gCardListView.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (CanSpecialSummon(ref->player) == 0)
                return 0;
            if (CollectEffectTargets(ref->player, CARD_NUMBER(ref->id), 0) == 0) {
                if (1 & ((u8 *)ref)[2])
                    return 0;
                TextBoxOpen(0x206, 0x712, 0xB, gStrRecruiterNoCardsInDeck);
                return 0x64;
            }
            if (1 & ((u8 *)ref)[2]) {
                if (AiPickCardListEntry(ref->id) < 0)
                    return 0;
                gCardListView.row = 0;
                gCardListView.top = S15F00->listPos;
                return 0x7D;
            }
            TextBoxOpen(0x206, 0x712, 0xB, gStrRecruiterSummonPrompt);
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gTextBox.flag14 == 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrRecruiterSelectMonster);
            return 0x7E;
        case 0x7E:
            CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7C;
        case 0x7C:
            QueueSpecialSummon(ref->player, &gCardListView.cards[gCardListView.row + gCardListView.top], 1, 0, 0);
            return 0x7B;
        case 0x7B:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int EffectAddRitualCardToHandResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (CollectEffectTargets(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                return 0;
            switch (CARD_NUMBER(ref->id)) {
            case 0x455:
                TextBoxOpen(0x206, 0x712, 0xB, gStrSenjuAddRitualMonsterPrompt);
                break;
            case 0x462:
                TextBoxOpen(0x206, 0x712, 0xB, gStrSonicBirdAddRitualMagicPrompt);
                break;
            default:
                return 0;
            }
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gTextBox.flag14 == 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrRitualSearchSelectCard);
            return 0x7E;
        case 0x7E:
            CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D:
            AddDeckCardToHand(ref->player, ((const u16 *)0x08622AB4)[CARD_ID11(gCardListView.cards[gCardListView.row + gCardListView.top])]);
            return 0x7C;
        }
    }
    return 0;
}
int EffectKarateManResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
    return 0;
}
int EffectSummonSameNameFromDeckResolve(struct CardRef *ref)
{
    char buf[0x80];
    int out;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                LoseLifePoints(1 - ref->player, 500);
                break;
            case 0x45B:
                GainLifePoints(ref->player, 1000);
                break;
            }
            return 0x7F;
        case 0x7F:
            if (CollectEffectTargets(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                return 0;
            if (1 & ((u8 *)ref)[2]) {
                gTextBox.flag14 = 1;
                return 0x7E;
            }
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                FormatStr(buf, gStrGiantGermSummonPrompt, (u8 *)0x0822C720 + ref->id * 64);
                break;
            case 0x45B:
            case 0x51B:
                FormatStr(buf, gStrSameNameSetPrompt, (u8 *)gCardNames + ref->id * 64);
                break;
            }
            TextBoxOpen(0x206, 0x712, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gTextBox.flag14 != 0) {
                int r = RemoveDeckCardByNumber(ref->player, CARD_NUMBER(ref->id), &out);

                if (r >= 0) {
                    u32 *dest = &ES544->card;
                    struct DuelCard *src = (struct DuelCard *)&gDuelDecks[ref->player];

                    src = (struct DuelCard *)((u32)src + r * 4);
                    CopyDuelCard(dest, src);
                    return 0x7D;
                }
            }
            goto dflt;
        case 0x7D:
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                QueueSpecialSummon(ref->player, &ES544->card, 1, 0, 0);
                break;
            case 0x45B:
                QueueSpecialSummon(ref->player, &ES544->card, 0, 1, 0);
                break;
            case 0x51B:
                QueueSpecialSummon(ref->player, &ES544->card, 0, 1, 0);
                return 0xA;
            }
            return 0x7F;
        default:
        dflt:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        }
    }
    return 0;
}
