#include "global.h"

/*
 * Duel "script command" handlers (continued from code_0800EAA8). The
 * dispatcher around 0x0801F3xx calls one of these per frame; each reads its
 * operands from the command block at 0x020185C0 and clears the "command
 * running" flag (bit 5 of byte 0x020185C0+0x80D) when it is done. Multi-frame
 * handlers keep their state in the 7-bit `step` counter at +0x80A.
 * See wiki/functions/code-0800fb10.md.
 */

/* A card instance word as stored in the duel state. */
struct DuelCard {
    u32 id:12;          /* bits 0-11: card ID; 0 = none */
    u32 owner:1;        /* bit 12: owning player */
    u32 unk13:5;
    u32 flag18:1;       /* bit 18 */
    u32 unk19:4;
    u32 flag23:1;       /* bit 23 */
    u32 flag24:1;       /* bit 24 */
    u32 unk25:3;
    u32 flag28:1;       /* bit 28 */
    u32 unk29:3;
};

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002 */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 slot:7;         /* 0x80A bits 7-13: a zone chosen by a handler (DuelCmd_EquipGraveyardCardToOpponent) */
    u16 unk80A_14:2;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 filler80E[0x814 - 0x80E];
    struct DuelCard card;   /* 0x814: card being moved */
};

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 numList684;          /* +0x002 */
    u8 unk3;
    u8 numList904;          /* +0x004 */
    u8 unk5[6];
    u8 unkB_0:3;            /* +0x00B bits 0-2 */
    u8 unkB_3:5;
    u8 fillerC[0x684 - 0xC];
    struct DuelCard list684[160];   /* +0x684 */
    struct DuelCard list904[160];   /* +0x904 */
    u8 fillerB84[0xD64 - 0xB84];
};

/* Card location descriptor passed to the card-move animation DuelAnim_MoveCard. */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4: 11 hand, 13, 14, 15 (hypothesis: graveyard-like areas) */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;       /* bit 14 */
    u16 flag15:1;       /* bit 15 */
    u16 unk2;
};

extern struct DuelCmd gDuelCmd;
extern struct DuelPlayer gDuelPlayers[2];
extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */

/* Acting player of the current command (bit 15; read as a plain u16 shift). */
#define CMD_PLAYER() (gDuelCmd.cmd >> 15)
#define CARD_OWNER(w) (((w) << 19) >> 31)

void AddCardToGraveyard(void *card);
void AddCardToDeckTop(u32 player, struct DuelCard *card);
void AddCardToDeckBottom(u32 player, struct DuelCard *card);
void RemoveCardFromFusionDeck(u32 player, struct DuelCard *card);
u16 RemoveCardFromGraveyard(int player, struct DuelCard *card);
void DrawAllAreaTiles(void);
void DuelScreen_StartScroll(u32);
void RemoveCardFromBanished(u32 player, struct DuelCard *card);
void DuelAnim_MoveCard(u32 id, struct CardLoc *from, struct CardLoc *to);
void DuelScreen_ScrollToZone(u32, u32);
void DuelCursor_Select(u32, u32, u32);
void AddCardToBanished(void *card);
u32 GetGraveyardCardById(u32 player, u32 idx, struct DuelCard *card);
void RemoveGraveyardCardById(u32 player, u32 idx);
void AddCardToHand(u32 player, struct DuelCard *card);
void ClearCardStatusFlags(struct DuelCard *card);

/* Card of the command block through a pointer, so id reads load the whole word. */
#define CMD_CARD ((struct DuelCard *)((u8 *)&gDuelCmd + 0x814))

void DuelCmd_AddCardToDeckTop(void)
{
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    AddCardToDeckTop(CARD_OWNER(card), (struct DuelCard *)&card);
    gDuelCmd.running = 0;
}

void DuelCmd_AddCardToDeckBottom(void)
{
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    AddCardToDeckBottom(CARD_OWNER(card), (struct DuelCard *)&card);
    gDuelCmd.running = 0;
}

void DuelCmd_SetCrushCardTurns(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    gDuelPlayers[cmd->cmd >> 15].unkB_0 = cmd->arg2;
    cmd->running = 0;
}
void DuelCmd_RemoveCardFromFusionDeck(void)
{
    u32 player = CMD_PLAYER();
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    RemoveCardFromFusionDeck(player, (struct DuelCard *)&card);
    gDuelCmd.running = 0;
}

#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_KIND(id) ((CARD_STATS(id) & 0xC0000) >> 18)

/* Card category (same inline as in code_08009A68): 3/1 for card numbers 1910/1911-1912,
 * 7/8/9 for types 22/21/23, else the monster kind (stats bits 18-19). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return CARD_KIND(id);
    }
}

/*
 * Returns card word arg2 | arg4 << 16 of the acting player from area 14 to
 * the hand, or to area 12 when its category is 2 (a fusion monster; hypothesis:
 * area 12 is the fusion deck). Then calls ClearCardStatusFlags, AddCardToHand and
 * DuelCursor_Select(player, 11, 0).
 */
void DuelCmd_ReturnGraveyardCardToHand(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromGraveyard(player, (struct DuelCard *)&card);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 11;
        to.index = gDuelPlayers[player & 1].numList684;
        to.flag14 = 0;
        to.flag15 = 0;
        if (GetCardSubtype(CMD_CARD->id) == 2) {
            to.area = 12;
            to.index = 0;
        }
        DuelAnim_MoveCard((&gDuelCmd.card)->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        ClearCardStatusFlags((struct DuelCard *)&card);
        AddCardToHand(player, (struct DuelCard *)&card);
        DuelCursor_Select(player, 11, 0);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Takes card arg2 of the acting player's area 14 list (GetGraveyardCardById; stops if
 * it returns 0), animates it to area 13, then calls ClearCardStatusFlags, AddCardToDeckTop and
 * DuelCursor_Select(player, 13, 0).
 */
void DuelCmd_ReturnGraveyardCardToDeckTop(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    u32 idx;
    u32 player = cmd->cmd >> 15;
    struct CardLoc from, to;

    /* This empty compiler barrier keeps the player load before arg2, as in
     * the ROM. It emits no instructions and leaves cmd and player unchanged. */
    asm volatile ("" : "+r"(cmd) : "r"(player));
    idx = cmd->arg2;

    switch (cmd->step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        cmd->step++;
        break;
    case 1:
        if (GetGraveyardCardById(player, idx, ((struct DuelCard *)((u8 *)cmd + 0x814)))) {
            RemoveCardFromGraveyard(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            DuelAnim_MoveCard(((struct DuelCard *)((u8 *)cmd + 0x814))->id, &from, &to);
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        ClearCardStatusFlags(((struct DuelCard *)((u8 *)cmd + 0x814)));
        AddCardToDeckTop(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
        DrawAllAreaTiles();
        DuelCursor_Select(player, 13, 0);
        cmd->running = 0;
        break;
    }
}
/* Same as DuelCmd_ReturnGraveyardCardToDeckTop but places the card with AddCardToDeckBottom. */
void DuelCmd_ReturnGraveyardCardToDeckBottom(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    u32 idx;
    u32 player = cmd->cmd >> 15;
    struct CardLoc from, to;

    /* This empty compiler barrier keeps the player load before arg2, as in
     * the ROM. It emits no instructions and leaves cmd and player unchanged. */
    asm volatile ("" : "+r"(cmd) : "r"(player));
    idx = cmd->arg2;

    switch (cmd->step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        cmd->step++;
        break;
    case 1:
        if (GetGraveyardCardById(player, idx, ((struct DuelCard *)((u8 *)cmd + 0x814)))) {
            RemoveCardFromGraveyard(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            DuelAnim_MoveCard(((struct DuelCard *)((u8 *)cmd + 0x814))->id, &from, &to);
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        ClearCardStatusFlags(((struct DuelCard *)((u8 *)cmd + 0x814)));
        AddCardToDeckBottom(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
        DrawAllAreaTiles();
        DuelCursor_Select(player, 13, 0);
        cmd->running = 0;
        break;
    }
}
/*
 * Moves card word arg2 | arg4 << 16 of the acting player from area 14 to
 * area 15 (RemoveCardFromGraveyard, animation), then calls AddCardToBanished and DuelCursor_Select(player, 15, 0).
 */
void DuelCmd_BanishGraveyardCard(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromGraveyard(player, (struct DuelCard *)&card);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = player;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToBanished(&card);
        DrawAllAreaTiles();
        DuelCursor_Select(player, 15, 0);
        gDuelCmd.running = 0;
        break;
    }
}
void DuelCmd_RemoveCardFromGraveyard(void)
{
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    RemoveCardFromGraveyard(CARD_OWNER(card), (struct DuelCard *)&card);
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

void TakeGraveyardCardAt(u32 player, u32 idx, struct DuelCard *card);

/*
 * Loop: while the acting player's list904 (area 14) is not empty, take entry
 * 0 (TakeGraveyardCardAt), animate it to area 13 and commit it (ClearCardStatusFlags +
 * AddCardToDeckTop); then DuelCursor_Select(player, 13, 0).
 */
void DuelCmd_ReturnGraveyardToDeck(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelPlayers[player & 1].numList904 != 0) {
            TakeGraveyardCardAt(player, 0, CMD_CARD);
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
        } else {
            gDuelCmd.step = 10;
        }
        break;
    case 2:
        ClearCardStatusFlags(CMD_CARD);
        AddCardToDeckTop(player, CMD_CARD);
        DrawAllAreaTiles();
        gDuelCmd.step = 1;
        break;
    default:
        DuelCursor_Select(player, 13, 0);
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Takes card arg2 of the opponent's area 14 list (GetGraveyardCardById / RemoveGraveyardCardById)
 * and animates it into the acting player's hand, then sets card bit 18 and
 * commits with AddCardToHand.
 */
void DuelCmd_TakeOpponentGraveyardCard(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 other = 1 - player;
    u16 idx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(other, 14);
        gDuelCmd.step++;
        break;
    case 1:
        GetGraveyardCardById(other, idx, CMD_CARD);
        RemoveGraveyardCardById(other, idx);
        from.player = other;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = player;
        to.area = 11;
        to.index = gDuelPlayers[player & 1].numList684;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        CMD_CARD->flag18 = 1;
        AddCardToHand(player, CMD_CARD);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Moves card word arg2 | arg4 << 16 from its owner's area 15 to area 14
 * (hypothesis: from the graveyard to removed from play): RemoveCardFromBanished, animation,
 * then AddCardToGraveyard.
 */
void DuelCmd_ReturnBanishedCardToGraveyard(void)
{
    struct CardLoc from, to;
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    u32 owner = CARD_OWNER(card);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_StartScroll(0x50);
        gDuelCmd.step++;
        break;
    case 1:
        RemoveCardFromBanished(owner, (struct DuelCard *)&card);
        from.player = owner;
        from.area = 15;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = owner;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToGraveyard(&card);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
void DuelCmd_AddCardToGraveyardNoRedraw(void)
{
    u32 w = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    AddCardToGraveyard((void *)&w);
    gDuelCmd.running = 0;
}
void DuelCmd_ClearPendingEquip(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    gDuelPlayers[cmd->cmd >> 15].list904[cmd->arg2].flag24 = 0;
    cmd->running = 0;
}

void PlaceSpellTrapCard(u32 player, u32 slot, struct DuelCard *card, u32 a3);
void AddZoneLink(u32 a, u32 b, u32 c);
u32 FindFreeSpellTrapZone(u32 player);

/*
 * Takes entry arg2 of the acting player's area 14 list (TakeGraveyardCardAt) and
 * places it face-up (flag24 cleared) into a spell/trap zone of the opponent
 * chosen by FindFreeSpellTrapZone (zone - 5 passed to PlaceSpellTrapCard).
 */
void DuelCmd_EquipGraveyardCardToOpponent(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 idx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        TakeGraveyardCardAt(player, idx, CMD_CARD);
        gDuelCmd.card.flag24 = 0;
        gDuelCmd.slot = FindFreeSpellTrapZone(1 - player);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = 1 - player;
        to.area = 5;
        to.index = gDuelCmd.slot;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        PlaceSpellTrapCard(1 - player, gDuelCmd.slot - 5, CMD_CARD, 1);
        AddZoneLink((u8)(1 - player) | (gDuelCmd.card.unk25 << 8),
                     (u8)(1 - player) | (gDuelCmd.slot << 8), 10);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
void sub_080106BC(void)
{
    struct DuelCmd *cmd = &gDuelCmd;
    gDuelPlayers[cmd->cmd >> 15].list904[cmd->arg2].flag28 = 0;
    cmd->running = 0;
}
void sub_08010708(void)
{
    u32 *entry;
    u32 card = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    int i = 0;
    s16 player = CMD_PLAYER();

    for (; i < gDuelPlayers[player].numList904; i++) {
        entry = (u32 *)&gDuelPlayers[player].list904[i];
        if (*entry == card) {
            gDuelPlayers[player].list904[i].flag23 = 1;
            gDuelCmd.running = 0;
            return;
        }
    }
    gDuelCmd.running = 0;
} /* 0x08010708 size 0x8C */

void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void CompactHand(u32 player);

/*
 * Moves hand card arg2 of the acting player to its owner's area 14 (like
 * DuelCmd_SendHandFusionMaterialToGraveyard without the flag20 update); committed with AddCardToGraveyard.
 * arg4 != 0 also calls CompactHand first.
 */
void DuelCmd_SendHandCardToGraveyard(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].list684 + handIdx);
        (gDuelPlayers[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToGraveyard(CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/* Like DuelCmd_SendHandCardToGraveyard but to the owner's area 15, committed with AddCardToBanished. */
void DuelCmd_BanishHandCard(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].list684 + handIdx);
        (gDuelPlayers[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToBanished(CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Hand card arg2 of the acting player to its owner's area 13, then
 * AddCardToDeckTop (arg4 != 0) or AddCardToDeckBottom to place it.
 */
void DuelCmd_ReturnHandCardToDeck(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].list684 + handIdx);
        (gDuelPlayers[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 13;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        if (gDuelCmd.arg4 != 0)
            AddCardToDeckTop(player, CMD_CARD);
        else
            AddCardToDeckBottom(player, CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
