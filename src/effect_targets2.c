/*
 * effect_targets2 (0x0803EDC4-0x0803FE6F): card effect target selection, part 2
 * (wiki/functions/effect-targets2-c.md; part 1 is effect_targets1.c, which describes the protocol).
 *
 * Twelve more ChainB handlers of gCardEffects (struct CardEffect, include/effect.h): Patrol Robo, Greenkappa,
 * Penguin Soldier, the take-control Magic cards (Invader of the Throne, Change of Heart, Snatch Steal), Kunai
 * with Chain, Acid Trap Hole, Bell of Destruction, Magical Hats, 7 Completed, Magic-Arm Shield, Remove Trap and
 * Two-Pronged Attack. Chain_Build calls a link's ChainB handler every frame until it returns 1; then
 * link->targets[0..numTargets-1] holds the chosen targets, and 0 means "call me again next frame".
 *
 * A target is a board position player | zone << 8 (DUEL_LOC; zones 0-4 monsters, 5-9 spells and traps).
 * Greenkappa, Penguin Soldier, the take-control cards, Bell of Destruction and Magic-Arm Shield choose for
 * the CPU (player 1) at once, by scanning the field or with the AI helpers. The human (player 0) gets a text
 * box prompt, then moves the field cursor, which only stops on positions that match a FieldPickMask
 * (DuelCursor_PickTarget returns 1 when A is pressed); B goes back to step 0. The human's steps are counted
 * in gChain.targetStep, which Chain_Build clears before the first call. The other seven handlers have no CPU
 * branch (the CPU presumably never activates those cards; not verified).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelZoneIndex, FieldPickMask, DUEL_LOC */
#include "constants/sound.h"        /* SE_ERROR */
#include "gba.h"                    /* B_BUTTON */
#include "main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h and duel_screen.h do not pull in the legacy header. After H0, replace the block
 * (BEGIN to END) with #include "duel.h" and #include "sound.h" (build/readability/issues/effect_targets2.md). */
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
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
u32 GetZoneCardAtk(u32 player, u32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* AiFindStrongestMonster, AI_FLAG_EXODIA */
#include "chain.h"                  /* struct ChainEntry, struct ChainState, gChain */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* AddEffectTarget, TryAddEffectTarget, CanActivateEffect, shared prompt texts */
#include "effect_handlers.h"        /* the handlers defined here and the Check/Prepare handlers they call */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: CanCardTargetZone is defined with a u16 return; this unit tests the result as an int (cmp r0, #0
 * with no lsl #16). */
extern int CanCardTargetZoneInt(u16 cardId, int player, int zone) asm("CanCardTargetZone");
/* Matching: EffectGreenkappaPrepare is a condition callback that ignores its arguments, so the header gives it
 * only the link. Greenkappa's ChainB passes it the second argument it received (the ROM leaves that in r1
 * from entry to the call). */
extern int EffectGreenkappaPrepare2(struct ChainEntry *card, int prevLink) asm("EffectGreenkappaPrepare");

/* Prompts (ROM; only this unit uses them) */
extern const u8 gStrDesignateOpponentFaceDownCard[];    /* 0x0808405C: Patrol Robo */
extern const u8 gStrDesignateFirstCardToDestroy[];      /* 0x080840A4: Greenkappa */
extern const u8 gStrDesignateSecondCardToDestroy[];     /* 0x080840D8: Greenkappa */
extern const u8 gStrAskReturnMonsterToHand[];           /* 0x0808410C: Penguin Soldier (Yes/No) */
extern const u8 gStrDesignateMonsterToReturnToHand[];   /* 0x0808413C: Penguin Soldier */
extern const u8 gStrAskReturnAnotherMonster[];          /* 0x08084178: Penguin Soldier (Yes/No) */
extern const u8 gStrDesignateMonsterToSwitchControl[];  /* 0x080841AC: Invader of the Throne */
extern const u8 gStrDesignateMonsterToControl[];        /* 0x08084200: Change of Heart */
extern const u8 gStrDesignateFaceUpMonsterToControl[];  /* 0x08084244: Snatch Steal, key 1514 */
extern const u8 gStrDesignateOwnMonsterToEquip[];       /* 0x08084290: Kunai with Chain */
extern const u8 gStrDesignateFaceDownDefenseMonster[];  /* 0x080842CC: Acid Trap Hole */
extern const u8 gStrDesignateOneMonsterToDestroy[];     /* 0x08084318: Bell of Destruction, key 1451 */
extern const u8 gStrDesignateOneOwnMonster[];           /* 0x08084348: Magical Hats */
extern const u8 gStrDesignateOwnMonsterToRecall[];      /* 0x08084368: key 1316 */
extern const u8 gStrDesignateOwnMonsterToBanish[];      /* 0x080843A0: key 1319 */
extern const u8 gStrAskSevenCompletedStat[];            /* 0x080843E4: 7 Completed ('Which do you wish to
                                                         * increase?', menu lines 'ATK+700' / 'DEF+700') */
extern const u8 gStrSelectNewAttackTarget[];            /* 0x08084420: Magic-Arm Shield */
extern const u8 gStrSelectFaceUpTrapToDestroy[];        /* 0x08084470: Remove Trap */
extern const u8 gStrDesignateFirstOwnMonster[];         /* 0x0808449C: Two-Pronged Attack */
extern const u8 gStrDesignateSecondOwnMonster[];        /* 0x080844C0: Two-Pronged Attack */

/* Text box of the target prompts: cell (6, 2), 18 x 7 cells. */
#define TARGET_PROMPT_POS 0x206
#define TARGET_PROMPT_SIZE 0x712
/* The box of 7 Completed's ATK/DEF menu: cell (6, 2), 19 x 6 cells. */
#define STAT_MENU_SIZE 0x613

/* Pick masks: the same positions on both sides, a face-up monster in either position, and a face-down card
 * (a face-down monster in either position, or a set Magic/Trap card). */
#define PICK_BOTH_SIDES(mask) ((mask) | PICK_PLAYER1(mask))
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)
#define PICK_FACE_DOWN_CARD (PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_DOWN_MONSTER | PICK_ATTACK_POSITION \
                             | PICK_DEFENSE_POSITION)

/* gChain.targetStep (+0x3E5) through a byte pointer to gChain. Matching: where the ROM forms the address as
 * gChain + 0x3E5 from two literals (ldr =gChain; ldr =0x3E5; add), the code writes chain + TARGET_STEP with
 * chain = CHAIN_BYTES; the member gChain.targetStep folds into one literal. */
#define CHAIN_BYTES ((u8 *)&gChain)
#define TARGET_STEP OFFSET_OF(struct ChainState, targetStep)

/* The cursor selection words of gDuelScreen through a byte pointer (screen = (u8 *)&gDuelScreen). Matching:
 * the ROM adds the offsets to the base at run time (ldr =0x824; add; then #4 / #8); the members fold into
 * one literal each. */
#define SEL_WORD(screen, field) (*(u32 *)((screen) + OFFSET_OF(struct DuelScreen, field)))

/* The player bit of a chain link read as the raw byte at +0x02 (ldrb; and #1). Matching: Greenkappa, Take
 * Control and Bell of Destruction keep the flag in a variable (isCpu) and need this form there; where it is
 * only tested (Penguin Soldier, Magic-Arm Shield) the bitfield read link->player gives the same code. */
#define LINK_PLAYER_BYTE(link) (1 & ((u8 *)(link))[2])

/* &gDuelZones[player].zones[zone] by byte arithmetic. Matching: the ROM adds the zone term, then the player
 * term, then the gDuelZones literal. player must be 0 or 1. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
/* The same with the player term first. Matching: the ROM's address order in EffectTakeControlChainB's
 * face-up test. */
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))
/* The card word of a zone as one u32 (ldr), and its card ID (lsl #20; lsr #20). Matching: a read of the
 * bitfield card.id generates other code. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
#define ZONE_CARD_ID(zone) CARD_ID(CARD_WORD((zone)->card))
/* 1 if the zone holds a card (ID != 0), tested with a single lsl #20. */
#define HAS_CARD(card) (CARD_WORD(card) << 20 != 0)
/* The card word's 11 bits that the card tables index with (CARD_ID_MASK), as lsl #21; lsr #21. */
#define CARD_ID11(word) (((word) << 21) >> 21)

/* gCardIdToNumber (0x08622AB4) through its integer-constant address. Matching: the symbol form
 * gCardIdToNumber[...] gives other code (it changes the literal pool). */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
/* The same for a card word, indexed with the shift pair (lsl #21; lsr #21) that the Exodia scan of
 * EffectPenguinSoldierChainB uses. */
#define CARD_NUMBER_OF_WORD(word) (((const u16 *)0x08622AB4)[CARD_ID11(word)])

/* Card number 1351 is no EDS card (gCardIdToNumber has no ID with it), so constants/cards.h has no name
 * for it (build/readability/issues/effect_targets2.md). */
#define UNUSED_CARD_NUMBER_1351 1351

/*
 * ChainB of Patrol Robo: pick one of the opponent's face-down cards (a monster or a set Magic/Trap). No CPU
 * branch.
 *   Step 0: prompt.
 *   Then: a pick among the opponent's face-down cards; a refused pick plays SE_ERROR; B goes back to step 0.
 */
int EffectPatrolRoboChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentFaceDownCard);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail (mov r0, #0; strb; the same
         * register is the return value). */
        u8 zero = 0;
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_DOWN_CARD)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* gChain.targetStep of the human's Greenkappa prompts. */
enum GreenkappaStep {
    GREENKAPPA_STEP_PROMPT_FIRST = 0,   /* gate on EffectGreenkappaPrepare; prompt for the 1st card */
    GREENKAPPA_STEP_PICK_FIRST = 1,
    GREENKAPPA_STEP_PROMPT_SECOND = 2,
    GREENKAPPA_STEP_PICK_SECOND = 3     /* the pick must differ from targets[0] */
};

/*
 * ChainB of Greenkappa (destroy 2 set Magic/Trap cards). prevLink is the link this one answers; it only
 * goes to EffectGreenkappaPrepare, which ignores it.
 *   CPU: the first two occupied face-down cards in zones 5-9 (player 0 first); returns 1. A card counts
 *        toward the two even if TryAddEffectTarget refuses it.
 *   Step 0: no target unless EffectGreenkappaPrepare allows it (two set Magic/Trap cards on the field),
 *           else the prompt for the 1st card.
 *   Step 1: pick a set Magic/Trap card of either side.
 *   Step 2: prompt for the 2nd card.
 *   Step 3: pick a second, different set card; done. A refused pick plays SE_ERROR; B goes back to step 0.
 */
int EffectGreenkappaChainB(struct ChainEntry *link, int prevLink)
{
    u8 *chain;
    u8 *step;
    int isCpu = LINK_PLAYER_BYTE(link);

    if (isCpu) {
        int found = 0;
        int player;
        link->numTargets = 0;
        for (player = 0; player <= 1; player++) {
            int zone;
            for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);    /* Matching: the redundant & 1 is the ROM's */
                if (HAS_CARD(z->card) && !z->isFaceUp) {
                    TryAddEffectTarget(link, player, zone);
                    found++;
                    if (found == 2)
                        return 1;
                }
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    switch (*step) {
    case GREENKAPPA_STEP_PROMPT_FIRST:
        link->numTargets = 0;
        if (EffectGreenkappaPrepare2(link, prevLink) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateFirstCardToDestroy);
        (*step)++;
        return 0;
    case GREENKAPPA_STEP_PICK_FIRST:
        if (gMain.newKeys & B_BUTTON) {
        reset:
            /* FAKEMATCH: isCpu is 0 on this path; storing it reuses the ROM's zero register (a plain 0 gives
             * another mov). */
            *step = isCpu;
            return 0;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
            if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0) {
                (*step)++;
                return 0;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    case GREENKAPPA_STEP_PROMPT_SECOND:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateSecondCardToDestroy);
        (*step)++;
        return 0;
    case GREENKAPPA_STEP_PICK_SECOND:
        if (gMain.newKeys & B_BUTTON)
            goto reset;
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            int pos = DUEL_LOC(*(u8 *)selPlayer, (u8)zone);
            if (link->targets[0] != pos && TryAddEffectTarget(link, player, zone) != 0)
                return 1;
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 0;
    }
}

/* gChain.targetStep of the human's Penguin Soldier prompts. */
enum PenguinSoldierStep {
    PENGUIN_STEP_ASK_FIRST = 0,         /* gate; Yes/No 'return a monster to your hand?' */
    PENGUIN_STEP_ANSWER_FIRST = 1,      /* No: done without a target; Yes: the pick prompt */
    PENGUIN_STEP_PICK_FIRST = 2,        /* cursor pick; done if it was the only monster on the field */
    PENGUIN_STEP_ASK_SECOND = 3,        /* Yes/No 'return another monster?' */
    PENGUIN_STEP_ANSWER_SECOND = 4,     /* shares step 1's code: No: done with one target */
    PENGUIN_STEP_PICK_SECOND = 5        /* cursor pick of a different monster; done */
};

/*
 * ChainB of Penguin Soldier (return up to 2 monsters to the hand). prevLink is the link this one answers
 * (Chain_Build passes the pointer; defined as int) and only goes to CanActivateEffect.
 *   CPU: no target unless CanActivateEffect allows it and player 0 has a monster. Two passes, each choosing
 *        one target (a position, 0xFFFF = none): with AI_FLAG_EXODIA, one of its own Exodia cards (card
 *        numbers 16-20: the four limbs and Exodia itself; presumably to get them back into its hand, a
 *        hypothesis), else the human's strongest monster (AiFindStrongestMonster; the second pass skips
 *        zone 0, not the zone of the first pick). The second pass may not repeat the first target. Stops at
 *        the first pass without a candidate.
 *   Human, 6 steps (see enum PenguinSoldierStep): Yes/No, pick, Yes/No, pick.
 */
int EffectPenguinSoldierChainB(struct ChainEntry *link, int prevLink)
{
    if (link->player) {
        int pass;
        link->numTargets = 0;
        if (CanActivateEffect(link, (struct ChainEntry *)prevLink, 0) == 0)
            return 1;
        if (CountMonsters(0) <= 0)
            return 1;
        for (pass = 0; pass <= 1; pass++) {
            /* Matching: the 'no candidate' marker in a variable; the ROM keeps it in sl for the compares. */
            int none = 0xFFFF;
            u16 candidate = 0xFFFF; /* position player | zone << 8, or none */
            if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) {
                int zone;
                for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                    /* Quirk kept from the ROM: this tests zone `pass` (0 or 1) of the CPU's side for a card, not
                     * zone `zone`; the test does not depend on the loop, so the ROM computes it before the loop. */
                    if (ZONE_CARD_ID(ZONE_AT(1, pass)) != 0) {
                        switch (CARD_NUMBER_OF_WORD(CARD_WORD(ZONE_AT(1, zone)->card))) {
                        case CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE ... CARD_EXODIA_THE_FORBIDDEN_ONE:
                            /* Matching: a range case gives the ROM's `cmp #0x14; bgt` then `cmp #0x10; blt`. */
                            if (pass != 0) {
                                if (link->targets[0] == (u16)DUEL_LOC(1, (u8)zone))
                                    continue;
                            }
                            candidate = DUEL_LOC(1, (u8)zone);
                            zone = MONSTER_ZONE_COUNT;    /* leave the scan: the first card found wins */
                        }
                    }
                }
            }
            if (candidate == none) {
                int skipZone = -1;
                int found;
                if (pass > 0)
                    skipZone = 0;
                found = AiFindStrongestMonster(0, skipZone, 1, 1);
                if (found >= 0)
                    candidate = DUEL_LOC(0, (u8)found);
            }
            if (pass > 0 && candidate == link->targets[0])
                candidate = 0xFFFF;
            if (candidate == none)
                return 1;
            TryAddEffectTarget(link, (u8)candidate, (u8)(candidate >> 8));
        }
        /* Shares step 5's `return 1` (the ROM keeps a single r0 = 1 block after case 5). */
        goto done;
    } else {
        u8 *chain = CHAIN_BYTES;
        int state = chain[TARGET_STEP];
        /* FAKEMATCH: a second copy of the gChain base, from which the shared reset forms the step pointer
         * (the ROM keeps it in its own register). */
        u8 *chainCopy = chain;
        switch (state) {
        case PENGUIN_STEP_ASK_FIRST:
            link->numTargets = 0;
            if (CanActivateEffect(link, (struct ChainEntry *)prevLink, 0) == 0)
                return 1;
            if (CountMonsters(0) + CountMonsters(1) == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrAskReturnMonsterToHand);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            gChain.targetStep++;
            return 0;
        case PENGUIN_STEP_ANSWER_FIRST:
        case PENGUIN_STEP_ANSWER_SECOND:
            if (gTextBox.result == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToReturnToHand);
            gChain.targetStep++;
            return 0;
        case PENGUIN_STEP_PICK_FIRST:
            if (gMain.newKeys & B_BUTTON)
                goto reset;
            if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_MONSTER)) == 0)
                return 0;
            if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0) {
                int monsters = CountMonsters(0);
                monsters += CountMonsters(1);
                if (monsters == 1)
                    return 1;
                gChain.targetStep++;
            }
            /* Also reached after a good pick: the ROM plays the error sound here too, which is not heard
             * (PlaySE plays one sound per frame and TryAddEffectTarget's SE_CONFIRM came first). */
            PlaySE(SE_ERROR);
            return 0;
        case PENGUIN_STEP_ASK_SECOND:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrAskReturnAnotherMonster);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            gChain.targetStep++;
            return 0;
        case PENGUIN_STEP_PICK_SECOND:
            if (gMain.newKeys & B_BUTTON) {
            reset:
                {
                    u8 *resetStep = chainCopy + TARGET_STEP;
                    /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
                    u8 zero = 0;
                    *resetStep = zero;
                    return zero;
                }
            }
            if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_MONSTER)) == 0)
                return 0;
            {
                u8 *screen = (u8 *)&gDuelScreen;
                u32 *selPlayer = &SEL_WORD(screen, selPlayer);
                int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
                int player = *selPlayer;
                int pos = DUEL_LOC(*(u8 *)selPlayer, (u8)zone);
                if (link->targets[0] == pos || TryAddEffectTarget(link, player, zone) == 0) {
                    u32 sound = SE_ERROR;
                    /* FAKEMATCH: keeps this sound call from being cross-jumped into the identical one of
                     * PENGUIN_STEP_PICK_FIRST (the ROM has both). Emits no code. */
                    asm("" : "+r"(sound));
                    PlaySE(sound);
                    return 0;
                }
            }
        done:
            return 1;
        default:
            return 0;
        }
    }
}

/*
 * ChainB of Invader of the Throne, Change of Heart, Snatch Steal and keys 1244 and 1514: pick an opponent's
 * monster to take control of. Invader and Change of Heart take any monster; Snatch Steal and the keys need
 * a face-up one.
 *   CPU: the human's strongest monster (AiFindStrongestMonster); returns 1.
 *   Step 0: no target (return 1) if the opponent has no such monster, else the card's prompt.
 *   Then: a pick; it must pass CanCardTargetZone, and Snatch Steal refuses card number 1351 (dead: no EDS card
 *         has it); Snatch Steal and key 1244 also refuse a face-down card (SE_ERROR).
 * No B handling.
 */
int EffectTakeControlChainB(struct ChainEntry *link)
{
    int one;
    int isCpu;
    u32 mask;
    u8 *chain;

    isCpu = LINK_PLAYER_BYTE(link);
    /* Matching: the constant 1 in a variable; the ROM keeps it in one register for the opponent's side
     * (one - link->player, in the first two prompts) and for AiFindStrongestMonster's useDef argument. */
    one = 1;
    if (isCpu) {
        int noZone;
        int zone;
        link->numTargets = 0;
        noZone = -1;    /* Matching: the sentinel in a variable (one register for the call and the test) */
        zone = AiFindStrongestMonster(0, noZone, 1, one);
        if (zone > noZone)
            TryAddEffectTarget(link, 0, zone);
        return 1;
    }
    chain = CHAIN_BYTES;
    if (chain[TARGET_STEP] == 0) {
        link->numTargets = 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_INVADER_OF_THE_THRONE:
            if (CountMonstersFiltered(one - link->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToSwitchControl);
            break;
        case CARD_CHANGE_OF_HEART:
            if (CountMonstersFiltered(one - link->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToControl);
            break;
        case CARD_SNATCH_STEAL:
        case CARD_1514:
            if (CountMonstersFiltered(1 - link->player, 1, 0) == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateFaceUpMonsterToControl);
            break;
        default:
            if (CountMonstersFiltered(1 - link->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOpponentMonsterTarget);
            break;
        }
        gChain.targetStep++;
        return 0;
    }
    /* Matching: no default, as in the ROM (the compiler warns that mask might be uninitialized). Every row of
     * gCardEffects that uses this handler is one of these five numbers, so the mask is always set. */
    switch (CARD_NUMBER(link->card)) {
    case CARD_INVADER_OF_THE_THRONE:
    case CARD_CHANGE_OF_HEART:
        mask = PICK_PLAYER1(PICK_ANY_MONSTER);
        break;
    case CARD_SNATCH_STEAL:
    case CARD_1244:
    case CARD_1514:
        mask = PICK_PLAYER1(PICK_FACE_UP_MONSTER_ANY);
        break;
    }
    if (DuelCursor_PickTarget(mask) != 0) {
        u8 *screen = (u8 *)&gDuelScreen;
        u32 *selPlayer = &SEL_WORD(screen, selPlayer);
        int player = *selPlayer;
        int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
        int side = 1 & player;
        struct DuelZone *target = ZONE_AT(side, zone);
        u16 targetId = ZONE_CARD_ID(target);
        if (CanCardTargetZoneInt(link->card, player, zone) != 0) {
            switch (CARD_NUMBER(link->card)) {
            case CARD_SNATCH_STEAL:
                if (CARD_NUMBER(targetId) == UNUSED_CARD_NUMBER_1351) {
                refuse:
                    PlaySE(SE_ERROR);
                    return 0;
                }
                /* fall through: Snatch Steal and key 1244 both need a face-up card */
            case CARD_1244:
                {
                    int side2 = 1 & player;
                    if (!ZONE_AT_PLAYER_FIRST(side2, zone)->isFaceUp)
                        goto refuse;
                }
                break;
            }
            TryAddEffectTarget(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of Kunai with Chain: pick one of your face-up monsters to equip. No CPU branch.
 *   Step 0: no target (return 1) if you have no face-up monster, else the prompt.
 *   Then: a pick among your face-up monsters; done if TryAddEffectTarget accepts it. B goes back to step 0.
 */
int EffectKunaiWithChainChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        link->numTargets = 0;
        if (CountMonstersFiltered(link->player, 1, 0) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateOwnMonsterToEquip);
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
        u8 zero = 0;
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER_ANY) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of Acid Trap Hole: pick a face-down defense-position monster of either side. No CPU branch.
 *   Step 0: prompt.
 *   Then: a pick; the result of TryAddEffectTarget is ignored, so the pick always finishes. B goes back to
 *         step 0.
 */
int EffectAcidTrapHoleChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateFaceDownDefenseMonster);
        (*step)++;
        return 0;
    }
    if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
        u8 zero = 0;
        *step = zero;
        return zero;
    }
    if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_MONSTER | PICK_DEFENSE_POSITION)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/*
 * ChainB of Bell of Destruction and keys 1330, 1451 and 1527: pick a face-up monster.
 *   CPU: for player 0, then player 1: the face-up monster with the highest ATK (GetZoneCardAtk); the first
 *        side whose best monster TryAddEffectTarget accepts gives the only target. Returns 1.
 *   Step 0: the prompt (Bell of Destruction and key 1451 say 'destroy'; the others 'Designate 1 monster.').
 *   Then: a pick among the face-up monsters of both sides; done if TryAddEffectTarget accepts it. B goes
 *         back to step 0.
 */
int EffectTargetableFaceUpMonsterChainB(struct ChainEntry *link)
{
    u8 *chain;
    u8 *step;
    int isCpu = LINK_PLAYER_BYTE(link);

    if (isCpu) {
        int player;
        link->numTargets = 0;
        for (player = 0; player <= 1; player++) {
            int bestAtk = -1;
            int bestZone = -1;
            int zone;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);    /* Matching: the redundant & 1 is the ROM's */
                if (HAS_CARD(z->card) && z->isFaceUp) {
                    int atk = GetZoneCardAtk(player, zone);
                    if (atk > bestAtk) {
                        bestAtk = atk;
                        bestZone = zone;
                    }
                }
            }
            if (bestZone >= 0) {
                if (TryAddEffectTarget(link, player, bestZone) != 0)
                    return 1;
            }
        }
        return 1;
    }
    chain = CHAIN_BYTES;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        link->numTargets = 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_BELL_OF_DESTRUCTION:
        case CARD_1451:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOneMonsterToDestroy);
            break;
        default:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateOneMonster);
            break;
        }
        gChain.targetStep++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = 0;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
    }
    return 0;
}

/*
 * ChainB of Magical Hats and keys 1316 and 1319: pick one of your own monsters. No CPU branch.
 *   Step 0: the card's prompt (Magical Hats: 'one of your monsters'; 1316: 'recall'; 1319: 'remove from
 *           play').
 *   Step 1: a pick among your monsters; the result of TryAddEffectTarget is ignored, so the pick always
 *           finishes. B goes back to step 0.
 */
int EffectOwnMonsterTargetChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    switch (*step) {
    case 0:
        link->numTargets = 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_MAGICAL_HATS:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateOneOwnMonster);
            break;
        case CARD_1316:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOwnMonsterToRecall);
            break;
        case CARD_1319:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateOwnMonsterToBanish);
            break;
        }
        gChain.targetStep++;
        break;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
            u8 zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
            TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
            return 1;
        }
        break;
    }
    return 0;
}

/* gChain.targetStep of 7 Completed's prompts. */
enum SevenCompletedStep {
    SEVEN_STEP_PROMPT_MONSTER = 0,
    SEVEN_STEP_PICK_MONSTER = 1,
    SEVEN_STEP_PROMPT_STAT = 2,         /* 'Which do you wish to increase?' menu */
    SEVEN_STEP_ADD_STAT = 3
};

/*
 * ChainB of 7 Completed: two targets, the monster to equip and the stat to raise. No CPU branch.
 *   Step 0: prompt.
 *   Step 1: a pick among the face-up monsters of both sides that EffectEquipTargetCheck accepts (else
 *           SE_ERROR); B goes back to step 0.
 *   Step 2: menu 'Which do you wish to increase?' (ATK+700 / DEF+700).
 *   Step 3: the answer plus 1 is the second target (SEVEN_COMPLETED_ATK or SEVEN_COMPLETED_DEF); done.
 */
int EffectSevenCompletedChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    switch (*step) {
    case SEVEN_STEP_PROMPT_MONSTER:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToEquip);
        link->numTargets = 0;
        (*step)++;
        return 0;
    case SEVEN_STEP_PICK_MONSTER:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
            u8 zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            if (EffectEquipTargetCheck(link, DUEL_LOC(*(u8 *)selPlayer, (u8)zone)) != 0) {
                TryAddEffectTarget(link, player, zone);
                (*step)++;
                return 0;
            }
            PlaySE(SE_ERROR);
        }
        break;
    case SEVEN_STEP_PROMPT_STAT:
        TextBoxOpen(TARGET_PROMPT_POS, STAT_MENU_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrAskSevenCompletedStat);
        TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
        (*step)++;
        return 0;
    case SEVEN_STEP_ADD_STAT:
        /* The menu answer is the line chosen (0 = ATK+700, 1 = DEF+700); +1 gives SEVEN_COMPLETED_ATK or
         * SEVEN_COMPLETED_DEF, which EffectEquipResolve stores as the zone's declared value. */
        AddEffectTarget(link, gTextBox.result + 1);
        return 1;
    }
    return 0;
}

/*
 * ChainB of Magic-Arm Shield: pick the opponent's monster that becomes the new attack target.
 *   CPU (in step 0): the first monster zone of the human's side that EffectMagicArmShieldCheck accepts;
 *        returns 1.
 *   Step 0: prompt.
 *   Then: a pick among the opponent's face-up monsters, accepted by EffectMagicArmShieldCheck (else
 *         SE_ERROR). No B handling.
 */
int EffectMagicArmShieldChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        link->numTargets = 0;
        if (link->player) {
            int zone;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                if (EffectMagicArmShieldCheck(link, (u8)(1 - link->player) | (u8)zone << 8) != 0) {
                    TryAddEffectTarget(link, 1 - link->player, zone);
                    return 1;
                }
            }
            return 1;
        }
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectNewAttackTarget);
        (*step)++;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        u8 *screen = (u8 *)&gDuelScreen;
        u32 *selPlayer = &SEL_WORD(screen, selPlayer);
        int player = *selPlayer;
        int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
        /* The check gets the opponent's side and selIndex alone as the zone: the cursor is on a monster, so
         * selArea is 0. */
        if (EffectMagicArmShieldCheck(link, (u8)(1 - link->player) | (u8)SEL_WORD(screen, selIndex) << 8) != 0) {
            TryAddEffectTarget(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of Remove Trap: pick a face-up Trap card of either side. No CPU branch.
 *   Step 0: prompt.
 *   Then: a pick; the result of TryAddEffectTarget is ignored, so the pick always finishes. B goes back to
 *         step 0.
 */
int EffectRemoveTrapChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrSelectFaceUpTrapToDestroy);
        link->numTargets = 0;
        (*step)++;
        return 0;
    }
    if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
        u8 zero = 0;
        *step = zero;
        return zero;
    }
    if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_TRAP)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/* gChain.targetStep of Two-Pronged Attack's prompts. */
enum TwoProngedStep {
    TWO_PRONGED_STEP_PROMPT_FIRST = 0,
    TWO_PRONGED_STEP_PICK_FIRST = 1,
    TWO_PRONGED_STEP_PROMPT_SECOND = 2,
    TWO_PRONGED_STEP_PICK_SECOND = 3,   /* must differ from targets[0] */
    TWO_PRONGED_STEP_PROMPT_OPPONENT = 4,
    TWO_PRONGED_STEP_PICK_OPPONENT = 5
};

/*
 * ChainB of Two-Pronged Attack (destroy 2 of your monsters and 1 of the opponent's), 6 steps. No CPU branch
 * and no early exit: the card's Prepare already guarantees two own monsters and one opponent monster. B goes
 * back to step 0 in every picking step.
 *   Step 0: prompt for the 1st own monster.
 *   Step 1: pick it.
 *   Step 2: prompt for the 2nd own monster.
 *   Step 3: pick a different one (SE_ERROR if it is the 1st monster again).
 *   Step 4: prompt for the opponent's monster.
 *   Step 5: pick it; done once TryAddEffectTarget accepts. A step above 5 also returns 1.
 */
int EffectTwoProngedAttackChainB(struct ChainEntry *link)
{
    u8 *chain = CHAIN_BYTES;
    int state = chain[TARGET_STEP];
    /* FAKEMATCH: a second copy of the gChain base, from which the shared reset forms the step pointer (the
     * ROM keeps it in its own register). */
    u8 *chainCopy = chain;

    switch (state) {
    case TWO_PRONGED_STEP_PROMPT_FIRST:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateFirstOwnMonster);
        link->numTargets = 0;
        goto next_step;
    case TWO_PRONGED_STEP_PICK_FIRST:
        if (gMain.newKeys & B_BUTTON)
            goto reset;
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) == 0)
            goto wait;
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) == 0)
            goto wait;
        goto next_step;
    case TWO_PRONGED_STEP_PROMPT_SECOND:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateSecondOwnMonster);
        {
            u8 *e = CHAIN_BYTES;
            register int offset __asm__("r2") = TARGET_STEP;
            u8 *p;
            /* FAKEMATCH: retain this initialized offset in r2 so this arm keeps its address setup. The
             * signed subtraction is e + offset; it preserves the ROM's ADD operand order. No instruction is
             * emitted by the empty constraint. */
            __asm__("" : "+r"(offset));
            p = e - (-offset);
            (*p)++;
        }
        goto wait;
    case TWO_PRONGED_STEP_PICK_SECOND:
        if (gMain.newKeys & B_BUTTON)
            goto reset;
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
            u8 *screen = (u8 *)&gDuelScreen;
            u32 *selPlayer = &SEL_WORD(screen, selPlayer);
            int zone = SEL_WORD(screen, selArea) + SEL_WORD(screen, selIndex);
            int player = *selPlayer;
            int pos = DUEL_LOC(*(u8 *)selPlayer, (u8)zone);
            if (link->targets[0] != pos) {
                if (TryAddEffectTarget(link, player, zone) != 0)
                    gChain.targetStep++;
            } else {
                u32 sound = SE_ERROR;
                /* FAKEMATCH: retain the initialized sound id separately from the call in
                 * TWO_PRONGED_STEP_PICK_OPPONENT, preserving this call's branch tail. The empty constraint
                 * emits no instruction. */
                __asm__("" : "+r"(sound));
                PlaySE(sound);
            }
        }
        goto wait;
    case TWO_PRONGED_STEP_PROMPT_OPPONENT:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentMonsterToDestroy);
    next_step:
        gChain.targetStep++;
    wait:
        return 0;
    case TWO_PRONGED_STEP_PICK_OPPONENT:
        if (gMain.newKeys & B_BUTTON) {
        reset:
            {
                u8 *resetStep = chainCopy + TARGET_STEP;
                /* FAKEMATCH: the zero in a u8 variable keeps the ROM's own reset tail. */
                u8 zero = 0;
                *resetStep = zero;
                return zero;
            }
        }
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER)) != 0) {
            if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
                return 1;
            PlaySE(SE_ERROR);
        }
        goto wait;
    default:
        return 1;
    }
}
