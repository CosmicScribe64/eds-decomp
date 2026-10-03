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
extern struct SaveMirror gSaveData;
extern const s32 gPackRarityThresholds[8];

extern int Random(void);

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
extern struct Main gMain;
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
extern const u16 gPackCursorSlideOffsets[8];

struct PackBuf {
    u8 pad0[0x102];
    u16 ids[5];       /* +0x102 card ids of the pack being opened */
    u8 state[5];      /* +0x10C per-card reveal state (0x17 = revealed) */
    u8 pad111[0x114 - 0x111];
    struct PackScroll scroll;   /* +0x114 */
};
extern struct PackBuf gPackOpenWork;

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
extern const u16 gCardNumberToId[];
static inline int TrunkCount(struct TrunkEntry *e) { return e->count; }
static inline int TrunkF2(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f2; }
static inline int TrunkF4(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f4; }
static inline int TrunkF6(struct TrunkEntry *e) { return ((struct TrunkFlags *)e)->f6; }

int GetPackCommonSlot(struct PackSlots *p)
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

int RollPackRarity(struct PackSlots *p, u16 id)
{
    int r = Random() % 180;
    int i;
    struct PackSlot *s;
    const s32 *th;
    u16 *pity;
    if (gSaveData.lastPack == id)
        r = Random() % 270;
    if (gSaveData.pityCount > 5 && (gSaveData.lastPack != id || gSaveData.pityCount > 10)) {
        r = Random() % 12;
        gSaveData.pityCount = 0;
    }
    gSaveData.lastPack = id;
    i = 0;
    pity = &gSaveData.pityCount;
    for (s = p->slot, th = gPackRarityThresholds; i <= 6; s++, th++, i++) {
        if (r < *th && s->count > 0) {
            if (i <= 4 && i < GetPackCommonSlot(p))
                *pity = 0;
            return i;
        }
    }
    gSaveData.pityCount++;
    return GetPackCommonSlot(p);
}

u16 PickPackSlotCard(struct PackSlots *p, int slot)
{
    struct PackSlot *s = (struct PackSlot *)(slot * 8 + (u32)p);
    s32 n = s->count;
    return s->cards[Random() % n];
}

extern const u16 gCardIdToNumber[];
struct PackTblEntry {
    struct PackSlots *p;
    u16 id;
    u16 pad;
};
extern const struct PackTblEntry gPackContents[28];
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
            id = Random() % 0x335;                                     \
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

/* Pack-buffer view used by the generator (gPackOpenWork). */
struct PackGenBuf {
    u16 pad0;
    u16 buf[0x80];    /* +0x002 shuffled copy of the common slot */
    u16 ids[5];       /* +0x102 */
    u8 state[5];      /* +0x10C */
    u8 pad111;
    u16 x112;         /* +0x112 */
};
#define PACK_GEN ((struct PackGenBuf *)&gPackOpenWork)
#define PACK_SLOT(p, k) ((struct PackSlot *)((k) * 8 + (u32)(p)))

/* Five distinct random cards for a special pack: not a number in 0x780..0x7CF, of the wanted type
 * when TYPECHECK, and not a duplicate of an earlier pick. valid, id and j are per-case variables
 * (block scope): shared ones change the global-alloc priorities. */
#define PACK_PICK_RANDOM(TYPECHECK)                                          \
    {                                                                        \
        int valid;                                                           \
        u16 id;                                                              \
        for (i = 0; i <= 4; i++) {                                           \
            retry = 1;                                                       \
            do {                                                             \
                valid = 0;                                                   \
                id = Random() % 0x335;                                 \
                if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F TYPECHECK) {       \
                    int j;                                                   \
                    valid = 1;                                               \
                    for (j = 0; j < i; j++) {                                \
                        if (CARD_NUMBER(out[j]) == CARD_NUMBER(id))          \
                            valid = 0;                                       \
                    }                                                        \
                }                                                            \
                if (valid) {                                                 \
                    retry = 0;                                               \
                    out[i] = CARD_NUMBER(id);                                \
                }                                                            \
            } while (retry);                                                 \
        }                                                                    \
    }

/* Fills out[0..4] with the five cards of a booster pack; returns the rolled rarity slot, or -1. */
int GeneratePackCards(u16 *out, u16 packId)
{
    /* FAKEMATCH: one variable is both the special packs' retry flag and the pack pointer; as two
     * variables, out takes r9 instead of sl. */
    int retry = 0;
#define pack ((struct PackSlots *)retry)
    int i;
    int rolled;
    int common;
    int idx;

    /* Each special case has its own tail; cross-jumping merges them after reload, which keeps the
     * reload-register rotation of the ROM. */
    switch (packId) {
    case 0x6E:
        PACK_PICK_RANDOM()
        PACK_GEN->x112 = 9999;
        return -1;
    case 0x66:
        PACK_PICK_RANDOM(&& CARD_TYPE(id) == 0x15)
        PACK_GEN->x112 = 9999;
        return -1;
    case 0x67:
        PACK_PICK_RANDOM(&& CARD_TYPE(id) == 0x16)
        PACK_GEN->x112 = 9999;
        return -1;
    }

    for (i = 0; i < sizeof(gPackContents) / sizeof(gPackContents[0]); i++) {
        if (gPackContents[i].id == packId)
            retry = (int)gPackContents[i].p;
    }
    if (pack == 0)
        return -1;
    rolled = RollPackRarity(pack, packId);
    common = GetPackCommonSlot(pack);
    out[0] = PickPackSlotCard(pack, rolled);
    for (i = 0; i < PACK_SLOT(pack, common)->count; i++)
        PACK_GEN->buf[i] = PACK_SLOT(pack, common)->cards[i];
    for (i = 0; i < PACK_SLOT(pack, common)->count * 2; i++) {
        int a = Random() % PACK_SLOT(pack, common)->count;
        int b = Random() % PACK_SLOT(pack, common)->count;
        u16 x = PACK_GEN->buf[a];
        PACK_GEN->buf[a] = PACK_GEN->buf[b];
        PACK_GEN->buf[b] = x;
    }
    idx = 0;
    for (i = 1; i < 5; i++) {
        while (PACK_GEN->buf[idx] == out[0]) {
            idx++;
            idx %= PACK_SLOT(pack, common)->count;
        }
        out[i] = PACK_GEN->buf[idx];
        idx++;
        idx %= PACK_SLOT(pack, common)->count;
    }
    if (rolled != common)
        PACK_GEN->x112 = out[0];
    for (i = 0; i < 25; i++) {
        int a = Random() % 5;
        int b = Random() % 5;
        u16 x = out[a];
        out[a] = out[b];
        out[b] = x;
    }
    return rolled;
#undef pack
}
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
u8 IsCardNumberOwned(u16 id)
{
    struct TrunkEntry *e;
    struct SaveMirror *save;
    u32 off;

    /* Split the key scaling so the save-base load sits between the two shifts. */
    off = (u32)(u16)IdToKey(id) << 16;
    save = &gSaveData;
    e = (struct TrunkEntry *)((off >> 14) + (u32)save);
    if (TrunkCount(e) == 0 && TrunkF2(e) == 0 && TrunkF4(e) == 0 && TrunkF6(e) == 0)
        return 0;
    return 1;
}

void PackList_ClearWork(void);
void PackList_AddUnlockedPacks(void);
int PackList_Init(void);
int PackList_HandleInput(void);
int PackList_FadeOut(void);
void SaveGame(void);
void MemClear16(void *dst, u32 size);
int GeneratePackCards(u16 *out, u16 packId);

/* Pack info table entry (0x48 bytes, see wiki/data/booster-packs.md). */
struct PackInfoEntry {
    u16 id;
    u8 pad2[0x46];
};
extern struct PackInfoEntry gPackInfo[];
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
extern struct SlideState gSceneWork;

/* Runs the pack-opening sequence: sub-step at gMain+0x485A, pack id argument at gMain+0x4876. */
int GetPack_SelectAndGenerate(void)
{
    u16 *index;
    struct PackInfoEntry *info;
    int r;
    u16 id;
    u16 *ids;
    u8 *st;
    struct Main *m = &gMain;
    st = &m->sub1;
    /* Keep the initialized command cursor live across the state calls. */
    __asm__ __volatile__("" : : "r"(st));
    switch (*st) {
    case 0:
        PackList_ClearWork();
        if (m->unk4876 != 0) {
            GeneratePackCards(gPackOpenWork.ids, m->unk4876);
            SaveGame();
            return 1;
        }
        PackList_AddUnlockedPacks();
        goto next;
    case 1:
        r = PackList_Init();
        goto check;
    case 2:
        r = PackList_HandleInput();
        goto check;
    case 3:
        r = PackList_FadeOut();
    check:
        if ((r << 16) != 0) {
            gSceneWork.state = 0;
            gSceneWork.unk4 = 0;
        next:
            (*st)++;
        }
        return 0;
    default:
        MemClear16(&gPackOpenWork, 0x11C);
        index = &gSceneWork.list[(gSceneWork.frames + gSceneWork.current) % gSceneWork.count];
        ids = gPackOpenWork.ids;
        info = (struct PackInfoEntry *)0x080865DC;
        /* The ROM loads the table base before reading the selected index. */
        asm volatile ("" : : "r"(info));
        id = info[*index].id;
        GeneratePackCards(ids, id);
        SaveGame();
        return 1;
    }
}

void ResetVideo(void);
void ClearBgMapBuffers(void);
void LoadSystemGfx(void);
void SetBrightnessBlack(void);
void ResetBgScroll(void);
void MemCopy16(void *dst, const void *src, u32 size);
void GetPack_HBlank(void);
extern const u8 gHandCursorPal[], gCardIconPal[], gHandCursorGfx[], gUnk_0867817C[], gCardIconNormalGfx[];
extern const u8 gCardIconEffectGfx[], gCardIconFusionGfx[], gCardIconRitualGfx[], gCardIconMagicGfx[], gCardIconTrapGfx[];
extern const u8 gPackSceneBgPal[], gPackCursorFramePal[], gPackSceneBgTiles[], gPackCursorFrameTiles[];
extern struct { u32 pad0; void (*hblankCallback)(void); } IntrTable;

/* Pack list scene setup: display registers, palettes, tiles, BG maps, reveal-state reset. */
/* Two u16 fields of the pack buffer after the scroll byte; as 16-bit bitfields their zero store
 * goes through an SImode constant that the reveal-state loop reuses (plain u16 stores reload it). */
struct PackTail {
    u8 pad[0x116];
    u16 x116 : 16;
    u16 x118 : 16;
};
int GetPack_InitScene(void)
{
    int i;
    int j;
    u16 *p;
    u16 *q;
    struct PackBuf *pb;
    gMain.vblankFlags = 0xB83;
    REG_DISPCNT = 0x40;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x105;
    REG_BG2CNT = 0x206;
    REG_BG3CNT = 0x307;
    ResetVideo();
    ClearBgMapBuffers();
    LoadSystemGfx();
    REG_MOSAIC = 0;
    SetBrightnessBlack();
    MemCopy16((void *)0x05000200, gHandCursorPal, 0x20);
    MemCopy16((void *)0x05000220, gCardIconPal, 0x20);
    MemCopy16((void *)0x06010000, gHandCursorGfx, 0x800);
    MemCopy16((void *)0x06010800, gUnk_0867817C, 0x800);
    MemCopy16((void *)0x06011000, gCardIconNormalGfx, 0x800);
    MemCopy16((void *)0x06011800, gCardIconEffectGfx, 0x800);
    MemCopy16((void *)0x06012000, gCardIconFusionGfx, 0x800);
    MemCopy16((void *)0x06012800, gCardIconRitualGfx, 0x800);
    MemCopy16((void *)0x06013000, gCardIconMagicGfx, 0x800);
    MemCopy16((void *)0x06013800, gCardIconTrapGfx, 0x800);
    MemCopy16((void *)0x05000020, gPackSceneBgPal, 0x20);
    MemCopy16((void *)0x05000040, gPackCursorFramePal, 0x20);
    MemCopy16((void *)0x06006600, gPackSceneBgTiles, 0x80);
    MemCopy16((void *)0x06008000, gPackCursorFrameTiles, 0x120);
    p = gMain.bgMap[3];
    for (i = 0; i < 16; i++) {
        j = 15; /* set before q: lengthens j's live range so p wins r2 in global alloc */
        q = p + 32;
        for (; j >= 0; j--) {
            p[0] = 0x1130;
            p[1] = 0x1131;
            q[0] = 0x1132;
            p[33] = 0x1133;
            q += 2;
            p += 2;
        }
        p += 32;
    }
    p = gMain.bgMap[1];
    p[0] = 0x2200;
    p[96] = 0x2201;
    p[125] = 0x2202;
    p[29] = 0x2203;
    for (i = 1; i <= 28; i++) {
        q = (u16 *)((u16)i * 2 + (u32)p); /* offset-first sum: adds r1, r0, r2 */
        q[0] = 0x2204;
        q[32] = 0x2208;
        q[64] = 0x2208;
        q[96] = 0x2205;
    }
    p[32] = 0x2206;
    p[64] = 0x2206;
    p[61] = 0x2207;
    p[93] = 0x2207;
    ResetBgScroll();
    pb = &gPackOpenWork;
    ((struct PackTail *)pb)->x116 = 0;
    ((struct PackTail *)pb)->x118 = 0;
    for (i = 0; i < 5; i++) /* loop.c reverses this into the ROM's 0x110-down store loop */
        pb->state[i] = 0;
    ResetBgScroll();
    gMain.vblankCallback = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    IntrTable.hblankCallback = GetPack_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    gMain.bgVofs[1] = -(gPackOpenWork.scroll.a << 5) - gPackCursorSlideOffsets[gPackOpenWork.scroll.b];
    gMain.bgVofs[0] = 3;
    return 1;
}
void GetPack_ScrollBg(void);
int FadeFromBlack(int a);

u16 GetPack_FadeIn(void)
{
    REG_DISPCNT |= 0x1F00;
    GetPack_ScrollBg();
    return FadeFromBlack(4);
}
void GetPack_DrawCardSprites();
void PlaySE(int a);
void GetPack_DrawCardRow(int idx, u16 key);

/* Pack-opening reveal: advances the five per-card states; returns 1 once more than four are past 0x17. */
int GetPack_RevealCards(void)
{
    int i;
    int done = 0;
    GetPack_ScrollBg();
    if ((u16)FadeFromBlack(4) == 0)
        return 0;
    GetPack_DrawCardSprites();
    for (i = 0; i <= 4; i++) {
        if (gPackOpenWork.state[i] <= 0x17) {
            if (gPackOpenWork.state[i] == 2)
                PlaySE(6);
            if ((gMain.keysHeld & 3) != 0 && gPackOpenWork.state[i] <= 0x16) {
                gPackOpenWork.state[i] = 0x17;
            } else if (i != 0) {
                if (gPackOpenWork.state[i - 1] > 0xC)
                    gPackOpenWork.state[i]++;
            } else {
                gPackOpenWork.state[0]++;
            }
        } else {
            done++;
        }
    }
    for (i = 0; i <= 4; i++) {
        if (gPackOpenWork.state[i] == 0x17)
            GetPack_DrawCardRow(i, IdToKey(gPackOpenWork.ids[i]));
    }
    return done > 4;
}
int GetPack_HandleInput(void)
{
    GetPack_ScrollBg();
    GetPack_DrawCardSprites();
    gMain.bgVofs[1] = -(gPackOpenWork.scroll.a << 5) - gPackCursorSlideOffsets[gPackOpenWork.scroll.b];
    if (gPackOpenWork.scroll.c != 0) {
        switch (gPackOpenWork.scroll.c) {
        case 1:
            if (gPackOpenWork.scroll.b != 0) {
                gPackOpenWork.scroll.b--;
            done:
                return 0;
            }
            gPackOpenWork.scroll.c = 0;
            break;
        case 2:
            gPackOpenWork.scroll.b++;
            if (gPackOpenWork.scroll.b != 0)
                goto done;
            gPackOpenWork.scroll.c = 0;
            gPackOpenWork.scroll.b = 0;
            gPackOpenWork.scroll.a++;
            break;
        default:
            gPackOpenWork.scroll.c = 0;
            break;
        }
    }
    if (gMain.keysNew & 0x40) {
        if (gPackOpenWork.scroll.a != 0) {
            gPackOpenWork.scroll.a--;
            gPackOpenWork.scroll.b = 7;
            gPackOpenWork.scroll.c = 1;
            PlaySE(0);
        } else {
            PlaySE(3);
        }
    }
    if (gMain.keysNew & 0x80) {
        if (gPackOpenWork.scroll.a <= 3) {
            gPackOpenWork.scroll.b = 0;
            gPackOpenWork.scroll.c = 2;
            PlaySE(0);
        } else {
            PlaySE(3);
        }
    }
    if (gMain.keysNew & 1) {
        PlaySE(1);
        gMain.step += 3;
        gMain.sub1 = 0;
        goto done;
    }
    if ((gMain.keysNew & 2) == 0)
        goto done;
    PlaySE(2);
    return 1;
}

void GetPack_DrawCardSprites(int a, int b, int c);
int FadeToBlack(int a);
void AddCardToTrunk(u16 key);

int GetPack_FadeOutAndAddCards(void)
{
    int i;
    GetPack_ScrollBg();
    GetPack_DrawCardSprites(5, -1, 0);
    if ((u16)FadeToBlack(2) == 0)
        return 0;
    for (i = 0; i < 5; i++)
        AddCardToTrunk(IdToKey(gPackOpenWork.ids[i]));
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
extern struct CardShow gCardDetail;
void CardDetail_Reset(void);
u16 CardDetail_InitVideo(void);
void CardDetail_DrawCard(void);
int CardDetail_FadeIn(void);
int CardDetail_HandleInput(void);
int CardDetail_FadeOut(void);

/* Pack card browser: state machine on gMain+0x485A (0 setup, 1-2 fade in, 3 input, 4 / 10 wait, 11 exit). */
int GetPack_ShowCardDetail(void)
{
    switch (gMain.sub1) {
    case 0:
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~2;
        IntrTable.hblankCallback = 0;
        REG_IME = 1;
        CardDetail_Reset();
        gCardDetail.key = IdToKey(gPackOpenWork.ids[gPackOpenWork.scroll.a]);
        gCardDetail.atk = CardAtkValue(IdToKey(gPackOpenWork.ids[gPackOpenWork.scroll.a]));
        gCardDetail.def = CardDefValue(IdToKey(gPackOpenWork.ids[gPackOpenWork.scroll.a]));
        gMain.sub1++;
        return 0;
    case 1:
        if ((u16)CardDetail_InitVideo() != 0) {
            CardDetail_DrawCard();
            gMain.sub1++;
        }
        return 0;
    case 2:
        if ((CardDetail_FadeIn() << 16) != 0)
            gMain.sub1++;
        return 0;
    case 3:
        if ((CardDetail_HandleInput() << 16) != 0)
            gMain.sub1++;
        if (gMain.keysNew & 0x10) {
            gPackOpenWork.scroll.a = (gPackOpenWork.scroll.a + 1) % 5;
            gMain.sub1 = 10;
        }
        if (gMain.keysNew & 0x20) {
            gPackOpenWork.scroll.a = (gPackOpenWork.scroll.a + 4) % 5;
            gMain.sub1 = 10;
        }
        return 0;
    case 4:
    case 10:
        if ((CardDetail_FadeOut() << 16) != 0)
            gMain.sub1++;
        return 0;
    case 11:
        gMain.sub1 = 0;
        return 0;
    default:
        return 1;
    }
}
