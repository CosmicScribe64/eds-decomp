#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_TYPE, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, ZoneLinkKind, ResponseEventKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER, enum TokenKind */
#include "constants/sound.h"        /* enum SoundEffect */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/*
 * Card effect handlers: the Resolve slot of gCardEffects (include/effect.h) for the effect keys 1221-1318
 * (wiki/functions/effect-resolve9-c.md). No EDS card has these numbers (gCardNumberToId[key] is 0), so the
 * game never reaches them: they are leftovers of the shared Duel Monsters engine, named after what they do
 * (include/effect_handlers.h names the OCG card each key behaves like, as a hypothesis).
 *
 * Chain_Resolve calls a link's resolve handler once per frame with gChain.effectStep as the handler's step:
 * 0x80 (EFFECT_STEP_START) on the first call, then whatever the previous call returned, until a handler
 * returns 0. The steps count down from 0x80 and are private to each handler; a returned step without a case
 * (0x64, 0x78, 0x0A, ...) ends the effect on the next call. Most handlers do nothing when the link was negated.
 *
 * Players: 0 is the human, 1 the CPU (or the link partner); the CPU skips the Yes/No questions. Duel commands
 * queued for player 1 carry DUEL_CMD_PLAYER (bit 15). Zones 0-4 hold monsters, 5-9 spells and traps, 10 the
 * Field Magic; a target or location is player | zone << 8 (DUEL_LOC).
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, card_list_view.h, duel_cmd.h, duel_screen.h and summon.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" and #include "sound.h" (see
 * build/readability/issues/effect_resolve9.md). */
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
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4[0x28 - 0x4];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelZone gDuelFieldZone;          /* 0x020198D4 = gDuelPlayers[0].zones[ZONE_FIELD] */

u32 IsMonsterZoneFree(int player, int zone);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int FindFreeSpellTrapZone(int player);
int CanPlaceSpellTrapCard(int player, u16 cardId);
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
u32 GetZoneCardType(s32 player, s32 slot);
u32 GetZoneCardAtk(u32 player, u32 slot);
int CountGraveyardCardsByNumber(int player, u16 cardNo);
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out);
int FindDeckCardByNumber(int player, u16 number, int limit);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* discards, draws, equips, field moves, QueueAddZoneLink */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_PostDiscard */
#include "duel_screen.h"            /* gDuelScreen.selPlayer / selArea / selIndex, DuelCursor_PickTarget */
#include "effect.h"                 /* gCardEffects, FindCardEffect, the target and destruction helpers */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers, EffectOwnSkullOrThunderCheck */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr, MemCopy16, Random */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: CollectEffectTargets is defined to return u16; the ROM tests the result as an int, without the
 * narrowing of r0 that the u16 return adds. */
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: the definition returns u32; EffectDestroyWeakerDefenseResolve compares DEF with the ATK as signed
 * ints (cmp; bge). */
int GetZoneCardDefInt(int player, int zone) asm("GetZoneCardDef");
/* Matching: EffectRedirectTargetResolve passes its int chainedTo on and returns the result as a u16, without
 * narrowing it (the definition returns int and takes a struct ChainEntry *). */
u16 EffectFairysHandMirrorResolveU16(struct ChainEntry *link, int chainedTo) asm("EffectFairysHandMirrorResolve");
/* Matching: the definition takes u16 byOpponentEffect / compactHand; EffectDiscardHandsAndRedrawResolve passes
 * its loop counter without the narrowing (lsl #16; lsr #16) a u16 parameter adds. */
void DiscardHandCardInt(int player, int handIdx, int byOpponentEffect, int compactHand) asm("DiscardHandCard");

/* Alias symbols (address-suffixed, card_data.h and chain.h list them). Matching: the ROM loads these element
 * addresses from their own literals. */
extern const u16 gUnk_086243C8[];   /* &gCardNumberToId[CARD_GAZELLE_THE_KING_OF_MYTHICAL_BEASTS] */
extern const u16 gUnk_086247C8[];   /* &gCardNumberToId[CARD_1258] (0: no EDS card) */
extern u32 gUnk_02017F84[];         /* gChain.scratch.effect.effectCards */

/* Prompts used only by this unit. */
extern const u8 gStrAddFromDeckToHandPrompt[];  /* 0x08083430 "Do you wish to add %s from the Deck to your
                                                 * hand?" */
extern const u8 gStrEquipFromGraveyardPrompt[]; /* 0x08083468 "%s has been sent to the Graveyard. Do you wish to
                                                 * equip it to a monster on the field?" */

/* ---- Helpers ---- */

/* The command id for a player: player 1's commands carry DUEL_CMD_PLAYER. CMD_FOR is the link's player,
 * CMD_FOR_OPPONENT the other one. */
#define CMD_FOR_PLAYER(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))
#define CMD_FOR(link, cmd)          CMD_FOR_PLAYER((link)->player, cmd)
#define CMD_FOR_OPPONENT(link, cmd) CMD_FOR_PLAYER(!(link)->player, cmd)

/* Text box placement: TextBoxOpen takes pos = x | y << 8 and size = width | height << 8, in cells. */
#define TEXT_BOX_CELLS(x, y)        ((x) | (y) << 8)
#define PROMPT_POS                  TEXT_BOX_CELLS(6, 2)
#define PROMPT_SIZE_18x7            TEXT_BOX_CELLS(18, 7)
#define PROMPT_SIZE_19x6            TEXT_BOX_CELLS(19, 6)

/* DuelCursor_PickTarget mask: face-up monsters in either position (0xE0; PICK_PLAYER1 for player 1's side). */
#define PICK_FACE_UP_MONSTERS       (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)

/* DUEL_CMD_PLACE_CARD arg2: zone | faceUp << 8 | defense << 9. */
#define PLACE_FACE_UP               (1 << 8)

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts. */
#define CARD_WORD(card)             (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)               (((word) << 20) >> 20)
/* DuelCard.owner (bit 12) of a card word: lsl #19; lsr #31. */
#define CARD_WORD_OWNER(word)       ((u32)(word) << 19 >> 31)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0), gCardIdToNumber (0x08622AB4),
 * gCardNumberToId (0x08623DF4) and gCardNames (0x0822C720). Matching: these give the ROM's literal pools and
 * reloads; the symbol forms (card_data.h) generate other code.
 */
#define CARD_STATS(id)              (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)             (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id)               CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */
#define CARD_NUMBER_TO_ID(number)   (((const u16 *)0x08623DF4)[number])
#define CARD_NAMES_ADDR             ((const char *)0x0822C720)

/*
 * &gDuelZones[side].zones[zone] by byte arithmetic, side being 0 or 1. Matching: the two forms add the zone
 * and player terms in the orders the ROM uses.
 */
#define ZONE_AT(side, zone) /* zone term first */ \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (side) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(side, zone) /* player term first */ \
    ((struct DuelZone *)((side) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* The Field Magic zone of player `side` through gDuelFieldZone with the player stride. */
#define FIELD_ZONE(side) \
    ((struct DuelZone *)((side) * sizeof(struct DuelPlayer) + (u32)&gDuelFieldZone))

/* gChain.scratch.effect.effectCards[0] as its two halfwords (the duel commands take a card word that way). */
#define EFFECT_CARD_HALVES          ((u16 *)&gChain.scratch.effect.effectCards[0])

/* ---- Resolve handlers ---- */

/*
 * Key 1221 (hypothesis: OCG Card Destruction): both players discard their whole hand and draw as many cards
 * as they discarded of their own (hand cards owned by the other player are not counted). link->targets[p]
 * holds player p's draw count.
 *   0x80  for the player, then the opponent: count, queue the discards; then both draw
 *   0x7F  Forced Requisition for the player's discards
 *   0x7E  the same for the opponent's
 */
int EffectDiscardHandsAndRedrawResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80: {
            int i;

            for (i = 0; i <= 1; i++) {
                int player;
                int k;

                if (i)
                    player = 1 - link->player;
                else
                    player = link->player;
                link->targets[player] = gDuelPlayers[player & 1].handCount;
                /* The discards are queued commands: the hand keeps its cards while this loop runs, and each
                 * command discards the then first card. */
                for (k = 0; k < gDuelPlayers[player & 1].handCount; k++) {
                    if (CARD_WORD_OWNER(CARD_WORD(gDuelPlayers[player & 1].hand[k])) != player)
                        link->targets[player]--;    /* a card of the other player's */
                    DiscardHandCardInt(player, 0, i, TRUE);
                }
            }
            for (i = 0; i <= 1; i++) {
                int player;

                if (i)
                    player = 1 - link->player;
                else
                    player = link->player;
                DrawCards(player, link->targets[player]);
            }
            return 0x7F;
        }
        case 0x7F:
            TriggerForcedRequisition(link->player, link->targets[link->player]);
            return 0x7E;
        case 0x7E:
            TriggerForcedRequisition(1 - link->player, link->targets[1 - link->player]);
            return 0x7D;
        }
    }
    return 0;
}

/* Key 1232 (hypothesis: OCG Multiply): summon a token (TOKEN_KIND_1, card number 1921, defense position)
 * into each of the player's free monster zones. */
int EffectSummonTokensInFreeZonesResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int zone;

        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            if (IsMonsterZoneFree(link->player, zone))
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SUMMON_TOKEN), (u8)zone | link->zone << 8, TOKEN_KIND_1, 0);
        }
    }
    return 0;
}

/*
 * Key 1240 (hypothesis: OCG Berfomet): add Gazelle the King of Mythical Beasts from the deck to the hand.
 *   0x80  needs a Gazelle in the deck (target collector, key 1240); the human is asked (Yes/No), the CPU
 *         goes straight on
 *   0x7F  wait for the answer; No ends the effect
 *   0x7E  add the card
 */
int EffectAddGazelleFromDeckResolve(struct ChainEntry *link)
{
    char text[0x80];

    switch (gChain.effectStep) {
    case 0x80:
        if (CollectEffectTargetsInt(link->player, CARD_1240, 0) == 0)
            return 0;
        if (link->player)
            goto addCard;
        FormatStr(text, (const char *)gStrAddFromDeckToHandPrompt,
                  (const char *)gCardNames + gUnk_086243C8[0] * CARD_NAME_SIZE);   /* Gazelle's name */
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        return 0x7F;
    case 0x7F:
        if (gTextBox.result == 0)
            return 0;
    addCard:
        return 0x7E;
    case 0x7E:
        AddDeckCardToHand(link->player, CARD_GAZELLE_THE_KING_OF_MYTHICAL_BEASTS);
        return 0x7D;
    }
    return 0;
}

/*
 * Key 1242, sent to the graveyard: offer to equip the card from the graveyard to a face-up monster on either
 * side.
 *   0x80  needs the card in the graveyard, a free spell/trap zone and a face-up monster
 *   0x7F  ask "%s has been sent to the Graveyard. Do you wish to equip it ...?" (Yes/No)
 *   0x7E  on Yes: "Select a monster that you wish to equip with %s"
 *   0x7D  cursor pick of a face-up monster (B goes back to the question): take the card out of the graveyard,
 *         place it face up in the free zone and equip it to the picked monster
 */
int EffectEquipSelfFromGraveyardResolve(struct ChainEntry *link)
{
    char text[0x80];

    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80: {
            int i;

            if (CountGraveyardCardsByNumber(link->player, CARD_1242) == 0)
                return 0;
            if (FindFreeSpellTrapZone(link->player) == -1)
                return 0;
            for (i = 0; i <= 1; i++) {
                int zone;

                for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                    struct DuelZone *z = ZONE_AT_PLAYER_FIRST(i & 1, zone);

                    if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp)
                        goto ask;
                }
            }
            return 0;
        }
        case 0x7F:
            /* Matching: the card names through CARD_NAMES_ADDR: the constant stays in the add, and the reload
             * goes into the register the ROM uses (r0 here, r3 in step 0x7E), which the symbol does not. */
            FormatStr(text, (const char *)gStrEquipFromGraveyardPrompt, CARD_NAMES_ADDR + link->card * CARD_NAME_SIZE);
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return 0x7E;
        case 0x7E:
            if (gTextBox.result == 0)
                return 0;
            FormatStr(text, (const char *)gStrSelectEquipTarget, CARD_NAMES_ADDR + link->card * CARD_NAME_SIZE);
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
        pick:
            return 0x7D;
        case 0x7D: {
            u32 targetPlayer;
            u32 targetZone;
            int freeZone;
            u16 *card;

            if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTERS) | PICK_FACE_UP_MONSTERS) != 0) {
                targetPlayer = gDuelScreen.selPlayer;
                targetZone = gDuelScreen.selArea + gDuelScreen.selIndex;    /* area 0 + zone */
                freeZone = FindFreeSpellTrapZone(link->player);
                PlaySE(SE_CONFIRM);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), (u16)gDuelScreen.selPlayer,
                             (u8)gDuelScreen.selArea | (u8)gDuelScreen.selIndex << 8, 0);
                GetGraveyardCardById(link->player, link->card, (struct DuelCard *)(card = EFFECT_CARD_HALVES));
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), card[0], card[1], 0);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_PLACE_CARD), (u8)freeZone | PLACE_FACE_UP, card[0], card[1]);
                EquipCard(link->player, link->player | (u8)freeZone << 8, (u8)targetPlayer | (u8)targetZone << 8);
                return 0x64;
            }
            if (!(gMain.newKeys & B_BUTTON))
                goto pick;
        ask:
            return 0x7F;
        }
        }
    }
    return 0;
}

/* Key 1245 (hypothesis: OCG Scapegoat): with at least 4 free monster zones, summon four tokens (TOKEN_KIND_2,
 * card number 1922, defense position) and forbid the player's Normal and Special Summons this turn. */
int EffectSummonFourTokensResolve(struct ChainEntry *link)
{
    if (!link->negated && CountFreeMonsterZones(link->player) > 3) {
        int summoned;
        int zone;

        for (zone = ZONE_MONSTER_0, summoned = 0; zone <= ZONE_MONSTER_4 && summoned <= 3; zone++) {
            if (IsMonsterZoneFree(link->player, zone)) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SUMMON_TOKEN), (u8)zone | link->zone << 8, TOKEN_KIND_2, 0);
                summoned++;
            }
        }
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_SUMMON_LOCKS), TRUE, TRUE, 0);
    }
    return 0;
}

/*
 * Key 1246, on summon: this monster gains the base ATK/DEF of an opponent's face-up monster
 * (ZONE_LINK_ADD_CARD_STATS link with the picked card's ID).
 *   0x80  needs a face-up opponent monster; "Select an opponent's monster that you wish to transfer the
 *         ATK/DEF factors."
 *   0x7F  cursor pick on player 1's side, then link the picked card's ID to this card's zone
 */
int EffectGainOpponentMonsterStatsResolve(struct ChainEntry *link)
{
    switch (gChain.effectStep) {
    case 0x80:
        if (CountMonstersFiltered(1 - link->player, TRUE, FALSE) == 0)
            return 0;
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrSelectStatsSourceMonster);
    pick:
        return 0x7F;
    case 0x7F: {
        int selPlayer;
        int zone;
        u16 cardId;

        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTERS)) == 0)
            goto pick;
        selPlayer = gDuelScreen.selPlayer;
        zone = gDuelScreen.selIndex;
        /* The picked card's ID: gDuelZones[selPlayer & 1].zones[zone] (Matching: the base added first). */
        cardId = CARD_ID(*(u32 *)((u32)gDuelZones + zone * sizeof(struct DuelZone)
                                  + (selPlayer & 1) * sizeof(struct DuelPlayer)));
        QueueAddZoneLink(link->player, cardId, link->player | link->zone << 8, ZONE_LINK_ADD_CARD_STATS);
        return 0x78;
    }
    }
    return 0;
}

/*
 * Key 1247 (hypothesis: OCG Mystical Refpanel): run the opponent's one-player Magic card (chainedTo) for this
 * card's player, then negate the original. The first call copies chainedTo into gChain.proxyLink with the
 * player bit flipped and looks up its resolve handler (gChain.proxyResolve). For the accepted Magic cards
 * (the heal and burn cards, Graceful Charity, Pot of Greed, ...) each call runs one step of that handler;
 * when it is finished the original activation is negated (DUEL_CMD_NEGATE_ACTIVATION). Returns the proxied
 * handler's step, or 0.
 */
int EffectReflectPlayerMagicResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    if (link->negated)
        return 0;
    if (gChain.effectStep == 0x80) {
        MemCopy16(&gChain.proxyLink, chainedTo, sizeof(struct ChainEntry));
        {
            int otherPlayer = 1 - gChain.proxyLink.player;

            gChain.proxyLink.player = otherPlayer;
        }
        gChain.proxyResolve = gCardEffects[FindCardEffect(gChain.proxyLink.card)].resolve;
    }
    switch (CARD_NUMBER(chainedTo->card)) {
    case CARD_MOOYAN_CURRY ... CARD_OOKAZI:
    case CARD_GRACEFUL_CHARITY:
    case CARD_BLUE_MEDICINE:
    case CARD_RAIMEI:
    case CARD_POT_OF_GREED:
    case CARD_THE_CHEERFUL_COFFIN:
    case CARD_RESTRUCTER_REVOLUTION:
    case CARD_CONFISCATION:
    case CARD_DELINQUENT_DUO:
    case CARD_PAINFUL_CHOICE:
        gChain.effectStep = gChain.proxyResolve(&gChain.proxyLink, NULL);
        if (gChain.effectStep == 0) {
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), TRUE, 0, 0);
            return 0;
        }
        return gChain.effectStep;
    }
    return 0;
}

/* Key 1248: with the target (the player's face-up Summoned Skull or Thunder monster, re-checked) still valid,
 * destroy every face-up monster on the other side whose DEF is below the target's ATK. */
int EffectDestroyWeakerDefenseResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1 && EffectOwnSkullOrThunderCheck(link, link->targets[0])) {
        int targetPlayer = (u8)link->targets[0];
        int atk = GetZoneCardAtk(targetPlayer, DUEL_LOC_ZONE(link->targets[0]));
        int zone;
        int player;

        /* Matching: separate assignments (not initializers) give the ROM's register order. */
        zone = ZONE_MONSTER_0;
        player = 1 - targetPlayer;     /* the side opposite the target */
        for (; zone <= ZONE_MONSTER_4; zone++) {
            struct DuelZone *z = ZONE_AT(player & 1, zone);

            if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp && GetZoneCardDefInt(player, zone) < atk
                && IsZoneTargetable(player, zone)) {
                DestroyFieldCardByEffect(player, zone);
                OnCardDestroyedByEffect(link->player, player, zone);
            }
        }
    }
    return 0;
}

/* Key 1255, flip: take control of the target, an opponent's face-up Machine, if the player has a free monster
 * zone. The ZONE_LINK_CARD_EFFECT link marks it; DuelCmd_TurnEnd gives it back at the end of the turn. */
int EffectTakeControlOfMachineResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->numTargets == 1) {
            int targetPlayer = (u8)link->targets[0];
            int targetZone = DUEL_LOC_ZONE(link->targets[0]);
            int freeZone = FindFreeMonsterZone(link->player);

            if (targetPlayer != link->player) {
                int side = targetPlayer & 1;
                struct DuelZone *z = ZONE_AT(side, targetZone);

                if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp && freeZone != -1
                    && GetZoneCardType(targetPlayer, targetZone) == CARD_TYPE_MACHINE) {
                    MoveFieldCard(link->player, link->targets[0], link->player | (u8)freeZone << 8);
                    QueueAddZoneLink(link->player, link->card, link->player | (u8)freeZone << 8, ZONE_LINK_CARD_EFFECT);
                }
            }
        }
    }
    return 0;
}

/*
 * Key 1256, flip: each player discards one card.
 *   0x80  the player, if the hand is not empty
 *   0x7F  the opponent (byOpponent set)
 */
int EffectEachPlayerDiscardsResolve(struct ChainEntry *link, int unused)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80:
            if (gDuelPlayers[link->player].handCount != 0)
                DuelPrompt_PostDiscard(link->player, 1, FALSE, FALSE);
            return 0x7F;
        case 0x7F:
            if (gDuelPlayers[(1 - link->player) & 1].handCount != 0)
                DuelPrompt_PostDiscard(1 - link->player, 1, FALSE, TRUE);
            return 0x7E;
        }
    }
    return 0;
}

/*
 * Key 1257, sent to the graveyard: take key 1258 out of the deck, equip it to a face-up monster and give that
 * monster to the other side. Does not test negated.
 *   0x80  needs key 1258 in the deck, room for it and a face-up monster; takes it into
 *         gChain.scratch.effect.effectCards[0]
 *   0x7F  "Select a monster that you wish to equip with %s"
 *   0x7E  cursor pick: place the card face up and equip it; the monster's location goes to
 *         gChain.scratch.effect.savedValue
 *   0x7D  move the monster to a free zone of the other player, shuffle the deck
 */
int EffectEquipFromDeckAndSwitchControlResolve(struct ChainEntry *link)
{
    char text[0x80];

    switch (gChain.effectStep) {
    case 0x80: {
        int i;
        int number;

        /* 999: search the whole deck. Matching: the number is assigned inside the call. */
        if (FindDeckCardByNumber(link->player, number = CARD_1258, 999) == -1)
            return 0;
        if (!CanPlaceSpellTrapCard(link->player, CARD_NUMBER_TO_ID(number)))
            return 0;
        for (i = 0; i <= 1; i++) {
            int zone;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT_PLAYER_FIRST(i & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp)
                    goto found;
            }
        }
        return 0;
    }
    case 0x7F:
        FormatStr(text, (const char *)gStrSelectEquipTarget, (const char *)gCardNames + gUnk_086247C8[0] * CARD_NAME_SIZE);
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
    pick:
        return 0x7E;
    case 0x7E: {
        int player;
        int freeZone;
        u32 targetPlayer;
        u32 targetZone;
        int targetLoc;
        u16 *card;

        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTERS) | PICK_FACE_UP_MONSTERS) == 0)
            goto pick;
        player = link->player;
        freeZone = FindFreeSpellTrapZone(player);
        targetPlayer = gDuelScreen.selPlayer;
        targetZone = gDuelScreen.selIndex;
        DuelCmd_Push(CMD_FOR_PLAYER(player, DUEL_CMD_PLACE_CARD), (u8)freeZone | PLACE_FACE_UP,
                     (card = EFFECT_CARD_HALVES)[0], card[1]);
        EquipCard(link->player, player | (u8)freeZone << 8, targetLoc = (u8)targetPlayer | (u8)targetZone << 8);
        gChain.scratch.effect.savedValue = targetLoc;
        return 0x7D;
    }
    case 0x7D: {
        int otherPlayer = 1 - (u8)gChain.scratch.effect.savedValue;
        int zone = FindFreeMonsterZone(otherPlayer);

        if (zone >= 0)
            MoveFieldCard(link->player, gChain.scratch.effect.savedValue, (u8)otherPlayer | (u8)zone << 8);
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
        return 0x7C;
    }
    }
    goto done;
found:
    /* Matching: the call after the switch, reached by goto, keeps the loop counters out of callee-saved
     * registers. */
    RemoveDeckCardByNumber(link->player, CARD_1258, gUnk_02017F84);
    return 0x7F;
done:
    return 0;
}

/* Key 1258: return the card to the top of the deck and shuffle. */
int EffectReturnSelfToDeckResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        ReturnFieldCardToDeck(link->player, link->zone);
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
    }
    return 0;
}

/*
 * Key 1303 (hypothesis: OCG Gamble): coin toss. A right call draws until the hand holds 5 cards, a wrong one
 * skips the player's next turn.
 *   0x80  the human calls Heads or Tails (two-choice menu); the CPU calls at random
 *   0x7F  toss (DUEL_CMD_TOSS_COIN shows it), then the draw or DUEL_CMD_SKIP_NEXT_TURN
 * Returns 0x0A (no case: the effect ends on the next call).
 */
int EffectCoinTossDrawToFiveResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80:
            if (!link->player) {
                TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrCoinTossMenu);
                TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            } else {
                gTextBox.result = Random() & 1;     /* enum CoinFace */
            }
            return 0x7F;
        case 0x7F: {
            int result = Random() & 1;  /* enum CoinFace */

            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_TOSS_COIN), gTextBox.result, (u16)result, 0);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);
            if (result == gTextBox.result) {
                if (gDuelPlayers[1 & link->player].handCount <= 4)
                    DrawCards(link->player, 5 - gDuelPlayers[1 & link->player].handCount);
            } else {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SKIP_NEXT_TURN), 1, 0, 0);
            }
            return 0x0A;
        }
        }
    }
    return 0;
}

/* Key 1311 (hypothesis: OCG Burning Land): destroy the card in each player's Field Magic zone. */
int EffectDestroyFieldMagicsResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = 0; i <= 1; i++) {
            if (CARD_ID(CARD_WORD(FIELD_ZONE(i & 1)->card)))
                DestroyFieldCard(i, ZONE_FIELD, TRUE);
        }
    }
    return 0;
}

/* Key 1312 (hypothesis: OCG Cold Wave): neither player may play Magic or Trap cards for 2 turns. */
int EffectLockMagicTrapResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_MAGIC_TRAP_LOCK_TURNS), 2, 0, 0);
        DuelCmd_Push(CMD_FOR_OPPONENT(link, DUEL_CMD_SET_MAGIC_TRAP_LOCK_TURNS), 2, 0, 0);
    }
    return 0;
}

/* Key 1314 (hypothesis: OCG Limiter Removal): link this card's effect (a stat modifier) to each of the
 * player's face-up Machines. Does not test negated. */
int EffectDoubleMachineAtkResolve(struct ChainEntry *link)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        if (CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(1 & link->player, zone)->card))
            && ZONE_AT_PLAYER_FIRST(1 & link->player, zone)->isFaceUp
            && GetZoneCardType(link->player, zone) == CARD_TYPE_MACHINE && IsZoneTargetable(link->player, zone))
            QueueAddZoneLink(link->player, link->card, link->player | (u8)zone << 8, ZONE_LINK_CARD_EFFECT);
    }
    return 0;
}

/* Key 1316: return the target (one of the player's monsters) and the player's whole hand to the deck,
 * shuffle, and draw as many cards as the hand held. */
int EffectReturnMonsterAndRedrawResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->numTargets == 1) {
            int targetPlayer = (u8)link->targets[0];
            int targetZone = DUEL_LOC_ZONE(link->targets[0]);

            if (CARD_ID(CARD_WORD(gDuelPlayers[targetPlayer & 1].zones[targetZone].card))) {
                int count;
                int i;

                ReturnFieldCardToDeck(targetPlayer, targetZone);
                count = gDuelPlayers[link->player & 1].handCount;
                for (i = 0; i < count; i++)
                    ReturnHandCardToDeck(link->player, 0, TRUE);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
                DrawCards(link->player, count);
            }
        }
    }
    return 0;
}

/*
 * Key 1317 (hypothesis: OCG Shift): answering an attack declaration, make the target (one of the player's
 * monsters) the new attack target. Answering anything else, EffectFairysHandMirrorResolve moves chainedTo's
 * effect onto the new target. Returns that handler's step, or 0.
 */
u16 EffectRedirectTargetResolve(struct ChainEntry *link, int chainedTo)
{
    if (link->negated)
        return 0;
    if (link->event == RESPONSE_ATTACK_DECLARED) {
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player,
                     (DUEL_LOC_ZONE(link->targets[0])) << 8 | DUEL_AREA_MONSTER, 0);
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_ATTACK_TARGET), link->targets[0], TRUE, 0);
        return 0;
    }
    return EffectFairysHandMirrorResolveU16(link, chainedTo);
}

/*
 * Key 1318 (hypothesis: OCG Insect Imitation): Special Summon an Insect from the deck whose level is
 * targets[0] (the tributed monster's level + 1, set by EffectTributeForLevelChainB).
 *   0x80  needs one target and a matching deck Insect (target collector, key 1318); "Please select a monster
 *         from the list that you wish to Special-Summon from your Deck"
 *   0x7F  open the card list
 *   0x7E  take the picked card out of the deck
 *   0x7D  Special Summon it (position asked)
 *   0x7C  shuffle the deck
 */
int EffectSummonInsectFromDeckResolve(struct ChainEntry *link)
{
    u32 *card = &gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80:
            if (link->numTargets == 1 && CollectEffectTargetsInt(link->player, CARD_1318, link->targets[0])) {
                TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, gStrSelectDeckMonsterToSummon);
                return 0x7F;
            }
            break;
        case 0x7F:
            CardListView_Open(link->player, -1, CARD_1318, link->targets[0]);
            return 0x7E;
        case 0x7E:
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_DECK), ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7D;
        case 0x7D:
            /* faceUp FALSE; the status flag is the one graveyard summons set, although the card comes from
             * the deck. */
            QueueSpecialSummonChoosePosition(link->player,
                (struct DuelCard *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top],
                FALSE, ZONE_STATUS_FROM_GRAVEYARD);
            return 0x7C;
        case 0x7C:
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
