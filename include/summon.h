#ifndef GUARD_SUMMON_H
#define GUARD_SUMMON_H

/*
 * Summons. A Queue* builder fills the single pending summon gSummonAction (kind = enum SummonActionKind in
 * constants/duel.h) and calls SummonAction_Start; every frame DuelMainStep calls SummonAction_Update, which runs
 * the kind's step machine, applies the post-summon effects and opens the opponent's response window. In a link
 * duel a summon by player 1 is sent to the partner (LINKMSG_QUEUED_ACTION) and runs there.
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration keep that view as a commented local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "duel.h"

/*
 * gSummonAction (0x0201CF90, 0x14 bytes): the pending summon. The record is also sent over the link as is.
 * The first eight bytes are one u32 bitfield group; cardId crosses the word boundary at +0x4 (agbcc packs
 * bitfields without moving them to the next container).
 */
struct SummonAction {
    u32 player:1;               /* +0x0 bit 0: summoning player */
    u32 zone:5;                 /* bits 1-5: destination monster zone 0-4 */
    u32 sourceIndex:8;          /* bits 6-13: hand index (kinds 1, 2, 6), field zone (kind 3), 0 (kinds 4, 5) */
    u32 isFaceUp:1;             /* bit 14: bit 8 of the placement argument */
    u32 isDefense:1;            /* bit 15: bit 9 of the placement argument */
    u32 tribute1Zone:3;         /* bits 16-18: first tribute's monster zone */
    u32 tribute2Zone:3;         /* bits 19-21: second tribute's monster zone */
    u32 tribute3Zone:3;         /* bits 22-24: third tribute's zone (read by kind 6, never set) */
    u32 hasTribute1:1;          /* bit 25 */
    u32 hasTribute2:1;          /* bit 26 */
    u32 hasTribute3:1;          /* bit 27: never set */
    u32 tribute1Player:1;       /* bit 28: owner of the first tribute */
    u32 tribute2Player:1;       /* bit 29: owner of the second tribute */
    u32 unk0_30:1;              /* bit 30 */
    u32 cardId:16;              /* bits 31-46: card ID of the summoned card */
    u32 unk4_15:17;             /* bits 47-63 */
    struct DuelCard card;       /* +0x08: copy of the summoned card word (kinds 4/5) */
    u16 statusFlags;            /* +0x0C: enum ZoneStatusFlag mask for DUEL_CMD_SET_ZONE_STATUS_FLAGS */
    u32 active:1;               /* +0x0E bit 0: a summon is pending */
    u32 linked:1;               /* +0x0E bit 1: mirrored over the link (waits for, or sends,
                                 * LINKMSG_QUEUED_ACTION_DONE) */
    u32 kind:3;                 /* +0x0E bits 2-4: enum SummonActionKind */
    u32 step:7;                 /* +0x0E bits 5-11: step of the kind's step machine */
    u32 unkE_12:4;              /* +0x0F bits 4-7: cleared by SummonAction_Start; no reader */
    u32 unk10_0:3;              /* +0x10: cleared only */
    u32 unk10_3:8;              /* cleared only */
    u32 unk10_11:8;             /* cleared only */
    u32 unk10_19:13;
};

extern struct SummonAction gSummonAction;   /* 0x0201CF90 */

/* ---- Rules ---- */

/* 1 unless the player's Normal Summons are blocked (noNormalSummon, keys 1426/1526). */
int CanNormalSummon(int player);
/* 1 unless the player's Special Summons are blocked (noSpecialSummon, keys 1426/1510/1526). */
int CanSpecialSummon(int player);
/* Can this hand monster be summoned now (tributes, free zone, special procedures, Toon World)? */
int CanSummonFromHand(int player, u16 cardId);
/* Valkyrion the Magna Warrior: Alpha, Beta and Gamma The Magnet Warrior in hand or face up on the field. */
int CanSummonValkyrion(int player);
/* Summon condition of key 1257 (not an EDS card). */
int CanSummonKey1257(int player);
/* Can the player banish the monsters that the Special Summon of keys 1514-1519 costs (not EDS cards)? */
int CanPayBanishSummonCost(int player, u16 cardId);

/* ---- Queueing a summon ---- */

/* Kind 1: Normal Summon (faceUp) or Set of hand[handIndex] into zone. tributes: two bytes, each
 * zone (bits 0-2) | owner (bit 4) | present (bit 7). */
void QueueNormalSummon(int player, int handIndex, int zone, int tributes, int faceUp);
/* Kind 2: a Normal Summon granted by an effect; the position is asked when it runs. */
void QueueNormalSummonChoosePosition(int player, int handIndex, int zone, u16 tributes);
/* Kind 3: Flip Summon of the face-down monster in (player, zone). */
void QueueFlipSummon(int player, int zone);
/* Kind 4: Special Summon *card (not from the hand or field) in the given position. */
void QueueSpecialSummon(int player, struct DuelCard *card, u16 faceUp, u16 isDefense, u16 statusFlags);
/* Kind 5: Special Summon *card; the position is asked when it runs. */
void QueueSpecialSummonChoosePosition(int player, struct DuelCard *card, u16 faceUp, u16 statusFlags);
/* Kind 6: Special Summon hand[handIndex] face up into zone (attackPosition 0 = face-up defense). */
void QueueSpecialSummonFromHand(int player, int handIndex, int zone, int tributes, int attackPosition);

/* ---- Running a summon ---- */

/* Activate the filled record; for player 1 in a link duel send it to the partner. */
void SummonAction_Start(void);
/* Start a record received from the link partner (already copied into gSummonAction). */
void SummonAction_StartFromLink(void);
/* Per-frame driver called by DuelMainStep: nonzero while a summon is in progress. */
u16 SummonAction_Update(void);
/* Kind 1 step machine: tribute, place the monster, show it and run the on-summon effects; 1 when done. */
int ExecuteSummonAction(void);
/* Kind 2 step machine: ask Attack/Defense, then as ExecuteSummonAction; 1 when done. */
int ExecuteSummonActionAskPosition(void);
/* Kind 3 step machine (Flip Summon); 1 when done. */
u16 SummonStep_Flip(void);
/* Kind 4 step machine (Special Summon in a given position); 1 when done. */
u16 SummonStep_Special(void);
/* Kind 5 step machine (Special Summon, position chosen now); 1 when done. */
u16 SummonStep_SpecialChoosePosition(void);
/* Kind 6 step machine (Special Summon from the hand); 1 when done. */
u16 SummonStep_SpecialFromHand(void);

/* ---- Position menu ('Select display position of card.') ---- */

/* Text-box draw callback: the card upright (Attack) and turned (Defense); the selected one pulses. */
void SummonPositionMenu_Draw(void);
/* Text-box input callback: Left/Right toggle gTextBox.result, A confirms; 1 when done. */
u16 SummonPositionMenu_HandleInput(void);

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char summon_h_check_size[sizeof(struct SummonAction) == 0x14 ? 1 : -1];
typedef char summon_h_check_card[(u32)&((struct SummonAction *)0)->card == 0x8 ? 1 : -1];
typedef char summon_h_check_flags[(u32)&((struct SummonAction *)0)->statusFlags == 0xC ? 1 : -1];

#endif /* GUARD_SUMMON_H */
