#include "global.h"
#include "gba.h"
#include "main.h"       /* struct Main */
#include "duel_ui.h"    /* struct DuelCmd/DuelScreen (pulls in duel.h) */

/* gMain (0x03000040): canonical layout in main.h. */
#define gMain gMain

typedef u16 (*StepFunc)(void);

/* Message box request block at 0x02017A30. */
struct DuelMsg {
    StepFunc func;                  /* 0x0: handler from gDuelSceneHandlers[id] */
    u16 id:15;                      /* 0x4 bits 0-14 */
    u16 flag:1;                     /* 0x4 bit 15 */
    u16 arg;                        /* 0x6 */
    u8 filler8[2];
    u8 stepIndex;                   /* 0xA: index into gDuelSceneRunnerSteps */
    u8 state;                       /* 0xB */
    u8 unkC;                        /* 0xC */
    u8 unkD;                        /* 0xD */
};
extern struct DuelMsg gDuelScene;
#define gMsg gDuelScene

/* Duel command queue at 0x020185C0: canonical layout in duel_ui.h. */

/* Local view: canonical struct DuelCard puts the owner bit at +0x1 bit 4 and
   flag20 at bit 20; CardMenu_Execute reads the zone word's flag at bit 18 instead,
   so keep a unit-specific view. */
struct ZoneWord {
    u32 cardId:12;
    u32 unk0_12:6;
    u32 flag0_18:1;                 /* bit 18 */
    u32 unk0_19:13;
};

/* Card command menu at 0x020192E0+0x1B2C (see code_0801CE68). Canonical duel.h
   stops at DuelState.phaseStep +0x1B20, so this is a unit-specific view used only
   by CardMenu_Execute. */
struct SelMask {
    u16 flag0:1;
    u16 active:1;
    u16 cursor:4;
    u16 rows:4;
    u32 mask:16;
    u32 state:8;
    u32 subState:8;                 /* bits 34-41: step counter of the chosen command */
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;                   /* bit 57 */
    u32 zone:7;                     /* bits 58-64 */
    u32 column:8;                   /* bits 65-72 */
    u32 unk73:23;
};

/* Same bits as struct SelMask with a u16 column. CardMenu_Execute's case 7 reads the
   column through it so the atkSlot bit-field store gets a HImode value and regmove
   ties the AND to the constant (`movs r1, #7; ands r1, r0`); FAKEMATCH: the u32
   column gives `ands r1, r0` with the column as destination. */
struct SelMaskCol {
    u16 flag0:1;
    u16 active:1;
    u16 cursor:4;
    u16 rows:4;
    u32 mask:16;
    u32 state:8;
    u32 subState:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;
    u32 zone:7;
    u16 column:8;                   /* bits 65-72 */
    u32 unk73:23;
};

/* Unit-specific view: canonical struct DuelPlayer names +0x08/+0x09 unk8/unk9,
   but this unit's draft reads flags at +0x08 bit 4 and +0x09 bit 5. */
struct DuelPlayerView {
    u8 unk0[8];
    u8 unk8_0:4;
    u8 flag8_4:1;                   /* 0x08 bit 4 */
    u8 unk8_5:3;
    u8 unk9_0:5;
    u8 flag9_5:1;                   /* 0x09 bit 5 */
    u8 unk9_6:2;
    u8 unkA[0x28 - 0xA];
    struct DuelZone zones[11];      /* 0x28 */
    u8 filler684[0xD64 - 0x684];
};

/* Unit-specific view: canonical struct DuelState has no +0x1B16 bitfield, no
   selCard +0x1B28 / sel +0x1B2C, names +0x1B12 bits 2..4 phase1B12 (here
   unk1B12_2) and ends at +0x1B20 phaseStep (here unk1B20), with no +0x1B21. */
struct DuelStateView {
    u32 unk0;
    struct DuelPlayerView players[2];   /* 0x004 */
    u8 filler1ACC[0x1B12 - 0x1ACC];
    u8 flag1B12_0:1;                /* 0x1B12 bit 0 */
    u8 unk1B12_1:1;
    u32 unk1B12_2:3;                /* 0x1B12 bits 2-4 (u32: see CardMenu_Execute) */
    u8 unk1B12_5:1;
    u8 result:2;                    /* 0x1B12 bits 6-7 */
    u8 filler1B13[0x1B16 - 0x1B13];
    u16 unk1B16_0:1;
    u16 unk1B16_1:8;                /* 0x1B16 bits 1-8 */
    u16 unk1B16_9:7;
    u8 filler1B18[0x1B20 - 0x1B18];
    u8 unk1B20;                     /* 0x1B20 (canonical phaseStep) */
    u8 unk1B21;                     /* 0x1B21 (not in canonical struct) */
    u8 filler1B22[0x1B28 - 0x1B22];
    u16 selCard;                    /* 0x1B28 */
    u16 unk1B2A;
    union {
        struct SelMask x;
        struct SelMaskCol col;
    } sel;                          /* 0x1B2C */
};
extern struct DuelStateView gUnk_020192E0View asm("gDuel");
#define SEL gUnk_020192E0View.sel.x

struct Battle {
    u16 unk0_0:6;
    u16 atkSlot:3;                  /* bits 6-8 */
    u16 unk0_9:7;
};
extern struct Battle gBattle;

struct Unk0201AE60 {
    u8 filler0[0x14];
    u16 unk14;
};
extern struct Unk0201AE60 gTextBox;

extern const u32 gCardStats[];   /* card stats */
extern const u16 gCardIdToNumber[];   /* maps card ID to card number */
extern const u16 gUnk_0862467A;
extern const u8 gStrDoYouSurrender[];
#define CARD_TYPE(id) ((*(gCardStats + ((id) & 0x7FF)) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (*(gCardIdToNumber + ((id) & 0x7FF)))

void CardMenu_ChangePosition(u32 cmd);
void CardMenu_FlipSummon(void);
void CardMenu_SummonMonster(u32 a, u32 b);
void CardMenu_PlaySpellTrapFromHand(u32 a, u32 b, u32 c);
void CardMenu_FusionSummon(void);
void DiscardHandCard(int player, int idx, int a, int b);
void Chain_AddPending(u32 card, u32 b);
void ShowCardEffect(int player, u16 id);
void LoseLifePoints(int player, int lp);
int TributeMonster(int player, int column);
u16 TakeDeckCardByNumber(int player, u16 cardNo, void *out);
void QueueSpecialSummonChoosePosition(int player, void *card, u32 a, u32 b);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
void DuelCmd_Push(u16 cmd, u16 arg2, int arg4Word, int arg6Word);

/* Duel screen state (0x0201CFB0): canonical layout in duel_ui.h. */

struct Unk02015EE8 {
    u8 phase;
    u8 link:1;                      /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct Unk02015EE8 gDuelCtrl;

/* Step runner state at 0x02015EF0. */
struct Unk02015EF0 {
    u8 index;
    u8 state;
};
extern struct Unk02015EF0 gAiState;

struct OpponentBgm {
    u16 opponent;
    u16 bgm;
};

extern StepFunc gAiTurnPhases[];
extern StepFunc gDuelSceneHandlers[];    /* message handlers by id */
extern StepFunc gDuelSceneRunnerSteps[];
extern const struct OpponentBgm gOpponentDuelBGM[0x18];

void StopBGM(void);
void ResetBgScroll(void);            /* ResetBgScroll */
u32 DuelScreen_FadeOutStep(void);
void DuelScreen_Init(void);
u16 FadeFromBlack(u16 speed);        /* FadeFromBlack */
void FadeOutBGM(void);
void PlayBGM(u16 bgm);

/* Execute the command chosen in the card command menu (SEL.cursor = 1..12). */
/* Zones and players are reached through casts so the field offsets (+6, +8/+9)
   stay in the ldrb/strb as in the ROM instead of folding into the base constant. */
struct ZoneFlags1E260 {
    u32 card;
    u16 serial;
    u8 flag6_0:1;
    u8 flag6_1:1;                   /* +0x06 bit 1 */
    u8 unk6_2:6;
};

struct PlayerFlags1E260 {
    u8 unk0[8];
    u8 unk8_0:4;
    u8 flag8_4:1;                   /* 0x08 bit 4 */
    u8 unk8_5:3;
    u8 unk9_0:5;
    u8 flag9_5:1;                   /* 0x09 bit 5 */
    u8 unk9_6:2;
};

#define ZONE_FLAGS(p, i) ((struct ZoneFlags1E260 *)&gUnk_020192E0View.players[p].zones[i])
#define ZONE_WORD(p, i) ((struct ZoneWord *)&gUnk_020192E0View.players[p].zones[i])
#define PLAYER_FLAGS(pl, p) ((struct PlayerFlags1E260 *)&(pl)[p])

/* Execute the command chosen in the card command menu (SEL.cursor = 1..12).
   FAKEMATCH notes: `ev = (zone << 16 | kind)` inside the Chain_AddPending packings stops
   fold from floating the kind constant out of the OR chain; per-site `pl` locals keep
   the constant-1 pseudo from inheriting an r4 preference; `unk1B12_2 > 1u` on the u32
   view (struct DuelStateView) drops two pre-combine extension insns so the reloaded base wins r5 over
   &column; TakeDeckCardByNumber is called as int-returning (the ROM tests r0 unextended). */
void CardMenu_Execute(void)
{
    u8 buf[4];
    s16 zone;
    u32 ev;
    int idx;
    int p;

    switch (SEL.cursor) {
    case 1:
    case 2:
        if (SEL.zone != 0)
            break;
        CardMenu_ChangePosition(SEL.cursor);
        return;
    case 3:
        if (SEL.zone != 0)
            break;
        CardMenu_FlipSummon();
        return;
    case 4:
        if (SEL.zone != 11)
            break;
        if (((((const u32 *)0x08621DE0)[gUnk_020192E0View.selCard & 0x7FF] & 0x1F00000) >> 20) <= 20)
            CardMenu_SummonMonster(0, 0);
        else
            CardMenu_PlaySpellTrapFromHand(0, 0, 0);
        return;
    case 5:
        CardMenu_SummonMonster(1, 0);
        return;
    case 11:
        CardMenu_SummonMonster(1, 1);
        return;
    case 12:
        CardMenu_SummonMonster(0, 1);
        return;
    case 10:
        CardMenu_FusionSummon();
        return;
    case 6:
        zone = SEL.zone;
        switch (zone) {
        case 11:
            if (CARD_TYPE(gUnk_020192E0View.selCard) > 20) {
                CardMenu_PlaySpellTrapFromHand(1, 0, 0);
                return;
            }
            switch (((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF]) {
            case 0x47:
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag8_4 = 1;
                }
                CardMenu_PlaySpellTrapFromHand(1, 0, 0);
                return;
            case 0x1A8:
                DiscardHandCard(SEL.player, SEL.column, 0, 1);
                Chain_AddPending(((SEL.player & 1) << 31) | (ev = 0x600000 | gUnk_020192E0View.selCard), 0);
                break;
            }
            break;
        case 10:
            if (!ZONE_FLAGS(SEL.player & 1, 10)->flag6_1)
                DuelCmd_Push(SEL.player ? 0x807F : 0x7F, 10, 0, 0);
            Chain_AddPending(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x200000) | gUnk_020192E0View.selCard, 0);
            if (gUnk_020192E0View.unk1B12_2 > 1u) {
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag9_5 = 1;
                }
            }
            break;
        case 5:
            p = SEL.player & 1;
            idx = SEL.column;
            idx += 5;
            if (!ZONE_FLAGS(p, idx)->flag6_1)
                DuelCmd_Push(SEL.player ? 0x807F : 0x7F, SEL.column + SEL.zone, 0, 0);
            if (ZONE_WORD(SEL.player & 1, SEL.column)->flag0_18) {
                ShowCardEffect(SEL.player, gUnk_0862467A);
                LoseLifePoints(SEL.player, 2000);
            }
            Chain_AddPending(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x200000) | gUnk_020192E0View.selCard, 0);
            if (gUnk_020192E0View.unk1B12_2 > 1u) {
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag9_5 = 1;
                }
            }
            break;
        case 0:
            switch (((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF]) {
            case 0x51:
            case 0x186:
                switch (SEL.subState) {
                case 0:
                    if (TributeMonster(SEL.player, SEL.column)) {
                        SEL.subState++;
                        return;
                    }
                    SEL.active = 0;
                    SEL.subState = 0;
                    return;
                case 1:
                    if (((int (*)(int, u16, void *))TakeDeckCardByNumber)(SEL.player, ((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF] == 0x51 ? 0x2E5 : 0x187, buf)) {
                        QueueSpecialSummonChoosePosition(SEL.player, buf, 1, 1);
                        SEL.subState++;
                        return;
                    }
                }
                SEL.active = 0;
                SEL.subState = 0;
                return;
            case 0x1A0:
            case 0x243:
            case 0x2DB:
                Chain_AddPending(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x4400000) | gUnk_020192E0View.selCard, 0);
                break;
            default:
                Chain_AddPending(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x400000) | gUnk_020192E0View.selCard, 0);
                break;
            }
            break;
        }
        break;
    case 7:
        gBattle.atkSlot = gUnk_020192E0View.sel.col.column;
        gUnk_020192E0View.unk1B16_1 = 2;
        break;
    case 8:
        gUnk_020192E0View.unk1B20++;
        SEL.active = 0;
        SEL.subState = 0;
        return;
    case 9:
        switch (SEL.subState) {
        case 0:
            TextBoxOpen(0x206, 0x412, 0xB, gStrDoYouSurrender);
            TextBoxSetMenu(1, 0, 0);
            SEL.subState++;
            break;
        case 1:
            if (gTextBox.unk14)
                DuelCmd_Push(0x40, 1, 0, 0);
            SEL.active = 0;
            SEL.subState = 0;
            break;
        }
        return;
    }
    SEL.active = 0;
    SEL.subState = 0;
}

u16 AiRunTurn(void)
{
    StepFunc step = gAiTurnPhases[gAiState.index];
    if (step != NULL) {
        if (step()) {
            gUnk_020192E0View.unk1B20 = 0;
            gUnk_020192E0View.unk1B21 = 0;
            gAiState.state = 0;
            gAiState.index++;
        }
        return 0;
    }
    return 1;
}

/* Start message `id` (handler gDuelSceneHandlers[id]). */
void DuelScene_Start(u16 id, u32 flag)
{
    gMsg.stepIndex = 0;
    gMsg.state = 0;
    gMsg.unkC = 0;
    gMsg.unkD = 0;
    gMsg.flag = flag;
    gMsg.id = id;
    gMsg.func = gDuelSceneHandlers[id];
    switch (gMsg.id) {
    case 1:
    case 5:
        StopBGM();
        gDuel.flag1B12_0 = 0;
        break;
    }
    gDuelScreen.flag0_1 = 0;
}

u16 DuelScene_FadeOutDuelScreen(void)
{
    if (gMsg.state == 0) {
        gMain.vblankCallback = NULL;
        ResetBgScroll();
        gDuelScreen.busy = 0;
        gDuelScreen.flag0_1 = 0;
        gDuelScreen.flag0_2 = 0;
        gMsg.state++;
    }
    return DuelScreen_FadeOutStep();
}

u16 DuelScene_RunHandler(void)
{
    StepFunc func = gMsg.func;
    if (func != NULL)
        return func();
    return 0;}

u16 DuelScene_FadeInDuelScreen(void)
{
    struct DuelMsg *msg = &gMsg;

    switch (msg->state) {
    case 0:
        REG_DISPCNT = 0;
        break;
    case 1:
        DuelScreen_Init();
        break;
    default:
        return FadeFromBlack(4);
    }
    msg->state++;
    return 0;
}

u16 DuelScene_Run(void)
{
    StepFunc step = gDuelSceneRunnerSteps[gMsg.stepIndex];
    if (step != NULL) {
        if (step()) {
            gMsg.stepIndex++;
            gMsg.state = 0;
            gMsg.unkC = 0;
            gMsg.unkD = 0;
        }
        return 0;
    }
    gMsg.stepIndex = 0;
    return 1;
}

/* HBlank: wavy BG0/BG1/BG3 HOFS from the per-line table. */
void HBlank_WaveBg013(void)
{
    u16 hofs = gDuelCmd.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];
    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* HBlank: same for all four BGs. */
void HBlank_WaveAllBgs(void)
{
    u16 hofs = gDuelCmd.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];
    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG2HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* Start the duel BGM for the current opponent (or event / link BGM). */
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
    } while (++i <= 0x17);
    if (gMain.events & 0xF000000)
        bgm = 0x13;
    if (gDuelCtrl.link)
        bgm = 5;
    if (!gDuel.flag1B12_0)
        bgm = 0xFFFF;
    if (bgm == 0xFFFF)
        FadeOutBGM();
    else
        PlayBGM(bgm);
}

/* Append a command to the duel command queue (max 256).
 * The original r2/r3 entry shifts explicitly decode both final words as u16. */
void DuelCmd_Push(u16 cmd, u16 arg2, int arg4Word, int arg6Word)
{
    u16 arg4 = arg4Word;
    u16 arg6 = arg6Word;
    if (gDuelCmd.queueCount < 0x100) {
        gDuelCmd.queue[gDuelCmd.queueCount].cmd = cmd;
        gDuelCmd.queue[gDuelCmd.queueCount].arg2 = arg2;
        gDuelCmd.queue[gDuelCmd.queueCount].arg4 = arg4;
        gDuelCmd.queue[gDuelCmd.queueCount].arg6 = arg6;
        gDuelCmd.queueCount++;
    }
}

void DuelCmd_Attack(void);
void DuelCmd_DirectAttack(void);
void DuelCmd_PrepareBattlePhase(void);
void DuelCmd_MarkAttacked(void);
void DuelCmd_StartBattleScene(void);
void DuelCmd_PlayBattleScene(void);
void DuelCmd_SetBattleProtection(void);
void DuelCmd_EndBattlePhase(void);
void DuelCmd_SetAttackTarget(void);
void DuelCmd_SetAttacker(void);
void DuelCmd_ZeroAttackerAtk(void);
void DuelCmd_NegateAttack(void);
void DuelCmd_PlaceCard(void);
void DuelCmd_ClearZoneCard(void);
void DuelCmd_AddCardToGraveyard(void);
void DuelCmd_AddCardToBanished(void);
void DuelCmd_ChangePosition(void);
void DuelCmd_FlipCard(void);
void DuelCmd_SendToGraveyard(void);
void DuelCmd_Banish(void);
void DuelCmd_BanishFlagged(void);
void DuelCmd_ReturnToHand(void);
void DuelCmd_ReturnToDeck(void);
void DuelCmd_MoveToZone(void);
void DuelCmd_SwapZones(void);
void DuelCmd_AddEquipLink(void);
void DuelCmd_AddZoneLink(void);
void DuelCmd_RemoveZoneLink(void);
void DuelCmd_SetZoneDeclaredValue(void);
void DuelCmd_SetDestroyedByOpponentFlag(void);
void DuelCmd_AddZoneTurnCounter(void);
void DuelCmd_SetZoneTurnCounter(void);
void DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue(void);
void DuelCmd_ClearZoneLinks(void);
void DuelCmd_MoveZoneLinks(void);
void DuelCmd_AddProhibition(void);
void DuelCmd_RemoveProhibition(void);
void DuelCmd_ShuffleDeck(void);
void DuelCmd_DrawCards(void);
void DuelCmd_SendTopDeckCardsToGraveyard(void);
void DuelCmd_BanishTopDeckCards(void);
void DuelCmd_AddDeckCardToHand(void);
void DuelCmd_RemoveCardFromDeck(void);
void DuelCmd_SummonFromDeck(void);
void DuelCmd_SendDeckCardToGraveyard(void);
void DuelCmd_BanishDeckCard(void);
void DuelCmd_SendFusionDeckCardToGraveyard(void);
void DuelCmd_AddCardToDeckTop(void);
void DuelCmd_AddCardToDeckBottom(void);
void DuelCmd_SetCrushCardTurns(void);
void DuelCmd_RemoveCardFromFusionDeck(void);
void DuelCmd_ReturnGraveyardCardToHand(void);
void DuelCmd_ReturnGraveyardCardToDeckTop(void);
void DuelCmd_ReturnGraveyardCardToDeckBottom(void);
void DuelCmd_BanishGraveyardCard(void);
void DuelCmd_RemoveCardFromGraveyard(void);
void DuelCmd_ReturnGraveyardToDeck(void);
void DuelCmd_TakeOpponentGraveyardCard(void);
void DuelCmd_ReturnBanishedCardToGraveyard(void);
void DuelCmd_AddCardToGraveyardNoRedraw(void);
void DuelCmd_ClearPendingEquip(void);
void DuelCmd_EquipGraveyardCardToOpponent(void);
void sub_080106BC(void);
void sub_08010708(void);
void DuelCmd_SendHandCardToGraveyard(void);
void DuelCmd_BanishHandCard(void);
void DuelCmd_ReturnHandCardToDeck(void);
void DuelCmd_RemoveCardFromHand(void);
void DuelCmd_PlaceMonsterFromHand(void);
void DuelCmd_PlaceSpellTrapFromHand(void);
void DuelCmd_CompactHand(void);
void DuelCmd_AddCardToHand(void);
void DuelCmd_BanishHandCardFaceDown(void);
void DuelCmd_ReturnBanishedCardToHand(void);
void DuelCmd_ExchangeHandCards(void);
void DuelCmd_SendHandFusionMaterialToGraveyard(void);
void DuelCmd_BanishHandFusionMaterial(void);
void DuelCmd_NegateActivation(void);
void DuelCmd_NopB2(void);
void DuelCmd_SetSpellTrapDisabled(void);
void DuelCmd_UpdateZoneLpPaid(void);
void DuelCmd_IncrementZoneTurnCounter(void);
void DuelCmd_SetZoneStatusFlags(void);
void DuelCmd_ClearZoneStatusFlags(void);
void DuelCmd_SetEffectUnused(void);
void DuelCmd_TributeMonster(void);
void DuelCmd_PlantInOpponentDeck(void);
void DuelCmd_SetDestroyCountdown(void);
void DuelCmd_Nop99(void);
void DuelCmd_Nop9A(void);
void DuelCmd_Nop9B(void);
void DuelCmd_Nop9C(void);
void DuelCmd_Nop9D(void);
void DuelCmd_Nop9E(void);
void DuelCmd_HalveAttack(void);
void DuelCmd_SetCannotAttackNextTurn(void);
void DuelCmd_SetCannotAttack(void);
void DuelCmd_Nop9F(void);
void DuelCmd_SetPositionLocked(void);
void DuelCmd_SetReturnAfterBattle(void);
void DuelCmd_SummonToken(void);
void DuelCmd_SetZoneCardWord(void);
void DuelCmd_MoveMonsterFaceDown(void);
void DuelCmd_SetMagicalHatsCard(void);
void DuelCmd_BanishMonsterUntilEndPhase(void);
void DuelCmd_ReturnBanishedMonster(void);
void DuelCmd_SendFusionMaterialToGrave(void);
void sub_08013104(void);
void DuelCmd_ClearZoneLinks2(void);
void DuelCmd_ShowDuelResult(void);
void DuelCmd_Surrender(void);
void DuelCmd_ShowJustAMomentBanner(void);
void DuelCmd_SetNegationFlag(void);
void DuelCmd_TurnStart(void);
void DuelCmd_TurnEnd(void);
void DuelCmd_ShowEndTurnHand(void);
void DuelCmd_SkipNextDrawPhase(void);
void DuelCmd_SkipNextStandbyPhase(void);
void DuelCmd_SkipNextTurn(void);
void DuelCmd_SetExtraBattlePhase(void);
void DuelCmd_SetPositionChangeLock(void);
void DuelCmd_SetSummonLocks(void);
void DuelCmd_SetMagicTrapLockTurns(void);
void DuelCmd_SetStatChangesReversed(void);
void DuelCmd_SetAtkDefSwapped(void);
void DuelCmd_AdjustDelayedSummonCount(void);
void sub_08014B5C(void);
void DuelCmd_ShowCardDetail(void);
void DuelCmd_ShowCardAssemble(void);
void DuelCmd_ShowCardZoomIn(void);
void DuelCmd_ShowCardEffect(void);
void DuelCmd_ShowCardScatter(void);
void DuelCmd_ShowCardUnrollDown(void);
void DuelCmd_ShowCardUnrollSideways(void);
void DuelCmd_ResetDuelState(void);
void DuelCmd_OpenDuelScreen(void);
void DuelCmd_CloseDuelScreen(void);
void DuelCmd_SetFieldBackground(void);
void DuelCmd_EnterBattlePhase(void);
void DuelCmd_ShowChainBanner(void);
void DuelCmd_PointAtCard(void);
void DuelCmd_MoveCursor(void);
void DuelCmd_StartDuelBanner(void);
void DuelCmd_ExodiaWinScene(void);
void DuelCmd_DestinyBoardWinScene(void);
void DuelCmd_TossCoin(void);
void DuelCmd_TossThreeCoins(void);
void DuelCmd_RollGracefulDice(void);
void DuelCmd_RollSkullDice(void);
void DuelCmd_RollPlainDie(void);
void DuelCmd_ChangeLifePoints(u32);
void DuelCmd_EnterPhase(u32);

/* Run the current duel command (id = bits 0-11 of gDuelCmd.cmd). */
void DuelCmd_Dispatch(void)
{
    gDuelScreen.busy = 0;
    switch (gDuelCmd.cmd & 0xFFF) {
    case 0x01:
        DuelCmd_TurnStart();
        break;
    case 0x02:
        DuelCmd_TurnEnd();
        break;
    case 0x03:
        DuelCmd_ShowEndTurnHand();
        break;
    case 0x04:
        DuelCmd_ShowDuelResult();
        break;
    case 0x05:
        DuelCmd_ExodiaWinScene();
        break;
    case 0x06:
        DuelCmd_DestinyBoardWinScene();
        break;
    case 0x07:
        DuelCmd_ShowChainBanner();
        break;
    case 0x08:
        DuelCmd_PointAtCard();
        break;
    case 0x09:
        DuelCmd_MoveCursor();
        break;
    case 0x10:
        DuelCmd_ResetDuelState();
        break;
    case 0x11:
        DuelCmd_SetFieldBackground();
        break;
    case 0x12:
        DuelCmd_OpenDuelScreen();
        break;
    case 0x13:
        DuelCmd_CloseDuelScreen();
        break;
    case 0x14:
        DuelCmd_StartDuelBanner();
        break;
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
        DuelCmd_SetNegationFlag();
        break;
    case 0x1C:
        DuelCmd_SetStatChangesReversed();
        break;
    case 0x1D:
        DuelCmd_SetAtkDefSwapped();
        break;
    case 0x30:
        DuelCmd_StartBattleScene();
        break;
    case 0x31:
        DuelCmd_PlayBattleScene();
        break;
    case 0x32:
        DuelCmd_PrepareBattlePhase();
        break;
    case 0x33:
        DuelCmd_Attack();
        break;
    case 0x34:
        DuelCmd_DirectAttack();
        break;
    case 0x35:
        DuelCmd_MarkAttacked();
        break;
    case 0x36:
        DuelCmd_SetBattleProtection();
        break;
    case 0x37:
        DuelCmd_EndBattlePhase();
        break;
    case 0x38:
        DuelCmd_SetAttackTarget();
        break;
    case 0x39:
        DuelCmd_SetAttacker();
        break;
    case 0x3A:
        DuelCmd_ZeroAttackerAtk();
        break;
    case 0x3B:
        DuelCmd_NegateAttack();
        break;
    case 0x40:
        DuelCmd_Surrender();
        break;
    case 0x41:
        DuelCmd_ShowJustAMomentBanner();
        break;
    case 0x42:
        DuelCmd_ChangeLifePoints(1);
        break;
    case 0x43:
        DuelCmd_ChangeLifePoints(0);
        break;
    case 0x44:
        DuelCmd_SkipNextDrawPhase();
        break;
    case 0x45:
        DuelCmd_SkipNextStandbyPhase();
        break;
    case 0x46:
        DuelCmd_SkipNextTurn();
        break;
    case 0x47:
        DuelCmd_SetExtraBattlePhase();
        break;
    case 0x48:
        DuelCmd_SetPositionChangeLock();
        break;
    case 0x49:
        DuelCmd_SetSummonLocks();
        break;
    case 0x4A:
        DuelCmd_SetMagicTrapLockTurns();
        break;
    case 0x4B:
        DuelCmd_AdjustDelayedSummonCount();
        break;
    case 0x4C:
        sub_08014B5C();
        break;
    case 0x50:
        DuelCmd_EnterPhase(0);
        break;
    case 0x51:
        DuelCmd_EnterPhase(1);
        break;
    case 0x52:
        DuelCmd_EnterPhase(2);
        break;
    case 0x53:
        DuelCmd_EnterBattlePhase();
        break;
    case 0x54:
        DuelCmd_EnterPhase(4);
        break;
    case 0x55:
        DuelCmd_EnterPhase(5);
        break;
    case 0x61:
        DuelCmd_DrawCards();
        break;
    case 0x62:
        DuelCmd_SendTopDeckCardsToGraveyard();
        break;
    case 0x63:
        DuelCmd_BanishTopDeckCards();
        break;
    case 0x60:
        DuelCmd_ShuffleDeck();
        break;
    case 0x64:
        DuelCmd_AddDeckCardToHand();
        break;
    case 0x65:
        DuelCmd_RemoveCardFromDeck();
        break;
    case 0x66:
        DuelCmd_SummonFromDeck();
        break;
    case 0x67:
        DuelCmd_SendDeckCardToGraveyard();
        break;
    case 0x68:
        DuelCmd_BanishDeckCard();
        break;
    case 0x69:
        DuelCmd_SetCrushCardTurns();
        break;
    case 0x6A:
        DuelCmd_AddCardToDeckTop();
        break;
    case 0x6B:
        DuelCmd_AddCardToDeckBottom();
        break;
    case 0x70:
        DuelCmd_ShowCardDetail();
        break;
    case 0x71:
        DuelCmd_ShowCardAssemble();
        break;
    case 0x72:
        DuelCmd_ShowCardZoomIn();
        break;
    case 0x73:
        DuelCmd_ShowCardEffect();
        break;
    case 0x74:
        DuelCmd_ShowCardScatter();
        break;
    case 0x75:
        DuelCmd_ShowCardUnrollDown();
        break;
    case 0x76:
        DuelCmd_ShowCardUnrollSideways();
        break;
    case 0x77:
        DuelCmd_PlaceCard();
        break;
    case 0x78:
        DuelCmd_ClearZoneCard();
        break;
    case 0x7C:
        DuelCmd_AddCardToGraveyard();
        break;
    case 0x7D:
        DuelCmd_AddCardToBanished();
        break;
    case 0x7E:
        DuelCmd_ChangePosition();
        break;
    case 0x7F:
        DuelCmd_FlipCard();
        break;
    case 0x79:
        DuelCmd_SendToGraveyard();
        break;
    case 0x7A:
        DuelCmd_Banish();
        break;
    case 0x7B:
        DuelCmd_BanishFlagged();
        break;
    case 0x80:
        DuelCmd_ReturnToHand();
        break;
    case 0x81:
        DuelCmd_ReturnToDeck();
        break;
    case 0x82:
        DuelCmd_MoveToZone();
        break;
    case 0x83:
        DuelCmd_AddEquipLink();
        break;
    case 0x84:
        DuelCmd_SwapZones();
        break;
    case 0x85:
        DuelCmd_AddZoneLink();
        break;
    case 0x86:
        DuelCmd_RemoveZoneLink();
        break;
    case 0x87:
        DuelCmd_SetZoneDeclaredValue();
        break;
    case 0x88:
        DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue();
        break;
    case 0x89:
        DuelCmd_SetZoneTurnCounter();
        break;
    case 0x8A:
        DuelCmd_AddZoneTurnCounter();
        break;
    case 0x8B:
        DuelCmd_SetDestroyedByOpponentFlag();
        break;
    case 0x8C:
        DuelCmd_ClearZoneLinks();
        break;
    case 0x8D:
        DuelCmd_MoveZoneLinks();
        break;
    case 0x8E:
        DuelCmd_AddProhibition();
        break;
    case 0x8F:
        DuelCmd_RemoveProhibition();
        break;
    case 0x90:
        DuelCmd_SetZoneStatusFlags();
        break;
    case 0x91:
        DuelCmd_ClearZoneStatusFlags();
        break;
    case 0x92:
        DuelCmd_SetEffectUnused();
        break;
    case 0x93:
        DuelCmd_TributeMonster();
        break;
    case 0x94:
        DuelCmd_PlantInOpponentDeck();
        break;
    case 0x95:
        DuelCmd_SetDestroyCountdown();
        break;
    case 0x96:
        DuelCmd_SetCannotAttack();
        break;
    case 0x97:
        DuelCmd_SetCannotAttackNextTurn();
        break;
    case 0x98:
        DuelCmd_HalveAttack();
        break;
    case 0x99:
        DuelCmd_Nop99();
        break;
    case 0x9A:
        DuelCmd_Nop9A();
        break;
    case 0x9B:
        DuelCmd_Nop9B();
        break;
    case 0x9C:
        DuelCmd_Nop9C();
        break;
    case 0x9D:
        DuelCmd_Nop9D();
        break;
    case 0x9E:
        DuelCmd_Nop9E();
        break;
    case 0x9F:
        DuelCmd_Nop9F();
        break;
    case 0xA0:
        DuelCmd_ClearZoneLinks2();
        break;
    case 0xA1:
        DuelCmd_SetPositionLocked();
        break;
    case 0xA2:
        DuelCmd_SetReturnAfterBattle();
        break;
    case 0xA3:
        DuelCmd_SummonToken();
        break;
    case 0xA4:
        DuelCmd_SetZoneCardWord();
        break;
    case 0xA5:
        DuelCmd_SendFusionMaterialToGrave();
        break;
    case 0xA6:
        sub_08013104();
        break;
    case 0xA7:
        DuelCmd_MoveMonsterFaceDown();
        break;
    case 0xA8:
        DuelCmd_SetMagicalHatsCard();
        break;
    case 0xA9:
        DuelCmd_BanishMonsterUntilEndPhase();
        break;
    case 0xAA:
        DuelCmd_ReturnBanishedMonster();
        break;
    case 0xB0:
        DuelCmd_NegateActivation();
        break;
    case 0xB2:
        DuelCmd_NopB2();
        break;
    case 0xB1:
        DuelCmd_SetSpellTrapDisabled();
        break;
    case 0xB3:
        DuelCmd_UpdateZoneLpPaid();
        break;
    case 0xB4:
        DuelCmd_IncrementZoneTurnCounter();
        break;
    case 0xC0:
        DuelCmd_SendHandCardToGraveyard();
        break;
    case 0xC1:
        DuelCmd_BanishHandCard();
        break;
    case 0xC3:
        DuelCmd_ReturnHandCardToDeck();
        break;
    case 0xC2:
        DuelCmd_RemoveCardFromHand();
        break;
    case 0xC4:
        DuelCmd_PlaceMonsterFromHand();
        break;
    case 0xC5:
        DuelCmd_PlaceSpellTrapFromHand();
        break;
    case 0xC7:
        DuelCmd_ExchangeHandCards();
        break;
    case 0xCB:
        DuelCmd_AddCardToHand();
        break;
    case 0xCA:
        DuelCmd_CompactHand();
        break;
    case 0xCC:
        DuelCmd_SendHandFusionMaterialToGraveyard();
        break;
    case 0xCD:
        DuelCmd_BanishHandFusionMaterial();
        break;
    case 0xCE:
        DuelCmd_BanishHandCardFaceDown();
        break;
    case 0xCF:
        DuelCmd_ReturnBanishedCardToHand();
        break;
    case 0xD0:
        DuelCmd_ReturnGraveyardCardToDeckTop();
        break;
    case 0xD1:
        DuelCmd_ReturnGraveyardCardToDeckBottom();
        break;
    case 0xD2:
        DuelCmd_ReturnGraveyardCardToHand();
        break;
    case 0xD4:
        DuelCmd_BanishGraveyardCard();
        break;
    case 0xD3:
        DuelCmd_RemoveCardFromGraveyard();
        break;
    case 0xD5:
        DuelCmd_TakeOpponentGraveyardCard();
        break;
    case 0xD6:
        DuelCmd_ReturnGraveyardToDeck();
        break;
    case 0xD7:
        DuelCmd_AddCardToGraveyardNoRedraw();
        break;
    case 0xD8:
        DuelCmd_ClearPendingEquip();
        break;
    case 0xD9:
        DuelCmd_EquipGraveyardCardToOpponent();
        break;
    case 0xDA:
        sub_080106BC();
        break;
    case 0xDB:
        sub_08010708();
        break;
    case 0xDC:
        DuelCmd_RemoveCardFromFusionDeck();
        break;
    case 0xDD:
        DuelCmd_SendFusionDeckCardToGraveyard();
        break;
    case 0xDE:
        DuelCmd_ReturnBanishedCardToGraveyard();
        break;
    case 0xE0:
        DuelCmd_TossCoin();
        break;
    case 0xE1:
        DuelCmd_TossThreeCoins();
        break;
    case 0xE2:
        DuelCmd_RollGracefulDice();
        break;
    case 0xE4:
        DuelCmd_RollPlainDie();
        break;
    case 0xE3:
    case 0xE5:
        DuelCmd_RollSkullDice();
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}
