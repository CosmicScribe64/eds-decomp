#include "global.h"

void DuelCmd_Push(u16 cmd, u16 arg2, u16 arg4, u16 arg6);
void ReturnFieldCardToHand(int player, int zone, int a);
struct DuelZone { u32 w0; u8 unk4; u8 unk5; u8 flags6; u8 unk7[0x94 - 7]; };
#define ID(z) (((z)->w0 << 20) >> 20)
extern u8 gDuel[];
struct AE60 { u8 unk0[0x14]; u16 h14; };
extern struct AE60 gTextBox;
extern const char gStrAskTransferControlFmt[];
extern const u16 gUnk_08624244[];
extern const char gCardNames[];
void FormatStr(char *dst, const char *fmt, const char *arg);
void TextBoxOpen(int a, int b, int c, char *s);
void TextBoxSetMenu(int a, int b, int c);
struct DuelZoneF { u32 w0; u8 pad4[0x8C - 4]; u8 b8c; u8 pad8d[0x94 - 0x8D]; };
void DestroyFieldCard(int player, int zone, int a);
struct DuelZoneL { u32 w0; u8 pad4[2]; u8 flags6; u8 pad7; u8 b8; u8 pad9[0x94 - 9]; };
extern const u16 gUnk_08624848[];
struct SB { u8 b0 : 1; u8 who : 1; u8 rest : 6; u8 pad[7]; };
struct PS9 { u8 pad[9]; u8 f0 : 2; u8 f2 : 1; u8 rest : 5; u8 pad2[0xD64 - 10]; };
extern struct PS9 gDuelPlayers[];
void DuelCursor_Select(int a, int b, int c);
int DuelScreen_HandleInput(void);
void DrawCards(int player, int n);
struct BHdr { u8 b0; u8 lo : 1; u8 defSlot : 3; u8 hi : 4; u8 pad[6]; };
extern struct BHdr gBattle;
extern const u16 gUnk_086249F8[];
int Duel_CheckWin(void);
void ApplyKotodamaToZone(int player, int zone);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int CountActiveCardsOnField(int player, u16 number);
int CountZoneLinksFromCard(int player, int zone, u16 number);
void MoveFieldCard(int player, u16 a, u16 b);
void ShowCardEffect(int player, u16 id);
extern u8 gDuelScreen[];
struct MainK { u8 pad0[6]; u16 keys; u8 pad8[0x485E - 8]; u16 anim; };
extern struct MainK gMain;
extern const u16 gPulseScaleCurve[];
void AddAffineSprite(u32 a, int b, int c, int d);
extern int (*const volatile gBattleStageHandlers[])(int);
/* Adjacent duel steps: word +0x1B14 bits 9-16, then halfword +0x1B16
 * bits 1-8. The latter is bits 17-24 relative to +0x1B14, not an alias. */
struct StepWord { u32 lo : 9; u32 step : 8; u32 hi : 15; };
struct StepHalf { u16 lo : 1; u16 step : 8; u16 hi : 7; u8 pad[6]; };

/* Step of a battle effect for card 0x602 or the defender zone's card: if the defender zone (1-player) holds a card and the player's list count is positive and CountZoneLinksFromCard(q, def, 0x4DB) holds, send MoveFieldCard; if card 0x602 is present on either side announce it and finish (return 1), otherwise reset the step and return 0. */
int BattleStage_EndAttack(int player)
{
    int found = 0;
    int q, qb;
    u8 *e;
    struct DuelZone *zn;
    if (Duel_CheckWin() != 0)
        return 1;
    q = 1 - player;
    ApplyKotodamaToZone(q, gBattle.defSlot);
    qb = q & 1;
    zn = (struct DuelZone *)(gBattle.defSlot * 0x94 + qb * 0xD64 + 0x0201930C);
    if ((zn->w0 << 20) != 0 && CountFreeMonsterZones(player) > 0 && CountZoneLinksFromCard(q, gBattle.defSlot, 0x4DB) != 0) {
        int t = q;
        u32 pos;
        /* FAKEMATCH: preserve the initialized player copy before narrowing;
         * q remains live as the first call argument. Emits no instructions. */
        __asm__("" : "+r"(t));
        pos = (u8)t | gBattle.defSlot << 8;
        MoveFieldCard(q, pos, (u8)player | (u8)FindFreeMonsterZone(player) << 8);
    }
    if (CountActiveCardsOnField(player, 0x602) != 0 || CountActiveCardsOnField(1 - player, 0x602) != 0)
        found = 1;
    if (found == 0) {
        e = gDuel;
        ((struct StepWord *)(e + 0x1B14))->step = 1;
        ((struct StepHalf *)(e + 0x1B16))->step = 0;
        return 0;
    }
    ShowCardEffect(player, gUnk_086249F8[0]);
    return 1;
}
void CopyDuelCard(void *, const void *);
int FindFreeSpellTrapZone(int);
int HasZoneCardEffectLink(int, int, u16);
void ChangeBattlePosition(int, int, int, int);
void DuelPrompt_Post(int, int, u16, int);
void ResolvePendingGraveyardEquip(int, int, int);
void QueueSpecialSummonChoosePosition(int, void *, int, int);
extern u8 gDuelCtrl[];
extern const u16 gUnk_08624730[];
extern const u16 gUnk_0862486C[];
/* Views of the duel state at 0x020192E0 for BattleStage_EndBattlePhase: player records (0xD64 bytes) at +4 with the
 * graveyard count at +4 and the graveyard list at +0x904, zones (0x94 bytes) at +0x2C, and u16 bitfields
 * at +0x1B14 where the selected player (bits 9-16 from +0x1B16) straddles the halfword, as agbcc lays
 * out straddling bitfields (byte-split access). */
struct DCZone { u32 w0; u8 pad4[2]; u16 h6; u8 pad8[0x94 - 8]; };
struct DCPlayer { u8 pad0[4]; u8 count; u8 pad5[3]; u32 lo : 6; u32 f6 : 1; u32 x : 5; u32 f12 : 1; u32 hi : 19; u8 padC[0xD64 - 0xC]; };
struct DCCard { u32 lo : 28; u32 f28 : 1; u32 hi : 3; };
struct DCState {
    u8 pad0[0x1B14];
    u16 lo : 9; u16 step1 : 8; u16 step : 8; u16 player : 8; u16 index : 8; u16 hi : 7;
    u8 pad1B1A[2];
    struct DCCard c1B1C;
    u8 pad1B20[0x1B64 - 0x1B20];
    u16 h1B64;
};
#define DC_ID(z) (((z)->w0 << 20) >> 20)
#define DC_PL(p) ((struct DCPlayer *)(gDuel + 4) + ((p) & 1))
#define DCS ((struct DCState *)gDuel)
#define DC_ZONES (gDuel + 0x2C)
#define DC_GRAVE (gDuel + 0x908)
/* Graveyard effect steps on the halfword step at +0x1B16: step 0 handles card 0x540 in the player's
 * zones and card 0x49E for both players, 1 resolves graveyard entries with bit 24 (linked zone, card
 * 0x52F), 2 selects an entry with bit 28 and jumps to 5-8 (messages 0xDA/0xD3, DuelPrompt_Post,
 * CopyDuelCard, QueueSpecialSummonChoosePosition); other steps message 0x47 and reset or set the player's byte-9 bit 4.
 * Matching notes: the duel state is read through the global (GCSE keeps one copy in r8); the outer
 * loops use i and the inner ones j in every case. */
int BattleStage_EndBattlePhase(int player)
{
    int i, j;
    /* FAKEMATCH: card is first assigned in the zone check (through n, so the compare stays int-wide)
     * and reassigned 0x49E in the inner loop; the earlier first use stops loop.c from folding the
     * constant into the call and its priority puts it in r8 ahead of player. */
    u16 card;
    int n;
    switch (DCS->step) {
    case 0:
        for (i = 0; i <= 4; i++) {
            struct DCZone *z = (struct DCZone *)(i * 0x94 + (player & 1) * 0xD64 + DC_ZONES);
            u16 id = DC_ID(z);
            if (id != 0 && (card = n = ((const u16 *)0x08622AB4)[(u16)id & 0x7FF], n == 0x540) && (z->h6 & 0x2003) == 2) {
                ShowCardEffect(player, id);
                ChangeBattlePosition(player, i, 0, 0);
            }
            for (j = 0; j <= 1; j++) {
                /* FAKEMATCH: the table pointer is set at the top of the body so loop.c hoists it. */
                const u16 *t;
                card = 0x49E;
                t = gUnk_08624730;
                if (HasZoneCardEffectLink(j, i, card)) {
                    ShowCardEffect(j, t[0]);
                    DestroyFieldCard(j, i, 1);
                }
            }
        }
        DCS->step++;
        return 0;
    case 1:
        for (i = 0; i <= 1; i++) {
            for (j = 0; j < DC_PL(i)->count; j++) {
                u32 *g = (u32 *)((i & 1) * 0xD64 + DC_GRAVE);
                /* FAKEMATCH: the g local keeps (p & 1) * 0xD64 + list in the loop (no strength
                 * reduction) and the int sum puts g first in the add. */
                u32 card = *(u32 *)((int)g + (j << 2));
                if ((s32)(card << 7) < 0) {
                    int q = 1 - i;
                    int qs = q & 1; /* computed before the slot field, as in the ROM */
                    struct DCZone *z = (struct DCZone *)(((card << 4) >> 29) * 0x94 + qs * 0xD64 + DC_ZONES);
                    u32 id = DC_ID(z);
                    int special = 0;
                    if ((((u8 *)z)[6] & 2) && id && FindFreeSpellTrapZone(q) >= 0 && ((const u16 *)0x08622AB4)[(u16)id & 0x7FF] == 0x52F)
                        special = 1;
                    ResolvePendingGraveyardEquip(i, j, special);
                    return 0;
                }
            }
        }
        DCS->step++;
        return 0;
    case 2:
        for (i = 0; i <= 1; i++) {
            /* FAKEMATCH: the comma expression loads the player base first in the loop test, so loop.c
             * hoists it in pass 1 and the inner test keeps its own copy (G - 0x904). */
            u8 *y;
            for (j = 0; j < ((struct DCPlayer *)(y = gDuel + 4, y + (i & 1) * 0xD64))->count; j++) {
                u32 *g = (u32 *)((i & 1) * 0xD64 + DC_GRAVE);
                u32 card = g[j];
                if ((s32)(card << 3) < 0) {
                    DCS->player = i;
                    DCS->index = j;
                    DCS->step = 5;
                    return 0;
                }
            }
        }
        DCS->step++;
        return 0;
    case 5:
        if (FindFreeMonsterZone(1 - DCS->player) == -1) {
            int pl = DCS->player;
            u16 msg = 0xDA;
            if (pl)
                msg = 0x80DA;
            DuelCmd_Push(msg, DCS->index, 1, 0);
            DCS->step = 2;
            return 0;
        }
        ShowCardEffect(player, gUnk_0862486C[0]);
        DCS->step++;
        return 0;
    case 6:
        if (DCS->player && !(gDuelCtrl[1] & 1)) {
            DCS->h1B64 = 1;
            DCS->step = 8;
            return 0;
        }
        DuelPrompt_Post(1 - DCS->player, 0x12, ((struct DCZone *)((DCS->player & 1) * 0xD64 + (u8 *)DCS + 0x908 + DCS->index * 4))->w0 << 20 >> 20, 0);
        DCS->step++;
        return 0;
    case 7:
        if (DCS->h1B64) {
            u16 *card = (u16 *)((DCS->player & 1) * 0xD64 + (u8 *)DCS + 0x908 + DCS->index * 4);
            int msg = 0xD3;
            if (DCS->player)
                msg = 0x80D3;
            DuelCmd_Push(msg, card[0], card[1], 0);
            CopyDuelCard(&DCS->c1B1C, card);
            DCS->step++;
        } else {
            int pl = DCS->player;
            u16 msg = 0xDA;
            if (pl)
                msg = 0x80DA;
            DuelCmd_Push(msg, DCS->index, 1, 0);
            DCS->step = 2;
        }
        return 0;
    case 8:
        DCS->c1B1C.f28 = 0;
        QueueSpecialSummonChoosePosition(1 - DCS->player, &DCS->c1B1C, 1, 0x20);
        DCS->step = 2;
        return 0;
    default:
        {
            u8 *y = gDuel + 4;
            struct DCPlayer *ps = (struct DCPlayer *)(y + (player & 1) * 0xD64);
            if (ps->f6) {
                u16 msg = 0x47;
                if (player)
                    msg = 0x8047;
                DuelCmd_Push(msg, 0, 0, 0);
                DCS->step1 = 0;
                DCS->step = 0;
                return 0;
            }
            ps->f12 = 1;
            return 1;
        }
    }
}
struct E240Flags { u8 b0:1; u8 b1:1; u8 b2:1; u8 rest:5; };
/* Find the first occupied zone with flag +0x8C bit 1 or 2. */
int BattleStage_Cleanup(int player)
{
    int i = 0, j;
    u8 *zb = (u8 *)0x0201930C;
    for (; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            int side = i & 1;
            struct DuelZoneF *zn = (struct DuelZoneF *)(j * 0x94 + side * 0xD64 + (int)zb);
            u8 opp = 1 - i;
            if ((zn->w0 << 20) != 0) {
                if (((struct E240Flags *)&zn->b8c)->b1) {
                    DestroyFieldCard(i, j, 1);
                found:
                    return 0;
                }
                if (((struct E240Flags *)&zn->b8c)->b2) {
                    int r = FindFreeMonsterZone(1 - i);
                    u16 msg = 0xA2;
                    u32 pos;
                    if (player != 0)
                        msg = 0x80A2;
                    pos = (u8)i | (u8)j << 8;
                    DuelCmd_Push(msg, pos, 0, 0);
                    if (r >= 0) {
                        MoveFieldCard(player, pos, (u8)r << 8 | opp);
                        goto found;
                    }
                    DestroyFieldCard(i, j, 1);
                    goto found;
                }
            }
        }
    }
    return 1;
}

int BattlePhase_Run(int player)
{
    int (*const volatile *table)(int) = gBattleStageHandlers;
    u8 *e = gDuel;
    struct StepWord *step = (struct StepWord *)(e + 0x1B14);
    u32 packed = *(u32 *)step << 15;
    if (table[packed >> 24] != 0) {
        /* FAKEMATCH: keep the initialized shifted step live across the null
         * check, so the callback index is extracted again. No instructions. */
        __asm__("" : "+r"(packed));
        if ((u16)table[packed >> 24](player) != 0) {
            ((struct StepHalf *)(e + 0x1B16))->step = 0;
            step->step++;
        }
        return 0;
    } else {
        int flag = e[0x1B12] & 2;
        int msg = 0x54;
        if (flag)
            msg = 0x8054;
        DuelCmd_Push(msg, 0, 0, 0);
        return 1;
    }
}

void sub_0804E3BC(void)
{
}
/* Queue a sprite/effect draw whose first argument depends on how far the list at 0x0201CFB0+0x810 has advanced; the last argument comes from an animation table indexed by bits 1-4 of a gMain halfword. */
void sub_0804E3C0(void)
{
    u8 *e = gDuelScreen;
    u8 *m;
    const u16 *t;
    int d = *(int *)(e + 0x810) - e[4];
    u32 v = 0x002800A0;
    if (d <= 0x47)
        v = 0x007000A0;
    AddAffineSprite(v, 0x40C0, 0xF364, (t = gPulseScaleCurve, m = (u8 *)&gMain, t[(*(u16 *)(m + 0x485E) >> 1) & 0xF] << 16));
}

/* Duel screen fields at 0x0201CFB0 (same layout as struct DuelScreen in code_08012C4C). */
struct E420Screen {
    u8 filler0[0x808];
    u8 unk808_0 : 3;
    u8 busy : 1;
    u8 unk808_4 : 4;
    u8 filler809[0x85C - 0x809];
    u32 unk85C;
};
/* Multi-step routine on the step byte at 0x020192E4+0x1B1C (0x0201AE00), keyed on
 * the active player (bit 1 of the duel flags byte at 0x020192E4+0x1B0E). If that
 * player's byte-9 bit 2 is set, it is cleared and the routine ends (returns 1).
 * Otherwise step 0 sends message 0x50 (0x8050), step 1 finishes with
 * DrawCards(1, 1) when bit 1 is set or runs DuelCursor_Select(who, 0xD, 0), step 2
 * sets the screen busy bit, step 4 clears the screen word at +0x85C and finishes
 * with DrawCards(0, 1). Other steps advance on key 0x100 when DuelScreen_HandleInput()
 * is 0. Both player indexes are written `who & 1`: CSE shares the constant 1, and
 * combine folds the AND only in the first block, as in the ROM. The early return
 * in the default case keeps the per-case step increments from being cross-jumped. */
int DuelPhase_Draw(void)
{
    u8 *e4 = (u8 *)gDuelPlayers;
    struct SB *sb = (struct SB *)(e4 + 0x1B0E);
    u8 *st;
    int z = gDuelPlayers[sb->who & 1].f2;
    if (z != 0) {
        gDuelPlayers[sb->who & 1].f2 = 0;
        return 1;
    }
    st = e4 + 0x1B1C;
    switch (*st) {
    case 0:
        DuelCmd_Push((*(u8 *)sb & 2) ? 0x8050 : 0x50, 0, 0, 0);
        (*st)++;
        return 0;
    case 1:
        if ((*(u8 *)sb & 2) != 0) {
            DrawCards(1, 1);
            return 1;
        }
        DuelCursor_Select(sb->who, 0xD, 0);
        (*st)++;
        return 0;
    case 2:
        ((struct E420Screen *)gDuelScreen)->busy = 1;
        (*st)++;
        return 0;
    case 4:
        ((struct E420Screen *)gDuelScreen)->unk85C = 0;
        DrawCards(0, 1);
        return 1;
    default:
        if (DuelScreen_HandleInput() != 0 || (gMain.keys & 0x100) == 0)
            return 0;
        gDuel[0x1B20]++;
        return 0;
    }
}
/* For every monster zone (0-4) of `player` holding a face-down card (flags6 bit 1) whose card key is 0x16: send message 0x73 (0x8073 for player 1) and run ReturnFieldCardToHand on it. */
void EndPhase_ReturnWickedWormBeast(int player)
{
    int i;
    for (i = 0; i <= 4; i++) {
        int s1 = i * 0x94 + (player & 1) * 0xD64;
        struct DuelZone *zn = (struct DuelZone *)(s1 + 0x0201930C);
        u32 id = ID(zn);
        u32 n = id;
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if (((const u16 *)0x08622AB4)[(u16)id & 0x7FF] == 0x16) {
                u32 msg = 0x73;
                if (player != 0)
                    msg = 0x8073;
                DuelCmd_Push(msg, n, 1, 0);
                ReturnFieldCardToHand(player, i, 0);
            }
        }
    }
}
/* Three-step routine on the step byte 0x020192E0+0x1B22 with a zone counter at +0x1B23. Step 0: needs the player's counter (0x020192E4 halfword) > 0x1F3 and CountFreeMonsterZones(1-p) != 0, then scans zones counter..4 for a face-down card with key 0x228 (advancing the step, twice for player 1 which also sets 0x0201AE60+0x14); step 1 prints a card-name prompt; step 2 sends message 0x43 (0x8043) and MoveFieldCard for the found zone, then advances the counter and finally the step.
 * The loop reads the duel state through the global (not the local e) and recomputes player & 1 at the top of its body: loop.c then hoists the constant 1 first (it later serves the h14 store) and CSE keeps the step value across the h14 store. */
int EndPhase_TransferMushroomMan2(int player)
{
    char buf[0x80];
    u8 *e = gDuel;
    u8 *st = e + 0x1B22;
    switch (*st) {
    case 0: {
        u8 *pe = e + 4;
        s16 base = (player & 1) * 0xD64;
        if (*(u16 *)(base + (int)pe) <= 0x1F3)
            return 1;
        if (CountFreeMonsterZones(1 - player) == 0)
            return 1;
        for (; gDuel[0x1B23] <= 4; gDuel[0x1B23]++) {
            int side = player & 1;
            int s1 = gDuel[0x1B23] * 0x94 + side * 0xD64;
            struct DuelZone *zn = (struct DuelZone *)(s1 + (int)(gDuel + 0x2C));
            u16 id = ID(zn);
            if (id != 0 && (zn->flags6 & 2) != 0 && ((const u16 *)0x08622AB4)[id & 0x7FF] == 0x228) {
                gDuel[0x1B22]++;
                if (player != 0) {
                    gTextBox.h14 = 1;
                    gDuel[0x1B22]++;
                }
                return 0;
            }
        }
        return 1;
    }
    case 1:
        FormatStr(buf, gStrAskTransferControlFmt, gCardNames + (gUnk_08624244[0] << 6));
        TextBoxOpen(0x206, 0x712, 0xB, buf);
        TextBoxSetMenu(1, 0, 0);
        (*st)++;
        return 0;
    case 2:
        if (gTextBox.h14 != 0) {
            u32 msg = 0x43;
            if (player != 0)
                msg = 0x8043;
            DuelCmd_Push(msg, 500, 1, 0);
            MoveFieldCard(player, (u8)player | e[0x1B23] << 8, (u8)(1 - player) | (u8)FindFreeMonsterZone(1 - player) << 8);
        }
        {
            u8 *g = gDuel;
            (*(g + 0x1B23))++;
            if (*(g + 0x1B23) <= 4)
                *(g + 0x1B22) = 0;
            else
                (*(g + 0x1B22))++;
        }
        return 0;
    default:
        return 1;
    }
}
/* Two passes over the face-down monster zones of `player`: the first sets `found` when a monster of level <= 3 with byte +8 bit 0 clear exists (cleared again unless card 0x52A is present on either side; then message 0x73 is sent), the second destroys such monsters (DestroyFieldCard) when found, and sends message 0xA6 (0x80A6) for each other face-down zone. */
extern u8 gDuelZones[];
static inline int Level_E780(u16 id)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}
/* Zone addresses go through the symbol gDuelZones so GCSE keeps one copy of it in sl.
 * Loop 1 computes the zone address twice, player term first: loop.c hoists the 0x94 only
 * in its second pass, so the zone pointer is strength-reduced while i still counts up
 * (a hand-stepped pointer gets a reversed counter). */
void EndPhase_DestroyLowLevelMonsters(int player)
{
    int found = 0;
    int i;
    for (i = 0; i <= 4; i++) {
        struct DuelZoneL *zn = (struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones);
        u32 id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)Level_E780(id) <= 3 && (((struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones))->b8 & 1) == 0)
                found = 1;
        }
    }
    if (CountActiveCardsOnField(0, 0x52A) == 0 && CountActiveCardsOnField(1, 0x52A) == 0)
        found = 0;
    if (found != 0) {
        u16 msg = 0x73;
        if (player != 0)
            msg = 0x8073;
        DuelCmd_Push(msg, gUnk_08624848[0], 1, 0);
    }
    for (i = 0; i <= 4; i++) {
        struct DuelZoneL *zn = (struct DuelZoneL *)(i * 0x94 + (player & 1) * 0xD64 + (int)gDuelZones);
        u16 id = ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)Level_E780(id) <= 3 && (((struct DuelZoneL *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones))->b8 & 1) == 0 && found != 0) {
                DestroyFieldCard(player, i, 1);
            } else {
                u32 msg = 0xA6;
                if (player != 0)
                    msg = 0x80A6;
                DuelCmd_Push(msg, i, 1, 0);
            }
        }
    }
}
int FindFaceUpCardOnField2(int, u16);
int CountFaceUpMonstersByNumber(int, u16);
int CountMonstersFiltered(int, int, int);
int FindFreeSpellTrapZone(int);
int FindZoneLinkFromCard(int, int, u16);
void Chain_AddPending(u32, int);
void DuelPrompt_Post(int, int, u16, int);
void DuelPrompt_PostDiscard(int, int, int, int);
void OnCardDestroyedByEffect(int, int, int);
void PlaceNextSpiritMessage(int, u8);
int EndPhase_TransferMushroomMan2(int);
void EndPhase_DestroyLowLevelMonsters(int);
extern const u16 gUnk_086249C8[], gUnk_08624A0C[];
struct E9Zone {
    u32 card;
    u8 unk4, unk5;
    u8 f6;
    u8 unk7;
    u16 unk8;
    u16 links[0x43];
    u8 unk90;
    u8 b91;
    u8 unk92[2];
};
struct E9Player {
    u16 life; u8 handCount; u8 pad3; u8 graveCount; u8 pad5[7];
    u8 c0 : 4; s8 c4 : 1; u8 c5 : 3; u8 padD[0x904 - 0xD]; u32 grave[(0xD64 - 0x904) / 4];
};
extern struct E9Player gE9PS_020192E4[];
extern u32 gDuelGraveyards[];
struct E9State {
    u32 header; struct E9Player players[2]; u8 pad1ACC[0x1B12 - 0x1ACC]; u8 flags; u8 pad13[0x1B20 - 0x1B13];
    u8 step, zone, cursor, subcursor; u8 pad24[0x1B64 - 0x1B24]; u16 choice;
};
#define E9 ((struct E9State *)gDuel)
#define E9_E gDuel
#define E9_STEP (E9->step)
#define E9_ZONE (E9->zone)
#define E9_PS gE9PS_020192E4
#define E9_ID(z) (((z)->card << 20) >> 20)
#define E9_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
static inline u16 E9CardId(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= 0x7CF) return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
/* Effect dispatcher on the duel step byte (0x020192E0+0x1B20) for the player in flags bit 1:
 * announces and resolves end-of-phase card effects (linked-zone destruction for card 0x60C,
 * the opponent's 0x15B/0x4CE/0x5F8 zones, the 0x5EF prompt, graveyard bit-23 notices, the
 * flagsC bit-4 events for both players), then trims the hand to six (returns 1 when done).
 * Case 10 uses three FAKEMATCH forms, explained at each site: an opaque zone base `zb` from
 * which step++ is formed, a dead product that sets loop.c's insn count (so it hoists
 * player & 1 in its first pass and 0x94, 0xD64 and the side product in its second, as in
 * the ROM), and the table pointer at the top of the loop body (hoisted, left without a hard
 * register and reloaded, which keeps the ROM's reload-register rotation). */
int DuelPhase_End(void)
{
    u32 player = ((u32)gDuel[0x1B12] << 30) >> 31;
    u32 kind;
    switch (gDuel[0x1B20]) {
    case 0: {
        DuelCmd_Push(player ? 0x8055 : 0x55, 0, 0, 0);
        E9_E[0x1B22] = 0;
        E9_E[0x1B23] = 0;
        E9_E[0x1B20]++;
    }
    case 1:
        if ((u16)EndPhase_TransferMushroomMan2(player)) {
            u16 msg = 0x47;
            if (player) msg = 0x8047;
            DuelCmd_Push(msg, 0, 0, 0);
            EndPhase_ReturnWickedWormBeast(player);
            E9_STEP++;
        }
        return 0;
    case 2:
        EndPhase_DestroyLowLevelMonsters(player);
        E9_STEP++;
        return 0;
    case 3: {
        int i;
        for (i = 0; i <= 4; i++) {
            int other = 1 - player;
            struct E9Zone *z = (struct E9Zone *)(i * 0x94 + (other & 1) * 0xD64 + (u32)gDuelZones);
            if (E9_ID(z) && (z->f6 & 2)) {
                int idx = FindZoneLinkFromCard(player, i, 0x60C);
                if (idx >= 0) {
                    u16 link = ((struct E9Zone *)((player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones))->links[idx];
                    u8 who = link;
                    u32 slot = link >> 8;
                    struct E9Zone *row = (struct E9Zone *)((who & 1) * 0xD64 + (u32)gDuelZones);
                    struct E9Zone *lz = &row[slot];
                    if ((lz->f6 & 0x3C) == 8 && !(lz->b91 & 8)) {
                        ReturnFieldCardToHand(who, slot, 0);
                        DestroyFieldCard(other, i, 1);
                        OnCardDestroyedByEffect(player, other, i);
                    }
                }
            }
        }
        E9_STEP++;
        E9_ZONE = 5;
        return 0;
    }
    case 4:
        for (; E9_ZONE <= 9; E9_ZONE++) {
            int other = 1 - player;
            int side = other & 1;
            struct E9Zone *z = (struct E9Zone *)(E9_ZONE * 0x94 + side * 0xD64 + E9_E + 0x2C);
            u32 id = E9_ID(z);
            u8 f;
            if (id && ((f = z->f6) & 2)) {
                switch (E9_NUMBER(id)) {
                case 0x15B:
                    if (((u32)f << 26) >> 28 <= 1) {
                        u16 msg = 0x8A;
                        if (player != 1) msg = 0x808A;
                        DuelCmd_Push(msg, E9_ZONE, 1, 0);
                    } else
                        DestroyFieldCard(other, E9_ZONE, 1);
                    E9_ZONE++;
                    return 0;
                case 0x4CE:
                    if (!(f & 0x3C)) {
                        u16 msg = 0x8A;
                        /* FAKEMATCH: f outlives the AND, so regmove gives its result the
                         * 0x3C register (`ands r0, r3`) instead of f's. */
                        asm("" :: "r"(f));
                        if (player != 1) msg = 0x808A;
                        DuelCmd_Push(msg, E9_ZONE, 1, 0);
                    } else
                        DestroyFieldCard(other, E9_ZONE, 1);
                    E9_ZONE++;
                    return 0;
                case 0x5F8:
                    if (FindFreeSpellTrapZone(other) >= 0
                        && !(((struct E9Zone *)(side * 0xD64 + E9_ZONE * 0x94 + E9_E + 0x2C))->b91 & 8)) {
                        u16 msg = 0x8A;
                        if (player != 1) msg = 0x808A;
                        DuelCmd_Push(msg, E9_ZONE, 1, 0);
                        PlaceNextSpiritMessage(other, E9_ZONE);
                        E9_ZONE++;
                        return 0;
                    }
                    break;
                }
            }
        }
        E9_STEP++;
        return 0;
    case 5: {
        int other = 1 - player;
        if (CountFaceUpMonstersByNumber(other, 0x5EF) && CountMonstersFiltered(player, 1, 0) > 0) {
            DuelPrompt_Post(other, 15, 0x5EF, 0);
            E9_STEP++;
        } else
            E9_STEP = 10;
        return 0;
    }
    case 6:
        if (E9->choice) {
            int other = 1 - player;
            u32 kind;
            int index = FindFaceUpCardOnField2(other, 0x5EF);
            Chain_AddPending(((u32)(other & 1) << 31) | (((index & 31) << 16) | (kind = 0x6400000)) | E9CardId(0x5EF), 0);
        }
        E9_STEP = 10;
        return 0;
    case 10:
        E9_ZONE = 0;
        do {
            /* Declared at the top of the body: loop.c hoists the table address, it gets no
             * hard register and is reloaded at the call (keeps the reload rotation). */
            const u16 *t = gUnk_08624A0C;
            if (CountZoneLinksFromCard(player, E9_ZONE, 0x60C)) {
                u16 idx = FindZoneLinkFromCard(player, E9_ZONE, 0x60C);
                int side = player & 1;
                u32 off = E9_ZONE * 0x94 + side * 0xD64;
                u8 *zb = E9_E + 0x2C;
                u16 link;
                u32 who, slot, wb;
                struct E9Zone *lz;
                /* FAKEMATCH: zb is opaque, so step++ below is formed as (base+0x2C)+0x1AF4
                 * and stays in the loop; otherwise CSE relates it to the loop-top symbol
                 * and loop.c hoists it. */
                asm("" : "+r"(zb));
                link = ((struct E9Zone *)(off + (u32)zb))->links[idx];
                who = (u8)link;
                slot = link >> 8;
                wb = who & 1;
                lz = (struct E9Zone *)(slot * 0x94 + wb * 0xD64 + (u32)zb);
                if (!(lz->b91 & 8)) {
                    if (!(lz->f6 & 0x3C)) {
                        /* FAKEMATCH: dead product, deleted by flow before register allocation;
                         * it only raises loop.c's insn count to the ROM's (127 insns). */
                        u16 msg = who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who;
                        msg = 0x8A;
                        if (who) msg = 0x808A;
                        DuelCmd_Push(msg, slot, 1, 0);
                    } else {
                        ShowCardEffect(player, t[0]);
                        ReturnFieldCardToHand(who, slot, 0);
                        ((struct E9State *)(zb - 0x2C))->step++;
                        return 0;
                    }
                }
            }
            E9_ZONE++;
        } while (E9_ZONE <= 4);
        E9_STEP = 20;
        return 0;
    case 11:
        DestroyFieldCard(player, E9_ZONE, 1);
        E9_ZONE++;
        E9_STEP = 10;
        return 0;
    case 20: {
        int i;
        for (i = 0; i < E9_PS[player & 1].graveCount; i++) {
            u32 *row = (u32 *)((player & 1) * 0xD64 + (u32)gDuelGraveyards);
            u32 *p = row + i;
            u32 card = *p;
            if ((s32)(card << 8) < 0) {
                u16 msg = 0xD2;
                if (player) msg = 0x80D2;
                DuelCmd_Push(msg, card, card >> 16, 0);
            }
        }
        E9_STEP++;
        return 0;
    }
    case 21:
        if (E9_PS[player & 1].c4 < 0) {
            u16 msg = 0x4C;
            if (player) msg = 0x804C;
            DuelCmd_Push(msg, 0, 0, 0);
            if (CountMonstersFiltered(1 - player, 1, 0) > 0) {
                const u16 *t = gUnk_086249C8;
                Chain_AddPending(((player & 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        E9_STEP++;
        return 0;
    case 22:
        if (E9_PS[player ^ 1].c4 < 0) {
            u16 msg = 0x4C;
            if (player != 1) msg = 0x804C;
            DuelCmd_Push(msg, 0, 0, 0);
            if (CountMonstersFiltered(player, 1, 0) > 0) {
                const u16 *t = gUnk_086249C8;
                Chain_AddPending(((player ^ 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        E9_STEP++;
        return 0;
    default:
        if (CountActiveCardsOnField(0, 0x593) <= 0 && CountActiveCardsOnField(1, 0x593) <= 0) {
            if (E9_PS[player].handCount > 6)
                DuelPrompt_PostDiscard(player, E9_PS[player].handCount - 6, 0, 0);
        }
        return 1;
    }
}
