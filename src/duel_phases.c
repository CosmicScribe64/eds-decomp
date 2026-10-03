#include "global.h"

struct Ui {
    u8 u0[8];
    u16 x;      /* +8 */
    u16 y;      /* +0xA */
    u8 uC[2];
    u16 h;      /* +0xE */
    u8 u10[4];
    u16 sel;    /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;   /* +0x22 */
    u8 timer;   /* +0x23 */
};
extern struct Ui gTextBox;
struct SelMask {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u32 unk65 : 8;
    u32 unk73 : 23;
};
struct DuelGlobal {
    u8 unk0[0x1B12];
    u8 b1B12;
    u8 unk1B13;
    u32 f1B14_0 : 9;
    u32 stage : 8;
    u32 f1B14_17 : 15;
    u16 f1B18;
    u8 unk1B1A[0x1B20 - 0x1B1A];
    u8 step;    /* +0x1B20 */
    u8 unk1B21[0x1B26 - 0x1B21];
    u8 f1B26_0 : 1;
    u8 f1B26_1 : 7;
    u8 unk1B27[0x1B2C - 0x1B27];
    struct SelMask sel;
};
extern struct DuelGlobal gDuel;
struct PS {
    u8 u0[8];
    u8 b8_0 : 4;
    u8 b8_4 : 1;
    u8 b8_5 : 1;
    u8 b8_6 : 2;
    u8 b9_0 : 3;
    u8 b9_3 : 1;
    u8 b9_4 : 1;
    u8 b9_5 : 1;
    u8 b9_6 : 2;
    u8 uA;
    u8 bB_0 : 3;
    u8 bB_3 : 1;
    u8 bB_4 : 4;
    u8 bC_0 : 5;
    u8 bC_5 : 1;
    u8 bC_6 : 2;
    u8 rest[0xD64 - 0xD];
};
extern struct PS gDuelPlayers[];
struct Ui2 { u8 u0[1]; u8 b1; u8 pad[0x20]; };
extern struct Ui2 gDuelCtrl;
extern u8 gAiState[];
void DuelScreen_ScrollToZone(int player, int a);
void DuelLink_SendMessage(u16 msg, u16 a, u16 b, u16 c);
void UpdateMonstersAtTurnStart(int player);
void ApplyPumpkingBoost(int player, int zone);
void QueueAddZoneLink(int player, u16 cardId, u16 pos, u16 a);
int GetZoneCardType(int player, int zone);
extern const u8 gStrEndMainPhaseMenu[], gStrEndYourTurn[];
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, void (*b)(void), int (*c)(void));
int CanEnterBattlePhase(int player);
int DuelScreen_HandleInput(void);
int BattlePhase_Run(int a);
void PhaseMenu_DrawCursor(void);
int PhaseMenu_HandleInput(void);
struct Main { u8 u0[6]; u16 keys; };
extern struct Main gMain;
struct Zone {
    u32 card;   /* +0: 12-bit card id */
    u16 w4;
    u8 f6;      /* +6: bit 1 = face-down */
    u8 f7;
    u8 pad[0x8C - 8];
    u8 b8C_0 : 4;
    u8 b8C_4 : 1;
    u8 b8C_5 : 3;
    u8 pad2[0x94 - 0x8D];
};
extern struct Zone gDuelZones[];
#define ZB(p, z) ((struct Zone *)((z) * 0x94 + (p) * 0xD64 + (u32)&gDuelZones[0]))
extern const u16 gCardIdToNumber[];
#define TBL(id) (*(u16 *)((u8 *)gCardIdToNumber + (((id) & 0x7FF) << 1)))
int CanActivateEffectInZone(int player, int zone, u16 kind);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
void PlaySE(u16 se);
int EffectBlastJugglerCheck(int a, int b);

#define ZB2(p, z) ((struct Zone *)((p) * 0xD64 + (z) * 0x94 + (u32)&gDuelZones[0]))
void UpdateMonstersAtTurnStart(int player)
{
    int i;
    for (i = 0; i < 5; i++) {
        if ((ZB2(player & 1, i)->card << 20) != 0 && (ZB2(player & 1, i)->f6 & 2)) {
            int found = 0;
            if (ZB2(player & 1, i)->b8C_4)
                ZB2(player & 1, i)->b8C_4 = 0;
            switch (((const u16 *)0x08622AB4)[(ZB2(player & 1, i)->card << 20 >> 20) & 0x7FF]) {
            case 0x52:
                if (!(ZB2(player & 1, i)->f7 & 0x20))
                    found = 1;
                break;
            case 0x62:
                if (((u32)(ZB2(player & 1, i)->f6 << 26) >> 28) <= 3)
                    ApplyPumpkingBoost(player, i);
                break;
            }
            if (found != 0 && ((u32)(ZB2(player & 1, i)->f6 << 26) >> 28) <= 5) {
                int p, j;
                for (p = 0; p < 2; p++) {
                    for (j = 0; j < 5; j++) {
                        if ((ZB2(p & 1, j)->card << 20) != 0 && (ZB2(p & 1, j)->f6 & 2) && GetZoneCardType(p, j) == 2
                            && ZB2(p & 1, j)->w4 <= ZB2(player & 1, i)->w4)
                            QueueAddZoneLink(player, (u8)player | (u8)i << 8, (u8)p | (u8)j << 8, 2);
                    }
                }
            }
        }
    }
}

/* Byte +0x1B12 of the duel global: bit 1 is the current player. Padded past 4 bytes so it is read with ldrb. */
struct F168Flags {
    u8 b0 : 1;
    u8 cur : 1;
    u8 rest : 6;
    u8 pad[4];
};
/* gDuel with the player array at +4 (the unit's struct DuelGlobal has no players member). */
struct F168Duel {
    u32 u0;
    struct PS players[2];
    u8 pad[0x1B12 - 4 - 2 * 0xD64];
    u8 flags;   /* +0x1B12 */
    u8 pad2[0x1B20 - 0x1B13];
    u8 step;    /* +0x1B20 */
};
#define F168_CUR(p) (((struct F168Flags *)(p))->cur)

/* Step machine on gDuel.step; returns 1 when finished. */
int DuelPhase_TurnStart(void)
{
    struct F168Duel *e = (struct F168Duel *)&gDuel;
    switch (e->step) {
    case 0:
        DuelScreen_ScrollToZone(F168_CUR(&e->flags), 0xB);
        e->step++;
        return 0;
    case 1: {
        struct PS *ps = e->players;
        /* bit 3 of byte +9, tested as the sign of the byte shifted left by 4 (the ROM's lsl #28; bge) */
        if ((s8)(((u8 *)&ps[F168_CUR(&e->flags) & 1])[9] << 4) < 0) {
            ps[F168_CUR(&e->flags) & 1].b9_3 = 0;
            if (!(gDuelCtrl.b1 & 1) && !(e->flags & 2)) {
                gAiState[0] = 0;
                gAiState[1] = 0;
            }
            if (gDuelCtrl.b1 & 1)
                DuelLink_SendMessage(0xF002, 0, 0, 0);
            gDuelCtrl.u0[0] += 5;
            return 1;
        }
        DuelCmd_Push((e->flags & 2) ? 0x8001 : 1, 0, 0, 0);
        e->step++;
        return 0;
    }
    case 2:
        UpdateMonstersAtTurnStart(F168_CUR(&e->flags));
        UpdateMonstersAtTurnStart(1 - F168_CUR(&e->flags));
        e->step++;
        return 0;
    default:
        /* the same flag byte addressed from the players symbol (0x020192E4 + 0x1B0E) */
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].b9_4 = 0;
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].b9_5 = 0;
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].b8_4 = 0;
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].b8_5 = 0;
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].bB_3 = 0;
        gDuelPlayers[F168_CUR((u8 *)gDuelPlayers + 0x1B0E)].bC_5 = 0;
        return 1;
    }
}

/* Draws the menu cursor sprite (blinks while confirming). */
void PhaseMenu_DrawCursor(void)
{
    struct Ui *u = &gTextBox;
    u32 x = (u->x + 1) * 8;
    int y = (u->y + 2) * 8 + u->sel * 12 - 2;
    y -= (u->h + u->y - u->b21 + 2) * 8;
    if (u->state == 1) {
        if (u->timer & 2)
            AddSprite((y << 16) | x, 0, 0x431F);
    } else {
        AddSprite((y << 16) | x, 0, 0x431F);
    }
}

/* 3-choice menu (A = confirm, B = choose last, up/down move); returns 1 when finished. */
int PhaseMenu_HandleInput(void)
{
    u32 st = gTextBox.state;
    switch ((u8)st) {
    case 1:
        if (gTextBox.timer <= 0x3B)
            gTextBox.timer++;
        else
            gTextBox.state = st + 1;
        break;
    case 2:
        return 1;
    default:
        if (gMain.keys & 0x80) {
            PlaySE(0);
            if (gTextBox.sel <= 1)
                gTextBox.sel++;
            else
                gTextBox.sel = 0;
        }
        if (gMain.keys & 0x40) {
            PlaySE(0);
            if (gTextBox.sel != 0)
                gTextBox.sel--;
            else
                gTextBox.sel = 2;
        }
        if (gMain.keys & 1) {
            PlaySE(1);
            gTextBox.state = 1;
            gTextBox.timer = 0;
        }
        if (gMain.keys & 2) {
            PlaySE(2);
            gTextBox.sel = 2;
            gTextBox.state = 1;
            gTextBox.timer = 0;
        }
        break;
    }
    return 0;
}
/* Confirmation-menu step machine on gDuel.step. */
int DuelPhase_Main(void)
{
    switch (gDuel.step) {
    case 0:
        DuelCmd_Push(0x52, 0, 0, 0);
        gDuel.sel.flag0 = 0;
        gDuel.sel.active = 0;
        gDuel.step++;
        return 0;
    case 10:
        if (CanEnterBattlePhase(0)) {
            TextBoxOpen(0x205, 0x615, 0xB, gStrEndMainPhaseMenu);
            TextBoxSetMenu(5, PhaseMenu_DrawCursor, PhaseMenu_HandleInput);
            gDuel.step = 0x14;
        } else {
            TextBoxOpen(0x206, 0x412, 0xB, gStrEndYourTurn);
            TextBoxSetMenu(1, 0, 0);
            gDuel.step = gDuel.step + 1;
        }
        return 0;
    case 11:
        if (gTextBox.sel != 0)
            return 1;
        gDuel.step = 1;
        return 0;
    case 20:
        if (gTextBox.sel != 0) {
            if (gTextBox.sel == 1)
                return 1;
            gDuel.step = 1;
        } else {
            gDuel.f1B26_0 = 0;
            gDuel.stage = 0;
            gDuel.step = gDuel.step + 1;
            /* FAKEMATCH: keep this completed store separate from the
             * step=1 tail shared by other cases. Emits no instructions. */
            __asm__ volatile("" ::: "memory");
        }
        return 0;
    case 21:
        if (BattlePhase_Run(0)) {
            if (gDuel.f1B26_0)
                return 1;
            gDuel.step = 1;
        }
        return 0;
    default:
        if (DuelScreen_HandleInput() == 0 && (gMain.keys & 2))
            gDuel.step = 10;
        return 0;
    }
    return 0;
}

int GetMaintenanceLpCost(u16 id)
{
    switch (id) {
    case 0x44B:
        return 2000;
    case 0x482:
        return 700;
    case 0x58C:
        return 1000;
    case 0x3BA:
    case 0x590:
        return 500;
    default:
        return 0;
    }
}
 /* 0x0804F654 size 0x54 */
int sub_0804F6A8(int a, int b)
{
    int n = 0;
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            if (i == a && j == b)
                continue;
            if (EffectBlastJugglerCheck(i, j) != 0)
                n++;
        }
    }
    return n;
}
 /* 0x0804F6A8 size 0x44 */
int HasActivatableStandbyCard(int player)
{
    int i = 0;
    /* FAKEMATCH: retain this initialized player offset in the ROM's
     * callee-saved register; player itself stays live for the predicate. */
    register u32 poff __asm__("r6") = (player & 1) * 0xD64;
    for (; i < 10; i++) {
        struct Zone *z = (struct Zone *)(i * 0x94 + poff + (u32)gDuelZones);
        u32 id = z->card << 20 >> 20;
        if (id != 0) {
            switch (((const u16 *)gCardIdToNumber)[(u16)id & 0x7FF]) {
            case 0x243:
            case 0x1A0:
            case 0x2DB:
                if (i > 4)
                    continue;
                if (!(((struct Zone *)(i * 0x94 + poff + (u32)gDuelZones))->f6 & 2))
                    continue;
                break;
            case 0x428:
                if ((z->f6 & 2) || i <= 4)
                    continue;
                break;
            default:
                continue;
            }
            if (CanActivateEffectInZone(player, i, 2))
                return 1;
        }
    }
    return 0;
}
void ShowCardEffect(int player, u16 id);
void LoseLifePoints(int player, int amount);
void GainLifePoints(int player, int amount);
void ChangeBattlePosition(int player, int zone, int a, int b);
void DestroyFieldCard(int player, int zone, int a);
void DamageOpponentPerBanishedMonster(int player);
int CountMonsters(int player);
int CountActiveZoneLinksFromCard(int player, int zone, u16 number);
int CountZoneLinksFromCard(int player, int zone, u16 number);
int GetZoneCardAtk(int player, int zone);
int FindMagicInHand(int player);
int CountGraveyardCardsByNumber(int player, u16 number);
int HalveRoundUp(int amount);
/* Player block view (0xD64 bytes): an array-of-struct declaration is needed so that
 * gEndPS_020192E4[x].field goes through get_inner_reference (constant base loaded first,
 * and the 0x46B case's index `player > 99` scaled separately from the base). */
struct EndPS {
    u16 life;           /* +0 */
    u8 pad2_[9];
    u8 bB;              /* +0xB: bits 4-7 = low 4 bits of a 5-bit zone mask */
    u8 bC;              /* +0xC: bit 0 = high bit of that mask */
    u8 pad[0x28 - 0xD];
    struct Zone zones[22]; /* +0x28 */
    u8 pad2[0x84];
};
extern struct EndPS gEndPS_020192E4[];
#define EZB(p, z) ((struct Zone *)((z) * 0x94 + (p) * 0xD64 + (u32)((u8 *)gEndPS_020192E4 + 0x28)))
#define END_ID(z) (((z)->card << 20) >> 20)
#define END_NUMBER(id) (((const u16 *)0x08622AB4)[(u16)(id) & 0x7FF])
void ApplyStandbyPhaseEffects(int player)
{
    int i;
    u32 id1 = 0x527;
    for (i=0; i<=4; i++) {
        int lo = gEndPS_020192E4[player&1].bB >> 4;
        if ((((int)(((gEndPS_020192E4[player&1].bC&1)<<4)|lo) >> i)&1) &&
            (gEndPS_020192E4[player&1].zones[i].card << 20) == 0) {
            ShowCardEffect(player,((const u16 *)0x08623DF4)[id1]);
            DuelCmd_Push(player ? 0x80AA : 0xAA,(u16)i,1,0);
        }
    }
    for (i=0; i<=4; i++) {
        struct Zone *z=EZB(player&1,i);
        u16 id=END_ID(z);
        u32 face=((u32)z->f6<<30)>>31;
        u32 position=((u32)z->f6<<31)>>31;
        if (id && face) {
            switch (END_NUMBER(id)) {
            case 0x228:
                ShowCardEffect(player,id);
                LoseLifePoints(player,300);
                break;
            case 0x459: {
                int count=CountMonsters(player);
                if (count==1) {
                    ShowCardEffect(player,id);
                    if (!(z->f6 & count)) ChangeBattlePosition(player,i,0,0);
                    z->f7 |= 4;
                }
                break;
            }
            case 0x59D:
                if (position) break;
                goto gain1000;
            case 0x59E:
                if (!position) break;
            gain1000:
                ShowCardEffect(player,id);
                GainLifePoints(player,1000);
                break;
            case 0x5A1:
                ShowCardEffect(player,id);
                GainLifePoints(player,800);
                break;
            }
            {
            int number = 0x58B;
            if (CountActiveZoneLinksFromCard(player,i,number)) {
                ShowCardEffect(player,((const u16 *)0x08623DF4)[number]);
                LoseLifePoints(player,CountActiveZoneLinksFromCard(player,i,number)*500);
            }
            }
            {
            int number = 0x2DF;
            if (CountZoneLinksFromCard(player,i,number)) {
                int amount=GetZoneCardAtk(player,i);
                ShowCardEffect(player,((const u16 *)0x08623DF4)[number]);
                DestroyFieldCard(player,i,1);
                LoseLifePoints(player,amount);
            }
            }
            {
            int number = 0x492;
            if (CountZoneLinksFromCard(player,i,number)) {
                int amount=GetZoneCardAtk(player,i);
                ShowCardEffect(player,((const u16 *)0x08623DF4)[number]);
                amount=HalveRoundUp(amount);
                GainLifePoints(1-player,amount*CountZoneLinksFromCard(player,i,number));
            }
            }
        }
    }
    DamageOpponentPerBanishedMonster(player);
    for (i=5; i<=10; i++) {
        int p = player & 1;
        struct Zone *z;
        u16 id;
        u32 face;
        u32 disabled;
        /* FAKEMATCH: the do/while(0) wrapper puts loop notes after the zone reads, so the
         * first CSE pass starts a new block there and 0xD64/base stay in the loop; the
         * post-loop CSE then reuses them for the 0x46B case as in the ROM. */
        do {
        z=EZB(p,i);
        id=END_ID(z);
        face=((u32)z->f6<<30)>>31;
        disabled=((u32)((u8 *)z)[0x91]<<28)>>31;
        } while (0);
        if (id && face && !disabled) {
            int count;
            int number = 0x589;
            count=CountActiveZoneLinksFromCard(player,i,number);
            if (count>0) {
                ShowCardEffect(player,((const u16 *)0x08623DF4)[number]);
                LoseLifePoints(player,count*500);
            }
            switch (END_NUMBER(id)) {
            case 0x46B:
                /* This compares player itself to 99 in the ROM. */
                if (gEndPS_020192E4[player > 99].life) {
                    ShowCardEffect(player,id);
                    DuelCmd_Push(player ? 0x8043 : 0x43,100,1,0);
                }
                break;
            case 0x51F:
                ShowCardEffect(player,id);
                LoseLifePoints(player,500);
                break;
            case 0x527:
                ShowCardEffect(player,id);
                break;
            }
        }
    }
    for (i=5; i<=9; i++) {
        int opponent=1-player;
        struct Zone *z=EZB(opponent&1,i);
        u16 id=END_ID(z);
        u32 face=((u32)z->f6<<30)>>31;
        u32 disabled=((u32)((u8 *)z)[0x91]<<28)>>31;
        if (id && face && !disabled) {
            switch (END_NUMBER(id)) {
            case 0x445:
                if (FindMagicInHand(player)==-1) break;
            case 0x42C:
                ShowCardEffect(opponent,id);
                GainLifePoints(player,1000);
                break;
            case 0x516:
                ShowCardEffect(opponent,id);
                LoseLifePoints(player,500);
                break;
            case 0x51F:
                ShowCardEffect(player,id);
                LoseLifePoints(player,500);
                break;
            }
        }
    }
    {
    int number = 0x5A6;
    if (CountGraveyardCardsByNumber(player,number)>0) {
        ShowCardEffect(player,((const u16 *)0x08623DF4)[number]);
        GainLifePoints(player,200*CountGraveyardCardsByNumber(player,number));
    }
    }
}
struct FcPlayer {
    u16 life; u8 handCount; u8 pad3[3]; u8 listCount; u8 pad7;
    u8 flags8; u8 flags9; u8 padA; u8 flagsB; u8 flagsC;
    u8 padD[0xCC4-0xD]; u16 cardList[(0xD64-0xCC4)/2];
};
typedef char FcPlayerStride[(sizeof(struct FcPlayer)==0xD64)?1:-1];
extern const u16 gCardNumberToId[], gUnk_0862457C[], gUnk_086249F4[];
extern const u32 gCardStats[];
extern const char gStrCompleteStandbyPhase[], gStrMaintainLpCostFmt[], gStrMaintainTributeFmt[], gStrSelectMonsterAsTribute[];
extern const char gCardNames[];
extern u8 gDuelScreen[];
void ApplyStandbyPhaseEffects(int player);
int SinisterSerpentStandbyStep(int player);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int RemoveGraveyardCardByNumber(int player, u16 number, u32 *card);
void ShowCardEffect(int player, u16 id);
void sub_080197C0(int player, u16 id);
void QueueSpecialSummon(int player, u32 *card, u16 a, u16 b, u16 c);
int CountActiveCardsOnField(int player, u16 number);
int CountFaceUpMonstersByNumber(int player, u16 number);
int FindFaceUpCardOnField2(int player, u16 number);
int CountMonstersFiltered(int player, int a, int b);
void DuelPrompt_Post(int player, int a, u16 number, u16 b);
void Chain_AddPending(u32 event, int a);
void DuelCursor_Refresh(void);
int HasActivatableStandbyCard(int player);
int Random(void);
void DestroyFieldCard(int player, int zone, int a);
int CountTributableMonsters(int player, int exclude);
int GetMaintenanceLpCost(u16 number);
int AiFindHandCardByNumber(int player, u16 number);
void FormatStr(char *dst, const char *format, const char *arg);
void FormatInt(char *dst, const char *format, int arg);
u32 DuelCursor_PickTarget(u32 a);
int TributeMonster(int a, int b);
struct FcState {
    u32 header; struct FcPlayer players[2]; u8 pad1ACC[0x1B12-0x1ACC]; u8 flags; u8 pad13[0x1B20-0x1B13];
    u8 step, zone, cursor, subcursor; u8 pad24[0x1B64-0x1B24]; u16 choice;
};
/* The ROM also addresses the same step byte from 0x020192E4 + 0x1B1C. */
struct FcPlayerStateView {
    struct FcPlayer players[2];
    u8 pad1AC8[0x1B1C-0x1AC8];
    u8 step;
};
#define FC_PLAYER_STEP (((struct FcPlayerStateView *)gDuelPlayers)->step)
struct FcFlags { u8 pad0[9]; u8 bit0:1; u8 bit1:1; u8 rest:6; };
#define FC_E ((u8 *)&gDuel)
#define FC_STEP (((struct FcState *)&gDuel)->step)
#define FC_ZONE (((struct FcState *)&gDuel)->zone)
#define FC_CURSOR (((struct FcState *)&gDuel)->cursor)
#define FC_SUBCURSOR (((struct FcState *)&gDuel)->subcursor)
#define FC_CHOICE (((struct FcState *)&gDuel)->choice)
#define FC_PS(p) ((u8 *)gDuelPlayers+(p)*0xD64)
#define FC_PLAYER(p) (((struct FcState *)&gDuel)->players[p])
#define FC_LIFE(p) (((struct FcPlayer *)gDuelPlayers)[p].life)
#define FC_ID(z) (((z)->card<<20)>>20)
#define FC_NUMBER(id) ((const u16 *)0x08622AB4)[(u16)(id)&0x7FF]
static inline int EndTurnLevel(u16 id)
{
    int type=(((const u32 *)0x08621DE0)[id&0x7FF]&0x1F00000)>>20;
    switch (type) {
    case 21: case 22: case 23: return 0;
    case 24: return 10;
    default: return (((const u32 *)0x08621DE0)[id&0x7FF]&0x1E000000)>>25;
    }
}
static inline u16 EndTurnCardId(u16 number)
{
    if (number==0xFFFF) return 0;
    if (number<=0x7CF) return ((const u16 *)0x08623DF4)[number&0x7FF];
    return ((const u16 *)0x08623DF4)[(number-0x7D0)&0x7FF]+1;
}
struct FcFlagsS { u8 pad0[9]; u8 bit0:1; s8 bit1:1; u8 rest:6; };
static inline int FcNum(u32 id) { return ((const u16 *)0x08622AB4)[id&0x7FF]; }
struct FcCfb0 { u8 pad0[0x824]; int a824; u8 pad828[4]; int a82C; };
/* FAKEMATCH: a dead 64-factor product (FC_LOOP_PAD) pads case 101's loop. flow deletes it before register
   allocation, so it emits nothing, but loop.c counts its insns: the first loop pass then sees ~207 insns
   (without it 144), finds &zone and the 0xD64 constant not worth hoisting, and leaves them to the second pass.
   That gives the ROM preheader order (E0 reload, &zone, 0xD64, player*0xD64). The ROM loop evidently had
   ~201-210 insns at loop time; the window is 58-70 factors (more pushes the second pass over 200 insns). */
#define FC_PAD8(x) x*x*x*x*x*x*x*x
#define FC_LOOP_PAD(x) (FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x)*FC_PAD8(x))
int DuelPhase_Standby(void)
{
    u32 card; /* Written by RemoveGraveyardCardByNumber on success before it is consumed. */
    char text[128];
    char format[128];
    u32 player; u8 *base; u8 *b4; struct FcFlagsS *ps;
    u16 id;
    player=((struct FcState *)&gDuel)->flags; player=((u32)player<<30)>>31;
    base=FC_E; b4=base+4; ps=(struct FcFlagsS *)(b4+player*0xD64);
    if (ps->bit1<0) {
        ps->bit1=0;
        goto done;
    }
    switch (FC_STEP) {
    case 0: {
        u16 msg=0x51;
        if (player) msg=0x8051;
        DuelCmd_Push(msg,0,0,0);
        FC_STEP=20;
        return 0;
    }
    case 1:
        ApplyStandbyPhaseEffects(player);
        FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
        FC_STEP++;
        return 0;
    case 2:
        if (SinisterSerpentStandbyStep(player)) {
            FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
            FC_STEP++;
        }
        return 0;
    case 3: {
        struct FcPlayer *pl=(struct FcPlayer *)gDuelPlayers;
        if (((u32)(pl[player].flagsC)<<28)>>29) {
            u16 msg=0x4B;
            if (player) msg=0x804B;
            DuelCmd_Push(msg,0,0,0);
            if (CountFreeMonsterZones(player)>0 && RemoveGraveyardCardByNumber(player,0x57D,&card)) {
                ShowCardEffect(player,EndTurnCardId(0x57D));
                QueueSpecialSummon(player,&card,1,1,0x20);
                return 0;
            }
        }
        FC_STEP++;
        FC_CURSOR=0;
        return 0;
    }
    case 4:
        for (;FC_CURSOR<FC_PLAYER(player).listCount;FC_CURSOR++) {
            u32 c=FC_PLAYER(player&1).cardList[FC_CURSOR]; if (c==0x402) {
                u16 msg;
                ShowCardEffect(player,gUnk_0862457C[0]);
                msg=0xCF;
                if (player) msg=0x80CF;
                DuelCmd_Push(msg,FC_CURSOR,1,0);
                return 0;
            }
        }
        FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
        FC_STEP++;
        return 0;
    case 5: {
        int other=1-player;
        struct FcPlayer *pl;
        if (CountActiveCardsOnField(other,0x489) && (pl=(struct FcPlayer *)gDuelPlayers, pl[other&1].life>499) && pl[player&1].handCount) {
            DuelPrompt_Post(other,15,0x489,0);
            FC_PLAYER_STEP++;
        } else FC_STEP=7;
        return 0;
    }
    case 6:
        if (FC_CHOICE) {
            int other, index;
            u32 kind=0x6200000;
            u16 msg=0x43;
            if (player!=1) msg=0x8043;
            DuelCmd_Push(msg,500,0,0);
            other=1-player;
            index=FindFaceUpCardOnField2(other,0x489);
            Chain_AddPending(((u32)(other&1)<<31)|(((index&31)<<16)|kind)|EndTurnCardId(0x489),0);
            FC_STEP=5;
        } else FC_STEP=7;
        return 0;
    case 7: {
        int other=1-player;
        if (CountFaceUpMonstersByNumber(other,0x5ED) && CountMonstersFiltered(player,1,0)>0) {
            DuelPrompt_Post(other,15,0x5ED,0);
            FC_STEP++;
        } else {
            FC_ZONE=5;
            FC_STEP=9;
        }
        return 0;
    }
    case 8:
        if (FC_CHOICE) {
            int other=1-player;
            u32 kind;
            int index=FindFaceUpCardOnField2(other,0x5ED);
            Chain_AddPending(((u32)(other&1)<<31)|(((index&31)<<16)|(kind=0x6400000))|EndTurnCardId(0x5ED),0);
        }
        FC_ZONE=5;
        FC_STEP=9;
        return 0;
    case 9:
        for (;FC_ZONE<=9;FC_ZONE++) {
            u32 zone=FC_ZONE;
            u8 *zp=FC_E+0x2C;
            struct Zone *z=(struct Zone *)(zone*0x94+player*0xD64+zp);
            u32 id=FC_ID(z);
            if (id && (z->f6&2) && !(((u8 *)z)[0x91]&8) && FcNum(id)==0x592 && CountFreeMonsterZones(player)>0) {
                u16 msg;
                ShowCardEffect(player,FC_ID((struct Zone *)(FC_ZONE*0x94+player*0xD64+FC_E+0x2C)));
                msg=0xA3;
                if (player) msg=0x80A3;
                DuelCmd_Push(msg,(u8)FindFreeMonsterZone(player)|(FC_ZONE<<8),3,0);
                FC_ZONE++;
                return 0;
            }
        }
        DuelCursor_Refresh();
        FC_STEP++;
        return 0;
    case 10:
        if ((u16)HasActivatableStandbyCard(player)==0) {
            DuelCursor_Refresh();
            FC_STEP=100;
        } else if (DuelScreen_HandleInput()==0 && (gMain.keys&2)) {
            TextBoxOpen(0x206,0x713,11,gStrCompleteStandbyPhase);
            TextBoxSetMenu(1,0,0);
            FC_STEP++;
        }
        return 0;
    case 11:
        switch (gTextBox.sel) {
        case 0: FC_STEP=10; break;
        case 1: FC_STEP=100; break;
        }
        return 0;
    case 20: {
        const u16 *tbl=gUnk_086249F4;
        int count=CountActiveCardsOnField(player,0x600);
        if (count>0) do {
            int die=Random()%6+1;
            int p, i;
            u16 msg;
            ShowCardEffect(player,tbl[0]);
            msg=0xE4;
            if (player) msg=0x80E4;
            DuelCmd_Push(msg,die,0,0);
            msg=0x12;
            if (player) msg=0x8012;
            DuelCmd_Push(msg,0,0,0);
            for (p=0;p<=1;p++) {
                for (i=0;i<=4;i++) {
                    struct Zone *z=ZB(p&1,i);
                    u32 id=FC_ID(z);
                    if (id && (z->f6&2)) {
                        int level=EndTurnLevel(id);
                        register int destroy asm("r0")=0; /* FAKEMATCH: the ROM gives destroy r0 and level r1; global alloc otherwise hands level r0 first */
                        if (level==die) destroy=1;
                        if (level>5 && die==6) destroy=1;
                        if (destroy) DestroyFieldCard(p,i,1);
                    }
                }
            }
        } while (--count);
        FC_STEP=1;
        return 0;
    }
    case 21: FC_STEP=20; break;
    case 100:
        FC_ZONE=0;
        FC_STEP++;
        /* fall through */
    case 101:
        for (;FC_ZONE<=9;FC_ZONE++) {
            struct Zone *z=(struct Zone *)(FC_ZONE*0x94+player*0xD64+FC_E+0x2C);
            if (FC_ID(z) && (z->f6&2)) {
                int destroy;
                id=FC_ID(z);
                destroy=FC_LOOP_PAD(id); /* FAKEMATCH: dead, see FC_LOOP_PAD */
                destroy=0;
                switch (FC_NUMBER(id)) {
                case 0x47A:
                    if (CountTributableMonsters(player,-1)>0) { FC_STEP=120; return 0; }
                    destroy=1;
                    break;
                case 0x597:
                    if (CountTributableMonsters(player,FC_ZONE)>0) { FC_STEP=120; return 0; }
                    destroy=1;
                    break;
                case 0x3BA: case 0x44B: case 0x482: case 0x58C: case 0x590:
                    if (FC_LIFE(player)<GetMaintenanceLpCost(FC_NUMBER(id))) destroy=1;
                    else { FC_PLAYER_STEP=110; return 0; }
                    break;
                }
                if (destroy) {
                    u16 msg=0x74;
                    if (player) msg=0x8074;
                    DuelCmd_Push(msg,id,1,0);
                    DestroyFieldCard(player,FC_ZONE,1);
                }
            }
        }
        FC_STEP++;
        return 0;
    case 102: {
        int i;
        for (i=5;i<=9;i++) {
            struct Zone *z=ZB(player,i);
            u32 id=FC_ID(z);
            if (id && (z->f6&2)) {
                int number=FcNum(id);
                if (number==0x416 || number==0x424) {
                    u16 msg=0xB4;
                    if (player) msg=0x80B4;
                    DuelCmd_Push(msg,i,1,0);
                }
            }
        }
        FC_STEP++;
        return 0;
    }
    case 110: {
        id=FC_ID((struct Zone *)((player&1)*0xD64+FC_ZONE*0x94+FC_E+0x2C));
        if (player) {
            switch (FcNum(id)) {
            case 0x3BA: {
                int found=0;
                if (AiFindHandCardByNumber(1,0x2D6)) found=1;
                if (AiFindHandCardByNumber(1,0x2D7)) found=1;
                if (AiFindHandCardByNumber(1,0x2D8)) found=1;
                if (AiFindHandCardByNumber(1,0x2FE)) found=1;
                if (CountFaceUpMonstersByNumber(1,0x2D6)) found=1;
                if (CountFaceUpMonstersByNumber(1,0x2D7)) found=1;
                if (CountFaceUpMonstersByNumber(1,0x2D8)) found=1;
                if (CountFaceUpMonstersByNumber(1,0x2FE)) found=1;
                gTextBox.sel=0;
                if (found && (((struct FcState *)&gDuel)->players[1].life)>1000) gTextBox.sel=1;
                break;
            }
            case 0x44B:
                if ((((struct FcState *)&gDuel)->players[1].life)>7000) gTextBox.sel=1;
                else gTextBox.sel=0;
                break;
            default:
                if ((((struct FcState *)&gDuel)->players[1].life)>GetMaintenanceLpCost(FcNum(id))+1000) gTextBox.sel=1;
                else gTextBox.sel=0;
                break;
            }
        } else {
            FormatStr(format,gStrMaintainLpCostFmt,(const char *)0x0822C720+(id<<6));
            FormatInt(text,format,GetMaintenanceLpCost(FcNum(id)));
            TextBoxOpen(0x206,0x613,11,text);
            TextBoxSetMenu(1,0,0);
        }
        sub_080197C0(player,id);
        FC_STEP++;
        return 0;
    }
    case 111:
        if (gTextBox.sel) {
            u16 msg;
            id=FC_ID((struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+player*0xD64));
            msg=0x43;
            if (player) msg=0x8043;
            DuelCmd_Push(msg,GetMaintenanceLpCost(FcNum(id)),1,0);
            if (FcNum(id)==0x3BA) {
                u16 msg2=0xB3;
                if (player) msg2=0x80B3;
                DuelCmd_Push(msg2,FC_ZONE,500,0);
            }
        } else DestroyFieldCard(player,FC_ZONE,1);
        FC_ZONE++;
        FC_STEP=101;
        return 0;
    case 120: {
        u32 id=FC_ID((struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+player*0xD64));
        FormatStr(text,gStrMaintainTributeFmt,gCardNames+(id<<6));
        TextBoxOpen(0x206,0x813,11,text);
        TextBoxSetMenu(1,0,0);
        FC_STEP++;
        return 0;
    }
    case 121:
        if (gTextBox.sel) {
            TextBoxOpen(0x206,0x712,11,gStrSelectMonsterAsTribute);
            FC_STEP++;
            return 0;
        } else {
            DestroyFieldCard(player,FC_ZONE,1);
            FC_ZONE++;
            FC_STEP=101;
        }
        return 0;
    case 122:
        if (DuelCursor_PickTarget(0xF0)) {
            TributeMonster(((struct FcCfb0 *)gDuelScreen)->a824,((struct FcCfb0 *)gDuelScreen)->a82C);
            FC_ZONE++;
            FC_STEP=101;
        }
        return 0;
    default:
        goto done;
    }
    return 0;
done:
    return 1;
}


