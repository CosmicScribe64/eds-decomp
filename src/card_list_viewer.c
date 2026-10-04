/*
 * card_list_viewer (0x0802AAC0-0x0802BACF): the duel card-list viewer's runner, input and Open, then the
 * target checks of card effects.
 *
 * Card-list viewer (card_list_view.h; the drawing helpers and CardListView_InitScreen are in turn_order_steps.c).
 * CardListView_Open fills gCardListView from a player's graveyard, fusion deck, banished pile or deck, or for
 * area -1 from the effect-target collector. DuelMainStep calls CardListView_Run every frame (it returns 1 while
 * the viewer is active), which runs CardListView_Update and then the step gCardListViewSteps[step] (enum
 * CardListViewStep): InitScreen, HandleInput, Exit. In HandleInput Up/Down move the cursor box (or scroll the
 * page at the edges), Left/Right pick a button, A presses it (Card View opens Card Detail for the entry) and B
 * leaves if Exit is enabled.
 *
 * Target checks: can a card effect target the card in (player, zone)? CanCardTargetZone and IsZoneTargetable
 * are the generic protections: Lord of D. protects face-up Dragons, and under Umi cards 1326 and 1329 (not in
 * this game) cannot be targeted by Magic cards other than Equip Magic. The Effect*Check functions are the
 * 'check' slot of gCardEffects (effect_handlers.h): the effect code calls them for every (player, zone),
 * packed as pos = zone << 8 | player, to find the legal targets of the card in `entry`.
 */
#include "global.h"
#include "legacy/gba.h"                    /* keys */
#include "legacy/main.h"                   /* gMain.newKeys, gMain.bgVofs */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute, SpellSubtype */
#include "constants/duel.h"         /* enum DuelArea, DuelZoneIndex, BanishKind, ZoneLinkKind */
#include "constants/sound.h"        /* enum SoundEffect */
#include "util.h"                   /* MemClear16 */
#include "text.h"                   /* TextCanvasToTiles */
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* */
#include "card_detail.h"            /* CardDetail_Init, CardDetail_Run */

/* ---- BEGIN header subset (pre-H0) ----
 * The parts of duel.h and sound.h this unit and the headers below need, with the canonical headers' tags,
 * names, types and bitfield containers (unused bytes are padding). include/duel.h and sound.h still hold the
 * legacy headers until the header switch (H0, build/readability/HEADERS.md); chain.h, duel_screen.h and
 * card_list_view.h include duel.h, so this block also defines duel.h's include guard. After H0, replace the
 * block (BEGIN to END) with
 *     #include "legacy/duel.h"
 *     #include "legacy/sound.h"
 * which gives identical assembly (checked against the staged headers). */

/* duel.h */
#define GUARD_DUEL_H
struct DuelCard {
    u32 id:12;                          /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                        /* bit 12: owning player */
    u32 unk13:19;
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
    struct DuelCard card;               /* +0x00 */
    u16 serial;                         /* +0x04 */
    u8 isDefense:1;                     /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                      /* +0x06 bit 1: face up */
    u8 turnCounter:4;                   /* +0x06 bits 2-5: turns a face-up card has been active */
    u8 unk6_6:2;
    u8 unk7[3];
    u16 links[32];                      /* +0x0A: DUEL_LOC of a card affecting this one, or a value / card ID */
    u16 linkKinds[32];                  /* +0x4A: low byte enum ZoneLinkKind, high byte stack count / value */
    u16 numLinks;                       /* +0x8A: entries in links / linkKinds */
    u8 unk8C[8];
};
struct DuelPlayer {
    u16 lifePoints;                     /* +0x000 */
    u8 handCount;                       /* +0x002 */
    u8 deckCount;                       /* +0x003: entries in deck[] */
    u8 graveCount;                      /* +0x004: entries in graveyard[] */
    u8 fusionCount;                     /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;                   /* +0x006: entries in banished[] and banishedInfo[] */
    u8 unk7[0x28 - 0x7];
    struct DuelZone zones[11];          /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];           /* +0x684 */
    struct DuelCard deck[80];           /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];      /* +0x904 */
    struct DuelCard fusionDeck[80];     /* +0xA44 */
    struct DuelCard banished[80];       /* +0xB84 */
    u16 banishedInfo[80];               /* +0xCC4: parallel to banished[]: low byte enum BanishKind */
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};
extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];
void CopyDuelCard(u32 *dst, u32 *src);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int GetFaceUpFieldMagicNumber(void);
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
u32 GetZoneCardAtk(u32 player, u32 slot);
u32 GetZoneCardType(s32 player, s32 slot);
u32 GetZoneCardAttribute(s32 player, s32 slot);

/* sound.h */
void PlaySE(u32 seId);
/* ---- END header subset ---- */

#include "chain.h"                  /* struct ChainEntry (the effect checks' card) */
#include "effect.h"                 /* CanCardTargetZone, IsZoneTargetable */
#include "effect_handlers.h"        /* the Effect*Check handlers defined here */
#include "duel_screen.h"            /* DuelScreen_Init, DuelScreen_DrawCursorInfo, DuelScreen_FadeInStep */
#include "card_list_view.h"         /* gCardListView, the CardListView_* functions */

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* FadeToBlack as this unit calls it: returning u16, so the callers truncate the result (lsl #16) before testing
 * it. palette.h has the definition's u32 return. */
u16 FadeToBlackU16(s32 step) asm("FadeToBlack");

/* CollectEffectTargets with an int card number and no result: CardListView_Open passes its argument on
 * unnarrowed (effect.h: u16 cardNumber, u16 return). */
void CollectEffectTargetsInt(int player, int cardNumber, int arg) asm("CollectEffectTargets");

/* ---- ROM data used only here ---- */

/* 0x0819A7B8: the viewer's steps (enum CardListViewStep), NULL-terminated. */
extern u16 (*const gCardListViewSteps[])(void);

/* 0x0819A788: BG1 scroll offsets of the 4-frame cursor-box slide, [cursorMoveDir][cursorMoveTimer]: dir 1 (up)
 * 15, 13, 10, 5; dir 2 (down) -15, -13, -10, -5 (row 0 unused). */
extern const s32 gCardListViewCursorSlide[][4];

/* ---- Helpers ---- */

/* The card word as one u32 and its card ID (bits 0-11). Matching: the ROM always loads the whole word (ldr)
 * and extracts the ID with lsl #20; lsr #20, where a bitfield read of .id loads a halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)

/* The card tables through their integer addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: each use reloads the table base literal, as in the ROM. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))           /* enum CardType */
#define CARD_SUBTYPE(id) CARD_STATS_SUBTYPE(CARD_STATS(id))     /* enum SpellSubtype */

/* &gDuelZones[player].zones[zone] by byte arithmetic; player is already masked with & 1. Matching: written
 * zone term first, agbcc emits the address in the ROM's order (array indexing gives another order). */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* The same address with the player term written first; agbcc then emits the zone multiply first. */
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* Byte 6 of a zone (isDefense bit 0, isFaceUp bit 1, turnCounter) as one byte. Matching: EffectStopDefenseCheck
 * returns the isDefense bit as the loaded byte & 1 (the bitfield read gives lsl; lsr). */
#define ZONE_FLAGS(zone) (((u8 *)(zone))[6])
#define ZONE_FLAG_DEFENSE 1

/* gCardListView.cursorMoveDir, and the length of the cursor-box slide (cursorMoveTimer counts it down). */
enum CardListCursorMove {
    CARDLIST_CURSOR_IDLE = 0,
    CARDLIST_CURSOR_UP = 1,
    CARDLIST_CURSOR_DOWN = 2,
};
#define CARDLIST_CURSOR_SLIDE_FRAMES 4

/* Player and zone of a packed u16 location (DUEL_LOC: zone << 8 | player), such as the `pos` of the target
 * checks and a zone link. */
#define LOC_PLAYER(loc) ((u8)(loc))
#define LOC_ZONE(loc) ((loc) >> 8)

/* ======================================================================================================== */
/* Card-list viewer                                                                                         */
/* ======================================================================================================== */

/*
 * Step 2 of gCardListViewSteps (CARDLIST_STEP_EXIT), by state: fades to black and hides the sprites, then
 * rebuilds the duel screen and its info bar; then returns DuelScreen_FadeInStep(), 1 once the duel screen has
 * faded back in.
 */
u16 CardListView_Exit(void)
{
    struct CardListView *view = &gCardListView;

    switch (view->state) {
    case 0:
        if (FadeToBlackU16(4)) {
            view->showSprites = 0;
            view->state++;
        }
        break;
    case 1:
        DuelScreen_Init();
        DuelScreen_DrawCursorInfo();
        view->state++;
        break;
    default:
        return DuelScreen_FadeInStep();
    }
    return 0;
}

/*
 * The per-frame part, run before every step: converts the text canvas to BG tiles when the names were redrawn
 * (textDirty), slides the cursor box (BG1 scroll) and, when the slide ends, moves cursorRow and redraws the
 * selected entry's panel and cursor box; then the button bar and, unless the box is moving or the list is empty,
 * the selected entry's CARD STATUS icons.
 */
void CardListView_Update(void)
{
    int idle = 1;
    struct CardListView *view = &gCardListView;

    if (view->textDirty) {
        view->textDirty = 0;
        TextCanvasToTiles((u16 *)(VRAM + 0x4000 + 0x10 * 32), 0);   /* BG tile 0x10 of char block 1 */
    }
    /* BG1's VOFS -(cursorRow * 16) puts the box on the cursor row; the slide offsets lead it to the next row. */
    if (view->cursorMoveDir) {
        if (view->cursorMoveTimer) {
            view->cursorMoveTimer--;
            gMain.bgVofs[1] = gCardListViewCursorSlide[view->cursorMoveDir][view->cursorMoveTimer]
                            - (view->cursorRow << 4);
            idle = 0;
        } else {
            switch (view->cursorMoveDir) {
            case CARDLIST_CURSOR_UP:
                view->cursorRow--;
                break;
            case CARDLIST_CURSOR_DOWN:
                view->cursorRow++;
                break;
            }
            /* Matching: from here on the code reads gCardListView again instead of through `view`. */
            gCardListView.cursorMoveDir = CARDLIST_CURSOR_IDLE;
            gMain.bgVofs[1] = -(gCardListView.cursorRow << 4);
            CardListView_DrawSelectedInfo();
            CardListView_DrawSelectedCursorFrame();
        }
    }
    if (gCardListView.showSprites) {
        CardListView_DrawButtons(gCardListView.button, gCardListView.buttonMask);
        if (idle && gCardListView.count)
            CardListView_DrawCardStatus(
                (struct DuelCard *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow]);
    }
}

/*
 * Step 1 of gCardListViewSteps (CARDLIST_STEP_INPUT). Returns 1 to close the viewer (Exit, Decide or B).
 * States 1-4 open Card Detail for the selected entry and come back (enum CardListViewInputState); states 0
 * and 5 take input:
 *   Up / Down    move the cursor box (a 4-frame slide), or scroll the page at the top / bottom row; error
 *                sound at either end of the list
 *   Right / Left the next / previous enabled button
 *   B            close, if Exit is enabled
 *   A            the selected button: Card View (refused for the opponent's face-down banished cards), Exit,
 *                Arrange (only a sound) or Decide
 */
u16 CardListView_HandleInput(void)
{
    int index = gCardListView.top + gCardListView.cursorRow;
    u32 id = CARD_ID(gCardListView.cards[index]);

    switch (gCardListView.state) {
    case CARDLIST_INPUT_FADE_OUT:
        if (FadeToBlackU16(4)) {
            gCardListView.showSprites = 0;
            gCardListView.state++;
        }
        break;
    case CARDLIST_INPUT_OPEN_DETAIL:
        CardDetail_Init(id, 0, 0);
        gCardListView.state++;
        break;
    case CARDLIST_INPUT_WAIT_DETAIL:
        if (CardDetail_Run()) {
            gCardListView.initState = 0;
            gCardListView.state++;
        }
        break;
    case CARDLIST_INPUT_REINIT:
        if (CardListView_InitScreen()) {
            gCardListView.initState = 0;
            gCardListView.state++;
        }
        break;
    default:
        if (gCardListView.cursorMoveDir != CARDLIST_CURSOR_IDLE)
            break;
        if (gMain.newKeys & DPAD_UP) {
            if (index > 0) {
                if (gCardListView.cursorRow) {
                    gCardListView.cursorMoveDir = CARDLIST_CURSOR_UP;
                    gCardListView.cursorMoveTimer = CARDLIST_CURSOR_SLIDE_FRAMES;
                } else {
                    gCardListView.top--;
                    CardListView_DrawPage();
                    CardListView_DrawSelectedInfo();
                    CardListView_DrawSelectedCursorFrame();
                }
                PlaySE(SE_CURSOR);
            } else {
                PlaySE(SE_ERROR);
            }
        }
        if (gMain.newKeys & DPAD_DOWN) {
            if (index < gCardListView.count - 1) {
                if (gCardListView.cursorRow <= 2) {
                    gCardListView.cursorMoveDir = CARDLIST_CURSOR_DOWN;
                    gCardListView.cursorMoveTimer = CARDLIST_CURSOR_SLIDE_FRAMES;
                } else {
                    gCardListView.top++;
                    CardListView_DrawPage();
                    CardListView_DrawSelectedInfo();
                    CardListView_DrawSelectedCursorFrame();
                }
                PlaySE(SE_CURSOR);
            } else {
                PlaySE(SE_ERROR);
            }
        }
        if (gMain.newKeys & DPAD_RIGHT) {
            PlaySE(SE_CURSOR);
            do {
                gCardListView.button++;
            } while (!((gCardListView.buttonMask >> gCardListView.button) & 1));
        }
        if (gMain.newKeys & DPAD_LEFT) {
            PlaySE(SE_CURSOR);
            do {
                gCardListView.button--;
            } while (!((gCardListView.buttonMask >> gCardListView.button) & 1));
        }
        if ((gMain.newKeys & B_BUTTON) && (gCardListView.buttonMask & (1 << CARDLIST_BUTTON_EXIT))) {
            PlaySE(SE_CANCEL);
            return 1;
        }
        if (gMain.newKeys & A_BUTTON) {
            switch (gCardListView.button) {
            case CARDLIST_BUTTON_CARD_VIEW:
                if (gCardListView.mode == CARDLIST_MODE_BANISHED) {
                    int player = gCardListView.player;
                    int i = gCardListView.top + gCardListView.cursorRow;
                    if ((u8)gDuelPlayers[player & 1].banishedInfo[i] == BANISH_FACE_DOWN && player) {
                        PlaySE(SE_ERROR);
                        break;
                    }
                }
                PlaySE(SE_CONFIRM);
                gCardListView.state = CARDLIST_INPUT_FADE_OUT;
                break;
            case CARDLIST_BUTTON_EXIT:
                PlaySE(SE_CANCEL);
                return 1;
            case CARDLIST_BUTTON_ARRANGE:
                PlaySE(SE_CONFIRM);
                break;
            case CARDLIST_BUTTON_DECIDE:
                PlaySE(SE_CONFIRM);
                return 1;
            }
        }
        break;
    }
    return 0;
}

/*
 * Runs the viewer for one frame while it is active: CardListView_Update, then the current step; when the step
 * returns non-zero the next one starts (state, initState and unk4 reset). Returns 1 while active; at the end
 * of gCardListViewSteps the viewer turns itself off and returns 0.
 */
u16 CardListView_Run(void)
{
    struct CardListView *view = &gCardListView;

    if (view->active) {
        if (gCardListViewSteps[view->step] != NULL) {
            CardListView_Update();
            if (gCardListViewSteps[view->step]()) {
                view->state = 0;
                view->initState = 0;
                view->unk4 = 0;
                view->step++;
            }
            return 1;
        }
        view->active = 0;
    }
    return 0;
}

/*
 * Opens the viewer on one of `player`'s piles (area DUEL_AREA_GRAVEYARD, FUSION_DECK, BANISHED or DECK), with
 * only Exit enabled, or for area -1 on the effect targets of card number `cardNumber` (CollectEffectTargets
 * fills cards[], sources[] and count), with only Decide enabled. Any other area leaves the viewer off.
 */
void CardListView_Open(int player, int area, int cardNumber, int arg)
{
    struct DuelCard *src;
    int copy = 0;
    int i;

    MemClear16(gCardListView.cards, sizeof(gCardListView.cards));
    gCardListView.player = player & 1;
    gCardListView.count = 0;
    gCardListView.buttonMask = 0;
    gCardListView.button = 0;
    switch (area) {
    case DUEL_AREA_GRAVEYARD:
        /* CARDLIST_MODE_YOUR_GRAVEYARD or CARDLIST_MODE_OPPONENT_GRAVEYARD */
        gCardListView.mode = player;
        gCardListView.buttonMask = 1 << CARDLIST_BUTTON_EXIT;
        gCardListView.count = gDuelPlayers[player & 1].graveCount;
        src = gDuelPlayers[player & 1].graveyard;
        copy = 1;
        break;
    case DUEL_AREA_FUSION_DECK:
        gCardListView.mode = CARDLIST_MODE_FUSION_DECK;
        gCardListView.buttonMask = 1 << CARDLIST_BUTTON_EXIT;
        gCardListView.count = gDuelPlayers[player & 1].fusionCount;
        src = gDuelPlayers[player & 1].fusionDeck;
        copy = 1;
        break;
    case DUEL_AREA_BANISHED:
        gCardListView.mode = CARDLIST_MODE_BANISHED;
        gCardListView.buttonMask = 1 << CARDLIST_BUTTON_EXIT;
        gCardListView.count = gDuelPlayers[player & 1].banishedCount;
        src = gDuelPlayers[player & 1].banished;
        copy = 1;
        break;
    case DUEL_AREA_DECK:
        gCardListView.mode = CARDLIST_MODE_DECK;
        gCardListView.buttonMask = 1 << CARDLIST_BUTTON_EXIT;
        gCardListView.count = gDuelPlayers[player & 1].deckCount;
        src = gDuelPlayers[player & 1].deck;
        copy = 1;
        break;
    case -1:
        gCardListView.mode = CARDLIST_MODE_TARGETS;
        gCardListView.buttonMask = 1 << CARDLIST_BUTTON_DECIDE;
        CollectEffectTargetsInt(player, cardNumber, arg);
        break;
    default:
        gCardListView.active = 0;
        return;
    }
    if (copy) {
        struct DuelCard *dst = (struct DuelCard *)gCardListView.cards;
        for (i = 0; i < gCardListView.count; i++)
            CopyDuelCard((u32 *)dst++, (u32 *)src++);
    }
    gCardListView.cursorRow = 0;
    gCardListView.cursorMoveTimer = 0;
    gCardListView.cursorMoveDir = CARDLIST_CURSOR_IDLE;
    gCardListView.top = 0;
    gCardListView.active = 1;
    gCardListView.step = CARDLIST_STEP_INIT;
}

/* ======================================================================================================== */
/* Target checks                                                                                            */
/* ======================================================================================================== */

/*
 * Can the effect of card `cardId` target the card in (player, zone)? 0 if the zone is empty; 1 if the card is
 * face down. A face-up card cannot be targeted if it is a Dragon (its effective type) while Lord of D. is face
 * up on either side, or if it is card number 1326 or 1329 (not in this game) under Umi and `cardId` is a Magic
 * card other than an Equip Magic.
 */
u16 CanCardTargetZone(u16 cardId, int player, int zone)
{
    u16 ok = 1;
    int p = player & 1;
    struct DuelZone *target = ZONE_AT(p, zone);
    u32 targetId = CARD_ID(CARD_WORD(target->card));

    if (targetId == 0)
        return 0;
    if (!target->isFaceUp)
        return 1;
    if (GetZoneCardType(player, zone) == CARD_TYPE_DRAGON) {
        if (CountFaceUpMonstersByNumber(0, CARD_LORD_OF_D) > 0)
            ok = 0;
        if (CountFaceUpMonstersByNumber(1, CARD_LORD_OF_D) > 0)
            ok = 0;
    }
    /* 1329 (0x531) has no CARD_ name: no card in this game has that number. */
    if ((CARD_NUMBER(targetId) == CARD_1326 || CARD_NUMBER(targetId) == 1329)
        && GetFaceUpFieldMagicNumber() == CARD_UMI && CARD_TYPE(cardId) == CARD_TYPE_MAGIC
        && CARD_SUBTYPE(cardId) != SPELL_EQUIP)
        ok = 0;
    return ok;
}

/*
 * The card-independent part of CanCardTargetZone: 1 if (player, zone) holds a card that is not a face-up card
 * number 1326 or 1329 under Umi, else 0.
 */
int IsZoneTargetable(int player, int zone)
{
    int p = player & 1;
    struct DuelZone *target = ZONE_AT(p, zone);
    u32 id = CARD_ID(CARD_WORD(target->card));

    if (id == 0)
        return 0;
    if ((CARD_NUMBER(id) == CARD_1326 || CARD_NUMBER(id) == 1329) && GetFaceUpFieldMagicNumber() == CARD_UMI
        && target->isFaceUp)
        return 0;
    return 1;
}

/*
 * Check of Larvae Moth, Great Moth, Perfectly Ultimate Great Moth and Wall Shadow, whose summon tributes an
 * equipped monster: the target must be one of entry's player's own monsters (zones 0-4) that is Petit Moth
 * (Labyrinth Wall for Wall Shadow), card 1418 must be on neither field, and one of the target's equip links
 * must be Cocoon of Evolution (Magical Labyrinth) with a turn counter above the summoned card's limit: 1 for
 * Larvae Moth, 3 for Great Moth, 5 for Perfectly Ultimate Great Moth, none for Wall Shadow.
 *
 * Matching: the u16 cid temporary adds pre-combine insns after the last use of the 0xD64 constant, which
 * lengthens the zone * 0x94 invariant's life, so the constant gets r7 and zone * 0x94 gets ip, as in the ROM.
 */
int EffectEquippedTributeCheck(struct ChainEntry *entry, u16 pos)
{
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    u32 id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(player & 1, zone)->card));
    u16 equipNo = CARD_COCOON_OF_EVOLUTION;
    u16 targetNo = CARD_PETIT_MOTH;
    int limit;
    int i;

    if (id == 0 || zone > ZONE_MONSTER_4 || player != entry->player)
        return 0;
    switch (CARD_NUMBER(entry->card)) {
    case CARD_LARVAE_MOTH:
        limit = 1;
        break;
    case CARD_GREAT_MOTH:
        limit = 3;
        break;
    case CARD_PERFECTLY_ULTIMATE_GREAT_MOTH:
        limit = 5;
        break;
    case CARD_WALL_SHADOW:
        limit = -1;
        equipNo = CARD_MAGICAL_LABYRINTH;
        targetNo = CARD_LABYRINTH_WALL;
        break;
    default:
        return 0;
    }
    if (CARD_NUMBER(id) != targetNo)
        return 0;
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0)
        return 0;
    for (i = 0; i < ZONE_AT_PLAYER_FIRST(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT_PLAYER_FIRST(player & 1, zone)->links[i];

        if ((u8)ZONE_AT_PLAYER_FIRST(player & 1, zone)->linkKinds[i] == ZONE_LINK_EQUIP) {
            int linkPlayer = LOC_PLAYER(link);
            int linkZone = LOC_ZONE(link);
            int p = linkPlayer & 1;
            struct DuelZone *equip = ZONE_AT(p, linkZone);
            u16 cid = CARD_ID(CARD_WORD(equip->card));

            if (CARD_NUMBER(cid) == equipNo && equip->turnCounter > limit)
                return 1;
        }
    }
    return 0;
}

/*
 * Check of Reaper of the Cards and Trap Master: an occupied spell/trap or field zone (5-10) whose card is face
 * down or a face-up Trap.
 */
int EffectTrapTargetCheck(struct ChainEntry *entry, u16 pos)
{
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    int p = player & 1;
    struct DuelZone *target = ZONE_AT(p, zone);
    int id = CARD_ID(CARD_WORD(target->card));
    /* Matching: a u16 copy of the ID, taken before the tests, indexes the card table. */
    u16 copy;
    int type;

    copy = id;
    if (id == 0 || (u32)(zone - ZONE_SPELL_0) > ZONE_FIELD - ZONE_SPELL_0)
        return 0;
    if (target->isFaceUp) {
        type = CARD_TYPE(copy);
        return type == CARD_TYPE_TRAP;
    }
    return 1;
}

/*
 * Check shared by Crass Clown, Dream Clown, Spellbinding Circle, Change of Heart and other effects that target
 * an opponent's monster: an occupied monster zone (0-4) of the other player that CanCardTargetZone allows.
 */
int EffectOpponentMonsterCheck(struct ChainEntry *entry, u16 pos)
{
    int zone;
    int player;

    player = LOC_PLAYER(pos);
    zone = LOC_ZONE(pos);

    if (entry->player != player && zone <= ZONE_MONSTER_4 && CanCardTargetZone(entry->card, player, zone)) {
        int p = player & 1;
        struct DuelZone *target = ZONE_AT(p, zone);
        if (CARD_ID(CARD_WORD(target->card)))
            return 1;
    }
    return 0;
}

/*
 * Check of the equip cards: the target must be a face-up monster (zones 0-4) that CanCardTargetZone allows.
 * Then, by the equip card's number: any monster; only the player's own (Kunai with Chain, Ring of Magnetism);
 * a monster of one effective type (GetZoneCardType) or attribute (GetZoneCardAttribute), or not a Machine
 * (Germ Infection, Paralyzing Potion); or one particular monster (Cocoon of Evolution: the player's own Petit
 * Moth not cocooned yet; Cyber Shield: Harpie Lady or Harpie Lady Sisters; Magical Labyrinth: Labyrinth Wall).
 */
int EffectEquipTargetCheck(struct ChainEntry *entry, u16 pos)
{
    u16 equipId = entry->card;
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    /* Matching: the target's card is read once here through the player's zone array plus a byte offset, and
     * again below through ZONE_AT. */
    struct DuelZonesPlayer *zones = &gDuelZones[player & 1];
    struct DuelZone *first = (struct DuelZone *)zones;
    u32 targetId;
    u16 type;
    u16 attribute;
    struct DuelZone *target;

    first = (struct DuelZone *)((u32)first + zone * sizeof(struct DuelZone));
    targetId = CARD_ID(CARD_WORD(first->card));
    type = GetZoneCardType(player, zone);
    attribute = GetZoneCardAttribute(player, zone);

    if (zone > ZONE_MONSTER_4)
        return 0;
    target = ZONE_AT(player & 1, zone);
    if (!CARD_ID(CARD_WORD(target->card)) || !target->isFaceUp
        || !CanCardTargetZone(entry->card, player, zone))
        return 0;
    switch (CARD_NUMBER(equipId)) {
    case CARD_GERM_INFECTION:
    case CARD_PARALYZING_POTION:
        return type != CARD_TYPE_MACHINE;
    case CARD_KUNAI_WITH_CHAIN:
    case CARD_RING_OF_MAGNETISM:
        return entry->player == player;
    case CARD_COCOON_OF_EVOLUTION:
        /* the player's own Petit Moth, not cocooned yet */
        if (CARD_NUMBER(targetId) != CARD_PETIT_MOTH)
            return 0;
        if (entry->player != player)
            return 0;
        if (CountZoneLinksFromCard(player, zone, CARD_COCOON_OF_EVOLUTION) != 0)
            return 0;
        return 1;
    case CARD_CYBER_SHIELD:
        switch (CARD_NUMBER(targetId)) {
        case CARD_HARPIE_LADY:
        case CARD_HARPIE_LADY_SISTERS:
        case 1249:  /* no card in this game */
            return 1;
        }
        return 0;
    case CARD_MAGICAL_LABYRINTH:
        if (CARD_NUMBER(targetId) == CARD_LABYRINTH_WALL)
            return 1;
        return 0;
    case CARD_1540:
        if (CARD_NUMBER(targetId) == 1339)  /* no card in this game */
        return_true:    /* Matching: the cases below jump here (a shared return 1), as in the ROM's block order */
            return 1;
        return 0;
    case CARD_LASER_CANNON_ARMOR:
    case CARD_INSECT_ARMOR_WITH_LASER_CANNON:
        return type == CARD_TYPE_INSECT;
    case CARD_MACHINE_CONVERSION_FACTORY:
    case CARD_7_COMPLETED:
    case CARD_1314:
        return type == CARD_TYPE_MACHINE;
    case CARD_SILVER_BOW_AND_ARROW:
        return type == CARD_TYPE_FAIRY;
    case CARD_RAISE_BODY_HEAT:
        return type == CARD_TYPE_DINOSAUR;
    case CARD_POWER_OF_KAISHIN:
        return type == CARD_TYPE_AQUA;
    case CARD_ELECTRO_WHIP:
        return type == CARD_TYPE_THUNDER;
    case CARD_LEGENDARY_SWORD:
    case CARD_SWORD_OF_DRAGONS_SOUL:
    case CARD_1422:
    case CARD_1550:
        return type == CARD_TYPE_WARRIOR;
    case CARD_BOOK_OF_SECRET_ARTS:
        return type == CARD_TYPE_SPELLCASTER;
    case CARD_DRAGON_TREASURE:
        return type == CARD_TYPE_DRAGON;
    case CARD_FOLLOW_WIND:
        return type == CARD_TYPE_WINGED_BEAST;
    case CARD_VILE_GERMS:
        return type == CARD_TYPE_PLANT;
    case CARD_MYSTICAL_MOON:
        return type == CARD_TYPE_BEAST_WARRIOR;
    case CARD_VIOLET_CRYSTAL:
        return type == CARD_TYPE_ZOMBIE;
    case CARD_BEAST_FANGS:
        return type == CARD_TYPE_BEAST;
    case CARD_DARK_ENERGY:
        return type == CARD_TYPE_FIEND;
    case CARD_INVIGORATION:
        return attribute == ATTRIBUTE_EARTH;
    case CARD_STEEL_SHELL:
        return attribute == ATTRIBUTE_WATER;
    case CARD_SALAMANDRA:
    case CARD_BURNING_SPEAR:
        return attribute == ATTRIBUTE_FIRE;
    case CARD_ELFS_LIGHT:
    case CARD_BRIGHT_CASTLE:
        return attribute == ATTRIBUTE_LIGHT;
    case CARD_GUST_FAN:
        return attribute == ATTRIBUTE_WIND;
    case CARD_SWORD_OF_DARK_DESTRUCTION:
        return attribute == ATTRIBUTE_DARK;
    /* any face-up monster */
    case CARD_AXE_OF_DESPAIR:
    case CARD_BLACK_PENDANT:
    case CARD_HORN_OF_LIGHT:
    case CARD_HORN_OF_THE_UNICORN:
    case CARD_MALEVOLENT_NUZZLER:
    case CARD_MEGAMORPH:
    case CARD_METALMORPH:
    case CARD_SWORD_OF_DEEP_SEATED:
    case CARD_STIM_PACK:
    case CARD_1258:
    case CARD_1313:
    case CARD_1419:
    case CARD_1420:
    case CARD_1448:
    case CARD_1449:
    case CARD_1450:
    case CARD_1548:
        goto return_true;
    }
    return 0;
}

/*
 * Check of Stop Defense: an occupied monster zone (0-4) of the other player that CanCardTargetZone allows;
 * returns its isDefense bit (only a monster in defense position qualifies).
 */
int EffectStopDefenseCheck(struct ChainEntry *entry, u16 pos)
{
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    struct DuelZone *target;

    if (zone <= ZONE_MONSTER_4 && player != entry->player) {
        int p = player & 1;
        target = ZONE_AT(p, zone);
        if (CARD_ID(CARD_WORD(target->card)) && CanCardTargetZone(entry->card, player, zone))
            return ZONE_FLAGS(target) & ZONE_FLAG_DEFENSE;  /* isDefense */
    }
    return 0;
}

/*
 * Check of Blast Juggler: an occupied, face-up card with an effective ATK of at most 1000 that
 * CanCardTargetZone allows, other than Blast Juggler itself. The zone is not range-checked.
 */
int EffectBlastJugglerCheck(struct ChainEntry *entry, u16 pos)
{
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    int p = player & 1;
    struct DuelZone *target = ZONE_AT(p, zone);

    /* Matching: GetZoneCardAtk (u32 in duel.h) is compared as signed. */
    if (!CARD_ID(CARD_WORD(target->card)) || !target->isFaceUp
        || (int)GetZoneCardAtk(player, zone) > 1000
        || !CanCardTargetZone(entry->card, player, zone)
        || (player == entry->player && zone == entry->zone))
        return 0;
    return 1;
}

/*
 * Check of Armed Ninja and De-Spell: an occupied spell/trap or field zone (5-10) whose card is face down or a
 * face-up Magic card (not a face-up Trap).
 */
int EffectMagicTargetCheck(struct ChainEntry *entry, u16 pos)
{
    int player = LOC_PLAYER(pos);
    int zone = LOC_ZONE(pos);
    int p = player & 1;
    struct DuelZone *target = ZONE_AT(p, zone);
    int id = CARD_ID(CARD_WORD(target->card));
    /* Matching: a u16 copy of the ID, taken before the tests, indexes the card table. */
    u16 copy;
    int type;

    copy = id;
    if (id == 0 || (u32)(zone - ZONE_SPELL_0) > ZONE_FIELD - ZONE_SPELL_0)
        return 0;
    if (target->isFaceUp && (type = CARD_TYPE(copy)) == CARD_TYPE_TRAP)
        return 0;
    return 1;
}
