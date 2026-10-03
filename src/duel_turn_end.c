#include "global.h"

struct Player { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
extern struct Player gDuelPlayers[];
extern const u32 gCardStats[];
#define CARD_STATS(id) (*(gCardStats + ((id) & 0x7FF)))
#define CARD_STATS_RAW(id) (*(gCardStats + (id)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Menu block 0x0201AE60 */
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
/* Action list block 0x02017A40 (see duel_setup). */
struct ActBlk {
    u8 u0[0x3D6];
    s16 effIdx;         /* +0x3D6 */
    u32 fn3D8;
    u8 u3DC[0x3E0 - 0x3DC];
    u8 b3E0;
    u8 b3E1;
    u8 u3E2[0x3E4 - 0x3E2];
    u8 b3E4;
    u8 b3E5;
    u8 u3E6[0x480 - 0x3E6];
    u32 fn480;
    u32 fn484;
    u8 u488[0x4FC - 0x488];
    u8 b4FC;
    u8 count;           /* +0x4FD */
};
extern struct ActBlk gChain;
/* Byte views of the duel global 0x020192E0 and the link block 0x02017FB0 */
struct MainKeys { u8 u0[6]; u16 keys; };
extern struct MainKeys gMain;
struct Cnt2 { u8 b0; u8 b1; };
extern struct Cnt2 gDuelCtrl;
u16 DuelLink_RunPartnerRequests(void);
int DuelScreen_HandleInput(void);
int AiRunTurn(void);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void DuelLink_SendMessage(u16 msg, u16 a, u16 b, u16 c);
u16 DuelLink_SendMessageData(u16 head, const void *src, int size);
u16 DuelLink_RunCardPrompt(void);
u16 DuelLink_RunRemoteChainB(void);
u16 DuelLink_RunRemoteChainA(void);
u16 DuelLink_RunRemoteResolve(void);
int DuelLink_AnswerActivateQuery(void);
int ChainListScreen_Run(void);
struct DG3 {
    u8 pad[0x1B12];
    u8 b1B12;
    u8 pad2[0x1B62 - 0x1B13];
    u8 step;
};
extern const u8 gStrDiscardFromHand[];
int AiPickDiscard(void);
int AiPickWeakestHandCard(struct Player *ps, int a);
int FindMagicInHand(int player);
int FindTrapInHand(int player);
int Random(void);
void DuelScreen_ScrollToZone(int player, int a);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, void (*b)(void), int (*c)(void));
void DiscardPrompt_DrawRemaining(void);
int DiscardPrompt_HandleInput(void);
void TriggerForcedRequisition(int player, int a);
void EventResponse_Request(int player, int kind, u32 arg);
extern const u8 gStrAttackTargetZeroAtkFmt[], gStrKuribohDiscardFmt[], gStrAttackTargetSubstituteFmt[], gStrAttackTargetRedirectFmt[];
extern const u8 gStrTurnsUntilDestroyedFmt[];
extern const u8 gCardNames[];
extern const u16 gCardIdToNumber[];
extern const u16 gUnk_08623E66;
extern const u16 gUnk_0862467A;
void FormatStr(char *dst, const void *fmt, const void *name);
struct Bits8 { u8 g0 : 1; u8 g1 : 1; u8 g2 : 1; u8 g3 : 5; u8 pad[4]; };
struct T8 { u8 f0 : 1; u8 rest : 7; u8 pad[4]; };
struct T16 { u16 lo : 8; u16 hi : 8; };
struct EffEntry4 { u16 idx; u16 u2; u32 fn4; u8 pad[24 - 8]; };
struct HandW { u32 lo : 17; u32 f17 : 1; u32 f18 : 1; u32 rest : 13; };
struct DuelScreen { u8 unk0[0x82C]; u32 w82C; };
extern struct DuelScreen gDuelScreen;
struct LinkBlk;
#define OFF(t, f) ((u32)&((t *)0)->f)
struct LinkBlk {
    u8 u0[0x306];
    u8 b306;
    u8 b307;
    u32 f308_0 : 1;
    u32 f308_1 : 1;
    u32 f308_2 : 1;
    u32 f308_3 : 1;
    u32 f308_4 : 1;
    u32 f308_5 : 1;
    u32 f308_6 : 1;
    u32 f308_7 : 25;
    u8 u30C[0x450 - 0x30C];
    u32 f450_0 : 1;
    u32 f450_1 : 15;
    u16 h452;
    u16 h454;
    u16 h456;
    u16 h458;
    u16 h45A;
    u16 id45C;
    u8 b45E;
    u8 u45F[0x48D - 0x45F];
    u8 step48D;
    u8 step48E;
    u8 step48F;
};
typedef char chk452[(OFF(struct LinkBlk, h452) == 0x452) ? 1 : -1];
typedef char chk45C[(OFF(struct LinkBlk, id45C) == 0x45C) ? 1 : -1];
extern struct LinkBlk gLinkState;
struct EffEntry { u16 idx; u16 u2; u32 fn4; u8 u8_[0x10 - 8]; u32 fn10; u32 fn14; };
extern struct EffEntry gCardEffects[];
struct DuelSel {
    u8 unk0[0x1B10];
    u16 w1B10;      /* +0x1B10 */
    u8 b1B12;       /* +0x1B12 */
    u8 u1B13[0x1B20 - 0x1B13];
    u8 step;        /* +0x1B20 */
    u8 b1B21;       /* +0x1B21 */
    u8 u1B22[0x1B54 - 0x1B22];
    u16 w1B54;      /* +0x1B54 */
};
extern struct DuelSel gDuel;
extern u8 gDuelZones[];      /* per-player zone block (0x94-byte entries) */
extern struct Cnt2 gAiState;
void ShowCardEffect(int player, u16 a);
void DestroyFieldCard(int player, int idx, int a);
void FormatInt(char *a, char *b, int n);
int FindCardEffect(int id);
int DuelCursor_PickTarget(u32 mask);
void DiscardHandCard(int player, int idx, int a, int b);
void DuelCursor_Select(u32 player, u32 a, u32 b);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
void PlaySE(u16 se);
u16 DiscardPrompt_TryDiscardSelected(u16 a, u16 b);

struct S50Card { u32 id:12; u32 b12:6; u32 f18:1; u32 b19:13; };
struct S50Zone {
    struct S50Card card;    /* +0x00 */
    u16 serial;             /* +0x04 */
    u16 f6_0:1;
    u16 f6_1:1;             /* +0x06 bit 1 */
    u16 f6_2:4;
    u16 cnt:4;              /* +0x06 bits 6..9 */
    u16 f6_10:6;
    u8 pad8[0x94 - 8];
};
struct S50Player {
    u16 lp;
    u8 handCount;
    u8 pad3[0x28 - 3];
    struct S50Zone zones[11];   /* +0x28 */
    struct S50Card hand[80];    /* +0x684 */
    u8 padx[0xD64 - 0x684 - 80 * 4];
};
struct S50Duel {
    u32 unk0;
    struct S50Player players[2];
    u8 pad[0x1B10 - 4 - 2 * 0xD64];
    u16 w1B10;
    u8 f0:1;
    u8 turn:1;
    u8 f2:6;
    u8 pad13[0x1B20 - 0x1B13];
    u8 step;
    u8 idx;
};
#define D50 (*(struct S50Duel *)&gDuel)
#define P50 ((struct S50Player *)gDuelPlayers)
/* Zone z of player p (0x0201930C = players[0].zones). The loops want the zone term first and the
 * found blocks the player term first (agbcc's multiply order follows the operand order). */
#define ZONE50(p, z) ((struct S50Zone *)((z) * 0x94 + ((p) & 1) * 0xD64 + (u32)gDuelZones))
#define ZONE50F(p, z) ((struct S50Zone *)(((p) & 1) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))

static inline int S50Sub(u16 id)
{
    u32 st = ((const u32 *)0x08621DE0)[id & 0x7FF];
    switch ((int)((st & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (st & 0xE0000) >> 17;
    default:
        return 0;
    }
}

/* Duel step machine on 0x020192E0+0x1B20: 0/1 = turn player / opponent: play the first hand card
 * with bit 18 set (DiscardHandCard), else flip the first marked card in zones 5..10 (skipping face-up
 * field/equip/continuous spells, subtypes 2..4); 2 = text box for each monster zone whose counter
 * (+6 bits 6..9) is > 1; 3/4 = queue events 2/3 (+0x8000 for player 1); then link sync, return 1. */
int DuelPhase_TurnEnd(void)
{
    char buf[0x80];
    int p = D50.turn;
    int i;
    struct S50Player *ps;
    struct S50Player *ps0; /* separate pointer per case: the ROM gives them different registers */
    u8 n0;
    u8 n2;
    int op;

    switch (D50.step) {
    case 0:
        i = 0;
        ps0 = P50;
        if (i < ps0[p & 1].handCount) {
            u8 *hands = (u8 *)ps0->hand;
            n0 = ps0[p & 1].handCount;
            do {
                struct S50Card c = *(struct S50Card *)((p & 1) * 0xD64 + (u32)hands + i * 4);
                if (c.f18) {
                    ShowCardEffect(p, gUnk_0862467A);
                    DiscardHandCard(p, i, 0, 1);
                    return 0;
                }
                i++;
            } while (i < n0);
        }
        for (i = 5; i <= 10; i++) {
            struct S50Zone *z = ZONE50(p, i);
            struct S50Card c = z->card;
            if (c.id != 0 && c.f18) {
                int ok = 1;
                if (z->f6_1) {
                    switch (S50Sub(c.id)) {
                    case 2:
                    case 3:
                    case 4:
                        ok = 0;
                    }
                }
                if (ok) {
                    ZONE50F(p, i)->card.f18 = 0;
                    ShowCardEffect(p, gUnk_0862467A);
                    DestroyFieldCard(p, i, 0);
                    return 0;
                }
            }
        }
        D50.step++;
        return 0;
    case 1:
        i = 0;
        ps = P50;
        if (i < ps[(1 - p) & 1].handCount) {
            u8 *hands;
            /* FAKEMATCH: the first call takes this copy, the second recomputes 1 - p (as in the ROM) */
            op = 1 - p;
            hands = (u8 *)ps->hand;
            n2 = ps[(1 - p) & 1].handCount;
            do {
                struct S50Card c = *(struct S50Card *)((op & 1) * 0xD64 + (u32)hands + i * 4);
                if (c.f18) {
                    ShowCardEffect(op, gUnk_0862467A);
                    DiscardHandCard(1 - p, i, 0, 1);
                    return 0;
                }
                i++;
            } while (i < n2);
        }
        for (i = 5; i <= 10; i++) {
            struct S50Zone *z = ZONE50(1 - p, i);
            struct S50Card c = z->card;
            if (c.id != 0 && c.f18) {
                int ok = 1;
                if (z->f6_1) {
                    switch (S50Sub(c.id)) {
                    case 2:
                    case 3:
                    case 4:
                        ok = 0;
                    }
                }
                if (ok) {
                    ZONE50F(1 - p, i)->card.f18 = 0;
                    ShowCardEffect(1 - p, gUnk_0862467A);
                    DestroyFieldCard(1 - p, i, 0);
                    return 0;
                }
            }
        }
        if (p)
            D50.step++;
        D50.step++;
        D50.idx = 0;
        return 0;
    case 2:
        while (D50.idx <= 4) {
            u8 n = D50.idx;
            struct S50Zone *z = (struct S50Zone *)(n * 0x94 + p * 0xD64 + (u32)D50.players[0].zones);
            struct S50Card c = z->card;
            s16 id = c.id; /* FAKEMATCH: s16 adds loop insns so loop.c keeps movs #0x94 in the loop */
            if (id != 0 && z->cnt > 1) {
                /* FAKEMATCH: integer table address; the gCardNames symbol shifts reload registers */
                FormatStr(buf, gStrTurnsUntilDestroyedFmt, (const u8 *)0x0822C720 + id * 0x40);
                FormatInt(buf, buf, z->cnt - 1);
                TextBoxOpen(0x206, 0x712, 0xB, buf);
                D50.idx++;
                return 0;
            }
            D50.idx = n + 1;
        }
        D50.step++;
        return 0;
    case 3:
        DuelCmd_Push((D50.turn) ? 0x8002 : 2, 0, 0, 0);
        D50.step++;
        return 0;
    case 4:
        DuelCmd_Push((D50.turn) ? 0x8003 : 3, 0, 0, 0);
        D50.step++;
        return 0;
    default:
        if (!(gDuelCtrl.b1 & 1)) {
            if (!D50.turn) {
                gAiState.b0 = 0;
                gAiState.b1 = 0;
            }
        }
        if (gDuelCtrl.b1 & 1)
            DuelLink_SendMessage(0xF002, 0, 0, 0);
        D50.w1B10++;
        return 1;
    }
}
/* ROM table views at fixed addresses preserve the target lookup allocation. */
u16 DuelLink_RunCardPrompt(void)
{
    char buf[0x100];
    switch (gLinkState.h452) {
    case 0:
        switch (((const u16 *)0x08622AB4)[gLinkState.h454 & 0x7FF]) {
        case 0x172:
        case 0x173:
        case 0x174:
            FormatStr(buf, gStrAttackTargetZeroAtkFmt, ((const u8 *)0x0822C720) + (gLinkState.h454 << 6));
            TextBoxOpen(0x204, 0xA14, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            break;
        case 0x39:
            FormatStr(buf, gStrKuribohDiscardFmt, ((const u8 *)0x0822C720) + (gUnk_08623E66 << 6));
            TextBoxOpen(0x204, 0x915, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            break;
        case 0x4DB:
            FormatStr(buf, gStrAttackTargetSubstituteFmt, ((const u8 *)0x0822C720) + (gLinkState.h454 << 6));
            TextBoxOpen(0x206, 0x713, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            break;
        case 0x5F2:
            FormatStr(buf, gStrAttackTargetRedirectFmt, ((const u8 *)0x0822C720) + (gLinkState.h454 << 6));
            TextBoxOpen(0x206, 0x713, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
            break;
        }
        gLinkState.h452++;
        return 0;
    case 1:
        switch (((const u16 *)0x08622AB4)[gLinkState.h454 & 0x7FF]) {
        case 0x172:
        case 0x173:
        case 0x174:
        case 0x39:
        case 0x4DB:
        case 0x5F2:
            gLinkState.h45A = gTextBox.sel;
            return 1;
        }
        return 0;
    default:
        return 1;
    }
}

u16 DuelLink_RunRemoteChainA(void)
{
    struct LinkBlk *b = &gLinkState;
    u8 *step = &b->step48D;
    switch (*step) {
    case 0:
        gChain.effIdx = FindCardEffect(b->id45C);
        if (gChain.effIdx < 0)
            return 1;
        gChain.fn480 = gCardEffects[gChain.effIdx].fn10;
        if (gChain.fn480 == 0)
            return 1;
        ((u8 *)&gChain)[0x3E4] = 0;
        (*step)++;
        return 0;
    case 1:
        if (((u16(*)(void *, void *))gChain.fn480)((u8 *)b + 0x45C, (u8 *)b + 0x470) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
u16 DuelLink_RunRemoteChainB(void)
{
    struct LinkBlk *b = &gLinkState;
    u8 *step = &b->step48E;
    switch (*step) {
    case 0:
        gChain.effIdx = FindCardEffect(b->id45C);
        if (gChain.effIdx < 0)
            return 1;
        gChain.fn484 = gCardEffects[gChain.effIdx].fn14;
        if (gChain.fn484 == 0)
            return 1;
        *(u8 *)((u8 *)&gChain + 0x3E5) = 0;
        (*step)++;
        return 0;
    case 1:
        if (((u16(*)(void *, void *))gChain.fn484)((u8 *)b + 0x45C, (u8 *)b + 0x470) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
/* Link copy of an effect source/target ref (0x14 bytes, at link block +0x45C and +0x470). */
struct LinkRef {
    u16 id;
    u8 b2;              /* +0x02: bit 0 player (struct T8 view) */
    u8 u3[3];
    u16 pos;            /* +0x06: low byte player */
    u16 w8;             /* +0x08: low byte player */
    u8 uA[0x14 - 0xA];
};
#define LINK_REF(l, off) ((struct LinkRef *)((l) + (off)))
#define LINK_REF_PLAYER(l, off) (((struct T8 *)&LINK_REF(l, off)->b2)->f0)
/* Swaps the low-byte player of a packed halfword: 1 - player, high byte kept. */
#define FLIP_LO_PLAYER(h) ((u8)(1 - (h)) | ((h) >> 8 << 8))
/* Mirror both refs to the other side's point of view (player = 1 - player), then look up and start the
 * effect handler fn4 (step 0); call it with (ref, target or NULL) until it returns 0 (step 1). */
u16 DuelLink_RunRemoteResolve(void)
{
    u8 *l = (u8 *)&gLinkState;
    u8 *step = l + 0x48F;
    switch (*step) {
    case 0:
        /* FAKEMATCH: the u8 constant gives the minuend its own QImode register, as in the ROM */
        { u8 v = LINK_REF_PLAYER(l, 0x45C); u8 one = 1; LINK_REF_PLAYER(l, 0x45C) = one - v; }
        { u8 v = LINK_REF_PLAYER(l, 0x470); u8 one = 1; LINK_REF_PLAYER(l, 0x470) = one - v; }
        { u16 *hp = &LINK_REF(l, 0x45C)->pos; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x45C)->w8; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x470)->pos; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x470)->w8; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        gChain.effIdx = FindCardEffect(gLinkState.id45C);
        if (gChain.effIdx < 0)
            return 1;
        gChain.fn3D8 = gCardEffects[gChain.effIdx].fn4;
        if (gChain.fn3D8 == 0)
            return 1;
        gChain.b3E0 = 0x80;
        gChain.b3E1 = 0;
        (*step)++;
        return 0;
    case 1:
        if (l[0x490] & 1)
            gChain.b3E0 = ((u8(*)(void *, void *))gChain.fn3D8)(l + 0x45C, l + 0x470);
        else
            gChain.b3E0 = ((u8(*)(void *, void *))gChain.fn3D8)(l + 0x45C, 0);
        if (gChain.b3E0 == 0)
            gLinkState.step48F++;
        return 0;
    default:
        return 1;
    }
}
#undef LINK_REF
#undef LINK_REF_PLAYER
#undef FLIP_LO_PLAYER
u16 DuelLink_RunPartnerRequests(void)
{
    if (gLinkState.f450_0 && !((gLinkState.b306 << 26) < 0)) {
        if (DuelLink_RunCardPrompt() != 0) {
            gLinkState.f450_0 = 0;
            DuelLink_SendMessage(0xF058, gLinkState.h45A, gLinkState.h456, gLinkState.h458);
        }
        return 1;
    }
    if (gLinkState.f308_2) {
        if (DuelLink_RunRemoteChainA() != 0) {
            gLinkState.f308_2 = 0;
            gLinkState.b45E |= 1;
            DuelLink_SendMessageData(0xF092, &gLinkState.id45C, 0x14);
        }
        return 1;
    }
    if (gLinkState.f308_0) {
        if (DuelLink_RunRemoteChainB() != 0) {
            gLinkState.f308_0 = 0;
            gLinkState.b45E |= 1;
            DuelLink_SendMessageData(0xF082, &gLinkState.id45C, 0x14);
        }
        return 1;
    }
    if ((*(u16 *)&gLinkState.b306 & 0x420) == 0x400) {
        if (DuelLink_AnswerActivateQuery() != 0)
            ((struct Bits8 *)&gLinkState.b307)->g2 = 0;
        return 1;
    }
    if (gLinkState.f308_4) {
        if (DuelLink_RunRemoteResolve() != 0) {
            gLinkState.f308_4 = 0;
            DuelLink_SendMessage(0xF073, 0, 0, 0);
        }
        return 1;
    }
    if (gLinkState.f308_6) {
        if (ChainListScreen_Run() != 0) {
            gLinkState.f308_6 = 0;
            DuelLink_SendMessage(0xF065, 0, 0, 0);
        }
        return 1;
    }
    return 0;
}
struct DG2 {
    u8 pad[0x1B12];
    u8 f0 : 1;
    u8 f1 : 1;
    u8 f2 : 6;
    u8 pad2[0x1B20 - 0x1B13];
    u8 step;
    u8 b1B21;
};
int DuelPhase_OpponentTurn(void)
{
    struct Bits8 *q;
    ((struct DG2 *)&gDuel)->f1 = 1;
    if (gDuelCtrl.b1 & 1) {
        u8 t = gLinkState.b306;
        if ((t << 28) < 0) {
            gDuelCtrl.b0++;
            return 1;
        }
        if ((t << 26) < 0)
            return 0;
        if (DuelLink_RunPartnerRequests() != 0)
            return 0;
        q = (struct Bits8 *)((u8 *)&gDuel + 0x1B14);
        if (q->g1) {
            if (DuelScreen_HandleInput() != 0)
                return 0;
            if (gMain.keys & 2) {
                q->g1 = 0;
                DuelCmd_Push(3, 0, 0, 0);
                DuelLink_SendMessage(0xF006, 0, 0, 0);
            }
        } else if (gMain.keys & 1) {
            DuelLink_SendMessage(0xF004, 0, 0, 0);
        }
        {
            u8 t2 = gLinkState.b306;
            if ((t2 << 29) >= 0)
                return 0;
            {
                int m = -5;
                m &= t2;
                gLinkState.b306 = m;
            }
        }
    } else if (AiRunTurn() == 0) {
        return 0;
    }
    ((struct DG2 *)&gDuel)->f1 = 0;
    gDuelCtrl.b0 -= 6;
    ((struct DG2 *)&gDuel)->step = 0;
    ((struct DG2 *)&gDuel)->b1B21 = 0;
    return 0;
}
static inline int W515Type(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}
/* Count hand cards of `player` whose word has neither bit 17 nor bit 18; flag != 0 also requires
 * type <= 0x14. flag 0: just the hand count. */
int CountDiscardableHandCards(int player, u16 flag)
{
    int six;
    int i;
    int count = 0;
    int n;

    if (flag == 0)
        return gDuelPlayers[player & 1].handCount;
    n = gDuelPlayers[player & 1].handCount;
    for (i = 0; i < n; i++) {
        u32 *p = &gDuelPlayers[player & 1].hand[i];
        if ((u32)W515Type((*p << 20) >> 20) <= 20 || flag == 0) {
            /* FAKEMATCH: the mask goes through a temporary (permuter find); it swaps count (r5) and the base (r6). */
            if (!(((u8 *)p)[2] & (six = 6)))
                count++;
        }
    }
    return count;
}

/* Returns 1 when player 0 hands over the selected card (a: type <= 0x14 only). */
u16 DiscardPrompt_TryDiscardSelected(u16 a, u16 b)
{
    struct Player *ps = gDuelPlayers;
    if (ps->handCount == 0)
        return 1;
    if (DuelCursor_PickTarget(1) != 0) {
        u32 idx = gDuelScreen.w82C;
        u32 off4 = idx << 2;
        u32 *hp = gDuelPlayers->hand;
        struct HandW *w = (struct HandW *)((u8 *)hp + off4);
        if (a == 0 || CARD_TYPE((*(u32 *)w << 20) >> 20) <= 0x14) {
            struct HandW v = *w;
            int ok = 1;
            if (v.f17)
                ok = 0;
            if (v.f18)
                ok = 0;
            if (ok != 0) {
                DiscardHandCard(0, idx, b, 1);
                return 1;
            }
        }
        PlaySE(3);
    }
    return 0;
}

/* Draws the cursor sprites for the pending list entries. */
void DiscardPrompt_DrawRemaining(void)
{
    int i;
    int x = (gTextBox.x + 1) << 3;
    int y = (gTextBox.b21 - gTextBox.h + 2) << 3;
    for (i = 0; i < gChain.count; i++) {
        /* FAKEMATCH: no-op self-store (found by the permuter). It enlarges the loop body enough that loop.c
         * keeps y << 16 inside the loop, and the extra uses of the count address give that pointer r6 ahead of y. */
        gChain.count += 0;
        AddSprite((x + i * 10) | (y << 16), 0, 0x431C);
    }
}
int DiscardPrompt_HandleInput(void)
{
    struct Ui *u = &gTextBox;
    u8 *st = &u->timer;
    switch (*st) {
    case 0:
        if (DiscardPrompt_TryDiscardSelected(gDuel.w1B54 & 1, gDuel.w1B54 & 2)) {
        inc:
            (*st)++;
        }
        break;
    case 1:
        DuelCursor_Select(0, 0xB, 0);
        goto inc;
    case 2:
        gChain.count--;
        if (gChain.count == 0)
            return 1;
        *st = 0;
        return 0;
    }
    return 0;
}
int DuelPrompt_Discard(int player, int b, int c, u16 d)
{
    struct DG3 *e = (struct DG3 *)&gDuel;
    u8 *step = &e->step;
    switch (*step) {
    case 0:
        DuelScreen_ScrollToZone(player, 0xB);
        gChain.b4FC = 0;
        gChain.count = b;
        (*step)++;
        return 0;
    case 1:
        if (player != 0) {
            struct Player *ps;
            int r;
            if (gChain.count == 0)
                goto inc;
            ps = (struct Player *)((u8 *)e + 4);
            if (ps[player & 1].handCount == 0)
                goto inc;
            r = AiPickDiscard();
            if (r < 0) {
                r = AiPickWeakestHandCard(ps, 1);
                if (r < 0) {
                    r = FindMagicInHand(1);
                    if (r < 0) {
                        r = FindTrapInHand(1);
                        if (r < 0) {
                            u8 *q = (u8 *)e + 0xD6A;
                            if (*q > 2)
                                r = Random() % *q;
                            else
                                r = 0;
                        }
                    }
                }
            }
            DiscardHandCard(player, r, d, 1);
            gChain.count--;
            return 0;
        }
        DuelCursor_Select(0, 0xB, 0);
        TextBoxOpen(0x209, 0x50E, 0xB, gStrDiscardFromHand);
        TextBoxSetMenu(5, DiscardPrompt_DrawRemaining, DiscardPrompt_HandleInput);
    inc:
        ((struct DG3 *)&gDuel)->step++;
        return 0;
    case 2:
        TriggerForcedRequisition(player, b);
        (*step)++;
        return 0;
    case 3:
        EventResponse_Request(1 - ((u32)(e->b1B12 << 30) >> 31), 0x1D, (u8)player);
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
int DuelPrompt_DiscardCost(int player, int b, int c, u16 d)
{
    struct DG3 *e = (struct DG3 *)&gDuel;
    u8 *step = &e->step;
    switch (*step) {
    case 0:
        DuelScreen_ScrollToZone(player, 0xB);
        gChain.b4FC = 0;
        gChain.count = b;
        (*step)++;
        return 0;
    case 1:
        if (player != 0) {
            struct Player *ps;
            int r;
            if (gChain.count == 0)
                goto inc;
            ps = (struct Player *)((u8 *)e + 4);
            if (ps[player & 1].handCount == 0)
                goto inc;
            r = AiPickDiscard();
            if (r < 0) {
                r = AiPickWeakestHandCard(ps, 1);
                if (r < 0) {
                    r = FindMagicInHand(1);
                    if (r < 0) {
                        r = FindTrapInHand(1);
                        if (r < 0) {
                            u8 *q = (u8 *)e + 0xD6A;
                            if (*q > 2)
                                r = Random() % *q;
                            else
                                r = 0;
                        }
                    }
                }
            }
            DiscardHandCard(player, r, d, 1);
            gChain.count--;
            return 0;
        }
        DuelCursor_Select(0, 0xB, 0);
        TextBoxOpen(0x209, 0x50E, 0xB, gStrDiscardFromHand);
        TextBoxSetMenu(5, DiscardPrompt_DrawRemaining, DiscardPrompt_HandleInput);
    inc:
        ((struct DG3 *)&gDuel)->step++;
        return 0;
    default:
        return 1;
    }
}
