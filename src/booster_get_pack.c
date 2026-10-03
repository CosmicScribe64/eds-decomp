/*
 * booster_get_pack (0x08063A28-0x08064AE8): the Get-a-pack scene runner and pack list, the New Game
 * starter-deck choice, and the save progress predicates behind the opponent and pack unlocks
 * (wiki/functions/booster-get-pack-c.md).
 *
 * CB_GetPack and GetRewardPack run the gGetPackSteps table indexed by gMain.seqIndex1 (enum GetPackStep
 * in booster.h): the list, generation and reveal steps live in booster_pack.c and card_canvas.c, while
 * GetPack_RestoreScene and GetPack_FadeInAndResume here rebuild the reveal scene around the step-7 card
 * detail. The pack list and the starter-deck screen (StarterDeckSelect_Run, New Game's "select an Initial
 * Deck") share the gSceneWork area (struct PackListWork and struct StarterDeckSelectWork in booster.h) and
 * the PackList_* video and drawing helpers. The rest of the unit is read-only checks over the save mirror:
 * IsCampaignLevel2Unlocked to IsCampaignLevel5Unlocked test the duel records of one league,
 * IsOpponentUnlocked and IsPackUnlocked gate the campaign opponents and the packs, GetCampaignLevel folds
 * the league checks into a tier, and IsCardCollectionComplete tests for a full collection.
 */
#include "global.h"
#include "gba.h"                    /* REG_DISPCNT, REG_BG0CNT to REG_BG3CNT, REG_IE, REG_IME, REG_MOSAIC,
                                     * REG_BLDCNT, REG_BLDALPHA, A_BUTTON, DPAD_LEFT, DPAD_RIGHT */
#include "card_data.h"              /* CARD_NUMBER_ALT_ART; gCardNumberToId is read through CARD_ID_OF below */
#include "constants/game.h"         /* enum DuelistId, enum BoosterPackId */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM */
#include "main.h"                   /* struct Main gMain: newKeys, vblankFlags, vblankCallback, seqIndex1,
                                     * seqState1, seqState2, rewardPack */
#include "sprite.h"                 /* AddSprite */

/* ---- BEGIN header subset (pre-H0) ----
 * The parts of save.h and sound.h this unit uses, with the headers' names and layouts. include/save.h and
 * include/sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md): the legacy sound.h does not declare PlaySE, and save.h's struct
 * DuelRecord declares the win count in a u32 container where the checks below load it from the low
 * halfword, so the record keeps the u16 view. After H0, replace the block (BEGIN to END) with the include
 * lines of save.h and sound.h, in that order (build/readability/issues/booster_get_pack.md). */

/* One card of the collection, gSaveData.trunk[cardId] (4 bytes per card id): the trunk copies plus the
 * copies placed in the three saved decks. */
struct TrunkEntry {
    u16 count:10;                   /* +0x0 bits 0-9: copies in the trunk */
    u16 deckCopies:2;               /* +0x0 bits 10-11: copies in the saved Deck */
    u16 sideCopies:2;               /* +0x0 bits 12-13: copies in the saved Side Deck */
    u16 fusionCopies:2;             /* +0x0 bits 14-15: copies in the saved Fusion Deck */
    u8 flag0:1;                     /* +0x2 bit 0: no reader found */
    u8 passwordUsed:1;              /* +0x2 bit 1: this card's password was redeemed */
    u8 unk2_2:6;                    /* +0x2 bits 2-7 */
    u8 unk3;                        /* +0x3 */
};

/* Win/loss/draw counts against one opponent, gSaveData.duelRecords[DuelistId] (4 bytes). The canonical
 * struct DuelRecord splits one u32 as wins:11, losses:11, draws:10; this view keeps the same fields in
 * u16 containers (see the block comment above). */
struct DuelRecord {
    u16 wins:11;                    /* bits 0-10: duels won against this opponent */
    u16 lossesLow:5;                /* bits 11-15: low half of losses */
    u16 unk2;                       /* +0x02: high half of losses and the draws */
};

/* The save image fields this unit reads. */
struct SaveData {
    u8 unk0[8];                             /* +0x0000 */
    struct TrunkEntry trunk[0x800];         /* +0x0008: the collection, indexed by card id */
    u8 unk2008[0x20D0 - 0x2008];
    struct DuelRecord duelRecords[32];      /* +0x20D0: indexed by enum DuelistId */
    u8 unk2150[0x2162 - 0x2150];
    u8 championshipWins;                    /* +0x2162: National Championship titles (saturating) */
    u8 unk2163[0x2170 - 0x2163];
};

extern struct SaveData gSaveData;           /* 0x02011C20 */

/* Reset gSaveData to a new game (save.h). */
void InitSaveData(void);
/* Write gSaveData to SRAM and verify it (save.h). */
void SaveGame(void);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END header subset ---- */

#include "booster.h"                /* struct PackOpenWork, PackListWork, StarterDeckSelectWork, PackInfo,
                                     * gPackOpenWork, gPackInfo, and the prototypes of the functions
                                     * defined here (plus GetPack_InitScene and the card_canvas.c helpers) */

/* gSceneWork (0x02020310) under the pack-list view; the starter-deck screen uses the second view of the
 * same area. */
extern struct PackListWork gSceneWork;
extern struct StarterDeckSelectWork gStarterDeckSelectWork __asm__("gSceneWork");

/* ---- ROM data used only here ---- */

extern u16 (*const gGetPackSteps[])(void);          /* 0x081A572C: the Get-a-pack steps (enum GetPackStep) */
extern const u16 gUnlockablePackIds[];              /* 0x081A5758: the 27 unlockable packs, in list order */
/* = gCardNumberToId[CARD_TOON_WORLD] (card_data.h): IsOpponentUnlocked reads Toon World's card ID through
 * its own symbol, as the ROM loads it. */
extern const u16 gUnk_08624568[];
/* = gCardIdToNumber[821], the padding entry (card_data.h): IsCardCollectionComplete loads it once as its
 * gate value, as the ROM does. */
extern const u16 gUnk_0862311E[];
extern const u8 gStarterDeckBgImage[];              /* 0x0863CF3C */
extern const u8 gStarterDeckBoxBlackImage[];        /* 0x0863D12C */
extern const u8 gStarterDeckBoxRedImage[];          /* 0x0863E39C */
extern const u8 gStarterDeckBoxGreenImage[];        /* 0x0863F54C */
extern const u8 gHandCursorPal[];                   /* 0x0867793C */
extern const u8 gHandCursorGfx[];                   /* 0x0867797C */
/* = &gSceneWork.packCount: PackList_DrawCovers loads the count through its own symbol, as the ROM does
 * (see the local views below). */
extern u16 gUnk_0202037C;                           /* 0x0202037C */
/* BG tilemap buffer in IWRAM, 32 columns (= &gMain.bgMapBuffer[3][0]; other units read it by this name). */
extern u16 gUnk_03001C5C[];                         /* 0x03001C5C */
/* Tile map at the start of gMain.bgMapBuffer, written by PackList_DrawCoverTiles. */
extern u16 gUnk_0300045C[];                         /* 0x0300045C */

/* The interrupt vector table at the start of IWRAM; entry 1 is the HBlank handler (cleared while the
 * interrupt registers are rewritten, as in booster_pack.c). */
struct IntrTable {
    u32 unk0;                           /* +0x00 */
    void (*hblankCallback)(void);       /* +0x04 */
};
extern struct IntrTable IntrTable;      /* 0x03000000 */

/* ---- Local views kept for matching (build/readability/issues/booster_get_pack.md) ---- */

/* The pack-list tile maps of gMain as one view starting at bgMapBuffer[1]: PackList_DrawCovers derives
 * the second clear address from the first map pointer, which is the form the matched code takes. */
struct PackMainMaps {
    u8 unk0[0xC1C];
    u16 maps[4][0x400];                 /* +0x0C1C: gMain.bgMapBuffer[1] to [4] */
    u8 unk2C1C[0x442A - 0x2C1C];
    u16 bgHofs1;                        /* +0x442A: gMain.bgHofs[1] */
};
extern struct PackMainMaps packMainMaps __asm__("gMain");

/* Matching: the ROM reloads the table base on every lookup, so gCardNumberToId is read through its
 * address instead of the symbol (which the compiler would hoist out of the loop). */
#define CARD_ID_OF(number)  (((const u16 *)0x08623DF4)[(number)])

/* bg.h prototypes LoadBgImageMap1 with four parameters; StarterDeckSelect_DrawBackground calls it with
 * three (no image pointer), so the declaration stays unprototyped here. */
void LoadBgImageMap1();
void ResetVideo(void);                  /* bg.h */
void ResetBgScroll(void);

/* palette.h declares the fades returning u32; the two callers here test the result as a u16, so the
 * prototypes stay u16 (the operands are narrowed at the call). */
void SetBrightnessBlack(void);          /* palette.h */
u16 FadeFromBlack(u32 step);
u16 FadeToBlack(u32 step);

/* libgcc signed modulo, called by the %= in PackList_DrawCovers. */
extern s32 __modsi3(s32 a, s32 b);

/* The unlock predicates defined below, called before their definitions. */
u16 IsCampaignLevel2Unlocked(void);
u16 IsCampaignLevel3Unlocked(void);
u16 IsCampaignLevel4Unlocked(void);
u16 IsCampaignLevel5Unlocked(void);
u16 IsCardCollectionComplete(void);
u16 IsPackUnlocked(u32 packId);

/* Card number to base-card key: alt-art numbers (CARD_NUMBER_ALT_ART + n) take the next ID after card n,
 * and 0xFFFF (no card) is key 0. Static inline, as in the matched code. */
static inline int IdToKey(u16 id)
{
    int key;
    if (id == 0xFFFF)
        key = 0;
    else if (id <= CARD_NUMBER_ALT_ART - 1)
        key = CARD_ID_OF(id & 0x7FF);
    else
        key = CARD_ID_OF((id - CARD_NUMBER_ALT_ART) & 0x7FF) + 1;
    return key;
}

/* Step 8 (GETPACK_STEP_RESTORE_SCENE): rebuild the reveal scene with every card settled, after the card
 * detail. Maps the 5 card numbers of the pack to their base-card keys and draws each row. */
u32 GetPack_RestoreScene(void) {
    s32 i;
    GetPack_InitScene();
    for (i = 0; i <= 4; i++) {
        gPackOpenWork.revealFrame[i] = PACK_REVEAL_DONE;
        GetPack_DrawCardRow(i, (u16)IdToKey(gPackOpenWork.cardNumbers[i]));
    }
    return 1;
}

/* Step 9 (GETPACK_STEP_RESUME): turn all layers on, scroll and draw the cards, and once the fade-in is
 * done go back 5 steps to GETPACK_STEP_INPUT. Always returns 0. */
u32 GetPack_FadeInAndResume(void) {
    REG_DISPCNT |= 0x1F00;
    GetPack_ScrollBg();
    GetPack_DrawCardSprites();
    if (FadeFromBlack(4))
        gMain.seqIndex1 -= 5;
    return 0;
}

/* The "Get a pack" runner behind the debug-menu entry: run gGetPackSteps[gMain.seqIndex1]; a step that
 * returns non-zero advances the sequence. Returns 1 at the NULL end of the table. */
u32 CB_GetPack(void) {
    if (gGetPackSteps[gMain.seqIndex1] != 0) {
        if (gGetPackSteps[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* As CB_GetPack, but first stores the given pack in gMain.rewardPack (a reward pack skips the list). */
u32 GetRewardPack(u32 packId) {
    gMain.rewardPack = packId;
    if (gGetPackSteps[gMain.seqIndex1] != 0) {
        if (gGetPackSteps[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* Returns 1. No callers. */
u32 GetPack_UnusedReturnTrue(void) {
    return 1;
}

/* 1 if the duel records of the first league (opponents 1 to 5) all have more than 1 win. The signed
 * copies are the matched form (an 11-bit bitfield compares unsigned otherwise). */
u16 IsCampaignLevel2Unlocked(void) {
    struct SaveData *s = &gSaveData;
    s32 wins;
    wins = s->duelRecords[1].wins; if (wins <= 1) return 0;
    wins = s->duelRecords[2].wins; if (wins <= 1) return 0;
    wins = s->duelRecords[3].wins; if (wins <= 1) return 0;
    wins = s->duelRecords[4].wins; if (wins <= 1) return 0;
    wins = s->duelRecords[5].wins; if (wins <= 1) return 0;
    return 1;
}

/* Records 6 to 10 all have more than 2 wins. */
u16 IsCampaignLevel3Unlocked(void) {
    struct SaveData *s = &gSaveData;
    s32 wins;
    wins = s->duelRecords[6].wins; if (wins <= 2) return 0;
    wins = s->duelRecords[7].wins; if (wins <= 2) return 0;
    wins = s->duelRecords[8].wins; if (wins <= 2) return 0;
    wins = s->duelRecords[9].wins; if (wins <= 2) return 0;
    wins = s->duelRecords[10].wins; if (wins <= 2) return 0;
    return 1;
}

/* Records 11 to 15 all have more than 3 wins. */
u16 IsCampaignLevel4Unlocked(void) {
    struct SaveData *s = &gSaveData;
    s32 wins;
    wins = s->duelRecords[11].wins; if (wins <= 3) return 0;
    wins = s->duelRecords[12].wins; if (wins <= 3) return 0;
    wins = s->duelRecords[13].wins; if (wins <= 3) return 0;
    wins = s->duelRecords[14].wins; if (wins <= 3) return 0;
    wins = s->duelRecords[15].wins; if (wins <= 3) return 0;
    return 1;
}

/* Records 16 to 20 all have more than 4 wins. */
u16 IsCampaignLevel5Unlocked(void) {
    struct SaveData *s = &gSaveData;
    s32 wins;
    wins = s->duelRecords[16].wins; if (wins <= 4) return 0;
    wins = s->duelRecords[17].wins; if (wins <= 4) return 0;
    wins = s->duelRecords[18].wins; if (wins <= 4) return 0;
    wins = s->duelRecords[19].wins; if (wins <= 4) return 0;
    wins = s->duelRecords[20].wins; if (wins <= 4) return 0;
    return 1;
}

/* 1 if every counted card is owned: over card ids 1 to 0x334 (820 cards), an id counts when the gate
 * value is a non-token number, and owned means trunk copies, Deck copies or Side Deck copies; a copy
 * only in the Fusion Deck does not count. */
u16 IsCardCollectionComplete(void)
{
    s32 have = 0;
    /* Matching: retain ROM counter registers and rematerialized loop bound. */
    register s32 total __asm__("r4") = 0;
    register s32 id __asm__("r3") = 1;
    u16 key = gUnk_0862311E[0];
    u32 limit = 0x77F;
    struct SaveData *s = &gSaveData;
    struct TrunkEntry *e = &s->trunk[1];
    register s32 bound __asm__("r0");
    do {
        if (key <= limit) {
            u32 v;
            total++;
            /* Matching: the ROM tests the fields with shifts, not bitfield masks: count != 0, then the
             * Deck and Side Deck copies out of byte +1. */
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

/* Unlock condition for opponent `id` (enum DuelistId, 1 to 24); returns 1 when fulfilled. */
u16 IsOpponentUnlocked(u16 id) {
    switch (id) {
    case DUELIST_REX:
    case DUELIST_ESPA_ROBA:
    case DUELIST_WEEVIL:
    case DUELIST_MAKO:
    case DUELIST_MAI:
        return IsCampaignLevel2Unlocked();
    case DUELIST_RARE_HUNTER:
    case DUELIST_ARKANA:
    case DUELIST_STRINGS:
    case DUELIST_UMBRA_LUMIS:
    case DUELIST_MARIK:
        return IsCampaignLevel3Unlocked();
    case DUELIST_KAIBA:
    case DUELIST_ISHIZU:
    case DUELIST_SHADI:
    case DUELIST_YAMI_BAKURA:
    case DUELIST_YAMI_YUGI:
        return IsCampaignLevel4Unlocked();
    case DUELIST_SIMON: {
        u32 r = 0;
        if (gSaveData.championshipWins > 1)
            r = 1;
        return r;
    }
    case DUELIST_PEGASUS: {
        /* Toon World owned, the same trunk test as IsCardCollectionComplete. */
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
    case DUELIST_YUGI:
    case DUELIST_TEA:
    case DUELIST_JOEY:
    case DUELIST_TRISTAN:
    case DUELIST_BAKURA:
        return 1;
    case DUELIST_GRANDPA:
        return IsCardCollectionComplete();
    case DUELIST_DUEL_COMPUTER:
        return IsCampaignLevel5Unlocked();
    default:
        return 0;
    }
}

/* The campaign tier (1 to 5): the highest of the league checks that passes. */
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

/* Whether pack `packId` (enum BoosterPackId) is unlocked, from the duel records: a league predicate, one
 * league's wins summed over 9, every win count of a league over 9, one record over 19, or Simon's record
 * nonzero. The case bodies stay in ROM order (the switch layout follows the source order). */
u16 IsPackUnlocked(u32 packId)
{
    u16 id = packId;
    switch (id) {
    case PACK_VOL_1:
    case PACK_VOL_2:
    case PACK_VOL_3:
        return 1;
    case PACK_EXPERT_2:
        return IsCampaignLevel2Unlocked();
    case PACK_EXPERT_4:
        return IsCampaignLevel3Unlocked();
    case PACK_PREMIUM_3:
        return IsCampaignLevel4Unlocked();
    case PACK_EXPERT_1: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        int sum = s->duelRecords[1].wins + s->duelRecords[2].wins + s->duelRecords[3].wins + s->duelRecords[4].wins + s->duelRecords[5].wins;
        if (sum > 9)
            result = 1;
        return result;
    }
    case PACK_VOL_4: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[1].wins) > 9 && (value = s->duelRecords[2].wins) > 9 && (value = s->duelRecords[3].wins) > 9 && (value = s->duelRecords[4].wins) > 9 && (value = s->duelRecords[5].wins) > 9)
            result = 1;
        return result;
    }
    case PACK_VOL_5: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        int sum = s->duelRecords[6].wins + s->duelRecords[7].wins + s->duelRecords[8].wins + s->duelRecords[9].wins + s->duelRecords[10].wins;
        if (sum > 9)
            result = 1;
        return result;
    }
    case PACK_VOL_6: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[6].wins) > 9 && (value = s->duelRecords[7].wins) > 9 && (value = s->duelRecords[8].wins) > 9 && (value = s->duelRecords[9].wins) > 9 && (value = s->duelRecords[10].wins) > 9)
            result = 1;
        return result;
    }
    case PACK_MAGIC_RULER: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        int sum = s->duelRecords[11].wins + s->duelRecords[12].wins + s->duelRecords[13].wins + s->duelRecords[14].wins + s->duelRecords[15].wins;
        if (sum > 9)
            result = 1;
        return result;
    }
    case PACK_CELEMONY: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[11].wins) > 9 && (value = s->duelRecords[12].wins) > 9 && (value = s->duelRecords[13].wins) > 9 && (value = s->duelRecords[14].wins) > 9 && (value = s->duelRecords[15].wins) > 9)
            result = 1;
        return result;
    }
    case PACK_DUELIST_PACK: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        int sum = s->duelRecords[16].wins + s->duelRecords[17].wins + s->duelRecords[18].wins + s->duelRecords[19].wins + s->duelRecords[20].wins;
        if (sum > 9)
            result = 1;
        return result;
    }
    case PACK_THE_FINAL_DUELIST: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[16].wins) > 9 && (value = s->duelRecords[17].wins) > 9 && (value = s->duelRecords[18].wins) > 9 && (value = s->duelRecords[19].wins) > 9 && (value = s->duelRecords[20].wins) > 9)
            result = 1;
        return result;
    }
    case PACK_LOB_EWD: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[1].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_PHANTOM_OF_G: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[3].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_EXPERT_3: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[9].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_VOL_7: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[10].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_EXPERT_5: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[14].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_PHARAOHS_SERVANT: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[15].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_RARE_SELECTIONS: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[16].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_CURSE_OF_ANUBIS: {
        int result = 0;
        struct SaveData *s = &gSaveData;
        s32 value;
        if ((value = s->duelRecords[20].wins) > 19)
            result = 1;
        return result;
    }
    case PACK_LIMITED_COLLECTION: {
        int value = gSaveData.duelRecords[DUELIST_SIMON].wins;
        if (value != 0)
            value = 1;
        return value;
    }
    default:
        return 0;
    }
}

/* Run the unlock check for each of the 27 entries in gUnlockablePackIds and add the ones that pass. */
void PackList_AddUnlockedPacks(void) {
    u32 i;
    const u16 *p;
    for (i = 0, p = gUnlockablePackIds; i <= 0x1A; p++, i++) {
        if (IsPackUnlocked(*p))
            PackList_AddPack(*p);
    }
}

/* Video setup shared by the pack list and the starter-deck screen: mode 0 with the BG map assignments
 * of the list, then blank the screen, reset the scrolls and take the HBlank interrupt off (twice, around
 * clearing its handler). */
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
    gMain.vblankCallback = 0;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    {
        struct IntrTable *irq = &IntrTable;
        irq->hblankCallback = 0;
    }
    REG_IME = 1;
}

/* Load the deck-select background image at map cell 0xC80, then fill rows rowStart..rowEnd-1 of the
 * 32-column tile map buffer with tileBase / 2. The image argument is not passed on to the loader. */
void StarterDeckSelect_DrawBackground(s32 rowStart, s32 rowEnd, u16 tileBase, const void *image) {
    s32 i;
    s32 j;
    LoadBgImageMap1(0xC80, 0, tileBase);
    for (i = rowStart; i < rowEnd; ) {
        u16 x;
        j = 0;
        x = i;
        i++;
        for (; j <= 0x1F; j++)
            gUnk_03001C5C[(u16)j + (x << 5)] = tileBase >> 1;
    }
}

/* Draw the hand cursor sprite at the slot cursorSlot is sliding to: slots are 80 pixels wide and the
 * slide runs 4 frames per slot. */
void StarterDeckSelect_DrawCursor(void) {
    s32 frames;
    s32 pos;
    if (gStarterDeckSelectWork.cursorSlot != gStarterDeckSelectWork.choice && gStarterDeckSelectWork.slideFrames == 0)
        gStarterDeckSelectWork.slideFrames = 4;
    frames = gStarterDeckSelectWork.slideFrames;
    if (frames > 0) {
        s32 from = gStarterDeckSelectWork.cursorSlot * 80;
        s32 to = gStarterDeckSelectWork.choice * 80;
        s32 base = to + 0x20;
        s32 d = (to - from) * frames;
        pos = base - d / 4;
        gStarterDeckSelectWork.slideFrames = frames - 1;
        if (gStarterDeckSelectWork.slideFrames == 0)
            gStarterDeckSelectWork.cursorSlot = gStarterDeckSelectWork.choice;
    } else {
        pos = gStarterDeckSelectWork.choice * 80 + 0x20;
    }
    AddSprite(pos | 0x580000, 0x80, 0x100);
}

/* Zero the 0x8070-byte starter-deck work area in gSceneWork with a DMA3 fill. */
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

/* Step 1 (StarterDeckInitState): state 0 places the cursor on the middle deck (STARTER_DECK_RED) and sets
 * up the video, state 1 loads the background, the three deck-box images and the cursor graphics, and
 * state 2 turns the layers on and fades in. */
u16 StarterDeckSelect_Init(void) {
    u32 state = gStarterDeckSelectWork.state;
    switch (state) {
    case STARTERDECK_INIT_VIDEO:
        gStarterDeckSelectWork.choice = 1;
        gStarterDeckSelectWork.cursorSlot = 1;
        gStarterDeckSelectWork.slideFrames = state;
        PackList_InitVideo();
        gMain.vblankFlags |= 2;
        gStarterDeckSelectWork.state++;
        return 0;
    case STARTERDECK_INIT_GFX:
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
        gStarterDeckSelectWork.state++;
        return 0;
    default:
        REG_DISPCNT |= 0x1640;
        StarterDeckSelect_DrawCursor();
        return FadeFromBlack(2);
    }
}

/* Step 2: while the cursor is settled, A confirms (SE_CONFIRM, returns 1) and LEFT/RIGHT move the choice
 * over the three decks (SE_CURSOR). */
u16 StarterDeckSelect_HandleInput(void) {
    StarterDeckSelect_DrawCursor();
    if (gStarterDeckSelectWork.slideFrames == 0) {
        if (gMain.newKeys & A_BUTTON) {
            PlaySE(SE_CONFIRM);
            return 1;
        }
        if (gMain.newKeys & DPAD_LEFT) {
            if (gStarterDeckSelectWork.choice > 0) {
                PlaySE(SE_CURSOR);
                gStarterDeckSelectWork.choice--;
            }
        }
        if (gMain.newKeys & DPAD_RIGHT) {
            if (gStarterDeckSelectWork.choice <= 1) {
                PlaySE(SE_CURSOR);
                gStarterDeckSelectWork.choice++;
            }
        }
    }
    return 0;
}

/* Step 3: blink the cursor for 60 frames (drawn every other group of 4), then fade to black and hide the
 * layers; returns 1 once faded. */
u16 StarterDeckSelect_FadeOut(void) {
    if (gStarterDeckSelectWork.state <= 0x3B) {
        if ((gStarterDeckSelectWork.state >> 2) & 1)
            StarterDeckSelect_DrawCursor();
        gStarterDeckSelectWork.state++;
        return 0;
    }
    StarterDeckSelect_DrawCursor();
    if (FadeToBlack(2)) {
        REG_DISPCNT &= 0xEEFF;
        return 1;
    }
    return 0;
}

/* The starter-deck screen (enum StarterDeckSelectStep on gMain.seqIndex1). The last step builds the
 * chosen deck and starts the new game; returns 1 then and for unknown steps. */
u16 StarterDeckSelect_Run(void) {
    switch (gMain.seqIndex1) {
    case STARTERDECK_STEP_CLEAR:
        StarterDeckSelect_ClearWork();
        goto advance;
    case STARTERDECK_STEP_INIT:
        if (StarterDeckSelect_Init())
            goto advance;
        break;
    case STARTERDECK_STEP_INPUT:
        if (StarterDeckSelect_HandleInput())
            goto advance;
        break;
    case STARTERDECK_STEP_FADE_OUT:
        if (StarterDeckSelect_FadeOut())
            goto advance;
        break;
    case STARTERDECK_STEP_FINISH:
        goto last;
    default:
        return 1;
    }
    return 0;
advance:
    gStarterDeckSelectWork.state = 0;
    gStarterDeckSelectWork.unused4 = 0;
    gMain.seqIndex1++;
    return 0;
last:
    InitSaveData();
    BuildStarterDeck(gStarterDeckSelectWork.choice);
    SaveGame();
    return 1;
}

/* Copy the cover tile buffer to BG char block 1 and the BG map buffers to VRAM, eight DMA blocks each,
 * then clear the redraw flag. */
void PackList_FlushVram(void)
{
    /* Matching: retain ROM iterator, DMA temporaries, and constant scheduling. */
    register s32 i __asm__("r4") = 0;
    struct PackListWork *s = &gSceneWork;
    vu32 *dma = (vu32 *)0x040000D4;
    u32 dst = 0x06004000;
    u32 src = (u32)s->bgTiles;
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

/* Zero the 0x8070-byte pack-list work area in gSceneWork with a DMA3 fill (same code as
 * StarterDeckSelect_ClearWork). */
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

/* Append the gPackInfo row of pack `id` to the pack list (packRows, counted by packCount); nothing for
 * an unknown id. */
void PackList_AddPack(u32 id) {
    u32 i = 0;
    u16 *cnt = &gSceneWork.packCount;
    u16 *list = cnt - 0x20;
    const struct PackInfo *row = gPackInfo;
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

/* Load gPackListPal and the three background tiles tile..tile + 2 into the tile buffer by DMA3, then
 * fill the 32x20 tile map: rows rowStart..rowEnd-1 take tile, the others tile + 1. The DMA sources stay
 * integer addresses and the scopes stay separate: both are the matched form. */
void PackList_DrawBackground(s32 rowStart, s32 rowEnd, u16 tile) {
    s32 i;
    s32 j;
    s32 next;
    {
        vu32 *dma = (vu32 *)0x040000D4;
        dma[0] = 0x0863CC7C; /* gPackListPal */
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
        dma[0] = 0x0863CE7C; /* gPackListBgTiles */
        dma[1] = (u32)&gSceneWork.bgTiles[tile << 6];
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
        dma[0] = 0x0863CEBC; /* gPackListBgTiles + 0x40 */
        dma[1] = (u32)&gSceneWork.bgTiles[(tile + 1) << 6];
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
        dma[0] = 0x0863CEFC; /* gPackListBgTiles + 0x80 */
        dma[1] = (u32)&gSceneWork.bgTiles[(tile + 2) << 6];
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
            if (rowStart <= i && i < rowEnd)
                gUnk_03001C5C[(u16)j + (x << 5)] = tile;
            else
                gUnk_03001C5C[(u16)j + (x << 5)] = tile + 1;
        }
    }
}

/* The cover graphics of pack `id` from the pack info table, or NULL for an unknown id. */
const u8 *GetPackCoverGfx(u16 id) {
    u32 i;
    for (i = 0; i <= 0x16; i++) {
        if (gPackInfo[i].id == id)
            return gPackInfo[i].coverGfx;
    }
    return 0;
}

/* Returns 0. No callers. */
u32 PackList_UnusedReturnFalse(void) {
    return 0;
}

/* Blend the covers over the background at eva/16: BLDCNT 0x442 (alpha mode), BLDALPHA eva with the
 * complementary (16 - eva) as the other coefficient. */
void PackList_SetCoverAlpha(u32 eva) {
    REG_BLDCNT = 0x442;
    REG_BLDALPHA = ((0x10 - eva) << 8) | eva;
}

/* Copy the cover of pack `packId` into the tile buffer at slot `slot` (tile 0x10 + slot * 0x62). */
void PackList_LoadCoverGfx(u16 slot, u16 packId) {
    const u8 *src = GetPackCoverGfx(packId);
    u16 tile = slot * 0x62;
    tile += 0x10;
    if (src) {
        /* an SDK-style DMA macro (do { ... } while (0)) */
        do {
            vu32 *dma = (vu32 *)0x040000D4;
            dma[0] = (u32)src;
            dma[1] = 0x0202037E + (tile << 6); /* gSceneWork.bgTiles */
            dma[2] = 0x80000C40;
            dma[2];
            while (dma[2] & 0x80000000)
                ;
        } while (0);
    }
}

/* Fill a 7-wide, 14-tall block of the tile map buffer at mapPos with the consecutive tile numbers of
 * slot cArg (from tile 0x10 + slot * 0x62). */
void PackList_DrawCoverTiles(u32 a, u32 bArg, u32 cArg)
{
    u32 b = (u16)bArg;
    /* Matching: retain the ROM's tile-value and inner-loop counter registers. */
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

/* Clear the cover tile maps, then draw the covers of list entries first..first+2 (mod the count) at
 * tile columns 0x62 + 10*i, highlight the centre slot, and set the redraw flag for PackList_FlushVram. */
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
    packMainMaps.bgHofs1 = 0;
    cnt = &gUnk_0202037C;   /* hoisted: the ROM rematerialises it in the loop */
    for (i = 0; i <= 2; i++) {
        PackList_LoadCoverGfx(i, gPackInfo[gSceneWork.packRows[sel]].id);
        PackList_DrawCoverTiles(1, (u16)(i * 10 + 0x62), (u16)i);
        if (gSceneWork.centerSlot == i)
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
