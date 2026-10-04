/*
 * effect_targets4 (0x08040EBC-0x08041F9B): card effect target selection, part 4
 * (wiki/functions/effect-targets4-c.md; parts 1-3 are effect_targets1.c, effect_targets2.c and effect_targets3.c).
 *
 * Thirteen ChainB handlers of gCardEffects (struct CardEffect, include/effect.h) and CanActivateFieldCard.
 * All thirteen belong to effect keys 1318-1546 that no EDS card uses (tribute and banish costs, attack
 * redirection, Ground Collapse- and Mask of Dispel-like effects): they are shared engine code that no EDS
 * duel reaches. They follow the contract of effect_targets3: Chain_Build calls a link's ChainB handler every
 * frame until it returns 1; then link->targets[0..numTargets-1] holds the chosen targets, and 0 means "call me
 * again next frame". The steps are counted in gChain.targetStep, which Chain_Build clears first: step 0 opens
 * the prompt text box and clears numTargets; the next step waits for the field cursor, which only stops on
 * positions that match a FieldPickMask (DuelCursor_PickTarget returns 1 when A is pressed; the position is
 * gDuelScreen.selPlayer and selArea + selIndex). A refused pick plays SE_ERROR. In most handlers B (when the
 * handler checks it) goes back to step 0.
 *
 * CanActivateFieldCard is the "can this set Magic/Trap be activated now?" test of the response window
 * (EventResponse_Run, duel_response.c) and of the CPU.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_*, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, DuelPhase, DuelPromptKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* DUEL_CMD_POINT_AT_CARD, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CONFIRM, SE_ERROR */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_cmd.h, card_list_view.h and duel_screen.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with the include lines of duel.h and sound.h, in that order
 * (build/readability/issues/effect_targets4.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x90 - 0x7];
    u32 unk90_0:10;
    u32 canActivate:1;              /* +0x91 bit 2: a set card that may be activated */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:20;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 unk2[0x7 - 0x2];
    u8 unk7_0:6;
    u8 magicTrapLockTurns:2;        /* +0x007 bits 6-7: nonzero blocks Magic/Trap activation */
    u8 unk8[0x28 - 0x8];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 unk684[0xD64 - 0x684];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B64 - 0x1ACC];
    u16 promptResult;               /* +0x1B64: the answer of the last duel prompt */
    u8 unk1B66[0x1B78 - 0x1B66];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

int CountGraveyardMonsters(int player);
int CountActiveCardsOnField(int player, u16 cardNo);
u32 IsMonsterZoneFree(int player, int zone);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* TributeMonster */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget / PickAny */
#include "effect.h"                 /* AddEffectTarget, TryAddEffectTarget, GetCardSpellSpeed, CanActivateEffect */
#include "effect_handlers.h"        /* the handlers defined here and the Check handlers they call */
#include "text_box.h"               /* TextBoxOpen */
#include "util.h"                   /* FormatStr, FormatInt */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: this unit calls DuelCmd_Push through u16 parameters (the operands are narrowed at the call). */
extern void DuelCmd_PushU16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
/* Matching: EffectReturnTwoSpellTrapsChainB calls the Prepare handler like a Prepare slot, with
 * (link, chainedTo, fromHand); the definition (effect_handlers.h) takes no parameters. */
extern int EffectTwoSpellTrapsOnFieldPrepare3(struct ChainEntry *link, int prevLink, int fromHand)
    asm("EffectTwoSpellTrapsOnFieldPrepare");

/* Matching, in every B-cancel (`*step = zero; return zero;` with a local `int zero = 0`): the ROM stores and
 * returns one shared zero register; a plain `*step = 0; return 0;` compiles differently. */

/* gChain as a byte array (the same symbol). Matching: the ROM forms the address of a step byte as gChain +
 * 0x3E5 from two literals (ldr =gChain; ldr =0x3E5; adds); the member gChain.targetStep folds into one. */
extern u8 gChainBytes[] asm("gChain");
#define TARGET_STEP     OFFSET_OF(struct ChainState, targetStep)    /* 0x3E5 */
#define TARGET_WORK     OFFSET_OF(struct ChainState, targetWork)    /* 0x3E6 */

/* &gCardNumberToId[CARD_1405] (0x08623DF4 + 2 * 1405); the entry is 0 in EDS (no such card). Matching: the
 * ROM loads this element address from its own literal. */
extern const u16 gCardNumberToId_1405[];

/* Prompts (ROM; only this unit uses them; the shared ones are in effect.h). */
extern const char gStrSelectAttackTargetFmt[];                  /* 0x080849B4: key 1428, %s = card 1405 */
extern const u8 gStrDesignateMonsterYouWishToTribute[];         /* 0x080849F4: keys 1318, 1432, 1442 */
extern const u8 gStrSelectZoneToBlock[];                        /* 0x08084A30: key 1320 */
extern const u8 gStrSelectAnotherZoneToBlock[];                 /* 0x08084A68: key 1320 */
extern const u8 gStrDesignateOpponentMonsterToFlip[];           /* 0x08084AA8: key 1337 */
extern const u8 gStrDesignateMagicForMask[];                    /* 0x08084AF8: key 1417 */
extern const u8 gStrDesignateAnotherMonsterToTribute[];         /* 0x08084B38: keys 1432, 1442 */
extern const u8 gStrDesignateFirstCardToReturn[];               /* 0x08084B6C: key 1521 */
extern const u8 gStrDesignateSecondCardToReturn[];              /* 0x08084BA0: key 1521 */
extern const u8 gStrSelectReplacementAttacker[];                /* 0x08084BD4: key 1529 */
extern const char gStrSelectGraveMonstersToBanishFmt[];         /* 0x08084C3C: key 1532, %d = count */
extern const char gStrRemainingCountFmt[];                      /* 0x08084C84: key 1532, '%d remaining.' */
extern const u8 gStrDesignateOpponentSpellTrapToReturn[];       /* 0x08084C98: key 1539 */
extern const u8 gStrSelectTrapToForceActivate[];                /* 0x08084CEC: key 1545 */
extern const u8 gStrDesignateFusionToReturnToDeck[];            /* 0x08084D20: key 1546 */

/* Text box of the target prompts: cell (6, 2), 18 x 7 cells. */
#define TARGET_PROMPT_POS 0x206
#define TARGET_PROMPT_SIZE 0x712

/* Pick masks: the same positions on both sides; a face-up monster in either position (0xE0); any monster
 * (PICK_ANY_MONSTER, 0xF0); any Magic or Trap card, face up or set (0xE). */
#define PICK_BOTH_SIDES(mask) ((mask) | PICK_PLAYER1(mask))
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)
#define PICK_ANY_SPELL_TRAP (PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP)

/* &gDuelZones[player].zones[zone] by byte arithmetic; player must be 0 or 1. Matching: the two forms add
 * the zone and player terms in the orders the ROM uses (ZONE_AT: zone term first, ZONE_AT_PZ: player term
 * first; array indexing gives yet another order). */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelZonesPlayer) \
                         + (u32)gDuelZones))
#define ZONE_AT_PZ(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelZonesPlayer) + (zone) * sizeof(struct DuelZone) \
                         + (u32)gDuelZones))
/* Zone of the opponent of link's player. */
#define OPPONENT_ZONE(link, zone) ZONE_AT_PZ((1 - (link)->player) & 1, zone)

/* The card word of a zone as one u32 (ldr), and its card ID: 12 bits (lsl #20; lsr #20). Matching: a bitfield
 * read of .id loads a halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
/* 1 if a card word holds a card (ID != 0): one lsl #20 and a compare with 0. */
#define HAS_CARD(card) ((CARD_WORD(card) << 20) != 0)

/* Matching: CanActivateFieldCard tests DuelZone.canActivate and isDisabled as bits of the byte at +0x91
 * (ldrb; movs #4 / #8; ands); duel.h puts canActivate in a u32 container, which compiles to a shift and a sign
 * test. */
struct DuelZoneBits91 {
    u8 unk0[0x91];
    u8 unk91_0:2;
    u8 canActivate:1;               /* +0x91 bit 2: a set card that may be activated */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u8 unk91_4:4;
};

/* ChainEntry.player read as the raw byte (ldrb; and #1). Matching: a bitfield read gives lsl/lsr. */
#define ENTRY_PLAYER_BYTE(entry) (1 & ((u8 *)(entry))[2])

/* A target position, player | zone << 8, as the ROM builds it: byte-narrowed operands, player term first
 * (DUEL_LOC puts the zone first, which changes the registers). */
#define POSITION(player, zone) ((u8)(player) | (u8)(zone) << 8)
/* The same packing of (area, index) in the arg4 of DUEL_CMD_POINT_AT_CARD. */
#define AREA_INDEX(area, index) ((u8)(area) | (u8)(index) << 8)

/* Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the symbol forms (card_data.h) load other literals. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))       /* enum CardType */

/* Level of a card (stars) as a switch statement: Magic, Trap and Ticket cards have none, a Divine card counts
 * as level 10, a monster uses its stats. */
#define CARD_LEVEL(id, level)                                   \
    switch ((int)CARD_TYPE(id)) {                               \
    case CARD_TYPE_TRAP:                                        \
    case CARD_TYPE_MAGIC:                                       \
    case CARD_TYPE_TICKET:                                      \
        level = 0;                                              \
        break;                                                  \
    case CARD_TYPE_DIVINE:                                      \
        level = 10;                                             \
        break;                                                  \
    default:                                                    \
        level = CARD_STATS_LEVEL(CARD_STATS(id));               \
        break;                                                  \
    }

/*
 * ChainB of key 1428 (not an EDS card): redirect the attack to the player's face-up copy of card 1405. Step
 * 0: the prompt 'Select the %s that you wish to designate as an attack target.' (the name lookup
 * gCardNumberToId[1405] gives card 0 in EDS) and numTargets = 0. Then a pick among the player's face-up
 * monsters: it must hold card number 1405, and is added without the targetability test
 * (AddEffectTargetUnchecked); anything else plays SE_ERROR. No B-cancel.
 */
int EffectRedirectAttackChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    char prompt[0x100];

    if (*step == 0) {
        FormatStr(prompt, gStrSelectAttackTargetFmt, (const char *)gCardNames + gCardNumberToId_1405[0] * CARD_NAME_SIZE);
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
        {
            /* link->numTargets = 0, as a read-modify-write of the byte at +0xA. FAKEMATCH: the named r1
             * byte and the empty asm use keep the ROM's scratch register (the bitfield store uses another
             * one). */
            int mask = ~7;
            register u8 fields __asm__("r1");

            fields = ((u8 *)link)[0xA];
            __asm__("" : : "r"(fields));
            ((u8 *)link)[0xA] = mask & fields;
        }
        (*step)++;
    } else if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER_ANY) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        int side = 1 & player;
        struct DuelZone *z = ZONE_AT(side, zone);
        u32 id = CARD_ID(CARD_WORD(z->card));
        if (z->isFaceUp && id != 0 && CARD_NUMBER(id) == CARD_1405) {
            AddEffectTargetUnchecked(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of key 1318: tribute one of the player's monsters and keep its level + 1.
 *   Step 0: prompt 'Designate one of your monsters that you wish to tribute.' and numTargets = 0.
 *   Step 1: B goes back to step 0. Else a pick among the player's monsters that EffectTributeForInsectCheck
 *     accepts: SE_CONFIRM, point at the card (DUEL_CMD_POINT_AT_CARD), TributeMonster, and targets[0] =
 *     level + 1 of the card; returns 1. A refused pick plays SE_ERROR.
 *   Later steps do nothing.
 * ROM bug, kept: the packed position (player | zone << 8) is passed as the zone to TributeMonster and to the
 * level lookup, so only zone 0 of player 0 works; other zones tribute nothing and read the level from unrelated
 * memory. Key 1318 has no EDS card, so this never runs.
 */
int EffectTributeForLevelChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int stepValue = *step;

    switch (stepValue) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateMonsterYouWishToTribute);
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            u16 position = POSITION(player, zone);
            if (EffectTributeForInsectCheck(link, position) != 0) {
                u16 cmd;
                int side;
                int level;
                struct DuelZone *z;
                u32 id;
                PlaySE(SE_CONFIRM);
                /* stepValue is 1 here: it doubles as the mask of the player bit. */
                cmd = (stepValue & ((u8 *)link)[2]) ? DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER
                                                    : DUEL_CMD_POINT_AT_CARD;
                DuelCmd_PushU16(cmd, gDuelScreen.selPlayer, AREA_INDEX(gDuelScreen.selArea, gDuelScreen.selIndex), 0);
                TributeMonster(player, position);
                side = player & stepValue;
                z = ZONE_AT(side, position);
                id = CARD_ID(CARD_WORD(z->card));
                CARD_LEVEL(id, level);
                AddEffectTarget(link, level + 1);
                return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 0;
    }
}

/*
 * ChainB of key 1320 (make two Monster Zones unusable; hypothesis: OCG Ground Collapse). It uses the
 * unfiltered cursor (DuelCursor_PickAny) and checks the position itself.
 *   Step 0: prompt 'Select a Monster Zone that you wish to make unplayable.' and numTargets = 0.
 *   Step 1: B goes back to step 0. Else a monster zone (selArea 0) of either player that IsMonsterZoneFree
 *     accepts is added unchecked (SE_ERROR otherwise).
 *   Step 2: prompt 'Select another Monster Zone ...'.
 *   Step 3: B goes back to step 0. Else a monster zone with no card in it and not the first pick
 *     (targets[0]); the second add finishes the handler (the default case returns 1).
 */
int EffectBlockMonsterZonesChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectZoneToBlock);
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickAny() != 0) {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selIndex;
            if (gDuelScreen.selArea == DUEL_AREA_MONSTER && IsMonsterZoneFree(player, zone) != 0) {
                AddEffectTargetUnchecked(link, player, zone);
                (*step)++;
                return 0;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectAnotherZoneToBlock);
        (*step)++;
        return 0;
    case 3:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickAny() != 0) {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selIndex;
            if (gDuelScreen.selArea == DUEL_AREA_MONSTER && !HAS_CARD(ZONE_AT_PZ(1 & player, zone)->card)
                && (u16)POSITION(player, zone) != link->targets[0]) {
                AddEffectTargetUnchecked(link, player, zone);
                (*step)++;
            } else
                PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 1;
    }
}

/*
 * ChainB of key 1337: flip an opponent's set monster face-up.
 *   Step 0: numTargets = 0; with no face-down defense card in the opponent's monster zones 0-4 (the zone's
 *     isDefense set and isFaceUp clear) it returns 1 with no target. Else prompt 'Designate one of your
 *     opponent's monster cards that you wish to flip face-up.'.
 *   Step 1: B goes back to step 0. Else a pick among the opponent's face-down defense monsters
 *     (PICK_FACE_DOWN_MONSTER | PICK_DEFENSE_POSITION on player 1); TryAddEffectTarget returns 1 when it
 *     accepts, and a refusal is silent.
 */
int EffectFlipOpponentSetMonsterChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    switch (*step) {
    case 0: {
        int i;
        int found;
        link->numTargets = 0;
        found = 0;
        for (i = 0; i <= ZONE_MONSTER_4; i++) {
            /* a face-down defense monster: isDefense set, isFaceUp clear */
            if (HAS_CARD(OPPONENT_ZONE(link, i)->card) && OPPONENT_ZONE(link, i)->isDefense
                && !OPPONENT_ZONE(link, i)->isFaceUp)
                found = 1;
        }
        if (found == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentMonsterToFlip);
        gChainBytes[TARGET_STEP]++;
        break;
    }
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_DOWN_MONSTER | PICK_DEFENSE_POSITION)) != 0) {
            if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
                return 1;
        }
        break;
    }
    return 0;
}

/*
 * ChainB of key 1417 (place a Mask on a Magic card; hypothesis: OCG Mask of Dispel). Step 0: prompt
 * 'Designate the Magic card that you wish to place the Mask on.' and numTargets = 0. Then B goes back to step
 * 0; else a pick among the face-up Magic cards of both players (PICK_FACE_UP_MAGIC): it must not be this
 * card's own position (link->player, link->zone) and TryAddEffectTarget must accept it (returns 1); else
 * SE_ERROR.
 */
int EffectFaceUpMagicTargetChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int zero = 0;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMagicForMask);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MAGIC)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (player != link->player || zone != link->zone) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of keys 1432 and 1442: pick two of the player's monsters to tribute.
 *   Step 0: prompt 'Designate one of your monsters that you wish to tribute.' and numTargets = 0.
 *   Step 1: B goes back to step 0. Else a pick among the player's monsters (PICK_ANY_MONSTER); it advances
 *     when TryAddEffectTarget accepts (a refusal is silent).
 *   Step 2: prompt 'Designate another monster that you wish to tribute.'.
 *   Step 3: B goes back to step 0. Else a pick that is not targets[0] and that TryAddEffectTarget accepts
 *     returns 1; anything else plays SE_ERROR.
 *   Later steps do nothing.
 */
int EffectTributeTwoMonstersChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateMonsterYouWishToTribute);
        link->numTargets = 0;
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) == 0)
            return 0;
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0) {
            (*step)++;
            return 0;
        }
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateAnotherMonsterToTribute);
        (*step)++;
        return 0;
    case 3:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) == 0)
            return 0;
        {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            if (POSITION(player, zone) != link->targets[0]) {
                if (TryAddEffectTarget(link, player, zone) != 0)
                    return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 0;
    }
}

/*
 * ChainB of key 1448 (an equip card that sets the monster's attribute).
 *   Step 0: the duel prompt PROMPT_SELECT_ATTRIBUTE for the player, numTargets = 0.
 *   Step 1: targets[0] = the declared attribute + 1 (gDuel.promptResult).
 *   Step 2: prompt 'Designate the monster that you wish to equip.'.
 *   Later: B goes back to step 0 (declare again). Else a pick among the face-up monsters of both players;
 *     EffectEquipTargetCheck and TryAddEffectTarget must accept it (returns 1); else SE_ERROR.
 */
int EffectDeclareAttributeEquipChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        DuelPrompt_Post(link->player, PROMPT_SELECT_ATTRIBUTE, 0, 0);
        link->numTargets = 0;
        (*step)++;
        return 0;
    case 1:
        AddEffectTarget(link, gDuel.promptResult + 1);
        (*step)++;
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToEquip);
        (*step)++;
        return 0;
    default:
        if (gMain.newKeys & B_BUTTON) {
            u8 *chain2 = gChainBytes;
            u8 *step2 = chain2 + TARGET_STEP;
            int zero = 0;

            *step2 = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            if (EffectEquipTargetCheck(link, POSITION(player, zone)) != 0) {
                if (TryAddEffectTarget(link, player, zone) != 0)
                    return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    }
}

/*
 * ChainB of key 1521 (return two Magic/Trap cards to the hand): the first pick, then a second one.
 *   Step 0: numTargets = 0; EffectTwoSpellTrapsOnFieldPrepare must still hold, else it returns 1 with no
 *     target. Prompt 'Designate the 1st card to be returned to the hand.'.
 *   Step 1: B goes back to step 0. Else a pick among any Magic or Trap card of both players; it advances
 *     when TryAddEffectTarget accepts (a refusal is silent).
 *   Step 2: prompt 'Designate the 2nd card ...'.
 *   Step 3: B goes back to step 0. Else a pick that is not targets[0] and that TryAddEffectTarget accepts
 *     returns 1; anything else plays SE_ERROR.
 *   Later steps do nothing.
 */
int EffectReturnTwoSpellTrapsChainB(struct ChainEntry *link, int prevLink)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        link->numTargets = 0;
        if (EffectTwoSpellTrapsOnFieldPrepare3(link, prevLink, 0) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateFirstCardToReturn);
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_SPELL_TRAP)) == 0)
            return 0;
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0) {
            (*step)++;
            return 0;
        }
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateSecondCardToReturn);
        (*step)++;
        return 0;
    case 3:
        if (gMain.newKeys & B_BUTTON) {
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_SPELL_TRAP)) == 0)
            return 0;
        {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            u16 position = POSITION(player, zone);
            if (link->targets[0] != position) {
                if (TryAddEffectTarget(link, player, zone) != 0)
                    return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 0;
    }
}

/*
 * ChainB of key 1529: choose which opponent monster attacks instead of the current attacker. Step 0: prompt
 * 'Select one of your opponent's monsters that you wish to have attack in place of the current attacker.' and
 * numTargets = 0. Then a pick among the opponent's face-up monsters: the zone must hold a card, must not be
 * the attacker's zone (link->loc0 >> 8) and TryAddEffectTarget must accept it (returns 1); else SE_ERROR. No
 * B-cancel.
 */
int EffectSwitchAttackerChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectReplacementAttacker);
        link->numTargets = 0;
        (*step)++;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        int side = 1 & player;
        if (HAS_CARD(ZONE_AT_PZ(side, zone)->card) && DUEL_LOC_ZONE(link->loc0) != zone) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * Card level as a u8 inline with a return per case: the QImode result pseudo is copied to its use through a
 * subreg, which reproduces the ROM's level in r0 plus a register copy (`adds r4,r0,#0` / `adds r1,r0,#0`).
 * Same rule as CARD_LEVEL: 0 for Magic, Trap and Ticket cards, 10 for a Divine card, else the stats level.
 */
static inline u8 CardLevelU8(u32 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS(id));
    }
}

/*
 * ChainB of key 1532: destroy a monster by banishing as many graveyard monsters as its level (6 steps).
 *   Step 0: prompt 'Designate a monster that you wish to destroy.' and numTargets = 0.
 *   Step 1: B goes back to step 0. Else a pick among the face-up monsters of both players; its level must be
 *     at most CountGraveyardMonsters(player) and TryAddEffectTarget must accept it: targets[1] = level,
 *     gChain.targetWork = level (banishes still to choose), next step. Else SE_ERROR.
 *   Step 2: FormatInt 'Select %d monster in the Graveyard that you wish to remove from play.'.
 *   Step 3: open the card list of the effect targets collected for card number 1532 (CardListView_Open,
 *     area -1): the player's graveyard monsters.
 *   Step 4: banish the card picked in the list (DUEL_CMD_BANISH_GRAVEYARD_CARD with its card word as two
 *     halves; the player bit marks player 1) and decrement targetWork.
 *   Step 5: while targetWork is not 0, show '%d remaining.' and go back to step 3; else return 1.
 *   Any later step returns 1.
 */
int EffectBanishGraveToDestroyChainB(struct ChainEntry *link)
{
    char countText[0x40];
    char remainingText[0x80];
    u8 *chain;
    /* Matching: reading the step before copying gChain into `chain` keeps the 0x3E5 constant in one register
     * that the ROM copies; `switch (*(chain + TARGET_STEP))` folds it into `chain` instead. */
    int stepValue = gChainBytes[TARGET_STEP];
    chain = gChainBytes;
    switch (stepValue) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToDestroy);
        gChainBytes[TARGET_STEP]++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            u8 *step = chain + TARGET_STEP;
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) == 0)
            goto waiting;
        {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            int side = 1 & player;
            struct DuelZone *z = ZONE_AT(side, zone);
            u32 id = CARD_ID(CARD_WORD(z->card));
            if (CardLevelU8(id) <= CountGraveyardMonsters(link->player)) {
                if (TryAddEffectTarget(link, player, zone) != 0) {
                    AddEffectTarget(link, CardLevelU8(id));
                    gChainBytes[TARGET_WORK] = CardLevelU8(id);
                    gChainBytes[TARGET_STEP]++;
                    return 0;
                }
            }
            PlaySE(SE_ERROR);
        }
        goto waiting;
    case 2:
        FormatInt(countText, gStrSelectGraveMonstersToBanishFmt, *(chain + TARGET_WORK));
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)countText);
        (*(chain + TARGET_STEP))++;
        return 0;
    case 3:
        CardListView_Open(link->player, -1, CARD_1532, 0);
        gChainBytes[TARGET_STEP]++;
        return 0;
    case 4: {
        u32 *card = &gCardListView.cards[gCardListView.top + gCardListView.cursorRow];
        u16 cmd = ENTRY_PLAYER_BYTE(link) ? DUEL_CMD_BANISH_GRAVEYARD_CARD | DUEL_CMD_PLAYER
                                          : DUEL_CMD_BANISH_GRAVEYARD_CARD;
        DuelCmd_PushU16(cmd, ((u16 *)card)[0], ((u16 *)card)[1], 0);
        gChainBytes[TARGET_WORK]--;
        gChainBytes[TARGET_STEP]++;
        return 0;
    }
    case 5:
        /* Matching: testing for "banishes left" (not for zero) keeps the fall-through free of a label, so
         * post-reload CSE turns the second load of the count into `adds r2,r0,#0`; and the `waiting` label
         * inside this branch makes its `return 0` merge with those of the other cases. */
        if (*(chain + TARGET_WORK) != 0) {
            FormatInt(remainingText, gStrRemainingCountFmt, *(chain + TARGET_WORK));
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)remainingText);
            *(chain + TARGET_STEP) = 3;
waiting:
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}

/*
 * ChainB of key 1539: return an opponent's Magic or Trap card to the hand. Step 0: prompt 'Designate your
 * opponent's Magic or Trap card that you wish returned to the hand.' and numTargets = 0. Then B goes back to
 * step 0; else a pick among the opponent's Magic and Trap cards (PICK_ANY_SPELL_TRAP on player 1);
 * TryAddEffectTarget returns 1 when it accepts, and a refusal is silent.
 */
int EffectReturnOpponentSpellTrapChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int zero = 0;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentSpellTrapToReturn);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_SPELL_TRAP)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of key 1545: activate a set Trap by force. Step 0: numTargets = 0 and the prompt 'Select the Trap
 * that you wish to activate by force.'. Then B goes back to step 0; else a pick among the set Magic and Trap
 * cards of both players (PICK_FACE_DOWN_SPELL_TRAP); TryAddEffectTarget returns 1 when it accepts, and a
 * refusal is silent.
 */
int EffectForceActivateTrapChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int zero = 0;
    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectTrapToForceActivate);
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of key 1546: return a Fusion Monster to the deck. Step 0: numTargets = 0 and the prompt 'Designate
 * the Fusion Monster that you wish to have returned to the Deck.'. Then B goes back to step 0; else a pick
 * among the monsters of both players (PICK_ANY_MONSTER); EffectFaceUpFusionMonsterCheck and
 * TryAddEffectTarget must accept it (returns 1); else SE_ERROR.
 */
int EffectReturnFusionToDeckChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int zero = 0;
    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateFusionToReturnToDeck);
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_MONSTER)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (EffectFaceUpFusionMonsterCheck(link, POSITION(player, zone)) != 0) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* gDuel's turn flags byte (+0x1B12): bit 1 turnPlayer, bits 2-4 phase (enum DuelPhase). CanActivateFieldCard
 * reads it twice, once through gDuelZones + 0x1AE6 and once through gDuel (the same address, 0x0201ADF2);
 * both loads and the shift extractions below are matched forms. */
#define TURN_FLAGS_OFFSET 0x1B12
#define TURN_FLAGS_VIA_ZONES (((u8 *)gDuelZones)[TURN_FLAGS_OFFSET - 0x2C])    /* gDuelZones = gDuel + 0x2C */
struct DuelTurnFlagsView {
    u8 unk0[TURN_FLAGS_OFFSET];
    u8 flags;                       /* +0x1B12 */
};
#define TURN_FLAGS_PLAYER(flags) (((flags) << 30) >> 31)
#define TURN_FLAGS_PHASE(flags) (((flags) << 27) >> 29)

/*
 * Can the Magic/Trap card in (player, zone 5-9) be activated now, outside a chain? Returns 0 unless:
 *   - the zone holds a card of type Trap or higher (not a monster);
 *   - the player is the turn player, or the card has spell speed 2 or more;
 *   - the card is face down (a face-up card only for number 1324, 1017 Ultimate Offering, 1428 and 1532);
 *   - the zone's canActivate bit is set and its isDisabled bit is clear;
 *   - a Trap card needs Jinzo (751) active on neither side of the field;
 *   - the card has spell speed 2 or more, or it is Main Phase 1 or 2 and the player is the turn player.
 * Then it fills ref (card, player, zone), refuses a Magic or Trap card while the player's magicTrapLockTurns
 * is nonzero, and returns CanActivateEffect(ref, NULL, 0) narrowed to u16 (word-valued callers rely on the
 * zero-extended halfword).
 */
int CanActivateFieldCard(struct ChainEntry *ref, int player, int zone)
{
    u32 id;
    int faceUp;
    int side = 1 & player;
    struct DuelZone *z = ZONE_AT(side, zone);
    id = CARD_ID(CARD_WORD(z->card));
    faceUp = z->isFaceUp;
    if (id == 0)
        return 0;
    if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)
        return 0;                           /* a monster */
    /* Outside the player's own turn only a card of spell speed 2 or more may be activated. */
    if (TURN_FLAGS_PLAYER((u32)TURN_FLAGS_VIA_ZONES) != player) {
        if (GetCardSpellSpeed(id) <= SPELL_SPEED_1)
            return 0;
    }
    /* A face-up card cannot be activated, except these four (Ultimate Offering and keys 1324, 1428, 1532). */
    switch (CARD_NUMBER(id)) {
    case CARD_1324:
    case CARD_ULTIMATE_OFFERING:
    case CARD_1428:
    case CARD_1532:
        faceUp = 0;
        break;
    }
    if (faceUp != 0)
        return 0;
    {
        /* The zone must allow activation and the card must not be negated. */
        int side2 = 1 & player;
        struct DuelZoneBits91 *z2 = (struct DuelZoneBits91 *)ZONE_AT(side2, zone);
        if (!z2->canActivate)
            return 0;
        if (z2->isDisabled)
            return 0;
    }
    /* Jinzo negates Trap cards while it is active on either side. */
    if (CARD_TYPE(id) == CARD_TYPE_TRAP) {
        int jinzo = CARD_JINZO;
        if (CountActiveCardsOnField(0, jinzo) != 0)
            return 0;
        if (CountActiveCardsOnField(1, jinzo) != 0)
            return 0;
    }
    /* Spell speed 2 or more always passes; speed 1 only in the turn player's Main Phase 1 or 2. */
    if (GetCardSpellSpeed(id) > SPELL_SPEED_1)
        goto fill;
    {
        struct DuelTurnFlagsView *duel = (struct DuelTurnFlagsView *)&gDuel;
        u32 flags = duel->flags;
        u32 phase = TURN_FLAGS_PHASE(flags);
        if (phase == PHASE_MAIN1 || phase == PHASE_MAIN2) {
            if (TURN_FLAGS_PLAYER(flags) == player)
                goto fill;
        }
    }
refuse:
    return 0;
fill:
    ref->card = id;
    ref->player = player & 1;
    ref->zone = zone & 0x3F;
    {
        /* FAKEMATCH: the empty asm clobbering r0 keeps the ROM's registers for player and zone without
         * emitting code. */
        __asm__("" : : : "r0");
        /* A Magic or Trap card is refused while the player's magicTrapLockTurns is nonzero. */
        switch ((int)CARD_TYPE(id & CARD_ID_MASK)) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
            if (gDuelPlayers[1 & player].magicTrapLockTurns != 0)
                goto refuse;
            break;
        }
    }
    return (u16)CanActivateEffect(ref, 0, 0);
}
