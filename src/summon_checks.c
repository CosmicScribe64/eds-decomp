/*
 * Painful Choice's five-card prompt, the hand-summon legality checks, the summon position prompt and the step
 * machines of the two hand-summon actions (wiki/functions/summon-checks-c.md).
 *
 * DuelPrompt_PickOneOfFiveCards is prompt kind 12 (PROMPT_PICK_ONE_OF_FIVE_CARDS): one player picks one of the
 * five card IDs in gDuel.promptArgs, with FiveCardMenu_Draw and FiveCardMenu_HandleInput as the menu callbacks.
 *
 * CanSummonFromHand decides whether a hand monster can be summoned now: fusion and ritual monsters never, Toon
 * monsters only with Toon World, the cards with their own procedure (Gate Guardian, Valkyrion, the Moths, the
 * Graveyard-banish Special Summons of the non-EDS keys 1514-1519) by that procedure, and everything else by
 * its level (1-4 needs a free monster zone, 5-6 one tributable monster, 7 and up two). CanSummonValkyrion,
 * CanSummonKey1257 and CanPayBanishSummonCost are its helpers.
 *
 * ExecuteSummonAction and ExecuteSummonActionAskPosition are the step machines of the summon records of kind
 * SUMMON_ACTION_NORMAL (1) and SUMMON_ACTION_NORMAL_CHOOSE_POSITION (2) in gSummonAction (summon.h), run by
 * SummonAction_Update (summon_action.c). The second asks Attack or Defense first, with the SummonPositionMenu_*
 * callbacks.
 */
#include "global.h"
#include "gba.h"                    /* A_BUTTON, DPAD_LEFT, DPAD_RIGHT */
#include "main.h"                   /* gMain.newKeys, gMain.frameCounter */
#include "sound.h"                  /* PlaySE */
#include "util.h"                   /* Random */
#include "sprite.h"                 /* AddAffineSprite, SPRITE_SHAPE_32x32 */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu, TextBoxMenuState */
#include "card_data.h"              /* gCardIdToNumber, gCardStats, CARD_ID_MASK, CARD_STATS_* */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute, CardKind */
#include "constants/duel.h"         /* enum DuelArea, ZONE_*, RESPONSE_SUMMONED, CHAIN_KIND_MONSTER */
#include "constants/duel_cmds.h"    /* DUEL_CMD_*, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM */
#include "duel.h"                   /* gDuel, gDuelPlayers, gDuelZones, gDuelGraveyards, struct DuelCard / DuelZone /
                                     * DuelPlayer, the card and zone counters, IsToonMonster, IsCardProhibited */

#include "chain.h"                  /* Chain_AddPending */
#include "summon.h"                 /* struct SummonAction, gSummonAction, the summon rules */
#include "duel_actions.h"           /* TributeMonster, ChangeBattlePosition, DestroyFieldCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gPulseScaleCurve, gStrSelectDisplayPosition */
#include "duel_prompt.h"            /* the five-card menu prototypes */
#include "duel_screen.h"            /* DuelCursor_Select, GetCardIconObjTile, DuelInfo_DrawCard, TextCellsClear */
#include "ai.h"                     /* AiShouldSetMonster */
#include "effect.h"                 /* CanActivateEffectOfCard, TriggerMysteriousPuppeteer */

/* ROM data used only here. */
extern const u8 gStrPromptSelectOneOfFive[];            /* "Select 1 card from out of 5." */

/* Text boxes, in cells: the five-card prompt at (6, 2), 19 x 2; the position prompt at (7, 2), 15 x 3. */
#define BOX_ONE_OF_FIVE_POS     TEXTBOX_POS(6, 2)
#define BOX_ONE_OF_FIVE_SIZE    TEXTBOX_SIZE(19, 2)
#define BOX_POSITION_POS        TEXTBOX_POS(7, 2)
#define BOX_POSITION_SIZE       TEXTBOX_SIZE(15, 3)

/* gCardIdToNumber, gCardStats and gPulseScaleCurve through their integer addresses (0x08622AB4, 0x08621DE0,
 * 0x081A4424): the ROM's register allocation needs these forms (the symbols give other code). */
#define CARD_NUMBER_C(id)       (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_STATS_C(id)        (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE_C(id)         CARD_STATS_TYPE(CARD_STATS_C(id))
#define PULSE_SCALE_CURVE       ((const u16 *)0x081A4424)

/* Card ID, bits 0-11, of a card word. */
#define CARD_WORD_ID(word)      (((word) << 20) >> 20)
/* The card word of zone (player, zone), with the address terms in the ROM's order: player * 0xD64 + zone * 0x94 +
 * the base of gDuelZones. */
#define ZONE_CARD_WORD(player, zone) \
    (*(u32 *)(((player) & 1) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/*
 * Monster level of a card ID, as the prompts use it: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards,
 * else the stars in the stats word.
 */
static inline int GetCardLevel(u16 cardId)
{
    switch ((int)CARD_TYPE_C(cardId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_C(cardId));
    }
}

/*
 * enum CardKind of a card ID, the frame kind: Obelisk counts as a Ritual monster, Slifer and Ra as Effect
 * monsters; Magic, Trap and Ticket cards have their own kinds; other monsters use the stats kind bits.
 */
static inline int GetCardKind(u16 cardId)
{
    switch (CARD_NUMBER_C(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_TYPE_C(cardId)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_C(cardId));
    }
}

/* Chain_AddPending trigger word, as ChainEntry lays it out: player << 31 | event << 25 | kind << 21 | zone << 16 |
 * card ID. These are the bits that do not depend on the card: a monster-kind RESPONSE_SUMMONED trigger. */
#define SUMMONED_TRIGGER_BITS   ((RESPONSE_SUMMONED << 25) | (CHAIN_KIND_MONSTER << 21))

/*
 * Draw callback of the five-card pick: the card IDs gDuel.promptArgs[0..4] as 32x32 affine card sprites at
 * x = 0x28 + 32 * slot, y = (revealRow - height + 2) * 8. The selected card (gTextBox.result) pulses with
 * gPulseScaleCurve; the others keep scale 0x100.
 */
void FiveCardMenu_Draw(void)
{
    int y = (gTextBox.revealRow - gTextBox.height + 2) * 8;
    int i = 0;
    /* Matching: the pulse curve is read by its integer address (the symbol gPulseScaleCurve gives other code).
     * FAKEMATCH: promptArgs is reached as gDuel + 0x1B52 through the two locals base and off; the member
     * gDuel.promptArgs gives other code. */
    u32 base = (u32)&gDuel;
    int x = 0x28;
    u32 off = OFFSET_OF(struct DuelState, promptArgs);
    u16 *cards;
    u16 *frame;

    cards = (u16 *)(base + off);

    frame = &gMain.frameCounter;
    do {
        u16 id = *cards;
        u16 pulse = PULSE_SCALE_CURVE[(*frame >> 1) & 0xF];
        u32 yx;
        u16 tile;
        if (i != gTextBox.result)
            pulse = 0x100;
        yx = ((u32)y << 16) | (u32)x;
        tile = GetCardIconObjTile(id) + 0x1000;     /* OBJ palette 1 */
        AddAffineSprite(yx, SPRITE_SHAPE_32x32, tile, (u32)pulse << 16);
        x += 0x20;
        cards++;
    } while (++i <= 4);
}


/*
 * Input callback of the five-card pick: returns 1 on A. The first frame (menuState 0) shows the info of
 * the selected card; then Left and Right move the selection (modulo 5) and redraw the info.
 */
u16 FiveCardMenu_HandleInput(void)
{
    struct TextBox *box = &gTextBox;
    u8 *state = &box->menuState;
    if (*state == TEXTBOX_MENU_STATE_SELECT) {
        DuelInfo_DrawCard(gDuel.promptArgs[box->result], 1);
        (*state)++;
        return 0;
    }
    if (gMain.newKeys & DPAD_LEFT)
        box->result = box->result + 4;
    else if (gMain.newKeys & DPAD_RIGHT)
        box->result = box->result + 1;
    else
        goto check_a;
    box->result = box->result % 5;
    TextCellsClear();
    DuelInfo_DrawCard(gDuel.promptArgs[box->result], 1);
    return 0;
check_a:    /* FAKEMATCH: the shared tail is a goto target, as in the ROM's block order */
    if (gMain.newKeys & A_BUTTON)
        return 1;
    return 0;
}

/*
 * PROMPT_PICK_ONE_OF_FIVE_CARDS (Painful Choice): the answering player picks one of the five card IDs in
 * promptArgs. The CPU's prompt (promptPlayer) picks Random() % 5; the human gets the text box with the
 * five-card menu and the next call takes the menu's result. Either way promptResult = the chosen card ID.
 * Returns 1 when finished.
 */
int DuelPrompt_PickOneOfFiveCards(void)
{
    struct DuelState *duel = &gDuel;
    int pick;
    /* FAKEMATCH: retain the initialized step pointer across menu setup. */
    register u8 *step asm("r5");
    if (duel->promptPlayer) {
        pick = Random() % 5;
        goto store;     /* FAKEMATCH: the store below is shared by a goto into the if body (the ROM's block order) */
    }
    {
        /* FAKEMATCH: initialized base/offset copies retain the ADD order. */
        register u32 base asm("r4") = (u32)duel;
        register u32 off asm("r0") = OFFSET_OF(struct DuelState, promptStep);
        asm("" : "+r"(off));
        step = (u8 *)(base + off);
    }
    if (*step != 0) {
        pick = gTextBox.result;
    store:
        {
            u32 byteOffset = (u32)pick << 1;
            u32 argsOffset = OFFSET_OF(struct DuelState, promptArgs);
            u32 args = (u32)duel + argsOffset;
            duel->promptResult = *(u16 *)(byteOffset + args);
        }
        return 1;
    }
    TextBoxOpen(BOX_ONE_OF_FIVE_POS, BOX_ONE_OF_FIVE_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectOneOfFive);
    TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, FiveCardMenu_Draw, FiveCardMenu_HandleInput);
    (*step)++;
    return 0;
}


/*
 * Valkyrion the Magna Warrior's summon condition. Alpha, Beta and Gamma The Magnet Warrior must each be in
 * the hand or face up on the field; field copies count only while key 1418 (which forbids Tributes) is
 * active on neither field. The CPU (player 1) must also hold Valkyrion in its hand. Returns 0 when no monster
 * zone is free and none of the three is on the field (to be Tributed), else 1.
 */
int CanSummonValkyrion(int player)
{
    int hasAlpha = 0, hasBeta = 0, hasGamma = 0;
    int tributesAllowed = 1;
    int fieldCount = 0;
    u16 noTributeKey = CARD_1418;
    if (CountActiveCardsOnField(0, noTributeKey) > 0)
        tributesAllowed = 0;
    if (CountActiveCardsOnField(1, noTributeKey) > 0)
        tributesAllowed = 0;
    if (player != 0 && CountHandCardsByNumber(player, CARD_VALKYRION_THE_MAGNA_WARRIOR) == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(player, CARD_ALPHA_THE_MAGNET_WARRIOR) && tributesAllowed) {
        hasAlpha = 1;
        fieldCount++;
    }
    if (CountHandCardsByNumber(player, CARD_ALPHA_THE_MAGNET_WARRIOR))
        hasAlpha = 1;
    if (hasAlpha == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(player, CARD_BETA_THE_MAGNET_WARRIOR) && tributesAllowed) {
        hasBeta = 1;
        fieldCount++;
    }
    if (CountHandCardsByNumber(player, CARD_BETA_THE_MAGNET_WARRIOR))
        hasBeta = 1;
    if (hasBeta == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(player, CARD_GAMMA_THE_MAGNET_WARRIOR) && tributesAllowed) {
        hasGamma = 1;
        fieldCount++;
    }
    if (CountHandCardsByNumber(player, CARD_GAMMA_THE_MAGNET_WARRIOR))
        hasGamma = 1;
    if (hasGamma == 0)
        return 0;
    if (CountFreeMonsterZones(player) == 0 && fieldCount == 0)
        return 0;
    return 1;
}

/*
 * Summon condition of key 1257 (not an EDS card): key 1418 (no Tributes) active on neither field, one of the
 * keys 1410 or 1412 among the player's monsters (face up or down), and more than one tributable monster.
 */
int CanSummonKey1257(int player)
{
    int hasKeyMonster = 0;
    u16 noTributeKey = CARD_1418;
    if (CountActiveCardsOnField(0, noTributeKey) > 0 || CountActiveCardsOnField(1, noTributeKey) > 0)
        return 0;
    if (CountMonstersByNumber(player, CARD_1410))
        hasKeyMonster = 1;
    if (CountMonstersByNumber(player, CARD_1412))
        hasKeyMonster = 1;
    if (hasKeyMonster != 0 && CountTributableMonsters(player, -1) > 1)
        return 1;
    return 0;
}

/*
 * Can the player pay the "remove monsters from play" Special Summon cost of card `cardId` (the non-EDS keys
 * 1514-1519)? 1514 needs 3 Fiends, 1515 two LIGHT monsters, 1516 FIRE, 1517 WATER, 1518 EARTH and 1519 WIND
 * one each. Normally the monsters are counted in the player's Graveyard, and the cost cannot be paid while key
 * 1511 is active on the opponent's field. With banishCostFromField set it counts the monsters in the
 * monster zones by GetZoneCardType / GetZoneCardAttribute instead and ignores key 1511. Returns 1 as soon as
 * enough are found.
 */
int CanPayBanishSummonCost(int player, u16 cardId)
{
    int need = 1;
    int attribute = 0;
    int i;
    int fromField;
    /* FAKEMATCH: retain the initialized attribute across the opponent-card test. */
    asm("" : "+r"(attribute));
    fromField = gDuelPlayers[player & 1].banishCostFromField;
    if (CountActiveCardsOnField(1 - player, CARD_1511) > 0 && fromField == 0)
        return 0;
    switch (CARD_NUMBER_C(cardId)) {
    case CARD_1514:
        need = 3;
        if (fromField) {
            for (i = 0; i <= 4; i++) {
                if (ZONE_CARD_WORD(player, i) << 20 != 0 && (int)GetZoneCardType(player, i) == CARD_TYPE_FIEND) {
                    if (--need == 0)
                        return 1;
                }
            }
        } else {
            for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
                /* FAKEMATCH: preserve the initialized grave-base load. */
                register u32 graveBase asm("r0") = (u32)gDuelGraveyards;
                u32 offset = (player & 1) * sizeof(struct DuelPlayer) + i * sizeof(struct DuelCard);
                u16 id;

                id = CARD_WORD_ID(*(u32 *)(offset + graveBase));
                if (CARD_TYPE_C(id) == CARD_TYPE_FIEND) {
                    if (--need == 0)
                        return 1;
                }
            }
        }
        return 0;
    case CARD_1515:
        need = 2;
        attribute = ATTRIBUTE_LIGHT;
        break;
    case CARD_1516:
        attribute = ATTRIBUTE_FIRE;
        break;
    case CARD_1517:
        attribute = ATTRIBUTE_WATER;
        break;
    case CARD_1518:
        attribute = ATTRIBUTE_EARTH;
        break;
    case CARD_1519:
        attribute = ATTRIBUTE_WIND;
        break;
    }
    if (fromField) {
        for (i = 0; i <= 4; i++) {
            if (ZONE_CARD_WORD(player, i) << 20 != 0 && (int)GetZoneCardAttribute(player, i) == attribute) {
                if (--need == 0)
                    return 1;
            }
        }
    } else {
        for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
            /* FAKEMATCH: each initialized grave base stays in the load scratch. */
            register u32 graveBase asm("r0") = (u32)gDuelGraveyards;
            u32 offset = (player & 1) * sizeof(struct DuelPlayer) + i * sizeof(struct DuelCard);
            u16 id;
            u32 stats;

            id = CARD_WORD_ID(*(u32 *)(offset + graveBase));
            stats = CARD_STATS_C(id);
            if (CARD_STATS_TYPE(stats) <= CARD_TYPE_REPTILE && (int)CARD_STATS_ATTR(stats) == attribute) {
                if (--need == 0)
                    return 1;
            }
        }
    }
    return 0;
}

/*
 * Can the player summon the hand monster `cardId` now? Returns 0 for an empty slot, a card under Prohibition, a
 * non-monster (type above CARD_TYPE_REPTILE), a ritual or fusion monster, and a Toon monster without the
 * player's face-up Toon World. Then per card number:
 *   1514-1519 (non-EDS keys): a Special Summon must be allowed, and a free monster zone (or banishCostFromField)
 *     and CanPayBanishSummonCost;
 *   Red-Eyes Black Metal Dragon, Harpie Lady Sisters, Metalzoa, Dark Sage: never;
 *   Larvae Moth, Great Moth, Perfectly Ultimate Great Moth, Wall Shadow: a Special Summon must be allowed and
 *     CanActivateEffectOfCard;
 *   keys 1250 and 1350: their own hand and monster-count conditions;
 *   Gate Guardian: a Special Summon, more than two tributable monsters and face-up Sanga, Kazejin and Suijin;
 *   Valkyrion: CanSummonValkyrion; key 1257: CanSummonKey1257;
 *   any other monster by level: 1-4 needs a free monster zone, 5-6 a tributable monster, 7 and up two.
 * The result is a zero-extended halfword as the ROM callers consume it.
 */
int CanSummonFromHand(int player, u16 cardId)
{
    u16 number;
    int level;
    if (cardId == 0)
        return 0;
    if (IsCardProhibited(cardId) != 0)
        return 0;
    if (CARD_TYPE_C(cardId) > CARD_TYPE_REPTILE)
        return 0;
    if (GetCardKind(cardId) == CARD_KIND_RITUAL)
        return 0;
    if (GetCardKind(cardId) == CARD_KIND_FUSION)
        return 0;
    number = CARD_NUMBER_C(cardId);
    if (IsToonMonster(number) != 0 && HasFaceUpToonWorld(player) == 0)
        return 0;
    switch (number) {
    case CARD_1514:     /* keys 1514-1519: Special Summons that remove monsters from play (non-EDS) */
    case CARD_1515:
    case CARD_1516:
    case CARD_1517:
    case CARD_1518:
    case CARD_1519:
        if (CanSpecialSummon(player) != 0)
            goto summon_check;  /* FAKEMATCH: the label sits after the Valkyrion case (the ROM's block order) */
        return 0;
    case CARD_RED_EYES_BLACK_METAL_DRAGON:
    case CARD_HARPIE_LADY_SISTERS:
    case CARD_METALZOA:
    case CARD_DARK_SAGE:
        return 0;
    case CARD_LARVAE_MOTH:
    case CARD_GREAT_MOTH:
    case CARD_PERFECTLY_ULTIMATE_GREAT_MOTH:
    case CARD_WALL_SHADOW:
        if (CanSpecialSummon(player) == 0)
            return 0;
        return (u16)(CanActivateEffectOfCard(player, cardId, 1));
    case CARD_1250:
        if (gDuelPlayers[player & 1].handCount == 1 && CountFreeMonsterZones(player) > 0)
            return 1;
        if (CountTributableMonsters(player, -1) > 1)
            return 1;
        return 0;
    case CARD_1350:
        if (CountMonsters(player) + 1 < CountMonsters(1 - player) && CountFreeMonsterZones(player) > 0)
            return 1;
        if (CountTributableMonsters(player, -1) > 0)
            return 1;
        return 0;
    case CARD_GATE_GUARDIAN:
        if (CanSpecialSummon(player) == 0)
            return 0;
        if (CountTributableMonsters(player, -1) <= 2)
            return 0;
        if (CountFaceUpMonstersByNumber(player, CARD_SANGA_OF_THE_THUNDER) == 0)
            return 0;
        if (CountFaceUpMonstersByNumber(player, CARD_KAZEJIN) == 0)
            return 0;
        if (CountFaceUpMonstersByNumber(player, CARD_SUIJIN) == 0)
            return 0;
        return 1;
    case CARD_VALKYRION_THE_MAGNA_WARRIOR:
        if (CanSpecialSummon(player) == 0)
            return 0;
        return (u16)(CanSummonValkyrion(player));
    summon_check:
        if (CountFreeMonsterZones(player) == 0 && !gDuelPlayers[player & 1].banishCostFromField)
            return 0;
        return (u16)(CanPayBanishSummonCost(player, cardId));
    case CARD_1257:
        return (u16)(CanSummonKey1257(player));
    default:
        level = GetCardLevel(cardId);
        /* FAKEMATCH: keeps the initialized level live at the switch join, as in the ROM; emits no instruction. */
        __asm__("" : : "r"(level));
        switch (level) {
        case 5:
        case 6:
            if (CountTributableMonsters(player, -1) <= 0)
                return 0;
            return 1;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            if (CountFreeMonsterZones(player) > 0)
                return 1;
            return 0;
        default:
            if (CountTributableMonsters(player, -1) > 1)
                return 1;
            return 0;
        }
    }
}

/*
 * Draw callback of the summon position menu ('Select display position of card.'): two 32x32 card sprites
 * at x = 0x40 and 0x90, y = textTop * 8 + 0x20 - (height + textTop - revealRow + 2) * 8. The left one shows the
 * record's card upright (Attack Position); the right one is turned a quarter turn (angle 0x20 of 128, Defense
 * Position) and shows the card back (OBJ tile 0x40) unless the record is already face up. The one that
 * gTextBox.result selects (0 = left) pulses with gPulseScaleCurve; the other keeps scale 0x100.
 */
void SummonPositionMenu_Draw(void)
{
    int y = gTextBox.y * 8 + 0x20;
    u32 yx1, yx2;
    u16 tile1, tile2;
    u32 scaleAngle1, scaleAngle2;
    y -= (gTextBox.height + gTextBox.y - gTextBox.revealRow + 2) * 8;
    yx1 = (y << 16) | 0x40;
    tile1 = GetCardIconObjTile(gSummonAction.cardId) | 0x1000;      /* OBJ palette 1 */
    if (gTextBox.result == 0)
        scaleAngle1 = gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16;
    else
        scaleAngle1 = 0x01000000;
    AddAffineSprite(yx1, SPRITE_SHAPE_32x32, tile1, scaleAngle1);
    yx2 = (y << 16) | 0x90;
    if (gSummonAction.isFaceUp)
        tile2 = GetCardIconObjTile(gSummonAction.cardId) | 0x1000;
    else
        tile2 = 0x40;                                               /* card back */
    if (gTextBox.result != 0)
        scaleAngle2 = (gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16) | 0x20;
    else
        scaleAngle2 = 0x01000020;
    AddAffineSprite(yx2, SPRITE_SHAPE_32x32, tile2, scaleAngle2);
}

/*
 * Input callback of the summon position menu: returns 1 when done. SELECT: Left or Right toggles
 * result = 1 - result (SE_CURSOR), A plays SE_CONFIRM and enters CONFIRMED. CONFIRMED: a 60-frame flash on
 * menuTimer, then DONE. DONE: returns 1.
 */
u16 SummonPositionMenu_HandleInput(void)
{
    struct TextBox *box = &gTextBox;
    u8 *state = &box->menuState;
    int s = *state;
    unsigned short t = s; /* FAKEMATCH: the short copy keeps the state byte in r2 and `s + 1` in r2 */
    /* Matching: the (&gTextBox)-> spelling below is kept; through `box` the code differs. */
    switch (t) {
    case TEXTBOX_MENU_STATE_CONFIRMED:
        if ((&gTextBox)->menuTimer <= 0x3B)
            (&gTextBox)->menuTimer++;
        else
            *state = s + 1;
        return 0;
    case TEXTBOX_MENU_STATE_DONE:
        return 1;
    default:
        if (gMain.newKeys & (DPAD_LEFT | DPAD_RIGHT)) {
            PlaySE(SE_CURSOR);
            (&gTextBox)->result = 1 - (&gTextBox)->result;
        }
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CONFIRM);
            (&gTextBox)->menuState = TEXTBOX_MENU_STATE_CONFIRMED;
            (&gTextBox)->menuTimer = 0;
        }
        return 0;
    }
}

/*
 * Step machine of the summon record of kind SUMMON_ACTION_NORMAL (Normal Summon or Set from the hand, with up
 * to two Tributes); returns 1 when done.
 *   0: Tribute the marked monsters (owners tribute1Player / tribute2Player), then push
 *      DUEL_CMD_PLACE_MONSTER_FROM_HAND with arg4 = zone | hand index << 4 | isFaceUp << 8 | isDefense << 9;
 *   1: move the cursor to the zone; a face-down Set ends here (step 10), a face-up summon pushes
 *      DUEL_CMD_SHOW_CARD_ASSEMBLE;
 *   2: if the zone still holds a card, push DUEL_CMD_SET_ZONE_STATUS_FLAGS with statusFlags, run the Mysterious
 *      Puppeteer gain and the on-summon effects: Dragon Seeker, Senju of the Thousand Hands, Sonic Bird and
 *      keys 1240, 1246 and 1332 queue a monster-kind RESPONSE_SUMMONED trigger; Total Defense Shogun changes
 *      position; Boar Soldier and key 1413 are destroyed. Then the step advances.
 */
int ExecuteSummonAction(void)
{
    switch (gSummonAction.step) {
    case 0: {
        u16 msg;
        if (gSummonAction.hasTribute1)
            TributeMonster(gSummonAction.tribute1Player, gSummonAction.tribute1Zone);
        if (gSummonAction.hasTribute2)
            TributeMonster(gSummonAction.tribute2Player, gSummonAction.tribute2Zone);
        msg = gSummonAction.player ? DUEL_CMD_PLACE_MONSTER_FROM_HAND | DUEL_CMD_PLAYER
                                   : DUEL_CMD_PLACE_MONSTER_FROM_HAND;
        {
            int id = gSummonAction.cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)&gSummonAction >> 6;    /* sourceIndex, bits 6-13 */
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower)); /* FAKEMATCH: second use of the shared mask (see above) */
                lower &= gSummonAction.zone;
                mask = lower;
            }
            packed |= mask;
            packed |= (gSummonAction.isFaceUp | gSummonAction.isDefense << 1) << 8;
            DuelCmd_Push(msg, id, packed, 0);
        }
        gSummonAction.step++;
        return 0;
    }
    case 1:
        DuelCursor_Select(gSummonAction.player, DUEL_AREA_MONSTER, gSummonAction.zone);
        if (!gSummonAction.isFaceUp) {
            gSummonAction.step = 10;
            return 0;
        }
        DuelCmd_Push(DUEL_CMD_SHOW_CARD_ASSEMBLE, gSummonAction.cardId, 1, 0);
        gSummonAction.step++;
        return 0;
    case 2: {
        /* FAKEMATCH: keep the player extraction in its original two scratches. */
        register u32 playerBits asm("r1") = (u32)*(u8 *)&gSummonAction << 31;
        register u32 player asm("r3") = playerBits >> 31;
        u32 zone = gSummonAction.zone;
        u16 msg;
        u32 a, t;
        if ((*(u32 *)(player * sizeof(struct DuelPlayer) + zone * sizeof(struct DuelZone) + (u32)gDuelZones) << 20) == 0)
            return 1;
        msg = DUEL_CMD_SET_ZONE_STATUS_FLAGS;
        if (player)
            msg = DUEL_CMD_SET_ZONE_STATUS_FLAGS | DUEL_CMD_PLAYER;
        DuelCmd_Push(msg, zone, gSummonAction.statusFlags, 0);
        TriggerMysteriousPuppeteer(gSummonAction.player);
        switch (*((gSummonAction.cardId & CARD_ID_MASK) + gCardIdToNumber)) {
        case CARD_DRAGON_SEEKER:
        case CARD_SENJU_OF_THE_THOUSAND_HANDS:
        case CARD_SONIC_BIRD:
        case CARD_1240:
        case CARD_1246:
        case CARD_1332:
            a = gSummonAction.player << 31;
            t = (gSummonAction.zone << 16) | SUMMONED_TRIGGER_BITS;
            Chain_AddPending(a | t | gSummonAction.cardId, gSummonAction.player | (gSummonAction.zone << 8));
            break;
        case CARD_TOTAL_DEFENSE_SHOGUN:
            ChangeBattlePosition(gSummonAction.player, gSummonAction.zone, 0, 0);
            break;
        case CARD_BOAR_SOLDIER:
        case CARD_1413:
            DestroyFieldCard(gSummonAction.player, gSummonAction.zone, 1);
            break;
        }
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}

/*
 * Card number of a card ID through gCardIdToNumber with the table address held in r3, as ExecuteSummonActionAskPosition
 * does (ExecuteSummonAction reads the table symbol directly).
 */
static inline u16 ActionBNumber(u16 id)
{
    u32 off = (id & CARD_ID_MASK) * 2;
    /* FAKEMATCH: this initialized table address uses the original r3 scratch. */
    register const u16 *base asm("r3") = gCardIdToNumber;
    off += (u32)base;
    return *(const u16 *)off;
}

/*
 * Step machine of the summon record of kind SUMMON_ACTION_NORMAL_CHOOSE_POSITION (a Normal Summon granted by an
 * effect, Attack or Defense chosen now); returns 1 when done.
 *   0: the CPU decides with AiShouldSetMonster into gTextBox.result; the human gets 'Select display position of
 *      card.' with the SummonPositionMenu_* callbacks;
 *   1: isDefense = result and isFaceUp = !isDefense, except that the card is face up while key 1151 (Light of
 *      Intervention) is active on either field;
 *   2: Tribute and push DUEL_CMD_PLACE_MONSTER_FROM_HAND as ExecuteSummonAction's step 0 does (the Tributes
 *      belong to the acting player);
 *   3: move the cursor, push DUEL_CMD_SET_ZONE_STATUS_FLAGS, then a Set ends (step 10) or a face-up summon
 *      pushes DUEL_CMD_SHOW_CARD_ASSEMBLE;
 *   4: the Mysterious Puppeteer gain and the on-summon effects of ExecuteSummonAction's step 2.
 */
int ExecuteSummonActionAskPosition(void)
{
    int step = gSummonAction.step;
    struct SummonAction *action = &gSummonAction;

    switch (step) {
    case 0:
        if (action->player) {
            gTextBox.result = AiShouldSetMonster(action->cardId, 0);
        } else {
            TextBoxOpen(BOX_POSITION_POS, BOX_POSITION_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectDisplayPosition);
            TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, SummonPositionMenu_Draw, SummonPositionMenu_HandleInput);
        }
        action->step++;
        return 0;
    case 1: {
        u16 lightOfIntervention;
        gSummonAction.isDefense = gTextBox.result;
        if (gSummonAction.isDefense)
            gSummonAction.isFaceUp = 0;
        else
            gSummonAction.isFaceUp = 1;
        lightOfIntervention = CARD_LIGHT_OF_INTERVENTION;
        if (CountActiveCardsOnField(0, lightOfIntervention) != 0 || CountActiveCardsOnField(1, lightOfIntervention) != 0)
            gSummonAction.isFaceUp = 1;
        goto next_step;     /* FAKEMATCH: step 1 shares the step increment at the end of step 4 (the ROM's block order) */
    }
    case 2: {
        u16 msg;
        if (action->hasTribute1)
            TributeMonster(action->player, action->tribute1Zone);
        if (action->hasTribute2)
            TributeMonster(action->player, action->tribute2Zone);
        msg = action->player ? DUEL_CMD_PLACE_MONSTER_FROM_HAND | DUEL_CMD_PLAYER : DUEL_CMD_PLACE_MONSTER_FROM_HAND;
        {
            int id = action->cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)action >> 6;    /* sourceIndex, bits 6-13 */
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower)); /* FAKEMATCH: second use of the shared mask (see above) */
                lower &= action->zone;
                mask = lower;
            }
            packed |= mask;
            packed |= (action->isFaceUp | action->isDefense << 1) << 8;
            DuelCmd_Push(msg, id, packed, 0);
        }
        action->step++;
        return 0;
    }
    case 3: {
        /* FAKEMATCH: preserve an initialized pointer copy before the calls. */
        register struct SummonAction *copy asm("r5") = action;
        struct SummonAction *record;
        u16 msg;
        asm("" : : "r"(copy));
        record = copy;
        DuelCursor_Select(record->player, DUEL_AREA_MONSTER, record->zone);
        msg = record->player ? DUEL_CMD_SET_ZONE_STATUS_FLAGS | DUEL_CMD_PLAYER : DUEL_CMD_SET_ZONE_STATUS_FLAGS;
        DuelCmd_Push(msg, record->zone, record->statusFlags, 0);
        if (!record->isFaceUp) {
            record->step = 10;
            return 0;
        }
        DuelCmd_Push(DUEL_CMD_SHOW_CARD_ASSEMBLE, action->cardId, 1, 0);
        action->step++;
        return 0;
    }
    case 4: {
        u32 a, t;
        TriggerMysteriousPuppeteer(gSummonAction.player);
        switch (ActionBNumber(gSummonAction.cardId)) {
        case CARD_DRAGON_SEEKER:
        case CARD_SENJU_OF_THE_THOUSAND_HANDS:
        case CARD_SONIC_BIRD:
        case CARD_1240:
        case CARD_1246:
        case CARD_1332:
            a = gSummonAction.player << 31;
            t = (gSummonAction.zone << 16) | SUMMONED_TRIGGER_BITS;
            Chain_AddPending(a | t | gSummonAction.cardId, gSummonAction.player | (gSummonAction.zone << 8));
            break;
        case CARD_TOTAL_DEFENSE_SHOGUN:
            ChangeBattlePosition(gSummonAction.player, gSummonAction.zone, 0, 0);
            break;
        case CARD_BOAR_SOLDIER:
        case CARD_1413:
            DestroyFieldCard(gSummonAction.player, gSummonAction.zone, 1);
            break;
        }
    next_step:
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}
