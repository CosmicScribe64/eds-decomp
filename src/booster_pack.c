/*
 * booster_pack (0x080629F0-0x08063A27): the booster pack generator and the first half of the
 * Get-a-pack opening scene (wiki/functions/booster-pack-c.md).
 *
 * GeneratePackCards() builds the five card numbers of a pack: one from a rarity-rolled slot
 * and four from a shuffled copy of the commons slot (the three random packs draw straight
 * from the whole card pool instead). The remaining functions are steps 0-5 and 7 of the
 * Get-a-pack sequence (enum GetPackStep in booster.h): select the pack on the scrolling list
 * and generate its cards, load the scene graphics, fade in, flip the five cards face up,
 * move the hand cursor over them, fade out and add the cards to the trunk, and show one
 * card in the detail viewer. booster_get_pack.c runs the scene and draws the cards;
 * deck_edit_panel.c runs the pack list steps.
 */
#include "global.h"
#include "card_data.h"            /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_NUMBER_TOKEN_FIRST/END, CARD_ID_COUNT, CARD_STATS_*, gCardIconPal, gCardIconNormalGfx/Effect/Fusion/Ritual/Trap/MagicGfx */
#include "constants/card_stats.h" /* enum CardType, CARD_STATS_DEF_MASK, CARD_STATS_POINTS_SCALE */
#include "constants/game.h"       /* enum BoosterPackId: PACK_RANDOM_TRAP, PACK_RANDOM_MAGIC, PACK_RANDOM_ANY */
#include "constants/sound.h"      /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR, SE_CARD_FLIP */
#include "gba.h"                  /* REG_DISPCNT, REG_BG0CNT..REG_BG3CNT, REG_MOSAIC, REG_IE, REG_IME, OBJ_PLTT, BG_PLTT, VRAM, OBJ_VRAM0, A_BUTTON, B_BUTTON, DPAD_* */
#include "main.h"                 /* struct Main, gMain */
#include "booster.h"              /* struct PackSlots, struct PackOpenWork, struct PackInfo, struct PackContentsEntry, struct PackListWork, gPackOpenWork, gPackInfo, enum GetPackDetailState, enum PackRevealFrame, enum PackCursorAnim */
#include "card_detail.h"          /* struct CardDetail, gCardDetail, CardDetail_Reset, CardDetail_InitVideo, CardDetail_DrawCard */
#include "save.h"                 /* struct SaveData, gSaveData, SaveGame, AddCardToTrunk */
#include "util.h"                 /* Random, MemClear16, MemCopy16 */
#include "bg.h"                   /* ResetVideo, ClearBgMapBuffers, LoadSystemGfx */
#include "palette.h"              /* SetBrightnessBlack, FadeFromBlack, FadeToBlack */

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);

/* Not declared by bg.h; the scene units declare it locally (as in card_detail.c). */
void ResetBgScroll(void);

/*
 * Before H0 (build/readability/HEADERS.md) the legacy include/gba.h lacks the interrupt
 * names below. The new headers define them with the same values, so this block compiles
 * away with them. Delete it once the new headers are installed.
 */
#ifndef INTR_FLAG_HBLANK
#define INTR_FLAG_HBLANK 0x0002 /* REG_IE bit: HBlank interrupt enable */
#define INTR_SLOT_HBLANK 1      /* IntrTable slot of the HBlank handler */
extern void (*IntrTable[16])(void);
#endif

/* -------------------------------------------------------------------------- */
/* Local views kept for matching                                              */
/* -------------------------------------------------------------------------- */
/*
 * booster.h declares the pack-list steps (defined in deck_edit_panel.c) and card_detail.h
 * the detail steps with u16 returns. This unit calls them through int returns and tests
 * the results as (value << 16) != 0, which only reproduces the ROM if the upper half of
 * the returned register is treated as unknown, so the calls go through these aliased
 * views. At H0, delete the views and call the headers' declarations directly
 * (build/readability/issues/booster_pack.md).
 */
extern int PackList_InitInt(void) asm("PackList_Init");
extern int PackList_HandleInputInt(void) asm("PackList_HandleInput");
extern int PackList_FadeOutInt(void) asm("PackList_FadeOut");
extern int CardDetail_FadeInInt(void) asm("CardDetail_FadeIn");
extern int CardDetail_HandleInputInt(void) asm("CardDetail_HandleInput");
extern int CardDetail_FadeOutInt(void) asm("CardDetail_FadeOut");

/* Matching: the fade-out calls GetPack_DrawCardSprites with (5, -1, 0) left in r0-r2 even
 * though the callee ignores its arguments (booster.h declares the (void) prototype of the
 * definition in booster_get_pack.c), so this call needs a 3-argument view. */
extern void GetPack_DrawCardSpritesArgs(int a, int b, int c) asm("GetPack_DrawCardSprites");

/*
 * Trunk entry views for IsCardNumberOwned. The entry word for a card sits at gSaveData +
 * key*4 + 8 (the trunk array starts 8 bytes into the save block), so the views are based
 * 8 bytes early: the copy count is read as a halfword field, the per-deck counts as fields
 * of the second byte (struct TrunkEntry in save.h is the same entry at its true base). The
 * counts go through separate getters because adjacent bitfield tests would otherwise be
 * merged into a single load.
 */
struct TrunkEntryView {
    u8 unk0[8];
    u16 count:10;       /* bits 0-9: copies in the trunk */
    u16 unk10:6;
    u8 unk[4];
};
struct TrunkCopiesView {
    u8 unk0[9];
    u8 unk9_0:2;
    u8 deckCopies:2;    /* bits 2-3: copies in the saved Deck */
    u8 sideCopies:2;    /* bits 4-5: copies in the saved Side Deck */
    u8 fusionCopies:2;  /* bits 6-7: copies in the saved Fusion Deck */
    u8 unk[3];
};

static inline int TrunkCount(struct TrunkEntryView *entry)
{
    return entry->count;
}
static inline int TrunkDeckCopies(struct TrunkEntryView *entry)
{
    return ((struct TrunkCopiesView *)entry)->deckCopies;
}
static inline int TrunkSideCopies(struct TrunkEntryView *entry)
{
    return ((struct TrunkCopiesView *)entry)->sideCopies;
}
static inline int TrunkFusionCopies(struct TrunkEntryView *entry)
{
    return ((struct TrunkCopiesView *)entry)->fusionCopies;
}

/*
 * Tail view of gPackOpenWork for the two zero stores in GetPack_InitScene: the ROM stores
 * the BG3 scroll offsets (+0x116/+0x118) through u16 bitfields, which makes the compiler
 * emit the shared SImode zero that the reveal-frame byte loop after them reuses. Plain
 * u16 fields (as in struct PackOpenWork) would store differently.
 */
struct PackBgScrollView {
    u8 pad[0x116];
    u16 bgScrollX:16;   /* +0x116 */
    u16 bgScrollY:16;   /* +0x118 */
};

/* Card table lookups. The tables are addressed by their ROM literals on purpose: going
 * through the gCardStats[] symbols makes the compiler load different literal pools here. */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats[id] */
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS_WORD(id))                /* enum CardType */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber[id] */
#define CARD_ID_OF(number)  (((const u16 *)0x08623DF4)[(number)])               /* gCardNumberToId[number] */

/* Slot k of a pack's slot table; the ROM walks the table with byte arithmetic. */
#define PACK_SLOT(p, k) ((struct PackSlot *)((k) * 8 + (u32)(p)))

/* The card ID of a card number, or 0 for the empty number (0xFFFF). Numbers from
 * CARD_NUMBER_ALT_ART (2000) up are alternate arts and use the next ID after their
 * printed card. */
static inline int CardNumberToId(u16 number)
{
    int cardId;
    if (number == 0xFFFF)
        cardId = 0;
    else if (number <= CARD_NUMBER_TOKEN_END - 1)
        cardId = CARD_ID_OF(number & CARD_ID_MASK);
    else
        cardId = CARD_ID_OF((number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK) + 1;
    return cardId;
}

/* -------------------------------------------------------------------------- */
/* ROM data used only here                                                    */
/* -------------------------------------------------------------------------- */

extern const struct PackContentsEntry gPackContents[28]; /* 0x081A562C: each pack's slot table */
extern const s32 gPackRarityThresholds[8];               /* 0x081A570C: rarity roll cut-offs */
extern const u16 gPackCursorSlideOffsets[8];             /* 0x0808658C: cursor slide speed by distance */

/* The pack list's work area, viewed as the list state (same address as gSceneWork). */
extern struct PackListWork gSceneWork;                   /* 0x02020310 */

/* Picture banks for the opening scene and the card icons. */
extern const u8 gHandCursorPal[];         /* 0x0867793C */
extern const u8 gHandCursorGfx[];         /* 0x0867797C */
extern const u8 gUnk_0867817C[];          /* 0x0867817C: card back graphic (hypothesis) */
extern const u8 gPackSceneBgPal[];        /* 0x0863CA9C */
extern const u8 gPackSceneBgTiles[];      /* 0x0863CABC */
extern const u8 gPackCursorFramePal[];    /* 0x0863CB3C */
extern const u8 gPackCursorFrameTiles[];  /* 0x0863CB5C */

/*
 * GetPackCommonSlot
 *
 * Index of the highest non-empty slot of the pack, searching 7 down to 1 (the commons
 * slot); 0 if none. Slot 0 is skipped: it is the rarest slot.
 */
int GetPackCommonSlot(struct PackSlots *pack)
{
    int i;
    struct PackSlot *slot;
    i = 7;
    slot = &pack->slot[7];
    for (; i > 0; slot--, i--) {
        if (slot->count > 0)
            return i;
    }
    return 0;
}

/*
 * RollPackRarity
 *
 * Rolls the rarity slot of the pack's first card. The roll is Random() % 180, or % 270
 * when buying the same pack as last time (gSaveData.lastPackId); when packs in a row
 * fell back to the commons slot (gSaveData.packPityCount > 5, or > 10 on a re-buy) the
 * roll becomes % 12, which lands on a high slot. The first slot (0..6) whose cumulative
 * threshold at 0x081A570C beats the roll wins; landing on slot 0-4 of the common slot
 * clears the pity counter.
 */
int RollPackRarity(struct PackSlots *pack, u16 packId)
{
    int roll = Random() % 180;
    int i;
    struct PackSlot *slot;
    const s32 *thresholds;
    u16 *pityCount;
    if (gSaveData.lastPackId == packId)
        roll = Random() % 270;
    if (gSaveData.packPityCount > 5 && (gSaveData.lastPackId != packId || gSaveData.packPityCount > 10)) {
        roll = Random() % 12;
        gSaveData.packPityCount = 0;
    }
    gSaveData.lastPackId = packId;
    i = 0;
    pityCount = &gSaveData.packPityCount;
    for (slot = pack->slot, thresholds = gPackRarityThresholds; i <= 6; slot++, thresholds++, i++) {
        if (roll < *thresholds && slot->count > 0) {
            if (i <= 4 && i < GetPackCommonSlot(pack))
                *pityCount = 0;
            return i;
        }
    }
    gSaveData.packPityCount++;
    return GetPackCommonSlot(pack);
}

/*
 * PickPackSlotCard
 *
 * A random card number from slot `slot` of the pack.
 */
u16 PickPackSlotCard(struct PackSlots *pack, int slot)
{
    struct PackSlot *entry = PACK_SLOT(pack, slot);
    s32 count = entry->count;
    return entry->cards[Random() % count];
}

/* One random card of a special pack into cardNumbers[i]: not a token number
 * (CARD_NUMBER_TOKEN_FIRST..), of the wanted type when TYPECHECK, and not a duplicate of
 * the previous picks. valid, cardId and j are per-case variables (block scope): shared
 * ones change the global-alloc priorities. */
#define PACK_PICK_RANDOM(TYPECHECK)                                                  \
    {                                                                                \
        int valid;                                                                   \
        u16 cardId;                                                                  \
        for (i = 0; i <= 4; i++) {                                                   \
            retry = 1;                                                               \
            do {                                                                     \
                valid = 0;                                                           \
                cardId = Random() % CARD_ID_COUNT;                                   \
                if ((u16)(CARD_NUMBER(cardId) - CARD_NUMBER_TOKEN_FIRST) > (CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST - 1) TYPECHECK) { \
                    int j;                                                           \
                    valid = 1;                                                       \
                    for (j = 0; j < i; j++) {                                        \
                        if (CARD_NUMBER(cardNumbers[j]) == CARD_NUMBER(cardId))      \
                            valid = 0;                                               \
                    }                                                                \
                }                                                                    \
                if (valid) {                                                         \
                    retry = 0;                                                       \
                    cardNumbers[i] = CARD_NUMBER(cardId);                            \
                }                                                                    \
            } while (retry);                                                         \
        }                                                                            \
    }

/*
 * GeneratePackCards
 *
 * Fills cardNumbers[0..4] with the five card numbers of a booster pack, in shuffled
 * order; returns the rolled rarity slot, or -1 for the random packs
 * (PACK_RANDOM_TRAP/MAGIC/ANY) and unknown ids. One card comes from the rolled slot,
 * the other four from a shuffled copy of the commons slot (gPackOpenWork.commonPool).
 */
int GeneratePackCards(u16 *cardNumbers, u16 packId)
{
    /* FAKEMATCH: one variable is both the special packs' retry flag and the pack pointer; as two
     * variables, cardNumbers takes r9 instead of sl. */
    int retry = 0;
#define pack ((struct PackSlots *)retry)
    int i;
    int rolledSlot;
    int commonSlot;
    int poolIndex;

    /* Each special case has its own tail; cross-jumping merges them after reload, which keeps the
     * reload-register rotation of the ROM. */
    switch (packId) {
    case PACK_RANDOM_ANY:
        PACK_PICK_RANDOM()
        gPackOpenWork.rareCardNumber = 9999;
        return -1;
    case PACK_RANDOM_TRAP:
        PACK_PICK_RANDOM(&& CARD_TYPE(cardId) == CARD_TYPE_TRAP)
        gPackOpenWork.rareCardNumber = 9999;
        return -1;
    case PACK_RANDOM_MAGIC:
        PACK_PICK_RANDOM(&& CARD_TYPE(cardId) == CARD_TYPE_MAGIC)
        gPackOpenWork.rareCardNumber = 9999;
        return -1;
    }

    for (i = 0; i < ARRAY_COUNT(gPackContents); i++) {
        if (gPackContents[i].packId == packId)
            retry = (int)gPackContents[i].slots;
    }
    if (pack == 0)
        return -1;
    rolledSlot = RollPackRarity(pack, packId);
    commonSlot = GetPackCommonSlot(pack);
    cardNumbers[0] = PickPackSlotCard(pack, rolledSlot);
    for (i = 0; i < PACK_SLOT(pack, commonSlot)->count; i++)
        gPackOpenWork.commonPool[i] = PACK_SLOT(pack, commonSlot)->cards[i];
    for (i = 0; i < PACK_SLOT(pack, commonSlot)->count * 2; i++) {
        int swapA = Random() % PACK_SLOT(pack, commonSlot)->count;
        int swapB = Random() % PACK_SLOT(pack, commonSlot)->count;
        u16 swapCard = gPackOpenWork.commonPool[swapA];
        gPackOpenWork.commonPool[swapA] = gPackOpenWork.commonPool[swapB];
        gPackOpenWork.commonPool[swapB] = swapCard;
    }
    poolIndex = 0;
    for (i = 1; i < 5; i++) {
        while (gPackOpenWork.commonPool[poolIndex] == cardNumbers[0]) {
            poolIndex++;
            poolIndex %= PACK_SLOT(pack, commonSlot)->count;
        }
        cardNumbers[i] = gPackOpenWork.commonPool[poolIndex];
        poolIndex++;
        poolIndex %= PACK_SLOT(pack, commonSlot)->count;
    }
    if (rolledSlot != commonSlot)
        gPackOpenWork.rareCardNumber = cardNumbers[0];
    for (i = 0; i < 25; i++) {
        int swapA = Random() % 5;
        int swapB = Random() % 5;
        u16 swapCard = cardNumbers[swapA];
        cardNumbers[swapA] = cardNumbers[swapB];
        cardNumbers[swapB] = swapCard;
    }
    return rolledSlot;
#undef pack
}

/*
 * IsCardNumberOwned
 *
 * Returns 1 if any copy of the card with that number is owned (trunk, deck, side or
 * fusion list).
 */
u8 IsCardNumberOwned(u16 cardNumber)
{
    struct TrunkEntryView *entry;
    struct SaveData *save;
    u32 keyOffset;

    /* Split the key scaling so the save-base load sits between the two shifts. */
    keyOffset = (u32)(u16)CardNumberToId(cardNumber) << 16;
    save = &gSaveData;
    entry = (struct TrunkEntryView *)((keyOffset >> 14) + (u32)save);
    if (TrunkCount(entry) == 0 && TrunkDeckCopies(entry) == 0 && TrunkSideCopies(entry) == 0 && TrunkFusionCopies(entry) == 0)
        return 0;
    return 1;
}

/*
 * GetPack_SelectAndGenerate
 *
 * Step 0 of the Get-a-pack sequence, on the sub-state at gMain.seqState1: the pack list
 * (or, for a reward pack id in gMain.rewardPack, straight to generation), then
 * GeneratePackCards and SaveGame. Sub-states 1-3 are the pack list steps of
 * deck_edit_panel.c.
 */
int GetPack_SelectAndGenerate(void)
{
    u16 *index;
    const struct PackInfo *packInfo;
    int result;
    u16 packId;
    u16 *cardNumbers;
    u8 *state;
    struct Main *main = &gMain;
    state = &main->seqState1;
    /* Keep the initialized command cursor live across the state calls. */
    __asm__ __volatile__("" : : "r"(state));
    switch (*state) {
    case 0:
        PackList_ClearWork();
        if (main->rewardPack != 0) {
            GeneratePackCards(gPackOpenWork.cardNumbers, main->rewardPack);
            SaveGame();
            return 1;
        }
        PackList_AddUnlockedPacks();
        goto next;
    case 1:
        result = PackList_InitInt();
        goto check;
    case 2:
        result = PackList_HandleInputInt();
        goto check;
    case 3:
        result = PackList_FadeOutInt();
    check:
        if ((result << 16) != 0) {
            gSceneWork.state = 0;
            gSceneWork.unused4 = 0;
        next:
            (*state)++;
        }
        return 0;
    default:
        MemClear16(&gPackOpenWork, sizeof(gPackOpenWork));
        index = &gSceneWork.packRows[(gSceneWork.firstIndex + gSceneWork.centerSlot) % gSceneWork.packCount];
        cardNumbers = gPackOpenWork.cardNumbers;
        packInfo = gPackInfo;
        /* The ROM loads the table base before reading the selected index. */
        asm volatile ("" : : "r"(packInfo));
        packId = packInfo[*index].id;
        GeneratePackCards(cardNumbers, packId);
        SaveGame();
        return 1;
    }
}

/*
 * GetPack_InitScene
 *
 * Step 1: video, palettes, tiles and BG maps of the reveal screen; resets the reveal
 * frames and the BG3 scroll between two ResetBgScroll calls and installs GetPack_HBlank
 * as the HBlank callback. Returns 1.
 */
int GetPack_InitScene(void)
{
    int i;
    int j;
    u16 *map;
    u16 *row;
    struct PackOpenWork *work;
    gMain.vblankFlags = 0xB83; /* VBlank work of the scene: OAM/map copies and the BG3 scroll */
    REG_DISPCNT = 0x40;        /* OBJ 1D mapping; the layers switch on at fade-in */
    REG_BG0CNT = 4;            /* char base 1, priority 0 */
    REG_BG1CNT = 0x105;        /* char base 1, screen base 1, priority 1 */
    REG_BG2CNT = 0x206;        /* char base 1, screen base 2, priority 2 */
    REG_BG3CNT = 0x307;        /* char base 1, screen base 3, priority 3 */
    ResetVideo();
    ClearBgMapBuffers();
    LoadSystemGfx();
    REG_MOSAIC = 0;
    SetBrightnessBlack();
    MemCopy16((void *)OBJ_PLTT, gHandCursorPal, 0x20);
    MemCopy16((void *)(OBJ_PLTT + 0x20), gCardIconPal, 0x20);
    MemCopy16((void *)OBJ_VRAM0, gHandCursorGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x800), gUnk_0867817C, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x1000), gCardIconNormalGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x1800), gCardIconEffectGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x2000), gCardIconFusionGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x2800), gCardIconRitualGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x3000), gCardIconMagicGfx, 0x800);
    MemCopy16((void *)(OBJ_VRAM0 + 0x3800), gCardIconTrapGfx, 0x800);
    MemCopy16((void *)(BG_PLTT + 0x20), gPackSceneBgPal, 0x20);
    MemCopy16((void *)(BG_PLTT + 0x40), gPackCursorFramePal, 0x20);
    MemCopy16((void *)(VRAM + 0x6600), gPackSceneBgTiles, 0x80);
    MemCopy16((void *)(VRAM + 0x8000), gPackCursorFrameTiles, 0x120);
    map = gMain.bgMapBuffer[3];
    for (i = 0; i < 16; i++) {
        j = 15; /* set before row: lengthens j's live range so map wins r2 in global alloc */
        row = map + 32;
        for (; j >= 0; j--) {
            map[0] = 0x1130;
            map[1] = 0x1131;
            row[0] = 0x1132;
            map[33] = 0x1133;
            row += 2;
            map += 2;
        }
        map += 32;
    }
    map = gMain.bgMapBuffer[1];
    map[0] = 0x2200;
    map[96] = 0x2201;
    map[125] = 0x2202;
    map[29] = 0x2203;
    for (i = 1; i <= 28; i++) {
        row = (u16 *)((u16)i * 2 + (u32)map); /* offset-first sum: adds r1, r0, r2 */
        row[0] = 0x2204;
        row[32] = 0x2208;
        row[64] = 0x2208;
        row[96] = 0x2205;
    }
    map[32] = 0x2206;
    map[64] = 0x2206;
    map[61] = 0x2207;
    map[93] = 0x2207;
    ResetBgScroll();
    work = &gPackOpenWork;
    ((struct PackBgScrollView *)work)->bgScrollX = 0;
    ((struct PackBgScrollView *)work)->bgScrollY = 0;
    for (i = 0; i < 5; i++) /* loop.c reverses this into the ROM's 0x110-down store loop */
        work->revealFrame[i] = 0;
    ResetBgScroll();
    gMain.vblankCallback = 0;
    REG_IME = 0;
    REG_IE &= ~INTR_FLAG_HBLANK;
    IntrTable[INTR_SLOT_HBLANK] = GetPack_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_HBLANK;
    REG_IME = 1;
    gMain.bgVofs[1] = -(gPackOpenWork.cursorRow << 5) - gPackCursorSlideOffsets[gPackOpenWork.cursorAnimStep];
    gMain.bgVofs[0] = 3;
    return 1;
}

/*
 * GetPack_FadeIn
 *
 * Step 2: switches the layers on, scrolls the background and fades in from black.
 */
u16 GetPack_FadeIn(void)
{
    REG_DISPCNT |= 0x1F00; /* BG0-3 and OBJ layers on */
    GetPack_ScrollBg();
    return FadeFromBlack(4);
}

/*
 * GetPack_RevealCards
 *
 * Step 3: advances each card's flip animation (enum PackRevealFrame). Card i advances
 * once card i-1 has passed PACK_REVEAL_FACE_SHOWN; A/B snaps the remaining face-down
 * time. Once a card reaches PACK_REVEAL_LAST_FRAME its text row is drawn. Returns 1
 * when all five are settled.
 */
int GetPack_RevealCards(void)
{
    int i;
    int done = 0;
    GetPack_ScrollBg();
    if ((u16)FadeFromBlack(4) == 0)
        return 0;
    GetPack_DrawCardSprites();
    for (i = 0; i <= 4; i++) {
        if (gPackOpenWork.revealFrame[i] <= PACK_REVEAL_LAST_FRAME) {
            if (gPackOpenWork.revealFrame[i] == 2)
                PlaySE(SE_CARD_FLIP);
            if ((gMain.heldKeys & (A_BUTTON | B_BUTTON)) != 0 && gPackOpenWork.revealFrame[i] <= PACK_REVEAL_LAST_FRAME - 1) {
                gPackOpenWork.revealFrame[i] = PACK_REVEAL_LAST_FRAME;
            } else if (i != 0) {
                if (gPackOpenWork.revealFrame[i - 1] > PACK_REVEAL_FACE_SHOWN)
                    gPackOpenWork.revealFrame[i]++;
            } else {
                gPackOpenWork.revealFrame[0]++;
            }
        } else {
            done++;
        }
    }
    for (i = 0; i <= 4; i++) {
        if (gPackOpenWork.revealFrame[i] == PACK_REVEAL_LAST_FRAME)
            GetPack_DrawCardRow(i, CardNumberToId(gPackOpenWork.cardNumbers[i]));
    }
    return done > 4;
}

/*
 * GetPack_HandleInput
 *
 * Step 4: UP/DOWN slide the highlight frame over the five card rows (the slide offsets
 * come from gPackCursorSlideOffsets via the BG1 offset shadow); A opens the card
 * detail (step += 3, to GETPACK_STEP_CARD_DETAIL), B finishes.
 */
int GetPack_HandleInput(void)
{
    GetPack_ScrollBg();
    GetPack_DrawCardSprites();
    gMain.bgVofs[1] = -(gPackOpenWork.cursorRow << 5) - gPackCursorSlideOffsets[gPackOpenWork.cursorAnimStep];
    if (gPackOpenWork.cursorAnimDir != 0) {
        switch (gPackOpenWork.cursorAnimDir) {
        case PACK_CURSOR_ANIM_UP:
            if (gPackOpenWork.cursorAnimStep != 0) {
                gPackOpenWork.cursorAnimStep--;
            done:
                return 0;
            }
            gPackOpenWork.cursorAnimDir = 0;
            break;
        case PACK_CURSOR_ANIM_DOWN:
            gPackOpenWork.cursorAnimStep++;
            if (gPackOpenWork.cursorAnimStep != 0)
                goto done;
            gPackOpenWork.cursorAnimDir = 0;
            gPackOpenWork.cursorAnimStep = 0;
            gPackOpenWork.cursorRow++;
            break;
        default:
            gPackOpenWork.cursorAnimDir = 0;
            break;
        }
    }
    if (gMain.newKeys & DPAD_UP) {
        if (gPackOpenWork.cursorRow != 0) {
            gPackOpenWork.cursorRow--;
            gPackOpenWork.cursorAnimStep = 7;
            gPackOpenWork.cursorAnimDir = PACK_CURSOR_ANIM_UP;
            PlaySE(SE_CURSOR);
        } else {
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & DPAD_DOWN) {
        if (gPackOpenWork.cursorRow <= 3) {
            gPackOpenWork.cursorAnimStep = 0;
            gPackOpenWork.cursorAnimDir = PACK_CURSOR_ANIM_DOWN;
            PlaySE(SE_CURSOR);
        } else {
            PlaySE(SE_ERROR);
        }
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_CONFIRM);
        gMain.seqIndex1 += 3;
        gMain.seqState1 = 0;
        goto done;
    }
    if ((gMain.newKeys & B_BUTTON) == 0)
        goto done;
    PlaySE(SE_CANCEL);
    return 1;
}

/*
 * GetPack_FadeOutAndAddCards
 *
 * Step 5: fades out and adds the five cards to the trunk.
 */
int GetPack_FadeOutAndAddCards(void)
{
    int i;
    GetPack_ScrollBg();
    GetPack_DrawCardSpritesArgs(5, -1, 0);
    if ((u16)FadeToBlack(2) == 0)
        return 0;
    for (i = 0; i < 5; i++)
        AddCardToTrunk(CardNumberToId(gPackOpenWork.cardNumbers[i]));
    return 1;
}

/* Displayed ATK of a card: its stat word value times CARD_STATS_POINTS_SCALE; 0 for
 * Trap/Magic/Ticket, 4000 for Divine. */
static inline int CardAtkValue(u16 id)
{
    int value;
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        value = 0;
        break;
    case CARD_TYPE_DIVINE:
        value = 4000;
        break;
    default:
        value = CARD_STATS_ATK(CARD_STATS_WORD(id)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return value;
}

/* Displayed DEF of a card: same type rules as CardAtkValue. */
static inline int CardDefValue(u16 id)
{
    int value;
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        value = 0;
        break;
    case CARD_TYPE_DIVINE:
        value = 4000;
        break;
    default:
        value = CARD_STATS_DEF(CARD_STATS_WORD(id)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return value;
}

/*
 * GetPack_ShowCardDetail
 *
 * Step 7: the Card Detail view of the card under the cursor, on the sub-state at
 * gMain.seqState1 (enum GetPackDetailState): setup loads the card, the viewer fades in
 * and handles input, LEFT/RIGHT switch to the neighbouring pack card (state 10), and
 * state 11 resets to setup. Other states return 1 (done).
 */
int GetPack_ShowCardDetail(void)
{
    switch (gMain.seqState1) {
    case PACKDETAIL_SETUP:
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        IntrTable[INTR_SLOT_HBLANK] = 0;
        REG_IME = 1;
        CardDetail_Reset();
        gCardDetail.cardId = CardNumberToId(gPackOpenWork.cardNumbers[gPackOpenWork.cursorRow]);
        gCardDetail.atk = CardAtkValue(CardNumberToId(gPackOpenWork.cardNumbers[gPackOpenWork.cursorRow]));
        gCardDetail.def = CardDefValue(CardNumberToId(gPackOpenWork.cardNumbers[gPackOpenWork.cursorRow]));
        gMain.seqState1++;
        return 0;
    case PACKDETAIL_INIT_VIDEO:
        if ((u16)CardDetail_InitVideo() != 0) {
            CardDetail_DrawCard();
            gMain.seqState1++;
        }
        return 0;
    case PACKDETAIL_FADE_IN:
        if ((CardDetail_FadeInInt() << 16) != 0)
            gMain.seqState1++;
        return 0;
    case PACKDETAIL_INPUT:
        if ((CardDetail_HandleInputInt() << 16) != 0)
            gMain.seqState1++;
        if (gMain.newKeys & DPAD_RIGHT) {
            gPackOpenWork.cursorRow = (gPackOpenWork.cursorRow + 1) % 5;
            gMain.seqState1 = PACKDETAIL_SWITCH_CARD;
        }
        if (gMain.newKeys & DPAD_LEFT) {
            gPackOpenWork.cursorRow = (gPackOpenWork.cursorRow + 4) % 5;
            gMain.seqState1 = PACKDETAIL_SWITCH_CARD;
        }
        return 0;
    case PACKDETAIL_CLOSE:
    case PACKDETAIL_SWITCH_CARD:
        if ((CardDetail_FadeOutInt() << 16) != 0)
            gMain.seqState1++;
        return 0;
    case PACKDETAIL_RESTART:
        gMain.seqState1 = 0;
        return 0;
    default:
        return 1;
    }
}
