/*
 * duel_response (0x08041F9C-0x080431E3): event response windows and the link partner's activation query
 * (wiki/functions/duel-response-c.md).
 *
 * After a game event (summon, attack, damage, a card destroyed, ...) EventResponse_Request opens a response
 * window in gChain (responseEvent, responseEventArg, askPlayer). DuelMainStep calls EventResponse_Update every
 * frame, which runs the step machine EventResponse_Run (enum EventResponseStep): it asks each player in turn
 * whether to activate a Quick-Play Magic or a Trap ('You Summoned a monster. Do you wish to activate ...?').
 *   - The human gets a Yes/No box and picks the card with the field cursor and the card menu.
 *   - The CPU searches its set Magic/Trap zones (AiShouldActivateSetCard).
 *   - In a link duel the question goes to the partner (LINKMSG_ACTIVATE_QUERY) and the partner's
 *     DuelLink_AnswerActivateQuery answers it.
 * A chosen card is queued with Chain_AddPending (and flipped face-up first); CanActivateHandCard (here) and
 * CanActivateFieldCard (effect_targets4.c) decide which cards may answer. DuelLink_AnswerActivateQuery is
 * also the partner's side of Chain_AskResponse: it answers a chain-link query (LINKMSG_CHAIN_QUERY) the same
 * way, runs the chosen card's chainA / chainB handlers here and sends the card back.
 *
 * Players: 0 is the human, 1 the CPU or the link partner. A response is a card of spell speed 2 or more.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_*, gCardNames */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum ResponseEventKind, DuelArea, CardMenuCommandMask, PHASE_*, FieldPickMask */
#include "constants/duel_cmds.h"    /* DUEL_CMD_FLIP_CARD, DUEL_CMD_CHAIN_BANNER, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CONFIRM, SE_ERROR */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* struct Main gMain, newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_link.h, card_list_view.h and duel_screen.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with the include lines of duel.h and sound.h, in that order
 * (build/readability/issues/duel_response.md). */
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
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 unk3[0x7 - 0x3];
    u8 unk7_0:6;
    u8 magicTrapLockTurns:2;        /* +0x007 bits 6-7: nonzero blocks Magic/Trap activation */
    u8 unk8[0x28 - 0x8];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

/* The card command menu (gDuel.cardMenu, 0xC bytes): the human's card pick in a response window. */
struct CardMenu {
    u16 open:1;                     /* bit 0: menu open, the caller runs CardMenu_Update */
    u16 confirmed:1;                /* bit 1: a command was chosen */
    u16 command:4;                  /* bits 2-5: enum CardMenuCommand */
    u16 slide:4;                    /* bits 6-9: slide/zoom animation step 0-8 */
    u32 available:16;               /* bits 10-25: enum CardMenuCommandMask bits */
    u32 state:8;                    /* bits 26-33: CardMenu_Update state */
    u32 step:8;                     /* bits 34-41: step of the command handler */
    u8 summonSeq:4;                 /* bits 42-45 */
    u32 tributeSources:4;           /* bits 46-49 */
    u16 timer:7;                    /* bits 50-56: pulse timer of the selected icon */
    u16 player:1;                   /* bit 57: player of the confirmed command */
    u32 area:7;                     /* bits 58-64: enum DuelArea of the cursor at confirm */
    u32 index:8;                    /* bits 65-72: zone index (field) or hand index (hand) */
    u32 placeZone:8;                /* bits 73-80 */
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
    u8 result:2;                    /* +0x1B12 bits 6-7 */
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

/* Effective stats of the card in a zone (GetZoneCardStats). */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4 */
    s32 def;                        /* +0x8 */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

int CanPlaceSpellTrapCard(int player, u16 cardId);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* AiShouldActivateSetCard */
#include "card_list_view.h"         /* CardListView_Open */
#include "card_menu.h"              /* CardMenu_Update, CardMenu_PlaySpellTrapFromHand */
#include "chain.h"                  /* struct ChainEntry, gChain, the functions defined here */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_link.h"              /* gLinkState, DuelLink_SendMessage / SendMessageData, LINKMSG_* */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget / GetCardId */
#include "effect.h"                 /* GetCardSpellSpeed, CanActivateEffect, FindCardEffect, gCardEffects */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* StrCopy, StrCat, FormatStr, FormatInt */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: this unit calls DuelCmd_Push through u16 parameters (the operands are narrowed at the call). */
extern void DuelCmd_PushU16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
/* Matching: the CPU search passes a second argument; the definition (ai.h) takes only the entry. */
extern int AiShouldActivateSetCard2(struct ChainEntry *entry, int unused) asm("AiShouldActivateSetCard");

/*
 * Matching: the card menu and the cards around it in gDuel as the two step machines read them. Same layout
 * as struct DuelState up to +0x1B38, but `index` is a u16 bitfield (struct CardMenu has it in a u32
 * container): the ROM recomputes `index + area` from the shifted halfword. Only the alias symbols below use it.
 */
struct CardMenuView {
    u16 open:1;                     /* bit 0 */
    u16 confirmed:1;                /* bit 1 */
    u16 command:4;                  /* bits 2-5 */
    u16 slide:4;                    /* bits 6-9 */
    u32 available:16;               /* bits 10-25 */
    u32 state:8;                    /* bits 26-33 */
    u32 step:8;                     /* bits 34-41 */
    u32 unk42:8;                    /* bits 42-49: summonSeq, tributeSources */
    u16 timer:7;                    /* bits 50-56 */
    u16 player:1;                   /* bit 57 */
    u32 area:7;                     /* bits 58-64 */
    u16 index:8;                    /* bits 65-72 */
    u16 unk73:7;                    /* bits 73-79: placeZone */
};

struct DuelStateMenuView {
    u8 unk0[0x1B12];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5 */
    u8 result:2;                    /* +0x1B12 bits 6-7 */
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardMenuCard;               /* +0x1B28 */
    u8 unk1B2A[2];
    struct CardMenuView cardMenu;   /* +0x1B2C */
};

/*
 * Alias symbols: second names for gChain, gDuel, gTextBox, gMain, gDuelScreen, gLinkState and gCardEffects
 * (the address is the suffix). The ROM loads each of these from its own literal-pool entry in the two step
 * machines, so the matched code keeps one set of aliases per function.
 */
/* EventResponse_Run */
extern struct ChainState gAliasB_02017A40;      /* = gChain */
extern struct DuelStateMenuView gAliasB_020192E0;   /* = gDuel */
extern struct TextBox gAliasB_0201AE60;         /* = gTextBox */
extern struct Main gAliasB_03000040;            /* = gMain */
extern struct DuelScreen gAliasB_0201CFB0;      /* = gDuelScreen */
extern struct LinkState gAliasB_02017FB0;       /* = gLinkState */
#define gRunChain       gAliasB_02017A40
#define gRunDuel        gAliasB_020192E0
#define gRunTextBox     gAliasB_0201AE60
#define gRunMain        gAliasB_03000040
#define gRunScreen      gAliasB_0201CFB0
#define gRunLinkState   gAliasB_02017FB0
/* DuelLink_AnswerActivateQuery */
extern struct ChainState gAlias_02017A40;       /* = gChain */
extern struct DuelStateMenuView gAlias_020192E0;    /* = gDuel */
extern struct TextBox gAlias_0201AE60;          /* = gTextBox */
extern struct Main gAlias_03000040;             /* = gMain */
extern struct DuelScreen gAlias_0201CFB0;       /* = gDuelScreen */
extern const struct CardEffect gAlias_0819A9D4[];   /* = gCardEffects */
#define gAnswerChain    gAlias_02017A40
#define gAnswerDuel     gAlias_020192E0
#define gAnswerTextBox  gAlias_0201AE60
#define gAnswerMain     gAlias_03000040
#define gAnswerScreen   gAlias_0201CFB0
#define gAnswerEffects  gAlias_0819A9D4

/* Prompts of the response window (ROM; only this unit uses them). Event texts, by ResponseEventKind;
 * the @n colour codes of the strings are left out of the comments. */
extern const char gStrEventYouSummoned[];               /* 0x08084D6C 'You Summoned a monster.' */
extern const char gStrEventOpponentSummoned[];          /* 0x08084D88 */
extern const char gStrEventYouFlipSummoned[];           /* 0x08084DB0 */
extern const char gStrEventOpponentFlipSummoned[];      /* 0x08084DD4 */
extern const char gStrEventYouSpecialSummoned[];        /* 0x08084E00 */
extern const char gStrEventOpponentSpecialSummoned[];   /* 0x08084E24 */
extern const char gStrEventYouSet[];                    /* 0x08084E54 'You Set a monster.' */
extern const char gStrEventOpponentSet[];               /* 0x08084E6C */
extern const char gStrEventAttackTargetFmt[];           /* 0x08084E90 'The target for attack is ...' */
extern const char gStrEventPositionChanged[];           /* 0x08084ECC */
extern const char gStrEventFlippedFaceUp[];             /* 0x08084EF4 */
extern const char gStrEventControlSwitched[];           /* 0x08084F18 */
extern const char gStrEventBattleFlipEffect[];          /* 0x08084F44 */
extern const char gStrEventYouDeclaredBattle[];         /* 0x08084F78 'You have declared battle.' */
extern const char gStrEventOpponentDeclaredBattle[];    /* 0x08084F94 */
extern const char gStrEventBattleDestroyed[];           /* 0x08084FB8 */
extern const char gStrEventYouTookBattleDamage[];       /* 0x08084FE4 */
extern const char gStrEventYouDealtBattleDamage[];      /* 0x08085014 */
extern const char gStrEventYouTookDeflectedDamage[];    /* 0x08085058 */
extern const char gStrEventOpponentTookDeflectedDamage[]; /* 0x08085094 */
extern const char gStrEventYouTookDamage[];             /* 0x080850D8 'You have received damage.' */
extern const char gStrEventYouDealtDamage[];            /* 0x080850F4 */
extern const char gStrEventMagicDestroyed[];            /* 0x08085114 */
extern const char gStrEventTrapDestroyed[];             /* 0x0808512C */
extern const char gStrEventContinuousTrapPlayed[];      /* 0x08085144 */
extern const char gStrEventContinuousMagicPlayed[];     /* 0x0808516C */
extern const char gStrEventFieldMagicPlayed[];          /* 0x08085198 */
extern const char gStrEventEquipped[];                  /* 0x080851B8 */
extern const char gStrEventCardDrawn[];                 /* 0x080851E8 'A player drew a card.' */
extern const char gStrEventMonsterReturnedToHand[];     /* 0x08085200 */
extern const char gStrEventDeckToGraveyard[];           /* 0x08085238 */
extern const char gStrEventYouDiscarded[];              /* 0x08085278 */
extern const char gStrEventOpponentDiscarded[];         /* 0x0808528C */
extern const char gStrEventMonsterSentToGraveyard[];    /* 0x080852AC */
extern const char gStrEventSeparator[];                 /* 0x080852E4 ' ' */
extern const char gStrAskActivateQuickPlayOrTrap[];     /* 0x080852E8 'Do you wish to activate a Quick-play ...?' */
/* FAKEMATCH: gStrEventCardDrawn under a second name. Event 26 shows the same text for both players; the
 * second name keeps the compiler from merging the two identical switch arms. */
extern const char gAlias_080851E8[];                    /* = gStrEventCardDrawn */
/* Prompts of the link partner's answer. */
extern const char gStrLinkChainPromptEffect[];          /* 0x08085330 '%s's effect is activated. Resolve it ...' */
extern const char gStrLinkChainPromptCard[];            /* 0x08085374 '%s is activated. Resolve it ...' */
extern const u8 gStrSelectSpellTrapForChain[];          /* 0x080853AC 'Please select a Magic or Trap ... Chain.' */
extern const u8 gStrSelectSpellTrapToActivate[];        /* 0x080853F8 'Please select a Magic or Trap ... activated.' */

/* Type of gChain.chainA / chainB (struct ChainState). */
typedef u16 (*ChainHandler)(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* The flag byte of the response window in gChain (+0x491): aiZone, askPlayer, firstAskPlayer, requestPending
 * and responseAdded, reached through a pointer into gChain. Matching: EventResponse_Request forms the pointer
 * before it stores the flags. */
#define RESPONSE_FLAGS_OFFSET 0x491
struct ResponseFlagsView {
    u8 aiZone:4;
    u8 askPlayer:1;
    u8 firstAskPlayer:1;
    u8 requestPending:1;
    u8 responseAdded:1;
};

/* Text boxes: the question of a response window (cell (4, 2), 22 x 9) and the card prompt (cell (6, 2), 18 x 7). */
#define QUESTION_BOX_POS 0x204
#define QUESTION_BOX_SIZE 0x916
#define PROMPT_BOX_POS 0x206
#define PROMPT_BOX_SIZE 0x712

/* Pick masks (enum FieldPickMask): any Magic or Trap card, face up or set (0xE); a face-up monster in either
 * position (0xE0). The masks here name player 0's positions only. */
#define PICK_ANY_SPELL_TRAP (PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP)
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)

/* &gDuelZones[player].zones[zone] as the ROM adds it: the player term first, from the gDuel alias symbol
 * (gDuelZones = gDuel + 0x2C). Matching: the zone base must use the same symbol as the card-menu struct, so
 * that CSE turns it into base + 0x2C. player must be 0 or 1. */
#define DUEL_ZONES_OFFSET   (OFFSET_OF(struct DuelState, players) + OFFSET_OF(struct DuelPlayer, zones))   /* 0x2C */
#define ALIAS_ZONE_AT(duel, player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelZonesPlayer) + (zone) * sizeof(struct DuelZone) \
                         + (u32)((u8 *)&(duel) + DUEL_ZONES_OFFSET)))
#define RUN_ZONE(player, zone)      ALIAS_ZONE_AT(gRunDuel, player, zone)
#define ANSWER_ZONE(player, zone)   ALIAS_ZONE_AT(gAnswerDuel, player, zone)
/* &gDuelZones[player].zones[zone] by byte arithmetic; player must be 0 or 1. Matching: the two forms add the
 * zone and player terms in the orders the ROM uses (ZONE_AT: zone term first, ZONE_AT_PZ: player term first;
 * array indexing gives yet another order). */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelZonesPlayer) \
                         + (u32)gDuelZones))
#define ZONE_AT_PZ(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelZonesPlayer) + (zone) * sizeof(struct DuelZone) \
                         + (u32)gDuelZones))

/* The card word of a zone or hand entry as one u32 (ldr) and its card ID: 12 bits (lsl #20; lsr #20).
 * Matching: a bitfield read of .id loads a halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)

/* Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardNames (0x0822C720).
 * Matching: the symbol forms (card_data.h) load other literals. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))       /* enum CardType */
#define CARD_NAME(id) (((const char (*)[CARD_NAME_SIZE])0x0822C720)[id])

/* Hand card of a player as the ROM adds it: (gDuelHands + index * 4) + player * 0xD64. */
#define HAND_CARD_WORD(player, index) \
    (*(u32 *)((u8 *)gDuelHands + (index) * sizeof(struct DuelCard) + (1 & (player)) * sizeof(struct DuelPlayer)))

/* Turn flags byte of gDuel (+0x1B12): bit 1 turnPlayer, bits 2-4 phase (enum DuelPhase). CanActivateHandCard
 * reads it through gDuelHands + 0x148A (the same address, 0x0201ADF2). The shift extractions are matched forms. */
#define TURN_FLAGS_OFFSET 0x1B12
#define TURN_FLAGS_VIA_HANDS \
    (((u8 *)gDuelHands)[TURN_FLAGS_OFFSET - OFFSET_OF(struct DuelState, players) - OFFSET_OF(struct DuelPlayer, hand)])
#define TURN_FLAGS_PLAYER(flags) (((flags) << 30) >> 31)
#define TURN_FLAGS_PHASE(flags) (((flags) << 27) >> 29)

/*
 * Can hand card handIdx of player (a Magic card) be activated now, outside a chain? Needs
 * CanPlaceSpellTrapCard, type Magic, and then spell speed 2 or more, or the Main Phase 1/2 of the turn player.
 * Fills ref->card; refuses a Magic or Trap card while the player's magicTrapLockTurns is nonzero; returns
 * CanActivateEffect(ref, NULL, 1) narrowed to u16.
 */
int CanActivateHandCard(struct ChainEntry *ref, int player, int handIdx)
{
    u16 id = CARD_ID(HAND_CARD_WORD(player, handIdx));
    u32 n = CARD_ID_MASK & id;
    if (CanPlaceSpellTrapCard(player, id) != 0
        && CARD_STATS_TYPE(((const u32 *)0x08621DE0)[n]) == CARD_TYPE_MAGIC) {
        if (GetCardSpellSpeed(id) > SPELL_SPEED_1)
            goto activate;
        {
            u32 flags = TURN_FLAGS_VIA_HANDS;
            u32 phase = TURN_FLAGS_PHASE(flags);
            if (phase == PHASE_MAIN1 || phase == PHASE_MAIN2) {
                if (TURN_FLAGS_PLAYER(flags) == player)
                    goto activate;
            }
        }
    }
refuse:
    return 0;
activate:
    {
        int type;
        ref->card = id;
        type = CARD_TYPE(id);
        switch (type) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
            if (gDuelPlayers[1 & player].magicTrapLockTurns != 0)
                goto refuse;
        }
        return (u16)CanActivateEffect(ref, 0, 1);
    }
}

/*
 * Card-menu command mask for the card under the cursor in a response window: area DUEL_AREA_HAND (11) asks
 * CanActivateHandCard(ref, player, index), area DUEL_AREA_SPELL_TRAP (5) asks CanActivateFieldCard(ref,
 * player, zone index + 5). 0x41 (Card View + Activate) when it allows activation, else 1 (Card View only).
 */
int EventResponse_GetCommands(struct ChainEntry *ref, int player, int area, int index)
{
    int mask = CARDMENU_MASK_CARD_VIEW;
    switch (area) {
    case DUEL_AREA_HAND:
        if ((u16)CanActivateHandCard(ref, player, index))
            mask = CARDMENU_MASK_CARD_VIEW | CARDMENU_MASK_ACTIVATE;
        break;
    case DUEL_AREA_SPELL_TRAP:
        if ((u16)CanActivateFieldCard(ref, player, index + ZONE_SPELL_0))
            mask = CARDMENU_MASK_CARD_VIEW | CARDMENU_MASK_ACTIVATE;
        break;
    }
    return mask;
}

/* Matching: the player array at gDuel + 4, wrapped so the hand-loop test loads the constant base first (an
 * ARRAY_REF, not pointer arithmetic). */
struct DuelPlayersView {
    struct DuelPlayer p[2];
};

/*
 * Does the player hold a card that can answer the open event? 1 when a Magic/Trap zone 5-9 card has spell
 * speed 2 or more and CanActivateFieldCard accepts it; else, only when player is the turn player, when a hand
 * card has spell speed 2 or more and CanActivateHandCard accepts it. Uses gEventResponseEntry as the scratch
 * ref the tests fill.
 */
int EventResponse_CanPlayerRespond(int player)
{
    int i;
    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        u32 id = CARD_ID(CARD_WORD(ZONE_AT_PZ(1 & player, i)->card));
        if (id != 0) {
            int speed = GetCardSpellSpeed(id);
            int canActivate = (u16)CanActivateFieldCard(&gEventResponseEntry, player, i);
            if (speed > SPELL_SPEED_1 && canActivate != 0)
                goto found;
        }
    }
    if (gDuel.turnPlayer == player)
        goto check_hand;
    return 0;
found:
    return 1;
check_hand:
    for (i = 0; i < ((struct DuelPlayersView *)((u8 *)&gDuel + 4))->p[1 & player].handCount; i++) {
        u32 id = CARD_ID(HAND_CARD_WORD(player, i));
        if (id != 0) {
            int speed = GetCardSpellSpeed(id);
            int canActivate = (u16)CanActivateHandCard(&gEventResponseEntry, player, i);
            if (speed > SPELL_SPEED_1 && canActivate != 0)
                goto found;
        }
    }
    return 0;
}

/*
 * Write the description of the event in `ev` into buf: the text of event ev->event ('You Summoned a monster.',
 * 'Your opponent has declared battle.', ...) chosen by the acting player (the low byte of loc0), by the
 * selector in loc1 (events 13-15, 26, 29), or, for RESPONSE_DAMAGE_STEP, 'The target for attack is '%s'
 * (ATK:%d/DEF:%d)' for the face-up monster at loc1 (player | zone << 8). Then append a space and 'Do you wish
 * to activate a Quick-play Magic or Trap card?'. An event with no text (or a selector that is not 0 or 1)
 * leaves buf holding only the question.
 */
void EventResponse_BuildPromptText(struct ChainEntry *ev, u8 *buf)
{
    struct ZoneCardStats stats;
    char tmp[0x100];
    const char *text;
    switch (ev->event) {
    case RESPONSE_SUMMONED:
        switch ((u8)ev->loc0) {
        case 0:
            text = gStrEventYouSummoned;
            break;
        case 1:
            text = gStrEventOpponentSummoned;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_FLIP_SUMMONED:
        switch ((u8)ev->loc0) {
        case 0:
            text = gStrEventYouFlipSummoned;
            break;
        case 1:
            text = gStrEventOpponentFlipSummoned;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_SPECIAL_SUMMONED:
        switch ((u8)ev->loc0) {
        case 0:
            text = gStrEventYouSpecialSummoned;
            break;
        case 1:
            text = gStrEventOpponentSpecialSummoned;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_SET:
        switch ((u8)ev->loc0) {
        case 0:
            text = gStrEventYouSet;
            break;
        case 1:
            text = gStrEventOpponentSet;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_DAMAGE_STEP: {
        u8 player;
        u16 target;
        int zone;
        int side;
        struct DuelZone *z;
        u32 id;
        *(u16 *)buf = 0;
        player = (u8)ev->loc1;
        target = ev->loc1;
        zone = target >> 8;
        side = 1 & player;
        z = ZONE_AT(side, zone);
        id = CARD_ID(CARD_WORD(z->card));
        if (id != 0 && z->isFaceUp) {
            GetZoneCardStats(player, zone, &stats);
            FormatStr((char *)buf, gStrEventAttackTargetFmt, (const char *)gCardNames + id * CARD_NAME_SIZE);
            FormatInt(tmp, (char *)buf, stats.atk);
            FormatInt((char *)buf, tmp, stats.def);
        }
        goto append_question;
    }
    case RESPONSE_POSITION_CHANGED:
        text = gStrEventPositionChanged;
        goto copy_text;
    case RESPONSE_FLIPPED:
        text = gStrEventFlippedFaceUp;
        goto copy_text;
    case RESPONSE_CONTROL_SWITCHED:
        text = gStrEventControlSwitched;
        goto copy_text;
    case RESPONSE_BATTLE_FLIP_EFFECT:
        text = gStrEventBattleFlipEffect;
        goto copy_text;
    case RESPONSE_ATTACK_DECLARED:
        switch ((u8)ev->loc0) {
        case 0:
            text = gStrEventYouDeclaredBattle;
            break;
        case 1:
            text = gStrEventOpponentDeclaredBattle;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_BATTLE_DESTROYED:
        text = gStrEventBattleDestroyed;
        goto copy_text;
    case RESPONSE_BATTLE_DAMAGE:
        switch (((u8)ev->loc1 & 0xF)) {
        case 0:
            text = gStrEventYouTookBattleDamage;
            break;
        case 1:
            text = gStrEventYouDealtBattleDamage;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_BATTLE_DEFLECTED_DAMAGE:
        switch (((u8)ev->loc1 & 0xF)) {
        case 0:
            text = gStrEventYouTookDeflectedDamage;
            break;
        case 1:
            text = gStrEventOpponentTookDeflectedDamage;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_LP_CHANGE:
        switch (ev->loc1) {
        case 0:
            text = gStrEventYouTookDamage;
            break;
        case 1:
            text = gStrEventYouDealtDamage;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_MAGIC_TO_GRAVE:
        text = gStrEventMagicDestroyed;
        goto copy_text;
    case RESPONSE_TRAP_TO_GRAVE:
        text = gStrEventTrapDestroyed;
        goto copy_text;
    case RESPONSE_CONTINUOUS_TRAP_PLAYED:
        text = gStrEventContinuousTrapPlayed;
        goto copy_text;
    case RESPONSE_CONTINUOUS_MAGIC_PLAYED:
        text = gStrEventContinuousMagicPlayed;
        goto copy_text;
    case RESPONSE_FIELD_MAGIC_PLAYED:
        text = gStrEventFieldMagicPlayed;
        goto copy_text;
    case RESPONSE_EQUIP:
        text = gStrEventEquipped;
        goto copy_text;
    case RESPONSE_DREW:
        switch (ev->loc1) {
        case 0:
            text = gStrEventCardDrawn;
            break;
        case 1:
            text = gAlias_080851E8;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_MONSTER_TO_HAND:
        text = gStrEventMonsterReturnedToHand;
        goto copy_text;
    case RESPONSE_DECK_TO_GRAVE:
        text = gStrEventDeckToGraveyard;
        goto copy_text;
    case RESPONSE_DISCARDED:
        switch (ev->loc1) {
        case 0:
            text = gStrEventYouDiscarded;
            break;
        case 1:
            text = gStrEventOpponentDiscarded;
            break;
        default:
            goto append_question;
        }
        goto copy_text;
    case RESPONSE_MONSTER_TO_GRAVE:
        goto monster_to_grave;
    }
    goto append_question;
copy_text:
    StrCopy((char *)buf, text);
    goto append_question;
monster_to_grave:
    StrCopy((char *)buf, gStrEventMonsterSentToGraveyard);
append_question:
    StrCat((char *)buf, gStrEventSeparator);
    StrCat((char *)buf, gStrAskActivateQuickPlayOrTrap);
}

/*
 * One step of the open response window, on gChain.askStep (enum EventResponseStep; the cases are decimal in
 * the ROM). Returns 1 when the window is finished (responseAdded tells whether a card was queued), else 0.
 *   EVRESP_START (0): fill gEventResponseEntry from responseEvent / responseEventArg for askPlayer; if
 *     nobody of that player can answer (EventResponse_CanPlayerRespond), skip to EVRESP_NEXT_PLAYER.
 *   EVRESP_ASK (1): player 1 goes to the link query (link duel) or the CPU search; player 0 gets a Yes/No
 *     box with the event text (EventResponse_BuildPromptText).
 *   EVRESP_WAIT_ANSWER (2): No -> EVRESP_NEXT_PLAYER; Yes -> close the card menu and pick a card.
 *   EVRESP_PICK_CARD (10): the field cursor (spell/trap cards, and the hand on the human's own turn) and the
 *     card menu with EventResponse_GetCommands; B goes back to EVRESP_ASK. Cards of the piles open the card
 *     list viewer.
 *   EVRESP_ACTIVATE_PICKED (11): a hand card is played through the card menu (CardMenu_PlaySpellTrapFromHand);
 *     a Magic/Trap zone card is flipped face-up and queued with Chain_AddPending. Finished with a response.
 *   EVRESP_LINK_QUERY / EVRESP_LINK_WAIT (100, 101): send LINKMSG_ACTIVATE_QUERY and wait for the partner's
 *     reply (gLinkState.queryReplyReceived): a card came back (responseAdded) or none did.
 *   EVRESP_CPU_SEARCH / EVRESP_CPU_ACTIVATE (200, 201): the CPU looks at its zones 5-9 for a card of spell
 *     speed 2 or more that CanActivateFieldCard and AiShouldActivateSetCard accept (it remembers the zone in
 *     aiZone), then flips and queues it.
 *   EVRESP_NEXT_PLAYER (240): switch askPlayer; back to EVRESP_START for the other player, finished when it is
 *     back at firstAskPlayer.
 */
int EventResponse_Run(void)
{
    u8 text[0x100];
    int slot;
    int step = gRunChain.askStep;

    switch ((u8)step) {
    case EVRESP_START:
        gRunChain.responseAdded = 0;
        gRunChain.responseEntry.card = 0;
        gRunChain.responseEntry.player = gRunChain.askPlayer;
        gRunChain.responseEntry.event = gRunChain.responseEvent;
        gRunChain.responseEntry.loc0 = gRunChain.responseEventArg;
        gRunChain.responseEntry.loc1 = gRunChain.responseEventArg >> 16;
        if ((u16)EventResponse_CanPlayerRespond(gRunChain.askPlayer) == 0) {
            gRunChain.askStep = EVRESP_NEXT_PLAYER;
            return 0;
        }
        gRunChain.askStep++;
        /* falls through to EVRESP_ASK */
    case EVRESP_ASK:
        if (gRunChain.askPlayer) {
            u8 next;
            if (gDuelCtrl.isLinkDuel)
                next = EVRESP_LINK_QUERY;
            else
                next = EVRESP_CPU_SEARCH;
            gRunChain.askStep = next;
        } else {
            EventResponse_BuildPromptText(&gRunChain.responseEntry, text);
            TextBoxOpen(QUESTION_BOX_POS, QUESTION_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            gRunChain.askStep++;
        }
        return 0;
    case EVRESP_WAIT_ANSWER:
        if (gRunTextBox.result == 0) {
            gRunChain.askStep = EVRESP_NEXT_PLAYER;
            return 0;
        }
        gRunChain.askStep = EVRESP_PICK_CARD;
        gRunDuel.cardMenu.open = 0;
        gRunDuel.cardMenu.confirmed = 0;
        return 0;
    case EVRESP_PICK_CARD:
        if (gRunDuel.cardMenu.open) {
            CardMenu_Update();
            return 0;
        }
        if (gRunDuel.cardMenu.confirmed) {
            gRunChain.askStep++;
            return 0;
        }
        if (gRunMain.newKeys & B_BUTTON) {
            gRunChain.askStep = EVRESP_ASK;
            return 0;
        }
        {
            /* The human's own Magic/Trap cards; the hand too on the human's own turn (turnPlayer 0). */
            u32 mask = PICK_ANY_SPELL_TRAP;
            if (!(gRunDuel.turnPlayer))
                mask = PICK_HAND | PICK_ANY_SPELL_TRAP;
            if (DuelCursor_PickTarget(mask) == 0)
                return 0;
        }
        {
            u32 player = gRunScreen.selPlayer;
            u32 area = gRunScreen.selArea;
            u32 index = gRunScreen.selIndex;
            u16 card = DuelCursor_GetCardId();
            switch (area) {
            case DUEL_AREA_MONSTER:
            case DUEL_AREA_SPELL_TRAP:
            case DUEL_AREA_FIELD:
            case DUEL_AREA_HAND:
                if (card != 0) {
                    gRunDuel.cardMenu.open = 1;
                    gRunDuel.cardMenu.state = 0;
                    gRunDuel.cardMenu.available =
                        (u16)EventResponse_GetCommands(&gEventResponseEntry, player, area, index);
                    return 0;
                }
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_DECK:
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
            case DUEL_AREA_GRAVEYARD:
            case DUEL_AREA_BANISHED:
                CardListView_Open(player, area, 0, 0);
                PlaySE(SE_CONFIRM);
                return 0;
            }
        }
        return 0;
    case EVRESP_ACTIVATE_PICKED:
        switch (gRunDuel.cardMenu.area) {
        case DUEL_AREA_HAND:
            CardMenu_PlaySpellTrapFromHand(1, 0, &gRunChain.responseEntry);
            if (gRunDuel.cardMenu.confirmed)
                return 0;
            break;
        case DUEL_AREA_SPELL_TRAP:
            gRunDuel.cardMenu.confirmed = 0;
            {
                /* Flip the set card face-up (DUEL_CMD_FLIP_CARD; the player bit marks player 1). */
                u32 player = 1 & gRunDuel.cardMenu.player;
                if (!(RUN_ZONE(player, gRunDuel.cardMenu.index + gRunDuel.cardMenu.area)->isFaceUp)) {
                    u16 cmd;
                    if (gRunDuel.cardMenu.player)
                        cmd = DUEL_CMD_FLIP_CARD | DUEL_CMD_PLAYER;
                    else
                        cmd = DUEL_CMD_FLIP_CARD;
                    DuelCmd_PushU16(cmd, gRunDuel.cardMenu.index + gRunDuel.cardMenu.area, 0, 0);
                }
            }
            {
                /* Queue the activation: card | zone << 16 | kind << 21 | event << 25 | player << 31, and
                 * the event's locations. Matching: the hi / z temporaries keep fold from moving the
                 * 0x200000 constant out of the zone term. */
                u32 hi = ((1 & gRunDuel.cardMenu.player) << 31) | (gRunChain.responseEntry.event << 25);
                u32 z = (((gRunDuel.cardMenu.index + gRunDuel.cardMenu.area) & 0x1F) << 16)   /* 5-bit zone */
                        | (CHAIN_KIND_SPELL_TRAP << 21);
                Chain_AddPending(hi | z | gRunDuel.cardMenuCard,
                                 gRunChain.responseEntry.loc0 | (gRunChain.responseEntry.loc1 << 16));
            }
            break;
        }
        gRunChain.responseAdded = 1;
        gRunChain.requestPending = 0;
        return 1;
    case EVRESP_LINK_QUERY:
        DuelLink_SendMessageData(LINKMSG_ACTIVATE_QUERY, &gRunChain.responseEntry, sizeof(struct ChainEntry));
        gRunChain.responseAdded = 0;
        gRunLinkState.queryReplyReceived = 0;
        gRunChain.askStep++;
        return 0;
    case EVRESP_LINK_WAIT:
        /* FAKEMATCH: "& 1" keeps fold from turning the bitfield test into a mask test (the ROM tests it with
         * lsl #28). */
        if (!(gRunLinkState.queryReplyReceived & 1))
            return 0;
        if (!gRunChain.responseAdded) {
            gRunChain.askStep = EVRESP_NEXT_PLAYER;
            return 0;
        }
        gRunChain.requestPending = 0;
        return 1;
    case EVRESP_CPU_SEARCH:
        for (slot = ZONE_SPELL_0; slot <= ZONE_SPELL_4; slot++) {
            u32 id = CARD_ID(CARD_WORD(RUN_ZONE(1 & gRunChain.askPlayer, slot)->card));
            if (id != 0) {
                int speed = GetCardSpellSpeed(id);
                int canActivate = (u16)CanActivateFieldCard(&gRunChain.responseEntry, gRunChain.askPlayer, slot);
                if (speed > SPELL_SPEED_1 && canActivate != 0
                    && AiShouldActivateSetCard2(&gRunChain.responseEntry, 0) != 0)
                    goto found;
            }
        }
        gRunChain.askStep = EVRESP_NEXT_PLAYER;
        return 0;
    case EVRESP_CPU_ACTIVATE:
        if (!(RUN_ZONE(gRunChain.askPlayer, gRunChain.aiZone)->isFaceUp)) {
            u16 cmd;
            if (gRunChain.askPlayer)
                cmd = DUEL_CMD_FLIP_CARD | DUEL_CMD_PLAYER;
            else
                cmd = DUEL_CMD_FLIP_CARD;
            DuelCmd_PushU16(cmd, gRunChain.aiZone, 0, 0);
        }
        {
            u32 hi = (gRunChain.askPlayer << 31) | (gRunChain.responseEntry.event << 25);
            u32 z = (gRunChain.aiZone << 16) | (CHAIN_KIND_SPELL_TRAP << 21);
            Chain_AddPending(hi | z | CARD_ID(CARD_WORD(RUN_ZONE(gRunChain.askPlayer, gRunChain.aiZone)->card)),
                             gRunChain.responseEntry.loc0 | (gRunChain.responseEntry.loc1 << 16));
        }
        gRunChain.responseAdded = 1;
        gRunChain.requestPending = 0;
        return 1;
    case EVRESP_NEXT_PLAYER:
        {
            /* FAKEMATCH: the int temporary keeps combine from turning (1 - askPlayer) & 1 into an eor. */
            int other = 1 - gRunChain.askPlayer;
            gRunChain.askPlayer = other;
        }
        if (gRunChain.askPlayer != gRunChain.firstAskPlayer) {
            gRunChain.askStep = EVRESP_START;
            return 0;
        }
        return 1;
    found:
        gRunChain.aiZone = slot;
        gRunChain.askStep++;
        return 0;
    default:
        gRunChain.requestPending = 0;
        return 1;
    }
}

/*
 * Open a response window for a game event (enum ResponseEventKind; arg = loc0 | loc1 << 16): ask `player`
 * first, then the other player. In a link duel, during player 1's turn (the partner's), the request is
 * forwarded instead as LINKMSG_TRIGGER_EVENT {1 - player, event, arg low half, arg high half}, and the
 * partner opens the window. Otherwise it stores responseEvent / responseEventArg and starts the window:
 * askPlayer = firstAskPlayer = player & 1, askStep = EVRESP_START, aiZone = 0, requestPending = 1,
 * responseAdded = 0.
 */
void EventResponse_Request(int player, u16 event, u32 arg)
{
    if (gDuelCtrl.isLinkDuel) {
        if (gDuel.turnPlayer) {
            u16 message[4];
            message[0] = 1 - player;
            message[1] = event;
            message[2] = arg;
            message[3] = arg >> 16;
            /* The block is 0xA bytes although message holds 8: two stray bytes go along; the receiver reads
             * the four halfwords. */
            DuelLink_SendMessageData(LINKMSG_TRIGGER_EVENT, message, 0xA);
            return;
        }
    }
    {
        struct ResponseFlagsView *flags;
        u8 zero;
        zero = 0;
        gChain.responseEvent = event;
        gChain.responseEventArg = arg;
        flags = (struct ResponseFlagsView *)((u8 *)&gChain + RESPONSE_FLAGS_OFFSET);
        flags->askPlayer = 1 & player;
        flags->firstAskPlayer = 1 & player;
        gChain.askStep = zero;
        flags->aiZone = 0;
        flags->requestPending = 1;
        flags->responseAdded = 0;
    }
}

/*
 * Per-frame driver called by DuelMainStep: while a response window is open (gChain.requestPending), run
 * EventResponse_Run and close the window when it finishes. Returns 1 while a window was open (the duel step
 * waits), else 0.
 */
int EventResponse_Update(void)
{
    if (gChain.requestPending) {
        if ((u16)EventResponse_Run() != 0)
            gChain.requestPending = 0;
        return 1;
    }
    return 0;
}

/*
 * Dead code (no callers): is one of the entries of the chain list `list` (a struct ChainList: 16 entries and
 * a count at +0x140) at (player, zone)?
 */
int IsZoneInChainList(struct ChainList *list, int player, int zone)
{
    int i;
    for (i = 0; i < ((struct ChainList *)list)->count; i++) {
        struct ChainEntry *entry = &((struct ChainList *)list)->entries[i];
        if (entry->player == player && entry->zone == zone)
            return 1;
    }
    return 0;
}

/*
 * Answer the link partner's query "do you respond?" (gLinkState.activateQueryPending): the question is an
 * event (queryIsChainLink 0, LINKMSG_ACTIVATE_QUERY; the partner's event is in gChain.queryEntry) or a chain
 * link (queryIsChainLink 1, LINKMSG_CHAIN_QUERY; the partner's link is in gChain.queryLink). The step machine
 * runs on gChain.queryStep (enum LinkQueryStep); it returns 1 when the answer has been sent.
 *   LINKQUERY_ASK (0): the Yes/No box: for a chain link '<name>'s effect is activated. Resolve it as part of
 *     a Chain?' (an effect text for a monster, a card text otherwise), for an event the event text
 *     (EventResponse_BuildPromptText).
 *   LINKQUERY_WAIT_ANSWER (1): No -> LINKMSG_QUERY_DECLINED and finished. Yes -> 'Please select a Magic or
 *     Trap card ...'; close the card menu.
 *   LINKQUERY_PICK_CARD (2): the field cursor (mask 0xEE: the player's own Magic and Trap cards, set or face
 *     up, and face-up monsters; not the hand) and the card menu; commands from Chain_GetResponseCommands
 *     (chain query) or EventResponse_GetCommands (event query); the Field Magic zone and the hand only
 *     offer Card View; B asks again.
 *   LINKQUERY_SETUP_HANDLERS (3): flip the card face-up (a chain query also shows the 'Chain' banner), fill
 *     queryEntry (card, zone, player 0), look up the card's chainA / chainB in gCardEffects (FindCardEffect;
 *     none for a card without an effect) and clear costStep and targetStep.
 *   LINKQUERY_RUN_CHAIN_A / RUN_CHAIN_B (4, 5): call the handler every frame until it returns nonzero (a
 *     missing handler counts as done). They get queryEntry and queryLink.
 *   LINKQUERY_SEND_RESULT (6 and above): send the card back, LINKMSG_CHAIN_CARD (chain query) or
 *     LINKMSG_ACTIVATE_CARD (event query), with queryEntry; return 1.
 * Matching: every early exit is an explicit `return 0` (cross-jumping keeps the copy after CardMenu_Update);
 * the zone base uses the same symbol as the card-menu struct so that CSE turns it into base + 0x2C; the menu's
 * `index` is a u16:8 field (its extraction is recomputed from the shifted halfword); the flip command is an
 * if/else (jump.c hoists the 0x7F set before the compare).
 */
int DuelLink_AnswerActivateQuery(void)
{
    char text[0x100];

    switch (gAnswerChain.queryStep) {
    case LINKQUERY_ASK:
        if (gAnswerChain.queryIsChainLink) {
            u16 card = gAnswerChain.queryLink.card;
            if (CARD_TYPE(card) <= CARD_TYPE_REPTILE)
                FormatStr(text, gStrLinkChainPromptEffect, CARD_NAME(card));
            else
                FormatStr(text, gStrLinkChainPromptCard, CARD_NAME(card));
        } else {
            EventResponse_BuildPromptText(&gAnswerChain.queryEntry, (u8 *)text);
        }
        TextBoxOpen(QUESTION_BOX_POS, QUESTION_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        gAnswerChain.queryStep++;
        return 0;
    case LINKQUERY_WAIT_ANSWER:
        if (gAnswerTextBox.result == 0) {
            DuelLink_SendMessage(LINKMSG_QUERY_DECLINED, 0, 0, 0);
            return 1;
        }
        if (gAnswerChain.queryIsChainLink)
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectSpellTrapForChain);
        else
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectSpellTrapToActivate);
        gAnswerChain.queryStep++;
        gAnswerDuel.cardMenu.open = 0;
        gAnswerDuel.cardMenu.confirmed = 0;
        return 0;
    case LINKQUERY_PICK_CARD:
        if (gAnswerDuel.cardMenu.open) {
            CardMenu_Update();
            return 0;
        }
        if (gAnswerDuel.cardMenu.confirmed) {
            gAnswerChain.queryStep++;
            return 0;
        }
        if (gAnswerMain.newKeys & B_BUTTON) {
            gAnswerChain.queryStep = LINKQUERY_ASK;
            return 0;
        }
        if (DuelCursor_PickTarget(PICK_ANY_SPELL_TRAP | PICK_FACE_UP_MONSTER_ANY) == 0)
            return 0;
        {
            u32 player = gAnswerScreen.selPlayer;
            u32 area = gAnswerScreen.selArea;
            u32 index = gAnswerScreen.selIndex;
            u16 card = DuelCursor_GetCardId();
            switch (area) {
            case DUEL_AREA_MONSTER:
            case DUEL_AREA_SPELL_TRAP:
                if (card != 0) {
                    gAnswerDuel.cardMenu.open = 1;
                    gAnswerDuel.cardMenu.state = 0;
                    if (gAnswerChain.queryIsChainLink)
                        gAnswerDuel.cardMenu.available =
                            (u16)Chain_GetResponseCommands(&gAnswerChain.queryLink, player, area, index);
                    else
                        gAnswerDuel.cardMenu.available =
                            (u16)EventResponse_GetCommands(&gAnswerChain.queryEntry, player, area, index);
                    return 0;
                }
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_FIELD:
            case DUEL_AREA_HAND:
                if (card != 0) {
                    gAnswerDuel.cardMenu.open = 1;
                    gAnswerDuel.cardMenu.state = 0;
                    gAnswerDuel.cardMenu.available = CARDMENU_MASK_CARD_VIEW;
                    return 0;
                }
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_DECK:
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
            case DUEL_AREA_GRAVEYARD:
            case DUEL_AREA_BANISHED:
                CardListView_Open(player, area, 0, 0);
                PlaySE(SE_CONFIRM);
                return 0;
            }
        }
        return 0;
    case LINKQUERY_SETUP_HANDLERS:
        gAnswerDuel.cardMenu.confirmed = 0;
        if (gAnswerChain.queryIsChainLink)
            DuelCmd_PushU16(DUEL_CMD_CHAIN_BANNER, 0, 0, 0);
        {
            u32 player = 1 & gAnswerDuel.cardMenu.player;
            if (!(ANSWER_ZONE(player, gAnswerDuel.cardMenu.index + gAnswerDuel.cardMenu.area)->isFaceUp)) {
                u16 cmd;
                if (gAnswerDuel.cardMenu.player)
                    cmd = DUEL_CMD_FLIP_CARD | DUEL_CMD_PLAYER;
                else
                    cmd = DUEL_CMD_FLIP_CARD;
                DuelCmd_PushU16(cmd, gAnswerDuel.cardMenu.index + gAnswerDuel.cardMenu.area, 0, 0);
            }
        }
        gAnswerChain.queryEntry.zone = gAnswerDuel.cardMenu.area + gAnswerDuel.cardMenu.index;
        gAnswerChain.queryEntry.card =
            CARD_ID(CARD_WORD(ANSWER_ZONE(1 & gAnswerDuel.cardMenu.player, gAnswerChain.queryEntry.zone)->card));
        gAnswerChain.queryEntry.player = 0;
        {
            int n = FindCardEffect(gAnswerChain.queryEntry.card);
            if (n == -1) {
                gAnswerChain.chainA = NULL;
                gAnswerChain.chainB = NULL;
            } else {
                /* struct CardEffect declares the handlers int-returning, struct ChainState u16-returning. */
                gAnswerChain.chainA = (ChainHandler)gAnswerEffects[n].chainA;
                gAnswerChain.chainB = (ChainHandler)gAnswerEffects[n].chainB;
            }
        }
        gAnswerChain.costStep = 0;
        gAnswerChain.targetStep = 0;
        gAnswerChain.queryStep++;
        return 0;
    case LINKQUERY_RUN_CHAIN_A:
        if (gAnswerChain.chainA != NULL) {
            if (gAnswerChain.chainA(&gAnswerChain.queryEntry, &gAnswerChain.queryLink) != 0)
                gAnswerChain.queryStep++;
        } else {
            gAnswerChain.queryStep++;
        }
        return 0;
    case LINKQUERY_RUN_CHAIN_B:
        if (gAnswerChain.chainB != NULL) {
            if (gAnswerChain.chainB(&gAnswerChain.queryEntry, &gAnswerChain.queryLink) != 0)
                gAnswerChain.queryStep++;
        } else {
            gAnswerChain.queryStep++;
        }
        return 0;
    default:
        if (gAnswerChain.queryIsChainLink)
            DuelLink_SendMessageData(LINKMSG_CHAIN_CARD, &gAnswerChain.queryEntry, sizeof(struct ChainEntry));
        else
            DuelLink_SendMessageData(LINKMSG_ACTIVATE_CARD, &gAnswerChain.queryEntry, sizeof(struct ChainEntry));
        return 1;
    }
}
