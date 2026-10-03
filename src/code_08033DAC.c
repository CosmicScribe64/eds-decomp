#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel card-effect step handlers, part 2, with the signature int f(struct EffCtx *ctx).
 * Same conventions as code_08032CB0 (see wiki/functions/code-08033dac.md).
 *
 * DuelCard/DuelZone/DuelZonesPlayer/DuelPlayer/DuelScreen and the globals
 * gDuel/gDuelPlayers/gDuelZones/gDuelScreen come from duel.h and duel_ui.h.
 * The unit keeps struct EffCtx (not in any shared header) and the local gChain
 * (ActBlk) / gCardListView (SelBlk) / gTextBox views, which have no shared header.
 */

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Effect context (argument of every handler). */
struct EffCtx {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 */
    u8 flags4;          /* +0x04: bit 2 = skip */
    u8 unk5;
    u16 pos6;           /* +0x06: player | zone << 8 of a card (CardRef view) */
    u8 filler8[2];
    u8 phaseA : 3;      /* +0x0A bits 0-2: phase / count of targets */
    u8 unkA_3 : 5;
    u8 fillerB;
    u16 pos;            /* +0x0C: player | zone << 8 of a target */
    u16 unkE;           /* +0x0E */
    u16 unk10;          /* +0x10 */
    u8 filler12[2];
};

/* Player bit read as a raw byte (the bitfield form gives lsl/lsr). */
#define PLAYER_RAW(c) (1 & ((u8 *)(c))[2])

struct ActBlk {
    u8 filler[0x3E0];
    u8 step;            /* +0x3E0 */
    u8 sub;             /* +0x3E1 */
};
extern struct ActBlk gChain;
/*
 * Local view: duel.h models gDuel as struct DuelState, whose u32 at +0x1ACC
 * (unk1ACC_0:15) has no byte-sized field at +0x1ACD. EffectShieldAndSwordResolve tests bit 6 of the byte at
 * +0x1ACD (u32 bit 14) and only matches with a u8 load, so keep a unit-local byte view.
 */
struct DuelStateByteView {
    u8 filler[0x1ACD];
    u8 unk1ACD;
};
extern struct DuelStateByteView gDuelStateBytes asm("gDuel");

void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
int EffectAttackResponsePrepare(struct EffCtx *ctx, int a, int b);
int EffectMagicArmShieldCheck(struct EffCtx *ctx, u16 pos);
int FindFreeMonsterZone(int player);
void MoveFieldCard(int player, u16 pos, int c);
int CollectEffectTargets(int player, int number, int b);
void CardListView_Open(int player, int a, u16 number, int b);
void DrawCards(int player, int a);
int IsSameCardName(u16 a, u16 b);
void DiscardHandCard(int player, int index, int a, int b);
void SendDeckCopiesToGraveyard(int player, u16 number, int a);
void DestroyFieldCard(int player, int zone, int a);
void DestroyFieldCardByEffect(int player, int zone);
void OnCardDestroyedByEffect(int a, int player, int zone);
int CountActiveCardsOnField2(int player, u16 number);
int CountFreeMonsterZones(int player);
int GetGraveyardCardById(int player, u16 id, u32 *out);
void QueueSpecialSummonChoosePosition(int player, u32 *card, int a, int b);
struct CardWord {
    u32 id : 12;
    u32 flag12 : 1;
    u32 rest : 19;
};
struct SelBlk {
    u8 filler0[5];
    u8 sel : 2;             /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 base;               /* +6 */
    u8 filler8[4];
    u32 cards[64];          /* +0xC: card words */
};
extern struct SelBlk gCardListView;
extern struct { u8 filler[0x14]; u16 unk14; } gTextBox;
extern const char gStrSelectGraveyardCardToBanish[];
extern const char gStrBanishAnotherGraveyardCardQuestion[];
void TextBoxOpen(u16 a, u16 b, int c, const char *text);
void TextBoxSetMenu(int a, int b, int c);
int DuelCursor_PickTarget(u32 mask);
void ShowCardDetail(int player, u16 id);
void QueueAddZoneLink(int a, int b, int c, int d);
int CanCardTargetZone(u16 id, int player, int zone);
int GetZoneCardAtk(int player, int zone);
int GetZoneCardDef(int player, int zone);
int IsZoneTargetable(int player, int zone);
void ShowCardEffect(int player, u16 id);
extern const char *const gTributeSummonPrompts[];
extern const char gStrSelectMonsterToSummonFromHand[];
int CanSummonFromHand(int player, u16 id);
int IsSpecialSummonOnly(u16 id);
int IsTributableMonster(u32 a, u32 b);
void PlaySE(u16 id);
void QueueNormalSummonChoosePosition(int player, u16 a, u16 b, u16 c);
#define CARD_LEVEL(id) ((CARD_STATS(id) & 0x1E000000) >> 25)
void DuelPrompt_PostDiscard(int player, int a, int b, int c);

int EffectShieldAndSwordResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x801D : 0x1D, !((gDuelStateBytes.unk1ACD >> 6) & 1), 0, 0);
    return 0;
}
int EffectGracefulCharityResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gChain.step) {
        case 0x80:
            DrawCards(ctx->player, 3);
            return 0x7F;
        case 0x7F:
            DuelPrompt_PostDiscard(ctx->player, 2, 0, 0);
            return 0x7E;
        }
    }
    return 0;
}
int EffectChainDestructionResolve(struct EffCtx *ctx)
{
    int player = ((u8 *)&ctx->pos6)[0];
    int zone = ctx->pos6 >> 8;
    int p = 1 & player;
    struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);
    u16 id = CARD_ID(CARD_WORD(z->card));
    u32 zero = (u8)(ctx->flags4 & 4);

    if (zero == 0) {
        switch (gChain.step) {
        case 0x80:
            if (id == 0)
                return 0;
            gChain.sub = zero;
            gChain.step--;
            /* fall through */
        case 0x7F:
            while (gChain.sub < gDuelPlayers[1 & player].handCount) {
                if (IsSameCardName(CARD_ID(CARD_WORD(gDuelPlayers[1 & player].hand[gChain.sub])), id) != 0) {
                    DiscardHandCard(player, gChain.sub, 1, 1);
                    return 0x7F;
                }
                gChain.sub++;
            }
            return 0x7E;
        case 0x7E:
            SendDeckCopiesToGraveyard(player, CARD_NUMBER(id), 1);
            return 0x7D;
        case 0x7D:
            DuelCmd_Push(player ? 0x8060 : 0x60, 0, 0, 0);
            return 0x78;
        }
    }
    return 0;
}
int EffectMesmericControlResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        DuelCmd_Push(PLAYER_RAW(ctx) == 0 ? 0x8048 : 0x48, 1, 0, 0);
    return 0;
}
int EffectMagicArmShieldResolve(struct EffCtx *ctx, int a)
{
    if (!(ctx->flags4 & 4) && EffectAttackResponsePrepare(ctx, a, 0) != 0) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1 && EffectMagicArmShieldCheck(ctx, ctx->pos) != 0) {
            int v = FindFreeMonsterZone(ctx->player);

            ctx->unkE = ctx->player | ((u8)v << 8);
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80A2 : 0xA2, ctx->pos, 1, 0);
            MoveFieldCard(ctx->player, ctx->pos, ctx->unkE);
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8038 : 0x38, ctx->unkE, 1, 0);
        }
    }
    return 0;
}
int EffectFissureResolve(struct EffCtx *ctx)
{
    int best = 99999;
    int idx = -1;

    if (!(ctx->flags4 & 4)) {
        int i;

        for (i = 0; i <= 4; i++) {
            int p = (1 - ctx->player) & 1;
            struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gDuelZones);

            /* Keep the player shift and zone product in the ROM scratch registers. */
            __asm__("" : : : "r1");
            if (CARD_WORD(z->card) << 20 != 0) {
                int p2 = (1 - ctx->player) & 1;
                struct DuelZone *z2 = (struct DuelZone *)(i * 0x94 + p2 * 0xD64 + (u32)gDuelZones);

                if (z2->flag6_1) {
                    int a = GetZoneCardAtk(1 - ctx->player, i);

                    if (a < best) {
                        best = a;
                        idx = i;
                    }
                }
            }
        }
        if (idx != -1) {
            if (IsZoneTargetable(1 - ctx->player, idx) != 0) {
                DestroyFieldCardByEffect(1 - ctx->player, idx);
                OnCardDestroyedByEffect(ctx->player, 1 - ctx->player, idx);
            } else {
                int p3 = (1 - ctx->player) & 1;
                struct DuelZone *z3 = (struct DuelZone *)(idx * 0x94 + p3 * 0xD64 + (u32)gDuelZones);

                if (CARD_WORD(z3->card) << 20 != 0) {
                    ShowCardEffect(ctx->player, CARD_ID(CARD_WORD(((struct DuelZone *)(((1 - ctx->player) & 1) * 0xD64 + idx * 0x94 + (u32)gDuelZones))->card)));
                }
            }
        }
    }
    return 0;
}
static inline int Le500(int value)
{
    int r = 0;

    if (value <= 500)
        r = 1;
    return r;
}

static inline int Gt999(int value)
{
    int r = 0;

    if (value > 999)
        r = 1;
    return r;
}

int EffectTrapHoleResolve(struct EffCtx *ctx)
{
    int player = ((u8 *)&ctx->pos6)[0];
    int zone = ctx->pos6 >> 8;
    int ok = 1;

    /* FAKEMATCH: preserve the initialized default result separately
     * from the later player mask, matching the ROM's two constants. */
    __asm__("" : "+r"(ok));
    if (!(ctx->flags4 & 4) && (ctx->kind == 5 || ctx->kind == 6)) {
        int p = 1 & player;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);

        if (CARD_WORD(z->card) << 20 != 0 && CanCardTargetZone(ctx->id, player, zone) != 0) {
            switch (CARD_NUMBER(ctx->id)) {
            case 0x3EA:
                ok = Gt999(GetZoneCardAtk(player, zone));
                break;
            case 0x2A8:
                ok = Le500(GetZoneCardDef(player, zone));
                break;
            case 0x2A9:
                ok = Le500(GetZoneCardAtk(player, zone));
                break;
            }
            if (ok != 0) {
                DestroyFieldCardByEffect(player, zone);
                OnCardDestroyedByEffect(ctx->player, player, zone);
            }
        }
    }
    /* FAKEMATCH: reserve r5 at the return so ctx/zone use r5/r4. */
    __asm__("" : : : "r5");
    return 0;
}
int EffectRemoveTrapResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1) {
            int player = ((u8 *)&ctx->pos)[0];
            int zone = ctx->pos >> 8;
            int p = phase & player;
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);
            u32 id = CARD_ID(CARD_WORD(z->card));

            if (id != 0 && CARD_TYPE(id) == 0x15 && (z->flag6_1))
                DestroyFieldCard(player, zone, 1);
        }
    }
    return 0;
}
int EffectTwoProngedAttackResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (7 & ((u8 *)ctx)[0xA]) == 3) {
        int i;

        for (i = 0; i < ctx->phaseA; i++) {
            register int off __asm__("r1") = i * 2;
            register u8 *loopbase __asm__("r0") = (u8 *)ctx;
            u16 *e;
            int zone, p;
            struct DuelZone *z;

            /* FAKEMATCH: initialized index/base constraints retain the ROM's
             * per-iteration address setup rather than a walking pointer.
             * The second input keeps +0xC on the base. No code emitted. */
            asm("" : "+r"(off), "+r"(loopbase));
            loopbase += 0xC;
            asm("" : : "r"(loopbase));
            e = (u16 *)(loopbase + off);
            zone = *e >> 8;
            p = 1 & ((u8 *)e)[0];
            z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);

            if (!CARD_ID(CARD_WORD(z->card)))
                return 0;
        }
        for (i = 0; i < ctx->phaseA; i++) {
            register int off __asm__("r1") = i * 2;
            register u8 *loopbase __asm__("r0") = (u8 *)ctx;
            u16 *e;
            int player, zone;

            /* FAKEMATCH: same initialized address setup as the check loop. */
            asm("" : "+r"(off), "+r"(loopbase));
            loopbase += 0xC;
            asm("" : : "r"(loopbase));
            e = (u16 *)(loopbase + off);
            player = ((u8 *)e)[0];
            zone = *e >> 8;

            DestroyFieldCardByEffect(player, zone);
            OnCardDestroyedByEffect(ctx->player, player, zone);
        }
    }
    return 0;
}
int EffectMonsterRebornResolve(struct EffCtx *ctx)
{
    u32 out;
    struct CardWord card;

    if (!(ctx->flags4 & 4)) {
        if (CARD_NUMBER(ctx->id) == 0x3F0) {
            if (CountActiveCardsOnField2(0, 0x402) > 0 || CountActiveCardsOnField2(1, 0x402) > 0)
                return 0;
        }
        if ((7 & ((u8 *)ctx)[0xA]) == 2 && CountFreeMonsterZones(ctx->player) > 0) {
            *(u32 *)&card = ctx->unkE << 16 | ctx->pos;
            if (GetGraveyardCardById(card.flag12, card.id, &out) != 0) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, ctx->pos, ctx->unkE, 0);
                QueueSpecialSummonChoosePosition(ctx->player, (u32 *)&card, 1, 0x30);
            }
        }
    }
    return 0;
}
int EffectDrawCardsResolve(struct EffCtx *ctx)
{
    int n = 0;

    if (!(ctx->flags4 & 4)) {
        switch (CARD_NUMBER(ctx->id)) {
        case 0x21B:
        case 0x5A7:
            n = 1;
            break;
        case 0x3F2:
            n = 2;
            break;
        }
        if (n > 0)
            DrawCards(ctx->player, n);
    }
    return 0;
}
int EffectBanishGraveyardCardsResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gChain.step) {
        case 0x80:
            switch (CARD_NUMBER(ctx->id)) {
            case 0x3F3:
                gChain.sub = 2;
                break;
            case 0x400:
                gChain.sub = 5;
                break;
            }
            gChain.step--;
            /* fall through */
        case 0x7F:
            if (CollectEffectTargets(ctx->player, CARD_NUMBER(ctx->id), 0) <= 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectGraveyardCardToBanish);
            return 0x7E;
        case 0x7E:
            CardListView_Open(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7D;
        case 0x7D: {
            u32 *card = &gCardListView.cards[gCardListView.sel + gCardListView.base];

            DuelCmd_Push((int)(gCardListView.cards[gCardListView.sel + gCardListView.base] << 19) < 0 ? 0x80D4 : 0xD4,
                         ((u16 *)card)[0], ((u16 *)card)[1], 0);
            gChain.sub--;
            if (gChain.sub == 0)
                return 0;
            return 0x7C;
        }
        case 0x7C:
            if (CollectEffectTargets(ctx->player, CARD_NUMBER(ctx->id), 0) == 0)
                return 0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrBanishAnotherGraveyardCardQuestion);
            TextBoxSetMenu(1, 0, 0);
            return 0x7B;
        case 0x7B:
            if (gTextBox.unk14 == 0)
                return 0;
            return 0x7F;
        }
    }
    return 0;
}
int EffectTheInexperiencedSpyResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        if (gDuelPlayers[1 & ctx->player].handCount != 0) {
            if (DuelCursor_PickTarget(0x10000) != 0) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8008 : 0x8, (u16)gDuelScreen.player,
                             *(u8 *)&gDuelScreen.zone | (*(u8 *)&gDuelScreen.cursor << 8), 0);
                ShowCardDetail(ctx->player, CARD_ID(CARD_WORD(gDuelPlayers[(1 - ctx->player) & 1].hand[gDuelScreen.cursor])));
            } else
                goto r80;
        }
    }
    return 0;
r80:
    return 0x80;
}
int EffectAddStatModifierResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1) {
            u16 pos = ctx->pos;
            int zone = pos >> 8;
            int p = phase & ((u8 *)&ctx->pos)[0];
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);

            if ((z->flag6_1) && CARD_ID(CARD_WORD(z->card)))
                QueueAddZoneLink(ctx->player, ctx->id, pos, 3);
        }
    }
    return 0;
}
int EffectUltimateOfferingResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gChain.step) {
        case 0x80: {
            int i;

            for (i = 0; i < gDuelPlayers[1 & ctx->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & ctx->player].hand[i]));

                if (CanSummonFromHand(ctx->player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                    TextBoxOpen(0x206, 0x712, 0xB, gStrSelectMonsterToSummonFromHand);
                    return 0x7F;
                }
            }
            break;
        }
        case 0x7F:
            if (DuelCursor_PickTarget(1) != 0) {
                u32 cursor = gDuelScreen.cursor;
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[ctx->player].hand[cursor]));

                if (CanSummonFromHand(ctx->player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                    int v;

                    ctx->pos = cursor;
                    switch ((int)CARD_TYPE(id)) {
                    case 0x15:
                    case 0x16:
                    case 0x17:
                        v = 0;
                        break;
                    case 0x18:
                        v = 10;
                        break;
                    default:
                        v = CARD_LEVEL(id);
                        break;
                    }
                    switch (v) {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        ctx->unkE = FindFreeMonsterZone(ctx->player);
                        ctx->unk10 = 0;
                        return 0x64;
                    case 5:
                    case 6:
                        return 0x6E;
                    default:
                        return 0x78;
                    }
                }
                PlaySE(3);
            }
            return 0x7F;
        case 0x78:
            TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[1]);
            TextBoxSetMenu(1, 0, 0);
            return 0x77;
        case 0x77:
            if (gTextBox.unk14 == 0)
                return 0x80;
            TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[2]);
            return 0x76;
        case 0x76:
            if (DuelCursor_PickTarget(0xF0) != 0) {
                if (IsTributableMonster(gDuelScreen.player, gDuelScreen.cursor) != 0) {
                    PlaySE(1);
                    DuelCmd_Push(8, (u16)gDuelScreen.player, *(u8 *)&gDuelScreen.zone | (*(u8 *)&gDuelScreen.cursor << 8), 0);
                    ctx->unkE = gDuelScreen.cursor;
                    ctx->unk10 = (u8)((int)gDuelScreen.cursor | 0x80) << 8;
                    return 0x75;
                }
                PlaySE(3);
            }
            return 0x76;
        case 0x75:
            TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[3]);
            return 0x74;
        case 0x74:
            if (DuelCursor_PickTarget(0xF0) != 0) {
                if (IsTributableMonster(gDuelScreen.player, gDuelScreen.cursor) != 0 && gDuelScreen.cursor != ctx->unkE) {
                    PlaySE(1);
                    DuelCmd_Push(8, (u16)gDuelScreen.player, *(u8 *)&gDuelScreen.zone | (*(u8 *)&gDuelScreen.cursor << 8), 0);
                    ctx->unk10 = (u8)((int)gDuelScreen.cursor | 0x80) | ctx->unk10;
                    return 0x64;
                }
                PlaySE(3);
            }
            return 0x74;
        case 0x6E:
            TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[0]);
            TextBoxSetMenu(1, 0, 0);
            return 0x6D;
        case 0x6D:
            if (gTextBox.unk14 == 0)
                return 0x80;
            TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[4]);
            return 0x6C;
        case 0x6C:
            if (DuelCursor_PickTarget(0xF0) != 0) {
                if (IsTributableMonster(gDuelScreen.player, gDuelScreen.cursor) != 0) {
                    PlaySE(1);
                    DuelCmd_Push(8, (u16)gDuelScreen.player, *(u8 *)&gDuelScreen.zone | (*(u8 *)&gDuelScreen.cursor << 8), 0);
                    ctx->unkE = gDuelScreen.cursor;
                    ctx->unk10 = (u8)((int)gDuelScreen.cursor | 0x80);
                    return 0x64;
                }
                PlaySE(3);
            }
            return 0x6C;
        case 0x64:
            QueueNormalSummonChoosePosition(ctx->player, ctx->pos, ctx->unkE, ctx->unk10);
            return 0xA;
        }
    }
    return 0;
}
int EffectAncientTelescopeResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        CollectEffectTargets(ctx->player, CARD_NUMBER(ctx->id), 0);
        CardListView_Open(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
    }
    return 0;
}
int HasFlipEffect(u16 number, int flag);

int EffectNegateChainedCardResolve(struct EffCtx *ctx, struct EffCtx *other)
{
    int i;

    if (!(ctx->flags4 & 4) && other != NULL) {
        switch (CARD_NUMBER(ctx->id)) {
        case 0x3FB:
            if (CARD_NUMBER(other->id) == 0x14F && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 0; i <= 4; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gDuelZones);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        DestroyFieldCardByEffect(1 - ctx->player, i);
                        OnCardDestroyedByEffect(ctx->player, 1 - ctx->player, i);
                    }
                }
            }
            return 0;
        case 0x3FD:
            if (CARD_NUMBER(other->id) == 0x3F0 && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            break;
        case 0x3FE:
            if (CARD_NUMBER(other->id) == 0x150 && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 0; i <= 4; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gDuelZones);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        DestroyFieldCardByEffect(1 - ctx->player, i);
                        OnCardDestroyedByEffect(ctx->player, 1 - ctx->player, i);
                    }
                }
            }
            return 0;
        case 0x405:
            if (CARD_TYPE(other->id) == 0x16) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            return 0;
        case 0x406:
            if (CARD_TYPE(other->id) == 0x15) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            return 0;
        case 0x426:
            if (CARD_NUMBER(other->id) == 0x29F && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 5; i <= 10; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gDuelZones);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        DuelCmd_Push(PLAYER_RAW(ctx) == 0 ? 0x808B : 0x8B, i, 1, 0);
                        DestroyFieldCard(1 - ctx->player, i, 1);
                    }
                }
            }
            return 0;
        case 0x5FA:
            if (CARD_TYPE(other->id) <= 0x14
                && (HasFlipEffect(CARD_NUMBER(other->id), 1) != 0 || HasFlipEffect(CARD_NUMBER(other->id), 0) != 0))
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
            break;
        case 0x5FB:
            switch (CARD_NUMBER(other->id)) {
            case 0x12C ... 0x13C:
            case 0x13E ... 0x147:
            case 0x28B:
            case 0x28D:
            case 0x290:
            case 0x3C2:
            case 0x3F0:
            case 0x3F4:
            case 0x3F5:
            case 0x3FF:
            case 0x403:
            case 0x412:
            case 0x413:
            case 0x416:
            case 0x417:
            case 0x422:
            case 0x424:
            case 0x42C:
            case 0x430:
            case 0x433:
            case 0x434:
            case 0x485:
            case 0x488:
            case 0x49E:
            case 0x4BB:
            case 0x4C4:
            case 0x521:
            case 0x58B:
            case 0x58C:
            case 0x58E:
            case 0x5A8 ... 0x5AB:
            case 0x604:
            case 0x60A:
            case 0x60C:
            case 0x60E:
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                DuelCmd_Push(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                break;
            }
            break;
        }
    }
    return 0;
}
