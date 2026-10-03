/*
 * Campaign steps 1-3, 5 and 6 (gCampaignSteps, run by CB_Campaign in campaign.c; enum CampaignStep):
 *  - Campaign_SelectOpponent: the opponent-select screen, unless the day's event fixed the opponent, then
 *    the opponent's pre-duel dialogue;
 *  - Campaign_DecideTurnOrder: rock-paper-scissors for the first duel of a match, a first/second choice for
 *    the later ones;
 *  - Campaign_SetupDuel: clears the duel and loads and shuffles both decks;
 *  - Campaign_ShowDuelResult: best-of-3 match scoring and the opponent's result text; an open match goes
 *    back to CAMPAIGN_STEP_TURN_ORDER after a side-deck swap;
 *  - Campaign_GiveRewards: records the duel (Campaign_RecordDuelResult), then hands out the prize of the
 *    day's event: Championship tickets, holiday and Grandpa Cup packs, the card a Rare Hunter takes, or
 *    the regular pick-a-pack screen.
 * A step returns 1 when it is done; gMain.subStep is its sub-state. See wiki/functions/campaign-steps-c.md.
 */
#include "global.h"
#include "constants/cards.h"    /* CARD_THE_MONARCHY, ... (the three Championship tickets) */
#include "constants/duel.h"     /* enum DuelResult, DuelFormat */
#include "constants/game.h"     /* enum BoosterPackId, DuelistId */
#include "save.h"               /* gSaveData, RecordDuel*, Add/RemoveCardFromTrunk, SaveGame, ... */
#include "calendar.h"           /* enum CalendarEvent (gMain.events) */
#include "campaign.h"           /* struct OpponentResultTexts, Campaign_*, OpponentSelect_Run */
#include "bustup.h"             /* StartDialogue, CB_Bustup */
#include "turn_order.h"         /* TurnOrder_RunRps, TurnOrder_RunPlayerChoice, TurnOrder_RunCpuChoice */
#include "duel_flow.h"          /* Duel_Setup, gDuelCtrl */
#include "ai.h"                 /* LoadOpponentDeck */
#include "booster.h"            /* CB_GetPack, GetRewardPack */
#include "card_detail.h"        /* CardDetail_Init, CardDetail_Run */
#include "deck_edit.h"          /* SideDeckSwap_Run */

/* ---- BEGIN header subset (pre-H0) ---- */
/*
 * The parts of main.h, duel.h and sound.h this unit uses, with the headers' tags, names, types and bitfield
 * containers. include/main.h, duel.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md), and the legacy main.h has none of the +0x4888 names. After H0, replace this
 * block (BEGIN to END) with:
 *     #include "main.h"
 *     #include "duel.h"
 *     #include "sound.h"
 */
struct Main {
    u8 unk0[0x4857];
    u8 seqIndexCampaign;                /* +0x4857 step index of the Campaign runner */
    u8 seqState0;                       /* +0x4858 */
    u8 seqIndex1;                       /* +0x4859 step index of the menu/Password/Trading/Deck Edit runners */
    u8 seqState1;                       /* +0x485A */
    u8 seqState2;                       /* +0x485B */
    u8 unk485C[0x4870 - 0x485C];
    u8 firstPlayer:1;                   /* +0x4870 bit 0: who takes the first turn, 0 = this player */
    u8 opponent:5;                      /* +0x4870 bits 1-5: duelist ID of the Campaign opponent */
    u8 result:2;                        /* +0x4870 bits 6-7 */
    u8 unk4871[0x4876 - 0x4871];
    u16 rewardPack;                     /* +0x4876 reward for the Get Pack screen */
    u8 unk4878[4];
    u32 events;                         /* +0x487C CalendarEvent mask of the current duel (0 = ordinary) */
    u16 rewardCard;                     /* +0x4880 card given after a calendar-event duel (0 = none) */
    u8 unk4882[6];
    u8 unk4888_0:1;                     /* +0x4888 bit 0 */
    u8 opponentFixed:1;                 /* +0x4888 bit 1: opponent already chosen, skip OpponentSelect_Run */
    u8 duelFormat:2;                    /* +0x4888 bits 2-3: enum DuelFormat (1 single, 3 best of 3) */
    u8 matchDuelCount:2;                /* +0x4888 bits 4-5: duels played in the current match */
    u8 unk4888_6:2;
    s8 matchScore;                      /* +0x4889 wins minus losses in the current match */
    u16 startField:4;                   /* +0x488A bits 0-3 */
    u16 subStep:8;                      /* +0x488A bits 4-11: sub-state of the Campaign/Link/menu step */
    u16 unk488A_12:4;
};
extern struct Main gMain;

struct DuelState {
    u8 unk0[0x1B12];
    u8 bgmOn:1;                         /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                    /* +0x1B12 bit 1 */
    u8 phase:3;                         /* +0x1B12 bits 2-4 */
    u8 linkError:1;                     /* +0x1B12 bit 5 */
    u8 result:2;                        /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13;
};
extern struct DuelState gDuel;
void ShuffleDeck(int player, int passes);

void PlayBGM(u32 songId);
/* ---- END header subset ---- */

/* ---- Local data and views ---- */

/*
 * Matching: these callers test or return the result of four u16 functions as a whole word (no lsl #16
 * after the call), so they are called through u32-returning views of the same symbols.
 */
u32 CB_BustupU32(void) asm("CB_Bustup");
u32 OpponentSelect_RunU32(void) asm("OpponentSelect_Run");
u32 SideDeckSwap_RunU32(void) asm("SideDeckSwap_Run");
u32 IsPackUnlockedU32(u32 packId) asm("IsPackUnlocked");

/* 0x080819BE: the 28 booster packs of the Get Pack list in display order. */
extern const u16 gPackDisplayOrder[28];
/* 0x080817FC: what each opponent (enum DuelistId) says after a duel. */
extern const struct OpponentResultTexts gOpponentResultTexts[];
/* 0x0808198C: text before the next duel of an open match, per opponent. */
extern const u16 gOpponentNextMatchDuelText[];

/* The card IDs of the three Championship tickets, read through address-suffixed aliases of
 * gCardNumberToId entries: each gets its own literal, as in the ROM (a single base would be shared). */
extern const u16 gUnk_08624CCE;         /* gCardNumberToId[CARD_THE_MONARCHY]: round 2 prize */
extern const u16 gUnk_08624CD0;         /* gCardNumberToId[CARD_SET_SAIL_FOR_THE_KINGDOM]: round 1 prize */
extern const u16 gUnk_08624CD2;         /* gCardNumberToId[CARD_GLORY_OF_THE_KINGS_HAND]: semifinal prize */

/* Clear the sub-states of the screen runners before the next one starts. */
#define RESET_SEQ_STATE() (gMain.seqIndex1 = 0, gMain.seqState1 = 0, gMain.seqState2 = 0)

/*
 * Campaign step 1. Sub-step 0 runs the opponent-select screen until an opponent is confirmed (skipped when
 * gMain.opponentFixed: an event or an open match chose the opponent); 1 starts the pre-duel dialogue; 2
 * waits for it to close. Returns 1 after that.
 */
u16 Campaign_SelectOpponent(void)
{
    switch (gMain.subStep) {
    case 0:
        if (!gMain.opponentFixed) {
            if (OpponentSelect_RunU32()) {
                gMain.subStep++;
                RESET_SEQ_STATE();
            }
            return 0;
        }
        gMain.subStep++;
        /* fall through */
    case 1:
        Campaign_StartPreDuelDialogue();
        gMain.subStep++;
        /* fall through */
    case 2:
        if (CB_BustupU32()) {
            gMain.subStep++;
            RESET_SEQ_STATE();
        }
        return 0;
    }
    return 1;
}

/*
 * Campaign step 2: the first duel of a match starts with rock-paper-scissors. In the later duels the
 * loser of the previous duel picks: the player after a loss, otherwise the opponent. Returns the runner's
 * result (1 when the turn order is decided).
 */
u16 Campaign_DecideTurnOrder(void)
{
    if (!gMain.matchDuelCount)
        return TurnOrder_RunRps();
    else if (gDuel.result == DUEL_RESULT_LOSE)
        return TurnOrder_RunPlayerChoice();
    else
        return TurnOrder_RunCpuChoice();
}

/* Campaign step 3: clear the duel state, then load and shuffle the player's and the opponent's decks.
 * Returns 1. */
u16 Campaign_SetupDuel(void)
{
    Duel_Setup();
    gDuelCtrl.isLinkDuel = 0;
    LoadPlayerDeckFromSave();
    ShuffleDeck(0, 8);
    LoadOpponentDeck(0);
    ShuffleDeck(1, 8);
    return 1;
}

/*
 * Record the duel's win, loss or draw against gMain.opponent. If that raised the campaign level, set
 * unlockNotices bit 0 (new opponents, text 350 on the next day); if it unlocked more of the Get Pack list,
 * set bit 1 (text 351, shown by Campaign_GiveRewards).
 */
void Campaign_RecordDuelResult(void)
{
    /* Matching: compared as signed below, so GetCampaignLevel's u32 result is read as int. */
    int levelBefore = GetCampaignLevel();
    u32 i;
    int packsBefore, packsAfter;

    for (i = 0, packsBefore = 0; i < ARRAY_COUNT(gPackDisplayOrder); i++) {
        if (IsPackUnlockedU32(gPackDisplayOrder[i]))
            packsBefore++;
    }
    switch (gDuel.result) {
    case DUEL_RESULT_WIN:
        RecordDuelWin(gMain.opponent);
        break;
    case DUEL_RESULT_LOSE:
        RecordDuelLoss(gMain.opponent);
        break;
    case DUEL_RESULT_DRAW:
        RecordDuelDraw(gMain.opponent);
        break;
    }
    if ((int)GetCampaignLevel() > levelBefore)
        gSaveData.unlockNotices |= 1;
    for (i = 0, packsAfter = 0; i < ARRAY_COUNT(gPackDisplayOrder); i++) {
        if (IsPackUnlockedU32(gPackDisplayOrder[i]))
            packsAfter++;
    }
    if (packsAfter > packsBefore)
        gSaveData.unlockNotices |= 2;
}

/* Campaign_GiveRewards sub-steps (gMain.subStep). */
enum {
    REWARD_STEP_RECORD = 0,             /* Campaign_RecordDuelResult */
    REWARD_STEP_CHOOSE = 1,             /* pick the prize from the result and gMain.events */
    REWARD_STEP_NEW_PACKS_TEXT = 2,     /* text 351 if new packs were unlocked (skips to 4 if not) */
    REWARD_STEP_WAIT_TEXT = 3,
    REWARD_STEP_PICK_PACK = 4,          /* the regular pick-a-pack screen */
    REWARD_STEP_TICKET = 0xA,           /* Championship: give the ticket, or take all three back */
    REWARD_STEP_CHAMPION_PACK = 0xB,    /* Championship final: pack 509 */
    REWARD_STEP_SHOW_TICKET = 0xC,      /* Card Detail of the ticket (and 0xD) */
    REWARD_STEP_GRANDPA_CUP_TEXT = 0xE, /* then 0xF */
    REWARD_STEP_GIVE_PACK = 0xF,        /* GetRewardPack(gMain.rewardPack) */
    REWARD_STEP_LOSS_TEXT = 0x14,       /* tournament loss text, then done */
    REWARD_STEP_SHOW_LOST_CARD = 0x16,  /* Card Detail of the card a Rare Hunter took (and 0x17) */
    REWARD_STEP_RARE_HUNTER_TEXT = 0x18,
    REWARD_STEP_DONE = 0x19,
};

/*
 * Campaign step 6: record the duel, then hand out what the result earned. A won event duel gives a fixed
 * pack (or a Championship ticket); a lost Rare Hunter duel costs a random rare card; any other win or loss
 * goes on to the new-packs notice and the regular pick-a-pack screen. Returns 1 when done.
 */
u16 Campaign_GiveRewards(void)
{
    switch (gMain.subStep) {
    case REWARD_STEP_RECORD:
        Campaign_RecordDuelResult();
        gMain.subStep = REWARD_STEP_CHOOSE;
        return 0;
    case REWARD_STEP_CHOOSE:
        switch (gDuel.result) {
        case DUEL_RESULT_WIN:
            RESET_SEQ_STATE();
            switch (gMain.events) {
            /* Championship rounds 1-3: a ticket for the next round (texts 200/202/204). */
            case CAL_TOURNAMENT_ROUND1:
                StartDialogue(200);
                gSaveData.tournamentRound++;
                gMain.rewardCard = gUnk_08624CD0;
                gMain.subStep = REWARD_STEP_TICKET;
                return 0;
            case CAL_TOURNAMENT_ROUND2:
                StartDialogue(202);
                gSaveData.tournamentRound++;
                gMain.rewardCard = gUnk_08624CCE;
                gMain.subStep = REWARD_STEP_TICKET;
                return 0;
            case CAL_TOURNAMENT_SEMIFINAL:
                StartDialogue(204);
                gSaveData.tournamentRound++;
                gMain.rewardCard = gUnk_08624CD2;
                gMain.subStep = REWARD_STEP_TICKET;
                return 0;
            /* Final (text 206): the champion title. championshipWins is also bumped by
             * IncrementChampionshipWins in REWARD_STEP_TICKET (original behaviour). */
            case CAL_TOURNAMENT_FINAL:
                StartDialogue(206);
                gSaveData.tournamentRound = 0;
                gSaveData.championshipWins++;
                gMain.rewardCard = 0;
                gMain.subStep = REWARD_STEP_TICKET;
                return 0;
            case CAL_SUGOROKU_PRELIM:
                gSaveData.sugorokuQualified = 1;
                gMain.rewardPack = PACK_DUELIST_PACK;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_SUGOROKU_MATCH:
                StartDialogue(703);
                gMain.rewardPack = PACK_GRANDPA_CUP_PRIZE;
                gMain.subStep = REWARD_STEP_GRANDPA_CUP_TEXT;
                return 0;
            case CAL_RARE_HUNTER:
                gMain.rewardPack = PACK_RARE_SELECTIONS;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            /* Holiday special duels (Campaign_StartPreDuelDialogue). */
            case CAL_NEW_YEARS_DAY:
            case CAL_GREENERY_DAY:
            case CAL_EMPERORS_BIRTHDAY:
            case CAL_MURAN_BIRTHDAY:
                gMain.rewardPack = PACK_LIMITED_COLLECTION;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_FOUNDATION_DAY:
                gMain.rewardPack = PACK_VOL_5;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_CONSTITUTION_DAY:
                gMain.rewardPack = PACK_MAGIC_RULER;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_CITIZENS_HOLIDAY:
                gMain.rewardPack = PACK_LOB_EWD;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_CHILDRENS_DAY:
            case CAL_LABOR_THANKSGIVING_DAY:
            case CAL_SPORTS_DAY:
                gMain.rewardPack = PACK_THE_FINAL_DUELIST;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_MARINE_DAY:
                gMain.rewardPack = PACK_PHANTOM_OF_G;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_RESPECT_FOR_AGED_DAY:
                gMain.rewardPack = PACK_PHARAOHS_SERVANT;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_CULTURE_DAY:
                gMain.rewardPack = PACK_CURSE_OF_ANUBIS;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_COMING_OF_AGE_DAY:
                gMain.rewardPack = PACK_VOL_4;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_SPRING_DAY:
            case CAL_AUTUMN_DAY:
            case CAL_HALLOWEEN:
            case CAL_CHRISTMAS_EVE:
                gMain.rewardPack = PACK_RARE_SELECTIONS;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            case CAL_VALENTINES_DAY:
            case CAL_WHITE_DAY:
                gMain.rewardPack = PACK_DUELIST_PACK;
                gMain.subStep = REWARD_STEP_GIVE_PACK;
                return 0;
            }
            gMain.subStep++;
            return 0;
        case DUEL_RESULT_LOSE:
            RESET_SEQ_STATE();
            switch (gMain.events) {
            /* Lost to a Ghoul (texts X001): the Rare Hunter takes one of the player's rare cards. */
            case CAL_RARE_HUNTER:
                switch (gMain.opponent) {
                case DUELIST_RARE_HUNTER: StartDialogue(11001); break;
                case DUELIST_ARKANA: StartDialogue(12001); break;
                case DUELIST_STRINGS: StartDialogue(13001); break;
                case DUELIST_UMBRA_LUMIS: StartDialogue(14001); break;
                case DUELIST_MARIK: StartDialogue(15001); break;
                }
                gMain.subStep = REWARD_STEP_SHOW_LOST_CARD;
                gMain.rewardCard = PickRandomOwnedRareCard();
                RemoveCardFromTrunk(gMain.rewardCard);
                SaveGame();
                return 0;
            /* A lost Championship round ends the year's tournament. */
            case CAL_TOURNAMENT_ROUND1:
            case CAL_TOURNAMENT_ROUND2:
            case CAL_TOURNAMENT_SEMIFINAL:
            case CAL_TOURNAMENT_FINAL:
                gSaveData.tournamentRound = 0;
                /* fall through */
            case CAL_DUEL_CEREMONY:
            case CAL_SUGOROKU_PRELIM:
            case CAL_SUGOROKU_MATCH:
                StartDialogue(300);
                gMain.subStep = REWARD_STEP_LOSS_TEXT;
                return 0;
            }
            break;
        }
        return 1;
    case REWARD_STEP_NEW_PACKS_TEXT:
        {
            u16 newPacks = gSaveData.unlockNotices & 2;
            if (newPacks) {
                gSaveData.unlockNotices &= ~2;
                StartDialogue(351);
            } else {
                /* No notice: skip the text step; rewardPack 0 = the regular pack list. */
                gMain.rewardPack = 0;
                RESET_SEQ_STATE();
                gMain.subStep++;
            }
        }
        gMain.subStep++;
        return 0;
    case REWARD_STEP_WAIT_TEXT:
        if (CB_BustupU32()) {
            gMain.subStep++;
            RESET_SEQ_STATE();
        }
        return 0;
    case REWARD_STEP_PICK_PACK:
        return CB_GetPack();
    case REWARD_STEP_TICKET:
        if (CB_BustupU32()) {
            if (gMain.rewardCard) {
                AddCardToTrunk(gMain.rewardCard);
                SaveGame();
                gMain.subStep = REWARD_STEP_SHOW_TICKET;
                return 0;
            } else {
                /* Champion: the three tickets are handed back. */
                RemoveCardFromTrunk(gUnk_08624CCE);
                RemoveCardFromTrunk(gUnk_08624CD0);
                RemoveCardFromTrunk(gUnk_08624CD2);
                IncrementChampionshipWins();
                SaveGame();
                RESET_SEQ_STATE();
                gMain.subStep++;
            }
        }
        return 0;
    case REWARD_STEP_CHAMPION_PACK:
        if (GetRewardPack(PACK_LIMITED_COLLECTION)) {
            RESET_SEQ_STATE();
            gMain.subStep = REWARD_STEP_DONE;
            return 0;
        }
        return 0;
    case REWARD_STEP_SHOW_TICKET:
        CardDetail_Init(gMain.rewardCard, 0, 0);
        gMain.subStep++;
        /* fall through */
    case REWARD_STEP_SHOW_TICKET + 1:
        return CardDetail_Run();
    case REWARD_STEP_GRANDPA_CUP_TEXT:
        if (CB_BustupU32()) {
            RESET_SEQ_STATE();
            gMain.subStep++;
        }
        return 0;
    case REWARD_STEP_GIVE_PACK:
        return GetRewardPack(gMain.rewardPack);
    case REWARD_STEP_LOSS_TEXT:
        return CB_BustupU32();
    case REWARD_STEP_SHOW_LOST_CARD:
        CardDetail_Init(gMain.rewardCard, 0, 0);
        gMain.subStep++;
        /* fall through */
    case REWARD_STEP_SHOW_LOST_CARD + 1:
        if (CardDetail_Run()) {
            RESET_SEQ_STATE();
            gMain.subStep++;
        }
        return 0;
    case REWARD_STEP_RARE_HUNTER_TEXT:
        return CB_BustupU32();
    case REWARD_STEP_DONE:
        /* Matching: written out so that the jump table has this last entry. */
        return 1;
    }
    return 1;
}

/* Campaign_ShowDuelResult sub-steps (gMain.subStep). */
enum {
    RESULT_STEP_START = 0,              /* score the match, start the result text */
    RESULT_STEP_WAIT_RESULT_TEXT = 1,   /* the duel or match is over: wait, then return 1 */
    RESULT_STEP_WAIT_MATCH_TEXT = 2,    /* open match: wait for the between-duels text */
    RESULT_STEP_SIDE_DECK = 3,          /* side-deck swap, then the next-duel text */
    RESULT_STEP_NEXT_DUEL = 4,          /* wait, then go back to CAMPAIGN_STEP_TURN_ORDER */
};

/*
 * Campaign step 5 (nothing for opponents 0, 25 and 31). In a match (DUEL_FORMAT_MATCH, best of 3) each duel adds
 * +1 (win) or -1 (loss) to gMain.matchScore: after two duels a score of +-2 decides, after three the sign
 * does (0 is a draw), and gDuel.result becomes the match result, so the rewards are for the match. A
 * decided match or a single duel shows the opponent's win/lose/draw text (Rare Hunters have their own) and
 * returns 1. An open match shows a between-duels text, runs the side-deck swap and the next-duel text, sets
 * opponentFixed and goes back 3 steps, to CAMPAIGN_STEP_TURN_ORDER.
 */
u16 Campaign_ShowDuelResult(void)
{
    s32 opp = gMain.opponent;
    u16 text;
    int matchDecided;

    switch (opp) {
    case 0:
    case 25:
    case 31:
        return 1;
    }
    text = gOpponentFirstMeetingText[opp];
    switch (gMain.subStep) {
    case RESULT_STEP_START:
        matchDecided = 0;
        if (gMain.duelFormat == DUEL_FORMAT_SINGLE) {
            matchDecided = 1;
        } else {
            switch (gDuel.result) {
            case DUEL_RESULT_WIN:
                gMain.matchScore++;
                break;
            case DUEL_RESULT_LOSE:
                gMain.matchScore--;
                break;
            case DUEL_RESULT_DRAW:
                /* Matching: the empty case keeps the ROM's compare tree. */
                break;
            }
            gMain.matchDuelCount++;
            switch (gMain.matchDuelCount) {
            case 3:
                if (gMain.matchScore < 0)
                    gDuel.result = DUEL_RESULT_LOSE;
                if (gMain.matchScore > 0)
                    gDuel.result = DUEL_RESULT_WIN;
                if (gMain.matchScore == 0)
                    gDuel.result = DUEL_RESULT_DRAW;
                matchDecided = 1;
                break;
            case 2:
                switch (gMain.matchScore) {
                case -2:
                    gDuel.result = DUEL_RESULT_LOSE;
                    matchDecided = 1;
                    break;
                case 2:
                    gDuel.result = DUEL_RESULT_WIN;
                    matchDecided = 1;
                    break;
                }
                break;
            }
        }
        if (!matchDecided) {
            switch (gDuel.result) {
            case DUEL_RESULT_WIN:
                StartDialogue(gOpponentResultTexts[opp].matchDuelWon);
                break;
            case DUEL_RESULT_LOSE:
                StartDialogue(gOpponentResultTexts[opp].matchDuelNotWon);
                break;
            case DUEL_RESULT_DRAW:
                StartDialogue(gOpponentResultTexts[opp].matchDuelNotWon);
                break;
            }
            RESET_SEQ_STATE();
            gMain.subStep++;
            gMain.subStep++;
            PlayBGM(0x15);
            return 0;
        }
        switch (gDuel.result) {
        case DUEL_RESULT_WIN:
            /* The 5th and the 10th win get their own text (wins counts the earlier ones). */
            switch (gSaveData.duelRecords[opp].wins) {
            case 4:
                text = gOpponentResultTexts[opp].win5th;
                break;
            case 9:
                text = gOpponentResultTexts[opp].win10th;
                break;
            default:
                text = gOpponentResultTexts[opp].win;
                break;
            }
            PlayBGM(0x18);
            break;
        case DUEL_RESULT_LOSE:
            text = gOpponentResultTexts[opp].lose;
            if (gMain.events == CAL_RARE_HUNTER)
                PlayBGM(0x1C);
            else
                PlayBGM(0x19);
            break;
        case DUEL_RESULT_DRAW:
            text = gOpponentResultTexts[opp].draw;
            PlayBGM(0x19);
            break;
        }
        /* Rare Hunter event: the Ghoul's own texts (X003 after a win, X000 otherwise). */
        if (gMain.events == CAL_RARE_HUNTER) {
            switch (gDuel.result) {
            case DUEL_RESULT_WIN:
                switch (opp) {
                case DUELIST_RARE_HUNTER: text = 11003; break;
                case DUELIST_ARKANA: text = 12003; break;
                case DUELIST_STRINGS: text = 13003; break;
                case DUELIST_UMBRA_LUMIS: text = 14003; break;
                case DUELIST_MARIK: text = 15003; break;
                }
                break;
            default:
                switch (opp) {
                case DUELIST_RARE_HUNTER: text = 11000; break;
                case DUELIST_ARKANA: text = 12000; break;
                case DUELIST_STRINGS: text = 13000; break;
                case DUELIST_UMBRA_LUMIS: text = 14000; break;
                case DUELIST_MARIK: text = 15000; break;
                }
                break;
            }
        }
        StartDialogue(text);
        RESET_SEQ_STATE();
        gMain.subStep++;
        /* fall through */
    case RESULT_STEP_WAIT_RESULT_TEXT:
        if (CB_BustupU32())
            return 1;
        break;
    case RESULT_STEP_WAIT_MATCH_TEXT:
        if (CB_BustupU32()) {
            RESET_SEQ_STATE();
            gMain.opponentFixed = 1;
            gMain.subStep++;
        }
        break;
    case RESULT_STEP_SIDE_DECK:
        if (SideDeckSwap_RunU32()) {
            StartDialogue(gOpponentNextMatchDuelText[opp]);
            RESET_SEQ_STATE();
            gMain.subStep++;
        }
        break;
    case RESULT_STEP_NEXT_DUEL:
        if (CB_BustupU32()) {
            gMain.seqIndexCampaign -= CAMPAIGN_STEP_RESULT - CAMPAIGN_STEP_TURN_ORDER;
            gMain.subStep = 0;
            gMain.seqState0 = 0;
            RESET_SEQ_STATE();
        }
        break;
    }
    return 0;
}
