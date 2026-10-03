#include "global.h"

int CountActiveCardsOnField(int player, u16 number);
/* 4-byte entries of gRitualRecipes: two 13-bit card numbers (a, num) and a 6-bit count (hypothesis: a fusion/ritual style recipe list) */
struct RecipeEnt { u32 a : 13; u32 num : 13; u32 cnt : 6; };
extern struct RecipeEnt gRitualRecipes[];
extern const u16 gCardIdToNumber[];
struct DuelPlayerC { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
extern struct DuelPlayerC gDuelPlayers[2];
extern const u32 gCardStats[];
struct DuelZoneD { u32 w0; u8 pad[0x94 - 4]; };
struct DuelZonesPlayerD { struct DuelZoneD zones[11]; u8 filler[0xD64 - 11 * 0x94]; };
int IsTributableMonster(int player, int zone);
struct CardListD { u32 w[80]; u8 pad[0xD64 - 0x140]; };
extern struct CardListD gDuelGraveyards[2];
int IsSpecialSummonOnly(u16 id);
int HandHasRitualMonster(int player, int idx);
int SumHandLevelsExcept(int player, u16 x);
int SumTributableMonsterLevels(int player);
struct AE60 { u8 unk0[8]; u16 h8; u16 hA; u8 unkC[2]; u16 hE; u8 unk10[0x21 - 0x10]; u8 b21; };
extern struct AE60 gTextBox;
extern u8 gChain[];
struct F1ACC { u8 lo : 6; u8 b6 : 1; u8 b7 : 1; };
struct F1ACD { u8 b0 : 1; u8 rest : 7; };
extern u8 gDuel[];
extern u8 gUnk_0201ADAC[];
struct G5EE8 { u8 unk0; u8 b1; };
extern struct G5EE8 gDuelCtrl;
extern u8 gUnk_0201ADF2[];
extern const u8 gStrDebugSpellTrapEnabled[], gStrDebugSpellTrapDisabled[];
void DuelCmd_Push(u32 msg, u16 zone, int a, int b);
void DebugPrintf(const u8 *fmt, int p, int z);
void DebugPrintFlush(void);
struct ZoneE { u32 w0; u8 pad[6 - 4]; u8 flags6; u8 pad2[0x91 - 7]; u8 b91; u8 pad3[0x94 - 0x92]; };
struct ZonesE { struct ZoneE z[11]; u8 filler[0xD64 - 11 * 0x94]; };
extern struct ZonesE gDuelZones[2];
void AddSprite(u32 pos, int a, int b);
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 rest[0x12 - 4];
};
int CanSpecialSummon(int player);
extern const u16 gCardNumberToId[];
#define RE_A(e) (((u32)*(u16 *)(e) << 19) >> 19)
#define CARD_STATS(id) (gCardStats[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Is card number 0x2EF or 0x409 on the field/hand of either player? (count of CountActiveCardsOnField, hypothesis) */
int IsJinzoOrRoyalDecreeActive(void)
{
    if (CountActiveCardsOnField(0, 0x2EF) == 0 && CountActiveCardsOnField(1, 0x2EF) == 0
        && CountActiveCardsOnField(0, 0x409) == 0 && CountActiveCardsOnField(1, 0x409) == 0)
        return 0;
    return 1;
}

/* Same for card number 0x482. */
int IsImperialOrderActive(void)
{
    if (CountActiveCardsOnField(0, 0x482) == 0 && CountActiveCardsOnField(1, 0x482) == 0)
        return 0;
    return 1;
}
/* 0x0201ADAD = 0x020192E0 + 0x1ACD: bit 0 card 0x601 present, bits 1-4 tested per Magic/Trap subtype. */
extern u8 gUnk_0201ADAD[];
/* FAKEMATCH: flag tests go through int-typed helpers. A plain u8 & const test is shortened to a
 * QImode and plus a zero-extension, which pushes the zone loop past loop.c's hoist threshold
 * (0x7FF and (p & 1) * 0xD64 then stay in the loop). Pointer-first keeps the hoisted 0x0201ADAD
 * address live across the mask load; mask-first gives the ROM's order for 0x0201ADF2. */
static inline int Flag325C(u8 *f, int mask)
{
    return mask & *f;
}
static inline int Flag325D(int mask, u8 *f)
{
    return mask & *f;
}
static inline int Sub325C(u32 id)
{
    u32 stats = ((const u32 *)0x08621DE0)[id & 0x7FF];
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}
static inline struct ZoneE *Zone325C(int player, int zone, u32 base)
{
    int pp = player & 1;
    return (struct ZoneE *)(zone * 0x94 + pp * 0xD64 + base);
}
void UpdateSpellTrapNegation(void)
{
    u8 *e;
    int p;
    int z;
    /* FAKEMATCH: zone base pinned to r9; as a hoisted constant its doubled REG_EQUIV live length
     * ranks it below (p & 1) * 0xD64. zb2 is a second, spilled copy used only for the r == 0 test
     * so that its load is a round-robin reload. */
    register u32 zb asm("r9");
    u32 zb2 = (u32)gDuelZones;
    int has2EF = IsJinzoOrRoyalDecreeActive();
    e = gDuel;
    ((struct F1ACC *)(e + 0x1ACC))->b7 = has2EF;
    /* FAKEMATCH: the ROM masks a u16 result (callee probably returns u16) */
    ((struct F1ACC *)(e + 0x1ACC))->b6 = ((u16 (*)(void))IsImperialOrderActive)();
    ((struct F1ACD *)(e + 0x1ACD))->b0 = 0;
    if (CountActiveCardsOnField(0, 0x601) != 0 || CountActiveCardsOnField(1, 0x601) != 0)
        ((struct F1ACD *)(e + 0x1ACD))->b0 = 1;
    for (p = 0, zb = (u32)gDuelZones; p <= 1; p++) {
        for (z = 5; z <= 10; z++) {
            struct ZoneE *zn = (struct ZoneE *)((z * 0x94) + ((p & 1) * 0xD64) + zb);
            u32 id;
            int r;
            int f; /* FAKEMATCH: int temporaries keep the byte tests in SImode (see Flag325C) */
            if (((zn->w0 << 20) >> 20) == 0)
                continue;
            if (!((f = zn->flags6) & 2))
                continue;
            r = 0;
            id = (zn->w0 << 20) >> 20;
            switch (((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
            case 0x15:
                if ((f = *(u8 *)(zb + 0x1AA0)) & 0x80)
                    r = 1;
                break;
            case 0x16:
                if ((f = *(u8 *)(zb + 0x1AA0)) & 0x40)
                    r = 1;
                break;
            }
            switch (Sub325C(id)) {
            case 3:
                if (Flag325C(gUnk_0201ADAD, 3))
                    r = 1;
                break;
            case 2:
                if (Flag325C(gUnk_0201ADAD, 4))
                    r = 1;
                break;
            case 4:
                switch (((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
                case 0x15:
                    if (Flag325C(gUnk_0201ADAD, 0x10))
                        r = 1;
                    break;
                case 0x16:
                    if (Flag325C(gUnk_0201ADAD, 8))
                        r = 1;
                    break;
                }
                break;
            }
            switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
            case 0x603:
                r = 0;
                break;
            }
            if (((f = gDuelCtrl.b1) & 1) && Flag325D(2, gUnk_0201ADF2))
                continue;
            if (r == 0) {
                if ((f = Zone325C(p, z, zb2)->b91) & 8) {
                    DuelCmd_Push(p ? 0x80B1 : 0xB1, z, 0, 0);
                    DebugPrintf(gStrDebugSpellTrapEnabled, p, z);
                    DebugPrintFlush();
                }
            }
            if (r != 0) {
                if ((f = ((struct ZoneE *)((z * 0x94) + ((p & 1) * 0xD64) + zb))->b91) & 8)
                    continue;
                r = 1;
                DebugPrintf(gStrDebugSpellTrapDisabled, p, z);
                DebugPrintFlush();
                switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
                case 0x409:
                    if (CountActiveCardsOnField(0, 0x2EF) == 0) {
                        r = 0;
                        if (CountActiveCardsOnField(1, 0x2EF) != 0)
                            r = 1;
                    }
                    break;
                }
                if (r != 0)
                    DuelCmd_Push(p ? 0x80B1 : 0xB1, z, 1, 0);
            }
        }
    }
}
/* Index of the entry whose num equals the card number of id, or -1. */
int FindRitualRecipe(u16 id)
{
    int i = 0;
    struct RecipeEnt *p = gRitualRecipes;
    if (p->num != 0) {
        for (i = 0; gRitualRecipes[i].num != 0; i++) {
            if (gRitualRecipes[i].num == ((const u16 *)0x08622AB4)[0x7FF & id])
                return i;
        }
    }
    return -1;
}
/* Read the full hand word; keep the index, player offset and hand base distinct. */
static inline u32 GetRecipeHandWord(int player, int idx)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(idx * 4 + off + (u32)gDuelPlayers + 0x684);
}
static inline u32 GetRecipeFirstNumber(int idx)
{
    u32 base = (u32)gRitualRecipes;
    u32 off = idx * 4;
    return ((u32)*(u16 *)(base + off) << 19) >> 19;
}
int HandHasRitualMonster(int player, int idx)
{
    u16 i;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 card = GetRecipeHandWord(player, i);
        u16 a = GetRecipeFirstNumber(idx);
        if (*(u16 *)((u8 *)0x08622AB4 + ((card << 21) >> 20)) == a)
            return 1;
    }
    return 0;
}
/* Types 21-23 contribute no level; Divine type 24 contributes ten. */
static inline int GetRecipeCardLevel(u16 id)
{
    int type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    switch (type) {
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
int SumHandLevelsExcept(int player, u16 x)
{
    int total = 0;
    int found = 0;
    int i;
    for (i = 0; i < gDuelPlayers[1 & player].handCount; i++) {
        u16 id = (GetRecipeHandWord(player, i) << 20) >> 20;
        if (found != 0 || id != x) {
            int c = GetRecipeCardLevel(id);
            c += total;
            total = c;
        }
        if (x == id)
            found = 1;
    }
    return total;
}
/* Sum of the "cost" of the player's monster-zone cards that pass IsTributableMonster. */
int SumTributableMonsterLevels(int player)
{
    int total = 0;
    int i;
    for (i = 0; i <= 4; i++) {
        u16 id = (*(u32 *)((player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones) << 20) >> 20;
        if (IsTributableMonster(player, i) != 0) {
            int c = GetRecipeCardLevel(id);
            c += total;
            total = c;
        }
    }
    return total;
}
/* Draw a row of marker sprites (count at 0x02017A40+0x510; the first 0x02017A40+0x3E1 use tile 0x431E, the rest 0x431D). */
void DrawRitualStarGauge(void)
{
    int x0 = (gTextBox.h8 + 1) << 3;
    int y = (gTextBox.hA - (gTextBox.hE - gTextBox.b21)) << 3;
    int i;
    y += 8;
    for (i = 0; i < gChain[0x510]; i++) {
        AddSprite((x0 + i * 10) | (y << 16), 0, i < gChain[0x3E1] ? 0x431E : 0x431D);
    }
}
/* Effect queue at 0x02017A40: 0x14-byte entries from +0x280, entry count at +0x3C0, remaining cost at +0x3E1. */
struct EffEnt37CC { u16 id; u8 player : 1; u8 rest2 : 7; u8 pad[0x14 - 3]; };
struct EffState37CC { u8 pad[0x280]; struct EffEnt37CC ent[16]; u16 count; u8 pad3C2[0x3E1 - 0x3C2]; u8 need; };
#define EQ ((struct EffState37CC *)gChain)
#define EQ_CUR (EQ->ent[EQ->count - 1])
struct Scr828 { u8 pad[0x828]; u32 mode; u32 sel; };
extern struct Scr828 gDuelScreen;
extern u32 gDuelHands[];
int CountMonsters(int player);
u32 DuelCursor_PickTarget(u32 msg);
int CountHandCardsByNumber(int player, u16 number);
void TributeMonster(int player, int zone);
void DiscardHandCard(int player, int a, int b, int c);
void PlaySE(int se);
#define CARD_ID(w) (((w) << 20) >> 20)
/* Tribute selection step (hypothesis): the chosen card's level is taken off the remaining cost at
 * 0x02017A40+0x3E1; returns 1 once it is paid. Mode 11 picks from the hand, mode 0 from the monster zones.
 * The s8 level passed between the two inline helpers produces the ROM's join-point copy (adds r2, r0, #0). */
static inline void PayCost37CC(s8 lv)
{
    if (EQ->need < lv)
        EQ->need = 0;
    else
        EQ->need -= lv;
}
/* Level of a card of the given type: 0 for types 0x15-0x17, 10 for 0x18, else stats bits 25-28. */
static inline s8 Lv37CC(int type, u16 id)
{
    switch (type) {
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
int RitualTributeSelectStep(void)
{
    u32 msg = 0xF0;
    u16 id;
    FindRitualRecipe(EQ_CUR.id);
    if (CountMonsters(EQ_CUR.player) <= 4)
        msg = 0xF1;
    if (DuelCursor_PickTarget(msg) != 0) {
        switch (gDuelScreen.mode) {
        case 11: {
            int p;
            u32 type;
            p = EQ_CUR.player;
            id = CARD_ID(*(u32 *)(p * 0xD64 + gDuelScreen.sel * 4 + (u32)gDuelHands));
            if (((const u16 *)0x08622AB4)[id & 0x7FF] == gRitualRecipes[FindRitualRecipe(EQ_CUR.id)].a && CountHandCardsByNumber(EQ_CUR.player, ((const u16 *)0x08622AB4)[id & 0x7FF]) <= 1) {
                PlaySE(3);
                return 0;
            }
            type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            if (type <= 0x14) {
                PayCost37CC(Lv37CC(type, id));
                DiscardHandCard(EQ_CUR.player, gDuelScreen.sel, 0, 1);
            }
            break;
        }
        case 0: {
            int p;
            u32 type;
            p = EQ_CUR.player;
            id = CARD_ID(*(u32 *)(p * 0xD64 + gDuelScreen.sel * 0x94 + (u32)gDuelZones));
            type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            if (type <= 0x14) {
                PayCost37CC(Lv37CC(type, id));
                TributeMonster(EQ_CUR.player, gDuelScreen.sel);
            }
            break;
        }
        }
    }
    return EQ->need == 0;
}
/* Can the recipe entry matching this card be satisfied (enough cost in hand + field)? */
int EffectRitualSummonPrepare(struct CardRef *ref)
{
    int i;
    struct RecipeEnt *tbl;
    if (CanSpecialSummon(ref->player) == 0)
        return 0;
    i = 0;
    tbl = gRitualRecipes;
again:
    {
        struct RecipeEnt *e = (struct RecipeEnt *)(i * 4 + (u32)tbl);
        if ((*(u16 *)e << 19) == 0)
            return 0;
        if (e->num == ((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
            u32 a;
            u32 x;
            int t;
            int p;
            if ((u16)HandHasRitualMonster(ref->player, i) == 0)
                return 0;
            p = ref->player;
            a = RE_A(e);
            if (a == 0xFFFF) {
                x = 0;
            } else if (a <= 0x7CF) {
                x = ((const u16 *)0x08623DF4)[a & 0x7FF];
            } else {
                x = ((const u16 *)0x08623DF4)[(a - 0x7D0) & 0x7FF] + 1;
            }
            t = SumHandLevelsExcept(p, (u16)x);
            t += SumTributableMonsterLevels(ref->player);
            if (t >= gRitualRecipes[i].cnt)
                return 1;
            return 0;
        }
        i++;
        goto again;
    }
}

extern u8 gUnk_02017E28[];
extern const u8 gStrRitualTributePrompt[];
void CopyDuelCard(void *dst, u32 *card);
void ShowCardEffect(int player, u16 id);
void QueueSpecialSummonChoosePosition(int player, void *list, int a, int b);
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, void (*draw)(void), int (*step)(void));
int EffectRitualSummonPrepare(struct CardRef *ref);
#define REF_SKIP(r) (((u8 *)(r))[4] & 4)
int RitualTributeSelectStep(void);
static inline u8 GetRecipeCardLevelV(u16 id)
{
    int type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    int v;
    switch (type) {
    case 0x15:
    case 0x16:
    case 0x17:
        v = 0;
        break;
    case 0x18:
        v = 10;
        break;
    default:
        v = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
        break;
    }
    return v;
}
/* Ritual summon executor (hypothesis): 0x80 checks the recipe and starts tribute selection, 0x78 lets the
 * CPU pick tributes by level, 0x64 finds the ritual monster in hand, 0x63 plays it. */
/* Level for the hand-loop "ok" test. */
static inline u8 GetRecipeCardLevelT(u16 id)
{
    int type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    int v;
    switch (type) {
    case 0x15:
    case 0x16:
    case 0x17:
        v = 0;
        break;
    case 0x18:
        v = 10;
        break;
    default:
        v = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
        break;
    }
    /* FAKEMATCH: consuming v keeps the constant arms (0, 10) jumping to the shared zero test; without it jump2
     * threads them past the test. */
    asm volatile("" : : "r"(v));
    return v;
}
static inline u8 GetRecipeCardLevelW(u16 id)
{
    u16 type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    u8 v;
    switch (type) {
    case 0x15:
    case 0x16:
    case 0x17:
        v = 0;
        break;
    case 0x18:
        v = 10;
        break;
    default:
        v = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
        break;
    }
    return v;
}
int EffectRitualSummonResolve(struct CardRef *ref)
{
    u16 id = ref->id;
    u32 tab = (u32)gCardIdToNumber;
    int i;

    u8 skip;

    if ((skip = REF_SKIP(ref)) != 0)
        return 0;
    {
    switch (gChain[0x3E0]) {
    case 0x80: {
        int idx;
        int n;
        EQ->need = skip;
        n = 0x58A;
        if (CountActiveCardsOnField(0, n) > 0) {
            ShowCardEffect(ref->player, ((const u16 *)0x08623DF4)[n]);
            return 0;
        } else if (CountActiveCardsOnField(1, n) > 0) {
            ShowCardEffect(ref->player, ((const u16 *)0x08623DF4)[n]);
            return 0;
        }
        if ((u16)((int (*)(struct CardRef *, int, int))EffectRitualSummonPrepare)(ref, 0, 0) == 0)
            return 0;
        idx = FindRitualRecipe(ref->id);
        if (idx < 0 || (u16)HandHasRitualMonster(ref->player, idx) == 0)
            return 0;
        { u8 c = gRitualRecipes[idx].cnt; EQ->need = c; }
        /* FAKEMATCH: volatile re-read keeps the ROM's ldrb of need after the store */
        gChain[0x510] = *(volatile u8 *)&EQ->need;
        if (ref->player)
            return 0x78;
        TextBoxOpen(0x206, 0x612, 0xB, gStrRitualTributePrompt);
        TextBoxSetMenu(5, DrawRitualStarGauge, RitualTributeSelectStep);
        return 0x64;
    }
    case 0x64:
        for (i = 0; i < gDuelPlayers[ref->player & 1].handCount; i++) {
            u32 p = ref->player & 1;
            /* Hand word through the integer base gDuelPlayers + 0x684 (as GetRecipeHandWord), with the offset as a
             * separate statement: the pre-test base then feeds the hoisted base+0x684 add, and the table load
             * stays in the loop. */
            u32 off = i * 4 + p * 0xD64;
            if (gCardIdToNumber[(*(u32 *)(off + ((u32)gDuelPlayers + 0x684)) << 21) >> 21] == gRitualRecipes[FindRitualRecipe(ref->id)].a) {
                u32 *c = &gDuelPlayers[ref->player & 1].hand[i];
                CopyDuelCard(gUnk_02017E28, c);
                DuelCmd_Push(ref->player ? 0x80C2 : 0xC2, ((u16 *)c)[0], ((u16 *)c)[1], 0);
                return 0x63;
            }
        }
        return 0;
    case 0x63:
        QueueSpecialSummonChoosePosition(ref->player, gChain + 0x3E8, 1, 1);
        return 0x62;
    case 0x78: {
        int handLv = 0;
        int handIdx = -1;
        int fieldLv = 0;
        int fieldIdx = -1;
        if (CountMonsters(ref->player) <= 4) {
            for (i = 0; i < gDuelPlayers[ref->player].handCount; i++) {
                int ok = 1;
                int lv;
                u16 cid = (*(u32 *)((ref->player & 1) * 0xD64 + i * 4 + (u32)gDuelHands) << 20) >> 20;
                const u16 *num = (const u16 *)((cid & 0x7FF) * 2 + tab);
                if (*num == gRitualRecipes[FindRitualRecipe(id)].a && CountHandCardsByNumber(ref->player, *num) <= 1)
                    ok = 0;
                lv = GetRecipeCardLevelT(cid);
                if (lv <= 0)
                    ok = 0;
                if (ok && handLv < GetRecipeCardLevelW(cid)) {
                    handLv = GetRecipeCardLevelW(cid);
                    handIdx = i;
                }
            }
        }
        for (i = 0; i <= 4; i++) {
            if ((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) {
                u16 cid = ((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) >> 20;
                if (fieldLv < GetRecipeCardLevelV(cid)) {
                    fieldLv = GetRecipeCardLevelV(((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones)) << 20) >> 20);
                    fieldIdx = i;
                }
            }
        }
        if (handLv == 0 && fieldLv == 0)
            return 0;
        if (handLv > fieldLv) {
            DuelCmd_Push(ref->player ? 0x80C0 : 0xC0, handIdx, 0, 0);
            if ((int)EQ->need > handLv) EQ->need -= handLv; else EQ->need = 0;
        } else {
            TributeMonster(ref->player, fieldIdx);
            if ((int)EQ->need > fieldLv) EQ->need -= fieldLv; else EQ->need = 0;
        }
        return EQ->need != 0 ? 0x78 : 0x64;
    }
    default:
        gChain[0x3E0] = 0;
        return 0;
    }
    }
    return 0;
}
static inline u32 GetRecipeListWord(int player, int idx)
{
    u32 p = player & 1;
    u32 idx4 = idx * 4;
    u32 off = p * 0xD64;
    return *(u32 *)(idx4 + off + (u32)gDuelGraveyards);
}
/* All classes fit in a signed byte; narrowing keeps the common class test. */
static inline s8 GetRecipeCardClass(u16 id)
{
    int num = ((const u16 *)0x08622AB4)[0x7FF & id];
    int t;
    int v;
    switch (num) {
    case 0x776:
        v = 3;
        break;
    case 0x777:
    case 0x778:
        v = 1;
        break;
    default:
        t = ((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20);
        switch (t) {
        case 0x16:
            v = 7;
            break;
        case 0x15:
            v = 8;
            break;
        case 0x17:
            v = 9;
            break;
        default:
            v = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0xC0000) >> 18;
            break;
        }
        break;
    }
    return v;
}
/* Class zero or a failed eligibility test succeeds; otherwise return flag 14. */
int CanReviveGraveyardCard(int player, int idx)
{
    u32 off = (1 & player) * 0xD64;
    u32 base = (u32)gDuelGraveyards;
    u32 *slot = (u32 *)(off + base + idx * 4);
    u32 id = (*(u32 *)(idx * 4 + off + base) << 20) >> 20;
    int v = GetRecipeCardClass(id);

    switch (v) {
    case 0:
        goto yes;
    case 2:
    case 3:
        break;
    default:
        if (IsSpecialSummonOnly((GetRecipeListWord(player, idx) << 20) >> 20) == 0)
            goto yes;
        break;
    }
    return (*slot << 17) >> 31;
yes:
    return 1;
}
