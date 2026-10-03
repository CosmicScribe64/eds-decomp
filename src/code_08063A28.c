#include "global.h"
#include "gba.h"

/*
 * Booster-pack scene helpers ("Get a pack" debug entry 0x08063AF8), card-key
 * lookup and the duelist-progress checks used for unlocks.
 * See wiki/functions/code-08063a28.md
 */

/* Per-duelist record, 4 bytes at save+0x20D0 + idx*4 (hypothesis: idx is a duelist/opponent index). */
struct DuelistRec {
    u16 a : 11;
    u16 rest : 5;
    u16 hi;
};
/* Per-card collection record, 4 bytes at save+8 + id*4 (see wiki/functions/code-0807717c.md). */
struct CardCount {
    u16 count : 10;                 /* copies owned */
    u16 rest : 6;
    u16 unkA;
};
struct CardBits {                   /* byte view of +1 of the record */
    u8 unk0;
    u8 pad : 2;
    u8 n1 : 2;
    u8 n2 : 2;
    u8 n3 : 2;
    u16 unkA;
};
union CardEntry {
    struct CardCount c;
    struct CardBits b;
};

struct Save {
    u8 pad0[8];
    union CardEntry cards[0x800];   /* +0x0008 */
    u8 pad2008[0xC8];
    struct DuelistRec rec[0x20];    /* +0x20D0 */
    u8 pad2150[0x12];
    u8 unk2162;                     /* +0x2162 saturating counter */
};
extern struct Save gSaveData;

/* Pack scene state at 0x02015160. */
struct PackScene {
    u8 pad0[0x102];
    u16 ids[5];                     /* +0x102 */
    u8 bytes[5];                    /* +0x10C */
};
extern struct PackScene gPackOpenWork;

extern const u16 gCardNumberToId[];
extern const u16 gUnk_08624568[];
u16 IsCampaignLevel2Unlocked(void);
u16 IsCampaignLevel3Unlocked(void);
u16 IsCampaignLevel4Unlocked(void);
u16 IsCampaignLevel5Unlocked(void);
u16 IsCardCollectionComplete(void);
extern const u16 gUnk_0862311E[];
/* gMain (0x03000040): step index of the running sequence at +0x4859, sub-counters at +0x485A/+0x485B. */
struct Main {
    u8 pad0[6];
    u16 keysNew;                    /* +0x06 newly pressed keys (hypothesis) */
    u8 pad8[0x40E - 8];
    u16 vblankFlags;                /* +0x40E */
    u8 pad410[4];
    u32 unk414;                     /* +0x414 */
    u8 pad418[0x442A - 0x418];
    u16 unk442A;                    /* +0x442A */
    u8 pad442C[0x4859 - 0x442C];
    u8 step;                        /* +0x4859 */
    u8 sub1;                        /* +0x485A */
    u8 sub2;                        /* +0x485B */
    u8 pad485C[0x4876 - 0x485C];
    u16 unk4876;                    /* +0x4876 */
};
extern struct Main gMain;
extern u16 (*const gGetPackSteps[])(void);
/* Cursor/scroll state at 0x02020310 (hypothesis: menu cursor slide animation). */
struct Slide {
    s32 state;                      /* +0x00 */
    s32 unk4;                       /* +0x04 */
    s32 target;                     /* +0x08 */
    s32 current;                    /* +0x0C */
    s32 frames;                     /* +0x10 */
    u8 pad14[4];
    u8 flags;                       /* +0x18 */
    u8 pad19[0x2C - 0x19];
    u16 list[0x20];                 /* +0x2C (0x0202033C) */
};
extern struct Slide gSceneWork;
/* Pack list state at 0x0202033C: row indices of the pack info table plus a count at +0x40. */
extern const u8 gPackListPal[], gPackListBgTiles[], gUnk_0863CEBC[], gUnk_0863CEFC[];
extern u16 gUnk_0202037C;
extern u16 gUnk_0202033C[];       /* count, list of u16 at -0x40 */
struct PackInfo {
    u16 id;
    u8 pad2[2];
    const u8 *image;                /* +0x04 cover art */
    u8 name[0x40];
};
extern struct PackInfo gPackInfo[];
/* Start of IWRAM (hypothesis: interrupt/vblank state). */
struct Irq {
    u32 unk0;
    u32 unk4;
};
extern struct Irq IntrTable;
extern u16 gUnk_03001C5C[];
void LoadBgImageMap1();
void StarterDeckSelect_DrawBackground(s32 a, s32 b, u16 c, const void *src);
void PackList_InitVideo(void);
void StarterDeckSelect_ClearWork(void);
u16 StarterDeckSelect_Init(void);
u16 StarterDeckSelect_HandleInput(void);
u16 StarterDeckSelect_FadeOut(void);
void InitSaveData(void);
void BuildStarterDeck(u32 a);
void SaveGame(void);
void StarterDeckSelect_DrawCursor(void);
extern const u8 gStarterDeckBgImage[], gStarterDeckBoxBlackImage[], gStarterDeckBoxRedImage[], gStarterDeckBoxGreenImage[], gHandCursorPal[], gHandCursorGfx[];
void ResetVideo(void);
void SetBrightnessBlack(void);
void ResetBgScroll(void);
void AddSprite(u32 a, u32 b, u32 c);
u16 IsPackUnlocked(u32 a);
extern const u16 gUnlockablePackIds[];
void PackList_AddPack(u32 a);
u32 GetPack_InitScene(void);
void PackList_LoadCoverGfx(u16 a, u16 b);
void PackList_DrawCoverTiles(u32 a, u32 b, u32 c);
extern s32 __modsi3(s32 a, s32 b);
void GetPack_ScrollBg(void);
void GetPack_DrawCardSprites(void);
u16 FadeFromBlack(u32 a);
void PlaySE(u32 id);
u16 FadeToBlack(u32 a);
void GetPack_DrawCardRow(u32 idx, u32 key);

/* BEGIN GetPack_RestoreScene */
/* Map the 5 card ids in the pack scene to their base-card keys and register them. */
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
u32 GetPack_RestoreScene(void) {
    s32 i;
    GetPack_InitScene();
    for (i = 0; i <= 4; i++) {
        gPackOpenWork.bytes[i] = 0x18;
        GetPack_DrawCardRow(i, (u16)IdToKey(gPackOpenWork.ids[i]));
    }
    return 1;
}
/* END GetPack_RestoreScene */
/* BEGIN GetPack_FadeInAndResume */
/* Pack scene init step: enable BG/OBJ layers in DISPCNT, run the two setup routines, step back by 5 if 0x08075AE4(4) succeeds. */
u32 GetPack_FadeInAndResume(void) {
    REG_DISPCNT |= 0x1F00;
    GetPack_ScrollBg();
    GetPack_DrawCardSprites();
    if (FadeFromBlack(4))
        gMain.step -= 5;
    return 0;
}
/* END GetPack_FadeInAndResume */
/* BEGIN CB_GetPack */
/* "Get a pack" debug runner: call step gGetPackSteps[gMain.step]; when it returns non-zero go to the next step. Returns 1 at the end of the table. */
u32 CB_GetPack(void) {
    if (gGetPackSteps[gMain.step] != 0) {
        if (gGetPackSteps[gMain.step]()) {
            gMain.step++;
            gMain.sub1 = 0;
            gMain.sub2 = 0;
        }
        return 0;
    }
    return 1;
}
/* END CB_GetPack */
/* BEGIN GetRewardPack */
/* Same runner, but first stores `arg` in gMain+0x4876. */
u32 GetRewardPack(u32 arg) {
    gMain.unk4876 = arg;
    if (gGetPackSteps[gMain.step] != 0) {
        if (gGetPackSteps[gMain.step]()) {
            gMain.step++;
            gMain.sub1 = 0;
            gMain.sub2 = 0;
        }
        return 0;
    }
    return 1;
}
/* END GetRewardPack */
/* BEGIN GetPack_UnusedReturnTrue */
u32 GetPack_UnusedReturnTrue(void) {
    return 1;
}
/* END GetPack_UnusedReturnTrue */
/* BEGIN IsCampaignLevel2Unlocked */
/* 1 if records 1..5 all have a > 1 (hypothesis: the 5 opponents of the first league beaten twice). */
u16 IsCampaignLevel2Unlocked(void) {
    struct Save *s = &gSaveData;
    s32 a;
    a = s->rec[1].a; if (a <= 1) return 0;
    a = s->rec[2].a; if (a <= 1) return 0;
    a = s->rec[3].a; if (a <= 1) return 0;
    a = s->rec[4].a; if (a <= 1) return 0;
    a = s->rec[5].a; if (a <= 1) return 0;
    return 1;
}
/* END IsCampaignLevel2Unlocked */
/* BEGIN IsCampaignLevel3Unlocked */
/* Records 6..10 all > 2. */
u16 IsCampaignLevel3Unlocked(void) {
    struct Save *s = &gSaveData;
    s32 a;
    a = s->rec[6].a; if (a <= 2) return 0;
    a = s->rec[7].a; if (a <= 2) return 0;
    a = s->rec[8].a; if (a <= 2) return 0;
    a = s->rec[9].a; if (a <= 2) return 0;
    a = s->rec[10].a; if (a <= 2) return 0;
    return 1;
}
/* END IsCampaignLevel3Unlocked */
/* BEGIN IsCampaignLevel4Unlocked */
/* Records 11..15 all > 3. */
u16 IsCampaignLevel4Unlocked(void) {
    struct Save *s = &gSaveData;
    s32 a;
    a = s->rec[11].a; if (a <= 3) return 0;
    a = s->rec[12].a; if (a <= 3) return 0;
    a = s->rec[13].a; if (a <= 3) return 0;
    a = s->rec[14].a; if (a <= 3) return 0;
    a = s->rec[15].a; if (a <= 3) return 0;
    return 1;
}
/* END IsCampaignLevel4Unlocked */
/* BEGIN IsCampaignLevel5Unlocked */
/* Records 16..20 all > 4. */
u16 IsCampaignLevel5Unlocked(void) {
    struct Save *s = &gSaveData;
    s32 a;
    a = s->rec[16].a; if (a <= 4) return 0;
    a = s->rec[17].a; if (a <= 4) return 0;
    a = s->rec[18].a; if (a <= 4) return 0;
    a = s->rec[19].a; if (a <= 4) return 0;
    a = s->rec[20].a; if (a <= 4) return 0;
    return 1;
}
/* END IsCampaignLevel5Unlocked */
/* BEGIN IsCardCollectionComplete */
u16 IsCardCollectionComplete(void)
{
    s32 have = 0;
    /* FAKEMATCH: retain ROM counter registers and rematerialized loop bound. */
    register s32 total __asm__("r4") = 0;
    register s32 id __asm__("r3") = 1;
    u16 key = gUnk_0862311E[0];
    u32 limit = 0x77F;
    struct Save *s = &gSaveData;
    union CardEntry *e = &s->cards[1];
    register s32 bound __asm__("r0");
    do {
        if (key <= limit) {
            u32 v;
            total++;
            v = *(u16 *)e;
            if ((v << 22) != 0) {
                have++;
            } else {
                u32 b = ((u8 *)e)[1];
                if (((b << 28) >> 30) != 0 || ((b << 26) >> 30) != 0)
                    have++;
            }
        }
        e++;
        id++;
        bound = 0x334;
    } while (id <= bound);
    if (have == total)
        return 1;
    return 0;
}
/* END IsCardCollectionComplete */
/* BEGIN IsOpponentUnlocked */
/* Achievement/unlock condition check for entry `id` (1..24); returns 1 when fulfilled. */
u16 IsOpponentUnlocked(u16 id) {
    switch (id) {
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        return IsCampaignLevel2Unlocked();
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        return IsCampaignLevel3Unlocked();
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
        return IsCampaignLevel4Unlocked();
    case 22: {
        u32 r = 0;
        if (gSaveData.unk2162 > 1)
            r = 1;
        return r;
    }
    case 23: {
        u8 *base = (u8 *)&gSaveData;
        u8 *p = base + gUnk_08624568[0] * 4;
        u32 v = *(u16 *)(p + 8);
        u32 b;
        if ((v << 22) != 0)
            return 1;
        b = p[9];
        if (((b << 28) >> 30) != 0 || ((b << 26) >> 30) != 0)
            return 1;
        return 0;
    }
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 1;
    case 24:
        return IsCardCollectionComplete();
    case 21:
        return IsCampaignLevel5Unlocked();
    default:
        return 0;
    }
}
/* END IsOpponentUnlocked */
/* BEGIN GetCampaignLevel */
/* Highest fulfilled tier (1..5) of the duelist-progress checks. */
u32 GetCampaignLevel(void) {
    u32 tier = 1;
    if (IsCampaignLevel2Unlocked())
        tier = 2;
    if (IsCampaignLevel3Unlocked())
        tier = 3;
    if (IsCampaignLevel4Unlocked())
        tier = 4;
    if (IsCampaignLevel5Unlocked())
        tier = 5;
    return tier;
}
/* END GetCampaignLevel */
/* Progress predicates for the booster-pack ids; save record values are compared as signed integers. */
u16 IsPackUnlocked(u32 arg)
{
    u16 id = arg;
    switch (id) {
    case 1:
    case 2:
    case 3:
        return 1;
    case 0x1F6:
        return IsCampaignLevel2Unlocked();
    case 0x1FB:
        return IsCampaignLevel3Unlocked();
    case 0x21:
        return IsCampaignLevel4Unlocked();
    case 0x1F5: {
        int result = 0;
        struct Save *s = &gSaveData;
        int sum = s->rec[1].a + s->rec[2].a + s->rec[3].a + s->rec[4].a + s->rec[5].a;
        if (sum > 9)
            result = 1;
        return result;
    }
    case 4: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[1].a) > 9 && (value = s->rec[2].a) > 9 && (value = s->rec[3].a) > 9 && (value = s->rec[4].a) > 9 && (value = s->rec[5].a) > 9)
            result = 1;
        return result;
    }
    case 5: {
        int result = 0;
        struct Save *s = &gSaveData;
        int sum = s->rec[6].a + s->rec[7].a + s->rec[8].a + s->rec[9].a + s->rec[10].a;
        if (sum > 9)
            result = 1;
        return result;
    }
    case 6: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[6].a) > 9 && (value = s->rec[7].a) > 9 && (value = s->rec[8].a) > 9 && (value = s->rec[9].a) > 9 && (value = s->rec[10].a) > 9)
            result = 1;
        return result;
    }
    case 0x15: {
        int result = 0;
        struct Save *s = &gSaveData;
        int sum = s->rec[11].a + s->rec[12].a + s->rec[13].a + s->rec[14].a + s->rec[15].a;
        if (sum > 9)
            result = 1;
        return result;
    }
    case 0x29: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[11].a) > 9 && (value = s->rec[12].a) > 9 && (value = s->rec[13].a) > 9 && (value = s->rec[14].a) > 9 && (value = s->rec[15].a) > 9)
            result = 1;
        return result;
    }
    case 0x1F8: {
        int result = 0;
        struct Save *s = &gSaveData;
        int sum = s->rec[16].a + s->rec[17].a + s->rec[18].a + s->rec[19].a + s->rec[20].a;
        if (sum > 9)
            result = 1;
        return result;
    }
    case 0x1F9: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[16].a) > 9 && (value = s->rec[17].a) > 9 && (value = s->rec[18].a) > 9 && (value = s->rec[19].a) > 9 && (value = s->rec[20].a) > 9)
            result = 1;
        return result;
    }
    case 11: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[1].a) > 19)
            result = 1;
        return result;
    }
    case 12: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[3].a) > 19)
            result = 1;
        return result;
    }
    case 0x1F7: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[9].a) > 19)
            result = 1;
        return result;
    }
    case 7: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[10].a) > 19)
            result = 1;
        return result;
    }
    case 0x1FC: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[14].a) > 19)
            result = 1;
        return result;
    }
    case 0x16: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[15].a) > 19)
            result = 1;
        return result;
    }
    case 0x1FA: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[16].a) > 19)
            result = 1;
        return result;
    }
    case 0x17: {
        int result = 0;
        struct Save *s = &gSaveData;
        s32 value;
        if ((value = s->rec[20].a) > 19)
            result = 1;
        return result;
    }
    case 0x1FD: {
        int value = gSaveData.rec[22].a;
        if (value != 0)
            value = 1;
        return value;
    }
    default:
        return 0;
    }
}

/* BEGIN PackList_AddUnlockedPacks */
/* Run the unlock check for each of the 27 entries in gUnlockablePackIds and apply the ones that pass. */
void PackList_AddUnlockedPacks(void) {
    u32 i;
    const u16 *p;
    for (i = 0, p = gUnlockablePackIds; i <= 0x1A; p++, i++) {
        if (IsPackUnlocked(*p))
            PackList_AddPack(*p);
    }
}
/* END PackList_AddUnlockedPacks */
/* BEGIN PackList_InitVideo */
/* Scene init: video registers, IRQ enables (HBLANK off), clear the vblank flag. */
void PackList_InitVideo(void) {
    gMain.vblankFlags = 0x21;
    REG_DISPCNT = 0x40;
    REG_BG0CNT = 0x84;
    REG_BG1CNT = 0x4185;
    REG_BG2CNT = 0x386;
    REG_BG3CNT = 0x484;
    ResetVideo();
    REG_MOSAIC = 0;
    SetBrightnessBlack();
    ResetBgScroll();
    gMain.unk414 = 0;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    {
        struct Irq *irq = &IntrTable;
        irq->unk4 = 0;
    }
    REG_IME = 1;
}
/* END PackList_InitVideo */
/* BEGIN StarterDeckSelect_DrawBackground */
/* Fill rows a..b-1 of the 32x32 tile map at 0x03001C5C with c/2 after clearing 0xC80 bytes. */
void StarterDeckSelect_DrawBackground(s32 a, s32 b, u16 c, const void *src) {
    s32 i;
    s32 j;
    LoadBgImageMap1(0xC80, 0, c);
    for (i = a; i < b; ) {
        u16 x;
        j = 0;
        x = i;
        i++;
        for (; j <= 0x1F; j++)
            gUnk_03001C5C[(u16)j + (x << 5)] = c >> 1;
    }
}
/* END StarterDeckSelect_DrawBackground */
/* BEGIN StarterDeckSelect_DrawCursor */
/* Slide a sprite between two 80-pixel slots over 4 frames (hypothesis: menu cursor). */
void StarterDeckSelect_DrawCursor(void) {
    s32 c;
    s32 pos;
    if (gSceneWork.current != gSceneWork.target && gSceneWork.frames == 0)
        gSceneWork.frames = 4;
    c = gSceneWork.frames;
    if (c > 0) {
        s32 from = gSceneWork.current * 80;
        s32 to = gSceneWork.target * 80;
        s32 base = to + 0x20;
        s32 d = (to - from) * c;
        pos = base - d / 4;
        gSceneWork.frames = c - 1;
        if (gSceneWork.frames == 0)
            gSceneWork.current = gSceneWork.target;
    } else {
        pos = gSceneWork.target * 80 + 0x20;
    }
    AddSprite(pos | 0x580000, 0x80, 0x100);
}
/* END StarterDeckSelect_DrawCursor */
/* BEGIN StarterDeckSelect_ClearWork */
/* Clear the 0x70-byte slide state at 0x02020310 with DMA3. */
void StarterDeckSelect_ClearWork(void) {
    vu16 zero = 0;
    vu32 *dma = (vu32 *)0x040000D4;

    dma[0] = (u32)&zero;
    dma[1] = (u32)&gSceneWork;
    dma[2] = 0x81004038;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}
/* END StarterDeckSelect_ClearWork */
/* BEGIN StarterDeckSelect_Init */
/* Slide-in scene step: 0 = init state, 1 = load tile maps, palettes and graphics, 2+ = animate. */
u16 StarterDeckSelect_Init(void) {
    u32 state = gSceneWork.state;
    switch (state) {
    case 0:
        gSceneWork.target = 1;
        gSceneWork.current = 1;
        gSceneWork.frames = state;
        PackList_InitVideo();
        gMain.vblankFlags |= 2;
        gSceneWork.state++;
        return 0;
    case 1:
        StarterDeckSelect_DrawBackground(2, 0x10, 0x200, gStarterDeckBgImage);
        LoadBgImageMap1(0x82, 0, 0x20, gStarterDeckBoxBlackImage);
        LoadBgImageMap1(0x8C, 0, 0xAC, gStarterDeckBoxRedImage);
        LoadBgImageMap1(0x96, 0, 0x138, gStarterDeckBoxGreenImage);
        {
            vu32 *dma = (vu32 *)0x040000D4;
            dma[0] = (u32)gHandCursorPal;
            dma[1] = 0x05000200;
            dma[2] = 0x80000010;
            dma[2];
            while (dma[2] & 0x80000000)
                ;
        }
        {
            vu32 *dma = (vu32 *)0x040000D4;
            dma[0] = (u32)gHandCursorGfx;
            dma[1] = 0x06012000;
            dma[2] = 0x80000100;
            dma[2];
            while (dma[2] & 0x80000000)
                ;
        }
        gSceneWork.state++;
        return 0;
    default:
        REG_DISPCNT |= 0x1640;
        StarterDeckSelect_DrawCursor();
        return FadeFromBlack(2);
    }
}
/* END StarterDeckSelect_Init */
/* BEGIN StarterDeckSelect_HandleInput */
/* Menu input for the slide cursor: A confirms (returns 1), LEFT/RIGHT move the target slot 0..2. */
u16 StarterDeckSelect_HandleInput(void) {
    StarterDeckSelect_DrawCursor();
    if (gSceneWork.frames == 0) {
        if (gMain.keysNew & 1) {
            PlaySE(1);
            return 1;
        }
        if (gMain.keysNew & 0x20) {
            if (gSceneWork.target > 0) {
                PlaySE(0);
                gSceneWork.target--;
            }
        }
        if (gMain.keysNew & 0x10) {
            if (gSceneWork.target <= 1) {
                PlaySE(0);
                gSceneWork.target++;
            }
        }
    }
    return 0;
}
/* END StarterDeckSelect_HandleInput */
/* BEGIN StarterDeckSelect_FadeOut */
/* Slide-out step: blink for 0x3C frames (every 4th frame draws), then fade and hide layers; returns 1 when done. */
u16 StarterDeckSelect_FadeOut(void) {
    if (gSceneWork.state <= 0x3B) {
        if ((gSceneWork.state >> 2) & 1)
            StarterDeckSelect_DrawCursor();
        gSceneWork.state++;
        return 0;
    }
    StarterDeckSelect_DrawCursor();
    if (FadeToBlack(2)) {
        REG_DISPCNT &= 0xEEFF;
        return 1;
    }
    return 0;
}
/* END StarterDeckSelect_FadeOut */
/* BEGIN StarterDeckSelect_Run */
/* Slide-cursor scene main callback (steps 0..4). Returns 1 when the scene is finished. */
u16 StarterDeckSelect_Run(void) {
    switch (gMain.step) {
    case 0:
        StarterDeckSelect_ClearWork();
        goto advance;
    case 1:
        if (StarterDeckSelect_Init())
            goto advance;
        break;
    case 2:
        if (StarterDeckSelect_HandleInput())
            goto advance;
        break;
    case 3:
        if (StarterDeckSelect_FadeOut())
            goto advance;
        break;
    case 4:
        goto last;
    default:
        return 1;
    }
    return 0;
advance:
    gSceneWork.state = 0;
    gSceneWork.unk4 = 0;
    gMain.step++;
    return 0;
last:
    InitSaveData();
    BuildStarterDeck(gSceneWork.target);
    SaveGame();
    return 1;
}
/* END StarterDeckSelect_Run */
/* BEGIN PackList_FlushVram */
/* Copy slide graphics and tilemaps to VRAM in eight DMA blocks each. */
void PackList_FlushVram(void)
{
    /* FAKEMATCH: retain ROM iterator, DMA temporaries, and constant scheduling. */
    register s32 i __asm__("r4") = 0;
    struct Slide *s = &gSceneWork;
    vu32 *dma = (vu32 *)0x040000D4;
    u32 dst = 0x06004000;
    u32 src = (u32)s + 0x6E;
    do {
        register u32 count __asm__("r0");
        register u32 mask __asm__("r1");
        u32 busy;
        dma[0] = src;
        dma[1] = dst;
        count = 0x80000800;
        dma[2] = count;
        dma[2];
        busy = dma[2];
        mask = 0x80000000;
        if ((s32)busy < 0) {
            do {
                busy = dma[2];
                busy &= mask;
            } while (busy != 0);
        }
        {
            register u32 step __asm__("r0") = 0x1000;
            dst += step;
            src += step;
        }
        i++;
    } while (i <= 7);
    i = 0;
    {
        register vu32 *dma2 __asm__("r3") = (vu32 *)0x040000D4;
        u32 base = 0x0300045C;
        do {
            register u32 count __asm__("r0");
            register u32 mask __asm__("r2");
            u32 busy;
            register u32 off __asm__("r1") = i << 11;
            register u32 vram __asm__("r0");
            register s32 next __asm__("r1");
            dma2[0] = off + base;
            vram = 0x06000000;
            dma2[1] = off + vram;
            count = 0x80000400;
            dma2[2] = count;
            dma2[2];
            busy = dma2[2];
            mask = 0x80000000;
            next = i + 1;
            if ((s32)busy < 0) {
                do {
                    busy = dma2[2];
                    busy &= mask;
                } while (busy != 0);
            }
            i = next;
        } while (i <= 7);
    }
    {
        u32 flags = s->flags;
        flags &= ~1;
        s->flags = flags;
    }
}
/* END PackList_FlushVram */
/* BEGIN PackList_ClearWork */
/* Clear the 0x70-byte slide state at 0x02020310 with DMA3 (same as 0x080643E4). */
void PackList_ClearWork(void) {
    vu16 zero = 0;
    vu32 *dma = (vu32 *)0x040000D4;

    dma[0] = (u32)&zero;
    dma[1] = (u32)&gSceneWork;
    dma[2] = 0x81004038;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
}
/* END PackList_ClearWork */
/* BEGIN PackList_AddPack */
/* Find pack `id` in the pack info table (0x48-byte rows) and append its row index to the list at 0x0202033C (count at 0x0202037C). */
void PackList_AddPack(u32 id) {
    u32 i = 0;
    u16 *cnt = &gUnk_0202037C;
    u16 *list = cnt - 0x20;
    struct PackInfo *row = gPackInfo;
    do {
        if (row->id == id) {
            list[*cnt] = i;
            (*cnt)++;
            break;
        }
        row++;
        i++;
    } while (i <= 0x16);
}
/* END PackList_AddPack */
/* BEGIN PackList_DrawBackground */
extern u8 gUnk_0202037E[];
/* Load the pack-list palette and 3 label graphics via DMA3, then fill the 32x20 tile map: rows a..b-1 use tile c, others c + 1. */
void PackList_DrawBackground(s32 a, s32 b, u16 c) {
    s32 i;
    s32 j;
    s32 next;
    {
        vu32 *dma = (vu32 *)0x040000D4;
        dma[0] = 0x0863CC7C;
        dma[1] = 0x05000000;
        dma[2] = 0x80000100;
        dma[2];
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        while (dma[2] & 0x80000000)
            ;
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        dma[0] = 0x0863CE7C;
        dma[1] = (u32)&gUnk_0202037E[c << 6];
        dma[2] = 0x80000020;
        dma[2];
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        while (dma[2] & 0x80000000)
            ;
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        dma[0] = 0x0863CEBC;
        dma[1] = (u32)&gUnk_0202037E[(c + 1) << 6];
        dma[2] = 0x80000020;
        dma[2];
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        while (dma[2] & 0x80000000)
            ;
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        dma[0] = 0x0863CEFC;
        dma[1] = (u32)&gUnk_0202037E[(c + 2) << 6];
        dma[2] = 0x80000020;
        dma[2];
    }
    {
        vu32 *dma = (vu32 *)0x040000D4;
        while (dma[2] & 0x80000000)
            ;
    }
    for (i = 0; i <= 0x13; i = next) {
        u16 x = i;
        next = i + 1;
        for (j = 0; j <= 0x1F; j++) {
            if (a <= i && i < b)
                gUnk_03001C5C[(u16)j + (x << 5)] = c;
            else
                gUnk_03001C5C[(u16)j + (x << 5)] = c + 1;
        }
    }
}
/* END PackList_DrawBackground */
/* BEGIN GetPackCoverGfx */
/* Cover image of pack `id` from the pack info table, or 0. */
const u8 *GetPackCoverGfx(u16 id) {
    u32 i;
    for (i = 0; i <= 0x16; i++) {
        if (gPackInfo[i].id == id)
            return gPackInfo[i].image;
    }
    return 0;
}
/* END GetPackCoverGfx */
/* BEGIN PackList_UnusedReturnFalse */
u32 PackList_UnusedReturnFalse(void) {
    return 0;
}
/* END PackList_UnusedReturnFalse */
/* BEGIN PackList_SetCoverAlpha */
/* Alpha blend: BLDCNT = 0x442 (OBJ+BG1? 1st target, alpha mode), BLDALPHA = (16 - a) << 8 | a. */
void PackList_SetCoverAlpha(u32 a) {
    REG_BLDCNT = 0x442;
    REG_BLDALPHA = ((0x10 - a) << 8) | a;
}
/* END PackList_SetCoverAlpha */
/* BEGIN PackList_LoadCoverGfx */
/* DMA the cover image of pack `b` into the 64-byte-row pack graphics buffer at slot 0x62 * a + 0x10. */
void PackList_LoadCoverGfx(u16 a, u16 b) {
    const u8 *src = GetPackCoverGfx(b);
    u16 slot = a * 0x62;
    slot += 0x10;
    if (src) {
        /* an SDK-style DMA macro (do { ... } while (0)) */
        do {
            vu32 *dma = (vu32 *)0x040000D4;
            dma[0] = (u32)src;
            dma[1] = 0x0202037E + (slot << 6);
            dma[2] = 0x80000C40;
            dma[2];
            while (dma[2] & 0x80000000)
                ;
        } while (0);
    }
}

/* END PackList_LoadCoverGfx */
/* BEGIN PackList_DrawCoverTiles */
extern u16 gUnk_0300045C[];
/* Fill a 7x14 block of the tile map at 0x0300045C with consecutive tile numbers starting at 0x62 * c + 0x10. */
void PackList_DrawCoverTiles(u32 a, u32 bArg, u32 cArg)
{
    u32 b = (u16)bArg;
    /* FAKEMATCH: retain the ROM's tile-value and inner-loop counter registers. */
    register u32 c __asm__("r4") = (u16)cArg;
    u32 shifted;
    s32 next;
    s32 i;
    register s32 j __asm__("r2");
    u16 t = c * 0x62;
    t += 0x10;
    for (i = 0; i <= 0xD; i = next) {
        u16 *p = (u16 *)((a << 11) + (u32)&gUnk_0300045C + (b << 1));
        b += 0x20;
        next = i + 1;
        j = 6;
        do {
            *p++ = t++;
        } while (--j >= 0);
        shifted = b << 16;
        b = shifted >> 16;
    }
}
/* END PackList_DrawCoverTiles */
/* BEGIN PackList_DrawCovers */
/* helpers for the parked PackList_DrawCovers draft */
struct PackMainMaps { u8 pad0[0xC1C]; u16 maps[4][0x400]; u8 pad2[0x442A - 0x2C1C]; u16 scroll; };
extern struct PackMainMaps packMainMaps __asm__("gMain");
/* Clear the pack-list tile maps, then draw three pack covers (rows sel, sel+1, sel+2 mod count) at
 * tile columns 0x62 + 10*i and highlight the current one; finally set the redraw flag. */
void PackList_DrawCovers(s32 arg) {
    vu16 zero;
    s32 sel;
    s32 i;
    u16 *cnt;
    sel = arg;
    zero = 0;
    { vu32 *dma = (vu32 *)0x040000D4;
      dma[0] = (u32)&zero;
      dma[1] = (u32)packMainMaps.maps[0];
      dma[2] = 0x81000800;
      dma[2];
    }
    { vu32 *dma = (vu32 *)0x040000D4;
      while (dma[2] & 0x80000000) ;
    }
    zero = 0;
    { vu32 *dma = (vu32 *)0x040000D4;
      dma[0] = (u32)&zero;
      dma[1] = (u32)packMainMaps.maps[3];
      dma[2] = 0x81000400;
      dma[2];
    }
    { vu32 *dma = (vu32 *)0x040000D4;
      while (dma[2] & 0x80000000) ;
    }
    packMainMaps.scroll = 0;
    cnt = &gUnk_0202037C;   /* hoisted: the ROM rematerialises it in the loop */
    for (i = 0; i <= 2; i++) {
        PackList_LoadCoverGfx(i, gPackInfo[gSceneWork.list[sel]].id);
        PackList_DrawCoverTiles(1, (u16)(i * 10 + 0x62), (u16)i);
        if (gSceneWork.current == i)
            PackList_DrawCoverTiles(4, (u16)(i * 10 + 0x62), (u16)i);
        sel++;
        sel %= *cnt;
    }
    {
        /* A u32 temporary (not u8 or |=) gives the ROM's register choice for the flag update. */
        u32 f = gSceneWork.flags;
        f |= 1;
        gSceneWork.flags = f;
    }
}
/* END PackList_DrawCovers */
