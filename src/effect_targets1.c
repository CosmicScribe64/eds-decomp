/*
 * effect_targets1 (0x0803DD7C-0x0803EDC3): card effect target selection, part 1
 * (wiki/functions/effect-targets1-c.md; part 2 is effect_targets2.c).
 *
 * The target helpers AddEffectTarget, TryAddEffectTarget and AddEffectTargetUnchecked, and the first eleven
 * ChainB handlers of gCardEffects (struct CardEffect, include/effect.h). Chain_Build calls a link's ChainB
 * handler every frame until it returns 1; then link->targets[0..numTargets-1] holds the chosen targets
 * (possibly none: the effect then does nothing), and 0 means "call me again next frame".
 *
 * A target is a board position player | zone << 8 (DUEL_LOC; zones 0-4 monsters, 5-9 spells and traps, 10
 * the Field Magic). Player 1 (the CPU) picks at once with the AI helpers. The human (player 0) gets a text
 * box prompt, then moves the field cursor, which only stops on positions that match a FieldPickMask
 * (DuelCursor_PickTarget returns 1 when A is pressed); B goes back to step 0. The human's steps are counted
 * in gChain.targetStep, which Chain_Build clears before the first call.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelZoneIndex, ChainEntryKind, ResponseEventKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* DUEL_CMD_POINT_AT_CARD, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_cmd.h, card_list_view.h and duel_screen.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" and #include "sound.h"
 * (build/readability/issues/effect_targets1.md). */
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
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 unk2[0x28 - 0x2];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 unk684[0xD64 - 0x684];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

int CountMonsters(int player);
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* AiFindStrongestMonster, AiFindWeakestMonster, AiPickEffectTribute, ... */
#include "card_list_view.h"         /* gCardListView, gCardListViewCards, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* ShowPickedCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* AddEffectTarget, CanActivateEffect, shared prompt texts */
#include "effect_handlers.h"        /* the handlers defined here and the Check/Prepare handlers they call */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: CanCardTargetZone is defined with a u16 return; this unit tests the result as an int (cmp r0, #0
 * with no lsl #16). */
extern int CanCardTargetZoneInt(u16 cardId, int player, int zone) asm("CanCardTargetZone");
/* Matching: CollectEffectTargets is defined with a u16 return and a u16 card number; this unit tests the
 * count as an int. */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: GetZoneCardAtk is defined with u32 parameters and return; EffectEquipTargetChainB compares the
 * ATK with a signed best value (blt, not bcc). */
extern int GetZoneCardAtkInt(int player, int zone) asm("GetZoneCardAtk");

/* Prompts (ROM; only this unit uses them). */
extern const u8 gStrSelectListTarget[];                     /* 0x08083C50 */
extern const u8 gStrDesignateTrapToDestroy[];               /* 0x08083C94 */
extern const u8 gStrDesignateOwnTribute[];                  /* 0x08083CC8 */
extern const u8 gStrDesignateOpponentMonsterToReturn[];     /* 0x08083CF8: Crass Clown */
extern const u8 gStrDesignateOpponentMonsterCardToDestroy[]; /* 0x08083D50: Dream Clown, Barrel Dragon */
extern const u8 gStrDesignateOpponentMonsterToAbsorb[];     /* 0x08083D90: Relinquished, key 1334 */
extern const u8 gStrDesignateMonsterForAttackPosition[];    /* 0x08083E44: Stop Defense */
extern const u8 gStrAskWhoseLpToRecover[];                  /* 0x08083E8C: menu 'Your own' / 'Your opponent's' */
extern const u8 gStrAskDestroyMonster[];                    /* 0x08083ED0: Blast Juggler (Yes/No) */
extern const u8 gStrDesignateAtk1000MonsterToDestroy[];     /* 0x08083EF4: Blast Juggler */
extern const u8 gStrSelectAnotherMonsterToDestroy[];        /* 0x08083F38: Blast Juggler */
extern const u8 gStrDesignateMagicToDestroy[];              /* 0x08083F60 */
extern const char gStrDesignateTypeMonsterToDestroyFmt[];   /* 0x08083F94: '... the %s monster ... destroy.' */
extern const char gStrDragonType[];                         /* 0x08083FC8: 'Dragon' */
extern const u8 gStrDesignateMonsterToHaveReturned[];       /* 0x08084000: Hane-Hane */

/* Text box of the target prompts: cell (6, 2), 18 x 7 cells. */
#define TARGET_PROMPT_POS 0x206
#define TARGET_PROMPT_SIZE 0x712
/* The taller box of the 'whose LP' menu: cell (5, 2), 20 x 5 cells. */
#define LP_MENU_POS 0x205
#define LP_MENU_SIZE 0x514

/* Pick masks: the same positions on both sides, and a face-up monster in either position. */
#define PICK_BOTH_SIDES(mask) ((mask) | PICK_PLAYER1(mask))
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)

/* gChain.targetStep (+0x3E5) through a byte pointer to gChain. Matching: where the ROM forms the address as
 * gChain + 0x3E5 from two literals (ldr =gChain; ldr =0x3E5; add), the code writes chain + TARGET_STEP with
 * chain = CHAIN_BYTES; the member gChain.targetStep folds into one literal. */
#define CHAIN_BYTES ((u8 *)&gChain)
#define TARGET_STEP OFFSET_OF(struct ChainState, targetStep)

/* The cursor selection words of gDuelScreen through a byte pointer (screen = (u8 *)&gDuelScreen). Matching:
 * the ROM adds the offsets to the base at run time (ldr =0x824; add; then #4 / #8); the members fold into
 * one literal each. */
#define SEL_WORD(screen, field) (*(u32 *)((screen) + OFFSET_OF(struct DuelScreen, field)))

/* &gDuelZones[player].zones[zone] by byte arithmetic. Matching: the ROM adds the zone term, then the player
 * term, then the gDuelZones literal. player must be 0 or 1. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
/* The card word of a zone as one u32 (ldr), and its card ID (lsl #20; lsr #20). Matching: a read of the
 * bitfield card.id generates other code. The CPU scans keep the ID in a u32 and narrow it to u16 for
 * CARD_TYPE, which gives the ROM's registers. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
#define ZONE_CARD_ID(zone) CARD_ID(CARD_WORD((zone)->card))

/* Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the ROM loads the 0x7FF mask before the table literal; the symbols hoist the table literal. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/* Append a target word (player | zone << 8, or half of a card word) to card->targets. No bounds check: the
 * callers add at most 3. */
void AddEffectTarget(struct ChainEntry *card, u16 target)
{
    if (card != NULL) {
        card->targets[card->numTargets] = target;
        card->numTargets++;
    }
}

/*
 * Add (player, zone) to card's targets if CanCardTargetZone allows it: the confirm sound for the human,
 * DUEL_CMD_POINT_AT_CARD (the pulsing hand pointer at the card; arg4 = area | index << 8), then
 * AddEffectTarget. Returns 1 if added, 0 if the zone may not be targeted (nothing queued).
 */
u16 TryAddEffectTarget(struct ChainEntry *card, int player, int zone)
{
    int area = DUEL_AREA_MONSTER;
    int index = zone;

    if (zone > ZONE_MONSTER_4) {
        area = DUEL_AREA_SPELL_TRAP;
        index = zone - ZONE_SPELL_0;
    }
    if (zone == ZONE_FIELD) {
        area = DUEL_AREA_FIELD;
        index = 0;
    }
    if (CanCardTargetZoneInt(card->card, player, zone) != 0) {
        if (!card->player)
            PlaySE(SE_CONFIRM);
        DuelCmd_Push(card->player ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD,
                     player, (u8)index << 8 | area, 0);
        AddEffectTarget(card, (u8)player | (u8)zone << 8);     /* DUEL_LOC, in the ROM's operand order */
        return 1;
    }
    return 0;
}

/* TryAddEffectTarget without the CanCardTargetZone test, for the effects that check the zone themselves
 * (keys 1428 and 1320 in effect_targets4.c). */
void AddEffectTargetUnchecked(struct ChainEntry *card, int player, int zone)
{
    int area = DUEL_AREA_MONSTER;
    int index = zone;

    if (zone > ZONE_MONSTER_4) {
        /* FAKEMATCH: the do-while block (a permuter result) gives the ROM's registers, area in r7 and index
         * in r4; a bare block does not. */
        do { area = DUEL_AREA_SPELL_TRAP; index = zone - ZONE_SPELL_0; } while (0);
    }
    if (zone == ZONE_FIELD) {
        area = DUEL_AREA_FIELD;
        index = 0;
    }
    if (!card->player)
        PlaySE(SE_CONFIRM);
    DuelCmd_Push(card->player ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD,
                 player, (u8)index << 8 | area, 0);
    AddEffectTarget(card, (u8)player | (u8)zone << 8);
}

/*
 * ChainB of Mask of Darkness, Magician of Faith, Monster Eye, Monster Reborn, Graverobber, Call of the
 * Haunted, Premature Burial and key 1241: the target is a card chosen in the card-list viewer, stored as the
 * two halves of its card word (targets[0] low, targets[1] high).
 *   CPU: the AI's pick from the candidates (AiPickCardListEntry), shown with ShowPickedCard; returns 1.
 *   Step 0: no target (return 1) if CollectEffectTargets finds no candidate, else the prompt.
 *   Step 1: open the viewer on the candidates (area -1).
 *   Step 2: take the card under the viewer's cursor; return 1.
 */
int EffectCardListTargetChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;
    u16 *halves;    /* one card word of gCardListView.cards as two u16 halves */

    if (link->player) {
        int i;
        link->numTargets = 0;
        i = AiPickCardListEntry(link->card);
        if (i >= 0) {
            halves = (u16 *)&gCardListViewCards[i];
            ShowPickedCard(link->player, CARD_ID(*(u32 *)halves));
            AddEffectTarget(link, halves[0]);
            AddEffectTarget(link, halves[1]);
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        link->numTargets = 0;
        if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectListTarget);
        (*step)++;
        return 0;
    case 1:
        CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
        (*step)++;
        return 0;
    default:
        ShowPickedCard(link->player, CARD_ID(gCardListView.cards[gCardListView.top + gCardListView.cursorRow]));
        halves = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];
        AddEffectTarget(link, halves[0]);
        AddEffectTarget(link, halves[1]);
        return 1;
    }
}

/*
 * ChainB of Reaper of the Cards and Trap Master: pick the Trap card to destroy.
 *   CPU: for player 0, then 1: the first face-up Trap in zones 5-9, else the first face-down card there.
 *   Human: prompt, then a pick among face-up Traps and set cards of both players.
 */
int EffectTrapTargetChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;

    if (link->player) {
        int player;
        link->numTargets = 0;
        for (player = 0; player <= 1; player++) {
            int zone;
            for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                u32 id = ZONE_CARD_ID(z);
                if (id != 0 && z->isFaceUp && CARD_TYPE((u16)id) == CARD_TYPE_TRAP) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
            for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                if (ZONE_CARD_ID(z) != 0 && !z->isFaceUp) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateTrapToDestroy);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
        return 0;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_TRAP | PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of Catapult Turtle, Cannon Soldier and The Little Swordsman of Aile: pick one of your monsters to
 * tribute. CPU: the zone AiPickEffectTribute chooses. Human: prompt, then a pick among your monsters.
 * EffectAxeOfDespairResolve also calls this directly.
 */
int EffectTributeTargetChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;

    if (link->player) {
        int noZone = -1;    /* Matching: the sentinel in a variable (one register for the call and the test) */
        int zone = AiPickEffectTribute(noZone);
        link->numTargets = 0;
        if (zone > noZone) {
            if (TryAddEffectTarget(link, 1, zone) != 0)
                return 1;
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateOwnTribute);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
        return 0;
    } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of Crass Clown, Dream Clown, Spellbinding Circle, Dark-Eyes Illusionist, Relinquished, Barrel
 * Dragon and keys 1332/1334: pick one of the opponent's monsters.
 *   CPU: the human's strongest monster (ATK + DEF), if the human has one.
 *   Step 0: no target if the opponent has no monster, else the card's prompt.
 *   Then: a pick among the opponent's monsters; a refused pick plays SE_ERROR.
 */
int EffectOpponentMonsterChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;

    if (link->player) {
        int noZone;
        link->numTargets = 0;
        if (CountMonsters(0) > 0) {
            int zone;
            noZone = -1;    /* Matching: the sentinel in a variable, as in EffectTributeTargetChainB */
            zone = AiFindStrongestMonster(0, noZone, 1, 1);
            if (zone > noZone) {
                if (TryAddEffectTarget(link, 0, zone) != 0)
                    return 1;
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        link->numTargets = 0;
        if (CountMonsters(1 - link->player) == 0)
            return 1;
        switch (CARD_NUMBER(link->card)) {
        case CARD_CRASS_CLOWN:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOpponentMonsterToReturn);
            break;
        case CARD_DREAM_CLOWN:
        case CARD_BARREL_DRAGON:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOpponentMonsterCardToDestroy);
            break;
        case CARD_RELINQUISHED:
        case CARD_1334:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOpponentMonsterToAbsorb);
            break;
        default:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOpponentMonsterTarget);
            break;
        }
        gChain.targetStep++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of every equip spell (Legendary Sword ... Megamorph, Metalmorph, Cocoon of Evolution; 49 rows):
 * pick the monster to equip. No target for the off-field trigger of an equip (kind CHAIN_KIND_OFF_FIELD,
 * the sent-to-the-graveyard link).
 *   CPU: starts on its own side, or on the opponent's for the harmful equips (Germ Infection, Paralyzing
 *        Potion, keys 1419/1548) and for Megamorph while its LP are higher (Megamorph then halves ATK).
 *        On each side it takes the monster with the highest effective ATK that EffectEquipTargetCheck
 *        accepts; once a side has a candidate it is added.
 *   Human: prompt, then a pick among face-up monsters that EffectEquipTargetCheck accepts (else SE_ERROR).
 */
int EffectEquipTargetChainB(struct ChainEntry *link)
{
    if (link->kind == CHAIN_KIND_OFF_FIELD)
        return 1;
    if (link->player) {
        int side;
        int bestAtk;
        int bestPlayer;
        int bestZone;
        int round;
        link->numTargets = 0;
        side = link->player;
        switch (CARD_NUMBER(link->card)) {
        case CARD_MEGAMORPH:
            /* Matching: the redundant & 1 masks are the ROM's. */
            if (gDuelPlayers[1 & link->player].lifePoints > gDuelPlayers[(1 - link->player) & 1].lifePoints)
                side = 1 - link->player;
            break;
        case CARD_GERM_INFECTION:
        case CARD_PARALYZING_POTION:
        case CARD_1419:
        case CARD_1548:
            side = 1 - link->player;
            break;
        }
        bestAtk = -1;
        bestPlayer = -1;
        bestZone = -1;
        for (round = 0; round <= 1; round++, side = 1 - side) {
            int zone;
            int sideByte;
            /* FAKEMATCH: the counter is set before the byte copy of side, outside the for: the ROM's order. */
            zone = 0;
            sideByte = (u8)side;
            for (; zone <= ZONE_MONSTER_4; zone++) {
                if (EffectEquipTargetCheck(link, DUEL_LOC(sideByte, (u8)zone)) != 0
                    && bestAtk < GetZoneCardAtkInt(side, zone)) {
                    bestAtk = GetZoneCardAtkInt(side, zone);
                    bestPlayer = side;
                    bestZone = zone;
                }
            }
            if (bestAtk > -1 && bestPlayer > -1 && bestZone > -1) {
                if (TryAddEffectTarget(link, bestPlayer, bestZone) != 0)
                    return 1;
            }
        }
        if (bestAtk > -1 && bestPlayer > -1 && bestZone > -1) {
            if (TryAddEffectTarget(link, bestPlayer, bestZone) != 0)
                return 1;
        }
        return 1;
    } else {
        u8 *chain = CHAIN_BYTES;
        u8 *step = chain + TARGET_STEP;
        if (*step == 0) {
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToEquip);
            link->numTargets = 0;
            (*step)++;
        } else if (gMain.newKeys & B_BUTTON) {
            *step = 0;
        } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            /* Matching: the result is tested as a u16, the type of the table's check slot. */
            if ((u16)EffectEquipTargetCheck(link, DUEL_LOC(*(u8 *)selPlayer, (u8)zone)) != 0) {
                /* FAKEMATCH: the unused test of the u16 result gives the ROM's lsl #16 after the call. */
                if (TryAddEffectTarget(link, player, zone) != 0)
                    return 1;
                return 1;
            }
            PlaySE(SE_ERROR);
        }
    }
    return 0;
}

/* ChainB of Stop Defense: prompt, then a pick among the opponent's defense-position monsters. No CPU
 * branch. */
int EffectStopDefenseChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateMonsterForAttackPosition);
        (*step)++;
        goto waiting;
    }
    if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own mov r0, #0; strb tail (a plain
         * `*step = 0; return 0;` is merged with the shared return 0). */
        u8 zero = 0;
        *step = zero;
        return zero;
    }
    if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_DOWN_MONSTER | PICK_FACE_UP_MONSTER | PICK_DEFENSE_POSITION))
        != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
waiting:
    return 0;
}

/*
 * ChainB of the LP-recovery spells (Mooyan Curry, Goblin's Secret Remedy, Soul of the Pure, Blue Medicine):
 * whose LP to recover, as targets[0] (enum ChosenPlayer, relative to the activating player). The CPU always
 * picks itself.
 *   Step 0: the two-choice menu 'Your own' / 'Your opponent's'.
 *   Step 1: the answer becomes targets[0].
 *   Step 2: return 1.
 */
int EffectGainLpChosenPlayerChainB(struct ChainEntry *link)
{
    u8 *step;
    u8 *chain;

    if (link->player) {
        AddEffectTarget(link, CHOSEN_PLAYER_SELF);
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    switch (*step) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(LP_MENU_POS, LP_MENU_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrAskWhoseLpToRecover);
        TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
        break;
    case 1:
        AddEffectTarget(link, gTextBox.result);
        break;
    default:
        return 1;
    }
    (*step)++;
    return 0;
}

/*
 * ChainB of Blast Juggler (Tribute it in your Standby Phase to destroy up to 2 monsters with ATK 1000 or
 * less). No CPU branch. link->event is set to RESPONSE_OWN_STANDBY, so that EffectBlastJugglerPrepare checks
 * the activation as in the Standby Phase. B in a pick goes back to step 0.
 *   Step 0: no target unless EffectBlastJugglerPrepare allows it, else Yes/No 'destroy a monster?'.
 *   Step 1: No -> no target; Yes -> the pick prompt.
 *   Step 2: pick a face-up monster accepted by EffectBlastJugglerCheck; done if it is the only monster on
 *           the field. The SE_ERROR after a good pick is not heard: PlaySE plays one sound per frame and
 *           TryAddEffectTarget's SE_CONFIRM came first.
 *   Step 3: prompt for the second monster.
 *   Step 4: pick a second, different monster; done.
 */
int EffectBlastJugglerChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    int state = chain[TARGET_STEP];
    /* FAKEMATCH: a second copy of the gChain base, from which both B branches form the step pointer for
     * the shared reset (the ROM keeps it in its own register). */
    u8 *chainCopy = chain;
    u8 *step;

    switch (state) {
    case 0:
        link->numTargets = 0;
        link->event = RESPONSE_OWN_STANDBY;
        if (EffectBlastJugglerPrepare(link, 0, 0) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrAskDestroyMonster);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        gChain.targetStep++;
        return 0;
    case 1:
        if (gTextBox.result == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateAtk1000MonsterToDestroy);
        gChain.targetStep++;
        return 0;
    case 2:
        if (gMain.newKeys & B_BUTTON) {
            step = chainCopy + TARGET_STEP;
            goto restart;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            if (EffectBlastJugglerCheck(link, DUEL_LOC(*(u8 *)selPlayer, (u8)zone)) != 0
                && TryAddEffectTarget(link, player, zone) != 0) {
                int monsters = CountMonsters(0);
                monsters += CountMonsters(1);
                if (monsters == 1)
                    return 1;
                gChain.targetStep++;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    case 3:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectAnotherMonsterToDestroy);
        gChain.targetStep++;
        return 0;
    case 4:
        if (gMain.newKeys & B_BUTTON) {
            step = chainCopy + TARGET_STEP;
        restart:
            {
                /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail (see
                 * EffectStopDefenseChainB); both B branches share it through the goto. */
                u8 zero = 0;
                *step = zero;
                return zero;
            }
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            int pos = DUEL_LOC(*(u8 *)selPlayer, (u8)zone);
            if (link->targets[0] != pos && EffectBlastJugglerCheck(link, pos) != 0
                && TryAddEffectTarget(link, player, zone) != 0)
                return 1;
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 0;
    }
}

/*
 * ChainB of Armed Ninja and De-Spell: pick the Magic card to destroy (EffectTrapTargetChainB for Magic,
 * with the Field Magic zone).
 *   CPU: for player 0, then 1: the first face-up Magic in zones 5-10, else the first face-down card there.
 *   Human: no target if neither player has a Magic/Trap card on the field, else prompt and a pick among
 *          face-up Magic and set cards of both players (a refused pick plays SE_ERROR).
 */
int EffectMagicTargetChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;

    if (link->player) {
        int player;
        link->numTargets = 0;
        for (player = 0; player <= 1; player++) {
            int zone;
            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                u32 id = ZONE_CARD_ID(z);
                if (id != 0 && z->isFaceUp && CARD_TYPE((u16)id) == CARD_TYPE_MAGIC) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                if (ZONE_CARD_ID(z) != 0 && !z->isFaceUp) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        link->numTargets = 0;
        if (CountSpellTrapsFiltered(0, 0, 0, 1) == 0 && CountSpellTrapsFiltered(1, 0, 0, 1) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMagicToDestroy);
        gChain.targetStep++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
        return 0;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MAGIC | PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of Dragon Seeker: pick a face-up Dragon to destroy.
 *   Step 0 (both players): scan player 0, then 1, zones 0-4, for a position EffectDragonSeekerCheck accepts;
 *           none: no target. The CPU adds the first one and returns 1; the human gets the prompt
 *           'Designate the Dragon monster ...' (formatted into a stack buffer).
 *   Then: a pick among face-up monsters that EffectDragonSeekerCheck accepts (else SE_ERROR).
 */
int EffectDragonSeekerChainB(struct ChainEntry *link)
{
    char prompt[0x80];
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        int player;
        u8 *stepCopy;
        link->numTargets = 0;
        /* FAKEMATCH: the counters are set outside the for loops, before the copies (stepCopy, the player
         * byte): the ROM's order and registers. */
        player = 0;
        stepCopy = step;
        for (; player <= 1; player++) {
            int zone;
            int playerByte;
            zone = 0;
            playerByte = (u8)player;
            for (; zone <= ZONE_MONSTER_4; zone++) {
                if (EffectDragonSeekerCheck(link, DUEL_LOC(playerByte, (u8)zone)) != 0) {
                    if (link->player) {
                        /* FAKEMATCH: the unused test of the u16 result gives the ROM's lsl #16. */
                        if (TryAddEffectTarget(link, player, zone) != 0)
                            return 1;
                        return 1;
                    }
                    FormatStr(prompt, gStrDesignateTypeMonsterToDestroyFmt, gStrDragonType);
                    TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
                    (*stepCopy)++;
                    return 0;
                }
            }
        }
        return 1;
    } else if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail (see EffectStopDefenseChainB). */
        u8 zero = 0;
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        u8 *screen = (u8 *)&gDuelScreen;
        u32 *selPlayer = &SEL_WORD(screen, selPlayer);
        int player;
        int zone;

        zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
        /* FAKEMATCH: keeps the cursor sum in r4 before the player cursor is loaded into r5. Emits no code. */
        __asm__("" : "+r"(zone));
        player = *selPlayer;
        /* The check gets selIndex alone as the zone: the cursor is on a monster, so selArea is 0. */
        if (EffectDragonSeekerCheck(link, DUEL_LOC(*(u8 *)selPlayer, (u8)SEL_WORD(screen, selIndex))) != 0) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of Man-Eater Bug, Hane-Hane, Tribute to The Doomed and keys 1211/1301: pick any monster. prevLink
 * is the link this one answers (Chain_Build passes the pointer; defined as int) and only goes to
 * CanActivateEffect.
 *   CPU: no target unless CanActivateEffect allows it; then the human's strongest monster, else its own
 *        weakest, else the first occupied monster zone (player 0 first).
 *   Step 0: no target unless CanActivateEffect allows it (Tribute to The Doomed skips this test), else the
 *           card's prompt.
 *   Then: a pick among all monsters that CanCardTargetZone allows (else SE_ERROR).
 */
int EffectTargetableMonsterChainB(struct ChainEntry *link, int prevLink)
{
    u8 *chain;
    u8 *step;

    if (link->player) {
        int player;
        link->numTargets = 0;
        if (CanActivateEffect(link, (struct ChainEntry *)prevLink, 0) == 0)
            return 1;
        for (player = 0; player <= 1; player++) {
            if (CountMonsters(player) > 0) {
                int zone;
                if (player != 0)
                    zone = AiFindWeakestMonster(player, -1, 1, 1);
                else
                    zone = AiFindStrongestMonster(0, -1, 1, 1);
                if (zone >= 0) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
        }
        for (player = 0; player <= 1; player++) {
            int zone;
            for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                if (ZONE_CARD_ID(z) != 0) {
                    if (TryAddEffectTarget(link, player, zone) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        link->numTargets = 0;
        if (CanActivateEffect(link, (struct ChainEntry *)prevLink, 0) == 0
            && CARD_NUMBER(link->card) != CARD_TRIBUTE_TO_THE_DOOMED)
            return 1;
        switch (CARD_NUMBER(link->card)) {
        case CARD_MAN_EATER_BUG:
        case CARD_TRIBUTE_TO_THE_DOOMED:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToDestroy);
            break;
        case CARD_HANE_HANE:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToHaveReturned);
            break;
        default:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateOneMonster);
            break;
        }
        gChain.targetStep++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_MONSTER)) != 0) {
        int player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (CanCardTargetZoneInt(link->card, player, zone) != 0) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}
