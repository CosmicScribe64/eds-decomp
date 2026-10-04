/*
 * effect_hooks (0x08046738-0x08048FDF): passive card effects, the effect-table lookup, the summon permission
 * checks and the summon/tribute state machine of the card menu (wiki/functions/effect-hooks-c.md).
 *
 * The first sixteen functions are hooks that the duel flow calls at fixed events, not through gCardEffects:
 * they count the active copies of one card (CountActiveCardsOnField and friends), show the card
 * (ShowCardEffect) and then change the duel through the action functions or by queueing a duel command
 * (DuelCmd_Push). Pumpking, Mysterious Puppeteer, Dragon Capture Jar, Sinister Serpent, the Trap disabling of
 * Jinzo, Chain Energy, Kotodama, Appropriate and Forced Requisition are EDS cards; the others belong to
 * effect keys with no EDS card (1306, 1514, 1533, 1536, 1541-1544; see include/effect.h).
 *
 * Then come two stubs for gCardEffects rows (EffectNopResolve, EffectSpiritMessagePrepare), the row lookup
 * (FindCardEffect: a binary search of the 426 rows by card number) and its Check-handler wrapper
 * (CanEffectTargetZone), the Normal and Special Summon permission checks, and CardMenu_SummonMonster: the
 * state machine behind the card menu's Set, Summon and Special Summon commands for a hand card. It places a
 * low-level monster at once, asks for one or two Tributes for levels 5 and up, and runs the special
 * procedures of the moths, Wall Shadow, Gate Guardian, Valkyrion and keys 1257 and 1514-1519.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_*, gCardNames, gCardNumberToId */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute */
#include "constants/duel.h"         /* enum DuelZoneIndex, ZoneLinkKind, CardMenuCommand, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CONFIRM, SE_CANCEL, SE_ERROR */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md). This block declares the part of the canonical duel.h that this unit and the
 * headers below use, with the header's names, types and bitfield containers (unused bytes are padding), and
 * defines duel.h's include guard so that chain.h, card_list_view.h, duel_screen.h and summon.h do not pull in
 * the legacy header. After H0, replace the block (BEGIN to END) with
 *     #include "legacy/duel.h"
 *     #include "legacy/sound.h"
 * (see build/readability/issues/effect_hooks.md). */
#define GUARD_DUEL_H

/* A card in a zone or pile: one 32-bit word. id 0 is an empty slot. */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 planted:1;                  /* bit 17 */
    u32 graverobbed:1;              /* bit 18 */
    u32 unk19:1;
    u32 isFusionMaterial:1;         /* bit 20 */
    u32 destroyedInBattle:1;        /* bit 21 */
    u32 destroyedByOpponent:1;      /* bit 22 */
    u32 flag23:1;
    u32 pendingEquip:1;
    u32 equipZone:3;
    u32 pendingOpponentSummon:1;
    u32 unk29:3;
};

/* A card location on the duel screen, packed in 16 bits (gDuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

/* One field zone (0x94 bytes): zones 0-4 hold monsters, 5-9 spells and traps, 10 the Field Magic. */
struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5: turns a face-up card has been active */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

/* One player's side of the duel (0xD64 bytes; gDuelPlayers = gDuel.players). */
struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006: entries in banished[] and banishedInfo[] */
    u8 unk7_0:3;
    u8 noNormalSummon:1;            /* +0x007 bit 3: DuelCmd_SetSummonLocks */
    u8 noSpecialSummon:1;           /* +0x007 bit 4: DuelCmd_SetSummonLocks */
    u8 unk7_5:3;
    u8 unk8[0xC - 0x8];
    u8 unkC_0:5;
    u8 banishCostFromField:1;       /* +0x00C bit 5: graveyard-banish summon costs are paid from the field */
    u8 unkC_6:2;
    u8 unkD[0x684 - 0xD];
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84: removed from play */
    u16 banishedInfo[80];           /* +0xCC4 */
};

/* gDuel (0x1B78 bytes at 0x020192E0): only the byte this unit reads by name. */
struct DuelState {
    u8 unk0[0x1B22];
    u8 phaseSubStep;                /* +0x1B22: secondary step of the phase handlers (Sinister Serpent) */
    u8 unk1B23[0x1B78 - 0x1B23];
};

/* gDuelZones is &players[0].zones[0]: the zones of each player with the player stride. */
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

STATIC_ASSERT(sizeof(struct DuelCard) == 4, DuelCardSize);
STATIC_ASSERT(sizeof(struct DuelZone) == 0x94, DuelZoneSize);
STATIC_ASSERT(sizeof(struct DuelPlayer) == 0xD64, DuelPlayerSize);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayer, banished) == 0xB84, DuelPlayerBanished);
STATIC_ASSERT(OFFSET_OF(struct DuelState, phaseSubStep) == 0x1B22, DuelStatePhaseSubStep);

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

/* Number of active (face-up, not disabled) copies of the card in zones 0-10. */
int CountActiveCardsOnField(int player, u16 cardNo);
/* The same count (a byte-identical second copy that some callers use). */
int CountActiveCardsOnField2(int player, u16 cardNo);
/* Number of face-up monsters with that card number. */
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
/* Number of monsters with that card number, face up or down. */
int CountMonstersByNumber(int player, u16 cardNo);
/* Number of other face-up monsters on both fields with the same name as the card in (player, zone). */
int CountOtherFaceUpSameNameMonsters(int player, int zone);
/* Number of graveyard cards with that card number. */
int CountGraveyardCardsByNumber(int player, u16 cardNo);
/* Number of hand cards with that card number. */
int CountHandCardsByNumber(int player, u16 cardNo);
/* Number of free monster zones / the first free one, or -1. */
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
/* First free spell/trap zone 5-9, or -1. */
int FindFreeSpellTrapZone(int player);
/* Nonzero if the zone holds a monster that may be tributed: not a token, and card 1418 is not face up. */
u16 IsTributableMonster(int player, int zone);
/* Effective card type / attribute of the card in (player, slot), 0 if empty. */
u32 GetZoneCardType(s32 player, s32 slot);
u32 GetZoneCardAttribute(s32 player, s32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, Chain_AddPending */
#include "duel_actions.h"           /* ShowCardEffect, DrawCards, LoseLifePoints, TributeMonster, ... */
#include "duel_prompt.h"            /* DuelPrompt_PostDiscard */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* gCardEffects, gTributeSummonPrompts, FindCardEffect, CanEffectTargetZone */
#include "effect_handlers.h"        /* EffectNopResolve, EffectSpiritMessagePrepare, EffectEquippedTributeCheck */
#include "summon.h"                 /* QueueNormalSummon, QueueSpecialSummonFromHand */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr, FormatInt, Random */

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md).
 */

/* Matching: the ROM passes the u32 command id to DuelCmd_Push unnarrowed (duel_cmd.h takes a u16). */
extern void DuelCmd_Push32(u32 cmd, u16 arg2, int arg4, int arg6) asm("DuelCmd_Push");

/* Card IDs of single card numbers, as address-suffixed aliases of the gCardNumberToId entries (card_data.h
 * lists them): the ROM loads the element address from its own literal. */
extern const u16 gUnk_08623F3E[];   /* &gCardNumberToId[CARD_MYSTERIOUS_PUPPETEER] */
extern const u16 gUnk_08624084[];   /* &gCardNumberToId[CARD_DRAGON_CAPTURE_JAR] */
extern const u16 gUnk_086241A8[];   /* &gCardNumberToId[CARD_SINISTER_SERPENT] */
extern const u16 gUnk_086246BC[];   /* &gCardNumberToId[CARD_KOTODAMA] */
extern const u16 gUnk_086249EE[];   /* &gCardNumberToId[CARD_1533] */

/* Prompt of Sinister Serpent (ROM): "Do you wish to returne @2%s@0 from the Graveyard to your hand?" (sic) */
extern const char gStrSinisterSerpentPrompt[];

/* The ROM's integer-constant addresses: gDuelZones (0x0201930C), gCardIdToNumber (0x08622AB4), gCardStats
 * (0x08621DE0) and gCardNumberToId (0x08623DF4). Matching: the cast-literal forms give the ROM's literal
 * pools and the order of the address additions; the symbol forms generate other code. */
#define CARD_NUMBER(id)         (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_NUMBER_OF(word)    (((const u16 *)0x08622AB4)[((word) << 21) >> 21])   /* index: ID & 0x7FF by shifts */
#define CARD_ID_OF(number)      (((const u16 *)0x08623DF4)[number])                 /* gCardNumberToId[number] */
#define CARD_STATS(id)          (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])

/* The card word of a zone or pile entry as one u32, and its ID by shifts (lsl #20; lsr #20). Matching: the ROM
 * loads the whole word; a .id bitfield read would load a halfword. */
#define CARD_WORD(card)         (*(u32 *)&(card))
#define CARD_ID(word)           (((word) << 20) >> 20)
/* 1 if the zone holds a card (ID != 0), tested with a single lsl #20. */
#define HAS_CARD(card)          (CARD_WORD(card) << 20 != 0)

/* Text box placement (TextBoxOpen pos = x | y << 8, size = width | height << 8, in cells). */
#define TEXTBOX_XY(x, y)    ((x) | (y) << 8)

#define ZONE_STRIDE     0x94    /* sizeof(struct DuelZone) */
#define PLAYER_STRIDE   0xD64   /* sizeof(struct DuelPlayer) */

/* Duel commands for player 1 carry DUEL_CMD_PLAYER. */
#define CMD_FOR_PLAYER(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/*
 * Pumpking the King of Ghosts: the hook after a zone card appears. If the card in (player, zone) is face up
 * and Castle of Dark Illusions is active on either field, show the card and link a +100 boost to it
 * (ZONE_LINK_STATS_UP_100).
 */
void ApplyPumpkingBoost(int player, int zone)
{
    int side = player & 1;
    int offset = zone * ZONE_STRIDE + side * PLAYER_STRIDE;
    /* Matching: a symbol-backed byte base (not the integer address) reproduces the ROM's zone copy. */
    u8 *base = (u8 *)gDuelZones;
    struct DuelZone *zn = (struct DuelZone *)(offset + (int)base);

    if (HAS_CARD(zn->card) && zn->isFaceUp) {
        if (CountActiveCardsOnField2(0, CARD_CASTLE_OF_DARK_ILLUSIONS) > 0
            || CountActiveCardsOnField2(1, CARD_CASTLE_OF_DARK_ILLUSIONS) > 0) {
            sub_080197C0(player, CARD_ID(CARD_WORD(zn->card)));
            /* the DUEL_LOC of (player, zone), each narrowed to a byte */
            QueueAddZoneLink(player, CARD_ID(CARD_WORD(zn->card)), (u8)player | (u8)zone << 8,
                             ZONE_LINK_STATS_UP_100);
        }
    }
}

/*
 * Mysterious Puppeteer: each active copy gives its controller 500 LP. Called for the player whose turn
 * begins (hypothesis); both sides are counted, and the opponent gains from its own copies.
 */
void TriggerMysteriousPuppeteer(int player)
{
    int mine = CountActiveCardsOnField2(player, CARD_MYSTERIOUS_PUPPETEER);
    int other = 1 - player;
    int theirs = CountActiveCardsOnField2(other, CARD_MYSTERIOUS_PUPPETEER);

    if (mine > 0 || theirs > 0) {
        ShowCardEffect(player, gUnk_08623F3E[0]);
        GainLifePoints(player, mine * 500);
        GainLifePoints(other, theirs * 500);
    }
}

/*
 * Dragon Capture Jar: if it is active on either field and some monster zone holds a face-up Dragon in attack
 * position, show the Jar and switch every such Dragon to Defense Position (ChangeBattlePosition).
 */
void ApplyDragonCaptureJar(int player)
{
    int found;
    int p, z;
    u32 id = CARD_DRAGON_CAPTURE_JAR;

    if (CountActiveCardsOnField(0, id) > 0 || CountActiveCardsOnField(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2;) {
            /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
            register int next asm("r6");
            z = 0;
            next = p + 1;
            for (; z < 5; z++) {
                int offset = z * ZONE_STRIDE + (p & 1) * PLAYER_STRIDE;
                u8 *base = (u8 *)gDuelZones;
                struct DuelZone *zn = (struct DuelZone *)(offset + (int)base);

                /* face up and in attack position: bits 0-1 of byte +6 read as 2 */
                if (HAS_CARD(zn->card) && (zn->isFaceUp && !zn->isDefense)) {
                    if (GetZoneCardType(p, z) == CARD_TYPE_DRAGON)
                        found = 1;
                }
            }
            p = next;
        }
        if (found != 0) {
            u32 cmd = DUEL_CMD_SHOW_CARD_EFFECT;

            if (player != 0)
                cmd = DUEL_CMD_PLAYER | DUEL_CMD_SHOW_CARD_EFFECT;
            DuelCmd_Push32(cmd, gUnk_08624084[0], 1, 0);
            for (p = 0; p < 2;) {
                /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
                register int next asm("r6");
                z = 0;
                next = p + 1;
                for (; z < 5; z++) {
                    int offset = z * ZONE_STRIDE + (p & 1) * PLAYER_STRIDE;
                    u8 *base = (u8 *)gDuelZones;
                    struct DuelZone *zn = (struct DuelZone *)(offset + (int)base);

                    if (HAS_CARD(zn->card) && (zn->isFaceUp && !zn->isDefense)) {
                        if (GetZoneCardType(p, z) == CARD_TYPE_DRAGON)
                            ChangeBattlePosition(p, z, 0, 0);
                    }
                }
                p = next;
            }
        }
    }
}

/*
 * Sinister Serpent, Standby Phase: may it return from the graveyard to the hand? A two-step handler on
 * gDuel.phaseSubStep, called each frame until it returns 1.
 *   step 0  done (1) if no Sinister Serpent is in the player's graveyard. The CPU answers Yes at once; the
 *           human gets the Yes/No prompt.
 *   step 1  on Yes: show the card and return it to the hand.
 * Returns 0 while running, 1 when finished.
 */
int SinisterSerpentStandbyStep(int player)
{
    char text[0x80];
    u8 *base = (u8 *)&gDuel;
    u8 *step = base + OFFSET_OF(struct DuelState, phaseSubStep);

    switch (*step) {
    case 0: {
        int cardNo = CARD_SINISTER_SERPENT;

        if (CountGraveyardCardsByNumber(player, cardNo) == 0)
            return 1;
        if (player != 0) {
            /* FAKEMATCH: materialize the base in r0 before loading the flag value. */
            register struct TextBox *answer __asm__("r0") = &gTextBox;
            __asm__ __volatile__("" : : "r"(answer));
            answer->result = 1;
        } else {
            FormatStr(text, gStrSinisterSerpentPrompt, (const char *)gCardNames + CARD_ID_OF(cardNo) * CARD_NAME_SIZE);
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
        }
        break;
    }
    case 1:
        if (gTextBox.result != 0) {
            int cardNo = CARD_SINISTER_SERPENT;

            ShowCardEffect(player, gUnk_086241A8[0]);
            ReturnGraveyardCardToHand(player, cardNo);
        }
        break;
    default:
        return 1;
    }
    (*step)++;
    return 0;
}

/* The card type (enum CardType) of a card ID. Matching: both the inline argument and the caller's card ID are
 * narrow (u16) in the ROM, and the stats table is read through its integer address. */
static inline u32 CardType(u16 id)
{
    return CARD_STATS_TYPE(CARD_STATS(id));
}

/*
 * The Trap disabling of Jinzo / Royal Decree: queue DUEL_CMD_SET_SPELL_TRAP_DISABLED for every face-up Trap in
 * the spell/trap zones 5-9 of both players.
 */
void DisableFaceUpTraps(void)
{
    int p, z;

    for (p = 0; p < 2; p++) {
        for (z = ZONE_SPELL_0; z <= ZONE_SPELL_4; z++) {
            /* Matching: the zone array by its integer address (0x0201930C = gDuelZones). */
            struct DuelZone *zn = (struct DuelZone *)(z * ZONE_STRIDE + (p & 1) * PLAYER_STRIDE + 0x0201930C);
            u16 id = CARD_ID(CARD_WORD(zn->card));

            if (id != 0 && zn->isFaceUp) {
                if (CardType(id) == CARD_TYPE_TRAP) {
                    u32 cmd = DUEL_CMD_SET_SPELL_TRAP_DISABLED;

                    if (p != 0)
                        cmd = DUEL_CMD_PLAYER | DUEL_CMD_SET_SPELL_TRAP_DISABLED;
                    DuelCmd_Push32(cmd, z, 1, 0);
                }
            }
        }
    }
}

/*
 * Chain Energy: for each active copy on either field, `player` loses 500 LP (called when the player starts
 * a turn, hypothesis).
 */
void PayChainEnergyCost(int player)
{
    u32 id = CARD_CHAIN_ENERGY;
    int n = CountActiveCardsOnField(player, id);

    n += CountActiveCardsOnField(1 - player, id);
    if (n > 0) {
        u32 cmd;

        ShowCardEffect(player, CARD_ID_OF(id));
        cmd = DUEL_CMD_LOSE_LP;
        if (player != 0)
            cmd = DUEL_CMD_PLAYER | DUEL_CMD_LOSE_LP;
        DuelCmd_Push32(cmd, n * 500, 1, 0);
    }
}

/*
 * Kotodama: while a face-up Kotodama is on either field, every monster that has another face-up monster with
 * the same name is destroyed (all zones, after a monster appeared).
 */
void ApplyKotodama(void)
{
    int found;
    int p, z;
    u32 id = CARD_KOTODAMA;

    if (CountFaceUpMonstersByNumber(0, id) > 0 || CountFaceUpMonstersByNumber(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2; p++) {
            for (z = 0; z < 5; z++) {
                if (CountOtherFaceUpSameNameMonsters(p, z) != 0)
                    found = 1;
            }
        }
        if (found != 0) {
            ShowCardEffect(0, gUnk_086246BC[0]);
            for (p = 0; p < 2; p++) {
                for (z = 0; z < 5; z++) {
                    if (CountOtherFaceUpSameNameMonsters(p, z) != 0)
                        DestroyFieldCard(p, z, 1);
                }
            }
        }
    }
}

/* Kotodama for one monster zone, after the monster in (player, zone) appeared: destroy it if it has a
 * face-up namesake and a face-up Kotodama is on either field. */
void ApplyKotodamaToZone(int player, int zone)
{
    u32 id = CARD_KOTODAMA;

    if (CountFaceUpMonstersByNumber(0, id) > 0 || CountFaceUpMonstersByNumber(1, id) > 0) {
        if (CountOtherFaceUpSameNameMonsters(player, zone) > 0) {
            ShowCardEffect(player, CARD_ID_OF(id));
            DestroyFieldCard(player, zone, 1);
        }
    }
}

/* Appropriate: the player draws 2 cards for each of its active copies (a hook of the draw step). */
void TriggerAppropriate(int player)
{
    u32 id = CARD_APPROPRIATE;
    int n = CountActiveCardsOnField(player, id);

    if (n > 0) {
        ShowCardEffect(player, CARD_ID_OF(id));
        DrawCards(player, n * 2);
    }
}

/*
 * Forced Requisition: when `player` discards `discarded` cards (hand or deck effect), each active copy makes the
 * opponent discard that many cards as well.
 */
void TriggerForcedRequisition(int player, int discarded)
{
    u32 id = CARD_FORCED_REQUISITION;

    discarded *= CountActiveCardsOnField(player, id);
    if (discarded > 0) {
        ShowCardEffect(player, CARD_ID_OF(id));
        DuelPrompt_PostDiscard(1 - player, discarded, 0, 1);
    }
}

/*
 * Effect key 1306 (no EDS card): while it is active on either field, `player` loses 300 LP for each of the
 * `count` cards just sent to the graveyard.
 */
void LoseLpOnSendToGraveyard(int player, int count)
{
    u32 id = CARD_1306;
    int n = CountActiveCardsOnField(player, id);

    n += CountActiveCardsOnField(1 - player, id);
    if (n > 0) {
        ShowCardEffect(player, CARD_ID_OF(id));
        LoseLifePoints(player, count * 300);
    }
}

/*
 * Resolve a graveyard card marked to be equipped to an opponent monster at the end of the turn: when `equip`
 * is set, queue DUEL_CMD_EQUIP_GRAVEYARD_CARD_TO_OPPONENT for graveyard[graveIdx]; otherwise clear the mark
 * (DUEL_CMD_CLEAR_PENDING_EQUIP).
 */
void ResolvePendingGraveyardEquip(int player, int graveIdx, u16 equip)
{
    if (equip != 0)
        DuelCmd_Push32(CMD_FOR_PLAYER(player, DUEL_CMD_EQUIP_GRAVEYARD_CARD_TO_OPPONENT), graveIdx, 1, 0);
    else
        DuelCmd_Push32(CMD_FOR_PLAYER(player, DUEL_CMD_CLEAR_PENDING_EQUIP), graveIdx, 0, 0);
}

/*
 * Called after an effect of `srcPlayer` destroyed the card in (player, zone). If that card is key 1514 (no
 * EDS card), the destroyer is the opponent, the card was a monster (zone 0-4) and Banisher of the Light is
 * face up on neither field, show it and queue DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING for `player` (the
 * delayed trigger of duel step 6).
 */
void OnCardDestroyedByEffect(int srcPlayer, int player, int zone)
{
    int side = player & 1;
    int offset = zone * ZONE_STRIDE + side * PLAYER_STRIDE;
    /* Matching: a symbol-backed byte base (not the integer address) reproduces the ROM's zone copy. */
    u8 *base = (u8 *)gDuelZones;
    struct DuelZone *zn = (struct DuelZone *)(offset + (int)base);

    if (CARD_NUMBER_OF(CARD_WORD(zn->card)) == CARD_1514 && srcPlayer != player) {
        u32 id = CARD_BANISHER_OF_THE_LIGHT;

        if (CountFaceUpMonstersByNumber(0, id) <= 0 && CountFaceUpMonstersByNumber(1, id) <= 0
            && zone <= ZONE_MONSTER_4) {
            u32 cmd;

            sub_080197C0(player, CARD_ID(CARD_WORD(zn->card)));
            cmd = DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING;
            if (player != 0)
                cmd = DUEL_CMD_PLAYER | DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING;
            DuelCmd_Push32(cmd, 1, 0, 0);
        }
    }
}

/*
 * Effect keys 1541-1544 (no EDS card): the "spirit message" cards. When a board card with turn counter 0-3 asks
 * for the next one, the card of key 1541 + turnCounter is put into play: from the deck into the first free
 * spell/trap zone (PlaceDeckCardOnField), or else from the hand (DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND). Any other
 * counter does nothing. (player, zone) is the board card; the keys are only placed by this function
 * (EffectSpiritMessagePrepare returns 0, so they can never be activated from the hand).
 */
void PlaceNextSpiritMessage(int player, int zone)
{
    /* FAKEMATCH: keep the queried position in r8 across the selection branches. */
    register int freeZone asm("r8") = FindFreeSpellTrapZone(player);
    int side = player & 1;
    struct DuelZone *zn = (struct DuelZone *)(zone * ZONE_STRIDE + side * PLAYER_STRIDE + 0x0201930C);
    u16 cardNumber;

    switch (zn->turnCounter) {
    case 0:
        cardNumber = CARD_1541;
        break;
    case 1:
        cardNumber = CARD_1542;
        break;
    case 2:
        cardNumber = CARD_1543;
        break;
    case 3:
        cardNumber = CARD_1544;
        break;
    default:
        return;
    }
    if (PlaceDeckCardOnField(player, cardNumber, freeZone) != 0) {
        /* The card ID of cardNumber, as gCardNumberToId maps it (alternate-art numbers 2000+ map to the ID
           after the original's, 0xFFFF to 0). cardNumber is 1541-1544 here, so only the middle arm runs. */
        /* FAKEMATCH: preserve the lookup result in r0 until the u16 call conversion. */
        register u32 id asm("r0");
        if (cardNumber == 0xFFFF) {
            id = 0;
        } else if (cardNumber <= CARD_NUMBER_ALT_ART - 1) {
            /* FAKEMATCH: retain the table load after the byte-index calculation. */
            int index = cardNumber * 2;
            const u16 *table = (const u16 *)0x08623DF4;
            __asm__("" : "+r"(table));
            id = *(const u16 *)(index + (int)table);
        } else {
            /* Dead arm: the ROM's copy of the alternate-art branch; `|= 0x30` is what reproduces its bytes. */
            cardNumber |= 0x30;
            {
                /* FAKEMATCH: use r4 for the table base after computing the byte index. */
                int index = cardNumber * 2;
                register const u16 *table asm("r4") = (const u16 *)0x08623DF4;
                __asm__("" : "+r"(table));
                id = *(const u16 *)(index + (int)table) + 1;
            }
        }
        ShowCardEffect(player, id);
    } else {
        /* No room or no such card in the deck: look for it in the hand. */
        int i = 0;
        u8 *base = (u8 *)gDuelPlayers;
        int offset = (player & 1) * PLAYER_STRIDE;
        struct DuelPlayer *pl = (struct DuelPlayer *)(offset + (int)base);

        if (i < pl->handCount) {
            u8 *handBase = base + OFFSET_OF(struct DuelPlayer, hand);
            /* FAKEMATCH: keep the slot mask in r9 and preserve its reload for masking freeZone. */
            register int mask asm("r9") = 0xF;
            __asm__("" : "+r"(mask));
            {
                /* FAKEMATCH: retain the masked position in ip through the hand scan. */
                register int lowZone asm("r12") = freeZone & mask;
                u32 *card = (u32 *)(offset + (int)handBase);
                int bound;
                do {
                    u32 cardId = (*card << 20) >> 20;
                    if (CARD_NUMBER(cardId) == cardNumber) {
                        u32 cmd = DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND;

                        if (player != 0)
                            cmd = DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND;
                        {
                            /* arg4: zone | hand index << 4 | faceUp << 8 */
                            int packed = (i & mask) << 4;
                            /* FAKEMATCH: combine the saved position through r5. */
                            register int low asm("r5") = lowZone;
                            __asm__("" : "+r"(low));
                            packed |= low;
                            {
                                /* FAKEMATCH: materialize the flag in r7, retaining its move to r0. */
                                register int flag asm("r7") = 0x100;
                                register int value asm("r0") = flag;
                                __asm__("" : "+r"(value));
                                packed |= value;
                            }
                            DuelCmd_Push32(cmd, cardId, packed, 0);
                        }
                        ShowCardEffect(player, (*card << 20) >> 20);
                        return;
                    }
                    card++;
                    i++;
                    bound = pl->handCount;
                    /* FAKEMATCH: preserve the loop-bound reload in the original scratch register. */
                    __asm__("" : "+r"(bound));
                } while (i < bound);
            }
        }
    }
}

/*
 * Effect key 1533 (no EDS card): if it is active for `player`, count the opponent's banished monsters (the
 * pile at +0xB84 of its side, entries of type <= CARD_TYPE_REPTILE) and make the opponent lose 100 LP each.
 */
void DamageOpponentPerBanishedMonster(int player)
{
    if (CountActiveCardsOnField(player, CARD_1533) != 0) {
        /* FAKEMATCH: keep the qualifying-card count in the original r4. */
        register int n asm("r4") = 0;
        u8 *base = (u8 *)gDuelPlayers;
        int opponent = (1 - player) & 1;
        /* FAKEMATCH: retain the offset in r2; integer address sums preserve ADD operand order. */
        register int offset asm("r2") = opponent * PLAYER_STRIDE;
        int count = ((struct DuelPlayer *)(offset + (int)base))->banishedCount;

        if (n < count) {
            int pileOffset = OFFSET_OF(struct DuelPlayer, banished);
            /* FAKEMATCH: materialize the pile base in r0 before adding the player offset. */
            register u8 *pileBase asm("r0") = base + pileOffset;
            u32 *cursor = (u32 *)(offset + (int)pileBase);
            /* FAKEMATCH: keep the only hoisted lookup constant in r6. */
            register int mask asm("r6") = CARD_ID_MASK;
            /* FAKEMATCH: reuse r2 for the remaining-card counter. */
            register int remaining asm("r2") = count;
            /* FAKEMATCH: this back edge leaves the table and type-mask loads in the loop. */
        loop:
            {
                u16 id = CARD_ID(*cursor);

                if (CARD_STATS_TYPE(((const u32 *)0x08621DE0)[id & mask]) <= CARD_TYPE_REPTILE)
                    n++;
                cursor++;
            }
            if (--remaining != 0)
                goto loop;
        }
        if (n > 0) {
            ShowCardEffect(player, gUnk_086249EE[0]);
            LoseLifePoints(1 - player, n * 100);
        }
    }
}

/*
 * Effect key 1536 (no EDS card), a dice effect: if it is active for `player`, roll a six-sided die, announce
 * the roll (DUEL_CMD_ROLL_PLAIN_DIE), reopen the duel screen and destroy every face-up monster whose level equals
 * the roll (a roll of 6 also takes levels above 6). Trap, Magic and Ticket cards count as level 0 and Divine
 * cards as level 10. Nothing calls it: the Standby Phase handler carries an inlined copy.
 */
void RollDieDestroyMonstersByLevel(int player)
{
    int z, p;
    u32 id = CARD_1536;

    if (CountActiveCardsOnField(player, id) != 0) {
        int roll = Random() % 6 + 1;
        /* FAKEMATCH: retain the roll in the ROM's register before announcing it. */
        __asm__ __volatile__("" : : "r"(roll));
        ShowCardEffect(player, CARD_ID_OF(id));
        {
            u32 cmd = DUEL_CMD_ROLL_PLAIN_DIE;

            if (player != 0)
                cmd = DUEL_CMD_PLAYER | DUEL_CMD_ROLL_PLAIN_DIE;
            DuelCmd_Push32(cmd, roll, 0, 0);
        }
        {
            u32 cmd = DUEL_CMD_OPEN_DUEL_SCREEN;

            if (player != 0)
                cmd = DUEL_CMD_PLAYER | DUEL_CMD_OPEN_DUEL_SCREEN;
            DuelCmd_Push32(cmd, 0, 0, 0);
        }
        for (p = 0; p < 2; p++) {
            for (z = 0; z <= ZONE_MONSTER_4; z++) {
                /* Matching: the zone array by its integer address (0x0201930C = gDuelZones). */
                struct DuelZone *zn = (struct DuelZone *)(z * ZONE_STRIDE + (p & 1) * PLAYER_STRIDE + 0x0201930C);
                u16 cardId = CARD_ID(CARD_WORD(zn->card));

                if (cardId != 0 && zn->isFaceUp) {
                    int level;
                    int hit;
                    int type = CARD_STATS_TYPE(CARD_STATS(cardId));

                    switch (type) {
                    case CARD_TYPE_TRAP:
                    case CARD_TYPE_MAGIC:
                    case CARD_TYPE_TICKET:
                        level = 0;
                        break;
                    case CARD_TYPE_DIVINE:
                        level = 10;
                        break;
                    default:
                        level = CARD_STATS_LEVEL(CARD_STATS(cardId));
                        break;
                    }
                    hit = 0;
                    if (level == roll)
                        hit = 1;
                    if (level > 5 && roll == 6)
                        hit = 1;
                    if (hit != 0)
                        DestroyFieldCard(p, z, 1);
                }
            }
        }
    }
}

/*
 * Resolve slot of the continuous and passive cards whose effect is applied by hooks elsewhere (Larvae Moth,
 * Toon World, Chain Energy, Mirror Wall, Appropriate, Forced Requisition, Aqua Chorus, ...): 38 rows of
 * gCardEffects. Returns EFFECT_STEP_DONE, so the chain link ends at once.
 */
int EffectNopResolve(void)
{
    return 0;
}

/* Prepare slot of keys 1541-1544: always 0, so the spirit message cards can never be activated. */
int EffectSpiritMessagePrepare(void)
{
    return 0;
}

/*
 * Binary search of gCardEffects (426 rows, sorted by card number) for the row of a card ID: the key is the card
 * number gCardIdToNumber[cardId & 0x7FF]. Returns the row index, or -1 if the card has no effect.
 */
int FindCardEffect(u32 cardId)
{
    int lo = 0;
    int hi = 425;                   /* the last row */
    u16 key = CARD_NUMBER_OF(cardId);

    for (;;) {
        int mid = (lo + hi) / 2;
        u16 number = gCardEffects[mid].number;

        if (key == number)
            return mid;
        if (lo == hi)
            return -1;
        if (key > number)
            lo = mid;
        if (key < number)
            hi = mid;
        if ((lo + hi) / 2 == mid)
            lo = hi;
    }
}

/*
 * Ask the Check handler of a card's effect whether it accepts the board position (player, zone). `card` is a
 * struct ChainEntry, whose first halfword is the card ID. Returns 0 for NULL, 1 when the card has no
 * gCardEffects row or its row has no Check handler (it accepts any target), else the handler's verdict.
 */
u16 CanEffectTargetZone(u16 *card, int player, int zone)
{
    if (card != 0) {
        int row = FindCardEffect(*card);
        u16 (*check)(struct ChainEntry *, u16);

        if (row < 0 || (check = gCardEffects[row].check) == 0)
            return 1;
        return check((struct ChainEntry *)card, (u8)player | (u8)zone << 8);
    }
    return 0;
}

/*
 * May `player` Normal Summon or Set a monster? Not if its noNormalSummon flag is set, key 1426 (no EDS card)
 * is active on its field, or key 1526 (no EDS card) is active on either field.
 */
int CanNormalSummon(int player)
{
    if (!gDuelPlayers[player & 1].noNormalSummon && CountActiveCardsOnField(player, CARD_1426) == 0
        && CountActiveCardsOnField(0, 1526) == 0 && CountActiveCardsOnField(1, 1526) == 0)
        return 1;
    return 0;
}

/*
 * May `player` Special Summon? Not if its noSpecialSummon flag is set, key 1426 is active on its field, or key
 * 1510 or 1526 is active on either field (all three have no EDS card). Matching: the separate returns keep
 * each constant's own literal load (a chain of && lets the compiler derive 1526 from 1510).
 */
int CanSpecialSummon(int player)
{
    if (gDuelPlayers[player & 1].noSpecialSummon)
        return 0;
    if (CountActiveCardsOnField(player, CARD_1426) != 0)
        return 0;
    if (CountActiveCardsOnField(0, CARD_1510) != 0)
        return 0;
    if (CountActiveCardsOnField(1, CARD_1510) != 0)
        return 0;
    if (CountActiveCardsOnField(0, 1526) != 0)
        return 0;
    if (CountActiveCardsOnField(1, 1526) != 0)
        return 0;
    return 1;
}

/*
 * ---- CardMenu_SummonMonster ----
 */

/*
 * Matching: three views of the card menu's state. The state is struct CardMenu (gDuel.cardMenu at
 * gDuel + 0x1B2C) with gDuel.cardMenuCard and gDuel.summonTributes in front of it, but the matched code reads
 * it with other bitfield containers than the header (the header's CardMenu has u16 open/confirmed/command
 * and u32 step), and the ROM reaches it from three bases: the gDuel literal, and the gDuelPlayers and gDuelZones
 * literals (+4 and +0x2C further in, hence the shorter prefixes). Each access keeps the base it has in the ROM.
 * The fields carry the names of struct CardMenu.
 */
#define CARD_MENU_FIELDS \
    u16 cardMenuCard;           /* gDuel +0x1B28: card ID of the hand card being summoned */ \
    u16 summonTributes;         /* +0x1B2A: the picked Tribute zones (see TRIBUTE_*) */ \
    u8 open:1;                  /* +0x1B2C bit 0 */ \
    u8 confirmed:1;             /* +0x1B2C bit 1: cleared when the command is finished */ \
    u8 command:4;               /* +0x1B2C bits 2-5: enum CardMenuCommand */ \
    u8 unk2C_6:2; \
    u8 unk2D[3]; \
    u16 unk30_0:2; \
    u16 step:8;                 /* +0x1B30 bits 2-9: enum SummonStep */ \
    u8 summonSeq:4;             /* +0x1B31 bits 2-5: which named Tribute is being picked (0-2) */ \
    u32 tributeSources:4;       /* +0x1B31 bits 6-9: TRIBUTE_FROM_* bits */ \
    u8 unk32_2:6; \
    u8 unk33_0:1; \
    u8 player:1;                /* +0x1B33 bit 1: the player who confirmed the command */ \
    u8 unk33_2:6; \
    u16 unk34_0:1; \
    u16 index:8;                /* +0x1B34 bits 1-8: hand index of the card being summoned */ \
    u32 placeZone:8;            /* +0x1B34 bits 9-16: zone of the monster (key 1520's trigger) */ \
    u8 tail[4]
struct CardMenuView {           /* from gDuel */
    u8 prefix[0x1B28];
    CARD_MENU_FIELDS;
};
struct CardMenuViewFromPlayers {    /* from gDuelPlayers (gDuel + 4) */
    u8 prefix[0x1B24];
    CARD_MENU_FIELDS;
};
struct CardMenuViewFromZones {      /* from gDuelZones (gDuel + 0x2C) */
    u8 prefix[0x1AFC];
    CARD_MENU_FIELDS;
};
extern struct CardMenuView gCardMenuView asm("gDuel");
#define CM                  gCardMenuView
#define CM_FROM_PLAYERS     (*(struct CardMenuViewFromPlayers *)gDuelPlayers)
#define CM_FROM_ZONES       (*(struct CardMenuViewFromZones *)gDuelZones)

/*
 * Matching: a player's side addressed from gDuel (the base of struct DuelState) rather than from gDuelPlayers,
 * so its fields are 4 bytes further in: handCount at +6 and the byte at +0x10 (DuelPlayer +0x0C), whose bit 5 is
 * banishCostFromField. The sum is spelled as an unsigned subtraction of the negated offset to keep the ROM's
 * ADD operand order (FAKEMATCH).
 */
struct DuelPlayerFromDuel {
    u8 prefix[6];
    u8 handCount;               /* DuelPlayer.handCount */
    u8 unk7[9];
    u8 flagsC;                  /* DuelPlayer +0x0C: bit 5 = banishCostFromField */
    u8 rest[0xD64 - 0x11];
};
#define PLAYER_FROM_DUEL(p) (*(struct DuelPlayerFromDuel *)((u32)&CM - (u32)(-((p) * PLAYER_STRIDE))))
/* DuelPlayer.banishCostFromField of the player, tested as the sign of the byte << 26 (bit 5). With the flag
 * set the banish-to-summon costs are paid from the field with the cursor, else from the graveyard list. */
#define BANISH_FROM_FIELD(p) (gDuelPlayers[(p) & 1].banishCostFromField)

/* gDuel.cardMenu.tributeSources (CardMenu.tributeSources): where the named Tribute may come from. */
#define TRIBUTE_FROM_HAND   1
#define TRIBUTE_FROM_FIELD  2

/* Cursor-pick mask for a face-up monster in either position (0xE0). */
#define PICK_FACE_UP_MONSTERS   (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)

/* The cursor's pick as the position words the effect code uses (Matching: the ROM narrows each part to a byte). */
#define PICKED_POS          ((u8)gDuelScreen.selPlayer | ((u8)gDuelScreen.selIndex << 8))        /* player | zone << 8 */
#define PICKED_AREA_ZONE    ((u8)gDuelScreen.selArea | ((u8)gDuelScreen.selIndex << 8))          /* area | zone << 8 */
#define PICKED_PLAYER_ZONE  (((u8)gDuelScreen.selPlayer << 8) | (u8)gDuelScreen.selIndex)        /* player << 8 | zone */

/* The name of a card ID (gCardNames, 0x40-byte records). */
#define CARD_NAME(id)       ((const char *)gCardNames + (id) * CARD_NAME_SIZE)

/* gDuel.cardMenu.step of CardMenu_SummonMonster: the values are the case labels of its switch. */
enum SummonStep {
    SUMMON_STEP_START = 0,                  /* dispatch on the card number and the level */
    SUMMON_STEP_TWO_TRIBUTES_ASK = 1,       /* answer of the "2 Tributes?" Yes/No prompt */
    SUMMON_STEP_TWO_TRIBUTES_PROMPT_1 = 2,  /* prompt for the first Tribute */
    SUMMON_STEP_TWO_TRIBUTES_PICK_1 = 3,
    SUMMON_STEP_TWO_TRIBUTES_PROMPT_2 = 4,  /* prompt for the second Tribute (also used by key 1257) */
    SUMMON_STEP_TWO_TRIBUTES_PICK_2 = 5,
    SUMMON_STEP_TWO_TRIBUTES_SUMMON = 6,
    SUMMON_STEP_ONE_TRIBUTE_ASK = 10,       /* answer of the "1 Tribute?" Yes/No prompt */
    SUMMON_STEP_ONE_TRIBUTE_PICK = 11,
    SUMMON_STEP_ONE_TRIBUTE_SUMMON = 12,
    SUMMON_STEP_NAMED_TRIBUTE_PICK = 20,    /* moths and Wall Shadow: pick the monster the prompt names */
    SUMMON_STEP_GATE_GUARDIAN_PROMPT = 30,  /* Sanga, Kazejin and Suijin in turn (summonSeq 0-2) */
    SUMMON_STEP_GATE_GUARDIAN_PICK = 31,
    SUMMON_STEP_VALKYRION_PROMPT = 40,      /* Alpha, Beta and Gamma in turn: from the hand and/or the field */
    SUMMON_STEP_VALKYRION_PICK = 41,
    SUMMON_STEP_VALKYRION_SUMMON = 42,
    SUMMON_STEP_BANISH_FIENDS_PROMPT = 50,  /* key 1514: banish three Fiends */
    SUMMON_STEP_BANISH_FIENDS_PICK_1 = 51,
    SUMMON_STEP_BANISH_FIENDS_BANISH_1 = 52,
    SUMMON_STEP_BANISH_FIENDS_LEFT_2 = 53,
    SUMMON_STEP_BANISH_FIENDS_PICK_2 = 54,
    SUMMON_STEP_BANISH_FIENDS_BANISH_2 = 55,
    SUMMON_STEP_BANISH_FIENDS_LEFT_1 = 56,
    SUMMON_STEP_BANISH_FIENDS_PICK_3 = 57,
    SUMMON_STEP_BANISH_FIENDS_BANISH_3 = 58,
    SUMMON_STEP_BANISH_LIGHT_PROMPT = 60,   /* key 1515: banish two LIGHT monsters */
    SUMMON_STEP_BANISH_LIGHT_PICK_1 = 61,
    SUMMON_STEP_BANISH_LIGHT_BANISH_1 = 62,
    SUMMON_STEP_BANISH_LIGHT_LEFT_1 = 63,
    SUMMON_STEP_BANISH_LIGHT_PICK_2 = 64,
    SUMMON_STEP_BANISH_LIGHT_BANISH_2 = 65,
    SUMMON_STEP_BANISH_ATTRIBUTE_PROMPT = 70,   /* keys 1516-1519: banish one FIRE / WATER / EARTH / WIND monster */
    SUMMON_STEP_BANISH_ATTRIBUTE_PICK = 71,
    SUMMON_STEP_BANISH_ATTRIBUTE_BANISH = 72,
    SUMMON_STEP_BANISH_SUMMON = 73,         /* Special Summon after the banishes */
    SUMMON_STEP_KEY_1257_PROMPT = 80,       /* key 1257: Tribute the two named monsters */
    SUMMON_STEP_KEY_1257_PICK_1 = 81,
    SUMMON_STEP_KEY_1257_PROMPT_2 = 82,
    SUMMON_STEP_KEY_1257_PICK_2 = 83,
    SUMMON_STEP_KEY_1257_SUMMON = 84,
};

/* The level of a card ID as the Tribute rules see it: Trap, Magic and Ticket cards 0, Divine-Beasts 10,
 * monsters their stars (stats bits 25-28). */
static inline int GetSummonLevel(u16 id)
{
    int type = CARD_STATS_TYPE(CARD_STATS(id));

    switch (type) {
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

/* The card ID of a card number (gCardNumberToId; numbers 2000+ are the alternate art, the ID after the
 * original's; 0xFFFF maps to 0). Matching: the cast-literal table address gives the ROM's literal pool. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= CARD_NUMBER_ALT_ART - 1)
        return ((const u16 *)0x08623DF4)[number & CARD_ID_MASK];
    return ((const u16 *)0x08623DF4)[(number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/* Matching: IsTributableMonster returns u16 (duel.h); the ROM tests the result as an int (no narrowing). */
extern int IsTributableMonsterInt(int player, int zone) asm("IsTributableMonster");

/* Matching: the ROM narrows the card ID under the cursor to 16 bits (duel_screen.h declares a u32 return). */
extern u16 DuelCursor_GetCardId16(void) asm("DuelCursor_GetCardId");

/* Matching: CollectEffectTargets returns u16; the ROM compares the count as an int (no narrowing). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");

/* Prompts of the special summons (ROM). */
extern const char gStrSpecialSummonSelectTribute[];     /* "To Special Summon '%s', select %s as Tribute." */
extern const char gStrTributeFromField[];               /* "tribute %s from the Field." */
extern const char gStrTributeFromFieldOrHand[];         /* "tribute %s from either the Field or your hand." */
extern const char gStrTributeFromHand[];                /* "tribute %s from your hand." */
extern const char gStrBanishFieldMonstersCount[];       /* "Select from your own Field %d %s monster(s) for removal from play." */
extern const char gStrFiend[];                          /* "Fiend" */
extern const char gStrBanishGraveyardMonstersCount[];   /* "Select from Graveyard %d %s monster(s) for removal from play." */
extern const char gStrCardsRemaining[];                 /* "There are %d cards remaining." */
extern const char gStrLight[];                          /* "LIGHT" */
extern const char gStrBanishFieldMonster[];             /* "Select from your own Field %s monster for removal from play." */
extern const char gStrFire[];                           /* "FIRE" */
extern const char gStrBanishGraveyardMonster[];         /* "Select from Graveyard %s monster for removal from play." */
extern const char gStrWater[];                          /* "WATER" */
extern const char gStrEarth[];                          /* "EARTH" */
extern const char gStrWind[];                           /* "WIND" */
extern const char gStrTributeEitherFromField[];         /* "Select from Field either %s or %s as Tribute." */
extern const char gStrTributeOneFromField[];            /* "Select from Field %s as Tribute." */

/* Card IDs of Petit Moth and Labyrinth Wall: address-suffixed aliases of gCardNumberToId entries (card_data.h). */
extern const u16 gUnk_0862401E[];   /* &gCardNumberToId[CARD_PETIT_MOTH] */
extern const u16 gUnk_086240CE[];   /* &gCardNumberToId[CARD_LABYRINTH_WALL] */

/* The prompt box of the summon procedures: x 6, y 2; 18 x 7 cells for a question or a pick prompt (OPEN_PROMPT),
 * 18 x 4 cells for a message (OPEN_MESSAGE). */
#define OPEN_PROMPT(text)   TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, (const u8 *)(text))
#define OPEN_MESSAGE(text)  TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 4), TEXTBOX_FLAGS_DEFAULT, (const u8 *)(text))

/* The entries of gTributeSummonPrompts (include/effect.h). */
enum TributePrompt {
    TRIBUTE_PROMPT_ONE = 0,             /* "1 Tribute?" Yes/No question */
    TRIBUTE_PROMPT_TWO = 1,             /* "2 Tributes?" Yes/No question */
    TRIBUTE_PROMPT_SELECT_FIRST = 2,    /* select the first Tribute */
    TRIBUTE_PROMPT_SELECT_SECOND = 3,   /* select the second Tribute */
    TRIBUTE_PROMPT_SELECT_ONE = 4,      /* select a monster as the Tribute */
};

/* The Tribute argument of QueueNormalSummon / QueueSpecialSummonFromHand: one byte per Tribute holding
 * player << 4 | zone, with bit 7 set when that Tribute is used (hypothesis from the call sites). */
#define TRIBUTE_ONE     0x80
#define TRIBUTE_TWO     0x8080

/* Key 1257's two named Tributes: effect keys with no EDS card. */
#define KEY_1257_TRIBUTE_A  1410
#define KEY_1257_TRIBUTE_B  1412

/* Byte offset in gDuel of the word that holds step and tributeSources (CardMenu bits 34-49). */
#define CARD_MENU_STEP_WORD_OFFSET  0x1B30

/* The name record of a card ID from the integer address of gCardNames (0x0822C720). Matching: the integer base
 * is a reload in the ROM. */
#define CARD_NAME_INT(id)   ((const char *)0x0822C720 + (id) * CARD_NAME_SIZE)

/* Queue the pointer at the card the cursor picked (DUEL_CMD_POINT_AT_CARD: player, area | zone << 8). */
#define POINT_AT_PICKED_CARD() DuelCmd_Push32(DUEL_CMD_POINT_AT_CARD, gDuelScreen.selPlayer, PICKED_AREA_ZONE, 0)

/* Banish the graveyard card that the list viewer's cursor is on (the CPU's pick). */
#define BANISH_LISTED_CARD() \
    BanishGraveyardCard(gDuelScreen.selPlayer, (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow])

/*
 * Format into dstBuf the prompt fmtStr (one of the gStrTributeFrom* formats) with the name of the card with
 * number `number`. Matching (FAKEMATCH): the lookup is spelled out as an inlined CardNumberToId (gCardNumberToId,
 * then the 0x40-byte name record) with every temporary pinned to the register the ROM's reload rotation chose.
 * The alternate-art arm is dead (the numbers are below 2000) but is part of the ROM's code.
 */
#define FORMAT_TRIBUTE_PROMPT(dstBuf, fmtStr, number) { \
    register char *dst asm("r3") = (dstBuf); \
    register const char *fmt asm("r4") = (fmtStr); \
    register u32 id asm("r0"); \
    register unsigned offset asm("r2"); \
    register const char *names asm("r5"); \
    asm("" : : "r"(dst), "r"(fmt)); \
    if ((number) == 0xFFFF) { \
        id = 0; \
    } else if ((number) <= CARD_NUMBER_ALT_ART - 1) { \
        register unsigned index asm("r0") = ((number) & CARD_ID_MASK) * 2; \
        register const u16 *map asm("r2"); \
        asm("" : : "r"(index)); \
        map = gCardNumberToId; \
        asm("" : : "r"(map)); \
        id = *(const u16 *)(index + (u32)map); \
    } else { \
        register int adjustment asm("r5") = -CARD_NUMBER_ALT_ART; \
        register unsigned index asm("r0"); \
        register const u16 *map asm("r1"); \
        asm("" : : "r"(adjustment)); \
        index = (((number) + adjustment) & CARD_ID_MASK) * 2; \
        asm("" : : "r"(index)); \
        map = gCardNumberToId; \
        asm("" : : "r"(map)); \
        id = *(const u16 *)(index + (u32)map) + 1; \
    } \
    asm("" : : "r"(id)); \
    offset = id << 16; \
    asm("" : "+r"(offset)); \
    offset >>= 10; \
    asm("" : "+r"(offset)); \
    names = (const char *)gCardNames; \
    asm("" : : "r"(names)); \
    FormatStr(dst, fmt, (const char *)(offset + (u32)names)); \
}

/*
 * The handler of the card menu's Set / Summon / Special Summon commands for the hand card
 * gDuel.cardMenuCard. DuelCmdQueue calls it every frame while the command is confirmed (CM.confirmed): faceUp
 * says whether the card is summoned face up and special whether it is a Special Summon (the callers pass
 * (0, 0), (1, 0), (1, 1) or (0, 1)). It is a state machine on CM.step (enum SummonStep) and clears
 * CM.confirmed when the command is finished or cancelled.
 *
 * STEP_START chooses the procedure from the card number and, by default, from the level:
 *   level 0-4   the monster is placed at once (QueueNormalSummon / QueueSpecialSummonFromHand);
 *   level 5-6   "1 Tribute?" prompt (steps 10-12): pick one monster to Tribute;
 *   level 7+    "2 Tributes?" prompt (steps 1-6): pick two monsters to Tribute.
 * The picks use the duel cursor (DuelCursor_PickTarget); B steps back to the question. The summoned monster
 * takes the zone of the first Tribute. The special cards have their own steps:
 *   moths and Wall Shadow     20      Tribute the named monster (Petit Moth, Labyrinth Wall) from the field
 *   Gate Guardian             30-31   Tribute Sanga of the Thunder, Kazejin and Suijin in turn
 *   Valkyrion                 40-42   Tribute Alpha, Beta and Gamma in turn, from the hand and/or the field
 *   key 1514                  50-58   banish three Fiends from the field or the graveyard, then summon
 *   key 1515                  60-65   banish two LIGHT monsters
 *   keys 1516-1519            70-72   banish one FIRE / WATER / EARTH / WIND monster
 *   key 1257                  80-84   Tribute the two monsters of keys 1410 and 1412 from the field
 *   key 1250                  START   summon at once when the card is alone in the hand, else the 2-Tribute prompt
 *   key 1350                  START   a Special Summon command (11, 12) places it at once
 * The banish procedures take the monsters from the field with the cursor when the player's banishCostFromField
 * flag is set (the human) and from the graveyard list (CardListView_Open, area -1) otherwise.
 *
 * Matching: the function carries many FAKEMATCH register pins and empty asm statements; each one makes the
 * compiler keep a temporary in the register that the ROM's reload and allocation rotation chose.
 */
void CardMenu_SummonMonster(u16 faceUp, u16 special)
{
    char text[0x80];
    char format[0x80];
    struct ChainEntry summonedCard;     /* the card being summoned, for the Tribute check of step 20 */
    u16 requiredNumber; /* Assigned only for sequence values 0..2, as in the ROM. */
    switch (CM.step) {
    case SUMMON_STEP_START:
        switch (CARD_NUMBER(CM.cardMenuCard)) {
        case CARD_LARVAE_MOTH:
        case CARD_GREAT_MOTH:
        case CARD_PERFECTLY_ULTIMATE_GREAT_MOTH:
            /* The moths are Special Summoned by Tributing a Petit Moth. */
            FormatStr(format, gStrSpecialSummonSelectTribute, CARD_NAME(CM.cardMenuCard));
            FormatStr(text, format, CARD_NAME(gUnk_0862401E[0]));
            OPEN_PROMPT(text);
            CM.step = SUMMON_STEP_NAMED_TRIBUTE_PICK;
            break;
        case CARD_WALL_SHADOW:
            /* Wall Shadow is Special Summoned by Tributing a Labyrinth Wall. */
            FormatStr(format, gStrSpecialSummonSelectTribute, CARD_NAME(CM.cardMenuCard));
            FormatStr(text, format, CARD_NAME(gUnk_086240CE[0]));
            OPEN_PROMPT(text);
            CM.step = SUMMON_STEP_NAMED_TRIBUTE_PICK;
            break;
        case CARD_GATE_GUARDIAN:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_GATE_GUARDIAN_PROMPT;
            break;
        case CARD_VALKYRION_THE_MAGNA_WARRIOR:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_VALKYRION_PROMPT;
            break;
        case 1250:  /* no EDS card */
            /* handCount of the player's side (read from gDuel): summon at once when this card is alone in the
               hand (hypothesis: key 1250's Tributes come from the hand), else ask for two Tributes. */
            if (PLAYER_FROM_DUEL(CM.player).handCount == 1) {
                QueueNormalSummon(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, 1);
                CM.confirmed = 0;
            } else {
                OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_TWO]);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
                CM.summonTributes = 0;
                CM.step++;
            }
            break;
        case CARD_1257:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_KEY_1257_PROMPT;
            break;
        case CARD_1514:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_BANISH_FIENDS_PROMPT;
            break;
        case 1515:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_BANISH_LIGHT_PROMPT;
            break;
        case 1516:
        case 1517:
        case 1518:
        case 1519:
            CM.summonTributes = 0;
            CM.summonSeq = 0;
            CM.tributeSources = 0;
            CM.step = SUMMON_STEP_BANISH_ATTRIBUTE_PROMPT;
            break;
        case CARD_1350:
            switch (CM.command) {
            case CARDMENU_CMD_SP_SUMMON:
            case CARDMENU_CMD_SP_SUMMON_SET:
                QueueSpecialSummonFromHand(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, faceUp);
                CM.confirmed = 0;
                break;
            default:
                goto normal_summon;
            }
            break;
        default:
        normal_summon:
            /* FAKEMATCH: consume the level so its constant arms still enter the switch head tests. */
            switch (({ int level = GetSummonLevel(CM.cardMenuCard); asm volatile("" : : "r"(level)); level; })) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                /* No Tribute needed: summon into the first free monster zone. */
                if (special)
                    QueueSpecialSummonFromHand(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, faceUp);
                else
                    QueueNormalSummon(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, faceUp);
                if (CARD_NUMBER(CM.cardMenuCard) == CARD_1520) {
                    switch (CM.command) {
                    case CARDMENU_CMD_SP_SUMMON:
                    case CARDMENU_CMD_SP_SUMMON_SET:
                        /* Special Summoning key 1520's card also lets the opponent answer with a Special
                           Summon (hypothesis): if its graveyard has a revivable monster, it has a free zone
                           and may Special Summon, queue a RESPONSE_SPECIAL_SUMMONED trigger for it
                           (Chain_AddPending: card | zone << 16 | kind << 21 | event << 25 | player << 31). */
                        if (CollectEffectTargetsInt(1 - CM.player, CARD_CALL_OF_THE_HAUNTED, 0) != 0
                            && CountFreeMonsterZones(1 - CM.player) > 0
                            && (u16)CanSpecialSummon(1 - CM.player) != 0) {
                            u32 event;
                            u32 playerBit = (1u & CM.player) << 31;
                            /* FAKEMATCH: extract the event field through r2 before masking into r1. */
                            register u32 eventField asm("r2") = CM.placeZone;
                            asm("" : "+r"(eventField)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            event = ((eventField & 0x1F) << 16) | (CHAIN_KIND_MONSTER << 21) | (RESPONSE_SPECIAL_SUMMONED << 25);
                            /* FAKEMATCH: combine the event word before the player/card bits. */
                            asm("" : "+r"(event));
                            Chain_AddPending(playerBit | event | CM.cardMenuCard, 0);
                        }
                    }
                }
                CM.confirmed = 0;
                break;
            case 5:
            case 6:
                /* Level 5-6: ask whether to Tribute one monster. */
                OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_ONE]);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
                CM.summonTributes = 0;
                CM.step = SUMMON_STEP_ONE_TRIBUTE_ASK;
                break;
            default:
                /* Level 7 and up: ask whether to Tribute two monsters. */
                OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_TWO]);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
                CM.summonTributes = 0;
                CM.step++;
                break;
            }
            break;
        }
        break;

    /* ---- Two Tributes (level 7 and up) ---- */
    case SUMMON_STEP_TWO_TRIBUTES_ASK:
        if (gTextBox.result == 0) {         /* answered No: cancel */
            CM.confirmed = 0;
            break;
        }
        CM.step++;
        /* fall through */
    case SUMMON_STEP_TWO_TRIBUTES_PROMPT_1:
        OPEN_MESSAGE(gTributeSummonPrompts[TRIBUTE_PROMPT_SELECT_FIRST]);
        CM.step++;
        break;
    case SUMMON_STEP_TWO_TRIBUTES_PICK_1:
        if (gMain.newKeys & B_BUTTON) {
            OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_TWO]);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
            CM.summonTributes = 0;
            CM.step = SUMMON_STEP_TWO_TRIBUTES_ASK;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex)) {
                CM.summonTributes = PICKED_PLAYER_ZONE;
                PlaySE(SE_CONFIRM);
                POINT_AT_PICKED_CARD();
                CM.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_TWO_TRIBUTES_PICK_2:
        if (gMain.newKeys & B_BUTTON) {
            /* Back to the question: reopen it, clear the picks and set CM.step to SUMMON_STEP_TWO_TRIBUTES_ASK
               (the low byte of 4 in the masked word is step = 1). */
            OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_TWO]);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
            {
                struct StepBits { u16 lo:2; u16 step:8; u16 hi:6; u8 pad[4]; };
                register u8 *duel asm("r2") = (u8 *)&CM; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register int selectedOffset asm("r3") = OFFSET_OF(struct CardMenuView, summonTributes);
                register int stepOffset asm("r4"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register struct StepBits *step asm("r2");
                asm("" : : "r"(duel), "r"(selectedOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                {
                    register u16 *selected asm("r1") = (u16 *)((u32)duel + selectedOffset); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    asm("" : : "r"(selected));
                    *selected = 0;
                }
                stepOffset = CARD_MENU_STEP_WORD_OFFSET;
                asm("" : : "r"(stepOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                step = (struct StepBits *)((u32)duel + stepOffset);
                {
                    register u32 mask asm("r0") = 0xFFFFFC03; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    register u16 value asm("r5");
                    asm("" : : "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    value = *(u16 *)step;
                    asm("" : : "r"(value)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    *(u16 *)step = (mask & value) | (SUMMON_STEP_TWO_TRIBUTES_ASK << 2);
                }
            }
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            /* A different monster than the first pick, and tributable. */
            if (CM.summonTributes != ((u8)gDuelScreen.selIndex | ((u8)gDuelScreen.selPlayer << 8)) && IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex)) {
                PlaySE(SE_CONFIRM);
                POINT_AT_PICKED_CARD();
                /* Pack both picks: low byte the first (player << 4 | zone), high byte the second. */
                CM.summonTributes = (CM.summonTributes & 15) | (((u8)(CM.summonTributes >> 8) & 15) << 4)
                    | (((gDuelScreen.selIndex & 15) | ((gDuelScreen.selPlayer & 15) << 4)) << 8);
                CM.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_TWO_TRIBUTES_SUMMON:
        if (special)
            QueueSpecialSummonFromHand(CM.player, CM.index, CM.summonTributes & 7, CM.summonTributes | TRIBUTE_TWO, faceUp);
        else
            QueueNormalSummon(CM.player, CM.index, CM.summonTributes & 7, CM.summonTributes | TRIBUTE_TWO, faceUp);
        CM.confirmed = 0;
        break;

    /* ---- One Tribute (level 5-6) ---- */
    case SUMMON_STEP_ONE_TRIBUTE_ASK:
        if (!gTextBox.result) {             /* answered No: cancel */
            CM.confirmed = 0;
            break;
        }
        OPEN_MESSAGE(gTributeSummonPrompts[TRIBUTE_PROMPT_SELECT_ONE]);
        CM.step++;
        break;
    case SUMMON_STEP_ONE_TRIBUTE_PICK:
        if (gMain.newKeys & B_BUTTON) {
            OPEN_PROMPT(gTributeSummonPrompts[TRIBUTE_PROMPT_ONE]);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
            CM.summonTributes = 0;
            CM.step = SUMMON_STEP_ONE_TRIBUTE_ASK;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex)) {
                PlaySE(SE_CONFIRM);
                POINT_AT_PICKED_CARD();
                CM.summonTributes = (gDuelScreen.selIndex & 15) | ((gDuelScreen.selPlayer & 15) << 4);
                CM.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_ONE_TRIBUTE_SUMMON:
        QueueNormalSummon(CM.player, CM.index, CM.summonTributes & 7, CM.summonTributes | TRIBUTE_ONE, faceUp);
        CM.confirmed = 0;
        break;

    /* ---- Moths and Wall Shadow: Tribute the named monster ---- */
    case SUMMON_STEP_NAMED_TRIBUTE_PICK:
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            CM.confirmed = 0;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            struct PlayerBits { u8 low:1; u8 player:1; u8 high:6; u8 pad[4]; };
            /* FAKEMATCH: retain the ROM duel-base and player-field lifetimes. */
            register struct CardMenuView *duel asm("r8");
            register struct PlayerBits *player asm("r6"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            summonedCard.card = (duel = &CM)->cardMenuCard;
            /* the byte at gDuel + 0x1B33 holds CardMenu.player (bit 1) */
            summonedCard.player = (player = (struct PlayerBits *)((u8 *)duel + 0x1B33))->player;
            /* The check handler of the equipped-Tribute rule accepts the picked monster if it is the one the
               prompt names. */
            if (EffectEquippedTributeCheck(&summonedCard, PICKED_POS)) {
                u32 message;
                PlaySE(SE_CONFIRM);
                message = player->player ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD;
                DuelCmd_Push32(message, gDuelScreen.selPlayer, PICKED_AREA_ZONE, 0);
                TributeMonster(player->player, gDuelScreen.selIndex);
                QueueSpecialSummonFromHand(player->player, duel->index, gDuelScreen.selIndex, 0, faceUp);
                duel->confirmed = 0;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;

    /* ---- Gate Guardian: Tribute Sanga of the Thunder, Kazejin and Suijin in turn (summonSeq 0-2) ---- */
    case SUMMON_STEP_GATE_GUARDIAN_PROMPT: {
        FormatStr(text, gStrTributeFromField, CARD_NAME_INT(CardNumberToId(CM.summonSeq + CARD_SANGA_OF_THE_THUNDER)));
        OPEN_PROMPT(text);
        CM.step++;
        break;
    }
    case SUMMON_STEP_GATE_GUARDIAN_PICK:
        if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTERS)) {
            {
                /* FAKEMATCH: keep the cursor in r8. */
                register struct DuelScreen *cursor asm("r8") = &gDuelScreen;
                int player = cursor->selPlayer;
                {
                    int *zonePtr = &cursor->selIndex;
                    int zone = *zonePtr;
                    int pi = player & 1;
                    int offset = zone * ZONE_STRIDE + pi * PLAYER_STRIDE;
                    u8 *zoneBase = (u8 *)gDuelZones;
                    struct DuelZone *card = (struct DuelZone *)(offset + (int)zoneBase);
                    u32 id = CARD_ID(CARD_WORD(card->card));
                    if (id != 0 && card->isFaceUp) {
                        const u16 *numberPtr = &((const u16 *)0x08622AB4)[id & CARD_ID_MASK];
                        unsigned sequence = CM_FROM_ZONES.summonSeq;
                        /* The pick must be the Tribute whose turn it is (Sanga + sequence). */
                        if (*numberPtr == sequence + CARD_SANGA_OF_THE_THUNDER) {
                            u32 message = DUEL_CMD_POINT_AT_CARD;
                            if (player)
                                message = DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD;
                            {
                                u16 eventPlayer = player;
                                u8 *area = (u8 *)&cursor->selArea;
                                DuelCmd_Push32(message, eventPlayer, ((u8)zone << 8) | *area, 0);
                            }
                            TributeMonster(player, zone);
                            CM_FROM_ZONES.summonSeq++;
                            if (CM_FROM_ZONES.summonSeq <= 2) {
                                CM_FROM_ZONES.step--;       /* the next Tribute */
                            } else {
                                QueueSpecialSummonFromHand(CM_FROM_ZONES.player, CM_FROM_ZONES.index, *zonePtr, 0, faceUp);
                                CM_FROM_ZONES.confirmed = 0;
                            }
                        } else {
                            PlaySE(SE_ERROR);
                        }
                    }
                }
            }
        }
        break;

    /* ---- Valkyrion: Tribute Alpha, Beta and Gamma in turn, from the hand and/or the field ---- */
    /* State 0 enters with summonSeq = 0; state 41 repeats while summonSeq <= 2. */
    case SUMMON_STEP_VALKYRION_PROMPT:
        switch (CM.summonSeq) {
        case 0: requiredNumber = CARD_ALPHA_THE_MAGNET_WARRIOR; break;
        case 1: requiredNumber = CARD_BETA_THE_MAGNET_WARRIOR; break;
        case 2: requiredNumber = CARD_GAMMA_THE_MAGNET_WARRIOR; break;
        }
        /* tributeSources: the field if the required monster is face up there and key 1418 (which stops
           Tributes from the field) is not active; the hand if it is there (player 0 only, as in the ROM), and
           for Gamma also a free monster zone. */
        CM.tributeSources = 0;
        if (CountActiveCardsOnField(0, CARD_1418) == 0 && CountActiveCardsOnField(1, CARD_1418) == 0
            && CountFaceUpMonstersByNumber(0, requiredNumber) > 0)
            CM.tributeSources |= TRIBUTE_FROM_FIELD;
        if (CountHandCardsByNumber(0, requiredNumber) && (CM.summonSeq <= 1 || CountFreeMonsterZones(0) > 0))
            CM.tributeSources |= TRIBUTE_FROM_HAND;
        /* Choose the prompt: "tribute %s from either the Field or your hand", "from your hand" or "from the
           Field" (the three blocks below); with neither source the command ends. The sources are read as the
           word at gDuel + 0x1B30 (bits 14-17) through pinned registers. */
        {
            register unsigned off asm("r1"); register unsigned raw asm("r0"); register unsigned shifted asm("r2"); register unsigned choices asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            raw = (u32)&CM; asm("" : "+r"(raw)); off = CARD_MENU_STEP_WORD_OFFSET; asm("" : "+r"(off)); raw = *(u32 *)(raw + off);
            shifted = raw << 14;
            choices = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & TRIBUTE_FROM_HAND) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (choices & TRIBUTE_FROM_FIELD) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    FORMAT_TRIBUTE_PROMPT(text, gStrTributeFromFieldOrHand, requiredNumber)
                }
            }
        }
        {
            register unsigned off asm("r1"); register unsigned raw asm("r0"); register unsigned shifted asm("r2"); register unsigned choices asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            raw = (u32)&CM; asm("" : "+r"(raw)); off = CARD_MENU_STEP_WORD_OFFSET; asm("" : "+r"(off)); raw = *(u32 *)(raw + off);
            shifted = raw << 14;
            choices = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & TRIBUTE_FROM_HAND) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (!(choices & TRIBUTE_FROM_FIELD)) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    FORMAT_TRIBUTE_PROMPT(text, gStrTributeFromHand, requiredNumber)
                }
            }
        }
        if (!(CM.tributeSources & TRIBUTE_FROM_HAND) && (CM.tributeSources & TRIBUTE_FROM_FIELD))
            FORMAT_TRIBUTE_PROMPT(text, gStrTributeFromField, requiredNumber)
        {
        register char *t3 asm("r3"); /* FAKEMATCH: keep r3/r5 live across the test so reload picks the ROM spill registers. */
        register int t5 asm("r5");
        asm("" : "=r"(t3), "=r"(t5));
        /* Neither source is available (bits 14-17 of the word at gDuel + 0x1B30 are tributeSources): done. */
        if ((((struct { u8 prefix[0x1B30]; u32 w; } *)&CM)->w & 0x3C000) == 0) {
            CM.confirmed = 0;
            break;
        }
        asm("" : : "r"(t3), "r"(t5));
        }
        OPEN_PROMPT(text);
        CM.step++;
        break;
    case SUMMON_STEP_VALKYRION_PICK: {
        int mask;
        switch (CM.summonSeq) {
        case 0: requiredNumber = CARD_ALPHA_THE_MAGNET_WARRIOR; break;
        case 1: requiredNumber = CARD_BETA_THE_MAGNET_WARRIOR; break;
        case 2: requiredNumber = CARD_GAMMA_THE_MAGNET_WARRIOR; break;
        }
        /* Pick mask: the hand if it is a source, plus face-up field monsters if the field is one. */
        {
            register unsigned choiceBits asm("r1") = ((struct { u8 prefix[0x1B30]; u32 choices; } *)&CM)->choices << 14; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            register unsigned choices asm("r0") = choiceBits >> 28;
            mask = PICK_HAND;
            mask &= choices;
            asm("" : : "r"(choices), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            choiceBits = choices;
            asm("" : : "r"(choiceBits), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            if (choiceBits & TRIBUTE_FROM_FIELD)
                mask |= PICK_FACE_UP_MONSTERS;
        }
        if (DuelCursor_PickTarget(mask)) {
            int player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selIndex;
            int id = DuelCursor_GetCardId16();
            if (id) {
                if (CARD_NUMBER(id) == requiredNumber) {
                    u32 message = gDuelScreen.selPlayer ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD;
                    DuelCmd_Push32(message, gDuelScreen.selPlayer, PICKED_AREA_ZONE, 0);
                    switch (gDuelScreen.selArea) {
                    case DUEL_AREA_HAND: DiscardHandCard(player, zone, 0, 0); break;
                    case DUEL_AREA_MONSTER: TributeMonster(player, zone); break;
                    }
                    CM.summonSeq++;
                    if (CM.summonSeq <= 2)
                        CM.step--;      /* the next Tribute */
                    else
                        CM.step++;
                } else {
                    PlaySE(SE_ERROR);
                }
            }
        }
        break;
    }
    case SUMMON_STEP_VALKYRION_SUMMON:
        QueueSpecialSummonFromHand(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, faceUp);
        CM.confirmed = 0;
        break;

    /* ---- Key 1514: banish three Fiends ---- */
    case SUMMON_STEP_BANISH_FIENDS_PROMPT:
        if (BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            FormatStr(format, gStrBanishFieldMonstersCount, gStrFiend);
        else
            FormatStr(format, gStrBanishGraveyardMonstersCount, gStrFiend);
        FormatInt(text, format, 3);
        OPEN_PROMPT(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_FIENDS_PICK_1:
    case SUMMON_STEP_BANISH_FIENDS_PICK_2:
    case SUMMON_STEP_BANISH_FIENDS_PICK_3:
        /* The CPU's pick comes from the list of CollectEffectTargets (CardListView_Open); the human's from the
           field with the cursor. */
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer)) {
            CardListView_Open(gDuelScreen.selPlayer, -1, CARD_NUMBER(CM_FROM_PLAYERS.cardMenuCard), 0);
            CM_FROM_PLAYERS.step++;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if (GetZoneCardType(gDuelScreen.selPlayer, gDuelScreen.selIndex) == CARD_TYPE_FIEND) {
                BanishFieldCard(gDuelScreen.selPlayer, gDuelScreen.selIndex, 0);
                CM_FROM_PLAYERS.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_BANISH_FIENDS_BANISH_1:
    case SUMMON_STEP_BANISH_FIENDS_BANISH_2:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            BANISH_LISTED_CARD();
        CM_FROM_PLAYERS.step++;
        break;
    case SUMMON_STEP_BANISH_FIENDS_LEFT_2:
        FormatInt(text, gStrCardsRemaining, 2);
        OPEN_MESSAGE(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_FIENDS_LEFT_1:
        FormatInt(text, gStrCardsRemaining, 1);
        OPEN_MESSAGE(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_FIENDS_BANISH_3:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            BANISH_LISTED_CARD();
        CM_FROM_PLAYERS.step = SUMMON_STEP_BANISH_SUMMON;
        break;

    /* ---- Key 1515: banish two LIGHT monsters ---- */
    case SUMMON_STEP_BANISH_LIGHT_PROMPT:
        if (BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            FormatStr(format, gStrBanishFieldMonstersCount, gStrLight);
        else
            FormatStr(format, gStrBanishGraveyardMonstersCount, gStrLight);
        FormatInt(text, format, 2);
        OPEN_PROMPT(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_LIGHT_PICK_1:
    case SUMMON_STEP_BANISH_LIGHT_PICK_2:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer)) {
            CardListView_Open(gDuelScreen.selPlayer, -1, CARD_NUMBER(CM_FROM_PLAYERS.cardMenuCard), 0);
            CM_FROM_PLAYERS.step++;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if (GetZoneCardAttribute(gDuelScreen.selPlayer, gDuelScreen.selIndex) == ATTRIBUTE_LIGHT) {
                BanishFieldCard(gDuelScreen.selPlayer, gDuelScreen.selIndex, 0);
                CM_FROM_PLAYERS.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_BANISH_LIGHT_BANISH_1:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            BANISH_LISTED_CARD();
        CM_FROM_PLAYERS.step++;
        break;
    case SUMMON_STEP_BANISH_LIGHT_LEFT_1:
        FormatInt(text, gStrCardsRemaining, 1);
        OPEN_MESSAGE(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_LIGHT_BANISH_2:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            BANISH_LISTED_CARD();
        CM_FROM_PLAYERS.step = SUMMON_STEP_BANISH_SUMMON;
        break;

    /* ---- Keys 1516-1519: banish one FIRE / WATER / EARTH / WIND monster ---- */
    case SUMMON_STEP_BANISH_ATTRIBUTE_PROMPT:
        switch (CARD_NUMBER(CM.cardMenuCard)) {
        case 1516:  /* FIRE */
            if (((s32)((u32)PLAYER_FROM_DUEL(gDuelScreen.selPlayer & 1).flagsC << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrFire);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrFire);
            break;
        case 1517:  /* WATER */
            if (((s32)((u32)PLAYER_FROM_DUEL(gDuelScreen.selPlayer & 1).flagsC << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrWater);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrWater);
            break;
        case 1518:  /* EARTH */
            if (((s32)((u32)PLAYER_FROM_DUEL(gDuelScreen.selPlayer & 1).flagsC << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrEarth);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrEarth);
            break;
        case 1519:  /* WIND */
            if (((s32)((u32)PLAYER_FROM_DUEL(gDuelScreen.selPlayer & 1).flagsC << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrWind);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrWind);
            break;
        }
        OPEN_PROMPT(text);
        CM.step++;
        break;
    case SUMMON_STEP_BANISH_ATTRIBUTE_PICK:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer)) {
            CardListView_Open(gDuelScreen.selPlayer, -1, CARD_NUMBER(CM_FROM_PLAYERS.cardMenuCard), 0);
            CM_FROM_PLAYERS.step++;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            int attribute = 0;
            switch (CARD_NUMBER(CM_FROM_PLAYERS.cardMenuCard)) {
            case 1516: attribute = ATTRIBUTE_FIRE; break;
            case 1517: attribute = ATTRIBUTE_WATER; break;
            case 1518: attribute = ATTRIBUTE_EARTH; break;
            case 1519: attribute = ATTRIBUTE_WIND; break;
            }
            if (GetZoneCardAttribute(gDuelScreen.selPlayer, gDuelScreen.selIndex) == attribute) {
                BanishFieldCard(gDuelScreen.selPlayer, gDuelScreen.selIndex, 0);
                CM.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_BANISH_ATTRIBUTE_BANISH:
        if (!BANISH_FROM_FIELD(gDuelScreen.selPlayer))
            BANISH_LISTED_CARD();
        CM_FROM_PLAYERS.step++;
        break;
    case SUMMON_STEP_BANISH_SUMMON:
        QueueSpecialSummonFromHand(CM.player, CM.index, FindFreeMonsterZone(CM.player), 0, faceUp);
        CM.confirmed = 0;
        break;

    /* ---- Key 1257: Tribute the monsters of keys 1410 and 1412 from the field ---- */
    case SUMMON_STEP_KEY_1257_PROMPT: {
        int player = CM.player;
        int number1 = KEY_1257_TRIBUTE_A;
        u16 first = CountMonstersByNumber(player, number1) != 0;
        int player2 = CM.player;
        int number2 = KEY_1257_TRIBUTE_B;
        u16 second = CountMonstersByNumber(player2, number2) != 0;
        if (first) {
            if (second) {
                /* both on the field: either one will do */
                FormatStr(format, gStrTributeEitherFromField, CARD_NAME(CardNumberToId(number1)));
                /* A plain table read: SummonCardId's folded branches above end the CSE block, so the table
                 * constant is not kept in a register across the call (the ROM reloads it). */
                FormatStr(text, format, CARD_NAME(((const u16 *)0x08623DF4)[number2]));
                OPEN_PROMPT(text);
                CM.step++;
            } else {
                /* Integer name-table base: reload loads it here, as in the ROM. */
                FormatStr(text, gStrTributeOneFromField, CARD_NAME_INT(CardNumberToId(number1)));
                OPEN_PROMPT(text);
                CM.step++;
            }
        } else if (second) {
            /* Integer name-table base: reload loads it here, as in the ROM. */
            FormatStr(text, gStrTributeOneFromField, CARD_NAME_INT(CardNumberToId(number2)));
            OPEN_PROMPT(text);
            CM.step++;
        } else {
            CM.confirmed = 0;           /* neither is on the field */
        }
        break;
    }
    case SUMMON_STEP_KEY_1257_PICK_1:
        if (gMain.newKeys & B_BUTTON) {
            CM.step = SUMMON_STEP_KEY_1257_PROMPT;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex)) {
                /* The ROM uses cursor.area for this player term; preserve it. */
                int player = gDuelScreen.selArea & 1;
                int zone = gDuelScreen.selIndex;
                u32 word = *(u32 *)(player * PLAYER_STRIDE + zone * ZONE_STRIDE + (u32)gDuelZones);
                u16 number = *(u16 *)((u8 *)gCardIdToNumber + ((word << 21) >> 20));
                if (number == KEY_1257_TRIBUTE_A || number == KEY_1257_TRIBUTE_B) {
                    CM_FROM_ZONES.summonTributes = gDuelScreen.selIndex;
                    /* FAKEMATCH: retain the number through the selection store. */
                    asm("" : : "r"(number));
                    PlaySE(SE_CONFIRM);
                    POINT_AT_PICKED_CARD();
                    CM_FROM_ZONES.step++;
                } else {
                    PlaySE(SE_ERROR);
                }
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_TWO_TRIBUTES_PROMPT_2:
    case SUMMON_STEP_KEY_1257_PROMPT_2:
        OPEN_MESSAGE(gTributeSummonPrompts[TRIBUTE_PROMPT_SELECT_SECOND]);
        CM.step++;
        break;
    case SUMMON_STEP_KEY_1257_PICK_2:
        if (gMain.newKeys & B_BUTTON) {
            CM.step = SUMMON_STEP_KEY_1257_PROMPT;
        } else if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            /* The second pick must be another zone than the first (the low byte of summonTributes). */
            if ((u8)CM.summonTributes != (u8)gDuelScreen.selIndex && IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex)) {
                PlaySE(SE_CONFIRM);
                POINT_AT_PICKED_CARD();
                CM.summonTributes = ((u8)gDuelScreen.selIndex << 8) | (u8)CM.summonTributes;
                CM.step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        break;
    case SUMMON_STEP_KEY_1257_SUMMON:
        TributeMonster(CM.player, (u8)CM.summonTributes);
        TributeMonster(CM.player, CM.summonTributes >> 8);
        QueueSpecialSummonFromHand(CM.player, CM.index, (u8)CM.summonTributes & 7, 0, faceUp);
        CM.confirmed = 0;
        break;
    default:
        CM.confirmed = 0;
        break;
    }
}
