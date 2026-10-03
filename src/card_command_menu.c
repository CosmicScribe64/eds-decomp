/*
 * The card command menu, part 1: the handlers of the commands that the menu confirms, and the builders of the
 * command masks for a hand card, a monster and a Magic/Trap on the field (wiki/functions/card-command-menu-c.md).
 *
 * CardMenu_Execute (duel_cmd_queue.c) runs the confirmed command of gDuel.cardMenu: Flip, Def Pos / Atk Pos,
 * Set / Activate of a Magic or Trap from the hand, and Fusion (CardMenu_FlipSummon, CardMenu_ChangePosition,
 * CardMenu_PlaySpellTrapFromHand, CardMenu_FusionSummon). Multi-step ones count gDuel.cardMenu.step; every
 * handler clears gDuel.cardMenu.confirmed when it is done. The CPU fills gDuel.cardMenu itself and uses the same
 * handlers, and Chain_AskResponse calls CardMenu_PlaySpellTrapFromHand to answer a chain link.
 *
 * CardMenu_GetAvailableCommands (card_menu_input.c) calls CardMenu_GetHandCardCommands,
 * CardMenu_GetMonsterCommands and CardMenu_GetSpellTrapCommands when the menu opens. They return a mask of
 * enum CardMenuCommandMask bits and carry the per-card rules (Spirit Message letters, Cocoon of Evolution,
 * Thunder Dragon, Dragon Capture Jar, Spellbinding Circle, ...); CanActivateMonsterEffect is the rule for the
 * Activate command of a face-up monster.
 *
 * Commands are only offered to player 0: the CPU decides on its own.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_SUBTYPE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, SpellSubtype */
#include "constants/duel.h"         /* CardMenuCommandMask, DuelPhase, DuelZoneIndex, ChainEntryKind, ... */
#include "constants/duel_cmds.h"    /* DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h (build/readability/hcheck/duel_core/staged/duel.h)
 * that this unit and the headers below use, with its names, types and bitfield containers (unused bytes are
 * padding), and defines duel.h's include guard so that the headers below do not pull in the legacy one.
 * After H0, replace the block (BEGIN to END) with #include "duel.h". */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:5;
    u32 graverobbed:1;              /* bit 18: taken with Graverobber; cleared when it leaves the field */
    u32 unk19:13;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9 */
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet */
    u8 unk7_6:2;
    u8 unk8[0x91 - 0x8];
    u8 canActivate:1;
    u8 unk91_3:7;
    u8 unk92[0x94 - 0x92];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 unk5[2];
    u8 unk7_0:5;
    u8 positionChangeLocked:1;      /* +0x007 bit 5: blocks the DEF/ATK position and Flip commands */
    u8 magicTrapLockTurns:2;        /* +0x007 bits 6-7: nonzero blocks Magic/Trap activation */
    u8 unk8_0:4;
    u8 normalSummonUsed:1;          /* +0x008 bit 4: the Normal Summon of this turn is done */
    u8 unk8_5:3;
    u8 unk9_0:5;
    u8 magicTrapActivatedThisTurn:1;/* +0x009 bit 5: a Magic/Trap was activated from zones 5-10 */
    u8 unk9_6:2;
    u8 unkA[0x26 - 0xA];
    u16 attackedMask;               /* +0x026: monster zones that have attacked this turn */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};

/* The card command menu (gDuel.cardMenu, 0xC bytes). */
struct CardMenu {
    u16 open:1;                     /* bit 0: menu open, the caller runs CardMenu_Update */
    u16 confirmed:1;                /* bit 1: a command was chosen (the AI sets it directly) */
    u16 command:4;                  /* bits 2-5: enum CardMenuCommand */
    u16 slide:4;                    /* bits 6-9 */
    u32 available:16;               /* bits 10-25: enum CardMenuCommandMask bits */
    u32 state:8;                    /* bits 26-33 */
    u32 step:8;                     /* bits 34-41: step of the command handler */
    u8 summonSeq:4;                 /* bits 42-45 */
    u32 tributeSources:4;           /* bits 46-49 */
    u16 timer:7;                    /* bits 50-56 */
    u16 player:1;                   /* bit 57: player of the confirmed command */
    u32 area:7;                     /* bits 58-64: enum DuelArea of the cursor at confirm */
    u32 index:8;                    /* bits 65-72: zone index (field) or hand index (hand) */
    u32 placeZone:8;                /* bits 73-80: spell/trap zone chosen by CardMenu_PlaySpellTrapFromHand */
    u32 unk0A_1:15;
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
    u8 result:2;                    /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardMenuCard;               /* +0x1B28: card ID under the cursor when the command was confirmed */
    u16 summonTributes;             /* +0x1B2A */
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[0x1B78 - 0x1B38];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u32 IsSpecialSummonOnly(u16 cardId);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountMonsters(int player);
int CountFreeMonsterZones(int player);
int CountTributableMonsters(int player, int excludeZone);
u16 FindAbsorbedMonsterLink(int player, int zone);
int FindFreeSpellTrapZone(int player);
int CanPlaceSpellTrapCard(int player, u16 cardId);
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
u32 GetZoneCardType(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "battle.h"                 /* CanMonsterAttack */
#include "card_menu.h"              /* the CardMenu_* functions defined here, CanActivateMonsterEffect */
#include "chain.h"                  /* struct ChainEntry, gChain, Chain_AddLink, Chain_AddPending */
#include "duel_actions.h"           /* ChangeBattlePosition, DestroyFieldCard, ShowCardEffect, LoseLifePoints */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_screen.h"            /* DuelCursor_Select */
#include "effect.h"                 /* CanActivateEffectOfCard / InZone, CanCardTargetZone, PayChainEnergyCost */
#include "effect_handlers.h"        /* EffectPolymerizationResolve */
#include "summon.h"                 /* QueueFlipSummon, CanNormalSummon, CanSpecialSummon, CanSummonFromHand */

/* ROM data used only here. */
/* Matching: card IDs read through their alias symbols (= &gCardNumberToId[number], see card_data.h). */
extern const u16 gUnk_0862467A;                 /* CARD_GRAVEROBBER: Graverobber's card ID */
extern const u16 gUnk_08624A0A[];               /* CARD_1547 (0 in EDS): the Fusion Gate-like key the Fusion command resolves */

/* Matching: the card tables through their integer addresses (not the gCardStats / gCardIdToNumber symbols:
 * different literal-pool entries). */
#define CARD_NUMBER_AT(index)   (((const u16 *)0x08622AB4)[(index)])   /* the same, for an index already masked */
#define CARD_NUMBER_OF(id)      CARD_NUMBER_AT((id) & CARD_ID_MASK)
#define CARD_STATS_OF(id)       CARD_STATS_AT((id) & CARD_ID_MASK)
#define CARD_STATS_AT(index)    (((const u32 *)0x08621DE0)[(index)])   /* the same, for an index already masked */

/* Byte offsets in gDuel: the zones of player 0, and a zone from there (zone * zone size + player * stride). */
#define ZONES_OFFSET            OFFSET_OF(struct DuelState, players[0].zones)
#define SPELL_ZONES_OFFSET      OFFSET_OF(struct DuelState, players[0].zones[ZONE_SPELL_0])
#define FIELD_ZONE_OFFSET       OFFSET_OF(struct DuelState, players[0].zones[ZONE_FIELD])
#define CARD_MENU_PLACE_ZONE_OFFSET (OFFSET_OF(struct DuelState, cardMenu) + 8)   /* the word with index and placeZone */
#define ZONE_OFFSET(player, zone) ((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer))

/*
 * Matching: the card menu (gDuel + 0x1B2C) as the handlers below read it, through a cast of &gDuel (so the
 * addresses are formed like those of the other gDuel fields), with the containers that the ROM uses instead of
 * struct CardMenu's: the open/confirmed flags in a byte, the step in a halfword (at4.s), the player in a byte
 * (at4.b, at4.v) and in a word (at4.w), and the index in a halfword (at8.h).
 */
struct CardMenuView {
    u8 unk0[0x1B2C];
    u8 unk0_0:1;                        /* +0x1B2C: open */
    u8 confirmed:1;                     /* bit 1 */
    u8 unk0_2:6;
    u8 unk1[3];
    union {                             /* +0x1B30 */
        struct { u16 unk0:2; u16 step:8; u16 unk10:6; } s;              /* cardMenu.step */
        struct { u32 unk0:25; u32 player:1; u32 unk26:6; } w;           /* cardMenu.player, word view */
        struct { u8 pad[3]; u8 unk0:1; u8 player:1; u8 unk2:6; } b;     /* cardMenu.player, byte view */
        struct { u8 pad[3]; u8 value; } v;                              /* the byte at +0x1B33 */
    } at4;
    union {                             /* +0x1B34 */
        struct { u16 unk0:1; u16 index:8; u16 unk9:7; } h;              /* cardMenu.index */
    } at8;
};
#define MENU_VIEW           (*(struct CardMenuView *)&gDuel)
#define MENU_STEP_H         (MENU_VIEW.at4.s.step)
#define MENU_PLAYER_B       (MENU_VIEW.at4.b.player)
#define MENU_PLAYER_W       (MENU_VIEW.at4.w.player)
#define MENU_PLAYER_BYTE    (MENU_VIEW.at4.v.value)
#define MENU_INDEX_H        (MENU_VIEW.at8.h.index)
/* The same through a base pointer e = (u8 *)&gDuel (the other handlers). */
#define MENU_FLAGS(e)       ((struct CardMenuView *)(e))
#define MENU_STEP(e)        (((struct CardMenuView *)(e))->at4.s.step)
#define MENU_PLAYER(e)      (((struct CardMenuView *)(e))->at4.b.player)
#define MENU_INDEX(e)       (((struct CardMenuView *)(e))->at8.h.index)

/* The word of gDuel.cardMenu with index and placeZone (+0x1B34), to store placeZone through its own address. */
struct MenuPlaceZoneWord {
    u32 unk0:9;
    u32 placeZone:8;
    u32 unk17:15;
};
/* FAKEMATCH: the ROM extracts the second zone through r2 before masking it into r1. */
#define PLACE_ZONE_R2 ({ register u32 zone_ asm("r2") = gDuel.cardMenu.placeZone; asm("" : "+r"(zone_)); zone_; })

/* Low and high nibble of a byte as the DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND operand packs them. */
static inline int LowNibble(u8 x) { return x & 0xF; }
static inline int HighNibble(u8 x) { return (x & 0xF) << 4; }
static inline void ShowGraverobber(u8 player) { ShowCardEffect(player, gUnk_0862467A); }
static inline u32 ZoneCardWord(u8 player, u8 zone) { return *(u32 *)&gDuel.players[player].zones[zone].card; }

/* Flip command: queue the Flip Summon of the monster in (cardMenu.player, cardMenu.index), lock its position
 * (positionLocked) and clear confirmed. */
void CardMenu_FlipSummon(void)
{
    u8 *e = (u8 *)&gDuel;
    struct DuelZone *z;
    u32 player, zone;
    int offset;
    u8 *zones;

    QueueFlipSummon(MENU_PLAYER(e), MENU_INDEX(e));
    player = MENU_PLAYER(e);
    zone = MENU_INDEX(e);
    offset = ZONE_OFFSET(player, zone);
    zones = e + ZONES_OFFSET;
    z = (struct DuelZone *)(offset + (int)zones);
    z->positionLocked = 1;
    MENU_FLAGS(e)->confirmed = 0;
}

/*
 * Place the hand Magic/Trap gDuel.cardMenuCard (hand index cardMenu.index, player cardMenu.player): Set it, or
 * Activate it when `activate` is set. Shared by the card menu, the CPU (which fills gDuel.cardMenu first) and
 * Chain_AskResponse (asChainLink = 1, trigger = the chain link being answered). A step machine on
 * cardMenu.step:
 *   0        placeZone = FindFreeSpellTrapZone(player). A Field Magic (Magic, subtype FIELD) instead destroys
 *            both players' Field Magic (DestroyFieldCard(p, ZONE_FIELD, 0)) and uses ZONE_FIELD. Push
 *            DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND (arg4: zone | hand index << 4 | activate << 8), pay
 *            PayChainEnergyCost
 *   1        DuelCursor_Select(player, 0, placeZone)
 *   default  when activating: Graverobber's cost (the card word of the zone is graverobbed and owned by the
 *            other player: ShowCardEffect + LoseLifePoints 2000), then the chain entry: Chain_AddLink when
 *            answering a link, else Chain_AddPending to start a new chain. The word is player << 31 |
 *            zone << 16 | card, plus event << 25 and locs of the trigger when there is one. Always:
 *            magicTrapActivatedThisTurn after the Standby Phase (also for a Set), then confirmed is cleared.
 *
 * Matching: the player is read through the byte view (at4.b), the byte (at4.v) and the word (at4.w) of
 * the card menu, as the ROM does; placeZone is stored through its own address (a direct member store computes
 * it after the call).
 */
void CardMenu_PlaySpellTrapFromHand(u16 activate, u16 asChainLink, struct ChainEntry *trigger)
{
    switch (MENU_STEP_H) {
    case 0:
        ((struct MenuPlaceZoneWord *)((u8 *)&gDuel + CARD_MENU_PLACE_ZONE_OFFSET))->placeZone =
            (u16)FindFreeSpellTrapZone(MENU_PLAYER_B);
        {
            u32 stats = CARD_STATS_OF(gDuel.cardMenuCard);

            /* A Field Magic replaces the Field Magic of both players. */
            if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) == SPELL_FIELD) {
                int i;

                for (i = 0; i <= 1; i++) {
                    int pp = i ? 1 - MENU_PLAYER_B : MENU_PLAYER_B;

                    if ((*(u32 *)((u8 *)&MENU_VIEW + FIELD_ZONE_OFFSET + (pp & 1) * sizeof(struct DuelPlayer)) << 20) != 0)
                        DestroyFieldCard(pp, ZONE_FIELD, 0);
                }
                gDuel.cardMenu.placeZone = ZONE_FIELD;
            }
        }
        DuelCmd_Push((2 & MENU_PLAYER_BYTE) ? DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND
                                            : DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND,
                     gDuel.cardMenuCard,
                     HighNibble(MENU_INDEX_H) | LowNibble(gDuel.cardMenu.placeZone) | (1 & activate) << 8, 0);
        PayChainEnergyCost(MENU_PLAYER_B);
        MENU_STEP_H++;
        break;
    case 1:
        DuelCursor_Select(MENU_PLAYER_B, 0, gDuel.cardMenu.placeZone);
        MENU_STEP_H++;
        break;
    default:
        if (activate) {
            /* Graverobber's cost: a graverobbed card word (bit 18) owned by the other player. */
            if ((int)(ZoneCardWord(1 & MENU_PLAYER_B, gDuel.cardMenu.placeZone) << 13) < 0) {
                if (((ZoneCardWord(1 & MENU_PLAYER_B, gDuel.cardMenu.placeZone) << 19) >> 31) != MENU_PLAYER_W) {
                    /* FAKEMATCH: the loop note ends the cse block, so the next argument is reloaded. */
                    do {
                        ShowGraverobber(MENU_PLAYER_W);
                    } while (0);
                    LoseLifePoints(MENU_PLAYER_B, 2000);
                }
            }
            if (asChainLink) {
                Chain_AddLink(MENU_PLAYER_B << 31 | trigger->event << 25
                              | (0x1F & PLACE_ZONE_R2) << 16 | gDuel.cardMenuCard,
                              trigger->loc1 << 16 | trigger->loc0);
            } else if (trigger == NULL) {
                Chain_AddPending(MENU_PLAYER_B << 31 | (0x1F & PLACE_ZONE_R2) << 16 | gDuel.cardMenuCard, 0);
            } else {
                Chain_AddPending(MENU_PLAYER_B << 31 | trigger->event << 25
                                 | (0x1F & PLACE_ZONE_R2) << 16 | gDuel.cardMenuCard,
                                 trigger->loc1 << 16 | trigger->loc0);
            }
        }
        if (gDuel.phase > PHASE_STANDBY) {
            struct DuelPlayer *players = (struct DuelPlayer *)((u8 *)&MENU_VIEW + OFFSET_OF(struct DuelState, players));

            players[MENU_PLAYER_B].magicTrapActivatedThisTurn = 1;
        }
        MENU_VIEW.confirmed = 0;
        break;
    }
}

/* Def Pos / Atk Pos commands: change the position of the monster in (cardMenu.player, cardMenu.index) (command 1
 * or 2; any other value only locks), lock its position (positionLocked) and clear confirmed. */
void CardMenu_ChangePosition(u16 command)
{
    u8 *e;
    struct DuelZone *z;
    u32 player, zone;
    int offset;
    u8 *zones;

    switch (command) {
    case CARDMENU_CMD_DEF_POS:
    case CARDMENU_CMD_ATK_POS: {
        u8 *e1 = (u8 *)&gDuel;
        ChangeBattlePosition(MENU_PLAYER(e1), MENU_INDEX(e1), 0, 0);
    }
    }
    e = (u8 *)&gDuel;
    player = MENU_PLAYER(e);
    zone = MENU_INDEX(e);
    offset = ZONE_OFFSET(player, zone);
    zones = e + ZONES_OFFSET;
    z = (struct DuelZone *)(offset + (int)zones);
    z->positionLocked = 1;
    MENU_FLAGS(e)->confirmed = 0;
}

/*
 * Fusion command (on the Fusion Deck): run Polymerization's resolve handler as the card key 1547 (not an EDS
 * card, so CardMenu_GetAvailableCommands never offers the command in this ROM). Step 0 starts gChain.effectStep at
 * 0x80 (EFFECT_STEP_START) and falls into step 1, which runs EffectPolymerizationResolve on a stack ChainEntry
 * every frame, stores its result in gChain.effectStep and advances when it returns EFFECT_STEP_DONE. Later steps
 * clear confirmed.
 */
void CardMenu_FusionSummon(void)
{
    struct ChainEntry ref;
    u8 *e = (u8 *)&gDuel;

    switch (MENU_STEP(e)) {
    case 0:
        gChain.effectStep = 0x80;
        MENU_STEP(e)++;
        /* fall through */
    case 1: {
        u16 id = gUnk_08624A0A[0];

        ref.card = id;
        ref.player = 0;
        ref.negated = 0;
        gChain.effectStep = EffectPolymerizationResolve(&ref, 0);
        if (gChain.effectStep == EFFECT_STEP_DONE) {
            u8 *e2 = (u8 *)&gDuel;   /* a fresh base: reusing e keeps it live across the call */

            MENU_STEP(e2)++;
        }
        break;
    }
    default:
        MENU_FLAGS(e)->confirmed = 0;
        break;
    }
}


/*
 * Command mask for the hand card `id` of `player` (the menu's SET / SUMMON / ACTIVATE / SP_SUMMON bits).
 * Only in Main Phase 1 and 2:
 *   Magic     SET when CanPlaceSpellTrapCard, plus ACTIVATE when CanActivateEffectOfCard(player, id, 1); the
 *             Spirit Message letters (keys 1541-1544) cannot be Set; key 1312 loses ACTIVATE once a Magic/Trap was
 *             activated or the Normal Summon was used
 *   Trap      SET when CanPlaceSpellTrapCard
 *   Monster   when CanSummonFromHand accepts it: Special Summon only -> SP_SUMMON | SP_SUMMON_SET, else SET |
 *             SUMMON while the Normal Summon is unused. La Jinn (with key 1243 face up), key 1520 and key 1350
 *             (fewer monsters than the opponent and a free zone; no tributable monster drops SET | SUMMON) get the
 *             SP bits. The SP bits need CanSpecialSummon(0), SUMMON needs CanNormalSummon(0). Cocoon of Evolution
 *             (free zone, Normal Summon unused) and Thunder Dragon get ACTIVATE.
 * On the player's own turn in any phase a Quick-Play Magic gets ACTIVATE. Anti-Magic Fragrance on either field
 * removes ACTIVATE from a Magic; a locked player (magicTrapLockTurns) loses SET and ACTIVATE of Magic and Trap.
 *
 * The ROM returns an int that the callers OR into a u16.
 *
 * Matching: the explicit `(u16)(flags | MASK)` forms and the one plain `flags |= SP_SUMMON | SP_SUMMON_SET` (the
 * Special Summon branch) are not interchangeable: the ROM keeps the lsl/lsr truncation after every OR except
 * that one. The unused `ref` only reserves the ROM's 0x14-byte stack slot.
 */
int CardMenu_GetHandCardCommands(u16 id, int player)
{
    u16 flags;
    u8 *e;
    struct ChainEntry ref; /* FAKEMATCH: unused, but the ROM reserves its 0x14-byte stack slot */
    u32 n;
    u8 *pb;
    int t;

    flags = 0;
    e = (u8 *)&gDuel;
    switch (gDuel.phase) {
    case PHASE_MAIN1:
    case PHASE_MAIN2:
        n = CARD_ID_MASK & id;
        switch (CARD_STATS_TYPE(CARD_STATS_AT(n))) {
        case CARD_TYPE_MAGIC:
            if (CanPlaceSpellTrapCard(player, id) == 0)
                break;
            if (CanActivateEffectOfCard(player, id, 1) != 0)
                flags = CARDMENU_MASK_ACTIVATE;
            flags = (u16)(flags | CARDMENU_MASK_SET);
            switch (CARD_NUMBER_AT(n)) {
            case CARD_1541:
            case CARD_1542:
            case CARD_1543:
            case CARD_1544:
                flags &= (u16)~CARDMENU_MASK_SET;
                break;
            case CARD_1312:
                {
                    /* Matching: the base of the players first, as a separate term (it also fixes the
                     * later reload registers). */
                    u8 *players = e + OFFSET_OF(struct DuelState, players);
                    int stride = (player & 1) * sizeof(struct DuelPlayer);
                    pb = (u8 *)(stride + (int)players);
                }
                /* Byte tests of the flag bytes: pb[9] bit 5 is magicTrapActivatedThisTurn, pb[8] bit 4
                 * normalSummonUsed (bitfield tests compile to other instructions). */
                if ((pb[9] << 26) < 0 || (pb[8] << 27) < 0)
                    flags &= (u16)~CARDMENU_MASK_ACTIVATE;
                break;
            }
            break;
        case CARD_TYPE_TRAP:
            if (CanPlaceSpellTrapCard(player, id) != 0)
                flags = CARDMENU_MASK_SET;
            break;
        default:
            if (CanSummonFromHand(player, id) != 0) {
                if (IsSpecialSummonOnly(id) != 0) {
                    flags |= CARDMENU_MASK_SP_SUMMON | CARDMENU_MASK_SP_SUMMON_SET;
                    if ((u16)CanSpecialSummon(0) == 0)
                        flags = 0;
                } else {
                    if (!gDuel.players[player & 1].normalSummonUsed)
                        flags = CARDMENU_MASK_SET | CARDMENU_MASK_SUMMON;
                    switch (CARD_NUMBER_AT(n)) {
                    case CARD_LA_JINN_THE_MYSTICAL_GENIE_OF_THE_LAMP:
                        if (CountFaceUpMonstersByNumber(player, CARD_1243) <= 0)
                            break;
                        /* fall through */
                    case CARD_1520:
                        flags = (u16)(flags | CARDMENU_MASK_SP_SUMMON | CARDMENU_MASK_SP_SUMMON_SET);
                        break;
                    case CARD_1350:
                        if (CountMonsters(0) + 1 < CountMonsters(1) && CountFreeMonsterZones(0) > 0)
                            flags = (u16)(flags | CARDMENU_MASK_SP_SUMMON | CARDMENU_MASK_SP_SUMMON_SET);
                        if (CountTributableMonsters(player, -1) == 0)
                            flags &= (u16)~(CARDMENU_MASK_SET | CARDMENU_MASK_SUMMON);
                        break;
                    }
                }
                if ((u16)CanSpecialSummon(0) == 0)
                    flags &= (u16)~(CARDMENU_MASK_SP_SUMMON | CARDMENU_MASK_SP_SUMMON_SET);
                if ((u16)CanNormalSummon(0) == 0)
                    flags &= (u16)~CARDMENU_MASK_SUMMON;
            }
            switch (CARD_NUMBER_OF(id)) {
            case CARD_COCOON_OF_EVOLUTION:
                if (CanPlaceSpellTrapCard(player, id) != 0 && CanActivateEffectOfCard(player, id, 1) != 0
                    && CountFreeMonsterZones(player) > 0 && !gDuelPlayers[player & 1].normalSummonUsed)
                    flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
                break;
            case CARD_THUNDER_DRAGON:
                if (CanActivateEffectOfCard(player, id, 1) != 0)
                    flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
                break;
            }
            break;
        }
        break;
    }
    if (gDuel.turnPlayer == 0) {
        u32 stats = CARD_STATS_OF(id);
        /* Quick-Play Magic can be activated in any phase of the player's own turn. */
        if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) == SPELL_QUICK_PLAY
            && CanPlaceSpellTrapCard(player, id) != 0 && CanActivateEffectOfCard(player, id, 1) != 0)
            flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
    }
    if (CARD_STATS_TYPE(CARD_STATS_OF(id)) == CARD_TYPE_MAGIC && (flags & CARDMENU_MASK_ACTIVATE) != 0) {
        if (CountActiveCardsOnField(0, CARD_ANTI_MAGIC_FRAGRANCE) > 0)
            flags &= (u16)~CARDMENU_MASK_ACTIVATE;
        if (CountActiveCardsOnField(1, CARD_ANTI_MAGIC_FRAGRANCE) > 0)
            flags &= (u16)~CARDMENU_MASK_ACTIVATE;
    }
    t = CARD_STATS_TYPE(CARD_STATS_OF(id));
    switch (t) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        if (gDuelPlayers[player & 1].magicTrapLockTurns != 0) {
            flags &= (u16)~CARDMENU_MASK_SET;
            flags &= (u16)~CARDMENU_MASK_ACTIVATE;
        }
        break;
    }
    return flags;
}

/* Matching: byte view of gDuel; indexing the array keeps the base and the offset 0x1B12 in separate registers. */
extern u8 gDuelBytes[] asm("gDuel");

/* Matching: FindAbsorbedMonsterLink, CanCardTargetZone, CanActivateEffectInZone and CanMonsterAttack are defined
 * with u16 results (duel.h, effect.h, battle.h); these functions compare and return them as ints, without
 * narrowing r0. */
extern int FindAbsorbedMonsterLinkInt(int player, int zone) asm("FindAbsorbedMonsterLink");
extern int CanCardTargetZoneInt(u16 cardId, int player, int zone) asm("CanCardTargetZone");
extern int CanActivateEffectInZoneInt(int player, int zone, u16 event) asm("CanActivateEffectInZone");
extern int CanMonsterAttackInt(int player, int zone, int checkCost) asm("CanMonsterAttack");

/* The card words of a player's deck and graveyard as plain u32 (the ROM reads them through a byte address). */
#define DECK_CARD_WORD(player, i) \
    (((u32 *)((u8 *)gDuelPlayers + OFFSET_OF(struct DuelPlayer, deck) + ((player) & 1) * sizeof(struct DuelPlayer)))[i])
#define GRAVEYARD_CARD_WORD(player, i) \
    (((u32 *)((u8 *)gDuelPlayers + OFFSET_OF(struct DuelPlayer, graveyard) + ((player) & 1) * sizeof(struct DuelPlayer)))[i])

/* The card type of a card ID; the u16 parameter narrows the argument like the ROM's helper does. */
static inline int CardTypeOfId(u16 id)
{
    return CARD_STATS_TYPE(CARD_STATS_AT(id & CARD_ID_MASK));
}

/*
 * Can the face-up monster `id` in (player, zone) use its activated effect now? This is the ACTIVATE bit of the
 * monster menu. By card number:
 *   Catapult Turtle, Cannon Soldier     need a tributable monster (CountTributableMonsters(player, -1) > 0)
 *   The Little Swordsman of Aile        needs one other than itself
 *   Time Wizard, Monster Eye, Cyber-Stein, Goddess of Whim, Gale Dogra, Barrel Dragon, Valkyrion the Magna
 *   Warrior, Karate Man, keys 1430-1444 and 1510     CanActivateEffectInZone(player, zone, 0)
 *   Blast Juggler, Patrol Robo, Jigen Bakudan        only in the Standby Phase (event 2)
 *   Red-Eyes B. Dragon, Zoa             equipped with Metalmorph and with Red-Eyes Black Metal Dragon /
 *                                       Metalzoa in the deck (not while key 1418 is on a field)
 *   Relinquished, key 1334              nothing absorbed, a free spell/trap zone and an opponent monster that it
 *                                       can target: the zone's effectUnused bit
 *   key 1513                            effectUnused and a monster in the graveyard
 * Everything else returns 0.
 *
 * Matching: the unused ChainEntry only reserves the ROM's 0x14-byte stack slot; its two stores stay.
 */
int CanActivateMonsterEffect(u16 id, int player, int zone)
{
    struct ChainEntry ref;
    u32 effectUnused;
    int i;
    int target;

    effectUnused = ((struct DuelZone *)((player & 1) * sizeof(struct DuelPlayer) + zone * sizeof(struct DuelZone)
                                        + (int)((u8 *)gDuelPlayers + OFFSET_OF(struct DuelPlayer, zones))))->effectUnused;
    ref.player = player;
    ref.card = id;

    switch (CARD_NUMBER_OF(id)) {
    case CARD_CATAPULT_TURTLE:
    case CARD_CANNON_SOLDIER:
        return CountTributableMonsters(player, -1) > 0;
    case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
        return CountTributableMonsters(player, zone) > 0;

    case CARD_TIME_WIZARD:
    case CARD_MONSTER_EYE:
    case CARD_CYBER_STEIN:
    case CARD_GODDESS_OF_WHIM:
    case CARD_GALE_DOGRA:
    case CARD_BARREL_DRAGON:
    case CARD_VALKYRION_THE_MAGNA_WARRIOR:
    case CARD_KARATE_MAN:
    case CARD_1430:
    case CARD_1432:
    case CARD_1433:
    case CARD_1439:
    case CARD_1442:
    case CARD_1444:
    case CARD_1510:
        return CanActivateEffectInZone(player, zone, 0);

    case CARD_BLAST_JUGGLER:
    case CARD_PATROL_ROBO:
    case CARD_JIGEN_BAKUDAN:
        if ((gDuelBytes[0x1B12] & 0x1C) != PHASE_STANDBY << 2)   /* gDuel.phase != PHASE_STANDBY */
            return 0;
        return CanActivateEffectInZone(player, zone, RESPONSE_OWN_STANDBY);

    case CARD_RED_EYES_B_DRAGON:
    case CARD_ZOA:
        if (CountZoneLinksFromCard(player, zone, CARD_METALMORPH) == 0)
            return 0;
        target = 0;
        switch (CARD_NUMBER_OF(id)) {
        case CARD_RED_EYES_B_DRAGON:
            target = CARD_RED_EYES_BLACK_METAL_DRAGON;
            break;
        case CARD_ZOA:
            target = CARD_METALZOA;
            break;
        }
        if (target <= 0)
            return 0;
        for (i = 0; i < gDuelPlayers[player & 1].deckCount; i++) {
            if (CARD_NUMBER_OF((DECK_CARD_WORD(player, i) << 20) >> 20) == target) {
                if (CountActiveCardsOnField(0, CARD_1418) > 0)
                    return 0;
                if (CountActiveCardsOnField(1, CARD_1418) > 0)
                    return 0;
                return 1;
            }
        }
        return 0;

    case CARD_RELINQUISHED:
    case CARD_1334:
        if (FindAbsorbedMonsterLinkInt(player, zone) != 0xFFFF)
            return 0;
        if (FindFreeSpellTrapZone(player) == -1)
            return 0;
        for (i = 0; i <= 4; i++) {
            if (CanCardTargetZoneInt(id, 1 - player, i) != 0)
                return effectUnused;
        }
        return 0;

    case CARD_1513:
        if (effectUnused == 0)
            return 0;
        for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
            /* A monster (type 1-20) in the graveyard. */
            if ((u32)CardTypeOfId((GRAVEYARD_CARD_WORD(player, i) << 20) >> 20) <= CARD_TYPE_REPTILE)
                return 1;
        }
        return 0;

    default:
        return 0;
    }
}

/* Matching: the card ID of a zone (11 bits) from the whole card word (ldr), not from the halfword. */
#define ZONE_CARD_ID(zone) ((*(u32 *)&(zone)->card << 21) >> 21)

/* Matching: the flag bytes of a zone, +6 (isDefense bit 0, isFaceUp bit 1) and +0x91 (canActivate bit 2). The
 * ROM tests them by masking or shifting the byte; the struct DuelZone bitfields compile to other instructions. */
struct ZoneFlagBytes {
    u8 unk0[6];
    u8 flags6;
    u8 unk7[0x91 - 7];
    u8 flags91;
    u8 unk92[0x94 - 0x92];
};
struct ZoneFlagBytesPlayer {
    struct ZoneFlagBytes zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* Matching: the zone of (player, zone) of gDuelZones, with the player stride first and a plain u32 base. */
#define ZONE_VIA_BASE(player, zone) \
    ((struct DuelZone *)(sizeof(struct DuelZone) * (zone) + sizeof(struct DuelPlayer) * ((player) & 1) + (u32)gDuelZones))

/*
 * Command mask for a monster of player 0 (player 1 gets 0: the human cannot command the CPU's monsters, who only
 * get Card View from the caller). By phase:
 *   Standby    ACTIVATE for a face-up Blast Juggler / Patrol Robo / Jigen Bakudan when
 *              CanActivateEffectInZone(.., RESPONSE_OWN_STANDBY)
 *   Main 1/2   unless the zone is positionLocked, the player positionChangeLocked, the monster held by
 *              Spellbinding Circle or key 1244, or it is the Dragon that a Dragon Capture Jar holds:
 *              face up -> ATK_POS in defense / DEF_POS in attack; face down -> FLIP (needs CanNormalSummon(0))
 *              plus DEF_POS if in attack position; with key 1334 on a field only that card keeps these bits.
 *              Then ACTIVATE for a face-up monster when CanActivateMonsterEffect
 *   Battle     ATTACK when CanMonsterAttack(player, zone, 1) and the zone's attackedMask bit is clear
 */
u16 CardMenu_GetMonsterCommands(u16 id, int player, int zone)
{
    u16 cardId = id;
    int stride = sizeof(struct DuelPlayer) * (player & 1);
    u32 base = (u32)gDuelZones;
    struct DuelZone *z = (struct DuelZone *)(stride + base + sizeof(struct DuelZone) * zone);
    u16 flags = 0;
    int dragonHeld = 0;
    u8 *e;

    if (player != 0)
        return 0;
    /* A face-up Dragon with a Dragon Capture Jar on either field cannot change position. */
    if (ZONE_VIA_BASE(0, zone)->isFaceUp && GetZoneCardType(0, zone) == CARD_TYPE_DRAGON
        && (CountActiveCardsOnField(0, CARD_DRAGON_CAPTURE_JAR) != 0
            || CountActiveCardsOnField(1, CARD_DRAGON_CAPTURE_JAR) != 0))
        dragonHeld = 1;
    e = (u8 *)&gDuel;
    switch (gDuel.phase) {
    case PHASE_STANDBY: {
        int side = player & 1;
        int offset = ZONE_OFFSET(side, zone);
        u8 *zones = e + ZONES_OFFSET;
        struct DuelZone *zn = (struct DuelZone *)(offset + (int)zones);

        if (zn->isFaceUp) {
            u16 number = CARD_NUMBER_AT(ZONE_CARD_ID(zn));

            switch (number) {
            case CARD_BLAST_JUGGLER:
            case CARD_PATROL_ROBO:
            case CARD_JIGEN_BAKUDAN:
                if (CanActivateEffectInZoneInt(player, zone, RESPONSE_OWN_STANDBY) != 0)
                    flags |= CARDMENU_MASK_ACTIVATE;
                break;
            }
        }
        break;
    }
    case PHASE_MAIN1:
    case PHASE_MAIN2:
        if (!z->positionLocked && !gDuelPlayers[player & 1].positionChangeLocked
            && CountZoneLinksFromCard(player, zone, CARD_SPELLBINDING_CIRCLE) == 0
            && CountZoneLinksFromCard(player, zone, CARD_1244) == 0 && dragonHeld == 0) {
            if (z->isFaceUp) {
                if (z->isDefense)
                    flags = (u16)(flags | CARDMENU_MASK_ATK_POS);
                else
                    flags = (u16)(flags | CARDMENU_MASK_DEF_POS);
            } else {
                flags = (u16)(flags | CARDMENU_MASK_FLIP);
                if (!z->isDefense)
                    flags = (u16)(flags | CARDMENU_MASK_DEF_POS);
                if ((u16)CanNormalSummon(0) == 0)
                    flags &= (u16)~CARDMENU_MASK_FLIP;
            }
            /* While key 1334 is on a field, every other monster is locked in its position. */
            if (CountActiveCardsOnField(0, CARD_1334) > 0 || CountActiveCardsOnField(1, CARD_1334) > 0) {
                int side = player & 1;
                int offset = ZONE_OFFSET(side, zone);

                if (CARD_NUMBER_AT(ZONE_CARD_ID((struct DuelZone *)(offset + (u32)gDuelZones))) != CARD_1334)
                    flags &= (u16)~(CARDMENU_MASK_DEF_POS | CARDMENU_MASK_ATK_POS | CARDMENU_MASK_FLIP);
            }
        }
        {
            int side = player & 1;
            int offset = ZONE_OFFSET(side, zone);

            if (((struct DuelZone *)(offset + (u32)gDuelZones))->isFaceUp
                && (u16)CanActivateMonsterEffect(cardId, player, zone) != 0)
                flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
        }
        break;
    case PHASE_BATTLE:
        asm("" : "+r"(e)); /* FAKEMATCH: hide e's constant value from CSE so `e + side * 0xD64` keeps its operand order */
        if (CanMonsterAttackInt(player, zone, 1) != 0
            && !((((struct DuelState *)e)->players[player & 1].attackedMask >> zone) & 1))
            flags = (u16)(flags | CARDMENU_MASK_ATTACK);
        break;
    }
    return flags;
}

/*
 * CARDMENU_MASK_ACTIVATE or 0 for the card `id` of player 0 in spell/trap zone `zone` + 5 (zone 5 = the field
 * zone, 10) that is set or face up; needs canActivate. Player 1 gets 0.
 *   Magic   face down, and Quick-Play or a Main Phase of the player's own turn, and CanActivateEffectOfCard; in
 *           the Standby Phase also Inspection on the opponent's turn and a set Curse of Fiend on the player's own
 *   Trap    face down, or face up for Ultimate Offering and keys 1324 / 1428 / 1532; CanActivateEffectInZone
 * A player with magicTrapLockTurns loses ACTIVATE.
 */
int CardMenu_GetSpellTrapCommands(u16 id, int player, int zone)
{
    u8 *e;
    u32 flags;
    u32 n;
    u32 stats;
    int t;
    struct ZoneFlagBytesPlayer *pz = &((struct ZoneFlagBytesPlayer *)gDuelZones)[player & 1];
    struct ZoneFlagBytes *zn = &pz->zones[zone + ZONE_SPELL_0];

    flags = 0;
    if (player != 0)
        return 0;
    n = CARD_ID_MASK & id;
    stats = CARD_STATS_AT(n);
    switch (CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_MAGIC:
        if ((zn->flags91 & 4) != 0) {                   /* canActivate */
            if ((zn->flags6 << 30) >= 0) {              /* face down */
                int allowed = 0;

                if (CARD_STATS_SUBTYPE(stats) == SPELL_QUICK_PLAY)
                    allowed = 1;
                {
                    /* gDuel.turnPlayer / phase, through the gDuelZones symbol. */
                    struct DuelState *duel = (struct DuelState *)((u8 *)gDuelZones - ZONES_OFFSET);

                    if ((duel->phase == PHASE_MAIN1 || duel->phase == PHASE_MAIN2) && duel->turnPlayer == 0)
                        allowed = 1;
                }
                if (allowed != 0 && CanActivateEffectOfCard(player, id, 0) != 0)
                    flags |= CARDMENU_MASK_ACTIVATE;
            }
        }
        e = (u8 *)&gDuel;
        {
            struct DuelState *duel = (struct DuelState *)e;

            if (duel->phase == PHASE_STANDBY) {
                if (duel->turnPlayer) {
                    /* The opponent's Standby Phase: Inspection. */
                    if (CARD_NUMBER_AT(CARD_ID_MASK & id) == CARD_INSPECTION) {
                        if (CanActivateEffectInZoneInt(1 - player, zone + ZONE_SPELL_0, RESPONSE_OPPONENT_STANDBY) != 0)
                            flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
                    }
                } else {
                    /* The player's own Standby Phase: a set Curse of Fiend. */
                    if (CARD_NUMBER_AT(CARD_ID_MASK & id) == CARD_CURSE_OF_FIEND) {
                        int offset = (player & 1) * sizeof(struct DuelPlayer) + zone * sizeof(struct DuelZone);
                        u8 *zones = e + SPELL_ZONES_OFFSET;

                        if (!((struct DuelZone *)(offset + (int)zones))->isFaceUp) {
                            if (CanActivateEffectInZoneInt(player, zone + ZONE_SPELL_0, RESPONSE_OWN_STANDBY) != 0)
                                flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
                        }
                    }
                }
            }
        }
        break;
    case CARD_TYPE_TRAP:
        if ((zn->flags91 & 4) != 0) {                   /* canActivate */
            int faceUp = (u32)(zn->flags6 << 30) >> 31;

            /* Ultimate Offering and keys 1324 / 1428 / 1532 can also be activated face up. */
            switch (CARD_NUMBER_AT(n)) {
            case CARD_1324:
            case CARD_ULTIMATE_OFFERING:
            case CARD_1428:
            case CARD_1532:
                faceUp = 0;
                break;
            }
            if (faceUp == 0) {
                if (CanActivateEffectInZoneInt(player, zone + ZONE_SPELL_0, 0) != 0)
                    flags = (u16)(flags | CARDMENU_MASK_ACTIVATE);
            }
        }
        break;
    default:
        break;
    }
    t = CARD_STATS_TYPE(CARD_STATS_OF(id));
    switch (t) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        if (gDuelPlayers[player & 1].magicTrapLockTurns != 0)
            flags &= (u16)~CARDMENU_MASK_ACTIVATE;
        break;
    }
    return flags;
}
