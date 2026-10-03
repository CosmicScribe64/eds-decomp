#include "global.h"
#include "main.h"
#include "duel.h"

#define gMain gMain

/*
 * Canonical main.h splits +0x4888 as unk4888_0:4 / counter4888:2 / unk4888_6:2.
 * This unit reads bit 1 (skipScript) and bits 2-3 (unk4888_2) separately, so it keeps
 * a unit-specific view of that one byte at the same address.
 */
struct MainFlags4888 {
    u8 filler0[0x4888];
    u8 unk4888_0:1;
    u8 skipScript:1;                /* 0x4888 bit 1 */
    u8 unk4888_2:2;                 /* 0x4888 bits 2-3 */
    u8 rest4888_4:4;
};
#define gMainBits (*(struct MainFlags4888 *)&gMain)

/* Save image (0x02011C20). */
struct SaveOpponent {
    u16 unk0_0:11;
    u16 unk0_11:5;
    u16 unk2;
};

struct SaveData {
    u8 filler0[0x20D0];
    struct SaveOpponent opponents[(0x215C - 0x20D0) / 4];  /* 0x20D0, indexed by opponent */
    u16 unk215C;
    u16 unk215E;                    /* 0x215E: counter, reset by some events */
    u16 unk2160;                    /* 0x2160 */
    u8 unk2162;                     /* 0x2162 */
    u8 unk2163;
    u16 unk2164;                    /* 0x2164: bit 0/1 set after a duel (see Campaign_RecordDuelResult) */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

struct Unk02015EE8 {
    u8 phase;                       /* 0x0: duel phase index (see program-flow) */
    u8 unk1_0:1;
    u8 unk1_1:7;
};
extern struct Unk02015EE8 gDuelCtrl;

extern const u16 gPackDisplayOrder[];   /* 0x1C card IDs */

/* Per-opponent post-duel text IDs (0x10 bytes each). */
struct OpponentText {
    u16 win;        /* 0x0 */
    u16 lose;       /* 0x2 */
    u16 draw;       /* 0x4 */
    u16 unk6;       /* 0x6 */
    u16 unk8;       /* 0x8 */
    u16 winAlt4;    /* 0xA: if save state == 4 */
    u16 winAlt9;    /* 0xC: if save state == 9 */
    u16 unkE;
};
extern const struct OpponentText gOpponentResultTexts[];
extern const u16 gOpponentFirstMeetingText[];
extern const u16 gOpponentNextMatchDuelText[];

u32 OpponentSelect_Run(void);
void Campaign_StartPreDuelDialogue(void);
u32 CB_Bustup(void);

u16 Campaign_SelectOpponent(void)
{
    switch (gMain.step488A) {
    case 0:
        if (!gMainBits.skipScript) {
            if (OpponentSelect_Run()) {
                gMain.step488A++;
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
            }
            return 0;
        }
        gMain.step488A++;
    case 1:
        Campaign_StartPreDuelDialogue();
        gMain.step488A++;
    case 2:
        if (CB_Bustup()) {
            gMain.step488A++;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

u16 TurnOrder_RunRps(void);
u16 TurnOrder_RunCpuChoice(void);
u16 TurnOrder_RunPlayerChoice(void);

u16 Campaign_DecideTurnOrder(void)
{
    if (!gMain.counter4888)
        return TurnOrder_RunRps();
    else if (gDuel.result == 2)
        return TurnOrder_RunPlayerChoice();
    else
        return TurnOrder_RunCpuChoice();
}

void Duel_Setup(void);
void LoadPlayerDeckFromSave(void);
void ShuffleDeck(u32, u32);
void LoadOpponentDeck(u32);

u16 Campaign_SetupDuel(void)
{
    Duel_Setup();
    gDuelCtrl.unk1_0 = 0;
    LoadPlayerDeckFromSave();
    ShuffleDeck(0, 8);
    LoadOpponentDeck(0);
    ShuffleDeck(1, 8);
    return 1;
}

int GetCampaignLevel(void);
u32 IsPackUnlocked(u16);
void RecordDuelWin(u32);
void RecordDuelLoss(u32);
void RecordDuelDraw(u32);

void Campaign_RecordDuelResult(void)
{
    int before = GetCampaignLevel();
    u32 i;
    int countBefore, countAfter;

    for (i = 0, countBefore = 0; i <= 0x1B; i++) {
        if (IsPackUnlocked(gPackDisplayOrder[i]))
            countBefore++;
    }
    switch (gDuel.result) {
    case 1:
        RecordDuelWin(gMain.opponent);
        break;
    case 2:
        RecordDuelLoss(gMain.opponent);
        break;
    case 3:
        RecordDuelDraw(gMain.opponent);
        break;
    }
    if (GetCampaignLevel() > before)
        gSaveData.unk2164 |= 1;
    for (i = 0, countAfter = 0; i <= 0x1B; i++) {
        if (IsPackUnlocked(gPackDisplayOrder[i]))
            countAfter++;
    }
    if (countAfter > countBefore)
        gSaveData.unk2164 |= 2;
}

void StartDialogue(u16 textId);
void PlayBGM(u16 bgm);
u32 SideDeckSwap_Run(void);
extern const u16 gUnk_08624CCE;
extern const u16 gUnk_08624CD0;
extern const u16 gUnk_08624CD2;

u32 CB_GetPack(void);
u32 GetRewardPack(u16 pack);
u16 PickRandomOwnedRareCard(void);
void RemoveCardFromTrunk(u16 card);
void SaveGame(void);
void AddCardToTrunk(u16 card);
void IncrementChampionshipWins(void);
void CardDetail_Init(u16 card, u16 b, u16 c);
u16 CardDetail_Run(void);

#define RESET_SEQ()             \
    do {                        \
        gMain.seqIndex1 = 0;    \
        gMain.seqState1 = 0;    \
        gMain.seqState2 = 0;    \
    } while (0)

/* Campaign step after the post-duel text: rewards (cards / booster packs). */
u16 Campaign_GiveRewards(void)
{
    switch (gMain.step488A) {
    case 0:
        Campaign_RecordDuelResult();
        gMain.step488A = 1;
        return 0;
    case 1:
        switch (gDuel.result) {
        case 1:
            RESET_SEQ();
            switch (gMain.events) {
            case 0x1000000:
                StartDialogue(0xC8);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CD0;
                gMain.step488A = 0xA;
                return 0;
            case 0x2000000:
                StartDialogue(0xCA);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CCE;
                gMain.step488A = 0xA;
                return 0;
            case 0x4000000:
                StartDialogue(0xCC);
                gSaveData.unk215E++;
                gMain.rewardCard = gUnk_08624CD2;
                gMain.step488A = 0xA;
                return 0;
            case 0x8000000:
                StartDialogue(0xCE);
                gSaveData.unk215E = 0;
                gSaveData.unk2162++;
                gMain.rewardCard = 0;
                gMain.step488A = 0xA;
                return 0;
            case 0x10000000:
                gSaveData.unk2160 = 1;
                gMain.rewardPack = 0x1F8;
                gMain.step488A = 0xF;
                return 0;
            case 0x20000000:
                StartDialogue(0x2BF);
                gMain.rewardPack = 0x386;
                gMain.step488A = 0xE;
                return 0;
            case 0x800000:
                gMain.rewardPack = 0x1FA;
                gMain.step488A = 0xF;
                return 0;
            case 1:
            case 4:
            case 0x400:
            case 0x8000:
                gMain.rewardPack = 0x1FD;
                gMain.step488A = 0xF;
                return 0;
            case 2:
                gMain.rewardPack = 5;
                gMain.step488A = 0xF;
                return 0;
            case 8:
                gMain.rewardPack = 0x15;
                gMain.step488A = 0xF;
                return 0;
            case 0x10:
                gMain.rewardPack = 0xB;
                gMain.step488A = 0xF;
                return 0;
            case 0x20:
            case 0x200:
            case 0x1000:
                gMain.rewardPack = 0x1F9;
                gMain.step488A = 0xF;
                return 0;
            case 0x40:
                gMain.rewardPack = 0xC;
                gMain.step488A = 0xF;
                return 0;
            case 0x80:
                gMain.rewardPack = 0x16;
                gMain.step488A = 0xF;
                return 0;
            case 0x100:
                gMain.rewardPack = 0x17;
                gMain.step488A = 0xF;
                return 0;
            case 0x800:
                gMain.rewardPack = 4;
                gMain.step488A = 0xF;
                return 0;
            case 0x2000:
            case 0x4000:
            case 0x10000:
            case 0x20000:
                gMain.rewardPack = 0x1FA;
                gMain.step488A = 0xF;
                return 0;
            case 0x40000:
            case 0x80000:
                gMain.rewardPack = 0x1F8;
                gMain.step488A = 0xF;
                return 0;
            }
            gMain.step488A++;
            return 0;
        case 2:
            RESET_SEQ();
            switch (gMain.events) {
            case 0x800000:
                switch (gMain.opponent) {
                case 11: StartDialogue(0x2AF9); break;
                case 12: StartDialogue(0x2EE1); break;
                case 13: StartDialogue(0x32C9); break;
                case 14: StartDialogue(0x36B1); break;
                case 15: StartDialogue(0x3A99); break;
                }
                gMain.step488A = 0x16;
                gMain.rewardCard = PickRandomOwnedRareCard();
                RemoveCardFromTrunk(gMain.rewardCard);
                SaveGame();
                return 0;
            case 0x1000000:
            case 0x2000000:
            case 0x4000000:
            case 0x8000000:
                gSaveData.unk215E = 0;
            case 0x400000:
            case 0x10000000:
            case 0x20000000:
                StartDialogue(0x12C);
                gMain.step488A = 0x14;
                return 0;
            }
            break;
        }
        return 1;
    case 2:
        {
            u16 flag = gSaveData.unk2164 & 2;
            if (flag) {
                gSaveData.unk2164 &= ~2;
                StartDialogue(0x15F);
            } else {
                gMain.rewardPack = flag;
                RESET_SEQ();
                gMain.step488A++;
            }
        }
        gMain.step488A++;
        return 0;
    case 3:
        if (CB_Bustup()) {
            gMain.step488A++;
            RESET_SEQ();
        }
        return 0;
    case 4:
        return CB_GetPack();
    case 0xA:
        if (CB_Bustup()) {
            if (gMain.rewardCard) {
                AddCardToTrunk(gMain.rewardCard);
                SaveGame();
                gMain.step488A = 0xC;
                return 0;
            } else {
                RemoveCardFromTrunk(gUnk_08624CCE);
                RemoveCardFromTrunk(gUnk_08624CD0);
                RemoveCardFromTrunk(gUnk_08624CD2);
                IncrementChampionshipWins();
                SaveGame();
                RESET_SEQ();
                gMain.step488A++;
            }
        }
        return 0;
    case 0xB:
        if (GetRewardPack(0x1FD)) {
            RESET_SEQ();
            gMain.step488A = 0x19;
            return 0;
        }
        return 0;
    case 0xC:
        CardDetail_Init(gMain.rewardCard, 0, 0);
        gMain.step488A++;
    case 0xD:
        return CardDetail_Run();
    case 0xE:
        if (CB_Bustup()) {
            RESET_SEQ();
            gMain.step488A++;
        }
        return 0;
    case 0xF:
        return GetRewardPack(gMain.rewardPack);
    case 0x14:
        return CB_Bustup();
    case 0x16:
        CardDetail_Init(gMain.rewardCard, 0, 0);
        gMain.step488A++;
    case 0x17:
        if (CardDetail_Run()) {
            RESET_SEQ();
            gMain.step488A++;
        }
        return 0;
    case 0x18:
        return CB_Bustup();
    case 0x19:
        return 1;
    }
    return 1;
}


u16 Campaign_ShowDuelResult(void)
{
    s32 opp = gMain.opponent;
    u16 text;
    int done;

    switch (opp) {
    case 0:
    case 0x19:
    case 0x1F:
        return 1;
    }
    text = gOpponentFirstMeetingText[opp];
    switch (gMain.step488A) {
    case 0:
        done = 0;
        if (gMainBits.unk4888_2 == 1) {
            done = 1;
        } else {
            switch (gDuel.result) {
            case 1:
                gMain.score++;
                break;
            case 2:
                gMain.score--;
                break;
            case 3:
                break;
            }
            gMain.counter4888++;
            switch (gMain.counter4888) {
            case 3:
                if (gMain.score < 0)
                    gDuel.result = 2;
                if (gMain.score > 0)
                    gDuel.result = 1;
                if (gMain.score == 0)
                    gDuel.result = 3;
                done = 1;
                break;
            case 2:
                switch (gMain.score) {
                case -2:
                    gDuel.result = 2;
                    done = 1;
                    break;
                case 2:
                    gDuel.result = 1;
                    done = 1;
                    break;
                }
                break;
            }
        }
        if (!done) {
            switch (gDuel.result) {
            case 1:
                StartDialogue(gOpponentResultTexts[opp].unk6);
                break;
            case 2:
                StartDialogue(gOpponentResultTexts[opp].unk8);
                break;
            case 3:
                StartDialogue(gOpponentResultTexts[opp].unk8);
                break;
            }
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
            gMain.step488A++;
            PlayBGM(0x15);
            return 0;
        }
    show:
        switch (gDuel.result) {
        case 1:
            switch (gSaveData.opponents[opp].unk0_0) {
            case 4:
                text = gOpponentResultTexts[opp].winAlt4;
                break;
            case 9:
                text = gOpponentResultTexts[opp].winAlt9;
                break;
            default:
                text = gOpponentResultTexts[opp].win;
                break;
            }
            PlayBGM(0x18);
            break;
        case 2:
            text = gOpponentResultTexts[opp].lose;
            if (gMain.events == 0x800000)
                PlayBGM(0x1C);
            else
                PlayBGM(0x19);
            break;
        case 3:
            text = gOpponentResultTexts[opp].draw;
            PlayBGM(0x19);
            break;
        }
        if (gMain.events == 0x800000) {
            switch (gDuel.result) {
            case 1:
                switch (opp) {
                case 11: text = 0x2AFB; break;
                case 12: text = 0x2EE3; break;
                case 13: text = 0x32CB; break;
                case 14: text = 0x36B3; break;
                case 15: text = 0x3A9B; break;
                }
                break;
            default:
                switch (opp) {
                case 11: text = 0x2AF8; break;
                case 12: text = 0x2EE0; break;
                case 13: text = 0x32C8; break;
                case 14: text = 0x36B0; break;
                case 15: text = 0x3A98; break;
                }
                break;
            }
        }
        StartDialogue(text);
        gMain.seqIndex1 = 0;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
        gMain.step488A++;
    case 1:
        if (CB_Bustup())
            return 1;
        break;
    case 2:
        if (CB_Bustup()) {
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMainBits.skipScript = 1;
            gMain.step488A++;
        }
        break;
    case 3:
        if (SideDeckSwap_Run()) {
            StartDialogue(gOpponentNextMatchDuelText[opp]);
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
        }
        break;
    case 4:
        if (CB_Bustup()) {
            gMain.seqIndexCampaign -= 3;
            gMain.step488A = 0;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    }
    return 0;
}

