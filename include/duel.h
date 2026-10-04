#ifndef GUARD_DUEL_H
#define GUARD_DUEL_H

/*
 * The duel data model: gDuel (struct DuelState) holds a placement serial, the two players (struct
 * DuelPlayer, 0xD64 bytes each) and the duel-wide state that follows them: negation flags, the
 * Prohibition list, the turn counter and phase, the battle-stage machine, the card command menu
 * (struct CardMenu) and the duel prompt. Each player has 11 field zones (struct DuelZone, 0x94 bytes)
 * and five 80-card piles; every card in them is a 32-bit word (struct DuelCard).
 *
 * Field names and offsets were merged from how every unit uses this memory (the 2026-10 naming pass).
 * The STATIC_ASSERTs below check sizes and byte offsets; the bitfield positions were measured with
 * old_agbcc (designated-initializer probes). agbcc packs bitfields contiguously: a bitfield starts right
 * after the previous one even when it crosses a byte, halfword or word boundary.
 *
 * A bitfield's declared type is its access width (ldrb/ldrh/ldr) and changes the generated code. The
 * containers here are the ones the matched code uses most; a unit that needs another width, a whole-byte
 * view of a flag group or an address-suffixed alias keeps its local view, with a comment (see "Alias
 * symbols" below).
 *
 * Locations: many helpers take (player, zone) or a packed location (zone << 8) | player (DUEL_LOC in
 * constants/duel.h). Card IDs index the card tables; card numbers are the printed numbers, mapped with
 * gCardIdToNumber / gCardNumberToId (include/card_data.h).
 */

#include "global.h"
#include "constants/duel.h"

/* A card in a zone or pile: one 32-bit word. id 0 is an empty slot. Bits 14-17 are the summon status
 * bits that enum ZoneStatusFlag (bits 0-3) sets and clears. */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:1;                    /* bit 13: DuelCmd_SummonToken sets it like owner (controller? hypothesis) */
    u32 unk14:1;                    /* bit 14: set for tribute, flip, Toon and special summons and tokens, not
                                     * for a plain Normal Summon */
    u32 normalSummoned:1;           /* bit 15: Normal Summoned (not Set); cleared when the summon is negated */
    u32 specialSummoned:1;          /* bit 16: Special Summoned (summon actions 4-6, tokens, Parasite Paracide) */
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 graverobbed:1;              /* bit 18: taken with Graverobber; cleared when it leaves the field */
    u32 unk19:1;
    u32 isFusionMaterial:1;         /* bit 20: one of Polymerization's materials */
    u32 destroyedInBattle:1;        /* bit 21: set before a battle-destroyed monster enters the graveyard */
    u32 destroyedByOpponent:1;      /* bit 22 */
    u32 flag23:1;                   /* bit 23: set by DUEL_CMD_MARK_GRAVEYARD_CARD; reader unknown */
    u32 pendingEquip:1;             /* bit 24: graveyard card waiting to be equipped at end of turn (low) */
    u32 equipZone:3;                /* bits 25-27: monster zone that pendingEquip card goes to (low) */
    u32 pendingOpponentSummon:1;    /* bit 28: graveyard card the opponent may Special Summon at end of turn */
    u32 unk29:3;
};

/* The status bits of a card word as single-bit u8 fields: ClearCardStatusFlags clears them one byte
 * access at a time, which struct DuelCard (u32 container) cannot express. It is applied to a whole zone,
 * whose card word comes first, hence the zone size. Names and bits as in struct DuelCard. */
struct DuelCardStatusBytes {
    u8 cardIdLow;                   /* +0x00: card word bits 0-7 */
    u8 cardWordBits8to13:6;         /* +0x01: id (high bits), owner, unk13 */
    u8 unk14:1;                     /* +0x01 bit 6 = card bit 14 */
    u8 normalSummoned:1;            /* bit 15 */
    u8 specialSummoned:1;           /* +0x02 bit 0 = card bit 16 */
    u8 planted:1;                   /* bit 17 */
    u8 graverobbed:1;               /* bit 18 */
    u8 unk19:1;                     /* bit 19 (ClearCardStatusFlags keeps it) */
    u8 isFusionMaterial:1;          /* bit 20 */
    u8 destroyedInBattle:1;         /* bit 21 */
    u8 destroyedByOpponent:1;       /* bit 22 (kept) */
    u8 flag23:1;                    /* bit 23 */
    u8 pendingEquip:1;              /* +0x03 bit 0 = card bit 24 */
    u8 equipZone:3;                 /* bits 25-27 (kept) */
    u8 pendingOpponentSummon:1;     /* bit 28 */
    u8 unk29:3;
    u8 restOfZone[0x94 - 4];
};

/* A card location on the duel screen, packed in 16 bits (padded to 4 bytes): the endpoints of the card
 * move animations (gDuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;                       /* +0x02: padding, copied with the word */
};

/* One field zone (0x94 bytes): the card, its position and per-card state, and the links of other cards
 * that affect it (equips, continuous effects, stat modifiers). Zones 0-4 hold monsters, 5-9 spells and
 * traps, 10 the Field Magic (enum DuelZoneIndex). */
struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed (replay check) */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5: turns a face-up card has been active (Swords of Revealing
                                     * Light, Germ Infection, Stim-Pack) */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9: turns until the monster is destroyed (Zone Eater 5,
                                     * Steel Scorpion 3); u16 container */
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position (DuelCmd_SetPositionLocked, tokens) */
    u8 unk7_3:1;                    /* +0x07 bit 3: blocks AiCanChangePosition; meaning unknown */
    u8 unk7_4:1;                    /* +0x07 bit 4: CanMonsterAttack refuses; writer unknown */
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet (Ameba, Griggle) */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6: ZONE_STATUS_MONSTER_REBORN; Call of the Dark destroys these */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7: ZONE_STATUS_FROM_GRAVEYARD (exact meaning: hypothesis) */
    u8 levelCheckDone:1;            /* +0x08 bit 0 */
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one, or a value / card ID,
                                     * depending on the kind */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind, high byte stack count / value */
    u16 numLinks;                   /* +0x8A: entries in links / linkKinds */
    u8 unk8C_0:1;                   /* +0x8C: battle flags */
    u8 destroyAfterBattle:1;        /* +0x8C bit 1: non-monster placed in a monster zone (Magical Hats) */
    u32 returnAfterBattle:1;        /* +0x8C bit 2: borrowed by Magic-Arm Shield; u32 container */
    u8 cannotAttackNextTurn:1;      /* +0x8C bit 3: Electric Lizard; moved into cannotAttack */
    u8 cannotAttack:1;              /* +0x8C bit 4: DuelCmd_SetCannotAttack, Toon monsters on the summon turn */
    u8 atkHalved:1;                 /* +0x8C bit 5: Riryoku halving until end of turn (DuelCmd_HalveAttack) */
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;                  /* +0x90 bits 6-9: set to 0xF by PlaceSpellTrapCard; meaning unknown */
    u32 canActivate:1;              /* +0x91 bit 2: a set card that may be activated */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9: value chosen when the card resolved (DNA Surgery type,
                                     * an ATK/DEF choice, an attribute) */
    u32 unk92_2:14;
};

/* One player's side of the duel (0xD64 bytes; gDuelPlayers = gDuel.players). */
struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[]; also the slot the next card lands in */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006: entries in banished[] and banishedInfo[] */
    u8 deckOut:1;                   /* +0x007 bit 0: tried to draw from an empty deck */
    u8 exodiaWin:1;                 /* +0x007 bit 1: the five Exodia pieces are in the hand */
    u8 destinyBoardWin:1;           /* +0x007 bit 2: Destiny Board and its four letters are on the field */
    u8 noNormalSummon:1;            /* +0x007 bit 3: DuelCmd_SetSummonLocks */
    u8 noSpecialSummon:1;           /* +0x007 bit 4: DuelCmd_SetSummonLocks */
    u8 positionChangeLocked:1;      /* +0x007 bit 5: blocks the DEF/ATK position and Flip commands */
    u8 magicTrapLockTurns:2;        /* +0x007 bits 6-7: nonzero blocks Magic/Trap activation */
    u8 noBattleDamage:1;            /* +0x008 bit 0 */
    u8 battleProtected:1;           /* +0x008 bit 1 */
    u8 unk8_2:1;
    u8 insectQueenWonBattle:1;      /* +0x008 bit 3: set by BattleStage_DestroyMonsters */
    u8 normalSummonUsed:1;          /* +0x008 bit 4: the Normal Summon of this turn is done */
    u8 summonedThisTurn:1;          /* +0x008 bit 5: a summon/set action was started this turn */
    u8 extraBattlePhase:1;          /* +0x008 bit 6: a second Battle Phase is allowed */
    u8 unk8_7:1;
    u8 handRevealed:1;              /* +0x009 bit 0: forces IsHandRevealed; writer unknown */
    u8 skipStandbyPhase:1;          /* +0x009 bit 1: consumed by DuelPhase_Standby */
    u8 skipDrawPhase:1;             /* +0x009 bit 2: consumed by DuelPhase_Draw */
    u8 skipTurn:1;                  /* +0x009 bit 3: consumed by DuelPhase_TurnStart */
    u8 battlePhaseDone:1;           /* +0x009 bit 4: set when the Battle Phase ends */
    u8 magicTrapActivatedThisTurn:1;/* +0x009 bit 5: a Magic/Trap was activated from zones 5-10 */
    u32 lockedZones:10;             /* +0x009 bit 6 .. +0x00A bit 7: one bit per zone 0-9, read by
                                     * IsSpellTrapZoneFree; no writer found */
    u8 crushCardTurns:3;            /* +0x00B bits 0-2: turns left of Crush Card's draw check */
    u8 monsterSentToGraveThisTurn:1;/* +0x00B bit 3: Last Will condition */
    u32 removedMask:5;              /* +0x00B bit 4 .. +0x00C bit 0: monster zones held for a monster banished
                                     * until the End Phase (BANISH_UNTIL_END_PHASE); u32 container, read as
                                     * two byte loads */
    u8 delayedSummonCount:3;        /* +0x00C bits 1-3 */
    u8 destroyedTriggerPending:1;   /* +0x00C bit 4: DuelPhase_End steps 21/22 */
    u8 banishCostFromField:1;       /* +0x00C bit 5: graveyard-banish summon costs are paid from the field */
    u8 unkC_6:2;
    u8 unkD;
    u16 lpPaid[11];                 /* +0x00E: per zone, LP paid for the card there (Toon World), paid back
                                     * when it leaves the field */
    u16 attackableMask;             /* +0x024: monster zones that may attack (BuildAttackableMask) */
    u16 attackedMask;               /* +0x026: monster zones that have attacked this turn */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84: removed from play (area 15) */
    u16 banishedInfo[80];           /* +0xCC4: parallel to banished[]: low byte enum BanishKind, high byte zone */
};

/* The card command menu (gDuel.cardMenu, 0xC bytes). CardMenu_Update runs it while open; the human and
 * AI paths set confirmed and the chosen command, player, area and index, and CardMenu_Execute acts on them.
 * The bitfields cross byte and halfword boundaries; the container types are the ones the code uses. */
struct CardMenu {
    u16 open:1;                     /* bit 0: menu open, the caller runs CardMenu_Update */
    u16 confirmed:1;                /* bit 1: a command was chosen (the AI sets it directly) */
    u16 command:4;                  /* bits 2-5: enum CardMenuCommand */
    u16 slide:4;                    /* bits 6-9: slide/zoom animation step 0-8 */
    u32 available:16;               /* bits 10-25: enum CardMenuCommandMask bits */
    u32 state:8;                    /* bits 26-33: CardMenu_Update state */
    u32 step:8;                     /* bits 34-41: step of the command handler; reset before confirming */
    u8 summonSeq:4;                 /* bits 42-45: which named Tribute is being picked (Gate Guardian:
                                     * Sanga, Kazejin, Suijin; Valkyrion: Alpha, Beta, Gamma); u8 container */
    u32 tributeSources:4;           /* bits 46-49: where it may come from: bit 0 hand, bit 1 field */
    u16 timer:7;                    /* bits 50-56: pulse timer of the selected icon */
    u16 player:1;                   /* bit 57: player of the confirmed command */
    u32 area:7;                     /* bits 58-64: enum DuelArea of the cursor at confirm */
    u32 index:8;                    /* bits 65-72: zone index (field) or hand index (hand) */
    u32 placeZone:8;                /* bits 73-80: spell/trap zone chosen by CardMenu_PlaySpellTrapFromHand */
    u32 unk0A_1:15;
};

/* gDuel (0x1B78 bytes at 0x020192E0). */
struct DuelState {
    u16 serial;                     /* +0x0000: placement serial, stamped into DuelZone.serial */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */

    /* Rule flags at +0x1ACC: field background, duel over, and the negation flags that duel commands
     * 0x15-0x1C set. Some units read +0x1ACC as one u16 (bit 13 statChangesReversed, bit 14
     * atkDefSwapped) or test bits 8-9 together (equip links have no effect while either is set); they
     * keep a local view. */
    u8 fieldBackground:4;           /* +0x1ACC bits 0-3: field background 0-14 (0 none, else GetFieldMagicIndex) */
    u8 duelOver:1;                  /* +0x1ACC bit 4: set by Duel_CheckWin */
    u8 unk1ACC_5:1;
    u8 magicNegated:1;              /* +0x1ACC bit 6: Imperial Order */
    u8 trapsNegated:1;              /* +0x1ACC bit 7: Jinzo / Royal Decree */
    u8 equipMagicNegated:1;         /* +0x1ACD bit 0: not cleared at turn end */
    u8 equipMagicNegatedThisTurn:1; /* +0x1ACD bit 1: Armored Glass */
    u8 fieldMagicNegatedThisTurn:1; /* +0x1ACD bit 2: World Suppression */
    u8 contMagicNegatedThisTurn:1;  /* +0x1ACD bit 3: Mystic Probe */
    u8 contTrapNegatedThisTurn:1;   /* +0x1ACD bit 4: Metal Detector */
    u8 statChangesReversed:1;       /* +0x1ACD bit 5: Reverse Trap: stat modifiers are subtracted */
    u8 atkDefSwapped:1;             /* +0x1ACD bit 6: Shield & Sword */
    u32 prohibitionCount:4;         /* +0x1ACD bit 7 .. +0x1ACE bit 2: entries in the Prohibition list */
    u32 unk1ACE_3:13;
    u16 prohibitionZones[16];       /* +0x1AD0: DUEL_LOC of each active Prohibition */
    u16 prohibitedCards[16];        /* +0x1AF0: card ID that Prohibition declared */

    u16 turnCount;                  /* +0x1B10: 0 on the first turn (no Battle Phase then) */
    u8 bgmOn:1;                     /* +0x1B12 bit 0: duel BGM allowed (cleared for the win scenes) */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5: no acknowledgement from the link partner in 120 frames */
    u8 result:2;                    /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13_0:1;                 /* +0x1B13 bit 0: cleared by DuelPhase_ShowResult */
    u8 unk1B13_1:7;

    /* Battle-stage machine (BattlePhase_Run). */
    u16 unk1B14_0:1;
    u16 interruptActive:1;          /* +0x1B14 bit 1: the non-turn link player is acting ("Just a moment") */
    u16 unk1B14_2:7;
    u32 battleStage:8;              /* +0x1B15 bit 1 .. +0x1B16 bit 0: enum BattleStage */
    u16 battleStep:8;               /* +0x1B16 bits 1-8: step inside the stage */
    u16 battleArg0:8;               /* +0x1B17 bit 1 .. +0x1B18 bit 0: stage scratch (a zone or player) */
    u16 battleArg1:8;               /* +0x1B18 bit 1 .. +0x1B19 bit 0: stage scratch (a graveyard index) */
    u16 unk1B19_1:7;
    u8 unk1B1A[2];
    struct DuelCard battleCard;     /* +0x1B1C: graveyard card the opponent Special Summons at end of battle */

    /* Duel-step handlers (DuelMainStep). */
    u8 phaseStep;                   /* +0x1B20: step of the current duel-step handler */
    u8 phaseCounter;                /* +0x1B21: zone cursor / counter of the phase handlers */
    u8 phaseSubStep;                /* +0x1B22: secondary step or index of the phase handlers */
    u8 phaseSubCounter;             /* +0x1B23: zone cursor of EndPhase_TransferMushroomMan2 */
    u8 unk1B24[2];
    u8 endTurnAfterBattle:1;        /* +0x1B26 bit 0: 'Complete your turn' chosen in the Battle Phase menu */
    u8 unk1B26_1:7;
    u8 unk1B27;

    /* Card command menu. */
    u16 cardMenuCard;               /* +0x1B28: card ID under the cursor when the command was confirmed */
    u16 summonTributes;             /* +0x1B2A: picked Tribute zones: low / high byte one zone each */
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[8];
    u8 cmdQueueStep;                /* +0x1B40: DuelCmdQueue_Run step */
    u8 unk1B41[2];
    u8 unk1B43;                     /* +0x1B43: only cleared (by dead code) */
    u8 unk1B44;                     /* +0x1B44: only cleared (by dead code) */
    u8 unk1B45[11];

    /* Duel prompt (DuelPrompt_Post / DuelPrompt_PostData). */
    u8 promptLinked:1;              /* +0x1B50 bit 0: the prompt is mirrored over the link */
    u8 promptActive:1;              /* +0x1B50 bit 1: a prompt is pending */
    u8 promptPlayer:1;              /* +0x1B50 bit 2: player who answers */
    u8 unk1B50_3:1;
    u16 promptKind:6;               /* +0x1B50 bits 4-9: enum DuelPromptKind */
    u16 unk1B51_2:6;
    u16 promptArgs[8];              /* +0x1B52: [0] argument, [1] value (DuelPrompt_Post); all 8 for PostData;
                                     * forwarded over the link as one 16-byte block */
    u8 promptStep;                  /* +0x1B62: step of the running prompt handler */
    u8 unk1B63;
    u16 promptResult;               /* +0x1B64: the answer (a hand slot, a card ID); the link partner's 16-byte
                                     * result is copied here */
    u8 unk1B66[0x1B78 - 0x1B66];
};

/* The word at gDuel+0x1B14 read as one u32 (battleStage and battleStep in a u32 container): the view
 * duel_stat_queries uses for the battle-stage tests. Same bits as the DuelState members of that name. */
struct DuelStateBattleWord {
    u32 unk0_0:9;                   /* +0x1B14 bits 0-8: unk1B14_0, interruptActive, unk1B14_2 */
    u32 battleStage:8;              /* bits 9-16: enum BattleStage */
    u32 battleStep:8;               /* bits 17-24 */
    u32 unk3_1:7;                   /* bits 25-31: low bits of battleArg0 */
};

/* gDuelZones (0x0201930C = &gDuel.players[0].zones): the zones of each player with the player stride, so
 * gDuelZones[player].zones[zone] is gDuelPlayers[player].zones[zone]. Most units use this view. */
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride (piles, next player's header) */
};

/* Effective stats of the card in a zone (GetZoneCardStats), after field, equip and link modifiers. */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

/* Layout checks (byte offsets; the bitfield positions were checked with designated-initializer probes). */
STATIC_ASSERT(sizeof(struct DuelCard) == 4, DuelCardSize);
STATIC_ASSERT(sizeof(struct DuelCardStatusBytes) == 0x94, DuelCardStatusBytesSize);
STATIC_ASSERT(sizeof(struct DuelLoc) == 4, DuelLocSize);
STATIC_ASSERT(sizeof(struct DuelZone) == 0x94, DuelZoneSize);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, serial) == 0x04, DuelZoneSerial);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, unk9) == 0x09, DuelZoneUnk9);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, links) == 0x0A, DuelZoneLinks);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, linkKinds) == 0x4A, DuelZoneLinkKinds);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, numLinks) == 0x8A, DuelZoneNumLinks);
STATIC_ASSERT(OFFSET_OF(struct DuelZone, unk8D) == 0x8D, DuelZoneUnk8D);
STATIC_ASSERT(sizeof(struct DuelPlayer) == 0xD64, DuelPlayerSize);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, unkD) == 0x00D, DuelPlayerUnkD);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, lpPaid) == 0x00E, DuelPlayerLpPaid);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, attackedMask) == 0x026, DuelPlayerAttackedMask);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, zones) == 0x028, DuelPlayerZones);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, hand) == 0x684, DuelPlayerHand);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, deck) == 0x7C4, DuelPlayerDeck);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, graveyard) == 0x904, DuelPlayerGraveyard);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, fusionDeck) == 0xA44, DuelPlayerFusionDeck);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, banished) == 0xB84, DuelPlayerBanished);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, banishedInfo) == 0xCC4, DuelPlayerBanishedInfo);
STATIC_ASSERT(sizeof(struct CardMenu) == 0xC, CardMenuSize);
STATIC_ASSERT(sizeof(struct DuelState) == 0x1B78, DuelStateSize);
STATIC_ASSERT(OFFSET_OF(struct DuelState, players) == 0x0004, DuelStatePlayers);
STATIC_ASSERT(OFFSET_OF(struct DuelState, prohibitionZones) == 0x1AD0, DuelStateProhibitionZones);
STATIC_ASSERT(OFFSET_OF(struct DuelState, prohibitedCards) == 0x1AF0, DuelStateProhibitedCards);
STATIC_ASSERT(OFFSET_OF(struct DuelState, turnCount) == 0x1B10, DuelStateTurnCount);
STATIC_ASSERT(OFFSET_OF(struct DuelState, unk1B1A) == 0x1B1A, DuelStateUnk1B1A);
STATIC_ASSERT(OFFSET_OF(struct DuelState, battleCard) == 0x1B1C, DuelStateBattleCard);
STATIC_ASSERT(OFFSET_OF(struct DuelState, phaseStep) == 0x1B20, DuelStatePhaseStep);
STATIC_ASSERT(OFFSET_OF(struct DuelState, phaseSubCounter) == 0x1B23, DuelStatePhaseSubCounter);
STATIC_ASSERT(OFFSET_OF(struct DuelState, unk1B27) == 0x1B27, DuelStateUnk1B27);
STATIC_ASSERT(OFFSET_OF(struct DuelState, cardMenuCard) == 0x1B28, DuelStateCardMenuCard);
STATIC_ASSERT(OFFSET_OF(struct DuelState, summonTributes) == 0x1B2A, DuelStateSummonTributes);
STATIC_ASSERT(OFFSET_OF(struct DuelState, cardMenu) == 0x1B2C, DuelStateCardMenu);
STATIC_ASSERT(OFFSET_OF(struct DuelState, cmdQueueStep) == 0x1B40, DuelStateCmdQueueStep);
STATIC_ASSERT(OFFSET_OF(struct DuelState, unk1B43) == 0x1B43, DuelStateUnk1B43);
STATIC_ASSERT(OFFSET_OF(struct DuelState, unk1B44) == 0x1B44, DuelStateUnk1B44);
STATIC_ASSERT(OFFSET_OF(struct DuelState, promptArgs) == 0x1B52, DuelStatePromptArgs);
STATIC_ASSERT(OFFSET_OF(struct DuelState, promptStep) == 0x1B62, DuelStatePromptStep);
STATIC_ASSERT(OFFSET_OF(struct DuelState, promptResult) == 0x1B64, DuelStatePromptResult);
STATIC_ASSERT(sizeof(struct DuelStateBattleWord) == 4, DuelStateBattleWordSize);
STATIC_ASSERT(sizeof(struct DuelZonesPlayer) == 0xD64, DuelZonesPlayerSize);
STATIC_ASSERT(sizeof(struct ZoneCardStats) == 0xC, ZoneCardStatsSize);
STATIC_ASSERT(OFFSET_OF(struct ZoneCardStats, atk) == 0x4, ZoneCardStatsAtk);

/*
 * RAM. gDuel, gDuelPlayers and gDuelZones are three symbols for one object (0x020192E0, +0x4, +0x2C).
 * The other symbols below are further fixed addresses inside it that the units load from their literal
 * pools; they are separate linker symbols, kept because the matched code refers to them by name.
 */
extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelZone gDuelSpellTrapZones;     /* 0x020195F0 = gDuelPlayers[0].zones[ZONE_SPELL_0] */
extern struct DuelZone gDuelFieldZone;          /* 0x020198D4 = gDuelPlayers[0].zones[ZONE_FIELD] */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */
extern struct DuelCard gDuelDecks[];            /* 0x02019AA8 = gDuelPlayers[0].deck */
extern struct DuelCard gDuelGraveyards[];       /* 0x02019BE8 = gDuelPlayers[0].graveyard */
extern struct DuelCard gDuelFusionDecks[];      /* 0x02019D28 = gDuelPlayers[0].fusionDeck */
extern struct DuelCard gDuelBanished[];         /* 0x02019E68 = gDuelPlayers[0].banished */
extern u16 gDuelBanishedInfo[];                 /* 0x02019FA8 = gDuelPlayers[0].banishedInfo */
extern struct DuelZone gDuelZonesP1[11];        /* 0x0201A070 = gDuelPlayers[1].zones */
extern struct DuelCard gDuelHandP1[80];         /* 0x0201A6CC = gDuelPlayers[1].hand */
extern struct DuelCard gDuelDeckP1[80];         /* 0x0201A80C = gDuelPlayers[1].deck */

/*
 * Alias symbols. Some units reach single fields of gDuel through address-suffixed symbols (declared
 * locally, with the type the matched code needs): gUnk_0201A04A = gDuelPlayers[1].handCount,
 * gUnk_0201ADAC / gUnk_0201ADAD = the rule flags at +0x1ACC / +0x1ACD, gUnk_0201ADF2 = +0x1B12
 * (turnPlayer), gUnk_0201ADF6 = battleStep (+0x1B16), gUnk_0201AE0C = cardMenu, gUnk_0201AE42 =
 * promptStep, gUnk_0201AE44 = promptResult. Keep the access form a unit uses; only rename or annotate.
 */

/* ROM data shared by several units. */
extern const u16 gBounceScaleCurve[];           /* 0x081A43E4: [16] OBJ affine scale per step of a card
                                                 * move: shrinks to half size with a small rebound */

/* --- Card identity (card IDs unless the parameter says cardNo) --- */

/* 1 if two card IDs are the same card: alternate art (number + 2000) folded, plus three equivalent pairs. */
u32 IsSameCardName(u32 cardId1, u32 cardId2);
/* 1 if card number cardNo is one of the four Toon effect monsters (Toon Alligator is a Normal Monster). */
u32 IsToonMonster(u16 cardNo);
/* 1 if card number cardNo has a flip effect that applies (inBattle: flipped face up by an attack). */
u32 HasFlipEffect(u16 cardNo, int inBattle);
/* 1 if the card is an Effect Monster (kind 1, or one of five card numbers listed by hand). */
u32 IsEffectMonster(u16 cardId);
/* 1 if the monster cannot be Normal Summoned or Set (Fusion, Ritual and some effect monsters). */
u32 IsSpecialSummonOnly(u16 cardId);
/* 1 if the card is a Fusion monster. */
u32 IsFusionMonster(u16 cardId);
/* 1-14 for the Field Magic cards (the field background index), else 0. */
u32 GetFieldMagicIndex(u16 cardNo);

/* --- Card words and life points --- */

/* *dst = *src for a duel card word. */
void CopyDuelCard(u32 *dst, u32 *src);
/* Swap two duel card words. */
void SwapDuelCards(u32 *a, u32 *b);
/* Clear the transient status bits 14-18, 20, 21, 23, 24 and 28 of a card word. */
void ClearCardStatusFlags(struct DuelCardStatusBytes *card);
/* ClearCardStatusFlags on the card in (player, zone). */
void ClearZoneCardStatusFlags(u32 player, u32 zone);
/* players[player & 1].lifePoints -= amount, clamped at 0. */
void SubtractLifePoints(struct DuelPlayer *players, u32 player, s32 amount);

/* --- Placing and removing field cards (immediate; the animated versions are in duel_actions.h) --- */

/* Clear the zone, put the card there and stamp the serial; Toon monsters cannot attack this turn. */
void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 defense, u16 faceUp);
/* Put the card into spell/trap zone 5 + slot (or the field zone for a Field Magic) and set its flags. */
void PlaceSpellTrapCard(int player, int slot, struct DuelCard *card, u16 faceUp);
/* Clear the 0x94-byte zone. */
void ClearZone(int player, int zone);
/* Send the zone's card to the graveyard (or banish it) and clear the zone; links are not removed. */
void SendZoneCardToGraveyardOrBanished(int player, int zone, u16 banish);
/* Send the zone's card to the graveyard, remove the links to it and clear the zone. */
void SendZoneCardToGraveyard(int player, int zone);
/* Banish the zone's card and clear the zone (no link cleanup). */
void BanishZoneCard(int player, int zone);
/* Return the zone's card to its owner's hand (Fusion monsters to the fusion deck) and clear the zone. */
void ReturnZoneCardToHand(int player, int zone);
/* Return the zone's card to the top of its owner's deck (Fusion monsters to the fusion deck). */
void ReturnZoneCardToDeck(int player, int zone);

/* --- Deck and fusion deck --- */

/* Put the card on top of the deck (deck[0]), shifting the others down. */
void AddCardToDeckTop(int player, struct DuelCard *card);
/* Append the card at the bottom of the deck. */
void AddCardToDeckBottom(int player, struct DuelCard *card);
/* Append the card to the fusion deck. */
void AddCardToFusionDeck(int player, struct DuelCard *card);
/* Move the first copy of cardNo below the run of copies already on top to the top (unused, buggy). */
void MoveDeckCardNumberToTop(int player, u16 cardNo);
/* Shuffle: passes * deckCount random swaps. */
void ShuffleDeck(int player, int passes);
/* Remove deck[index] into *out and close the gap; 0 if index >= deckCount. */
u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out);
/* Remove the first deck entry equal to *card; 1 if found. */
int RemoveCardFromDeck(int player, struct DuelCard *card);
/* Remove the first deck card with card number cardNo into *out; 1 if found. */
u16 TakeDeckCardByNumber(int player, u16 cardNo, struct DuelCard *out);
/* Move the top deck card to the hand (no animation, no triggers; compare DrawCards). */
void DuelDrawCard(int player);
/* Remove the first fusion-deck entry equal to *card; 1 if found. */
int RemoveCardFromFusionDeck(int player, struct DuelCard *card);
/* Index of the first deck card with that number among the first min(deckCount, limit), or -1. */
int FindDeckCardByNumber(int player, u16 number, int limit);
/* Index of the first fusion-deck card with that number, or -1. */
int FindFusionDeckCardByNumber(int player, u16 number);
/* Deck building: put card number `number` on top of the deck (a Fusion monster: of the fusion deck). */
void AddCardNumberToDeckTop(int player, u16 number);
/* Remove every deck card with that number. */
void RemoveAllDeckCardsByNumber(int player, u16 number);
/* Number of deck cards with that number. */
int CountDeckCardsByNumber(int player, u16 number);
/* Remove all copies of every card whose count exceeds GetCardCopyLimit. */
void RemoveOverLimitDeckCards(int player);

/* --- Graveyard and banished --- */

/* Append the card to its owner's graveyard (not empty cards or tokens). */
void AddCardToGraveyard(struct DuelCard *card);
/* Append the card to its owner's banished pile (BANISH_NORMAL). */
void AddCardToBanished(struct DuelCard *card);
/* Banish the card until the End Phase (BANISH_UNTIL_END_PHASE); its zone stays held in removedMask. */
void AddCardToBanishedTemporarily(struct DuelCard *card, int zone);
/* Bring the card banished until the End Phase back to its zone. */
void ReturnTemporarilyBanishedCard(int player, int zone);
/* Append the card to its owner's banished pile face down (BANISH_FACE_DOWN). */
void AddCardToBanishedFaceDown(struct DuelCard *card);
/* Remove graveyard[index] and close the gap; 1, or 0 if index >= graveCount. */
int RemoveGraveyardCardAt(int player, int index);
/* Remove graveyard[index] into *out; 1, or 0 if index >= graveCount. */
int TakeGraveyardCardAt(int player, int index, struct DuelCard *out);
/* Remove the first graveyard entry equal to *card; 1 if found. */
u16 RemoveCardFromGraveyard(int player, struct DuelCard *card);
/* Remove the first graveyard entry with that card ID; 1 if found. */
u16 RemoveGraveyardCardById(int player, u16 cardId);
/* Copy the first graveyard entry with that card ID to *out (the pile is unchanged); 1 if found. */
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out);
/* 1 if some graveyard entry equals *card. */
int IsCardInGraveyard(int player, struct DuelCard *card);
/* Number of graveyard cards with that card number. */
int CountGraveyardCardsByNumber(int player, u16 cardNo);
/* Number of graveyard cards of that card type. */
int CountGraveyardCardsOfType(int player, u16 type);
/* Number of monsters in the graveyard. */
int CountGraveyardMonsters(int player);
/* Remove banished[index] and its banishedInfo entry; 1, or 0 if out of range. */
int RemoveBanishedCardAt(int player, int index);
/* Remove the first banished entry equal to *card; 1 if found. */
u16 RemoveCardFromBanished(int player, struct DuelCard *card);

/* --- Hand --- */

/* Append the card to the hand (Fusion monsters go to the fusion deck; not empty cards or tokens). */
void AddCardToHand(int player, struct DuelCard *card);
/* Clear hand[index].id, leaving a hole for CompactHand (unused). */
void ClearHandCardAt(int player, int index);
/* Remove the first hand entry equal to *card; 1 if found. */
int RemoveCardFromHand(int player, struct DuelCard *card);
/* Delete the empty (id 0) hand entries, keeping the order. */
void CompactHand(int player);
/* Index of the first Trap card in the hand, or -1. */
int FindTrapInHand(int player);
/* Index of the first Magic card in the hand, or -1. */
int FindMagicInHand(int player);
/* Index of the first Magic card in the hand that is not a Field Magic, or -1. */
int FindNonFieldMagicInHand(int player);
/* Number of hand cards with that card number. */
int CountHandCardsByNumber(int player, u16 cardNo);
/* Index of the first hand card with that card number, or -1. */
int FindHandCardByNumber(int player, u16 cardNo);
/* Number of monsters in the hand. */
int CountHandMonsters(int player);
/* 1 if the player's hand is shown (handRevealed, The Eye of Truth, Ceremonial Bell, ...). */
int IsHandRevealed(int player);

/* --- Field queries ("active" = face up and not disabled) --- */

/* First zone 0-10 except skipZone with a face-up card of that number, or -1. */
int FindFaceUpCardOnField(int player, u16 cardNo, int skipZone);
/* Byte-identical copy of FindFaceUpCardOnField (its callers pass only two arguments). */
int FindFaceUpCardOnField2(int player, u16 cardNo, int skipZone);
/* Number of active copies of the card in zones 0-10 except skipZone. */
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
/* Number of active copies of the card in zones 0-10: is the card in effect for this player. */
int CountActiveCardsOnField(int player, u16 cardNo);
/* CountActiveCardsOnField with the players array as a parameter. */
int CountActiveCardsOnFieldIn(struct DuelPlayer *players, int player, u16 cardNo);
/* Same result as CountActiveCardsOnField (CountActiveCardsOnFieldIn on gDuelPlayers). */
int CountActiveCardsOnField2(int player, u16 cardNo);
/* Number of not-disabled copies of the card in spell/trap zones 5-9 (face up or down). */
int CountEnabledSpellTrapCards(int player, u16 cardNo);
/* Number of face-up monsters whose effective type is type. */
int CountFaceUpMonstersOfType(int player, u16 type);
/* Number of face-up monsters whose effective attribute is attr. */
int CountFaceUpMonstersOfAttribute(int player, u16 attr);
/* 1 if Toon World is face up in the player's spell/trap zones. */
int HasFaceUpToonWorld(int player);
/* 1 if one of the player's face-up monsters is a Toon. */
int HasFaceUpToonMonster(int player);
/* 0 if a face-up monster is LIGHT, DARK or WIND, else 1. */
int HasNoFaceUpLightDarkWindMonster(int player);
/* Number of face-up monsters with that card number. */
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
/* First monster zone with a face-up card of that number, or -1. */
int FindFaceUpMonsterByNumber(int player, u16 cardNo);
/* Number of monsters with that card number, face up or down. */
int CountMonstersByNumber(int player, u16 cardNo);
/* Number of face-up monsters with that card number in that position (defense 1 / attack 0). */
int CountFaceUpMonstersByNumberInPosition(int player, u16 cardNo, u16 defense);
/* Number of occupied monster zones. */
int CountMonsters(int player);
/* Number of monsters, optionally only face-up ones and/or only those in attack position. */
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
/* Number of occupied spell/trap zones 5-9. */
int CountSpellTraps(int player);
/* Number of spell/trap cards by face (both 0 = all), optionally including the field zone. */
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);
/* Number of face-up spell/trap cards of that card type (Trap or Magic). */
int CountFaceUpSpellTrapsOfType(int player, u16 type);
/* Number of set cards with that number that may be activated (players array as a parameter). */
int CountActivatableSetCardsIn(struct DuelPlayer *players, int player, u16 cardNo);
/* CountActivatableSetCardsIn on gDuelPlayers. */
int CountActivatableSetCards(int player, int cardNoWord);
/* Number of other face-up monsters on both fields with the same name as the card in (player, zone). */
int CountOtherFaceUpSameNameMonsters(int player, int zone);
/* Number of active Aqua Chorus if the monster in (player, zone) shares its name with another, else 0. */
int CountAquaChorusBoosts(int player, int zone);
/* Card number of the face-up Field Magic (player 0's first), or 0. */
int GetFaceUpFieldMagicNumber(void);
/* 1 if an active Prohibition declares the same card name. */
u32 IsCardProhibited(u16 cardId);

/* --- Free zones and tributes --- */

/* 1 if the monster zone is empty and not held for a banished monster (removedMask). */
u32 IsMonsterZoneFree(int player, int zone);
/* Number of free monster zones. */
int CountFreeMonsterZones(int player);
/* First free monster zone, or -1. */
int FindFreeMonsterZone(int player);
/* Nonzero if the zone holds a monster that may be tributed: not a token, and card 1418 is not face up. */
u16 IsTributableMonster(int player, int zone);
/* Number of tributable monsters, not counting excludeZone (-1 = none). */
int CountTributableMonsters(int player, int excludeZone);
/* Nonzero if the spell/trap zone is empty and not locked (lockedZones). */
u16 IsSpellTrapZoneFree(int player, int zone);
/* First free spell/trap zone 5-9, or -1. */
int FindFreeSpellTrapZone(int player);
/* 1 if the spell/trap card has somewhere to go (the field zone for a Field Magic, else a free zone 5-9). */
int CanPlaceSpellTrapCard(int player, u16 cardId);

/* --- Zone links (equips, continuous effects, stat modifiers) --- */

/* Add a link of that kind to zone loc (DUEL_LOC); an existing link to the same target is stacked. */
void AddZoneLink(u16 loc, u16 target, u16 kind);
/* Remove the first link of that kind (and, for most kinds, that target) from zone loc. */
void RemoveZoneLink(u16 loc, u16 target, u16 kind);
/* Remove link idx of the zone, shifting the later ones down. */
void RemoveZoneLinkAt(int player, int zone, int idx);
/* Remove the links of both players' monsters that point at (player, zone) (kinds 1, 2, 5, 7, 10). */
void RemoveLinksToZone(int player, int zone);
/* DUEL_LOC of the first face-up monster with a link (kinds 1, 2, 5, 7, 10) to (player, zone), or 0xFFFF. */
u16 FindMonsterWithLinkTo(int player, int zone);
/* DUEL_LOC of the monster absorbed by (player, zone) (first ZONE_LINK_ABSORBED link), or 0xFFFF. */
u16 FindAbsorbedMonsterLink(int player, int zone);
/* Remove the type/attribute-restricted equips whose condition the monster no longer meets. */
void DestroyInvalidEquips(int player, int zone);
/* Number of equip links on (player, zone), with optional Magic / equip-subtype filters. */
int CountZoneEquips(int player, int zone, u16 requireMagic, u16 requireEquipSubtype);
/* Number of links on (player, zone) that come from card number cardNo. */
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
/* CountZoneLinksFromCard, skipping links whose source zone is disabled. */
int CountActiveZoneLinksFromCard(int player, int zone, u16 cardNo);
/* 1 if (player, zone) has a ZONE_LINK_CARD_EFFECT link from card number cardNo. */
int HasZoneCardEffectLink(int player, int zone, u16 cardNo);
/* Index of the first link on (player, zone) from card number cardNo, or -1. */
int FindZoneLinkFromCard(int player, int zone, u16 cardNo);
/* Number of the player's face-up monsters with a link from card number cardNo. */
int CountMonstersAffectedByCard(u32 player, u16 cardNo);
/* First face-up monster zone with a link from card number cardNo, or -1. */
int FindMonsterAffectedByCard(u32 player, u16 cardNo);
/* 1 if some face-up monster has a link from a card with the same number as the card in (player, slot). */
s32 IsCardLinkedToMonster(s32 player, s32 slot);
/* Nonzero if the equip card in (equipPlayer, equipSlot) may be equipped to (targetPlayer, targetSlot). */
u16 IsValidEquipTarget(u32 equipPlayer, u32 equipSlot, u32 targetPlayer, u32 targetSlot);
/* Number of monster zones (both players) the equip card in (equipPlayer, equipSlot) may equip. */
int CountValidEquipTargets(u32 equipPlayer, u32 equipSlot);
/* DUEL_LOC of the first face-up monster linked to a card with the same number as (player, slot), or
 * 0xFFFF; 0 for an empty zone. */
u16 FindMonsterLinkedToCard(s32 player, s32 slot);

/* --- Effective stats --- */

/* Card ID, effective type, attribute, ATK and DEF of the card in (player, zone). */
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
/* Effective ATK of the card in (player, slot). */
u32 GetZoneCardAtk(u32 player, u32 slot);
/* Effective DEF of the card in (player, slot). */
u32 GetZoneCardDef(u32 player, u32 slot);
/* Effective card type of the card in (player, slot) (type overrides apply to face-up monsters), 0 if empty. */
u32 GetZoneCardType(s32 player, s32 slot);
/* Effective attribute of the card in (player, slot), 0 if empty. */
u32 GetZoneCardAttribute(s32 player, s32 slot);

#endif /* GUARD_DUEL_H */
