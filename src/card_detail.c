/*
 * Card Detail viewer and duel card helpers (0x08006878-0x08007993).
 *
 * The first half is the full-screen Card Detail viewer: the card frame, the art, the name/type/description text
 * (drawn by CardDetail_DrawInfo in title_menu.c) and the ATK/DEF sprites, with Up/Down scrolling the
 * description. Deck Edit, the duel, campaign rewards, Password, Card Trading and pack opening use it, and so do
 * the debug menu items "Card Detail" (a manual browser) and "Auto Detail" (shows every card ID in turn).
 *
 * The second half holds small duel helpers: life points, card-word status bits, copying and swapping card
 * words, and card predicates keyed by card number (IsToonMonster, HasFlipEffect) or card ID (IsSameCardName,
 * IsEffectMonster, IsSpecialSummonOnly).
 */
#include "global.h"
#include "gba.h"                    /* REG_*, OBJ_PLTT, OBJ_VRAM0, DISPCNT_*, BGCNT_*, key masks */
#include "main.h"                   /* gMain, IntrTable, ResetBgScroll, enum VBlankFlag */
#include "duel.h"                   /* struct DuelPlayer, struct DuelCard, struct DuelCardStatusBytes, gDuelZones */
#include "sound.h"                  /* PlaySE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/sound.h"        /* SE_CANCEL */
#include "card_data.h"              /* card tables, CARD_STATS_* field extractors, frame and digit graphics */
#include "util.h"                   /* MemClear16, MemCopy16, CopyDoubleWords */
#include "palette.h"                /* SetBrightnessBlack, ClearBlend, FadeFromBlack */
#include "bg.h"                     /* ResetVideo, LoadSystemGfx, LoadBgImageMap1, DrawCardPortrait */
#include "debug.h"                  /* DebugCardDetail_*, CB_DebugCardDetail, CB_DebugAutoDetail */
#include "card_detail.h"            /* struct CardDetail gCardDetail, CardDetail_* */

/* Local views kept on purpose (matching choices, see build/readability/HEADERS.md). */

/* gCardStats[id] and gCardIdToNumber[id] through their integer addresses 0x08621DE0 / 0x08622AB4 (HEADERS.md
 * pattern 4). Matching: GCC then reloads the table address at every use instead of keeping it in a register;
 * the symbol forms of card_data.h give different code. */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats[id] */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber[id] */

/* A view of FadeToBlack (palette.h: u32 FadeToBlack(s32 step)) with a u16 parameter and result. Matching:
 * CardDetail_FadeOut tests only the low halfword of the result (lsl #16 after the call), which the u32
 * prototype leaves out. */
extern u16 FadeToBlack16(u16 step) asm("FadeToBlack");

/* ROM data used only here. */
/* 0x08198D50: { DebugCardDetail_Init, DebugCardDetail_Browse, NULL } */
extern u16 (*const gDebugCardDetailSteps[])(void);
/* 0x08198D5C: { DebugCardDetail_Init, DebugAutoDetail_Cycle, NULL } */
extern u16 (*const gDebugAutoDetailSteps[])(void);
/* 0x08198A50: rows of 16 BG1/BG2 HOFS values for the wave intro's HBlank handler: a sine wave whose amplitude
 * decays from 24 to 1. */
extern const s16 gCardDetailWaveTable[24][16];

/* Card numbers 1920-1999 are monster tokens. */
#define IS_TOKEN(id) \
    ((u16)(CARD_NUMBER(id) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

#define LAST_CARD_ID            (CARD_ID_COUNT - 1)     /* 820 */
#define DEBUG_FIRST_CARD_ID     800                     /* World Suppression */
#define DESCRIPTION_VIEW_HEIGHT 0x70                    /* visible lines of the description (BG3) */

/* gMain.seqState1 in DebugCardDetail_Browse. */
enum {
    BROWSE_INPUT = 0,       /* wait for a card change or A/B */
    BROWSE_FADE_OUT = 1,    /* card changed: fade out */
    BROWSE_LOAD = 2,        /* draw the new card */
    BROWSE_FADE_IN = 3,     /* fade in, then back to BROWSE_INPUT */
    BROWSE_EXIT = 10,       /* A/B: fade out, then BROWSE_DONE */
    BROWSE_DONE = 11
};

/* gMain.seqState1 in DebugAutoDetail_Cycle. */
enum {
    CYCLE_START = 0,        /* start from card ID 1 */
    CYCLE_FADE_OUT = 1,
    CYCLE_LOAD = 2,
    CYCLE_FADE_IN = 3,
    CYCLE_NEXT = 4,         /* next card ID, or CYCLE_EXIT after the last one */
    CYCLE_EXIT = 5,         /* fade out, then CYCLE_DONE */
    CYCLE_DONE = 6
};

/* The HBlank IRQ of the wave intro. IME is off while IE or IntrTable change. (Comma-expression macros: static
 * inline functions and do-while blocks change the code of the callers.) */
#define DISABLE_HBLANK_INTR() (REG_IME = 0, REG_IE &= ~INTR_FLAG_HBLANK, REG_IME = 1)
#define SET_HBLANK_HANDLER(handler) \
    (REG_IME = 0, REG_IE &= ~INTR_FLAG_HBLANK, IntrTable[INTR_SLOT_HBLANK] = (handler), REG_IME = 1)
#define ENABLE_HBLANK_INTR() (REG_IME = 0, REG_IE |= INTR_FLAG_HBLANK, REG_IME = 1)

/* Displayed ATK of a card ID: 0 for Trap/Magic/Ticket cards, 4000 for the Egyptian Gods, else the stat * 10. */
static inline s32 GetCardAtk(u16 cardId)
{
    switch ((u8)CARD_STATS_TYPE(CARD_STATS_WORD(cardId))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(CARD_STATS_WORD(cardId)) * CARD_STATS_POINTS_SCALE;
    }
}

/* Displayed DEF, same rules as GetCardAtk. */
static inline s32 GetCardDef(u16 cardId)
{
    switch ((u8)CARD_STATS_TYPE(CARD_STATS_WORD(cardId))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS_WORD(cardId)) * CARD_STATS_POINTS_SCALE;
    }
}

/* enum CardKind of a card ID: the stats kind for monsters, Magic/Trap/Ticket by type. The Egyptian Gods are
 * special-cased by number: Obelisk counts as a Ritual monster, Slifer and Ra as Effect monsters. */
static inline int GetCardKind(u16 cardId)
{
    switch (CARD_NUMBER(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_STATS_TYPE(CARD_STATS_WORD(cardId))) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_WORD(cardId));
    }
}

/* Clears gCardDetail. */
void CardDetail_Reset(void)
{
    MemClear16(&gCardDetail, sizeof(gCardDetail));
}

/* Prepares the viewer for a card: displayed ATK/DEF, auto-close timer in frames (0 = wait for A/B) and the wave
 * intro flag (every caller passes 0). Callers then run CardDetail_Run every frame. */
void CardDetail_Init(u16 cardId, u16 timer, u16 waveIntro)
{
    CardDetail_Reset();
    gCardDetail.cardId = cardId;
    gCardDetail.atk = GetCardAtk(cardId);
    gCardDetail.def = GetCardDef(cardId);
    gCardDetail.timer = timer;
    gCardDetail.waveIntro = waveIntro;
}

/* Viewer step 0: video setup with every layer hidden. BG0 holds the name and type line, BG1 the card art, BG2
 * the card frame and BG3 the description (256x512, scrolled by CardDetail_HandleInput). Loads the system font,
 * the stat digit palette and tiles, removes the VBlank callback and the HBlank handler and, for the wave intro,
 * installs CardDetail_HBlank. Returns 1. */
u16 CardDetail_InitVideo(void)
{
    gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS | VBLANK_BG3_VOFS;
    REG_DISPCNT = DISPCNT_OBJ_1D_MAP;   /* mode 0, every layer off */
    ResetVideo();
    REG_MOSAIC = 0;
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_MOSAIC | BGCNT_16COLOR | BGCNT_SCREENBASE(0);
    REG_BG1CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_256COLOR | BGCNT_SCREENBASE(1);
    REG_BG2CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_256COLOR | BGCNT_SCREENBASE(2);
    REG_BG3CNT = BGCNT_PRIORITY(1) | BGCNT_CHARBASE(1) | BGCNT_16COLOR | BGCNT_SCREENBASE(4) | BGCNT_TXT256x512;
    ResetBgScroll();
    gCardDetail.scrollPos = 0;
    SetBrightnessBlack();
    LoadSystemGfx();
    CopyDoubleWords((void *)(OBJ_PLTT + 3 * 0x20), gCardStatDigitsPal, 0x20);  /* OBJ palette 3 */
    CopyDoubleWords((void *)(OBJ_VRAM0 + 0x30 * 0x20), gCardStatDigitsGfx, 0x200); /* OBJ tiles 0x30-0x3F */
    gMain.vblankCallback = NULL;
    DISABLE_HBLANK_INTR();
    SET_HBLANK_HANDLER(NULL);
    if (gCardDetail.waveIntro) {
        gMain.hblankY = -0x20;
        SET_HBLANK_HANDLER(CardDetail_HBlank);
        ENABLE_HBLANK_INTR();
    }
    return 1;
}

/* Shows BG0-3 and OBJ, draws the stat sprites and steps the fade from black; returns 1 when it is done. */
u16 CardDetail_FadeIn(void)
{
    REG_DISPCNT |= DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    CardDetail_DrawSprites();
    return FadeFromBlack(4);
}

/* Draws the stat sprites and steps the fade to black; once black, hides BG0-3 and OBJ and returns 1. */
u16 CardDetail_FadeOut(void)
{
    CardDetail_DrawSprites();
    if (FadeToBlack16(4)) {
        REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
        return 1;
    }
    return 0;
}

/* Per-frame input. A or B closes the viewer (SE_CANCEL), and so does the auto-close timer when it runs out.
 * Up/Down set the description's scroll target to the top/bottom; BG3 then scrolls 2 px per frame towards it.
 * Returns 1 to close, else 0. */
u16 CardDetail_HandleInput(void)
{
    CardDetail_DrawSprites();
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        PlaySE(SE_CANCEL);
        return 1;
    }
    if (gCardDetail.timer != 0) {
        if (--gCardDetail.timer == 0)
            return 1;
    }
    if ((gMain.newKeys & DPAD_UP) && gCardDetail.scrollTarget > 0)
        gCardDetail.scrollTarget = 0;
    if ((gMain.newKeys & DPAD_DOWN)
        && gCardDetail.scrollTarget < gCardDetail.textHeight - DESCRIPTION_VIEW_HEIGHT)
        gCardDetail.scrollTarget = gCardDetail.textHeight - DESCRIPTION_VIEW_HEIGHT;
    if (gCardDetail.scrollPos != gCardDetail.scrollTarget) {
        if (gCardDetail.scrollPos < gCardDetail.scrollTarget)
            gCardDetail.scrollPos += 2;
        else
            gCardDetail.scrollPos -= 2;
    }
    gMain.bgVofs[3] = gCardDetail.scrollPos;
    return 0;
}

/* Card frame image of a card ID: by type for Trap/Magic/Ticket cards, else by enum CardKind. */
static inline const void *GetCardFrameGfx(u16 cardId)
{
    switch ((u8)CARD_STATS_TYPE(CARD_STATS_WORD(cardId))) {
    case CARD_TYPE_TRAP:
        return gCardFrameTrapGfx;
    case CARD_TYPE_MAGIC:
        return gCardFrameMagicGfx;
    case CARD_TYPE_TICKET:
        return gCardFrameTicketGfx;
    default:
        switch (GetCardKind(cardId)) {
        case CARD_KIND_EFFECT:
            return gCardFrameEffectGfx;
        case CARD_KIND_FUSION:
            return gCardFrameFusionGfx;
        case CARD_KIND_RITUAL:
            return gCardFrameRitualGfx;
        default:
            return gCardFrameNormalGfx;
        }
    }
}

/* Draws gCardDetail.cardId: the text and icons (CardDetail_DrawInfo), the frame (none for tokens) and the art.
 * The wave intro layout moves the frame and the art 9 cells to the right. */
void CardDetail_DrawCard(void)
{
    u32 shift = gCardDetail.waveIntro * 9;

    CardDetail_DrawInfo(gCardDetail.cardId);
    /* Frame: 8bpp image from colour 0x20 and tile 0x10. Cell 0x440 of map buffer 1 is row 2 of map buffer 2
     * (BG2). */
    if (!IS_TOKEN(gCardDetail.cardId))
        LoadBgImageMap1(shift | 0x440, 0x20, 0x10, (u16 *)GetCardFrameGfx(gCardDetail.cardId));
    /* Art: map buffer 1 (BG1) from row 6, column 2; tiles from 0x130, colours 0x80-0xBF. */
    DrawCardPortrait(1, shift + 0xC2, gCardDetail.cardId, 0x130, 0x80);
}

/* Runs the viewer for one frame (enum CardDetailState in gCardDetail.state); returns 1 once it has closed.
 * The wave intro replaces the fade-in: for 12 frames the mosaic shrinks, the darkening fades out and the
 * HBlank handler bends BG1/BG2 with one row of gCardDetailWaveTable per frame; then the handler is removed. */
u16 CardDetail_Run(void)
{
    switch (gCardDetail.state) {
    case CARD_DETAIL_SETUP:
        if (CardDetail_InitVideo()) {
            CardDetail_DrawCard();
            gCardDetail.state++;
        }
        return 0;
    case CARD_DETAIL_FADE_IN:
        if (!gCardDetail.waveIntro) {
            if (CardDetail_FadeIn())
                gCardDetail.state++;
        } else {
            REG_DISPCNT |= DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
            if (gMain.hblankY < 0xA0) {
                s32 line = gMain.hblankY + 0x20;    /* 0, 0x10, ... 0xB0 */
                s32 mosaic = 15 - line / 12;        /* 15 .. 1 */

                REG_BLDY = 0x18 - line / 8;         /* 24 .. 2 (BLDY saturates at 16) */
                REG_MOSAIC = ((mosaic & 0xF) << 4) | (mosaic & 0xF);    /* BG mosaic V and H */
                /* The row index is recomputed from hblankY: reusing `line` changes the code. */
                MemCopy16(gMain.hblankScroll, gCardDetailWaveTable[(gMain.hblankY + 0x20) / 8],
                          sizeof(gCardDetailWaveTable[0]));
                gMain.hblankY += 0x10;
            } else {
                REG_BG0CNT &= ~BGCNT_MOSAIC;
                REG_MOSAIC = 0;
                ClearBlend();
                ResetBgScroll();
                DISABLE_HBLANK_INTR();
                SET_HBLANK_HANDLER(NULL);
                gCardDetail.state++;
            }
        }
        return 0;
    case CARD_DETAIL_INPUT:
        if (CardDetail_HandleInput())
            gCardDetail.state++;
        return 0;
    case CARD_DETAIL_FADE_OUT:
        if (CardDetail_FadeOut())
            gCardDetail.state++;
        return 0;
    }
    return 1;   /* CARD_DETAIL_DONE */
}

/* Step 0 of both debug viewers: sets up Card Detail on World Suppression (an inlined CardDetail_Init that also
 * sets browseCardId), runs the video setup and draws the card, then returns CardDetail_FadeIn's result. */
u16 DebugCardDetail_Init(void)
{
    switch (gMain.seqState1) {
    case 0:
        MemClear16(&gCardDetail, sizeof(gCardDetail));
        gCardDetail.cardId = gCardDetail.browseCardId = DEBUG_FIRST_CARD_ID;
        gCardDetail.atk = GetCardAtk(DEBUG_FIRST_CARD_ID);
        gCardDetail.def = GetCardDef(gCardDetail.browseCardId);
        gMain.seqState1++;
        break;
    case 1:
        if (CardDetail_InitVideo()) {
            CardDetail_DrawCard();
            gMain.seqState1++;
        }
        break;
    default:
        return CardDetail_FadeIn();
    }
    return 0;
}

/* Debug "Card Detail" browser: Right/Left step the card ID by 1, R/L (held) by 10, within 1..820; each change
 * fades out, draws the new card and fades in. A/B (CardDetail_HandleInput) fades out and returns 1. */
u16 DebugCardDetail_Browse(void)
{
    u32 changed;

    CardDetail_DrawSprites();
    switch (gMain.seqState1) {
    case BROWSE_INPUT:
        changed = 0;
        if ((gMain.newKeys & DPAD_RIGHT) && gCardDetail.browseCardId < LAST_CARD_ID) {
            gCardDetail.browseCardId++;
            changed = 1;
        }
        if (gMain.heldKeys & R_BUTTON) {
            if (gCardDetail.browseCardId < LAST_CARD_ID - 10)
                gCardDetail.browseCardId += 10;
            else
                gCardDetail.browseCardId = LAST_CARD_ID;
            changed = 1;
        }
        if ((gMain.newKeys & DPAD_LEFT) && gCardDetail.browseCardId > 1) {
            gCardDetail.browseCardId--;
            changed = 1;
        }
        if (gMain.heldKeys & L_BUTTON) {
            if (gCardDetail.browseCardId > 10)
                gCardDetail.browseCardId -= 10;
            else
                gCardDetail.browseCardId = 1;
            changed = 1;
        }
        if (CardDetail_HandleInput())
            gMain.seqState1 = BROWSE_EXIT;
        else if (changed)
            gMain.seqState1 = BROWSE_FADE_OUT;
        return 0;
    case BROWSE_FADE_OUT:
    case BROWSE_EXIT:
        if (CardDetail_FadeOut())
            gMain.seqState1++;
        return 0;
    case BROWSE_LOAD:
        gCardDetail.cardId = gCardDetail.browseCardId;
        gCardDetail.atk = GetCardAtk(gCardDetail.browseCardId);
        gCardDetail.def = GetCardDef(gCardDetail.browseCardId);
        CardDetail_DrawCard();
        gMain.seqState1++;
        return 0;
    case BROWSE_FADE_IN:
        if (CardDetail_FadeIn())
            gMain.seqState1 = BROWSE_INPUT;
        return 0;
    }
    return 1;   /* BROWSE_DONE */
}

/* Debug "Auto Detail": shows card IDs 1..820 one after another with no pause (fade out, draw, fade in), then
 * fades out and returns 1. Its purpose (a render test of every card) is a hypothesis. */
u16 DebugAutoDetail_Cycle(void)
{
    CardDetail_DrawSprites();
    switch (gMain.seqState1) {
    case CYCLE_START:
        gCardDetail.browseCardId = 1;
        gMain.seqState1++;
        return 0;
    case CYCLE_NEXT:
        if (gCardDetail.browseCardId < LAST_CARD_ID) {
            gCardDetail.browseCardId++;
            gMain.seqState1 = CYCLE_FADE_OUT;
        } else {
            gMain.seqState1++;
        }
        return 0;
    case CYCLE_FADE_OUT:
    case CYCLE_EXIT:
        if (CardDetail_FadeOut())
            gMain.seqState1++;
        return 0;
    case CYCLE_LOAD:
        gCardDetail.cardId = gCardDetail.browseCardId;
        gCardDetail.atk = GetCardAtk(gCardDetail.browseCardId);
        gCardDetail.def = GetCardDef(gCardDetail.browseCardId);
        CardDetail_DrawCard();
        gMain.seqState1++;
        return 0;
    case CYCLE_FADE_IN:
        if (CardDetail_FadeIn())
            gMain.seqState1++;
        return 0;
    }
    return 1;   /* CYCLE_DONE */
}

/* Main callback of the debug menu item "Card Detail": runs gDebugCardDetailSteps[gMain.seqIndex1] and moves to
 * the next step when one returns nonzero. Returns 1 at the end of the list. */
u16 CB_DebugCardDetail(void)
{
    if (gDebugCardDetailSteps[gMain.seqIndex1] != NULL) {
        if (gDebugCardDetailSteps[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* Main callback of the debug menu item "Auto Detail": like CB_DebugCardDetail over gDebugAutoDetailSteps, with
 * gMain.seqState2 = 1 while it runs and 0 at the end. */
u16 CB_DebugAutoDetail(void)
{
    gMain.seqState2 = 1;
    if (gDebugAutoDetailSteps[gMain.seqIndex1] != NULL) {
        if (gDebugAutoDetailSteps[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
        }
        return 0;
    }
    gMain.seqState2 = 0;
    return 1;
}

/* players[player & 1].lifePoints -= amount, clamped at 0. */
void SubtractLifePoints(struct DuelPlayer *players, u32 player, s32 amount)
{
    struct DuelPlayer *duelist = &players[player & 1];

    if (duelist->lifePoints > amount)
        duelist->lifePoints -= amount;
    else
        duelist->lifePoints = 0;
}

/* Clears the transient status bits of a card word (bits 14-18, 20, 21, 23, 24 and 28) when the card leaves
 * the field or the graveyard. The byte view does one byte access per bit, as the ROM does. */
void ClearCardStatusFlags(struct DuelCardStatusBytes *card)
{
    card->unk14 = 0;
    card->normalSummoned = 0;
    card->specialSummoned = 0;
    card->planted = 0;
    card->graverobbed = 0;
    card->isFusionMaterial = 0;
    card->destroyedInBattle = 0;
    card->flag23 = 0;
    card->pendingEquip = 0;
    card->pendingOpponentSummon = 0;
}

/* ClearCardStatusFlags on the card in zone `zone` of `player`. */
void ClearZoneCardStatusFlags(u32 player, u32 zone)
{
    struct DuelZonesPlayer *side = &gDuelZones[player & 1];

    ClearCardStatusFlags((struct DuelCardStatusBytes *)&side->zones[zone]);
}

/* 1 if two card IDs are the same card: the same card number with alternate art (number + 2000) folded, or one
 * of three pairs of numbers that count as one card. */
u32 IsSameCardName(u32 cardId1, u32 cardId2)
{
    /* Pointer arithmetic, not gCardIdToNumber[i]: it computes the index before loading the table address, as
     * the ROM does. */
    u16 cardNo1 = *(gCardIdToNumber + (cardId1 & CARD_ID_MASK));
    u16 cardNo2 = *(gCardIdToNumber + (cardId2 & CARD_ID_MASK));

    if (cardNo1 >= CARD_NUMBER_ALT_ART)
        cardNo1 -= CARD_NUMBER_ALT_ART;
    if (cardNo2 >= CARD_NUMBER_ALT_ART)
        cardNo2 -= CARD_NUMBER_ALT_ART;
    if (cardNo1 == cardNo2)
        return 1;
    switch (cardNo1) {
    case CARD_POLYMERIZATION:
    case CARD_POLYMERIZATION_ALT:
        if (cardNo2 == CARD_POLYMERIZATION || cardNo2 == CARD_POLYMERIZATION_ALT)
            return 1;
        break;
    case CARD_DARK_MAGICIAN:
    case CARD_1210:     /* not in EDS */
        if (cardNo2 == CARD_DARK_MAGICIAN || cardNo2 == CARD_1210)
            return 1;
        break;
    case CARD_HARPIE_LADY:
    case CARD_1249:     /* not in EDS */
        if (cardNo2 == CARD_HARPIE_LADY || cardNo2 == CARD_1249)
            return 1;
        break;
    }
    return 0;
}

/* *dst = *src for a card word. */
void CopyDuelCard(struct DuelCard *dst, const struct DuelCard *src)
{
    *dst = *src;
}

/* Swaps two card words. */
void SwapDuelCards(struct DuelCard *a, struct DuelCard *b)
{
    struct DuelCard tmp = *a;
    *a = *b;
    *b = tmp;
}

/* 1 if card number cardNo is one of the four Toon effect monsters (Toon Alligator is a Normal Monster). */
u32 IsToonMonster(u16 cardNo)
{
    switch (cardNo) {
    case CARD_MANGA_RYU_RAN:
    case CARD_TOON_MERMAID:
    case CARD_TOON_SUMMONED_SKULL:
    case CARD_BLUE_EYES_TOON_DRAGON:
        return 1;
    }
    return 0;
}

/* 1 if card number cardNo has a flip effect that applies; inBattleArg is nonzero when the card was flipped face
 * up by an attack. Invader of the Throne cannot use its effect then; Blast Sphere and Kiseitai only can (they
 * return the flag itself). Only the low half of the flag counts: one caller in ai_steps passes a whole word
 * (0x08622AB4, left over in r1), and for those two cards the result is its low half. */
u32 HasFlipEffect(u16 cardNo, int inBattleArg)
{
    u16 inBattle = inBattleArg;

    switch (cardNo) {
    case CARD_DRAGON_PIPER:
    case CARD_CASTLE_OF_DARK_ILLUSIONS:
    case CARD_REAPER_OF_THE_CARDS:
    case CARD_MASK_OF_DARKNESS:
    case CARD_BIG_EYE:
    case CARD_TRAP_MASTER:
    case CARD_PRINCESS_OF_TSURUGI:
    case CARD_MAGICIAN_OF_FAITH:
    case CARD_THE_IMMORTAL_OF_THUNDER:
    case CARD_ARMED_NINJA:
    case CARD_NEEDLE_BALL:
    case CARD_MAN_EATER_BUG:
    case CARD_SKELENGEL:
    case CARD_HANE_HANE:
    case CARD_NEEDLE_WORM:
    case CARD_WEATHER_REPORT:
    case CARD_GREENKAPPA:
    case CARD_MORPHING_JAR:
    case CARD_PENGUIN_SOLDIER:
    case CARD_HIROS_SHADOW_SCOUT:
    case CARD_HOURGLASS_OF_COURAGE:
    case CARD_DARK_EYES_ILLUSIONIST:
    case CARD_JIGEN_BAKUDAN:
    case CARD_PARASITE_PARACIDE:
    case CARD_THE_STERN_MYSTIC:
    case CARD_CYBER_JAR:
    case CARD_SPEAR_CRETIN:
    case CARD_MORPHING_JAR_2:
    case CARD_1254:     /* 1254-1524: not in EDS */
    case CARD_1255:
    case CARD_1256:
    case CARD_1307:
    case CARD_1330:
    case CARD_1337:
    case CARD_1338:
    case CARD_1435:
    case CARD_1436:
    case CARD_1512:
    case CARD_1521:
    case CARD_1524:
        return 1;
    case CARD_INVADER_OF_THE_THRONE:
        return inBattle == 0;
    case CARD_BLAST_SPHERE:
    case CARD_KISEITAI:
        return inBattle;
    }
    return 0;
}

/* 1 if a card ID is an Effect Monster: a monster of kind CARD_KIND_EFFECT, or one of five card numbers listed by
 * hand (Relinquished and Alligator's Sword Dragon, which are Ritual/Fusion monsters with effects, and three
 * numbers not in EDS). Non-monsters and the Egyptian Gods give 0. */
u32 IsEffectMonster(u16 cardId)
{
    u32 result = 0;

    if (CARD_STATS_TYPE(CARD_STATS_WORD(cardId)) > CARD_TYPE_REPTILE)
        return 0;
    if (GetCardKind(cardId) == CARD_KIND_EFFECT)
        result = 1;
    switch (CARD_NUMBER(cardId)) {
    case CARD_RELINQUISHED:
    case CARD_ALLIGATORS_SWORD_DRAGON:
    case CARD_1241:
    case CARD_1334:
    case CARD_1526:     /* not in EDS */
        result = 1;
    }
    return result;
}

/* 1 if a monster cannot be Normal Summoned or Set: every Fusion and Ritual monster, and the effect monsters
 * listed by card number. Normal monsters, non-monsters and the Egyptian Gods give 0. */
u32 IsSpecialSummonOnly(u16 cardId)
{
    u16 kind;   /* u16: an int here lets GCC jump from the constant kinds straight past the switch head */

    if (CARD_STATS_TYPE(CARD_STATS_WORD(cardId)) > CARD_TYPE_REPTILE)
        return 0;
    kind = GetCardKind(cardId);
    switch (kind) {
    case CARD_KIND_NORMAL:
        return 0;
    case CARD_KIND_FUSION:
    case CARD_KIND_RITUAL:
        goto yes;
    }
    switch (CARD_NUMBER(cardId)) {
    case CARD_LARVAE_MOTH:
    case CARD_GREAT_MOTH:
    case CARD_HARPIE_LADY_SISTERS:
    case CARD_PERFECTLY_ULTIMATE_GREAT_MOTH:
    case CARD_WALL_SHADOW:
    case CARD_GATE_GUARDIAN:
    case CARD_METALZOA:
    case CARD_MANGA_RYU_RAN:
    case CARD_TOON_MERMAID:
    case CARD_TOON_SUMMONED_SKULL:
    case CARD_RED_EYES_BLACK_METAL_DRAGON:
    case CARD_BLUE_EYES_TOON_DRAGON:
    case CARD_VALKYRION_THE_MAGNA_WARRIOR:
    case CARD_DARK_SAGE:
    case CARD_1257:     /* 1257-1519: not in EDS */
    case CARD_1514:
    case CARD_1515:
    case CARD_1516:
    case CARD_1517:
    case CARD_1518:
    case CARD_1519:
    yes:
        return 1;
    }
    return 0;
}
