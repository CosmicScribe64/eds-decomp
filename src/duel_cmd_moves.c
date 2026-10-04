/*
 * Duel command handlers (wiki/functions/duel-cmd-moves-c.md): Magical Hats, a monster banished until the End
 * Phase and its return, Polymerization's materials, two zone-state commands, the duel result / Surrender /
 * "Just a moment" banners, the life-point counter and the Magic/Trap negation flags.
 *
 * DuelCmd_Dispatch calls a handler once per frame while gDuelCmd.running is set. The handler reads its
 * operands from gDuelCmd (arg2/arg4/arg6; the acting player is bit 15 of cmd), steps through gDuelCmd.step
 * and clears gDuelCmd.running when it is done. The banners and the life-point digits are 4bpp sprites loaded
 * into OBJ palette 15 and the OBJ tiles from BANNER_OBJ_TILE on (VRAM 0x06016C80).
 */
#include "global.h"
#include "legacy/gba.h"                    /* OBJ_PLTT, OBJ_VRAM0, B_BUTTON */
#include "legacy/main.h"                   /* gMain.heldKeys */
#include "legacy/sound.h"                  /* PlaySE, PlayJingle, SoundIsBGMPlaying */
#include "constants/card_stats.h"   /* CARD_TYPE_*, CARD_STATS_TYPE_* */
#include "constants/duel.h"         /* DUEL_AREA_* */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "card_data.h"              /* CARD_NUMBER_TOKEN_*, CARD_ID_MASK */
#include "util.h"                   /* CopyDoubleWords */
#include "sprite.h"                 /* AddSprite, AddAffineSprite, SPRITE_SHAPE_* */
#include "duel_flow.h"              /* gPulseScaleCurve */

#ifdef DISPCNT_MODE_4
#include "legacy/duel.h"                   /* struct DuelCard / DuelLoc / DuelZone / DuelPlayer, gDuel, gDuelPlayers */
#include "duel_cmd.h"               /* gDuelCmd, struct DuelResultBanner, the banner and digit graphics */
#include "duel_screen.h"            /* gDuelScreen, card animations, field cells, DrawLifePoints */
#else
/* ---- BEGIN pre-H0 subset ---- */
/*
 * Before H0 (build/readability/HEADERS.md) include/gba.h, main.h, sound.h and duel.h still hold the legacy
 * headers, whose duel structs have other field names, and duel_cmd.h / duel_screen.h need the new duel.h.
 * Until then this block repeats the part of the new duel.h, duel_cmd.h, duel_screen.h and sound.h that the
 * unit uses: the same tags, field names, types and bitfield containers (truncated structs end after the last
 * field used here), and the same prototypes. With the new headers installed the block is skipped; then
 * delete it (build/readability/issues/duel_cmd_moves.md).
 */
struct DuelCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;
    u32 specialSummoned:1;
    u32 planted:1;
    u32 graverobbed:1;
    u32 unk19:1;
    u32 isFusionMaterial:1;
    u32 destroyedInBattle:1;
    u32 destroyedByOpponent:1;
    u32 flag23:1;
    u32 pendingEquip:1;
    u32 equipZone:3;
    u32 pendingOpponentSummon:1;
    u32 unk29:3;
};
struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};
struct DuelZone {
    struct DuelCard card;
    u16 serial;
    u8 isDefense:1;
    u8 isFaceUp:1;
    u8 turnCounter:4;
    u16 destroyCountdown:4;
    u8 positionLocked:1;
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;
    u8 revivedByMonsterReborn:1;
    u8 summonedFromGraveyard:1;
    u8 levelCheckDone:1;
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];
    u16 linkKinds[32];
    u16 numLinks;
    u8 unk8C_0:1;
    u8 destroyAfterBattle:1;
    u32 returnAfterBattle:1;
    u8 cannotAttackNextTurn:1;
    u8 cannotAttack:1;
    u8 atkHalved:1;
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;
    u32 unk90_6:4;
    u32 canActivate:1;
    u8 isDisabled:1;
    u32 unk91_4:1;
    u32 declaredValue:5;
    u32 unk92_2:14;
};
struct DuelPlayer {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 banishedCount;
    u8 unk7;                        /* bits not used here */
    u8 unk8;
    u8 unk9_0:6;
    u32 lockedZones:10;
    u8 crushCardTurns:3;
    u8 monsterSentToGraveThisTurn:1;
    u32 removedMask:5;
    u8 delayedSummonCount:3;
    u8 destroyedTriggerPending:1;
    u8 banishCostFromField:1;
    u8 unkC_6:2;
    u8 unkD;
    u16 lpPaid[11];
    u16 attackableMask;
    u16 attackedMask;
    struct DuelZone zones[11];
    struct DuelCard hand[80];
    struct DuelCard deck[80];
    struct DuelCard graveyard[80];
    struct DuelCard fusionDeck[80];
    struct DuelCard banished[80];
    u16 banishedInfo[80];
};
struct DuelState {
    u16 serial;
    u16 unk2;
    struct DuelPlayer players[2];
    u8 fieldBackground:4;
    u8 duelOver:1;
    u8 unk1ACC_5:1;
    u8 magicNegated:1;
    u8 trapsNegated:1;
    u8 equipMagicNegated:1;
    u8 equipMagicNegatedThisTurn:1;
    u8 fieldMagicNegatedThisTurn:1;
    u8 contMagicNegatedThisTurn:1;
    u8 contTrapNegatedThisTurn:1;
    u8 statChangesReversed:1;
    u8 atkDefSwapped:1;
    u32 prohibitionCount:4;
    u32 unk1ACE_3:13;
};
extern struct DuelState gDuel;
extern struct DuelPlayer gDuelPlayers[2];
extern const u16 gBounceScaleCurve[];
void CopyDuelCard(u32 *dst, u32 *src);
void SubtractLifePoints(struct DuelPlayer *players, u32 player, s32 amount);
void SendZoneCardToGraveyardOrBanished(int player, int zone, u16 banish);
void AddCardToBanishedTemporarily(struct DuelCard *card, int zone);
void ReturnTemporarilyBanishedCard(int player, int zone);

struct DuelCmdEntry {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
};
struct DuelCmd {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
    struct DuelCmdEntry queue[256];
    u16 queueCount;
    u16 step:7;
    u16 counter:7;
    u16 unk80A_14:2;
    u32 unk80C_0:5;
    u32 timer:7;
    u32 unk80C_12:1;
    u32 running:1;
    u32 unk80C_14:18;
    u16 *hofsTable;
    struct DuelCard card;
};
struct DuelResultBanner {
    const u8 *pal;
    const u8 *gfx;
    u16 jingle;
};
extern struct DuelCmd gDuelCmd;
extern const u16 gBannerSlideOffsets[];
extern const u16 gLpDigitsPal[];
extern const u16 gLpDigitsGfx[];
extern const u8 gDuelBannerPal[];
extern const u8 gSmokePuffAnim[];

struct DuelScreen {
    u8 fast:1;
    u8 uiGfxLoaded:1;
    u8 active:1;
    u8 unk0_3:5;
    u8 unk1;
    u16 fieldBgScroll;
    u8 scroll;
    u8 scrollFrom;
    u8 scrollTo;
    u8 scrollSteps:4;
    u8 fieldBackground:4;
    u8 textTiles[0x800];
    u16 textTilesDirty:1;
    u16 textMapReset:1;
    u16 cursorDone:1;
    u16 showCursor:1;
    u16 cursorRotate180:1;
    u16 cursorAltTile:1;
    u16 cursorSteps:4;
    u16 unk808_10:6;
    u8 unk80A[0x85C - 0x80A];       /* +0x80A..+0x85B: fields not used here */
    void (*overlayCallback)(void);
};
extern struct DuelScreen gDuelScreen;
void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelCursor_Refresh(void);
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
void DuelAnim_PlayZoneEffect(struct DuelLoc *loc, u32 anim, u32 dx, u32 dy);
void ClearZoneTiles(u32 player, u32 area);
void DrawAllAreaTiles(void);
void DrawLifePoints(int player, int lifePoints);
void DrawLpChangeAmount(u32 x, u32 y, s32 value, u32 colorSet);

void PlaySE(u32 seId);
void PlayJingle(u32 songId);
int SoundIsBGMPlaying(s32 song);
/* ---- END pre-H0 subset ---- */
#endif

/*
 * Local view of PlaceMonsterCard (duel.h: int, int, struct DuelCard *, u16 defense, u16 faceUp). This unit
 * passes the flags as u32: the u16 parameters add narrowing at the call, which the ROM does not have.
 */
void PlaceMonsterCardU32(u32 player, u32 zone, struct DuelCard *card, u32 defense, u32 faceUp)
    asm("PlaceMonsterCard");

/* ROM data used only by this unit. */
extern const struct DuelResultBanner gDuelResultBanners[];  /* [3]: YOU WIN, YOU LOSE, DRAW */
extern const u8 gSurrenderBannerGfx[];      /* 64x32 4bpp "Surrender" banner (0x400 bytes) */
extern const u8 gJustAMomentBannerGfx[];    /* 64x32 4bpp "Just a moment" banner (0x400 bytes) */

/* The acting player of the current command (cmd bit 15) and the command id. */
#define CMD_PLAYER()    (gDuelCmd.cmd >> 15)
#define CMD_ID()        (gDuelCmd.cmd & DUEL_CMD_ID_MASK)

/*
 * gDuelCmd.card, the card word the command moves, for field reads. A direct gDuelCmd.card.id or .owner read
 * compiles to other code (the two handlers using it no longer match); the pointer form matches.
 */
#define CMD_CARD        ((struct DuelCard *)&gDuelCmd.card)

/* Fast-forward: B held, or the duel's fast mode. */
#define FAST_FORWARD()  ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)

/*
 * Zone (player, zone) of gDuelPlayers, addressed as zones base + byte offsets. The two association orders
 * of the sum are kept because they compile differently: ZONE() for most handlers, ZONE_OF() (zone and player
 * offsets summed first) for DuelCmd_SetMagicalHatsCard.
 */
#define ZONE(player, zone)    ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (zone) * 0x94 + (player) * 0xD64))
#define ZONE_OF(player, zone) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + ((zone) * 0x94 + (player) * 0xD64)))

/*
 * Card tables read through their integer addresses (this access form is part of the match):
 * 0x08621DE0 = gCardStats, 0x08622AB4 = gCardIdToNumber.
 */
#define CARD_TYPE(id) \
    ((((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* Sprite position argument of AddSprite*: y in the high halfword, x in the low one. */
#define SPRITE_YX(x, y) (((y) << 16) | (x))

/* Where the banners and the life-point digits go: OBJ palette 15 and OBJ tiles from 0x364. */
#define BANNER_PAL_SLOT 15
#define BANNER_OBJ_TILE 0x364
#define BANNER_PAL      ((void *)(OBJ_PLTT + BANNER_PAL_SLOT * 0x20))       /* 0x050003E0 */
#define BANNER_GFX      ((void *)(OBJ_VRAM0 + BANNER_OBJ_TILE * 0x20))      /* 0x06016C80 */
#define BANNER_ATTR2    ((BANNER_PAL_SLOT << 12) | BANNER_OBJ_TILE)         /* 0xF364 */
#define BANNER_X        0x58

/* Sound effects (constants/sound.h has no names for these yet; the names say where they are played). */
#define SE_ZONE_EFFECT      0x10    /* with a sprite effect on a zone: the smoke puff here, the negate and
                                     * tribute animations in duel_cmd_status.c */
#define SE_BANNER_SLIDE     0x16    /* a banner slides in (also the Chain banner) */
#define SE_LP_GAIN          12      /* life points counting up */
#define SE_LP_LOSS          13      /* life points counting down */

/*
 * Command 0xA8 (DUEL_CMD_SET_MAGICAL_HATS_CARD): Magical Hats puts one of its shuffled cards (the monster or
 * one of the two Magic/Trap cards from the deck) into a monster zone in defense position.
 * arg2: monster zone in bits 0-7, face up in bit 8 (Light of Intervention); arg4 | arg6 << 16: the card word.
 * Step 0 places the card, plays the smoke puff and clears the cell; the next call redraws the field.
 */
void DuelCmd_SetMagicalHatsCard(void)
{
    u32 player = CMD_PLAYER();
    u8 zone = gDuelCmd.arg2;
    u32 faceUp = (gDuelCmd.arg2 >> 8) & 1;
    struct DuelCard card;
    struct DuelLoc loc;
    u16 cardNo;

    *(u32 *)&card = (gDuelCmd.arg6 << 16) | gDuelCmd.arg4;
    cardNo = CARD_NUMBER(card.id);
    /* monster tokens are always face up (two ifs: the && form compiles to other code) */
    if (cardNo >= CARD_NUMBER_TOKEN_FIRST)
        if (cardNo < CARD_NUMBER_TOKEN_END)
            faceUp = 1;

    switch (gDuelCmd.step) {
    case 0:
        PlaceMonsterCardU32(player, zone, &card, 1, faceUp);

        /* a Magic/Trap card is only a decoy: it is destroyed after a battle */
        if (CARD_TYPE(card.id) > CARD_TYPE_REPTILE) {
            struct DuelZone *z = ZONE_OF(player, zone);     /* (the local pointer is part of the match) */
            z->destroyAfterBattle = 1;
        }

        PlaySE(SE_ZONE_EFFECT);
        loc.player = player;
        loc.area = DUEL_AREA_MONSTER;
        loc.index = zone;
        loc.isDefense = 1;
        loc.isFaceUp = faceUp;
        DuelAnim_PlayZoneEffect(&loc, (u32)gSmokePuffAnim, 0, 0);
        ClearZoneTiles(player, zone);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0xA9 (DUEL_CMD_BANISH_UNTIL_END_PHASE), arg2 = monster zone: the monster leaves the field until the
 * End Phase. Step 0 scrolls to the acting player's hand row. Step 1 moves the card word into gDuelCmd.card,
 * empties the zone and animates the card to its owner's banished pile. The last step banishes it as
 * BANISH_UNTIL_END_PHASE, which keeps the zone held (removedMask), and redraws the field.
 */
void DuelCmd_BanishMonsterUntilEndPhase(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 zone = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        /* FAKEMATCH: the extra uses raise player's and zone's refs so global-alloc
         * takes player first (r7), then zone evicts the local in r6 and step the
         * local in r5, as in the ROM. */
        asm("" :: "r"(player), "r"(zone));
        CopyDuelCard((u32 *)&gDuelCmd.card, (u32 *)&gDuelPlayers[player & 1].zones[zone].card);
        /* FAKEMATCH: pointer arithmetic, not zones[zone]: the array form compiles to other code */
        (gDuelPlayers[player & 1].zones + zone)->card.id = 0;
        ClearZoneTiles(player, zone);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = zone;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToBanishedTemporarily(&gDuelCmd.card, zone);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0xAA (DUEL_CMD_RETURN_BANISHED_MONSTER), arg2 = monster zone: the End Phase brings back the monster
 * banished by DUEL_CMD_BANISH_UNTIL_END_PHASE. Step 1 animates a card from the banished pile to the zone (the
 * animation is given card ID 1, not the returning card); the last step puts the card back and redraws.
 */
void DuelCmd_ReturnBanishedMonster(void)
{
    u32 player = CMD_PLAYER();
    u16 zone = gDuelCmd.arg2;
    struct DuelLoc from, to;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        from.player = player;
        from.area = DUEL_AREA_BANISHED;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 0;
        to.player = player;
        to.area = DUEL_AREA_MONSTER;
        to.index = zone;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        ReturnTemporarilyBanishedCard(player, zone);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0xA5 (DUEL_CMD_FUSION_MATERIAL_TO_GRAVE), arg2 = zone, arg4 = banish instead: a fusion material on
 * the field (Polymerization) goes to its owner's graveyard, or to the banished pile.
 * Step 0 marks the card as a fusion material, keeps a copy in gDuelCmd.card, moves it out of the zone and
 * animates it; the next call redraws the field.
 */
void DuelCmd_SendFusionMaterialToGrave(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 zone = gDuelCmd.arg2;
    u16 banish = gDuelCmd.arg4;

    switch (gDuelCmd.step) {
    case 0:
        ClearZoneTiles(player, zone);
        ZONE(player & 1, zone)->card.isFusionMaterial = 1;
        CopyDuelCard((u32 *)&gDuelCmd.card, (u32 *)&gDuelPlayers[player & 1].zones[zone].card);
        SendZoneCardToGraveyardOrBanished(player, zone, banish);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = zone;
        /* read after the zone was cleared, so the card always starts upright and face down (as in the ROM) */
        from.isDefense = ZONE(player & 1, zone)->isDefense;
        from.isFaceUp = ZONE(player & 1, zone)->isFaceUp;
        to.player = CMD_CARD->owner;
        to.area = banish ? DUEL_AREA_BANISHED : DUEL_AREA_GRAVEYARD;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0xA6 (DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG): levelCheckDone of the acting player's zone arg2 = arg4.
 * The duel step that queues it (for each face-up monster) is also the only reader (meaning: hypothesis).
 */
void DuelCmd_SetZoneLevelCheckFlag(void)
{
    ZONE(CMD_PLAYER(), gDuelCmd.arg2)->levelCheckDone = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* Command 0xA0 (DUEL_CMD_CLEAR_ZONE_LINKS_2): remove every link of the acting player's zone arg2. Byte for byte
 * the same code as DuelCmd_ClearZoneLinks (0x8C). */
void DuelCmd_ClearZoneLinks2(void)
{
    ZONE(CMD_PLAYER(), gDuelCmd.arg2)->numLinks = 0;
    gDuelCmd.running = 0;
}

/*
 * Command 0x04 (DUEL_CMD_SHOW_DUEL_RESULT), arg2 = banner: 0 YOU WIN, 1 YOU LOSE, 2 DRAW (the handler accepts
 * up to 4, but only three banners exist). Step 0 loads the 64x64 banner and starts its jingle. Step 1 spins
 * and zooms it in over 0x40 frames (scale 0x900 - 32t, angle 4t). Step 2 waits for the jingle to end and
 * step 3 holds the banner for 0x1E more frames.
 */
void DuelCmd_ShowDuelResult(void)
{
    s32 step = gDuelCmd.step;
    s32 timer;

    switch (step) {
    case 0:
        if (gDuelCmd.arg2 <= 4) {
            CopyDoubleWords(BANNER_PAL, gDuelResultBanners[gDuelCmd.arg2].pal, 0x20);
            CopyDoubleWords(BANNER_GFX, gDuelResultBanners[gDuelCmd.arg2].gfx, 0x800);
            PlayJingle(gDuelResultBanners[gDuelCmd.arg2].jingle);
            gDuelScreen.overlayCallback = NULL;
        }
        gDuelCmd.step++;
        gDuelCmd.timer = 0;
        /* fall through */
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 0x40) {
            AddAffineSprite(SPRITE_YX(BANNER_X, 0x20), SPRITE_SHAPE_64x64, BANNER_ATTR2,
                            ((0x900 - timer * 32) << 16) | (timer * 4));
            gDuelCmd.timer++;
        } else {
            AddSprite(SPRITE_YX(BANNER_X, 0x20), SPRITE_SHAPE_64x64, BANNER_ATTR2);
            gDuelCmd.step++;
        }
        break;
    case 2:
        AddSprite(SPRITE_YX(BANNER_X, 0x20), SPRITE_SHAPE_64x64, BANNER_ATTR2);
        if (SoundIsBGMPlaying(gDuelResultBanners[gDuelCmd.arg2].jingle) == 0) {
            gDuelCmd.step++;
            gDuelCmd.timer = 0;
        }
        break;
    case 3:
        AddSprite(SPRITE_YX(BANNER_X, 0x20), SPRITE_SHAPE_64x64, BANNER_ATTR2);
        if (gDuelCmd.timer < 0x1E)
            gDuelCmd.timer++;
        else
            gDuelCmd.step++;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x40 (DUEL_CMD_SURRENDER): the "Surrender" banner wobbles (gBounceScaleCurve) for 0x60 frames, then
 * the acting player's life points drop to 0.
 */
void DuelCmd_Surrender(void)
{
    u32 player = CMD_PLAYER();
    u32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 0;
        CopyDoubleWords(BANNER_PAL, gDuelBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gSurrenderBannerGfx, 0x400);
        gDuelScreen.overlayCallback = NULL;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer < 0x60) {
            AddAffineSprite(SPRITE_YX(BANNER_X, 0x30), SPRITE_SHAPE_64x32, BANNER_ATTR2,
                            gBounceScaleCurve[gDuelCmd.timer & 0x1F] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x77)
                gDuelCmd.timer += 7;
            break;
        }
        /* fall through */
    default:
        gDuelPlayers[player].lifePoints = 0;
        DrawLifePoints(player, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x41 (DUEL_CMD_SHOW_JUST_A_MOMENT): the link-duel "Just a moment" banner, shown while the other
 * player acts. Step 1 slides it in over 16 frames from the acting player's side (from the bottom for player
 * 0, the top for player 1), step 2 pulses it at y 0x40 for 0x40 frames, step 3 slides it back out.
 */
void DuelCmd_ShowJustAMomentBanner(void)
{
    u32 player = CMD_PLAYER();
    s32 step = gDuelCmd.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 0;
        CopyDoubleWords(BANNER_PAL, gDuelBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gJustAMomentBannerGfx, 0x400);
        gDuelScreen.overlayCallback = NULL;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        PlaySE(SE_BANNER_SLIDE);
        break;
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 16) {
            if (player)
                y = gBannerSlideOffsets[timer];
            else
                y = 0x80 - gBannerSlideOffsets[timer];
            AddSprite(SPRITE_YX(BANNER_X, y), SPRITE_SHAPE_64x32, BANNER_ATTR2);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 7)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 0;
        gDuelCmd.step = step + 1;
        PlaySE(SE_BANNER_SLIDE);
        /* fall through */
    case 2:
        if (gDuelCmd.timer < 0x40) {
            AddAffineSprite(SPRITE_YX(BANNER_X, 0x40), SPRITE_SHAPE_64x32, BANNER_ATTR2,
                            gPulseScaleCurve[gDuelCmd.timer & 0xF] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x37)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
        /* fall through */
    case 3:
        timer = gDuelCmd.timer;
        if (timer != 0) {
            if (player)
                y = gBannerSlideOffsets[timer - 1];
            else
                y = 0x80 - gBannerSlideOffsets[timer - 1];
            AddSprite(SPRITE_YX(BANNER_X, y), SPRITE_SHAPE_64x32, BANNER_ATTR2);
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 8)
                gDuelCmd.timer -= 7;
            break;
        }
        /* fall through */
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Draws a signed life-point change as 16x16 digit sprites, right-aligned at x + 0x50, with a '+' or '-' in
 * front (0 is drawn as a single '0'). colorSet selects one of the digit colour sets of gLpDigitsGfx; digit d
 * is tile d * 4 of the set, '+' is 10 and '-' is 11.
 */
void DrawLpChangeAmount(u32 x, u32 y, s32 value, u32 colorSet)
{
    u16 tile = colorSet * 0x30 + BANNER_ATTR2;
    u32 sign;

    if (value < 0) {
        sign = 11;
        value = -value;
    } else {
        sign = 10;
    }
    x += 0x50;
    if (value == 0) {
        AddSprite((y << 16) | x, SPRITE_SHAPE_16x16, tile);
    } else {
        do {
            AddSprite(x | (y << 16), SPRITE_SHAPE_16x16, value % 10 * 4 + tile);
            value /= 10;
            x -= 16;
        } while (value != 0);
        AddSprite((y << 16) | x, SPRITE_SHAPE_16x16, tile + sign * 4);
    }
}

/*
 * Commands 0x42 (DUEL_CMD_GAIN_LP, gain = 1) and 0x43 (DUEL_CMD_LOSE_LP, gain = 0), arg2 = amount.
 * Step 0 loads the digits and scrolls to player 0's monster row. Step 1 shows the signed amount next to the
 * acting player's LP box for 0x5A frames. Step 2 counts the amount into the life points, 100, 10 or 1 points
 * per frame, taking them from arg2 and redrawing the LP box each time (a loss goes through SubtractLifePoints,
 * which stops at 0); the count sound repeats every 11 frames. Counting stops at once when the life points are
 * 0. Finally the cursor is restored.
 */
void DuelCmd_ChangeLifePoints(u16 gain)
{
    u32 player = CMD_PLAYER();
    u32 x = player * 0x68 + 8;
    u32 y = 0x58 - player * 24;
    u16 se = gain ? SE_LP_GAIN : SE_LP_LOSS;
    s32 step = gDuelCmd.step;
    struct DuelPlayer *self;
    struct DuelPlayer *players;

    switch (step) {
    case 0:
        CopyDoubleWords(BANNER_PAL, gLpDigitsPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gLpDigitsGfx, 0xC00);
        gDuelScreen.overlayCallback = NULL;
        DuelScreen_ScrollToZone(0, DUEL_AREA_MONSTER);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        DrawLpChangeAmount(x, y, gain ? gDuelCmd.arg2 : -gDuelCmd.arg2, gain != 0);
        if (gDuelCmd.timer < 0x5A) {
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x4F)
                gDuelCmd.timer += 7;
            break;
        }
        PlaySE(se);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        players = gDuelPlayers;
        self = &players[player];
        if (self->lifePoints == 0) {
            gDuelCmd.step = step + 1;
            break;
        }
        DrawLpChangeAmount(x, y, gain ? gDuelCmd.arg2 : -gDuelCmd.arg2, gain != 0);
        if (gDuelCmd.arg2 >= 100) {
            gDuelCmd.arg2 -= 100;
            if (gain)
                self->lifePoints += 100;
            else
                SubtractLifePoints(players, player, 100);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
        if (gDuelCmd.arg2 >= 10) {
            gDuelCmd.arg2 -= 10;
            if (gain)
                self->lifePoints += 10;
            else
                SubtractLifePoints(players, player, 10);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
        if (gDuelCmd.arg2 != 0) {
            gDuelCmd.arg2 -= 1;
            if (gain)
                self->lifePoints += 1;
            else
                SubtractLifePoints(players, player, 1);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
        /* fall through */
    default:
        DuelCursor_Refresh();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Commands 0x15-0x1B: store arg2 in one of the Magic/Trap negation flags of gDuel. The "this turn" flags are
 * cleared by DuelCmd_TurnEnd; trapsNegated, magicNegated and equipMagicNegated stay until a command clears
 * them.
 */
void DuelCmd_SetNegationFlag(void)
{
    switch (CMD_ID()) {
    case DUEL_CMD_NEGATE_EQUIP_THIS_TURN:       /* Armored Glass */
        gDuel.equipMagicNegatedThisTurn = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_FIELD_THIS_TURN:       /* World Suppression */
        gDuel.fieldMagicNegatedThisTurn = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_CONT_TRAP_THIS_TURN:   /* Metal Detector */
        gDuel.contTrapNegatedThisTurn = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_CONT_MAGIC_THIS_TURN:  /* Mystic Probe */
        gDuel.contMagicNegatedThisTurn = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_TRAPS:                 /* Jinzo, Royal Decree */
        gDuel.trapsNegated = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_MAGIC:                 /* Imperial Order */
        gDuel.magicNegated = gDuelCmd.arg2;
        break;
    case DUEL_CMD_NEGATE_EQUIP:
        gDuel.equipMagicNegated = gDuelCmd.arg2;
        break;
    }
    gDuelCmd.running = 0;
}
