#ifndef GUARD_BOOSTER_H
#define GUARD_BOOSTER_H

/*
 * Booster packs: the pack generator, the Get-a-pack scene (pack list, opening and the five-card reveal),
 * and the New Game starter-deck choice, which reuses the pack-list layout.
 *
 * Pack contents hold card NUMBERS (the printed numbers, gCardIdToNumber), not card IDs. A normal pack has
 * five cards: one from a rolled rarity slot (RollPackRarity) and four from a shuffled copy of the commons
 * slot (gPackOpenWork.commonPool).
 *
 * gSceneWork (0x02020310, struct SceneWork in duel_scenes.h) is the work area of the pack list and the
 * starter-deck screen; struct PackListWork and struct StarterDeckSelectWork describe it for those screens
 * (0x8070 bytes, larger than struct SceneWork). Offsets in those structs are from the start of gSceneWork.
 *
 * Code: booster_pack.c (generator, opening scene), booster_get_pack.c (runner, pack list, starter deck),
 * card_canvas.c (opening-scene drawing), deck_edit_panel.c (pack-list steps), title_screen.c
 * (BuildStarterDeck). Prototypes are the definitions as compiled; units that call a function through a
 * different local prototype keep that view (build/readability/proto_mismatches.txt).
 */

#include "global.h"

/*
 * gGetPackSteps index (gMain.seqIndex1), run by CB_GetPack and GetRewardPack. Entries 6 and 10 are NULL
 * (the runner returns 1). GetPack_HandleInput adds 3 to reach the card detail; GetPack_FadeInAndResume
 * subtracts 5 to come back to GETPACK_STEP_INPUT.
 */
enum GetPackStep {
    GETPACK_STEP_SELECT = 0,         /* GetPack_SelectAndGenerate: pack list (or reward pack), generate */
    GETPACK_STEP_INIT_SCENE = 1,     /* GetPack_InitScene */
    GETPACK_STEP_FADE_IN = 2,        /* GetPack_FadeIn */
    GETPACK_STEP_REVEAL = 3,         /* GetPack_RevealCards */
    GETPACK_STEP_INPUT = 4,          /* GetPack_HandleInput */
    GETPACK_STEP_FINISH = 5,         /* GetPack_FadeOutAndAddCards */
    GETPACK_STEP_END = 6,            /* NULL */
    GETPACK_STEP_CARD_DETAIL = 7,    /* GetPack_ShowCardDetail */
    GETPACK_STEP_RESTORE_SCENE = 8,  /* GetPack_RestoreScene */
    GETPACK_STEP_RESUME = 9,         /* GetPack_FadeInAndResume (jumps back to 4) */
    GETPACK_STEP_DETAIL_END = 10,    /* NULL, not reached */
};

/* gMain.seqState1 inside GetPack_ShowCardDetail (it runs the Card Detail steps itself). */
enum GetPackDetailState {
    PACKDETAIL_SETUP = 0,        /* HBlank handler off, load the card under the cursor */
    PACKDETAIL_INIT_VIDEO = 1,   /* CardDetail_InitVideo, CardDetail_DrawCard */
    PACKDETAIL_FADE_IN = 2,      /* CardDetail_FadeIn */
    PACKDETAIL_INPUT = 3,        /* CardDetail_HandleInput; LEFT/RIGHT go to PACKDETAIL_SWITCH_CARD */
    PACKDETAIL_CLOSE = 4,        /* CardDetail_FadeOut */
    PACKDETAIL_DONE = 5,         /* returns 1 */
    PACKDETAIL_SWITCH_CARD = 10, /* CardDetail_FadeOut before showing the next/previous pack card */
    PACKDETAIL_RESTART = 11,     /* back to PACKDETAIL_SETUP */
};

/* Slide of the highlight frame over the five card rows: PackOpenWork.cursorAnimDir. */
enum PackCursorAnim {
    PACK_CURSOR_ANIM_NONE = 0,
    PACK_CURSOR_ANIM_UP = 1,
    PACK_CURSOR_ANIM_DOWN = 2,
};

/* Per-card flip animation frame: PackOpenWork.revealFrame[i]. */
enum PackRevealFrame {
    PACK_REVEAL_START = 0,       /* frames 0-11 show the card back */
    PACK_REVEAL_FACE_SHOWN = 12, /* frames 12-23 show the face */
    PACK_REVEAL_LAST_FRAME = 23, /* 0x17: draws the card's text row */
    PACK_REVEAL_DONE = 24,       /* 0x18: settled */
};

/* The three starter decks (deck-box colours): StarterDeckSelectWork.choice, BuildStarterDeck(choice). */
enum StarterDeck {
    STARTER_DECK_BLACK = 0,
    STARTER_DECK_RED = 1,
    STARTER_DECK_GREEN = 2,
};

/* gSceneWork.state inside StarterDeckSelect_Init. */
enum StarterDeckInitState {
    STARTERDECK_INIT_VIDEO = 0,   /* choice = cursorSlot = 1 (middle deck), video setup */
    STARTERDECK_INIT_GFX = 1,     /* background and deck-box images */
    STARTERDECK_INIT_FADE_IN = 2,
};

/* gMain.seqIndex1 inside StarterDeckSelect_Run (New Game "select an Initial Deck" screen). */
enum StarterDeckSelectStep {
    STARTERDECK_STEP_CLEAR = 0,    /* StarterDeckSelect_ClearWork */
    STARTERDECK_STEP_INIT = 1,     /* StarterDeckSelect_Init */
    STARTERDECK_STEP_INPUT = 2,    /* StarterDeckSelect_HandleInput */
    STARTERDECK_STEP_FADE_OUT = 3, /* StarterDeckSelect_FadeOut */
    STARTERDECK_STEP_FINISH = 4,   /* InitSaveData, BuildStarterDeck(choice), SaveGame; returns 1 */
};

/* One rarity slot of a pack: the card numbers it can give. */
struct PackSlot {
    const u16 *cards; /* +0x0 card numbers (NULL when count is 0) */
    s32 count;        /* +0x4 number of cards; 0 = empty slot */
};

/* The rarity slots of one pack: slot[0] is the rarest, slot[7] the commons. */
struct PackSlots {
    struct PackSlot slot[8]; /* +0x00 */
};

/* One row of gPackContents (0x081A562C, 28 rows): which slot lists a pack id uses. */
struct PackContentsEntry {
    const struct PackSlots *slots; /* +0x0 rarity slot lists */
    u16 packId;                    /* +0x4 pack id (1-7, 11, ... 903) */
    u16 pad;                       /* +0x6 */
};

/*
 * Layout of the highlight-frame cursor byte at gPackOpenWork + 0x114. agbcc pads every struct to 4 bytes,
 * so struct PackOpenWork cannot embed this struct (+0x115 follows); it has the same three bitfields as
 * cursorRow, cursorAnimStep and cursorAnimDir.
 */
struct PackCursor {
    u8 row:3;      /* bits 0-2: card row under the frame (0-4); also the card GetPack_ShowCardDetail shows */
    u8 animStep:3; /* bits 3-5: index into gPackCursorSlideOffsets during a slide */
    u8 animDir:2;  /* bits 6-7: enum PackCursorAnim */
};

/* One row of gPackInfo (0x080865DC, 23 rows): a pack on the Get-a-pack list. */
struct PackInfo {
    u16 id;               /* +0x00 pack id (enum BoosterPackId); the slot is 4 bytes but read as u16 */
    u16 pad2;             /* +0x02 */
    const u8 *coverGfx;   /* +0x04 cover: 98 8bpp tiles (56x112), palette gPackListPal */
    char name[0x40];      /* +0x08 pack name ("Vol.1", "Expert Pack 1", ...) */
};

/*
 * gSceneWork (0x02020310) during the Get-a-pack list (0x8070 bytes, cleared by PackList_ClearWork). The
 * covers sit in BG1 slots of 80 pixels; a slide eases BG1 HOFS from slideFrom to slideTo.
 */
struct PackListWork {
    s32 state;             /* +0x00 sub-state of the current step */
    s32 unused4;           /* +0x04 zeroed with state; never read */
    s32 unused8;           /* +0x08 not used by the list (StarterDeckSelectWork.choice) */
    s32 centerSlot;        /* +0x0C visible slot of the chosen pack (1) */
    s32 firstIndex;        /* +0x10 packRows index shown in the left slot */
    s32 nextFirstIndex;    /* +0x14 firstIndex after the running slide */
    u8 flags;              /* +0x18 bit 0: bgTiles must be flushed to VRAM (PackList_FlushVram) */
    u8 pad19;              /* +0x19 */
    u16 coverAlpha;        /* +0x1A current blend level of the covers (BLDALPHA EVA) */
    u16 coverAlphaTarget;  /* +0x1C 8 at rest, 0x10 during a slide */
    u16 pad1E;             /* +0x1E */
    s32 slideTo;           /* +0x20 target BG1 HOFS (slideFrom -/+ 0x50) */
    s32 slideFrom;         /* +0x24 BG1 HOFS at the start of the slide */
    u16 slideFrame;        /* +0x28 frames left, 8 -> 0 */
    u16 slideDir;          /* +0x2A 1 = left, 2 = right */
    u16 packRows[0x20];    /* +0x2C gPackInfo row of each list entry */
    u16 packCount;         /* +0x6C entries in packRows */
    u8 bgTiles[0x8000];    /* +0x6E 8bpp tile buffer for BG char block 1: covers at tile 0x10 + slot * 0x62
                            *       (slots 0-3), background tiles at 0x1FD-0x1FF */
};

/*
 * gSceneWork (0x02020310) during the starter-deck screen. StarterDeckSelect_ClearWork clears the same
 * 0x8070 bytes as PackList_ClearWork; only the first fields are used here.
 */
struct StarterDeckSelectWork {
    s32 state;          /* +0x00 sub-state of the current step; blink/fade frame counter in the fade-out */
    s32 unused4;        /* +0x04 zeroed on every step change; never read */
    s32 choice;         /* +0x08 selected deck 0-2 (enum StarterDeck), passed to BuildStarterDeck */
    s32 cursorSlot;     /* +0x0C slot the hand cursor is drawn at; catches up with choice */
    s32 slideFrames;    /* +0x10 frames left in the 4-frame cursor slide */
    u8 unk14[4];        /* +0x14 */
    u8 flags;           /* +0x18 unused here (same offset as PackListWork.flags) */
    u8 unk19[0x8070 - 0x19]; /* +0x19 rest of the pack-list work area */
};

/* gPackOpenWork (0x02015160, 0x11C bytes): the pack being opened and the reveal scene's state. */
struct PackOpenWork {
    u16 unused0;             /* +0x000 no access found */
    u16 commonPool[0x80];    /* +0x002 shuffled copy of the commons slot, source of cards 2-5 */
    u16 cardNumbers[5];      /* +0x102 card numbers of the opened pack (GeneratePackCards output) */
    u8 revealFrame[5];       /* +0x10C per-card flip frame (enum PackRevealFrame) */
    u8 pad111;               /* +0x111 */
    u16 rareCardNumber;      /* +0x112 card from the rolled slot; its name is drawn in the colour-cycled
                              *        colour 15. 9999 for the random packs (no match) */
    u8 cursorRow:3;          /* +0x114 bits 0-2: struct PackCursor.row */
    u8 cursorAnimStep:3;     /*        bits 3-5: struct PackCursor.animStep */
    u8 cursorAnimDir:2;      /*        bits 6-7: struct PackCursor.animDir (enum PackCursorAnim) */
    u8 pad115;               /* +0x115 */
    u16 bgScrollX;           /* +0x116 BG3 HOFS, incremented each frame by GetPack_ScrollBg */
    u16 bgScrollY;           /* +0x118 BG3 VOFS: the pattern scrolls diagonally */
    u16 colorCyclePhase;     /* +0x11A decremented each frame; GetPack_HBlank colours BG palette 0 entry 15
                              *        with gPackSceneRasterColors[(VCOUNT + phase / 2) & 7] */
};

/* One row of gStarterDeckPools (0x08198744, 11 rows): a pool and how many cards each deck takes from it. */
struct StarterDeckPool {
    const u16 *cards;   /* +0x0 card numbers */
    u32 count:10;       /* +0x4 bits 0-9: pool size */
    u32 pick0:5;        /* bits 10-14: cards taken for deck choice 0 */
    u32 pick1:5;        /* bits 15-19: cards taken for deck choice 1 */
    u32 pick2:5;        /* bits 20-24: cards taken for deck choice 2 */
};

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char booster_h_check_slot[sizeof(struct PackSlot) == 0x8 ? 1 : -1];
typedef char booster_h_check_slots[sizeof(struct PackSlots) == 0x40 ? 1 : -1];
typedef char booster_h_check_contents[sizeof(struct PackContentsEntry) == 0x8 ? 1 : -1];
typedef char booster_h_check_cursor[sizeof(struct PackCursor) == 0x4 ? 1 : -1];
typedef char booster_h_check_info[sizeof(struct PackInfo) == 0x48 ? 1 : -1];
typedef char booster_h_check_info_name[(u32)&((struct PackInfo *)0)->name == 0x8 ? 1 : -1];
typedef char booster_h_check_list[sizeof(struct PackListWork) == 0x8070 ? 1 : -1];
typedef char booster_h_check_list_flags[(u32)&((struct PackListWork *)0)->flags == 0x18 ? 1 : -1];
typedef char booster_h_check_list_rows[(u32)&((struct PackListWork *)0)->packRows == 0x2C ? 1 : -1];
typedef char booster_h_check_list_count[(u32)&((struct PackListWork *)0)->packCount == 0x6C ? 1 : -1];
typedef char booster_h_check_list_tiles[(u32)&((struct PackListWork *)0)->bgTiles == 0x6E ? 1 : -1];
typedef char booster_h_check_starter[sizeof(struct StarterDeckSelectWork) == 0x8070 ? 1 : -1];
typedef char booster_h_check_starter_flags[(u32)&((struct StarterDeckSelectWork *)0)->flags == 0x18 ? 1 : -1];
typedef char booster_h_check_open[sizeof(struct PackOpenWork) == 0x11C ? 1 : -1];
typedef char booster_h_check_open_cards[(u32)&((struct PackOpenWork *)0)->cardNumbers == 0x102 ? 1 : -1];
typedef char booster_h_check_open_reveal[(u32)&((struct PackOpenWork *)0)->revealFrame == 0x10C ? 1 : -1];
typedef char booster_h_check_open_rare[(u32)&((struct PackOpenWork *)0)->rareCardNumber == 0x112 ? 1 : -1];
typedef char booster_h_check_open_pad115[(u32)&((struct PackOpenWork *)0)->pad115 == 0x115 ? 1 : -1];
typedef char booster_h_check_open_scroll[(u32)&((struct PackOpenWork *)0)->bgScrollX == 0x116 ? 1 : -1];
typedef char booster_h_check_open_phase[(u32)&((struct PackOpenWork *)0)->colorCyclePhase == 0x11A ? 1 : -1];
typedef char booster_h_check_pool[sizeof(struct StarterDeckPool) == 0x8 ? 1 : -1];

/* Get-a-pack work area (0x02015160). Cleared before a normal pack is generated; written by the generator
 * and the opening scene. */
extern struct PackOpenWork gPackOpenWork;

/* 0x080865DC: the 23 packs of the Get-a-pack list. */
extern const struct PackInfo gPackInfo[];

/* --- Pack generator (booster_pack.c) --- */

/* Fills out[0..4] with the card numbers of a new pack of packId, in shuffled order. Returns the rolled
 * rarity slot, or -1 for the random packs (0x66, 0x67, 0x6E) and unknown ids. */
int GeneratePackCards(u16 *out, u16 packId);
/* Index of the highest non-empty slot of pack, searching 7 down to 1 (the commons slot); 0 if none. */
int GetPackCommonSlot(struct PackSlots *pack);
/* Rolls the rarity slot of the pack's first card (Random() % 180, % 270 when re-buying the last pack;
 * packs that keep falling back to commons get better odds). */
int RollPackRarity(struct PackSlots *pack, u16 packId);
/* A random card number from pack->slot[slot]. */
u16 PickPackSlotCard(struct PackSlots *pack, int slot);
/* 1 if any copy of the card with that number is owned (trunk, deck, side or fusion list). Unreferenced. */
u8 IsCardNumberOwned(u16 cardNumber);

/* --- Get-a-pack scene (gGetPackSteps) --- */

/* Scene callback: runs gGetPackSteps[gMain.seqIndex1]; returns 1 at a NULL entry. */
u32 CB_GetPack(void);
/* Sets gMain.rewardPack = packId and runs the steps like CB_GetPack (skips the list for a nonzero id). */
u32 GetRewardPack(u32 packId);
/* Step 0: the pack list (or the reward pack), then GeneratePackCards and SaveGame. */
int GetPack_SelectAndGenerate(void);
/* Step 1: video, palettes, tiles and BG maps of the reveal screen; resets the reveal frames. Returns 1. */
int GetPack_InitScene(void);
/* Step 2: scrolls the background and fades in. */
u16 GetPack_FadeIn(void);
/* Step 3: advances each card's flip animation; returns 1 when all five are settled. */
int GetPack_RevealCards(void);
/* Step 4: UP/DOWN move the highlight frame over the five cards; A opens the card detail (step 7), B
 * finishes. */
int GetPack_HandleInput(void);
/* Step 5: fades out and adds the five cards to the trunk. */
int GetPack_FadeOutAndAddCards(void);
/* Step 7: Card Detail view of the card under the cursor; LEFT/RIGHT switch cards (GetPackDetailState). */
int GetPack_ShowCardDetail(void);
/* Step 8: rebuilds the reveal screen with every card settled. Returns 1. */
u32 GetPack_RestoreScene(void);
/* Step 9: fades back in and returns to step 4 itself (always returns 0). */
u32 GetPack_FadeInAndResume(void);
/* Returns 1. No callers. */
u32 GetPack_UnusedReturnTrue(void);

/* --- Get-a-pack scene drawing (card_canvas.c) --- */

/* HBlank handler: cycles BG palette 0 colour 15 per scanline (gPackSceneRasterColors). */
void GetPack_HBlank(void);
/* Per-frame background motion: BG3 scroll from gPackOpenWork.bgScrollX/Y (post-incremented). */
void GetPack_ScrollBg(void);
/* Draws the five cards as 32x32 sprites by reveal frame (card back, flip, face). */
void GetPack_DrawCardSprites(void);
/* Draws row 0-4 of the card list: the name of card ID id (colour 15 for the rare card), its icons, and
 * for monsters ATK/DEF and level stars. */
void GetPack_DrawCardRow(int row, u16 id);

/* --- Pack list (booster_get_pack.c, deck_edit_panel.c) --- */

/* GetPack_SelectAndGenerate sub-state 1: sets up the list with packRows[0] in the centre slot. */
u16 PackList_Init(void);
/* Sub-state 2: LEFT/RIGHT slide the covers, A picks the centre pack. */
u16 PackList_HandleInput(void);
/* Sub-state 3: fades to black; returns 1 when done. */
u16 PackList_FadeOut(void);
/* Empty (a stripped debug print); called with the slide frame or 0. */
void PackList_DebugNop(u32 value);
/* Zeroes the 0x8070-byte PackListWork in gSceneWork (DMA fill). */
void PackList_ClearWork(void);
/* Video setup shared by the pack list and the starter-deck screen (BG maps are uploaded by hand). */
void PackList_InitVideo(void);
/* Adds every unlocked pack of gUnlockablePackIds to the list (PackList_AddPack). */
void PackList_AddUnlockedPacks(void);
/* Appends the gPackInfo row of packId to packRows; nothing for an unknown id. */
void PackList_AddPack(u32 packId);
/* Copies the tile buffer to BG char block 1 and the BG map buffers to VRAM. */
void PackList_FlushVram(void);
/* Loads gPackListPal and three background tiles at tile..tile + 2 of the tile buffer; fills the BG2 map
 * with tile on rows rowStart..rowEnd - 1 and tile + 1 elsewhere. */
void PackList_DrawBackground(s32 rowStart, s32 rowEnd, u16 tile);
/* Clears the cover maps and draws the covers of packRows[first..] in slots 0-2. */
void PackList_DrawCovers(s32 first);
/* Writes a 7x14 block of the slot's cover tile numbers into gMain.bgMapBuffer[bg] at mapPos. */
void PackList_DrawCoverTiles(u32 bg, u32 mapPos, u32 slot);
/* Copies the cover of packId into the tile buffer at tile 0x10 + slot * 0x62. */
void PackList_LoadCoverGfx(u16 slot, u16 packId);
/* Blends the BG1 covers over BG2 at eva/16 (BLDCNT 0x442, BLDALPHA eva | (16 - eva) << 8). */
void PackList_SetCoverAlpha(u32 eva);
/* The cover graphics of packId (gPackInfo[].coverGfx), or NULL. */
const u8 *GetPackCoverGfx(u16 packId);
/* Returns 0. No callers. */
u32 PackList_UnusedReturnFalse(void);

/* --- Starter deck (New Game) --- */

/* New Game "select an Initial Deck" screen (StarterDeckSelectStep); returns 1 once the deck is built. */
u16 StarterDeckSelect_Run(void);
/* Zeroes the 0x8070-byte work area in gSceneWork (same code as PackList_ClearWork). */
void StarterDeckSelect_ClearWork(void);
/* Step 1 (StarterDeckInitState): video, background and the three deck boxes, fade in. */
u16 StarterDeckSelect_Init(void);
/* Step 2: LEFT/RIGHT choose a deck, A confirms. */
u16 StarterDeckSelect_HandleInput(void);
/* Step 3: blinks the cursor for 60 frames, then fades out. */
u16 StarterDeckSelect_FadeOut(void);
/* Draws the hand cursor under slot cursorSlot (sliding towards choice). */
void StarterDeckSelect_DrawCursor(void);
/* Loads image (palette and tiles) at tileBase and fills rows rowStart..rowEnd - 1 of the BG2 map with
 * tile tileBase / 2. */
void StarterDeckSelect_DrawBackground(s32 rowStart, s32 rowEnd, u16 tileBase, const void *image);
/* Builds the starting deck for choice (choice % 3) from the 11 gStarterDeckPools. */
void BuildStarterDeck(s32 choice);

#endif /* GUARD_BOOSTER_H */
