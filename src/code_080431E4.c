#include "global.h"

int sub_08008524(int player, u16 number);
/* 4-byte entries of gUnk_0819A990: two 13-bit card numbers (a, num) and a 6-bit count (hypothesis: a fusion/ritual style recipe list) */
struct RecipeEnt { u32 a : 13; u32 num : 13; u32 cnt : 6; };
extern struct RecipeEnt gUnk_0819A990[];
extern const u16 gUnk_08622AB4[];
struct DuelPlayerC { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
extern struct DuelPlayerC gUnk_020192E4[2];
extern const u32 gUnk_08621DE0[];
struct DuelZoneD { u32 w0; u8 pad[0x94 - 4]; };
struct DuelZonesPlayerD { struct DuelZoneD zones[11]; u8 filler[0xD64 - 11 * 0x94]; };
int sub_08008A6C(int player, int zone);
struct CardListD { u32 w[80]; u8 pad[0xD64 - 0x140]; };
extern struct CardListD gUnk_02019BE8[2];
int sub_08007834(u16 id);
int sub_08043594(int player, int idx);
int sub_08043600(int player, u16 x);
int sub_080436B8(int player);
struct AE60 { u8 unk0[8]; u16 h8; u16 hA; u8 unkC[2]; u16 hE; u8 unk10[0x21 - 0x10]; u8 b21; };
extern struct AE60 gUnk_0201AE60;
extern u8 gUnk_02017A40[];
struct F1ACC { u8 lo : 6; u8 b6 : 1; u8 b7 : 1; };
struct F1ACD { u8 b0 : 1; u8 rest : 7; };
extern u8 gUnk_020192E0[];
extern u8 gUnk_0201ADAC[];
struct G5EE8 { u8 unk0; u8 b1; };
extern struct G5EE8 gUnk_02015EE8;
extern u8 gUnk_0201ADF2[];
extern const u8 gUnk_08085434[], gUnk_08085448[];
void sub_0801EC58(u32 msg, u16 zone, int a, int b);
void sub_0801A7DC(const u8 *fmt, int p, int z);
void sub_0801A7E8(void);
struct ZoneE { u32 w0; u8 pad[6 - 4]; u8 flags6; u8 pad2[0x91 - 7]; u8 b91; u8 pad3[0x94 - 0x92]; };
struct ZonesE { struct ZoneE z[11]; u8 filler[0xD64 - 11 * 0x94]; };
extern struct ZonesE gUnk_0201930C[2];
void sub_080761F0(u32 pos, int a, int b);
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 rest[0x12 - 4];
};
int sub_08047170(int player);
extern const u16 gUnk_08623DF4[];
#define RE_A(e) (((u32)*(u16 *)(e) << 19) >> 19)
#define CARD_STATS(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Is card number 0x2EF or 0x409 on the field/hand of either player? (count of sub_08008524, hypothesis) */
int sub_080431E4(void)
{
    if (sub_08008524(0, 0x2EF) == 0 && sub_08008524(1, 0x2EF) == 0
        && sub_08008524(0, 0x409) == 0 && sub_08008524(1, 0x409) == 0)
        return 0;
    return 1;
}

/* Same for card number 0x482. */
int sub_08043230(void)
{
    if (sub_08008524(0, 0x482) == 0 && sub_08008524(1, 0x482) == 0)
        return 0;
    return 1;
}
#if 0 /* NONMATCHING: structure and all callee behaviour decoded, but the
       * hoisting differs. The ROM hoists 0x7FF (r8), the zone base (r9) and
       * p*0xD64 (sl) out of both loops and spills p+1 to the stack; this build
       * hoists p*0xD64 and p+1 only (about 60 diff lines). Tricks tried, none
       * matched: a ull zone-base temp (v_zb_ull, more diff); caching the zone
       * base in a `struct ZonesE *zb` local, caching p*0xD64 in a local, and m
       * as ull. */
void sub_0804325C(void)
{
    u8 *e;
    int p;
    int z;
    int one;
    s8 m = 0x7FF;
    int r7 = sub_080431E4();
    e = gUnk_020192E0;
    ((struct F1ACC *)(e + 0x1ACC))->b7 = r7;
    one = 1;
    ((struct F1ACC *)(e + 0x1ACC))->b6 = one & sub_08043230();
    ((struct F1ACD *)(e + 0x1ACD))->b0 = 0;
    if (sub_08008524(0, 0x601) != 0 || sub_08008524(1, 0x601) != 0)
        ((struct F1ACD *)(e + 0x1ACD))->b0 = one;
    for (p = 0; p <= 1; p++) {
        for (z = 5; z <= 10; z++) {
            struct ZoneE *zn = (struct ZoneE *)((z * 0x94) + ((1 & p) * 0xD64) + (u32)gUnk_0201930C);
            u32 id = (zn->w0 << 20) >> 20;
            u32 idc;
            s16 r;
            s16 v;
            if (id == 0)
                continue;
            if (!(zn->flags6 & 2))
                continue;
            r = 0;
            idc = id;
            switch (((((const u32 *)0x08621DE0)[idc & m] & 0x1F00000) >> 20)) {
            case 0x15:
                if (0x80 & *((u8 *)gUnk_0201930C + 0x1AA0))
                    r = 1;
                break;
            case 0x16:
                if (0x40 & *((u8 *)gUnk_0201930C + 0x1AA0))
                    r = 1;
                break;
            }
            {
                int t = ((((const u32 *)0x08621DE0)[idc & m] & 0x1F00000) >> 20);
                if (t <= 0x16 && t >= 0x15)
                    v = (((const u32 *)0x08621DE0)[idc & m] & 0xE0000) >> 17;
                else
                    v = 0;
            }
            switch (v) {
            case 3:
                if (v & gUnk_0201ADAC[1])
                    r = 1;
                break;
            case 2:
                if (4 & gUnk_0201ADAC[1])
                    r = 1;
                break;
            case 4:
                switch (((((const u32 *)0x08621DE0)[idc & m] & 0x1F00000) >> 20)) {
                case 0x15:
                    if (0x10 & gUnk_0201ADAC[1])
                        r = 1;
                    break;
                case 0x16:
                    if (8 & gUnk_0201ADAC[1])
                        r = 1;
                    break;
                }
                break;
            }
            if (((const u16 *)0x08622AB4)[idc & m] == 0x603)
                r = 0;
            if (1 & gUnk_02015EE8.b1) {
                if (2 & gUnk_0201ADF2[0])
                    continue;
            }
            if (r == 0) {
                if (gUnk_0201930C[1 & p].z[z].b91 & 8) {
                    sub_0801EC58(p ? 0x80B1 : 0xB1, z, 0, 0);
                    sub_0801A7DC(gUnk_08085434, p, z);
                    sub_0801A7E8();
                }
            }
            if (r != 0) {
                if (gUnk_0201930C[1 & p].z[z].b91 & 8)
                    continue;
                r = 1;
                sub_0801A7DC(gUnk_08085448, p, z);
                sub_0801A7E8();
                if (((const u16 *)0x08622AB4)[idc & m] == 0x409) {
                    if (sub_08008524(0, 0x2EF) == 0)
                        r = sub_08008524(1, 0x2EF) != 0;
                }
                if (r != 0)
                    sub_0801EC58(p ? 0x80B1 : 0xB1, z, 1, 0);
            }
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080431E4", sub_0804325C); /* 0x0804325C size 0x2E0 */
/* Index of the entry whose num equals the card number of id, or -1. */
int sub_0804353C(u16 id)
{
    int i = 0;
    struct RecipeEnt *p = gUnk_0819A990;
    if (p->num != 0) {
        for (i = 0; gUnk_0819A990[i].num != 0; i++) {
            if (gUnk_0819A990[i].num == ((const u16 *)0x08622AB4)[0x7FF & id])
                return i;
        }
    }
    return -1;
}
/* Read the full hand word; keep the index, player offset and hand base distinct. */
static inline u32 GetRecipeHandWord(int player, int idx)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(idx * 4 + off + (u32)gUnk_020192E4 + 0x684);
}
static inline u32 GetRecipeFirstNumber(int idx)
{
    u32 base = (u32)gUnk_0819A990;
    u32 off = idx * 4;
    return ((u32)*(u16 *)(base + off) << 19) >> 19;
}
int sub_08043594(int player, int idx)
{
    u16 i;
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
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
int sub_08043600(int player, u16 x)
{
    int total = 0;
    int found = 0;
    int i;
    for (i = 0; i < gUnk_020192E4[1 & player].handCount; i++) {
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
/* Sum of the "cost" of the player's monster-zone cards that pass sub_08008A6C. */
int sub_080436B8(int player)
{
    int total = 0;
    int i;
    for (i = 0; i <= 4; i++) {
        u16 id = (*(u32 *)((player & 1) * 0xD64 + i * 0x94 + (u32)gUnk_0201930C) << 20) >> 20;
        if (sub_08008A6C(player, i) != 0) {
            int c = GetRecipeCardLevel(id);
            c += total;
            total = c;
        }
    }
    return total;
}
/* Draw a row of marker sprites (count at 0x02017A40+0x510; the first 0x02017A40+0x3E1 use tile 0x431E, the rest 0x431D). */
void sub_08043758(void)
{
    int x0 = (gUnk_0201AE60.h8 + 1) << 3;
    int y = (gUnk_0201AE60.hA - (gUnk_0201AE60.hE - gUnk_0201AE60.b21)) << 3;
    int i;
    y += 8;
    for (i = 0; i < gUnk_02017A40[0x510]; i++) {
        sub_080761F0((x0 + i * 10) | (y << 16), 0, i < gUnk_02017A40[0x3E1] ? 0x431E : 0x431D);
    }
}
/* Effect queue at 0x02017A40: 0x14-byte entries from +0x280, entry count at +0x3C0, remaining cost at +0x3E1. */
struct EffEnt37CC { u16 id; u8 player : 1; u8 rest2 : 7; u8 pad[0x14 - 3]; };
struct EffState37CC { u8 pad[0x280]; struct EffEnt37CC ent[16]; u16 count; u8 pad3C2[0x3E1 - 0x3C2]; u8 need; };
#define EQ ((struct EffState37CC *)gUnk_02017A40)
#define EQ_CUR (EQ->ent[EQ->count - 1])
struct Scr828 { u8 pad[0x828]; u32 mode; u32 sel; };
extern struct Scr828 gUnk_0201CFB0;
extern u32 gUnk_02019968[];
int sub_08008860(int player);
u32 sub_08052F38(u32 msg);
int sub_0800A2A8(int player, u16 number);
void sub_08017FF4(int player, int zone);
void sub_080193D4(int player, int a, int b, int c);
void sub_08077AEC(int se);
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
int sub_080437CC(void)
{
    u32 msg = 0xF0;
    u16 id;
    sub_0804353C(EQ_CUR.id);
    if (sub_08008860(EQ_CUR.player) <= 4)
        msg = 0xF1;
    if (sub_08052F38(msg) != 0) {
        switch (gUnk_0201CFB0.mode) {
        case 11: {
            int p;
            u32 type;
            p = EQ_CUR.player;
            id = CARD_ID(*(u32 *)(p * 0xD64 + gUnk_0201CFB0.sel * 4 + (u32)gUnk_02019968));
            if (((const u16 *)0x08622AB4)[id & 0x7FF] == gUnk_0819A990[sub_0804353C(EQ_CUR.id)].a && sub_0800A2A8(EQ_CUR.player, ((const u16 *)0x08622AB4)[id & 0x7FF]) <= 1) {
                sub_08077AEC(3);
                return 0;
            }
            type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            if (type <= 0x14) {
                PayCost37CC(Lv37CC(type, id));
                sub_080193D4(EQ_CUR.player, gUnk_0201CFB0.sel, 0, 1);
            }
            break;
        }
        case 0: {
            int p;
            u32 type;
            p = EQ_CUR.player;
            id = CARD_ID(*(u32 *)(p * 0xD64 + gUnk_0201CFB0.sel * 0x94 + (u32)gUnk_0201930C));
            type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
            if (type <= 0x14) {
                PayCost37CC(Lv37CC(type, id));
                sub_08017FF4(EQ_CUR.player, gUnk_0201CFB0.sel);
            }
            break;
        }
        }
    }
    return EQ->need == 0;
}
/* Can the recipe entry matching this card be satisfied (enough cost in hand + field)? */
int sub_08043AA8(struct CardRef *ref)
{
    int i;
    struct RecipeEnt *tbl;
    if (sub_08047170(ref->player) == 0)
        return 0;
    i = 0;
    tbl = gUnk_0819A990;
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
            if ((u16)sub_08043594(ref->player, i) == 0)
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
            t = sub_08043600(p, (u16)x);
            t += sub_080436B8(ref->player);
            if (t >= gUnk_0819A990[i].cnt)
                return 1;
            return 0;
        }
        i++;
        goto again;
    }
}

extern u8 gUnk_02017E28[];
extern const u8 gUnk_0808545C[];
void sub_08007558(void *dst, u32 *card);
void sub_080197E0(int player, u16 id);
void sub_08056094(int player, void *list, int a, int b);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, void (*draw)(void), int (*step)(void));
int sub_08043AA8(struct CardRef *ref);
#define REF_SKIP(r) (((u8 *)(r))[4] & 4)
int sub_080437CC(void);
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
#if 0 /* NONMATCHING (score 4): NONMATCHING: 4 lines: only the case 0x64 hoist of hand base differs (built 'adds
       * r0,r1,#0; ldr r1,=0x684; adds r0,r0,r1', ROM 'ldr r0,=0x684; adds r0,r0,r1': ROM's hoisted add has the 0x684
       * pseudo as first operand). Keys: function-level u32 tab = gUnk_08622AB4 local (rematerialized as a reload in the
       * hand loop, fixes reload rotation) with index-first (cid & 0x7FF) * 2 + tab; gUnk_02019968 symbol directly in the
       * hand-word address (local-alloc tie p vs i*4); cast table ((const u16 *)0x08623DF4)[n] in case 0x80; u8 temp for
       * need (ref r9/ok r8 priority); u16-type level helper W for levels 2/3 (cid r7/num r6 priority); volatile re-read
       * of need (FAKEMATCH). Failed for the hoist: GetRecipeHandWord/sum-of-address forms (80-98), hoff local, int p,
       * 1&player. */
/* Ritual summon executor (hypothesis): 0x80 checks the recipe and starts tribute selection, 0x78 lets the
 * CPU pick tributes by level, 0x64 finds the ritual monster in hand, 0x63 plays it. */
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
int sub_08043B98(struct CardRef *ref)
{
    u16 id = ref->id;
    u32 tab = (u32)gUnk_08622AB4;
    int i;

    u8 skip;

    if ((skip = REF_SKIP(ref)) != 0)
        return 0;
    {
    switch (gUnk_02017A40[0x3E0]) {
    case 0x80: {
        int idx;
        int n;
        EQ->need = skip;
        n = 0x58A;
        if (sub_08008524(0, n) > 0) {
            sub_080197E0(ref->player, ((const u16 *)0x08623DF4)[n]);
            return 0;
        } else if (sub_08008524(1, n) > 0) {
            sub_080197E0(ref->player, ((const u16 *)0x08623DF4)[n]);
            return 0;
        }
        if ((u16)((int (*)(struct CardRef *, int, int))sub_08043AA8)(ref, 0, 0) == 0)
            return 0;
        idx = sub_0804353C(ref->id);
        if (idx < 0 || (u16)sub_08043594(ref->player, idx) == 0)
            return 0;
        { u8 c = gUnk_0819A990[idx].cnt; EQ->need = c; }
        gUnk_02017A40[0x510] = *(volatile u8 *)&EQ->need;
        if (ref->player)
            return 0x78;
        sub_080602A4(0x206, 0x612, 0xB, gUnk_0808545C);
        sub_08060308(5, sub_08043758, sub_080437CC);
        return 0x64;
    }
    case 0x64:
        for (i = 0; i < gUnk_020192E4[ref->player & 1].handCount; i++) {
            u32 p = ref->player & 1;
            if (gUnk_08622AB4[(gUnk_020192E4[p].hand[i] << 21) >> 21] == gUnk_0819A990[sub_0804353C(ref->id)].a) {
                u32 *c = &gUnk_020192E4[ref->player & 1].hand[i];
                sub_08007558(gUnk_02017E28, c);
                sub_0801EC58(ref->player ? 0x80C2 : 0xC2, ((u16 *)c)[0], ((u16 *)c)[1], 0);
                return 0x63;
            }
        }
        return 0;
    case 0x63:
        sub_08056094(ref->player, gUnk_02017A40 + 0x3E8, 1, 1);
        return 0x62;
    case 0x78: {
        int handLv = 0;
        int handIdx = -1;
        int fieldLv = 0;
        int fieldIdx = -1;
        if (sub_08008860(ref->player) <= 4) {
            for (i = 0; i < gUnk_020192E4[ref->player].handCount; i++) {
                int ok = 1;
                int lv;
                u16 cid = (*(u32 *)((ref->player & 1) * 0xD64 + i * 4 + (u32)gUnk_02019968) << 20) >> 20;
                const u16 *num = (const u16 *)((cid & 0x7FF) * 2 + tab);
                if (*num == gUnk_0819A990[sub_0804353C(id)].a && sub_0800A2A8(ref->player, *num) <= 1)
                    ok = 0;
                lv = GetRecipeCardLevelV(cid);
                if (lv <= 0)
                    ok = 0;
                if (ok && handLv < GetRecipeCardLevelW(cid)) {
                    handLv = GetRecipeCardLevelW(cid);
                    handIdx = i;
                }
            }
        }
        for (i = 0; i <= 4; i++) {
            if ((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gUnk_0201930C)) << 20) {
                u16 cid = ((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gUnk_0201930C)) << 20) >> 20;
                if (fieldLv < GetRecipeCardLevelV(cid)) {
                    fieldLv = GetRecipeCardLevelV(((*(u32 *)((ref->player & 1) * 0xD64 + i * 0x94 + (u32)gUnk_0201930C)) << 20) >> 20);
                    fieldIdx = i;
                }
            }
        }
        if (handLv == 0 && fieldLv == 0)
            return 0;
        if (handLv > fieldLv) {
            sub_0801EC58(ref->player ? 0x80C0 : 0xC0, handIdx, 0, 0);
            if ((int)EQ->need > handLv) EQ->need -= handLv; else EQ->need = 0;
        } else {
            sub_08017FF4(ref->player, fieldIdx);
            if ((int)EQ->need > fieldLv) EQ->need -= fieldLv; else EQ->need = 0;
        }
        return EQ->need != 0 ? 0x78 : 0x64;
    }
    default:
        gUnk_02017A40[0x3E0] = 0;
        return 0;
    }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080431E4", sub_08043B98); /* 0x08043B98 size 0x594 */
static inline u32 GetRecipeListWord(int player, int idx)
{
    u32 p = player & 1;
    u32 idx4 = idx * 4;
    u32 off = p * 0xD64;
    return *(u32 *)(idx4 + off + (u32)gUnk_02019BE8);
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
int sub_0804412C(int player, int idx)
{
    u32 off = (1 & player) * 0xD64;
    u32 base = (u32)gUnk_02019BE8;
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
        if (sub_08007834((GetRecipeListWord(player, idx) << 20) >> 20) == 0)
            goto yes;
        break;
    }
    return (*slot << 17) >> 31;
yes:
    return 1;
}
