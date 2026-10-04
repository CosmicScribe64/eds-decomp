#ifndef GUARD_CONSTANTS_DUEL_H
#define GUARD_CONSTANTS_DUEL_H

/*
 * Duel-wide constants: zones and screen areas, turn phases and duel steps, results, zone links and status
 * flags, battle stages, chain and response kinds, card-menu commands, prompt kinds, summon actions, the
 * field-pick masks of the effect cursor, the start field and the duel format.
 *
 * The values are the numbers the matched code uses (collected in the 2026-10 naming pass). Units that
 * compare against literals keep doing so where the literal form decides the generated code; these names
 * are for new code, comments and readable rewrites that compile to the same instructions.
 *
 * Players: 0 is the human player, 1 the CPU (the partner in a link duel). Each player has 11
 * field zones (enum DuelZoneIndex) and five 80-entry piles: hand, deck, graveyard, fusion deck, banished
 * (struct DuelPlayer, include/duel.h).
 */

/* A field location packed in 16 bits: (zone << 8) | player. Zone links, equip targets, the prohibition
 * list and many duel commands pass locations in this form. */
#define DUEL_LOC(player, zone)  (((zone) << 8) | (player))
#define DUEL_LOC_PLAYER(loc)    ((loc) & 0xFF)
#define DUEL_LOC_ZONE(loc)      ((loc) >> 8)

/* Capacity of each per-player pile (hand, deck, graveyard, fusion deck, banished). */
#define DUEL_PILE_CAPACITY      80

/* Field zone of a card: the zone byte of a location (loc >> 8) and the index into DuelPlayer.zones.
 * Effects test ranges: 0-4 monsters, 5-9 spell/trap, 5-10 spell/trap including the field zone. */
enum DuelZoneIndex {
    ZONE_MONSTER_0 = 0,
    ZONE_MONSTER_1 = 1,
    ZONE_MONSTER_2 = 2,
    ZONE_MONSTER_3 = 3,
    ZONE_MONSTER_4 = 4,
    ZONE_SPELL_0 = 5,
    ZONE_SPELL_1 = 6,
    ZONE_SPELL_2 = 7,
    ZONE_SPELL_3 = 8,
    ZONE_SPELL_4 = 9,
    ZONE_FIELD = 10,    /* Field Magic zone */
};

#define MONSTER_ZONE_COUNT      5
#define SPELL_TRAP_ZONE_COUNT   5
#define DUEL_ZONE_COUNT         11

/* Screen area of a card location (DuelLoc.area, gDuelScreen.selArea, the area argument of the field-drawing
 * helpers, index into gDuelZonePositions[player]). Field rows are 0 and 5 (+ zone index within the row). */
enum DuelArea {
    DUEL_AREA_MONSTER = 0,       /* monster row, zones 0-4 */
    DUEL_AREA_SPELL_TRAP = 5,    /* spell/trap row, zones 5-9 */
    DUEL_AREA_FIELD = 10,        /* Field Magic zone */
    DUEL_AREA_HAND = 11,
    DUEL_AREA_FUSION_DECK = 12,
    DUEL_AREA_DECK = 13,
    DUEL_AREA_GRAVEYARD = 14,
    DUEL_AREA_BANISHED = 15,
};

/* Turn phase in gDuel.phase (+0x1B12 bits 2-4), watched in the emulator: Draw, Standby, Main 1, Battle,
 * Main 2, End. Not the same as enum DuelStep, the index of the duel-step handler. */
enum DuelPhase {
    PHASE_DRAW = 0,
    PHASE_STANDBY = 1,
    PHASE_MAIN1 = 2,
    PHASE_BATTLE = 3,
    PHASE_MAIN2 = 4,
    PHASE_END = 5,
    PHASE_NONE = 7,     /* written by the duel restart; meaning unknown */
};

/* gDuelCtrl byte 0: index into the duel-step table run by DuelMainStep. Only values with evidence are
 * listed; 0, 1, 9 and 10 have medium confidence. */
enum DuelStep {
    DUEL_STEP_INIT = 0,
    DUEL_STEP_OPENING = 1,
    DUEL_STEP_TURN_START = 2,
    DUEL_STEP_DRAW_PHASE = 3,
    DUEL_STEP_STANDBY_PHASE = 4,
    DUEL_STEP_MAIN_PHASE = 5,
    DUEL_STEP_END_PHASE = 6,
    DUEL_STEP_TURN_END = 7,
    DUEL_STEP_OPPONENT_TURN = 8,
    DUEL_STEP_RESULT = 9,
    DUEL_STEP_DONE = 10,
};

/* gDuel.result (+0x1B12 bits 6-7), from player 0's point of view. Duel_Setup starts it at DRAW; the
 * duel-over check sets it from life points, deck-out and the special wins (Exodia, Destiny Board). */
enum DuelResult {
    DUEL_RESULT_NONE = 0,
    DUEL_RESULT_WIN = 1,
    DUEL_RESULT_LOSE = 2,
    DUEL_RESULT_DRAW = 3,
};

/* gDuel.battleStage (+0x1B14 bits 9-16): index into gBattleStageHandlers, run by BattlePhase_Run. Order
 * verified with an emulator trace of a CPU attack. */
enum BattleStage {
    BATTLE_STAGE_START = 0,
    BATTLE_STAGE_SELECT_ATTACKER = 1,
    BATTLE_STAGE_SELECT_TARGET = 2,
    BATTLE_STAGE_DECLARE_ATTACK = 3,
    BATTLE_STAGE_PAY_ATTACK_COSTS = 4,
    BATTLE_STAGE_ATTACK_RESPONSE = 5,
    BATTLE_STAGE_REVEAL_DEFENDER = 6,
    BATTLE_STAGE_DAMAGE_CALC = 7,
    BATTLE_STAGE_INFLICT_DAMAGE = 8,
    BATTLE_STAGE_FLIP_EFFECT = 9,
    BATTLE_STAGE_DESTROY = 10,
    BATTLE_STAGE_END_ATTACK = 11,
    BATTLE_STAGE_END = 12,
    BATTLE_STAGE_CLEANUP = 13,
    BATTLE_STAGE_COUNT = 14,
};

/* Low byte of a DuelPlayer.banishedInfo entry (the high byte is a zone). */
enum BanishKind {
    BANISH_NORMAL = 0,
    BANISH_UNTIL_END_PHASE = 1,     /* comes back to its zone, which removedMask keeps free meanwhile */
    BANISH_FACE_DOWN = 2,
};

/* Low byte of DuelZone.linkKinds[i]: how the card in links[i] affects this zone's card. The high byte is
 * a stack count or a value. Kinds 6 and 7 also occur (DestroyLinkedCards destroys kind 6 like kind 5,
 * RemoveLinksToZone removes kind 7) but their source cards are unknown; 7 stays numeric. */
enum ZoneLinkKind {
    ZONE_LINK_EQUIP = 1,                /* equip card at links[i] */
    ZONE_LINK_CONTINUOUS = 2,           /* continuous effect / target of a card at links[i] */
    ZONE_LINK_CARD_EFFECT = 3,          /* links[i] is a card ID whose effect applies (stat modifier) */
    ZONE_LINK_ATK_BONUS = 4,
    ZONE_LINK_ABSORBED = 5,             /* monster absorbed by this one; destroyed with it */
    ZONE_LINK_6 = 6,
    ZONE_LINK_ADD_CARD_STATS = 8,
    ZONE_LINK_STATS_DOWN_500 = 9,
    ZONE_LINK_EQUIP_ATK_200 = 10,       /* repeatable +200 ATK equip: always a new entry */
    ZONE_LINK_ATK_300_PER_VALUE = 11,   /* +300 ATK per unit of the high byte: key 1344's banish effect
                                         * (effect_resolve11); DuelCmd_TurnStart removes the kind-11 link of
                                         * the card's own ID from several monsters every turn (Time Wizard,
                                         * Goddess of Whim, Patrol Robo, ...; meaning there not checked) */
    ZONE_LINK_ATK_DOWN_200 = 12,
    ZONE_LINK_STATS_UP_100 = 13,
};

/* linkKinds[i] value: a kind with its stack count or value in the high byte (key 1523's self link is
 * ZONE_LINK_KIND(ZONE_LINK_ATK_DOWN_200, 1)). Units that build the value with the count first keep their
 * operand order: it can change the generated code. */
#define ZONE_LINK_KIND(kind, count) ((kind) | (count) << 8)

/* Bit mask (arg4) of DUEL_CMD_SET_ZONE_STATUS_FLAGS / DUEL_CMD_CLEAR_ZONE_STATUS_FLAGS and of the summon
 * action record: bits 0-3 map to card-word bits 14-17 (struct DuelCard), bits 4-5 to DuelZone +0x07
 * bits 6-7. Solemn Judgment and Horn of Heaven clear UNK14 | NORMAL_SUMMONED | SPECIAL_SUMMONED (0x7). */
enum ZoneStatusFlag {
    ZONE_STATUS_UNK14 = 0x01,
    ZONE_STATUS_NORMAL_SUMMONED = 0x02,
    ZONE_STATUS_SPECIAL_SUMMONED = 0x04,
    ZONE_STATUS_PLANTED = 0x08,
    ZONE_STATUS_MONSTER_REBORN = 0x10,      /* DuelZone.revivedByMonsterReborn */
    ZONE_STATUS_FROM_GRAVEYARD = 0x20,      /* DuelZone.summonedFromGraveyard */
};

/* Card command menu commands: gDuel.cardMenu.command, the CardMenu_Execute switch, and the bit index in
 * cardMenu.available (enum CardMenuCommandMask). The label tile is 0x264 + 8 * command. */
enum CardMenuCommand {
    CARDMENU_CMD_CARD_VIEW = 0,
    CARDMENU_CMD_DEF_POS = 1,
    CARDMENU_CMD_ATK_POS = 2,
    CARDMENU_CMD_FLIP = 3,
    CARDMENU_CMD_SET = 4,
    CARDMENU_CMD_SUMMON = 5,
    CARDMENU_CMD_ACTIVATE = 6,
    CARDMENU_CMD_ATTACK = 7,
    CARDMENU_CMD_DRAW = 8,
    CARDMENU_CMD_SURRENDER = 9,
    CARDMENU_CMD_FUSION = 10,
    CARDMENU_CMD_SP_SUMMON = 11,
    CARDMENU_CMD_SP_SUMMON_SET = 12,
};

/* u16 command masks (bit i = 1 << CARDMENU_CMD_i) returned by CardMenu_GetAvailableCommands and the
 * per-area builders, stored in gDuel.cardMenu.available. */
enum CardMenuCommandMask {
    CARDMENU_MASK_CARD_VIEW = 0x0001,
    CARDMENU_MASK_DEF_POS = 0x0002,
    CARDMENU_MASK_ATK_POS = 0x0004,
    CARDMENU_MASK_FLIP = 0x0008,
    CARDMENU_MASK_SET = 0x0010,
    CARDMENU_MASK_SUMMON = 0x0020,
    CARDMENU_MASK_ACTIVATE = 0x0040,
    CARDMENU_MASK_ATTACK = 0x0080,
    CARDMENU_MASK_DRAW = 0x0100,
    CARDMENU_MASK_SURRENDER = 0x0200,
    CARDMENU_MASK_FUSION = 0x0400,
    CARDMENU_MASK_SP_SUMMON = 0x0800,
    CARDMENU_MASK_SP_SUMMON_SET = 0x1000,
};

/* Chain entry kind: bits 21-24 of a Chain_AddPending word (ChainEntry.kind), as pushed by
 * CardMenu_Execute. 2 and 3 have medium confidence. */
enum ChainEntryKind {
    CHAIN_KIND_SPELL_TRAP = 1,
    CHAIN_KIND_MONSTER = 2,
    CHAIN_KIND_OFF_FIELD = 3,
};

/* Event that opens a response window: gChain.responseEvent, ChainEntry.event, the second argument of
 * EventResponse_Request and bits 25-30 of Chain_AddPending trigger words. The list is the set of kinds
 * EventResponse_BuildPromptText describes; 1, 4 and 9 are not used. */
enum ResponseEventKind {
    RESPONSE_NONE = 0,
    RESPONSE_OWN_STANDBY = 2,
    RESPONSE_OPPONENT_STANDBY = 3,
    RESPONSE_SUMMONED = 5,
    RESPONSE_FLIP_SUMMONED = 6,
    RESPONSE_SPECIAL_SUMMONED = 7,
    RESPONSE_SET = 8,
    RESPONSE_POSITION_CHANGED = 10,
    RESPONSE_FLIPPED = 11,
    RESPONSE_CONTROL_SWITCHED = 12,
    RESPONSE_BATTLE_DAMAGE = 13,            /* battle damage to the defender's player (BattleStage_InflictDamage
                                             * step 2; the attacking card is the chained card) */
    RESPONSE_BATTLE_DEFLECTED_DAMAGE = 14,  /* battle damage to the attacker's player (step 0; the defending
                                             * card is the chained card) */
    RESPONSE_LP_CHANGE = 15,
    RESPONSE_ATTACK_DECLARED = 16,
    RESPONSE_DAMAGE_STEP = 17,              /* prompt text: 'The target for attack is ...' */
    RESPONSE_BATTLE_FLIP_EFFECT = 18,       /* a flip effect in battle; BattleStage_DestroyMonsters also queues it
                                             * for the battle effects of Dimensional Warrior and Wall of Illusion
                                             * (hypothesis: a general "this monster battled" trigger) */
    RESPONSE_BATTLE_DESTROYED = 19,
    RESPONSE_MAGIC_TO_GRAVE = 20,
    RESPONSE_TRAP_TO_GRAVE = 21,
    RESPONSE_CONTINUOUS_TRAP_PLAYED = 22,
    RESPONSE_CONTINUOUS_MAGIC_PLAYED = 23,
    RESPONSE_FIELD_MAGIC_PLAYED = 24,
    RESPONSE_EQUIP = 25,
    RESPONSE_DREW = 26,
    RESPONSE_MONSTER_TO_HAND = 27,
    RESPONSE_DECK_TO_GRAVE = 28,
    RESPONSE_DISCARDED = 29,
    RESPONSE_MONSTER_TO_GRAVE = 30,
};

/* Kind of a duel prompt (DuelPrompt_Post / DuelPrompt_PostData), stored in gDuel.promptKind. */
enum DuelPromptKind {
    PROMPT_NONE = 0,
    PROMPT_DISCARD = 1,
    PROMPT_DISCARD_COST = 2,
    PROMPT_DISCARD_RANDOM = 3,
    PROMPT_BANISH_RANDOM = 4,
    PROMPT_BANISH_RANDOM_FACE_DOWN = 5,
    PROMPT_PICK_OPPONENT_HAND_CARD = 6,     /* Confiscation, The Forceful Sentry */
    PROMPT_TRIBUTE = 7,
    PROMPT_SELECT_TYPE = 8,
    PROMPT_SELECT_ATTRIBUTE = 9,
    PROMPT_SELECT_TWO_ATTRIBUTES = 10,
    PROMPT_PICK_ONE_OF_TWO_ATTRIBUTES = 11,
    PROMPT_PICK_ONE_OF_FIVE_CARDS = 12,     /* Painful Choice: promptArgs holds 5 card IDs */
    PROMPT_SET_MONSTER_FROM_HAND = 13,
    PROMPT_SELECT_GRAVEYARD_MONSTER = 14,
    PROMPT_CONFIRM_CARD_EFFECT = 15,
    PROMPT_SELECT_OPPONENT_REPLACEMENT_TARGET = 16,
    PROMPT_OFFER_DISCARD_MAGIC = 17,
    PROMPT_CONFIRM_SPECIAL_SUMMON = 18,
    PROMPT_CONFIRM_GRAVEYARD_SUMMON = 19,
    PROMPT_SELECT_OWN_REPLACEMENT_TARGET = 20,
};

/* gSummonAction.kind: the summon/set action that SummonAction_Update runs. */
enum SummonActionKind {
    SUMMON_ACTION_NORMAL = 1,
    SUMMON_ACTION_NORMAL_CHOOSE_POSITION = 2,
    SUMMON_ACTION_FLIP = 3,
    SUMMON_ACTION_SPECIAL = 4,
    SUMMON_ACTION_SPECIAL_CHOOSE_POSITION = 5,
    SUMMON_ACTION_SPECIAL_FROM_HAND = 6,
};

/* u32 mask argument of DuelCursor_PickTarget and DuelCursor_IsValidTarget: which cursor positions an
 * effect may pick. Bits 0-7 are player 0's positions, the same bits << 16 player 1's (PICK_PLAYER1).
 * A monster zone needs one face bit (0x10/0x20) and one position bit (0x40/0x80). Examples from the
 * effects: 0xE0 face-up monster, 0xB0 defense-position monster, 0xD2 face-down card, 0xA trap or set
 * card, 0x6 magic or set card, 0x90 face-down defense-position monster (Acid Trap Hole). */
enum FieldPickMask {
    PICK_HAND = 0x01,
    PICK_FACE_DOWN_SPELL_TRAP = 0x02,
    PICK_FACE_UP_MAGIC = 0x04,
    PICK_FACE_UP_TRAP = 0x08,
    PICK_FACE_DOWN_MONSTER = 0x10,
    PICK_FACE_UP_MONSTER = 0x20,
    PICK_ATTACK_POSITION = 0x40,
    PICK_DEFENSE_POSITION = 0x80,
};

/* Any monster, face up or down, in either position (0xF0); also the mask of the tribute picks. */
#define PICK_ANY_MONSTER        (PICK_FACE_DOWN_MONSTER | PICK_FACE_UP_MONSTER \
                                 | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)
/* Any face-up monster, in either position (0xE0). */
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)
/* Any Magic or Trap card, face up or set (0xE). */
#define PICK_ANY_SPELL_TRAP (PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP)
/* The same positions on player 1's side. */
#define PICK_PLAYER1_SHIFT      16
#define PICK_PLAYER1(mask)      ((mask) << PICK_PLAYER1_SHIFT)
/* The positions of mask on both sides of the field. */
#define PICK_BOTH_SIDES(mask) ((mask) | PICK_PLAYER1(mask))

/* Start field of a duel (gMain.startField, +0x488A bits 0-3): 0 none, else the Field Magic that the duel
 * setup puts into play (SetupStartFieldCard maps the value to the card number). */
enum DuelField {
    FIELD_NONE = 0,
    FIELD_FOREST = 1,
    FIELD_WASTELAND = 2,
    FIELD_MOUNTAIN = 3,
    FIELD_SOGEN = 4,
    FIELD_UMI = 5,
    FIELD_YAMI = 6,
    FIELD_CHORUS_OF_SANCTUARY = 7,
    FIELD_GAIA_POWER = 8,
    FIELD_UMIIRUKA = 9,
    FIELD_MOLTEN_DESTRUCTION = 10,
    FIELD_RISING_AIR_CURRENT = 11,
    FIELD_LUMINOUS_SPARK = 12,
    FIELD_MYSTIC_PLASMA_ZONE = 13,
};

/* Duel format of a link duel (gMain.duelFormat, +0x4888 bits 2-3). */
enum DuelFormat {
    DUEL_FORMAT_SINGLE = 1,
    DUEL_FORMAT_MATCH = 3,
};

#endif /* GUARD_CONSTANTS_DUEL_H */
