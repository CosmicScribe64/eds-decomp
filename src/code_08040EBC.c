#include "global.h"

/*
 * Target-selection routines ("choose a card/zone" prompts). Each is
 * `int f(struct CardRef *ref)` and returns 1 when done or 0 while waiting.
 * The step counter is at 0x02017A40+0x3E5 (0 = prompt, >0 = wait for input).
 * See wiki/functions/code-08040ebc.md.
 */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];     /* +0x0C */
};

void PlaySE(int a);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void DuelPrompt_Post(int a, int b, int c, int d);
void AddEffectTarget(struct CardRef *ref, u16 v);
u16 TryAddEffectTarget(struct CardRef *ref, int a, int z);
int CanCardTargetZone(u16 id, int a, int z);
u32 DuelCursor_PickTarget(u32 keys);
extern u8 gChain[];
#define SEL_STEP gChain[0x3E5]
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gMain;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gDuelScreen;
struct DuelGlobal {
    u8 unk0[0x1B64];
    u16 w1B64;          /* prompt result */
    u16 w1B66;
};
extern struct DuelGlobal gDuel;
extern const u8 gStrDesignateOpponentSpellTrapToDestroy[];
void FormatStr(char *dst, const char *fmt, const char *arg);
int EffectOwnSkullOrThunderCheck(struct CardRef *ref, u16 v);
int GetZoneCardType(int player, int zone);
extern const u16 gUnk_08623E1E[];
extern const char gCardNames[][0x40];
extern const char gStrDesignateFaceUpMonsterOfTwoFmt[], gStrThunderType[], gStrSelectOpponentMonsterToControlFmt[], gStrMachineType[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 unk7[0x91 - 7];
    u8 f91_0 : 1;
    u8 f91_1 : 1;
    u8 f91_2 : 1;       /* bit 2 */
    u8 f91_3 : 1;       /* bit 3 */
    u8 f91_4 : 4;
    u8 unk92[2];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
int EffectFaceUpFusionMonsterCheck(struct CardRef *ref, u16 pos);
extern const char gStrDesignateMonsterToDestroy[], gStrSelectGraveMonstersToBanishFmt[], gStrRemainingCountFmt[];
void FormatInt(char *dst, const char *fmt, int n);
int CountGraveyardMonsters(int player);
void CardListView_Open(int player, int area, int a2, int a3);
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
};
extern struct ListView gCardListView;
extern const u8 gStrDesignateMonsterYouWishToTribute[];
int EffectTributeForInsectCheck(struct CardRef *ref, u16 pos);
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void TributeMonster(int player, int zone);
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }
int GetCardSpellSpeed(u16 id);
int CountActiveCardsOnField(int player, u16 number);
int CanActivateEffect(struct CardRef *ref, struct CardRef *other, u16 flags);
struct DG12 { u8 pad[0x1B12]; u8 b; };
struct PS7 { u8 pad[7]; u8 b7; u8 rest[0xD64 - 8]; };
extern struct PS7 gDuelPlayers[2];
extern const u8 gStrDesignateFirstCardToReturn[], gStrDesignateSecondCardToReturn[];
int EffectTwoSpellTrapsOnFieldPrepare(struct CardRef *ref, int a, int b);
extern const u8 gStrSelectZoneToBlock[], gStrSelectAnotherZoneToBlock[];
int DuelCursor_PickAny(void);
int IsMonsterZoneFree(int a, int b);
extern const u8 gStrDesignateMonsterYouWishToTribute[], gStrDesignateAnotherMonsterToTribute[];
extern const char gStrSelectAttackTargetFmt[];
extern const char gCardNames[][0x40];
extern const u16 gUnk_086248EE[];
void FormatStr(char *dst, const char *fmt, const char *arg);
void AddEffectTargetUnchecked(struct CardRef *ref, int a, int z);
extern const u8 gStrDesignateOpponentMonsterToFlip[], gStrDesignateMonsterToEquip[];
int EffectEquipTargetCheck(struct CardRef *ref, u16 pos);
extern const u8 gStrDesignateOpponentSpellTrapToReturn[], gStrSelectTrapToForceActivate[], gStrDesignateFusionToReturnToDeck[], gStrSelectReplacementAttacker[], gStrDesignateMagicForMask[];

static inline int CardLevel(u32 id)
{
    int r;
    CARD_LEVEL(id, r);
    return r;
}
int EffectRedirectAttackChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    char buf[0x100];
    if (*st == 0) {
        FormatStr(buf, gStrSelectAttackTargetFmt, gCardNames[gUnk_086248EE[0]]);
        TextBoxOpen(0x206, 0x712, 0xB, buf);
        {
            int mask = ~7;
            register u8 fields __asm__("r1");

            /* FAKEMATCH: keep this bitfield-update scratch in r1. */
            fields = ((u8 *)ref)[0xA];
            __asm__("" : : "r"(fields));
            ((u8 *)ref)[0xA] = mask & fields;
        }
        (*st)++;
    } else if (DuelCursor_PickTarget(0xE0) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        int pl = 1 & p;
        struct DuelZone *z = ZB(pl, zn);
        u32 id = (*(u32 *)z << 20) >> 20;
        if ((z->flags6 & 2) && id != 0 && ((const u16 *)0x08622AB4)[id & 0x7FF] == 0x57D) {
            AddEffectTargetUnchecked(ref, p, zn);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}
int EffectTributeForLevelChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int v = *st;
    switch (v) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterYouWishToTribute);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF0) != 0) {
            u32 p = gDuelScreen.w824;
            int sum = gDuelScreen.w828 + gDuelScreen.w82C;
            u16 w = (u8)p | (u8)sum << 8;
            if (EffectTributeForInsectCheck(ref, w) != 0) {
                u16 msg;
                int pl;
                int lvl;
                struct DuelZone *zp;
                u32 id;
                PlaySE(1);
                msg = (v & ((u8 *)ref)[2]) ? 0x8008 : 8;
                DuelCmd_Push(msg, gDuelScreen.w824, (u8)gDuelScreen.w828 | (u8)gDuelScreen.w82C << 8, 0);
                TributeMonster(p, w);
                pl = p & v;
                zp = ZB(pl, w);
                id = (*(u32 *)zp << 20) >> 20;
                CARD_LEVEL(id, lvl);
                AddEffectTarget(ref, lvl + 1);
                return 1;
            }
            PlaySE(3);
        }
        return 0;
    default:
        return 0;
    }
}
int EffectBlockMonsterZonesChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectZoneToBlock);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickAny() != 0) {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w82C;
            if (gDuelScreen.w828 == 0 && IsMonsterZoneFree(p, zn) != 0) {
                AddEffectTargetUnchecked(ref, p, zn);
                (*st)++;
                return 0;
            }
            PlaySE(3);
        }
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectAnotherZoneToBlock);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickAny() != 0) {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w82C;
            if (gDuelScreen.w828 == 0 && (*(u32 *)ZB2(1 & p, zn) << 20) == 0
                && (u16)((u8)p | (u8)zn << 8) != ref->targets[0]) {
                AddEffectTargetUnchecked(ref, p, zn);
                (*st)++;
            } else
                PlaySE(3);
        }
        return 0;
    default:
        return 1;
    }
}
int EffectFlipOpponentSetMonsterChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0: {
        int i;
        int found;
        ref->numTargets = 0;
        found = 0;
        for (i = 0; i <= 4; i++) {
            if ((*(u32 *)ZB2((1 - ref->player) & 1, i) << 20) != 0 && (ZB2((1 - ref->player) & 1, i)->flags6 & 3) == 1)
                found = 1;
        }
        if (found == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToFlip);
        gChain[0x3E5]++;
        break;
    }
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0x900000) != 0) {
            if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
                return 1;
        }
        break;
    }
    return 0;
}
int EffectFaceUpMagicTargetChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMagicForMask);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0x40004) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        if (p != ref->player || zn != ref->zone) {
            if (TryAddEffectTarget(ref, p, zn) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}
int EffectTributeTwoMonstersChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterYouWishToTribute);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF0) == 0)
            return 0;
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0) {
            (*st)++;
            return 0;
        }
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateAnotherMonsterToTribute);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF0) == 0)
            return 0;
        {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            if (((u8)p | (u8)zn << 8) != ref->targets[0]) {
                if (TryAddEffectTarget(ref, p, zn) != 0)
                    return 1;
            }
            PlaySE(3);
        }
        return 0;
    default:
        return 0;
    }
}
int EffectDeclareAttributeEquipChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        DuelPrompt_Post(ref->player, 9, 0, 0);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        AddEffectTarget(ref, gDuel.w1B64 + 1);
        (*st)++;
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToEquip);
        (*st)++;
        return 0;
    default:
        if (gMain.h6 & 2) {
            u8 *e2 = gChain;
            u8 *q = e2 + 0x3E5;
            int z = 0;

            *q = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            if (EffectEquipTargetCheck(ref, (u8)p | (u8)zn << 8) != 0) {
                if (TryAddEffectTarget(ref, p, zn) != 0)
                    return 1;
            }
            PlaySE(3);
        }
        return 0;
    }
}
int EffectReturnTwoSpellTrapsChainB(struct CardRef *ref, int a)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (EffectTwoSpellTrapsOnFieldPrepare(ref, a, 0) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFirstCardToReturn);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E) == 0)
            return 0;
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0) {
            (*st)++;
            return 0;
        }
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateSecondCardToReturn);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E) == 0)
            return 0;
        {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            u16 w = (u8)p | (u8)zn << 8;
            if (ref->targets[0] != w) {
                if (TryAddEffectTarget(ref, p, zn) != 0)
                    return 1;
            }
            PlaySE(3);
        }
        return 0;
    default:
        return 0;
    }
}
int EffectSwitchAttackerChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectReplacementAttacker);
        ref->numTargets = 0;
        (*st)++;
    } else if (DuelCursor_PickTarget(0xE00000) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        int pl = 1 & p;
        if ((*(u32 *)ZB2(pl, zn) << 20) != 0 && (ref->pos >> 8) != zn) {
            if (TryAddEffectTarget(ref, p, zn) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}
/*
 * Card level as a u8 inline with a return per case: the QImode result pseudo
 * is copied to its use through a subreg, which reproduces the ROM's level in
 * r0 plus a register copy (`adds r4,r0,#0` / `adds r1,r0,#0`).
 */
static inline u8 CardLevelU8(u32 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS(id) & 0x1E000000) >> 25;
    }
}
int EffectBanishGraveToDestroyChainB(struct CardRef *ref)
{
    char buf1[0x40];
    char buf2[0x80];
    u8 *es;
    int sw = gChain[0x3E5];
    es = gChain;
    switch (sw) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToDestroy);
        gChain[0x3E5]++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            u8 *q = es + 0x3E5;
            int z = 0;
            *q = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E0) == 0)
            goto ret0;
        {
            u32 p = gDuelScreen.w824;
            int zn = gDuelScreen.w828 + gDuelScreen.w82C;
            int pl = 1 & p;
            struct DuelZone *zp = ZB(pl, zn);
            u32 id = (*(u32 *)zp << 20) >> 20;
            if (CardLevelU8(id) <= CountGraveyardMonsters(ref->player)) {
                if (TryAddEffectTarget(ref, p, zn) != 0) {
                    AddEffectTarget(ref, CardLevelU8(id));
                    gChain[0x3E6] = CardLevelU8(id);
                    gChain[0x3E5]++;
                    return 0;
                }
            }
            PlaySE(3);
        }
        goto ret0;
    case 2:
        FormatInt(buf1, gStrSelectGraveMonstersToBanishFmt, *(es + 0x3E6));
        TextBoxOpen(0x206, 0x712, 0xB, buf1);
        (*(es + 0x3E5))++;
        return 0;
    case 3:
        CardListView_Open(ref->player, -1, 0x5FC, 0);
        gChain[0x3E5]++;
        return 0;
    case 4: {
        u32 *c = &gCardListView.cards[gCardListView.top + gCardListView.row];
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x80D4 : 0xD4;
        DuelCmd_Push(msg, ((u16 *)c)[0], ((u16 *)c)[1], 0);
        gChain[0x3E6]--;
        gChain[0x3E5]++;
        return 0;
    }
    case 5:
        /* The positive test keeps the fall-through label-free, so post-reload
         * CSE turns the second count load into `adds r2,r0,#0`; the ret0 label
         * here makes this tail merge into case 4's `return 0`. */
        if (*(es + 0x3E6) != 0) {
            FormatInt(buf2, gStrRemainingCountFmt, *(es + 0x3E6));
            TextBoxOpen(0x206, 0x712, 0xB, buf2);
            *(es + 0x3E5) = 3;
ret0:
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}
int EffectReturnOpponentSpellTrapChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentSpellTrapToReturn);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xE0000) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}
int EffectForceActivateTrapChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectTrapToForceActivate);
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0x20002) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}
int EffectReturnFusionToDeckChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFusionToReturnToDeck);
        (*st)++;
    } else if (gMain.h6 & 2) {
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xF000F0) != 0) {
        u32 p = gDuelScreen.w824;
        int zn = gDuelScreen.w828 + gDuelScreen.w82C;
        if (EffectFaceUpFusionMonsterCheck(ref, (u8)p | (u8)zn << 8) != 0) {
            if (TryAddEffectTarget(ref, p, zn) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}
/* Preserve the zero-extended halfword result for word-valued callers. */
int CanActivateFieldCard(struct CardRef *ref, int p, int z)
{
    u32 id;
    int f;
    int pl = 1 & p;
    struct DuelZone *zp = ZB(pl, z);
    id = (*(u32 *)zp << 20) >> 20;
    f = ((u32)zp->flags6 << 30) >> 31;
    if (id == 0)
        return 0;
    if (CARD_TYPE(id) <= 0x14)
        return 0;
    if ((((u32)((u8 *)gDuelZones)[0x1AE6] << 30) >> 31) != p) {
        if (GetCardSpellSpeed(id) <= 1)
            return 0;
    }
    switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
    case 0x52C:
    case 0x3F9:
    case 0x594:
    case 0x5FC:
        f = 0;
        break;
    }
    if (f != 0)
        return 0;
    {
        int pl2 = 1 & p;
        struct DuelZone *z2 = ZB(pl2, z);
        if (!z2->f91_2)
            return 0;
        if (z2->f91_3)
            return 0;
    }
    if (CARD_TYPE(id) == 0x15) {
        int n = 0x2EF;
        if (CountActiveCardsOnField(0, n) != 0)
            return 0;
        if (CountActiveCardsOnField(1, n) != 0)
            return 0;
    }
    if (GetCardSpellSpeed(id) > 1)
        goto fill;
    {
        struct DG12 *g = (struct DG12 *)&gDuel;
        u32 b = g->b;
        u32 t = (b << 27) >> 29;
        if (t == 2 || t == 4) {
            if (((b << 30) >> 31) == p)
                goto fill;
        }
    }
ret0:
    return 0;
fill:
    ref->id = id;
    ref->player = p & 1;
    ref->zone = z & 0x3F;
    {
        /* FAKEMATCH: preserve the ROM's p/z registers without emitting code. */
        __asm__("" : : : "r0");
        switch ((int)CARD_TYPE(id & 0x7FF)) {
        case 0x15:
        case 0x16:
            if ((gDuelPlayers[1 & p].b7 >> 6) != 0)
                goto ret0;
            break;
        }
    }
    return (u16)CanActivateEffect(ref, 0, 0);
}
