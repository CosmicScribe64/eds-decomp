#include "global.h"
#include "constants/cards.h"        /* CARD_SNATCH_STEAL, CARD_INSECT_MONSTER_TOKEN */
#include "constants/duel.h"         /* enum DuelArea, DuelPhase, DuelZoneIndex, ZoneStatusFlag; DUEL_LOC_ZONE */
#include "constants/duel_cmds.h"    /* enum TokenKind */
#include "card_data.h"              /* CARD_NUMBER_TOKEN_FIRST / _END, CARD_NUMBER_ALT_ART */
#include "util.h"                   /* MemCopy16 */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_actions.h"           /* UpdateMonsterControl */

/*
 * Duel command handlers 0x90-0xA7 and 0xB0-0xB4 (wiki/functions/duel-cmd-status-c.md), called by
 * DuelCmd_Dispatch once per frame while gDuelCmd.running is set. Each reads its operands from gDuelCmd
 * (arg2 is usually a zone of the acting player, arg4 a value) and clears running when it is done; the
 * multi-frame handlers step through gDuelCmd.step.
 *  - one-frame setters of zone state: the summon status bits, the once-per-turn effect flag, the destroy
 *    countdown, the cannot-attack bits, Riryoku's ATK halving, Magic-Arm Shield's return-after-battle bit,
 *    the turn counter, the LP paid for Toon World, and the negation of a chain link (counter traps);
 *  - step machines with animations: tribute a monster, plant Parasite Paracide in the opponent's deck,
 *    summon a token, disable a spell/trap, move a monster face down;
 *  - ten no-op handlers (0x99-0x9F, 0xB2 and two unreferenced ones).
 */

/* ---- BEGIN pre-H0 subset of duel.h, duel_cmd.h, duel_screen.h, chain.h and sound.h ---- */
/*
 * The part of those headers this unit uses, with their names, types and bitfield containers. include/duel.h
 * and include/sound.h still hold the legacy headers until the header switch (H0, build/readability/HEADERS.md),
 * and duel_cmd.h, duel_screen.h and chain.h include duel.h. After H0, replace this block (BEGIN to END) with
 *     #include "duel.h"
 *     #include "duel_cmd.h"
 *     #include "duel_screen.h"
 *     #include "chain.h"
 *     #include "sound.h"
 * which gives identical assembly (checked against the staged headers).
 */
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

struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;                       /* +0x02: padding, copied with the word */
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed (replay check) */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5: turns a face-up card has been active */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9: turns until the monster is destroyed; u16 container */
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position (DuelCmd_SetPositionLocked, tokens) */
    u8 unk7_3:1;                    /* +0x07 bit 3 */
    u8 unk7_4:1;                    /* +0x07 bit 4 */
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet (Ameba, Griggle) */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6: ZONE_STATUS_MONSTER_REBORN */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7: ZONE_STATUS_FROM_GRAVEYARD */
    u8 levelCheckDone:1;            /* +0x08 bit 0 */
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];                  /* +0x0A */
    u16 linkKinds[32];              /* +0x4A */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C_0:1;                   /* +0x8C: battle flags */
    u8 destroyAfterBattle:1;        /* +0x8C bit 1 */
    u32 returnAfterBattle:1;        /* +0x8C bit 2: borrowed by Magic-Arm Shield; u32 container */
    u8 cannotAttackNextTurn:1;      /* +0x8C bit 3: Electric Lizard; moved into cannotAttack */
    u8 cannotAttack:1;              /* +0x8C bit 4 */
    u8 atkHalved:1;                 /* +0x8C bit 5: Riryoku halving until end of turn */
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;
    u32 canActivate:1;              /* +0x91 bit 2 */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9 */
    u32 unk92_2:14;
};

struct DuelPlayer {
    u8 unk0[0xE];
    u16 lpPaid[11];                 /* +0x00E: per zone, LP paid for the card there (Toon World) */
    u16 attackableMask;             /* +0x024 */
    u16 attackedMask;               /* +0x026: monster zones that have attacked this turn */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 piles[0xD64 - 0x684];        /* +0x684: hand, deck, graveyard, fusion deck, banished */
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5 */
    u8 result:2;                    /* +0x1B12 bits 6-7 */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

void CopyDuelCard(u32 *dst, u32 *src);
void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 defense, u16 faceUp);
void SendZoneCardToGraveyardOrBanished(int player, int zone, u16 banish);
void AddCardToDeckTop(int player, struct DuelCard *card);
void AddCardToGraveyard(struct DuelCard *card);
u16 FindMonsterWithLinkTo(int player, int zone);

struct DuelCmdEntry {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
};

struct DuelCmd {
    u16 cmd;                            /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                           /* +0x002 */
    u16 arg4;                           /* +0x004 */
    u16 arg6;                           /* +0x006 */
    struct DuelCmdEntry queue[256];     /* +0x008 */
    u16 queueCount;                     /* +0x808 */
    u16 step:7;                         /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;                      /* +0x80A bits 7-13 */
    u16 unk80A_14:2;
    u32 unk80C_0:5;                     /* +0x80C */
    u32 timer:7;                        /* +0x80C bits 5-11 */
    u32 unk80C_12:1;
    u32 running:1;                      /* +0x80D bit 5: cleared by the finished handler */
    u32 unk80C_14:18;
    u16 *hofsTable;                     /* +0x810 */
    struct DuelCard card;               /* +0x814: card saved by the current command (the moving card) */
    u16 savedDeckCount[2];              /* +0x818 */
    u16 savedFusionCount[2];            /* +0x81C */
    u32 savedDeck[2][80];               /* +0x820 */
    u32 savedFusionDeck[2][80];         /* +0xAA0 */
};

extern struct DuelCmd gDuelCmd;             /* 0x020185C0 */
extern struct DuelCard gDuelCmdCard;        /* 0x02018DD4: alias of gDuelCmd.card */
extern const u8 gSmokePuffAnim[];           /* smoke-puff sprite animation of a card set face down */

void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
void DuelAnim_PlayZoneEffect(struct DuelLoc *loc, u32 anim, u32 dx, u32 dy);
void ClearZoneTiles(u32 player, u32 area);
void DrawAllAreaTiles(void);
int GetZoneArea(int zone);

struct ChainEntry {
    u16 card;                   /* +0x00: card ID */
    u8 player:1;                /* +0x02 bit 0: activating player */
    u8 kind:3;                  /* +0x02 bits 1-3: enum ChainEntryKind */
    u16 zone:6;                 /* +0x02 bits 4-9: zone of the activated card */
    u16 event:6;                /* +0x02 bits 10-15 */
    u8 skipChainA:1;            /* +0x04 bit 0 */
    u8 skipChainB:1;            /* +0x04 bit 1 */
    u8 negated:1;               /* +0x04 bit 2: activation negated; most handlers return at once */
    u8 destroyIfNegated:1;      /* +0x04 bit 3: a card that would stay on the field goes to the graveyard */
    u8 flag4_4:1;               /* +0x04 bit 4 */
    u8 unk4_5:3;
    u8 unk5;                    /* +0x05 */
    u16 loc0;                   /* +0x06 */
    u16 loc1;                   /* +0x08 */
    u8 numTargets:3;            /* +0x0A bits 0-2 */
    u8 unkA_3:5;
    u8 unkB;                    /* +0x0B */
    u16 targets[3];             /* +0x0C */
    u16 unk12;                  /* +0x12 */
};

struct ChainState {
    struct ChainEntry pending[32];      /* +0x000 */
    struct ChainEntry links[16];        /* +0x280: the chain, resolved from the last link back */
    u16 linkCount;                      /* +0x3C0: entries in links */
};

extern struct ChainState gChain;            /* 0x02017A40 */

void PlaySE(u32 seId);
/* ---- END pre-H0 subset ---- */

/* ---- Unit-local helpers and views ---- */

/* Acting player of the current command: bit 15 of the command word (DUEL_CMD_PLAYER). */
#define CMD_PLAYER() (gDuelCmd.cmd >> 15)

/* Link duels: on the partner's turn (turnPlayer 1) the partner's GBA runs the duel and sends the results,
 * so some handlers skip their rule effects here. */
#define IS_LINK_PARTNER_TURN() (gDuelCtrl.isLinkDuel && gDuel.turnPlayer)

/* The same test with gDuel.turnPlayer read as bit 1 of the byte gDuelZones + 0x1AE6 (= gDuel + 0x1B12).
 * Matching: DuelCmd_SetSpellTrapDisabled forms that address from the zones literal it already holds. */
#define IS_LINK_PARTNER_TURN_ZONES() (gDuelCtrl.isLinkDuel && (((u8 *)gDuelZones)[0x1AE6] & 2))

/* &gDuelZones[player].zones[zone], spelled zone * 0x94 + player * 0xD64 + base. Matching: the index form
 * gives another evaluation order; through this inline the ROM's order comes out (callers pass player & 1
 * where the ROM masks the player). */
static inline struct DuelZone *ZoneAt(u32 player, int zone)
{
    return (struct DuelZone *)(zone * 0x94 + player * 0xD64 + (u8 *)gDuelZones);
}

/* DuelZone +0x06/+0x07 in a u32 container (the word at +0x04), with the DuelZone names. Matching: through
 * DuelZone's u8 containers the reads of isDefense / isFaceUp allocate registers differently (an r6/r7 swap
 * in DuelCmd_ClearZoneStatusFlags; arg4 spilled to the stack instead of kept in sl in
 * DuelCmd_TributeMonster), and the store of positionLocked loads arg4 with ldrb instead of ldrh
 * (DuelCmd_SetPositionLocked). */
struct DuelZoneWord4View {
    u32 card;
    u32 serial:16;
    u32 isDefense:1;        /* +0x06 bit 0 */
    u32 isFaceUp:1;         /* +0x06 bit 1 */
    u32 turnCounter:4;      /* +0x06 bits 2-5 */
    u32 destroyCountdown:4; /* +0x06 bits 6-9 */
    u32 positionLocked:1;   /* +0x07 bit 2 */
    u32 unk7_3:5;
};

/* DuelZone +0x90 in a u16 container, for DuelZone.isDisabled (+0x91 bit 3). Matching: with the u8 container
 * of DuelZone.isDisabled, CSE reuses the long-lived constant-1 register for the store mask, which swaps
 * r7/r8 for the whole of DuelCmd_SetSpellTrapDisabled. */
struct DuelZoneDisabledView {
    u8 filler0[0x90];
    u16 unk90_0:11;
    u16 isDisabled:1;       /* +0x91 bit 3 */
    u16 unk90_12:4;
};

/* gDuel.players addressed as gDuel + 4 instead of gDuel.players[]. Matching: agbcc then keeps the +4 base
 * apart from the +0x26 field offset and derives it from the zones literal (r3 - 0x28), as the ROM does. */
#define PLAYERS_AT_GDUEL_PLUS_4 ((struct DuelPlayer *)((u8 *)&gDuel + 4))

/* gCardIdToNumber[word & 0x7FF] for a card word: the card number of its ID. Matching: read through the
 * table's integer address (0x08622AB4), which forms the index ((word << 21) >> 20) before the table
 * address; the symbol form gCardIdToNumber[] gives other code. */
#define CARD_NUMBER_OF_WORD(word) (*(const u16 *)(0x08622AB4 + (((u32)(word) << 21) >> 20)))

/* gCardNumberToId[number]; an alternate-art number (2000 + n) maps to the ID after card n's. Each unit keeps
 * its own copy of this helper (card_data.h). Matching: read through the table's integer address
 * (0x08623DF4); the symbol form gives other code. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    else if (number < CARD_NUMBER_ALT_ART)
        return *(((const u16 *)0x08623DF4) + (number & 0x7FF));
    else
        return *(((const u16 *)0x08623DF4) + ((number - CARD_NUMBER_ALT_ART) & 0x7FF)) + 1;
}

/* Sprite animation streams for DuelAnim_PlayZoneEffect (only this unit uses them). Each is played with
 * sound effect 16, which constants/sound.h does not name yet. */
extern const u8 gNegateAnim[];          /* 0x0868DB94: a card is negated */
extern const u8 gNegateAnimSideways[];  /* 0x0868EC38: the same for a card in defense position */
extern const u8 gTributeAnim[];         /* 0x08690D0C: a whirlwind sweeps the card away */

/* ---- Handlers (in ROM order) ---- */

/* Unreferenced: puts card word arg2 | arg4 << 16 into its owner's graveyard and redraws the field (the
 * same as DuelCmd_AddCardToGraveyard plus the redraw). */
void DuelCmd_UnusedAddCardToGraveyard(void)
{
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    AddCardToGraveyard((struct DuelCard *)&card);
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/*
 * 0xB0, queued by the counter traps (Magic Jammer, Seven Tools of the Bandit, Solemn Judgment, ...): marks
 * the chain link they respond to, the one before the counter trap's own link, as negated and, if arg2 != 0,
 * to be destroyed (every caller passes 1). Skipped on the link partner's turn.
 */
void DuelCmd_NegateActivation(void)
{
    struct ChainEntry *link;

    if (!IS_LINK_PARTNER_TURN()) {
        if (gChain.linkCount > 1) {
            link = &gChain.links[gChain.linkCount - 2];
            link->negated = 1;
            if (gDuelCmd.arg2)
                link->destroyIfNegated = 1;
        }
    }
    gDuelCmd.running = 0;
}

/* 0xB2: nothing queues it; only finishes. */
void DuelCmd_NopB2(void)
{
    gDuelCmd.running = 0;
}

/*
 * 0xB1 (Jinzo, Royal Decree, Imperial Order, the counter traps, Armored Glass and the other negators): sets
 * isDisabled of the acting player's spell/trap or field zone arg2 (5-10) to arg4.
 *   step 0: an empty zone finishes at once. Disabling plays the negate animation and goes to step 3,
 *           enabling goes to step 1; on the link partner's turn enabling finishes at once and disabling
 *           goes to step 10, which only finishes.
 *   steps 1 and 3: if the card is Snatch Steal, UpdateMonsterControl decides again who controls the
 *           monster it is equipped to. Step 3 indexes the zone with the unmasked player.
 */
void DuelCmd_SetSpellTrapDisabled(void)
{
    struct DuelLoc loc;
    u32 player = CMD_PLAYER();
    int zoneIdx = gDuelCmd.arg2;
    struct DuelZone *zone;
    u16 equipTarget;    /* DUEL_LOC of the stolen monster */

    switch (gDuelCmd.step) {
    case 0:
        zone = ZoneAt(player & 1, zoneIdx);
        /* Empty zone. Read through a card pointer, which loads the whole word as the ROM does. */
        if (((struct DuelCard *)zone)->id == 0) {
            gDuelCmd.running = 0;
            break;
        }
        ((struct DuelZoneDisabledView *)zone)->isDisabled = gDuelCmd.arg4;
        if (gDuelCmd.arg4 != 0) {
            PlaySE(16);
            loc.player = player;
            loc.area = DUEL_AREA_SPELL_TRAP;
            loc.index = zoneIdx - ZONE_SPELL_0;
            if (zoneIdx > ZONE_SPELL_4) {
                loc.area = DUEL_AREA_FIELD;
                loc.index = 0;
            }
            loc.isDefense = zone->isDefense;
            loc.isFaceUp = zone->isFaceUp;
            DuelAnim_PlayZoneEffect(&loc, loc.isDefense ? (u32)gNegateAnimSideways : (u32)gNegateAnim, 0, 0);
            if (!IS_LINK_PARTNER_TURN_ZONES())
                gDuelCmd.step = 3;
            else
                gDuelCmd.step = 10;
        } else {
            if (!IS_LINK_PARTNER_TURN_ZONES())
                gDuelCmd.step++;
            else
                gDuelCmd.running = 0;
        }
        break;
    case 1:
        if (CARD_NUMBER_OF_WORD(*(u32 *)ZoneAt(player & 1, zoneIdx)) == CARD_SNATCH_STEAL) {
            equipTarget = FindMonsterWithLinkTo(player, zoneIdx);
            if (equipTarget != 0xFFFF)
                UpdateMonsterControl((u8)equipTarget, DUEL_LOC_ZONE(equipTarget), 0xFFFF);
        }
        gDuelCmd.running = 0;
        break;
    case 3:
        if (CARD_NUMBER_OF_WORD(*(u32 *)ZoneAt(player, zoneIdx)) == CARD_SNATCH_STEAL) {
            equipTarget = FindMonsterWithLinkTo(player, zoneIdx);
            if (equipTarget != 0xFFFF)
                UpdateMonsterControl((u8)equipTarget, DUEL_LOC_ZONE(equipTarget), 0xFFFF);
        }
        gDuelCmd.running = 0;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * 0xB3 (Toon World): LP paid for the card in the acting player's zone arg2. arg4 == 500 adds a Standby
 * Phase payment; any other value is stored (0 when Toon World is activated, and again when it leaves the
 * field after the total has been given back as LP).
 */
void DuelCmd_UpdateZoneLpPaid(void)
{
    u32 player = CMD_PLAYER();
    u16 zone = gDuelCmd.arg2;

    if (gDuelCmd.arg4 == 500) {
        gDuelPlayers[player].lpPaid[zone] += gDuelCmd.arg4;
    } else {
        gDuelPlayers[player].lpPaid[zone] = gDuelCmd.arg4;
    }
    gDuelCmd.running = 0;
}

/* 0xB4 (Germ Infection, Stim-Pack in the Standby Phase): turn counter of the acting player's zone arg2 + 1,
 * saturating at 15. */
void DuelCmd_IncrementZoneTurnCounter(void)
{
    u32 player = CMD_PLAYER();
    struct DuelZone *zone = ZoneAt(player, gDuelCmd.arg2);

    if (zone->turnCounter < 15)
        zone->turnCounter++;
    gDuelCmd.running = 0;
}

/*
 * 0x90, queued after a summon with the summon's status mask (a Normal Summon passes
 * ZONE_STATUS_NORMAL_SUMMONED; a Set queues nothing): sets the enum ZoneStatusFlag bits arg4 on the acting
 * player's zone arg2. A monster summoned in its controller's own Battle Phase also gets its attackedMask bit
 * cleared, so it can still attack.
 */
void DuelCmd_SetZoneStatusFlags(void)
{
    /* FAKEMATCH: the copy `player = p` gives the shift result its own short-lived pseudo (r0), which the
     * `player & 1` below reuses, while the copy lives in r6. */
    u32 p = CMD_PLAYER();
    u32 player = p;
    int zoneIdx = gDuelCmd.arg2;
    /* Matching: the zone, the +0x1B12 flag byte and the players base all come from the one zones literal. */
    struct DuelZone *zone = &gDuel.players[player & 1].zones[zoneIdx];

    if (gDuelCmd.arg4 & ZONE_STATUS_UNK14)
        zone->card.unk14 = 1;
    if (gDuelCmd.arg4 & ZONE_STATUS_NORMAL_SUMMONED)
        zone->card.normalSummoned = 1;
    if (gDuelCmd.arg4 & ZONE_STATUS_SPECIAL_SUMMONED)
        zone->card.specialSummoned = 1;
    if (gDuelCmd.arg4 & ZONE_STATUS_PLANTED)
        zone->card.planted = 1;
    if (gDuelCmd.arg4 & ZONE_STATUS_MONSTER_REBORN)
        zone->revivedByMonsterReborn = 1;
    if (gDuelCmd.arg4 & ZONE_STATUS_FROM_GRAVEYARD)
        zone->summonedFromGraveyard = 1;
    if (gDuel.phase == PHASE_BATTLE && player == gDuel.turnPlayer)
        PLAYERS_AT_GDUEL_PLUS_4[player & 1].attackedMask &= ~(1 << zoneIdx);
    gDuelCmd.running = 0;
}

/*
 * 0x91 (Solemn Judgment, Horn of Heaven, with arg4 = 7): the counterpart of DuelCmd_SetZoneStatusFlags for
 * a summon that is negated, before the monster is destroyed.
 *   step 0: negate animation on the acting player's monster zone arg2;
 *   step 1: clear the enum ZoneStatusFlag bits arg4 and finish.
 */
void DuelCmd_ClearZoneStatusFlags(void)
{
    struct DuelLoc loc;
    u32 player = CMD_PLAYER();
    u16 zoneIdx = gDuelCmd.arg2;
    struct DuelZone *zones = gDuelZones[player & 1].zones;
    struct DuelZone *zone = zones + zoneIdx;

    switch (gDuelCmd.step) {
    case 0:
        PlaySE(16);
        loc.player = player;
        loc.area = DUEL_AREA_MONSTER;
        loc.index = zoneIdx;
        loc.isDefense = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isDefense;
        loc.isFaceUp = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isFaceUp;
        DuelAnim_PlayZoneEffect(&loc, loc.isDefense ? (u32)gNegateAnimSideways : (u32)gNegateAnim, 0, 0);
        gDuelCmd.step++;
        break;
    default:
        if (gDuelCmd.arg4 & ZONE_STATUS_UNK14)
            zone->card.unk14 = 0;
        if (gDuelCmd.arg4 & ZONE_STATUS_NORMAL_SUMMONED)
            zone->card.normalSummoned = 0;
        if (gDuelCmd.arg4 & ZONE_STATUS_SPECIAL_SUMMONED)
            zone->card.specialSummoned = 0;
        if (gDuelCmd.arg4 & ZONE_STATUS_PLANTED)
            zone->card.planted = 0;
        if (gDuelCmd.arg4 & ZONE_STATUS_MONSTER_REBORN)
            zone->revivedByMonsterReborn = 0;
        if (gDuelCmd.arg4 & ZONE_STATUS_FROM_GRAVEYARD)
            zone->summonedFromGraveyard = 0;
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x92: effectUnused of the acting player's zone arg2 = arg4. Every caller passes 0: the once-per-turn or
 * one-shot effect has been used. */
void DuelCmd_SetEffectUnused(void)
{
    ZoneAt(CMD_PLAYER(), gDuelCmd.arg2)->effectUnused = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/*
 * 0x93, a Tribute paid as a cost: the acting player's monster in zone arg2 goes to its owner's graveyard,
 * or is banished if arg4 != 0 (every caller passes 0).
 *   step 0: whirlwind animation on the zone, clear its cell;
 *   step 1: take the card out of the zone (a copy stays in gDuelCmd.card) and animate it to the pile;
 *   step 2: redraw the field and finish.
 */
void DuelCmd_TributeMonster(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u32 zoneIdx = gDuelCmd.arg2;
    u16 banish = gDuelCmd.arg4;
    struct DuelZone *zones;
    struct DuelCard *card;
    u16 number;

    switch (gDuelCmd.step) {
    case 0:
        PlaySE(16);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = zoneIdx;
        from.isDefense = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isDefense;
        from.isFaceUp = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isFaceUp;
        DuelAnim_PlayZoneEffect(&from, (u32)gTributeAnim, -16, -32);
        ClearZoneTiles(player, zoneIdx);
        gDuelCmd.step++;
        break;
    case 1:
        card = &gDuelCmd.card;
        zones = gDuelZones[player & 1].zones;
        CopyDuelCard((u32 *)card, (u32 *)(zones + zoneIdx));
        SendZoneCardToGraveyardOrBanished(player, zoneIdx, banish);
        /* Tokens (card numbers 1920-1999) leave the duel without an animation. The number is looked up from
         * the low halfword of the card word. */
        number = CARD_NUMBER_OF_WORD(*(u16 *)card);
        if (number < CARD_NUMBER_TOKEN_FIRST || number >= CARD_NUMBER_TOKEN_END) {
            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = zoneIdx;
            from.isDefense = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isDefense;
            from.isFaceUp = ((struct DuelZoneWord4View *)ZoneAt(player & 1, zoneIdx))->isFaceUp;
            to.player = card->owner;
            to.area = banish ? DUEL_AREA_BANISHED : DUEL_AREA_GRAVEYARD;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 1;
            DuelAnim_MoveCard(card->id, &from, &to);
        }
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * 0x94, Parasite Paracide's FLIP effect: puts the acting player's monster in zone arg2 on top of the
 * opponent's deck with its planted bit set (when drawn it is summoned to the drawer's field and deals 1000
 * damage). The resolve then shuffles that deck (command 0x60). The opponent is player - 1, which
 * AddCardToDeckTop masks with & 1; arg4 is ignored.
 *   step 0: scroll to the zone's row;
 *   step 1: copy the card to gDuelCmd.card and mark it planted, clear the zone's cell, animate the card to
 *           the opponent's deck;
 *   step 2: put it on the deck, empty the zone, redraw, finish.
 */
void DuelCmd_PlantInOpponentDeck(void)
{
    struct DuelLoc from, to;
    struct DuelZone *zones;
    u32 player = CMD_PLAYER();
    u16 zoneIdx = gDuelCmd.arg2;
    u32 opponent = player - 1;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, GetZoneArea(zoneIdx));
        gDuelCmd.step++;
        break;
    case 1:
        /* gDuelCmdCard is gDuelCmd.card under its own symbol; the ROM addresses it by that symbol here. */
        zones = gDuelZones[player & 1].zones;
        CopyDuelCard((u32 *)&gDuelCmdCard, (u32 *)(zones + zoneIdx));
        gDuelCmdCard.planted = 1;
        ClearZoneTiles(player, zoneIdx);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = zoneIdx;
        from.isDefense = ZoneAt(player & 1, zoneIdx)->isDefense;
        from.isFaceUp = ZoneAt(player & 1, zoneIdx)->isFaceUp;
        to.player = opponent;
        to.area = DUEL_AREA_DECK;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(gDuelCmdCard.id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToDeckTop(opponent, &gDuelCmd.card);
        ZoneAt(player, zoneIdx)->card.id = 0;
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * 0x95 (Zone Eater and Swordsman from a Foreign Land: 5 turns; Steel Scorpion: 3): destroy countdown of the
 * acting player's monster zone arg2 = arg4, unless a shorter countdown is already running. The turn-end pass
 * counts it down and destroys the monster at 0.
 */
void DuelCmd_SetDestroyCountdown(void)
{
    u32 player = CMD_PLAYER();
    int turnsLeft;  /* int: the ROM compares signed */
    /* = ZoneAt(player, arg2), spelled out: through the inline the registers differ here. */
    struct DuelZone *zone = (struct DuelZone *)(gDuelCmd.arg2 * 0x94 + player * 0xD64 + (u8 *)gDuelZones);

    turnsLeft = zone->destroyCountdown;
    if (turnsLeft == 0 || turnsLeft > gDuelCmd.arg4)
        zone->destroyCountdown = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* Unreferenced no-op (no dispatcher case). */
void DuelCmd_UnusedNop(void)
{
    gDuelCmd.running = 0;
}

/* 0x99-0x9E (and 0x9F below): dispatchable no-ops that nothing queues. */
void DuelCmd_Nop99(void)
{
    gDuelCmd.running = 0;
}

void DuelCmd_Nop9A(void)
{
    gDuelCmd.running = 0;
}

void DuelCmd_Nop9B(void)
{
    gDuelCmd.running = 0;
}

void DuelCmd_Nop9C(void)
{
    gDuelCmd.running = 0;
}

void DuelCmd_Nop9D(void)
{
    gDuelCmd.running = 0;
}

void DuelCmd_Nop9E(void)
{
    gDuelCmd.running = 0;
}

/* 0x98 (Riryoku): halves the ATK of the acting player's monster in zone arg2; the turn-end pass clears the
 * flag. */
void DuelCmd_HalveAttack(void)
{
    ZoneAt(CMD_PLAYER(), gDuelCmd.arg2)->atkHalved = 1;
    gDuelCmd.running = 0;
}

/* 0x97 (Electric Lizard): cannotAttackNextTurn of the acting player's monster zone arg2 = arg4; the next
 * Battle Phase preparation moves it into cannotAttack. */
void DuelCmd_SetCannotAttackNextTurn(void)
{
    ZoneAt(CMD_PLAYER(), gDuelCmd.arg2)->cannotAttackNextTurn = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* 0x96: cannotAttack of the acting player's monster zone arg2 = arg4. */
void DuelCmd_SetCannotAttack(void)
{
    ZoneAt(CMD_PLAYER(), gDuelCmd.arg2)->cannotAttack = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* 0x9F: no-op, as 0x99-0x9E. */
void DuelCmd_Nop9F(void)
{
    gDuelCmd.running = 0;
}

/* 0xA1: positionLocked of the acting player's monster zone arg2 = arg4 (the monster cannot change its
 * position). */
void DuelCmd_SetPositionLocked(void)
{
    ((struct DuelZoneWord4View *)ZoneAt(CMD_PLAYER(), gDuelCmd.arg2))->positionLocked = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/*
 * 0xA2 (Magic-Arm Shield): returnAfterBattle of the monster at location arg2 (a DUEL_LOC, either player)
 * = arg4. After the battle the borrowed monster goes back to a free zone of its controller, or is destroyed
 * if there is none.
 */
void DuelCmd_SetReturnAfterBattle(void)
{
    u32 zoneIdx = DUEL_LOC_ZONE(gDuelCmd.arg2);
    u32 player = *(u8 *)&gDuelCmd.arg2;    /* DUEL_LOC_PLAYER. Matching: read as the low byte (ldrb) */

    ZoneAt(player & 1, zoneIdx)->returnAfterBattle = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* Unreferenced no-op (no dispatcher case). */
void DuelCmd_UnusedNop2(void)
{
    gDuelCmd.running = 0;
}

/*
 * 0xA3: summons token kind arg4 (enum TokenKind, card number 1920 + kind) into the acting player's monster
 * zone (u8)arg2 (the high byte, the source zone, is ignored): Special Summoned, face up, kinds 1 and 2 in
 * defense position, and position-locked. Only kind 0, Insect Queen's Insect Monster Token, is a card in EDS:
 * kinds 1-3 map to card ID 0, and larger kinds finish without a token.
 *   step 0: scroll the field to player 0's monster row;
 *   step 1: place the token, redraw, finish.
 */
void DuelCmd_SummonToken(void)
{
    struct DuelCard card;   /* bits 19-31 are not set */
    u32 player = CMD_PLAYER();
    u8 zoneIdx = gDuelCmd.arg2;
    u16 kind = gDuelCmd.arg4;
    u16 defense;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, DUEL_AREA_MONSTER);
        gDuelCmd.step++;
        return;
    }
    switch (kind) {
    case TOKEN_KIND_INSECT_MONSTER:
        defense = 0;
        break;
    case TOKEN_KIND_1:
        defense = 1;
        break;
    case TOKEN_KIND_2:
        defense = 1;
        break;
    case TOKEN_KIND_3:
        defense = 0;
        break;
    default:
        gDuelCmd.running = 0;
        return;
    }
    card.id = CardNumberToId(kind + CARD_INSECT_MONSTER_TOKEN);
    card.owner = player;
    card.unk13 = player;
    card.unk14 = 1;
    card.normalSummoned = 0;
    card.specialSummoned = 1;
    card.planted = 0;
    card.graverobbed = 0;
    PlaceMonsterCard(player, zoneIdx, &card, defense, 1);
    ZoneAt(player & 1, zoneIdx)->positionLocked = 1;
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/* 0xA4 (battle: Relinquished keeps its card when the monster it absorbed is destroyed in its place): writes
 * card word arg4 | arg6 << 16 back into the acting player's zone (u8)arg2 and redraws the field. */
void DuelCmd_SetZoneCardWord(void)
{
    u32 player = CMD_PLAYER();
    u8 zoneIdx = gDuelCmd.arg2;
    u32 card = (gDuelCmd.arg6 << 16) | gDuelCmd.arg4;

    CopyDuelCard((u32 *)&gDuelPlayers[player].zones[zoneIdx].card, &card);
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/*
 * 0xA7 (nothing queues it in EDS): moves the acting player's monster from zone arg2 to zone arg4 and sets
 * it face down in defense position.
 *   step 0: copy the whole zone record, empty the source, smoke puff at the destination, clear the source
 *           cell;
 *   step 1: redraw the field and finish.
 */
void DuelCmd_MoveMonsterFaceDown(void)
{
    struct DuelLoc loc;
    struct DuelZone *zones;
    u32 player = CMD_PLAYER();
    u16 from = gDuelCmd.arg2;
    u16 to = gDuelCmd.arg4;

    switch (gDuelCmd.step) {
    case 0:
        zones = gDuelZones[player & 1].zones;
        MemCopy16(zones + to, zones + from, sizeof(struct DuelZone));
        ZoneAt(player & 1, to)->isFaceUp = 0;
        ZoneAt(player & 1, to)->isDefense = 1;
        ZoneAt(player & 1, from)->card.id = 0;
        PlaySE(16);
        loc.player = player;
        loc.area = DUEL_AREA_MONSTER;
        loc.index = to;
        loc.isDefense = 1;
        loc.isFaceUp = 0;
        DuelAnim_PlayZoneEffect(&loc, (u32)gSmokePuffAnim, 0, 0);
        ClearZoneTiles(player, from);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
