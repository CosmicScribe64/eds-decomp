/*
 * password_trade (0x0807C7C8-0x0807D3D0): the Password scene's result steps and the Card Trading scene
 * (wiki/functions/password-trade-c.md).
 *
 * Both scenes are step machines driven by a scene callback (CB_Password, CB_CardTrading): the callback
 * runs the step function at gMain.seqIndex1 from gPasswordSteps / gCardTradingSteps, and a step that
 * returns non-zero advances seqIndex1 and clears seqState1/seqState2; a NULL table entry ends the scene.
 * The Password entry screen itself lives in link_sio.c; this unit has the steps after it:
 * Password_RollAndCheck runs the slot-machine roll over the eight digits and branches to the card reveal
 * (Password_RevealAndGiveCard), the ERROR message (Password_ShowError) or the USED message
 * (Password_ShowUsed). Card Trading picks a trunk card (CardTrading_SelectCard), throws it over the link
 * cable (CardTrading_ThrowCard, CardTrading_Exchange) and shows the card received in return
 * (CardTrading_ShowReceivedCard); CardTrading_DrawMenu draws its two buttons and the spinning card.
 */
#include "global.h"
#include "legacy/gba.h"                    /* REG_DISPCNT, REG_BG0CNT, REG_BG1CNT, REG_BG2HOFS, REG_IE, REG_IME, REG_MOSAIC, A_BUTTON, B_BUTTON, START_BUTTON, DPAD_UP, DPAD_DOWN */
#include "card_data.h"              /* gCardNames, CARD_NAME_SIZE, CARD_ID_MASK */
#include "save.h"                   /* struct SaveData gSaveData (trunk[].passwordUsed), AddCardToTrunk, RemoveCardFromTrunk, SaveGame */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM, SE_CANCEL, SE_ERROR, SE_PASSWORD_* */
#include "bg.h"                     /* ResetVideo, ClearBgMapBuffers, LoadSystemGfx, LoadBgImage4bppMap1 */
#include "text.h"                   /* TextCanvasInit, TextDrawString, TextCanvasToTiles */
#include "card_detail.h"            /* CardDetail_Init, CardDetail_Run */
#include "util.h"                   /* Random, MemClear16, MemCopy16, StrLenWide */
#include "sprite.h"                 /* AddSprite, AddAffineSprite */
#include "debug.h"                  /* DebugPrintf, DebugPrintFlush */
#include "palette.h"                /* SetBrightnessBlack */
#include "link.h"                   /* struct LinkSync, LinkSyncStart, LinkSyncStep, LinkSyncClose */
#include "password_trade.h"         /* struct CardTradingState gCardTrading, struct PasswordState gPassword, enum PasswordStep / CardTradingStep, the scene prototypes */
#include "deck_edit.h"              /* TradeCardSelect_Run */

/* ---- Local views kept for matching (build/readability/issues/password_trade.md) ---- */

/* gMain (0x03000040) as this unit reads it, a subset of include/main.h with main.h's names. Two fields
 * keep this a local view: pickedCardId (main.h folds +0x4872 into unk4871[3]; this unit reads it as a
 * halfword, the same divergence as deck_edit_prohibit.c) and tilemap (a staging window inside
 * bgMapBuffer[0] that main.h does not name). */
struct Main {
    u8 unk0[6];
    u16 newKeys;                    /* +0x0006 */
    u8 unk8[0x40E - 0x8];
    u16 vblankFlags;                /* +0x040E */
    u8 unk410[4];
    void (*vblankCallback)(void);   /* +0x0414 */
    u8 unk418[0x85C - 0x418];
    u16 tilemap[0x60];              /* +0x85C: tile staging inside gMain.bgMapBuffer[0] (halfword 0x220) */
    u8 unk91C[0x4428 - 0x91C];
    u16 bgHofs[4];                  /* +0x4428: [2] is the BG2 scroll shadow */
    u8 unk4430[0x4859 - 0x4430];
    u8 seqIndex1;                   /* +0x4859: scene step (enum PasswordStep / CardTradingStep) */
    u8 seqState1;                   /* +0x485A */
    u8 seqState2;                   /* +0x485B */
    u8 unk485C[0x4872 - 0x485C];
    u16 pickedCardId;               /* +0x4872: card ID returned by the card picker (TradeCardSelect_Run) */
    u8 mode4874:2;                  /* +0x4874 bits 0-1: card-list mode set for the picker */
    u8 unk4874_2:6;
};
extern struct Main gMain;           /* 0x03000040 */

/* The interrupt vector table at the start of IWRAM; entry 1 is the HBlank handler (cleared while the
 * interrupt registers are rewritten, as in booster_get_pack.c). */
struct IntrTable {
    u32 unk0;                           /* +0x00 */
    void (*hblankCallback)(void);       /* +0x04 */
};
extern struct IntrTable IntrTable;      /* 0x03000000 */

/* Second view of struct CardTradingState's hasCard/cursorRow flag pair as one 2-bit value: the ROM
 * tests both flags with a single `and #3` (3 = "trade now"), which the two separate bitfields would
 * not reproduce. */
struct CardTradingFlagPair {
    u16 tradeCardId;                /* +0x00 */
    u16 hasCardAndCursorRow:2;      /* +0x02 bits 0-1: hasCard | cursorRow << 1 */
    u16 rest:14;
    u8 unk4[0x24];
};

/* No shared header declares PlaySE yet (same local form as duel_response.c and link_sio.c). */
void PlaySE(u32 seId);

/* Matching: palette.h declares FadeToBlack/FadeFromBlack returning u32; this unit's call sites were
 * compiled against the u16-returning form (same wide-view idiom as duel_field_screen.c). */
extern u16 FadeToBlackU16(int step) asm("FadeToBlack");
extern u16 FadeFromBlackU16(int step) asm("FadeFromBlack");

void ResetBgScroll(void);
extern const u8 gCardTradingButtonsPal[];       /* 0x0870B5E0 */
extern const u8 gCardTradingButtonsDimPal[];    /* 0x0870B600 */
extern const u8 gCardTradingCardPal[];          /* 0x0870C620 */
extern const u8 gCardTradingButtonsGfx[];       /* 0x087095E0 */
extern const u8 gCardTradingButtonsDimGfx[];    /* 0x0870A5E0 */
extern const u8 gCardTradingCardGfx[];          /* 0x0870B620 */
extern const u8 gSystemFontPal[];               /* 0x0822C300 */
extern const u8 gCardTradingBgImage[];          /* 0x08707B28 */
extern const u32 gPasswordCardSlideHofs[];      /* 0x08087E88 */
extern const struct PasswordArrowFrame gPasswordArrowFrames[];  /* 0x08087F08 */
extern const u8 gStrDebugThrowItInNow[];        /* 0x08087FA0 */
extern u16 (*const gPasswordSteps[])(void);     /* 0x081A7970 */
extern u16 (*const gCardTradingSteps[])(void);  /* 0x081A79A4 */

u32 Password_RollAndCheck(void)
{
    struct PasswordState *s = &gPassword;

    s->blink = 0x20;
    s->keyBlink = 0x20;
    Password_DrawDigits();
    Password_DrawKeyCursor();
    if (gMain.newKeys & A_BUTTON)
        s->timer = 0xB4;
    if (s->timer++ <= 0xB3) {
        int i;
        u32 base;

        i = 0;
        base = (u32)s;

        /* Matching: the store address is (i + base), index first; digits[i] would swap the add's
         * operands. The % is also load-bearing (the address is computed between rand() and __modsi3). */
        for (; i < 8; i++)
            *(u8 *)(i + base) = Random() % 10;
        gPassword.key = Random() % 10;
        gPassword.pos = Random() & 7;
        if ((gPassword.timer & 0xF) == 8)
            PlaySE(SE_PASSWORD_ROLL);
    } else {
        s->timer = 0;
        s->blink = 0;
        s->keyBlink = 0;
        if (s->card == 0) {
            MemClear16(s, 8);
            gMain.seqIndex1 += 3; /* -> PASSWORD_STEP_ERROR */
        } else if (gSaveData.trunk[s->card].passwordUsed == 0) {
            MemCopy16(s, s->password, 8);
            Password_DrawCard(s->card);
            gMain.bgHofs[2] = 0;
            REG_BG2HOFS = 0;
            return 1;
        } else {
            MemClear16(s, 8);
            gMain.seqIndex1 += 6; /* -> PASSWORD_STEP_USED */
        }
    }
    return 0;
}

u32 Password_RevealAndGiveCard(void)
{
    if (gMain.newKeys & A_BUTTON)
        gPassword.timer = 0x12C;
    if (gPassword.timer <= 0x12B) {
        gPassword.blink++;
        if ((gPassword.blink & 0x1F) == 0x1F)
            PlaySE(SE_PASSWORD_GET_CARD);
        if ((gPassword.blink & 0x1F) <= 0x1C)
            Password_DrawDigits();
        if (gPassword.timer <= 0x1F) {
            gMain.bgHofs[2] = gPasswordCardSlideHofs[gPassword.timer];
            AddSprite(0x1C0070, 0, gPasswordArrowFrames[gPassword.timer].attr2);
            AddSprite(0x4C0070, 0, gPasswordArrowFrames[gPassword.timer].attr2);
            AddSprite(0x7C0070, 0, gPasswordArrowFrames[gPassword.timer].attr2);
        }
        gPassword.timer++;
        return 0;
    }
    switch (gMain.seqState1) {
    case 0:
        Password_DrawDigits();
        if (gMain.newKeys & (A_BUTTON | B_BUTTON))
            gMain.seqState1++;
        return 0;
    case 1:
        Password_DrawDigits();
        if (FadeToBlackU16(4)) {
            gSaveData.trunk[gPassword.card].passwordUsed = 1;
            AddCardToTrunk(gPassword.card);
            CardDetail_Init(gPassword.card, 0, 0);
            gMain.seqState1++;
        }
        return 0;
    case 2:
        if (CardDetail_Run())
            gMain.seqState1++;
        return 0;
    default:
        return 1;
    }
}

u32 Password_ShowError(void)
{
    if (gMain.newKeys & A_BUTTON)
        gPassword.timer = 0x12C;
    if (gPassword.timer <= 0x12B) {
        if (gPassword.timer <= 0xB3) {
            gPassword.blink++;
            if ((gPassword.blink & 0x1F) == 0x1F)
                PlaySE(SE_PASSWORD_REJECT);
        }
        if (gPassword.blink & 0x20) {
            AddSprite(0x80018, 0x40, 0x10CA);
            AddSprite(0x80028, 0x4080, 0x10CC);
        }
        if (!(gMain.newKeys & START_BUTTON)) {
            gPassword.timer++;
            return 0;
        }
    }
    return 1;
}

u16 Password_FadeOut(void)
{
    return FadeToBlackU16(2);
}

u32 Password_ShowUsed(void)
{
    if (gMain.newKeys & A_BUTTON)
        gPassword.timer = 0x12C;
    if (gPassword.timer <= 0x12B) {
        if (gPassword.timer <= 0xB3) {
            gPassword.blink++;
            if ((gPassword.blink & 0x1F) == 0x1F)
                PlaySE(SE_PASSWORD_REJECT);
        }
        if (gPassword.blink & 0x20)
            AddSprite(0x80028, 0x4080, 0x110B);
        if (!(gMain.newKeys & START_BUTTON)) {
            gPassword.timer++;
            return 0;
        }
    }
    return 1;
}
/* Password scene callback: runs the current step of the table. */
u32 CB_Password(void)
{
    u16 (*fn)(void) = gPasswordSteps[gMain.seqIndex1];

    if (fn) {
        if (fn()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}

/* Copies `rows` rows of `width` tiles into OBJ_VRAM0 (stride 0x400 bytes). */
void CardTrading_LoadObjTiles(const void *src, int tile, int width, int rows)
{
    u8 *dst = (u8 *)OBJ_VRAM0 + tile * 32;
    int i;

    for (i = 0; i < rows; i++) {
        MemCopy16(dst, src, width * 32);
        dst += 0x400;
        src = (const u8 *)src + width * 32;
    }
}
void CardTrading_DrawMenu(int cursorRow, u16 hasCard, int throwFrame)
{
    int i, j, limit;

    if (hasCard) {
        limit = 2;
        cursorRow &= 1;
    } else {
        limit = 1;
        cursorRow = 0;
    }
    i = 0;
    while (i < limit) {
        int iy, next;
        u32 yx;

        j = 0;
        iy = i << 7;
        next = i + 1;
        yx = (i << 21) + 0x300000;
        for (; j < 4; j++) {
            int on = 0;
            int x, y;

            if (i == cursorRow)
                on = 1;
            x = 0;
            if (on == 0)
                x = 0x10;
            y = 0;
            if (on == 0)
                y = 0x1000;
            AddSprite(yx | (j << 5), 0x80, (x + j * 4 + iy) | y);
        }
        i = next;
    }
    if (hasCard) {
        AddAffineSprite(0x380098, 0x80, ((throwFrame & 0xC) + 0x100) | 0x2000, ((throwFrame << 21) + 0x1000000) | gCardTrading.spinAngle);
        if (throwFrame == 0)
            gCardTrading.spinAngle = gCardTrading.spinAngle + 1;
    }
}
u32 CardTrading_ClearState(void)
{
    MemClear16(&gCardTrading, 0x28);
    return 1;
}
u32 CardTrading_InitVideo(void)
{
    int len;
    u8 w;
    int half;
    int extent;
    int x;

    gMain.vblankFlags = 3;
    REG_DISPCNT = 0;
    REG_BG0CNT = 4;
    REG_BG1CNT = 0x105;
    ResetVideo();
    ClearBgMapBuffers();
    LoadSystemGfx();
    REG_MOSAIC = 0;
    SetBrightnessBlack();
    ResetBgScroll();
    gMain.vblankCallback = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= ~2;
    IntrTable.hblankCallback = 0;
    REG_IME = 1;
    MemCopy16((void *)OBJ_PLTT, gCardTradingButtonsPal, 0x20);
    MemCopy16((void *)(OBJ_PLTT + 0x20), gCardTradingButtonsDimPal, 0x20);
    MemCopy16((void *)(OBJ_PLTT + 0x40), gCardTradingCardPal, 0x20);
    CardTrading_LoadObjTiles(gCardTradingButtonsGfx, 0, 0x10, 8);
    CardTrading_LoadObjTiles(gCardTradingButtonsDimGfx, 0x10, 0x10, 8);
    CardTrading_LoadObjTiles(gCardTradingCardGfx, 0x100, 0x10, 8);
    MemCopy16((void *)BG_PLTT, gSystemFontPal, 0x20);
    LoadBgImage4bppMap1(0, 0x10, 0x200, (u16 *)gCardTradingBgImage);
    if (gCardTrading.tradeCardId != 0) {
        int i;

        len = StrLenWide((const u16 *)(gCardNames + gCardTrading.tradeCardId * CARD_NAME_SIZE));
        w = 12;
        if (len > 0x12)
            w = 10;
        TextCanvasInit(0x20, 3);
        extent = (len * w) >> 1;
        x = 0x78 - extent;
        half = w >> 1;
        TextDrawString(x, 0xD - half, (w << 8) | 8, gCardNames + gCardTrading.tradeCardId * CARD_NAME_SIZE);
        TextDrawString(0x77 - extent, 0xC - half, (w << 8) | 7, gCardNames + gCardTrading.tradeCardId * CARD_NAME_SIZE);
        TextCanvasToTiles((u16 *)(VRAM + 0x5000), 0);
        for (i = 0; i < 0x60; i++)
            gMain.tilemap[i] = 0x80 + i;
    }
    return 1;
}

u32 CardTrading_FadeIn(void)
{
    CardTrading_DrawMenu(gCardTrading.cursorRow, gCardTrading.hasCard, gCardTrading.throwFrame);
    REG_DISPCNT |= 0x1300;
    return FadeFromBlackU16(2);
}
u32 CardTrading_HandleInput(void)
{
    struct CardTradingState *st = &gCardTrading;

    CardTrading_DrawMenu(st->cursorRow, st->hasCard, st->throwFrame);
    if (gMain.newKeys & B_BUTTON) {
        PlaySE(SE_CANCEL);
        gMain.seqIndex1 = CARD_TRADING_STEP_EXIT_FADE_OUT;
    } else if (gMain.newKeys & A_BUTTON) {
        if (((struct CardTradingFlagPair *)st)->hasCardAndCursorRow != 3)
            gMain.seqIndex1 = CARD_TRADING_STEP_SELECT_FADE_OUT;
        else
            gMain.seqIndex1 = CARD_TRADING_STEP_THROW;
        PlaySE(SE_CONFIRM);
    } else if (gMain.newKeys & (DPAD_UP | DPAD_DOWN)) {
        if (st->hasCard) {
            PlaySE(SE_CURSOR);
            st->cursorRow = 1 - st->cursorRow;
        } else {
            PlaySE(SE_ERROR);
        }
    }
    return 0;
}
u32 CardTrading_UnusedReturnFalse(void)
{
    return 0;
}
u32 CardTrading_FadeOut(void)
{
    CardTrading_DrawMenu(gCardTrading.cursorRow, gCardTrading.hasCard, gCardTrading.throwFrame);
    if (FadeToBlackU16(2) != 0) {
        REG_DISPCNT = 0;
        return 1;
    }
    return 0;
}
u32 CardTrading_SelectCard(void)
{
    if (TradeCardSelect_Run()) {
        if (gMain.pickedCardId != 0) {
            gCardTrading.tradeCardId = gMain.pickedCardId;
            gCardTrading.hasCard = 1;
            gCardTrading.cursorRow = 1;
        } else {
            gCardTrading.tradeCardId = 0;
            gCardTrading.hasCard = 0;
            gCardTrading.cursorRow = 0;
        }
        gMain.seqIndex1 = CARD_TRADING_STEP_INIT_VIDEO;
        gMain.seqState1 = 0;
        gMain.seqState2 = 0;
    }
    return 0;
}
u32 CardTrading_ThrowCard(void)
{
    struct CardTradingState *st = &gCardTrading;

    CardTrading_DrawMenu(st->cursorRow, st->hasCard, st->throwFrame);
    if (st->spinAngle != 0x60) {
        if (st->spinAngle <= 0x5B)
            st->spinAngle += 4;
    } else {
        if (st->throwFrame > 14) {
            LinkSyncStart((u8 *)&st->link);
            DebugPrintf(gStrDebugThrowItInNow);
            DebugPrintFlush();
            st->exchangeTimer = 0x200;
            return 1;
        }
        st->throwFrame++;
    }
    return 0;
}
u32 CardTrading_ReverseThrow(void)
{
    CardTrading_DrawMenu(gCardTrading.cursorRow, gCardTrading.hasCard, gCardTrading.throwFrame);
    if (gCardTrading.throwFrame > 1) {
        gCardTrading.throwFrame--;
        return 0;
    }
    return 1;
}
u32 CardTrading_Exchange(void)
{
    CardTrading_DrawMenu(gCardTrading.cursorRow, gCardTrading.hasCard, gCardTrading.throwFrame);
    /* Matching: gCardIdToNumber (0x08622AB4) and gCardNumberToId (0x08623DF4) stay literal; indexing
     * the card_data.h symbols instead changes register allocation (same lesson as link_sio.c). The
     * peer's card number: 0xFFFF = none, < 2000 plain, 2000 and up wraps with a +1. */
    if (LinkSyncStep(gCardTrading.linkMsgId, ((const u16 *)0x08622AB4)[gCardTrading.tradeCardId & CARD_ID_MASK], &gCardTrading.link)) {
        u16 id;
        u32 n;

        LinkSyncClose(gCardTrading.unk4, &gCardTrading.link.rx);
        id = gCardTrading.link.rx.data;
        if (id == 0xFFFF)
            n = 0;
        else if (id < 2000)
            n = ((const u16 *)0x08623DF4)[id & CARD_ID_MASK];
        else
            n = ((const u16 *)0x08623DF4)[(id - 2000) & CARD_ID_MASK] + 1;
        gCardTrading.receivedCardId = n;
        AddCardToTrunk(gCardTrading.receivedCardId);
        RemoveCardFromTrunk(gCardTrading.tradeCardId);
        SaveGame();
        return 1;
    }
    if (--gCardTrading.exchangeTimer == 0)
        gMain.seqIndex1 = CARD_TRADING_STEP_TIMEOUT_RETURN;
    return 0;
}
u32 CardTrading_ShowReceivedCard(void)
{
    switch (gMain.seqState1) {
    case 0:
        CardDetail_Init(gCardTrading.receivedCardId, 0, 0);
        gMain.seqState1++;
        break;
    case 1:
        if (CardDetail_Run())
            gMain.seqState1++;
        break;
    default:
        gCardTrading.tradeCardId = 0;
        gCardTrading.cursorRow = 0;
        gCardTrading.hasCard = 0;
        gCardTrading.throwFrame = 0;
        gMain.seqIndex1 = CARD_TRADING_STEP_INIT_VIDEO;
        break;
    }
    return 0;
}
/* Card Trading scene callback. */
u32 CB_CardTrading(void)
{
    gCardTrading.linkMsgId = 0x50;
    gMain.mode4874 = 1;
    {
        u16 (*fn)(void) = gCardTradingSteps[gMain.seqIndex1];

        if (fn) {
            if (fn()) {
                gMain.seqIndex1++;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
            }
            return 0;
        }
        return 1;
    }
}
u16 CardTrading_UnusedCallbackWrapper(void)
{
    return CB_CardTrading();
}
