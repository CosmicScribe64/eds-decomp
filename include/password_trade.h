#ifndef GUARD_PASSWORD_TRADE_H
#define GUARD_PASSWORD_TRADE_H

/*
 * Password scene (enter a card's 8-digit password to get the card) and Card Trading scene (send one trunk
 * card to the other player over the link cable and receive theirs).
 *
 * Code: link_sio.c (password entry, lookup and drawing), password_trade.c (password result steps, Card
 * Trading). Prototypes are the definitions as compiled; units that call a function through a different
 * local prototype keep that view (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "link.h" /* struct LinkSync (embedded in struct CardTradingState) */

/*
 * gPasswordSteps index (gMain.seqIndex1), run by CB_Password. Entries 6, 9 and 12 are NULL (the scene
 * ends). Password_RollAndCheck adds 3 (ERROR) or 6 (USED) to PASSWORD_STEP_ROLL; cancelling the entry
 * screen sets 10 and returns 1, which lands on PASSWORD_STEP_USED_FADE_OUT.
 */
enum PasswordStep {
    PASSWORD_STEP_INIT_VIDEO = 0,     /* Password_InitVideo */
    PASSWORD_STEP_INIT_STATE = 1,     /* Password_InitState */
    PASSWORD_STEP_FADE_IN = 2,        /* Password_FadeIn */
    PASSWORD_STEP_ENTRY = 3,          /* Password_HandleInput */
    PASSWORD_STEP_ROLL = 4,           /* Password_RollAndCheck */
    PASSWORD_STEP_REVEAL = 5,         /* Password_RevealAndGiveCard */
    PASSWORD_STEP_END_GOT_CARD = 6,   /* NULL */
    PASSWORD_STEP_ERROR = 7,          /* Password_ShowError: no card has this password */
    PASSWORD_STEP_ERROR_FADE_OUT = 8, /* Password_FadeOut */
    PASSWORD_STEP_END_ERROR = 9,      /* NULL */
    PASSWORD_STEP_USED = 10,          /* Password_ShowUsed: already redeemed */
    PASSWORD_STEP_USED_FADE_OUT = 11, /* Password_FadeOut */
    PASSWORD_STEP_END_USED = 12,      /* NULL */
};

/* Keypad keys of the Password screen: PasswordState.key and the gPasswordKeypad index. */
enum PasswordKeyId {
    PWKEY_0 = 0,
    PWKEY_1 = 1,
    PWKEY_2 = 2,
    PWKEY_3 = 3,
    PWKEY_4 = 4,
    PWKEY_5 = 5,
    PWKEY_6 = 6,
    PWKEY_7 = 7,
    PWKEY_8 = 8,
    PWKEY_9 = 9,
    PWKEY_GET_CARD = 10, /* look up the entered password */
};

/*
 * gCardTradingSteps index (gMain.seqIndex1), run by CB_CardTrading. HandleInput sets 4 (B) or 6/8 (A);
 * the exchange sets 14 on timeout; picking a card and showing the received one go back to 1. Entries 5,
 * 13 and 16 are NULL.
 */
enum CardTradingStep {
    CARD_TRADING_STEP_CLEAR = 0,             /* CardTrading_ClearState */
    CARD_TRADING_STEP_INIT_VIDEO = 1,        /* CardTrading_InitVideo */
    CARD_TRADING_STEP_FADE_IN = 2,           /* CardTrading_FadeIn */
    CARD_TRADING_STEP_INPUT = 3,             /* CardTrading_HandleInput */
    CARD_TRADING_STEP_EXIT_FADE_OUT = 4,     /* CardTrading_FadeOut (B) */
    CARD_TRADING_STEP_EXIT = 5,              /* NULL: leave the scene */
    CARD_TRADING_STEP_SELECT_FADE_OUT = 6,   /* CardTrading_FadeOut before the card picker */
    CARD_TRADING_STEP_SELECT_CARD = 7,       /* CardTrading_SelectCard */
    CARD_TRADING_STEP_THROW = 8,             /* CardTrading_ThrowCard */
    CARD_TRADING_STEP_EXCHANGE = 9,          /* CardTrading_Exchange */
    CARD_TRADING_STEP_DONE_RETURN = 10,      /* CardTrading_ReverseThrow */
    CARD_TRADING_STEP_DONE_FADE_OUT = 11,    /* CardTrading_FadeOut */
    CARD_TRADING_STEP_SHOW_RECEIVED = 12,    /* CardTrading_ShowReceivedCard (then back to 1) */
    CARD_TRADING_STEP_END_UNUSED = 13,       /* NULL, not reached */
    CARD_TRADING_STEP_TIMEOUT_RETURN = 14,   /* CardTrading_ReverseThrow */
    CARD_TRADING_STEP_TIMEOUT_FADE_OUT = 15, /* CardTrading_FadeOut */
    CARD_TRADING_STEP_END_TIMEOUT = 16,      /* NULL */
};

/*
 * gCardTrading (0x0201F780, 0x28 bytes): Card Trading scene state, cleared by CardTrading_ClearState.
 * hasCard | cursorRow << 1 == 3 means "trade now".
 */
struct CardTradingState {
    u16 tradeCardId;        /* +0x00 card ID picked to send (0 = none) */
    u16 hasCard:1;          /* +0x02 bit 0: a card is picked; shows "Trade it now" and the card sprite */
    u16 cursorRow:1;        /*       bit 1: 0 = "Select a card", 1 = "Trade it now" */
    u16 spinAngle:7;        /*       bits 2-8: card sprite rotation, 128 steps per turn; +1 per frame
                             *       while throwFrame == 0 */
    u16 throwFrame:4;       /*       bits 9-12: throw animation 0-15 (scale 0x100 + 0x20 * n) */
    u16 unused:3;           /*       bits 13-15: never touched */
    u8 unk4[0xC];           /* +0x04 unused; only its address is passed to LinkSyncClose (ignored) */
    struct LinkSync link;   /* +0x10 LinkSyncStep handshake block: tx +0x10, rx +0x14 (rx.data at +0x16 =
                             *       the peer's card number), state +0x18, timeout +0x1A */
    u8 unk1C[2];            /* +0x1C */
    u16 exchangeTimer;      /* +0x1E frames left for the exchange (0x200); at 0 the trade gives up */
    u16 receivedCardId;     /* +0x20 card ID received from the peer */
    u8 linkMsgId;           /* +0x22 handshake message id, 0x50 (set every frame by CB_CardTrading) */
    u8 unk23[5];            /* +0x23 */
};

/* One row of gPasswordArrowFrames (0x08087F08): attr2 of the three arrow sprites per animation frame. */
struct PasswordArrowFrame {
    u16 attr2;   /* +0x0 tile | palette */
    u16 unused;  /* +0x2 always 0 */
};

/* One key of gPasswordKeypad (0x08087E24, enum PasswordKeyId order): highlight sprite and D-pad links. */
struct PasswordKey {
    u8 x;          /* +0x0 highlight sprite x */
    u8 y;          /* +0x1 highlight sprite y */
    u16 unk2;      /* +0x2 always 0 */
    u16 tile:12;   /* +0x4 bits 0-11: highlight attr2 tile (0x00-0x12 big digits, 0x44 GET CARD) */
    u16 value:4;   /*      bits 12-15: key value 0-10 (not read) */
    u8 up:4;       /* +0x6 bits 0-3: key reached with Up */
    u8 down:4;     /*      bits 4-7: key reached with Down */
    u8 left:4;     /* +0x7 bits 0-3: key reached with Left */
    u8 right:4;    /*      bits 4-7: key reached with Right */
};

/*
 * gPassword (0x0201F7B0, 0x18 bytes): Password scene state. Password_InitState clears it. The bitfield
 * containers (u8/u16/u32) are as the units declare them; they decide the generated code.
 */
struct PasswordState {
    u8 digits[8];       /* +0x00 digits on screen (edited; scrambled by the roll) */
    u8 password[8];     /* +0x08 submitted copy: read by FindCardByPassword, copied back after the roll */
    u8 pos:4;           /* +0x10 bits 0-3: digit slot under the cursor, 0-7 */
    u16 key:4;          /*       bits 4-7: selected key (enum PasswordKeyId) */
    u8 blink:6;         /* +0x11 bits 0-5: blink counter (digit cursor, reveal, ERROR/USED: bit 5) */
    u32 keyBlink:6;     /*       word bits 14-19: keypad highlight blink counter */
    u16 timer:12;       /* +0x12 bits 4-15: frame counter of the roll (to 0xB4) and the reveal and
                         *       message steps (to 0x12C); A jumps it to the end */
    u16 card;           /* +0x14 card ID found by FindCardByPassword (0 = wrong password) */
    u16 pad;            /* +0x16 */
};

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char pwtrade_h_check_trade[sizeof(struct CardTradingState) == 0x28 ? 1 : -1];
typedef char pwtrade_h_check_trade_link[(u32)&((struct CardTradingState *)0)->link == 0x10 ? 1 : -1];
typedef char pwtrade_h_check_trade_timer[(u32)&((struct CardTradingState *)0)->exchangeTimer == 0x1E ? 1 : -1];
typedef char pwtrade_h_check_trade_rx[(u32)&((struct CardTradingState *)0)->receivedCardId == 0x20 ? 1 : -1];
typedef char pwtrade_h_check_trade_msg[(u32)&((struct CardTradingState *)0)->linkMsgId == 0x22 ? 1 : -1];
typedef char pwtrade_h_check_arrow[sizeof(struct PasswordArrowFrame) == 0x4 ? 1 : -1];
typedef char pwtrade_h_check_key[sizeof(struct PasswordKey) == 0x8 ? 1 : -1];
typedef char pwtrade_h_check_pw[sizeof(struct PasswordState) == 0x18 ? 1 : -1];
typedef char pwtrade_h_check_pw_copy[(u32)&((struct PasswordState *)0)->password == 0x8 ? 1 : -1];
typedef char pwtrade_h_check_pw_card[(u32)&((struct PasswordState *)0)->card == 0x14 ? 1 : -1];

/* Card Trading scene state (0x0201F780); written by the CardTrading_* steps. */
extern struct CardTradingState gCardTrading;

/* Password scene state (0x0201F7B0); written by the Password_* steps. */
extern struct PasswordState gPassword;

/* --- Password scene (gPasswordSteps, run by CB_Password) --- */

/* Scene callback: runs gPasswordSteps[gMain.seqIndex1]; returns 1 at a NULL entry. */
u32 CB_Password(void);
/* Step 0: video setup (BG1-3, 256-colour BG2), keypad and panel graphics, OBJ tiles. Returns 1. */
u32 Password_InitVideo(void);
/* Step 1: clears gPassword. Returns 1. */
u32 Password_InitState(void);
/* Step 2: display on, fades in. */
u16 Password_FadeIn(void);
/* Step 3, one frame of entry: L/R move the digit slot, the D-pad moves on the keypad, A enters a digit
 * or looks the password up (GET CARD), B deletes or cancels. */
u32 Password_HandleInput(void);
/* Step 4: the slot-machine roll (A skips), then goes to the reveal, ERROR (+3) or USED (+6). */
u32 Password_RollAndCheck(void);
/* Step 5: blinks the digits, slides in the card, marks the password used and adds the card to the
 * trunk. */
u32 Password_RevealAndGiveCard(void);
/* Step 7: blinks "ERROR" over the digits. */
u32 Password_ShowError(void);
/* Steps 8 and 11: fades to black; returns 1 when done. */
u16 Password_FadeOut(void);
/* Step 10: blinks "USED": the password was already redeemed. */
u32 Password_ShowUsed(void);

/* --- Password helpers (link_sio.c) --- */

/* Card ID whose password equals gPassword.password (packed BCD, gCardPasswords), or 0. */
u16 FindCardByPassword(void);
/* Draws the 8 digits of gPassword.digits as 8x16 sprites. */
void Password_DrawDigits(void);
/* Draws the animated cursor under digit slot. */
void Password_DrawSlotCursor(int slot);
/* Highlights the selected keypad key (big digit sprite, or the GET CARD frame). */
void Password_DrawKeyCursor(void);
/* Draws the won card on BG2: frame by card type and the card art. */
void Password_DrawCard(u16 cardId);
/* Draws card cardId's portrait into gMain.bgMapBuffer[screenBlock & 7] at cell (tiles from tileBase), or
 * clears the 10x10-cell block for cardId 0xFFFF. */
void DrawCardPortraitOrClear(u16 screenBlock, u16 cell, u16 cardId, u16 tileBase);

/* --- Card Trading scene (gCardTradingSteps, run by CB_CardTrading) --- */

/* Scene callback: sets linkMsgId and the trade card-list mode, runs gCardTradingSteps[gMain.seqIndex1]. */
u32 CB_CardTrading(void);
/* Step 0: clears gCardTrading. Returns 1. */
u32 CardTrading_ClearState(void);
/* Step 1: video setup, background, buttons and card graphics. */
u32 CardTrading_InitVideo(void);
/* Step 2: draws the menu and fades in. */
u32 CardTrading_FadeIn(void);
/* Step 3: UP/DOWN switch rows (once a card is picked); A goes to the card picker (step 6) or throws the
 * card (step 8), B leaves (step 4). */
u32 CardTrading_HandleInput(void);
/* Steps 4, 6, 11 and 15: draws the menu and fades to black; returns 1 when done. */
u32 CardTrading_FadeOut(void);
/* Step 7: runs the trunk card picker (TradeCardSelect_Run); the picked card becomes tradeCardId. */
u32 CardTrading_SelectCard(void);
/* Step 8: turns the spinning card to angle 0x60, then throws it (throwFrame up to 15). */
u32 CardTrading_ThrowCard(void);
/* Step 9: exchanges card numbers over the link, then adds the received card, removes the sent one and
 * saves; after exchangeTimer frames gives up (step 14). */
u32 CardTrading_Exchange(void);
/* Steps 10 and 14: the card comes back (throwFrame down to 1). */
u32 CardTrading_ReverseThrow(void);
/* Step 12: shows the received card in the Card Detail viewer, then resets the menu (step 1). */
u32 CardTrading_ShowReceivedCard(void);

/* --- Card Trading helpers --- */

/* Draws the "Select a card" button and, when hasCard, "Trade it now" and the spinning card. */
void CardTrading_DrawMenu(int cursorRow, u16 hasCard, int throwFrame);
/* Copies rows x width 4bpp tiles from a linear source to OBJ tile tile (2D mapping, 32-tile rows). */
void CardTrading_LoadObjTiles(const void *src, int tile, int width, int rows);
/* Returns 0. No callers. */
u32 CardTrading_UnusedReturnFalse(void);
/* Returns (u16)CB_CardTrading(). No callers. */
u16 CardTrading_UnusedCallbackWrapper(void);

#endif /* GUARD_PASSWORD_TRADE_H */
