#ifndef GUARD_DUEL_ACTIONS_H
#define GUARD_DUEL_ACTIONS_H

/*
 * Field actions of the duel rules: destroy, send to the graveyard, banish, return, flip, move, change
 * life points, draw. They do not change the field directly: each one checks the rules (Banisher of the
 * Light, Call of the Haunted, Snatch Steal, ...), queues the animated duel commands with DuelCmd_Push
 * (include/duel_cmd.h) and posts the response events that let cards answer. The field changes when the
 * duel command runner executes the queued commands. The immediate list and zone helpers they rely on
 * are in include/duel.h.
 *
 * Locations: "loc" parameters are packed (zone << 8) | player (DUEL_LOC in constants/duel.h). Card
 * words passed as u16 * / u32 * are struct DuelCard words (the definitions take them in halves).
 */

#include "global.h"

/* --- Zone links and control --- */

/* Give the monster in (player, zone) to the controller of its Snatch Steal-type equip (not removedLink). */
void UpdateMonsterControl(int player, int zone, u16 removedLink);
/* Queue duel command 0x85 (DuelCmd_AddZoneLink), which runs AddZoneLink(at, target, kind). */
void QueueAddZoneLink(int player, u16 target, u16 at, u16 kind);
/* Queue duel command 0x86 (DuelCmd_RemoveZoneLink), which runs RemoveZoneLink(at, target, kind). */
void QueueRemoveZoneLink(int player, u16 target, u16 at, u16 kind);
/* Equip the card at equipLoc to the monster at targetLoc (queues the equip link, posts RESPONSE_EQUIP). */
void EquipCard(int player, u16 equipLoc, u16 targetLoc);
/* Move an equip card from its current monster to the monster at newTargetLoc. */
void MoveEquipCard(u16 equipLoc, u16 newTargetLoc);
/* Meant to queue removal of the links other zones hold to (player, zone); buggy in the ROM (it indexes
 * the links with the player loop variable), kept as it matches. */
void QueueRemoveLinksToZone(int player, int zone);
/* Queue removal of the kind-2 links of (player, zone) that point at (targetPlayer, targetZone). */
void RemoveTargetLinksTo(int player, int zone, int targetPlayer, int targetZone);
/* Destroy the cards linked to a card that leaves the field (its equips, absorbed monsters, ...). */
void DestroyLinkedCards(int player, int zone, u16 destroyed);
/* Destroy every monster absorbed by (player, zone) (its ZONE_LINK_ABSORBED links). */
void DestroyAbsorbedMonsters(int player, int zone);

/* --- Cards leaving the field --- */

/* The card in (player, zone) goes to the graveyard: queues the move and runs the leave-field effects
 * when runTriggers is set; destroyed only picks the destroy animation and sound. */
void SendFieldCardToGrave(int player, int zone, u16 destroyed, u16 runTriggers);
/* Tribute the monster in (player, zone) (summons and tribute costs); 1, or 0 if it cannot be tributed
 * (the Banisher of the Light path returns no defined value). */
int TributeMonster(int player, int zone);
/* Send a Polymerization material on the field to the graveyard (like TributeMonster, no response event). */
int SendFusionMaterialToGrave(int player, int zone);
/* Destroy every face-up card with that card number on the player's field. */
void DestroyFaceUpCardsByNumber(int player, u16 cardNo);
/* Destroy the card in (player, zone): withEffects selects the destroy animation and the leave-field
 * effects. Only queues commands; the zone keeps its card until they run. */
void DestroyFieldCard(int player, int zone, u16 withEffects);
/* Destroy all of the player's monsters. */
void DestroyPlayerMonsters(int player, u16 withEffects);
/* Banish a monster destroyed in battle (card: copy of its word, the zone is already empty). */
void BanishBattleDestroyedCard(int player, int zone, u16 *card);
/* Send a battle-destroyed monster (card: its gBattle snapshot) to the graveyard and queue its triggers. */
void SendBattleDestroyedCardToGraveyard(int defender, int player, int zone, u16 *card);
/* Banish the card in (player, zone); flagged queues DUEL_CMD_BANISH_FLAGGED instead of DUEL_CMD_BANISH. */
void BanishFieldCard(int player, int zone, u16 flagged);
/* Return the card in (player, zone) to its owner's hand (Fusion: fusion deck; tokens: destroyed). */
void ReturnFieldCardToHand(int player, int zone, u16 cmdArg);
/* Return the card in (player, zone) to the top of its owner's deck (Fusion monsters to the fusion deck). */
void ReturnFieldCardToDeck(int player, int zone);
/* Return both players' monsters to the deck (opponent first), shuffling when shuffle is set. */
void ReturnAllMonstersToDeck(int player, u16 shuffle);

/* --- Position and movement --- */

/* Flip the card in (player, zone); triggerFlip runs the flip effect of a monster flipped face up. */
void FlipFieldCard(int player, int zone, u16 triggerFlip);
/* Change the battle position of the monster in (player, zone), flipping it face up if flipFaceUp. */
void ChangeBattlePosition(int player, int zone, u16 flipFaceUp, u16 triggerFlip);
/* Move the card at fromLoc to the empty zone toLoc (control change; player is unused). */
void MoveFieldCard(int player, u16 fromLoc, u16 toLoc);
/* Swap the cards at loc1 and loc2. */
void SwapFieldCards(int player, u16 loc1, u16 loc2);

/* --- Hand, deck and graveyard --- */

/* Return hand[handIdx] to the deck (top or bottom). */
void ReturnHandCardToDeck(int player, int handIdx, u16 toTop);
/* Discard hand[handIdx] to the graveyard (or banish it with Banisher of the Light). Pass compactHand = 0
 * while discarding several cards by index, and 1 with the last one. */
void DiscardHandCard(int player, int handIdx, u16 byOpponentEffect, u16 compactHand);
/* Discard the first hand card with that card number; 1 if there was one. */
int DiscardHandCardByNumber(int player, u16 cardNo);
/* Return the first graveyard card with that card number to the hand; 1 if there was one. */
int ReturnGraveyardCardToHand(int player, u16 cardNo);
/* Remove the first graveyard card with that card number (copied to *out); 1 if there was one. */
int RemoveGraveyardCardByNumber(int player, u16 cardNo, u32 *out);
/* Banish a graveyard card (card: its word). */
void BanishGraveyardCard(int player, u16 *card);
/* Remove the first deck card with that card number (copied to *out); its deck index, or -1. */
int RemoveDeckCardByNumber(int player, u16 cardNo, u32 *out);
/* Add the first deck card with that card number to the hand; 1 if there was one. */
int AddDeckCardToHand(int player, u16 cardNo);
/* Summon the first deck card with that card number to zone, face up in attack position; 1 if found. */
int PlaceDeckCardOnField(int player, u16 cardNo, int zone);
/* Draw count cards (one animated draw each) and run the draw triggers. */
void DrawCards(int player, int count);
/* Banish the top count cards of the deck. */
void BanishTopDeckCards(int player, int count);
/* Send the top count cards of the deck to the graveyard (banished instead with Banisher of the Light). */
void SendTopDeckCardsToGraveyard(int player, int count, u16 byOpponentEffect);
/* Send every deck and fusion-deck card with the same name as cardNo to the graveyard. */
void SendDeckCopiesToGraveyard(int player, u16 cardNo, u16 byOpponentEffect);
/* Banish every deck card with that card number. */
void BanishDeckCopies(int player, u16 cardNo);

/* --- Card pictures (full-screen card displays) --- */

/* Show the Card Detail view of cardId (300-frame auto-close), then reopen the duel screen. */
void ShowCardDetail(int player, u16 cardId);
/* Queue duel command 0x72 (DuelCmd_ShowCardZoomIn): the card picture zooms in, holds and fades. */
void ShowActivatedCard(int player, u16 arg);
/* Queue duel command 0x73 (DuelCmd_ShowCardEffect): the card picture with a white flash. */
void ShowCardEffect(int player, u16 cardId);
/* Queue duel command 0x74 (DuelCmd_ShowCardScatter): the card picture zooms in, then shrinks away. */
void ShowDestroyedCard(int player, u16 cardId);
/* Queue duel command 0x75 (DuelCmd_ShowCardUnrollDown): the card picture's rows grow from the top. */
void ShowPickedCard(int player, u16 arg);
/* Queue duel command 0x76 (DuelCmd_ShowCardUnrollSideways): the card picture slides in horizontally. */
void ShowRevealedCard(int player, u16 cardId);

/* --- Life points --- */

/* Lose amount LP (animated) and post RESPONSE_LP_CHANGE; nothing for 0. */
void LoseLifePoints(int player, int amount);
/* Battle damage to player: runs Robbin' Goblin and posts the battle-damage response event. */
void InflictBattleDamage(int player, int amount, u16 attackerLoc, u16 defenderLoc);
/* Gain amount LP (animated); nothing for 0. */
void GainLifePoints(int player, int amount);

#endif /* GUARD_DUEL_ACTIONS_H */
