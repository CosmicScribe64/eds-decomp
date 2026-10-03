#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors: int f(struct CardRef *ref), returning 0 (or a step state 0x7F/0x80).
 * See wiki/functions/code-0803732c.md.
 */

/* struct DuelCard / DuelZone / DuelZonesPlayer and gDuelZones come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already handled/skipped (hypothesis) */
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
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern u8 gChain[];
extern const u32 gCardStats[];
#define EFF_PHASE gChain[0x3E0]  /* 0x7D-0x80: step of a multi-step effect (hypothesis) */

/* Duel card-list viewer at 0x0201D810 (see code_0802AAC0). */
struct ListView {
    u8 flags0;
    u8 step;
    u8 state;
    u8 unk3;
    u8 unk4;
    u8 row : 2;         /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 top;            /* +6 */
    u8 unk8[4];
    u32 cards[1];       /* +0xC */
};
extern struct ListView gCardListView;
struct Unk02015F00 {
    u8 pad[0x1B22];
    u16 savedTop;       /* +0x1B22: saved list position */
};
extern struct Unk02015F00 gAiWork;
/* u16 at 0x0201AE60+0x14 (nonzero = flag) */
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gTextBox;
extern char gStrPromptAddGraveMonsterToHand[];
extern char gStrSelectMonsterToAddToHand[];
#define CARD_NUMBER_SYM(id) (gCardIdToNumber[0x7FF & (id)])
#define EFF_SIDE gChain[0x3E1]
#define CARD_ID11(w) (((w) << 21) >> 21)
extern const u16 gCardIdToNumber[];
int CollectEffectTargets(int player, int number, int b);
void AiPickCardListEntry(u16 id);
void TextBoxOpen(int a, int b, int c, char *s);
void TextBoxSetMenu(int a, int b, int c);
void CardListView_Open(int player, int area, int a2, int a3);
int ReturnGraveyardCardToHand(int player, u16 no);
void sub_08019820(int player, u16 id);

int DuelPrompt_TryPostSetMonster(int player);
void ShowRevealedCard(int player, u16 id);
void BanishFieldCard(int player, int zone, int c);
int HasFlipEffect(u16 number, int a);
void BanishDeckCopies(int player, u16 number);
#define CARD_STATS_T(id) CARD_TYPE(id)
/* Per-player duel state at 0x020192E4: struct DuelPlayer[2] from duel.h (same layout). */
#define PH(p) ((struct DuelPlayer *)((u32)gDuelPlayers + (p) * 0xD64))
#define ZBC(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + 0x0201930C))
int Random(void);
void DuelCursor_Select(int player, int a, int b);
void ShowCardDetail(int player, int id);
extern u8 gChainEffectWork[];
#define E28W (*(u32 *)((u32)gChainEffectWork + 8))
#define E28H0 (*(u16 *)((u32)gChainEffectWork + 8))
#define E28H1 (*(u16 *)((u32)gChainEffectWork + 10))
#define E21 (*(u8 *)((u32)gChainEffectWork + 1))
/* Effect scratch object at 0x02017E20 (only a byte at +1 and a card-ish word at +8 are used). */
struct Unk02017E20 {
    u8 unk0;
    u8 side;            /* +1: player being processed (hypothesis) */
    u8 pad2[6];
    u32 word8;          /* +8: card id in bits 0-11, bit 12 = player, bit 17 = flag (hypotheses) */
};
void CopyDuelCard(void *dst, void *src);
void QueueSpecialSummon(int player, u32 *card, int a, int b, int c);
void DiscardHandCard(int player, int a, int b, int c);
int CountFreeMonsterZones(int player);
int IsSpecialSummonOnly(u16 id);
void ReturnAllMonstersToDeck(int player, int a);
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
/* The turn-side bit at 0x020192E0+0x1B12 bit 1 is duel.h's struct DuelState.linkSkip. */
extern u8 gDuelCtrl[];
struct PosWord {
    u32 lo : 12;
    u32 flag12 : 1;     /* bit 12: player (hypothesis) */
    u32 rest : 19;
};
void DuelPrompt_Post(int a, int b, int c, int d);
void QueueSpecialSummonChoosePosition(int player, u32 *card, int a, int b);
void GainLifePoints(int player, int lp);
void LoseLifePoints(int player, int lp);
void ShowDestroyedCard(int player, int id);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int CountGraveyardCardsByNumber(int player, u16 number);
void DestroyInvalidEquips(int a, int b);
void UpdateSpellTrapNegation(int a);
void FlipFieldCard();
int IsEffectMonster(int id);
int CountActiveCardsOnField(int player, u16 number);
void ChangeBattlePosition(int player, int zone, int a, int b);
int GetZoneCardType(int player, int zone);
int GetZoneCardAtk(int player, int zone);
int HalveRoundUp(int a);
void QueueAddZoneLink(int player, int a, u16 b, u16 c);

int EffectEachPlayerReviveResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (CARD_NUMBER(ref->id) == 0x45C && (0xE & ((u8 *)ref)[2]) != 6) {
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            return 0;
        }
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = gDuel.linkSkip;
            EFF_PHASE--;
        case 0x7F:
            if (CountFreeMonsterZones(EFF_SIDE) > 0 && CollectEffectTargets(EFF_SIDE, CARD_NUMBER(ref->id), 0) > 0) {
                DuelPrompt_Post(EFF_SIDE, 0xE, ref->id, ref->player);
                return 0x7E;
            }
            return 0x7D;
        case 0x7E: {
            struct PosWord x;
            struct PosWord *px;
            u8 *g = (u8 *)&gDuel;

            *(u32 *)&x = *(u16 *)(g + 0x1B64) | *(u16 *)(g + 0x1B66) << 16;
            px = &x;
            if ((gDuelCtrl[1] & 1) && (g[0x1B12] & 2))
                px->flag12 = 1 - px->flag12;
            DuelCmd_Push(EFF_SIDE ? 0x80D3 : 0xD3, *(u32 *)&x & 0xFFFF, *(u32 *)&x >> 16, 0);
            switch (CARD_NUMBER(ref->id)) {
            case 0x45C:
                QueueSpecialSummonChoosePosition(EFF_SIDE, (u32 *)&x, 0, 0x20);
                break;
            case 0x487:
                QueueSpecialSummon(EFF_SIDE, (u32 *)&x, 0, 1, 0x20);
                break;
            }
            return 0x7D;
        }
        case 0x7D: {
            int sd;

            EFF_SIDE = 1 - EFF_SIDE;
            sd = gDuel.linkSkip;
            if (*(volatile u8 *)&EFF_SIDE != sd)   /* volatile: the ROM re-reads the byte after the store */
                return 0x7F;
            return 0x64;
        }
        }
    }
    return 0;
}
int EffectNegateCardsThisTurnResolve(struct CardRef *ref)
{
    if (EFF_PHASE == 0x80 && !ref->skip4) {
        int msg;

        switch (CARD_NUMBER(ref->id)) {
        case 0x470:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8015 : 0x15;
            break;
        case 0x471:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8016 : 0x16;
            break;
        case 0x472:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8018 : 0x18;
            break;
        case 0x473:
            msg = (1 & ((u8 *)ref)[2]) ? 0x8017 : 0x17;
            break;
        default:
            goto fail;
        }
        DuelCmd_Push(msg, 1, 0, 0);
        return 0x7F;
    }
fail:
    UpdateSpellTrapNegation(0);
    return 0;
}
int EffectNuminousHealerResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 n = CARD_NUMBER(ref->id);

        switch (n) {
        case 0x474: {
            int p = ref->player;

            GainLifePoints(p, CountGraveyardCardsByNumber(ref->player, n) * 500 + 1000);
            break;
        }
        case 0x518: {
            LoseLifePoints(1 - ref->player, CountGraveyardCardsByNumber(ref->player, n) * 300 + 700);
            break;
        }
        }
    }
    return 0;
} /* 0x080375F8 size 0x90 */
int EffectDeclareTypeResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            if (ref->numTargets == 1 && ref->targets[0] != 0) {
                DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8087 : 0x87, ref->zone, ref->targets[0], 0);
                return 0x7F;
            }
        } else if (CARD_NUMBER(ref->id) == 0x479) {
            int i, j;

            for (i = 0; i <= 1; i++)
                for (j = 0; j <= 4; j++)
                    DestroyInvalidEquips(i, j);
        }
    }
    return 0;
}
int EffectBackupSoldierResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (!CollectEffectTargets(ref->player, 0x47B, 0))
                goto ret0;
            if (1 & ((u8 *)ref)[2]) {
                AiPickCardListEntry(ref->id);
                gCardListView.row = 0;
                gCardListView.top = gAiWork.savedTop;
                return 0x7C;
            } else {
                TextBoxOpen(0x206, 0x712, 0xB, gStrPromptAddGraveMonsterToHand);
                TextBoxSetMenu(1, 0, 0);
                return 0x7E;
            }
        case 0x7E:
            if (gTextBox.flag14 == 0)
                goto ret0;
            TextBoxOpen(0x206, 0x712, 0xB, gStrSelectMonsterToAddToHand);
            return 0x7D;
        case 0x7D:
            CardListView_Open(ref->player, -1, 0x47B, 0);
            return 0x7C;
        case 0x7C: {
            struct ListView *lv;

            ReturnGraveyardCardToHand(ref->player, CARD_NUMBER(CARD_ID11((lv = &gCardListView)->cards[lv->top + lv->row])));
            sub_08019820(ref->player, CARD_ID(lv->cards[lv->top + lv->row]));
            if (--EFF_SIDE != 0)
                return 0x7F;
            return 0x64;
        }
        }
    }
ret0:
    return 0;
}
int EffectMajorRiotResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i, j;

            for (i = 0; i <= 1; i++) {
                ref->targets[i] = 0;
                for (j = 0; j <= 4; j++) {
                    struct DuelZone *z = ZB(i & 1, j);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        DuelCmd_Push(i ? 0x8080 : 0x80, j, 0, 0);
                        ref->targets[i]++;
                    }
                }
            }
            EFF_SIDE = ref->player;
            return 0x7F;
        }
        case 0x7F:
            if (ref->targets[ref->player] != 0 && DuelPrompt_TryPostSetMonster(ref->player)) {
                ref->targets[ref->player]--;
                return 0x7F;
            }
            goto ret7E;
        case 0x7E:
            if (ref->targets[1 - ref->player] != 0 && DuelPrompt_TryPostSetMonster(1 - ref->player)) {
                ref->targets[1 - ref->player]--;
ret7E:
                return 0x7E;
            }
            return 0x7D;
        }
    }
    return 0;
}
int EffectCeasefireResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i, j;
        int count;

        /* Approximately a flip-up of every card with flag bits & 3 == 1 in zones 0-4 of both sides (hypothesis) */
        for (i = 0; i <= 1; i++) {
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                if (CARD_WORD(z->card) << 20 != 0 && (3 & ZFLAGS(z)) == 1)
                    FlipFieldCard(i, j, 0);
            }
        }
        count = 0;
        for (i = 0; i <= 1; i++) {
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);
                int id = CARD_ID(CARD_WORD(z->card));

                if (id != 0 && IsEffectMonster(id) != 0)
                    count++;
            }
        }
        if (count > 0)
            LoseLifePoints(1 - ref->player, count * 500);
    }
    return 0;
}
int EffectNoblemanResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            u8 tp;
            int tz;
            struct DuelZone *z;
            int id;
            int pa;

            if (ref->numTargets != 1)
                goto ret0;
            tp = ref->targets[0];
            tz = ref->targets[0] >> 8;
            pa = tp & 1;
            z = ZB(pa, tz);
            id = CARD_ID(CARD_WORD(z->card));
            if (id == 0)
                goto ret0;
            if (2 & ZFLAGS(z))
                goto ret0;
            ref->targets[1] = id;
            FlipFieldCard(tp, tz, 0, 0);
            ShowRevealedCard(tp, ref->targets[1]);
            BanishFieldCard(tp, tz, 0);
            switch (CARD_NUMBER(ref->id)) {
            case 0x485:
                if (CARD_TYPE(ref->targets[1]) > 0x14)
                    goto ret0;
                if (HasFlipEffect(CARD_NUMBER(ref->targets[1]), 1) != 0 || HasFlipEffect(CARD_NUMBER(ref->targets[1]), 0) != 0)
                    goto ok;
                goto ret0;
            case 0x486:
                if (CARD_TYPE(ref->targets[1]) != 0x15)
                    goto ret0;
ok:
                return 0x7F;
            default:
                goto ret0;
            }
        }
        case 0x7F:
            BanishDeckCopies(ref->player, CARD_NUMBER(ref->targets[1]));
            return 0x7E;
        case 0x7E:
            BanishDeckCopies(1 - ref->player, CARD_NUMBER(ref->targets[1]));
            return 0x7D;
        case 0x7D:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x7C;
        case 0x7C:
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x7B;
        }
    }
ret0:
    return 0;
}
int EffectInspectionResolve(struct CardRef *ref)
{
    u8 skip = 4 & ((u8 *)ref)[4];

    if (!skip && (0xFC & ((u8 *)ref)[3]) == 0xC) {
        switch (EFF_PHASE) {
        case 0x80: {
            int pp = 1 & ref->player;
            struct DuelPlayer *ph0;
            struct DuelZone *z = (struct DuelZone *)(ref->zone * 0x94 + pp * 0xD64 + (u32)gDuelPlayers + 0x28);

            if (CARD_WORD(z->card) << 20 == 0)
                goto ret0;
            ph0 = gDuelPlayers;
            EFF_SIDE = ph0[(1 - ref->player) & 1].handCount * 2;
            if (ph0[(1 - ref->player) & 1].handCount == 1) {
                gChain[0x3E2] = 0;
                return 0x78;
            }
            EFF_PHASE--;
        }
        case 0x7F:
            if (EFF_SIDE != 0) {
                int side;
                int rnd;
                struct DuelPlayer *ph;

                EFF_SIDE--;
                side = 1 - ref->player;
                rnd = Random();
                ph = gDuelPlayers;
                DuelCursor_Select(side, 0xB, rnd % ph[(1 - ref->player) & 1].handCount);
                return 0x7F;
            } else {
                int rnd;
                struct DuelPlayer *ph;

                rnd = Random();
                ph = gDuelPlayers;
                gChain[0x3E2] = rnd % ph[(1 - ref->player) & 1].handCount;
                return 0x78;
            }
        case 0x78: {
            DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, 1 - ref->player, gChain[0x3E2] << 8 | 0xB, 0);
            ShowCardDetail(ref->player, CARD_ID(CARD_WORD(PH((1 - ref->player) & 1)->hand[gChain[0x3E2]])));
        }
        }
    }
ret0:
    return 0;
}
int EffectProhibitionResolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        ShowDestroyedCard(ref->player, ref->targets[0]);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x808E : 0x8E, ref->zone, ref->targets[0], 0);
    }
    return 0;
}
static inline u32 ED8Level(int type, int id)
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
static inline int ED8Idx(int p) { return (u8)p & 1; }
static inline int ED8Idx2(u8 p) { return p & 1; }
static inline struct DuelPlayer *ED8Pl(int p) { return &gDuelPlayers[(u8)p & 1]; }
static inline struct DuelPlayer *ED8Pl2(u8 p) { return &gDuelPlayers[p & 1]; }
static inline int ED8Hand(int p) { return gDuelPlayers[(u8)p & 1].handCount; }
int EffectMorphingJar2Resolve(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i, j;

            for (i = 0; i <= 1; i++) {
                ref->targets[i] = 0;
                for (j = 0; j <= 4; j++) {
                    u16 id = CARD_ID(CARD_WORD(gDuelPlayers[i & 1].zones[j].card));

                    if (id != 0 && CARD_TYPE(id) <= 0x14)
                        ref->targets[i]++;
                }
            }
            ReturnAllMonstersToDeck(ref->player, 1);
            EFF_SIDE = ref->player;
ret7F:
            return 0x7F;
        }
        case 0x7F: {
            if (gDuelPlayers[ED8Idx(EFF_SIDE)].deckCount != 0 && ref->targets[EFF_SIDE] != 0) {
                struct DuelCard *deck = gDuelPlayers[ED8Idx(EFF_SIDE)].deck;

                CopyDuelCard(&gChain[0x3E8], deck);
                DuelCmd_Push(EFF_SIDE ? 0x8061 : 0x61, 1, 1, 0);
                ShowRevealedCard(EFF_SIDE, CARD_ID(CARD_WORD(*deck)));
                return 0x7E;
            }
            EFF_SIDE = 1 - EFF_SIDE;
            /* FAKEMATCH: the ROM re-reads EFF_SIDE after the store */
            asm("" ::: "memory");
            if (EFF_SIDE != ref->player)
                goto ret7F;
            return 0x78;
        }
        case 0x7E: {
            int e;
            u32 w;

            w = E28W;
            if (((w << 19) >> 31) != (e = E21) && (int)(w << 14) < 0 && CARD_NUMBER(CARD_ID11(w)) == 0x2FA) {
                if (CountFreeMonsterZones(1 - e) > 0) {
                    DuelCmd_Push(E21 ? 0x80C2 : 0xC2, E28H0, E28H1, 0);
                    return 0x7D;
                }
                DiscardHandCard(E21, ED8Hand(E21) - 1, 0, 1);
                asm("");
                goto ret7F;
            }
            {
                int id = CARD_ID(E28W);
                u32 type = CARD_TYPE(id);

                if (type <= 0x14) {
                    if (ED8Level(type, id) <= 4 && IsSpecialSummonOnly(id) == 0) {
                        u16 *h = &E28H0;
                        DuelCmd_Push(E21 ? 0x80C2 : 0xC2, h[0], h[1], 0);
                        ref->targets[E21]--;
                        return 0x7C;
                    }
                    ref->targets[EFF_SIDE]--;
                }
            }
            {
                int s = EFF_SIDE;
                DiscardHandCard(s, ED8Hand(s) - 1, ref->player != s, 1);
            }
            goto ret7F;
        }
        case 0x7D:
            QueueSpecialSummon(1 - EFF_SIDE, (u32 *)&gChain[0x3E8], 0, 1, 0);
            goto ret7F;
        case 0x7C:
            QueueSpecialSummon(EFF_SIDE, (u32 *)&gChain[0x3E8], 0, 1, 0);
            goto ret7F;
        }
    }
    return 0;
}
int EffectWindstormOfEtaquaResolve(struct CardRef *ref)
{
    int flag = 0;

    if (!ref->skip4) {
        int i;

        if (CountActiveCardsOnField(0, 0x148) != 0 || CountActiveCardsOnField(1, 0x148) != 0)
            flag = 1;
        for (i = 0; i <= 4; i++) {
            int pp = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(pp, i);

            if (CARD_WORD(z->card) << 20 != 0) {
                struct DuelZone *z2 = ZB((1 - ref->player) & 1, i);

                if ((2 & ZFLAGS(z2)) != 0) {
                    u8 ok = 1;

                    if (flag) {
                        struct DuelZone *z3 = ZB((1 - ref->player) & 1, i);

                        if ((1 & ZFLAGS(z3)) != 0)
                            ok = GetZoneCardType(1 - ref->player, i) != 1;
                    }
                    if (ok)
                        ChangeBattlePosition(1 - ref->player, i, 0, 0);
                }
            }
        }
    }
    return 0;
}
int EffectSebeksBlessingResolve(struct CardRef *ref)
{
    if (!ref->skip4)
        GainLifePoints(ref->player, ref->pos);
    return 0;
}
int EffectRiryokuResolve(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int tp1 = (u8)ref->targets[0];
        int tz1 = ref->targets[0] >> 8;
        int tp2 = (u8)ref->targets[1];
        int tz2 = ref->targets[1] >> 8;
        int pa = tp1 & 1;
        struct DuelZone *za = ZB(pa, tz1);

        if (CARD_WORD(za->card) << 20 != 0 && (2 & ZFLAGS(za)) != 0) {
            int pb = tp2 & 1;
            struct DuelZone *zb = ZB(pb, tz2);

            if (CARD_WORD(zb->card) << 20 != 0 && (2 & ZFLAGS(zb)) != 0) {
                DuelCmd_Push(tp1 ? 0x8098 : 0x98, tz1, 0, 0);
                QueueAddZoneLink(ref->player, HalveRoundUp(GetZoneCardAtk(tp1, tz1)), ref->targets[1], 4);
            }
        }
    }
    return 0;
}
