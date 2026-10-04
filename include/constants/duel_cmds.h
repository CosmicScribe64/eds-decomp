#ifndef GUARD_CONSTANTS_DUEL_CMDS_H
#define GUARD_CONSTANTS_DUEL_CMDS_H

/*
 * Duel command ids and operand constants.
 *
 * A duel command is four halfwords {cmd, arg2, arg4, arg6} queued with DuelCmd_Push and run one at a time by
 * DuelCmdQueue_Run / DuelCmd_Dispatch (duel_cmd.h). The operands are named by their byte offset in struct
 * DuelCmd. "zone" means a zone index of the acting player (0-4 monsters, 5-9 spell/trap, 10 field) and
 * "loc" a packed location player | zone << 8. "card word" is a struct DuelCard passed as arg2 | arg4 << 16
 * (or arg4 | arg6 << 16 where noted). In a link duel both consoles run every command; DuelLink_MirrorCommand
 * converts the partner's commands to the local point of view.
 *
 * Values verified against the switch in DuelCmd_Dispatch (duel_cmd_queue.c).
 */

/* Command word: bits 0-11 are the id, bit 15 selects the acting player (set = player 1). */
#define DUEL_CMD_ID_MASK 0xFFF
#define DUEL_CMD_PLAYER  0x8000

/* Duel command ids (gDuelCmd.cmd & DUEL_CMD_ID_MASK; first argument of DuelCmd_Push). */
enum DuelCmdId {
    /* Turn, duel screen and banners */
    DUEL_CMD_TURN_START = 0x01,                     /* start-of-turn upkeep: counters, Crush Card, lock timers */
    DUEL_CMD_TURN_END = 0x02,                       /* end-of-turn cleanup: destroy countdowns, one-turn locks */
    DUEL_CMD_SHOW_END_TURN_HAND = 0x03,             /* waving-hand sprite over the acting player's field */
    DUEL_CMD_SHOW_DUEL_RESULT = 0x04,               /* arg2: banner (0 YOU WIN, 1 YOU LOSE, 2 DRAW) + jingle */
    DUEL_CMD_EXODIA_WIN_SCENE = 0x05,               /* Exodia win scene */
    DUEL_CMD_DESTINY_BOARD_WIN_SCENE = 0x06,        /* Destiny Board win scene */
    DUEL_CMD_CHAIN_BANNER = 0x07,                   /* "Chain" banner sliding in from the acting player's side */
    DUEL_CMD_POINT_AT_CARD = 0x08,                  /* arg2: player, arg4: area | index << 8; pulsing hand pointer */
    DUEL_CMD_MOVE_CURSOR = 0x09,                    /* arg2: area, arg4: index; show the field cursor there */
    DUEL_CMD_RESET_DUEL_STATE = 0x10,               /* clear the duel state but keep both decks; LP 8000 */
    DUEL_CMD_SET_FIELD_BACKGROUND = 0x11,           /* arg2: background index */
    DUEL_CMD_OPEN_DUEL_SCREEN = 0x12,               /* set up the duel screen and fade it in */
    DUEL_CMD_CLOSE_DUEL_SCREEN = 0x13,              /* fade out and leave the duel screen */
    DUEL_CMD_START_DUEL_BANNER = 0x14,              /* "Start Duel" banner, then the duel BGM */

    /* Duel-wide negation flags (arg2: new value; all handled by DuelCmd_SetNegationFlag) */
    DUEL_CMD_NEGATE_EQUIP_THIS_TURN = 0x15,
    DUEL_CMD_NEGATE_FIELD_THIS_TURN = 0x16,
    DUEL_CMD_NEGATE_CONT_TRAP_THIS_TURN = 0x17,
    DUEL_CMD_NEGATE_CONT_MAGIC_THIS_TURN = 0x18,
    DUEL_CMD_NEGATE_TRAPS = 0x19,
    DUEL_CMD_NEGATE_MAGIC = 0x1A,
    DUEL_CMD_NEGATE_EQUIP = 0x1B,
    DUEL_CMD_SET_STAT_CHANGES_REVERSED = 0x1C,      /* arg2: flag (cleared at turn end) */
    DUEL_CMD_SET_ATK_DEF_SWAPPED = 0x1D,            /* arg2: flag (cleared at turn end) */

    /* Battle */
    DUEL_CMD_START_BATTLE_SCENE = 0x30,             /* arg2/arg4: card ids of player 0's / player 1's card */
    DUEL_CMD_PLAY_BATTLE_SCENE = 0x31,              /* arg2/arg4: battle values of player 0 / 1, arg6: BattleSideFlags
                                                       per side (side 1 in bits 8-15) */
    DUEL_CMD_PREPARE_BATTLE_PHASE = 0x32,           /* acting player's monsters: "cannot attack next turn" becomes
                                                       "cannot attack", "destroy after battle" is cleared */
    DUEL_CMD_ATTACK = 0x33,                         /* arg2: attacker zone, arg4: defender zone; "Attack" banner */
    DUEL_CMD_DIRECT_ATTACK = 0x34,                  /* arg2: attacker zone; "Direct Attack" banner */
    DUEL_CMD_MARK_ATTACKED = 0x35,                  /* arg2: zone that has attacked */
    DUEL_CMD_SET_BATTLE_PROTECTION = 0x36,          /* arg2/arg4: set player +0x08 bit 0 / bit 1 (damage guards) */
    DUEL_CMD_END_BATTLE_PHASE = 0x37,
    DUEL_CMD_SET_ATTACK_TARGET = 0x38,              /* arg2: target loc, arg4: restart the defender steps */
    DUEL_CMD_SET_ATTACKER = 0x39,                   /* arg2: attacker loc, arg4: restart the battle steps */
    DUEL_CMD_ZERO_ATTACKER_ATK = 0x3A,              /* the attacker's ATK counts as 0 in this battle */
    DUEL_CMD_NEGATE_ATTACK = 0x3B,                  /* arg2: attacker zone */

    /* Life points and player flags */
    DUEL_CMD_SURRENDER = 0x40,                      /* "Surrender" banner, then the acting player's LP = 0 */
    DUEL_CMD_SHOW_JUST_A_MOMENT = 0x41,             /* "Just a moment" banner (link interrupt) */
    DUEL_CMD_GAIN_LP = 0x42,                        /* arg2: amount */
    DUEL_CMD_LOSE_LP = 0x43,                        /* arg2: amount */
    DUEL_CMD_SKIP_NEXT_DRAW_PHASE = 0x44,
    DUEL_CMD_SKIP_NEXT_STANDBY_PHASE = 0x45,
    DUEL_CMD_SKIP_NEXT_TURN = 0x46,
    DUEL_CMD_SET_EXTRA_BATTLE_PHASE = 0x47,         /* arg2: flag */
    DUEL_CMD_SET_POSITION_CHANGE_LOCK = 0x48,       /* arg2: flag (cleared at turn end) */
    DUEL_CMD_SET_SUMMON_LOCKS = 0x49,               /* arg2: no Normal Summon, arg4: no Special Summon */
    DUEL_CMD_SET_MAGIC_TRAP_LOCK_TURNS = 0x4A,      /* arg2: turns */
    DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT = 0x4B,    /* arg2: nonzero = +1, 0 = -1 */
    DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING = 0x4C,  /* arg2: flag (player +0x0C bit 4) */

    /* Phases: banner, then gDuel.phase (DuelCmd_EnterPhase / DuelCmd_EnterBattlePhase) */
    DUEL_CMD_DRAW_PHASE = 0x50,
    DUEL_CMD_STANDBY_PHASE = 0x51,
    DUEL_CMD_MAIN1_PHASE = 0x52,
    DUEL_CMD_BATTLE_PHASE = 0x53,
    DUEL_CMD_MAIN2_PHASE = 0x54,
    DUEL_CMD_END_PHASE = 0x55,

    /* Deck */
    DUEL_CMD_SHUFFLE_DECK = 0x60,                   /* shuffle animation; a link duel sends the new order */
    DUEL_CMD_DRAW_CARDS = 0x61,                     /* arg4: number of cards (arg2 is unused) */
    DUEL_CMD_SEND_TOP_DECK_CARDS_TO_GRAVEYARD = 0x62, /* arg2: number of cards */
    DUEL_CMD_BANISH_TOP_DECK_CARDS = 0x63,          /* arg2: number of cards */
    DUEL_CMD_ADD_DECK_CARD_TO_HAND = 0x64,          /* card word */
    DUEL_CMD_REMOVE_CARD_FROM_DECK = 0x65,          /* card word; no animation */
    DUEL_CMD_SUMMON_FROM_DECK = 0x66,               /* card word, arg6: monster zone */
    DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD = 0x67,    /* card word */
    DUEL_CMD_BANISH_DECK_CARD = 0x68,               /* card word */
    DUEL_CMD_SET_CRUSH_CARD_TURNS = 0x69,           /* arg2: turns */
    DUEL_CMD_ADD_CARD_TO_DECK_TOP = 0x6A,           /* card word (to its owner's deck); no animation */
    DUEL_CMD_ADD_CARD_TO_DECK_BOTTOM = 0x6B,        /* card word; no animation */

    /* Card presentation (arg2: card id) */
    DUEL_CMD_SHOW_CARD_DETAIL = 0x70,               /* full-screen Card Detail page */
    DUEL_CMD_SHOW_CARD_ASSEMBLE = 0x71,             /* card tiles fly together */
    DUEL_CMD_SHOW_CARD_ZOOM_IN = 0x72,              /* card grows from the centre */
    DUEL_CMD_SHOW_CARD_EFFECT = 0x73,               /* zoom-in with a white background flash */
    DUEL_CMD_SHOW_CARD_SCATTER = 0x74,              /* card appears, then its tiles fly apart */
    DUEL_CMD_SHOW_CARD_UNROLL_DOWN = 0x75,          /* card unrolls downwards */
    DUEL_CMD_SHOW_CARD_UNROLL_SIDEWAYS = 0x76,      /* card opens from its centre line */

    /* Field zones */
    DUEL_CMD_PLACE_CARD = 0x77,                     /* arg2: zone | faceUp << 8 | defense << 9,
                                                       arg4 | arg6 << 16: card word; no animation */
    DUEL_CMD_CLEAR_ZONE_CARD = 0x78,                /* arg2: zone */
    DUEL_CMD_SEND_TO_GRAVEYARD = 0x79,              /* arg2: zone, arg4 low byte: play the explosion */
    DUEL_CMD_BANISH = 0x7A,                         /* arg2: zone */
    DUEL_CMD_BANISH_FLAGGED = 0x7B,                 /* arg2: zone; sets card bit 20 first */
    DUEL_CMD_ADD_CARD_TO_GRAVEYARD = 0x7C,          /* card word (to its owner's graveyard) */
    DUEL_CMD_ADD_CARD_TO_BANISHED = 0x7D,           /* card word (to its owner's banished pile) */
    DUEL_CMD_CHANGE_POSITION = 0x7E,                /* arg2: zone, arg4: also turn face up */
    DUEL_CMD_FLIP_CARD = 0x7F,                      /* arg2: zone */
    DUEL_CMD_RETURN_TO_HAND = 0x80,                 /* arg2: zone */
    DUEL_CMD_RETURN_TO_DECK = 0x81,                 /* arg2: zone (to the top of the deck) */
    DUEL_CMD_MOVE_TO_ZONE = 0x82,                   /* arg2: source loc, arg4: destination loc */
    DUEL_CMD_ADD_EQUIP_LINK = 0x83,                 /* arg2: equip card loc, arg4: equipped monster loc */
    DUEL_CMD_SWAP_ZONES = 0x84,                     /* arg2, arg4: the two locs */
    DUEL_CMD_ADD_ZONE_LINK = 0x85,                  /* arg2: target loc, arg4: zone loc, arg6: link kind */
    DUEL_CMD_REMOVE_ZONE_LINK = 0x86,               /* arg2: target loc, arg4: zone loc, arg6: link kind */
    DUEL_CMD_SET_ZONE_DECLARED_VALUE = 0x87,        /* arg2: zone, arg4: value */
    DUEL_CMD_RESET_ZONE_TURN_COUNTER_AND_SET_DECLARED_VALUE = 0x88, /* arg2: zone, arg4: declared value */
    DUEL_CMD_SET_ZONE_TURN_COUNTER = 0x89,          /* arg2: zone, arg4: value */
    DUEL_CMD_ADD_ZONE_TURN_COUNTER = 0x8A,          /* arg2: zone, arg4: amount */
    DUEL_CMD_SET_DESTROYED_BY_OPPONENT_FLAG = 0x8B, /* arg2: zone, arg4: flag (card bit 22) */
    DUEL_CMD_CLEAR_ZONE_LINKS = 0x8C,               /* arg2: zone */
    DUEL_CMD_MOVE_ZONE_LINKS = 0x8D,                /* arg2: source zone, arg4: destination zone */
    DUEL_CMD_ADD_PROHIBITION = 0x8E,                /* arg2: Prohibition's zone, arg4: declared card id */
    DUEL_CMD_REMOVE_PROHIBITION = 0x8F,             /* arg2: Prohibition's zone */
    DUEL_CMD_SET_ZONE_STATUS_FLAGS = 0x90,          /* arg2: zone, arg4: ZoneStatusFlag mask */
    DUEL_CMD_CLEAR_ZONE_STATUS_FLAGS = 0x91,        /* arg2: zone, arg4: ZoneStatusFlag mask (negate animation) */
    DUEL_CMD_SET_EFFECT_UNUSED = 0x92,              /* arg2: zone, arg4: flag (0 = effect used) */
    DUEL_CMD_TRIBUTE_MONSTER = 0x93,                /* arg2: zone, arg4: banish instead of graveyard */
    DUEL_CMD_PLANT_IN_OPPONENT_DECK = 0x94,         /* arg2: zone (Parasite Paracide) */
    DUEL_CMD_SET_DESTROY_COUNTDOWN = 0x95,          /* arg2: zone, arg4: turns (the earlier deadline wins) */
    DUEL_CMD_SET_CANNOT_ATTACK = 0x96,              /* arg2: zone, arg4: flag */
    DUEL_CMD_SET_CANNOT_ATTACK_NEXT_TURN = 0x97,    /* arg2: zone, arg4: flag */
    DUEL_CMD_HALVE_ATTACK = 0x98,                   /* arg2: zone */
    DUEL_CMD_NOP_99 = 0x99,                         /* 0x99-0x9F: handlers that only finish */
    DUEL_CMD_NOP_9A = 0x9A,
    DUEL_CMD_NOP_9B = 0x9B,
    DUEL_CMD_NOP_9C = 0x9C,
    DUEL_CMD_NOP_9D = 0x9D,
    DUEL_CMD_NOP_9E = 0x9E,
    DUEL_CMD_NOP_9F = 0x9F,
    DUEL_CMD_CLEAR_ZONE_LINKS_2 = 0xA0,             /* arg2: zone; same as DUEL_CMD_CLEAR_ZONE_LINKS */
    DUEL_CMD_SET_POSITION_LOCKED = 0xA1,            /* arg2: zone, arg4: flag */
    DUEL_CMD_SET_RETURN_AFTER_BATTLE = 0xA2,        /* arg2: loc (any player), arg4: flag */
    DUEL_CMD_SUMMON_TOKEN = 0xA3,                   /* arg2: zone, arg4: enum TokenKind */
    DUEL_CMD_SET_ZONE_CARD_WORD = 0xA4,             /* arg2: zone, arg4 | arg6 << 16: card word */
    DUEL_CMD_FUSION_MATERIAL_TO_GRAVE = 0xA5,       /* arg2: zone, arg4: banish instead */
    DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG = 0xA6,      /* arg2: zone, arg4: flag (zone +0x08 bit 0) */
    DUEL_CMD_MOVE_MONSTER_FACE_DOWN = 0xA7,         /* arg2: source zone, arg4: destination zone */
    DUEL_CMD_SET_MAGICAL_HATS_CARD = 0xA8,          /* arg2: zone | faceUp << 8, arg4 | arg6 << 16: card word;
                                                     * Magical Hats always sets bit 9 of arg2 too (hypothesis:
                                                     * defense position, as in DUEL_CMD_PLACE_CARD) */
    DUEL_CMD_BANISH_UNTIL_END_PHASE = 0xA9,         /* arg2: zone */
    DUEL_CMD_RETURN_BANISHED_MONSTER = 0xAA,        /* arg2: zone */
    DUEL_CMD_NEGATE_ACTIVATION = 0xB0,              /* arg2: also destroy the negated card */
    DUEL_CMD_SET_SPELL_TRAP_DISABLED = 0xB1,        /* arg2: zone 5-10, arg4: flag */
    DUEL_CMD_NOP_B2 = 0xB2,
    DUEL_CMD_UPDATE_ZONE_LP_PAID = 0xB3,            /* arg2: zone, arg4: 500 adds 500, other values are stored */
    DUEL_CMD_INCREMENT_ZONE_TURN_COUNTER = 0xB4,    /* arg2: zone */

    /* Hand */
    DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD = 0xC0,    /* arg2: hand index, arg4: compact the hand */
    DUEL_CMD_BANISH_HAND_CARD = 0xC1,               /* arg2: hand index, arg4: compact the hand */
    DUEL_CMD_REMOVE_CARD_FROM_HAND = 0xC2,          /* card word; no animation */
    DUEL_CMD_RETURN_HAND_CARD_TO_DECK = 0xC3,       /* arg2: hand index, arg4: top (else bottom) */
    DUEL_CMD_PLACE_MONSTER_FROM_HAND = 0xC4,        /* arg2: card id, arg4: zone | hand index << 4 | faceUp << 8
                                                       | defense << 9 */
    DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND = 0xC5,     /* arg2: card id, arg4: zone | hand index << 4 | faceUp << 8 */
    DUEL_CMD_EXCHANGE_HAND_CARDS = 0xC7,            /* arg2: own hand index, arg4: opponent's hand index */
    DUEL_CMD_COMPACT_HAND = 0xCA,                   /* close the gaps left by the commands above */
    DUEL_CMD_ADD_CARD_TO_HAND = 0xCB,               /* card word */
    DUEL_CMD_SEND_HAND_FUSION_MATERIAL_TO_GRAVEYARD = 0xCC, /* arg2: hand index, arg4: compact the hand */
    DUEL_CMD_BANISH_HAND_FUSION_MATERIAL = 0xCD,    /* arg2: hand index, arg4: compact the hand */
    DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN = 0xCE,     /* arg2: hand index, arg4: compact the hand (Lightforce Sword) */
    DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND = 0xCF,   /* arg2: banished-pile index */

    /* Graveyard, banished pile and fusion deck */
    DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP = 0xD0,    /* arg2: card id */
    DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_BOTTOM = 0xD1, /* arg2: card id */
    DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND = 0xD2,  /* card word */
    DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD = 0xD3,     /* card word; no animation */
    DUEL_CMD_BANISH_GRAVEYARD_CARD = 0xD4,          /* card word */
    DUEL_CMD_TAKE_OPPONENT_GRAVEYARD_CARD = 0xD5,   /* arg2: card id (Graverobber) */
    DUEL_CMD_RETURN_GRAVEYARD_TO_DECK = 0xD6,       /* the whole graveyard */
    DUEL_CMD_ADD_CARD_TO_GRAVEYARD_NO_REDRAW = 0xD7, /* card word */
    DUEL_CMD_CLEAR_PENDING_EQUIP = 0xD8,            /* arg2: graveyard index */
    DUEL_CMD_EQUIP_GRAVEYARD_CARD_TO_OPPONENT = 0xD9, /* arg2: graveyard index */
    DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON = 0xDA,  /* arg2: graveyard index */
    DUEL_CMD_MARK_GRAVEYARD_CARD = 0xDB,            /* card word (sets card bit 23) */
    DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK = 0xDC,   /* card word; no animation */
    DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD = 0xDD, /* card word */
    DUEL_CMD_RETURN_BANISHED_CARD_TO_GRAVEYARD = 0xDE, /* card word */

    /* Coin and dice scenes (results are decided by the pusher) */
    DUEL_CMD_TOSS_COIN = 0xE0,                      /* arg2: called face, arg4: result */
    DUEL_CMD_TOSS_THREE_COINS = 0xE1,               /* arg2: results in bits 0-2 */
    DUEL_CMD_ROLL_GRACEFUL_DICE = 0xE2,             /* arg2: roll */
    DUEL_CMD_ROLL_SKULL_DICE = 0xE3,                /* arg2: roll */
    DUEL_CMD_ROLL_PLAIN_DIE = 0xE4,                 /* arg2: roll */
    DUEL_CMD_ROLL_SKULL_DICE_ALT = 0xE5             /* arg2: roll; same handler as DUEL_CMD_ROLL_SKULL_DICE */
};

/*
 * Token kind: arg4 of DUEL_CMD_SUMMON_TOKEN. The token's card number is 1920 + kind; kinds 1 and 2 are placed
 * in defense position. Only kind 0 is identified (low confidence); the others are tokens of non-EDS cards.
 */
enum TokenKind {
    TOKEN_KIND_INSECT_MONSTER = 0,
    TOKEN_KIND_1 = 1,
    TOKEN_KIND_2 = 2,
    TOKEN_KIND_3 = 3
};

#endif /* GUARD_CONSTANTS_DUEL_CMDS_H */
