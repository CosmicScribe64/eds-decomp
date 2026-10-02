#include "global.h"
#include "gba.h"

struct PackSlot {
    const u16 *cards;
    s32 count;
};

struct PackSlots {
    struct PackSlot slot[8]; /* 7 = commons ... 0 = rarest */
};

struct SaveMirror {
    u8 pad8[8];
    u32 trunk[0x850];
    u8 pad[0x2154 - 8 - 0x2140];
    u16 lastPack;   /* +0x2154 */
    u16 pityCount;  /* +0x2156 */
};
extern struct SaveMirror gUnk_02011C20;
extern const s32 gUnk_081A570C[8];

extern int sub_08076F9C(void);

struct Main {
    u8 pad0[4];
    u16 keysHeld;   /* +0x04 */
    u16 keysNew;    /* +0x06 */
    u8 pad8[0x40E - 8];
    u16 vblankFlags;    /* +0x40E */
    u8 pad410[0x414 - 0x410];
    void (*vblankCallback)(void); /* +0x414 */
    u8 pad418[4];
    u16 bgMap[8][0x400];    /* +0x41C */
    u8 pad441C[4];
    u16 bgVofs[4];  /* +0x4420 */
    u8 pad4428[0x4859 - 0x4428];
    u8 step;        /* +0x4859 */
    u8 sub1;        /* +0x485A */
    u8 pad485B[0x4876 - 0x485B];
    u16 unk4876;    /* +0x4876 */
};
extern struct Main gUnk_03000040;
typedef char main_bg_vofs_offset_check[(u32)&((struct Main *)0)->bgVofs == 0x4420 ? 1 : -1];
typedef char main_step_offset_check[(u32)&((struct Main *)0)->step == 0x4859 ? 1 : -1];
typedef char main_substep_offset_check[(u32)&((struct Main *)0)->sub1 == 0x485A ? 1 : -1];
typedef char main_pack_arg_offset_check[(u32)&((struct Main *)0)->unk4876 == 0x4876 ? 1 : -1];

/* Scroll position in the pack list: a = row (0-7), b = sub-step (0-7), c = animation (0 none, 1 up, 2 down). */
struct PackScroll {
    u8 a : 3;
    u8 b : 3;
    u8 c : 2;
    u8 pad[7];
};
extern const u16 gUnk_0808658C[8];

struct PackBuf {
    u8 pad0[0x102];
    u16 ids[5];       /* +0x102 card ids of the pack being opened */
    u8 state[5];      /* +0x10C per-card reveal state (0x17 = revealed) */
    u8 pad111[0x114 - 0x111];
    struct PackScroll scroll;   /* +0x114 */
};
extern struct PackBuf gUnk_02015160;

/* Card trunk entry (u32 per key, at save+8); declared 8 bytes wide so that bitfields are read piecewise. */
struct TrunkEntry {
    u8 pad0[8];
    u16 count : 10;
    u16 pad10 : 6;
    u8 pad[4];
};
struct TrunkFlags {
    u8 pad0[9];
    u8 f0 : 2;
    u8 f2 : 2;
    u8 f4 : 2;
    u8 f6 : 2;
    u8 pad[3];
};
extern const u16 gUnk_08623DF4[];
static inline int TrunkCount(struct TrunkEntry *e) { return e->count; }
static inline int TrunkF2(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f2; }
static inline int TrunkF4(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f4; }
static inline int TrunkF6(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f6; }

int sub_080629F0(struct PackSlots *p)
{
    int i;
    struct PackSlot *s;
    i = 7;
    s = &p->slot[7];
    for (; i > 0; s--, i--) {
        if (s->count > 0)
            return i;
    }
    return 0;
}

int sub_08062A0C(struct PackSlots *p, u16 id)
{
    int r = sub_08076F9C() % 180;
    int i;
    struct PackSlot *s;
    const s32 *th;
    u16 *pity;
    if (gUnk_02011C20.lastPack == id)
        r = sub_08076F9C() % 270;
    if (gUnk_02011C20.pityCount > 5 && (gUnk_02011C20.lastPack != id || gUnk_02011C20.pityCount > 10)) {
        r = sub_08076F9C() % 12;
        gUnk_02011C20.pityCount = 0;
    }
    gUnk_02011C20.lastPack = id;
    i = 0;
    pity = &gUnk_02011C20.pityCount;
    for (s = p->slot, th = gUnk_081A570C; i <= 6; s++, th++, i++) {
        if (r < *th && s->count > 0) {
            if (i <= 4 && i < sub_080629F0(p))
                *pity = 0;
            return i;
        }
    }
    gUnk_02011C20.pityCount++;
    return sub_080629F0(p);
}

u16 sub_08062AD4(struct PackSlots *p, int slot)
{
    struct PackSlot *s = (struct PackSlot *)(slot * 8 + (u32)p);
    s32 n = s->count;
    return s->cards[sub_08076F9C() % n];
}

extern const u16 gUnk_08622AB4[];
struct PackTblEntry {
    struct PackSlots *p;
    u16 id;
    u16 pad;
};
extern const struct PackTblEntry gUnk_081A562C[28];
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* One random card for slot i of a special pack: not a number in 1920..1999 (the 0x780 range), of the
 * wanted type when TYPECHECK, and not a duplicate of the previous picks. */
#define PICK_RANDOM(TYPECHECK)                                               \
    for (i = 0; i <= 4; i++) {                                               \
        retry = 1;                                                           \
        do {                                                                 \
            valid = 0;                                                       \
            id = sub_08076F9C() % 0x335;                                     \
            if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F TYPECHECK) {           \
                valid = 1;                                                   \
                if (i > 0) {                                                 \
                    for (j = 0; j < i; j++) {                                \
                        if (CARD_NUMBER(out[j]) == CARD_NUMBER(id))          \
                            valid = 0;                                       \
                    }                                                        \
                }                                                            \
            }                                                                \
            if (valid) {                                                     \
                retry = 0;                                                   \
                out[i] = CARD_NUMBER(id);                                    \
            }                                                                \
        } while (retry);                                                     \
    }

#if 0 /* NONMATCHING: control flow, switch dispatch and all stores match, but register allocation
       * differs (ROM: out=sl, pack/retry=r9, valid=r8, i+1 spilled to [sp+0x14], out+i in
       * [sp+8/0xC/0x10]) */
/* Fills out[0..4] with the five cards of a booster pack; returns the rolled rarity slot, or -1. */
int sub_08062AF4(u16 *out, u16 packId)
{
    struct PackSlots *pack = 0;
    int retry;
#define retry (*(int *)&pack)
    int valid;
    int i;
    u32 j;
    s16 id;
    u16 rolled;
    int common;
    int n;
    int idx;
    u16 *buf;

    switch (packId) {
    case 0x6E:
        PICK_RANDOM()
        break;
    case 0x66:
        PICK_RANDOM(&& CARD_TYPE(id) == 0x15)
        break;
    case 0x67:
        PICK_RANDOM(&& CARD_TYPE(id) == 0x16)
        break;
    default:
        goto normal;
    }
    *(u16 *)((u8 *)&gUnk_02015160 + 0x112) = 9999;
    return -1;
normal:
    for (i = 0; i < 28; i++) {
        if (gUnk_081A562C[i].id == packId)
            pack = gUnk_081A562C[i].p;
    }
    if (pack == 0)
        return -1;
    rolled = sub_08062A0C(pack, packId);
    common = sub_080629F0(pack);
    buf = (u16 *)((u8 *)&gUnk_02015160 + 2);
    out[0] = sub_08062AD4(pack, rolled);
    for (i = 0; i < pack->slot[common].count; i++)
        buf[i] = pack->slot[common].cards[i];
    for (i = 0; i < pack->slot[common].count * 2; i++) {
        int a = sub_08076F9C() % pack->slot[common].count;
        int b = sub_08076F9C() % pack->slot[common].count;
        u16 t = buf[a];
        buf[a] = buf[b];
        buf[b] = t;
    }
    idx = 0;
    n = pack->slot[common].count;
    for (i = 3; i >= 0; i--) {
        if (buf[idx] == out[0]) {
            u8 c = buf[idx];
            do {
                idx = (idx + 1) % n;
            } while (buf[idx] == c);
        }
        out[4 - i] = buf[idx];
        idx = (idx + 1) % n;
    }
    if (common != rolled)
        *(u16 *)((u8 *)&gUnk_02015160 + 0x112) = out[0];
    for (i = 24; i >= 0; i--) {
        int a = sub_08076F9C() % 5;
        int b = sub_08076F9C() % 5;
        u16 t = out[a];
        out[a] = out[b];
        out[b] = t;
    }
    return rolled;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080629F0", sub_08062AF4); /* 0x08062AF4 size 0x3F4 */
/* Card id to card key (0xFFFF means none and gives 0; ids >= 0x7D0 are alternate arts and use the table entry + 1). */
static inline int IdToKey(u16 id)
{
    int key;
    if (id == 0xFFFF)
        key = 0;
    else if (id <= 0x7CF)
        key = ((const u16 *)0x08623DF4)[id & 0x7FF];
    else
        key = ((const u16 *)0x08623DF4)[(id - 0x7D0) & 0x7FF] + 1;
    return key;
}

/* Returns 1 if the card with the given id (0xFFFF = none) appears in the trunk. */
u8 sub_08062EE8(u16 id)
{
    struct TrunkEntry *e;
    struct SaveMirror *save;
    u32 off;

    /* Split the key scaling so the save-base load sits between the two shifts. */
    off = (u32)(u16)IdToKey(id) << 16;
    save = &gUnk_02011C20;
    e = (struct TrunkEntry *)((off >> 14) + (u32)save);
    if (TrunkCount(e) == 0 && TrunkF2(e) == 0 && TrunkF4(e) == 0 && TrunkF6(e) == 0)
        return 0;
    return 1;
}

void sub_0806472C(void);
void sub_08064264(void);
int sub_08064AF4(void);
int sub_08064BA0(void);
int sub_08064DF8(void);
void sub_080754BC(void);
void sub_08075278(void *dst, u32 size);
int sub_08062AF4(u16 *out, u16 packId);

/* Pack info table entry (0x48 bytes, see wiki/data/booster-packs.md). */
struct PackInfoEntry {
    u16 id;
    u8 pad2[0x46];
};
extern struct PackInfoEntry gUnk_080865DC[];
extern u16 gUnk_0202037C;

struct SlideState {
    s32 state;      /* +0x00 */
    s32 unk4;       /* +0x04 */
    s32 target;     /* +0x08 */
    s32 current;    /* +0x0C */
    s32 frames;     /* +0x10 */
    u8 pad14[0x2C - 0x14];
    u16 list[0x20]; /* +0x2C */
    u8 pad6C[0x6C - 0x6C];
    u16 count;      /* +0x6C */
};
extern struct SlideState gUnk_02020310;

/* Runs the pack-opening sequence: sub-step at gMain+0x485A, pack id argument at gMain+0x4876. */
int sub_08062F6C(void)
{
    u16 *index;
    struct PackInfoEntry *info;
    int r;
    u16 id;
    u16 *ids;
    u8 *st;
    struct Main *m = &gUnk_03000040;
    st = &m->sub1;
    /* Keep the initialized command cursor live across the state calls. */
    __asm__ __volatile__("" : : "r"(st));
    switch (*st) {
    case 0:
        sub_0806472C();
        if (m->unk4876 != 0) {
            sub_08062AF4(gUnk_02015160.ids, m->unk4876);
            sub_080754BC();
            return 1;
        }
        sub_08064264();
        goto next;
    case 1:
        r = sub_08064AF4();
        goto check;
    case 2:
        r = sub_08064BA0();
        goto check;
    case 3:
        r = sub_08064DF8();
    check:
        if ((r << 16) != 0) {
            gUnk_02020310.state = 0;
            gUnk_02020310.unk4 = 0;
        next:
            (*st)++;
        }
        return 0;
    default:
        sub_08075278(&gUnk_02015160, 0x11C);
        index = &gUnk_02020310.list[(gUnk_02020310.frames + gUnk_02020310.current) % gUnk_02020310.count];
        ids = gUnk_02015160.ids;
        info = (struct PackInfoEntry *)0x080865DC;
        /* The ROM loads the table base before reading the selected index. */
        asm volatile ("" : : "r"(info));
        id = info[*index].id;
        sub_08062AF4(ids, id);
        sub_080754BC();
        return 1;
    }
}

void sub_08073574(void);
void sub_08073498(void);
void sub_08075630(void);
void sub_080759F4(void);
void sub_080757AC(void);
void sub_08075294(void *dst, const void *src, u32 size);
void sub_08062420(void);
extern const u8 gUnk_0867793C[], gUnk_0867795C[], gUnk_0867797C[], gUnk_0867817C[], gUnk_0867897C[];
extern const u8 gUnk_0867917C[], gUnk_0867997C[], gUnk_0867A17C[], gUnk_0867B17C[], gUnk_0867A97C[];
extern const u8 gUnk_0863CA9C[], gUnk_0863CB3C[], gUnk_0863CABC[], gUnk_0863CB5C[];
extern struct { u32 pad0; void (*hblankCallback)(void); } gUnk_03000000;

#if 0 /* NONMATCHING: same stores in the same order, but register allocation and scheduling differ
       * (loop counters, which BG-map pointer lives in r1/r2, constants chain for 0x2200..0x2208) */
/* Pack list scene setup: display registers, palettes, tiles, BG maps. */
int sub_08063040(void)
{
    s8 i;
    s16 j;
    s8 k;
    u16 *p;
    u16 *q;
    u16 *t;
    u8 *sp;
    gUnk_03000040.vblankFlags = 0xB83;
    REG_DISPCNT = 0x40;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x105;
    REG_BG2CNT = 0x206;
    REG_BG3CNT = 0x307;
    sub_08073574();
    sub_08073498();
    sub_08075630();
    REG_MOSAIC = 0;
    sub_080759F4();
    sub_08075294((void *)0x05000200, gUnk_0867793C, 0x20);
    sub_08075294((void *)0x05000220, gUnk_0867795C, 0x20);
    sub_08075294((void *)0x06010000, gUnk_0867797C, 0x800);
    sub_08075294((void *)0x06010800, gUnk_0867817C, 0x800);
    sub_08075294((void *)0x06011000, gUnk_0867897C, 0x800);
    sub_08075294((void *)0x06011800, gUnk_0867917C, 0x800);
    sub_08075294((void *)0x06012000, gUnk_0867997C, 0x800);
    sub_08075294((void *)0x06012800, gUnk_0867A17C, 0x800);
    sub_08075294((void *)0x06013000, gUnk_0867B17C, 0x800);
    sub_08075294((void *)0x06013800, gUnk_0867A97C, 0x800);
    sub_08075294((void *)0x05000020, gUnk_0863CA9C, 0x20);
    sub_08075294((void *)0x05000040, gUnk_0863CB3C, 0x20);
    sub_08075294((void *)0x06006600, gUnk_0863CABC, 0x80);
    p = gUnk_03000040.bgMap[3];
    sub_08075294((void *)0x06008000, gUnk_0863CB5C, 0x120);
    for (i = 15; i >= 0; i--) {
        q = p + 32;
        for (j = 15; j >= 0; j--) {
            p[0] = 0x1130;
            p[1] = 0x1131;
            q[0] = 0x1132;
            p[33] = 0x1133;
            q += 2;
            p += 2;
        }
        p += 32;
    }
    p = gUnk_03000040.bgMap[1];
    p[0] = 0x2200;
    q = p + 96;
    q[0] = 0x2201;
    q += 29;
    q[0] = 0x2202;
    p[29] = 0x2203;
    for (k = 1; k <= 28; k++) {
        t[0] = 0x2204;
        t[32] = 0x2208;
        t = &p[(u16)k];
        t[64] = 0x2208;
        t[96] = 0x2205;
    }
    q = p + 32;
    q[32] = 0x2206;
    q[0] = 0x2206;
    q[29] = 0x2207;
    q[61] = 0x2207;
    sub_080757AC();
    gUnk_03000040.vblankCallback = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    gUnk_03000000.hblankCallback = sub_08062420;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    gUnk_03000040.bgVofs[1] = -(gUnk_02015160.scroll.a << 5) - gUnk_0808658C[gUnk_02015160.scroll.b];
    gUnk_03000040.bgVofs[0] = 3;
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080629F0", sub_08063040); /* 0x08063040 size 0x2F0 */
void sub_0806245C(void);
int sub_08075AE4(int a);

u16 sub_08063330(void)
{
    REG_DISPCNT |= 0x1F00;
    sub_0806245C();
    return sub_08075AE4(4);
}
void sub_080624A4();
void sub_08077AEC(int a);
void sub_08062604(int idx, u16 key);

/* Pack-opening reveal: advances the five per-card states; returns 1 once more than four are past 0x17. */
int sub_08063354(void)
{
    int i;
    int done = 0;
    sub_0806245C();
    if ((u16)sub_08075AE4(4) == 0)
        return 0;
    sub_080624A4();
    for (i = 0; i <= 4; i++) {
        if (gUnk_02015160.state[i] <= 0x17) {
            if (gUnk_02015160.state[i] == 2)
                sub_08077AEC(6);
            if ((gUnk_03000040.keysHeld & 3) != 0 && gUnk_02015160.state[i] <= 0x16) {
                gUnk_02015160.state[i] = 0x17;
            } else if (i != 0) {
                if (gUnk_02015160.state[i - 1] > 0xC)
                    gUnk_02015160.state[i]++;
            } else {
                gUnk_02015160.state[0]++;
            }
        } else {
            done++;
        }
    }
    for (i = 0; i <= 4; i++) {
        if (gUnk_02015160.state[i] == 0x17)
            sub_08062604(i, IdToKey(gUnk_02015160.ids[i]));
    }
    return done > 4;
}
int sub_0806347C(void)
{
    sub_0806245C();
    sub_080624A4();
    gUnk_03000040.bgVofs[1] = -(gUnk_02015160.scroll.a << 5) - gUnk_0808658C[gUnk_02015160.scroll.b];
    if (gUnk_02015160.scroll.c != 0) {
        switch (gUnk_02015160.scroll.c) {
        case 1:
            if (gUnk_02015160.scroll.b != 0) {
                gUnk_02015160.scroll.b--;
            done:
                return 0;
            }
            gUnk_02015160.scroll.c = 0;
            break;
        case 2:
            gUnk_02015160.scroll.b++;
            if (gUnk_02015160.scroll.b != 0)
                goto done;
            gUnk_02015160.scroll.c = 0;
            gUnk_02015160.scroll.b = 0;
            gUnk_02015160.scroll.a++;
            break;
        default:
            gUnk_02015160.scroll.c = 0;
            break;
        }
    }
    if (gUnk_03000040.keysNew & 0x40) {
        if (gUnk_02015160.scroll.a != 0) {
            gUnk_02015160.scroll.a--;
            gUnk_02015160.scroll.b = 7;
            gUnk_02015160.scroll.c = 1;
            sub_08077AEC(0);
        } else {
            sub_08077AEC(3);
        }
    }
    if (gUnk_03000040.keysNew & 0x80) {
        if (gUnk_02015160.scroll.a <= 3) {
            gUnk_02015160.scroll.b = 0;
            gUnk_02015160.scroll.c = 2;
            sub_08077AEC(0);
        } else {
            sub_08077AEC(3);
        }
    }
    if (gUnk_03000040.keysNew & 1) {
        sub_08077AEC(1);
        gUnk_03000040.step += 3;
        gUnk_03000040.sub1 = 0;
        goto done;
    }
    if ((gUnk_03000040.keysNew & 2) == 0)
        goto done;
    sub_08077AEC(2);
    return 1;
}

void sub_080624A4(int a, int b, int c);
int sub_08075A6C(int a);
void sub_08077498(u16 key);

int sub_0806360C(void)
{
    int i;
    sub_0806245C();
    sub_080624A4(5, -1, 0);
    if ((u16)sub_08075A6C(2) == 0)
        return 0;
    for (i = 0; i < 5; i++)
        sub_08077498(IdToKey(gUnk_02015160.ids[i]));
    return 1;
}
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* ATK-like value (bits 9-17) times 10; 0 for Magic/Trap (types 0x15-0x17), 4000 for type 0x18. */
static inline int CardAtkValue(u16 id)
{
    int r;
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}

static inline int CardDefValue(u16 id)
{
    int r;
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = (CARD_STATS(id) & 0x1FF) * 10;
        break;
    }
    return r;
}

struct CardShow {
    u8 pad0[2];
    u16 key;        /* +0x02 */
    u8 pad4[0x2C - 4];
    s32 atk;        /* +0x2C */
    s32 def;        /* +0x30 */
};
extern struct CardShow gUnk_02013D90;
void sub_08006878(void);
u16 sub_0800696C(void);
void sub_08006B80(void);
int sub_08006A98(void);
int sub_08006AE8(void);
int sub_08006ABC(void);

/* Pack card browser: state machine on gMain+0x485A (0 setup, 1-2 fade in, 3 input, 4 / 10 wait, 11 exit). */
int sub_080636AC(void)
{
    switch (gUnk_03000040.sub1) {
    case 0:
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~2;
        gUnk_03000000.hblankCallback = 0;
        REG_IME = 1;
        sub_08006878();
        gUnk_02013D90.key = IdToKey(gUnk_02015160.ids[gUnk_02015160.scroll.a]);
        gUnk_02013D90.atk = CardAtkValue(IdToKey(gUnk_02015160.ids[gUnk_02015160.scroll.a]));
        gUnk_02013D90.def = CardDefValue(IdToKey(gUnk_02015160.ids[gUnk_02015160.scroll.a]));
        gUnk_03000040.sub1++;
        return 0;
    case 1:
        if ((u16)sub_0800696C() != 0) {
            sub_08006B80();
            gUnk_03000040.sub1++;
        }
        return 0;
    case 2:
        if ((sub_08006A98() << 16) != 0)
            gUnk_03000040.sub1++;
        return 0;
    case 3:
        if ((sub_08006AE8() << 16) != 0)
            gUnk_03000040.sub1++;
        if (gUnk_03000040.keysNew & 0x10) {
            gUnk_02015160.scroll.a = (gUnk_02015160.scroll.a + 1) % 5;
            gUnk_03000040.sub1 = 10;
        }
        if (gUnk_03000040.keysNew & 0x20) {
            gUnk_02015160.scroll.a = (gUnk_02015160.scroll.a + 4) % 5;
            gUnk_03000040.sub1 = 10;
        }
        return 0;
    case 4:
    case 10:
        if ((sub_08006ABC() << 16) != 0)
            gUnk_03000040.sub1++;
        return 0;
    case 11:
        gUnk_03000040.sub1 = 0;
        return 0;
    default:
        return 1;
    }
}
