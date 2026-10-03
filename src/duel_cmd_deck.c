#include "global.h"
#include "duel.h"

/*
 * Duel "script command" handlers. The dispatcher DuelCmd_Dispatch switches on
 * (gDuelCmd.cmd & 0xFFF) - 1 and calls one of these; each handler reads
 * its operands from the command block at 0x020185C0 and clears the
 * "command running" flag (bit 5 of byte 0x020185C0+0x80D) when it is done.
 */

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002: usually a zone/slot index */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 counter:7;      /* 0x80A bits 7-13 */
    u16 unk80A_14:2;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 filler80E[0x814 - 0x80E];
    u8 card[4];         /* 0x814 struct DuelCard: card being moved (u8 so the block stays 2-aligned) */
};

/* Card location for the card-move animation DuelAnim_MoveCard (4 bytes; see duel_cmd_hand). */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4: 0 monster zone, 5 spell/trap, 10 field, 11 hand, 13, 14, 15 */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

/*
 * Local views kept because the canonical declarations in duel.h differ from what
 * this unit needs to match (duel.h is shared, so it is not changed here):
 *
 * - struct DuelZoneWord4 (below): canonical struct DuelZone declares +0x04 as `u16
 *   serial` and +0x06 as the u8 bitfields flag6_0/flag6_1/counter6; this unit reads and
 *   writes +0x04 as one u32 bitfield container (its counter sits at bits 18-21, i.e.
 *   +0x06 bits 2-5), so DuelCmd_AddZoneTurnCounter and DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue keep this view.
 * - struct DuelCardBit22 (below): canonical struct DuelCard has flag20 at bit 20 and
 *   unk21:11 for bits 21-31, so the bit-22 flag DuelCmd_SetDestroyedByOpponentFlag writes has no canonical
 *   field name.
 * - struct DuelZone90 (below): canonical struct DuelZone stops at +0x8C (unk8C[8]), but
 *   DuelCmd_SetZoneDeclaredValue and DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue write the u32 bitfield at +0x90 bits 13-17.
 */
struct DuelZoneWord4 {
    u8 filler[4];
    u32 unk4_0:18;
    u32 unk4_18:4;      /* 0x06 bits 2-5 (canonical counter6) */
    u32 unk4_22:10;
};

struct DuelCardBit22 {
    u32 unk0_0:22;      /* 0x00 bits 0-21: card id and flags */
    u32 flag0_22:1;     /* 0x02 bit 6 */
    u32 unk0_23:9;
};

struct DuelZone90 {
    u8 filler[0x90];
    u32 unk90_0:13;
    u32 unk90_13:5;     /* 0x90 bits 13-17 */
    u32 unk90_18:14;
};

/* struct DuelPlayerFlags: canonical struct DuelPlayer declares +0x07 as the bitfields
 * deckOut/winA/...; DuelCmd_DrawCards sets it with a plain `|= 1` (ldrb/orrs/strb), so a
 * whole-byte view matches. */
struct DuelPlayerFlags {
    u8 filler[7];
    u8 flags7;
};

extern struct DuelCmd gDuelCmd;

#define CMD_PLAYER (gDuelCmd.cmd >> 15)
#define CMD_CARD ((struct DuelCard *)gDuelCmd.card)
/* Halfword at cmd +0x80C seen as bitfields (padded past 4 bytes so it is accessed with ldrh/strh). */
struct Cmd80C {
    u16 unk0:5;
    u16 unk5:7;         /* bits 5-11: cleared by DuelCmd_DrawCards (hypothesis: animation counter) */
    u16 unk12:4;
    u8 pad[4];
};
#define CMD_80C (*(struct Cmd80C *)&gDuelCmd.unk80C)
/* card id of a zone: low 12 bits of its first word */
#define ZONE_CARD_ID(z) ((*(u32 *)(z) << 20) >> 20)
#define ZONE_CARD(p, s) ZONE_CARD_ID(ZONE(p, s))
#define ZONE(p, s) ((struct DuelZone *)((s) * 0x94 + (p) * 0xD64 + (u8 *)gDuelZones))

void AddZoneLink(u16, u16, u16);
void RemoveZoneLink(u16, u16, u16);
void DuelScreen_DrawCursorInfo(void);
int RemoveCardFromDeck(u32 player, void *card);
void CopyDuelCard(struct DuelCard *dst, void *src);
void AddCardToGraveyard(struct DuelCard *card);
int TakeDeckCardAt(u32 player, u32 a, struct DuelCard *card);
void PlaceMonsterCard(u32 player, u32 zone, struct DuelCard *card, u32 a, u32 b);
void AddCardToBanished(struct DuelCard *card);
void DuelDrawCard(u32 player, u16 a);
void AddCardToHand(u32 player, void *card);
int RemoveCardFromFusionDeck(u32 player, void *card);
void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelCursor_Select(u32 player, u32 area, u32 index);
void DuelAnim_MoveCard(u32 id, struct CardLoc *from, struct CardLoc *to);
void DrawAllAreaTiles(void);
void DuelScreen_StartScroll(u32 a);
void DuelSprAnim_Load(const void *a);
void ShuffleDeck(u32 player, u32 a);
void DuelSprAnim_DrawAt(u32 a, u32 b, u32 c);
void PlaySE(u16 se);                        /* PlaySE */
void DuelSprAnim_Rewind(void);
void LoadDuelUiGfx(void);
void DuelLink_SendDeck(u32 player);
extern const u8 gDeckShuffleAnim[];
/* Duel screen state at 0x0201CFB0 (see duel_cmd_moves); only the field used here. */
struct DuelScreen {
    u8 filler0[0x852];
    u16 unk852;         /* 0x852: nonzero while busy (hypothesis) */
};
extern struct DuelScreen gDuelScreen;
extern u8 gDuelCtrl[];      /* byte 1 bit 0: link duel (hypothesis) */
/* Link state at 0x02017FB0 (see duel_prompts; u32 bitfield containers). */
struct Unk02017FB0 {
    u8 filler0[0x304];
    u32 unk304:8;
    u32 dirtyHand:1;    /* +0x305 bit 0 (hypothesis, per duel_prompts) */
    u32 dirtyDeck:1;    /* +0x305 bit 1 */
    u32 unk305_2:22;
};
extern struct Unk02017FB0 gLinkState;
void MemCopy16(void *dst, const void *src, u32 size);

void DuelCmd_AddEquipLink(void)
{
    AddZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, 1);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}
void DuelCmd_AddZoneLink(void)
{
    AddZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, gDuelCmd.arg6);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}
void DuelCmd_RemoveZoneLink(void)
{
    RemoveZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, gDuelCmd.arg6);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}
void DuelCmd_SetZoneDeclaredValue(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, (u8)gDuelCmd.arg2);
    ((struct DuelZone90 *)zone)->unk90_13 = gDuelCmd.arg4;
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}
void DuelCmd_SetDestroyedByOpponentFlag(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gDuelCmd.arg2);
    ((struct DuelCardBit22 *)zone)->flag0_22 = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}
void DuelCmd_AddZoneTurnCounter(void)
{
    u16 val = gDuelCmd.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gDuelCmd.arg2);
    if (ZONE_CARD_ID(zone))
        ((struct DuelZoneWord4 *)zone)->unk4_18 = val + ((struct DuelZoneWord4 *)zone)->unk4_18;
    gDuelCmd.running = 0;
}
void DuelCmd_SetZoneTurnCounter(void)
{
    u16 val = gDuelCmd.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gDuelCmd.arg2);
    u8 old = ((struct DuelZoneWord4 *)zone)->unk4_18;
    if (ZONE_CARD_ID(zone))
        ((struct DuelZoneWord4 *)zone)->unk4_18 = val;
    gDuelCmd.running = 0;
}
void DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gDuelCmd.arg2);
    if (ZONE_CARD_ID(zone)) {
        ((struct DuelZoneWord4 *)zone)->unk4_18 = 0;
        ((struct DuelZone90 *)zone)->unk90_13 = gDuelCmd.arg4;
    }
    gDuelCmd.running = 0;
}
void DuelCmd_ClearZoneLinks(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gDuelCmd.arg2);
    zone->numLinks = 0;
    gDuelCmd.running = 0;
}
void DuelCmd_MoveZoneLinks(void)
{
    struct DuelZone *base = (struct DuelZone *)((u8 *)gDuelZones + CMD_PLAYER * 0xD64);
    struct DuelZone *src = base + gDuelCmd.arg2;
    struct DuelZone *dst;

    __asm__("" : : : "r6");
    dst = base + gDuelCmd.arg4;
    MemCopy16(dst->links, src->links, 0x40);
    MemCopy16(dst->linkKinds, src->linkKinds, 0x40);
    dst->numLinks = src->numLinks;
    src->numLinks = 0;
    gDuelCmd.running = 0;
}
void DuelCmd_AddProhibition(void)
{
    if (gDuelCmd.arg4 != 0) {
        gDuel.queueZone[gDuel.queueCount] = ((u8)gDuelCmd.arg2 << 8) | CMD_PLAYER;
        gDuel.queueArg[gDuel.queueCount] = gDuelCmd.arg4;
        gDuel.queueCount++;
    }
    gDuelCmd.running = 0;
}
/*
 * Remove the marked-card queue entries whose zone word equals (arg2 << 8) | player.
 * The (u16) cast on the key is what makes the else arm recompute arg2 << 8
 * (combine splits the zero-extension off the shared shift).
 */
void DuelCmd_RemoveProhibition(void)
{
    s32 i, j;
    for (i = 0; i < gDuel.queueCount; i++) {
        /* FAKEMATCH: the ROM keeps the 0x8000 mask and the AND inside the loop (only the
           cmd load is hoisted); the empty asm volatile stops loop.c from hoisting the
           constant in its second pass. */
        if (gDuel.queueZone[i]
            == (u16)(((u8)gDuelCmd.arg2 << 8)
                     | ((gDuelCmd.cmd & ({ int mask = 0x8000; asm volatile("" : "+r"(mask)); mask; })) ? 1 : 0))) {
            gDuel.queueCount = (u16)(gDuel.queueCount - 1);
            for (j = i; j < gDuel.queueCount; j++) {
                gDuel.queueZone[j] = gDuel.queueZone[j + 1];
                gDuel.queueArg[j] = gDuel.queueArg[j + 1];
            }
        }
    }
    gDuelCmd.running = 0;
}
void DuelCmd_ShuffleDeck(void)
{
    int player = CMD_PLAYER;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_StartScroll(0x50);
        gDuelCmd.step++;
        break;
    case 1:
        DuelSprAnim_Load(gDeckShuffleAnim);
        gDuelCmd.counter = 0;
        gDuelCmd.step++;
        /* fallthrough */
    case 2:
        ShuffleDeck(player, 3);
        DuelSprAnim_DrawAt(0, 0, 1);
        if (gDuelScreen.unk852 != 0)
            break;
        gDuelCmd.counter++;
        if ((s8)gDuelCmd.counter <= 3) {
            PlaySE(7);
            DuelSprAnim_Rewind();
        } else {
            gDuelCmd.step++;
        }
        break;
    case 3:
        LoadDuelUiGfx();
        if ((gDuelCtrl[1] & 1) && !(gDuel.linkSkip)) {
            DuelLink_SendDeck(player);
            gDuelCmd.step = 10;
        } else {
            gDuelCmd.running = 0;
        }
        break;
    case 10:
        if (gLinkState.dirtyDeck)
            gDuelCmd.running = 0;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}
/* Draw arg4 cards for the acting player (deck to hand animation,
 * DuelDrawCard(player, arg2) per card). With an empty deck, set player flag +7
 * bit 0 and stop. */
void DuelCmd_DrawCards(void)
{
    struct CardLoc from, to;
    int player = gDuelCmd.cmd >> 15;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        if (gDuelPlayers[player].deckCount != 0)
            break;
        ((struct DuelPlayerFlags *)&gDuelPlayers[player])->flags7 |= 1;
        gDuelCmd.running = 0;
        break;
    case 1:
        from.player = player;
        from.area = 13;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gDuelPlayers[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(0, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        DuelDrawCard(player, gDuelCmd.arg2);
        gDuelCmd.arg4--;
        if (gDuelCmd.arg4 != 0) {
            ((struct Cmd80C *)&gDuelCmd.unk80C)->unk5 = 0;
            gDuelCmd.step--;
            break;
        }
        /* fallthrough */
    default:
        DuelCursor_Select(player, 11, gDuelPlayers[player].handCount - 1);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/* Repeat arg2 times: take a card for the acting player with TakeDeckCardAt(player, 0, &cmd card) and
 * animate it from area 13 to area 14, then commit with AddCardToGraveyard. */
void DuelCmd_SendTopDeckCardsToGraveyard(void)
{
    struct CardLoc from, to;
    u32 player = gDuelCmd.cmd >> 15;
    int p2 = player;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.arg2 == 0) {
            gDuelCmd.running = 0;
            break;
        }
        if (TakeDeckCardAt(player, 0, CMD_CARD)) {
            from.player = p2;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
        } else {
            DuelCursor_Select(p2, 13, 0);
            DrawAllAreaTiles();
            gDuelCmd.running = 0;
        }
        break;
    case 2:
        AddCardToGraveyard(CMD_CARD);
        DrawAllAreaTiles();
        gDuelCmd.arg2--;
        if (gDuelCmd.arg2 != 0)
            gDuelCmd.step--;
        else
            gDuelCmd.step++;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}
#if 0 /* NONMATCHING: case 0: ROM tests the stored counter by extracting it (lsl #18; lsr #25; cmp), this compiles to an and-mask test */
/* Repeat arg2 times (counter at cmd +0x80A bits 7-13): take a card for the acting player with
 * TakeDeckCardAt(player, 0, &cmd card), animate it from area 13 to area 15, commit with AddCardToBanished. */
void DuelCmd_BanishTopDeckCards(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = gDuelCmd.cmd >> 15;
    int p2 = player;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        cmd->counter = cmd->arg2;
        if (!(cmd->counter > 0)) {
            cmd->running = 0;
            break;
        }
        cmd->step++;
        break;
    case 1:
        if (TakeDeckCardAt(player, 0, CMD_CARD)) {
            from.player = p2 & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2 & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            cmd->step++;
        } else {
            DuelCursor_Select(p2, 13, 0);
            DrawAllAreaTiles();
            cmd->running = 0;
        }
        break;
    case 2:
        AddCardToBanished(CMD_CARD);
        DrawAllAreaTiles();
        cmd->counter--;
        if (cmd->counter > 0) {
            cmd->step--;
            break;
        }
        /* fallthrough */
    default:
        gDuelCmd.running = 0;
        break;
    }
}
#endif
/* Repeat arg2 times (counter at cmd +0x80A bits 7-13): take a card for the acting player with
 * TakeDeckCardAt(player, 0, &cmd card), animate it from area 13 to area 15, commit with AddCardToBanished. */
void DuelCmd_BanishTopDeckCards(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = gDuelCmd.cmd >> 15;
    u32 p2 = player;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        cmd->counter = cmd->arg2;
        if ((s8)cmd->counter == 0) {
            cmd->running = 0;
            break;
        }
        cmd->step++;
        break;
    case 1:
        if (TakeDeckCardAt(player, 0, CMD_CARD)) {
            from.player = p2 & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2 & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            cmd->step++;
        } else {
            DuelCursor_Select(p2, 13, 0);
            DrawAllAreaTiles();
            cmd->running = 0;
        }
        break;
    case 2:
        AddCardToBanished(CMD_CARD);
        DrawAllAreaTiles();
        cmd->counter--;
        if ((s8)cmd->counter > 0) {
            cmd->step--;
            break;
        }
        /* fallthrough */
    default:
        gDuelCmd.running = 0;
        break;
    }
}
/* Return card word (arg2 | arg4 << 16) from area 13 to the acting player's
 * hand. If its owner accepts it (RemoveCardFromDeck), animate it from area 13 to the
 * hand (area 11) and add it with AddCardToHand. */
void DuelCmd_AddDeckCardToHand(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = cmd->cmd >> 15;
    u32 w = cmd->arg2 | (cmd->arg4 << 16);

    switch (cmd->step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        cmd->step++;
        break;
    case 1:
        if (RemoveCardFromDeck(((struct DuelCard *)&w)->owner, &w) == 0) {
            cmd->running = 0;
            break;
        }
        from.player = player;
        from.area = 13;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gDuelPlayers[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(((struct DuelCard *)cmd->card)->id, &from, &to);
        cmd->step++;
        break;
    default:
        AddCardToHand(player, &w);
        DrawAllAreaTiles();
        DuelCursor_Select(player, 11, 0);
        cmd->running = 0;
        break;
    }
}
/* Calls RemoveCardFromDeck(player, card word arg2 | arg4 << 16). */
void DuelCmd_RemoveCardFromDeck(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    RemoveCardFromDeck(player, &w);
    gDuelCmd.running = 0;
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if RemoveCardFromDeck accepts it,
 * from area 13 to monster zone arg6 (animation), then place it there with PlaceMonsterCard. */
void DuelCmd_SummonFromDeck(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = gDuelCmd.cmd >> 15;
    u32 w = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);
    u32 zone = gDuelCmd.arg6;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (RemoveCardFromDeck(player, &w) != 0) {
            CopyDuelCard(CMD_CARD, &w);
            from.player = player;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 0;
            to.index = zone;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
            break;
        }
        goto done;
    default:
        PlaceMonsterCard(player, zone, CMD_CARD, 0, 1);
        DuelCursor_Select(player, 0, zone);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    done:
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if RemoveCardFromDeck accepts it,
 * from area 13 to area 14 (animation), then commit with AddCardToGraveyard. */
void DuelCmd_SendDeckCardToGraveyard(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    int player = gDuelCmd.cmd >> 15;
    u32 w = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (RemoveCardFromDeck(player, &w) != 0) {
            CopyDuelCard(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    default:
        AddCardToGraveyard(CMD_CARD);
        DuelCursor_Select(player, 14, 0);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if RemoveCardFromDeck accepts it,
 * from area 13 to area 15 (animation), then commit with AddCardToBanished. */
void DuelCmd_BanishDeckCard(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    int player = gDuelCmd.cmd >> 15;
    u32 w = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (RemoveCardFromDeck(player, &w) != 0) {
            CopyDuelCard(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    default:
        AddCardToBanished(CMD_CARD);
        DrawAllAreaTiles();
        DuelCursor_Select(player, 15, 0);
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if RemoveCardFromFusionDeck accepts it,
 * from area 12 to area 14 (animation), then commit with AddCardToGraveyard. */
void DuelCmd_SendFusionDeckCardToGraveyard(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    int player = gDuelCmd.cmd >> 15;
    u32 w = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        if (RemoveCardFromFusionDeck(player, &w) != 0) {
            CopyDuelCard(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 12;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    default:
        AddCardToGraveyard(CMD_CARD);
        DrawAllAreaTiles();
        DuelCursor_Select(player, 12, 0);
        cmd->running = 0;
        break;
    }
}
