#include "global.h"
#include "legacy/gba.h"                    /* B_BUTTON, OBJ_PLTT, OBJ_VRAM0 */
#include "legacy/main.h"                   /* gMain.heldKeys */
#include "util.h"                   /* CopyDoubleWords */
#include "sprite.h"                 /* AddAffineSprite, enum SpriteShape */
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_ATTR */
#include "battle_scene.h"           /* BattleScene_Init, BattleScene_Update */
#include "duel_flow.h"              /* gDuelCtrl */
#include "effect_handlers.h"        /* EffectEquipTargetCheck */
#include "constants/cards.h"
#include "constants/card_stats.h"
#include "constants/duel.h"

/*
 * Field-card queries and the battle duel commands (wiki/functions/duel-stat-queries-c.md):
 *  - effective ATK, DEF, monster type and attribute of the card in a zone (player, slot); the type and
 *    attribute apply the overrides of Parasite Paracide, DNA Surgery and two non-EDS equip keys;
 *  - searches for the monster an equip card is linked to, and the equip-target check loop;
 *  - the duel command handlers DUEL_CMD_START_BATTLE_SCENE .. DUEL_CMD_NEGATE_ATTACK (0x30-0x3B) and
 *    DUEL_CMD_PLACE_CARD, CLEAR_ZONE_CARD, ADD_CARD_TO_GRAVEYARD, ADD_CARD_TO_BANISHED (0x77, 0x78, 0x7C, 0x7D).
 *    DuelCmd_Dispatch calls the handler of the running command once per frame; a handler steps through
 *    gDuelCmd.step and clears gDuelCmd.running when it is done.
 */

/* ---- BEGIN header subset (pre-H0) ---- */
/*
 * The parts of duel.h, duel_screen.h, duel_cmd.h, battle.h and chain.h this unit uses, with the headers'
 * tags, names, types and bitfield containers. include/duel.h still holds the legacy header until the header
 * switch (H0, build/readability/HEADERS.md), and the other four headers include duel.h. After H0, replace
 * this block (BEGIN to END) with:
 *     #include "battle.h"
 *     #include "chain.h"
 *     #include "legacy/duel.h"
 *     #include "duel_cmd.h"
 *     #include "duel_screen.h"
 */

/* duel.h */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:4;
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 unk18:14;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed (replay check) */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7_0:5;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet */
    u8 unk7_6:2;
    u8 unk8[2];
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one, or a value / card ID */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind, high byte stack count / value */
    u16 numLinks;                   /* +0x8A: entries in links / linkKinds */
    u8 unk8C_0:1;                   /* +0x8C: battle flags */
    u8 destroyAfterBattle:1;        /* +0x8C bit 1: non-monster placed in a monster zone (Magical Hats) */
    u8 unk8C_2:1;
    u8 cannotAttackNextTurn:1;      /* +0x8C bit 3: Electric Lizard; moved into cannotAttack */
    u8 cannotAttack:1;              /* +0x8C bit 4 */
    u8 unk8C_5:3;
    u8 unk8D[3];
    u32 unk90_0:11;
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9: value chosen when the card resolved (DNA Surgery type, an
                                     * attribute) */
    u32 unk92_2:14;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 unk2[6];
    u8 noBattleDamage:1;            /* +0x008 bit 0 */
    u8 battleProtected:1;           /* +0x008 bit 1 */
    u8 unk8_2:6;
    u8 unk9[0x24 - 0x9];
    u16 attackableMask;             /* +0x024: monster zones that may attack (BuildAttackableMask) */
    u16 attackedMask;               /* +0x026: monster zones that have attacked this turn */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 piles[0xD64 - 0x684];        /* +0x684: hand, deck, graveyard, fusion deck, banished */
};

struct DuelState {
    u16 serial;                     /* +0x0000: placement serial, stamped into DuelZone.serial */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13;
    u16 unk1B14_0:1;
    u16 interruptActive:1;          /* +0x1B14 bit 1 */
    u16 unk1B14_2:7;
    u32 battleStage:8;              /* +0x1B15 bit 1 .. +0x1B16 bit 0: enum BattleStage */
    u16 battleStep:8;               /* +0x1B16 bits 1-8: step inside the stage */
    u16 unk1B17_1:7;
};

struct DuelStateBattleWord {
    u32 unk0_0:9;                   /* +0x1B14 bits 0-8: unk1B14_0, interruptActive, unk1B14_2 */
    u32 battleStage:8;              /* bits 9-16: enum BattleStage */
    u32 battleStep:8;               /* bits 17-24 */
    u32 unk3_1:7;                   /* bits 25-31: low bits of battleArg0 */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelZone gDuelSpellTrapZones;     /* 0x020195F0 = gDuelPlayers[0].zones[ZONE_SPELL_0] */
extern const u16 gBounceScaleCurve[];           /* 0x081A43E4 */

void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 defense, u16 faceUp);
void AddCardToGraveyard(struct DuelCard *card);
void AddCardToBanished(struct DuelCard *card);
int CountActiveCardsOnField(int player, u16 cardNo);
int FindZoneLinkFromCard(int player, int zone, u16 cardNo);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);

/* duel_screen.h (the fields used here) */
struct DuelScreen {
    u8 fast:1;                      /* +0x000 bit 0: fast-forward card animations, as if B were held */
    u8 uiGfxLoaded:1;
    u8 active:1;
    u8 unk0_3:5;
    u8 unk1[0x808 - 0x1];
    u16 textTilesDirty:1;           /* +0x808 bit 0 */
    u16 textMapReset:1;
    u16 cursorDone:1;
    u16 showCursor:1;               /* +0x808 bit 3: draw the cursor sprite */
    u16 unk808_4:12;
};
extern struct DuelScreen gDuelScreen;           /* 0x0201CFB0 */
u32 DuelScreen_FadeOutStep(void);
void DuelCursor_Select(s32 player, s32 area, s32 index);
void DrawAllAreaTiles(void);
s32 GetAreaX(int player, int area, int index);
s32 GetAreaY(u32 player, int area, int index);

/* duel_cmd.h (the fields used here) */
struct DuelCmdEntry {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
};
struct DuelCmd {
    u16 cmd;                        /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                       /* +0x002: first operand */
    u16 arg4;                       /* +0x004: second operand */
    u16 arg6;                       /* +0x006: third operand */
    struct DuelCmdEntry queue[256]; /* +0x008 */
    u16 queueCount;                 /* +0x808 */
    u16 step:7;                     /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;
    u16 unk80A_14:2;
    u32 unk80C_0:5;                 /* +0x80C */
    u32 timer:7;                    /* +0x80C bits 5-11: frame timer of the current step */
    u32 unk80C_12:1;
    u32 running:1;                  /* +0x80D bit 5: cleared by the finished handler */
    u32 unk80C_14:18;
};
extern struct DuelCmd gDuelCmd;                 /* 0x020185C0 */
extern const u8 gDuelBannerPal[];               /* 16-colour OBJ palette shared by the duel banners */

/* battle.h */
struct Battle {
    u16 attacker:1;                 /* +0x0 bit 0: attacking player */
    u16 direct:1;                   /* +0x0 bit 1: direct attack */
    u16 flipEffectPending:1;
    u16 attackDeclared:1;
    u16 attackCostsPaid:1;
    u16 zeroAttackerAtk:1;          /* +0x0 bit 5: the attacker's ATK counts as 0 */
    u16 atkSlot:3;                  /* +0x0 bits 6-8: attacking monster's zone */
    u16 defSlot:3;                  /* +0x0 bits 9-11: target monster's zone (5 = direct attack) */
    u16 unk0_12:4;
};
extern struct Battle gBattle;                   /* 0x02018450 */
void MarkMonsterAttacked(int player, int zone);

/* chain.h */
struct ChainEntry {
    u16 card;                       /* +0x00: card ID */
    u8 player:1;                    /* +0x02 bit 0: activating player */
    u8 kind:3;
    u16 zone:6;
    u16 event:6;
    u8 unk4[0x14 - 0x4];
};
/* ---- END header subset ---- */

/* gDuel +0x1ACD as a byte (alias symbol): bit 0 equipMagicNegated, bit 1 equipMagicNegatedThisTurn. */
extern u8 gDuelNegationFlags;

extern const u8 gAttackBannerGfx[];         /* "Attack" banner: 4bpp tiles of a 64x32 sprite (0x400 bytes) */
extern const u8 gDirectAttackBannerGfx[];   /* "Direct Attack" banner: 4bpp tiles of a 64x32 sprite */

/* Acting player of the running command: bit 15 of the command word (DUEL_CMD_PLAYER). */
#define CMD_ACTING_PLAYER() (gDuelCmd.cmd >> 15)
#define CMD_FINISH()        (gDuelCmd.running = 0)

/*
 * In a link duel the partner's console (player 1) runs the battle stages of its own turn and sends their
 * effects over the link, so the battle-state writes below are skipped while it is player 1's turn there.
 */
#define IS_LINK_PARTNER_TURN() (gDuelCtrl.isLinkDuel && gDuel.turnPlayer)

/* gDuel.battleStage and battleStep reached from gDuelPlayers (= gDuel + 4) as one u32 container. Matching:
 * DuelCmd_EndBattlePhase addresses them from the gDuelPlayers symbol it already uses. */
#define PLAYERS_BATTLE_WORD (*(struct DuelStateBattleWord *)((u8 *)gDuelPlayers + 0x1B10))

/*
 * Zone pointers as explicit address arithmetic (0xD64 = sizeof(struct DuelPlayer), 0x94 = sizeof(struct
 * DuelZone)); callers mask the player. Matching: the order of the two products is the order the ROM
 * computes them in, which array indexing does not control.
 */
#define ZONE_AT(p, s)              ((struct DuelZone *)((u8 *)gDuelZones + ((s) * 0x94 + (p) * 0xD64)))
#define ZONE_AT_PLAYER_FIRST(p, s) ((struct DuelZone *)((u8 *)gDuelZones + ((p) * 0xD64 + (s) * 0x94)))
/* (gDuelZones + slot offset) + player offset: the form of IsValidEquipTarget and DuelCmd_ClearZoneCard. */
#define ZONE_AT_SLOT_THEN_PLAYER(p, s) ((struct DuelZone *)((u8 *)gDuelZones + (s) * 0x94 + (p) * 0xD64))

/* Card ID of the card word at zone (a zone, whose card word comes first, or a card). Matching: the ROM loads
 * the whole word (ldr and shifts); a member access (zone->card.id) loads only the halfword. */
#define ZONE_CARD_ID(zone) (((struct DuelCard *)(zone))->id)

/* links[] of a zone as bytes: the link's player is the low byte, read with its own ldrb. */
struct ZoneLinkBytes {
    u8 filler0[0xA];
    u8 linkBytes[64];               /* +0x0A: links[i] = linkBytes[2 * i] | linkBytes[2 * i + 1] << 8 */
};
#define ZONE_LINK_PLAYER(zone, i) (((struct ZoneLinkBytes *)(zone))->linkBytes[(i) * 2] & 1)

/* Card tables read through integer-constant addresses. Matching: with the symbols (card_data.h) the ROM's
 * literal pool and load order differ. Same tables, same bytes. */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber[id] */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats[id] */

/* Destination of the duel banner graphics: OBJ palette 15 and OBJ tiles 0x364-0x383 (64x32 sprite). */
#define BANNER_PAL_DST  (OBJ_PLTT + 15 * 0x20)
#define BANNER_GFX_DST  (OBJ_VRAM0 + 0x364 * 0x20)
#define BANNER_ATTR2    0xF364      /* palette 15, priority 0, tile 0x364 */
#define BANNER_YX       0x00300058  /* x 88, y 48 */
#define SWORD_ATTR2     0x5200      /* palette 5, priority 0, tile 0x200: the sword sprite */

/* Effective ATK of the card in (player, slot) (GetZoneCardStats). */
u32 GetZoneCardAtk(u32 player, u32 slot)
{
    struct ZoneCardStats stats;

    GetZoneCardStats(player, slot, &stats);
    return stats.atk;
}

/* Effective DEF of the card in (player, slot) (GetZoneCardStats). */
u32 GetZoneCardDef(u32 player, u32 slot)
{
    struct ZoneCardStats stats;

    GetZoneCardStats(player, slot, &stats);
    return stats.def;
}

/*
 * Effective monster type (enum CardType) of the card in (player, slot), 0 for an empty zone: the type in
 * gCardStats, replaced for a face-up monster (slots 0-4) by the newest (highest zone serial) of
 *  - a face-up Parasite Paracide in the player's monster zones that was planted (card bit 17): Insect;
 *  - a face-up, not disabled DNA Surgery in either player's spell/trap zones: the type it declared;
 * and then by Dragon if the monster is equipped (ZONE_LINK_EQUIP) with card number 1550 (not an EDS card),
 * newer than that, unless the equip is disabled, card 1537 is active on either field, or equip magic is
 * negated (gDuel +0x1ACD bits 0-1). Same rules as the type pass of GetZoneCardStats.
 */
u32 GetZoneCardType(s32 player, s32 slot)
{
    u32 playerOffset;
    u32 cardId;
    u16 newestSerial;   /* u16: the compare is ldrh/bls and the assignment re-reads the field */
    u32 type;
    int i, j;
    struct DuelZone *zone;
    u8 *zones;
    u32 slotOffset;

    /* Player offset, zone base and slot offset staged in ROM order before newestSerial = 0. */
    playerOffset = (player & 1) * 0xD64;
    zones = (u8 *)gDuelZones;
    slotOffset = slot * 0x94;
    newestSerial = 0;
    zone = (struct DuelZone *)(zones + (slotOffset + playerOffset));
    cardId = ZONE_CARD_ID(zone);
    if (cardId == 0)
        return 0;
    type = CARD_STATS_TYPE(CARD_STATS_WORD(cardId));
    if (slot > ZONE_MONSTER_4 || !zone->isFaceUp)
        return type;

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        u32 id;
        /* Separate pointer from the entry's zone (different pseudo, r2 here vs r3 there). */
        struct DuelZone *zone;

        /* Parasite Paracide in the player's monster zone i. Re-forming (player & 1) * 0xD64 lets CSE reuse
         * the entry's playerOffset (spilled to [sp+4]). */
        zone = ZONE_AT(player & 1, i);
        id = ZONE_CARD_ID(zone);
        if (id != 0 && CARD_NUMBER(id) == CARD_PARASITE_PARACIDE
            && ((struct DuelCard *)zone)->planted
            && zone->isFaceUp && zone->serial > newestSerial) {
            newestSerial = zone->serial;
            type = CARD_TYPE_INSECT;
        }

        /* DNA Surgery in spell/trap zone 5 + i of either player. */
        for (j = 0; j <= 1; j++) {
            zone = (struct DuelZone *)((u8 *)&gDuelSpellTrapZones + ((j & 1) * 0xD64 + i * 0x94));
            id = ZONE_CARD_ID(zone);
            if (id != 0 && CARD_NUMBER(id) == CARD_DNA_SURGERY && zone->isFaceUp
                && !zone->isDisabled && zone->serial > newestSerial) {
                newestSerial = zone->serial;
                type = zone->declaredValue;
            }
        }
    }

    {
        /* Pointer set outside the loop: reloaded before its ldrb, after the #3 mask. */
        u8 *equipNegated = &gDuelNegationFlags;

        for (i = 0; i < ZONE_AT_PLAYER_FIRST(player & 1, slot)->numLinks; i++) {
            u16 link;
            u8 kind;
            int linkZone, linkPlayer;
            struct DuelZone *equip;
            u16 equipId;
            u8 *zoneBase;

            /* FAKEMATCH: the links[] read goes through a local copy of the zone base, which
             * changes the guard's allocation priorities (slot * 0x94 in r1, base in r4). */
            zoneBase = (u8 *)gDuelZones;
            link = ((struct DuelZone *)(zoneBase + ((player & 1) * 0xD64 + slot * 0x94)))->links[i];
            kind = ZONE_AT_PLAYER_FIRST(player & 1, slot)->linkKinds[i];
            linkZone = DUEL_LOC_ZONE(link);
            linkPlayer = ZONE_LINK_PLAYER(ZONE_AT_PLAYER_FIRST(player & 1, slot), i);
            equip = ZONE_AT(linkPlayer, linkZone);
            equipId = ZONE_CARD_ID(equip);
            /* Unlike the two overrides above, a match does not update newestSerial. */
            if (kind == ZONE_LINK_EQUIP && equipId != 0
                && !equip->isDisabled
                && CountActiveCardsOnField(0, CARD_1537) == 0
                && CountActiveCardsOnField(1, CARD_1537) == 0
                && !(*equipNegated & 3)
                && CARD_NUMBER(equipId) == CARD_1550
                && equip->serial > newestSerial)
                type = CARD_TYPE_DRAGON;
        }
    }
    return type;
}

/*
 * Effective attribute (enum CardAttribute) of the card in (player, slot), 0 for an empty zone: the attribute
 * in gCardStats, replaced for a face-up monster (slots 0-4) equipped with card number 1448 (not an EDS card)
 * by the value that equip declared, unless the equip is disabled, card 1537 is active on either field, or
 * equip magic is negated (gDuel +0x1ACD bits 0-1).
 */
u32 GetZoneCardAttribute(s32 player, s32 slot)
{
    u32 cardId;
    u32 attribute;
    int i;
    u32 playerOffset;
    u8 *zones;

    /* The player offset and the zone base are staged before the slot offset (ROM order). */
    playerOffset = (player & 1) * 0xD64;
    zones = (u8 *)gDuelZones;
    cardId = ZONE_CARD_ID(zones + (slot * 0x94 + playerOffset));
    if (cardId == 0)
        return 0;
    attribute = CARD_STATS_ATTR(CARD_STATS_WORD(cardId));
    if (slot > ZONE_MONSTER_4 || !ZONE_AT(player & 1, slot)->isFaceUp)
        return attribute;
    {
        /* Pointer set outside the loop: reloaded before its ldrb, after the #3 mask. */
        u8 *equipNegated = &gDuelNegationFlags;

        for (i = 0; i < ZONE_AT(player & 1, slot)->numLinks; i++) {
            u16 link;
            u8 kind;
            int linkZone, linkPlayer;
            struct DuelZone *equip;
            u16 equipId;

            /* FAKEMATCH: an extra use of the zone base the loop hoists, so global alloc keeps it
             * in r9 and spills the hoisted linkKinds pointer to [sp] instead. */
            asm("" :: "r"(gDuelZones));
            link = ZONE_AT(player & 1, slot)->links[i];
            kind = ZONE_AT(player & 1, slot)->linkKinds[i];
            linkZone = DUEL_LOC_ZONE(link);
            linkPlayer = ZONE_LINK_PLAYER(ZONE_AT(player & 1, slot), i);
            equip = ZONE_AT(linkPlayer, linkZone);
            equipId = ZONE_CARD_ID(equip);
            if (kind == ZONE_LINK_EQUIP && equipId != 0
                && !equip->isDisabled
                && CountActiveCardsOnField(0, CARD_1537) == 0
                && CountActiveCardsOnField(1, CARD_1537) == 0
                && !(*equipNegated & 3)
                && CARD_NUMBER(equipId) == CARD_1448)
                attribute = equip->declaredValue;
        }
    }
    return attribute;
}

/*
 * 1 if a face-up monster (either player, slots 0-4) has a zone link from a card with the same card number as
 * the card in (player, slot) (FindZoneLinkFromCard), else 0; 0 for an empty zone. Used to require that a
 * face-up equip card is attached to a monster.
 */
s32 IsCardLinkedToMonster(s32 player, s32 slot)
{
    const u16 *cardNumbers = gCardIdToNumber;
    u16 cardId;
    s32 p, s;

    cardId = ZONE_CARD_ID(ZONE_AT_PLAYER_FIRST(player & 1, slot));
    if (cardId == 0)
        return 0;
    for (p = 0; p <= 1; p++) {
        for (s = 0; s < MONSTER_ZONE_COUNT; s++) {
            struct DuelZone *zone = ZONE_AT(p & 1, s);

            if (ZONE_CARD_ID(zone) == 0)
                continue;
            if (!zone->isFaceUp)
                continue;
            if (FindZoneLinkFromCard(p, s, cardNumbers[cardId & CARD_ID_MASK]) != -1)
                return 1;
        }
    }
    return 0;
}

/*
 * Nonzero if the equip card in (equipPlayer, equipSlot) may be equipped to the monster in (targetPlayer,
 * targetSlot): EffectEquipTargetCheck with a chain entry that carries only the card ID and the player.
 */
u16 IsValidEquipTarget(u32 equipPlayer, u32 equipSlot, u32 targetPlayer, u32 targetSlot)
{
    struct ChainEntry entry;

    entry.card = ZONE_CARD_ID(ZONE_AT_SLOT_THEN_PLAYER(equipPlayer & 1, equipSlot));
    entry.player = equipPlayer;
    /* DUEL_LOC(targetPlayer, targetSlot), written with the operands in the ROM's order. */
    return EffectEquipTargetCheck(&entry, (u8)targetPlayer | ((u8)targetSlot << 8));
}

/* Number of monster zones (both players, slots 0-4) the equip card in (equipPlayer, equipSlot) may equip. */
int CountValidEquipTargets(u32 equipPlayer, u32 equipSlot)
{
    int count = 0;
    int p, s;

    for (p = 0; p <= 1; p++)
        for (s = 0; s < MONSTER_ZONE_COUNT; s++)
            if (IsValidEquipTarget(equipPlayer, equipSlot, p, s))
                count++;
    return count;
}

/*
 * DUEL_LOC of the first face-up monster (player 0 first, slots 0-4) with a zone link from a card with the same
 * card number as the card in (player, slot), or 0xFFFF; 0 for an empty zone (the same value as player 0's
 * zone 0). Matches by card number, so two copies of the same equip are not told apart.
 */
u16 FindMonsterLinkedToCard(s32 player, s32 slot)
{
    const u16 *cardNumbers = gCardIdToNumber;
    u16 cardId;
    s32 p, s;

    cardId = ZONE_CARD_ID(ZONE_AT_PLAYER_FIRST(player & 1, slot));
    if (cardId == 0)
        return 0;
    for (p = 0; p <= 1; p++) {
        for (s = 0; s < MONSTER_ZONE_COUNT; s++) {
            struct DuelZone *zone = ZONE_AT(p & 1, s);

            if (ZONE_CARD_ID(zone) == 0)
                continue;
            if (!zone->isFaceUp)
                continue;
            if (FindZoneLinkFromCard(p, s, cardNumbers[cardId & CARD_ID_MASK]) != -1)
                return (u8)p | ((u8)s << 8);     /* DUEL_LOC(p, s) in the ROM's operand order */
        }
    }
    return 0xFFFF;
}

/*
 * DUEL_CMD_ATTACK (arg2: attacker zone of the acting player, arg4: target zone of the other player).
 * Steps: 0 point the cursor at the attacker, 1 at the target, 2 hide the cursor, 3 spin a sword sprite over
 * the attacker (32 frames), 4 fly it to the target while it bounces in size (32 frames), 5 load the "Attack"
 * banner, 6 show the banner pulsing for 96 frames. B held or gDuelScreen.fast skips ahead (+7 per frame
 * for the sword, +3 for the banner).
 */
void DuelCmd_Attack(void)
{
    u32 player = CMD_ACTING_PLAYER();
    u16 attackerZone = gDuelCmd.arg2;
    u16 targetZone = gDuelCmd.arg4;
    s32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 1;
        DuelCursor_Select(player, DUEL_AREA_MONSTER, attackerZone);
        gDuelCmd.step++;
        break;
    case 1:
        gDuelScreen.showCursor = 1;
        DuelCursor_Select(1 - player, DUEL_AREA_MONSTER, targetZone);
        gDuelCmd.step++;
        break;
    case 2:
        gDuelScreen.showCursor = 0;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 3: {
        /* Sword over the attacker at scale 1.0, turning 4 of the 128 angle steps per frame (player 1's
         * starts half a turn round). */
        s32 x = GetAreaX(player, DUEL_AREA_MONSTER, attackerZone);
        s32 y = GetAreaY(player, DUEL_AREA_MONSTER, attackerZone);
        u32 yx = (u32)(x + 8) | ((u32)(y + 8) << 16);
        s32 t;

        AddAffineSprite(yx, SPRITE_SHAPE_16x16, SWORD_ATTR2,
                        (gDuelCmd.timer * 4 + (player ? 0x40 : 0)) | (0x100 << 16));
        gDuelCmd.timer++;
        t = gDuelCmd.timer;
        if (t <= 31) {
            if ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)
                if (t <= 23)
                    gDuelCmd.timer += 7;
        } else {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    }
    case 4: {
        /* Sword from the attacker to the target, position interpolated over 32 frames, scale from
         * gBounceScaleCurve. dy ends up holding the packed position (y << 16 | x). */
        u32 other = 1 - player;
        s32 dx = GetAreaX(other, DUEL_AREA_MONSTER, targetZone);
        s32 dy = GetAreaY(other, DUEL_AREA_MONSTER, targetZone);
        s32 t;

        dx -= GetAreaX(player, DUEL_AREA_MONSTER, attackerZone);
        dy -= GetAreaY(player, DUEL_AREA_MONSTER, attackerZone);
        dx *= gDuelCmd.timer;
        dy *= gDuelCmd.timer;
        dx /= 32;
        dy /= 32;
        dx += GetAreaX(player, DUEL_AREA_MONSTER, attackerZone) + 8;
        dy += GetAreaY(player, DUEL_AREA_MONSTER, attackerZone) + 8;
        dy = (dy << 16) | dx;
        AddAffineSprite(dy, SPRITE_SHAPE_16x16, SWORD_ATTR2,
                        ((u32)gBounceScaleCurve[gDuelCmd.timer] << 16) | (player ? 0x40 : 0));
        gDuelCmd.timer++;
        t = gDuelCmd.timer;
        if (t <= 31) {
            if ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)
                if (t <= 23)
                    gDuelCmd.timer += 7;
        } else {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    }
    case 5:
        CopyDoubleWords((void *)BANNER_PAL_DST, gDuelBannerPal, 0x20);
        CopyDoubleWords((void *)BANNER_GFX_DST, gAttackBannerGfx, 0x400);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 6: {
        s32 t = gDuelCmd.timer;

        if (t <= 95) {
            AddAffineSprite(BANNER_YX, SPRITE_SHAPE_64x32, BANNER_ATTR2, (u32)gBounceScaleCurve[t & 31] << 16);
            gDuelCmd.timer++;
            if ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)
                if (gDuelCmd.timer <= 87)
                    gDuelCmd.timer += 3;
            break;
        }
    }
    /* fallthrough: banner done */
    default:
        CMD_FINISH();
        break;
    }
}

/*
 * DUEL_CMD_DIRECT_ATTACK (arg2: attacker zone). Steps: 0 point the cursor at the attacker, 1 hide it and load
 * the "Direct Attack" banner, 2 show the banner pulsing for 96 frames (+7 per frame while B is held or
 * gDuelScreen.fast is set).
 */
void DuelCmd_DirectAttack(void)
{
    u32 player = CMD_ACTING_PLAYER();
    u16 attackerZone = gDuelCmd.arg2;
    s32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 1;
        DuelCursor_Select(player, DUEL_AREA_MONSTER, attackerZone);
        gDuelCmd.step++;
        break;
    case 1:
        gDuelScreen.showCursor = 0;
        CopyDoubleWords((void *)BANNER_PAL_DST, gDuelBannerPal, 0x20);
        CopyDoubleWords((void *)BANNER_GFX_DST, gDirectAttackBannerGfx, 0x400);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2: {
        s32 t = gDuelCmd.timer;

        if (t <= 95) {
            AddAffineSprite(BANNER_YX, SPRITE_SHAPE_64x32, BANNER_ATTR2, gBounceScaleCurve[t & 31] << 16);
            gDuelCmd.timer++;
            if ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast) {
                if (gDuelCmd.timer <= 87)
                    gDuelCmd.timer += 7;
            }
            return;
        }
    }
    /* fallthrough: banner done */
    default:
        CMD_FINISH();
        return;
    }
}

/*
 * DUEL_CMD_PREPARE_BATTLE_PHASE: for each monster zone of the acting player, "cannot attack next turn"
 * becomes "cannot attack" and destroyAfterBattle is cleared; a monster with card number 1336 (not an EDS
 * card) gets effectUnused set (its extra attack). Finishes at once.
 */
void DuelCmd_PrepareBattlePhase(void)
{
    struct DuelCmd *cmdTmp = &gDuelCmd;
    s32 i = 0;
    struct DuelCmd *cmd;
    u32 playerOffset;
    u8 *zones;
    u8 *playerZones;

    /* FAKEMATCH: the ROM loads &gDuelCmd before i = 0 and copies it into the long-lived cmd register
     * afterwards; a memory clobber between the two stops combine from merging the load into the copy. */
    asm volatile("" ::: "memory");
    cmd = cmdTmp;
    playerOffset = (cmd->cmd >> 15) * 0xD64;
    zones = (u8 *)gDuelZones;
    playerZones = zones + playerOffset;

    do {
        u32 zoneOffset = i * 0x94;
        struct DuelZone *zone = (struct DuelZone *)(playerZones + zoneOffset);
        struct DuelZone *sameZone;

        if (zone->cannotAttackNextTurn) {
            zone->cannotAttackNextTurn = 0;
            zone->cannotAttack = 1;
        }
        zone->destroyAfterBattle = 0;
        /* The same zone again: the integer sum keeps the ROM's (zoneOffset + playerOffset) + base order. */
        sameZone = (struct DuelZone *)(zoneOffset + playerOffset + (u32)zones);
        /* A one-case switch keeps the 1336 constant inside the loop (an == compare gets hoisted). */
        switch (CARD_NUMBER(ZONE_CARD_ID(sameZone))) {
        case CARD_1336:
            sameZone->effectUnused = 1;
        }
        i++;
    } while (i < MONSTER_ZONE_COUNT);
    cmd->running = 0;
}

/* DUEL_CMD_MARK_ATTACKED (arg2: zone): the acting player's monster there has attacked (MarkMonsterAttacked). */
void DuelCmd_MarkAttacked(void)
{
    MarkMonsterAttacked(CMD_ACTING_PLAYER(), gDuelCmd.arg2);
    CMD_FINISH();
}

/* DUEL_CMD_START_BATTLE_SCENE (arg2, arg4: card IDs of player 0's and player 1's card): fade the field out,
 * then set up the battle scene. */
void DuelCmd_StartBattleScene(void)
{
    if (DuelScreen_FadeOutStep()) {
        BattleScene_Init(gDuelCmd.arg2, gDuelCmd.arg4);
        CMD_FINISH();
    }
}

/* DUEL_CMD_PLAY_BATTLE_SCENE (arg2, arg4: battle values of player 0 / 1, arg6: per-side flags): run the
 * battle scene until BattleScene_Update reports it done. */
void DuelCmd_PlayBattleScene(void)
{
    if (BattleScene_Update(gDuelCmd.arg2, gDuelCmd.arg4, gDuelCmd.arg6))
        CMD_FINISH();
}

/* DUEL_CMD_SET_BATTLE_PROTECTION (Waboku): arg2 sets the acting player's noBattleDamage, arg4 its
 * battleProtected (its monsters are not destroyed by battle and it takes no battle damage). */
void DuelCmd_SetBattleProtection(void)
{
    u8 player = CMD_ACTING_PLAYER();

    if (gDuelCmd.arg2)
        gDuelPlayers[player].noBattleDamage = 1;
    if (gDuelCmd.arg4)
        gDuelPlayers[player].battleProtected = 1;
    CMD_FINISH();
}

/* DUEL_CMD_END_BATTLE_PHASE: no monster may attack any more; the battle stages jump to BATTLE_STAGE_END. */
void DuelCmd_EndBattlePhase(void)
{
    if (!IS_LINK_PARTNER_TURN()) {
        gDuelPlayers[0].attackableMask = 0;
        gDuelPlayers[1].attackableMask = 0;
        PLAYERS_BATTLE_WORD.battleStage = BATTLE_STAGE_END;
    }
    CMD_FINISH();
}

/* DUEL_CMD_SET_ATTACK_TARGET (arg2: target loc, arg4: restart): redirect the attack to the zone of arg2; with
 * arg4 the battle restarts at BATTLE_STAGE_REVEAL_DEFENDER. */
void DuelCmd_SetAttackTarget(void)
{
    if (!IS_LINK_PARTNER_TURN()) {
        gBattle.defSlot = (u8)DUEL_LOC_ZONE(gDuelCmd.arg2);     /* (u8): the ROM narrows before the store */
        if (gDuelCmd.arg4) {
            gDuel.battleStage = BATTLE_STAGE_REVEAL_DEFENDER;
            gDuel.battleStep = 0;
        }
    }
    CMD_FINISH();
}

/* DUEL_CMD_SET_ATTACKER (arg2: attacker loc, arg4: restart): the monster in the zone of arg2 becomes the
 * attacker; with arg4 the battle restarts at BATTLE_STAGE_REVEAL_DEFENDER. */
void DuelCmd_SetAttacker(void)
{
    if (!IS_LINK_PARTNER_TURN()) {
        gBattle.atkSlot = DUEL_LOC_ZONE(gDuelCmd.arg2);
        if (gDuelCmd.arg4) {
            gDuel.battleStage = BATTLE_STAGE_REVEAL_DEFENDER;
            gDuel.battleStep = 0;
        }
    }
    CMD_FINISH();
}

/* DUEL_CMD_ZERO_ATTACKER_ATK: the attacker's ATK counts as 0 in this battle. */
void DuelCmd_ZeroAttackerAtk(void)
{
    gBattle.zeroAttackerAtk = 1;
    CMD_FINISH();
}

/* DUEL_CMD_NEGATE_ATTACK (arg2: attacker zone): skip to BATTLE_STAGE_END_ATTACK and mark the attacker as
 * having attacked. */
void DuelCmd_NegateAttack(void)
{
    u32 player = CMD_ACTING_PLAYER();
    u16 attackerZone = gDuelCmd.arg2;

    if (!IS_LINK_PARTNER_TURN()) {
        gDuel.battleStage = BATTLE_STAGE_END_ATTACK;
        gDuel.battleStep = 0;
    }
    MarkMonsterAttacked(player, attackerZone);
    CMD_FINISH();
}

/* DUEL_CMD_PLACE_CARD (arg2: zone | faceUp << 8 | defense << 9, arg4 | arg6 << 16: card word): put the card
 * into the acting player's zone without animation (PlaceMonsterCard) and redraw the field. */
void DuelCmd_PlaceCard(void)
{
    u32 player = CMD_ACTING_PLAYER();
    u8 zone = gDuelCmd.arg2;
    u8 flags = gDuelCmd.arg2 >> 8;
    u16 faceUp = flags & 1;
    u16 defense = (flags & 2) >> 1;
    u32 cardWord = (gDuelCmd.arg6 << 16) | gDuelCmd.arg4;

    PlaceMonsterCard(player, zone, (struct DuelCard *)&cardWord, defense, faceUp);
    DrawAllAreaTiles();
    CMD_FINISH();
}

/* DUEL_CMD_CLEAR_ZONE_CARD (arg2: zone): clear the card ID of the acting player's zone (the rest of the zone
 * is kept) and redraw the field. */
void DuelCmd_ClearZoneCard(void)
{
    ZONE_CARD_ID(ZONE_AT_SLOT_THEN_PLAYER(CMD_ACTING_PLAYER(), gDuelCmd.arg2)) = 0;
    DrawAllAreaTiles();
    CMD_FINISH();
}

/* DUEL_CMD_ADD_CARD_TO_GRAVEYARD (arg2 | arg4 << 16: card word): add the card to its owner's graveyard
 * (nothing for card ID 0) and redraw the field. */
void DuelCmd_AddCardToGraveyard(void)
{
    u32 cardWord = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    if (((struct DuelCard *)&cardWord)->id)
        AddCardToGraveyard((struct DuelCard *)&cardWord);
    DrawAllAreaTiles();
    CMD_FINISH();
}

/* DUEL_CMD_ADD_CARD_TO_BANISHED (arg2 | arg4 << 16: card word): add the card to its owner's banished pile
 * (nothing for card ID 0) and redraw the field. */
void DuelCmd_AddCardToBanished(void)
{
    u32 cardWord = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    if (((struct DuelCard *)&cardWord)->id)
        AddCardToBanished((struct DuelCard *)&cardWord);
    DrawAllAreaTiles();
    CMD_FINISH();
}
