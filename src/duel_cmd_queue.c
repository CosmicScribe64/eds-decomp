/*
 * The duel command queue, the card command menu executor, the CPU's turn and the full-screen duel scenes
 * (wiki/functions/duel-cmd-queue-c.md).
 *
 *  - DuelCmd_Push appends a command to gDuelCmd.queue; DuelCmdQueue_Run (duel_setup.c) pops it and calls
 *    DuelCmd_Dispatch every frame until the handler clears gDuelCmd.running. The switch in DuelCmd_Dispatch
 *    is the map from enum DuelCmdId to the DuelCmd_* handlers.
 *  - CardMenu_Execute carries out the command chosen in the card command menu (enum CardMenuCommand).
 *  - AiRunTurn plays the CPU's turn with the turn-phase handlers of gAiTurnPhases.
 *  - DuelScene_* run the full-screen scenes of gDuelScene (coin toss, dice, the Exodia and Destiny Board wins).
 *  - PlayDuelBGM picks the duel music. The two HBlank wave handlers and DuelScene_FadeInDuelScreen are unused.
 */
#include "global.h"
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, CARD_STATS_TYPE_* */
#include "constants/duel.h"         /* enum CardMenuCommand, DuelArea, DuelPhase, ChainEntryKind */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "card_data.h"              /* gCardStats, CARD_ID_MASK, CARD_STATS_TYPE */
#include "calendar.h"               /* CAL_TOURNAMENT_* */

/* ---- BEGIN pre-H0 subset of gba.h, duel.h, main.h and sound.h ---- */
/*
 * include/gba.h, duel.h, main.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md); chain.h, duel_cmd.h, duel_screen.h, battle.h and summon.h include duel.h.
 * Until then this block repeats the part of the new headers that the unit uses, with the same tags, field
 * names, types and bitfield containers (structs are cut after the last field used here and padded to their
 * size). It defines GUARD_DUEL_H so that the headers below skip the legacy file. After H0, replace the block
 * (BEGIN to END) with the #include lines of these headers, in this order:
 *     gba.h duel.h main.h sound.h
 * (checked: that gives the same assembly with the new headers). The unit has no include line that the H0 sed
 * rewrites.
 */
#define GUARD_DUEL_H

#define REG_BASE        0x04000000                      /* gba.h */
#define REG16(off)      (*(vu16 *)(REG_BASE + (off)))
#define REG_DISPCNT     REG16(0x000)
#define REG_VCOUNT      REG16(0x006)
#define REG_BG0HOFS     REG16(0x010)
#define REG_BG1HOFS     REG16(0x014)
#define REG_BG2HOFS     REG16(0x018)
#define REG_BG3HOFS     REG16(0x01C)

struct DuelCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;
    u32 specialSummoned:1;
    u32 planted:1;
    u32 graverobbed:1;              /* bit 18 */
    u32 unk19:13;
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
    u8 unk4[0x94 - 4];
};

struct DuelPlayer {
    u8 unk0[0x28];
    struct DuelZone zones[11];      /* +0x028 */
    u8 unk684[0xD64 - 0x684];
};

struct CardMenu {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u32 state:8;
    u32 step:8;
    u8 summonSeq:4;
    u32 tributeSources:4;
    u16 timer:7;
    u16 player:1;
    u32 area:7;
    u32 index:8;
    u32 placeZone:8;
    u32 unk0A_1:15;
};

struct DuelState {
    u16 serial;
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004 */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;
    u8 phase:3;
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13_0:1;
    u8 unk1B13_1:7;
    u16 unk1B14_0:1;                /* +0x1B14 */
    u16 interruptActive:1;
    u16 unk1B14_2:7;
    u32 battleStage:8;
    u16 battleStep:8;               /* +0x1B16 bits 1-8 */
    u16 battleArg0:8;
    u16 battleArg1:8;
    u16 unk1B19_1:7;
    u8 unk1B1A[2];
    struct DuelCard battleCard;
    u8 phaseStep;                   /* +0x1B20 */
    u8 phaseCounter;                /* +0x1B21 */
    u8 unk1B22[0x1B28 - 0x1B22];
    u16 cardMenuCard;               /* +0x1B28 */
    u16 summonTributes;
    struct CardMenu cardMenu;       /* +0x1B2C */
};

extern struct DuelState gDuel;

u16 TakeDeckCardByNumber(int player, u16 cardNo, struct DuelCard *out);

struct Main {                                   /* main.h */
    u8 unk0[0x414];
    void (*vblankCallback)(void);   /* +0x0414 */
    u8 unk418[0x485E - 0x418];
    u16 frameCounter;               /* +0x485E */
    u8 unk4860[0x4870 - 0x4860];
    u8 firstPlayer:1;               /* +0x4870 */
    u8 opponent:5;
    u8 result:2;
    u8 unk4871[0x487C - 0x4871];
    u32 events;                     /* +0x487C */
};
extern struct Main gMain;
void ResetBgScroll(void);

void PlayBGM(u32 songId);                       /* sound.h */
void StopBGM(void);
void FadeOutBGM(void);
/* ---- END pre-H0 subset ---- */

#include "duel_cmd.h"               /* gDuelCmd, DuelCmd_Push, the DuelCmd_* handlers */
#include "duel_flow.h"              /* gDuelCtrl, struct OpponentBGM, PlayDuelBGM */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_Init, DuelScreen_FadeOutStep */
#include "duel_scenes.h"            /* gDuelScene, enum DuelSceneId, the DuelScene_* functions defined here */
#include "battle.h"                 /* gBattle */
#include "chain.h"                  /* Chain_AddPending */
#include "card_menu.h"              /* CardMenu_* */
#include "duel_actions.h"           /* DiscardHandCard, ShowCardEffect, LoseLifePoints, TributeMonster */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "ai.h"                     /* gAiState, AiRunTurn */
#include "palette.h"                /* FadeFromBlack */

/* ---- ROM data used only here ---- */

extern u16 (*const gAiTurnPhases[])(void);          /* 0x08198EDC: the CPU's turn steps, NULL-terminated */
extern u16 (*const gDuelSceneHandlers[])(void);     /* 0x08198EF8: scene handler per enum DuelSceneId */
extern u16 (*const gDuelSceneRunnerSteps[])(void);  /* 0x08198F14: fade out the duel screen, run the handler */
extern const struct OpponentBGM gOpponentDuelBGM[24];   /* 0x08198F20 */
extern const u8 gStrDoYouSurrender[];               /* "Do you surrender ?" */
extern const u16 gUnk_0862467A;                     /* = gCardNumberToId[CARD_GRAVEROBBER]: Graverobber's ID */

/* ---- Local views kept for matching ---- */

/* The card tables in both of the forms the ROM uses here: gCardStats through the symbol (pointer arithmetic),
 * and gCardStats / gCardIdToNumber through their constant addresses. */
#define CARD_TYPE(id)       CARD_STATS_TYPE(*(gCardStats + ((id) & CARD_ID_MASK)))
#define CARD_STATS_C(id)    (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER_C(id)   (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* TakeDeckCardByNumber returns u16; CardMenu_Execute tests r0 without narrowing it. */
#define TakeDeckCardByNumberInt ((int (*)(int, u16, void *))TakeDeckCardByNumber)

/*
 * gDuel as CardMenu_Execute reads it (through a cast of &gDuel, so the addresses are formed like those of the
 * other gDuel fields): the phase in a u32 container (duel.h: u8), which drops two extension instructions
 * before the `phase > PHASE_STANDBY` test so that the reloaded base wins r5, and the card menu with its index
 * in a u16 container (duel.h: u32), whose HImode value makes regmove tie the AND of the atkSlot store to the
 * constant (`movs r1, #7; ands r1, r0`). FAKEMATCH: both containers.
 */
struct CardMenuIndexU16 {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u32 state:8;
    u32 step:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;
    u32 area:7;
    u16 index:8;                    /* bits 65-72 */
    u32 unk73:23;
};
struct DuelStateMenuView {
    u8 unk0[0x1B12];
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;
    u32 phase:3;                    /* bits 2-4: enum DuelPhase */
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13[0x1B2C - 0x1B13];
    struct CardMenuIndexU16 cardMenu;   /* +0x1B2C */
};
#define gDuelMenuView (*(struct DuelStateMenuView *)&gDuel)

/* The head of a zone and the flag bytes of a player, reached through a cast of the element's address to a
 * struct type of its own, so the field offset (+6, +8/+9) stays in the ldrb/strb as in the ROM instead of
 * folding into the base address. Same bits as struct DuelZone and struct DuelPlayer. */
struct DuelZoneHead {
    struct DuelCard card;           /* +0x00 */
    u16 serial;
    u8 isDefense:1;                 /* +0x06 */
    u8 isFaceUp:1;
    u8 unk6_2:6;
};
struct DuelPlayerFlags {
    u8 unk0[8];
    u8 unk8_0:4;                    /* +0x08 */
    u8 normalSummonUsed:1;          /* +0x08 bit 4 */
    u8 unk8_5:3;
    u8 unk9_0:5;                    /* +0x09 */
    u8 magicTrapActivatedThisTurn:1; /* +0x09 bit 5 */
    u8 unk9_6:2;
};
#define ZONE_HEAD(player, zone)         ((struct DuelZoneHead *)&gDuel.players[player].zones[zone])
/* The card word of a zone through a struct DuelCard pointer: that loads the whole word (ldr), as the ROM does;
 * through the zone head the bit test is narrowed to a byte load. */
#define ZONE_CARD(player, zone)         ((struct DuelCard *)&gDuel.players[player].zones[zone])
#define PLAYER_FLAGS(players, player)   ((struct DuelPlayerFlags *)&(players)[player])

/* The card menu of gDuel (the command, its player, area and index were saved when it was confirmed). */
#define MENU (gDuel.cardMenu)

/*
 * Carry out the command confirmed in the card command menu (gDuel.cardMenu.command, enum CardMenuCommand) for
 * the card gDuel.cardMenuCard at (cardMenu.player, area, index). Multi-step commands (the Red-Eyes B. Dragon /
 * Zoa metal summons, the Surrender prompt) return early and keep cardMenu.confirmed set, so they run again next
 * frame; every other path clears confirmed and cardMenu.step at the end.
 *
 * FAKEMATCH notes: `ev = (...)` inside the Chain_AddPending packings stops fold from floating the kind constant
 * out of the OR chain; the per-site `players` locals keep the constant-1 pseudo from inheriting an r4
 * preference.
 */
void CardMenu_Execute(void)
{
    struct DuelCard taken;
    s16 area;
    u32 ev;
    int zone;
    int player;

    switch (MENU.command) {
    case CARDMENU_CMD_DEF_POS:
    case CARDMENU_CMD_ATK_POS:
        if (MENU.area != DUEL_AREA_MONSTER)
            break;
        CardMenu_ChangePosition(MENU.command);
        return;
    case CARDMENU_CMD_FLIP:
        if (MENU.area != DUEL_AREA_MONSTER)
            break;
        CardMenu_FlipSummon();
        return;
    case CARDMENU_CMD_SET:
        if (MENU.area != DUEL_AREA_HAND)
            break;
        if (CARD_STATS_TYPE(CARD_STATS_C(gDuel.cardMenuCard)) <= CARD_TYPE_REPTILE)
            CardMenu_SummonMonster(0, 0);
        else
            CardMenu_PlaySpellTrapFromHand(0, 0, 0);
        return;
    case CARDMENU_CMD_SUMMON:
        CardMenu_SummonMonster(1, 0);
        return;
    case CARDMENU_CMD_SP_SUMMON:
        CardMenu_SummonMonster(1, 1);
        return;
    case CARDMENU_CMD_SP_SUMMON_SET:
        CardMenu_SummonMonster(0, 1);
        return;
    case CARDMENU_CMD_FUSION:
        CardMenu_FusionSummon();
        return;
    case CARDMENU_CMD_ACTIVATE:
        area = MENU.area;
        switch (area) {
        case DUEL_AREA_HAND:
            if (CARD_TYPE(gDuel.cardMenuCard) > CARD_TYPE_REPTILE) {
                CardMenu_PlaySpellTrapFromHand(1, 0, 0);
                return;
            }
            switch (CARD_NUMBER_C(gDuel.cardMenuCard)) {
            case CARD_COCOON_OF_EVOLUTION:
                /* Played like a Magic card; it uses up the turn's Normal Summon. */
                {
                    struct DuelPlayer *players = gDuel.players;
                    PLAYER_FLAGS(players, MENU.player & 1)->normalSummonUsed = 1;
                }
                CardMenu_PlaySpellTrapFromHand(1, 0, 0);
                return;
            case CARD_THUNDER_DRAGON:
                /* Discarded from the hand for its effect. */
                DiscardHandCard(MENU.player, MENU.index, 0, 1);
                Chain_AddPending(((MENU.player & 1) << 31)
                                 | (ev = CHAIN_KIND_OFF_FIELD << 21 | gDuel.cardMenuCard), 0);
                break;
            }
            break;
        case DUEL_AREA_FIELD:
            if (!ZONE_HEAD(MENU.player & 1, ZONE_FIELD)->isFaceUp)
                DuelCmd_Push(MENU.player ? DUEL_CMD_PLAYER | DUEL_CMD_FLIP_CARD : DUEL_CMD_FLIP_CARD,
                             ZONE_FIELD, 0, 0);
            Chain_AddPending(((MENU.player & 1) << 31)
                             | (ev = (((MENU.index + MENU.area) & 0x1F) << 16) | CHAIN_KIND_SPELL_TRAP << 21)
                             | gDuel.cardMenuCard, 0);
            if (gDuelMenuView.phase > (u32)PHASE_STANDBY) {
                struct DuelPlayer *players = gDuel.players;
                PLAYER_FLAGS(players, MENU.player & 1)->magicTrapActivatedThisTurn = 1;
            }
            break;
        case DUEL_AREA_SPELL_TRAP:
            player = MENU.player & 1;
            zone = MENU.index;
            zone += ZONE_SPELL_0;
            if (!ZONE_HEAD(player, zone)->isFaceUp)
                DuelCmd_Push(MENU.player ? DUEL_CMD_PLAYER | DUEL_CMD_FLIP_CARD : DUEL_CMD_FLIP_CARD,
                             MENU.index + MENU.area, 0, 0);
            /* A card taken with Graverobber costs 2000 LP when activated. This reads the card word of
             * zones[index] (the monster zone in the same column), not of zones[5 + index]: the ROM does the
             * same, possibly an original bug. */
            if (ZONE_CARD(MENU.player & 1, MENU.index)->graverobbed) {
                ShowCardEffect(MENU.player, gUnk_0862467A);
                LoseLifePoints(MENU.player, 2000);
            }
            Chain_AddPending(((MENU.player & 1) << 31)
                             | (ev = (((MENU.index + MENU.area) & 0x1F) << 16) | CHAIN_KIND_SPELL_TRAP << 21)
                             | gDuel.cardMenuCard, 0);
            if (gDuelMenuView.phase > (u32)PHASE_STANDBY) {
                struct DuelPlayer *players = gDuel.players;
                PLAYER_FLAGS(players, MENU.player & 1)->magicTrapActivatedThisTurn = 1;
            }
            break;
        case DUEL_AREA_MONSTER:
            switch (CARD_NUMBER_C(gDuel.cardMenuCard)) {
            case CARD_RED_EYES_B_DRAGON:
            case CARD_ZOA:
                /* Step 0: tribute the monster; step 1: Special Summon its metal form from the deck. */
                switch (MENU.step) {
                case 0:
                    if (TributeMonster(MENU.player, MENU.index)) {
                        MENU.step++;
                        return;
                    }
                    MENU.confirmed = 0;
                    MENU.step = 0;
                    return;
                case 1:
                    if (TakeDeckCardByNumberInt(MENU.player,
                                                CARD_NUMBER_C(gDuel.cardMenuCard) == CARD_RED_EYES_B_DRAGON
                                                    ? CARD_RED_EYES_BLACK_METAL_DRAGON : CARD_METALZOA,
                                                &taken)) {
                        QueueSpecialSummonChoosePosition(MENU.player, &taken, 1, 1);
                        MENU.step++;
                        return;
                    }
                }
                MENU.confirmed = 0;
                MENU.step = 0;
                return;
            case CARD_BLAST_JUGGLER:
            case CARD_PATROL_ROBO:
            case CARD_JIGEN_BAKUDAN:
                /* Standby Phase monsters: the effect is queued with the event RESPONSE_OWN_STANDBY. */
                Chain_AddPending(((MENU.player & 1) << 31)
                                 | (ev = (((MENU.index + MENU.area) & 0x1F) << 16)
                                         | RESPONSE_OWN_STANDBY << 25 | CHAIN_KIND_MONSTER << 21)
                                 | gDuel.cardMenuCard, 0);
                break;
            default:
                Chain_AddPending(((MENU.player & 1) << 31)
                                 | (ev = (((MENU.index + MENU.area) & 0x1F) << 16) | CHAIN_KIND_MONSTER << 21)
                                 | gDuel.cardMenuCard, 0);
                break;
            }
            break;
        }
        break;
    case CARDMENU_CMD_ATTACK:
        gBattle.atkSlot = gDuelMenuView.cardMenu.index;
        gDuel.battleStep = 2;
        break;
    case CARDMENU_CMD_DRAW:
        gDuel.phaseStep++;
        MENU.confirmed = 0;
        MENU.step = 0;
        return;
    case CARDMENU_CMD_SURRENDER:
        switch (MENU.step) {
        case 0:
            TextBoxOpen(0x206, 0x412, TEXTBOX_FLAGS_DEFAULT, gStrDoYouSurrender);  /* at cell (6, 2), 18 x 4 */
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
            MENU.step++;
            break;
        case 1:
            if (gTextBox.result)
                DuelCmd_Push(DUEL_CMD_SURRENDER, 1, 0, 0);
            MENU.confirmed = 0;
            MENU.step = 0;
            break;
        }
        return;
    }
    MENU.confirmed = 0;
    MENU.step = 0;
}
#undef MENU

/* One frame of the CPU's turn: run gAiTurnPhases[gAiState.turnPhase] (the turn start, draw, standby and end
 * handlers of the human's turn, with AiRunStep as the main phase); when it returns nonzero, go to the next
 * one with fresh step bytes. Returns 1 at the end of the table (the turn is over), else 0. */
u16 AiRunTurn(void)
{
    u16 (*phase)(void) = gAiTurnPhases[gAiState.turnPhase];

    if (phase != NULL) {
        if (phase()) {
            gDuel.phaseStep = 0;
            gDuel.phaseCounter = 0;
            gAiState.step = 0;
            gAiState.turnPhase++;
        }
        return 0;
    }
    return 1;
}

/* Start full-screen scene `id` (enum DuelSceneId) with handler gDuelSceneHandlers[id]. The two win scenes
 * stop the duel BGM. */
void DuelScene_Start(u16 id, u32 flag)
{
    gDuelScene.runnerStep = 0;
    gDuelScene.sceneStep = 0;
    gDuelScene.unkC = 0;
    gDuelScene.unkD = 0;
    gDuelScene.flag = flag;
    gDuelScene.id = id;
    gDuelScene.handler = gDuelSceneHandlers[id];
    switch (gDuelScene.id) {
    case DUEL_SCENE_EXODIA_WIN:
    case DUEL_SCENE_DESTINY_BOARD_WIN:
        StopBGM();
        gDuel.bgmOn = 0;
        break;
    }
    gDuelScreen.uiGfxLoaded = 0;
}

/* Runner step 0: on the first call clear the VBlank callback, the BG scroll and the duel screen flags; then
 * fade the duel screen out. Returns 1 when it is gone. */
u16 DuelScene_FadeOutDuelScreen(void)
{
    if (gDuelScene.sceneStep == 0) {
        gMain.vblankCallback = NULL;
        ResetBgScroll();
        gDuelScreen.showCursor = 0;
        gDuelScreen.uiGfxLoaded = 0;
        gDuelScreen.active = 0;
        gDuelScene.sceneStep++;
    }
    return DuelScreen_FadeOutStep();
}

/* Runner step 1: run the scene handler; returns its result (1 when the scene is over), 0 without one. */
u16 DuelScene_RunHandler(void)
{
    u16 (*handler)(void) = gDuelScene.handler;

    if (handler != NULL)
        return handler();
    return 0;
}

/* Unreferenced runner step: blank the display, rebuild the duel screen and fade it in. Returns the
 * FadeFromBlack result (1 when done). The game reopens the field with DUEL_CMD_OPEN_DUEL_SCREEN instead. */
u16 DuelScene_FadeInDuelScreen(void)
{
    struct DuelScene *scene = &gDuelScene;

    switch (scene->sceneStep) {
    case 0:
        REG_DISPCNT = 0;
        break;
    case 1:
        DuelScreen_Init();
        break;
    default:
        return FadeFromBlack(4);
    }
    scene->sceneStep++;
    return 0;
}

/* Run the current scene: step through gDuelSceneRunnerSteps (fade out the duel screen, run the handler).
 * Returns 1 when the scene has finished, else 0. */
u16 DuelScene_Run(void)
{
    u16 (*step)(void) = gDuelSceneRunnerSteps[gDuelScene.runnerStep];

    if (step != NULL) {
        if (step()) {
            gDuelScene.runnerStep++;
            gDuelScene.sceneStep = 0;
            gDuelScene.unkC = 0;
            gDuelScene.unkD = 0;
        }
        return 0;
    }
    gDuelScene.runnerStep = 0;
    return 1;
}

/* Unused HBlank handler: a 16-line horizontal wave on BG0, BG1 and BG3 from gDuelCmd.hofsTable (which
 * nothing sets). */
void HBlank_WaveBg013(void)
{
    u16 hofs = gDuelCmd.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];

    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* Unused HBlank handler: the same wave on all four BGs. */
void HBlank_WaveAllBgs(void)
{
    u16 hofs = gDuelCmd.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];

    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG2HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* Start the duel music: the opponent's song from gOpponentDuelBGM, song 0x13 in a tournament duel, song 5 in
 * a link duel. Fades the BGM out instead when the duel BGM is off (gDuel.bgmOn) or no song was found. Called
 * after every duel command, so it is cheap when the song already plays. */
void PlayDuelBGM(void)
{
    u16 bgm = 0xFFFF;
    u32 i;

    i = 0;
    do {
        if (gOpponentDuelBGM[i].opponent == gMain.opponent) {
            bgm = gOpponentDuelBGM[i].bgm;
            break;
        }
    } while (++i <= ARRAY_COUNT(gOpponentDuelBGM) - 1);
    if (gMain.events & (CAL_TOURNAMENT_ROUND1 | CAL_TOURNAMENT_ROUND2 | CAL_TOURNAMENT_SEMIFINAL
                        | CAL_TOURNAMENT_FINAL))
        bgm = 0x13;
    if (gDuelCtrl.isLinkDuel)
        bgm = 5;
    if (!gDuel.bgmOn)
        bgm = 0xFFFF;
    if (bgm == 0xFFFF)
        FadeOutBGM();
    else
        PlayBGM(bgm);
}

/* Append {cmd, arg2, arg4, arg6} to the duel command queue (dropped when 256 commands are queued). cmd is an
 * enum DuelCmdId, with DUEL_CMD_PLAYER set when player 1 acts. arg4 and arg6 come in as words and are
 * truncated to u16 here (the ROM decodes both from r2/r3 with explicit shifts). */
void DuelCmd_Push(u16 cmd, u16 arg2, int arg4Word, int arg6Word)
{
    u16 arg4 = arg4Word;
    u16 arg6 = arg6Word;

    if (gDuelCmd.queueCount < ARRAY_COUNT(gDuelCmd.queue)) {
        gDuelCmd.queue[gDuelCmd.queueCount].cmd = cmd;
        gDuelCmd.queue[gDuelCmd.queueCount].arg2 = arg2;
        gDuelCmd.queue[gDuelCmd.queueCount].arg4 = arg4;
        gDuelCmd.queue[gDuelCmd.queueCount].arg6 = arg6;
        gDuelCmd.queueCount++;
    }
}

/* Run one frame of the current duel command: call the handler of its id (bits 0-11 of gDuelCmd.cmd). One
 * handler can serve several ids (the negation flags, the two life-point commands, the phase banners). Unknown
 * ids just finish. */
void DuelCmd_Dispatch(void)
{
    gDuelScreen.showCursor = 0;
    switch (gDuelCmd.cmd & DUEL_CMD_ID_MASK) {
    case DUEL_CMD_TURN_START:
        DuelCmd_TurnStart();
        break;
    case DUEL_CMD_TURN_END:
        DuelCmd_TurnEnd();
        break;
    case DUEL_CMD_SHOW_END_TURN_HAND:
        DuelCmd_ShowEndTurnHand();
        break;
    case DUEL_CMD_SHOW_DUEL_RESULT:
        DuelCmd_ShowDuelResult();
        break;
    case DUEL_CMD_EXODIA_WIN_SCENE:
        DuelCmd_ExodiaWinScene();
        break;
    case DUEL_CMD_DESTINY_BOARD_WIN_SCENE:
        DuelCmd_DestinyBoardWinScene();
        break;
    case DUEL_CMD_CHAIN_BANNER:
        DuelCmd_ShowChainBanner();
        break;
    case DUEL_CMD_POINT_AT_CARD:
        DuelCmd_PointAtCard();
        break;
    case DUEL_CMD_MOVE_CURSOR:
        DuelCmd_MoveCursor();
        break;
    case DUEL_CMD_RESET_DUEL_STATE:
        DuelCmd_ResetDuelState();
        break;
    case DUEL_CMD_SET_FIELD_BACKGROUND:
        DuelCmd_SetFieldBackground();
        break;
    case DUEL_CMD_OPEN_DUEL_SCREEN:
        DuelCmd_OpenDuelScreen();
        break;
    case DUEL_CMD_CLOSE_DUEL_SCREEN:
        DuelCmd_CloseDuelScreen();
        break;
    case DUEL_CMD_START_DUEL_BANNER:
        DuelCmd_StartDuelBanner();
        break;
    case DUEL_CMD_NEGATE_EQUIP_THIS_TURN:
    case DUEL_CMD_NEGATE_FIELD_THIS_TURN:
    case DUEL_CMD_NEGATE_CONT_TRAP_THIS_TURN:
    case DUEL_CMD_NEGATE_CONT_MAGIC_THIS_TURN:
    case DUEL_CMD_NEGATE_TRAPS:
    case DUEL_CMD_NEGATE_MAGIC:
    case DUEL_CMD_NEGATE_EQUIP:
        DuelCmd_SetNegationFlag();
        break;
    case DUEL_CMD_SET_STAT_CHANGES_REVERSED:
        DuelCmd_SetStatChangesReversed();
        break;
    case DUEL_CMD_SET_ATK_DEF_SWAPPED:
        DuelCmd_SetAtkDefSwapped();
        break;
    case DUEL_CMD_START_BATTLE_SCENE:
        DuelCmd_StartBattleScene();
        break;
    case DUEL_CMD_PLAY_BATTLE_SCENE:
        DuelCmd_PlayBattleScene();
        break;
    case DUEL_CMD_PREPARE_BATTLE_PHASE:
        DuelCmd_PrepareBattlePhase();
        break;
    case DUEL_CMD_ATTACK:
        DuelCmd_Attack();
        break;
    case DUEL_CMD_DIRECT_ATTACK:
        DuelCmd_DirectAttack();
        break;
    case DUEL_CMD_MARK_ATTACKED:
        DuelCmd_MarkAttacked();
        break;
    case DUEL_CMD_SET_BATTLE_PROTECTION:
        DuelCmd_SetBattleProtection();
        break;
    case DUEL_CMD_END_BATTLE_PHASE:
        DuelCmd_EndBattlePhase();
        break;
    case DUEL_CMD_SET_ATTACK_TARGET:
        DuelCmd_SetAttackTarget();
        break;
    case DUEL_CMD_SET_ATTACKER:
        DuelCmd_SetAttacker();
        break;
    case DUEL_CMD_ZERO_ATTACKER_ATK:
        DuelCmd_ZeroAttackerAtk();
        break;
    case DUEL_CMD_NEGATE_ATTACK:
        DuelCmd_NegateAttack();
        break;
    case DUEL_CMD_SURRENDER:
        DuelCmd_Surrender();
        break;
    case DUEL_CMD_SHOW_JUST_A_MOMENT:
        DuelCmd_ShowJustAMomentBanner();
        break;
    case DUEL_CMD_GAIN_LP:
        DuelCmd_ChangeLifePoints(1);
        break;
    case DUEL_CMD_LOSE_LP:
        DuelCmd_ChangeLifePoints(0);
        break;
    case DUEL_CMD_SKIP_NEXT_DRAW_PHASE:
        DuelCmd_SkipNextDrawPhase();
        break;
    case DUEL_CMD_SKIP_NEXT_STANDBY_PHASE:
        DuelCmd_SkipNextStandbyPhase();
        break;
    case DUEL_CMD_SKIP_NEXT_TURN:
        DuelCmd_SkipNextTurn();
        break;
    case DUEL_CMD_SET_EXTRA_BATTLE_PHASE:
        DuelCmd_SetExtraBattlePhase();
        break;
    case DUEL_CMD_SET_POSITION_CHANGE_LOCK:
        DuelCmd_SetPositionChangeLock();
        break;
    case DUEL_CMD_SET_SUMMON_LOCKS:
        DuelCmd_SetSummonLocks();
        break;
    case DUEL_CMD_SET_MAGIC_TRAP_LOCK_TURNS:
        DuelCmd_SetMagicTrapLockTurns();
        break;
    case DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT:
        DuelCmd_AdjustDelayedSummonCount();
        break;
    case DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING:
        sub_08014B5C();
        break;
    case DUEL_CMD_DRAW_PHASE:
        DuelCmd_EnterPhase(0);
        break;
    case DUEL_CMD_STANDBY_PHASE:
        DuelCmd_EnterPhase(1);
        break;
    case DUEL_CMD_MAIN1_PHASE:
        DuelCmd_EnterPhase(2);
        break;
    case DUEL_CMD_BATTLE_PHASE:
        DuelCmd_EnterBattlePhase();
        break;
    case DUEL_CMD_MAIN2_PHASE:
        DuelCmd_EnterPhase(4);
        break;
    case DUEL_CMD_END_PHASE:
        DuelCmd_EnterPhase(5);
        break;
    case DUEL_CMD_DRAW_CARDS:
        DuelCmd_DrawCards();
        break;
    case DUEL_CMD_SEND_TOP_DECK_CARDS_TO_GRAVEYARD:
        DuelCmd_SendTopDeckCardsToGraveyard();
        break;
    case DUEL_CMD_BANISH_TOP_DECK_CARDS:
        DuelCmd_BanishTopDeckCards();
        break;
    case DUEL_CMD_SHUFFLE_DECK:
        DuelCmd_ShuffleDeck();
        break;
    case DUEL_CMD_ADD_DECK_CARD_TO_HAND:
        DuelCmd_AddDeckCardToHand();
        break;
    case DUEL_CMD_REMOVE_CARD_FROM_DECK:
        DuelCmd_RemoveCardFromDeck();
        break;
    case DUEL_CMD_SUMMON_FROM_DECK:
        DuelCmd_SummonFromDeck();
        break;
    case DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD:
        DuelCmd_SendDeckCardToGraveyard();
        break;
    case DUEL_CMD_BANISH_DECK_CARD:
        DuelCmd_BanishDeckCard();
        break;
    case DUEL_CMD_SET_CRUSH_CARD_TURNS:
        DuelCmd_SetCrushCardTurns();
        break;
    case DUEL_CMD_ADD_CARD_TO_DECK_TOP:
        DuelCmd_AddCardToDeckTop();
        break;
    case DUEL_CMD_ADD_CARD_TO_DECK_BOTTOM:
        DuelCmd_AddCardToDeckBottom();
        break;
    case DUEL_CMD_SHOW_CARD_DETAIL:
        DuelCmd_ShowCardDetail();
        break;
    case DUEL_CMD_SHOW_CARD_ASSEMBLE:
        DuelCmd_ShowCardAssemble();
        break;
    case DUEL_CMD_SHOW_CARD_ZOOM_IN:
        DuelCmd_ShowCardZoomIn();
        break;
    case DUEL_CMD_SHOW_CARD_EFFECT:
        DuelCmd_ShowCardEffect();
        break;
    case DUEL_CMD_SHOW_CARD_SCATTER:
        DuelCmd_ShowCardScatter();
        break;
    case DUEL_CMD_SHOW_CARD_UNROLL_DOWN:
        DuelCmd_ShowCardUnrollDown();
        break;
    case DUEL_CMD_SHOW_CARD_UNROLL_SIDEWAYS:
        DuelCmd_ShowCardUnrollSideways();
        break;
    case DUEL_CMD_PLACE_CARD:
        DuelCmd_PlaceCard();
        break;
    case DUEL_CMD_CLEAR_ZONE_CARD:
        DuelCmd_ClearZoneCard();
        break;
    case DUEL_CMD_ADD_CARD_TO_GRAVEYARD:
        DuelCmd_AddCardToGraveyard();
        break;
    case DUEL_CMD_ADD_CARD_TO_BANISHED:
        DuelCmd_AddCardToBanished();
        break;
    case DUEL_CMD_CHANGE_POSITION:
        DuelCmd_ChangePosition();
        break;
    case DUEL_CMD_FLIP_CARD:
        DuelCmd_FlipCard();
        break;
    case DUEL_CMD_SEND_TO_GRAVEYARD:
        DuelCmd_SendToGraveyard();
        break;
    case DUEL_CMD_BANISH:
        DuelCmd_Banish();
        break;
    case DUEL_CMD_BANISH_FLAGGED:
        DuelCmd_BanishFlagged();
        break;
    case DUEL_CMD_RETURN_TO_HAND:
        DuelCmd_ReturnToHand();
        break;
    case DUEL_CMD_RETURN_TO_DECK:
        DuelCmd_ReturnToDeck();
        break;
    case DUEL_CMD_MOVE_TO_ZONE:
        DuelCmd_MoveToZone();
        break;
    case DUEL_CMD_ADD_EQUIP_LINK:
        DuelCmd_AddEquipLink();
        break;
    case DUEL_CMD_SWAP_ZONES:
        DuelCmd_SwapZones();
        break;
    case DUEL_CMD_ADD_ZONE_LINK:
        DuelCmd_AddZoneLink();
        break;
    case DUEL_CMD_REMOVE_ZONE_LINK:
        DuelCmd_RemoveZoneLink();
        break;
    case DUEL_CMD_SET_ZONE_DECLARED_VALUE:
        DuelCmd_SetZoneDeclaredValue();
        break;
    case DUEL_CMD_RESET_ZONE_TURN_COUNTER_AND_SET_DECLARED_VALUE:
        DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue();
        break;
    case DUEL_CMD_SET_ZONE_TURN_COUNTER:
        DuelCmd_SetZoneTurnCounter();
        break;
    case DUEL_CMD_ADD_ZONE_TURN_COUNTER:
        DuelCmd_AddZoneTurnCounter();
        break;
    case DUEL_CMD_SET_DESTROYED_BY_OPPONENT_FLAG:
        DuelCmd_SetDestroyedByOpponentFlag();
        break;
    case DUEL_CMD_CLEAR_ZONE_LINKS:
        DuelCmd_ClearZoneLinks();
        break;
    case DUEL_CMD_MOVE_ZONE_LINKS:
        DuelCmd_MoveZoneLinks();
        break;
    case DUEL_CMD_ADD_PROHIBITION:
        DuelCmd_AddProhibition();
        break;
    case DUEL_CMD_REMOVE_PROHIBITION:
        DuelCmd_RemoveProhibition();
        break;
    case DUEL_CMD_SET_ZONE_STATUS_FLAGS:
        DuelCmd_SetZoneStatusFlags();
        break;
    case DUEL_CMD_CLEAR_ZONE_STATUS_FLAGS:
        DuelCmd_ClearZoneStatusFlags();
        break;
    case DUEL_CMD_SET_EFFECT_UNUSED:
        DuelCmd_SetEffectUnused();
        break;
    case DUEL_CMD_TRIBUTE_MONSTER:
        DuelCmd_TributeMonster();
        break;
    case DUEL_CMD_PLANT_IN_OPPONENT_DECK:
        DuelCmd_PlantInOpponentDeck();
        break;
    case DUEL_CMD_SET_DESTROY_COUNTDOWN:
        DuelCmd_SetDestroyCountdown();
        break;
    case DUEL_CMD_SET_CANNOT_ATTACK:
        DuelCmd_SetCannotAttack();
        break;
    case DUEL_CMD_SET_CANNOT_ATTACK_NEXT_TURN:
        DuelCmd_SetCannotAttackNextTurn();
        break;
    case DUEL_CMD_HALVE_ATTACK:
        DuelCmd_HalveAttack();
        break;
    case DUEL_CMD_NOP_99:
        DuelCmd_Nop99();
        break;
    case DUEL_CMD_NOP_9A:
        DuelCmd_Nop9A();
        break;
    case DUEL_CMD_NOP_9B:
        DuelCmd_Nop9B();
        break;
    case DUEL_CMD_NOP_9C:
        DuelCmd_Nop9C();
        break;
    case DUEL_CMD_NOP_9D:
        DuelCmd_Nop9D();
        break;
    case DUEL_CMD_NOP_9E:
        DuelCmd_Nop9E();
        break;
    case DUEL_CMD_NOP_9F:
        DuelCmd_Nop9F();
        break;
    case DUEL_CMD_CLEAR_ZONE_LINKS_2:
        DuelCmd_ClearZoneLinks2();
        break;
    case DUEL_CMD_SET_POSITION_LOCKED:
        DuelCmd_SetPositionLocked();
        break;
    case DUEL_CMD_SET_RETURN_AFTER_BATTLE:
        DuelCmd_SetReturnAfterBattle();
        break;
    case DUEL_CMD_SUMMON_TOKEN:
        DuelCmd_SummonToken();
        break;
    case DUEL_CMD_SET_ZONE_CARD_WORD:
        DuelCmd_SetZoneCardWord();
        break;
    case DUEL_CMD_FUSION_MATERIAL_TO_GRAVE:
        DuelCmd_SendFusionMaterialToGrave();
        break;
    case DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG:
        sub_08013104();
        break;
    case DUEL_CMD_MOVE_MONSTER_FACE_DOWN:
        DuelCmd_MoveMonsterFaceDown();
        break;
    case DUEL_CMD_SET_MAGICAL_HATS_CARD:
        DuelCmd_SetMagicalHatsCard();
        break;
    case DUEL_CMD_BANISH_UNTIL_END_PHASE:
        DuelCmd_BanishMonsterUntilEndPhase();
        break;
    case DUEL_CMD_RETURN_BANISHED_MONSTER:
        DuelCmd_ReturnBanishedMonster();
        break;
    case DUEL_CMD_NEGATE_ACTIVATION:
        DuelCmd_NegateActivation();
        break;
    case DUEL_CMD_NOP_B2:
        DuelCmd_NopB2();
        break;
    case DUEL_CMD_SET_SPELL_TRAP_DISABLED:
        DuelCmd_SetSpellTrapDisabled();
        break;
    case DUEL_CMD_UPDATE_ZONE_LP_PAID:
        DuelCmd_UpdateZoneLpPaid();
        break;
    case DUEL_CMD_INCREMENT_ZONE_TURN_COUNTER:
        DuelCmd_IncrementZoneTurnCounter();
        break;
    case DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD:
        DuelCmd_SendHandCardToGraveyard();
        break;
    case DUEL_CMD_BANISH_HAND_CARD:
        DuelCmd_BanishHandCard();
        break;
    case DUEL_CMD_RETURN_HAND_CARD_TO_DECK:
        DuelCmd_ReturnHandCardToDeck();
        break;
    case DUEL_CMD_REMOVE_CARD_FROM_HAND:
        DuelCmd_RemoveCardFromHand();
        break;
    case DUEL_CMD_PLACE_MONSTER_FROM_HAND:
        DuelCmd_PlaceMonsterFromHand();
        break;
    case DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND:
        DuelCmd_PlaceSpellTrapFromHand();
        break;
    case DUEL_CMD_EXCHANGE_HAND_CARDS:
        DuelCmd_ExchangeHandCards();
        break;
    case DUEL_CMD_ADD_CARD_TO_HAND:
        DuelCmd_AddCardToHand();
        break;
    case DUEL_CMD_COMPACT_HAND:
        DuelCmd_CompactHand();
        break;
    case DUEL_CMD_SEND_HAND_FUSION_MATERIAL_TO_GRAVEYARD:
        DuelCmd_SendHandFusionMaterialToGraveyard();
        break;
    case DUEL_CMD_BANISH_HAND_FUSION_MATERIAL:
        DuelCmd_BanishHandFusionMaterial();
        break;
    case DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN:
        DuelCmd_BanishHandCardFaceDown();
        break;
    case DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND:
        DuelCmd_ReturnBanishedCardToHand();
        break;
    case DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP:
        DuelCmd_ReturnGraveyardCardToDeckTop();
        break;
    case DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_BOTTOM:
        DuelCmd_ReturnGraveyardCardToDeckBottom();
        break;
    case DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND:
        DuelCmd_ReturnGraveyardCardToHand();
        break;
    case DUEL_CMD_BANISH_GRAVEYARD_CARD:
        DuelCmd_BanishGraveyardCard();
        break;
    case DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD:
        DuelCmd_RemoveCardFromGraveyard();
        break;
    case DUEL_CMD_TAKE_OPPONENT_GRAVEYARD_CARD:
        DuelCmd_TakeOpponentGraveyardCard();
        break;
    case DUEL_CMD_RETURN_GRAVEYARD_TO_DECK:
        DuelCmd_ReturnGraveyardToDeck();
        break;
    case DUEL_CMD_ADD_CARD_TO_GRAVEYARD_NO_REDRAW:
        DuelCmd_AddCardToGraveyardNoRedraw();
        break;
    case DUEL_CMD_CLEAR_PENDING_EQUIP:
        DuelCmd_ClearPendingEquip();
        break;
    case DUEL_CMD_EQUIP_GRAVEYARD_CARD_TO_OPPONENT:
        DuelCmd_EquipGraveyardCardToOpponent();
        break;
    case DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON:
        sub_080106BC();
        break;
    case DUEL_CMD_MARK_GRAVEYARD_CARD:
        sub_08010708();
        break;
    case DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK:
        DuelCmd_RemoveCardFromFusionDeck();
        break;
    case DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD:
        DuelCmd_SendFusionDeckCardToGraveyard();
        break;
    case DUEL_CMD_RETURN_BANISHED_CARD_TO_GRAVEYARD:
        DuelCmd_ReturnBanishedCardToGraveyard();
        break;
    case DUEL_CMD_TOSS_COIN:
        DuelCmd_TossCoin();
        break;
    case DUEL_CMD_TOSS_THREE_COINS:
        DuelCmd_TossThreeCoins();
        break;
    case DUEL_CMD_ROLL_GRACEFUL_DICE:
        DuelCmd_RollGracefulDice();
        break;
    case DUEL_CMD_ROLL_PLAIN_DIE:
        DuelCmd_RollPlainDie();
        break;
    case DUEL_CMD_ROLL_SKULL_DICE:
    case DUEL_CMD_ROLL_SKULL_DICE_ALT:
        DuelCmd_RollSkullDice();
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}
