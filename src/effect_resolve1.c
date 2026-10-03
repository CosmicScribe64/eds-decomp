#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step code such as 0x7F, 0x80 or 0x92. See wiki/functions/code-08030b88.md.
 */

/* DuelCard/DuelZone/DuelZonesPlayer and gDuelZones come from duel.h. */
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

/* Same two bytes seen through a struct, for functions that CSE the two addresses. */
struct EffectState {
    u8 unk[0x3E0];
    u8 phase;
    u8 side;
};
#define EFF_STATE ((struct EffectState *)gChain)

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

/* Player state gDuelPlayers[p] (canonical struct DuelPlayer from duel.h). */

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


struct S15F00_30B88 { u8 unk0[0x1B22]; u16 listPos; };
#define S15F00_30B88 ((struct S15F00_30B88 *)gAiWork)
int EffectElegantEgotistResolve(struct CardRef *ref)
{
    int i, n;

    if (ref->skip4)
        return 0;
    switch (EFF_PHASE) {
    case 0x80:
        if (!CountActiveCardsOnField(0, 0x3D) && !CountActiveCardsOnField(1, 0x3D) && !CountActiveCardsOnField(0, 0x4E1)
            && !CountActiveCardsOnField(1, 0x4E1))
            return 0;
        if (CollectEffectTargets(ref->player, 0x13D, 0) == 0)
            return 0;
        if (CountFreeMonsterZones(ref->player) == 0)
            return 0;
        if (1 & ((u8 *)ref)[2]) {
            n = CollectEffectTargets(1, 0x13D, 0);
            /* `cards + i` (not cards[i]) keeps 0x0201D81C as one pool constant, so the
             * found blocks address the viewer as base - 12. The u16 id locals in the first
             * and third loops add the RTL insns that make loop.c hoist the card table only
             * in its second pass (after the pointer copy), as the ROM does. */
            for (i = 0; i < n; i++) {
                u16 id = CARD_ID(*(gCardListView.cards + i));
                if (CARD_NUMBER(id) == 0x3E)
                    goto found1;
            }
            for (i = 0; i < n; i++) {
                if (CARD_NUMBER(CARD_ID(*(gCardListView.cards + i))) == 0x4E1)
                    goto found2;
            }
            for (i = 0; i < n; i++) {
                u16 id = CARD_ID(*(gCardListView.cards + i));
                if (CARD_NUMBER(id) == 0x3D)
                    goto found3;
            }
            AiPickCardListEntry(ref->id);
            gCardListView.row = 0;
            gCardListView.top = S15F00_30B88->listPos;
            return 0x7E;
        } else {
            TextBoxOpen(0x205, 0x914, 0xB, gStrElegantEgotistSelectPrompt);
            return 0x7F;
        }
    case 0x7F:
        CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
        return 0x7E;
    case 0x7E: {
        u16 *c = (u16 *)&gCardListView.cards[gCardListView.row + gCardListView.top];

        switch (gCardListView.kinds[gCardListView.row + gCardListView.top]) {
        case 2:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, c[0], c[1], 0);
            break;
        case 1:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80C2 : 0xC2, c[0], c[1], 0);
            break;
        }
        return 0x7D;
    }
    case 0x7D:
        QueueSpecialSummonChoosePosition(ref->player, &gCardListView.cards[gCardListView.top + gCardListView.row], 1, 1);
        return 0x7C;
    case 0x7C:
        if (gCardListView.kinds[gCardListView.top + gCardListView.row] == 2)
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
        return 0x64;
    found1:
        gCardListView.row = 0;
        gCardListView.top = i;
        return 0x7E;
    found2:
        gCardListView.row = 0;
        gCardListView.top = i;
        return 0x7E;
    found3:
        gCardListView.row = 0;
        gCardListView.top = i;
        return 0x7E;
    }
    return 0;
}
int EffectStopDefenseResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);
        if (CARD_NUMBER(CARD_ID11(CARD_WORD(z->card))) == 0x4B1 && z->flag6_0 && !z->flag6_1) {
            u16 msg = tp ? 0x807F : 0x7F;

            DuelCmd_Push(msg, tz, 0, 0);
            sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
        } else {
            int p2 = tp & 1;
            struct DuelZone *z2 = ZB(p2, tz);

            if (z2->flag6_0)
                ChangeBattlePosition(tp, tz, 1, 1);
        }
    }
    return 0;
}
int EffectDragonCaptureJarResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int p;

        for (p = 0; p <= 1; p++) {
            int i;

            for (i = 0; i <= 4; i++) {
                struct DuelZone *z = ZB(p & 1, i);

                if (z->flag6_1 && !z->flag6_0 && CARD_ID(CARD_WORD(z->card)) && GetZoneCardType(p, i) == 1)
                    ChangeBattlePosition(p, i, 0, 0);
            }
        }
    }
    return 0;
}
int EffectFieldMagicResolve(struct CardRef *ref)
{
    if (CARD_ID(CARD_WORD(gDuelFieldZone[1 & ref->player]))) {
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x8011 : 0x11;

        DuelCmd_Push(msg, GetFieldMagicIndex(CARD_NUMBER(ref->id)), 1, 0);
        EventResponse_Request(1 - ref->player, 0x18, 0);
    }
    return 0;
}
int EffectDarkHoleResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 1 - ref->player;
            EFF_PHASE--;
        case 0x7F: {
            int i;
            
            for (i = 0; i <= 4; i++) {
                if (IsZoneTargetable(EFF_SIDE, i)) {
                    DestroyFieldCardByEffect(EFF_SIDE, i);
                    OnCardDestroyedByEffect(ref->player, EFF_SIDE, i);
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
int EffectRaigekiResolve(struct CardRef *ref)
{
    int i;

    if (ref->skip4)
        return 0;
    for (i = 0; i <= 4; i++) {
        if (IsZoneTargetable(1 - ref->player, i)) {
            DestroyFieldCardByEffect(1 - ref->player, i);
            OnCardDestroyedByEffect(ref->player, 1 - ref->player, i);
            return 0x80;
        }
    }
    return 0;
}

int EffectGainLpChosenPlayerResolve(struct CardRef *ref)
{
    int amount = 0;

    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x151:
            amount = 200;
            break;
        case 0x153:
            amount = 600;
            break;
        case 0x154:
            amount = 800;
            break;
        case 0x3EE:
            amount = 400;
            break;
        }
        if (amount != 0) {
            switch (ref->targets[0]) {
            case 0:
                GainLifePoints(ref->player, amount);
                break;
            case 1:
                GainLifePoints(1 - ref->player, amount);
                break;
            }
        }
    }
    return 0;
}
int EffectGainLpResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int p;
        u16 lp;

        switch (CARD_NUMBER(ref->id)) {
        case 0x152:
            p = ref->player;
            lp = 500;
            break;
        case 0x155:
            p = ref->player;
            lp = 1000;
            break;
        case 0x523:
            GainLifePoints(ref->player, 1000);
            {
                int other = ref->player;

                /* FAKEMATCH: initialized input keeps the reloaded player in r1,
                 * leaving r0 for the original 1-player subtraction. No code emitted. */
                asm("" : : "r"(other) : "r0");
                p = 1 - other;
            }
            lp = 1000;
            break;
        default:
            return 0;
        }
        GainLifePoints(p, lp);
    }
    return 0;
}

int EffectDamageOpponentResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x156:
            LoseLifePoints(1 - ref->player, 200);
            break;
        case 0x157:
            LoseLifePoints(1 - ref->player, 500);
            break;
        case 0x158:
            LoseLifePoints(1 - ref->player, 600);
            break;
        case 0x159:
            LoseLifePoints(1 - ref->player, 800);
            break;
        case 0x15A:
            LoseLifePoints(1 - ref->player, 1000);
            LoseLifePoints(ref->player, 500);
            break;
        case 0x3EF:
            LoseLifePoints(1 - ref->player, 300);
            break;
        case 0x40F: {
            struct DuelPlayer *pl = gDuelPlayers;
            int p = (1 - ref->player) & 1;

            if (pl[p].handCount != 0) {
                int a = 1 - ref->player;
                int q = (1 - ref->player) & 1;

                LoseLifePoints(a, pl[q].handCount * 200);
            }
            break;
        }
        }
    }
    return 0;
}
int EffectSwordsOfRevealingLightResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            int i;

            for (i = 0; i <= 4; i++) {
                int opp = 1 - ref->player;
                int p = opp & 1;
                struct DuelZone *z = ZB(p, i);

                if (CARD_ID(CARD_WORD(z->card)) && !z->flag6_1) {
                    FlipFieldCard(opp, i, 1);
                    ShowRevealedCard(opp, CARD_ID(CARD_WORD(z->card)));
                }
            }
            return 0x7F;
        }
        ApplyKotodama();
    }
    return 0;
}
int EffectSpellbindingCircleResolve(struct CardRef *ref)
{
    if (ref->numTargets == 1) {
        int p = ref->targets[0] & 1;
        struct DuelZone *z = ZB(p, ref->targets[0] >> 8);

        if (CARD_ID(CARD_WORD(z->card)))
            QueueAddZoneLink(ref->player, ref->player | (ref->zone << 8), ref->targets[0], 2);
    }
    return 0;
}

int EffectDarkPiercingLightResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 4; i++) {
            int p = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(p, i);

            if (CARD_ID(CARD_WORD(z->card))) {
                int p2 = (1 - ref->player) & 1;
                struct DuelZone *z2 = ZB(p2, i);

                if (!z2->flag6_1)
                    FlipFieldCard(1 - ref->player, i, 1);
            }
        }
    }
    return 0;
}
int EffectMonsterEyeResolve(struct CardRef *ref)
{
    if (ReturnGraveyardCardToHand(ref->player, 0x3EB) == 0)
        ReturnGraveyardCardToHand(ref->player, 0x40A);
    return 0;
}

int EffectBlastJugglerResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        TributeMonster(ref->player, ref->zone);
        for (i = 0; i < ref->numTargets && i <= 1; i++) {
            u8 tp = ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && EffectBlastJugglerCheck(ref, ref->targets[i])) {
                DestroyFieldCardByEffect(tp, tz);
                OnCardDestroyedByEffect(ref->player, tp, tz);
            }
        }
    }
    return 0;
}
struct S15F00_31550 { u8 unk0[0x1B22]; u16 listPos; };
#define S15F00_31550 ((struct S15F00_31550 *)gAiWork)

int EffectCyberSteinResolve(struct CardRef *ref)
{
    u8 skip = ((u8 *)ref)[4] & 4;

    if (!skip) {
        switch (EFF_PHASE) {
        case 0x80:
            if (gDuelPlayers[1 & ref->player].fusionCount == 0)
                return 0;
            switch (CARD_NUMBER(ref->id)) {
            case 0x1A3:
                if (CountFreeMonsterZones(ref->player) == 0)
                    return 0;
                if (1 & ((u8 *)ref)[2]) {
                    AiPickCardListEntry(ref->id);
                    gCardListView.row = 0;
                    gCardListView.top = S15F00_31550->listPos;
                    return 0x7E;
                }
                TextBoxOpen(0x205, 0x914, 0xB, gStrCyberSteinSelectPrompt);
                return 0x7F;
            case 0x1F9:
                if (1 & ((u8 *)ref)[2]) {
                    AiPickCardListEntry(ref->id);
                    gCardListView.row = 0;
                    gCardListView.top = S15F00_31550->listPos;
                    return 0x7E;
                }
                TextBoxOpen(0x205, 0x914, 0xB, gStrGaleDograSelectPrompt);
                return 0x7F;
            }
            break;
        case 0x7F:
            CardListView_Open(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7E;
        case 0x7E: {
            u16 *c = (u16 *)&gCardListView.cards[gCardListView.row + gCardListView.top];

            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x80DC : 0xDC, c[0], c[1], 0);
            return 0x7D;
        }
        case 0x7D:
            switch (CARD_NUMBER(ref->id)) {
            case 0x1A3:
                QueueSpecialSummon(ref->player, &gCardListView.cards[gCardListView.row + gCardListView.top], 1, 0, skip);
                return 0x64;
            case 0x1F9: {
                u16 *c = (u16 *)&gCardListView.cards[gCardListView.row + gCardListView.top];

                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x807C : 0x7C, c[0], c[1], 0);
                LoseLpOnSendToGraveyard(ref->player, 1);
                break;
            }
            }
            break;
        }
    }
    return 0;
}
int EffectThunderDragonResolve(struct CardRef *ref)
{
    char buf[0x100];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 2;
            EFF_PHASE--;
            /* fall through */
        case 0x7F: {
            int i;

            for (i = 0; i < gDuelPlayers[1 & ref->player].deckCount; i++) {
                u32 w = CARD_WORD(gDuelPlayers[1 & ref->player].deck[i]);
                u32 mask = 0x7FF; /* local mask (life 3): loop.c hoists it in its second pass, after the base copy */
                u16 n = ((const u16 *)0x08622AB4)[CARD_ID(w) & mask];

                if (n == 0x1A8) {
                    FormatStr(buf, gStrThunderDragonAddPromptFmt, gCardNames + (((const u16 *)0x08623DF4)[n] << 6));
                    TextBoxOpen(0x205, 0x914, 0xB, buf);
                    TextBoxSetMenu(1, 0, 0);
                    return 0x7E;
                }
            }
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
        case 0x7E:
            if (gTextBox.flag14 != 0 && AddDeckCardToHand(ref->player, 0x1A8) != 0) {
                if (--EFF_SIDE != 0)
                    return 0x7F;
            }
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int EffectTheImmortalOfThunderResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 msg;

        GainLifePoints(ref->player, 3000);
        msg = ref->player ? 0x8092 : 0x92;
        DuelCmd_Push(msg, ref->zone, 0, 0);
    }
    return 0;
}

int EffectDestroyMagicTargetResolve(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->targets[0];
        int zone = ref->targets[0] >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id == 0)
            return 0;
        if (z->flag6_1) {
            if (CARD_TYPE(id) != 0x15)
                DestroyFieldCard(player, zone, 1);
            return 0;
        }
        DuelCmd_Push(player ? 0x807F : 0x7F, zone, 0, 0);
        ShowRevealedCard(player, id);
        if (CARD_TYPE(id) != 0x15)
            DestroyFieldCard(player, zone, 1);
        else
            DuelCmd_Push(player ? 0x807F : 0x7F, zone, 0, 0);
    }
    return 0;
}
int EffectNeedleBallResolve(struct CardRef *ref)
{
    if (1 & ((u8 *)ref)[2]) {
        if (gDuelPlayers[0].lifePoints <= 999 && gDuelPlayers[1].lifePoints > 2000) {
            DuelCmd_Push(0x8043, 2000, 1, 0);
            LoseLifePoints(1 - ref->player, 1000);
        }
    } else {
        switch (EFF_PHASE) {
        case 0x80:
            TextBoxOpen(0x205, 0x914, 0xB, gStrNeedleBallPayLpPrompt);
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gTextBox.flag14) {
                DuelCmd_Push(0x43, 2000, 1, 0);
                LoseLifePoints(1 - ref->player, 1000);
            }
            return 0x7E;
        }
    }
    return 0;
}
int EffectYadoKaruResolve(struct CardRef *ref)
{
    if (!ref->skip4 && !(1 & ((u8 *)ref)[2])) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct DuelPlayer *pl = gDuelPlayers;
            int p = 1 & ref->player;

            if (pl[p].handCount == 0)
                return 0;
            TextBoxOpen(0x205, 0x914, 0xB, gStrYadoKaruReturnPrompt);
            TextBoxSetMenu(1, 0, 0);
            return 0x7F;
        }
        case 0x7F:
            if (gTextBox.flag14 == 0)
                return 0;
            TextBoxOpen(0x205, 0x914, 0xB, gStrYadoKaruSelectPrompt);
            return 0x7E;
        case 0x7E:
            if (DuelCursor_PickTarget(1) != 0) {
                ReturnHandCardToDeck(ref->player, gDuelScreen.unk82C, 0);
                return 0x80;
            }
            return 0x7E;
        }
    }
    return 0;
}
