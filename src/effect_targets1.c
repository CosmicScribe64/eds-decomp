#include "global.h"
#include "main.h"
#include "duel.h"

/* Duel target helpers. struct CardRef is described in wiki/functions/code-08030b88.md. */
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
void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int CanCardTargetZone(u16 id, int a, int b);
void AddEffectTarget(struct CardRef *ref, u16 v);
u16 TryAddEffectTarget(struct CardRef *ref, int a, int z);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
u32 DuelCursor_PickTarget(u32 keys);
int CollectEffectTargets(int player, int number, int b);
int AiPickCardListEntry(u16 id);
void sub_08019820(int player, int id);
void CardListView_Open(int player, int area, int a2, int a3);
int AiPickEffectTribute(int a);
extern const u8 gStrAskWhoseLpToRecover[];
extern const u8 gStrDesignateMonsterForAttackPosition[];
extern const u8 gStrDesignateOwnTribute[];
extern const u8 gStrSelectListTarget[];
extern const u8 gStrDesignateMonsterToDestroy[];
extern const u8 gStrDesignateMonsterToHaveReturned[];
extern const u8 gStrDesignateOneMonster[];
int CanActivateEffect(struct CardRef *ref, int a, int b);
int AiFindWeakestMonster(int a, int b, int c, int d);
extern const u8 gStrDesignateTypeMonsterToDestroyFmt[];
extern const u8 gStrDragonType[];
int EffectDragonSeekerCheck(struct CardRef *ref, u16 pos);
void FormatStr(void *dst, const void *a, const void *b);
extern const u8 gStrDesignateMagicToDestroy[];
int CountSpellTrapsFiltered(int a, int b, int c, int d);
extern const u8 gStrDesignateMonsterToEquip[];
int EffectEquipTargetCheck(struct CardRef *ref, u16 pos);
int GetZoneCardAtk(int player, int zone);
#define LP gDuelPlayers
extern const u8 gStrAskDestroyMonster[];
extern const u8 gStrDesignateAtk1000MonsterToDestroy[];
extern const u8 gStrSelectAnotherMonsterToDestroy[];
int EffectBlastJugglerPrepare(struct CardRef *ref, int a, int b);
int EffectBlastJugglerCheck(struct CardRef *ref, u16 pos);
extern const u8 gStrDesignateOpponentMonsterToReturn[];
extern const u8 gStrDesignateOpponentMonsterCardToDestroy[];
extern const u8 gStrDesignateOpponentMonsterToAbsorb[];
extern const u8 gStrDesignateOpponentMonsterTarget[];
int CountMonsters(int player);
int AiFindStrongestMonster(int a, int b, int c, int d);
extern const u8 gStrDesignateTrapToDestroy[];
extern const u16 gCardIdToNumber[];
extern u8 gChain[];
/* Step counter of the current target-selection routine (0 = prompt, 1 = wait for input, ...). */
#define SEL_STEP gChain[0x3E5]
struct AE60 { u8 unk0[0x14]; u16 flag14; };
extern struct AE60 gTextBox;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gDuelScreen;
struct ListView {
    u8 unk0[5];
    u8 row : 2;
    u8 unk5_2 : 6;
    u16 top;
    u8 unk8[4];
    u32 cards[0x80];
};
extern struct ListView gCardListView;
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern u32 gCardListViewCards[];   /* = gCardListView.cards */
#define CARD_NUMBER(id) (gCardIdToNumber[0x7FF & (id)])

/* Append a packed (zone << 8 | player) target word. */
void AddEffectTarget(struct CardRef *ref, u16 v)
{
    if (ref != 0) {
        ref->targets[ref->numTargets] = v;
        ref->numTargets++;
    }
}

/* Add target (a, zone) if the card can be placed; msg shows the zone (0-4 monster, 5-9 spell/trap, 10 field). */
u16 TryAddEffectTarget(struct CardRef *ref, int a, int z)
{
    int hi = 0;
    int lo = z;
    if (z > 4) {
        hi = 5;
        lo = z - 5;
    }
    if (z == 10) {
        hi = 10;
        lo = 0;
    }
    if (CanCardTargetZone(ref->id, a, z) != 0) {
        if (!(1 & ((u8 *)ref)[2]))
            PlaySE(1);
        DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, a, (u8)lo << 8 | hi, 0);
        AddEffectTarget(ref, (u8)a | (u8)z << 8);
        return 1;
    }
    return 0;
}

/* Same without the placement test. */
void AddEffectTargetUnchecked(struct CardRef *ref, int a, int z)
{
    int hi = 0;
    int lo = z;
    if (z > 4) {
        do { hi = 5; lo = z - 5; } while (0); /* FAKEMATCH: block form reproduces the ROM's hi/lo register order */
    }
    if (z == 10) {
        hi = 10;
        lo = 0;
    }
    if (!(1 & ((u8 *)ref)[2]))
        PlaySE(1);
    DuelCmd_Push((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, a, (u8)lo << 8 | hi, 0);
    AddEffectTarget(ref, (u8)a | (u8)z << 8);
}
/* Pick a card from the list viewer (its 12-bit id and the two halves become targets). */
int EffectCardListTargetChainB(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    u16 *c;
    if (1 & ((u8 *)ref)[2]) {
        int i;
        ref->numTargets = 0;
        i = AiPickCardListEntry(ref->id);
        if (i >= 0) {
            c = (u16 *)((u8 *)gCardListViewCards + (i << 2));
            sub_08019820(ref->player, *(u32 *)c << 20 >> 20);
            AddEffectTarget(ref, c[0]);
            AddEffectTarget(ref, c[1]);
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (CollectEffectTargets(ref->player, ((const u16 *)0x08622AB4)[0x7FF & ref->id], 0) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectListTarget);
        (*st)++;
        return 0;
    case 1:
        CardListView_Open(ref->player, -1, ((const u16 *)0x08622AB4)[0x7FF & ref->id], 0);
        (*st)++;
        return 0;
    default:
        sub_08019820(ref->player, gCardListView.cards[gCardListView.top + gCardListView.row] << 20 >> 20);
        c = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.row];
        AddEffectTarget(ref, c[0]);
        AddEffectTarget(ref, c[1]);
        return 1;
    }
}

/* Auto-pick / choose a spell-trap zone target (zones 5-9): first the face-down ones of type 0x15, then any card not flagged. */
int EffectTrapTargetChainB(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u32 id = (*(u32 *)z << 20) >> 20;
                if (id != 0 && (z->flag6_1) && CARD_TYPE((u16)id) == 0x15) {
                    if (TryAddEffectTarget(ref, i, j) != 0)
                        return 1;
                }
            }
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flag6_1)) {
                    if (TryAddEffectTarget(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateTrapToDestroy);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (DuelCursor_PickTarget(0xA000A) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}


/* Select a target for player 0 by cursor; player 1 (AI) asks AiPickEffectTribute for one. */
int EffectTributeTargetChainB(struct CardRef *ref)
{
    int pl = 1 & ((u8 *)ref)[2];
    u8 *es;
    u8 *st;
    if (pl) {
        int m = -1;
        int r = AiPickEffectTribute(m);
        ref->numTargets = 0;
        if (r > m) {
            if (TryAddEffectTarget(ref, 1, r) != 0)
                return 1;
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOwnTribute);
        ref->numTargets = 0;
        (*st)++;
    } else if (gMain.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (DuelCursor_PickTarget(0xF0) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Target selection with a per-card prompt text (numbers 0x5E / 0x77, 0x2E6 / 0x2DA, 0x536). */
int EffectOpponentMonsterChainB(struct CardRef *ref)
{
    int one;
    int pl;
    u8 *es;
    u8 *st;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int m;
        ref->numTargets = 0;
        if (CountMonsters(0) > 0) {
            int r;
            m = one - 2;
            r = AiFindStrongestMonster(0, m, 1, 1);
            if (r > m) {
                if (TryAddEffectTarget(ref, 0, r) != 0)
                    return 1;
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (CountMonsters(one - ref->player) == 0)
            return 1;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x5E:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToReturn);
            break;
        case 0x77:
        case 0x2E6:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterCardToDestroy);
            break;
        case 0x2DA:
        case 0x536:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterToAbsorb);
            break;
        default:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOpponentMonsterTarget);
            break;
        }
        gChain[0x3E5]++;
    } else if (gMain.newKeys & 2) {
        *st = pl;
    } else if (DuelCursor_PickTarget(0xF0 << 16) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
        PlaySE(3);
    }
    return 0;
}


/* AI picks the monster with the highest value (GetZoneCardAtk) among those EffectEquipTargetCheck accepts; player 0 picks with the cursor. */
int EffectEquipTargetChainB(struct CardRef *ref)
{
    int one;
    int pl;
    if ((((u8 *)ref)[2] & 0xE) == 6)
        return 1;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int side;
        int bestVal;
        int bestP;
        int bestZ;
        int i;
        ref->numTargets = 0;
        side = ref->player;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x290:
            if (LP[1 & ref->player].lifePoints > LP[(one - ref->player) & one].lifePoints)
                side = one - ref->player;
            break;
        case 0x416:
        case 0x417:
        case 0x58B:
        case 0x60C:
            side = 1 - ref->player;
            break;
        }
        bestVal = -1;
        bestP = -1;
        bestZ = -1;
        for (i = 0; i <= 1; i++, side = 1 - side) {
            int j;
            int sb;
            j = 0;
            sb = (u8)side;
            for (; j <= 4; j++) {
                if (EffectEquipTargetCheck(ref, (u8)j << 8 | sb) != 0 && bestVal < GetZoneCardAtk(side, j)) {
                    bestVal = GetZoneCardAtk(side, j);
                    bestP = side;
                    bestZ = j;
                }
            }
            if (bestVal > -1 && bestP > -1 && bestZ > -1) {
                if (TryAddEffectTarget(ref, bestP, bestZ) != 0)
                    return 1;
            }
        }
        if (bestVal > -1 && bestP > -1 && bestZ > -1) {
            if (TryAddEffectTarget(ref, bestP, bestZ) != 0)
                return 1;
        }
        return 1;
    } else {
        u8 *es = gChain;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToEquip);
            ref->numTargets = 0;
            (*st)++;
        } else if (gMain.newKeys & 2) {
            *st = pl;
        } else if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if ((u16)EffectEquipTargetCheck(ref, (u8)z << 8 | *(u8 *)pa) != 0) {
                if (TryAddEffectTarget(ref, p, z) != 0)
                    return 1;
                return 1;
            }
            PlaySE(3);
        }
    }
    return 0;
}

/* Prompt (text 0x08083E44), wait for a key press, then add the cursor position as a target. */
int EffectStopDefenseChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterForAttackPosition);
        (*st)++;
        goto ret0;
    }
    if (gMain.newKeys & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (DuelCursor_PickTarget(0xB0 << 16) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
    }
ret0:
    return 0;
}

/* Target selection: 1 = done (targets filled), 0 = still waiting. Player 1 (AI) picks by itself. */
int EffectGainLpChosenPlayerChainB(struct CardRef *ref)
{
    u8 *st;
    u8 *es;
    if (1 & ((u8 *)ref)[2]) {
        AddEffectTarget(ref, 0);
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        TextBoxOpen(0x205, 0x514, 0xB, gStrAskWhoseLpToRecover);
        TextBoxSetMenu(2, 0, 0);
        break;
    case 1:
        AddEffectTarget(ref, gTextBox.flag14);
        break;
    default:
        return 1;
    }
    (*st)++;
    return 0;
}

/* Two-target selection (kind 2): first target, then a second one that differs from it. */
int EffectBlastJugglerChainB(struct CardRef *ref)
{
    u8 *es = gChain;
    int s = es[0x3E5];
    u8 *e2 = es;
    u8 *q;
    switch (s) {
    case 0:
        ref->numTargets = 0;
        ref->kind = 2;
        if (EffectBlastJugglerPrepare(ref, 0, 0) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrAskDestroyMonster);
        TextBoxSetMenu(1, 0, 0);
        { u8 *e = gChain; e[0x3E5]++; }
        return 0;
    case 1:
        if (gTextBox.flag14 == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateAtk1000MonsterToDestroy);
        { u8 *e = gChain; e[0x3E5]++; }
        return 0;
    case 2:
        if (gMain.newKeys & 2) {
            q = e2 + 0x3E5;
            goto reset;
        }
        if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if (EffectBlastJugglerCheck(ref, (u8)z << 8 | *(u8 *)pa) != 0
                && TryAddEffectTarget(ref, p, z) != 0) {
                int n = CountMonsters(0);
                n += CountMonsters(1);
                if (n == 1)
                    return 1;
                { u8 *e = gChain; e[0x3E5]++; }
            }
            PlaySE(3);
        }
        return 0;
    case 3:
        TextBoxOpen(0x206, 0x712, 0xB, gStrSelectAnotherMonsterToDestroy);
        { u8 *e = gChain; e[0x3E5]++; }
        return 0;
    case 4:
        if (gMain.newKeys & 2) {
            q = e2 + 0x3E5;
        reset:
            {
                u8 zz = 0;
                *q = zz;
                return zz;
            }
        }
        if (DuelCursor_PickTarget(0xE000E0) != 0) {
            u8 *base = (u8 *)&gDuelScreen;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos && EffectBlastJugglerCheck(ref, pos) != 0 && TryAddEffectTarget(ref, p, z) != 0)
                return 1;
            PlaySE(3);
        }
        return 0;
    default:
        return 0;
    }
}


/* Like EffectTrapTargetChainB for spell/trap zones 5-10 of type 0x16. */
int EffectMagicTargetChainB(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u32 id = (*(u32 *)z << 20) >> 20;
                if (id != 0 && (z->flag6_1) && CARD_TYPE((u16)id) == 0x16) {
                    if (TryAddEffectTarget(ref, i, j) != 0)
                        return 1;
                }
            }
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flag6_1)) {
                    if (TryAddEffectTarget(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (CountSpellTrapsFiltered(0, 0, 0, 1) == 0 && CountSpellTrapsFiltered(1, 0, 0, 1) == 0)
            return 1;
        TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMagicToDestroy);
        { u8 *e = gChain; e[0x3E5]++; }
    } else if (gMain.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (DuelCursor_PickTarget(0x60006) != 0) {
        if (TryAddEffectTarget(ref, gDuelScreen.w824, gDuelScreen.w828 + gDuelScreen.w82C) != 0)
            return 1;
        PlaySE(3);
    }
    return 0;
}

/* Pick a card on a field position accepted by EffectDragonSeekerCheck: AI takes the first, player 0 gets a prompt (text built into a stack buffer). */
int EffectDragonSeekerChainB(struct CardRef *ref)
{
    char buf[0x80];
    u8 *es = gChain;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        int i;
        u8 *st2;
        ref->numTargets = 0;
        i = 0;
        st2 = st;
        for (; i <= 1; i++) {
            int j;
            int sb;
            j = 0;
            sb = (u8)i;
            for (; j <= 4; j++) {
                if (EffectDragonSeekerCheck(ref, (u8)j << 8 | sb) != 0) {
                    if (1 & ((u8 *)ref)[2]) {
                        if (TryAddEffectTarget(ref, i, j) != 0)
                            return 1;
                        return 1;
                    }
                    FormatStr(buf, gStrDesignateTypeMonsterToDestroyFmt, gStrDragonType);
                    TextBoxOpen(0x206, 0x712, 0xB, buf);
                    (*st2)++;
                    return 0;
                }
            }
        }
        return 1;
    } else if (gMain.newKeys & 2) {
        u8 zz = 0;
        *st = zz;
        return zz;
    } else if (DuelCursor_PickTarget(0xE000E0) != 0) {
        u8 *base = (u8 *)&gDuelScreen;
        u32 *pa = (u32 *)(base + 0x824);
        int p;
        int z;

        z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        /* Keep the cursor sum in r4 before loading the player cursor into r5. */
        __asm__("" : "+r"(z));
        p = *pa;
        if (EffectDragonSeekerCheck(ref, (u8)*(u32 *)(base + 0x82C) << 8 | *(u8 *)pa) != 0) {
            if (TryAddEffectTarget(ref, p, z) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}

/* AI: when CanActivateEffect accepts, pick a target via AiFindStrongestMonster / AiFindWeakestMonster, else the first occupied monster zone; player 0 gets a per-card prompt. */
int EffectTargetableMonsterChainB(struct CardRef *ref, int arg)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        if (CanActivateEffect(ref, arg, 0) == 0)
            return 1;
        for (i = 0; i <= 1; i++) {
            if (CountMonsters(i) > 0) {
                int r;
                if (i != 0)
                    r = AiFindWeakestMonster(i, -1, 1, 1);
                else
                    r = AiFindStrongestMonster(0, -1, 1, 1);
                if (r >= 0) {
                    if (TryAddEffectTarget(ref, i, r) != 0)
                        return 1;
                }
            }
        }
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0) {
                    if (TryAddEffectTarget(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gChain;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (CanActivateEffect(ref, arg, 0) == 0 && ((const u16 *)0x08622AB4)[0x7FF & ref->id] != 0x3FF)
            return 1;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x1F4:
        case 0x3FF:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToDestroy);
            break;
        case 0x21C:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateMonsterToHaveReturned);
            break;
        default:
            TextBoxOpen(0x206, 0x712, 0xB, gStrDesignateOneMonster);
            break;
        }
        { u8 *e = gChain; e[0x3E5]++; }
    } else if (gMain.newKeys & 2) {
        *st = pl;
    } else if (DuelCursor_PickTarget(0xF000F0) != 0) {
        int p = gDuelScreen.w824;
        int z = gDuelScreen.w828 + gDuelScreen.w82C;
        if (CanCardTargetZone(ref->id, p, z) != 0) {
            if (TryAddEffectTarget(ref, p, z) != 0)
                return 1;
        }
        PlaySE(3);
    }
    return 0;
}

