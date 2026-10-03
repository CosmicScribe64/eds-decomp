#include "global.h"

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4 (fields used here). */
struct DuelPlayer {
    u16 lifePoints;     /* +0x000 */
    u8 filler2[5];
    u8 deckOut:1;       /* +0x007 bit 0: lost by drawing from an empty deck (hypothesis) */
    u8 winA:1;          /* +0x007 bit 1: HasExodiaInHand (all five zones 0x10-0x14) */
    u8 winExodia:1;     /* +0x007 bit 2: HasDestinyBoardComplete (five special cards, hypothesis: Exodia) */
    u8 unk7_3:5;
    u8 filler8[0xD64 - 8];
};
extern struct DuelPlayer gDuelPlayers[2];

/* Selection widget at 0x020192E0+0x1B2C (layout from code_0801CE68). */
struct SelMask {
    u16 flag0:1;        /* bit 0 */
    u16 active:1;       /* bit 1 */
    u16 cursor:4;       /* bits 2-5 */
    u16 rows:4;         /* bits 6-9 */
    u32 mask:16;        /* bits 10-25 */
    u16 state:8;        /* bits 26-33 (straddles 0x1B2F/0x1B30) */
    u32 unk34:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;
    u32 zone:7;
    u32 unk65:8;
    u32 unk73:23;
};

/* 0x020192E0 duel state (fields used here). */
struct DuelState {
    u32 unk0;
    struct DuelPlayer players[2];   /* +0x004 (= 0x020192E4) */
    union { u8 raw; struct { u8 low:4; u8 over:1; u8 high:3; } __attribute__((packed)) bits; } __attribute__((packed)) flags1ACC;
    u8 effectFlags;
    u8 filler1ACE[0x1B10 - 0x1ACE];
    u16 unk1B10;        /* +0x1B10 */
    u8 unk1B12_0:1;
    u8 linkSkip:1;      /* +0x1B12 bit 1 (see code_08011BE0) */
    u8 unk1B12_2:3;
    u8 linkError:1;     /* +0x1B12 bit 5 */
    u8 result:2;        /* +0x1B12 bits 6-7: duel result 1..3 (hypothesis) */
    u8 unk1B13_0:1;     /* +0x1B13 bit 0 */
    u8 unk1B13_1:7;
    u8 unk1B14;         /* +0x1B14: bit 1 = surrender/end requested (hypothesis) */
    u8 filler1B15[0x1B20 - 0x1B15];
    u8 phaseStep;       /* +0x1B20 */
    u8 phaseTimer;      /* +0x1B21 */
    u8 filler1B22[0x1B2C - 0x1B22];
    struct SelMask sel; /* +0x1B2C: selection widget (see code_0801CE68) */
};
/* Command block at 0x020185C0 (see code_0801F454): queue count at +0x808. */
struct DuelCmd {
    u8 filler0[0x808];
    u16 queueCount;     /* +0x808 */
};
extern struct DuelCmd gDuelCmd;
extern struct DuelState gDuel;

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gDuelCtrl;

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 filler0[0x4870];
    u16 unk4870_0:1;     /* +0x4870 bit 0 */
    u16 unk4870_1:5;
    u16 result:2;        /* +0x4870 bits 6-7: last duel result (copied from 0x020192E0+0x1B12) */
    u16 unk4870_8:8;
};
extern struct Main gMain;
#define gMain gMain

/* 20-byte action entry (lists in gChain; see code_0801F454). */
struct ActEntry {
    u16 card;           /* +0x00: card ID in bits 0-10 */
    u16 flag2_0:1;      /* +0x02 bit 0: player (hypothesis) */
    u16 kind2:3;        /* +0x02 bits 1-3 */
    u16 val2_4:6;
    u16 val2_10:6;
    u8 flag4_0:1;
    u8 flag4_1:1;
    u8 flag4_2:1;
    u8 flag4_3:1;
    u8 flag4_4:1;
    u8 unk4_5:3;
    u8 unk5;
    u16 w6;
    u16 w8;
    u8 fillerA[0x14 - 0xA];
};

/* 0x02017A40: two entry lists. */
struct ActLists {
    struct ActEntry listA[32];  /* +0x000 */
    struct ActEntry listB[16];  /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u16 unk3C2;
    u16 countA;                 /* +0x3C4 */
    u8 filler3C6[0x3D0 - 0x3C6];
    u8 unk3D0;                  /* +0x3D0: bit 0 set calls Chain_Build */
    u8 unk3D1;
    union { u8 raw; struct { u8 active:1; u8 stage:7; } __attribute__((packed)) bits; } __attribute__((packed)) resolveFlags;
    u8 unk3D3;
    u16 unk3D4;
    s16 effectIndex;
    u32 (*resolve)(struct ActEntry *, struct ActEntry *);
    u32 savedCard;
    u8 effectStep;
    u8 effectSub;
};
extern struct ActLists gChain;

extern const u16 gCardIdToNumber[];   /* card ID to card number */
#define CARD_NUMBER(id) (gCardIdToNumber[(id) & 0x7FF])
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

void MemCopy16(void *dst, const void *src, u32 size);
int CountGraveyardCardsByNumber(int player, u16 number);
u32 Chain_Build(void);
u32 Chain_Resolve(void);
u32 CountHandCardsByNumber(int player, int zone);
u16 HasExodiaInHand(int player);
u16 HasDestinyBoardComplete(int player);
int CountActiveCardsOnField(int player, u16 number);
u16 DuelLink_SendMessage(u16 a, u16 b, u16 c, u16 d);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void StopBGM(void);
u16 DuelScreen_Update(void);
u16 DuelCmd_RunRemote(void);
u16 DuelCmdQueue_Run(void);
u16 CardListView_Run(void);
u16 TextBoxUpdate(void);
u16 DuelPrompt_Run(void);
u16 SummonAction_Update(void);
u16 EventResponse_Update(void);
u16 Chain_Update(void);
u16 Duel_CheckWin(void);
void UpdateSpellTrapNegation(int a);
void DrawLinkWaitIndicator(void);
void DrawAllAreaTiles(void);
void LinkWaitStart_Nop(void);
void DrawFieldOverlay(void);
void DrawHandCards(void);
void TextBoxDrawSprites(void);
void SaveGame(void);
extern u16 (*const gDuelPhaseTable[])(void);  /* duel phase table, 10 entries + NULL */

/* Link state at 0x02017FB0 (fields used here; u32 containers, see code_08021CC8). */
struct LinkState {
    u8 filler0[0x304];
    u32 unk304:16;
    u32 unk306_0:6;
    u32 unk306_6:1;     /* +0x306 bit 6 */
    u32 unk306_7:1;
    u32 unk307_0:1;     /* +0x307 bit 0: remote command pending */
    u32 unk307_1:7;
    union { u8 raw; struct { u32 low:5; u32 done:1; u32 high:1; u32 ack:1; } bits; } flags308;
};
extern struct LinkState gLinkState;

struct ResolveUi { u8 pad[0x824]; int player, kind, zone; };
extern struct ResolveUi gDuelScreen;
struct Unk0201AE60 {
    u8 flags0;          /* bit 0 */
    u8 filler1[0x18 - 1];
    void (*callback)(void); /* +0x18 */
    u8 filler1C[4];
    u8 unk20;           /* +0x20 */
};
extern struct Unk0201AE60 gTextBox;

/* The original reserves 0x100 bytes for the first link-message workspace. */
struct ResolvePacket {
    u16 count;
    u16 hasPrevious;
    struct ActEntry current;
    struct ActEntry previous;
};
struct ResolveEffect {
    u16 number;
    u16 flags;
    u32 (*resolve)(struct ActEntry *, struct ActEntry *);
    u8 rest[0x10];
};
extern struct ResolveEffect gCardEffects[];
extern const u32 gCardStats[];
struct ResolveZone { u32 card; u8 pad4[0x91 - 4]; u8 flags91; u8 pad92[2]; };
struct ResolveBoard { struct ResolveZone zones[23]; u8 tail[0xD64 - 23 * 0x94]; };
extern struct ResolveBoard gDuelZones[2];
static inline u8 ResolveDisabled(struct ResolveZone *zone) { return zone->flags91 & 8; }
extern u32 gUnk_02017E1C;
void CopyDuelCard(u32 *dest, u32 *src);
u32 HasFlipEffect(u16 number, u16 flag);
void DestroyFaceUpCardsByNumber(int player, u16 number);
void ShowCardEffect(int player, u16 id);
int ChainListScreen_Run(void);
void ChainListScreen_Start(u32 entry, u16 player);
u32 Chain_IsPartnerEntry(struct ActEntry *entry);
u32 Chain_CardGoesToGrave(struct ActEntry *entry);
u16 DuelLink_SendMessageData(u16 head, const void *data, int size);
void DuelCursor_Select(int player, int kind, int zone);
void LoseLpOnSendToGraveyard(int player, int arg);
int FindCardEffect(u32 card);
#define RESOLVE_LAST (gChain.listB[gChain.countB - 1])
#define RESOLVE_PREV (gChain.listB[gChain.countB - 2])
#define RESOLVE_LINK (gLinkState.flags308.raw)
#define RESOLVE_DUEL(offset) (((u8 *)&gDuel)[offset])
#define RESOLVE_ZONE(player, zone) ((struct ResolveZone *)((u8 *)gDuelZones + ((player) * 0xD64 + (zone) * 0x94)))
#define RESOLVE_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define RESOLVE_ADVANCE() (gChain.resolveFlags.bits.stage++)
#define RESOLVE_END() (gChain.resolveFlags.bits.stage = 100)
static inline u16 ResolveSubtype(u32 stats, int type) {
    switch (type) { case 21: case 22: return (stats & 0xE0000) >> 17; default: return 0; }
}
u32 Chain_Resolve(void)
{
    u16 packet[0x80];
    struct ResolvePacket pair;
    int stage = gChain.resolveFlags.raw >> 1;
    switch (stage) {
    case 0:
        if (gDuelCtrl.link) {
            int i;
            DuelLink_SendMessage(0xF061, gChain.countB, 0, 0);
            for (i = 0; i < gChain.countB; i++) {
                packet[0] = i;
                MemCopy16(packet + 1, &gChain.listB[i], 0x14);
                DuelLink_SendMessageData(0xF062, packet, 0x16);
            }
            DuelLink_SendMessage(0xF064, gChain.countB, 0, 0);
            gLinkState.flags308.bits.ack = 0;
        }
        RESOLVE_ADVANCE();
        return 1;
    case 1:
        ChainListScreen_Start((u32)gChain.listB, 1);
        RESOLVE_ADVANCE();
        return 1;
    case 2:
        if (ChainListScreen_Run())
            RESOLVE_ADVANCE();
        return 1;
    case 3:
        if (gDuelCtrl.link && !(RESOLVE_LINK >> 7))
            return 1;
        DuelCmd_Push(0x12, 0, 0, 0);
        RESOLVE_ADVANCE();
        return 1;
    case 4: {
        u32 stats, type;
        int disabled, subtype;
        gChain.effectIndex = FindCardEffect(RESOLVE_LAST.card);
        if (gChain.effectIndex < 0) {
            RESOLVE_END();
            return 1;
        }
        gChain.resolve = gCardEffects[gChain.effectIndex].resolve;
        if (!gChain.resolve) {
            RESOLVE_END();
            return 1;
        }
        stats = RESOLVE_STATS(RESOLVE_LAST.card);
        type = (stats & 0x1F00000) >> 20;
        if (type > 20) {
            disabled = 0;
            if (RESOLVE_LAST.kind2 != 3)
                disabled = ResolveDisabled(RESOLVE_ZONE(RESOLVE_LAST.flag2_0, RESOLVE_LAST.val2_4)) != 0;
            subtype = ResolveSubtype(stats, type);
            if (subtype != 2) {
                if (subtype == 3 && (gDuel.effectFlags & 3))
                    disabled = 1;
            } else if (gDuel.effectFlags & 4)
                disabled = 1;
            stats = RESOLVE_STATS(RESOLVE_LAST.card);
            switch ((stats & 0x1F00000) >> 20) {
            case 21:
                if (gDuel.flags1ACC.raw & 0x80)
                    disabled = 1;
                if (((stats & 0xE0000) >> 17) == 4 && (gDuel.effectFlags & 0x10))
                    disabled = 1;
                break;
            case 22:
                if (gDuel.flags1ACC.raw & 0x40)
                    disabled = 1;
                if (((stats & 0xE0000) >> 17) == 4 && (gDuel.effectFlags & 8))
                    disabled = 1;
                break;
            }
            if (CARD_NUMBER_C(RESOLVE_LAST.card) == 0x603) {
                disabled = 0;
                RESOLVE_LAST.flag4_2 = 0;
            }
            if (disabled) {
                {
                    u16 msg = RESOLVE_LAST.flag2_0 ? 0x80B1 : 0xB1;
                    u16 zone = RESOLVE_LAST.val2_4;
                    DuelCmd_Push(msg, zone, 1, 0);
                }
                RESOLVE_LAST.flag4_2 = 1;
            }
        } else {
            if ((HasFlipEffect(CARD_NUMBER_C(RESOLVE_LAST.card), 1)
                 || HasFlipEffect(CARD_NUMBER_C(RESOLVE_LAST.card), 0))
                && RESOLVE_LAST.flag4_2) {
                RESOLVE_END();
                return 1;
            }
        }
        {
            u32 *dest = &gChain.savedCard;
            u8 *base = (u8 *)gDuelZones + RESOLVE_LAST.flag2_0 * 0xD64;
            CopyDuelCard(dest, (u32 *)(base + RESOLVE_LAST.val2_4 * 0x94));
        }
        if ((u16)Chain_CardGoesToGrave(&RESOLVE_LAST)) {
            {
                u16 msg = RESOLVE_LAST.flag2_0 ? 0x8078 : 0x78;
                u16 zone = RESOLVE_LAST.val2_4;
                DuelCmd_Push(msg, zone, 0, 0);
            }
        }
        if (RESOLVE_LAST.flag4_3 && RESOLVE_LAST.flag4_2) {
            if (CARD_NUMBER_C((gChain.savedCard << 20) >> 20) == 0x412) {
                LoseLpOnSendToGraveyard((gChain.savedCard << 19) >> 31, 1);
                ShowCardEffect(0, (gChain.savedCard << 20) >> 20);
                {
                    u16 msg = (s32)(gChain.savedCard << 19) < 0 ? 0x806A : 0x6A;
                    u16 cardLow = gChain.savedCard;
                    u16 cardHigh = gChain.savedCard >> 16;
                    DuelCmd_Push(msg, cardLow, cardHigh, 0);
                }
            } else {
                {
                    u16 msg = RESOLVE_LAST.flag2_0 ? 0x807C : 0x7C;
                    u16 cardLow = gChain.savedCard;
                    u16 cardHigh = gChain.savedCard >> 16;
                    DuelCmd_Push(msg, cardLow, cardHigh, 0);
                }
                if (CARD_NUMBER_C((gChain.savedCard << 20) >> 20) == 0x14D) {
                    DestroyFaceUpCardsByNumber(RESOLVE_LAST.flag2_0, 0x58F);
                    DestroyFaceUpCardsByNumber(1 - RESOLVE_LAST.flag2_0, 0x58F);
                }
                LoseLpOnSendToGraveyard((gChain.savedCard << 19) >> 31, 1);
            }
            RESOLVE_END();
            return 0;
        } else {
            gChain.effectStep = 0x80;
            gChain.effectSub = 0;
            RESOLVE_ADVANCE();
            return 1;
        }
    }
    case 5:
        if ((u16)Chain_IsPartnerEntry(&RESOLVE_LAST)) {
            pair.count = gChain.countB;
            if (gChain.countB > 1) {
                pair.hasPrevious = 1;
                MemCopy16(&pair.current, &RESOLVE_LAST, 0x14);
                MemCopy16(&pair.previous, &RESOLVE_PREV, 0x14);
            } else {
                pair.hasPrevious = 0;
                MemCopy16(&pair.current, &RESOLVE_LAST, 0x14);
            }
            DuelLink_SendMessageData(0xF071, &pair, 0x2C);
        }
        gLinkState.flags308.bits.done = 0;
        RESOLVE_ADVANCE();
        return 1;
    case 6:
        if (!(u16)Chain_IsPartnerEntry(&RESOLVE_LAST)) {
            if (gChain.countB > 1)
                gChain.effectStep = gChain.resolve(&RESOLVE_LAST, &RESOLVE_PREV);
            else
                gChain.effectStep = gChain.resolve(&RESOLVE_LAST, 0);
            if (!gChain.effectStep)
                gLinkState.flags308.bits.done = 1;
        }
        if ((s32)((u32)RESOLVE_LINK << 26) < 0) {
            if ((u16)Chain_CardGoesToGrave(&RESOLVE_LAST)) {
                u32 *saved = &gChain.savedCard;
                u16 message = RESOLVE_LAST.flag2_0 ? 0x807C : 0x7C;
                DuelCmd_Push(message, ((u16 *)saved)[0], ((u16 *)saved)[1], 0);
                LoseLpOnSendToGraveyard((gChain.savedCard << 19) >> 31, 1);
            }
            RESOLVE_END();
        }
        return 1;
    case 100:
        if (--gChain.countB) {
            gChain.resolveFlags.raw &= 1;
            return 1;
        }
        /* fall through */
    default:
        gChain.resolveFlags.bits.active = 0;
        DuelCursor_Select(gDuelScreen.player,
                    gDuelScreen.kind,
                    gDuelScreen.zone);
        return 1;
    }
    return 1;
}


u16 Chain_Update(void)
{
    int i, j;
    int count[2];

    if (gChain.unk3D0 & 1)
        return Chain_Build();
    if (gChain.resolveFlags.raw & 1)
        return Chain_Resolve();
    if (gChain.countA != 0) {
        gChain.countB = 0;
        for (i = 0; i < gChain.countA; i++) {
            MemCopy16(&gChain.listB[i], &gChain.listA[i], 0x14);
            gChain.countB++;
        }
        count[0] = CountGraveyardCardsByNumber(0, 0x4DA);
        count[1] = CountGraveyardCardsByNumber(1, 0x4DA);
        for (i = 0; i < gChain.countB; i++) {
            u16 number = CARD_NUMBER(gChain.listB[i].card);
            if (number == 0x4DA) {
                if (count[gChain.listB[i].flag2_0] > 0) {
                    count[gChain.listB[i].flag2_0]--;
                } else {
                    gChain.countB--;
                    for (j = i; j < gChain.countB; j++)
                        MemCopy16(&gChain.listB[j], &gChain.listB[j + 1], 0x14);
                }
            }
        }
        gChain.countA = 0;
        {
            /* countB is u16: the negated sign bit is its nonzero test. */
            u32 negativeCount = -(u32)gChain.countB;
            u8 *active = &gChain.unk3D0;
            *active = negativeCount >> 31;
        }
        gChain.unk3D1 = 0;
        return 1;
    }
    return 0;
}

u16 HasExodiaInHand(int player)
{
    if (CountHandCardsByNumber(player, 0x10) && CountHandCardsByNumber(player, 0x11) && CountHandCardsByNumber(player, 0x12)
        && CountHandCardsByNumber(player, 0x13) && CountHandCardsByNumber(player, 0x14))
        return 1;
    return 0;
}
/* Does the player have all five cards 0x5F8, 0x605-0x607, 0x608 (hypothesis: the five Exodia pieces)? */
u16 HasDestinyBoardComplete(int player)
{
    if (CountActiveCardsOnField(player, 0x5F8) && CountActiveCardsOnField(player, 0x605) && CountActiveCardsOnField(player, 0x606)
        && CountActiveCardsOnField(player, 0x607) && CountActiveCardsOnField(player, 0x608))
        return 1;
    return 0;
}
/* Check whether the duel is over; sets the result (0x1B12 bits 6-7: 1 = player 0 wins, 2 = player 1
 * wins, 3 = draw; hypothesis) and returns 1 if so. */
u16 Duel_CheckWin(void)
{
    struct DuelPlayer *p0;

    if ((u8)(gDuelCtrl.phase - 2) > 6)
        return 0;
    if (gDuelCtrl.link && gDuel.linkSkip)
        return 0;
    gDuel.result = 3;
    p0 = gDuel.players;
    if (gDuel.players[0].lifePoints == 0 || gDuel.players[1].lifePoints == 0) {
        if (gDuel.players[0].lifePoints > gDuel.players[1].lifePoints)
            gDuel.result = 1;
        if (gDuel.players[0].lifePoints < gDuel.players[1].lifePoints)
            gDuel.result = 2;
        gDuel.flags1ACC.bits.over = 1;
        return 1;
    }
    if (p0->deckOut || gDuel.players[1].deckOut) {
        if (!p0->deckOut)
            gDuel.result = 1;
        if (!gDuel.players[1].deckOut)
            gDuel.result = 2;
        gDuel.flags1ACC.bits.over = 1;
        return 1;
    }
    p0->winA = HasExodiaInHand(0);
    gDuel.players[1].winA = HasExodiaInHand(1);
    if (p0->winA || gDuel.players[1].winA) {
        if (!gDuel.players[1].winA)
            gDuel.result = 1;
        if (!p0->winA)
            gDuel.result = 2;
        gDuel.flags1ACC.bits.over = 1;
        return 1;
    }
    p0->winExodia = HasDestinyBoardComplete(0);
    gDuel.players[1].winExodia = HasDestinyBoardComplete(1);
    if (p0->winExodia || gDuel.players[1].winExodia) {
        if (!gDuel.players[1].winExodia)
            gDuel.result = 1;
        if (!p0->winExodia)
            gDuel.result = 2;
        gDuel.flags1ACC.bits.over = 1;
        return 1;
    }
    return 0;
}

/* Duel phase 0 (entry 0 of the phase table 0x08198F80). */
u32 DuelPhase_Init(void)
{
    gDuel.linkSkip = gMain.unk4870_0;
    if ((gDuelCtrl.link) && gDuel.linkSkip) {
        gDuel.unk1B10++;
        DuelLink_SendMessage(0xF001, 0, 0, 0);
        gDuelCtrl.phase = 8;
        return 0;
    }
    return 1;
}
/* Duel phase 9 (last entry of the phase table 0x08198F80): duel end. */
u32 DuelPhase_ShowResult(void)
{
    switch (gDuel.phaseStep) {
    case 0:
        StopBGM();
        gDuel.unk1B13_0 = 0;
        if ((gDuel.players[0].winA) || (gDuel.players[1].winA)) {
            DuelCmd_Push(0x13, 0, 0, 0);
            DuelCmd_Push(5, 0, 0, 0);
            DuelCmd_Push(0x12, 0, 0, 0);
        }
        if ((gDuelPlayers[0].winExodia) || (gDuelPlayers[1].winExodia)) {
            DuelCmd_Push(0x13, 0, 0, 0);
            DuelCmd_Push(6, 0, 0, 0);
            DuelCmd_Push(0x12, 0, 0, 0);
        }
        switch (gDuel.result) {
        case 1:
            DuelCmd_Push(4, 0, 0, 0);
            break;
        case 2:
            DuelCmd_Push(4, 1, 0, 0);
            break;
        case 3:
            DuelCmd_Push(4, 2, 0, 0);
            break;
        }
        gDuel.phaseStep++;
        return 0;
    case 1:
        if (gDuelCmd.queueCount != 0)
            return 0;
        if (!gDuelCtrl.link)
            break;
        DuelLink_SendMessage(0xF003, 1 - gDuel.result, 0, 0);
        gDuel.phaseTimer = 0;
        gDuel.phaseStep++;
        return 0;
    case 2:
        if (++gDuel.phaseTimer <= 0x13)
            return 0;
        break;
    }
    return 1;
}
/* DuelMainStep: the shared Campaign/Link duel step (see program-flow). */
u32 DuelMainStep(void)
{
    u16 busy;
    u16 done;

    if (gDuelPhaseTable[gDuelCtrl.phase] != NULL) {
        busy = DuelScreen_Update();
        if (busy == 0) {
            if (gDuelCtrl.link && gLinkState.unk307_0)
                busy = DuelCmd_RunRemote();
            if (busy == 0) {
                busy = DuelCmdQueue_Run();
                if (busy == 0)
                    busy = CardListView_Run();
            }
        }
        if ((((u8 *)&gDuelScreen)[0] & 6) == 6 && (gTextBox.flags0 & 1) && gTextBox.unk20 <= 2) {
            if (gTextBox.callback)
                gTextBox.callback();
            else
                TextBoxDrawSprites();
        }
        if (busy == 0 && !TextBoxUpdate() && !DuelPrompt_Run() && !SummonAction_Update() && !Chain_Update()) {
            UpdateSpellTrapNegation(1);
            if (!EventResponse_Update()) {
                switch (gDuelCtrl.phase) {
                case 3:
                case 4:
                case 5:
                case 6:
                    if (gDuel.unk1B14 & 2) {
                        DrawLinkWaitIndicator();
                        goto end;
                    }
                    if (gLinkState.unk306_6) {
                        int ok = 1;
                        if (gDuel.sel.flag0) {
                            if (gDuel.sel.state == 2) {
                                gDuel.sel.flag0 = 0;
                                gDuel.sel.state = 0;
                            } else {
                                ok = 0;
                            }
                        }
                        if (gDuel.sel.active)
                            ok = 0;
                        if (ok) {
                            DuelCmd_Push(0x8041, 1, 0, 0);
                            DuelLink_SendMessage(0xF005, 0, 0, 0);
                            gDuel.unk1B14 |= 2;
                        }
                        DrawAllAreaTiles();
                        LinkWaitStart_Nop();
                    }
                }
                done = gDuelPhaseTable[gDuelCtrl.phase]();
                if (Duel_CheckWin()) {
                    gDuelCtrl.phase = 9;
                    gDuel.phaseStep = 0;
                    gDuel.phaseTimer = 0;
                } else if (done) {
                    gDuelCtrl.phase++;
                    gDuel.phaseStep = 0;
                    gDuel.phaseTimer = 0;
                }
            }
        }
    end:
        DrawFieldOverlay();
        DrawHandCards();
        return 0;
    }
    gMain.result = gDuel.result;
    SaveGame();
    return 1;
}

