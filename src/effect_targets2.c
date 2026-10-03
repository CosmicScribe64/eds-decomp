#include "global.h"

/* Duel target pickers, continued from effect_targets1 (see wiki/functions/code-0803dd7c.md). */
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
u16 TryAddEffectTarget(struct CardRef *ref, int player, int zone);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
u32 DuelCursor_PickTarget(u32 keys);
extern u8 gChain[];
extern const u8 gStrDesignateOpponentFaceDownCard[];
extern const u8 gStrAskReturnMonsterToHand[];
extern const u8 gStrDesignateMonsterToReturnToHand[];
extern const u8 gStrAskReturnAnotherMonster[];
int CanActivateEffect(struct CardRef *ref, int a, int b);
int CountMonsters(int player);
struct G5EE8 { u8 unk0[4]; u32 w4; };
extern struct G5EE8 gDuelCtrl;
extern const u8 gStrDesignateMonsterToSwitchControl[];
extern const u8 gStrDesignateMonsterToControl[];
extern const u8 gStrDesignateFaceUpMonsterToControl[];
extern const u8 gStrDesignateOpponentMonsterTarget[];
int AiFindStrongestMonster(int a, int b, int c, int d);
int CanCardTargetZone(u16 id, int a, int b);
extern const u8 gStrDesignateFirstOwnMonster[];
extern const u8 gStrDesignateSecondOwnMonster[];
extern const u8 gStrDesignateOpponentMonsterToDestroy[];
extern const u8 gStrDesignateOneMonsterToDestroy[];
extern const u8 gStrDesignateOneMonster[];
int GetZoneCardAtk(int player, int zone);
extern const u8 gStrDesignateMonsterToEquip[];
extern const u8 gStrAskSevenCompletedStat[];
extern const u8 gStrSelectNewAttackTarget[];
int EffectEquipTargetCheck(struct CardRef *ref, u16 pos);
int EffectMagicArmShieldCheck(struct CardRef *ref, u16 pos);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
void AddEffectTarget(struct CardRef *ref, u16 v);
struct AE60 { u8 unk0[0x14]; u16 flag14; };
extern struct AE60 gTextBox;
extern const u8 gStrDesignateFirstCardToDestroy[];
extern const u8 gStrDesignateSecondCardToDestroy[];
int EffectGreenkappaPrepare(struct CardRef *ref);
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 20;
};
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 unk7[0x94 - 7];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
extern const u8 gStrDesignateOneOwnMonster[];
extern const u8 gStrDesignateOwnMonsterToRecall[];
extern const u8 gStrDesignateOwnMonsterToBanish[];
extern const u8 gStrDesignateOwnMonsterToEquip[];
extern const u8 gStrDesignateFaceDownDefenseMonster[];
extern const u8 gStrSelectFaceUpTrapToDestroy[];
int CountMonstersFiltered(int player, int a, int b);
/* Step byte of the current target picker is gChain[0x3E5]. */
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gMain;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gDuelScreen;

/* Prompt, wait for keys 0xD2 << 16, add the cursor position as a target. */
int EffectPatrolRoboChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentFaceDownCard);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xD2 << 16) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
        PlaySE(3);
    }
    return 0;
}

/* EffectGreenkappaPrepare is a condition callback that ignores its arguments; this caller
 * passes its own second argument through to it (the ROM leaves it in r1 from
 * entry to the call). The unit's prototype only names ref. */
typedef int (*CondFunc_0803EE6C)(struct CardRef *ref, int arg);

/* (ref, arg): AI: add the first two occupied, unflagged spell/trap zones (5-9); player 0: gate on EffectGreenkappaPrepare(ref, arg), then a two-step pick where the second target must differ from the first. */
int EffectGreenkappaChainB(struct CardRef *ref, int arg)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int n = 0;
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flags6 & 2)) {
                    TryAddEffectTarget(ref, i, j);
                    n++;
                    if (n == 2)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (((CondFunc_0803EE6C)EffectGreenkappaPrepare)(ref, arg) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFirstCardToDestroy);
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
        reset:
            *st = pl;
            return 0;
        }
        if (DuelCursor_PickTarget(0x20002) != 0) {
            if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0) {
                (*st)++;
                return 0;
            }
            PlaySE(3);
        }
        return 0;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateSecondCardToDestroy);
        (*st)++;
        return 0;
    case 3:
        if (gMain.h6 & 2)
            goto reset;
        if (DuelCursor_PickTarget(0x20002) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos && TryAddEffectTarget(ref, p, z) != 0)
                return 1;
            PlaySE(3);
        }
        return 0;
    default:
        return 0;
    }
}
/* (ref, arg): AI picks up to two player-1 zones (card numbers 0x10-0x14) or falls back to AiFindStrongestMonster; player 0: 6-step prompt machine. */
int EffectPenguinSoldierChainB(struct CardRef *ref, int arg)
{
    if (1 & ((u8 *)ref)[2]) {
        int i;
        ref->numTargets = 0;
        if (CanActivateEffect(ref, arg, 0) == 0)
            return 1;
        if (CountMonsters(0) <= 0)
            return 1;
        for (i = 0; i <= 1; i++) {
            int none = 0xFFFF; /* compared against; loop.c hoists it to sl */
            u16 cand = 0xFFFF;
            if (gDuelCtrl.w4 & 0x200) {
                int j;
                for (j = 0; j <= 4; j++) {
                    if ((*(u32 *)ZB(1, i) << 20 >> 20) != 0) {
                        switch (((const u16 *)0x08622AB4)[*(u32 *)ZB(1, j) << 21 >> 21]) {
                        case 0x10 ... 0x14: /* range: `cmp #0x14; bgt` then `cmp #0x10; blt` */
                            if (i != 0) {
                                if (ref->targets[0] == (u16)((u8)j << 8 | 1))
                                    continue;
                            }
                            cand = (u8)j << 8 | 1;
                            j = 5;
                        }
                    }
                }
            }
            if (cand == none) {
                int m = -1;
                int r;
                if (i > 0)
                    m = 0;
                r = AiFindStrongestMonster(0, m, 1, 1);
                if (r >= 0)
                    cand = (u8)r << 8;
            }
            if (i > 0 && cand == ref->targets[0])
                cand = 0xFFFF;
            if (cand == none)
                return 1;
            TryAddEffectTarget(ref, (u8)cand, (u8)(cand >> 8));
        }
        /* Shares case 5's `return 1` (the ROM keeps a single r0=1 block after case 5). */
        goto ret1;
    } else {
        u8 *es = gChain;
        int s = es[0x3E5];
        u8 *e2 = es;
        switch (s) {
        case 0:
            ref->numTargets = 0;
            if (CanActivateEffect(ref, arg, 0) == 0)
                return 1;
            if (CountMonsters(0) + CountMonsters(1) == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrAskReturnMonsterToHand);
            TextBoxSetMenu(1, 0, 0);
            { u8 *e = gChain; e[0x3E5]++; }
            return 0;
        case 1:
        case 4:
            if (gTextBox.flag14 == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToReturnToHand);
            { u8 *e = gChain; e[0x3E5]++; }
            return 0;
        case 2:
            if (gMain.h6 & 2)
                goto reset;
            if (DuelCursor_PickTarget(0xF000F0) == 0)
                return 0;
            if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0) {
                int n = CountMonsters(0);
                n += CountMonsters(1);
                if (n == 1)
                    return 1;
                { u8 *e = gChain; e[0x3E5]++; }
            }
            PlaySE(3);
            return 0;
        case 3:
            TextBoxOpen(0x206, 0x712, 0xB, gStrAskReturnAnotherMonster);
            TextBoxSetMenu(1, 0, 0);
            { u8 *e = gChain; e[0x3E5]++; }
            return 0;
        case 5:
            if (gMain.h6 & 2) {
            reset:
                {
                    u8 *q = e2 + 0x3E5;
                    u8 zz = 0;
                    *q = zz;
                    return zz;
                }
            }
            if (DuelCursor_PickTarget(0xF000F0) == 0)
                return 0;
            {
                u8 *base = (u8 *)&gDuelScreen;
                u32 *pa = (u32 *)(base + 0x824);
                int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
                int p = *pa;
                int pos = (u8)z << 8 | *(u8 *)pa;
                if (ref->targets[0] == pos || TryAddEffectTarget(ref, p, z) == 0) {
                    u32 se = 3;
                    /* FAKEMATCH: keeps this sound call from being cross-jumped
                     * into case 2's identical one (the ROM has both). Emits no code. */
                    asm("" : "+r"(se));
                    PlaySE(se);
                    return 0;
                }
            }
        ret1:
            return 1;
        default:
            return 0;
        }
    }
}
/* AI: AiFindStrongestMonster(0, -1, 1, 1) result becomes target 0; player 0: per-card prompt (only if CountMonstersFiltered allows), then the pick is validated per card number (0x42C: not 0x547, 0x42C/0x4DC: face-down flag 2). */
int EffectTakeControlChainB(struct CardRef *ref)
{
    int one;
    int pl;
    u32 keys;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int m;
        int r;
        ref->numTargets = 0;
        m = -1;
        r = AiFindStrongestMonster(0, m, 1, one);
        if (r > m)
            TryAddEffectTarget(ref, 0, r);
        return 1;
    }
    if (gChain[0x3E5] == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x280:
            if (CountMonstersFiltered(one - ref->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToSwitchControl);
            break;
        case 0x403:
            if (CountMonstersFiltered(one - ref->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToControl);
            break;
        case 0x42C:
        case 0x5EA:
            if (CountMonstersFiltered(1 - ref->player, 1, 0) == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFaceUpMonsterToControl);
            break;
        default:
            if (CountMonstersFiltered(1 - ref->player, 0, 0) == 0)
                return 1;
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterTarget);
            break;
        }
        { u8 *e = gChain; e[0x3E5]++; }
        return 0;
    }
    switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
    case 0x280:
    case 0x403:
        keys = 0xF0 << 16;
        break;
    case 0x42C:
    case 0x4DC:
    case 0x5EA:
        keys = 0xE0 << 16;
        break;
    }
    if (DuelCursor_PickTarget(keys) != 0) {
        u8 *base = (u8 *)&gDuelScreen;
        u32 *pa = (u32 *)(base + 0x824);
        int p = *pa;
        int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        int pp = 1 & p;
        struct DuelZone *zn = ZB(pp, z);
        u16 id = (*(u32 *)zn << 20) >> 20;
        if (CanCardTargetZone(ref->id, p, z) != 0) {
            switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
            case 0x42C:
                if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x547) {
                snd:
                    PlaySE(3);
                    return 0;
                }
            case 0x4DC:
                {
                    int pq = 1 & p;
                    if (!(ZB2(pq, z)->flags6 & 2))
                        goto snd;
                }
                break;
            }
            TryAddEffectTarget(ref, p, z);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}

/* Prompt only if CountMonstersFiltered(p, 1, 0) allows it (else done at once), keys 0xE0. */
int EffectKunaiWithChainChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (CountMonstersFiltered(ref->player, 1, 0) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOwnMonsterToEquip);
        (*st)++;
    } else if (gMain.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    } else if (DuelCursor_PickTarget(0xE0) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Prompt (gStrDesignateFaceDownDefenseMonster), then keys 0x900090 add the cursor position unconditionally. */
int EffectAcidTrapHoleChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFaceDownDefenseMonster);
        (*st)++;
        return 0;
    }
    if (gMain.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (DuelCursor_PickTarget(0x900090) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}

/* AI: per side, the face-down (flag 2) zone with the highest GetZoneCardAtk value; player 0: per-card prompt (0x3AB / 0x5AB), keys 0xE000E0. */
int EffectTargetableFaceUpMonsterChainB(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int best = -1;
            int bestZ = -1;
            int j;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && (z->flags6 & 2)) {
                    int v = GetZoneCardAtk(i, j);
                    if (v > best) {
                        best = v;
                        bestZ = j;
                    }
                }
            }
            if (bestZ >= 0) {
                if (TryAddEffectTarget(ref, i, bestZ) != 0)
                    return 1;
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x3AB:
        case 0x5AB:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOneMonsterToDestroy);
            break;
        default:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOneMonster);
            break;
        }
        { u8 *e = gChain; e[0x3E5]++; }
    } else if (gMain.h6 & 2) {
        *st = pl;
    } else if (DuelCursor_PickTarget(0xE000E0) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Per-card prompt (numbers 0x3B1, 0x524, 0x527), then keys 0xF0 add the cursor position unconditionally. */
int EffectOwnMonsterTargetChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x3B1:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOneOwnMonster);
            break;
        case 0x524:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOwnMonsterToRecall);
            break;
        case 0x527:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOwnMonsterToBanish);
            break;
        }
        { u8 *e = gChain; e[0x3E5]++; }
        break;
    case 1:
        if (gMain.h6 & 2) {
            u8 z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xF0) != 0) {
            TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
            return 1;
        }
        break;
    }
    return 0;
}

/* Prompt, cursor pick accepted by EffectEquipTargetCheck, then a second prompt (0x613), and finally the value 0x0201AE60+0x14 + 1 as a target. */
int EffectSevenCompletedChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToEquip);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (gMain.h6 & 2) {
            u8 z = 0;
            *st = z;
            return z;
        }
        if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if (EffectEquipTargetCheck(ref, (u8)z << 8 | *(u8 *)pa) != 0) {
                TryAddEffectTarget(ref, p, z);
                (*st)++;
                return 0;
            }
            PlaySE(3);
        }
        break;
    case 2:
        TextBoxOpen(0x206, 0x613, 0xB, gStrAskSevenCompletedStat);
        TextBoxSetMenu(2, 0, 0);
        (*st)++;
        return 0;
    case 3:
        AddEffectTarget(ref, gTextBox.flag14 + 1);
        return 1;
    }
    return 0;
}

/* AI: first zone 0-4 of the opponent side accepted by EffectMagicArmShieldCheck; player 0: prompt (gStrSelectNewAttackTarget), keys 0xE0 << 16, pick must be accepted (opponent side). */
int EffectMagicArmShieldChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (1 & ((u8 *)ref)[2]) {
            int i;
            for (i = 0; i <= 4; i++) {
                if (EffectMagicArmShieldCheck(ref, (u8)(1 - ref->player) | (u8)i << 8) != 0) {
                    TryAddEffectTarget(ref, 1 - ref->player, i);
                    return 1;
                }
            }
            return 1;
        }
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectNewAttackTarget);
        (*st)++;
    } else if (DuelCursor_PickTarget(0xE0 << 16) != 0) {
        u8 *base = (u8 *)&gDuelScreen;
        u32 *pa = (u32 *)(base + 0x824);
        int p = *pa;
        int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        if (EffectMagicArmShieldCheck(ref, (u8)(1 - ref->player) | (u8)*(u32 *)(base + 0x82C) << 8) != 0) {
            TryAddEffectTarget(ref, p, z);
            return 1;
        }
        PlaySE(3);
    }
    return 0;
}

/* Prompt (gStrSelectFaceUpTrapToDestroy), then keys 0x80008 add the cursor position unconditionally. */
int EffectRemoveTrapChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectFaceUpTrapToDestroy);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    }
    if (gMain.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (DuelCursor_PickTarget(0x80008) != 0) {
        TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C);
        return 1;
    }
    return 0;
}

/* 6-step picker: three cursor picks with prompts between them; the third
 * pick (keys 0xF0 << 16) must succeed to finish. */
int EffectTwoProngedAttackChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    int s = es[0x3E5];
    u8 *e2 = es;
    switch (s) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateFirstOwnMonster);
        ref->numTargets = 0;
        goto inc;
    case 1:
        if (gMain.h6 & 2)
            goto reset;
        if (DuelCursor_PickTarget(0xF0) == 0)
            goto ret0;
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) == 0)
            goto ret0;
        goto inc;
    case 2:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateSecondOwnMonster);
        {
            u8 *e = gChain;
            register int off __asm__("r2") = 0x3E5;
            u8 *p;
            /* FAKEMATCH: retain this initialized offset in r2 so this arm
             * keeps its address setup. The signed subtraction is e + off;
             * it preserves the ROM's ADD operand order. No instruction is
             * emitted by the empty constraint. */
            __asm__("" : "+r"(off));
            p = e - (-off);
            (*p)++;
        }
        goto ret0;
    case 3:
        if (gMain.h6 & 2)
            goto reset;
        if (DuelCursor_PickTarget(0xF0) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos) {
                if (TryAddEffectTarget(ref, p, z) != 0) {
                    u8 *e = gChain;
                    int off = 0x3E5;
                    u8 *next = e + off;
                    (*next)++;
                }
            } else {
                u32 se = 3;
                /* FAKEMATCH: retain the initialized sound id separately
                 * from case 5's call, preserving this call's branch tail.
                 * The empty constraint emits no instruction. */
                __asm__("" : "+r"(se));
                PlaySE(se);
            }
        }
        goto ret0;
    case 4:
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToDestroy);
    inc:
        { u8 *e = gChain; e[0x3E5]++; }
    ret0:
        return 0;
    case 5:
        if (gMain.h6 & 2) {
        reset:
            {
                u8 *q = e2 + 0x3E5;
                u8 zz = 0;
                *q = zz;
                return zz;
            }
        }
        if (DuelCursor_PickTarget(0xF0 << 16) != 0) {
            if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
                return 1;
            PlaySE(3);
        }
        goto ret0;
    default:
        return 1;
    }
}


