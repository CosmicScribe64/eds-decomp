#include "global.h"
#include "gba.h"                    /* A_BUTTON, DPAD_*, L_BUTTON, R_BUTTON */
#include "main.h"                   /* gMain.newKeys, gMain.frameCounter */
#include "sprite.h"                 /* AddAffineSprite, SPRITE_SHAPE_32x32 */
#include "text_box.h"               /* TextBoxOpen, TextBoxSetMenu */
#include "constants/duel.h"         /* enum DuelArea, enum FieldPickMask */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CARD_FLIP, SE_ERROR */

/*
 * Duel field cursor and Big Eye's top-of-deck reorder (wiki/functions/duel-cursor-c.md).
 *
 * The first half is the cursor of the duel field screen. DuelCursor_PickTarget is the masked cursor of every
 * "select a card" prompt: it moves between the slots that DuelCursor_IsValidTarget accepts for a
 * field-pick mask, using the row walkers DuelCursor_FindTarget (up and down) and
 * DuelCursor_FindTargetHorizontal (left and right), and returns 1 on A. DuelCursor_PickAny is the free cursor of
 * the Main Phase, which also reaches the four piles.
 *
 * The second half is the five-card reorder of Big Eye (the state is gChain.scratch.deckReorder): the card
 * drawing, the text-box draw and input callbacks of the human, and the step machine that also plays the
 * CPU's side, which bubbles its key cards towards the top.
 *
 * Areas and players: the cursor is (player, area, index) with area = enum DuelArea (0 monster row,
 * 5 spell/trap row, 10 Field Magic zone, 11 hand, 12 fusion deck, 13 deck, 14 graveyard, 15 banished).
 * Player 0 is the human at the bottom of the screen; player 1's rows are mirrored, so Left and Right
 * (and the slot order along a row) are swapped for it.
 */

/* ---- BEGIN pre-H0 subset of duel.h ---- */
/*
 * The part of the staged duel.h that this unit and the headers it includes need (names, types and bitfield
 * containers as there; unused bytes are padding). include/duel.h still holds the legacy header until the
 * header switch (H0, build/readability/HEADERS.md), and chain.h and duel_screen.h include it, so this block
 * also defines its include guard. After H0, replace the block (BEGIN to END) with #include "duel.h".
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 unk12:20;
};

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
    u8 unk4[0x94 - 4];              /* not used here; chain.h embeds a whole zone */
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 unk3[0xD64 - 3];             /* the rest of the player's 0xD64 bytes */
};

extern struct DuelPlayer gDuelPlayers[2];   /* 0x020192E4 = gDuel.players */

void SwapDuelCards(u32 *a, u32 *b);         /* swap two duel card words */
void PlaySE(u32 seId);                      /* legacy sound.h lacks it */
/* ---- END pre-H0 subset ---- */

#include "chain.h"                  /* gChain.scratch.deckReorder (struct DeckReorderState) */
#include "card_data.h"              /* CARD_ID_MASK */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_*, CursorDirection, DuelInfo_DrawCard, ... */
#include "duel_flow.h"              /* gPulseScaleCurve */
#include "duel_prompt.h"            /* the DeckReorder_* prototypes */
#include "ai.h"                     /* AiIsKeyCard */

/*
 * Local views kept for matching (build/readability/HEADERS.md):
 * - the cursor flags at gDuelScreen +0x808 are updated as a byte; the header's u16 bitfield container would
 *   give halfword accesses;
 * - the cursor walkers test DuelCursor_IsValidTarget's result as a u16;
 * - the swap animation reads the dx and dy columns of gCardJumpArc through two table pointers, the second one
 *   through its own symbol gUnk_0819D280 (= &gCardJumpArc[0].dy); folding it into gCardJumpArc changes the code;
 * - DeckReorder_DrawCards loads the cursor byte (the low byte of the packed state word at gChain +0x53C,
 *   deckReorder.cursor) through its own symbol gUnk_02017F7C;
 * - the reorder state is a view of gChain at +0x53C with the card words as u32 (see DeckReorderWords).
 */
struct DuelScreenFlagsByte {
    u8 pad[0x808];
    u8 flags;                       /* +0x808 low byte: bit 3 = showCursor */
};
#define gDuelScreenFlagsByte (*(struct DuelScreenFlagsByte *)&gDuelScreen)
#define SHOW_CURSOR_FLAG        0x08

extern u16 DuelCursor_IsValidTarget16(int player, int area, int index, u32 mask) asm("DuelCursor_IsValidTarget");

extern const struct CardJumpArcEntry gCardJumpArc[16];  /* 0x0819D27C: the 16-step swap arc (dx, dy) */
extern const int gUnk_0819D280[];   /* 0x0819D280: the dy column of gCardJumpArc */
extern u8 gUnk_02017F7C;            /* 0x02017F7C: gChain.scratch.deckReorder.cursor */

extern const u8 gStrPromptReorderCards[];   /* "Switch the order of your cards using the following keys: ..." */

/* The reorder UI: five cards at x = 0x24 + 32 * slot, y = 0x60 (a hidden reorder, the CPU's, moves them
 * 16 pixels up per `hidden`). */
#define REORDER_CARD_COUNT      5
#define REORDER_LAST_SLOT       (REORDER_CARD_COUNT - 1)
#define REORDER_CARD_X          0x24
#define REORDER_CARD_SPACING    0x20
#define REORDER_CARD_Y          0x60
/* Last phase of the swap animation (gCardJumpArc has 16 steps, 0-15). */
#define REORDER_SWAP_LAST_PHASE 15
/* The CPU waits 30 frames per step (timer counts 0-0x1D). */
#define REORDER_CPU_THINK_LAST  0x1D
/* OBJ tile of a card back, and the OBJ palette 1 bits OR-ed into a card's tile. */
#define OBJ_TILE_CARD_BACK      0x40
#define OBJ_PALETTE_1           0x1000
/* Last argument of AddAffineSprite (scale << 16 | angle): scale 0x100, the size of a sprite that does not pulse. */
#define AFFINE_SCALE_NONE       0x1000000

/* Matching: the ROM reaches the reorder state as bitfields at an offset from the base of gChain, with the card
 * words as u32. gChain.scratch.deckReorder folds into one symbol+offset address and compiles differently. The
 * layout is that of struct DeckReorderState (chain.h). */
struct DeckReorderWords {
    u8 pad[OFFSET_OF(struct ChainState, scratch)];
    u32 cursor:8;                   /* bits 0-7: selected card 0-4 */
    u32 unk8:4;
    u32 mode:8;                     /* bits 12-19: enum DeckReorderMode */
    u32 phase:8;                    /* bits 20-27: swap animation phase 0-16 */
    u32 timer:8;                    /* bits 28-35: CPU think timer (straddles into the next byte) */
    u32 unk36:8;
    u32 unk44:20;
    u32 cards[REORDER_CARD_COUNT];  /* +0x544: the card words */
};
#define DECK_REORDER            (*(struct DeckReorderWords *)&gChain)

/* Card number of a card ID through the integer address of gCardIdToNumber (0x08622AB4): the ROM needs this
 * form (the symbol gives other code). */
#define CARD_NUMBER_OF(id)      (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
/* Card ID, bits 0-11, of a card word held as a u32. */
#define CARD_WORD_ID(word)      (((word) << 20) >> 20)

/*
 * Step the cursor one slot to the Right or Left along its row (CURSOR_DIR_RIGHT / CURSOR_DIR_LEFT in
 * `direction`), repeating until DuelCursor_IsValidTarget accepts the slot; that slot is stored through the
 * pointers and 1 is returned. Returns 0 when the walk comes back to its start.
 *
 * For player 0 Right counts up; player 1's rows are mirrored. The hand wraps at handCount, the spell/trap
 * row wraps modulo 5, and the monster row runs on into the Field Magic zone (area 10), which leads back to
 * the near end of the monster row.
 */
u16 DuelCursor_FindTargetHorizontal(u16 direction, int *player, int *area, int *index, u32 mask)
{
    int p = *player, a = *area, i = *index;
    int startPlayer = p, startArea = a, startIndex = i;
    struct DuelPlayer *players = gDuelPlayers;
    int side = p & 1;
    /* Matching: assigning the player offset to one variable in both direction blocks
     * keeps the multiply inside the loop: a twice-set destination is not
     * loop-invariant, so loop.c leaves the 0xD64 multiply in place as in ROM. */
    int off;
    for (;;) {
    if (direction & CURSOR_DIR_RIGHT) {
        switch (a) {
        case DUEL_AREA_HAND:
            if (p != 0) {
                if (i <= 0) { off = side * 0xD64; i = ((struct DuelPlayer *)(off + (u32)players))->handCount; }
            dec4:
                i--;
            } else {
                if (i < players[0].handCount - 1) i++;
                else i = 0;
            }
            break;
        case DUEL_AREA_MONSTER:
            if (p != 0) {
                if (i > 0) goto dec4; /* Matching: shares the hand case's decrement, as in the ROM */
                a = DUEL_AREA_FIELD; i = 0;
            } else {
                if (i <= 3) i++;
                else { a = DUEL_AREA_FIELD; i = 0; }
            }
            break;
        case DUEL_AREA_SPELL_TRAP:
            i = (p != 0 ? i + 4 : i + 1) % 5;
            break;
        case DUEL_AREA_FIELD:
            a = DUEL_AREA_MONSTER;
            i = p != 0 ? 4 : 0;
            break;
        }
    }
    if (direction & CURSOR_DIR_LEFT) {
        switch (a) {
        case DUEL_AREA_HAND:
            if (p != 0) {
                off = side * 0xD64;
                if (i < ((struct DuelPlayer *)(off + (u32)players))->handCount - 1) i++;
                else i = 0;
            } else {
                if (i <= 0) i = players[0].handCount;
            dec8:
                i--;
            }
            break;
        case DUEL_AREA_MONSTER:
            if (p != 0) {
                if (i <= 3) i++;
                else { a = DUEL_AREA_FIELD; i = 0; }
            } else {
                if (i > 0) goto dec8; /* Matching: shares the hand case's decrement, as in the ROM */
                a = DUEL_AREA_FIELD; i = 0;
            }
            break;
        case DUEL_AREA_SPELL_TRAP:
            i = (p != 0 ? i + 1 : i + 4) % 5;
            break;
        case DUEL_AREA_FIELD:
            a = DUEL_AREA_MONSTER;
            if (p != 0) i = 0; else i = 4;
            break;
        }
    }
    if (startPlayer == p && startArea == a && startIndex == i)
        return 0;
    if (DuelCursor_IsValidTarget16(p, a, i, mask)) {
        *player = p;
        *area = a;
        *index = i;
        return 1;
    }
    }
}

/*
 * Move the cursor to the next row in direction CURSOR_DIR_UP or CURSOR_DIR_DOWN. The rows run from player 0's
 * hand at the bottom up through the spell/trap and monster rows to player 1's rows and hand at the top,
 * and wrap around; an empty hand is skipped, and entering a hand from the far side selects its last
 * card. In each row slot 0 (the last card of a hand) is tried first, then DuelCursor_FindTargetHorizontal
 * scans the rest of the row. The first slot DuelCursor_IsValidTarget accepts is stored through the pointers and 1
 * is returned; 0 when the walk is back at its start. Left and Right go straight to
 * DuelCursor_FindTargetHorizontal.
 */
u16 DuelCursor_FindTarget(u16 direction, int *player, int *area, int *index, u32 mask)
{
    int p = *player;
    int a = *area;
    int i = *index;
    int startPlayer = p, startArea = a, startIndex = i;
    int backP, backA, backI;

    /* Matching: the main loop is nested inside this if so the "no horizontal/vertical
     * bit" path falls through to the shared DuelCursor_FindTargetHorizontal tail; writing it as an
     * early `return DuelCursor_FindTargetHorizontal(...)` instead makes agbcc move that tail to the
     * top of the function and the unit no longer matches. */
    if (direction & (CURSOR_DIR_UP | CURSOR_DIR_DOWN)) {
    for (;;) {
        if (direction & CURSOR_DIR_UP) {
            switch (a) {
            case DUEL_AREA_HAND:
                if (p != 0) {
                    /* player 1's hand is the top row: wrap to player 0's hand (its last card) */
                    if (gDuelPlayers[0].handCount != 0) {
                        p = 0;
                        i = gDuelPlayers[0].handCount - 1;
                    } else {
                        p = 0;
                        a = DUEL_AREA_SPELL_TRAP;
                        i = 0;
                    }
                } else {
                    p = 0;
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                }
                break;
            case DUEL_AREA_SPELL_TRAP:
                if (p == 0) {
                    a = DUEL_AREA_MONSTER;
                    i = 0;
                } else if (gDuelPlayers[1].handCount != 0) {
                    a = DUEL_AREA_HAND;
                    i = 0;
                } else if (gDuelPlayers[0].handCount != 0) {
                    p = 0;
                    a = DUEL_AREA_HAND;
                    i = 0;
                } else {
                    p = 0;
                    i = 0;
                }
                break;
            case DUEL_AREA_MONSTER:
                if (p != 0) {
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                } else {
                    p = 1;
                    i = 0;
                }
                break;
            case DUEL_AREA_FIELD:
                if (p != 0) {
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                } else {
                    a = DUEL_AREA_MONSTER;
                    i = 0;
                }
                break;
            }
        }
        if (direction & CURSOR_DIR_DOWN) {
            switch (a) {
            case DUEL_AREA_HAND:
                if (p != 0) {
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                } else if (gDuelPlayers[1].handCount != 0) {
                    /* player 0's hand is the bottom row: wrap to player 1's hand (its last card) */
                    p = 1;
                    i = gDuelPlayers[1].handCount - 1;
                } else {
                    p = 1;
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                }
                break;
            case DUEL_AREA_SPELL_TRAP:
                if (p != 0) {
                    a = DUEL_AREA_MONSTER;
                    i = 0;
                } else if (gDuelPlayers[0].handCount != 0) {
                    a = DUEL_AREA_HAND;
                    i = 0;
                } else if (gDuelPlayers[1].handCount != 0) {
                    p = 1;
                    a = DUEL_AREA_HAND;
                    i = 0;
                } else {
                    p = 1;
                    i = 0;
                }
                break;
            case DUEL_AREA_MONSTER:
                if (p != 0) {
                    p = 0;
                    i = 0;
                } else {
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                }
                break;
            case DUEL_AREA_FIELD:
                if (p != 0) {
                    a = DUEL_AREA_MONSTER;
                    i = 0;
                } else {
                    a = DUEL_AREA_SPELL_TRAP;
                    i = 0;
                }
                break;
            }
        }
        if (startPlayer == p && startArea == a && startIndex == i)
            return 0;
        if (DuelCursor_IsValidTarget16(p, a, i, mask)) {
            *player = p;
            *area = a;
            *index = i;
            return 1;
        }
        /* The row's first slot is not valid: scan the rest of the row for one. */
        backP = p;
        backA = a;
        backI = i;
        do {
            if (DuelCursor_FindTargetHorizontal(CURSOR_DIR_RIGHT, &p, &a, &i, mask)) {
                *player = p;
                *area = a;
                *index = i;
                return 1;
            }
        } while (p != backP || a != backA || i != backI);
    }
    }
    return DuelCursor_FindTargetHorizontal(direction, player, area, index, mask);
}

/*
 * One frame of the masked cursor of the effect prompts: it only rests on slots that DuelCursor_IsValidTarget
 * accepts for `mask` (enum FieldPickMask, player 1's slots shifted by 16). Shows the cursor (showCursor);
 * a cursor parked on a pile (areas 12-15) first jumps to (player, monster row, 0). A cursor on a slot the
 * mask rejects jumps to the first valid one (searching upwards). The D-pad then moves to the next valid slot
 * with SE_CURSOR. Returns 1 on A with the pick in gDuelScreen.selPlayer/selArea/selIndex; A also returns 1
 * when no slot matches, so callers re-check the pick. Returns 0 otherwise.
 */
int DuelCursor_PickTarget(u32 mask)
{
    u16 keys = gMain.newKeys;
    int player = gDuelScreen.selPlayer;
    int area = gDuelScreen.selArea;
    int index = gDuelScreen.selIndex;
    gDuelScreenFlagsByte.flags |= SHOW_CURSOR_FLAG;
    switch (area) {
    case DUEL_AREA_FUSION_DECK: case DUEL_AREA_DECK: case DUEL_AREA_GRAVEYARD: case DUEL_AREA_BANISHED:
        DuelCursor_Select(player, DUEL_AREA_MONSTER, 0);
        return 0;
    }
    if (!DuelCursor_IsValidTarget16(player, area, index, mask)) {
        if (DuelCursor_FindTarget(CURSOR_DIR_UP, &player, &area, &index, mask)) {
            DuelCursor_Select(player, area, index);
            return 0;
        }
    }
    if (keys & DPAD_UP) {
        if (DuelCursor_FindTarget(CURSOR_DIR_UP, &player, &area, &index, mask)) {
            PlaySE(SE_CURSOR);
            DuelCursor_Select(player, area, index);
            return 0;
        }
    }
    if (keys & DPAD_DOWN) {
        if (DuelCursor_FindTarget(CURSOR_DIR_DOWN, &player, &area, &index, mask)) {
            PlaySE(SE_CURSOR);
            DuelCursor_Select(player, area, index);
            return 0;
        }
    }
    if (keys & DPAD_LEFT) {
        if (DuelCursor_FindTarget(CURSOR_DIR_LEFT, &player, &area, &index, mask)) {
            PlaySE(SE_CURSOR);
            DuelCursor_Select(player, area, index);
            return 0;
        }
    }
    if (keys & DPAD_RIGHT) {
        if (DuelCursor_FindTarget(CURSOR_DIR_RIGHT, &player, &area, &index, mask)) {
            PlaySE(SE_CURSOR);
            DuelCursor_Select(player, area, index);
            return 0;
        }
    }
    if (keys & A_BUTTON)
        return 1;
    return 0;
}

/*
 * One frame of the free duel cursor (Main Phase browsing, no mask). It shows the cursor (showCursor) and moves
 * between every area of both players, including the piles, with SE_CURSOR; Left or Right inside an empty hand
 * play SE_ERROR instead. Returns 1 on A, else 0.
 *
 * Left and Right step along a row (player 1 mirrored); at its ends the cursor passes through the piles beside
 * the row (Field Magic zone 10 and graveyard 14 by the monster row, fusion deck 12 and deck 13 by the spell/trap
 * row). Up and Down go to the next row or, from a pile, to the pile or zone above or below it, crossing over to
 * the other player's side in the middle.
 *
 * Both rows have five slots (0-4), so `index <= 3` means "not the last slot" and 4 - index mirrors a slot.
 *
 * Matching: each case keeps its own sound and return so that cross-jumping merges the tails as in the
 * ROM; the first switch uses the local area, the later ones re-read gDuelScreen.selArea.
 */
int DuelCursor_PickAny(void)
{
    u16 keys = gMain.newKeys;
    int player = gDuelScreen.selPlayer;
    int area = gDuelScreen.selArea;
    int index = gDuelScreen.selIndex;
    struct DuelPlayer *players, *side;

    gDuelScreenFlagsByte.flags |= SHOW_CURSOR_FLAG;
    if (keys & DPAD_LEFT) {
        switch (player) {
        case 0:
            switch (area) {
            case DUEL_AREA_MONSTER:
                if (index > 0)
                    DuelCursor_Select(player, area, index - 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                if (index > 0)
                    DuelCursor_Select(player, area, index - 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                players = gDuelPlayers; side = &players[player & 1];
                if (side->handCount != 0) {
                    if (index > 0)
                        DuelCursor_Select(player, area, index - 1);
                    else
                        DuelCursor_Select(player, area, side->handCount - 1);
                    PlaySE(SE_CURSOR);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))PlaySE)(SE_ERROR);
                    break;
                }
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 4);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD: case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, 4);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        case 1:
            switch (area) {
            case DUEL_AREA_MONSTER:
                if (index <= 3)
                    DuelCursor_Select(player, area, index + 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                if (index <= 3)
                    DuelCursor_Select(player, area, index + 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                players = gDuelPlayers; side = &players[player & 1];
                if (side->handCount != 0) {
                    if (index < side->handCount - 1)
                        DuelCursor_Select(player, area, index + 1);
                    else
                        DuelCursor_Select(player, area, 0);
                    PlaySE(SE_CURSOR);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))PlaySE)(SE_ERROR);
                    break;
                }
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD: case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        }
    }
    if (keys & DPAD_RIGHT) {
        switch (player) {
        case 0:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                if (index <= 3)
                    DuelCursor_Select(player, area, index + 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                if (index <= 3)
                    DuelCursor_Select(player, area, index + 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                players = gDuelPlayers; side = &players[player & 1];
                if (side->handCount != 0) {
                    if (index < side->handCount - 1)
                        DuelCursor_Select(player, area, index + 1);
                    else
                        DuelCursor_Select(player, area, 0);
                    PlaySE(SE_CURSOR);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))PlaySE)(SE_ERROR);
                    break;
                }
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD: case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        case 1:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                if (index > 0)
                    DuelCursor_Select(player, area, index - 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                if (index > 0)
                    DuelCursor_Select(player, area, index - 1);
                else
                    DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                players = gDuelPlayers; side = &players[player & 1];
                if (side->handCount != 0) {
                    if (index > 0)
                        DuelCursor_Select(player, area, index - 1);
                    else
                        DuelCursor_Select(player, area, side->handCount - 1);
                    PlaySE(SE_CURSOR);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))PlaySE)(SE_ERROR);
                    break;
                }
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 4);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD: case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, 4);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        }
    }
    if (keys & DPAD_UP) {
        switch (player) {
        case 0:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                /* to the opponent's monster row, mirrored slot */
                DuelCursor_Select(1 - player, area, 4 - index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FIELD:
                DuelCursor_Select(1 - player, DUEL_AREA_BANISHED, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD:
                DuelCursor_Select(player, DUEL_AREA_BANISHED, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_BANISHED:
                DuelCursor_Select(1 - player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        case 1:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                DuelCursor_Select(player, DUEL_AREA_HAND, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                DuelCursor_Select(1 - player, DUEL_AREA_HAND, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(1 - player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(1 - player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD:
                DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        }
    }
    if (keys & DPAD_DOWN) {
        switch (player) {
        case 0:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                DuelCursor_Select(player, DUEL_AREA_HAND, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                /* from the bottom row to the opponent's hand */
                DuelCursor_Select(1 - player, area, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FIELD:
                DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(1 - player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(1 - player, DUEL_AREA_FUSION_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD:
                DuelCursor_Select(player, DUEL_AREA_DECK, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_BANISHED:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        case 1:
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
                DuelCursor_Select(1 - player, area, 4 - index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_SPELL_TRAP:
                DuelCursor_Select(player, DUEL_AREA_MONSTER, index);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_HAND:
                DuelCursor_Select(player, DUEL_AREA_SPELL_TRAP, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FIELD:
                DuelCursor_Select(1 - player, DUEL_AREA_BANISHED, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
                DuelCursor_Select(player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_DECK:
                DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_GRAVEYARD:
                DuelCursor_Select(player, DUEL_AREA_BANISHED, 0);
                PlaySE(SE_CURSOR);
                return 0;
            case DUEL_AREA_BANISHED:
                DuelCursor_Select(1 - player, DUEL_AREA_FIELD, 0);
                PlaySE(SE_CURSOR);
                return 0;
            }
            return 0;
        }
    }
    if (gMain.newKeys & A_BUTTON)
        return 1;
    return 0;
}

/*
 * Draw the five cards of the reorder as 32x32 affine card sprites, `hidden` rows of 16 pixels up from the
 * normal row (the CPU's reorder hides the cards: card backs, OBJ tile 0x40). The card under the cursor pulses
 * with gPulseScaleCurve; the others keep scale 0x100.
 */
void DeckReorder_DrawCards(int hidden)
{
    int i = 0;
    u32 *base = DECK_REORDER.cards;
    u32 y = (REORDER_CARD_Y - (hidden << 4)) << 16;
    int x = REORDER_CARD_X;
    u32 *cards;
    const u16 *pulseCurve;
    u16 *frame;
    u8 *cursor;
    asm("" :: "r"(base)); /* FAKEMATCH: keep base live so combine leaves the copy */
    cards = base;
    cursor = &gUnk_02017F7C;
    pulseCurve = gPulseScaleCurve;
    frame = &gMain.frameCounter;
    for (; i < REORDER_CARD_COUNT; i++) {
        u16 tile = GetCardIconObjTile(CARD_WORD_ID(*cards)) | OBJ_PALETTE_1;
        u32 flags;
        u32 position;
        if (hidden != 0)
            tile = OBJ_TILE_CARD_BACK;
        position = x | y;
        if (i == *cursor)
            flags = *(const u16 *)((u32)pulseCurve + (*frame & 0x1E)) << 16;
        else
            flags = AFFINE_SCALE_NONE;
        AddAffineSprite(position, SPRITE_SHAPE_32x32, tile, flags);
        x += REORDER_CARD_SPACING;
        cards++;
    }
}


/*
 * DeckReorder_DrawCards with the swap animation: card `from` moves by (+dx, -dy) and card `to` by (-dx, +dy),
 * with the offsets taken from gCardJumpArc[phase] (phase = deckReorder.phase, 0-15), so that the two cards
 * trade places along mirrored arcs.
 */
void DeckReorder_DrawSwap(int hidden, int from, int to)
{
    int i = 0;
    /* FAKEMATCH: pin cards to r8; otherwise hidden takes r8 and cards r9 */
    register u32 *cards asm("r8") = DECK_REORDER.cards;
    for (; i < REORDER_CARD_COUNT; i++) {
        u16 id = CARD_WORD_ID(cards[i]);
        int x = (i << 5) + REORDER_CARD_X;
        int y = REORDER_CARD_Y - (hidden << 4);
        u16 tile = GetCardIconObjTile(id) | OBJ_PALETTE_1;
        u32 position, flags;
        if (hidden != 0)
            tile = OBJ_TILE_CARD_BACK;
        if (i == from) {
            /* FAKEMATCH: table pointers set at block start (y before x) give
             * long lifetimes so loop.c hoists both; x then wins sl and y is
             * spilled and rematerialized by reload, which shifts reload's
             * register rotation so the `to` compare loads into r2. u8 phase
             * adds combinable insns that keep 0x03000040 from being hoisted
             * in the second loop pass. */
            const int *arcDy = gUnk_0819D280;
            const int *arcDx = &gCardJumpArc[0].dx;
            /* deckReorder.phase: bits 4-11 of the halfword at gChain +0x53E (cards - 6) */
            u8 phase = ((u32)*(u16 *)((u8 *)cards - 6) << 20) >> 24;
            x += *(const int *)((phase << 3) + (u32)arcDx);
            y -= *(const int *)((u32)arcDy + (phase << 3));
        }
        if (i == to) {
            const int *arcDy = gUnk_0819D280;
            const int *arcDx = &gCardJumpArc[0].dx;
            u8 phase = ((u32)*(u16 *)((u8 *)cards - 6) << 20) >> 24;
            x -= *(const int *)((phase << 3) + (u32)arcDx);
            y += *(const int *)((u32)arcDy + (phase << 3));
        }
        position = (y << 16) | x;
        if (i == *(u8 *)&gChain.scratch)
            flags = gPulseScaleCurve[(gMain.frameCounter & 0x1E) / 2] << 16;
        else
            flags = AFFINE_SCALE_NONE;
        AddAffineSprite(position, SPRITE_SHAPE_32x32, tile, flags);
    }
}

/*
 * Text-box draw callback of the reorder: while a swap animates (mode 10: the cursor card trades with its left
 * neighbour; mode 20: with its right one) and phase <= 15 it draws the swap, otherwise the five cards.
 * Everything is drawn face up (hidden = 0).
 */
void DeckReorder_Draw(void)
{
    /* Matching: the mode is read as (word << 12) >> 24, the cursor as the first byte, and the phase through
     * the halfword at gChain +0x53E, all from the base of gChain; the bitfield members compile differently. */
    struct ChainState *chain = &gChain;
    u32 *state = (u32 *)((u8 *)chain + OFFSET_OF(struct ChainState, scratch));
    switch ((int)((*state << 12) >> 24)) {
    case DECK_REORDER_SWAP_LEFT:
        if ((int)(((u32)*(u16 *)((u8 *)chain + OFFSET_OF(struct ChainState, scratch) + 2) << 20) >> 24)
            <= REORDER_SWAP_LAST_PHASE) {
            int to = *(u8 *)state;
            DeckReorder_DrawSwap(0, to - 1, to);
            break;
        }
        DeckReorder_DrawCards(0);
        break;
    case DECK_REORDER_SWAP_RIGHT:
        if ((int)(((u32)*(u16 *)((u8 *)chain + OFFSET_OF(struct ChainState, scratch) + 2) << 20) >> 24)
            <= REORDER_SWAP_LAST_PHASE) {
            int from = *(u8 *)state;
            DeckReorder_DrawSwap(0, from, from + 1);
            break;
        }
        DeckReorder_DrawCards(0);
        break;
    default:
        DeckReorder_DrawCards(0);
        break;
    }
}

/*
 * Text-box input callback of the reorder (the human's side); returns 1 when the order is confirmed.
 * In the swap modes it waits out the 16 animation phases, then swaps the cursor card with its left or right
 * neighbour, moves the cursor along with it and returns to DECK_REORDER_SELECT. In SELECT: Left/Right move
 * the cursor (modulo 5, SE_CURSOR) and redraw the card info; L starts a swap with the left neighbour and R
 * with the right one (SE_CARD_FLIP, or SE_ERROR at the end of the row); A plays SE_CONFIRM, sets
 * DECK_REORDER_DONE and returns 1.
 */
int DeckReorder_HandleInput(void)
{
    switch (DECK_REORDER.mode) {
    case DECK_REORDER_SWAP_LEFT:
        if (DECK_REORDER.phase <= REORDER_SWAP_LAST_PHASE) {
            DECK_REORDER.phase++;
        } else {
            SwapDuelCards(&DECK_REORDER.cards[DECK_REORDER.cursor - 1], &DECK_REORDER.cards[DECK_REORDER.cursor]);
            DECK_REORDER.cursor--;
            DECK_REORDER.mode = DECK_REORDER_SELECT;
        }
        return 0;
    case DECK_REORDER_SWAP_RIGHT:
        if (DECK_REORDER.phase <= REORDER_SWAP_LAST_PHASE) {
            DECK_REORDER.phase++;
        } else {
            SwapDuelCards(&DECK_REORDER.cards[DECK_REORDER.cursor + 1], &DECK_REORDER.cards[DECK_REORDER.cursor]);
            DECK_REORDER.cursor++;
            DECK_REORDER.mode = DECK_REORDER_SELECT;
        }
        return 0;
    default:
        if (gMain.newKeys & DPAD_LEFT) {
            PlaySE(SE_CURSOR);
            DECK_REORDER.cursor = (DECK_REORDER.cursor + REORDER_LAST_SLOT) % REORDER_CARD_COUNT;
            TextCellsClear();
            DuelInfo_DrawCard(CARD_WORD_ID(DECK_REORDER.cards[DECK_REORDER.cursor]), 1);
        }
        break;
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        PlaySE(SE_CURSOR);
        DECK_REORDER.cursor = (DECK_REORDER.cursor + 1) % REORDER_CARD_COUNT;
        TextCellsClear();
        DuelInfo_DrawCard(CARD_WORD_ID(DECK_REORDER.cards[DECK_REORDER.cursor]), 1);
    }
    if (gMain.newKeys & L_BUTTON) {
        if (DECK_REORDER.cursor != 0) {
            PlaySE(SE_CARD_FLIP);
            DECK_REORDER.mode = DECK_REORDER_SWAP_LEFT;
            DECK_REORDER.phase = 0;
            return 0;
        }
        PlaySE(SE_ERROR);
    }
    if (gMain.newKeys & R_BUTTON) {
        if (DECK_REORDER.cursor < REORDER_LAST_SLOT) {
            PlaySE(SE_CARD_FLIP);
            DECK_REORDER.mode = DECK_REORDER_SWAP_RIGHT;
            DECK_REORDER.phase = 0;
            return 0;
        }
        PlaySE(SE_ERROR);
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_CONFIRM);
        DECK_REORDER.mode = DECK_REORDER_DONE;
        return 1;
    }
    return 0;
}

/*
 * Step machine of the top-of-deck reorder on deckReorder.mode (enum DeckReorderMode); arg0 is 0 for the human
 * and nonzero for the CPU. Returns 1 when the reorder is finished.
 *
 * INIT: the human gets the text box 'Switch the order of your cards ...' with DeckReorder_Draw and
 * DeckReorder_HandleInput and the info of the first card; both players go on to SELECT.
 * SELECT: the human is done at once (the callbacks run the UI). The CPU draws the hidden cards and waits 30
 * frames per step. It then moves the cursor card right (mode 20) when that card is not an AI key card but the
 * next one is, or left (mode 10) when it is a key card and the previous one is not; otherwise it advances the
 * cursor (finishing after slot 4). SWAP_LEFT / SWAP_RIGHT: the CPU animates the swap, swaps the card words and
 * returns to SELECT. Any other mode (DECK_REORDER_DONE) returns 1.
 */
int DeckReorder_Run(int arg0)
{
    int mode = DECK_REORDER.mode;
    switch (mode) {
    case DECK_REORDER_INIT:
        if (arg0 == 0) {
            TextBoxOpen(0x206, 0x813, TEXTBOX_FLAGS_DEFAULT, gStrPromptReorderCards);
            TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, DeckReorder_Draw, (u16 (*)(void))DeckReorder_HandleInput);
            DECK_REORDER.timer = mode;
            TextCellsClear();
            DuelInfo_DrawCard(DECK_REORDER.cards[0] << 20 >> 20, 1);
        }
        DECK_REORDER.mode++;
        return 0;
    case DECK_REORDER_SELECT:
        if (arg0 == 0)
            return 1;
        DeckReorder_DrawCards(arg0);
        if (DECK_REORDER.timer <= REORDER_CPU_THINK_LAST) {
            DECK_REORDER.timer++;
            return 0;
        }
        DECK_REORDER.timer = 0;
        if (DECK_REORDER.cursor < REORDER_LAST_SLOT) {
            u32 cur = *(DECK_REORDER.cards + DECK_REORDER.cursor) << 20 >> 20;
            u32 nxt = *(DECK_REORDER.cards + (DECK_REORDER.cursor + 1)) << 20 >> 20;
            if (AiIsKeyCard(1, CARD_NUMBER_OF(cur)) == 0 &&
                AiIsKeyCard(1, CARD_NUMBER_OF(nxt)) != 0) {
                DECK_REORDER.mode = DECK_REORDER_SWAP_RIGHT;
                DECK_REORDER.phase = 0;
                return 0;
            }
        }
        if (DECK_REORDER.cursor != 0) {
            u32 cur = *(DECK_REORDER.cards + DECK_REORDER.cursor) << 20 >> 20;
            u32 prv = *(DECK_REORDER.cards + (DECK_REORDER.cursor - 1)) << 20 >> 20;
            if (AiIsKeyCard(1, CARD_NUMBER_OF(cur)) != 0 &&
                AiIsKeyCard(1, CARD_NUMBER_OF(prv)) == 0) {
                DECK_REORDER.mode = DECK_REORDER_SWAP_LEFT;
                DECK_REORDER.phase = 0;
                return 0;
            }
        }
        if (DECK_REORDER.cursor >= REORDER_LAST_SLOT)
            return 1;
        DECK_REORDER.cursor++;
        return 0;
    case DECK_REORDER_SWAP_LEFT:
        if (arg0 == 0)
            return 0;
        if (DECK_REORDER.phase <= REORDER_SWAP_LAST_PHASE) {
            DeckReorder_DrawSwap(arg0, DECK_REORDER.cursor - 1, DECK_REORDER.cursor);
            DECK_REORDER.phase++;
        } else {
            SwapDuelCards(&DECK_REORDER.cards[DECK_REORDER.cursor - 1], &DECK_REORDER.cards[DECK_REORDER.cursor]);
            DECK_REORDER.mode = DECK_REORDER_SELECT;
            DeckReorder_DrawCards(arg0);
        }
        return 0;
    case DECK_REORDER_SWAP_RIGHT:
        if (arg0 == 0)
            return 0;
        if (DECK_REORDER.phase <= REORDER_SWAP_LAST_PHASE) {
            DeckReorder_DrawSwap(arg0, DECK_REORDER.cursor, DECK_REORDER.cursor + 1);
            DECK_REORDER.phase++;
        } else {
            SwapDuelCards(DECK_REORDER.cards + (DECK_REORDER.cursor + 1), &DECK_REORDER.cards[DECK_REORDER.cursor]);
            DECK_REORDER.mode = DECK_REORDER_SELECT;
            DeckReorder_DrawCards(arg0);
        }
        return 0;
    }
    return 1;
}
