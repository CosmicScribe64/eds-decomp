#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_KIND */
#include "constants/cards.h"        /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, ... */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind */
#include "constants/duel.h"         /* enum DuelArea, enum DuelZoneIndex, enum ZoneLinkKind */

/*
 * Duel command handlers that move cards between the hand and the piles: deck, graveyard, banished pile and
 * fusion deck (commands 0x69-0x6B, 0xC0, 0xC1, 0xC3, 0xD0-0xDC and 0xDE; the other hand commands are in
 * duel_cmd_hand.c). DuelCmd_Dispatch calls the handler of gDuelCmd.cmd once per frame until the handler
 * clears gDuelCmd.running.
 *
 * One-frame handlers update a pile directly. The others are small state machines on gDuelCmd.step:
 *   0        scroll the field to the area involved (DuelScreen_ScrollToZone)
 *   1        take the card out of its pile into gDuelCmd.card and start the card-move animation
 *   2        put the card into its new pile
 *   default  redraw the piles and clear running
 * DuelMainStep runs the command queue only while DuelScreen_Update reports nothing in progress, so each step
 * starts after the scroll or animation of the previous one has finished.
 *
 * Operands: the acting player is bit 15 of the command; arg2 is a hand or graveyard index or a card ID, or
 * arg2 | arg4 << 16 is a whole card word (struct DuelCard). See wiki/functions/duel-cmd-piles-c.md.
 */

/* ---- BEGIN duel.h / duel_cmd.h / duel_screen.h subset (pre-H0) ---- */
/*
 * The declarations of include/duel.h, duel_cmd.h and duel_screen.h this unit uses, with the headers' tags,
 * field names, types and bitfield containers. include/duel.h still holds the legacy header until the header
 * switch (H0, build/readability/HEADERS.md), and duel_cmd.h and duel_screen.h include it. After H0, replace
 * this block (BEGIN to END) with #include "duel.h", "duel_cmd.h" and "duel_screen.h".
 */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:5;
    u32 graverobbed:1;              /* bit 18: taken with Graverobber; cleared when it leaves the field */
    u32 unk19:4;
    u32 flag23:1;                   /* bit 23: set by DUEL_CMD_MARK_GRAVEYARD_CARD; reader unknown */
    u32 pendingEquip:1;             /* bit 24: graveyard card waiting to be equipped at end of turn */
    u32 equipZone:3;                /* bits 25-27: monster zone that pendingEquip card goes to */
    u32 pendingOpponentSummon:1;    /* bit 28: graveyard card the opponent may Special Summon at end of turn */
    u32 unk29:3;
};

struct DuelCardStatusBytes;         /* ClearCardStatusFlags' view of a card word */

/* A card location on the duel screen: the endpoints of the card-move animation. */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;
};

struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;                   /* +0x002: entries in hand[]; also the slot the next card lands in */
    u8 unk3;
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 unk5[6];
    u8 crushCardTurns:3;            /* +0x00B bits 0-2: turns left of Crush Card's draw check */
    u8 unkB_3:5;
    u8 unkC[0x684 - 0xC];
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};

struct DuelCmd {
    u16 cmd;                        /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                       /* +0x002 */
    u16 arg4;                       /* +0x004 */
    u16 arg6;                       /* +0x006 */
    u8 queue[0x808 - 0x8];          /* +0x008: struct DuelCmdEntry queue[256] */
    u16 queueCount;                 /* +0x808 */
    u16 step:7;                     /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;                  /* +0x80A bits 7-13: handler loop counter or saved value */
    u16 unk80A_14:2;
    u32 unk80C_0:5;                 /* +0x80C */
    u32 timer:7;                    /* +0x80C bits 5-11 */
    u32 unk80C_12:1;
    u32 running:1;                  /* +0x80D bit 5: set by the queue runner, cleared by the finished handler */
    u32 unk80C_14:18;
    u16 *hofsTable;                 /* +0x810 */
    struct DuelCard card;           /* +0x814: card saved by the current command (the moving card) */
};

extern struct DuelPlayer gDuelPlayers[2];   /* 0x020192E4 */
extern struct DuelCmd gDuelCmd;             /* 0x020185C0 */

void CopyDuelCard(u32 *dst, u32 *src);
void ClearCardStatusFlags(struct DuelCardStatusBytes *card);
void PlaceSpellTrapCard(int player, int slot, struct DuelCard *card, u16 faceUp);
void AddCardToDeckTop(int player, struct DuelCard *card);
void AddCardToDeckBottom(int player, struct DuelCard *card);
int RemoveCardFromFusionDeck(int player, struct DuelCard *card);
void AddCardToGraveyard(struct DuelCard *card);
void AddCardToBanished(struct DuelCard *card);
int TakeGraveyardCardAt(int player, int index, struct DuelCard *out);
u16 RemoveCardFromGraveyard(int player, struct DuelCard *card);
u16 RemoveGraveyardCardById(int player, u16 cardId);
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out);
u16 RemoveCardFromBanished(int player, struct DuelCard *card);
void AddCardToHand(int player, struct DuelCard *card);
void CompactHand(int player);
int FindFreeSpellTrapZone(int player);
void AddZoneLink(u16 loc, u16 target, u16 kind);

void DuelScreen_StartScroll(u32 target);
void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelCursor_Select(s32 player, s32 area, s32 index);
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
void DrawAllAreaTiles(void);
/* ---- END duel.h / duel_cmd.h / duel_screen.h subset ---- */

/* Acting player of the current command (bit 15 of cmd, read as a whole halfword and shifted). */
#define CMD_PLAYER()    (gDuelCmd.cmd >> 15)
/* The card word that arg2 (low half) and arg4 (high half) carry. */
#define CMD_CARD_WORD() ((gDuelCmd.arg4 << 16) | gDuelCmd.arg2)
/* Owner bit (12) of a card word held in a u32. */
#define CARD_WORD_OWNER(word)   (((word) << 19) >> 31)

/*
 * The card the command moves, as a pointer. Matching: reading a field through the pointer loads the whole
 * word (ldr; lsl #20 for the ID); the member access gDuelCmd.card.id loads a halfword.
 */
#define CMD_CARD        (&gDuelCmd.card)

/*
 * Card tables read through integer-constant addresses. Matching: DuelCmd_ReturnGraveyardCardToHand only
 * matches with this form (card_data.h's symbols load the table address at another point).
 */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber[id] */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats[id] */

/*
 * Card kind (enum CardKind) of a card ID, the frame the card is drawn with: the Egyptian God cards by
 * number, Magic/Trap/Ticket cards by type, monsters by the kind bits of their stats. The same inline is
 * repeated in several units.
 */
static inline int GetCardKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_STATS_TYPE(CARD_STATS_WORD(id))) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_WORD(id));
    }
}

/* DUEL_CMD_ADD_CARD_TO_DECK_TOP (0x6A), one frame: put the card word on top of its owner's deck. */
void DuelCmd_AddCardToDeckTop(void)
{
    u32 cardWord = CMD_CARD_WORD();

    AddCardToDeckTop(CARD_WORD_OWNER(cardWord), (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/* DUEL_CMD_ADD_CARD_TO_DECK_BOTTOM (0x6B), one frame: put the card word at the bottom of its owner's deck. */
void DuelCmd_AddCardToDeckBottom(void)
{
    u32 cardWord = CMD_CARD_WORD();

    AddCardToDeckBottom(CARD_WORD_OWNER(cardWord), (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_SET_CRUSH_CARD_TURNS (0x69), one frame: the acting player's crushCardTurns = arg2. Crush Card
 * sends 3 for the opponent; while it is nonzero, drawn monsters with ATK 1500 or more go to the graveyard.
 */
void DuelCmd_SetCrushCardTurns(void)
{
    struct DuelCmd *cmd = &gDuelCmd;

    gDuelPlayers[cmd->cmd >> 15].crushCardTurns = cmd->arg2;
    cmd->running = 0;
}

/* DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK (0xDC), one frame: remove the card word from the acting player's
 * fusion deck (Polymerization, after its materials). */
void DuelCmd_RemoveCardFromFusionDeck(void)
{
    u32 player = CMD_PLAYER();
    u32 cardWord = CMD_CARD_WORD();

    RemoveCardFromFusionDeck(player, (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND (0xD2): remove the card word from the acting player's graveyard,
 * clear its status flags and add it to the hand (AddCardToHand sends a Fusion monster to the fusion deck).
 * The animation (its card ID, its target player and the Fusion test that retargets it to the fusion deck)
 * reads gDuelCmd.card, which this handler never writes, so it uses the card the previous command saved
 * there. The pile updates use the card word of the operands.
 */
void DuelCmd_ReturnGraveyardCardToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u32 cardWord = CMD_CARD_WORD();

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromGraveyard(player, (struct DuelCard *)&cardWord);
        from.player = player;
        from.area = DUEL_AREA_GRAVEYARD;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelPlayers[player & 1].handCount;
        to.isDefense = 0;
        to.isFaceUp = 0;
        if (GetCardKind(CMD_CARD->id) == CARD_KIND_FUSION) {
            to.area = DUEL_AREA_FUSION_DECK;
            to.index = 0;
        }
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        ClearCardStatusFlags((struct DuelCardStatusBytes *)&cardWord);
        AddCardToHand(player, (struct DuelCard *)&cardWord);
        DuelCursor_Select(player, DUEL_AREA_HAND, 0);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP (0xD0): find the first card with ID arg2 in the acting
 * player's graveyard (copied to gDuelCmd.card; the command ends if there is none), remove it, animate it to
 * the deck, clear its status flags and put it on top. Horn of Light and Malevolent Nuzzler send it with
 * their own ID after their 500 LP payment.
 */
void DuelCmd_ReturnGraveyardCardToDeckTop(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = cmd->cmd >> 15;
    u32 cardId = cmd->arg2;
    struct DuelLoc from, to;

    switch (cmd->step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        cmd->step++;
        break;
    case 1:
        if (GetGraveyardCardById(player, cardId, &cmd->card)) {
            RemoveCardFromGraveyard(player, &cmd->card);
            from.player = player;
            from.area = DUEL_AREA_GRAVEYARD;
            from.index = 0;
            from.isDefense = 0;
            from.isFaceUp = 1;
            to.player = player;
            to.area = DUEL_AREA_DECK;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 0;
            DuelAnim_MoveCard((&cmd->card)->id, &from, &to);     /* through the pointer, as CMD_CARD */
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        ClearCardStatusFlags((struct DuelCardStatusBytes *)&cmd->card);
        AddCardToDeckTop(player, &cmd->card);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_DECK, 0);
        cmd->running = 0;
        break;
    }
}

/* DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_BOTTOM (0xD1): DuelCmd_ReturnGraveyardCardToDeckTop, but the card
 * goes to the bottom of the deck. */
void DuelCmd_ReturnGraveyardCardToDeckBottom(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = cmd->cmd >> 15;
    u32 cardId = cmd->arg2;
    struct DuelLoc from, to;

    switch (cmd->step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        cmd->step++;
        break;
    case 1:
        if (GetGraveyardCardById(player, cardId, &cmd->card)) {
            RemoveCardFromGraveyard(player, &cmd->card);
            from.player = player;
            from.area = DUEL_AREA_GRAVEYARD;
            from.index = 0;
            from.isDefense = 0;
            from.isFaceUp = 1;
            to.player = player;
            to.area = DUEL_AREA_DECK;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 0;
            DuelAnim_MoveCard((&cmd->card)->id, &from, &to);     /* through the pointer, as CMD_CARD */
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        ClearCardStatusFlags((struct DuelCardStatusBytes *)&cmd->card);
        AddCardToDeckBottom(player, &cmd->card);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_DECK, 0);
        cmd->running = 0;
        break;
    }
}

/*
 * DUEL_CMD_BANISH_GRAVEYARD_CARD (0xD4): remove the card word from the acting player's graveyard, animate it
 * (face up) to the banished pile and add it there.
 */
void DuelCmd_BanishGraveyardCard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u32 cardWord = CMD_CARD_WORD();

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromGraveyard(player, (struct DuelCard *)&cardWord);
        from.player = player;
        from.area = DUEL_AREA_GRAVEYARD;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = player;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToBanished((struct DuelCard *)&cardWord);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_BANISHED, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD (0xD3), one frame: remove the card word from its owner's graveyard
 * (no animation) and redraw the piles. */
void DuelCmd_RemoveCardFromGraveyard(void)
{
    u32 cardWord = CMD_CARD_WORD();

    RemoveCardFromGraveyard(CARD_WORD_OWNER(cardWord), (struct DuelCard *)&cardWord);
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_RETURN_GRAVEYARD_TO_DECK (0xD6, Penguin Knight): steps 1 and 2 loop, one card per animation,
 * moving graveyard[0] to the top of the deck until the graveyard is empty; then step 10 (default) puts the
 * cursor on the deck. The deck shuffle is a separate command queued after this one.
 */
void DuelCmd_ReturnGraveyardToDeck(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelPlayers[player & 1].graveCount != 0) {
            TakeGraveyardCardAt(player, 0, CMD_CARD);
            from.player = player;
            from.area = DUEL_AREA_GRAVEYARD;
            from.index = 0;
            from.isDefense = 0;
            from.isFaceUp = 1;
            to.player = player;
            to.area = DUEL_AREA_DECK;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 0;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
        } else {
            gDuelCmd.step = 10;
        }
        break;
    case 2:
        ClearCardStatusFlags((struct DuelCardStatusBytes *)CMD_CARD);
        AddCardToDeckTop(player, CMD_CARD);
        DrawAllAreaTiles();
        gDuelCmd.step = 1;  /* next card */
        break;
    default:
        DuelCursor_Select(player, DUEL_AREA_DECK, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_TAKE_OPPONENT_GRAVEYARD_CARD (0xD5, Graverobber): take the first card with ID arg2 out of the
 * opponent's graveyard, animate it into the acting player's hand and add it there with its graverobbed bit
 * set (activating it then costs 2000 LP).
 */
void DuelCmd_TakeOpponentGraveyardCard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u32 opponent = 1 - player;
    u16 cardId = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(opponent, DUEL_AREA_GRAVEYARD);
        gDuelCmd.step++;
        break;
    case 1:
        GetGraveyardCardById(opponent, cardId, CMD_CARD);
        RemoveGraveyardCardById(opponent, cardId);
        from.player = opponent;
        from.area = DUEL_AREA_GRAVEYARD;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = player;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelPlayers[player & 1].handCount;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        CMD_CARD->graverobbed = 1;
        AddCardToHand(player, CMD_CARD);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_BANISHED_CARD_TO_GRAVEYARD (0xDE): scroll the field to the middle (80), remove the card
 * word from its owner's banished pile, animate it (face up) to the graveyard and add it there.
 */
void DuelCmd_ReturnBanishedCardToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 cardWord = CMD_CARD_WORD();
    u32 owner = CARD_WORD_OWNER(cardWord);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_StartScroll(80);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromBanished(owner, (struct DuelCard *)&cardWord);
        from.player = owner;
        from.area = DUEL_AREA_BANISHED;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = owner;
        to.area = DUEL_AREA_GRAVEYARD;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToGraveyard((struct DuelCard *)&cardWord);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_ADD_CARD_TO_GRAVEYARD_NO_REDRAW (0xD7), one frame: add the card word to its owner's graveyard
 * (Painful Choice, for the cards not picked); the piles are not redrawn. */
void DuelCmd_AddCardToGraveyardNoRedraw(void)
{
    u32 cardWord = CMD_CARD_WORD();

    AddCardToGraveyard((struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/* DUEL_CMD_CLEAR_PENDING_EQUIP (0xD8), one frame: clear pendingEquip of the acting player's graveyard[arg2]. */
void DuelCmd_ClearPendingEquip(void)
{
    struct DuelCmd *cmd = &gDuelCmd;

    gDuelPlayers[cmd->cmd >> 15].graveyard[cmd->arg2].pendingEquip = 0;
    cmd->running = 0;
}

/*
 * DUEL_CMD_EQUIP_GRAVEYARD_CARD_TO_OPPONENT (0xD9): take graveyard[arg2] of the acting player, place it face
 * up in the opponent's first free spell/trap zone (kept in gDuelCmd.counter between the steps) and link it
 * as a +200 ATK equip to the opponent's monster in the card's equipZone. Driven by the end-of-turn scan for
 * card number 1327, which is not in EDS.
 */
void DuelCmd_EquipGraveyardCardToOpponent(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 index = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        TakeGraveyardCardAt(player, index, CMD_CARD);
        gDuelCmd.card.pendingEquip = 0;
        gDuelCmd.counter = FindFreeSpellTrapZone(1 - player);   /* ZONE_SPELL_0..4 */
        from.player = player;
        from.area = DUEL_AREA_GRAVEYARD;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = 1 - player;
        to.area = DUEL_AREA_SPELL_TRAP;
        /* The zone number 5-9, where other handlers pass the index 0-4 within the row (as in the ROM). */
        to.index = gDuelCmd.counter;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        PlaceSpellTrapCard(1 - player, gDuelCmd.counter - ZONE_SPELL_0, CMD_CARD, TRUE);
        /* DUEL_LOC(opponent, zone) for both, written player first. Matching: the operand order decides
         * which half is computed first. */
        AddZoneLink((u8)(1 - player) | (gDuelCmd.card.equipZone << 8),
                    (u8)(1 - player) | (gDuelCmd.counter << 8), ZONE_LINK_EQUIP_ATK_200);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON (0xDA), one frame: clear pendingOpponentSummon of the acting
 * player's graveyard[arg2] (the opponent had no free zone or declined). */
void sub_080106BC(void)
{
    struct DuelCmd *cmd = &gDuelCmd;

    gDuelPlayers[cmd->cmd >> 15].graveyard[cmd->arg2].pendingOpponentSummon = 0;
    cmd->running = 0;
}

/* DUEL_CMD_MARK_GRAVEYARD_CARD (0xDB), one frame: set flag23 of the first entry of the acting player's
 * graveyard whose whole word equals the card word. */
void sub_08010708(void)
{
    u32 *entry;
    u32 cardWord = CMD_CARD_WORD();
    int i;
    s16 player = CMD_PLAYER();

    for (i = 0; i < gDuelPlayers[player].graveCount; i++) {
        entry = (u32 *)&gDuelPlayers[player].graveyard[i];
        if (*entry == cardWord) {
            gDuelPlayers[player].graveyard[i].flag23 = 1;
            gDuelCmd.running = 0;
            return;
        }
    }
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD (0xC0): copy hand[arg2] of the acting player to gDuelCmd.card and
 * clear its ID (a hole in the hand), animate it to its owner's graveyard and add it there. arg4 != 0
 * compacts the hand first; otherwise the caller queues DUEL_CMD_COMPACT_HAND later.
 */
void DuelCmd_SendHandCardToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_GRAVEYARD;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToGraveyard(CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_BANISH_HAND_CARD (0xC1): DuelCmd_SendHandCardToGraveyard, but to the owner's banished pile (used
 * instead of 0xC0 while Banisher of the Light is on the field). */
void DuelCmd_BanishHandCard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToBanished(CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_HAND_CARD_TO_DECK (0xC3): move hand[arg2] of the acting player (face down) to its owner's
 * deck, then compact the hand and put the card on top (arg4 != 0) or at the bottom of the deck.
 */
void DuelCmd_ReturnHandCardToDeck(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_DECK;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        if (gDuelCmd.arg4 != 0)
            AddCardToDeckTop(player, CMD_CARD);
        else
            AddCardToDeckBottom(player, CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
