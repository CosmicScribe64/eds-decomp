#include "global.h"

/* Output of GetZoneCardStats (card-in-zone info). */
struct ZoneCardInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
};

/* Duel command block at 0x020185C0 (see duel_cmd_field / duel_cmd_deck). */
struct DuelCmd {
    u16 hdr;          /* +0x00: bits 0-11 command id, bit 15 acting player */
    u16 arg1;         /* +0x02 */
    u16 arg2;         /* +0x04 */
    u16 arg3;         /* +0x06 */
    u8 filler8[0x80A - 0x8];
    u8 step : 7;      /* +0x80A bits 0-6: multi-frame handler state */
    u8 unk80A_7 : 1;
    u8 unk80B;
    u32 unk80C_0 : 5;
    u32 timer : 7;    /* +0x80C bits 5-11: frame counter inside a step */
    u32 unk80C_12 : 1;
    u32 running : 1;  /* +0x80D bit 5: command in progress */
    u32 unk80C_14 : 2;
    u32 unk80C_16 : 16;
};

/* Per-player duel state, 0xD64 bytes; two of them at 0x020192E4. */
struct DuelPlayer {
    u8 filler0[8];
    u8 flag8_0 : 1;   /* +0x08 bit 0 */
    u8 flag8_1 : 1;   /* +0x08 bit 1 */
    u8 unk8_2 : 6;
    u8 filler9[0x24 - 0x9];
    u16 unk24;        /* +0x24 */
    u8 filler26[0xD64 - 0x26];
};

/*
 * First word of a field zone. Zones are 0x94 bytes, at 0x0201930C + player*0xD64 + slot*0x94.
 * Reads of the card id use a full-word ldr, so the code must see a 4-byte struct here
 * (a larger struct makes agbcc narrow the read to ldrh).
 */
struct ZoneWord {
    u32 cardId : 12;  /* bits 0-11 (0 = empty) */
    u32 unk0_12 : 20;
};

/* Card reference passed to EffectEquipTargetCheck (0x14 bytes on the stack; layout beyond +2 unknown). */
struct CardRef {
    u16 id;           /* +0x00 card id */
    u8 player : 1;    /* +0x02 bit 0 */
    u8 unk2_1 : 7;
    u8 filler3[0x14 - 0x3];
};

/* Duel-wide phase word at 0x020192E0+0x1B14 (== 0x020192E4+0x1B10). */
struct DuelPhaseWord {
    u32 unk0 : 9;
    u32 phase : 8;    /* bits 9-16 */
    u32 unk17 : 8;    /* bits 17-24 */
    u32 unk25 : 7;
};

/* 0x020192E0: global duel state (hypothesis: players[] live at +4). */
struct DuelGlobal {
    u8 filler0[0x1B12];
    u8 flags1B12;     /* bit 1: ?, bit 5: link error */
    u8 unk1B13;
    struct DuelPhaseWord phaseWord; /* +0x1B14 */
};

/* The same memory seen from 0x020192E4. */
struct DuelPlayers {
    struct DuelPlayer p[2];
    u8 filler1AC8[0x1B10 - 0x1AC8];
    struct DuelPhaseWord phaseWord; /* +0x1B10 */
};

/* 0x02015EE8: byte 1 bit 0 = link (two-GBA) duel (hypothesis). */
struct Unk02015EE8 {
    u8 unk0;
    u8 flags1;
};

struct Unk02018450 {
    u16 unk0_0 : 5;
    u16 unk0_5 : 1;   /* bit 5 */
    u16 unk0_6 : 3;   /* bits 6-8 */
    u16 unk0_9 : 3;   /* bits 9-11 */
    u16 unk0_12 : 4;
};

extern struct DuelCmd gDuelCmd;
extern struct Unk02018450 gBattle;
extern struct DuelGlobal gDuel;
extern struct DuelPlayers gDuelPlayers;
extern struct Unk02015EE8 gDuelCtrl;
extern u8 gDuelZones[];
extern u16 gCardIdToNumber[];

#define gCmd gDuelCmd
#define CMD_PLAYER() (gCmd.hdr >> 15)
#define CMD_DONE() (gCmd.running = 0)
#define ZONE(p, s) ((struct ZoneWord *)(gDuelZones + (s) * 0x94 + (p) * 0xD64))

/* True unless this is a link duel whose flag 0x1B12 bit 1 is set (hypothesis: "not the slave side"). */
#define LINK_SKIP() ((gDuelCtrl.flags1 & 1) && (gDuel.flags1B12 & 2))

void GetZoneCardStats(u32 player, u32 slot, struct ZoneCardInfo *out);
void MarkMonsterAttacked(u32, u32);
u32 DuelScreen_FadeOutStep(void);
void BattleScene_Init(u32, u32);
u32 BattleScene_Update(u32, u32, u32);
void AddCardToGraveyard(void *);
void AddCardToBanished(void *);
void DrawAllAreaTiles(void);
u16 EffectEquipTargetCheck(struct CardRef *card, u16 pos);
void PlaceMonsterCard(u32 player, u32 slot, void *data, u16 a3, u16 a4);
s32 FindZoneLinkFromCard(s32, s32, u16);
s32 CountActiveCardsOnField(s32, s32);
extern u32 gCardStats[];
extern u8 gUnk_0201ADAD;
void DuelCursor_Select(u32, s32, u16);
void CopyDoubleWords(u32, void *, u32);
void AddAffineSprite(u32, u32, u32, u32);
/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast : 1;        /* +0x000 bit 0: fast-forward animations */
    u8 unk0_1 : 7;
    u8 filler1[0x808 - 1];
    u8 unk808_0 : 3;
    u8 busy : 1;        /* +0x808 bit 3 */
    u8 unk808_4 : 4;
};
extern struct DuelScreen gDuelScreen;
extern u16 gBounceScaleCurve[];
extern u8 gDuelBannerPal[];
extern u8 gDirectAttackBannerGfx[];
extern u16 gMain[];
extern u8 gDuelSpellTrapZones[];

u32 GetZoneCardAtk(u32 player, u32 slot)
{
    struct ZoneCardInfo info;
    GetZoneCardStats(player, slot, &info);
    return info.unk4;
}

u32 GetZoneCardDef(u32 player, u32 slot)
{
    struct ZoneCardInfo info;
    GetZoneCardStats(player, slot, &info);
    return info.unk8;
}

/* Field zone (0x94 bytes) as read by GetZoneCardType. */
struct C8BCZone {
    u32 card;               /* +0x00: bits 0-11 card id, bit 17 tested */
    u16 unk4;               /* +0x04: compared value (hypothesis: current ATK-like stat) */
    u8 flags6;              /* +0x06: bit 1 face up */
    u8 filler7[3];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkKinds[32];      /* +0x4A: low byte = link kind */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[4];
    u32 unk90;              /* +0x90: bits 13-17 replacement value; byte +0x91 bit 3 tested */
};
/* Byte view of links[], so the player byte is a separate ldrb from the same address as the ldrh. */
struct C8BCZoneB {
    u8 filler0[0xA];
    u8 linkBytes[64];       /* +0x0A */
};
/* Loop 1 order: slot term first (fold keeps it). */
#define C8BC_ZONE(p, s) ((struct C8BCZone *)(gDuelZones + ((s) * 0x94 + ((p) & 1) * 0xD64)))
/*
 * Loop 3 order: player term written first, which fold swaps, so the 0x94/slot chain is loop.c's
 * first movable and the threshold runs out before the 0xD64 multiply (ROM keeps it in the loop).
 */
#define C8BC_ZONE2(p, s) ((struct C8BCZone *)(gDuelZones + (((p) & 1) * 0xD64 + (s) * 0x94)))
#define C8BC_ZONEB2(p, s) ((struct C8BCZoneB *)(gDuelZones + (((p) & 1) * 0xD64 + (s) * 0x94)))
/* Linked zone; the caller passes the already-masked player bit. */
#define C8BC_LINKED(p, s) ((struct C8BCZone *)(gDuelZones + ((s) * 0x94 + (p) * 0xD64)))
#define C8BC_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define C8BC_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/*
 * Card stats bits 20-24 of the card in (player, slot), or 0 for an empty zone. For a face-up
 * monster (slot 0-4) the best (highest +4 value) of: a face-up card number 0x2FA with word bit 17
 * in the player's monster zones (gives 10), a face-up unflagged card number 0x479 in either
 * player's spell/trap zones (gives its +0x90 bits 13-17), and a kind-1 link to card number 0x60E
 * (no 0x601 on either side, gUnk_0201ADAD bits 0-1 clear; gives 1) replaces it.
 */
u32 GetZoneCardType(s32 player, s32 slot)
{
    u32 off;
    u32 id;
    u16 best;   /* u16: the compare is ldrh/bls and the assignment re-reads the field */
    u32 result;
    int i, j;
    struct C8BCZone *z;
    u8 *zones;
    u32 so;

    /* Player offset, zone base and slot offset staged in ROM order before best = 0. */
    off = (player & 1) * 0xD64;
    zones = gDuelZones;
    so = slot * 0x94;
    best = 0;
    z = (struct C8BCZone *)(zones + (so + off));
    id = (z->card << 20) >> 20;
    if (id == 0)
        return 0;
    result = (C8BC_STATS(id) & 0x1F00000) >> 20;
    if (slot > 4 || !(z->flags6 & 2))
        return result;
    for (i = 0; i <= 4; i++) {
        u32 w;
        u32 cid;
        /* Separate pointer from the entry's z (different pseudo, r2 here vs r3 there). */
        struct C8BCZone *z;

        /* Re-forming (player & 1) * 0xD64 lets CSE reuse the entry's off (spilled to [sp+4]). */
        z = C8BC_ZONE(player, i);
        w = z->card;
        cid = (w << 20) >> 20;
        if (cid != 0 && C8BC_NUMBER(cid) == 0x2FA && (s32)(w << 14) < 0
            && (z->flags6 & 2) && z->unk4 > best) {
            best = z->unk4;
            result = 10;
        }
        for (j = 0; j <= 1; j++) {
            /* gDuelSpellTrapZones = gDuelZones + 5 * 0x94: zone (j, 5 + i). */
            z = (struct C8BCZone *)(gDuelSpellTrapZones + ((j & 1) * 0xD64 + i * 0x94));
            cid = (z->card << 20) >> 20;
            if (cid != 0 && C8BC_NUMBER(cid) == 0x479 && (z->flags6 & 2)
                && !(((u8 *)z)[0x91] & 8) && z->unk4 > best) {
                best = z->unk4;
                result = (z->unk90 << 14) >> 27;
            }
        }
    }
    {
        /* Pointer set outside the loop: reloaded before its ldrb, after the #3 mask. */
        u8 *flags = &gUnk_0201ADAD;

        for (i = 0; i < C8BC_ZONE2(player, slot)->numLinks; i++) {
            u16 link;
            u8 kind;
            int lz, lp;
            struct C8BCZone *t;
            u16 tid;
            u8 *zb;

            /* FAKEMATCH: the links[] read goes through a local copy of the zone base, which
             * changes the guard's allocation priorities (slot * 0x94 in r1, base in r4). */
            zb = gDuelZones;
            link = ((struct C8BCZone *)(zb + ((player & 1) * 0xD64 + slot * 0x94)))->links[i];
            kind = C8BC_ZONE2(player, slot)->linkKinds[i];
            lz = link >> 8;
            lp = C8BC_ZONEB2(player, slot)->linkBytes[i * 2] & 1;
            t = C8BC_LINKED(lp, lz);
            tid = (t->card << 20) >> 20;
            if (kind == 1 && tid != 0
                && !(((u8 *)t)[0x91] & 8)
                && CountActiveCardsOnField(0, 0x601) == 0
                && CountActiveCardsOnField(1, 0x601) == 0
                && !(*flags & 3)
                && C8BC_NUMBER(tid) == 0x60E
                && t->unk4 > best)
                result = 1;
        }
    }
    return result;
}
/* Field zone (0x94 bytes) as read by GetZoneCardAttribute. */
struct CAF0Zone {
    u32 card;               /* +0x00: bits 0-11 card id */
    u8 filler4[2];
    u8 flags6;              /* +0x06: bit 1 face up */
    u8 filler7[3];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkKinds[32];      /* +0x4A: low byte = link kind */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[4];
    u32 unk90;              /* +0x90: bits 13-17 replacement state; byte +0x91 bit 3 tested */
};
/* Byte view of links[], so the player byte is a separate ldrb from the same address as the ldrh. */
struct CAF0ZoneB {
    u8 filler0[0xA];
    u8 linkBytes[64];       /* +0x0A */
};
#define CAF0_ZONE(p, s) ((struct CAF0Zone *)(gDuelZones + ((s) * 0x94 + ((p) & 1) * 0xD64)))
#define CAF0_ZONEB(p, s) ((struct CAF0ZoneB *)(gDuelZones + ((s) * 0x94 + ((p) & 1) * 0xD64)))
/* Linked zone; the caller passes the already-masked player bit. */
#define CAF0_LINKED(p, s) ((struct CAF0Zone *)(gDuelZones + ((s) * 0x94 + (p) * 0xD64)))
#define CAF0_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CAF0_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/*
 * Card stats bits 29-31 of the card in (player, slot), or 0 for an empty zone. For a face-up
 * monster (slot 0-4), a kind-1 link to a card with number 0x5A8 (not flagged at +0x91 bit 3,
 * no 0x601 on either side, gUnk_0201ADAD bits 0-1 clear) replaces it with that zone's +0x90
 * bits 13-17.
 */
u32 GetZoneCardAttribute(s32 player, s32 slot)
{
    u32 id;
    u32 result;
    int i;
    u32 off;
    u8 *zones;

    /* The player offset and the zone base are staged before the slot offset (ROM order). */
    off = (player & 1) * 0xD64;
    zones = gDuelZones;
    id = (((struct CAF0Zone *)(zones + (slot * 0x94 + off)))->card << 20) >> 20;
    if (id == 0)
        return 0;
    result = CAF0_STATS(id) >> 29;
    if (slot > 4 || !(CAF0_ZONE(player, slot)->flags6 & 2))
        return result;
    {
        /* Pointer set outside the loop: reloaded before its ldrb, after the #3 mask. */
        u8 *flags = &gUnk_0201ADAD;

        for (i = 0; i < CAF0_ZONE(player, slot)->numLinks; i++) {
            u16 link;
            u8 kind;
            int lz, lp;
            struct CAF0Zone *t;
            u16 tid;

            /* FAKEMATCH: an extra use of the zone base the loop hoists, so global alloc keeps it
             * in r9 and spills the hoisted linkKinds pointer to [sp] instead. */
            asm("" :: "r"(gDuelZones));
            link = CAF0_ZONE(player, slot)->links[i];
            kind = CAF0_ZONE(player, slot)->linkKinds[i];
            lz = link >> 8;
            lp = CAF0_ZONEB(player, slot)->linkBytes[i * 2] & 1;
            t = CAF0_LINKED(lp, lz);
            tid = (t->card << 20) >> 20;
            if (kind == 1 && tid != 0
                && !(((u8 *)t)[0x91] & 8)
                && CountActiveCardsOnField(0, 0x601) == 0
                && CountActiveCardsOnField(1, 0x601) == 0
                && !(*flags & 3)
                && CAF0_NUMBER(tid) == 0x5A8)
                result = (t->unk90 << 14) >> 27;
        }
    }
    return result;
}
s32 IsCardLinkedToMonster(s32 player, s32 slot)
{
    u16 *tab = gCardIdToNumber;
    u16 cardId;
    s32 p, s;
    cardId = *(u32 *)&gDuelZones[(player & 1) * 0xD64 + slot * 0x94] << 20 >> 20;
    if (cardId == 0) {
        return 0;
    }
    for (p = 0; p <= 1; p++) {
        for (s = 0; s <= 4; s++) {
            u8 *zone = &gDuelZones[s * 0x94 + (p & 1) * 0xD64];
            if ((*(u32 *)zone << 20) == 0) {
                continue;
            }
            if (!(zone[6] & 2)) {
                continue;
            }
            if (FindZoneLinkFromCard(p, s, tab[cardId & 0x7FF]) != -1) {
                return 1;
            }
        }
    }
    return 0;
}
/* Evaluates the card in (player, slot) against zone pos (targetPlayer, targetSlot) via EffectEquipTargetCheck. */
u16 IsValidEquipTarget(u32 player, u32 slot, u32 targetPlayer, u32 targetSlot)
{
    struct CardRef ref;
    ref.id = ZONE(player & 1, slot)->cardId;
    ref.player = player;
    return EffectEquipTargetCheck(&ref, (u8)targetPlayer | ((u8)targetSlot << 8));
}
/* Counts the zones (both players, slots 0-4) for which IsValidEquipTarget returns nonzero. */
int CountValidEquipTargets(u32 player, u32 slot)
{
    int count = 0;
    int p, s;
    for (p = 0; p <= 1; p++)
        for (s = 0; s <= 4; s++)
            if (IsValidEquipTarget(player, slot, p, s))
                count++;
    return count;
}
u16 FindMonsterLinkedToCard(s32 player, s32 slot)
{
    u16 *tab = gCardIdToNumber;
    u16 cardId;
    s32 p, s;
    cardId = *(u32 *)&gDuelZones[(player & 1) * 0xD64 + slot * 0x94] << 20 >> 20;
    if (cardId == 0) {
        return 0;
    }
    for (p = 0; p <= 1; p++) {
        for (s = 0; s <= 4; s++) {
            u8 *zone = &gDuelZones[s * 0x94 + (p & 1) * 0xD64];
            if ((*(u32 *)zone << 20) == 0) {
                continue;
            }
            if (!(zone[6] & 2)) {
                continue;
            }
            if (FindZoneLinkFromCard(p, s, tab[cardId & 0x7FF]) != -1) {
                return (u8)p | ((u8)s << 8);
            }
        }
    }
    return 0xFFFF;
}
s32 GetAreaX(u32, s32, u16);
s32 GetAreaY(u32, s32, u16);
extern u8 gAttackBannerGfx[];

void DuelCmd_Attack(void)
{
    u32 player = gCmd.hdr >> 15;
    u16 slot = gCmd.arg1;
    u16 otherSlot = gCmd.arg2;
    s32 step = gCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.busy = 1;
        DuelCursor_Select(player, 0, slot);
        gCmd.step++;
        break;
    case 1:
        gDuelScreen.busy = 1;
        DuelCursor_Select(1 - player, 0, otherSlot);
        gCmd.step++;
        break;
    case 2:
        gDuelScreen.busy = 0;
        gCmd.timer = 0;
        gCmd.step++;
        break;
    case 3: {
        s32 x = GetAreaX(player, 0, slot);
        s32 y = GetAreaY(player, 0, slot);
        u32 packed = (u32)(x + 8) | ((u32)(y + 8) << 16);
        s32 t;

        AddAffineSprite(packed, 0x40, 0x5200, (gCmd.timer * 4 + (player ? 0x40 : 0)) | 0x1000000);
        gCmd.timer++;
        t = gCmd.timer;
        if (t <= 31) {
            if ((gMain[2] & 2) || gDuelScreen.fast)
                if (t <= 23)
                    gCmd.timer += 7;
        } else {
            gCmd.timer = 0;
            gCmd.step++;
        }
        break;
    }
    case 4: {
        u32 other = 1 - player;
        s32 dx = GetAreaX(other, 0, otherSlot);
        s32 dy = GetAreaY(other, 0, otherSlot);
        s32 t;

        dx -= GetAreaX(player, 0, slot);
        dy -= GetAreaY(player, 0, slot);
        dx *= gCmd.timer;
        dy *= gCmd.timer;
        dx /= 32;
        dy /= 32;
        dx += GetAreaX(player, 0, slot) + 8;
        dy += GetAreaY(player, 0, slot) + 8;
        dy = (dy << 16) | dx;
        AddAffineSprite(dy, 0x40, 0x5200, ((u32)gBounceScaleCurve[gCmd.timer] << 16) | (player ? 0x40 : 0));
        gCmd.timer++;
        t = gCmd.timer;
        if (t <= 31) {
            if ((gMain[2] & 2) || gDuelScreen.fast)
                if (t <= 23)
                    gCmd.timer += 7;
        } else {
            gCmd.timer = 0;
            gCmd.step++;
        }
        break;
    }
    case 5:
        CopyDoubleWords(0x050003E0, gDuelBannerPal, 0x20);
        CopyDoubleWords(0x06016C80, gAttackBannerGfx, 0x400);
        gCmd.timer = 0;
        gCmd.step++;
        break;
    case 6: {
        s32 t = gCmd.timer;
        if (t <= 95) {
            AddAffineSprite(0x300058, 0x40C0, 0xF364, (u32)gBounceScaleCurve[t & 31] << 16);
            gCmd.timer++;
            if ((gMain[2] & 2) || gDuelScreen.fast)
                if (gCmd.timer <= 87)
                    gCmd.timer += 3;
            break;
        }
    }
    default:
        CMD_DONE();
        break;
    }
}
void DuelCmd_DirectAttack(void)
{
    u32 player = CMD_PLAYER();
    u16 arg = gDuelCmd.arg1;
    s32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.busy = 1;
        DuelCursor_Select(player, 0, arg);
        gDuelCmd.step++;
        break;
    case 1:
        gDuelScreen.busy = 0;
        CopyDoubleWords(0x050003E0, gDuelBannerPal, 0x20);
        CopyDoubleWords(0x06016C80, gDirectAttackBannerGfx, 0x400);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2: {
        s32 t = gDuelCmd.timer;
        if (t <= 0x5F) {
            AddAffineSprite(0x00300058, 0x40C0, 0xF364, gBounceScaleCurve[t & 0x1F] << 16);
            gDuelCmd.timer++;
            if ((step & gMain[2]) || gDuelScreen.fast) {
                if (gDuelCmd.timer <= 0x57) {
                    gDuelCmd.timer += 7;
                }
            }
            return;
        }
    }
    /* fallthrough */
    default:
        CMD_DONE();
        return;
    }
}
/* Zone byte +0x8C flags. */
struct ZoneFlags8C {
    u8 b0 : 1;
    u8 b1 : 1;        /* bit 1: cleared every call */
    u8 b2 : 1;
    u8 b3 : 1;        /* bit 3: moved to bit 4 */
    u8 b4 : 1;
    u8 rest : 3;
};

void DuelCmd_PrepareBattlePhase(void)
{
    struct DuelCmd *t = &gDuelCmd;
    s32 i = 0;
    struct DuelCmd *cmd;
    u32 off;
    u8 *gBase;
    u8 *pb;

    /* FAKEMATCH: the ROM loads &gCmd before i = 0 and copies it into the long-lived cmd register
     * afterwards; a memory clobber between the two stops combine from merging the load into the copy. */
    asm volatile("" ::: "memory");
    cmd = t;
    off = (cmd->hdr >> 15) * 0xD64;
    gBase = gDuelZones;
    pb = gBase + off;

    do {
        u32 zOff = i * 0x94;
        struct ZoneFlags8C *f = (struct ZoneFlags8C *)(pb + zOff + 0x8C);
        u8 *z;

        if (f->b3) {
            f->b3 = 0;
            f->b4 = 1;
        }
        f->b1 = 0;
        /* Integer sum keeps the ROM's (zOff + off) + base operand order. */
        z = (u8 *)(zOff + off + (u32)gBase);
        /* A one-case switch keeps the 0x538 constant inside the loop (an == compare gets hoisted). */
        switch (((u16 *)0x08622AB4)[((struct ZoneWord *)z)->cardId & 0x7FF]) {
        case 0x538:
            z[7] |= 0x20;
        }
        i++;
    } while (i <= 4);
    cmd->running = 0;
}


void DuelCmd_MarkAttacked(void)
{
    MarkMonsterAttacked(CMD_PLAYER(), gDuelCmd.arg1);
    CMD_DONE();
}

void DuelCmd_StartBattleScene(void)
{
    if (DuelScreen_FadeOutStep()) {
        BattleScene_Init(gDuelCmd.arg1, gDuelCmd.arg2);
        CMD_DONE();
    }
}

void DuelCmd_PlayBattleScene(void)
{
    if (BattleScene_Update(gDuelCmd.arg1, gDuelCmd.arg2, gDuelCmd.arg3))
        CMD_DONE();
}

void DuelCmd_SetBattleProtection(void)
{
    u8 player = CMD_PLAYER();
    if (gCmd.arg1)
        gDuelPlayers.p[player].flag8_0 = 1;
    if (gCmd.arg2)
        gDuelPlayers.p[player].flag8_1 = 1;
    CMD_DONE();
}
void DuelCmd_EndBattlePhase(void)
{
    if (!LINK_SKIP()) {
        gDuelPlayers.p[0].unk24 = 0;
        gDuelPlayers.p[1].unk24 = 0;
        gDuelPlayers.phaseWord.phase = 12;
    }
    CMD_DONE();
}
void DuelCmd_SetAttackTarget(void)
{
    if (!LINK_SKIP()) {
        gBattle.unk0_9 = (u8)(gCmd.arg1 >> 8);
        if (gCmd.arg2) {
            gDuel.phaseWord.phase = 6;
            gDuel.phaseWord.unk17 = 0;
        }
    }
    CMD_DONE();
}
void DuelCmd_SetAttacker(void)
{
    if (!LINK_SKIP()) {
        gBattle.unk0_6 = gCmd.arg1 >> 8;
        if (gCmd.arg2) {
            gDuel.phaseWord.phase = 6;
            gDuel.phaseWord.unk17 = 0;
        }
    }
    CMD_DONE();
}

void DuelCmd_ZeroAttackerAtk(void)
{
    gBattle.unk0_5 = 1;
    CMD_DONE();
}

void DuelCmd_NegateAttack(void)
{
    u32 player = CMD_PLAYER();
    u16 arg = gCmd.arg1;
    if (!LINK_SKIP()) {
        gDuel.phaseWord.phase = 11;
        gDuel.phaseWord.unk17 = 0;
    }
    MarkMonsterAttacked(player, arg);
    CMD_DONE();
}
void DuelCmd_PlaceCard(void)
{
    u32 player = CMD_PLAYER();
    u8 slot = gCmd.arg1;
    u8 flags = gCmd.arg1 >> 8;
    u16 flag0 = flags & 1;
    u16 flag1 = (u8)(flags & 2) >> 1;
    u32 data = (gCmd.arg3 << 16) | gCmd.arg2;
    PlaceMonsterCard(player, slot, &data, flag1, flag0);
    DrawAllAreaTiles();
    CMD_DONE();
}
void DuelCmd_ClearZoneCard(void)
{
    ZONE(CMD_PLAYER(), gCmd.arg1)->cardId = 0;
    DrawAllAreaTiles();
    CMD_DONE();
}

void DuelCmd_AddCardToGraveyard(void)
{
    u32 w = (gDuelCmd.arg2 << 16) | gDuelCmd.arg1;
    if (w << 20)
        AddCardToGraveyard(&w);
    DrawAllAreaTiles();
    CMD_DONE();
}

void DuelCmd_AddCardToBanished(void)
{
    u32 w = (gDuelCmd.arg2 << 16) | gDuelCmd.arg1;
    if (w << 20)
        AddCardToBanished(&w);
    DrawAllAreaTiles();
    CMD_DONE();
}
