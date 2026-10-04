/*
 * link_sio (0x0807B6B8-0x0807C7C8): sprite helpers, tweens, the SIO link layer and the card
 * password entry screen (wiki/functions/link-sio-c.md).
 *
 * The first half is shared drawing and motion helpers: OamListAddSprite builds one OAM entry
 * of a given pixel size in an OamList, DrawNumberSprites draws a decimal number from digit
 * templates, and TweenInit / TweenUpdate run the 2D tweens (approach or sine eases) used by
 * the turn-order banners. SetBgScrollRegs writes a BG scroll register pair.
 *
 * The middle is the link layer below the packet protocol: LinkSyncOpen / LinkSyncStep /
 * LinkSyncClose exchange one value with the partner GBA, and the Sio* helpers wrap the
 * multi-player SIO registers. LinkSyncStep is also the handshake inside Card Trading.
 *
 * The last third is the password entry screen's drawing and input (the Password_* steps live
 * in password_trade.c): digit and cursor sprites, the won card's frame and portrait, the
 * FindCardByPassword lookup, video setup and the one-frame keypad handler.
 */
#include "global.h"
#include "card_data.h"            /* CARD_ID_MASK, CARD_STATS_TYPE/KIND field extractors (the gCardStats and
                                   gCardNumberToId table bases below stay literal) */
#include "constants/cards.h"      /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/card_stats.h" /* CARD_STATS_KIND_MASK/SHIFT, enum CardType */
#include "constants/sound.h"      /* SE_CANCEL, SE_PASSWORD_CURSOR, SE_PASSWORD_PRESS */
#include "legacy/gba.h"                  /* REG_DISPCNT, REG_BG1CNT..REG_BG3CNT, REG_MOSAIC, REG_BLDCNT, REG_BLDY,
                                   REG_WIN0H, REG_WIN0V, REG_WININ, REG_WINOUT, REG_DMA3SAD/DAD/CNT,
                                   REG_RCNT, REG_SIOCNT, A_BUTTON, B_BUTTON, SELECT_BUTTON, START_BUTTON,
                                   DPAD_UP/DOWN/LEFT/RIGHT, R_BUTTON, L_BUTTON */
#include "legacy/main.h"                 /* struct Main gMain (newKeys, seqIndex1, frameCounter, vblankFlags, bgMapBuffer) */
#include "sprite.h"               /* struct OamList, struct OamListEntry, OamListAlloc, AddSprite,
                                   enum NumberSpriteMode; OamListAddSprite and DrawNumberSprites (defined here) */
#include "util.h"                 /* struct Tween, enum TweenMode, enum TweenState, TweenInit/TweenUpdate (defined
                                   here), MemClear16, gSineTable */
#include "link.h"                 /* struct LinkSync, struct LinkSyncPacket, enum LinkSyncState, enum LinkSyncPhase,
                                   LinkSioInit, LinkSioStop, LinkSioSend; LinkSync and Sio helpers (defined here) */
#include "password_trade.h"       /* struct PasswordState gPassword, struct PasswordKey, enum PasswordStep,
                                   enum PasswordKeyId; the Password helpers (defined here) */


/* IntrTable (0x03000000): LinkSioInit installs the serial handler in slot 0 and the Timer3
 * handler in slot 7 (+0x1C). */
extern u8 IntrTable[];          /* 0x03000000 */
/* Matching: MulFix8 called through int parameters (util.h declares s16; the tween easing passes
 * full-width products of the phase and the sine value). */
extern int MulFix8Int(int a, int b) asm("MulFix8");
/* Matching: this unit calls OamListAddSpriteGroup through full-width parameters (sprite.h
 * declares u16/u8 parameters; the digit x is a computed int). */
extern void OamListAddSpriteGroupWide(u8 *tmpls, u32 layer, u32 count, int x, u16 y, u32 mode,
                                      u8 priority, u8 sheetX, u8 sheetY, u32 format,
                                      u32 attr0Flags, u32 list) asm("OamListAddSpriteGroup");

/* bg.h declares the LoadBgImage* unpackers with u16 parameters; Password_DrawCard passes its
 * arguments in pinned registers, so the wide forms stay local. */
extern void LoadBgImageMap1(u32 mapOffset, u32 palStart, u32 tileBase, void *pack);
extern void ResetVideo(void);
extern void ClearBgMapBuffers(void);
extern void LoadBgImage4bppMap1(u32 mapOffset, u32 palStart, u32 tileBase, void *pack);
extern void SetBrightnessBlack(void);
/* ROM graphics of the password screen (read-only data). */
extern u8 gPasswordKeypadBgGfx[];       /* 0x0863862C */
extern u8 gPasswordPanelBgGfx[];        /* 0x0863975C */
extern u8 gPasswordObjPal[];            /* 0x08639DFC */
extern u8 gPasswordObjGfx[];            /* 0x08639E1C */
extern u16 gPasswordSlotCursorFrames[]; /* 0x08087E7C */

/* gPasswordKeypad: one key per enum PasswordKeyId (struct PasswordKey in password_trade.h). */
extern struct PasswordKey gPasswordKeypad[];   /* 0x08087E24 */
/* The fades: palette.h declares FadeFromBlack u32-returning; the password steps test the
 * narrowed u16 result, so this unit keeps the int form (see bg_image.c). */
extern int FadeFromBlack(int step);

/* The Password_InitState clear view of gPassword: wider containers than struct PasswordState
 * on purpose (the byte/word stores here are the ROM's clearing sequence). */
struct PasswordClearView {
    u8 pad[0x10];
    u8 posAndKey;   /* +0x10 whole byte: pos (bits 0-3) | key << 4 */
    u8 blink : 6;   /* +0x11 */
    u32 keyBlink : 6; /* word at +0x10, bits 14-19 */
    u16 timer : 12;   /* +0x12 bits 4-15 */
    u16 card;         /* +0x14 */
};

/* Matching: the ROM's LinkSioRecv takes (slot, dst); this unit passes the byte count as a third
 * argument (dead register), so the call keeps its own prototype (link.h declares the 2-arg form). */
extern int LinkSioRecv3(int slot, void *dst, int size) asm("LinkSioRecv");
/* Matching: SioGetMultiPlayerId read through a u16 view (link.h declares u32; the narrowed width
 * keeps the xor in r1 in LinkSyncStep). */
extern u16 SioGetMultiPlayerIdU16(void) asm("SioGetMultiPlayerId");


/* Allocates an OAM entry on `layer` and fills attr0-2: y/x, shape/size from the w x h pixel size,
 * tile, palette bank, priority; mode 8 = 256 colours. Returns the entry. */
/* Volatile view of the entry: the ROM reloads attr0/1 in every case instead of reusing the stored value. */
struct OamEntryV {
    vu32 w;
    u16 h;
};
struct OamListEntry *OamListAddSprite(u8 layer, u16 tile, u16 x, int y_, u8 width, u8 height, u8 bpp, u8 palette, u32 unused,
                               u16 attr0Flags, u8 attr1Bits, u8 priority, struct OamList *list)
{
    u16 y;
    struct OamEntryV *e;
    u32 v;

    x &= 0x1FF; /* in place: the longer live range gives x r6 and y r4 */
    y = y_;     /* int param narrowed here, not in the prologue */
    y &= 0xFF;
    e = (struct OamEntryV *)OamListAlloc(layer, list);
    v = attr1Bits << 25;

    if (bpp == 8) {
        v |= 0x2000;
        v |= attr0Flags;
    } else {
        v |= attr0Flags;
    }
    e->w = v;
    switch (width) {
    case 8:
        switch (height) {
        case 8:
            e->w |= x << 16 | y;
            break;
        case 16:
            e->w |= 0x8000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x40008000 | (x << 16 | y);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 16:
        switch (height) {
        case 8:
            e->w |= 0x4000 | (x << 16 | y);
            break;
        case 16:
            e->w |= 0x40000000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x80008000 | (x << 16 | y);
            break;
        case 64:
            while (1)
                ;
        }
        break;
    case 32:
        switch (height) {
        case 8:
            e->w |= 0x40004000 | (x << 16 | y);
            break;
        case 16:
            e->w |= 0x80004000 | (x << 16 | y);
            break;
        case 32:
            e->w |= 0x80000000 | (x << 16 | y);
            break;
        case 64:
            e->w |= 0xC0008000 | (x << 16 | y);
            break;
        }
        break;
    case 64:
        switch (height) {
        case 8:
            while (1)
                ;
        case 16:
            while (1)
                ;
        case 32:
            e->w |= 0xC0004000 | (x << 16 | y);
            break;
        case 64:
            e->w |= 0xC0000000 | (x << 16 | y);
            break;
        }
        break;
    }
    e->h = palette << 12 | tile | priority << 10;
    return (struct OamListEntry *)e;
}
/* Draws a decimal number with sprites (digit sprite table `base`, 8 bytes per digit), right to left. */
void DrawNumberSprites(u16 value, u8 numDigits, u8 mode, u16 x, u16 y, u8 *digitTemplates, u32 unused, u8 spacing, u8 sheetX, u8 sheetY, u8 priority, u32 list)
{
    u8 n;
    u8 drawn = 0;
    u16 d;

    switch (mode) {
    case NUMSPRITE_ZERO_PAD:
        for (n = 0; n < numDigits; n++) {
            d = value % 10;
            value = value / 10;
            OamListAddSpriteGroupWide(digitTemplates + d * 8, 0, 1, x - drawn++ * spacing, y, 2, priority, sheetX, sheetY, 0, 0, list);
        }
        break;
    case NUMSPRITE_NO_LEADING_ZEROS:
        if (value == 0) {
            OamListAddSpriteGroupWide(digitTemplates, 0, 1, x, y, 2, priority, sheetX, sheetY, value, value, list);
        } else {
            for (n = 0; n < numDigits; n++) {
                d = value % 10;
                value = value / 10;
                if (d == 0 && value == 0)
                    return;
                OamListAddSpriteGroupWide(digitTemplates + d * 8, 0, 1, x - drawn++ * spacing, y, 2, priority, sheetX, sheetY, 0, 0, list);
            }
        }
        break;
    }
    value++; /* FAKEMATCH: dead late use of value makes CSE keep value (not drawn) as the zero register */
}
void TweenInit(u16 startX, u16 startY, u16 endX, u16 endY, u16 vxOrFrames, u16 vy, struct Tween *tween, u8 mode)
{
    switch (mode) {
    case TWEEN_EASE_OUT:
    case TWEEN_EASE_IN:
        tween->step = 0x4000 / (s16)vxOrFrames;
        tween->phase = 0;
        tween->state = TWEEN_RUNNING;
        {
            int a = (s16)endX, b2 = (s16)startX;
            tween->dx = a - b2;
        }
        {
            int a = (s16)endY, b2 = (s16)startY;
            tween->dy = a - b2;
        }
        tween->x = startX;
        tween->y = startY;
        tween->startX = startX;
        tween->startY = startY;
        tween->endX = endX;
        tween->endY = endY;
        break;
    case TWEEN_APPROACH:
        tween->x = startX;
        tween->y = startY;
        tween->endX = endX;
        tween->endY = endY;
        tween->step = vxOrFrames;
        tween->phase = vy;
        tween->state = TWEEN_RUNNING;
        break;
    case TWEEN_EASE_IN_OUT:
        tween->step = ((s16)vxOrFrames + 0x7F) / (s16)vxOrFrames;
        tween->phase = 0;
        tween->state = mode;
        {
            int a = (s16)endX, b2 = (s16)startX;
            tween->dx = a - b2;
        }
        {
            int a = (s16)endY, b2 = (s16)startY;
            tween->dy = a - b2;
        }
        tween->x = startX;
        tween->y = startY;
        tween->startX = startX;
        tween->startY = startY;
        tween->endX = endX;
        tween->endY = endY;
        break;
    }
    tween->mode = mode;
}
/* Tween update (see struct Tween): mode 0 approaches (endX,endY) with a +-1 velocity, 1..3 ease along the sine table. */
void TweenUpdate(struct Tween *tween)
{
    int d;

    if (tween->state != TWEEN_RUNNING)
        return;
    switch (tween->mode) {
    case TWEEN_APPROACH:
        d = (s16)tween->x - (s16)tween->endX;
        if (d < 0)
            d = -d;
        if (d <= 7)
            tween->step = (tween->step > 0) ? 1 : -1;
        tween->x += tween->step;
        if (tween->step >= 0) {
            if ((s16)tween->x >= (s16)tween->endX)
                tween->x = tween->endX;
        } else {
            if ((s16)tween->x <= (s16)tween->endX)
                tween->x = tween->endX;
        }
        d = (s16)tween->y - (s16)tween->endY;
        if (d < 0)
            d = -d;
        if (d <= 7)
            tween->phase = (tween->phase > 0) ? 1 : -1;
        tween->y += tween->phase;
        if (tween->phase >= 0) {
            if ((s16)tween->y >= (s16)tween->endY)
                tween->y = tween->endY;
        } else {
            if ((s16)tween->y <= (s16)tween->endY)
                tween->y = tween->endY;
        }
        if (*(u32 *)&tween->x == *(u32 *)&tween->endX)
            tween->state = TWEEN_DONE;
        break;
    case TWEEN_EASE_IN_OUT:
        tween->phase += tween->step;
        if (tween->phase > 0x7F) {
            tween->phase = 0x7F;
            tween->state = TWEEN_DONE;
            tween->x = tween->endX;
            tween->y = tween->endY;
        } else {
            tween->x = tween->startX + (MulFix8Int(tween->dx << 4, (-gSineTable[(u8)tween->phase + 0x40] + 0x100) >> 1) >> 4);
            tween->y = tween->startY + (MulFix8Int(tween->dy << 4, (-gSineTable[(u8)tween->phase + 0x40] + 0x100) >> 1) >> 4);
        }
        break;
    case TWEEN_EASE_OUT:
        tween->phase += tween->step;
        if (tween->phase > 0x3F00) {
            tween->phase = 0x3F00;
            tween->state = TWEEN_DONE;
            tween->x = tween->endX;
            tween->y = tween->endY;
        } else {
            tween->x = tween->startX + (MulFix8Int(tween->dx << 4, gSineTable[(u8)((u16)tween->phase >> 8)]) >> 4);
            tween->y = tween->startY + (MulFix8Int(tween->dy << 4, gSineTable[(u16)tween->phase >> 8]) >> 4);
        }
        /* falls through, as in the original (enum TweenMode notes this) */
    case TWEEN_EASE_IN:
        tween->phase += tween->step;
        if (tween->phase > 0x3F00) {
            tween->phase = 0x3F00;
            tween->state = TWEEN_DONE;
            tween->x = tween->endX;
            tween->y = tween->endY;
        } else {
            tween->x = tween->startX + (MulFix8Int(tween->dx << 4, 0x100 - gSineTable[((u16)tween->phase >> 8) + 0x40]) >> 4);
            tween->y = tween->startY + (MulFix8Int(tween->dy << 4, 0x100 - gSineTable[((u16)tween->phase >> 8) + 0x40]) >> 4);
        }
        break;
    }
}
/* BGnHOFS/VOFS pair: bg selects the register pair (stride 4). */
void SetBgScrollRegs(u32 hofs, u32 vofs, u8 bg)
{
    *(u32 *)(0x04000010 + bg * 4) = (hofs & 0x1FF) | (vofs & 0x1FF) << 16;
}
void LinkSyncStart(u8 *sync)
{
    ((struct LinkSync *)sync)->state = LINKSYNC_OPEN;
}
u32 LinkSyncStep(u16 msgId, u16 data, struct LinkSync *sync)
{
    struct LinkSyncPacket *tx = &sync->tx;
    struct LinkSyncPacket *rx = &sync->rx;
    u8 *st = &sync->state;
    u8 other = 1 ^ SioGetMultiPlayerIdU16();

    switch (sync->state) {
    case LINKSYNC_OPEN:
        if (LinkSyncOpen(tx, rx))
            (*st)++;
        sync->timeout = 300;
        tx->phase = LINKSYNC_PHASE_NONE;
        break;
    case LINKSYNC_SEND:
        tx->msgId = msgId;
        tx->data = data;
        tx->phase = LINKSYNC_PHASE_REQUEST;
        if (LinkSioSend(tx, 4))
            *st = LINKSYNC_WAIT;
        break;
    case LINKSYNC_WAIT:
        if (LinkSioRecv3(other, rx, 4)) {
            switch (rx->phase) {
            case LINKSYNC_PHASE_REQUEST:
                if (rx->msgId != msgId) {
                    *st = LINKSYNC_OPEN;
                    return 0;
                }
                tx->phase = LINKSYNC_PHASE_ACK;
                LinkSioSend(tx, 4);
                sync->timeout = 300;
                *st = LINKSYNC_CLOSE;
                break;
            case LINKSYNC_PHASE_ACK:
                sync->timeout = 300;
                *st = LINKSYNC_CLOSE;
                return 1;
            case LINKSYNC_PHASE_RESTART:
                sync->timeout = 300;
                *st = LINKSYNC_OPEN;
                break;
            default:
                *st = LINKSYNC_OPEN;
                sync->timeout = 300;
                break;
            }
        }
        break;
    case LINKSYNC_RESEND:
        *st = LINKSYNC_SEND;
        break;
    case LINKSYNC_CLOSE:
        LinkSyncClose(tx, rx);
        return 1;
    }
    if (--sync->timeout == 0xFFFF)
        *st = LINKSYNC_OPEN;
    return 0;
}
u16 LinkSyncOpen(void *tx_, void *rx)
{
    u8 i;
    u16 *p = tx_;
    u8 *base = IntrTable;

    LinkSioInit(base, base + 0x1C);
    for (i = 0; i < 2; i++) {
        p[1] = 0;
        p += 2;
    }
    return 1;
}
u32 LinkSyncClose(void *a, void *b)
{
    LinkSioStop();
    MemClear16((void *)0x03005B60, 0xB38);
    return 1;
}
/* Multi-player SIO setup: baud rate & 3, IRQ enable & 1; clears SIOMLT_SEND and SIOMULTI0-3. */
int SioInitMultiPlayer(u8 baud, u8 irq)
{
    u16 i;

    REG_RCNT = 0;
    REG_SIOCNT = (baud & 3) | (((irq & 1) << 15) | 0x2000);
    *(vu16 *)0x0400012A = 0;   /* SIOMLT_SEND (unnamed in gba.h) */
    for (i = 0; i < 4; i++)
        ((vu16 *)0x04000120)[i] = 0;   /* SIOMULTI0-3 (unnamed in gba.h) */
}
u32 SioGetMultiPlayerId(void)
{
    return (REG_SIOCNT & 0x30) >> 4;
}
void SioSetMultiSend(u16 data)
{
    *(vu16 *)0x0400012A = data;   /* SIOMLT_SEND (unnamed in gba.h) */
}
u16 SioGetMultiRecv(u8 playerId)
{
    return ((vu16 *)0x04000120)[playerId];   /* SIOMULTI0-3 (unnamed in gba.h) */
}
u16 SioGetError(void)
{
    return REG_SIOCNT & 0x40;
}
u16 SioAllReady(void)
{
    return REG_SIOCNT & 8;
}
u16 SioIsChild(void)
{
    return REG_SIOCNT & 4;
}
void SioStart(void)
{
    REG_SIOCNT |= 0x80;
}
u16 SioIsBusy(void)
{
    return REG_SIOCNT & 0x80;
}
void Password_DrawDigits(void)
{
    int i;
    u32 v;

    i = 0;
    v = 0x18;
    for (; i < 8; i++) {
        AddSprite(0x80000 | v, 0x8000, gPassword.digits[i] + 0x1080);
        v += 8;
    }
}
void Password_DrawSlotCursor(int slot)
{
    u32 pos = ((slot << 3) + 0x18) | 0x100000;
    u16 *tbl = gPasswordSlotCursorFrames;
    u16 v = (gMain.frameCounter >> 3) % 6;

    AddSprite(pos, 0, tbl[v]);
}
void Password_DrawKeyCursor(void)
{
    int i;

    for (i = 0; i <= 10; i++) {
        u32 attr = 0x1000 | gPasswordKeypad[i].tile;

        if (i == gPassword.key) {
            if (i <= 9) {
                AddSprite(gPasswordKeypad[i].y << 16 | gPasswordKeypad[i].x, 0x40, attr);
            } else {
                AddSprite(0x880030, 0x4040, 0x1044);
                AddSprite(0x880050, 0x4000, 0x1048);
            }
        }
    }
}
/* Fill or clear the ten-row portrait map, then unpack the card's 6bpp art. */
/* Card portrait (hypothesis: RenderCardPortrait): fills a 9 x 10 tile block of the BG map
 * at 0x0300045C + (a & 7) * 0x800 + b * 2 with ascending tile numbers from c / 2 (or clears
 * a 10 x 10 block when id == 0xFFFF), copies card id's 64-colour palette to 0x05000100 and
 * unpacks its 6bpp image (720 x 3 halfwords) to 0x06004000 + c * 32 with bit 7 set.
 * The ROM tables are integer addresses so GCSE does not hoist their pool loads; t is an int
 * (its 0x3F mask then shares the SImode constant with the other two masks) and is shifted
 * as u16 to keep the logical shift separate from y >> 8. */
void DrawCardPortraitOrClear(u16 screenBlock, u16 cell, u16 cardId, u16 tileBase)
{
    u16 *map = (u16 *)((u8 *)gMain.bgMapBuffer + (screenBlock & 7) * 0x800);
    u16 i;
    u16 j;
    u16 tile;
    u32 off;
    const u16 *src;
    u16 *dst;

    map += cell;
    if (cardId == 0xFFFF) {
        i = 0;
        do {
            map[0] = 0;
            map[1] = 0;
            map[2] = 0;
            map[3] = 0;
            map[4] = 0;
            map[5] = 0;
            map[6] = 0;
            map[7] = 0;
            map[8] = 0;
            map[9] = 0;
            map += 0x20;
            i++;
        } while (i <= 9);
    } else {
        i = 0;
        tile = tileBase >> 1;
        off = tileBase << 5;
        do {
            for (j = 0; j <= 8; j++)
                map[j] = tile++;
            map += 0x20;
            i++;
        } while (i <= 9);
        cardId &= CARD_ID_MASK;
        /* Matching: the art palette/graphics table bases stay literal (0x08608360/0x082A6500) so
         * GCSE does not hoist their pool loads. */
        MemCopy16(0x05000100, (void *)(0x08608360 + cardId * 0x80), 0x80);
        src = (const u16 *)(0x082A6500 + cardId * 0x10E0);
        dst = (u16 *)(0x06004000 + off);

        for (i = 0; i <= 0x2CF; i++) {
            u32 x = src[0];
            u32 y = src[1];
            u32 z = src[2];
            int t;
            u16 p0, p1, p2, p3;

            p0 = (x & 0x3F) | (x & 0xFC0) << 2;
            p1 = x >> 12 | (y & 3) << 4 | (y & 0xFC) * 64;
            t = y >> 8;
            p2 = (t & 0x3F) | (((u16)t >> 6) | (z & 0xF) << 2) << 8;
            z >>= 4;
            p3 = (z & 0x3F) | (z & 0xFC0) << 2;
            dst[0] = p0 | 0x8080;
            dst[1] = p1 | 0x8080;
            dst[2] = p2 | 0x8080;
            dst[3] = p3 | 0x8080;
            src += 3;
            dst += 4;
        }
    }
}
/* Matching: the gCardStats/gCardNumberToId table bases stay literal (0x08621DE0/0x08622AB4);
 * indexing the header symbols changes register allocation (see ai_steps). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) ((int)CARD_STATS_TYPE(CARD_STATS(id)))

/* Loads the card frame graphics for card `id` (Trap/Magic/Ticket frames, else by card number / kind). */
void Password_DrawCard(u16 id)
{
    void *tbl;
    int v;
    int n;

    switch (CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
        tbl = gCardFrameTrapGfx;
        goto load;
    case CARD_TYPE_MAGIC:
        tbl = gCardFrameMagicGfx;
        goto load;
    case CARD_TYPE_TICKET:
        tbl = gCardFrameTicketGfx;
        goto load;
    }
    n = ((const u16 *)0x08622AB4)[id & CARD_ID_MASK];
    switch (n) {
    case CARD_OBELISK_THE_TORMENTOR:
        v = 3;
        break;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        v = 1;
        break;
    default:
        switch (CARD_TYPE(id)) {
        case CARD_TYPE_MAGIC:
            v = 7;
            break;
        case CARD_TYPE_TRAP:
            v = 8;
            break;
        case CARD_TYPE_TICKET:
            v = 9;
            break;
        default:
            v = CARD_STATS_KIND(CARD_STATS(id));
            break;
        }
        break;
    }
    switch (v) {
    case 1:
        tbl = gCardFrameEffectGfx;
        break;
    case 2:
        tbl = gCardFrameFusionGfx;
        break;
    case 3:
        tbl = gCardFrameRitualGfx;
        break;
    default:
        tbl = gCardFrameNormalGfx;
        break;
    }
load:
    {
        register u32 a asm("r0") = 0x420;
        register u32 b asm("r1") = 0x20;
        LoadBgImageMap1(a, b, 0x100, tbl);
    }
    DrawCardPortraitOrClear(2, 0xA2, id, 0x300);
}
u16 FindCardByPassword(void)
{
    u32 first;
    u8 buf[4];
    u32 i;
    u32 n;
    u8 *d;
    u8 *sp;
    u8 *e;
    /* FAKEMATCH: keep the stack-buffer pointer in r4 while the scan counts in r3. */
    register u8 *bufp __asm__("r4");
    u8 *base;

    i = 0;
    base = (u8 *)&gPassword;
    d = buf;
    sp = base + 8; /* gPassword.password, the submitted copy */
    for (; i < 4; i++) {
        *d = *sp << 4;
        *d |= sp[1];
        d++;
        sp += 2;
    }
    n = 0;
    bufp = buf;
    first = bufp[0];
    for (; n <= 0x334; n++) {
        e = (u8 *)0x08623120 + n * 4; /* gCardPasswords: 4 packed-BCD bytes per card */
        if (e[0] == first && e[1] == bufp[1] && e[2] == bufp[2] && e[3] == bufp[3])
            return n;
    }
    return 0;
}
u32 Password_InitVideo(void)
{
    vu32 *dma;

    ResetVideo();
    ClearBgMapBuffers();
    REG_DISPCNT = 0;
    REG_BG1CNT = 0x105;
    REG_BG2CNT = 0x286;
    REG_BG3CNT = 0x307;
    gMain.vblankFlags = 0x43;
    REG_MOSAIC = 0;
    REG_BLDCNT = 0;
    REG_BLDY = 0;
    REG_BG0VOFS = 0;
    REG_BG0HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    *(vu16 *)0x04000028 = 0;   /* BG2 affine reference point X (unnamed in gba.h) */
    *(vu16 *)0x0400002A = 0;
    *(vu16 *)0x0400003C = 0;   /* BG3 affine reference point X (unnamed in gba.h) */
    *(vu16 *)0x0400003E = 0;
    LoadBgImage4bppMap1(0, 0x10, 0x20, gPasswordKeypadBgGfx);
    LoadBgImage4bppMap1(0x800, 0x10, 0x88, gPasswordPanelBgGfx);
    MemCopy16(0x05000220, gPasswordObjPal, 0x20);
    dma = (vu32 *)&REG_DMA3SAD;
    dma[0] = (u32)gPasswordObjGfx;
    dma[1] = 0x06010000;
    dma[2] = 0x80001600;
    dma[2];
    while (dma[2] & 0x80000000)
        ;
    SetBrightnessBlack();
    return 1;
}
u32 Password_InitState(void)
{
    struct PasswordClearView *s = (struct PasswordClearView *)&gPassword;
    u16 mask;
    int zero;

    MemClear16(s, 0x18);
    mask = 15;
    zero = 0;
    /* Schedule the shared clearing constants before the field stores. */
    __asm__ __volatile__("" : : "r"(mask), "r"(zero));
    s->posAndKey = 0;
    s->blink = 0;
    s->keyBlink = 0;
    s->timer = 0;
    s->card = 0;
    return 1;
}
u16 Password_FadeIn(void)
{
    REG_DISPCNT = 0x1E00;
    REG_MOSAIC = 0;
    return FadeFromBlack(2);
}
extern const u8 gStrDebugPassword[];        /* 0x08087F88 */
extern const u8 gStrDebugPasswordDigit[];   /* 0x08087F94 */
extern const u8 gStrDebugPasswordCard[];    /* 0x08087F98 */
void PlaySE(u32 seId);
/* debug.h declares DebugPrintf(const char *, ...); the debug format strings here are u8 data. */
void DebugPrintf(const u8 *fmt, ...);
void DebugPrintFlush(void);
/* Password entry, one frame: L/R move the cursor, the d-pad moves between keys, A enters a digit (or looks up
 * the password on OK), B deletes or cancels. Returns 1 when the screen is done. */
u32 Password_HandleInput(void)
{
    int i;

    Password_DrawDigits();
    Password_DrawSlotCursor(gPassword.pos);
    Password_DrawKeyCursor();
    gPassword.blink++;
    gPassword.keyBlink = 0x20;
    if (gMain.newKeys & L_BUTTON) {
        gPassword.pos = (gPassword.pos + 7) & 7;
        gPassword.blink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & R_BUTTON) {
        gPassword.pos = (gPassword.pos + 9) & 7;
        gPassword.blink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & DPAD_UP) {
        gPassword.key = gPasswordKeypad[gPassword.key].up;
        gPassword.keyBlink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & DPAD_DOWN) {
        gPassword.key = gPasswordKeypad[gPassword.key].down;
        gPassword.keyBlink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & DPAD_LEFT) {
        gPassword.key = gPasswordKeypad[gPassword.key].left;
        gPassword.keyBlink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        gPassword.key = gPasswordKeypad[gPassword.key].right;
        gPassword.keyBlink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_PASSWORD_PRESS);
        if ((u32)gPassword.key <= 9) {
            gPassword.digits[gPassword.pos] = gPassword.key;
            AddSprite((gPasswordKeypad[gPassword.key].x - 4) | ((gPasswordKeypad[gPassword.key].y - 12) << 16), 0x80, 0x10E0);
            if (gPassword.pos <= 6) {
                gPassword.pos++;
                gPassword.blink = 0x20;
            } else {
                gPassword.key = PWKEY_GET_CARD;
                gPassword.keyBlink = 0x20;
            }
        } else {
            AddSprite(0x780024, 0x80, 0x10E3);
            AddSprite(0x780044, 0x80, 0x10E7);
            MemCopy16(gPassword.password, gPassword.digits, 8);
            gPassword.card = FindCardByPassword();
            gPassword.timer = 0;
            DebugPrintf(gStrDebugPassword);
            for (i = 0; i <= 7; i++)
                DebugPrintf(gStrDebugPasswordDigit, gPassword.password[i] + '0');
            DebugPrintf(gStrDebugPasswordCard, gPassword.card);
            DebugPrintFlush();
            return 1;
        }
    }
    if (gMain.newKeys & B_BUTTON) {
        if (gPassword.pos == 0) {
            /* Same tail as the L/R+B cancel below; cross-jumping merges the two copies, and keeping
             * them separate stops GCSE from reusing this block's 0x03000040 register below. */
            PlaySE(SE_CANCEL);
            gMain.seqIndex1 = PASSWORD_STEP_USED;
            return 1;
        }
        gPassword.pos--;
        gPassword.blink = 0x20;
        PlaySE(SE_PASSWORD_CURSOR);
    }
    if (gMain.newKeys & (SELECT_BUTTON | START_BUTTON)) {
        PlaySE(SE_CANCEL);
        gMain.seqIndex1 = PASSWORD_STEP_USED;
        return 1;
    }
    return 0;
}
