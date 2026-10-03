#include "global.h"

/* Menu block 0x0201AE60 (see code_0804EFF0) */
struct Ui {
    u8 u0[0xA];
    u16 w0A;        /* +0xA */
    u8 u0C[2];
    u16 h;          /* +0xE */
    u8 u10[4];
    u16 sel;        /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;       /* +0x22 */
    u8 timer;       /* +0x23 */
};
extern struct Ui gTextBox;
struct MainKeys { u8 u0[6]; u16 keys; u8 u8_[0x485E - 8]; u16 frameCounter; };
extern struct MainKeys gMain;
extern u16 gUnk_0300489E;   /* gMain.frameCounter */
/* Duel global 0x020192E0 */
struct Duel {
    u8 u0[0x1B50];
    u8 b1B50;
    u8 pad;
    u16 h1B52[5];       /* +0x1B52 candidate card ids */
    u8 u1B5C[0x1B62 - 0x1B5C];
    u8 step;            /* +0x1B62 */
    u8 pad2;
    u16 sel;            /* +0x1B64 */
};
extern u8 gDuel[];
#define DUEL (*(struct Duel *)gDuel)
extern const u16 gPulseScaleCurve[];
extern const u8 gStrPromptSelectOneOfFive[];
int GetCardIconObjTile(u16 a);
void PlaySE(u16 se);
/* A card instance word in the duel lists (see code_08008A1C). */
struct DuelCardBits {
    u32 id : 12;
    u32 unk12 : 20;
};
struct DuelZone {
    u32 card;               /* +0: struct DuelCard word */
    u8 unk4[0x94 - 4];
};
/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;           /* +2 */
    u8 unk3;
    u8 numGrave;            /* +4 */
    u8 unk5[7];
    u8 b0C_0 : 5;
    u8 flag0C_5 : 1;        /* +0xC bit 5 (hypothesis: the player's monsters are "sealed") */
    u8 b0C_6 : 2;
    u8 unkD[0x28 - 0xD];
    struct DuelZone zones[11];      /* +0x28 */
    u32 hand[80];                  /* +0x684 */
    u32 deck[80];                  /* +0x7C4 */
    u32 grave[80];                  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};
extern struct DuelPlayer gDuelPlayers[2];
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
extern u32 gDuelGraveyards[];
extern const u32 gCardStats[];
extern const u16 gCardIdToNumber[];
#define ZONE(p, i) (*(u32 *)(((p) & 1) * 0xD64 + (i) * 0x94 + (u32)gDuelZones))
#define GRAVE(p, i) (*(u32 *)(0x02019BE8 + ((p) & 1) * 0xD64 + (i) * 4))
#define CARD_ID(w) (((struct DuelCardBits *)&(w))->id)
#define CARD_STATS(id) (*(gCardStats + ((id) & 0x7FF)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
int GetZoneCardType(int player, int zone);
int GetZoneCardAttribute(int player, int zone);
/* Duel action record at 0x0201CF90 (see code_08055EB0): only the fields read here. */
struct ActRec {
    u32 player : 1;
    u32 zone5 : 5;
    u32 zone8 : 8;
    u32 f14 : 1;        /* bit 14: a card is attached (hypothesis) */
    u32 f15 : 1;
    u32 f16 : 3;
    u32 f19 : 3;
    u32 pad22 : 3;
    u32 f25 : 1;
    u32 f26 : 1;
    u32 pad27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 pad30 : 1;
    u32 cardId : 16;    /* bits 31-46 (straddles the word boundary) */
    u32 pad47 : 17;
    u32 card8;          /* +8 */
    u16 h0C;            /* +0xC */
    u16 lo5 : 5;        /* +0xE bits 0-4 */
    u16 step : 7;       /* +0xE bits 5-11: step of the action */
    u16 hi4 : 4;
};
void TributeMonster(int a, int b);
void DuelCursor_Select(u32 player, u32 a, u32 b);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void Chain_AddPending(u32 a, int b);
void TriggerMysteriousPuppeteer(int player);
void ChangeBattlePosition(int player, int zone, int a, int b);
void DestroyFieldCard(int player, int zone, int a);
u16 AiShouldSetMonster(u16 id, int a);
extern const u8 gStrSelectDisplayPosition[];
void SummonPositionMenu_Draw(void);
u16 SummonPositionMenu_HandleInput(void);
extern struct ActRec gSummonAction;
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, u32 affine);
void DuelInfo_DrawCard(u16 id, int a);
void TextCellsClear(void);
int Random(void);
void TextBoxOpen(u16 a, u16 b, u16 c, const u8 *d);
void TextBoxSetMenu(u16 a, void (*b)(void), u16 (*c)(void));
int CountActiveCardsOnField(int player, u16 number);
int CountMonstersByNumber(int player, u16 number);
int CountFaceUpMonstersByNumber(int player, u16 number);
int CountHandCardsByNumber(int player, u16 number);
int CountFreeMonsterZones(int player);
int IsCardProhibited(u16 id);
int IsToonMonster(u16 cardNo);
int HasFaceUpToonWorld(int player);
int CanSpecialSummon(int player);
int CanActivateEffectOfCard(int player, u16 id, int a);
int CountMonsters(int player);
int CanPayBanishSummonCost(int player, u16 id);
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE_C(id) ((CARD_STATS_C(id) & 0x1F00000) >> 20)
/* Monster level used by the prompts: 0 for Magic/Trap/Ticket, 10 for type 0x18, else stats bits 25-28. */
static inline int GetCardLevel(u16 id)
{
    switch ((int)CARD_TYPE_C(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS_C(id) & 0x1E000000) >> 25;
    }
}
/* Card category: 3 for card 1910, 1 for 1911-1912, 7 Magic, 8 Trap, 9 Ticket, else the monster kind (see code_080619E8). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER_C(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE_C(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (CARD_STATS_C(id) & 0xC0000) >> 18;
    }
}
int CountTributableMonsters(int player, int a);
u16 FiveCardMenu_HandleInput(void);
void FiveCardMenu_Draw(void);

void FiveCardMenu_Draw(void)
{
    int y = (gTextBox.b21 - gTextBox.h + 2) * 8;
    int i = 0;
    u32 base = (u32)&DUEL;
    int x = 0x28;
    u32 off = 0x1B52;
    u16 *cards;
    u16 *fc;

    cards = (u16 *)(base + off);

    fc = &gUnk_0300489E;
    do {
        u16 id = *cards;
        u16 pulse = ((const u16 *)0x081A4424)[(*fc >> 1) & 0xF];
        u32 yx;
        u16 tile;
        if (i != gTextBox.sel)
            pulse = 0x100;
        yx = ((u32)y << 16) | (u32)x;
        tile = GetCardIconObjTile(id) + 0x1000;
        AddAffineSprite(yx, 0x80, tile, (u32)pulse << 16);
        x += 0x20;
        cards++;
    } while (++i <= 4);
}


/* Key callback: left/right cycle the selection (5 entries), A confirms. */
u16 FiveCardMenu_HandleInput(void)
{
    struct Ui *u = &gTextBox;
    u8 *st = &u->state;
    if (*st == 0) {
        DuelInfo_DrawCard(DUEL.h1B52[u->sel], 1);
        (*st)++;
        return 0;
    }
    if (gMain.keys & 0x20)
        u->sel = u->sel + 4;
    else if (gMain.keys & 0x10)
        u->sel = u->sel + 1;
    else
        goto check_a;
    u->sel = u->sel % 5;
    TextCellsClear();
    DuelInfo_DrawCard(DUEL.h1B52[u->sel], 1);
    return 0;
check_a:
    if (gMain.keys & 1)
        return 1;
    return 0;
}
/* Candidate pick step: CPU (or forced random) picks one of the 5 candidates; the human gets a menu. */
int DuelPrompt_PickOneOfFiveCards(void)
{
    struct Duel *d = &DUEL;
    int idx;
    /* FAKEMATCH: retain the initialized step pointer across menu setup. */
    register u8 *st asm("r5");
    if (d->b1B50 & 4) {
        idx = Random() % 5;
        goto store;
    }
    {
        /* FAKEMATCH: initialized base/offset copies retain the ADD order. */
        register u32 base asm("r4") = (u32)d;
        register u32 off asm("r0") = 0x1B62;
        asm("" : "+r"(off));
        st = (u8 *)(base + off);
    }
    if (*st != 0) {
        idx = gTextBox.sel;
    store:
        {
            u32 off = (u32)idx << 1;
            u32 startOff = 0x1B52;
            u32 range = (u32)d + startOff;
            d->sel = *(u16 *)(off + range);
        }
        return 1;
    }
    TextBoxOpen(0x206, 0x213, 0xB, gStrPromptSelectOneOfFive);
    TextBoxSetMenu(5, FiveCardMenu_Draw, FiveCardMenu_HandleInput);
    (*st)++;
    return 0;
}


/* Usability test for a card chain: needs card numbers 0x2E1, 0x2F4, 0x320 on the player's side. */
int CanSummonValkyrion(int p)
{
    int a = 0, b = 0, c = 0;
    int ok = 1;
    int cnt = 0;
    u16 n = 0x58A;
    if (CountActiveCardsOnField(0, n) > 0)
        ok = 0;
    if (CountActiveCardsOnField(1, n) > 0)
        ok = 0;
    if (p != 0 && CountHandCardsByNumber(p, 0x34D) == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(p, 0x2E1) && ok) {
        a = 1;
        cnt++;
    }
    if (CountHandCardsByNumber(p, 0x2E1))
        a = 1;
    if (a == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(p, 0x2F4) && ok) {
        b = 1;
        cnt++;
    }
    if (CountHandCardsByNumber(p, 0x2F4))
        b = 1;
    if (b == 0)
        return 0;
    if (CountFaceUpMonstersByNumber(p, 0x320) && ok) {
        c = 1;
        cnt++;
    }
    if (CountHandCardsByNumber(p, 0x320))
        c = 1;
    if (c == 0)
        return 0;
    if (CountFreeMonsterZones(p) == 0 && cnt == 0)
        return 0;
    return 1;
}
int CanSummonKey1257(int p)
{
    int f = 0;
    u16 n = 0x58A;
    if (CountActiveCardsOnField(0, n) > 0 || CountActiveCardsOnField(1, n) > 0)
        return 0;
    if (CountMonstersByNumber(p, 0x582))
        f = 1;
    if (CountMonstersByNumber(p, 0x584))
        f = 1;
    if (f != 0 && CountTributableMonsters(p, -1) > 1)
        return 1;
    return 0;
}
/* Do the player's field (if `sealed`) or graveyard hold enough matching monsters (1-3) for the ritual/fusion-like card `id`? */
int CanPayBanishSummonCost(int player, u16 id)
{
    int need = 1;
    int mode = 0;
    int i;
    int flag;
    /* FAKEMATCH: retain the initialized mode across the opponent-card test. */
    asm("" : "+r"(mode));
    flag = gDuelPlayers[player & 1].flag0C_5;
    if (CountActiveCardsOnField(1 - player, 0x5E7) > 0 && flag == 0)
        return 0;
    switch (CARD_NUMBER_C(id)) {
    case 0x5EA:
        need = 3;
        if (flag) {
            for (i = 0; i <= 4; i++) {
                if (ZONE(player, i) << 20 != 0 && GetZoneCardType(player, i) == 3) {
                    if (--need == 0)
                        return 1;
                }
            }
        } else {
            for (i = 0; i < gDuelPlayers[player & 1].numGrave; i++) {
                /* FAKEMATCH: preserve the initialized grave-base load. */
                register u32 addr asm("r0") = (u32)gDuelGraveyards;
                u32 off = (player & 1) * 0xD64 + i * 4;
                u16 cid;

                cid = CARD_ID(*(u32 *)(off + addr));
                if (CARD_TYPE_C(cid) == 3) {
                    if (--need == 0)
                        return 1;
                }
            }
        }
        return 0;
    case 0x5EB:
        need = 2;
        mode = 1;
        break;
    case 0x5EC:
        mode = 4;
        break;
    case 0x5ED:
        mode = 3;
        break;
    case 0x5EE:
        mode = 5;
        break;
    case 0x5EF:
        mode = 6;
        break;
    }
    if (flag) {
        for (i = 0; i <= 4; i++) {
            if (ZONE(player, i) << 20 != 0 && GetZoneCardAttribute(player, i) == mode) {
                if (--need == 0)
                    return 1;
            }
        }
    } else {
        for (i = 0; i < gDuelPlayers[player & 1].numGrave; i++) {
            /* FAKEMATCH: each initialized grave base stays in the load scratch. */
            register u32 addr asm("r0") = (u32)gDuelGraveyards;
            u32 off = (player & 1) * 0xD64 + i * 4;
            u16 cid;
            u32 st;

            cid = CARD_ID(*(u32 *)(off + addr));
            st = CARD_STATS_C(cid);
            if (((st & 0x1F00000) >> 20) <= 0x14 && (st >> 29) == mode) {
                if (--need == 0)
                    return 1;
            }
        }
    }
    return 0;
}

/* Can the player use card `id` (a monster-or-not candidate of a prompt) right now? Nothing for empty slots, unsafe cards,
   fusion / ritual monsters, then per-card-number conditions; other cards depend on their level. */
/* Return the zero-extended halfword as a word, as the ROM callers consume it. */
int CanSummonFromHand(int player, u16 id)
{
    u16 number;
    int lvl;
    if (id == 0)
        return 0;
    if (IsCardProhibited(id) != 0)
        return 0;
    if (CARD_TYPE_C(id) > 0x14)
        return 0;
    if (GetCardSubtype(id) == 3)
        return 0;
    if (GetCardSubtype(id) == 2)
        return 0;
    number = CARD_NUMBER_C(id);
    if (IsToonMonster(number) != 0 && HasFaceUpToonWorld(player) == 0)
        return 0;
    switch (number) {
    case 0x5EA:
    case 0x5EB:
    case 0x5EC:
    case 0x5ED:
    case 0x5EE:
    case 0x5EF:
        if (CanSpecialSummon(player) != 0)
            goto summon_check;
        return 0;
    case 0x2E5:
    case 0x3E:
    case 0x187:
    case 0x4B2:
        return 0;
    case 0x37:
    case 0x38:
    case 0x42:
    case 0x170:
        if (CanSpecialSummon(player) == 0)
            return 0;
        return (u16)(CanActivateEffectOfCard(player, id, 1));
    case 0x4E2:
        if (gDuelPlayers[player & 1].handCount == 1 && CountFreeMonsterZones(player) > 0)
            return 1;
        if (CountTributableMonsters(player, -1) > 1)
            return 1;
        return 0;
    case 0x546:
        if (CountMonsters(player) + 1 < CountMonsters(1 - player) && CountFreeMonsterZones(player) > 0)
            return 1;
        if (CountTributableMonsters(player, -1) > 0)
            return 1;
        return 0;
    case 0x175:
        if (CanSpecialSummon(player) == 0)
            return 0;
        if (CountTributableMonsters(player, -1) <= 2)
            return 0;
        if (CountFaceUpMonstersByNumber(player, 0x172) == 0)
            return 0;
        if (CountFaceUpMonstersByNumber(player, 0x173) == 0)
            return 0;
        if (CountFaceUpMonstersByNumber(player, 0x174) == 0)
            return 0;
        return 1;
    case 0x34D:
        if (CanSpecialSummon(player) == 0)
            return 0;
        return (u16)(CanSummonValkyrion(player));
    summon_check:
        if (CountFreeMonsterZones(player) == 0 && !gDuelPlayers[player & 1].flag0C_5)
            return 0;
        return (u16)(CanPayBanishSummonCost(player, id));
    case 0x4E9:
        return (u16)(CanSummonKey1257(player));
    default:
        lvl = GetCardLevel(id);
        /* Keep the initialized level at the original switch join; no instructions. */
        __asm__("" : : "r"(lvl));
        switch (lvl) {
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

/* Draw callback of the yes/no prompt: two card sprites (the action record's card, or a blank tile) that pulse when selected. */
void SummonPositionMenu_Draw(void)
{
    int y = gTextBox.w0A * 8 + 0x20;
    u32 yx1, yx2;
    u16 tile1, tile2;
    u32 aff1, aff2;
    y -= (gTextBox.h + gTextBox.w0A - gTextBox.b21 + 2) * 8;
    yx1 = (y << 16) | 0x40;
    tile1 = GetCardIconObjTile(gSummonAction.cardId) | 0x1000;
    if (gTextBox.sel == 0)
        aff1 = gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16;
    else
        aff1 = 0x01000000;
    AddAffineSprite(yx1, 0x80, tile1, aff1);
    yx2 = (y << 16) | 0x90;
    if (gSummonAction.f14)
        tile2 = GetCardIconObjTile(gSummonAction.cardId) | 0x1000;
    else
        tile2 = 0x40;
    if (gTextBox.sel != 0)
        aff2 = (gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16) | 0x20;
    else
        aff2 = 0x01000020;
    AddAffineSprite(yx2, 0x80, tile2, aff2);
}
/* Key callback of a two-entry yes/no prompt: L/R toggle, A confirms; then a 60-tick flash before returning 1. */
u16 SummonPositionMenu_HandleInput(void)
{
    struct Ui *u = &gTextBox;
    u8 *st = &u->state;
    int s = *st;
    unsigned short t = s; /* FAKEMATCH: the short copy keeps the state byte in r2 and `s + 1` in r2 */
    switch (t) {
    case 1:
        if ((&gTextBox)->timer <= 0x3B)
            (&gTextBox)->timer++;
        else
            *st = s + 1;
        return 0;
    case 2:
        return 1;
    default:
        if (gMain.keys & 0x30) {
            PlaySE(0);
            (&gTextBox)->sel = 1 - (&gTextBox)->sel;
        }
        if (gMain.keys & 1) {
            PlaySE(1);
            (&gTextBox)->state = 1;
            (&gTextBox)->timer = 0;
        }
        return 0;
    }
}
/* Step machine of the action record at 0x0201CF90: step 0 announces it (messages 0xC4/0x80C4), 1 / 2 handle the zone
   (messages 0x71 / 0x90), then the step counter is advanced. Returns 1 when done. */
int ExecuteSummonAction(void)
{
    switch (gSummonAction.step) {
    case 0: {
        u16 msg;
        if (gSummonAction.f25)
            TributeMonster(gSummonAction.f28, gSummonAction.f16);
        if (gSummonAction.f26)
            TributeMonster(gSummonAction.f29, gSummonAction.f19);
        msg = gSummonAction.player ? 0x80C4 : 0xC4;
        {
            int id = gSummonAction.cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)&gSummonAction >> 6;
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower));
                lower &= gSummonAction.zone5;
                mask = lower;
            }
            packed |= mask;
            packed |= (gSummonAction.f14 | gSummonAction.f15 << 1) << 8;
            DuelCmd_Push(msg, id, packed, 0);
        }
        gSummonAction.step++;
        return 0;
    }
    case 1:
        DuelCursor_Select(gSummonAction.player, 0, gSummonAction.zone5);
        if (!gSummonAction.f14) {
            gSummonAction.step = 10;
            return 0;
        }
        DuelCmd_Push(0x71, gSummonAction.cardId, 1, 0);
        gSummonAction.step++;
        return 0;
    case 2: {
        /* FAKEMATCH: keep the player extraction in its original two scratches. */
        register u32 playerBits asm("r1") = (u32)*(u8 *)&gSummonAction << 31;
        register u32 player asm("r3") = playerBits >> 31;
        u32 zone = gSummonAction.zone5;
        u16 msg;
        u32 a, t;
        if ((*(u32 *)(player * 0xD64 + zone * 0x94 + (u32)gDuelZones) << 20) == 0)
            return 1;
        msg = 0x90;
        if (player)
            msg = 0x8090;
        DuelCmd_Push(msg, zone, gSummonAction.h0C, 0);
        TriggerMysteriousPuppeteer(gSummonAction.player);
        switch (*((gSummonAction.cardId & 0x7FF) + gCardIdToNumber)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            a = gSummonAction.player << 31;
            t = (gSummonAction.zone5 << 16) | 0x0A400000;
            Chain_AddPending(a | t | gSummonAction.cardId, gSummonAction.player | (gSummonAction.zone5 << 8));
            break;
        case 0x31C:
            ChangeBattlePosition(gSummonAction.player, gSummonAction.zone5, 0, 0);
            break;
        case 0x45E:
        case 0x585:
            DestroyFieldCard(gSummonAction.player, gSummonAction.zone5, 1);
            break;
        }
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}

/* Step machine of the yes/no prompt for the action record at 0x0201CF90: step 0 asks (CPU decides with AiShouldSetMonster, the
   human gets the prompt 0x207/0x30F with the callbacks SummonPositionMenu_Draw / SummonPositionMenu_HandleInput), 1 stores the answer in the record,
   2 / 3 / 4 announce and run the card (see ExecuteSummonAction). Returns 1 when done. */
static inline u16 ActionBNumber(u16 id)
{
    u32 off = (id & 0x7FF) * 2;
    /* FAKEMATCH: this initialized table address uses the original r3 scratch. */
    register const u16 *base asm("r3") = gCardIdToNumber;
    off += (u32)base;
    return *(const u16 *)off;
}

int ExecuteSummonActionAskPosition(void)
{
    int step = gSummonAction.step;
    struct ActRec *r = &gSummonAction;

    switch (step) {
    case 0:
        if (r->player) {
            gTextBox.sel = AiShouldSetMonster(r->cardId, 0);
        } else {
            TextBoxOpen(0x207, 0x30F, 0xB, gStrSelectDisplayPosition);
            TextBoxSetMenu(5, SummonPositionMenu_Draw, SummonPositionMenu_HandleInput);
        }
        r->step++;
        return 0;
    case 1: {
        u16 n;
        gSummonAction.f15 = gTextBox.sel;
        if (gSummonAction.f15)
            gSummonAction.f14 = 0;
        else
            gSummonAction.f14 = 1;
        n = 0x47F;
        if (CountActiveCardsOnField(0, n) != 0 || CountActiveCardsOnField(1, n) != 0)
            gSummonAction.f14 = 1;
        goto next_global;
    }
    case 2: {
        u16 msg;
        if (r->f25)
            TributeMonster(r->player, r->f16);
        if (r->f26)
            TributeMonster(r->player, r->f19);
        msg = r->player ? 0x80C4 : 0xC4;
        {
            int id = r->cardId;
            /* FAKEMATCH: preserve the initialized shifted field and shared mask. */
            register u32 shifted asm("r0") = *(u16 *)r >> 6;
            u16 mask = 15;
            u32 packed = mask;
            asm("" : : "r"(mask));
            packed &= shifted;
            packed <<= 4;
            {
                u32 lower = mask;
                asm("" : "+r"(lower));
                lower &= r->zone5;
                mask = lower;
            }
            packed |= mask;
            packed |= (r->f14 | r->f15 << 1) << 8;
            DuelCmd_Push(msg, id, packed, 0);
        }
        r->step++;
        return 0;
    }
    case 3: {
        /* FAKEMATCH: preserve an initialized pointer copy before the calls. */
        register struct ActRec *copy asm("r5") = r;
        struct ActRec *r2;
        u16 msg;
        asm("" : : "r"(copy));
        r2 = copy;
        DuelCursor_Select(r2->player, 0, r2->zone5);
        msg = r2->player ? 0x8090 : 0x90;
        DuelCmd_Push(msg, r2->zone5, r2->h0C, 0);
        if (!r2->f14) {
            r2->step = 10;
            return 0;
        }
        DuelCmd_Push(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    }
    case 4: {
        u32 a, t;
        TriggerMysteriousPuppeteer(gSummonAction.player);
        switch (ActionBNumber(gSummonAction.cardId)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            a = gSummonAction.player << 31;
            t = (gSummonAction.zone5 << 16) | 0x0A400000;
            Chain_AddPending(a | t | gSummonAction.cardId, gSummonAction.player | (gSummonAction.zone5 << 8));
            break;
        case 0x31C:
            ChangeBattlePosition(gSummonAction.player, gSummonAction.zone5, 0, 0);
            break;
        case 0x45E:
        case 0x585:
            DestroyFieldCard(gSummonAction.player, gSummonAction.zone5, 1);
            break;
        }
    next_global:
        gSummonAction.step++;
        return 0;
    }
    default:
        return 1;
    }
}

