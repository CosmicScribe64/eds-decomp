#ifndef GUARD_DUEL_CMD_H
#define GUARD_DUEL_CMD_H

/*
 * Duel command runner.
 *
 * Game logic never changes the duel state or the screen directly while a duel runs: it queues commands
 * (DuelCmd_Push: {cmd, arg2, arg4, arg6}, ids in constants/duel_cmds.h) and DuelMainStep runs the queue with
 * DuelCmdQueue_Run. That pops one command into gDuelCmd, sets gDuelCmd.running and calls DuelCmd_Dispatch once per
 * frame; the DuelCmd_* handler steps through gDuelCmd.step (scroll to the area, animate, commit) and clears
 * running when it is done. In a link duel every command is also sent to the partner (LINKMSG_COMMAND), which
 * runs it through DuelCmd_RunRemote (duel_link.h).
 *
 * Operands are named by their offset (arg2/arg4/arg6) in every unit; the handlers' meaning of each operand is
 * listed with the ids in constants/duel_cmds.h. The acting player is cmd bit 15, read as gDuelCmd.cmd >> 15.
 */

#include "global.h"
#include "constants/duel_cmds.h"
#include "duel.h"

/* One queued duel command (8 bytes): the same four halfwords as the head of struct DuelCmd. */
struct DuelCmdEntry {
    u16 cmd;    /* enum DuelCmdId in bits 0-11, acting player in bit 15 (DUEL_CMD_PLAYER) */
    u16 arg2;
    u16 arg4;
    u16 arg6;
};

/*
 * gDuelCmd: the command being executed, the queue, the handler step machine and the deck copies kept across
 * DUEL_CMD_RESET_DUEL_STATE. Cleared at duel start (0xD20 bytes).
 */
struct DuelCmd {
    u16 cmd;                            /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                           /* +0x002: first operand (zone, loc, amount or low half of a card word) */
    u16 arg4;                           /* +0x004: second operand */
    u16 arg6;                           /* +0x006: third operand */
    struct DuelCmdEntry queue[256];     /* +0x008: pending commands; DuelCmd_Push drops a command when full */
    u16 queueCount;                     /* +0x808 */
    u16 step:7;                         /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;                      /* +0x80A bits 7-13: handler loop counter or saved value */
    u16 unk80A_14:2;
    u32 unk80C_0:5;                     /* +0x80C */
    u32 timer:7;                        /* +0x80C bits 5-11: frame timer of the current step */
    u32 unk80C_12:1;
    u32 running:1;                      /* +0x80D bit 5: set by the queue runner, cleared by the finished handler */
    u32 unk80C_14:18;
    u16 *hofsTable;                     /* +0x810: 16-line HOFS wave read by the unused HBlank_WaveBg013 /
                                           HBlank_WaveAllBgs; no writer found */
    struct DuelCard card;               /* +0x814: card saved by the current command (the moving card) */
    u16 savedDeckCount[2];              /* +0x818: each player's deckCount, saved across the duel-state reset */
    u16 savedFusionCount[2];            /* +0x81C: each player's fusionCount, saved likewise */
    u32 savedDeck[2][80];               /* +0x820: copy of each player's deck[] */
    u32 savedFusionDeck[2][80];         /* +0xAA0: copy of each player's fusionDeck[] */
};

/*
 * FAKEMATCH view: gDuelCmd.timer read through a u16 container. DuelCmd_ShowCardZoomIn and
 * DuelCmd_ShowCardScatter need the narrower container (it changes loop hoisting); use it through gDuelCmdT16.
 */
struct DuelCmdTimer16 {
    u16 cmd;
    u16 arg2;
    u8 filler4[0x80C - 0x4];
    u16 unk80C_0:5;
    u16 timer:7;                        /* +0x80C bits 5-11: same bits as struct DuelCmd.timer */
    u16 unk80C_12:4;
};
#define gDuelCmdT16 (*(struct DuelCmdTimer16 *)&gDuelCmd)

/* One entry of gDuelResultBanners (indexed by arg2 of DUEL_CMD_SHOW_DUEL_RESULT: 0 win, 1 lose, 2 draw). */
struct DuelResultBanner {
    const u8 *pal;      /* 0x20-byte OBJ palette, copied to OBJ palette 15 */
    const u8 *gfx;      /* 0x800 bytes of 64x32 banner tiles, copied to OBJ VRAM 0x06016C80 */
    u16 jingle;         /* song started with PlayBGMIfSeEnabled; the banner holds until it ends */
};

/* RAM */
extern struct DuelCmd gDuelCmd;             /* 0x020185C0 */
extern struct DuelCard gDuelCmdCard;        /* 0x02018DD4: alias of gDuelCmd.card (prefer the member in new code) */

/* ROM data shared by the command handlers */
extern const s32 gScatterScaleCurve[];      /* [24] ease-out position scale 0x100 -> 0x3C6 (0x100 = 1.0) */
extern const u16 gShrinkScaleSteps[];       /* [8] affine scale 0xF0..0x80: pulse of the hand pointer */
extern const u16 gBannerSlideOffsets[];     /* [16] ease-out offsets 0..64 for banners sliding in from a side */
extern const u16 gLpDigitsPal[];            /* OBJ palette of the 16x16 life-point digits */
extern const u16 gLpDigitsGfx[];            /* 16x16 4bpp digit sprites, four colour sets */
extern const u8 gDuelBannerPal[];           /* 16-colour OBJ palette shared by the duel banners */
extern const u8 gSmokePuffAnim[];           /* smoke-puff sprite animation of a card set face down */

/* The VRAM slot of the duel banners (Surrender, Just a moment, Chain, Start Duel, the phase and attack banners):
 * gDuelBannerPal goes to OBJ palette 15 (0x050003E0), the banner tiles to OBJ tile 0x364 (0x06016C80). The units
 * spell the addresses out in their matched forms; these name the slot. */
#define DUEL_BANNER_PAL_SLOT        15
#define DUEL_BANNER_OBJ_TILE        0x364
#define DUEL_BANNER_ATTR2           ((DUEL_BANNER_PAL_SLOT << 12) | DUEL_BANNER_OBJ_TILE)  /* 0xF364 */

/* Queue and runner */
/* Queue a command; arg4 and arg6 are truncated to u16 inside. Callers that pass through a u16 view keep it. */
void DuelCmd_Push(u16 cmd, u16 arg2, int arg4, int arg6);
/* Pop and run the next queued command (link duels: send it and wait for the partner); 1 while busy. */
u32 DuelCmdQueue_Run(void);
/* Link duel: run the command received from the partner (gLinkState.remoteCmd); 1 while busy. */
u32 DuelCmd_RunRemote(void);
/* Call the handler of gDuelCmd.cmd once; unknown ids just clear running. */
void DuelCmd_Dispatch(void);

/* Turn, screen and banners */
void DuelCmd_TurnStart(void);                   /* 0x01: start-of-turn upkeep of counters and lock timers */
void DuelCmd_TurnEnd(void);                     /* 0x02: end-of-turn cleanup (countdowns, one-turn effects) */
void DuelCmd_ShowEndTurnHand(void);             /* 0x03: waving-hand sprite over the acting player's field */
void DuelCmd_ShowDuelResult(void);              /* 0x04: YOU WIN / YOU LOSE / DRAW banner and jingle */
void DuelCmd_ExodiaWinScene(void);              /* 0x05: run the Exodia win scene */
void DuelCmd_DestinyBoardWinScene(void);        /* 0x06: run the Destiny Board win scene */
void DuelCmd_ShowChainBanner(void);             /* 0x07: "Chain" banner */
void DuelCmd_PointAtCard(void);                 /* 0x08: select a card and point at it with a pulsing hand */
void DuelCmd_MoveCursor(void);                  /* 0x09: show the field cursor on an area/index */
void DuelCmd_ResetDuelState(void);              /* 0x10: clear the duel state, keep the decks, LP 8000 */
void DuelCmd_SetFieldBackground(void);          /* 0x11: load field background arg2 */
void DuelCmd_OpenDuelScreen(void);              /* 0x12: set up and fade in the duel screen */
void DuelCmd_CloseDuelScreen(void);             /* 0x13: fade out and leave the duel screen */
void DuelCmd_StartDuelBanner(void);             /* 0x14: "Start Duel" banner, then the duel BGM */
void DuelCmd_EnterPhase(u32 phase);             /* 0x50-0x52, 0x54, 0x55: phase banner, then gDuel.phase = phase */
void DuelCmd_EnterBattlePhase(void);            /* 0x53: Battle Phase banner and flash, then gDuel.phase */
void DuelCmd_Surrender(void);                   /* 0x40: "Surrender" banner, then the acting player's LP = 0 */
void DuelCmd_ShowJustAMomentBanner(void);       /* 0x41: "Just a moment" banner */

/* Life points, player flags and duel-wide flags */
void DuelCmd_ChangeLifePoints(u16 gain);        /* 0x42 (gain 1) / 0x43 (gain 0): count arg2 points in or out */
void DuelCmd_SetNegationFlag(void);             /* 0x15-0x1B: store arg2 in the gDuel negation flag of the id */
void DuelCmd_SetStatChangesReversed(void);      /* 0x1C: gDuel stat changes reversed = arg2 */
void DuelCmd_SetAtkDefSwapped(void);            /* 0x1D: gDuel ATK/DEF swapped = arg2 */
void DuelCmd_SkipNextDrawPhase(void);           /* 0x44 */
void DuelCmd_SkipNextStandbyPhase(void);        /* 0x45 */
void DuelCmd_SkipNextTurn(void);                /* 0x46 */
void DuelCmd_SetExtraBattlePhase(void);         /* 0x47: extra Battle Phase flag = arg2 */
void DuelCmd_SetPositionChangeLock(void);       /* 0x48: no position changes this turn = arg2 */
void DuelCmd_SetSummonLocks(void);              /* 0x49: no Normal Summon = arg2, no Special Summon = arg4 */
void DuelCmd_SetMagicTrapLockTurns(void);       /* 0x4A: magic/trap lock turns = arg2 */
void DuelCmd_AdjustDelayedSummonCount(void);    /* 0x4B: delayed summon count +1 (arg2 != 0) or -1 */
void sub_08014B5C(void);                        /* 0x4C: player +0x0C bit 4 = arg2 */
void DuelCmd_SetCrushCardTurns(void);           /* 0x69: Crush Card turns of the acting player = arg2 */

/* Battle */
void DuelCmd_StartBattleScene(void);            /* 0x30: fade the field out, set up the battle scene */
void DuelCmd_PlayBattleScene(void);             /* 0x31: run BattleScene_Update until it finishes */
void DuelCmd_PrepareBattlePhase(void);          /* 0x32: carry next-turn attack locks over */
void DuelCmd_Attack(void);                      /* 0x33: sword animation and "Attack" banner */
void DuelCmd_DirectAttack(void);                /* 0x34: "Direct Attack" banner */
void DuelCmd_MarkAttacked(void);                /* 0x35: mark monster zone arg2 as having attacked */
void DuelCmd_SetBattleProtection(void);         /* 0x36: set the acting player's battle damage guards */
void DuelCmd_EndBattlePhase(void);              /* 0x37: clear the attackable masks, end the battle stages */
void DuelCmd_SetAttackTarget(void);             /* 0x38: gBattle defender = arg2 >> 8 (restart if arg4) */
void DuelCmd_SetAttacker(void);                 /* 0x39: gBattle attacker = arg2 >> 8 (restart if arg4) */
void DuelCmd_ZeroAttackerAtk(void);             /* 0x3A: the attacker's ATK counts as 0 */
void DuelCmd_NegateAttack(void);                /* 0x3B: skip to the post-battle step, mark the attacker */

/* Cards on the field */
void DuelCmd_PlaceCard(void);                   /* 0x77: put a card word into a zone without animation */
void DuelCmd_ClearZoneCard(void);               /* 0x78: clear the card id of a zone */
void DuelCmd_ClearZoneCardNoRedraw(void);       /* unreferenced: clear a zone's card id, no redraw */
void DuelCmd_ChangePosition(void);              /* 0x7E: rotate a monster (optionally turning it face up) */
void DuelCmd_FlipCard(void);                    /* 0x7F: flip a zone card */
void DuelCmd_SendToGraveyard(void);             /* 0x79: zone card to its owner's graveyard */
void DuelCmd_Banish(void);                      /* 0x7A: zone card to its owner's banished pile */
void DuelCmd_BanishFlagged(void);               /* 0x7B: same, setting card bit 20 first */
void DuelCmd_ReturnToHand(void);                /* 0x80: zone card to its owner's hand (or fusion deck) */
void DuelCmd_ReturnToDeck(void);                /* 0x81: zone card to the top of its owner's deck (or fusion deck) */
void DuelCmd_MoveToZone(void);                  /* 0x82: move a whole zone record to another zone */
void DuelCmd_SwapZones(void);                   /* 0x84: swap two zone records */
void DuelCmd_TributeMonster(void);              /* 0x93: whirlwind, then the monster to graveyard or banished */
void DuelCmd_PlantInOpponentDeck(void);         /* 0x94: zone card to the top of the opponent's deck */
void DuelCmd_SummonToken(void);                 /* 0xA3: place a token (enum TokenKind arg4) in zone arg2 */
void DuelCmd_SetZoneCardWord(void);             /* 0xA4: overwrite a zone's card word */
void DuelCmd_MoveMonsterFaceDown(void);         /* 0xA7: move a monster to another zone face-down defense */
void DuelCmd_SetMagicalHatsCard(void);          /* 0xA8: place a card in defense position (Magical Hats) */
void DuelCmd_BanishMonsterUntilEndPhase(void);  /* 0xA9: banish a monster temporarily */
void DuelCmd_ReturnBanishedMonster(void);       /* 0xAA: return the temporarily banished monster */
void DuelCmd_SendFusionMaterialToGrave(void);   /* 0xA5: field fusion material to graveyard (or banished) */
void DuelCmd_ReturnSpellTrapToHand(void);       /* unreferenced: spell/trap zone card to the hand */
void DuelCmd_SendSpellTrapToGraveyard(void);    /* unreferenced: spell/trap zone card to the graveyard */

/* Zone state: links, counters and status flags */
void DuelCmd_AddEquipLink(void);                /* 0x83: link an equip card to a monster (link kind 1) */
void DuelCmd_AddZoneLink(void);                 /* 0x85: add a zone link of kind arg6 */
void DuelCmd_RemoveZoneLink(void);              /* 0x86: remove a zone link of kind arg6 */
void DuelCmd_ClearZoneLinks(void);              /* 0x8C: numLinks of a zone = 0 */
void DuelCmd_ClearZoneLinks2(void);             /* 0xA0: byte-identical copy of DuelCmd_ClearZoneLinks */
void DuelCmd_MoveZoneLinks(void);               /* 0x8D: move all links from zone arg2 to zone arg4 */
void DuelCmd_SetZoneDeclaredValue(void);        /* 0x87: declared value of a zone = arg4 */
void DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue(void); /* 0x88: turn counter = 0, declared value = arg4 */
void DuelCmd_SetZoneTurnCounter(void);          /* 0x89: turn counter of an occupied zone = arg4 */
void DuelCmd_AddZoneTurnCounter(void);          /* 0x8A: turn counter of an occupied zone += arg4 */
void DuelCmd_IncrementZoneTurnCounter(void);    /* 0xB4: turn counter += 1, saturating at 15 */
void DuelCmd_SetDestroyedByOpponentFlag(void);  /* 0x8B: card bit 22 of a zone = arg4 */
void DuelCmd_SetZoneStatusFlags(void);          /* 0x90: set ZoneStatusFlag bits arg4 on a zone */
void DuelCmd_ClearZoneStatusFlags(void);        /* 0x91: negate animation, clear ZoneStatusFlag bits arg4 */
void DuelCmd_SetEffectUnused(void);             /* 0x92: effect-unused flag of a zone = arg4 */
void DuelCmd_SetDestroyCountdown(void);         /* 0x95: destroy countdown of a zone (keeps the earlier one) */
void DuelCmd_HalveAttack(void);                 /* 0x98: set the ATK-halved flag of a monster */
void DuelCmd_SetCannotAttack(void);             /* 0x96: cannot-attack flag of a monster = arg4 */
void DuelCmd_SetCannotAttackNextTurn(void);     /* 0x97: cannot-attack-next-turn flag = arg4 */
void DuelCmd_SetPositionLocked(void);           /* 0xA1: position-locked flag of a monster = arg4 */
void DuelCmd_SetReturnAfterBattle(void);        /* 0xA2: return-after-battle flag of loc arg2 = arg4 */
void sub_08013104(void);                        /* 0xA6: zone +0x08 bit 0 = arg4 */
void DuelCmd_SetSpellTrapDisabled(void);        /* 0xB1: disable (with animation) or enable a spell/trap zone */
void DuelCmd_UpdateZoneLpPaid(void);            /* 0xB3: LP paid for a zone's card: add 500 or store arg4 */
void DuelCmd_NegateActivation(void);            /* 0xB0: negate the chain entry being responded to */
void DuelCmd_AddProhibition(void);              /* 0x8E: record a Prohibition and its declared card */
void DuelCmd_RemoveProhibition(void);           /* 0x8F: drop the Prohibition entries of a zone */

/* Deck and fusion deck */
void DuelCmd_ShuffleDeck(void);                 /* 0x60: shuffle animation; a link duel sends the deck */
void DuelCmd_DrawCards(void);                   /* 0x61: draw arg4 cards (sets deckOut on an empty deck) */
void DuelCmd_SendTopDeckCardsToGraveyard(void); /* 0x62: top arg2 deck cards to the graveyard */
void DuelCmd_BanishTopDeckCards(void);          /* 0x63: top arg2 deck cards to the banished pile */
void DuelCmd_AddDeckCardToHand(void);           /* 0x64: search a deck card into the hand */
void DuelCmd_RemoveCardFromDeck(void);          /* 0x65: remove a card from the deck, no animation */
void DuelCmd_SummonFromDeck(void);              /* 0x66: deck card to monster zone arg6, face-up attack */
void DuelCmd_SendDeckCardToGraveyard(void);     /* 0x67: deck card to the graveyard */
void DuelCmd_BanishDeckCard(void);              /* 0x68: deck card to the banished pile */
void DuelCmd_AddCardToDeckTop(void);            /* 0x6A: card word on top of its owner's deck */
void DuelCmd_AddCardToDeckBottom(void);         /* 0x6B: card word at the bottom of its owner's deck */
void DuelCmd_RemoveCardFromFusionDeck(void);    /* 0xDC: remove a card from the fusion deck, no animation */
void DuelCmd_SendFusionDeckCardToGraveyard(void); /* 0xDD: fusion deck card to the graveyard */

/* Graveyard and banished pile */
void DuelCmd_AddCardToGraveyard(void);          /* 0x7C: card word to its owner's graveyard */
void DuelCmd_AddCardToBanished(void);           /* 0x7D: card word to its owner's banished pile */
void DuelCmd_AddCardToGraveyardNoRedraw(void);  /* 0xD7: same as 0x7C without redrawing the field */
void DuelCmd_UnusedAddCardToGraveyard(void);    /* unreferenced: card word to the graveyard, redraw */
void DuelCmd_ReturnGraveyardCardToHand(void);   /* 0xD2: graveyard card to the hand (or fusion deck) */
void DuelCmd_ReturnGraveyardCardToDeckTop(void);    /* 0xD0: first graveyard card with id arg2 to the deck top */
void DuelCmd_ReturnGraveyardCardToDeckBottom(void); /* 0xD1: same, to the deck bottom */
void DuelCmd_BanishGraveyardCard(void);         /* 0xD4: graveyard card to the banished pile */
void DuelCmd_RemoveCardFromGraveyard(void);     /* 0xD3: remove a graveyard card, no animation */
void DuelCmd_ReturnGraveyardToDeck(void);       /* 0xD6: the whole graveyard back into the deck */
void DuelCmd_TakeOpponentGraveyardCard(void);   /* 0xD5: Graverobber: opponent's graveyard card to the hand */
void DuelCmd_ReturnBanishedCardToGraveyard(void); /* 0xDE: banished card to the graveyard */
void DuelCmd_ClearPendingEquip(void);           /* 0xD8: clear pendingEquip of graveyard[arg2] */
void DuelCmd_EquipGraveyardCardToOpponent(void); /* 0xD9: equip graveyard[arg2] to an opponent monster */
void sub_080106BC(void);                        /* 0xDA: clear bit 28 (pending opponent summon) of graveyard[arg2] */
void sub_08010708(void);                        /* 0xDB: set bit 23 of the graveyard card matching the card word */

/* Hand */
void DuelCmd_SendHandCardToGraveyard(void);     /* 0xC0: hand[arg2] to its owner's graveyard */
void DuelCmd_BanishHandCard(void);              /* 0xC1: hand[arg2] to its owner's banished pile */
void DuelCmd_ReturnHandCardToDeck(void);        /* 0xC3: hand[arg2] to the deck top (arg4) or bottom */
void DuelCmd_RemoveCardFromHand(void);          /* 0xC2: remove a hand card, no animation */
void DuelCmd_PlaceMonsterFromHand(void);        /* 0xC4: Normal Summon / Set / Special Summon from the hand */
void DuelCmd_PlaceSpellTrapFromHand(void);      /* 0xC5: Set or activate a magic/trap from the hand */
void DuelCmd_CompactHand(void);                 /* 0xCA: close the gaps in the hand */
void DuelCmd_AddCardToHand(void);               /* 0xCB: card word to the hand (fusion monsters: fusion deck) */
void DuelCmd_BanishHandCardFaceDown(void);      /* 0xCE: hand[arg2] banished face down (Lightforce Sword) */
void DuelCmd_ReturnBanishedCardToHand(void);    /* 0xCF: banished[arg2] back to the hand */
void DuelCmd_ExchangeHandCards(void);           /* 0xC7: swap one hand card with the opponent (Exchange) */
void DuelCmd_SendHandFusionMaterialToGraveyard(void); /* 0xCC: hand fusion material to the graveyard */
void DuelCmd_BanishHandFusionMaterial(void);    /* 0xCD: hand fusion material to the banished pile */

/* Card presentation (arg2: card id) */
void DuelCmd_ShowCardDetail(void);              /* 0x70: full-screen Card Detail page */
void DuelCmd_ShowCardAssemble(void);            /* 0x71: card tiles fly together, then a white flash */
void DuelCmd_ShowCardZoomIn(void);              /* 0x72: card grows from the centre */
void DuelCmd_ShowCardEffect(void);              /* 0x73: zoom-in with a white background flash */
void DuelCmd_ShowCardScatter(void);             /* 0x74: card appears, then its tiles fly apart */
void DuelCmd_ShowCardUnrollDown(void);          /* 0x75: card unrolls downwards and rolls back up */
void DuelCmd_ShowCardUnrollSideways(void);      /* 0x76: card opens and closes horizontally */

/* Coin and dice scenes (the pusher decides the result) */
void DuelCmd_TossCoin(void);                    /* 0xE0: one coin, arg2 called face, arg4 result */
void DuelCmd_TossThreeCoins(void);              /* 0xE1: three coins, results in arg2 bits 0-2 */
void DuelCmd_RollGracefulDice(void);            /* 0xE2: Graceful Dice roll arg2 */
void DuelCmd_RollSkullDice(void);               /* 0xE3 / 0xE5: Skull Dice roll arg2 */
void DuelCmd_RollPlainDie(void);                /* 0xE4: plain die roll arg2 */

/* Handlers that only finish */
void DuelCmd_Nop99(void);                       /* 0x99 */
void DuelCmd_Nop9A(void);                       /* 0x9A */
void DuelCmd_Nop9B(void);                       /* 0x9B */
void DuelCmd_Nop9C(void);                       /* 0x9C */
void DuelCmd_Nop9D(void);                       /* 0x9D */
void DuelCmd_Nop9E(void);                       /* 0x9E */
void DuelCmd_Nop9F(void);                       /* 0x9F */
void DuelCmd_NopB2(void);                       /* 0xB2 */
void DuelCmd_UnusedNop(void);                   /* no dispatcher case */
void DuelCmd_UnusedNop2(void);                  /* no dispatcher case */

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char duel_cmd_h_check_entry[sizeof(struct DuelCmdEntry) == 0x8 ? 1 : -1];
typedef char duel_cmd_h_check_queue[(u32)&((struct DuelCmd *)0)->queue == 0x8 ? 1 : -1];
typedef char duel_cmd_h_check_count[(u32)&((struct DuelCmd *)0)->queueCount == 0x808 ? 1 : -1];
typedef char duel_cmd_h_check_hofs[(u32)&((struct DuelCmd *)0)->hofsTable == 0x810 ? 1 : -1];
typedef char duel_cmd_h_check_card[(u32)&((struct DuelCmd *)0)->card == 0x814 ? 1 : -1];
typedef char duel_cmd_h_check_saved[(u32)&((struct DuelCmd *)0)->savedDeck == 0x820 ? 1 : -1];
typedef char duel_cmd_h_check_fusion[(u32)&((struct DuelCmd *)0)->savedFusionDeck == 0xAA0 ? 1 : -1];
typedef char duel_cmd_h_check_size[sizeof(struct DuelCmd) == 0xD20 ? 1 : -1];
typedef char duel_cmd_h_check_t16[sizeof(struct DuelCmdTimer16) == 0x810 ? 1 : -1];
typedef char duel_cmd_h_check_banner[sizeof(struct DuelResultBanner) == 0xC ? 1 : -1];

#endif /* GUARD_DUEL_CMD_H */
