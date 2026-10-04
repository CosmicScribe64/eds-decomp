/*
 * Campaign scene runner and its last steps, the battle calculation, and the card command menu
 * (wiki/functions/campaign-c.md):
 *  - CB_Campaign runs gCampaignSteps (enum CampaignStep); this unit has steps 7 (Campaign_DeliverMagazines:
 *    the Weekly Yu-Gi-Oh! and Yu-Gi-Oh! Magazine days give a pack), 8 (Campaign_AdvanceDay) and 10 (the
 *    deck-too-small message);
 *  - CalcBattle fills gBattle for an attack: both monsters' values with the card boosts, which monsters
 *    are destroyed and the life-point damage;
 *  - CardMenu_*: the box of command icons that opens over the card under the duel cursor, with a zoomed
 *    preview of the card and the selected command's name. CardMenu_Update slides it in and out, moves the
 *    selection with Left/Right, opens Card View, and leaves a confirmed command to CardMenu_Execute.
 */
#include "global.h"
#include "gba.h"                   /* A_BUTTON, B_BUTTON, DPAD_LEFT, DPAD_RIGHT */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelArea, ZoneLinkKind, CardMenuCommand */
#include "constants/game.h"         /* enum BoosterPackId */
#include "constants/sound.h"        /* SE_* */
#include "card_data.h"              /* CARD_ID_MASK */
#include "save.h"                   /* gSaveData.days */
#include "calendar.h"               /* struct Date, GetCurrentDate, GetCalendarEvents, enum CalendarEvent */
#include "campaign.h"               /* the steps defined here, enum CampaignStep */
#include "util.h"                   /* MemCopy16, HalveRoundUp, Random */
#include "palette.h"                /* FadeToBlack */
#include "sprite.h"                 /* AddSprite, AddAffineSprite, enum SpriteShape */
#include "bustup.h"                 /* StartDialogue, CB_Bustup */
#include "booster.h"                /* GetRewardPack */
#include "card_detail.h"            /* CardDetail_Init, CardDetail_Run */
#include "card_menu.h"              /* the CardMenu_* functions defined here, enum CardMenuState */
#include "duel_actions.h"           /* QueueAddZoneLink */
#include "duel_flow.h"              /* gPulseScaleCurve */
#include "main.h"                  /* struct Main gMain */
#include "duel.h"                  /* gDuel, duel structs, GetZoneCardStats, ... */
#include "sound.h"                 /* PlaySE */
#include "battle.h"                 /* gBattle */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_* */
#include "duel_cmd.h"               /* gShrinkScaleSteps */
#include "card_list_view.h"         /* CardListView_Open */
#include "battle_scene.h"           /* struct BattleScene (gBattle.scene) */

/* ---- Local data and views ---- */

/* A scene step: returns nonzero when done, and the runner moves on to the next entry. */
typedef u16 (*StepFunc)(void);
extern const StepFunc gCampaignSteps[];     /* 0x08198EAC: CB_Campaign's steps (enum CampaignStep) */

/*
 * Matching: the Campaign steps and CardMenu_Update test CB_Bustup and DuelScreen_FadeInStep (u16 in
 * bustup.h and duel_screen.h) as whole words (no lsl #16 after the call).
 */
u32 CB_BustupU32(void) asm("CB_Bustup");
u32 DuelScreen_FadeInStepU32(void) asm("DuelScreen_FadeInStep");

/* Matching: gCardIdToNumber[id] read through its integer address (0x08622AB4); the symbol form changes
 * CalcBattle. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* ---- Campaign steps 7, 8, 10 and the runner ---- */

/* Campaign_DeliverMagazines sub-steps (gMain.subStep). */
enum {
    MAGAZINE_STEP_CHECK = 0,            /* today's events; return 1 on a day without a magazine */
    MAGAZINE_STEP_WEEKLY = 1,           /* Weekly Yu-Gi-Oh! text (skips to 4 on a V Jump-only day) */
    MAGAZINE_STEP_WEEKLY_TEXT = 2,
    MAGAZINE_STEP_WEEKLY_PACK = 3,
    MAGAZINE_STEP_V_JUMP = 4,           /* Yu-Gi-Oh! Magazine text, or return 1 */
    MAGAZINE_STEP_V_JUMP_TEXT = 5,
    MAGAZINE_STEP_V_JUMP_PACK = 6,
};

/*
 * Campaign step 7. On a Weekly Yu-Gi-Oh! day (CAL_WEEKLY_JUMP) text 803 and a pack: 801 or, three times in
 * four, 901 (on 2001-01-09 text 802 and always 801). On a Yu-Gi-Oh! Magazine day (CAL_V_JUMP) text 801 (800
 * in January 2001) and pack 802. Today's events go to gMain.events. Returns 1 when done.
 */
u16 Campaign_DeliverMagazines(void)
{
    struct Date date;

    switch (gMain.subStep) {
    case MAGAZINE_STEP_CHECK:
        GetCurrentDate(&date);
        gMain.events = GetCalendarEvents(date.year, date.month, date.day);
        if (!(gMain.events & (CAL_WEEKLY_JUMP | CAL_V_JUMP)))
            return 1;
        gMain.subStep++;
        /* fall through */
    case MAGAZINE_STEP_WEEKLY:
        if (!(gMain.events & CAL_WEEKLY_JUMP)) {
            gMain.subStep++;
            gMain.subStep++;
            gMain.subStep++;
            return 0;
        }
        PlayBGM(0x1F);
        StartDialogue(803);
        GetCurrentDate(&date);
        /* gDuel.result (the finished duel's, no longer needed) holds the pack choice: 0 = pack 801. */
        gDuel.result = Random() & 3;
        if (date.year == 2001 && date.month == 1 && date.day == 9) {
            StartDialogue(802);
            gDuel.result = 0;
        }
        gMain.subStep++;
        /* fall through */
    case MAGAZINE_STEP_WEEKLY_TEXT:
    case MAGAZINE_STEP_V_JUMP_TEXT:
        if (CB_BustupU32()) {
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.subStep++;
        }
        return 0;
    case MAGAZINE_STEP_WEEKLY_PACK:
        if (gDuel.result == 0) {
            if (GetRewardPack(PACK_WEEKLY_YUGIOH)) {
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
                gMain.subStep++;
            }
        } else {
            if (GetRewardPack(PACK_WEEKLY_YUGIOH_B)) {
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
                gMain.subStep++;
            }
        }
        return 0;
    case MAGAZINE_STEP_V_JUMP:
        if (!(gMain.events & CAL_V_JUMP))
            return 1;
        PlayBGM(0x1F);
        StartDialogue(801);
        gMain.subStep++;
        GetCurrentDate(&date);
        if (date.year == 2001 && date.month == 1)
            StartDialogue(800);
        return 0;
    case MAGAZINE_STEP_V_JUMP_PACK:
        return GetRewardPack(PACK_YUGIOH_MAGAZINE);
    }
    return 1;
}

/* Campaign step 8: every Campaign duel moves the calendar on by one day. Returns 1. */
u16 Campaign_AdvanceDay(void)
{
    gSaveData.days++;
    return 1;
}

/* Campaign step 10: text 401 ("your Deck must contain 40 cards or more"); returns 1 when it is closed. */
u16 Campaign_DeckTooSmall(void)
{
    switch (gMain.subStep) {
    case 0:
        StartDialogue(401);
        gMain.subStep++;
        break;
    case 1:
        if (CB_BustupU32()) {
            gMain.subStep++;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

/*
 * Scene callback of main-menu slot 0: runs gCampaignSteps[gMain.seqIndexCampaign] and moves to the next
 * step when it returns nonzero. At a NULL entry the day is over: fade to black, then return 1.
 */
u16 CB_Campaign(void)
{
    StepFunc step = gCampaignSteps[gMain.seqIndexCampaign];

    if (step != NULL) {
        if (step()) {
            gMain.seqIndexCampaign++;
            gMain.subStep = 0;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return FadeToBlack(8);
}

/* ---- Battle calculation ---- */

/*
 * Matching: CalcBattle writes the BattleSide flags through u16 containers; battle.h has u8 containers for
 * destroyed, defensePos and effectDestroy, which changes the read-modify-write code. So the function uses
 * this view of gBattle (struct Battle up to the zone copies, with u16 side flags).
 */
struct BattleSideU16 {
    u16 slot:3;                         /* +0x0 bits 0-2 */
    u16 destroyed:1;                    /* +0x0 bit 3 */
    u16 defensePos:1;                   /* +0x0 bit 4 */
    u16 destroyedCopy:1;                /* +0x0 bit 5 */
    u16 effectDestroy:1;                /* +0x0 bit 6 */
    u16 unk0_7:1;
    u8 unk1;
    u16 cardId;                         /* +0x2 */
    u16 atk;                            /* +0x4 */
    u16 def;                            /* +0x6 */
    u16 battleValue;                    /* +0x8 */
    u16 damage;                         /* +0xA */
};
struct BattleView {
    u16 attacker:1;                     /* +0x0: the fields of struct Battle */
    u16 direct:1;
    u16 flipEffectPending:1;
    u16 attackDeclared:1;
    u16 attackCostsPaid:1;
    u16 zeroAttackerAtk:1;
    u16 atkSlot:3;
    u16 defSlot:3;
    u16 unk0_12:4;
    u16 flipCardId;                     /* +0x2 */
    u8 calculated:1;                    /* +0x4 */
    u8 unk4_1:7;
    u8 unk5[3];
    struct BattleSideU16 side[2];       /* +0x08 */
    struct DuelZone zones[2];           /* +0x20 */
};
STATIC_ASSERT(sizeof(struct BattleSideU16) == sizeof(struct BattleSide), BattleSideU16Size);
STATIC_ASSERT(OFFSET_OF(struct BattleView, zones) == OFFSET_OF(struct Battle, zones), BattleViewZones);
extern struct BattleView gBattleView asm("gBattle");

#define ATTACKER_SIDE (gBattleView.side[attacker])
#define DEFENDER_SIDE (gBattleView.side[1 - attacker])
#define ZONE_AT(player, zone) (&gDuelPlayers[player].zones[zone])

/*
 * Fill gBattle.side[] for an attack of the monster in zone gBattle.atkSlot of `attacker` on zone
 * gBattle.defSlot of the other player: copies of both zones, card IDs, ATK and DEF, the compared values,
 * the destroyed flags and the life-point damage. zeroAtk (or gBattle.zeroAttackerAtk) makes the attacker's
 * ATK 0. A direct attack deals the attacker's ATK to the defending player. Otherwise the attacker's ATK is
 * compared with the defender's ATK, or its DEF in defense position:
 *  - higher: the defender is destroyed; its player takes the difference if it was in attack position or
 *    the attacker pierces;
 *  - lower: the attacker's player takes the difference; the attacker is destroyed if the defender is in
 *    attack position;
 *  - equal: both are destroyed, unless the defender is in defense position or the values are 0.
 * Card rules (by card number) change the values and the outcome. A battle-protected defending player loses
 * neither the monster nor life points, and card 1423 on a player's field stops damage to that player.
 */
void CalcBattle(int attacker, u16 zeroAtk)
{
    int i;
    int slot;

    gBattleView.attacker = attacker;
    gBattleView.calculated = 1;
    for (i = 0; i <= 1; i++) {
        struct BattleSideU16 *side = &gBattleView.side[i];
        struct DuelZone *copy = &gBattleView.zones[i];
        if (i == attacker)
            slot = gBattleView.atkSlot;
        else
            slot = gBattleView.defSlot;
        MemCopy16(copy, ZONE_AT(i & 1, slot), sizeof(struct DuelZone));
        side->slot = slot;
        side->destroyed = 0;
        side->defensePos = ZONE_AT(i & 1, slot)->isDefense;
        /* Matching: the card word is read through a DuelCard pointer (a word load). */
        side->cardId = ((struct DuelCard *)ZONE_AT(i & 1, slot))->id;
        side->atk = GetZoneCardAtk(i, slot);
        side->def = GetZoneCardDef(i, slot);
        side->damage = 0;
    }
    /* FAKEMATCH: i is 2 here, so `i != attacker` is always true. */
    if (i != attacker && gBattleView.direct) {
        DEFENDER_SIDE.cardId = 0;
        DEFENDER_SIDE.atk = 0;
        DEFENDER_SIDE.def = 0;
    }
    if (gBattleView.zeroAttackerAtk)
        ATTACKER_SIDE.atk = 0;
    ATTACKER_SIDE.battleValue = ATTACKER_SIDE.atk;
    DEFENDER_SIDE.battleValue = DEFENDER_SIDE.atk;
    ATTACKER_SIDE.defensePos = 0;
    if (gBattleView.direct) {
        DEFENDER_SIDE.damage = ATTACKER_SIDE.atk;
        DEFENDER_SIDE.battleValue = 0;
        if (DEFENDER_SIDE.damage && CountActiveCardsOnFieldExcept(1 - attacker, CARD_1423, -1) > 0)
            DEFENDER_SIDE.damage = 0;
        return;
    }

    /* Boosts for this battle. */
    switch (CARD_NUMBER(ATTACKER_SIDE.cardId)) {
    case CARD_INSECT_SOLDIERS_OF_THE_SKY:
        if (GetZoneCardAttribute(1 - attacker, gBattleView.defSlot) == ATTRIBUTE_WIND)
            ATTACKER_SIDE.atk += 1000;
        break;
    case 1253:  /* no EDS card */
        if (GetZoneCardType(1 - attacker, gBattleView.defSlot) == CARD_TYPE_WARRIOR) {
            ATTACKER_SIDE.atk += 2000;
            ATTACKER_SIDE.def += 2000;
        }
        break;
    }
    switch (CARD_NUMBER(DEFENDER_SIDE.cardId)) {
    case CARD_DARK_ARTIST:
        if (GetZoneCardAttribute(attacker, gBattleView.atkSlot) == ATTRIBUTE_LIGHT)
            DEFENDER_SIDE.def /= 2;
        break;
    case 1253:  /* no EDS card */
        if (GetZoneCardType(attacker, gBattleView.atkSlot) == CARD_TYPE_WARRIOR) {
            DEFENDER_SIDE.atk += 2000;
            DEFENDER_SIDE.def += 2000;
        }
        break;
    }
    if (zeroAtk)
        ATTACKER_SIDE.atk = 0;
    ATTACKER_SIDE.battleValue = ATTACKER_SIDE.atk;
    DEFENDER_SIDE.battleValue = DEFENDER_SIDE.atk;
    if (DEFENDER_SIDE.defensePos)
        DEFENDER_SIDE.battleValue = DEFENDER_SIDE.def;
    {
        /* Each Metalmorph on the attacker adds half of the defender's ATK. */
        int metalmorphs = CountActiveZoneLinksFromCard(attacker, gBattleView.atkSlot, CARD_METALMORPH);
        ATTACKER_SIDE.battleValue += HalveRoundUp(DEFENDER_SIDE.atk) * metalmorphs;
    }

    /* The comparison. */
    if (ATTACKER_SIDE.battleValue == DEFENDER_SIDE.battleValue) {
        if (!DEFENDER_SIDE.defensePos && ATTACKER_SIDE.battleValue) {
            ATTACKER_SIDE.destroyed = 1;
            DEFENDER_SIDE.destroyed = 1;
        }
    } else if (ATTACKER_SIDE.battleValue > DEFENDER_SIDE.battleValue) {
        int piercing = 0;
        if (!DEFENDER_SIDE.defensePos)
            piercing = 1;
        if (CountActiveZoneLinksFromCard(attacker, gBattleView.atkSlot, CARD_1313))
            piercing = 1;
        if (CARD_NUMBER(ATTACKER_SIDE.cardId) == 1341)     /* no EDS card */
            piercing = 1;
        if (CountActiveZoneLinksFromCard(attacker, gBattleView.atkSlot, CARD_1540))
            piercing = 1;
        if (piercing)
            DEFENDER_SIDE.damage = ATTACKER_SIDE.battleValue - DEFENDER_SIDE.battleValue;
        DEFENDER_SIDE.destroyed = 1;
    } else {
        ATTACKER_SIDE.damage = DEFENDER_SIDE.battleValue - ATTACKER_SIDE.battleValue;
        if (!DEFENDER_SIDE.defensePos)
            ATTACKER_SIDE.destroyed = 1;
    }

    /* Card rules after the comparison. */
    switch (CARD_NUMBER(ATTACKER_SIDE.cardId)) {
    case 1231:  /* no EDS card: survives without damage and puts its effect on the defender */
        ATTACKER_SIDE.destroyed = 0;
        ATTACKER_SIDE.damage = 0;
        /* Matching: DUEL_LOC(1 - attacker, defSlot) written player first. */
        QueueAddZoneLink(attacker, ATTACKER_SIDE.cardId, (u8)(1 - attacker) | (gBattleView.defSlot << 8),
                         ZONE_LINK_CARD_EFFECT);
        break;
    case 1251:  /* no EDS card: not destroyed by a monster with 1900 ATK or more */
        if (DEFENDER_SIDE.atk > 1899)
            ATTACKER_SIDE.destroyed = 0;
        break;
    case CARD_MECHANICAL_SPIDER:
        if (GetZoneCardAttribute(1 - attacker, gBattleView.defSlot) == ATTRIBUTE_DARK)
            DEFENDER_SIDE.destroyed = 1;
        break;
    }
    /* Sword of Dragon's Soul destroys a Dragon it battles, after the battle. */
    if (CountActiveZoneLinksFromCard(attacker, gBattleView.atkSlot, CARD_SWORD_OF_DRAGONS_SOUL)
        && GetZoneCardType(1 - attacker, gBattleView.defSlot) == CARD_TYPE_DRAGON)
        DEFENDER_SIDE.effectDestroy = 1;
    if (CountActiveZoneLinksFromCard(1 - attacker, gBattleView.defSlot, CARD_SWORD_OF_DRAGONS_SOUL)
        && GetZoneCardType(attacker, gBattleView.atkSlot) == CARD_TYPE_DRAGON)
        ATTACKER_SIDE.effectDestroy = 1;
    if (CARD_NUMBER(DEFENDER_SIDE.cardId) == 1251 && ATTACKER_SIDE.atk > 1899)
        DEFENDER_SIDE.destroyed = 0;
    if (gDuelPlayers[(1 - attacker) & 1].battleProtected) {
        DEFENDER_SIDE.destroyed = 0;
        DEFENDER_SIDE.damage = 0;
    }
    if (DEFENDER_SIDE.damage && CountActiveCardsOnFieldExcept(1 - attacker, CARD_1423, -1) > 0)
        DEFENDER_SIDE.damage = 0;
    if (ATTACKER_SIDE.damage && CountActiveCardsOnFieldExcept(attacker, CARD_1423, -1) > 0)
        ATTACKER_SIDE.damage = 0;
    for (i = 0; i <= 1; i++)
        gBattleView.side[i].destroyedCopy = gBattleView.side[i].destroyed;
}

/* ---- Card command menu ---- */

#define CARDMENU_CMD_COUNT 13           /* enum CardMenuCommand 0-12 */
#define MENU (gDuel.cardMenu)

/*
 * Draw the icon of every available command (gDuel.cardMenu.available bit i; OBJ tile 0x224 + 4 * i,
 * palette 2) in a centred row at y = 160 - 10 * slide: the row slides up with the menu. For the Fusion
 * Deck and the Deck the row moves to the left or right edge. The selected command pulses.
 */
void CardMenu_DrawIcons(void)
{
    int i;
    int count = 0;
    u16 attr2;
    int x, y;

    for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
        s32 bits = MENU.available;
        bits >>= i;
        if (bits & 1)
            count++;
    }
    attr2 = OAM_ATTR2_PALETTE(2) | OAM_ATTR2_PRIORITY(1) | 0x224;
    x = 120 - count * 8;
    y = 160 - MENU.slide * 10;
    switch (gDuelScreen.selArea) {
    case DUEL_AREA_DECK:
        x = 216 - count * 8;
        break;
    case DUEL_AREA_FUSION_DECK:
        x = 40 - count * 8;
        break;
    }
    MENU.timer++;
    for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
        s32 bits = MENU.available;
        bits >>= i;
        if (bits & 1) {
            if (MENU.command == i)
                AddAffineSprite((y << 16) | x, SPRITE_SHAPE_16x16, attr2,
                                gPulseScaleCurve[(MENU.timer >> 1) & 0xF] << 16);
            else
                AddSprite((y << 16) | x, SPRITE_SHAPE_16x16, attr2);
            x += 16;
        }
        attr2 += 4;
    }
}

/* Draw the selected command's name label (32x16, OBJ tile 0x264 + 8 * command, palette 3) at y = 96 over
 * its icon. */
void CardMenu_DrawLabel(void)
{
    int i;
    int count = 0;
    u16 attr2;
    int x;

    for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
        s32 bits = MENU.available;
        bits >>= i;     /* Matching: a separate shift keeps the lsr #16 inside the loop. */
        if (bits & 1)
            count++;
    }
    attr2 = OAM_ATTR2_PALETTE(3) | OAM_ATTR2_PRIORITY(1) | 0x264;
    x = 112 - count * 8;
    switch (gDuelScreen.selArea) {
    case DUEL_AREA_DECK:
        x = 208 - count * 8;
        break;
    case DUEL_AREA_FUSION_DECK:
        x = 32 - count * 8;
        break;
    }
    for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
        if ((MENU.available >> i) & 1) {
            if (MENU.command == i)
                AddSprite((96 << 16) | x, SPRITE_SHAPE_32x16, attr2);
            x += 16;
        }
        attr2 += 8;
    }
}

/*
 * Draw the card under the cursor as a 32x32 affine sprite. While the menu slides (slide 0-7) the card moves
 * from its place on the field to (104, 32) and zooms in (gShrinkScaleSteps); a face-down field card turns
 * face up during slides 0-4. At slide 1 and 7 the card's zone tiles are cleared. With the menu
 * open (slide 8) it stays at (104, 32) at twice the size. Nothing for the Fusion Deck and the Deck.
 */
void CardMenu_DrawCardPreview(void)
{
    u16 attr2;
    int x0, y0;
    int x, y;
    int player;

    switch (gDuelScreen.selArea) {
    case DUEL_AREA_FUSION_DECK:
    case DUEL_AREA_DECK:
        return;
    }
    attr2 = GetCardIconObjTile(DuelCursor_GetCardId()) + (OAM_ATTR2_PALETTE(1) | OAM_ATTR2_PRIORITY(1));
    if (MENU.slide == 1 || MENU.slide == 7) {
        switch (gDuelScreen.selArea) {
        case DUEL_AREA_MONSTER:
        case DUEL_AREA_SPELL_TRAP:
            ClearZoneTiles(gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
            break;
        case DUEL_AREA_FIELD:
            ClearZoneTiles(gDuelScreen.selPlayer, DUEL_AREA_FIELD);
            break;
        }
    }
    if (MENU.slide < 8) {
        x0 = GetAreaX(gDuelScreen.selPlayer, gDuelScreen.selArea, gDuelScreen.selIndex);
        y0 = GetAreaY(gDuelScreen.selPlayer, gDuelScreen.selArea, gDuelScreen.selIndex);
        x = (104 - x0) * MENU.slide / 8;
        y = (32 - y0) * MENU.slide / 8;
        x += x0;
        y += y0;
        switch (gDuelScreen.selArea) {
        case DUEL_AREA_MONSTER:
        case DUEL_AREA_SPELL_TRAP:
        case DUEL_AREA_FIELD:
            player = gDuelScreen.selPlayer & 1;
            /* Matching: gDuel.players[player].zones[zone], with the offsets added to the zones base. */
            if (!((struct DuelZone *)((u8 *)gDuel.players[0].zones
                                      + (gDuelScreen.selArea + gDuelScreen.selIndex) * sizeof(struct DuelZone)
                                      + player * sizeof(struct DuelPlayer)))->isFaceUp) {
                /* Face down: the card turns over as it comes up. Slides 0-2 show the card-back frames
                 * (OBJ tile 0x40 + 16 * slide, palette 0), 3 and 4 the card icon's poses 2 and 1. */
                switch (MENU.slide) {
                case 0:
                case 1:
                case 2:
                    attr2 = MENU.slide * 16 + 0x440;
                    break;
                case 3:
                case 4:
                    attr2 += (5 - MENU.slide) * 16;
                    break;
                }
            }
            break;
        }
        if (y >= 0 && y < 160)    /* on screen */
            AddAffineSprite(x | (y << 16), SPRITE_SHAPE_32x32, attr2, gShrinkScaleSteps[MENU.slide] << 16);
    } else {
        AddAffineSprite((32 << 16) | 104, SPRITE_SHAPE_32x32, attr2, 0x80 << 16);
    }
}

/* Bit `command` of gDuel.cardMenu.available: is that command offered? */
#define MENU_HAS(command) ((s32)MENU.available >> (command) & 1)

/*
 * One frame of the card command menu (enum CardMenuState): select the first available command, slide the
 * menu in, then Left/Right move over the available commands, B closes the menu, and A confirms the
 * command: confirmed, step 0 and the cursor's card, player, area and index are stored for CardMenu_Execute.
 * Card View is run here instead: the Fusion Deck list (CardListView_Open), or Card Detail between a fade
 * out and in of the duel screen. Any other state closes the menu.
 */
void CardMenu_Update(void)
{
    int i;

    switch (MENU.state) {
    case CARDMENU_STATE_INIT:
        gDuelScreen.showCursor = 0;
        MENU.slide = 0;
        MENU.command = 0;
        /* Start at command 0, else the last available one. */
        for (i = 0; !MENU_HAS(MENU.command); ) {
            if (MENU.command)
                MENU.command--;
            else
                MENU.command = CARDMENU_CMD_COUNT - 1;
            if (++i > CARDMENU_CMD_COUNT - 1)
                break;
        }
        MENU.state++;
        break;
    case CARDMENU_STATE_SLIDE_IN:
        CardMenu_DrawCardPreview();
        MENU.slide++;
        CardMenu_DrawIcons();
        if (MENU.slide > 7)
            MENU.state++;
        break;
    case CARDMENU_STATE_INPUT:
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        CardMenu_DrawLabel();
        if (gMain.newKeys & DPAD_LEFT) {
            for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
                if (MENU.command)
                    MENU.command--;
                else
                    MENU.command = CARDMENU_CMD_COUNT - 1;
                if (MENU_HAS(MENU.command))
                    break;
            }
            PlaySE(SE_CURSOR);
        }
        if (gMain.newKeys & DPAD_RIGHT) {
            for (i = 0; i < CARDMENU_CMD_COUNT; i++) {
                if (MENU.command < CARDMENU_CMD_COUNT - 1)
                    MENU.command++;
                else
                    MENU.command = 0;
                if (MENU_HAS(MENU.command))
                    break;
            }
            PlaySE(SE_CURSOR);
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            MENU.confirmed = 0;
            MENU.state++;
        } else if (gMain.newKeys & A_BUTTON) {
            MENU.confirmed = 1;
            MENU.step = 0;
            gDuel.cardMenuCard = DuelCursor_GetCardId();
            MENU.player = gDuelScreen.selPlayer;
            MENU.area = (u16)gDuelScreen.selArea;    /* Matching: truncated before the straddling store */
            MENU.index = gDuelScreen.selIndex;
            if (MENU.command == CARDMENU_CMD_CARD_VIEW) {
                if (MENU.area == DUEL_AREA_FUSION_DECK) {
                    CardListView_Open(MENU.player, DUEL_AREA_FUSION_DECK, 0, 0);
                    PlaySE(SE_CONFIRM);
                    MENU.state = CARDMENU_STATE_INPUT;
                } else {
                    PlaySE(SE_CONFIRM);
                    MENU.confirmed = 0;
                    MENU.state = CARDMENU_STATE_DETAIL_FADE;
                }
            } else {
                MENU.state++;
            }
        }
        break;
    case CARDMENU_STATE_SLIDE_OUT:
        MENU.slide--;
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        if (!MENU.slide) {
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
            case DUEL_AREA_SPELL_TRAP:
                DrawZoneTiles(gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
                break;
            case DUEL_AREA_FIELD:
                DrawZoneTiles(gDuelScreen.selPlayer, DUEL_AREA_FIELD);
                break;
            }
            MENU.state++;
        }
        break;
    case CARDMENU_STATE_DETAIL_FADE:
        if (DuelScreen_FadeOutStep()) {
            gDuelScreen.uiGfxLoaded = 0;
            gDuelScreen.active = 0;
            CardDetail_Init(gDuel.cardMenuCard, 0, 0);
            MENU.state++;
        }
        break;
    case CARDMENU_STATE_DETAIL:
        if (CardDetail_Run()) {
            DuelScreen_Init();
            DuelScreen_DrawCursorInfo();
            switch (gDuelScreen.selArea) {
            case DUEL_AREA_MONSTER:
            case DUEL_AREA_SPELL_TRAP:
                ClearZoneTiles(gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
                break;
            case DUEL_AREA_FIELD:
                ClearZoneTiles(gDuelScreen.selPlayer, DUEL_AREA_FIELD);
                break;
            }
            MENU.state++;
        }
        break;
    case CARDMENU_STATE_RETURN:
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        CardMenu_DrawLabel();
        if (DuelScreen_FadeInStepU32())
            MENU.state = CARDMENU_STATE_INPUT;
        break;
    default:
        /* CARDMENU_STATE_CLOSE (after the slide-out) */
        MENU.open = 0;
        MENU.state = 0;
        break;
    }
}
