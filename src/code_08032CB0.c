#include "global.h"
#include "main.h"
#include "duel.h"

/*
 * Duel card-effect step handlers, gated on a u8 flag and the phase, with the signature
 * int f(struct EffCtx *ctx). See wiki/functions/code-08032cb0.md.
 *
 * Uses the shared layouts: struct Main (main.h) and the duel structs/globals (duel.h).
 */

#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)

/* Effect context (argument of every handler; 0x14+ bytes). */
struct EffCtx {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 */
    u8 flags4;          /* +0x04: bit 2 = skip */
    u8 filler5[5];
    u8 phaseA;          /* +0x0A: low 3 bits = phase */
    u8 fillerB;
    u16 pos;            /* +0x0C: player | zone << 8 of a target */
    u8 filler0E[6];
};

int GetZoneCardAtk(int player, int zone);
void TributeMonster(int player, int zone);
int HalveRoundUp(int a);
void LoseLifePoints(int player, int a);
void DestroyFieldCardByEffect(int player, int zone);
void OnCardDestroyedByEffect(int a, int player, int zone);
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
extern const u16 gCardNumberToId[];   /* card id to card index */
int CountFreeMonsterZones(int player);
int CountGraveyardCardsByNumber(int player, u16 number);
int GetGraveyardCardById(int player, u16 id, u32 *out);
void QueueSpecialSummonChoosePosition(int player, u32 *card, int a, int b);
struct Card2 {
    u16 id;
    u16 hi;
};
struct ActBlk {
    u8 filler[0x3E0];
    u8 step;            /* +0x3E0 */
    u8 sub;             /* +0x3E1 */
    u8 filler3E2[0x510 - 0x3E2];
    u8 count510;        /* +0x510 */
    u8 zone511;         /* +0x511 */
    u8 filler512[0x544 - 0x512];
    struct Card2 cards544[8];   /* +0x544: card words */
};
extern struct ActBlk gChain;

/* Card id (0..1999 and 2000+) to card index; 0xFFFF maps to 0. */
static inline u16 CardIdToIndex(u32 id)
{
    if (id == 0xFFFF)
        return 0;
    if (id <= 1999)
        return *(gCardNumberToId + (id & 0x7FF));
    return *(gCardNumberToId + ((id - 2000) & 0x7FF)) + 1;
}
int FindFreeSpellTrapZone(int player);
void MoveFieldCard(int player, u16 pos, int c);
void DestroyLinkedCards(int player, int zone, int a);
#define CARD_ID11(w) (((w) << 21) >> 21)
/* 12-byte slots at 0x02018450, one per player. */
struct Slot {
    u8 filler0[8];
    u8 unk8_0 : 4;
    u8 flag4 : 1;       /* +8 bit 4 */
    u8 flag5 : 1;       /* +8 bit 5 */
    u8 unk8_6 : 2;
    u8 unk9;
    u16 id;             /* +0x0A card id */
};
extern struct Slot gBattle[];
struct CardBuf {
    u16 lo;
    u8 unk2_0 : 5;
    u8 flag21 : 1;
    u8 unk2_6 : 2;
    u8 unk3;
};
int IsToonMonster(u16 number);
int HasFaceUpToonWorld(int player);
int FindFreeMonsterZone(int player);
int IsCardInGraveyard(int player, u32 *card);
void QueueSpecialSummon(int player, u32 *card, int a, int b, int c);
extern struct { u8 filler[0x82C]; u32 cursor; } gDuelScreen;
extern struct { u8 filler[0x14]; u16 unk14; } gTextBox;
struct HandCards {
    u32 cards[0xD64 / 4];
};
extern struct HandCards gDuelHands[2];
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const char gStrFluteSpecialSummonQuestion[];
extern const char gStrDragon[];
extern const char gStrSpecialSummonAnotherQuestion[];
extern const char gStrFluteSelectFromHand[];
void FormatStr(char *dst, const char *fmt, const char *arg);
void TextBoxOpen(u16 a, u16 b, int c, const char *text);
void TextBoxSetMenu(int a, int b, int c);
int DuelCursor_PickTarget(u32 mask);
void PlaySE(u16 id);
int EffectTheFluteOfSummoningDragonPrepare(struct EffCtx *ctx, int a, int b);
void DuelPrompt_PostRandomBanishFaceDown(int player);
#define CARD_STATS2(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
struct SelBlk {
    u8 filler0[5];
    u8 sel : 2;             /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 base;               /* +6 */
    u8 filler8[4];
    u32 cards[64];          /* +0xC: card words */
};
extern struct SelBlk gCardListView;
#define A_02017F84 ((u32 *)0x02017F84)
#define A_02017F88 ((void *)0x02017F88)
#define A_02017F8C ((void *)0x02017F8C)
#define A_02017E2C ((void *)0x02017E2C)
#define A_02017E28 ((void *)0x02017E28)
extern const char gStrMagicalHatsSelectFirst[];
extern const char gStrMagicalHatsSelectSecond[];
int CollectEffectTargets(int player, int number, int b);
int EffectAttackResponsePrepare(struct EffCtx *ctx, int a, int b);
void CardListView_Open(int player, int a, u16 number, int b);
void CopyDuelCard(void *dst, const void *src);
void *MemCopy16(void *dst, const void *src, int n);
s32 Random(void);
int CountActiveCardsOnField(int player, u16 number);
void QueueAddZoneLink(int a, int b, int c, int d);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
/* Player bit read as a raw byte (the bitfield form gives lsl/lsr). */
#define PLAYER_RAW(c) (1 & ((u8 *)(c))[2])
int EffectDarkEyesIllusionistResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (ctx->phaseA & 7) == 1) {
        int zone = ctx->pos >> 8;
        int player = ((u8 *)&ctx->pos)[0] & 1;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + player * 0xD64 + (u32)gDuelZones);

        if (CARD_ID(CARD_WORD(z->card)))
            QueueAddZoneLink(ctx->player, ctx->player | (ctx->zone << 8), ctx->pos, 2);
    }
    return 0;
}
int EffectRelinquishedResolve(struct EffCtx *ctx)
{
    int cnt = FindFreeSpellTrapZone(ctx->player);

    if (!(ctx->flags4 & 4)) {
        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8092 : 0x92, ctx->zone, 0, 0);
        if ((ctx->phaseA & 7) == 1 && cnt >= 0) {
            u16 player;
            register int zone asm("sl"); /* FAKEMATCH: ROM keeps zone in sl and the target id in r9 */
            struct DuelZone *tz;
            u32 tid;
            int p1;
            int p0;
            struct DuelZone *z1;

            player = ((u8 *)&ctx->pos)[0];
            zone = ctx->pos >> 8;
            p0 = player & 1;
            tz = (struct DuelZone *)(zone * 0x94 + p0 * 0xD64 + (u32)gDuelZones);
            tid = CARD_ID(CARD_WORD(tz->card));
            p1 = 1 & ctx->player;
            z1 = (struct DuelZone *)(ctx->zone * 0x94 + p1 * 0xD64 + (u32)gDuelZones);

            if (CARD_WORD(z1->card) << 20 != 0) {
                int p2 = 1 & ctx->player;
                struct DuelZone *z2 = (struct DuelZone *)(ctx->zone * 0x94 + p2 * 0xD64 + (u32)gDuelZones);

                /* FAKEMATCH: the r7 clobber stops reload reusing the 0xD64 constant in r7, as the ROM reloads it */
                asm volatile("" ::: "r7");
                if (CARD_NUMBER(CARD_ID11(CARD_WORD(z2->card))) == 0x2DA) {
                    int p3 = 1 & ctx->player;
                    struct DuelZone *z3 = (struct DuelZone *)(ctx->zone * 0x94 + p3 * 0xD64 + (u32)gDuelZones);

                    if ((z3->flag6_1) && tid != 0) {
                        MoveFieldCard(ctx->player, ctx->pos, ctx->player | (u8)cnt << 8);
                        DestroyLinkedCards(player, zone, 0);
                        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x808C : 0x8C, cnt, 1, 0);
                        QueueAddZoneLink(ctx->player, ctx->player | (u8)cnt << 8, ctx->player | (ctx->zone << 8), 5);
                    }
                }
            }
        }
    }
    return 0;
}
int EffectJigenBakudanResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        if (ctx->kind == 2) {
            int sum = 0;
            int i;

            for (i = 0; i <= 4; i++) {
                /* FAKEMATCH: preserve the shifted player bit separately from p.
                 * Both values are initialized; no extra instructions are emitted. */
                register u32 shifted asm("r3") = (u32)((u8 *)ctx)[2] << 31;
                register int p asm("r2") = 1 & (shifted >> 31);
                struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gDuelZones);

                if (CARD_ID(CARD_WORD(z->card))) {
                    sum += GetZoneCardAtk(p, i);
                    TributeMonster(ctx->player, i);
                }
            }
            LoseLifePoints(1 - ctx->player, HalveRoundUp(sum));
        } else {
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8092 : 0x92, ctx->zone, 0, 0);
        }
    }
    return 0;
}

static inline int TypeIs(u32 id, u32 t)
{
    u32 ty = CARD_TYPE(id);
    int r = 0;

    if (ty == t)
        r = 1;
    return r;
}

int EffectNegateTrapsOrMagicResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gChain.step) {
        case 0x80:
            gChain.count510 = 0;
            return 0x7F;
        case 0x7F:
            gChain.zone511 = 5;
            return 0x7E;
        case 0x7E: {
            int p;
            int p1;
            int zone;
            u32 id;
            struct DuelZone *z;

            if (gChain.count510 != 0)
                p = ctx->player;
            else
                p = 1 - ctx->player;
            zone = gChain.zone511;
            p1 = 1 & p;
            z = (struct DuelZone *)(zone * 0x94 + p1 * 0xD64 + (u32)gDuelZones);
            id = CARD_ID(CARD_WORD(z->card));
            if (id != 0 && !(p == ctx->player && zone == ctx->zone) && (z->flag6_1)) {
                int flag = 0;

                switch (CARD_NUMBER(ctx->id)) {
                case 0x409:
                case 0x2EF:
                    flag = TypeIs(id, 0x15);
                    break;
                case 0x482:
                    flag = TypeIs(id, 0x16);
                    break;
                case 0x601:
                    if (CARD_TYPE(id) == 0x16 && ((CARD_STATS2(id) & 0xE0000) >> 17) == 3)
                        flag = 1;
                    break;
                }
                if (flag != 0) {
                    DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8008 : 0x8, p, zone << 8, 0);
                    {
                        int msg = PLAYER_RAW(ctx) ? 0x8074 : 0x74;

                        DuelCmd_Push(msg, id, 1, 0);
                    }
                    DuelCmd_Push(p != 0 ? 0x80B1 : 0xB1, zone, 1, 0);
                }
            }
            return 0x7D;
        }
        case 0x7D:
            gChain.zone511++;
            if (gChain.zone511 <= 9)
                return 0x7E;
            gChain.count510++;
            if (gChain.count510 <= 1)
                return 0x7F;
            return 0x78;
        case 0x78:
            switch (CARD_NUMBER(ctx->id)) {
            case 0x409:
            case 0x2EF:
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8019 : 0x19, 1, 0, 0);
                break;
            case 0x482:
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x801A : 0x1A, 1, 0, 0);
                break;
            case 0x601:
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x801B : 0x1B, 1, 0, 0);
                break;
            }
            break;
        }
    }
    return 0;
}
int EffectParasiteParacideResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8094 : 0x94, ctx->zone, 1 - ctx->player, 0);
        DuelCmd_Push(PLAYER_RAW(ctx) == 0 ? 0x8060 : 0x60, 0, 0, 0);
    }
    return 0;
}
int EffectValkyrionTheMagnaWarriorResolve(struct EffCtx *ctx)
{
    u32 zero = (u8)(ctx->flags4 & 4);

    if (zero == 0) {
        u8 *base = (u8 *)&gChain;
        u8 *step = base + 0x3E0;
        u32 n;

        switch (*step) {
        case 0x80:
            if (CountFreeMonsterZones(ctx->player) <= 1)
                return 0;
            if (CountGraveyardCardsByNumber(ctx->player, 0x2E1) == 0)
                return 0;
            if (CountGraveyardCardsByNumber(ctx->player, 0x2F4) == 0)
                return 0;
            if (CountGraveyardCardsByNumber(ctx->player, 0x320) == 0)
                return 0;
            *(base + 0x3E1) = zero;
            (*step)--;
            /* fall through */
        case 0x7F: {
            u32 card;

            switch (gChain.sub) {
            case 0:
                n = 0x2E1;
                break;
            case 1:
                n = 0x2F4;
                break;
            case 2:
                n = 0x320;
                break;
            }
            if (GetGraveyardCardById(ctx->player, CardIdToIndex(n), &card) != 0) {
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, card, card >> 16, 0);
                QueueSpecialSummonChoosePosition(ctx->player, &card, 1, 0x20);
            }
            gChain.sub++;
            if (gChain.sub <= 2)
                return 0x7F;
            return 0x7E;
        }
        }
    }
    return 0;
}
int EffectBellOfDestructionResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (ctx->phaseA & 7) == 1) {
        int player = ((u8 *)&ctx->pos)[0];
        int zone = ctx->pos >> 8;
        int p = player & 1;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);

        if (CARD_ID(CARD_WORD(z->card)) && (z->flag6_1)) {
            int atk = GetZoneCardAtk(player, zone);

            DestroyFieldCardByEffect(player, zone);
            OnCardDestroyedByEffect(ctx->player, player, zone);
            switch ((int)CARD_NUMBER(ctx->id)) {
            case 0x3AB:
                LoseLifePoints(1 - ctx->player, atk);
                LoseLifePoints(ctx->player, atk);
                break;
            case 0x5AB:
                DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8044 : 0x44, 0, 0, 0);
                break;
            }
        }
    }
    return 0;
}
int EffectMagicalHatsResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gChain.step) {
        case 0x80: {
            int phase = 7 & ctx->phaseA;

            if (phase != 1)
                return 0;
            if (CollectEffectTargets(ctx->player, CARD_NUMBER(ctx->id), 0) <= 1)
                return 0;
            {
                int player = ((u8 *)&ctx->pos)[0];
                int zone = ctx->pos >> 8;
                int p = phase & player;
                struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gDuelZones);

                if (!CARD_ID(CARD_WORD(z->card)))
                    return 0;
                if (ctx->player != player)
                    return 0;
            }
            if (EffectAttackResponsePrepare(ctx, 0, 0) == 0)
                return 0;
            TextBoxOpen(0x205, 0x914, 0xB, gStrMagicalHatsSelectFirst);
            return 0x7F;
        }
        case 0x7F:
            CardListView_Open(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7E;
        case 0x7E: {
            u32 *card = &gCardListView.cards[gCardListView.base + gCardListView.sel];

            CopyDuelCard(A_02017F84, card);
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            TextBoxOpen(0x205, 0x914, 0xB, gStrMagicalHatsSelectSecond);
            return 0x7D;
        }
        case 0x7D:
            CardListView_Open(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7C;
        case 0x7C: {
            u32 *card = &gCardListView.cards[gCardListView.base + gCardListView.sel];
            u8 *b = (u8 *)0x02017F88;

            CopyDuelCard(b, card);
            /* FAKEMATCH: retain the initialized destination before the player mask. */
            __asm__("" : : "r"(b));
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            {
                u8 *dst = b + 4;
                struct DuelZonesPlayer *pp = &gDuelZones[1 & ctx->pos];
                struct DuelZone *z = (struct DuelZone *)((u32)pp + (ctx->pos >> 8) * 0x94);

                CopyDuelCard(dst, z);
            }
            {
                u8 *dst = b - 0x15C;
                struct DuelZonesPlayer *pp = &gDuelZones[1 & ctx->pos];
                struct DuelZone *z = (struct DuelZone *)((u32)pp + (ctx->pos >> 8) * 0x94);

                MemCopy16(dst, z, 0x94);
            }
            DuelCmd_Push((u8)ctx->pos ? 0x8078 : 0x78, ctx->pos >> 8, 8, 0);
            DuelCmd_Push((u8)ctx->pos ? 0x808D : 0x8D, ctx->pos >> 8, 0xA, 0);
            return 0x7B;
        }
        case 0x7B: {
            int i = 0;
            u8 *tmp = (u8 *)0x02017E28;
            u32 *arr = (u32 *)(tmp + 0x15C);

            do {
                int a;
                int b;

                do {
                    a = Random() % 3;
                    b = Random() % 3;
                } while (a == b);
                CopyDuelCard(tmp, &arr[a]);
                CopyDuelCard(&arr[a], &arr[b]);
                CopyDuelCard(&arr[b], tmp);
                i++;
            } while (i <= 15);
            gChain.sub = 0;
            return 0x7A;
        }
        case 0x7A: {
            int v = FindFreeMonsterZone(ctx->player);
            struct Card2 *card = &gChain.cards544[gChain.sub];
            int flag = 0;

            /* FAKEMATCH: keep the formed card pointer alive across the flag checks. */
            __asm__("" : : "r"(card));
            if (CountActiveCardsOnField(0, 0x47F) != 0 || CountActiveCardsOnField(1, 0x47F) != 0)
                flag = 1;
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80A8 : 0xA8, (u8)v | ((flag | 2) << 8), card->id, card->hi);
            if (CARD_TYPE(CARD_ID11((u32)gChain.cards544[gChain.sub].id)) <= 0x14)
                DuelCmd_Push((u8)ctx->pos ? 0x808D : 0x8D, 0xA, (u16)v, 0);
            gChain.sub++;
            if (gChain.sub <= 2)
                return 0x7A;
            return 0x79;
        }
        case 0x79:
            DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x78;
        }
    }
    return 0;
}
u16 EffectTimeMachineResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int p;
        int flag;
        u32 card;
        struct Slot *s;
        struct Slot *slots;
        int step = gChain.step;

        switch (step) {
        case 0x7F:
        case 0x80:
            {
                switch (step) {
                case 0x80:
                    p = ctx->player;
                    break;
                case 0x7F:
                    p = 1 - ctx->player;
                    break;
                }
                flag = 1;
                slots = gBattle;
                s = slots + p;
                if (IsToonMonster(CARD_NUMBER(s->id)) != 0)
                {
                    flag = 0;
                    if (HasFaceUpToonWorld(p) != 0)
                        flag = 1;
                }
                if (((int)((u32)((u8 *)s)[8] << 26) < 0) && flag && FindFreeMonsterZone(p) >= 0) {
                    int q;

                    if (GetGraveyardCardById(p, s->id, &card) != 0 && IsCardInGraveyard(p, &card) != 0) {
                        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, card, card >> 16, 0);
                        ((struct CardBuf *)&card)->flag21 = 0;
                        QueueSpecialSummon(p, &card, 1, s->flag4, 0x20);
                    }
                    q = 1 - p;
                    if (GetGraveyardCardById(q, gBattle[p].id, &card) != 0 && IsCardInGraveyard(q, &card) != 0) {
                        DuelCmd_Push(PLAYER_RAW(ctx) == 0 ? 0x80D3 : 0xD3, card, card >> 16, 0);
                        ((struct CardBuf *)&card)->flag21 = 0;
                        QueueSpecialSummon(p, &card, 1, gBattle[p].flag4, 0x20);
                    }
                }
                gBattle[p].flag5 = 0;
                return gChain.step - 1;
            }
        default:
            return 0;
        }
    }
    return 0;
}
int EffectEndBattlePhaseResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        DuelCmd_Push(PLAYER_RAW(ctx) ? 0x8037 : 0x37, 0, 0, 0);
    return 0;
}
int EffectLightforceSwordResolve(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        struct DuelPlayer *pl = gDuelPlayers;
        int p = 1 - ctx->player;

        if (pl[p & 1].handCount != 0)
            DuelPrompt_PostRandomBanishFaceDown(1 - ctx->player);
    }
    return 0;
}
int EffectTheFluteOfSummoningDragonResolve(struct EffCtx *ctx)
{
    char bufA[0x100];
    char bufB[0x100];
    char bufC[0x100];

    if (ctx->flags4 & 4)
        return 0;
    switch (gChain.step) {
    case 0x80:
        gChain.sub = 0;
        gChain.step--;
        /* fall through */
    case 0x7F:
        if (CountFreeMonsterZones(ctx->player) == 0)
            return 0;
        if (EffectTheFluteOfSummoningDragonPrepare(ctx, 0, 0) == 0)
            return 0;
        FormatStr(bufA, gStrFluteSpecialSummonQuestion, gStrDragon);
        TextBoxOpen(0x205, 0x914, 0xB, bufA);
        TextBoxSetMenu(1, 0, 0);
        return 0x7C;
    case 0x7E:
        if (gMain.newKeys & 2) {
            FormatStr(bufB, gStrFluteSpecialSummonQuestion, gStrDragon);
            TextBoxOpen(0x205, 0x914, 0xB, bufB);
            TextBoxSetMenu(1, 0, 0);
            return 0x7C;
        }
        if (DuelCursor_PickTarget(1) != 0) {
            int pl = ctx->player;
            u32 word = *(u32 *)(pl * 0xD64 + gDuelScreen.cursor * 4 + (u32)gDuelHands);
            u32 id = CARD_ID11(word);

            if (CARD_TYPE(id) == 1 && (IsToonMonster(CARD_NUMBER(id)) == 0 || HasFaceUpToonWorld(ctx->player) != 0)) {
                u32 *hand = (u32 *)((1 & ctx->player) * 0xD64 + (u32)gDuelHands);
                u32 *card = hand + gDuelScreen.cursor;
                /* FAKEMATCH: the signed byte/halfword flag adds RTL insns while the card pointer is live, so
                 * global alloc ranks the type (r4) above the card pointer (r5) as in the ROM. */
                s16 f = (s8)PLAYER_RAW(ctx);

                DuelCmd_Push(f ? 0x80C2 : 0xC2, ((u16 *)card)[0], ((u16 *)card)[1], 0);
                QueueSpecialSummonChoosePosition(ctx->player, card, 1, 0);
                return 0x7D;
            }
            PlaySE(3);
        }
        return 0x7E;
    case 0x7D:
        gChain.sub++;
        if (gChain.sub == 2)
            return 0;
        if (CountFreeMonsterZones(ctx->player) != 0) {
            int i;

            for (i = 0; i < gDuelPlayers[1 & ctx->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & ctx->player].hand[i]));
                /* FAKEMATCH: a named type-minus-one keeps the base copy before the 0x7FF load (r7/r4 as in
                 * the ROM) and compares against the immediate 1. */
                int t = CARD_TYPE(id) - 1;

                if (t != 0)
                    continue;
                TextBoxOpen(0x205, 0x914, 0xB, gStrSpecialSummonAnotherQuestion);
                TextBoxSetMenu(1, 0, 0);
                return 0x7C;
            }
        }
        return 0;
    case 0x7C:
        if (gTextBox.unk14 == 0)
            return 0;
        FormatStr(bufC, gStrFluteSelectFromHand, gStrDragon);
        TextBoxOpen(0x205, 0x914, 0xB, bufC);
        return 0x7E;
    }
    return 0;
}

