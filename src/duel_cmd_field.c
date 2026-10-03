#include "global.h"
#include "duel.h"

/*
 * Duel script-command handlers (commands 0x79-0x84, dispatched by DuelCmd_Dispatch).
 * Each one runs as a small multi-frame state machine on gDuelCmd.step and
 * clears gDuelCmd.running when finished. See wiki/functions/code-0800d8a4.md.
 */

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 hdr;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg1;           /* 0x002: usually a zone index */
    u16 arg2;           /* 0x004 */
    u16 arg3;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u8 step:7;          /* 0x80A: handler state */
    u8 unk80A_7:1;
    u8 unk80B;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 unk80E[6];
    struct DuelCard saved814;   /* 0x814: card word saved from a zone */
};

/* Card location passed to the card-move animations (DuelAnim_PlayZoneEffect / DuelAnim_MoveCard). */
struct CardLoc {
    u16 player:1;
    u16 area:4;
    u16 slot:9;
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

extern struct DuelCmd gDuelCmd;
#define gCmd gDuelCmd
#define CMD_PLAYER() (gCmd.hdr >> 15)
/* Card ID of a zone, read as a whole word (agbcc: pointer deref, not a member access). */
#define ZONE_CARD_ID(zone) (((struct DuelCard *)(zone))->id)
/* Zone pointer with the zone term first (the ROM's address order in the later handlers). */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gDuelZones + (p) * 0xD64 + (s) * 0x94))

u32 GetZoneArea(u32 slot);
void DuelScreen_ScrollToZone(u32 player, u32 a);
void DuelAnim_Request(u32 a, u32 b);
void DuelCursor_Select(u32 player, u32 a, u32 slot);
void DrawAllAreaTiles(void);
void PlaySE(u16 se);                          /* PlaySE */
void DuelAnim_PlayZoneEffect(struct CardLoc *loc, const void *anim, u32 a, u32 b);
void DuelAnim_MoveCard(u32 id, struct CardLoc *from, struct CardLoc *to);
void ClearZoneTiles(u32 player, u32 slot);
void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void SendZoneCardToGraveyard(u32 player, u32 slot);
void ClearZoneCardStatusFlags(u32 player, u32 slot);
void ReturnZoneCardToDeck(u32 player, u32 slot);
u32 IsFusionMonster(u16 id);
void BanishZoneCard(u32 player, u32 slot);
void ReturnZoneCardToHand(u32 player, u32 slot);
void DuelScreen_StartScroll(u32 bg);
void DuelAnim_SwapCards(struct CardLoc *from, struct CardLoc *to);
void MemClear16(void *dst, u32 size);
void MemCopy16(void *dst, const void *src, u32 size);
extern const u8 gExplosionAnim[];
extern const u16 gCardIdToNumber[];
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* Command 0x7E: flip the face-down flag of zone arg1 (with animation). */
void DuelCmd_ChangePosition(void)
{
    u32 player = CMD_PLAYER();
    register u32 slot asm("r5") = gCmd.arg1;
    u32 arg = gCmd.arg2;
    struct DuelZonesPlayer *zones = &gDuelZones[player];
    struct DuelZone *zone = &zones->zones[slot];

    if (ZONE_CARD_ID(zone) == 0) {
        gCmd.running = 0;
        return;
    }
    switch (gCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        gCmd.step++;
        break;
    case 1: {
        u32 a = (u8)slot << 8 | player;
        u32 fd = zone->flag6_0;
        u32 b = arg << 24;
        DuelAnim_Request(1, a | (fd << 16 | b));
        gCmd.step++;
        break;
    }
    default:
        zone->flag6_0 = !zone->flag6_0;
        if (arg && !zone->flag6_1)
            zone->flag6_1 = 1;
        DrawAllAreaTiles();
        DuelCursor_Select(player, 0, slot);
        gCmd.running = 0;
        break;
    }
}
/* Command 0x7F: toggle the flag6_1 flag of zone arg1 (stamping a serial when set). */
void DuelCmd_FlipCard(void)
{
    u32 player = CMD_PLAYER();
    register u32 slot asm("r6") = gCmd.arg1;
    register struct DuelZonesPlayer *zones asm("r1") = &gDuelZones[player];
    u32 off = 0x94 * slot;
    struct DuelZone *zone = (struct DuelZone *)((u8 *)zones + off);

    off += player * 0xD64;
    if (ZONE_CARD_ID((struct DuelZone *)((u8 *)gDuelZones + off)) == 0) {
        gCmd.running = 0;
        return;
    }
    switch (gCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        gCmd.step++;
        break;
    case 1:
        DuelAnim_Request(2, ((u8)slot << 8 | player) | ((zone->flag6_0 | zone->flag6_1 << 8) << 16));
        gCmd.step++;
        break;
    default:
        zone->flag6_1 = !zone->flag6_1;
        if (zone->flag6_1)
            zone->serial = (*(u16 *)((u8 *)gDuelZones - 0x2C))++;
        DuelCursor_Select(player, 0, slot);
        DrawAllAreaTiles();
        gCmd.running = 0;
        break;
    }
}
/* Byte view for card-word bit 18, kept separate from the shared DuelCard layout. */
struct ZoneFlags18 {
    u8 pad[2];
    u8 lo:2;
    u8 flag18:1;
    u8 hi:5;
};

void DuelCmd_SendToGraveyard(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0:
        if (((struct DuelCard *)(player * 0xD64 + slot * 0x94 + (u32)gDuelZones))->id == 0) {
            DrawAllAreaTiles();
            cmd->running = 0;
        }
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        cmd->step++;
        break;
    case 1:
        if (((u8 *)cmd)[4]) {
            struct DuelZone *zone;
            u32 pp;

            PlaySE(8);
            from.player = player;
            from.area = 0;
            from.slot = slot;
            pp = player & 1;
            zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gDuelZones);
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            DuelAnim_PlayZoneEffect(&from, gExplosionAnim, 0, 0);
        }
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    case 2: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *base = (u8 *)gDuelZones;
        u8 *zonebase = base + poff;
        u32 zoff = slot * 0x94;
        struct DuelZone *zone;
        u32 word, id;

        CopyDuelCard(saved, (struct DuelCard *)(zonebase + zoff));
        zone = (struct DuelZone *)(zoff + poff + (u32)base);
        ((struct ZoneFlags18 *)zone)->flag18 = 0;
        SendZoneCardToGraveyard(player, slot);
        word = *(u32 *)saved;
        id = (word << 20) >> 20;
        if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F) {
            from.player = player;
            from.area = 0;
            from.slot = slot;
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = 14;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(id, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        DrawAllAreaTiles();
        DuelCursor_Select(player, 0, slot);
        gCmd.running = 0;
        break;
    }
}

/* Reusing `from` across the steps keeps the byte stores after its address escapes. */
void DuelCmd_Banish(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0:
        if (((struct DuelCard *)(player * 0xD64 + slot * 0x94 + (u32)gDuelZones))->id == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    case 1: {
        struct DuelZone *zone;
        u32 pp;

        PlaySE(0x11);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        pp = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gDuelZones);
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        DuelAnim_PlayZoneEffect(&from, gExplosionAnim, 0, 0);
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *zonebase = (u8 *)gDuelZones + poff;
        u32 zoff = slot * 0x94;
        u32 word, id;

        CopyDuelCard(saved, (struct DuelCard *)(zonebase + zoff));
        BanishZoneCard(player, slot);
        word = *(u32 *)saved;
        id = (word << 20) >> 20;
        if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F) {
            struct DuelZone *zone;

            from.player = player;
            from.area = 0;
            from.slot = slot;
            zone = (struct DuelZone *)(zoff + poff + (u32)gDuelZones);
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = 15;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard(id, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        DuelCursor_Select(player, 0, slot);
        DrawAllAreaTiles();
        gCmd.running = 0;
        break;
    }
}

void DuelCmd_BanishFlagged(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gDuelZones);
        if (card->id == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelZone *zone;
        u32 pp;

        PlaySE(0x11);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        pp = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gDuelZones);
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        DuelAnim_PlayZoneEffect(&from, gExplosionAnim, 0, 0);
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        u32 one = 1;
        u32 pp = player & one;
        u32 zoff = slot * 0x94;
        u32 poff = pp * 0xD64;
        u32 zoneoff = zoff + poff;
        u8 *base = (u8 *)gDuelZones;
        struct DuelZone *zone = (struct DuelZone *)(zoneoff + (u32)base);
        struct DuelCard *saved;

        ((u8 *)zone)[2] |= 0x10;
        saved = &cmd->saved814;
        CopyDuelCard(saved, (struct DuelCard *)(poff + (u32)base + zoff));
        BanishZoneCard(player, slot);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = player;
        to.area = 15;
        to.slot = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard((*(u32 *)saved << 20) >> 20, &from, &to);
        cmd->step++;
        break;
    }
    default:
        DuelCursor_Select(player, 0, slot);
        DrawAllAreaTiles();
        gCmd.running = 0;
        break;
    }
}


void DuelCmd_ReturnToHand(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;
    u32 step = cmd->step;

    switch (step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gDuelZones);
        if (card->id == 0) {
            cmd->running = 0;
            break;
        }
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        cmd->step++;
        break;
    }
    case 1: {
            struct DuelCard *saved = &cmd->saved814;
            u32 poff = (player & 1) * 0xD64;
            u8 *base = (u8 *)gDuelZones;
            u8 *zonebase = base + poff;
            u32 zoff = slot * 0x94;
            struct DuelZone *zone;
            u32 word, area, destSlot;

            CopyDuelCard(saved, (struct DuelCard *)(zonebase + zoff));
            ClearZoneCardStatusFlags(player, slot);
            ClearZoneTiles(player, slot);
            word = *(u32 *)saved;
            if ((u16)((*(const u16 *)(0x08622AB4 + ((word << 21) >> 20))) - 0x780) > 0x4F) {
                from.player = player;
                from.area = 0;
                from.slot = slot;
                zone = (struct DuelZone *)(zoff + poff + (u32)base);
                from.flag14 = zone->flag6_0;
                from.flag15 = zone->flag6_1;
                to.player = (word << 19) >> 31;
                area = IsFusionMonster(((u32)*(u16 *)saved << 20) >> 20) ? 12 : 11;
                to.area = area;
                if (IsFusionMonster(((u32)*(u16 *)saved << 20) >> 20) == 0) {
                    /* Keep the initialized player base in r1 while the saved owner is read into r0. */
                    register u8 *players asm("r1") = base - 0x28;
                    u32 owner = (*(u32 *)saved << 19) >> 31;
                    destSlot = *(u8 *)(players + (owner & step) * 0xD64 + 2);
                } else
                    destSlot = 0;
                to.slot = destSlot;
                to.flag14 = 0;
                to.flag15 = ((struct DuelZone *)((player & 1) * 0xD64 + slot * 0x94 + (u32)gDuelZones))->flag6_1;
                DuelAnim_MoveCard((*(u32 *)&gCmd.saved814 << 20) >> 20, &from, &to);
            }
        }
        gCmd.step++;
        break;
    default:
        ReturnZoneCardToHand(player, slot);
        DrawAllAreaTiles();
        DuelCursor_Select(player, 0, slot);
        cmd->running = 0;
        break;
    }
}



void DuelCmd_ReturnToDeck(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gDuelZones);
        if (card->id == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *base = (u8 *)gDuelZones;
        u8 *zonebase = base + poff;
        u32 zoff = slot * 0x94;
        struct DuelZone *zone;
        u32 word;

        CopyDuelCard(saved, (struct DuelCard *)(zonebase + zoff));
        zone = (struct DuelZone *)(zoff + poff + (u32)base);
        ((struct ZoneFlags18 *)zone)->flag18 = 0;
        ClearZoneTiles(player, slot);
        word = *(u32 *)saved;
        if ((u16)((*(const u16 *)(0x08622AB4 + ((word << 21) >> 20))) - 0x780) > 0x4F) {
            from.player = player;
            from.area = 0;
            from.slot = slot;
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = IsFusionMonster((((u32)*(u16 *)saved << 20) >> 20)) ? 12 : 13;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            DuelAnim_MoveCard((*(u32 *)saved << 20) >> 20, &from, &to);
        }
        gCmd.step++;
        break;
    }
    default:
        ClearZoneCardStatusFlags(player, slot);
        ReturnZoneCardToDeck(player, slot);
        DuelCursor_Select(player, 0, slot);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
}


/* Byte view for zone byte 7 bit 2, so the clear emits mov #5; neg. */
struct ZoneFlags7 {
    u8 pad[7];
    u8 lo:2;
    u8 flag:1;
    u8 hi:5;
};

/* Command 0x84 (hypothesis): place the card saved in gCmd+0x814 into zone
 * (arg2) and clear zone (arg1). */
void DuelCmd_MoveToZone(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 a = (u8)cmd->arg1;
    u32 b = cmd->arg1 >> 8;
    u32 c = (u8)cmd->arg2;
    int d = cmd->arg2 >> 8;

    switch (gCmd.step) {
    case 0:
        if (((struct DuelCard *)((a & 1) * 0xD64 + b * 0x94 + (u32)gDuelZones))->id == 0) {
            cmd->running = 0;
        } else {
            DuelScreen_ScrollToZone(a, GetZoneArea(b));
            gCmd.step++;
        }
        break;
    case 1: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (a & 1) * 0xD64;
        u8 *base = (u8 *)gDuelZones;
        u8 *zonebase = base + poff;
        u32 zoff = b * 0x94;
        struct DuelZone *zone;

        CopyDuelCard(saved, (struct DuelCard *)(zonebase + zoff));
        ClearZoneTiles(a, b);
        from.player = a;
        from.area = 0;
        from.slot = b;
        zone = (struct DuelZone *)(zoff + poff + (u32)base);
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = c;
        to.area = 0;
        to.slot = d;
        to.flag14 = from.flag14;
        to.flag15 = from.flag15;
        if (d > 4)
            from.flag14 = 0;
        DuelAnim_MoveCard(saved->id, &from, &to);
        gCmd.step++;
        break;
    }
    default: {
        u8 *base = (u8 *)gDuelZones;
        u32 poffC = (c & 1) * 0xD64;
        u8 *zbaseC = base + poffC;
        u32 zoffD = d * 0x94;
        struct DuelZone *zoneC = (struct DuelZone *)(zbaseC + zoffD);
        u32 poffA = (a & 1) * 0xD64;
        u8 *zbaseA = base + poffA;
        struct DuelZone *zoneA = (struct DuelZone *)(zbaseA + b * 0x94);

        MemCopy16(zoneC, zoneA, 0x94);
        CopyDuelCard(&zoneC->card, &cmd->saved814);
        MemClear16(zoneA, 0x94);
        ((struct ZoneFlags7 *)(poffC + zoffD + (u32)base))->flag = 0;
        DuelCursor_Select(c, 0, d);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
    }
}
/* Command 0x84 (hypothesis): swap the contents of two zones. arg1/arg2 each
 * encode (player | slot << 8); step 0 checks both zones hold cards, step 1
 * animates the held card (DuelAnim_SwapCards), step 2 performs the swap. */
/* Zone address forms used by this handler. The add order picks the ROM's
 * evaluation order: a local `base` keeps `(base + p*0xD64) + s*0x94` from being
 * reassociated, and in a memory address the second product is emitted first. */
#define ZONE_874(p, s) ((struct DuelZone *)(base + (p) * 0xD64 + (s) * 0x94))
#define ZONE_874C(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
void DuelCmd_SwapZones(void)
{
    struct CardLoc from;
    struct CardLoc to;
    struct DuelZone tmp;
    struct DuelCmd *cmd = &gCmd;
    u32 a = (u8)cmd->arg1;
    u32 b = cmd->arg1 >> 8;
    u32 c = (u8)cmd->arg2;
    int d = cmd->arg2 >> 8;
    u32 step = gCmd.step;

    switch (step) {
    case 0:
        /* Two separate ifs (cross-jumped later) give cmd the extra reference
         * that puts it in r9 ahead of b. */
        if (ZONE_CARD_ID(ZONE_874C(a & 1, b)) == 0) {
            cmd->running = 0;
            break;
        }
        if (ZONE_CARD_ID(ZONE_874C(c & 1, d)) == 0) {
            cmd->running = 0;
            break;
        }
        DuelScreen_StartScroll(0x50);
        gCmd.step++;
        break;
    case 1: {
        u8 *base = (u8 *)gDuelZones;
        u32 poff;

        /* Naming only the player offset, assigned inside the argument, gives it
         * r5 and the slot product r4 (as in the ROM). */
        CopyDuelCard(&cmd->saved814, (struct DuelCard *)(base + (poff = (a & 1) * 0xD64) + b * 0x94));
        ClearZoneTiles(a, b);
        ClearZoneTiles(c, d);
        from.player = a;
        from.area = 0;
        from.slot = b;
        from.flag14 = ((struct DuelZone *)(b * 0x94 + poff + (u32)base))->flag6_0;
        from.flag15 = ((struct DuelZone *)(b * 0x94 + poff + (u32)base))->flag6_1;
        to.player = c;
        to.area = 0;
        to.slot = d;
        to.flag14 = from.flag14;
        to.flag15 = from.flag15;
        if (d > 4)
            from.flag14 = 0;
        DuelAnim_SwapCards(&from, &to);
        gCmd.step++;
        break;
    }
    default: {
        u8 *base = (u8 *)gDuelZones;

        MemCopy16(&tmp, ZONE_874(c & 1, d), 0x94);
        MemCopy16(ZONE_874(c & 1, d), ZONE_874(a & 1, b), 0x94);
        MemCopy16(ZONE_874(a & 1, b), &tmp, 0x94);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
    }
}
