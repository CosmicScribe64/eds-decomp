/*
 * ai_strategy (0x0805C508-0x0805D58C): the CPU's scripted combo strategies
 * (wiki/functions/ai-strategy-c.md).
 *
 * AiChooseStrategy (ai_steps.c) scans the CPU's hand and field for one of the nine scripted combos of
 * enum AiStrategy and stores (strategy << 1) | 1 in gAiWork (strategyActive, strategy). While a strategy
 * is active, step AI_STEP_STRATEGY runs AiStepRunStrategy, which dispatches to the matching handler below
 * every frame instead of the generic main-phase steps. Every handler is a small sub-step machine on
 * gAiState.stepState: it returns 0 while running and 1 for an invalid sub-step.
 *
 * The handlers play their cards through the same card menu the human uses: the "commit" idiom writes the
 * chosen hand card into gDuel.cardMenuCard / gDuel.cardMenu (see the local view below) and the handler
 * then waits for the menu's confirmed bit, or queues the play directly (QueueNormalSummon,
 * QueueSpecialSummonFromHand, DuelCmd_Push + Chain_AddPending). A handler that finishes or gives up
 * clears gAiWork.strategyActive; AiStepRunStrategy then resets the AI step state and the normal steps
 * resume at AI_STEP_SIMPLE_SPELLS.
 *
 * Several strategies key on effect-table numbers that are not EDS cards (CARD_1245, CARD_1314,
 * CARD_1514, CARD_1515), so this ROM never picks them. Players: 0 is the human, 1 the CPU.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, gCardStats, gCardIdToNumber */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "duel.h"               /* struct DuelState, struct DuelPlayer, struct DuelZone, gDuel, gDuelPlayers */
#include "ai.h"                     /* struct AiState, struct AiWork, gAiState, gAiWork, enum AiStrategy,
                                       AiTryPlaySpellTrap, AiPickTributeMonster */

/* ---- ROM data used only here ---- */
extern const u16 gCardNumberToId_ValkyrionTheMagnaWarrior[];   /* 0x0862448E: card id table (hypothesis; first entry is the id
                                       AiStrategyValkyrion summons) */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/*
 * Matching: gDuel as this unit reads it. Same layout as struct DuelState in the fields it names, but
 * hand1 is the raw card words (the strategies test them with CARD_ID below) and the commit block at
 * +0x1B28 (cardMenuCard, cardMenu) is declared with the u8 and u16 bitfield containers the ROM's byte and
 * halfword accesses use: clearing the menu step is a halfword store and setting the menu's player bit is a
 * byte OR. Fields after a member run on at that member's own address; all offsets are pinned by the use
 * sites. See the Matching tricks in wiki/functions/ai-strategy-c.md.
 */
struct DuelAiView {
    u8 unk0[0x13EC];
    u32 hand1[80];                  /* +0x13EC: gDuel.players[1].hand as card words (id = low 12 bits) */
    u8 unk152C[0x1B14 - 0x152C];
    union {                         /* +0x1B14: two views of the same two halfwords */
        struct { u32 lo:9, first:8, hi:15; } w;
        struct { u16 skip; u16 lo:1, second:8, hi:7; } h;
    } counters;
    u8 unk1B18[0x1B28 - 0x1B18];
    struct {                        /* +0x1B28: cardMenuCard and cardMenu (16 bytes) */
        u16 cardId;                 /* +0x00: cardMenuCard, the committed hand card's id */
        union {                     /* +0x04: cardMenu word 0 */
            u8 raw;
            struct { u8 a:1, confirmed:1, c:6; } bf;    /* bit 1 = cardMenu.confirmed */
        } f4;
        struct {                    /* +0x08: cardMenu words 1-2 */
            u16 a:2, b:8, c:6;      /* b = cardMenu.step */
            u8 pad;
            u8 f3;                  /* +0x0B: bit 1 = cardMenu.player */
        } s8;
        struct {                    /* +0x0C: cardMenu words 2-3 */
            u16 a:1, idx:8, c:7;    /* idx = cardMenu.index */
            u16 pad;
        } sC;
    } commit;
};

#define gCommitDuel ((struct DuelAiView *)&gDuel)
#define S gAiState

/* gDuel access forms the matched code loads from their own literal-pool entries (see duel_response.c). */
extern struct DuelZone gDuelZonesP1[];          /* 0x0201A070 = gDuelZones[1].zones */
extern u32 gCardListViewCards[];                /* 0x0201D81C = gCardListView.cards */

/* Board queries, summons and the card menu, with this unit's parameter types (duel_cmd.h, chain.h,
 * card_menu.h, summon.h, duel_actions.h and effect.h declare some of them more narrowly; including them
 * would change the code). */
int CountFaceUpMonstersByNumber(int player, u16 number);
int CountActiveCardsOnField(int player, u16 number);
int FindFaceUpMonsterByNumber(int player, u16 number);
int CountSpellTrapsFiltered(int player, u16 a, u16 b, u16 c);
int CountMonsters(int player);
int CanSummonFromHand(int player, u16 cardId);
int FindFreeMonsterZone(int player);
int GetFaceUpFieldMagicNumber(void);
int HasFaceUpToonWorld(int player);
u32 IsToonMonster(u16 number);
int CollectEffectTargets(int player, int cardNumber, int param);
void DuelCmd_Push(u16 cmd, u16 arg2, u16 arg4, u16 arg6);
void Chain_AddPending(u32 packed, int arg);
void CardMenu_PlaySpellTrapFromHand(int activate, int asChainLink, int unused);
void TributeMonster(int player, int zone);
void DiscardHandCard(int player, int handIdx, u16 a, u16 b);
void BanishGraveyardCard(int player, u32 *card);
void QueueNormalSummon(int player, int handIdx, int zone, u16 tributes, u16 faceUp);
void QueueSpecialSummonFromHand(int player, int handIdx, int zone, int tributes, int faceUp);

/* The low 12 bits of a card word (struct DuelCard.id, with the owner bit shifted out). */
#define CARD_ID(w) (((w) << 20) >> 20)

/* Use one literal-address view in both searches so their pool entry is shared. */
static inline u16 StrategyCardNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & CARD_ID_MASK];    /* 0x08622AB4 = gCardIdToNumber */
}

#define STRATEGY_COMMIT() do { \
    struct AiState *st = &S; \
    gCommitDuel->commit.cardId = CARD_ID(gCommitDuel->hand1[st->cardIndex]); \
    gCommitDuel->commit.s8.f3 |= 2; \
    gCommitDuel->commit.sC.idx = st->cardIndex; \
    gCommitDuel->commit.f4.bf.confirmed = 1; \
    st->stepState++; \
} while (0)

/* Strategy 1 (Valkyrion): clear the opponent's field, send the three Magnet Warriors to the graveyard
 * for Valkyrion, use its effect, then play Premature Burial or Monster Reborn. */
int AiStrategyValkyrion(void)
{
    int i, j;
    /* FAKEMATCH: each i=0..2 case initializes this callee-saved search key. */
    register int number asm("r6");
    int found;
    u32 target;
    switch (S.stepState) {
    case 0:
        if (CountSpellTrapsFiltered(0, 0, 0, 0) > 0) {
            if (AiTryPlaySpellTrap(CARD_HARPIES_FEATHER_DUSTER)) {
                gCommitDuel->commit.s8.b = 0;
                STRATEGY_COMMIT();
                return 0;
            }
            if (AiTryPlaySpellTrap(CARD_HEAVY_STORM) || AiTryPlaySpellTrap(CARD_GIANT_TRUNADE))
                goto commit;
        }
        S.stepState += 2;
        return 0;
    case 1:
    case 3:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if (!(gCommitDuel->commit.f4.raw & 2)) S.stepState--;
        return 0;
    case 2:
        if (CountMonsters(0) > 0) {
            if (AiTryPlaySpellTrap(CARD_DARK_HOLE) || AiTryPlaySpellTrap(CARD_RAIGEKI)) goto commit;
        }
        S.stepState += 2;
        return 0;
    case 4:
        if (!CanSummonFromHand(1, gCardNumberToId_ValkyrionTheMagnaWarrior[0])) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms retain this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r2") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    struct FlagByte *p = (struct FlagByte *)((u32)base + off);
                    p->found = 0;
                }
            }
            return 0;
        }
        for (i=0;i<3;i++) {
            found=0;
            switch(i) {
            case 0: number=CARD_ALPHA_THE_MAGNET_WARRIOR; break;
            case 1: number=CARD_BETA_THE_MAGNET_WARRIOR; break;
            case 2: number=CARD_GAMMA_THE_MAGNET_WARRIOR; break;
            }
            for(j=0;j<5 && !found;j++) {
                u32 id=CARD_ID(*(u32 *)(j*0x94+(u32)gDuelZonesP1));
                if(id && StrategyCardNumber(id)==number) {
                    TributeMonster(1, j);
                    found=1;
                }
            }
            for(j=0;j<gDuelPlayers[1].handCount && !found;j++) {
                /* Matching: index term first, then the hand base (the ROM computes j*4 before loading it). */
                u32 id=CARD_ID(*(u32 *)(j*4+(u32)gDuelHandP1));
                if(id && StrategyCardNumber(id)==number) {
                    DiscardHandCard(1, j, 0, 0);
                    found=1;
                }
            }
        }
        {
            int target = FindFreeMonsterZone(1);
            u8 *work = (u8 *)&gAiWork;
            work[0x1B25] = target;
        }
        i=0;
        {
          u8 *players = (u8 *)&gDuelPlayers;
          u32 countOffset = 0xD66;
          int n = *(u8 *)((u32)players + countOffset);
          if(i<n) {
            u32 needle=CARD_VALKYRION_THE_MAGNA_WARRIOR;
            int bound;
            u32 off=0x13E8;
            u32 *cards;
            u32 mask;
            const u16 *table;
            /* FAKEMATCH: retain the ROM's second hand-count read. */
            asm("" : "+r"(players));
            bound=*(u8 *)((u32)players+countOffset);
            cards=(u32 *)((u32)players+off);
            mask=CARD_ID_MASK; table=(const u16 *)0x08622AB4;
            do {
              if(table[CARD_ID(*cards)&mask]==needle)goto queue;
              cards++; i++;
            }while(i<bound);
          }
        }
        gAiWork.strategyActive=0;
        return 0;
    case 5:
        DuelCmd_Push(0x8008, 1, gAiWork.strategyZone << 8, 0);
        target=gAiWork.strategyZone;
        {
          /* FAKEMATCH: initialized packing temporaries preserve the OR order. */
          register u32 mask asm("r1") = 0x1F;
          register u32 hi asm("r0") = target & mask;
          hi <<= 16;
          {
          u32 lo=CARD_ID(*(u32 *)(target*0x94+(u32)gDuelZonesP1));
          lo |= 0x80400000;

          hi |= lo;
          Chain_AddPending(hi, 0);
          }
        }
        goto next;
    case 6:
        if(AiTryPlaySpellTrap(CARD_PREMATURE_BURIAL) || AiTryPlaySpellTrap(CARD_MONSTER_REBORN)) {
        commit:
            gCommitDuel->commit.s8.b=0;
            STRATEGY_COMMIT();
            return 0;
        }
        gAiWork.strategyActive=0;
        return 0;
    case 7:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if (!(gCommitDuel->commit.f4.raw & 2)) {
            gCommitDuel->counters.w.first=0;
            gCommitDuel->counters.h.second=0;
            S.stepState++;
        }
        return 0;
    case 8:
        gAiWork.strategyActive=0;
        return 0;
    queue:
        {
            /* FAKEMATCH: initialize the work base here, after the hand search. */
            register u8 *work asm("r0") = (u8 *)&gAiWork;
            QueueSpecialSummonFromHand(1, i, work[0x1B25], 0, 1);
        }
    next:
        S.stepState++;
        return 0;
    default:
        return 1;
    }
}
#undef STRATEGY_COMMIT

/* Strategy 2: play key 1245 (four tokens) and feed them to Cannon Soldier (never chosen in EDS):
 * commit CARD_1245, wait, then find a zone numbered 0x780..0x7CF. */
int AiStrategyFourTokensCannonSoldier(void)
{
    struct AiState *st = &S;
    int k;
    int r;
    /* FAKEMATCH: initialized before the zone scan, retained across found calls. */
    register struct DuelZone *base asm("r5");

    switch (st->stepState) {
    case 0:
        if (CountFaceUpMonstersByNumber(1, CARD_CANNON_SOLDIER) == 0)
            goto fail;
        if (AiTryPlaySpellTrap(CARD_1245) != 0) {
            {
                struct DuelAiView *duel = gCommitDuel;
                {
                    /* FAKEMATCH: initialized address terms preserve ADD order. */
                    register u32 baseAddr asm("r2") = (u32)duel;
                    register u32 off asm("r3") = 0x1B30;
                    asm("" : "+r"(off));
                    {
                        register u16 *p asm("r1") = (u16 *)(baseAddr + off);
                        u32 mask = 0xFFFFFC03;
                        register u32 old asm("r6");
                        old = *p;
                        asm("" : : "r"(old));
                        *p = mask & old;
                    }
                }
                duel->commit.cardId = CARD_ID(duel->hand1[st->cardIndex]);
                duel->commit.s8.f3 |= 2;
                duel->commit.sC.idx = st->cardIndex;
                duel->commit.f4.bf.confirmed = 1;
            }
            goto inc;
        }
        gAiWork.strategyActive = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gCommitDuel->commit.f4.raw & 2) == 0) {
        inc:
            st->stepState++;
        }
        return 0;
    case 2:
        k = CARD_1418;
        if (CountActiveCardsOnField(0, k) > 0)
            goto fail;
        if (CountActiveCardsOnField(1, k) > 0) {
            {
                /* FAKEMATCH: preserve this distinct flag-clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r3") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    u8 *p = (u8 *)((u32)base + off);
                    int mask = ~1;
                    register u32 old asm("r6");
                    old = *p;
                    asm("" : : "r"(old));
                    *p = mask & old;
                }
            }
            return 0;
        }
        if (CountFaceUpMonstersByNumber(1, CARD_CANNON_SOLDIER) == 0) {
        fail:
            gAiWork.strategyActive = 0;
            return 0;
        }
        base = gDuelZonesP1;
        {
            u32 mask = CARD_ID_MASK;
            struct DuelZone *zone = base;
            u32 span = 0x94 * 4;
            /* FAKEMATCH: initialized end pointer and range offset keep scratch registers. */
            register struct DuelZone *end asm("r3") = (struct DuelZone *)((u32)base + span);
            const u16 *table = gCardIdToNumber;

            do {
                /* Matching: whole-word load of the card word, not a halfword field load of card.id. */
                u32 id = CARD_ID(*(u32 *)&zone->card);
                if (id) {
                    u32 n = table[id & mask];
                    register int off asm("r6");
                    off = -0x780;
                    n += off;
                    if ((u16)n <= 0x4F)
                        goto found;
                }
                zone++;
            } while ((s32)zone <= (s32)end);
        }
        {
            u32 zero = 0;
            S.stepState = zero;
        }
        return 0;
    found:
        r = FindFaceUpMonsterByNumber(1, CARD_CANNON_SOLDIER);
        DuelCmd_Push(0x8008, 1, ((u32)r << 24) >> 16, 0);
        {
            u32 hi = (r & 0x1F) << 16;
            u32 lo = CARD_ID(*(u32 *)&base[r].card);
            lo |= 0x80400000;
            Chain_AddPending(hi | lo, 0);
        }
        return 0;
    default:
        return 1;
    }
}

/* Strategy 3 (Elegant Egotist): Harpie Lady, Elegant Egotist, then Rising Air Current: picks a hand
 * card, commits it and queues the play. */
int AiStrategyElegantEgotist(void)
{
    int j;
    struct AiState *st;
    u8 *targetPtr;
    struct AiWork *work;

    switch (S.stepState) {
    case 0:
        if (AiTryPlaySpellTrap(CARD_ELEGANT_EGOTIST) != 0)
            goto inc;
        if (gDuelPlayers[1].normalSummonUsed != 0)
            goto fail;
        if (FindFreeMonsterZone(1) == -1) {
            {
                struct FlagByte { u8 found : 1, rest : 7; };
                /* FAKEMATCH: initialized address terms preserve this clear prefix. */
                register u8 *base asm("r1") = (u8 *)&gAiWork;
                register u32 off asm("r6") = 0x1B24;
                asm("" : : "r"(base), "r"(off));
                {
                    struct FlagByte *p = (struct FlagByte *)((u32)base + off);
                    p->found = 0;
                }
            }
            return 0;
        }
        gAiWork.strategyZone = FindFreeMonsterZone(1);
        j=0;
        {
          u32 offset = 0xD66;
          int n = *(u8 *)((u32)&gDuelPlayers + offset);
          if(j<n) {
            /* FAKEMATCH: initialized search key in the original saved register. */
            register u32 needle asm("r6")=0x4E1;
            int bound=n;
            u32 off=0x13E8;
            u32 *cards=(u32 *)((u32)&gDuelPlayers+off);
            u32 mask=CARD_ID_MASK;
            const u16 *table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == needle)
                goto queue1;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        j=0;
        {
          u8 *players = (u8 *)&gDuelPlayers;
          u32 offset = 0xD66;
          u8 *count = (u8 *)((u32)players + offset);
          int n;
          work=&gAiWork;
          n=*count;
          if(j<n) {
            int bound;
            u32 off=0x13E8;
            u32 *cards;
            u32 mask;
            const u16 *table;
            /* FAKEMATCH: retain the second scan's count reload. */
            asm("" : "+r"(players));
            bound=*(u8 *)((u32)players+offset);
            cards=(u32 *)((u32)players+off);
            mask=CARD_ID_MASK; table=gCardIdToNumber;
            do {
              if (table[CARD_ID(*cards) & mask] == CARD_HARPIE_LADY)
                goto queue2;
              cards++;
              j++;
            } while (j < bound);
          }
        }
        work->strategyActive=0;
        return 0;
    case 1:
        if (AiTryPlaySpellTrap(CARD_ELEGANT_EGOTIST) == 0)
            goto fail;
        gCommitDuel->commit.s8.b = 0;
        st = &S;
        gCommitDuel->commit.cardId = CARD_ID(gCommitDuel->hand1[st->cardIndex]);
        gCommitDuel->commit.s8.f3 |= 2;
        gCommitDuel->commit.sC.idx = st->cardIndex;
        gCommitDuel->commit.f4.bf.confirmed = 1;
        st->stepState++;
        return 0;
    fail:
        gAiWork.strategyActive=0;
        return 0;
    case 2:
    case 4:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gCommitDuel->commit.f4.raw & 2) == 0)
            S.stepState++;
        return 0;
    case 3:
        if (GetFaceUpFieldMagicNumber() == CARD_RISING_AIR_CURRENT)
            goto reset;
        if (AiTryPlaySpellTrap(CARD_RISING_AIR_CURRENT) == 0)
            goto reset;
        gCommitDuel->commit.s8.b = 0;
        st = &S;
        gCommitDuel->commit.cardId = CARD_ID(gCommitDuel->hand1[st->cardIndex]);
        gCommitDuel->commit.s8.f3 |= 2;
        gCommitDuel->commit.sC.idx = st->cardIndex;
        gCommitDuel->commit.f4.bf.confirmed = 1;
        st->stepState++;
        return 0;
    case 5:
    reset:
        S.stepState = 0;
        return 0;
    queue1:
        {
            /* FAKEMATCH: initialized base is loaded after the first scan. */
            register u8 *first asm("r0") = (u8 *)&gAiWork;
            targetPtr = first + 0x1B25;
        }
        goto send;
    queue2:
        targetPtr=(u8 *)work+0x1B25;
    send:
        QueueNormalSummon(1, j, *targetPtr, 0, 1);
    inc:
        S.stepState++;
        return 0;
    default:
        return 1;
    }
}

/* Strategy 4: play key 1314 (double the Machines' ATK; never chosen in EDS): commits the chosen hand
 * card, then waits for the commit flag. */
int AiStrategyDoubleMachineAtk(void)
{
    struct AiState *st = &S;

    switch (st->stepState) {
    case 0:
        if (AiTryPlaySpellTrap(CARD_1314) != 0) {
            gCommitDuel->commit.s8.b = 0;
            gCommitDuel->commit.cardId = CARD_ID(gCommitDuel->hand1[st->cardIndex]);
            gCommitDuel->commit.s8.f3 |= 2;
            gCommitDuel->commit.sC.idx = st->cardIndex;
            gCommitDuel->commit.f4.bf.confirmed = 1;
            st->stepState++;
            return 0;
        }
        gAiWork.strategyActive = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gCommitDuel->commit.f4.raw & 2) == 0)
            st->stepState++;
        return 0;
    case 2:
        gAiWork.strategyActive = 0;
        return 0;
    default:
        return 1;
    }
}
/* Value of a card for the "sacrifice the weakest" choice: 0 for type 0x15-0x17, 4000 for 0x18, else 10 * a 9-bit stats field. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* 0x08621DE0 = gCardStats */
static inline int CardValue(u32 id)
{
    int r;

    switch ((int)((CARD_STATS(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}

/* Strategy 6: banish 3 graveyard monsters to Special Summon key 1514 (never chosen in EDS). */
/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant CARD_ID_MASK before the base;
 * it emits no instruction and does not change the card-value calculation. */
int AiStrategyBanishThreeSummon(void)
{
    register int count __asm__("r6");
    int best;
    int bestValue;
    int i;
    u32 id;

    switch (S.stepState) {
    case 0:
        S.stepIndex = 3;
        S.stepState++;
    case 1:
        count = CollectEffectTargets(1, CARD_1514, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = CARD_ID_MASK;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gCardListViewCards;
            do {
                id = CARD_ID(cards[i]);
                if (bestValue > CardValue(id)) {
                    bestValue = CardValue(id);
                    best = i;
                }
                i++;
            } while (i < count);
        }
        if (best < 0)
            goto fail;
        BanishGraveyardCard(1, &gCardListViewCards[best]);
        if (--S.stepIndex == 0)
            S.stepState++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gDuelPlayers[1].handCount;
            if (i < n) {
                u32 needle = CARD_1514;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gDuelPlayers + offset);
                mask = CARD_ID_MASK;
                table = gCardIdToNumber;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        QueueSpecialSummonFromHand(1, i, FindFreeMonsterZone(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gAiWork.strategyActive = 0;
        return 0;
    fail:
        gAiWork.strategyActive = 0;
        return 0;
    default:
        return 1;
    }
}

/* Strategy 7: as strategy 6 with 2 monsters and key 1515 (no CARD_ name; never chosen in EDS). */
/* FAKEMATCH: count in r6 and candidate base in r8 steer old_agbcc allocation.
 * The initialized r3 mask input stages the loop-invariant CARD_ID_MASK before the base;
 * it emits no instruction and does not change the card-value calculation. */
int AiStrategyBanishTwoSummon(void)
{
    register int count __asm__("r6");
    int best;
    int bestValue;
    int i;
    u32 id;

    switch (S.stepState) {
    case 0:
        S.stepIndex = 2;
        S.stepState++;
    case 1:
        count = CollectEffectTargets(1, 0x5EB, 0);
        best = -1;
        bestValue = 9999;
        i = 0;
        if (i < count) {
            register u32 loadMask __asm__("r3") = CARD_ID_MASK;
            register u32 *cards __asm__("r8");
            __asm__ volatile("" : : "r"(loadMask));
            cards = gCardListViewCards;
            do {
                id = CARD_ID(cards[i]);
                if (bestValue > CardValue(id)) {
                    bestValue = CardValue(id);
                    best = i;
                }
                i++;
            } while (i < count);
        }
        if (best < 0)
            goto fail;
        BanishGraveyardCard(1, &gCardListViewCards[best]);
        if (--S.stepIndex == 0)
            S.stepState++;
        return 0;
    case 2:
        i = 0;
        {
            u32 *cards;
            int n = gDuelPlayers[1].handCount;
            if (i < n) {
                u32 needle = 0x5EB;
                int bound = n;
                u32 offset = 0x13E8;
                u32 mask;
                const u16 *table;
                cards = (u32 *)((u32)&gDuelPlayers + offset);
                mask = CARD_ID_MASK;
                table = gCardIdToNumber;
                do {
                    if (table[CARD_ID(*cards) & mask] == needle) {
                        QueueSpecialSummonFromHand(1, i, FindFreeMonsterZone(1), 0, 1);
                        goto fail;
                    }
                    cards++;
                    i++;
                } while (i < bound);
            }
        }
        gAiWork.strategyActive = 0;
        return 0;
    fail:
        gAiWork.strategyActive = 0;
        return 0;
    default:
        return 1;
    }
}

/* FAKEMATCH: initialized address terms retain the loop-tail count load. */
static inline u8 StrategyHandCount(void)
{
    register u8 *base asm("r0") = (u8 *)&gDuelPlayers;
    register u32 off asm("r2") = 0xD66;
    asm("" : : "r"(base), "r"(off));
    return *(u8 *)((u32)base + off);
}

/* Strategy 8 (Toon World): play Toon World, then summon the Toon monsters of the hand. */
int AiStrategyToonWorld(void)
{
    struct AiState *st = &S;
    int j;
    int a;
    int b;
    u32 id;
    const u16 *p;

    switch (st->stepState) {
    case 0:
        if (HasFaceUpToonWorld(1) != 0) {
            st->stepState = 2;
            return 0;
        }
        if (AiTryPlaySpellTrap(CARD_TOON_WORLD) != 0) {
            gCommitDuel->commit.s8.b = 0;
            gCommitDuel->commit.cardId = CARD_ID(gCommitDuel->hand1[st->cardIndex]);
            gCommitDuel->commit.s8.f3 |= 2;
            gCommitDuel->commit.sC.idx = st->cardIndex;
            gCommitDuel->commit.f4.bf.confirmed = 1;
            goto inc;
        }
        gAiWork.strategyActive = 0;
        return 0;
    case 1:
        CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        if ((gCommitDuel->commit.f4.raw & 2) == 0) {
        inc:
            st->stepState++;
        }
        return 0;
    case 2:
        if (HasFaceUpToonWorld(1) != 0)
            goto inc;
        gAiWork.strategyActive = 0;
        return 0;
    case 3:
        j = 0;
        if (j < gDuelPlayers[1].handCount) {
            do {
                {
                    /* FAKEMATCH: initialized raw word stays in the load scratch.
                     * Matching: index term first, then the hand base. */
                    register u32 raw asm("r0") = *(u32 *)(j * 4 + (u32)gDuelHandP1);
                    raw <<= 20;
                    id = raw >> 20;
                }
                {
                    /* FAKEMATCH: retain the initialized mask copy before scaling id. */
                    register u32 mask asm("r0") = CARD_ID_MASK;
                    asm("" : : "r"(mask));
                    {
                        register u32 saved asm("r1") = mask;
                        register u32 off asm("r0");
                        asm("" : "+r"(saved));
                        off = id;
                        off &= saved;
                        off <<= 1;
                        off += (u32)gCardIdToNumber;
                        p = (const u16 *)off;
                    }
                }
                if (IsToonMonster(*p) == 0)
                    continue;
                {
                    /* FAKEMATCH: initialize the first argument before copying id. */
                    register int player asm("r0") = 1;
                    if (CanSummonFromHand(player, id) == 0)
                        continue;
                }
                switch (*p) {
                case CARD_TOON_MERMAID:
                    QueueSpecialSummonFromHand(1, j, FindFreeMonsterZone(1), 0, 1);
                    S.stepState--;
                    return 0;
                case CARD_TOON_SUMMONED_SKULL:
                    a = AiPickTributeMonster(-1, 0);
                    if (a == -1)
                        break;
                    {
                        u32 packed = 0x90;
                        packed |= a;
                        QueueSpecialSummonFromHand(1, j, a, packed, 1);
                    }
                    S.stepState--;
                    return 0;
                case CARD_BLUE_EYES_TOON_DRAGON:
                    a = AiPickTributeMonster(-1, 0);
                    b = AiPickTributeMonster(a, 0);
                    if (a == -1 || b == -1)
                        break;
                    {
                        /* FAKEMATCH: initialized tag and its copy keep the byte-pack order. */
                        register int t asm("r2") = -0x70;
                        asm("" : : "r"(t));
                        {
                            register u32 mask asm("r0") = t;
                            u32 lo = (u32)(mask | a) << 24;
                            u32 hi = (u32)(mask | b) << 24;
                            lo >>= 8;
                            QueueSpecialSummonFromHand(1, j, a, (lo | hi) >> 16, 1);
                        }
                    }
                    S.stepState--;
                    return 0;
                }
            } while (++j < StrategyHandCount());
        }
        {
            /* FAKEMATCH: preserve the distinct exhausted-hand flag-clear prefix. */
            register u8 *base asm("r1") = (u8 *)&gAiWork;
            register u32 off asm("r3") = 0x1B24;
            asm("" : : "r"(base), "r"(off));
            {
                u8 *p = (u8 *)((u32)base + off);
                int mask = ~1;
                u32 old;
                old = *p;
                *p = mask & old;
            }
        }
        return 0;
    default:
        return 0;
    }
}

/* Strategy 5: give up at once (never chosen): clears the "candidate found" flag. */
int AiStrategyNone(void)
{
    gAiWork.strategyActive = 0;
    return 0;
}

/* Step 8: run the handler of gAiWork.strategy; when it clears strategyActive, continue with the normal
 * steps at AI_STEP_SIMPLE_SPELLS. */
int AiStepRunStrategy(void)
{
    u16 r = 1;

    switch (gAiWork.strategy) {
    case 0:
        r = AiStrategyCyberStein();
        break;
    case 1:
        r = AiStrategyValkyrion();
        break;
    case 2:
        r = AiStrategyFourTokensCannonSoldier();
        break;
    case 3:
        r = AiStrategyElegantEgotist();
        break;
    case 4:
        r = AiStrategyDoubleMachineAtk();
        break;
    case 5:
        r = AiStrategyNone();
        break;
    case 6:
        r = AiStrategyBanishThreeSummon();
        break;
    case 7:
        r = AiStrategyBanishTwoSummon();
        break;
    case 8:
        r = AiStrategyToonWorld();
        break;
    }
    if (gAiWork.strategyActive == 0) {
        S.step = 1;
        S.stepState = 0;
        S.stepIndex = 0;
        S.unk4 = 0;
        S.flipZone = 0;
        return 0;
    }
    return r;
}
