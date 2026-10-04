/*
 * Link Battle (main-menu slot 1) and the start of a Campaign day (wiki/functions/link-battle-c.md):
 *  - ChainListScreen_Start, which arms the chain-list overlay, and nine empty stubs of the compiled-out
 *    debug output (DebugPrintf and DebugPrintFlush are still called from many places);
 *  - CB_LinkBattle and its steps gLinkBattleSteps (enum LinkBattleStep): deck check, rock-paper-scissors over
 *    the cable, the "Now Connecting..." deck exchange, the end of the duel, the link-error path and the
 *    deck-too-small message;
 *  - Campaign step 0 (Campaign_StartDay: today's calendar event can fix the opponent for a tournament,
 *    Grandpa Cup, Duel Ceremony or Rare Hunter duel) and the pre-duel dialogue
 *    (Campaign_StartPreDuelDialogue: the opponent's intro text, holiday special duels with their starting
 *    Field), with the rare-card helpers of the Rare Hunter event.
 */
#include "global.h"
#include "gba.h"                /* REG_DISPCNT, REG_MOSAIC, REG_BLDCNT, REG_BLDY, REG_IE, REG_IME, DISPCNT_BG0_ON,
                                 * INTR_FLAG_HBLANK */
#include "main.h"               /* gMain, IntrTable, SetMainCallback, ResetBgScroll, enum IntrSlot / VBlankFlag */
#include "duel.h"               /* gDuel, gDuelPlayers, ShuffleDeck */
#include "sound.h"              /* PlayBGM, FadeOutBGM */
#include "constants/duel.h"     /* enum DuelField, DuelFormat */
#include "constants/game.h"     /* enum DuelistId */
#include "constants/sound.h"    /* enum Song */
#include "card_data.h"          /* CARD_ID_COUNT, CARD_ID_MASK, CARD_NUMBER_ALT_ART */
#include "save.h"               /* gSaveData, AddCardToTrunk, LoadPlayerDeckFromSave */
#include "calendar.h"           /* struct Date, GetCurrentDate, GetCalendarEvents, enum CalendarEvent */
#include "campaign.h"           /* the steps defined here, enum CampaignStep / LinkBattleStep, unlock checks */
#include "util.h"               /* MemClear16, Random */
#include "palette.h"            /* SetBrightnessBlack */
#include "bg.h"                 /* ResetVideo, LoadBgImage4bpp */
#include "bustup.h"             /* StartDialogue, CB_Bustup */
#include "link.h"               /* LinkInit, LinkShutdown */
#include "debug.h"              /* DebugMenu_Init; DebugPrintf and DebugPrintFlush are defined here */
#include "turn_order.h"         /* TurnOrder_RunRpsLink */
#include "duel_flow.h"          /* Duel_Setup, gDuelCtrl */
#include "duel_screen.h"        /* gDuelScreen, ChainListScreen_Start (defined here) */
#include "duel_link.h"          /* gLinkState, enum LinkMsgId, DuelLink_SendMessage / SendDeck / SendFusionDeck /
                                 * PollMessage */

/* ---- Local data and views ---- */

/* A scene step: returns nonzero when done, and the runner moves on to the next entry. */
typedef u16 (*StepFunc)(void);

/* Views of functions that other units define with another type (build/readability/HEADERS.md, pattern 2):
 * the same symbols under a local name, so that the calls here compile as the ROM's do. */

/* Views of FadeToBlack / FadeFromBlack (palette.h: u32 f(s32 step)) with a u16 result. Matching: the Link
 * Battle steps test the result as a halfword (lsl #16 after the call), which the u32 prototype leaves out. */
u16 FadeToBlackU16(s32 step) asm("FadeToBlack");
u16 FadeFromBlackU16(s32 step) asm("FadeFromBlack");
/* A view of CB_Bustup (bustup.h: u16 CB_Bustup(void)) with a u32 result. Matching: Campaign_StartDay tests the
 * result as a whole word, where the u16 prototype adds an lsl #16 after each call; the other callers here use
 * the header prototype. */
u32 CB_BustupU32(void) asm("CB_Bustup");
/* Views of IsCampaignLevel2/3/4Unlocked (campaign.h: u16 f(void)) with an int result. Matching: Campaign_StartDay
 * uses the results as ints, where the u16 prototype adds an lsl #16 after each call. */
int IsCampaignLevel2UnlockedInt(void) asm("IsCampaignLevel2Unlocked");
int IsCampaignLevel3UnlockedInt(void) asm("IsCampaignLevel3Unlocked");
int IsCampaignLevel4UnlockedInt(void) asm("IsCampaignLevel4Unlocked");

/* The first bytes of gChainListScreen (duel_screen.h: struct ChainListScreen) as whole bytes. Matching:
 * ChainListScreen_Start stores the byte at +0x4 whole (resolving = bit 0, state = 0) and the timer at +0x5;
 * with the header's resolving:1 / state:7 bitfields the stores become read-modify-write. */
struct ChainListScreenBytes {
    u32 list;                           /* +0x0 the chain to list */
    u8 flags;                           /* +0x4 bit 0 resolving, bits 1-7 state */
    u8 timer;                           /* +0x5 */
};
extern struct ChainListScreenBytes gChainListScreenBytes asm("gChainListScreen");

extern const StepFunc gLinkBattleSteps[];   /* 0x08198E7C: CB_LinkBattle's steps (enum LinkBattleStep) */
extern u16 gLinkConnectingImage[];          /* 0x086893D8: "Now Connecting..." BG image pack */

/* 0x08081A6C: card numbers of 60 rare cards (Blue-Eyes, the Exodia pieces, ..., the three Gods). */
extern const u16 gRareCardNumbers[60];
/* Opponents (enum DuelistId) drawn for the day's event duels. */
extern const u16 gTournamentOpponents[20];          /* 0x08081A28: duelists 1-20, five per tier */
extern const u16 gGrandpaCupQualifierOpponents[4];  /* 0x08081A50: Joey, Tea, Tristan, Yugi */
extern const u16 gGrandpaCupFinalOpponents[5];      /* 0x08081A58: Mai, Weevil, Rex, Espa Roba, Yami Bakura */
extern const u16 gRareHunterOpponents[5];           /* 0x08081A62: the five Ghouls, duelists 11-15 */

/* Per-opponent dialogue ids (indexed by enum DuelistId) and the pre-duel BGM. */
extern const u16 gOpponentDialogueBGM[];            /* 0x080819F6 */
extern const u16 gOpponentRematchText[];            /* 0x08081B16: against the last opponent again */
extern const u16 gOpponentGreetingText[];           /* 0x08081B48: a returning opponent */
extern const u16 gOpponentMatchChallengeText[];     /* 0x08081B7A: "let's play a Match" */
extern const u16 gOpponentWeekendDuelText[];        /* 0x08081BAE: drawn for a Duel Ceremony */
extern const u16 gOpponentChampionshipText[];       /* 0x08081BE2: met in a Championship round */
extern const u16 gOpponentGrandpaCupText[];         /* 0x08081C16: met in the Grandpa Cup */
extern const u16 gOpponentChristmasText[];          /* 0x08081C42: Christmas Eve special duel */
extern const u16 gOpponentFieldDuelText[];          /* 0x08081C76: Valentine's / White Day Field duel */

/* Matching: the matched code reads gCardIdToNumber (0x08622AB4) and gCardNumberToId (0x08623DF4) through
 * integer addresses; keep that form (build/readability/HEADERS.md, pattern 4). */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* Card number to card ID: 0xFFFF maps to 0, an alternate-art number 2000 + n to the ID of n, plus 1. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= CARD_NUMBER_ALT_ART - 1)
        return ((const u16 *)0x08623DF4)[number & CARD_ID_MASK];
    return ((const u16 *)0x08623DF4)[(number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/* ---- Chain-list overlay and debug stubs ---- */

/*
 * Arm the chain-list overlay that ChainListScreen_Run draws: list is the chain (a struct ChainList *), and
 * resolving & 1 selects the header, 0 "Chain : Activating" or 1 "Chain : Resolving" (duel_screen.h calls
 * the parameter player). The state and the timer start at 0.
 */
void ChainListScreen_Start(u32 list, u16 resolving)
{
    gChainListScreenBytes.list = list;
    gChainListScreenBytes.flags = resolving & 1;
    /* FAKEMATCH: `resolving & ~resolving` is always 0; a plain 0 lets the scheduler hoist the movs r0,#0
     * above the first strb. */
    gChainListScreenBytes.timer = resolving & ~resolving;
}

/* Empty stubs without callers (compiled-out debug code). */
void DebugDrawValue(void)
{
}

void DebugUnusedNop1(void)
{
}

void DebugUnusedNop2(void)
{
}

void DebugPrint(void)
{
}

/* The debug printf, compiled out: only the variadic prologue is left. */
void DebugPrintf(const char *fmt, ...)
{
}

void DebugUnusedNop3(void)
{
}

/* Empty; called right after DebugPrintf (hypothesis: the flush half of the print API). */
void DebugPrintFlush(void)
{
}

void DebugUnusedNop4(void)
{
}

void DebugUnusedNop5(void)
{
}

/* ---- Link Battle ---- */

/*
 * Link Battle step 0. Sub-step 0 loads the player's deck from the save; with fewer than 40 cards the scene
 * jumps to LINKBATTLE_STEP_DECK_TOO_SMALL. 1 resets the starting Field, the link and the link state
 * (gLinkState). The next frame shuts the link down again and returns 1.
 */
int LinkBattle_Init(void)
{
    switch (gMain.seqIndex1) {
    case 0:
        LoadPlayerDeckFromSave();
        if (gDuelPlayers[0].deckCount < DECK_MIN_CARDS) {
            gMain.seqState0 = LINKBATTLE_STEP_DECK_TOO_SMALL;
            break;
        }
        gMain.seqIndex1++;
        /* fall through */
    case 1:
        gMain.startField = 0;
        LinkInit();
        MemClear16((void *)0x02017FB0, sizeof(struct LinkState));   /* &gLinkState, as an integer (matched) */
        gDuel.linkError = 0;
        gMain.seqIndex1++;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        break;
    default:
        LinkShutdown();
        return 1;
    }
    return 0;
}

/* Link Battle step 1: rock-paper-scissors over the link decides who goes first. Returns 1 when done. */
u16 LinkBattle_DecideTurnOrder(void)
{
    gDuel.linkError = 0;
    return TurnOrder_RunRpsLink();
}

/* LinkBattle_Connect sub-steps (gMain.seqIndex1). */
enum {
    CONNECT_STEP_LINK_INIT = 0,
    CONNECT_STEP_DUEL_SETUP = 1,        /* until Duel_Setup returns nonzero; marks the duel as linked */
    CONNECT_STEP_LOAD_DECK = 2,
    CONNECT_STEP_VIDEO = 3,             /* video setup (DebugMenu_Init: BG0, system font), all off, black */
    CONNECT_STEP_SHOW_IMAGE = 4,        /* "Now Connecting...", tell the partner about fast mode */
    CONNECT_STEP_FADE_IN = 5,           /* the second player is done here */
    CONNECT_STEP_SEND_DECK = 6,         /* on the partner's READY: READY back and our deck */
    CONNECT_STEP_REQUEST_DECK = 7,      /* our deck stored: ask for the partner's */
    CONNECT_STEP_SEND_FUSION_DECK = 8,  /* the partner's deck arrived: send our fusion deck */
    CONNECT_STEP_REQUEST_FUSION = 9,    /* our fusion deck stored: ask for the partner's */
    CONNECT_STEP_WAIT_FUSION = 10,      /* until the partner's fusion deck arrives */
    CONNECT_STEP_FADE_OUT = 11,
};

/*
 * Link Battle step 2: open the link, set up the duel, load and shuffle the deck and show the "Now
 * Connecting..." screen. Only the player who goes first runs the exchange; the second player
 * (gMain.firstPlayer set) returns 1 after the fade-in, and its duel sends LINKMSG_READY, which starts the
 * exchange here: each side sends its deck and fusion deck when the partner has stored the previous list.
 * Returns 1 when done.
 */
int LinkBattle_Connect(void)
{
    switch (gMain.seqIndex1) {
    case CONNECT_STEP_LINK_INIT:
        LinkInit();
        gMain.seqIndex1++;
        return 0;
    case CONNECT_STEP_DUEL_SETUP:
        if (Duel_Setup()) {
            gDuelCtrl.isLinkDuel = 1;
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_LOAD_DECK:
        LoadPlayerDeckFromSave();
        ShuffleDeck(0, 4);
        gMain.seqIndex1++;
        return 0;
    case CONNECT_STEP_VIDEO:
        DebugMenu_Init();
        REG_DISPCNT = 0;
        REG_MOSAIC = 0;
        REG_BLDCNT = 0;
        REG_BLDY = 0;
        gMain.vblankFlags = VBLANK_COPY_BG_MAPS;
        SetBrightnessBlack();
        ResetVideo();
        ResetBgScroll();
        gMain.seqIndex1++;
        return 0;
    case CONNECT_STEP_SHOW_IMAGE:
        LoadBgImage4bpp(0, 0, 0x100, gLinkConnectingImage);
        if (gDuelScreen.fast)
            DuelLink_SendMessage(LINKMSG_FAST_MODE, 0, 0, 0);
        gMain.seqIndex1++;
        return 0;
    case CONNECT_STEP_FADE_IN:
        REG_DISPCNT = DISPCNT_BG0_ON;
        if (FadeFromBlackU16(4)) {
            if (gMain.firstPlayer)
                return 1;
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_SEND_DECK:
        if (gLinkState.readyReceived) {
            DuelLink_SendMessage(LINKMSG_READY, 0, 0, 0);
            DuelLink_SendDeck(0);
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_REQUEST_DECK:
        if (gLinkState.deckAcked) {
            DuelLink_SendMessage(LINKMSG_REQUEST_DECK, 0, 0, 0);
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_SEND_FUSION_DECK:
        if (gLinkState.deckReceived) {
            DuelLink_SendFusionDeck(0);
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_REQUEST_FUSION:
        if (gLinkState.fusionAcked) {
            DuelLink_SendMessage(LINKMSG_REQUEST_FUSION_DECK, 0, 0, 0);
            gMain.seqIndex1++;
        }
        return 0;
    case CONNECT_STEP_WAIT_FUSION:
        if (gLinkState.fusionReceived)
            gMain.seqIndex1++;
        return 0;
    case CONNECT_STEP_FADE_OUT:
        if (FadeToBlackU16(4))
            gMain.seqIndex1++;
        return 0;
    default:
        gLinkState.readyReceived = 0;
        return 1;
    }
}

/* Link Battle step 4, after the duel: fade to black, then clear the link-duel flag and shut the link down.
 * Returns 1 when done (the next entry is NULL: back to the main menu). */
int LinkBattle_Finish(void)
{
    if (FadeToBlackU16(8)) {
        gDuelCtrl.isLinkDuel = 0;
        LinkShutdown();
        return 1;
    }
    return 0;
}

/*
 * Link Battle step 6, after a link error: send the abort message every frame while fading out. When black,
 * stop the music, disable the HBlank interrupt and remove its handler and the VBlank callback, and start
 * text 320 ("Couldn't link up"). Returns 1 then.
 */
int LinkBattle_ShowLinkError(void)
{
    DuelLink_SendMessage(LINKMSG_ABORT, 0, 0, 0);
    if (FadeToBlackU16(4)) {
        FadeOutBGM();
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        REG_IME = 1;
        /* Again, now also removing the handler (the ROM does both). */
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        IntrTable[INTR_SLOT_HBLANK] = NULL;
        REG_IME = 1;
        gMain.vblankCallback = NULL;
        StartDialogue(320);
        return 1;
    }
    return 0;
}

/* Link Battle step 7: send the abort message every 8th frame until the error dialogue is closed (1). */
u16 LinkBattle_RunErrorDialogue(void)
{
    if (!(gMain.frameCounter & 7))
        DuelLink_SendMessage(LINKMSG_ABORT, 0, 0, 0);
    return CB_Bustup();
}

/* Link Battle step 8: shut the link down; returns 1 (the next entry is NULL). */
int LinkBattle_ErrorShutdown(void)
{
    LinkShutdown();
    return 1;
}

/* Link Battle step 10: text 401 ("your Deck must contain 40 cards or more"); returns 1 when it is closed. */
int LinkBattle_DeckTooSmall(void)
{
    switch (gMain.subStep) {
    case 0:
        StartDialogue(401);
        gMain.subStep++;
        break;
    case 1:
        if (CB_Bustup()) {
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
 * Scene callback of main-menu slot 1: runs gLinkBattleSteps[gMain.seqState0] once per frame (polling the
 * partner's messages while the duel is linked) and moves to the next step when it returns nonzero. A link
 * error during steps 1-3 jumps to LINKBATTLE_STEP_LINK_ERROR. Returns 1 at a NULL entry.
 */
int CB_LinkBattle(void)
{
    if (gLinkBattleSteps[gMain.seqState0] != NULL) {
        if (gDuelCtrl.isLinkDuel)
            DuelLink_PollMessage();
        if (gLinkBattleSteps[gMain.seqState0]()) {
            gMain.seqState0++;
            gMain.subStep = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        /* A link error in steps TURN_ORDER .. DUEL (1-3). */
        if (gDuel.linkError && (u8)(gMain.seqState0 - LINKBATTLE_STEP_TURN_ORDER) <= 2) {
            gMain.seqState0 = LINKBATTLE_STEP_LINK_ERROR;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* ---- Campaign: pre-duel dialogue and start of the day ---- */

/* Give one copy of every card ID 1-820 with a card number that the trunk does not hold yet. */
void GiveMissingCards(void)
{
    int id;

    for (id = 1; id <= CARD_ID_COUNT - 1; id++) {
        if (CARD_NUMBER(id) < 0xFFFF && gSaveData.trunk[id].count == 0)
            AddCardToTrunk(id);
    }
}

/*
 * Start the pre-duel dialogue for gMain.opponent and set the duel's rules (gMain.duelFormat, gMain.events,
 * gMain.startField). Rare Hunter event: the Ghoul's text. Match days (DUEL_FORMAT_MATCH, set by
 * Campaign_StartDay): the Championship, Duel Ceremony or Grandpa Cup text, then the match text. Otherwise
 * the first-meeting, rematch or greeting text by the opponent's record; the first meeting with Simon gives
 * every missing card. Then, on some holidays, a special duel with one opponent: a best-of-3 match for that
 * holiday's prize (gMain.events narrowed to its bit), mostly with a starting Field, and BGM 0x30. Other
 * duels clear gMain.events and play the opponent's BGM. Returns 1 (0 for the later duels of a match day);
 * the caller ignores it.
 */
int Campaign_StartPreDuelDialogue(void)
{
    u32 opp = gMain.opponent;
    u16 text;
    u32 events;

    gMain.startField = 0;
    events = gMain.events;
    if (events == CAL_RARE_HUNTER) {
        switch (opp) {
        case DUELIST_RARE_HUNTER:
            StartDialogue(11002);
            break;
        case DUELIST_ARKANA:
            StartDialogue(12002);
            break;
        case DUELIST_STRINGS:
            StartDialogue(13002);
            break;
        case DUELIST_UMBRA_LUMIS:
            StartDialogue(14002);
            break;
        case DUELIST_MARIK:
            StartDialogue(15002);
            break;
        }
    } else if (opp - 1 <= 29) {     /* opponent 1-30 */
        text = gOpponentFirstMeetingText[opp];
        if (gMain.duelFormat == DUEL_FORMAT_MATCH) {
            if (events & (CAL_TOURNAMENT_ROUND1 | CAL_TOURNAMENT_ROUND2 | CAL_TOURNAMENT_SEMIFINAL
                          | CAL_TOURNAMENT_FINAL)) {
                StartDialogue(gOpponentChampionshipText[opp]);
                return 1;
            } else if (events & CAL_DUEL_CEREMONY) {
                StartDialogue(gOpponentWeekendDuelText[opp]);
                return 1;
            } else if (events & (CAL_SUGOROKU_PRELIM | CAL_SUGOROKU_MATCH)) {
                StartDialogue(gOpponentGrandpaCupText[opp]);
                return 1;
            }
            StartDialogue(gOpponentMatchChallengeText[opp]);
            /* The later duels of the match stop here. Otherwise the code goes on, and the StartDialogue
             * at the end replaces this text. */
            if (gMain.matchDuelCount)
                return 0;
        } else {
            if (gSaveData.duelRecords[opp].wins + gSaveData.duelRecords[opp].losses
                + gSaveData.duelRecords[opp].draws == 0) {
                if (opp == DUELIST_SIMON)
                    GiveMissingCards();
            } else if (opp == gSaveData.lastOpponent) {
                text = gOpponentRematchText[opp];
            } else {
                text = gOpponentGreetingText[opp];
            }
        }

        /* A holiday special duel: the text, a best-of-3 match for this holiday, a starting Field. */
#define SPECIAL_DUEL(event, textId, field)          \
    {                                               \
        StartDialogue(textId);                      \
        gMain.duelFormat = DUEL_FORMAT_MATCH;       \
        gMain.events = event;                       \
        gMain.startField = field;                   \
        PlayBGM(SONG_SPECIAL_DUEL);                 \
        return 1;                                   \
    }
        if ((gMain.events & CAL_NEW_YEARS_DAY) && opp == DUELIST_YAMI_YUGI)
            SPECIAL_DUEL(CAL_NEW_YEARS_DAY, 20003, FIELD_MYSTIC_PLASMA_ZONE)
        else if ((gMain.events & CAL_COMING_OF_AGE_DAY) && opp == DUELIST_MAI)
            SPECIAL_DUEL(CAL_COMING_OF_AGE_DAY, 10003, FIELD_MOUNTAIN)
        else if ((gMain.events & CAL_FOUNDATION_DAY) && opp == DUELIST_REX)
            SPECIAL_DUEL(CAL_FOUNDATION_DAY, 6002, FIELD_WASTELAND)
        else if ((gMain.events & CAL_GREENERY_DAY) && opp == DUELIST_WEEVIL)
            SPECIAL_DUEL(CAL_GREENERY_DAY, 8002, FIELD_FOREST)
        else if ((gMain.events & CAL_CONSTITUTION_DAY) && opp == DUELIST_BAKURA)
            SPECIAL_DUEL(CAL_CONSTITUTION_DAY, 5003, FIELD_YAMI)
        else if ((gMain.events & CAL_CITIZENS_HOLIDAY) && opp == DUELIST_TRISTAN)
            SPECIAL_DUEL(CAL_CITIZENS_HOLIDAY, 4003, FIELD_WASTELAND)
        else if ((gMain.events & CAL_CHILDRENS_DAY) && opp == DUELIST_YUGI)
            SPECIAL_DUEL(CAL_CHILDRENS_DAY, 1003, FIELD_YAMI)
        else if ((gMain.events & CAL_MARINE_DAY) && opp == DUELIST_MAKO)
            SPECIAL_DUEL(CAL_MARINE_DAY, 9002, FIELD_UMI)
        else if ((gMain.events & CAL_RESPECT_FOR_AGED_DAY) && opp == DUELIST_ARKANA)
            SPECIAL_DUEL(CAL_RESPECT_FOR_AGED_DAY, 12006, FIELD_MYSTIC_PLASMA_ZONE)
        else if ((gMain.events & CAL_SPORTS_DAY) && opp == DUELIST_JOEY)
            /* Original bug: stores CAL_COMING_OF_AGE_DAY, so the prize is Vol.4 instead of The Final
             * Duelist. */
            SPECIAL_DUEL(CAL_COMING_OF_AGE_DAY, 3003, FIELD_SOGEN)
        else if ((gMain.events & CAL_CULTURE_DAY) && opp == DUELIST_ESPA_ROBA)
            SPECIAL_DUEL(CAL_CULTURE_DAY, 7002, FIELD_LUMINOUS_SPARK)
        else if ((gMain.events & CAL_LABOR_THANKSGIVING_DAY) && opp == DUELIST_TEA)
            SPECIAL_DUEL(CAL_LABOR_THANKSGIVING_DAY, 2003, FIELD_CHORUS_OF_SANCTUARY)
        else if ((gMain.events & CAL_EMPERORS_BIRTHDAY) && opp == DUELIST_KAIBA) {
            StartDialogue(16002);
            gMain.duelFormat = DUEL_FORMAT_MATCH;
            gMain.events = CAL_EMPERORS_BIRTHDAY;
            gMain.startField = Random() % 7 + FIELD_CHORUS_OF_SANCTUARY;  /* .. FIELD_MYSTIC_PLASMA_ZONE */
            PlayBGM(SONG_SPECIAL_DUEL);
            return 1;
        } else if ((gMain.events & CAL_MURAN_BIRTHDAY) && opp == DUELIST_SIMON) {
            StartDialogue(22002);
            gMain.duelFormat = DUEL_FORMAT_MATCH;
            gMain.events = CAL_MURAN_BIRTHDAY;
            gMain.startField = Random() % 13 + FIELD_FOREST;    /* any Field; no BGM change */
            return 1;
        } else if ((gMain.events & CAL_HALLOWEEN) && opp == DUELIST_RARE_HUNTER) {
            StartDialogue(11006);
            gMain.duelFormat = DUEL_FORMAT_MATCH;
            gMain.events = CAL_HALLOWEEN;
            gMain.startField = Random() % 13 + FIELD_FOREST;    /* any Field */
            PlayBGM(SONG_SPECIAL_DUEL);
            return 1;
        } else if ((gMain.events & CAL_VALENTINES_DAY) && (opp == DUELIST_TEA || opp == DUELIST_MAI)) {
            StartDialogue(gOpponentFieldDuelText[opp]);
            gMain.duelFormat = DUEL_FORMAT_MATCH;
            gMain.events = CAL_VALENTINES_DAY;
            gMain.startField = Random() % 6 + FIELD_FOREST;     /* .. FIELD_YAMI */
            PlayBGM(SONG_SPECIAL_DUEL);
            return 1;
        } else if ((gMain.events & CAL_WHITE_DAY)
                   && (opp == DUELIST_YUGI || opp == DUELIST_JOEY || opp == DUELIST_TRISTAN
                       || opp == DUELIST_BAKURA)) {
            StartDialogue(gOpponentFieldDuelText[opp]);
            gMain.duelFormat = DUEL_FORMAT_MATCH;
            gMain.events = CAL_WHITE_DAY;
            gMain.startField = Random() % 6 + FIELD_FOREST;     /* .. FIELD_YAMI */
            PlayBGM(SONG_SPECIAL_DUEL);
            return 1;
        } else if (opp != DUELIST_DUEL_COMPUTER) {
            /* Christmas Eve, Spring Day and Autumn Day: any opponent but the Duel Computer. */
            u32 flags = gMain.events;
            if (flags & CAL_CHRISTMAS_EVE)
                SPECIAL_DUEL(CAL_CHRISTMAS_EVE, gOpponentChristmasText[opp], FIELD_CHORUS_OF_SANCTUARY)
            else if (flags & CAL_SPRING_DAY)
                SPECIAL_DUEL(CAL_SPRING_DAY, gOpponentMatchChallengeText[opp], FIELD_SOGEN)
            else if (flags & CAL_AUTUMN_DAY)
                SPECIAL_DUEL(CAL_AUTUMN_DAY, gOpponentMatchChallengeText[opp], FIELD_MOUNTAIN)
        }
#undef SPECIAL_DUEL
        gMain.events = 0;
        PlayBGM(gOpponentDialogueBGM[gMain.opponent]);
        StartDialogue(text);
    }
    return 1;
}

/* 1 if the player owns at least minCount of the 60 cards in gRareCardNumbers. */
u16 HasEnoughRareCards(int minCount)
{
    u32 i = 0;
    int owned = 0;

    for (; i < ARRAY_COUNT(gRareCardNumbers); i++) {
        if (gSaveData.trunk[CardNumberToId(gRareCardNumbers[i])].count != 0)
            owned++;
    }
    return owned >= minCount;
}

/* A random card ID from gRareCardNumbers that the player owns. Loops forever if none is owned; callers
 * check HasEnoughRareCards first. */
u16 PickRandomOwnedRareCard(void)
{
    u16 id;

    do {
        /* Matching: the named temporary keeps the table address from being hoisted out of the loop. */
        u32 r = (u32)Random() % ARRAY_COUNT(gRareCardNumbers);
        id = CardNumberToId(gRareCardNumbers[r]);
    } while (gSaveData.trunk[id].count == 0);
    return id;
}

/* Campaign_StartDay sub-steps (gMain.subStep). */
enum {
    START_DAY_STEP_DECK_CHECK = 0,
    START_DAY_STEP_EVENTS = 1,              /* today's events: start an event dialogue or return 1 */
    START_DAY_STEP_RARE_HUNTER = 7,         /* after the Rare Hunter intro: draw the Ghoul */
    START_DAY_STEP_RESET = 8,               /* SetMainCallback(NULL); nothing sets this sub-step */
    START_DAY_STEP_EVENT_INTRO = 9,         /* after a tournament / Duel Ceremony intro */
    START_DAY_STEP_UNLOCK_NOTICE = 10,      /* after the "new Duelists" notice */
};

/*
 * Campaign step 0: the start of the day. With fewer than 40 deck cards go to CAMPAIGN_STEP_DECK_TOO_SMALL.
 * Otherwise reset the match state and look at today's calendar events. Championship round, Grandpa Cup and
 * Duel Ceremony days draw the opponent from their table, show the event's intro and make the duel a
 * best-of-3 match (DUEL_FORMAT_MATCH) with a fixed opponent. Every 60th day, a player who owns 5 or more of the
 * rare cards meets a Rare Hunter in a single duel. Otherwise the "new Duelists" notice may be shown, and
 * today's events are kept in gMain.events for the pre-duel dialogue. Returns 1 when done.
 */
int Campaign_StartDay(void)
{
    struct Date date;
    u32 events;

    switch (gMain.subStep) {
    case START_DAY_STEP_DECK_CHECK:
        LoadPlayerDeckFromSave();
        if (gDuelPlayers[0].deckCount < DECK_MIN_CARDS) {
            gMain.seqIndexCampaign = CAMPAIGN_STEP_DECK_TOO_SMALL;
            return 0;
        }
        gMain.subStep++;
        /* fall through */
    case START_DAY_STEP_EVENTS:
        GetCurrentDate(&date);
        events = GetCalendarEvents(date.year, date.month, date.day);
        /* New Year's Eve: the year's tournament progress is reset. */
        if (date.month == 12 && date.day == 31) {
            gSaveData.tournamentRound = 0;
            gSaveData.sugorokuQualified = 0;
        }
        gMain.opponentFixed = 0;
        gMain.duelFormat = DUEL_FORMAT_SINGLE;
        gMain.matchDuelCount = 0;
        gMain.matchScore = 0;
        if (events & CAL_TOURNAMENT_ROUND1) {
            /* Championship rounds 1 to 3 and the final: an opponent of tier 1-4 (texts 201/203/205/207). */
            gMain.opponent = gTournamentOpponents[Random() % 5];
            StartDialogue(201);
            PlayBGM(SONG_CHAMPIONSHIP);
            gMain.events = CAL_TOURNAMENT_ROUND1;
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            return 0;
        } else if (events & CAL_TOURNAMENT_ROUND2) {
            gMain.opponent = gTournamentOpponents[Random() % 5 + 5];
            StartDialogue(203);
            PlayBGM(SONG_CHAMPIONSHIP);
            gMain.events = CAL_TOURNAMENT_ROUND2;
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            return 0;
        } else if (events & CAL_TOURNAMENT_SEMIFINAL) {
            gMain.opponent = gTournamentOpponents[Random() % 5 + 10];
            StartDialogue(205);
            PlayBGM(SONG_CHAMPIONSHIP);
            gMain.events = CAL_TOURNAMENT_SEMIFINAL;
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            return 0;
        } else if (events & CAL_TOURNAMENT_FINAL) {
            gMain.opponent = gTournamentOpponents[Random() % 5 + 15];
            gMain.events = CAL_TOURNAMENT_FINAL;
            StartDialogue(207);
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            PlayBGM(SONG_CHAMPIONSHIP_FINAL);
            return 0;
        } else if (events & CAL_SUGOROKU_PRELIM) {
            /* Grandpa Cup qualifier and final (texts 701/702). */
            gMain.opponent = gGrandpaCupQualifierOpponents[Random() & 3];
            gMain.events = CAL_SUGOROKU_PRELIM;
            StartDialogue(701);
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            gSaveData.sugorokuQualified = 0;
            PlayBGM(SONG_GRANDPA_CUP);
            return 0;
        } else if (events & CAL_SUGOROKU_MATCH) {
            gMain.opponent = gGrandpaCupFinalOpponents[Random() % 5u];
            gMain.events = CAL_SUGOROKU_MATCH;
            StartDialogue(702);
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            PlayBGM(SONG_GRANDPA_CUP);
            return 0;
        } else if (events & CAL_DUEL_CEREMONY) {
            /* Duel Ceremony (text 500): any opponent of the tiers unlocked so far. */
            int tiers = 1;

            if (IsCampaignLevel2UnlockedInt())
                tiers = 2;
            if (IsCampaignLevel3UnlockedInt())
                tiers++;
            if (IsCampaignLevel4UnlockedInt())
                tiers++;
            tiers *= 5;
            gMain.opponent = gTournamentOpponents[Random() % tiers];
            StartDialogue(500);
            gMain.events = CAL_DUEL_CEREMONY;
            gMain.subStep = START_DAY_STEP_EVENT_INTRO;
            PlayBGM(SONG_SPECIAL_DUEL);
            return 0;
        } else if (HasEnoughRareCards(5) && gSaveData.days != 0 && (u16)(gSaveData.days % 60) == 0) {
            /* Rare Hunter (text 900): every 60th day if the player owns 5 or more rare cards. */
            StartDialogue(900);
            PlayBGM(SONG_RARE_HUNTER_APPEARS);
            gMain.events = CAL_RARE_HUNTER;
            gMain.subStep = START_DAY_STEP_RARE_HUNTER;
            return 0;
        } else if (gSaveData.unlockNotices & 1) {
            /* "New Duelists" (text 350), noted by Campaign_RecordDuelResult. */
            gSaveData.unlockNotices &= ~1;
            StartDialogue(350);
            gMain.subStep = START_DAY_STEP_UNLOCK_NOTICE;
            return 0;
        } else {
            gMain.events = events;
            return 1;
        }
    case START_DAY_STEP_RARE_HUNTER:
        if (!CB_BustupU32())
            return 0;
        gMain.opponent = gRareHunterOpponents[Random() % 5];
        PlayBGM(SONG_RARE_HUNTER_CHALLENGE);
        gMain.opponentFixed = 1;
        return 1;
    case START_DAY_STEP_RESET:
        SetMainCallback(NULL);
        return 0;
    case START_DAY_STEP_EVENT_INTRO:
        if (!CB_BustupU32())
            return 0;
        gMain.opponentFixed = 1;
        gMain.duelFormat = DUEL_FORMAT_MATCH;
        return 1;
    case START_DAY_STEP_UNLOCK_NOTICE:
        if (!CB_BustupU32())
            return 0;
        return 1;
    }
    return 1;
}
