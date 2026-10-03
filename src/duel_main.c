/*
 * Chain resolution, the per-frame duel loop and the win check (wiki/functions/duel-main-c.md).
 *
 *  - Chain_Update is called every frame by DuelMainStep. It runs Chain_Build (duel_setup.c) or Chain_Resolve
 *    while they are active, and otherwise starts a new chain from the pending activations.
 *  - Chain_Resolve resolves gChain.links from the last link back: negation checks, the card's resolve handler
 *    from gCardEffects, and the move of one-shot cards to the graveyard.
 *  - Duel_CheckWin decides the duel: 0 LP, deck out, Exodia in the hand, the Destiny Board keys.
 *  - DuelPhase_Init and DuelPhase_ShowResult are duel steps 0 and 9 (gDuelPhaseTable).
 *  - DuelMainStep is the shared Campaign / Link Battle duel loop: the busy subsystems in priority order, the
 *    link interrupt ("Just a moment"), then the handler of the current duel step.
 */
#include "global.h"
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, SPELL_*, CARD_STATS_TYPE_* / CARD_STATS_SUBTYPE_* */
#include "constants/duel.h"         /* enum DuelStep, DuelResult, ChainEntryKind */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK */
#include "util.h"                   /* MemCopy16 */
#include "save.h"                   /* SaveGame */

/* ---- BEGIN pre-H0 subset of duel.h and sound.h ---- */
/*
 * include/duel.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md); chain.h, duel_cmd.h, duel_screen.h, duel_link.h, card_list_view.h and
 * summon.h include duel.h. Until then this block repeats the part of the new duel.h and sound.h that the unit
 * uses, with the same tags, field names, types and bitfield containers (structs are cut after the last field
 * used here and padded to their size). It defines GUARD_DUEL_H so that the headers below skip the legacy
 * file. After H0, replace the block (BEGIN to END) with the #include lines of these headers, in this order:
 *     duel.h sound.h
 * (checked: that gives the same assembly with the new headers). The unit has no include line that the H0 sed
 * rewrites.
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:19;
};

struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;
    u8 unk4[0x94 - 4];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 banishedCount;
    u8 deckOut:1;                   /* +0x007 bit 0 */
    u8 exodiaWin:1;                 /* +0x007 bit 1 */
    u8 destinyBoardWin:1;           /* +0x007 bit 2 */
    u8 unk7_3:5;
    u8 unk8[0xD64 - 8];
};

struct CardMenu {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u32 state:8;
    u32 step:8;
    u8 unk6[6];
};

struct DuelState {
    u16 serial;
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004 */
    u8 fieldBackground:4;           /* +0x1ACC */
    u8 duelOver:1;
    u8 unk1ACC_5:1;
    u8 magicNegated:1;
    u8 trapsNegated:1;
    u8 equipMagicNegated:1;         /* +0x1ACD */
    u8 equipMagicNegatedThisTurn:1;
    u8 fieldMagicNegatedThisTurn:1;
    u8 contMagicNegatedThisTurn:1;
    u8 contTrapNegatedThisTurn:1;
    u8 statChangesReversed:1;
    u8 atkDefSwapped:1;
    u8 unk1ACD_7:1;
    u8 unk1ACE[0x1B10 - 0x1ACE];
    u16 turnCount;                  /* +0x1B10 */
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;
    u8 phase:3;
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13_0:1;                 /* +0x1B13 */
    u8 unk1B13_1:7;
    u16 unk1B14_0:1;                /* +0x1B14 */
    u16 interruptActive:1;
    u16 unk1B14_2:14;
    u8 unk1B16[0x1B20 - 0x1B16];
    u8 phaseStep;                   /* +0x1B20 */
    u8 phaseCounter;                /* +0x1B21 */
    u8 unk1B22[0x1B2C - 0x1B22];
    struct CardMenu cardMenu;       /* +0x1B2C */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;
extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];

u32 HasFlipEffect(u16 cardNo, int inBattle);
void CopyDuelCard(u32 *dst, u32 *src);
int CountGraveyardCardsByNumber(int player, u16 cardNo);
int CountHandCardsByNumber(int player, u16 cardNo);
int CountActiveCardsOnField(int player, u16 cardNo);

void StopBGM(void);                             /* sound.h */
/* ---- END pre-H0 subset ---- */

#include "chain.h"                  /* gChain, struct ChainEntry, Chain_* */
#include "duel_cmd.h"               /* gDuelCmd, DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl, the duel steps defined here */
#include "duel_link.h"              /* gLinkState, enum LinkMsgId, DuelLink_SendMessage* */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_Select, ChainListScreen_*, field drawing */
#include "text_box.h"               /* gTextBox, TextBoxDrawSprites */
#include "effect.h"                 /* gCardEffects, FindCardEffect, LoseLpOnSendToGraveyard */
#include "duel_actions.h"           /* DestroyFaceUpCardsByNumber, ShowCardEffect */
#include "card_list_view.h"         /* CardListView_Run */
#include "summon.h"                 /* SummonAction_Update */

/* ---- Local views kept for matching ---- */

/* The duel-step handlers, indexed by gDuelCtrl.phase (enum DuelStep); NULL after DUEL_STEP_RESULT. */
extern u16 (*const gDuelPhaseTable[])(void);   /* 0x08198F80 */

/* DuelMainStep tests these results as u16 (the definitions return int or u32); the wider return type changes
 * the tests. */
u16 TextBoxUpdate16(void) asm("TextBoxUpdate");
u16 DuelPrompt_Run16(void) asm("DuelPrompt_Run");
u16 EventResponse_Update16(void) asm("EventResponse_Update");

/* UpdateSpellTrapNegation is defined (void); DuelMainStep passes 1 in r0, which it ignores. */
void UpdateSpellTrapNegation1(int unused) asm("UpdateSpellTrapNegation");

/* gMain +0x4870 with u16 containers (main.h: u8). DuelPhase_Init needs the wider container: with a u8 one
 * the `& 1` of firstPlayer is CSE'd with the later link-duel test. */
struct MainResultView {
    u8 unk0[0x4870];
    u16 firstPlayer:1;              /* +0x4870 bit 0 */
    u16 opponent:5;
    u16 result:2;                   /* +0x4870 bits 6-7 */
    u16 unk4871:8;
};
extern struct MainResultView gMainResultView asm("gMain");

/*
 * The views below are reached through casts of the real symbol (&gDuel, &gChain), not through asm() aliases:
 * so every address is formed as base + offset like the other fields of the object and can share its base
 * register. (A byte cast such as ((u8 *)&gChain)[0x3D0] folds into one literal, and an alias symbol does not
 * share the base register of gChain.)
 */

/* gDuel.cardMenu with state:8 in a u16 container (duel.h: u32). The narrower container gives DuelMainStep the
 * ROM's register allocation and lets the two step-reset tails merge. Same bits as struct CardMenu. */
struct CardMenuU16 {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u16 state:8;                    /* bits 26-33 */
};
struct DuelCardMenuU16View {
    u8 unk0[0x1B2C];
    struct CardMenuU16 cardMenu;    /* +0x1B2C */
};
#define gDuelCardMenuU16 ((*(struct DuelCardMenuU16View *)&gDuel).cardMenu)

/* gChain as Chain_Resolve and Chain_Update access it: the build flags as one byte, and the saved card as a
 * plain word, whose fields are taken with shifts of the loaded word (struct DuelCard members would be read
 * with narrower loads). */
struct ChainStateWordView {
    u8 unk0[0x3D0];
    u8 buildFlags;                  /* +0x3D0: building (bit 0) and buildStep (bits 1-7) */
    u8 unk3D1[0x3DC - 0x3D1];
    u32 savedCard;                  /* +0x3DC: struct DuelCard word */
};
#define gChainWords (*(struct ChainStateWordView *)&gChain)
#define CARD_WORD_ID(word)      ((word) << 20 >> 20)    /* struct DuelCard.id */
#define CARD_WORD_OWNER(word)   ((word) << 19 >> 31)    /* struct DuelCard.owner */

/* The type of gChain.resolve. */
typedef u32 (*ChainResolveFunc)(struct ChainEntry *link, struct ChainEntry *chainedTo);

#define LAST_LINK       (gChain.links[gChain.linkCount - 1])    /* the link being resolved */
#define PREV_LINK       (gChain.links[gChain.linkCount - 2])    /* the link it answers */

/* The two links sent to the partner when it resolves one of its links (LINKMSG_REMOTE_RESOLVE, 0x2C bytes). */
struct RemoteResolvePacket {
    u16 linkCount;
    u16 hasPrevious;
    struct ChainEntry current;
    struct ChainEntry previous;
};

/* The zone of (player, zone), with the offsets summed before the base as in the ROM. */
#define ZONE_AT(player, zone) \
    ((u8 *)gDuelZones + ((player) * sizeof(struct DuelZonesPlayer) + (zone) * sizeof(struct DuelZone)))
/* DuelZone.isDisabled (+0x91 bit 3) read as a masked byte. */
static inline u8 IsZoneDisabled(u8 *zone) { return zone[0x91] & 8; }

/* gCardStats through its constant address (the ROM reloads it per use). */
#define CARD_STATS_C(id)    (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
/* gCardIdToNumber through its constant address, and through the symbol. */
#define CARD_NUMBER_C(id)   (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)     (gCardIdToNumber[(id) & CARD_ID_MASK])

/* Magic/Trap subtype of a stats word (enum SpellSubtype), 0 for other card types. */
static inline u16 GetSpellSubtype(u32 stats, int type)
{
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/* Chain_Resolve stages (gChain.resolveStep). */
enum ChainResolveStage {
    RESOLVE_STAGE_SEND_CHAIN = 0,       /* link duel: send the chain to the partner */
    RESOLVE_STAGE_SHOW_LIST = 1,        /* 'Chain : Resolving' list */
    RESOLVE_STAGE_WAIT_LIST = 2,
    RESOLVE_STAGE_OPEN_SCREEN = 3,      /* link duel: wait for the partner's list; reopen the duel screen */
    RESOLVE_STAGE_CHECK_NEGATION = 4,   /* last link: look up the effect, apply the negation rules */
    RESOLVE_STAGE_SEND_REMOTE = 5,      /* a partner's link: let the partner resolve it */
    RESOLVE_STAGE_RUN_EFFECT = 6,       /* run the resolve handler until it returns 0 */
    RESOLVE_STAGE_NEXT_LINK = 100       /* drop the resolved link */
};
#define RESOLVE_NEXT_STAGE()    (gChain.resolveStep++)
#define RESOLVE_LINK_DONE()     (gChain.resolveStep = RESOLVE_STAGE_NEXT_LINK)

/*
 * Resolves the chain, one stage per call while gChain.resolving is set (started by Chain_Build), from the last
 * link back. For each link: find its gCardEffects row; a Magic/Trap is negated if its zone is disabled or a
 * negation flag covers it (Jinzo/Royal Decree, Imperial Order, the per-subtype flags); a negated monster with
 * a flip effect is skipped. Then the resolve handler runs as a step machine (gChain.effectStep, from 0x80)
 * until it returns 0; a partner's link is resolved on the partner's GBA. A card that leaves the field after
 * resolving (Chain_CardGoesToGrave) is cleared from its zone and its saved card word goes to the graveyard.
 * Returns 0 right after a negated card was sent away, else 1.
 */
u32 Chain_Resolve(void)
{
    u16 packet[0x80];
    struct RemoteResolvePacket pair;
    int stage = gChain.resolveStep;

    switch (stage) {
    case RESOLVE_STAGE_SEND_CHAIN:
        if (gDuelCtrl.isLinkDuel) {
            int i;
            DuelLink_SendMessage(LINKMSG_CHAIN_LIST_COUNT, gChain.linkCount, 0, 0);
            for (i = 0; i < gChain.linkCount; i++) {
                packet[0] = i;
                MemCopy16(packet + 1, &gChain.links[i], sizeof(struct ChainEntry));
                DuelLink_SendMessageData(LINKMSG_CHAIN_LIST_ENTRY, packet, 2 + sizeof(struct ChainEntry));
            }
            DuelLink_SendMessage(LINKMSG_SHOW_CHAIN_LIST_P1, gChain.linkCount, 0, 0);
            gLinkState.chainListShown = 0;
        }
        RESOLVE_NEXT_STAGE();
        return 1;
    case RESOLVE_STAGE_SHOW_LIST:
        ChainListScreen_Start((u32)gChain.links, 1);
        RESOLVE_NEXT_STAGE();
        return 1;
    case RESOLVE_STAGE_WAIT_LIST:
        if (ChainListScreen_Run())
            RESOLVE_NEXT_STAGE();
        return 1;
    case RESOLVE_STAGE_OPEN_SCREEN:
        if (gDuelCtrl.isLinkDuel && !gLinkState.chainListShown)
            return 1;
        DuelCmd_Push(DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        RESOLVE_NEXT_STAGE();
        return 1;
    case RESOLVE_STAGE_CHECK_NEGATION: {
        u32 stats, type;
        int disabled, subtype;

        gChain.effectIndex = FindCardEffect(LAST_LINK.card);
        if (gChain.effectIndex < 0) {
            RESOLVE_LINK_DONE();
            return 1;
        }
        /* (struct CardEffect declares the handler int-returning, struct ChainState u32-returning.) */
        gChain.resolve = (ChainResolveFunc)gCardEffects[gChain.effectIndex].resolve;
        if (!gChain.resolve) {
            RESOLVE_LINK_DONE();
            return 1;
        }
        stats = CARD_STATS_C(LAST_LINK.card);
        type = CARD_STATS_TYPE(stats);
        if (type > CARD_TYPE_REPTILE) {
            /* Magic or Trap: negated by a disabled zone (not for a card played from off the field) ... */
            disabled = 0;
            if (LAST_LINK.kind != CHAIN_KIND_OFF_FIELD)
                disabled = IsZoneDisabled(ZONE_AT(LAST_LINK.player, LAST_LINK.zone)) != 0;
            /* ... by the per-subtype negation of Field and Equip Magic ... */
            subtype = GetSpellSubtype(stats, type);
            if (subtype != SPELL_FIELD) {
                if (subtype == SPELL_EQUIP && (gDuel.equipMagicNegated || gDuel.equipMagicNegatedThisTurn))
                    disabled = 1;
            } else if (gDuel.fieldMagicNegatedThisTurn) {
                disabled = 1;
            }
            /* ... by Jinzo / Royal Decree (Traps), Imperial Order (Magic), and the Continuous negations. */
            stats = CARD_STATS_C(LAST_LINK.card);
            switch (CARD_STATS_TYPE(stats)) {
            case CARD_TYPE_TRAP:
                if (gDuel.trapsNegated)
                    disabled = 1;
                if (CARD_STATS_SUBTYPE(stats) == SPELL_CONTINUOUS && gDuel.contTrapNegatedThisTurn)
                    disabled = 1;
                break;
            case CARD_TYPE_MAGIC:
                if (gDuel.magicNegated)
                    disabled = 1;
                if (CARD_STATS_SUBTYPE(stats) == SPELL_CONTINUOUS && gDuel.contMagicNegatedThisTurn)
                    disabled = 1;
                break;
            }
            /* Key 1539 is never negated. */
            if (CARD_NUMBER_C(LAST_LINK.card) == CARD_1539) {
                disabled = 0;
                LAST_LINK.negated = 0;
            }
            if (disabled) {
                {
                    u16 cmd = LAST_LINK.player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_SPELL_TRAP_DISABLED
                                               : DUEL_CMD_SET_SPELL_TRAP_DISABLED;
                    u16 zone = LAST_LINK.zone;
                    DuelCmd_Push(cmd, zone, 1, 0);
                }
                LAST_LINK.negated = 1;
            }
        } else {
            /* A monster: a negated flip effect does not resolve. */
            if ((HasFlipEffect(CARD_NUMBER_C(LAST_LINK.card), 1)
                 || HasFlipEffect(CARD_NUMBER_C(LAST_LINK.card), 0))
                && LAST_LINK.negated) {
                RESOLVE_LINK_DONE();
                return 1;
            }
        }
        /* Save the card word of the link's zone before the zone may be cleared. */
        {
            u32 *dest = (u32 *)&gChain.savedCard;
            u8 *playerZones = (u8 *)gDuelZones + LAST_LINK.player * sizeof(struct DuelZonesPlayer);
            CopyDuelCard(dest, (u32 *)(playerZones + LAST_LINK.zone * sizeof(struct DuelZone)));
        }
        /* The (u16) casts on these u32 results are the ROM's narrowing. */
        if ((u16)Chain_CardGoesToGrave(&LAST_LINK)) {
            {
                u16 cmd = LAST_LINK.player ? DUEL_CMD_PLAYER | DUEL_CMD_CLEAR_ZONE_CARD : DUEL_CMD_CLEAR_ZONE_CARD;
                u16 zone = LAST_LINK.zone;
                DuelCmd_Push(cmd, zone, 0, 0);
            }
        }
        if (LAST_LINK.destroyIfNegated && LAST_LINK.negated) {
            /* The negated card goes away now; its effect does not resolve. */
            if (CARD_NUMBER_C(CARD_WORD_ID(gChainWords.savedCard)) == CARD_SWORD_OF_DEEP_SEATED) {
                /* Sword of Deep-Seated returns to the top of its owner's deck. */
                LoseLpOnSendToGraveyard(CARD_WORD_OWNER(gChainWords.savedCard), 1);
                ShowCardEffect(0, CARD_WORD_ID(gChainWords.savedCard));
                {
                    /* the owner bit, tested as the sign of word << 19 */
                    u16 cmd = (s32)(gChainWords.savedCard << 19) < 0
                                  ? DUEL_CMD_PLAYER | DUEL_CMD_ADD_CARD_TO_DECK_TOP
                                  : DUEL_CMD_ADD_CARD_TO_DECK_TOP;
                    u16 cardLow = gChainWords.savedCard;
                    u16 cardHigh = gChainWords.savedCard >> 16;
                    DuelCmd_Push(cmd, cardLow, cardHigh, 0);
                }
            } else {
                {
                    u16 cmd = LAST_LINK.player ? DUEL_CMD_PLAYER | DUEL_CMD_ADD_CARD_TO_GRAVEYARD
                                               : DUEL_CMD_ADD_CARD_TO_GRAVEYARD;
                    u16 cardLow = gChainWords.savedCard;
                    u16 cardHigh = gChainWords.savedCard >> 16;
                    DuelCmd_Push(cmd, cardLow, cardHigh, 0);
                }
                /* Umi leaving the field: destroy both players' key-1423 cards. */
                if (CARD_NUMBER_C(CARD_WORD_ID(gChainWords.savedCard)) == CARD_UMI) {
                    DestroyFaceUpCardsByNumber(LAST_LINK.player, CARD_1423);
                    DestroyFaceUpCardsByNumber(1 - LAST_LINK.player, CARD_1423);
                }
                LoseLpOnSendToGraveyard(CARD_WORD_OWNER(gChainWords.savedCard), 1);
            }
            RESOLVE_LINK_DONE();
            return 0;
        } else {
            gChain.effectStep = EFFECT_STEP_START;
            gChain.effectSubStep = 0;
            RESOLVE_NEXT_STAGE();
            return 1;
        }
    }
    case RESOLVE_STAGE_SEND_REMOTE:
        if ((u16)Chain_IsPartnerEntry(&LAST_LINK)) {
            pair.linkCount = gChain.linkCount;
            if (gChain.linkCount > 1) {
                pair.hasPrevious = 1;
                MemCopy16(&pair.current, &LAST_LINK, sizeof(struct ChainEntry));
                MemCopy16(&pair.previous, &PREV_LINK, sizeof(struct ChainEntry));
            } else {
                pair.hasPrevious = 0;
                MemCopy16(&pair.current, &LAST_LINK, sizeof(struct ChainEntry));
            }
            DuelLink_SendMessageData(LINKMSG_REMOTE_RESOLVE, &pair, sizeof(pair));
        }
        gLinkState.remoteResolveDone = 0;
        RESOLVE_NEXT_STAGE();
        return 1;
    case RESOLVE_STAGE_RUN_EFFECT:
        if (!(u16)Chain_IsPartnerEntry(&LAST_LINK)) {
            if (gChain.linkCount > 1)
                gChain.effectStep = gChain.resolve(&LAST_LINK, &PREV_LINK);
            else
                gChain.effectStep = gChain.resolve(&LAST_LINK, 0);
            if (!gChain.effectStep)
                gLinkState.remoteResolveDone = 1;
        }
        if (gLinkState.remoteResolveDone) {    /* set here or by the partner (LINKMSG_REMOTE_RESOLVE_DONE) */
            if ((u16)Chain_CardGoesToGrave(&LAST_LINK)) {
                u32 *saved = (u32 *)&gChain.savedCard;
                u16 cmd = LAST_LINK.player ? DUEL_CMD_PLAYER | DUEL_CMD_ADD_CARD_TO_GRAVEYARD
                                           : DUEL_CMD_ADD_CARD_TO_GRAVEYARD;
                DuelCmd_Push(cmd, ((u16 *)saved)[0], ((u16 *)saved)[1], 0);    /* the card word in two halves */
                LoseLpOnSendToGraveyard(CARD_WORD_OWNER(gChainWords.savedCard), 1);
            }
            RESOLVE_LINK_DONE();
        }
        return 1;
    case RESOLVE_STAGE_NEXT_LINK:
        if (--gChain.linkCount) {
            gChain.resolveStep = RESOLVE_STAGE_SEND_CHAIN;
            return 1;
        }
        /* fall through: the chain is resolved */
    default:
        gChain.resolving = 0;
        DuelCursor_Select(gDuelScreen.selPlayer, gDuelScreen.selArea, gDuelScreen.selIndex);
        return 1;
    }
    return 1;
}

/*
 * Per-frame chain driver: returns Chain_Build() while gChain.building is set and Chain_Resolve() while
 * gChain.resolving is set. Otherwise, if activations are pending, they become the links of a new chain: key
 * 1242 links beyond the number of key-1242 cards in that player's graveyard are dropped, and Chain_Build
 * starts if any link is left. Returns 1 then, 0 when there is nothing to do.
 */
u16 Chain_Update(void)
{
    int i, j;
    int graveCount[2];

    if (gChain.building)
        return Chain_Build();
    if (gChain.resolving)
        return Chain_Resolve();
    if (gChain.pendingCount != 0) {
        gChain.linkCount = 0;
        for (i = 0; i < gChain.pendingCount; i++) {
            MemCopy16(&gChain.links[i], &gChain.pending[i], sizeof(struct ChainEntry));
            gChain.linkCount++;
        }
        graveCount[0] = CountGraveyardCardsByNumber(0, CARD_1242);
        graveCount[1] = CountGraveyardCardsByNumber(1, CARD_1242);
        for (i = 0; i < gChain.linkCount; i++) {
            u16 number = CARD_NUMBER(gChain.links[i].card);
            if (number == CARD_1242) {
                if (graveCount[gChain.links[i].player] > 0) {
                    graveCount[gChain.links[i].player]--;
                } else {
                    gChain.linkCount--;
                    for (j = i; j < gChain.linkCount; j++)
                        MemCopy16(&gChain.links[j], &gChain.links[j + 1], sizeof(struct ChainEntry));
                }
            }
        }
        gChain.pendingCount = 0;
        {
            /* building = (linkCount != 0) and buildStep = 0, stored as one byte. linkCount is u16, so the sign
             * bit of its negation is its nonzero test. */
            u32 negativeCount = -(u32)gChain.linkCount;
            u8 *buildByte = &gChainWords.buildFlags;
            *buildByte = negativeCount >> 31;
        }
        gChain.buildIndex = 0;
        return 1;
    }
    return 0;
}

/* 1 if the player's hand holds all five pieces of Exodia the Forbidden One. */
u16 HasExodiaInHand(int player)
{
    if (CountHandCardsByNumber(player, CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE)
        && CountHandCardsByNumber(player, CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE)
        && CountHandCardsByNumber(player, CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE)
        && CountHandCardsByNumber(player, CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE)
        && CountHandCardsByNumber(player, CARD_EXODIA_THE_FORBIDDEN_ONE))
        return 1;
    return 0;
}

/* 1 if keys 1528 and 1541-1544 are all active on the player's field (hypothesis: Destiny Board and its four
 * letters; these keys are not EDS cards, so this never happens). */
u16 HasDestinyBoardComplete(int player)
{
    if (CountActiveCardsOnField(player, CARD_1528) && CountActiveCardsOnField(player, CARD_1541)
        && CountActiveCardsOnField(player, CARD_1542) && CountActiveCardsOnField(player, CARD_1543)
        && CountActiveCardsOnField(player, CARD_1544))
        return 1;
    return 0;
}

/*
 * Decides the duel during the turn steps (DUEL_STEP_TURN_START .. DUEL_STEP_OPPONENT_TURN), except on the
 * partner's turn of a link duel. In order: a player at 0 LP (the higher LP wins, equal LP is a draw), deck
 * out (the other player wins), Exodia in the hand, the Destiny Board. Sets gDuel.result (enum DuelResult,
 * player 0's view; a draw when both players meet the same condition) and gDuel.duelOver; returns 1 when the
 * duel is over.
 */
u16 Duel_CheckWin(void)
{
    struct DuelPlayer *p0;

    if ((u8)(gDuelCtrl.phase - DUEL_STEP_TURN_START) > DUEL_STEP_OPPONENT_TURN - DUEL_STEP_TURN_START)
        return 0;
    if (gDuelCtrl.isLinkDuel && gDuel.turnPlayer)
        return 0;
    gDuel.result = DUEL_RESULT_DRAW;
    p0 = gDuel.players;
    if (gDuel.players[0].lifePoints == 0 || gDuel.players[1].lifePoints == 0) {
        if (gDuel.players[0].lifePoints > gDuel.players[1].lifePoints)
            gDuel.result = DUEL_RESULT_WIN;
        if (gDuel.players[0].lifePoints < gDuel.players[1].lifePoints)
            gDuel.result = DUEL_RESULT_LOSE;
        gDuel.duelOver = 1;
        return 1;
    }
    if (p0->deckOut || gDuel.players[1].deckOut) {
        if (!p0->deckOut)
            gDuel.result = DUEL_RESULT_WIN;
        if (!gDuel.players[1].deckOut)
            gDuel.result = DUEL_RESULT_LOSE;
        gDuel.duelOver = 1;
        return 1;
    }
    p0->exodiaWin = HasExodiaInHand(0);
    gDuel.players[1].exodiaWin = HasExodiaInHand(1);
    if (p0->exodiaWin || gDuel.players[1].exodiaWin) {
        if (!gDuel.players[1].exodiaWin)
            gDuel.result = DUEL_RESULT_WIN;
        if (!p0->exodiaWin)
            gDuel.result = DUEL_RESULT_LOSE;
        gDuel.duelOver = 1;
        return 1;
    }
    p0->destinyBoardWin = HasDestinyBoardComplete(0);
    gDuel.players[1].destinyBoardWin = HasDestinyBoardComplete(1);
    if (p0->destinyBoardWin || gDuel.players[1].destinyBoardWin) {
        if (!gDuel.players[1].destinyBoardWin)
            gDuel.result = DUEL_RESULT_WIN;
        if (!p0->destinyBoardWin)
            gDuel.result = DUEL_RESULT_LOSE;
        gDuel.duelOver = 1;
        return 1;
    }
    return 0;
}

/* Duel step 0: the first player takes the first turn. In a link duel where the partner starts, count the
 * turn, tell the partner we are ready and go to the opponent's turn. Returns 1 to go on to the opening. */
u32 DuelPhase_Init(void)
{
    gDuel.turnPlayer = gMainResultView.firstPlayer;
    if ((gDuelCtrl.isLinkDuel) && gDuel.turnPlayer) {
        gDuel.turnCount++;
        DuelLink_SendMessage(LINKMSG_READY, 0, 0, 0);
        gDuelCtrl.phase = DUEL_STEP_OPPONENT_TURN;
        return 0;
    }
    return 1;
}

/*
 * Duel step 9, the end of the duel. Step 0: stop the BGM, queue the Exodia or Destiny Board win scene if a
 * player won that way, then the result banner (DUEL_CMD_SHOW_DUEL_RESULT: 0 win, 1 lose, 2 draw). Step 1:
 * wait for the command queue; a link duel sends the result to the partner and waits 20 more frames (step 2).
 * Returns 1 when done (DuelMainStep then ends the duel).
 */
u32 DuelPhase_ShowResult(void)
{
    switch (gDuel.phaseStep) {
    case 0:
        StopBGM();
        gDuel.unk1B13_0 = 0;
        if ((gDuel.players[0].exodiaWin) || (gDuel.players[1].exodiaWin)) {
            DuelCmd_Push(DUEL_CMD_CLOSE_DUEL_SCREEN, 0, 0, 0);
            DuelCmd_Push(DUEL_CMD_EXODIA_WIN_SCENE, 0, 0, 0);
            DuelCmd_Push(DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        }
        if ((gDuelPlayers[0].destinyBoardWin) || (gDuelPlayers[1].destinyBoardWin)) {
            DuelCmd_Push(DUEL_CMD_CLOSE_DUEL_SCREEN, 0, 0, 0);
            DuelCmd_Push(DUEL_CMD_DESTINY_BOARD_WIN_SCENE, 0, 0, 0);
            DuelCmd_Push(DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        }
        switch (gDuel.result) {
        case DUEL_RESULT_WIN:
            DuelCmd_Push(DUEL_CMD_SHOW_DUEL_RESULT, 0, 0, 0);
            break;
        case DUEL_RESULT_LOSE:
            DuelCmd_Push(DUEL_CMD_SHOW_DUEL_RESULT, 1, 0, 0);
            break;
        case DUEL_RESULT_DRAW:
            DuelCmd_Push(DUEL_CMD_SHOW_DUEL_RESULT, 2, 0, 0);
            break;
        }
        gDuel.phaseStep++;
        return 0;
    case 1:
        if (gDuelCmd.queueCount != 0)
            return 0;
        if (!gDuelCtrl.isLinkDuel)
            break;
        DuelLink_SendMessage(LINKMSG_DUEL_RESULT, 1 - gDuel.result, 0, 0);
        gDuel.phaseCounter = 0;
        gDuel.phaseStep++;
        return 0;
    case 2:
        if (++gDuel.phaseCounter <= 19)
            return 0;
        break;
    }
    return 1;
}

/*
 * The duel loop, once per frame (Campaign and Link Battle). The duel screen, the partner's command (link
 * duel), the command queue and the card list viewer run first; while one of them is busy nothing else
 * advances. Then the text box sprites are drawn, and the text box, the duel prompt, the pending summon, the
 * chain and the event responses get their turn. When all are idle, the handler of the current duel step
 * (gDuelPhaseTable[gDuelCtrl.phase]) runs; Duel_CheckWin forces DUEL_STEP_RESULT, and a handler that returns
 * 1 moves to the next step. Returns 1 when the duel is over (after the result was saved), else 0.
 */
u32 DuelMainStep(void)
{
    u16 busy;
    u16 done;

    if (gDuelPhaseTable[gDuelCtrl.phase] != NULL) {
        busy = DuelScreen_Update();
        if (busy == 0) {
            if (gDuelCtrl.isLinkDuel && gLinkState.remoteCmdPending)
                busy = DuelCmd_RunRemote();
            if (busy == 0) {
                busy = DuelCmdQueue_Run();
                if (busy == 0)
                    busy = CardListView_Run();
            }
        }
        /* Text box sprites, while the duel screen is up and the box has not started closing. */
        if (gDuelScreen.uiGfxLoaded && gDuelScreen.active && gTextBox.active
            && gTextBox.step <= TEXTBOX_STEP_SLIDE_OUT) {
            if (gTextBox.drawCallback)
                gTextBox.drawCallback();
            else
                TextBoxDrawSprites();
        }
        if (busy == 0 && !TextBoxUpdate16() && !DuelPrompt_Run16() && !SummonAction_Update() && !Chain_Update()) {
            UpdateSpellTrapNegation1(1);
            if (!EventResponse_Update16()) {
                switch (gDuelCtrl.phase) {
                case DUEL_STEP_DRAW_PHASE:
                case DUEL_STEP_STANDBY_PHASE:
                case DUEL_STEP_MAIN_PHASE:
                case DUEL_STEP_END_PHASE:
                    /* Link interrupt: while it is active ("Just a moment") the turn waits. */
                    if (gDuel.interruptActive) {
                        DrawLinkWaitIndicator();
                        goto end;
                    }
                    /* The partner asked to interrupt: accept unless the card menu is busy. */
                    if (gLinkState.interruptRequested) {
                        int ok = 1;
                        if (gDuelCardMenuU16.open) {
                            if (gDuelCardMenuU16.state == 2) {
                                gDuelCardMenuU16.open = 0;
                                gDuelCardMenuU16.state = 0;
                            } else {
                                ok = 0;
                            }
                        }
                        if (gDuelCardMenuU16.confirmed)
                            ok = 0;
                        if (ok) {
                            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_SHOW_JUST_A_MOMENT, 1, 0, 0);
                            DuelLink_SendMessage(LINKMSG_INTERRUPT_BEGIN, 0, 0, 0);
                            gDuel.interruptActive = 1;
                        }
                        DrawAllAreaTiles();
                        LinkWaitStart_Nop();
                    }
                }
                done = gDuelPhaseTable[gDuelCtrl.phase]();
                if (Duel_CheckWin()) {
                    gDuelCtrl.phase = DUEL_STEP_RESULT;
                    gDuel.phaseStep = 0;
                    gDuel.phaseCounter = 0;
                } else if (done) {
                    gDuelCtrl.phase++;
                    gDuel.phaseStep = 0;
                    gDuel.phaseCounter = 0;
                }
            }
        }
    end:
        DrawFieldOverlay();
        DrawHandCards();
        return 0;
    }
    gMainResultView.result = gDuel.result;
    SaveGame();
    return 1;
}
