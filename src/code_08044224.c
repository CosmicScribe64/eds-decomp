#include "global.h"

/* Local raw-word view: the search copies complete card words, including flags. */
struct TargetPlayer {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 otherCount;
    u8 unk7[0x684 - 7];
    u32 hand[80];
    u32 deck[80];
    u32 graveyard[80];
    u32 fusionDeck[80];
    u32 otherCards[80];
    u16 otherKinds[80];
};

/* The list viewer's +0x20C region is interpreted as 128 halfword area tags here. */
struct TargetList {
    u8 header[0xC];
    u32 cards[128];
    u16 areas[128];
    u16 count;
};

struct TargetPlayerList {
    u32 cards[80];
    u8 rest[0xD64 - 80 * 4];
};

struct TargetCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:4;
    u32 flag17:1;
    u32 unk18:2;
    s32 flag20:1;
    u32 flag21:1;
    u32 flag22:1;
    u32 unk23:9;
};
struct TargetPlayerS {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 otherCount;
    u8 unk7[0x684 - 7];
    struct TargetCard hand[80];
    struct TargetCard deck[80];
    struct TargetCard graveyard[80];
    struct TargetCard fusionDeck[80];
    struct TargetCard otherCards[80];
    u16 otherKinds[80];
};
/* One player record seen as raw card words (w) or as bitfield cards (s). */
union TargetPlayerU {
    struct TargetPlayer w;
    struct TargetPlayerS s;
};
extern union TargetPlayerU gUnk_020192E4[2];
extern struct TargetPlayerList gUnk_02019AA8[2];
extern struct TargetPlayerList gUnk_02019BE8[2];
extern struct TargetList gUnk_0201D810;
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];

void sub_08007558(u32 *dst, u32 *src);
u32 sub_0800756C(u16 number);
u32 sub_08007730(u16 id);
u32 sub_08007834(u16 id);
u16 sub_08008668(int player);
u32 sub_0800966C(u16 id);
int sub_0803CDEC(int a, int b);int sub_0803D0E8(int player, u16 id, u16 *materials);
int sub_0804412C(int player, int index);
u16 sub_08044224(int player, u16 number, int arg);

typedef char target_player_size[sizeof(struct TargetPlayer) == 0xD64 ? 1 : -1];
typedef char target_list_count[(u32)&((struct TargetList *)0)->count == 0x30C ? 1 : -1];
typedef char target_list_areas[(u32)&((struct TargetList *)0)->areas == 0x20C ? 1 : -1];

#define TARGET_ID(word) (((u32)(word) << 20) >> 20)
#define TARGET_STATS(id) (((volatile const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline int TargetType(u16 id)
{
    return (TARGET_STATS(id) & 0x1F00000) >> 20;
}
#define TARGET_TYPE(id) ((u32)TargetType(id))
#define TARGET_TYPE32(id) ((TARGET_STATS(id) & 0x1F00000) >> 20)
static inline u16 TargetNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define TARGET_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define PP ((struct TargetPlayer *)(b + off))
#define PPL (&gUnk_020192E4[player & 1].w)
#define CASE_LOCALS struct TargetPlayer *p; u32 *word; u32 id; int cardNo; u32 type; u32 allowed; u32 attribute; int pidx; u32 off; u8 *b; u32 *g;
#define ADD_TARGET(word, area) do { \
    gUnk_0201D810.cards[gUnk_0201D810.count] = (word); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
} while (0)

static inline u16 TargetAttack(u32 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS(id) << 14) >> 23) * 10;
}

#define TARGET_STATS_NV(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline u16 TargetAttackNV(u32 id)
{
    u32 type = ((TARGET_STATS_NV(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS_NV(id) << 14) >> 23) * 10;
}

static inline u32 TargetAttackT(u32 id, u32 type)
{
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS_NV(id) << 14) >> 23) * 10;
}

static inline u16 TargetDefense(u16 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return (TARGET_STATS(id) & 0x1FF) * 10;
}

static inline s8 TargetLevel(u16 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    }
    return (TARGET_STATS(id) & 0x1E000000) >> 25;
}

static inline s8 TargetKind(u32 id)
{
    switch (TARGET_NUMBER(id)) {
    case 0x776: return 3;
    case 0x777:
    case 0x778: return 1;
    }
    switch ((s32)((TARGET_STATS(id) & 0x1F00000) >> 20)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    }
    return (TARGET_STATS(id) & 0xC0000) >> 18;
}

static inline u32 ReadTargetDeckWord(int player, int index)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(index * 4 + off + (u32)gUnk_02019AA8);
}
static inline u32 CopyTargetDeckWord(int player, int index)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(index * 4 + off + (u32)gUnk_020192E4 + 0x7C4);
}
static inline u32 TargetSpellKind(u16 id)
{
    u32 stats = TARGET_STATS_NV(id);
    switch ((s32)((stats & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (stats & 0xE0000) >> 17;
    }
    return 0;
}
static inline u32 TargetSpellKind32(u32 id)
{
    u32 stats = TARGET_STATS_NV(id);
    switch ((s32)((stats & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (stats & 0xE0000) >> 17;
    }
    return 0;
}
static inline u16 TargetAttackNV16(u16 id)
{
    u32 type = ((TARGET_STATS_NV(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS_NV(id) << 14) >> 23) * 10;
}
static inline int TargetTypeNV(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}
#define TARGET_TYPE_NV(id) ((u32)TargetTypeNV(id))
/* Populate the list-view overlay with targets for a card/effect number. */
/* TargetKind with a u16 id (case 0x455): all three `id & 0x7FF` ANDs are then HImode, so their constants match
   and loop.c hoists 0x7FF in pass 1 (ROM preheader order); 0x454 needs the u32 TargetKind. */
static inline s8 TargetKind16(u16 id)
{
    switch (TARGET_NUMBER(id)) {
    case 0x776: return 3;
    case 0x777:
    case 0x778: return 1;
    }
    switch ((s32)((TARGET_STATS(id) & 0x1F00000) >> 20)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    }
    return (TARGET_STATS(id) & 0xC0000) >> 18;
}
#define CARDP(p) ((struct TargetCard *)(p))
#define PS ((struct TargetPlayerS *)(b + off))
struct TargetPlayerListS {
    struct TargetCard cards[80];
    u8 rest[0xD64 - 80 * 4];
};
/* Card type as a u8: the narrowing adds pass-1 loop insns (removed later by combine), which keeps loop.c
   from hoisting &count in pass 1 (0x447/0x45C: areas pointer before count pointer, as in the ROM). */
static inline u8 TargetType8(u16 id)
{
    return (TARGET_STATS(id) & 0x1F00000) >> 20;
}
/* u16 id: the parameter's zero-extension adds pass-1 loop insns, so 0x2F's &count is hoisted in loop
   pass 2 (after the 1500 constant), as in the ROM. */
static inline u16 TargetAttack16(u16 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS(id) << 14) >> 23) * 10;
}
#define GRAVE2(pl) ((struct TargetPlayerListS *)gUnk_02019BE8)[(pl) & 1].cards
#define PLS(pl) ((struct TargetPlayerS *)gUnk_020192E4)[(pl) & 1]
#define ADD_TARGETB(word, area) { \
    gUnk_0201D810.cards[gUnk_0201D810.count] = (word); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
}
#define ADD_TARGET_S(card, area) do { \
    ((struct TargetCard *)gUnk_0201D810.cards)[gUnk_0201D810.count] = (card); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
} while (0)
struct TargetCards {
    u32 cards[128];
    u16 areas[128];
    u16 count;
};
extern struct TargetCards gUnk_0201D81C;
u16 sub_08044224(int player, u16 number, int arg)
{
    u16 materials[4];
    int forceFilter = 0;
    int skipFilter = 0;
    int flag;
    int i;
    int j;
    int pidx;
    struct TargetPlayer *p;
    u32 *word;
    u32 id;
    int cardNo;
    u32 type;
    u32 allowed;
    u32 attribute;
    u16 fieldCount;

    gUnk_0201D810.count = 0;
    switch (number) {
    case 0x2F:
    {
        CASE_LOCALS
        i = 0;
        asm volatile("" ::: "r4", "r5", "r6", "r7");
        for (; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            struct TargetCard c = *(struct TargetCard *)((u8 *)gUnk_02019AA8 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(c.id) <= 20 && TargetAttack16(c.id) <= 1500)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        break;
    }
    case 0x65:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
                if (TARGET_TYPE(TARGET_ID(*word)) == 21)
                    ADD_TARGETB(*word, 4);
        }
        break;
    }
    case 0x13D:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.handCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x684 + (i * 4 + (player & 1) * 0xD64));
                switch (TARGET_NUMBER(TARGET_ID(*word))) {
                case 0x3D:
                case 0x3E:
                case 0x4E1:
                    ADD_TARGETB(*word, 1);
                    break;
                }
        }
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_02019AA8 + (i * 4 + (player & 1) * 0xD64));
                switch (TARGET_NUMBER(TARGET_ID(*word))) {
                case 0x3D:
                case 0x3E:
                case 0x4E1:
                    ADD_TARGETB(*word, 2);
                    break;
                }
        }
        break;
    }
    case 0xF:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.handCount; i++) {
            u32 *wp = (u32 *)((u8 *)gUnk_020192E4 + 0x684 + (i * 4 + (player & 1) * 0xD64));
            if ((cardNo = TARGET_NUMBER(TARGET_ID(*wp))) == 0x4B2)
                ADD_TARGETB(*wp, 1);
        }
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            u32 *wp = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if ((cardNo = TARGET_NUMBER(TARGET_ID(*wp))) == 0x4B2)
                ADD_TARGETB(*wp, 2);
        }
        break;
    }    case 0x191:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_02019BE8 + (i * 4 + (player & 1) * 0xD64));
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x3EB || cardNo == 0x40A)
                ADD_TARGETB(*word, 4);
        }
        break;
    }
    case 0x1A3:
    case 0x1F9:
    case 0x5E8:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.fusionCount; i++) {
            gUnk_0201D810.cards[gUnk_0201D810.count] = gUnk_020192E4[player & 1].w.fusionDeck[i];
            gUnk_0201D810.count++;
        }
        break;
    }    case 0x1A8:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x1A8)
                ADD_TARGETB(*word, 2);
        }
        break;
    }
    case 0x1AB:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGETB(*word, 4);
        }
        break;
    }
    case 0x23D:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            struct TargetCard c = *(struct TargetCard *)((u8 *)gUnk_02019AA8 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(c.id) <= 20 && TargetDefense(c.id) <= 1500)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        break;
    }
    case 0x3B1:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) > 20)
                ADD_TARGETB(*word, 2);
        
        }
        break;
    }
    case 0x3C6:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.handCount; i++) {
            id = TARGET_ID(*(u32 *)((u8 *)gUnk_020192E4 + 0x684 + (i * 4 + (player & 1) * 0xD64)));
            if (id != 0) {
                type = TARGET_TYPE(id);
                if (type == 1)
                    ADD_TARGETB(*(u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64)), type);
            }
        }
        break;
    }
    case 0x3EB:
    case 0x40A:
    case 0x60B:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.fusionCount; i++) {
            word = (u32 *)((u8 *)gUnk_020192E4 + 0xA44 + (i * 4 + (player & 1) * 0xD64));
            if (sub_0803D0E8(player, TARGET_ID(*word), materials) != 0)
                ADD_TARGETB(*word, 8);
        }
        break;
    }
    case 0x3F0:
    {
        CASE_LOCALS
        for (j = 0; j <= 1; j++) {
            for (i = 0; i < gUnk_020192E4[j & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_02019BE8 + (i * 4 + (j & 1) * 0xD64));
                if (TARGET_TYPE(TARGET_ID(*word)) <= 20
                    && (u16)sub_0804412C(j, i) != 0)
                    ADD_TARGETB(*word, 4);
            }
        }
        break;
    }
    case 0x3F3:
    {
        CASE_LOCALS
        skipFilter = 1;
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + ((1 - player) & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x3FA:
    {
        CASE_LOCALS
        skipFilter = 1;
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].w.deckCount && i <= 4; i++) {
            ADD_TARGETB(gUnk_020192E4[(1 - player) & 1].w.deck[i], 2);
            /* FAKEMATCH: the empty asm emits nothing; it only makes loop.c's second pass count 27 insns
               (26 without it). threshold(26) * savings(1) * life(1) < insn_count then keeps the cards base
               (list + 12) inside the loop, while the body/latch use the same (1 - player) & 1 so the latch
               pointer is hoisted in pass 1, before the deck-offset giv init (ROM order). */
            asm("");
        }
        break;
    }
    case 0x400:
    {
        CASE_LOCALS
        skipFilter = 1;
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            ADD_TARGETB(gUnk_020192E4[player & 1].w.graveyard[i], 4);
            /* FAKEMATCH: three empty asms pad loop.c's second pass from 24 to 27 insns so the cards base
               (list + 12) stays in the loop; same index as the test, so the latch pointer copy is hoisted in
               pass 1 before the giv init, as in the ROM (see case 0x3FA). Two are not enough. */
            asm("");
            asm("");
            asm("");
        }
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].w.graveCount; i++) {
            ADD_TARGETB(gUnk_020192E4[(1 - player) & 1].w.graveyard[i], 4);
            /* FAKEMATCH: loop padding, as above. */
            asm("");
            asm("");
            asm("");
        }
        break;
    }
    case 0x410:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x3EB || cardNo == 0x40A)
                ADD_TARGETB(*word, 2);
        
        }
        break;
    }
    case 0x41E:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            /* The test read folds to the constant gUnk_020192E4 + 0x7C4; loop.c matches the copy's
               `base + 0x7C4` with it, so that add stays in the loop next to the hoisted base (sl). */
            u16 idv = TARGET_ID(*(u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE_NV(idv) <= 20 && TargetAttackNV16(idv) <= 1500
                && sub_08007834(idv) == 0)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        forceFilter = 1;
        break;
    }
    case 0x439:
    {
        CASE_LOCALS
        skipFilter = 1;
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
            /* FAKEMATCH: three dead stores (nothing reads j/flag/fieldCount before the tail rewrites
               them, so flow deletes them after loop.c). They keep the loop at 27 insns in the second
               loop pass, so loop.c leaves the `list + 12` cards address in the loop
               (threshold 26 * savings 1 * life 1 < 27), as the ROM does, while the plain
               `player & 1` latch test still lets the first pass hoist the latch pointer copy. */
            j = i;
            flag = i;
            fieldCount = i;
        }
        break;
    }
    case 0x443:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + ((1 - player) & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x454:
    case 0x456:
    case 0x45D:
    case 0x45F:
    case 0x460:
    case 0x463:
    {
        CASE_LOCALS
        u8 ok;
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            id = TARGET_ID(*(u32 *)((u8 *)gUnk_02019AA8 + i * 4 + (player & 1) * 0xD64));
            type = (TARGET_STATS_NV(id) & 0x1F00000) >> 20;
            if (type > 20 || TargetAttackT(id, type) > 1500)
                continue;
            ok = 0;
            switch (number) {
            case 0x454:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 5;
                break;
            case 0x456:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 4;
                break;
            case 0x45D:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 1;
                break;
            case 0x45F:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 3;
                break;
            case 0x460:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 6;
                break;
            case 0x463:
                attribute = TARGET_STATS(id) >> 29;
                ok = attribute == 2;
                break;
            }
            if (TargetKind(id) == 3 || TargetKind(id) == 2)
                ok = 0;
            switch (TARGET_NUMBER(id)) {
            case 0x37:
            case 0x38:
            case 0x3E:
            case 0x42:
            case 0x170:
            case 0x175:
            case 0x187:
            case 0x2E5:
            case 0x34D:
                ok = 0;
                break;
            }
            if (ok != 0)
                ADD_TARGET(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        break;
    }
    case 0x455:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            u16 idv = TARGET_ID(*(u32 *)((u8 *)gUnk_02019AA8 + i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(idv) <= 20 && TargetKind16(idv) == 3)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        break;
    }
    case 0x45A:
    case 0x45B:
    case 0x51B:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_NUMBER(TARGET_ID(*word)) == number)
                ADD_TARGETB(*word, 2);
        
        }
        break;
    }
    case 0x462:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            struct TargetCard c = *(struct TargetCard *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(c.id) == 22 && TargetSpellKind(c.id) == 6)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.deck[i], 2);
        }
        break;
    }
    case 0x447:
    case 0x487:
    case 0x488:
    case 0x5F0:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_02019BE8 + (i * 4 + (player & 1) * 0xD64));
            if (TargetType8(TARGET_ID(*word)) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGET(*word, 4);
        }
        break;
    }
    case 0x45C:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_02019BE8 + (i * 4 + (player & 1) * 0xD64));
            if (TargetType8(TARGET_ID(*word)) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGETB(*word, 4);
        }
        if (player == arg) {
            flag = 1;
            for (i = 0; i < gUnk_0201D810.count && flag; i++) {
                u32 *t = gUnk_0201D810.cards;
                if (TARGET_NUMBER(TARGET_ID(t[i])) == 0x45C) {
                    flag = 0;
                    gUnk_0201D810.count--;
                    for (j = i; j < gUnk_0201D810.count; j++) {
                        sub_08007558(&gUnk_0201D810.cards[j], &gUnk_0201D810.cards[j + 1]);
                        gUnk_0201D810.areas[j] = gUnk_0201D810.areas[j + 1];
                    }
                }
            }
        }
        break;
    }
    case 0x47B:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            u16 idv = TARGET_ID(*(u32 *)((u8 *)gUnk_020192E4 + 0x904 + i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE_NV(idv) <= 20 && TargetAttackNV16(idv) <= 1500
                && sub_08007730(idv) == 0)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.graveyard[i], 4);
        }
        break;
    }
    case 0x4B2:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGETB(*word, 2);
        
        }
        break;
    }
    case 0x4BC:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            word = (u32 *)((u8 *)&gUnk_020192E4[0].w.deck[i] + (player & 1) * 0xD64);
            switch (TARGET_NUMBER(TARGET_ID(*word))) {
            case 0x22:
            case 0x4BA:
            case 0x7F2:
                ADD_TARGETB(*word, 2);
                break;
            }
        }
        break;
    }
    case 0x4D8:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x2EA)
                ADD_TARGETB(*word, 2);
        
        }
        break;
    }
    case 0x4D9:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x2EA || cardNo == 0x4D8)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x526:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.deckCount; i++) {
            struct TargetCard c = *(struct TargetCard *)((u8 *)gUnk_020192E4 + 0x7C4 + ((player & 1) * 0xD64 + i * 4));
            if (TARGET_TYPE(c.id) == 10 && TargetLevel(c.id) == arg
                && sub_08007834(TARGET_ID(*(u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + ((player & 1) * 0xD64 + i * 4)))) == 0)
                ADD_TARGETB(*(u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (i * 4 + (player & 1) * 0xD64)), 2);
        }
        break;
    }
    case 0x58D:
    {
        CASE_LOCALS
        i = 0;
        if (i < gUnk_020192E4[player & 1].w.graveCount) {
            off = (player & 1) * 0xD64;
            b = (u8 *)gUnk_020192E4;
            g = (u32 *)(b + 0x904);
            do {
                /* FAKEMATCH: off + g must stay in the loop ahead of the i << 2 (ROM 0x08045F6E); as a pseudo,
                   loop pass 2 hoists it (lifetime 2, 49 insns) and the read becomes a giv. Pinning it keeps it. */
                register u32 t asm("r0") = off + (u32)g;
                struct TargetCard w = ((struct TargetPlayerListS *)t)->cards[i];
                if (TARGET_TYPE(w.id) <= 20 && w.flag21)
                    ADD_TARGETB(*(u32 *)((u8 *)g + (off + i * 4)), 4);
                i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x59F:
    {
        CASE_LOCALS
        i = 0;
        if (i < gUnk_020192E4[player & 1].w.graveCount) {
            off = (player & 1) * 0xD64;
            b = (u8 *)gUnk_020192E4;
            g = (u32 *)(b + 0x904);
            do {
                /* FAKEMATCH: off + g must stay in the loop ahead of the i << 2 (ROM 0x08046012); as a pseudo,
                   loop pass 2 hoists it (lifetime 2, 49 insns) and the read becomes a giv. Pinning it keeps it. */
                register u32 t asm("r0") = off + (u32)g;
                struct TargetCard w = ((struct TargetPlayerListS *)t)->cards[i];
                if (TARGET_TYPE(w.id) == 22 && w.flag22)
                    ADD_TARGETB(*(u32 *)((u8 *)g + (off + i * 4)), 4);
                i++;
            } while (i < PPL->graveCount);
        }
        break;
    }    case 0x5E7:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + ((1 - player) & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x5E9:
    {
        CASE_LOCALS
        skipFilter = 1;
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x5EA:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
                word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) == 3)
                ADD_TARGETB(*word, 4);
        
        }
        break;
    }
    case 0x5EB:
    case 0x5EC:
    case 0x5ED:
    case 0x5EE:
    case 0x5EF:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            /* ROM order: attribute = 0 after the id read. The u16 id and the u8 type/attribute values add
               flow-time extension insns (combined away later) inside attribute's live range, which keeps
               its global-alloc priority (refs 14 / live 70, doubled by the REG_EQUIV of its first set)
               below number's (81 / 681), so number keeps r4 and attribute gets r5 as in the ROM. */
            u32 stats;
            u16 id16;
            u8 attr8;
            id16 = TARGET_ID(*(u32 *)((u8 *)gUnk_02019BE8 + i * 4 + (player & 1) * 0xD64));
            attr8 = 0;
            switch (number) {
            case 0x5EB:
                attr8 = 1;
                break;
            case 0x5EC:
                attr8 = 4;
                break;
            case 0x5ED:
                attr8 = 3;
                break;
            case 0x5EE:
                attr8 = 5;
                break;
            case 0x5EF:
                attr8 = 6;
                break;
            }
            stats = TARGET_STATS_NV(id16);
            if ((u8)((stats & 0x1F00000) >> 20) <= 20 && (u8)(stats >> 29) == attr8)
                ADD_TARGET(gUnk_020192E4[player & 1].w.graveyard[i], 4);
        }
        break;
    }
    case 0x5F4:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            if (gUnk_020192E4[player & 1].s.graveyard[i].flag20)
                ADD_TARGETB(gUnk_020192E4[player & 1].w.graveyard[i], 4);
        }
        break;
    }
    case 0x5FC:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGETB(*word, 4);
        }
        break;
    }
    case 0x60A:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.graveCount; i++) {
            word = (u32 *)((u8 *)gUnk_020192E4 + 0x904 + (i * 4 + (player & 1) * 0xD64));
            id = TARGET_ID(*word);
            if (sub_0803CDEC((u16)arg, id) != 0
                && gUnk_020192E4[player & 1].s.graveyard[i].flag20)
                ADD_TARGETB(*word, 4);
        }
        break;
    }
    case 0x60D:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].w.otherCount; i++) {
            u32 w = *(u32 *)((u8 *)gUnk_020192E4 + 0xB84 + (i * 4 + (player & 1) * 0xD64));
            if (TARGET_TYPE(TARGET_ID(w)) <= 20 && (u8)gUnk_020192E4[player & 1].w.otherKinds[i] != 2)
                ADD_TARGETB(w, 0x10);
        }
        break;
    }
    }
    if (gUnk_0201D810.count == 0)
        return 0;

    fieldCount = sub_08008668(player);
    if ((forceFilter || fieldCount == 0) && !skipFilter) {
        for (i = 0; i < gUnk_0201D810.count;) {
            u32 *t = gUnk_0201D810.cards;
            if (sub_0800756C(TARGET_NUMBER(TARGET_ID(t[i]))) != 0) {
                /* Retain the ROM's final one-past-count copy and untouched area tags. */
                for (j = i; j < gUnk_0201D810.count; j++)
                    gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
                gUnk_0201D810.count--;
            } else {
                i++;
            }
        }
    }
    for (i = 0; i < gUnk_0201D810.count;) {
        u32 *t = gUnk_0201D810.cards;
        if ((s32)(t[i] << 14) < 0) {
            for (j = i; j < gUnk_0201D810.count; j++)
                gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
            gUnk_0201D810.count--;
        } else {
            i++;
        }
    }
    for (i = 0; i < gUnk_0201D810.count;) {
        u32 *t = gUnk_0201D810.cards;
        if (sub_0800966C(TARGET_ID(t[i])) != 0) {
            for (j = i; j < gUnk_0201D810.count; j++)
                gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
            gUnk_0201D810.count--;
        } else {
            i++;
        }
    }
    return gUnk_0201D810.count;
}

