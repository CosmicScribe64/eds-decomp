#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

/* 20-byte action entry (lists in gChain; see code_08011BE0). */
struct ActEntry {
    u16 card;           /* +0x00: card ID in bits 0-10 */
    u16 flag2_0:1;      /* +0x02 bit 0 */
    u16 kind2:3;        /* +0x02 bits 1-3 */
    u16 val2_4:6;       /* +0x02 bits 4-9 */
    u16 val2_10:6;      /* +0x02 bits 10-15 */
    u8 flag4_0:1;       /* +0x04 bits 0-4 */
    u8 flag4_1:1;
    u8 flag4_2:1;
    u8 flag4_3:1;
    u8 flag4_4:1;
    u8 unk4_5:3;
    u8 unk5;
    u16 w6;             /* +0x06: low byte = player (hypothesis) */
    u16 w8;             /* +0x08 */
    u8 fillerA[0x14 - 0xA];
};

/* 0x02017A40: two entry lists. */
struct ActLists {
    struct ActEntry listA[32];  /* +0x000 */
    struct ActEntry listB[16];  /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u16 unk3C2;
    u16 countA;                 /* +0x3C4 */
};
extern struct ActLists gChain;

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gDuelCtrl;

/* Link state at 0x02017FB0 (fields used here; u32 containers, see code_08021CC8). */
struct LinkState {
    u8 filler0[0x200];
    u16 unk200;         /* +0x200 */
    u16 waitTimer;      /* +0x202 */
    u8 filler204[0x304 - 0x204];
    u32 unk304:24;
    u32 unk307_0:1;     /* +0x307 bit 0 */
    u32 unk307_1:1;     /* +0x307 bit 1 */
    u32 unk307_2:1;
    u32 unk307_3:1;     /* +0x307 bit 3 */
    u32 unk307_4:4;
    u8 filler308[0x484 - 0x308];
    u16 cmd[4];         /* +0x484: duel command received from the partner */
    u8 step48C;         /* +0x48C */
};

extern struct LinkState gLinkState;

/*
 * duel.h's struct DuelState is documented only up to +0x1B20, but this unit also reads/writes cmdStep
 * at +0x1B40. Unit-local view: the canonical struct from duel.h plus the extra tail byte. It must be a
 * single symbol, otherwise the extra reference adds a literal-pool entry and changes DuelCmdQueue_Run.
 */
struct DuelStateUnit {
    struct DuelState duel;                          /* canonical fields (duel.h) */
    u8 pad[0x1B40 - sizeof(struct DuelState)];
    u8 cmdStep;                                     /* +0x1B40 */
};
extern struct DuelStateUnit gUnk_020192E0u asm("gDuel");

extern u8 gAiState[2];

struct Unk0201AE60 {
    u8 filler0[0x22];
    u8 step22;          /* +0x22 */
    u8 timer23;         /* +0x23 */
};
extern struct Unk0201AE60 gTextBox;

extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */
#define CARD_STATS(id) (gCardStats[(id) & 0x7FF])
#define CARD_NUMBER(id) (gCardIdToNumber[(id) & 0x7FF])
/* Through a constant address: GCC then loads the mask before the table. */
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE_C(id) ((CARD_STATS_C(id) & 0x1F00000) >> 20)
#define CARD_STATS_X(id) CARD_STATS_C(id)
#define CARD_TYPE_X(id) ((CARD_STATS_X(id) & 0x1F00000) >> 20)

void MemCopy16(void *dst, const void *src, u32 size);

/* Magic/Trap subtype (stats bits 17-19) of a Magic or Trap card, else 0 (as in code_08009A68). */
static inline int GetMagicSubtype(u16 id)
{
    u32 stats = CARD_STATS_X(id);

    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}
/* a: bits 0-15 card, 16-20 val, 21-24 kind, 25-30 val2, 31 flag (explicit masks in the ROM) */
void Chain_Add(u16 toB, u32 a, u32 b);
u16 DuelLink_SendMessageData(u16 head, const void *src, int size);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void SetupStartFieldCard(void);
void MemClear16(void *dst, u32 size);    /* zero fill */
void FadeOutBGM(void);
void DuelCmd_Dispatch(void);
void PlayDuelBGM(void);
u16 DuelLink_SendMessage(u16 a, u16 b, u16 c, u16 d);
void AddCardNumberToDeckTop(int player, u16 number);
extern const u16 gCardNumberToId[];   /* card number to card id */
#define CARD_ID_TABLE ((const u16 *)0x08623DF4)

/* Card number to card id; 0xFFFF maps to 0 (this copy does not mask numbers below 2000). */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n < 2000)
        return *(CARD_ID_TABLE + n);
    return *(CARD_ID_TABLE + ((n - 2000) & 0x7FF)) + 1;
}
u32 CanChainFieldCard(struct ActEntry *e, int player, int zone);
u32 CanChainHandCard(struct ActEntry *e, int player, int zone);
int CountActiveCardsOnField(int player, u16 number);

extern u8 gCardListView[];
extern u8 gSummonAction[];
extern u8 gDuelScene[];
extern u8 gBattle[];

#define gMain gMain

/* Run the duel command queue: pop the next command into the command block and run it. */
u32 DuelCmdQueue_Run(void)
{
    int i;

    switch (gUnk_020192E0u.cmdStep) {
    case 0:
        if (gDuelCmd.queueCount == 0)
            return 0;
        MemCopy16(&gDuelCmd, &gDuelCmd.queue[0], 8);
        gDuelCmd.queueCount--;
        for (i = 0; i < gDuelCmd.queueCount; i++)
            MemCopy16(&gDuelCmd.queue[i], &gDuelCmd.queue[i + 1], 8);
        gLinkState.unk307_1 = 1;
        gDuelCmd.running = 1;
        if (gDuelCtrl.link) {
            DuelLink_SendMessageData(0xF041, &gDuelCmd, 8);
            gLinkState.unk307_1 = 0;
            gLinkState.unk200 = 0;
            gLinkState.waitTimer = 0;
        }
        gDuelCmd.step = 0;
        gDuelCmd.timer = 0;
        gUnk_020192E0u.cmdStep++;
    case 1:
        DuelCmd_Dispatch();
        if (gDuelCmd.running)
            return 1;
        gUnk_020192E0u.cmdStep++;
    case 2:
        if (!gLinkState.unk307_1) {
            if (++gLinkState.waitTimer < 0x78)
                return 1;
            gLinkState.waitTimer = 0;
            DuelLink_SendMessage(0xEE00, 0, 0, 0);
            FadeOutBGM();
            gUnk_020192E0u.duel.linkError = 1;
            return 0;
        }
        gLinkState.waitTimer = 0;
        if ((gDuelCmd.cmd & 0xFFF) != 4)
            PlayDuelBGM();
        gUnk_020192E0u.cmdStep = 0;
        if (gDuelCmd.queueCount)
            return 1;
        break;
    }
    return 0;
}
/* Link duel: run a duel command received from the partner (0x02017FB0+0x484). */
u32 DuelCmd_RunRemote(void)
{
    switch (gLinkState.step48C) {
    case 0:
        MemCopy16(&gDuelCmd, gLinkState.cmd, 8);
        gDuelCmd.running = 1;
        gDuelCmd.step = 0;
        gDuelCmd.timer = 0;
        gLinkState.step48C++;
    case 1:
        DuelCmd_Dispatch();
        if (!gDuelCmd.running)
            gLinkState.step48C++;
        else if ((gMain.frameCounter & 0xF) == 0)
            DuelLink_SendMessage(0xF042, 0, 0, 0);
        return 1;
    case 2:
        if ((gDuelCmd.cmd & 0xFFF) != 4)
            PlayDuelBGM();
        DuelLink_SendMessage(0xF043, 0, 0, 0);
        gLinkState.step48C++;
        return 1;
    }
    gLinkState.unk307_0 = 0;
    return 0;
}
/* Duel setup: clear all duel work areas. */
u32 Duel_Setup(void)
{
    MemClear16(&gDuelCtrl, 8);
    MemClear16(&gDuel, 0x1B78);
    MemClear16(&gDuelScreen, 0x860);
    MemClear16(gCardListView, 0x310);
    MemClear16(&gTextBox, 0x2124);
    MemClear16(&gDuelCmd, 0xD20);
    MemClear16(gSummonAction, 0x14);
    MemClear16(&gChain, 0x56C);
    MemClear16(gDuelScene, 0x10);
    MemClear16(&gLinkState, 0x494);
    MemClear16(gBattle, 0x160);
    gDuel.result = 3;
    FadeOutBGM();
    if (gMain.heldKeys & 0x300)
        gDuelScreen.fast = 1;
    return 1;
}
void SetupStartFieldCard(void)
{
    u16 number;

    switch (gMain.unk488A_0) {
    case 1:
        number = 0x149;
        break;
    case 2:
        number = 0x14A;
        break;
    case 3:
        number = 0x14B;
        break;
    case 4:
        number = 0x14C;
        break;
    case 5:
        number = 0x14D;
        break;
    case 6:
        number = 0x14E;
        break;
    case 7:
        number = 0x42D;
        break;
    case 8:
        number = 0x465;
        break;
    case 9:
        number = 0x466;
        break;
    case 10:
        number = 0x467;
        break;
    case 11:
        number = 0x468;
        break;
    case 12:
        number = 0x469;
        break;
    case 13:
        number = 0x46A;
        break;
    default:
        return;
    }
    AddCardNumberToDeckTop(1, number);
    DuelCmd_Push(0x8061, 0, 1, 0);
    DuelCmd_Push(0x80C5, CardNumberToId(number), 0x10A, 0);
    DuelCmd_Push(0x8011, gMain.unk488A_0, 1, 0);
}
/* Duel phase 1 (entry 1 of the phase table 0x08198F80). */
u32 DuelPhase_Opening(void)
{
    switch (gDuel.phaseStep) {
    case 0:
        DuelCmd_Push(0x10, 0, 0, 0);
        DuelCmd_Push(0x12, 0, 0, 0);
        SetupStartFieldCard();
        gDuel.phaseStep++;
        return 0;
    case 1:
        DuelCmd_Push(gDuel.linkSkip ? 0x8061 : 0x61, 0, 5, 0);
        DuelCmd_Push(!gDuel.linkSkip ? 0x8061 : 0x61, 0, 5, 0);
        DuelCmd_Push(0x14, 0, 0, 0);
        gDuel.phaseStep++;
        return 0;
    default:
        if (gDuel.linkSkip) {
            gAiState[0] = 0;
            gAiState[1] = 0;
            gDuelCtrl.phase = 8;
            break;
        }
        return 1;
    }
    return 0;
}
u32 Chain_IsPartnerEntry(struct ActEntry *e)
{
    if (gDuelCtrl.link && e->flag2_0 && CARD_NUMBER_C(e->card) != 0x3B6)
        return 1;
    return 0;
}
void Chain_Add(u16 toB, u32 a, u32 b)
{
    struct ActEntry *e;
    u16 m[5];

    if (!(a & 0xFFFF))
        return;
    if (gDuelCtrl.link && gDuel.linkSkip) {
        /* forward to the link partner, from its point of view */
        if (a & 0x80000000)
            a &= ~0x80000000;
        else
            a |= 0x80000000;
        m[0] = toB;
        m[1] = a;
        m[2] = a >> 16;
        m[3] = b;
        m[4] = b >> 16;
        DuelLink_SendMessageData(0xF072, m, 10);
        return;
    }
    if (toB)
        e = &gChain.listB[gChain.countB];
    else
        e = &gChain.listA[gChain.countA];
    e->card = a;
    e->flag2_0 = a >> 31;
    e->kind2 = (a & 0x1E00000) >> 21;
    e->val2_4 = (a & 0x1F0000) >> 16;
    e->flag4_0 = 0;
    e->flag4_1 = 0;
    e->flag4_2 = 0;
    e->flag4_3 = 0;
    e->flag4_4 = 0;
    e->val2_10 = (a & 0x7E000000) >> 25;
    e->w6 = b;
    e->w8 = b >> 16;
    if (toB)
        gChain.countB++;
    else
        gChain.countA++;
}
void Chain_AddPending(u32 b, u32 c)
{
    Chain_Add(0, b, c);
}
void Chain_AddLink(u32 b, u32 c)
{
    Chain_Add(1, b, c);
}
void Chain_AddPartnerEntry(u16 toB, struct ActEntry *src)
{
    struct ActEntry *e;
    u16 v;

    if (toB)
        e = &gChain.listB[gChain.countB];
    else
        e = &gChain.listA[gChain.countA];
    MemCopy16(e, src, 0x14);
    e->flag2_0 = 1;
    e->flag4_0 = 1;
    e->flag4_1 = 1;
    e->flag4_2 = 0;
    e->flag4_3 = 0;
    e->flag4_4 = 0;
    v = src->w6;
    e->w6 = (u8)(1 - v) | ((v >> 8) << 8);
    v = src->w8;
    e->w8 = (u8)(1 - v) | ((v >> 8) << 8);
    if (toB)
        gChain.countB++;
    else
        gChain.countA++;
}
u32 Chain_CardGoesToGrave(struct ActEntry *e)
{
    u32 ret = 1;
    u32 stats, type;
    int sub;

    if (e->kind2 == 3 && ((CARD_STATS_C(e->card) & 0x1F00000) >> 20) != 21)
        return 0;
    stats = CARD_STATS_C(e->card);
    type = (stats & 0x1F00000) >> 20;
    if (type <= 20)
        return 0;
    switch ((int)type) {
    case 21:
    case 22:
        sub = (stats & 0xE0000) >> 17;
        break;
    default:
        sub = 0;
        break;
    }
    switch (sub) {
    case 2:
    case 3:
    case 4:
        goto flag;
    }
    /* This empty compiler barrier preserves the ROM's card reload after the
     * subtype test. It emits no instructions and does not modify memory. */
    asm volatile ("" : : : "memory");
    switch (CARD_NUMBER_C(e->card)) {
    case 0x47:
    case 0x15B:
    case 0x4CE:
    case 0x4EA:
    case 0x609:
    flag:
        ret = e->flag4_3;
        break;
    }
    return ret;
}
/*
 * Unit-local zone view for Chain_GetResponseCommands. duel.h's struct DuelZone.card is a struct DuelCard.
 * Reading its 12-bit id makes agbcc emit a halfword load, but the ROM loads the whole u32 card
 * word and masks it (card << 20 >> 20). This view keeps a raw u32 card (ID in bits 0-11) so the
 * function matches. All other code in the unit uses the canonical struct DuelZone from duel.h.
 */
struct DuelZoneUnit {
    u32 card;           /* +0x00: card word, ID in bits 0-11 */
    u8 unk4[2];
    u8 unk6_0:1;
    u8 faceUp:1;        /* +0x06 bit 1 */
    u8 unk6_2:6;
    u8 filler7[0x94 - 7];
};
u32 Chain_GetResponseCommands(struct ActEntry *e, int player, int kind, int zone)
{
    u32 ret = 1;
    struct DuelZoneUnit *z;
    u32 id;

    switch (kind) {
    case 11:
        if (CanChainHandCard(e, player, zone))
            ret = 0x41;
        break;
    case 5:
        if (CanChainFieldCard(e, player, zone + 5))
            ret = 0x41;
        break;
    case 0:
        player &= 1; z = (struct DuelZoneUnit *)((u8 *)gDuelZones + (zone * 0x94 + player * 0xD64));
        id = z->card << 20 >> 20;
        if (id != 0 && z->faceUp && CARD_NUMBER_C(id) == 0x5F5 && CARD_TYPE_C(e->card) == 22
            && !CountActiveCardsOnField(0, 0x58A) && !CountActiveCardsOnField(1, 0x58A) && CARD_NUMBER_C(e->card) != 0x603)
            ret = 0x41;
        break;
    }
    return ret;
}
u32 sub_0801FE54(void)
{
    struct Unk0201AE60 *base = &gTextBox;
    u8 *p;
    u32 step;
    u32 copy;
    p = &base->step22;
    step = *p;
    copy = step;
    /* FAKEMATCH: keep the initialized step distinct from the switch copy.
     * This empty constraint emits no instructions and prevents case-value folding. */
    asm volatile ("" : "+r"(step));
    switch (copy) {
    case 0:
        if (!gLinkState.unk307_3)
            break;
        goto next;
    case 1:
        if (base->timer23 <= 59) {
            base->timer23++;
            break;
        }
        goto next;
    default:
        return 1;
    next:
        *p = step + 1;
        break;
    }
    return 0;
}

/* Selection widget at 0x020192E0+0x1B2C (layout from code_0801CE68). */
struct SelFEA0 {
    u16 flag0:1;
    u16 active:1;
    u16 cursor:4;
    u16 rows:4;
    u32 mask:16;        /* bits 10-25 */
    u32 state:8;        /* bits 26-33 */
    u32 unk34:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;       /* bit 57: copy of 0x0201CFB0+0x824 */
    u32 zone:7;         /* bits 58-64: copy of 0x0201CFB0+0x828 */
    u16 unk65:8;        /* bits 65-72: copy of 0x0201CFB0+0x82C */
    u16 unk73:7;
};
struct DuelFEA0 {
    u32 unk0;
    struct DuelPlayer players[2];
    u8 pad1ACC[0x1B12 - 0x1ACC];
    u8 f1B12_0:1;
    u8 linkSkip:1;
    u8 f1B12_2:6;
    u8 pad1B13[0x1B28 - 0x1B13];
    u16 selCard;        /* +0x1B28 */
    u16 unk1B2A;
    struct SelFEA0 sel; /* +0x1B2C */
};
/* Address-suffixed alias names (autosyms resolves them to the address). */
extern struct DuelFEA0 gAliasFEA0_020192E0;
#define gDuelFEA0 gAliasFEA0_020192E0
struct ZFEA0 { u32 card; u8 unk4, unk5, flags6; u8 unk7[0x94 - 7]; };
#define ZN_FEA0(p, z) ((struct ZFEA0 *)((p) * 0xD64 + (z) * 0x94 + (u32)((u8 *)&gDuelFEA0 + 0x2C)))
struct ALFEA0 {
    u8 pad[0x488];
    u8 flag488_0:1;
    u8 f488_1:7;
    u8 pad489[0x490 - 0x489];
    u8 step;            /* +0x490 */
    u8 f491_lo:6;
    u8 f491_6:1;
    u8 f491_7:1;
};
extern struct ALFEA0 gAliasFEA0_02017A40;
#define gALFEA0 gAliasFEA0_02017A40
struct AE60FEA0 { u8 pad[0x14]; u16 unk14; };
extern struct AE60FEA0 gAliasFEA0_0201AE60;
#define gAE60FEA0 gAliasFEA0_0201AE60
extern u8 gStrChainPromptEffect[];
extern u8 gStrChainPromptCard[];
/* Card names, 64 bytes each; through a constant address so the table base is reloaded per use. */
#define NAME_FEA0(id) (((const u8 (*)[0x40])0x0822C720)[id])
void CardMenu_Update(void);
void CardListView_Open(int player, int area, int a2, int a3);
void CardMenu_PlaySpellTrapFromHand(u16 a, u16 b, struct ActEntry *e);
u32 DuelCursor_PickTarget(u32 keys);
u32 AiTryChainResponse(struct ActEntry *e);
u16 DuelCursor_GetCardId(void);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
void FormatStr(void *dst, const void *a, const void *b);
void PlaySE(u16 id);
void Chain_AddLink(u32 b, u32 c);

/*
 * Card-target request step machine on 0x02017A40+0x490 (same shape as EventResponse_Run): show the card's
 * prompt (1), wait (2), let the player pick with the selection widget at 0x020192E0+0x1B2C (10/11) and add
 * the chosen zone to list B; when player != 0 hand off to the link (100/101) or to AiTryChainResponse (200).
 * The switch on (u8)st keeps the loaded step in its own register so the step++ in case 10 stays unfolded.
 */
s32 Chain_AskResponse(struct ActEntry *e, u32 player)
{
    u8 buf[0x200];
    u16 card;
    int st = gALFEA0.step;

    switch ((u8)st) {
    case 0:
        if (!gALFEA0.flag488_0) {
            gALFEA0.flag488_0 = 1;
            DuelCmd_Push(0x12, 0, 0, 0);
            gALFEA0.step++;
            return 0;
        }
        gALFEA0.step++;
    case 1:
        if (player) {
            u8 s;
            if (gDuelCtrl.link)
                s = 100;
            else
                s = 200;
            gALFEA0.step = s;
            return 0;
        }
        card = e->card;
        if (CARD_TYPE_C(card) <= 20)
            FormatStr(buf, gStrChainPromptEffect, NAME_FEA0(card));
        else
            FormatStr(buf, gStrChainPromptCard, NAME_FEA0(card));
        TextBoxOpen(0x206, 0x712, 11, buf);
        TextBoxSetMenu(1, 0, 0);
        gALFEA0.step++;
        return 0;
    case 2:
        if (gAE60FEA0.unk14 == 0)
            return 1;
        gALFEA0.step = 10;
        gDuelFEA0.sel.flag0 = 0;
        gDuelFEA0.sel.active = 0;
        return 0;
    case 10:
        if (gDuelFEA0.sel.flag0) {
            CardMenu_Update();
            return 0;
        }
        if (gDuelFEA0.sel.active) {
            gALFEA0.step++;
            return 0;
        }
        if (gMain.newKeys & 2) {
            gALFEA0.step = 1;
            return 0;
        }
        if (DuelCursor_PickTarget(!gDuelFEA0.linkSkip ? 0xF : 0xEE) == 0)
            return 0;
        {
            u32 p = gDuelScreen.player;
            u32 kind = gDuelScreen.zone;
            u32 cur = gDuelScreen.cursor;
            u16 id = DuelCursor_GetCardId();
            switch (kind) {
            case 0:
            case 5:
            case 10:
            case 11:
                if (id != 0) {
                    gDuelFEA0.sel.flag0 = 1;
                    gDuelFEA0.sel.state = 0;
                    gDuelFEA0.sel.mask = (u16)Chain_GetResponseCommands(e, p, kind, cur);
                    return 0;
                }
                PlaySE(3);
                return 0;
            case 13:
                PlaySE(3);
                return 0;
            case 12:
            case 14:
            case 15:
                CardListView_Open(p, kind, 0, 0);
                PlaySE(1);
                return 0;
            }
        }
        return 0;
    case 11:
        switch (gDuelFEA0.sel.zone) {
        case 11:
            CardMenu_PlaySpellTrapFromHand(1, 1, e);
            if (gDuelFEA0.sel.active)
                return 0;
            break;
        case 0:
        case 5:
            gDuelFEA0.sel.active = 0;
            {
                u32 p = 1 & gDuelFEA0.sel.player;
                if (!(ZN_FEA0(p, gDuelFEA0.sel.unk65 + gDuelFEA0.sel.zone)->flags6 & 2)) {
                    u16 msg;
                    if (gDuelFEA0.sel.player)
                        msg = 0x807F;
                    else
                        msg = 0x7F;
                    DuelCmd_Push(msg, gDuelFEA0.sel.unk65 + gDuelFEA0.sel.zone, 0, 0);
                }
            }
            {
                u32 hi = ((1 & gDuelFEA0.sel.player) << 31) | (e->val2_10 << 25);
                u32 z = (((gDuelFEA0.sel.unk65 + gDuelFEA0.sel.zone) & 0x1F) << 16) | 0x200000;
                Chain_AddLink(hi | z | gDuelFEA0.selCard, (e->w8 << 16) | e->w6);
            }
            break;
        }
        gALFEA0.f491_6 = 0;
        gALFEA0.f491_7 = 1;
        return 1;
    case 100:
        DuelLink_SendMessageData(0xF054, e, 0x14);
        gALFEA0.f491_7 = 0;
        gLinkState.unk307_3 = 0;
        gALFEA0.step++;
        return 0;
    case 101:
        return gLinkState.unk307_3;
    case 200:
        if (AiTryChainResponse(e))
            gALFEA0.f491_7 = 1;
        break;
    }
    return 1;
}
/*
 * Unit-local views for Chain_Build. The ROM tests the entry flags at +4 and the link flags at
 * 0x02017FB0+0x308 as u32-container bitfields (lsl/sign tests, as in code_08020AF4), so these
 * views differ from struct ActEntry / struct LinkState above.
 */
struct Ent0330 {
    u16 card;                   /* +0x00 */
    u16 flag2_0:1;              /* +0x02 bit 0: player (hypothesis) */
    u16 rest2:15;
    u32 flag4_0:1;              /* +0x04 bit 0: skip handler A */
    u32 flag4_1:1;              /* +0x04 bit 1: skip handler B */
    u32 rest4:14;
    u16 w6;
    u16 w8;
    u8 fillerA[0x14 - 0xA];
};
typedef u16 (*Fn0330)(struct Ent0330 *e, struct Ent0330 *prev);
struct St0330 {
    struct Ent0330 listA[32];
    struct Ent0330 listB[16];   /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u8 pad3C2[0x3D0 - 0x3C2];
    u8 active:1;                /* +0x3D0 bit 0: Chain_Update runs this function */
    u8 step:7;
    u8 idx;                     /* +0x3D1: list B entry being resolved */
    u8 b3D2;                    /* +0x3D2: code_08020AF4's resolveFlags */
    u8 b3D3;
    u8 pad3D4[0x3E4 - 0x3D4];
    u8 b3E4;
    u8 b3E5;
    u8 pad3E6[0x480 - 0x3E6];
    Fn0330 fnA;                 /* +0x480 */
    Fn0330 fnB;                 /* +0x484 */
    u8 b488_0:1;
    u8 b488_1:7;
    u8 pad489[0x490 - 0x489];
    u8 b490;
    u8 b491_lo:4;
    u8 b491_mid:3;
    u8 b491_hi:1;               /* +0x491 bit 7: restart from step 1 */
};
#define gSt0330 (*(struct St0330 *)&gChain)
struct Lnk0330 {
    u8 filler0[0x308];
    u32 f0:1, f1:1, f2:1, f3:1, f4:1, f5:1, f6:1, f7:1;     /* +0x308 */
};
#define gLnk0330 (*(struct Lnk0330 *)&gLinkState)
/* Effect table, 24-byte entries: handlers A and B at +0x10 / +0x14. */
struct EffDef0330 { u8 pad[0x10]; Fn0330 fnA; Fn0330 fnB; };
extern const struct EffDef0330 gCardEffects[];
/* 0x02017CC0 is list B; its +0x150 is 0x02017A40+0x3D0, the step byte. */
struct Step150 { u8 pad[0x150]; u8 flag : 1; u8 step : 7; };
extern struct Step150 gUnk_02017CC0;
s32 FindCardEffect(u16 card);
void sub_080197C0(int player, u16 card);
void ChainListScreen_Start(void *p, int a);
s32 ChainListScreen_Run(void);
s32 Chain_AskResponse(struct ActEntry *e, u32 player);
s32 CanPlayerChain(struct ActEntry *e, u32 player);
#define S gSt0330
#define LAST (S.listB[S.countB - 1])
/*
 * Resolves list B one step per call: for each entry look up its two effect handlers
 * (gCardEffects via FindCardEffect), run A then B until each reports done (0x02017FB0+0x308
 * bits 3 and 1), then send the list over the link, hand it to ChainListScreen_Start, and run the
 * per-player checks CanPlayerChain / Chain_AskResponse for the last entry (hypothesis).
 * Every case ends in its own `return 1`: the jump to the new return label is not cross-jumped,
 * so each case keeps its own copy of the step++ tail, as in the ROM.
 */
int Chain_Build(void)
{
    u32 r; /* one temporary for the effect index and the list count, as in the ROM (r4) */

    switch (S.step) {
    case 0:
        S.idx = 0;
        S.step++;
    case 1:
        r = FindCardEffect(S.listB[S.idx].card);
        if (r == -1) {
            S.fnA = NULL;
            S.fnB = NULL;
        } else {
            S.fnA = gCardEffects[r].fnA;
            S.fnB = gCardEffects[r].fnB;
        }
        if (S.listB[S.idx].flag4_0)
            S.fnA = NULL;
        if (S.listB[S.idx].flag4_1)
            S.fnB = NULL;
        sub_080197C0(S.listB[S.idx].flag2_0, S.listB[S.idx].card);
        gLnk0330.f1 = 0;
        gLnk0330.f3 = 0;
        S.b3E4 = 0;
        S.b3E5 = 0;
        S.step++;
        return 1;
    case 2:
        if (S.fnA == NULL) {
            S.step += 2;
            return 1;
        }
        if ((u16)Chain_IsPartnerEntry((struct ActEntry *)&S.listB[S.idx]))
            DuelLink_SendMessageData(0xF091, &S.listB[S.idx], 0x14);
        S.step++;
    case 3:
        if ((u16)Chain_IsPartnerEntry((struct ActEntry *)&S.listB[S.idx]) == 0) {
            {
                u16 *cnt = &S.countB;
                /* FAKEMATCH: keeps r0/r1 busy while the count address is live, so local-alloc
                 * puts it in r2 as the ROM does. Emits no instructions. */
                asm volatile("" ::: "r0", "r1");
                r = *cnt;
            }
            if (r > 1) {
                if (S.fnA(&S.listB[S.idx], &S.listB[r - 2]))
                    gLnk0330.f3 = 1;
            } else {
                if (S.fnA(&S.listB[S.idx], NULL))
                    gLnk0330.f3 = 1;
            }
        }
        if (gLnk0330.f3)
            S.step++;
        return 1;
    case 4:
        if (S.fnB == NULL) {
            S.step += 2;
            return 1;
        }
        if ((u16)Chain_IsPartnerEntry((struct ActEntry *)&S.listB[S.idx]))
            DuelLink_SendMessageData(0xF081, &S.listB[S.idx], 0x14);
        S.step++;
    case 5:
        if ((u16)Chain_IsPartnerEntry((struct ActEntry *)&S.listB[S.idx]) == 0) {
            {
                u16 *cnt = &S.countB;
                /* FAKEMATCH: as in case 3. */
                asm volatile("" ::: "r0", "r1");
                r = *cnt;
            }
            if (r > 1) {
                if (S.fnB(&S.listB[S.idx], &S.listB[r - 2]))
                    gLnk0330.f1 = 1;
            } else {
                if (S.fnB(&S.listB[S.idx], NULL))
                    gLnk0330.f1 = 1;
            }
        }
        if (gLnk0330.f1)
            S.step++;
        return 1;
    case 6:
        S.idx++;
        if (S.idx < S.countB) {
            S.step = 1;
            return 1;
        }
        S.step++;
    case 7:
        if (gDuelCtrl.link) {
            int i;
            u16 buf[0x80];
            DuelLink_SendMessage(0xF061, S.countB, 0, 0);
            for (i = 0; i < S.countB; i++) {
                buf[0] = i;
                MemCopy16(buf + 1, &S.listB[i], 0x14);
                DuelLink_SendMessageData(0xF062, buf, 0x16);
            }
            DuelLink_SendMessage(0xF063, S.countB, 0, 0);
            gLnk0330.f7 = 0;
        }
        S.step++;
        return 1;
    case 8:
        ChainListScreen_Start(&gUnk_02017CC0, 0);
        gUnk_02017CC0.step++;
        return 1;
    case 9:
        if (ChainListScreen_Run())
            S.step++;
        return 1;
    case 10:
        if (gDuelCtrl.link && !gLnk0330.f7)
            return 1;
        S.b488_0 = 0;
        S.step++;
    case 11:
        if (CanPlayerChain((struct ActEntry *)&LAST, 1 - LAST.flag2_0)) {
            S.b490 = 0;
            S.b491_lo = 0;
            S.b491_hi = 0;
        } else {
            S.step++;
        }
        S.step++;
        return 1;
    case 12:
        if ((u16)Chain_AskResponse((struct ActEntry *)&LAST, 1 - LAST.flag2_0)) {
            if (S.b491_hi)
                S.step = 1;
            else
                S.step++;
        }
        return 1;
    case 13:
        if (CanPlayerChain((struct ActEntry *)&LAST, LAST.flag2_0)) {
            S.b490 = 0;
            S.b491_hi = 0;
        } else {
            S.step++;
        }
        S.step++;
        return 1;
    case 14:
        if ((u16)Chain_AskResponse((struct ActEntry *)&LAST, LAST.flag2_0)) {
            if (S.b491_hi)
                S.step = 1;
            else
                S.step++;
        }
        return 1;
    default:
        S.active = 0;
        /* FAKEMATCH: a plain byte store through a pointer. As a field store, the field-insert
         * expansion leaves a zero constant that CSE hoists for the next store. */
        *(u8 *)&S.b3D2 = 1;
        S.b3D3 = 0;
        return 1;
    }
}
#undef S
#undef LAST
#undef gSt0330
#undef gLnk0330
