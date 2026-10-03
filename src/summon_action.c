/*
 * summon_action (0x08054E7C-0x08055EB0): the summon record and its step machines (wiki/functions/summon-action-c.md).
 *
 * Every summon in a duel goes through the one pending record gSummonAction (struct SummonAction, summon.h):
 * a Queue* builder fills it for a Normal Summon / Set (kinds 1, 2), a Flip Summon (3) or a Special Summon (4-6)
 * and calls SummonAction_Start. From then on DuelMainStep calls SummonAction_Update every frame. It runs the
 * step machine of the record's kind, which queues the duel commands that place and show the card and the
 * triggers of the cards involved. When the machine is done, the post-summon effects are applied and the
 * opponent gets a response window (SUMMONED, SET, FLIP_SUMMONED or SPECIAL_SUMMONED).
 *
 * The step machines of kinds 1 and 2 (ExecuteSummonAction, ExecuteSummonActionAskPosition) are in summon_checks.c.
 * In a link duel a summon of player 1 (the partner) is sent to the partner as LINKMSG_QUEUED_ACTION and runs
 * there; this side waits for LINKMSG_QUEUED_ACTION_DONE instead of running the steps.
 */
#include "global.h"
#include "card_data.h"              /* gCardNumberToId, CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_LEVEL */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_STATS_* layout, enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, ZoneStatusFlag, ResponseEventKind, ChainEntryKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_cmd.h, duel_screen.h, duel_link.h and summon.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with #include "duel.h"
 * (see build/readability/issues/summon_action.md). */
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
    u8 unk6_2:6;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u8 unk0[0x8];
    u8 unk8_0:4;
    u8 normalSummonUsed:1;          /* +0x008 bit 4: the Normal Summon of this turn is done */
    u8 summonedThisTurn:1;          /* +0x008 bit 5: a summon/set action was started this turn */
    u8 unk8_6:2;
    u8 unk9[0xD64 - 0x9];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

u32 IsToonMonster(u16 cardNo);
u32 HasFlipEffect(u16 cardNo, int inBattle);
void CopyDuelCard(u32 *dst, u32 *src);
int CountActiveCardsOnField(int player, u16 cardNo);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* AiShouldSetMonster */
#include "chain.h"                  /* Chain_AddPending, EventResponse_Request */
#include "duel_actions.h"           /* DestroyFieldCard, TributeMonster, ChangeBattlePosition, ShowCardEffect, DrawCards */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl, gStrSelectDisplayPosition */
#include "duel_link.h"              /* DuelLink_SendMessage, DuelLink_SendMessageData */
#include "duel_screen.h"            /* DuelCursor_Select */
#include "effect.h"                 /* CanActivateEffectOfCard, PayChainEnergyCost, the passive card hooks */
#include "summon.h"                 /* struct SummonAction, gSummonAction, the summon functions */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/* Matching: the unit calls DuelCmd_Push with four int parameters. With the header's u16 cmd and arg2 the
 * register allocation of the Special step machines (kinds 4-6) differs. Same function, other view. */
extern void DuelCmd_PushInt(int cmd, int arg2, int arg4, int arg6) asm("DuelCmd_Push");

/* Matching: SummonAction_Update tests the kind 1 and 2 machines and EventResponse_Request through other
 * views than the headers give: u16 results (the call is followed by lsl/lsr 16) and int arguments. */
extern u16 ExecuteSummonActionU16(void) asm("ExecuteSummonAction");
extern u16 ExecuteSummonActionAskPositionU16(void) asm("ExecuteSummonActionAskPosition");
extern void EventResponse_RequestInt(int player, int event, int arg) asm("EventResponse_Request");

/* Command id for DuelCmd_Push: DUEL_CMD_PLAYER (bit 15) marks a command of player 1. */
#define PLAYER_CMD(player, cmd) ((player) ? (DUEL_CMD_PLAYER | (cmd)) : (cmd))

/* A field location, player | zone << 8 (DUEL_LOC with the operands in the order the ROM evaluates them: DUEL_LOC's
 * zone-first order changes the registers). */
#define LOC(player, zone) ((player) | ((zone) << 8))

/* Chain_AddPending trigger word, as ChainEntry lays it out: player << 31 | event << 25 | kind << 21 | zone << 16 |
 * card ID. These are the bits that do not depend on the card: the event and the monster kind. */
#define MONSTER_TRIGGER_BITS(event) (((event) << 25) | (CHAIN_KIND_MONSTER << 21))

/* Card ID of a struct DuelCard, read as the whole word and masked. Matching: the bitfield access card.id loads
 * a halfword (ldrh), the ROM loads the word (ldr; lsl 20; lsr 20). */
#define CARD_ID_OF(card) (((*(u32 *)&(card)) << 20) >> 20)

/* Key 1526 has no EDS card and no effect row, so constants/cards.h has no name for it. */
#define CARD_1526 1526

#define ZONE_STRIDE 0x94        /* sizeof(struct DuelZone) */
#define PLAYER_STRIDE 0xD64     /* sizeof(struct DuelPlayer) */
/* The zone (p, z) with the address terms staged zone first (the ROM's order). Matching: SummonAction_Update
 * needs this macro; the FieldZone() inline below gives another instruction order there. */
#define ZONE(p, z) ((struct DuelZone *)((z) * ZONE_STRIDE + (p) * PLAYER_STRIDE + (u32)gDuelZones))

/* Matching: ROM tables read through integer-constant addresses (the ROM reloads the table address at every
 * use; the symbols gCardIdToNumber and gCardStats generate other literal pools). */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])  /* gCardStats */

/* Matching: the record's +0xE flag byte (active, linked, kind) is loaded as one byte and tested with masks and
 * shifts, as the ROM does; accessing the bitfields active / linked / kind separately gives other code. */
struct SummonActionFlagsByte {
    u8 unk0[0xE];
    u8 flags;                   /* +0xE: bit 0 active, bit 1 linked, bits 2-4 kind */
};
#define SUMMON_FLAG_ACTIVE 0x1
#define SUMMON_FLAG_LINKED 0x2
#define SUMMON_FLAGS_KIND(flags) (((u32)(flags) << 27) >> 29)
#define SUMMON_FLAGS(action) (((struct SummonActionFlagsByte *)(action))->flags)

/*
 * Card number -> card ID through gCardNumberToId.
 * FAKEMATCH: the original table lookup keeps its initialized address in r0.
 */
static inline int CardNumberToIdR0(int number)
{
    register u32 address __asm__("r0") = number * 2;
    address += (u32)gCardNumberToId;
    return *(const u16 *)address;
}

/* FAKEMATCH: the same lookup with the doubled table index retained in r1. */
static inline u16 CardNumberToIdR1(int number)
{
    register u32 off asm("r1") = number * 2;
    off += (u32)gCardNumberToId;
    return *(const u16 *)off;
}

/* Zone address of (player, zone). Matching: the address terms are staged in the same order as the record
 * loads (zone term, player term, base). */
static inline struct DuelZone *FieldZone(int player, int zone)
{
    int off = zone * ZONE_STRIDE;
    off += player * PLAYER_STRIDE;
    off += (u32)gDuelZones;
    return (struct DuelZone *)off;
}

/* The zone the record points at: its player and destination zone. */
static inline struct DuelZone *SummonZone(struct SummonAction *action)
{
    int player = action->player & 1;
    int zone = action->zone;
    return FieldZone(player, zone);
}

/* Monster level of a card ID the way the summon rules read it: Trap, Magic and Ticket cards count as 0,
 * the three Egyptian Gods as 10, every other card by its stars (gCardStats bits 25-28). */
static inline u32 SummonCardLevel(u32 id)
{
    switch ((int)CARD_STATS_TYPE(CARD_STATS_WORD(id))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_WORD(id));
    }
}

/* Steps of SummonStep_Flip (kind 3). */
enum FlipStep {
    FLIP_STEP_TURN_FACE_UP,         /* change position or flip the card, Crass Clown trigger */
    FLIP_STEP_SHOW_CARD,            /* select the zone, card assembles on screen */
    FLIP_STEP_GOBLIN_FAN,           /* Goblin Fan destroys a Flip Summoned monster of level 2 or lower */
    FLIP_STEP_STATUS_AND_TRIGGERS   /* status flags, Mysterious Puppeteer, flip triggers */
};

/*
 * Kind 3 step machine: Flip Summon of the face-down monster in (player, zone). Returns 1 when done.
 */
u16 SummonStep_Flip(void)
{
    struct SummonAction *action = &gSummonAction;
    switch (action->step) {
    case FLIP_STEP_TURN_FACE_UP: {
        int player = action->player;
        int side = player & 1;
        int zone = action->zone;
        /* A monster in Defense Position changes position (and is turned face up); any other card is flipped. */
        if (FieldZone(side, zone)->isDefense) {
            u32 id;
            DuelCmd_PushInt(PLAYER_CMD(player, DUEL_CMD_CHANGE_POSITION), zone, 1, 0);
            id = CARD_ID_OF(SummonZone(action)->card);
            /* Crass Clown queues its trigger for the position change (event POSITION_CHANGED). */
            if (CARD_NUMBER(id) == CARD_CRASS_CLOWN && CanActivateEffectOfCard(action->player, id, 0)) {
                int owner = action->player & 1;
                u32 trigger = (u32)owner << 31;
                int zoneIndex = action->zone;
                u32 destination = zoneIndex << 16;
                destination |= MONSTER_TRIGGER_BITS(RESPONSE_POSITION_CHANGED);
                trigger |= destination;
                trigger |= CARD_ID_OF(FieldZone(owner, zoneIndex)->card);
                Chain_AddPending(trigger, 0);
            }
        } else {
            DuelCmd_PushInt(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 1, 0);
        }
        gSummonAction.step++;
        return 0;
    }
    case FLIP_STEP_SHOW_CARD:
        DuelCursor_Select(action->player, 0, action->zone);
        DuelCmd_PushInt(DUEL_CMD_SHOW_CARD_ASSEMBLE, action->cardId, 1, 0);
        action->step++;
        return 0;
    case FLIP_STEP_GOBLIN_FAN:
        if (SummonCardLevel(action->cardId) <= 2) {
            int number = CARD_GOBLIN_FAN;
            if (CountActiveCardsOnField(0, number) > 0 || CountActiveCardsOnField(1, number) > 0) {
                ShowCardEffect(gSummonAction.player, CardNumberToIdR1(number));
                DestroyFieldCard(gSummonAction.player, gSummonAction.zone, 1);
                return 1;
            }
        }
        gSummonAction.step++;
        return 0;
    case FLIP_STEP_STATUS_AND_TRIGGERS:
        DuelCmd_PushInt(PLAYER_CMD(action->player, DUEL_CMD_SET_ZONE_STATUS_FLAGS), action->zone, action->statusFlags, 0);
        {
            /* FAKEMATCH: preserve the initialized record byte in its original scratch. */
            register u32 byte asm("r3") = *(u8 *)action;
            asm("" : : "r"(byte));
            TriggerMysteriousPuppeteer((byte << 31) >> 31);
        }
        switch (CARD_NUMBER(action->cardId)) {
        /* These cards queue their trigger by number when Flip Summoned (event FLIP_SUMMONED). */
        case CARD_DRAGON_SEEKER:
        case CARD_SENJU_OF_THE_THOUSAND_HANDS:
        case CARD_SONIC_BIRD:
        case CARD_1240:
        case CARD_1246:
        case CARD_1332:
            if (CanActivateEffectOfCard(gSummonAction.player, CARD_ID_OF(SummonZone(&gSummonAction)->card), 0)) {
                int player = gSummonAction.player;
                u32 trigger = (u32)(player & 1) << 31;
                int zone = gSummonAction.zone;
                u32 destination = zone << 16;
                destination |= MONSTER_TRIGGER_BITS(RESPONSE_FLIP_SUMMONED);
                trigger |= destination;
                trigger |= gSummonAction.cardId;
                Chain_AddPending(trigger, LOC(player, zone));
            }
            break;
        case CARD_TOTAL_DEFENSE_SHOGUN:
            /* Total Defense Shogun goes back to Defense Position when it is Flip Summoned. */
            ChangeBattlePosition(action->player, action->zone, 0, 0);
            break;
        }
        /* A monster with a flip effect activates it, unless key 1530 is on either field. */
        if (HasFlipEffect(CARD_NUMBER(gSummonAction.cardId), 0)) {
            if (CanActivateEffectOfCard(gSummonAction.player, CARD_ID_OF(SummonZone(&gSummonAction)->card), 0) &&
                !CountActiveCardsOnField(0, CARD_1530) && !CountActiveCardsOnField(1, CARD_1530)) {
                int player = gSummonAction.player;
                u32 trigger = (u32)(player & 1) << 31;
                int zone = gSummonAction.zone;
                u32 destination = zone << 16;
                destination |= MONSTER_TRIGGER_BITS(RESPONSE_FLIP_SUMMONED);
                trigger |= destination;
                trigger |= gSummonAction.cardId;
                Chain_AddPending(trigger, LOC(player, zone));
            }
        }
        gSummonAction.step++;
        return 0;
    default:
        return 1;
    }
}

/* Steps of SummonStep_Special (kind 4). */
enum SpecialStep {
    SPECIAL_STEP_PLACE_CARD,        /* put the card into the zone */
    SPECIAL_STEP_SHOW_CARD,
    SPECIAL_STEP_STATUS_AND_TRIGGER /* status flags, key 1246 trigger */
};

/*
 * Kind 4 step machine: Special Summon of the card copied into the record, in the position the builder chose
 * (isFaceUp, isDefense). Returns 1 when done.
 */
u16 SummonStep_Special(void)
{
    struct SummonAction *action = &gSummonAction;
    switch (action->step) {
    case SPECIAL_STEP_PLACE_CARD: {
        int command = PLAYER_CMD(action->player, DUEL_CMD_PLACE_CARD);
        int zone = action->zone;
        int faceUp = action->isFaceUp;
        int position = action->isDefense << 1;
        position |= faceUp;
        /* arg2: zone | faceUp << 8 | defense << 9; arg4 | arg6 << 16: the card word. */
        DuelCmd_PushInt(command, zone | (position << 8),
                    *(u16 *)&action->card, *((u16 *)&action->card + 1));
        action->step++;
        return 0;
    }
    case SPECIAL_STEP_SHOW_CARD:
        DuelCursor_Select(action->player, 0, action->zone);
        DuelCmd_PushInt(DUEL_CMD_SHOW_CARD_ASSEMBLE, action->cardId, 1, 0);
        action->step++;
        return 0;
    case SPECIAL_STEP_STATUS_AND_TRIGGER: {
        int command = PLAYER_CMD(action->player, DUEL_CMD_SET_ZONE_STATUS_FLAGS);
        int id;
        DuelCmd_PushInt(command, action->zone, action->statusFlags, 0);
        id = action->cardId;
        /* Key 1246 (no EDS card) queues its trigger after the Special Summon (event FLIP_SUMMONED). */
        if (CARD_NUMBER(id) == CARD_1246) {
            int player = action->player;
            u32 trigger = player << 31;
            int zone = action->zone;
            u32 destination = zone << 16;
            destination |= MONSTER_TRIGGER_BITS(RESPONSE_FLIP_SUMMONED);
            trigger |= destination;
            trigger |= id;
            Chain_AddPending(trigger, LOC(player, zone));
        }
        action->step++;
        return 0;
    }
    default:
        return 1;
    }
}

/* Steps of SummonStep_SpecialChoosePosition (kind 5). */
enum SpecialChoosePositionStep {
    CHOOSE_POSITION_STEP_ASK,           /* the CPU answers, the human gets the position menu */
    CHOOSE_POSITION_STEP_PLACE_CARD,    /* store the answer, put the card into the zone */
    CHOOSE_POSITION_STEP_SHOW_CARD,
    CHOOSE_POSITION_STEP_STATUS_AND_TRIGGERS   /* status flags, key 1246 trigger, key 1526 */
};

/*
 * Kind 5 step machine: Special Summon of the card copied into the record, in a position chosen now
 * (CPU: AiShouldSetMonster; human: the "Select display position of card." menu). Returns 1 when done.
 */
u16 SummonStep_SpecialChoosePosition(void)
{
    struct SummonAction *action = &gSummonAction;
    struct DuelCard copy;
    switch (action->step) {
    case CHOOSE_POSITION_STEP_ASK:
        if (action->player) {
            gTextBox.result = AiShouldSetMonster(action->cardId, action->isFaceUp);
        } else {
            /* Text box at cell (7, 2), 15 x 3 cells; the draw/input callbacks are in summon_checks.c. */
            TextBoxOpen(0x207, 0x30F, TEXTBOX_FLAGS_DEFAULT, gStrSelectDisplayPosition);
            TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, SummonPositionMenu_Draw, SummonPositionMenu_HandleInput);
        }
        action->step++;
        return 0;
    case CHOOSE_POSITION_STEP_PLACE_CARD: {
        int command, zone, faceUp, position;
        /* The answer is 0 for Attack Position, 1 for Defense; Attack Position is always face up. */
        action->isDefense = gTextBox.result;
        if (!action->isDefense) action->isFaceUp = 1;
        CopyDuelCard((u32 *)&copy, (u32 *)&action->card);
        command = PLAYER_CMD(action->player, DUEL_CMD_PLACE_CARD);
        zone = action->zone;
        faceUp = action->isFaceUp;
        position = action->isDefense << 1;
        position |= faceUp;
        DuelCmd_PushInt(command, zone | (position << 8), (u16)*(u32 *)&copy, *(u32 *)&copy >> 16);
        action->step++;
        return 0;
    }
    case CHOOSE_POSITION_STEP_SHOW_CARD:
        DuelCursor_Select(action->player, 0, action->zone);
        DuelCmd_PushInt(DUEL_CMD_SHOW_CARD_ASSEMBLE, action->cardId, 1, 0);
        action->step++;
        return 0;
    case CHOOSE_POSITION_STEP_STATUS_AND_TRIGGERS: {
        int command = PLAYER_CMD(action->player, DUEL_CMD_SET_ZONE_STATUS_FLAGS);
        int id, number;
        DuelCmd_PushInt(command, action->zone, action->statusFlags, 0);
        id = action->cardId;
        number = CARD_NUMBER(id);
        switch (number) {
        case CARD_1246: {
            int player = action->player;
            u32 trigger = player << 31;
            int zone = action->zone;
            u32 destination = zone << 16;
            destination |= MONSTER_TRIGGER_BITS(RESPONSE_FLIP_SUMMONED);
            trigger |= destination;
            trigger |= id;
            Chain_AddPending(trigger, LOC(player, zone));
        }
        break;
        case CARD_1526: {
            /* Key 1526 (no EDS card): the player's other monsters are destroyed. */
            int i;
            for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
                if (i != gSummonAction.zone)
                    DestroyFieldCard(gSummonAction.player, i, 1);
            }
        }
        break;
        }
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}

/* Steps of SummonStep_SpecialFromHand (kind 6). */
enum SpecialFromHandStep {
    FROM_HAND_STEP_TRIBUTE_AND_PLACE,   /* tribute the chosen monsters, place the card from the hand */
    FROM_HAND_STEP_SHOW_CARD,
    FROM_HAND_STEP_STATUS_AND_TRIGGER   /* status flags, key 1246 trigger */
};

/*
 * Kind 6 step machine: Special Summon of the hand card (sourceIndex) face up into the zone, after tributing the
 * monsters the record names (all on the summoning player's side). Returns 1 when done.
 */
u16 SummonStep_SpecialFromHand(void)
{
    struct SummonAction *action = &gSummonAction;
    switch (action->step) {
    case FROM_HAND_STEP_TRIBUTE_AND_PLACE: {
        u16 command;
        int id;
        if (action->hasTribute1) TributeMonster(action->player, action->tribute1Zone);
        if (action->hasTribute2) TributeMonster(action->player, action->tribute2Zone);
        if (action->hasTribute3) TributeMonster(action->player, action->tribute3Zone);
        command = PLAYER_CMD(action->player, DUEL_CMD_PLACE_MONSTER_FROM_HAND);
        id = action->cardId;
        {
            /* arg4: zone | hand index << 4 | faceUp << 8 | defense << 9. The hand index is the low 4 bits
             * of sourceIndex (record bits 6-9).
             * FAKEMATCH: the initialized shifted field stays in r0 until masked. */
            register u32 shifted asm("r0") = *(u16 *)action >> 6;
            u32 mask = 15;
            u32 packed = mask;
            packed &= shifted;
            packed <<= 4;
            mask &= action->zone;
            packed |= mask;
            packed |= (action->isFaceUp | action->isDefense << 1) << 8;
            DuelCmd_PushInt(command, id, packed, 0);
        }
        action->step++;
        return 0;
    }
    case FROM_HAND_STEP_SHOW_CARD:
        DuelCursor_Select(action->player, 0, action->zone);
        DuelCmd_PushInt(DUEL_CMD_SHOW_CARD_ASSEMBLE, action->cardId, 1, 0);
        action->step++;
        return 0;
    case FROM_HAND_STEP_STATUS_AND_TRIGGER: {
        int command = PLAYER_CMD(action->player, DUEL_CMD_SET_ZONE_STATUS_FLAGS);
        int id;
        DuelCmd_PushInt(command, action->zone, action->statusFlags, 0);
        id = action->cardId;
        /* Key 1246 (no EDS card) queues its trigger after the Special Summon (event SUMMONED). */
        if (CARD_NUMBER(id) == CARD_1246) {
            int player = action->player;
            u32 trigger = player << 31;
            int zone = action->zone;
            u32 destination = zone << 16;
            destination |= MONSTER_TRIGGER_BITS(RESPONSE_SUMMONED);
            trigger |= destination;
            trigger |= id;
            Chain_AddPending(trigger, LOC(player, zone));
        }
        action->step++;
        return 0;
    }
    default:
        return 1;
    }
}

/*
 * Per-frame driver of the pending summon, called by DuelMainStep. Runs the step machine of the record's kind
 * and, once it is done, the post-summon effects and the opponent's response window. Returns nonzero while the
 * summon is still in progress (the record's active bit).
 *
 * FAKEMATCH: the initialized u16 copy of the card ID below (savedId) preserves the original r8-to-r2
 * transfer. Its input is a 12-bit card id, so narrowing is lossless.
 */
u16 SummonAction_Update(void)
{
    int flags = SUMMON_FLAGS(&gSummonAction);
    if (flags & SUMMON_FLAG_ACTIVE) {
        /* A summon of player 1 that was mirrored over the link runs on the partner; wait for its answer. */
        if (!(SUMMON_FLAG_LINKED & flags) || !gSummonAction.player) {
            switch (SUMMON_FLAGS_KIND(flags)) {
            case SUMMON_ACTION_NORMAL: if (!ExecuteSummonActionU16()) return 1; break;
            case SUMMON_ACTION_NORMAL_CHOOSE_POSITION: if (!ExecuteSummonActionAskPositionU16()) return 1; break;
            case SUMMON_ACTION_FLIP: if (!SummonStep_Flip()) return 1; break;
            case SUMMON_ACTION_SPECIAL: if (!SummonStep_Special()) return 1; break;
            case SUMMON_ACTION_SPECIAL_CHOOSE_POSITION: if (!SummonStep_SpecialChoosePosition()) return 1; break;
            case SUMMON_ACTION_SPECIAL_FROM_HAND: if (!SummonStep_SpecialFromHand()) return 1; break;
            }
        } else {
            if (!gLinkState.queuedActionDone) return 1;    /* LINKMSG_QUEUED_ACTION_DONE not received yet */
        }
        {
            struct SummonAction *action = &gSummonAction;
            struct DuelZone *zone;
            int card;
            action->active = 0;
            {
                int player = action->player;
                int index = action->zone;
                zone = ZONE(player, index);
            }
            card = CARD_ID_OF(zone->card);
            /* The post-summon effects are skipped when the monster has already left the zone
             * (Goblin Fan destroying a Flip Summoned monster). */
            if (card != 0) {
                int id = card;
                int wasFaceUp = zone->isFaceUp;
                DuelCursor_Select(action->player, 0, action->zone);
                /* Key 1429: for a monster Special Summoned from the graveyard, draw one card per copy. */
                if (action->statusFlags & ZONE_STATUS_FROM_GRAVEYARD) {
                    int player = action->player;
                    int number = CARD_1429;
                    int count = CountActiveCardsOnField(player, number);
                    if (count > 0) {
                        DuelCmd_PushInt(PLAYER_CMD(action->player, DUEL_CMD_SHOW_CARD_EFFECT), CardNumberToIdR0(number), 1, 0);
                        DrawCards(action->player, count);
                    }
                }
                /* Key 1552 on either field: a face-up summoned monster gets the cannot-attack flag. */
                if (wasFaceUp) {
                    int number = CARD_1552;
                    if (CountActiveCardsOnField(0, number) || CountActiveCardsOnField(1, number)) {
                        DuelCmd_PushInt(PLAYER_CMD(gSummonAction.player, DUEL_CMD_SHOW_CARD_EFFECT), CardNumberToIdR0(number), 1, 0);
                        DuelCmd_PushInt(PLAYER_CMD(gSummonAction.player, DUEL_CMD_SET_CANNOT_ATTACK), gSummonAction.zone, 1, 0);
                    }
                }
                {
                    u32 index = CARD_ID_MASK;
                    /* FAKEMATCH: the 12-bit card ID is kept in r2 as an initialized u16 (see above). */
                    register u16 savedId __asm__("r2") = id;
                    int number;
                    index &= savedId;
                    number = ((const u16 *)0x08622AB4)[index];     /* gCardIdToNumber */
                    switch (number) {
                    case CARD_PUMPKING_THE_KING_OF_GHOSTS:
                        ApplyPumpkingBoost(gSummonAction.player, gSummonAction.zone);
                        break;
                    case CARD_JINZO:
                        /* Face-up Jinzo negates the Traps on the field. */
                        if (wasFaceUp) {
                            DuelCmd_PushInt(PLAYER_CMD(gSummonAction.player, DUEL_CMD_SHOW_CARD_EFFECT), CardNumberToIdR0(number), 1, 0);
                            DisableFaceUpTraps();
                        }
                        break;
                    case CARD_KOTODAMA:
                        if (wasFaceUp) ApplyKotodama();
                        break;
                    }
                }
                if (wasFaceUp)
                    ApplyKotodamaToZone(gSummonAction.player, gSummonAction.zone);
                {
                    u32 byte = *(u8 *)&gSummonAction;
                    ApplyDragonCaptureJar((byte << 31) >> 31);
                }
                {
                    /* The opponent may respond to the summon: SUMMONED or SET for a Normal Summon / Set,
                     * FLIP_SUMMONED for a Flip Summon, SPECIAL_SUMMONED for the rest. */
                    int event;
                    u32 kindFlags = SUMMON_FLAGS(&gSummonAction);
                    switch ((int)SUMMON_FLAGS_KIND(kindFlags)) {
                    case SUMMON_ACTION_NORMAL:
                    case SUMMON_ACTION_NORMAL_CHOOSE_POSITION: {
                        int choice = gSummonAction.isFaceUp ? RESPONSE_SUMMONED : RESPONSE_SET;
                        event = choice;
                        break;
                    }
                    case SUMMON_ACTION_FLIP: event = RESPONSE_FLIP_SUMMONED; break;
                    default: event = RESPONSE_SPECIAL_SUMMONED; break;
                    }
                    EventResponse_RequestInt(1 - gSummonAction.player, event,
                                 LOC(gSummonAction.player, gSummonAction.zone));
                }
            }
        }
        /* A summon that came over the link (player 0 here) tells the partner it is finished. */
        if ((SUMMON_FLAGS(&gSummonAction) & SUMMON_FLAG_LINKED) && !gSummonAction.player)
            DuelLink_SendMessage(LINKMSG_QUEUED_ACTION_DONE, 0, 0, 0);
    }
    return gSummonAction.active;
}

/*
 * Activate the record the builder has filled: clear the step and the unused progress fields, set active and
 * mark the player as having summoned this turn. For player 1 in a link duel, mirror the summon: set linked,
 * send the 0x14-byte record to the partner (LINKMSG_QUEUED_ACTION) and clear queuedActionDone.
 */
void SummonAction_Start(void)
{
    struct SummonAction *action = &gSummonAction;
    action->step = 0;
    action->unkE_12 = 0;
    action->unk10_0 = 0;
    action->unk10_3 = 0;
    action->unk10_11 = 0;
    action->active = 1;
    action->linked = 0;
    gDuelPlayers[action->player & 1].summonedThisTurn = 1;
    if (action->player && gDuelCtrl.isLinkDuel) {
        action->linked = 1;
        DuelLink_SendMessageData(LINKMSG_QUEUED_ACTION, action, sizeof(struct SummonAction));
        gLinkState.queuedActionDone = 0;
    }
}

/*
 * Start a record that the link partner sent (DuelLink_PollMessage has already copied it into gSummonAction).
 * The partner's player 1 is this side's player 0, so the player is reset to 0, the progress fields are
 * cleared, the record is active and linked (it answers LINKMSG_QUEUED_ACTION_DONE when finished), and the
 * owner bit of the copied card word is flipped to this side's point of view.
 */
void SummonAction_StartFromLink(void)
{
    struct SummonAction *action = &gSummonAction;
    action->player = 0;
    action->step = 0;
    action->unkE_12 = 0;
    action->unk10_0 = 0;
    action->unk10_3 = 0;
    action->unk10_11 = 0;
    action->active = 1;
    action->linked = 1;
    {
        struct DuelCard *card = &action->card;
        card->owner = 1 - card->owner;
    }
}

/* Bit 14 of the record is isFaceUp: bit 6 of its byte 1. */
#define RECORD_BYTE1_FACE_UP 0x40

/*
 * Kind 1: queue a Normal Summon (faceUp nonzero: face-up Attack) or Set (face-down Defense) of
 * hand[handIndex] into zone, and start it. tributes: two bytes, the low one for the first tribute, each
 * zone (bits 0-2) | owner (bit 4) | present (bit 7). Only the low halfword of the tributes and faceUp
 * words is used.
 *
 * Light of Intervention on either field makes a Set monster face-up Defense. A Toon monster (IsToonMonster)
 * is always summoned face up and does not use up the turn's Normal Summon: its statusFlags stay
 * ZONE_STATUS_NORMAL_SUMMONED | ZONE_STATUS_UNK14 and normalSummonUsed is left alone. Chain Energy's cost is
 * paid before the start.
 *
 * The record's card ID (16 bits at bits 31-46) is copied from the hand card in two stores: its bit 0 into
 * bit 7 of byte 3 and bits 1-11 into the low 15 bits of the halfword at +4.
 *
 * Word-valued callers are decoded by the original two halfword shifts.
 * FAKEMATCH: five initialized register bindings and two empty constraints preserve the original
 * packed-record loads and stores. Caller-saved bindings die before calls. The card ID spans byte 3 and
 * the low 15 bits of halfword 4 of the record; that split is staged explicitly to retain the original
 * cached low bit.
 */
void QueueNormalSummon(int player, int handIndex, int zone, int tributes, int faceUp)
{
    u16 tributeBytes = tributes;
    u16 faceUpWord = faceUp;
    /* FAKEMATCH: the record pointer and the card ID mask live in r9 and r8 across the calls below. */
    register struct SummonAction *action asm("r9");
    u32 idHighMask;
    register u32 idMask asm("r8");
    const u16 *idToNumber;
    gSummonAction.player = player;
    gSummonAction.zone = zone;
    gSummonAction.sourceIndex = handIndex;
    if (faceUpWord != 0) {
        gSummonAction.isFaceUp = 1;
        gSummonAction.isDefense = 0;
    }
    else {
        gSummonAction.isFaceUp = 0;
        gSummonAction.isDefense = 1;
    }
    if (CountActiveCardsOnField(0, CARD_LIGHT_OF_INTERVENTION) || CountActiveCardsOnField(1, CARD_LIGHT_OF_INTERVENTION))
        gSummonAction.isFaceUp = 1;
    /* Decode the two tribute bytes (zone | owner << 4 | present << 7). */
    if (tributeBytes != 0) {
        u8 lo = tributeBytes;
        u8 hi = tributeBytes >> 8;
        gSummonAction.tribute1Zone = lo & 7;
        gSummonAction.tribute2Zone = hi & 7;
        gSummonAction.hasTribute1 = lo >> 7;
        gSummonAction.hasTribute2 = hi >> 7;
        gSummonAction.tribute1Player = (lo >> 4) & 1;
        gSummonAction.tribute2Player = (hi >> 4) & 1;
        action = &gSummonAction;
    }
    else {
        gSummonAction.tribute1Zone = 0;
        gSummonAction.tribute2Zone = 0;
        gSummonAction.hasTribute1 = 0;
        gSummonAction.hasTribute2 = 0;
        action = &gSummonAction;
    }
    {
        struct SummonAction *record = action;
        u32 playerOffset;
        u32 byte3;
        {
            u32 side = player;
            u32 handAddress;
            side &= 1;
            handAddress = (u32)handIndex * 4;
            playerOffset = side * PLAYER_STRIDE;
            handAddress += playerOffset;
            handAddress += (u32)gDuelHands;
            handAddress = *(u32 *)handAddress;
            handAddress <<= 20;
            {
                /* Bit 0 of the hand card's ID goes to bit 7 of byte 3 of the record ... */
                u32 low = handAddress >> 20;
                low &= 1;
                low <<= 7;
                byte3 = *((u8 *)record + 3) & 0x7F;
                byte3 |= low;
                *((u8 *)record + 3) = byte3;
            }
            idHighMask = 0x7FFF;
            handAddress >>= 21;
            {
                /* ... and bits 1-11 go to the low 15 bits of the halfword at +4. */
                u32 clear = 0xFFFF8000;
                *(u16 *)((u8 *)record + 4) = (*(u16 *)((u8 *)record + 4) & clear) | handAddress;
            }
        }
        record->kind = SUMMON_ACTION_NORMAL;
        record->statusFlags = ZONE_STATUS_NORMAL_SUMMONED | ZONE_STATUS_UNK14;
        /* FAKEMATCH: the card number is read back from the record inside a statement expression, so the
         * ID halves stay in the registers the ROM uses. */
        if (!IsToonMonster(({
            u32 value;
            byte3 >>= 7;
            value = idHighMask;
            value &= *(u16 *)((u8 *)record + 4);
            value <<= 1;
            value |= byte3;
            idMask = CARD_ID_MASK;
            value &= idMask;
            value <<= 1;
            idToNumber = (const u16 *)0x08622AB4;   /* gCardIdToNumber */
            *(const u16 *)((u32)idToNumber + value);
        }
        ))) {
            /* Not a Toon monster: it counts as the turn's Normal Summon, statusFlags lose UNK14. */
            record->statusFlags = ZONE_STATUS_NORMAL_SUMMONED;
            {
                /* Matching: the player is addressed from gDuelHands (hand[] is at +0x684 of
                 * gDuelPlayers[0]); a gDuelPlayers literal gives other code. */
                u32 base = (u32)gDuelHands - 0x684;
                base = playerOffset + base;
                ((struct DuelPlayer *)base)->normalSummonUsed = 1;
            }
        }
    }
    /* A Toon monster is always summoned face up. */
    /* FAKEMATCH: the same read-back of the card number, with explicit register bindings for the record
     * pointer (r2) and the ID mask (r3). */
    if (IsToonMonster(({
        struct SummonAction *rp = action;
        u32 low = ((u8 *)rp)[3];
        u32 bit = low >> 7;
        register struct SummonAction *hp asm("r2") = action;
        u32 high = ((u16 *)hp)[2];
        u32 value;
        idHighMask &= high;
        value = (idHighMask << 1) | bit;
        {
            register u32 m asm("r3") = idMask;
            asm("" : : "r"(m));
            value &= m;
        }
        idToNumber[value];
    }
    ))) {
        u32 flags = RECORD_BYTE1_FACE_UP;
        /* FAKEMATCH: the byte-1 read is pinned to r6. */
        register u8 *readp asm("r6") = (u8 *)action;
        flags |= readp[1];
        {
            u8 *dst = (u8 *)action;
            /* FAKEMATCH: keep the store address in an ordinary low-register pseudo.
             * A direct r7 binding makes old_agbcc emit a separate ADD.
             * These scratch clobbers are dead here and emit no instructions. */
            asm("" : "+l"(dst) : "r"(flags) : "r1", "r2", "r3", "r4", "r5", "r6");
            dst[1] = flags;
        }
    }
    PayChainEnergyCost(player);
    SummonAction_Start();
}

/*
 * Kind 2: queue a Normal Summon / Set of hand[handIndex] into zone that an effect grants; the position is
 * asked when the step machine runs (ExecuteSummonActionAskPosition), so isDefense starts at 0 and isFaceUp is
 * only set under Light of Intervention. tributes is decoded as in QueueNormalSummon, statusFlags are 3
 * (a granted summon does not set normalSummonUsed). Chain Energy's cost is paid before the start.
 */
void QueueNormalSummonChoosePosition(int player, int handIndex, int zone, u16 tributes)
{
    struct SummonAction *action;
    gSummonAction.player = player;
    gSummonAction.zone = zone;
    gSummonAction.sourceIndex = handIndex;
    gSummonAction.isFaceUp = 0;
    if (CountActiveCardsOnField(0, CARD_LIGHT_OF_INTERVENTION) || CountActiveCardsOnField(1, CARD_LIGHT_OF_INTERVENTION))
        gSummonAction.isFaceUp = 1;
    if (tributes != 0) {
        u8 lo = tributes;
        u8 hi = tributes >> 8;
        action = &gSummonAction;
        action->tribute1Zone = lo & 7;
        action->tribute2Zone = hi & 7;
        action->hasTribute1 = lo >> 7;
        action->hasTribute2 = hi >> 7;
        action->tribute1Player = (lo >> 4) & 1;
        action->tribute2Player = (hi >> 4) & 1;
    } else {
        gSummonAction.tribute1Zone = 0;
        gSummonAction.tribute2Zone = 0;
        gSummonAction.hasTribute1 = 0;
        gSummonAction.hasTribute2 = 0;
        action = &gSummonAction;
    }
    action->cardId = ((*(u32 *)((player & 1) * PLAYER_STRIDE + handIndex * 4 + (u32)gDuelHands)) << 20) >> 20;
    action->kind = SUMMON_ACTION_NORMAL_CHOOSE_POSITION;
    action->statusFlags = ZONE_STATUS_NORMAL_SUMMONED | ZONE_STATUS_UNK14;
    PayChainEnergyCost(player);
    SummonAction_Start();
}
