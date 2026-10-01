#include "global.h"
#include "gba.h"
#include "main.h"
#include "duel.h"

/* Card Detail viewer (debug menu item 1) plus a few duel/card helpers. */

/* gMain (0x03000040): canonical layout in include/main.h. */
#define gMain gUnk_03000040

extern void (*gUnk_03000000[])(void);
#define IntrTable gUnk_03000000

/* Card Detail viewer state, 0x02013D90, 0x44 bytes. */
struct CardDetail {
    u8 useHBlank:1;         /* +0x00 bit 0 */
    u16 state:15;           /* +0x00 bits 1-15: sub_08006D08 step */
    u16 cardId;             /* +0x02 */
    u16 timer;              /* +0x04: auto-close countdown (0 = none) */
    u8 filler6[0x2C - 0x6];
    s32 atk;                /* +0x2C */
    s32 def;                /* +0x30 */
    s32 scrollPos;          /* +0x34 */
    s32 scrollTarget;       /* +0x38 */
    s32 scrollMax;          /* +0x3C */
    u16 unk40;              /* +0x40: card ID (Auto Detail) */
    u8 filler42[2];
};
extern struct CardDetail gUnk_02013D90;
#define gCardDetail gUnk_02013D90

extern const u32 gUnk_08621DE0[];    /* card stats, indexed by card ID */
#define gCardStats ((const u32 *)0x08621DE0)
extern const u16 gUnk_08622AB4[];    /* maps card ID to card number */
/* Indexed through a pointer rather than the array itself, which evaluates the
 * index before the table address, as the ROM's register allocation needs. */
#define gCardIdToNumber ((const u16 *)gUnk_08622AB4)
/* Same table as a constant address, so GCC reloads it at every use instead of CSEing it. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/* Card numbers 1920-1999 are tokens. */
#define IS_TOKEN(id) ((u16)(CARD_NUMBER(id) - 1920) < 80)

#define CARD_TYPE(stats) (((stats) & 0x1F00000) >> 20)
#define CARD_ATK(stats)  (((stats) << 14) >> 23)
#define CARD_DEF(stats)  ((stats) & 0x1FF)

extern u16 (*const gUnk_08198D50[])(void);  /* Card Detail steps */
extern u16 (*const gUnk_08198D5C[])(void);  /* Auto Detail steps */

/* Local view for sub_0800743C only. The canonical struct DuelZone (include/duel.h)
 * starts with a 0x00 card word and 0x04 serial, but this unit clears status flags in
 * byte offsets 1..3 of each 0x94-byte slot, so those accesses cannot use the header. */
struct DuelSlotFlags {
    u8 unk0;
    u8 unk1_0:6;
    u8 flag1_6:1;
    u8 flag1_7:1;
    u8 flag2_0:1;
    u8 flag2_1:1;
    u8 flag2_2:1;
    u8 flag2_3:1;
    u8 flag2_4:1;
    u8 flag2_5:1;
    u8 flag2_6:1;
    u8 flag2_7:1;
    u8 flag3_0:1;
    u8 flag3_1:3;
    u8 flag3_4:1;
    u8 flag3_5:3;
    u8 filler4[0x94 - 4];
};

void sub_08006878(void);

/* Displayed ATK/DEF: 0 for Trap/Magic/Ticket, 4000 for the Divine cards. */
static inline s32 GetCardAtk(u16 id)
{
    switch ((u8)CARD_TYPE(gCardStats[id & 0x7FF])) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return CARD_ATK(gCardStats[id & 0x7FF]) * 10;
    }
}

static inline s32 GetCardDef(u16 id)
{
    switch ((u8)CARD_TYPE(gCardStats[id & 0x7FF])) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return CARD_DEF(gCardStats[id & 0x7FF]) * 10;
    }
}
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(gCardStats[id & 0x7FF])) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (gCardStats[id & 0x7FF] & 0xC0000) >> 18;
    }
}

void sub_08075278(void *dst, u32 size);          /* MemClear16 */
void sub_080064AC(void);
u16 sub_08075A6C(u16 step);                       /* FadeToBlack */
u16 sub_08075AE4(u16 step);                       /* FadeFromBlack */
void sub_08077AEC(u16 se);                        /* PlaySE */
void sub_0800743C(struct DuelSlotFlags *slot);
u16 sub_0800696C(void);
void sub_08073574(void);                          /* ResetVideo */
void sub_080757AC(void);                          /* ResetBgScroll */
void sub_080759F4(void);                          /* SetBrightnessBlack */
void sub_08075630(void);                          /* LoadSystemFontGfx */
void sub_080752B0(void *dst, const void *src, u32 size); /* CopyDoubleWords */
void sub_08005818(void);                          /* HBlank handler */
extern const u8 gUnk_0863840C[];
extern const u8 gUnk_08198A50[];
void sub_08075294(void *dst, const void *src, u32 size); /* MemCopy16 */
void sub_080757F4(void);                          /* ClearBlend */
u16 sub_08006AE8(void);
u16 sub_08006ABC(void);
extern const u8 gUnk_08631558[], gUnk_0862EEC0[], gUnk_08633BF0[];
extern const u8 gUnk_08627AF8[], gUnk_0862A190[], gUnk_0862C828[], gUnk_08625460[];
void sub_08005A70(u16 cardId);
void sub_08072EB0(u16 tileBase, u32 b, u32 c, const void *gfx);
void sub_08072D28(u32 a, u32 b, u32 c, u32 d, u32 e);
extern const u8 gUnk_0863842C[];

/* Clear the Card Detail state. */
void sub_08006878(void)
{
    sub_08075278(&gCardDetail, sizeof(gCardDetail));
}

/* Set up the Card Detail viewer for a card: displayed ATK/DEF (0 for Trap/Magic/Ticket,
 * 4000 for the Divine cards), an auto-close timer and the HBlank flag. */
void sub_0800688C(u16 cardId, u16 timer, u16 useHBlank)
{
    sub_08006878();
    gCardDetail.cardId = cardId;
    gCardDetail.atk = GetCardAtk(cardId);
    gCardDetail.def = GetCardDef(cardId);
    gCardDetail.timer = timer;
    gCardDetail.useHBlank = useHBlank;
}
/* Card Detail step 0: set up video (BG0-3, font, sprite gfx) and, if requested,
 * install the HBlank handler sub_08005818. */
u16 sub_0800696C(void)
{
    gMain.vblankFlags = 0x803;
    REG_DISPCNT = 0x40;
    sub_08073574();
    REG_MOSAIC = 0;
    REG_BG0CNT = 0x44;
    REG_BG1CNT = 0x186;
    REG_BG2CNT = 0x285;
    REG_BG3CNT = 0x8405;
    sub_080757AC();
    gCardDetail.scrollPos = 0;
    sub_080759F4();
    sub_08075630();
    sub_080752B0((void *)0x05000260, gUnk_0863840C, 0x20);
    sub_080752B0((void *)0x06010600, gUnk_0863842C, 0x200);
    gMain.vblankCallback = NULL;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    IntrTable[1] = NULL;
    REG_IME = 1;
    if (gCardDetail.useHBlank) {
        gMain.hblankY = -0x20;
        REG_IME = 0;
        REG_IE &= 0xFFFD;
        IntrTable[1] = sub_08005818;
        REG_IME = 1;
        REG_IME = 0;
        REG_IE |= 2;
        REG_IME = 1;
    }
    return 1;
}
u16 sub_08006A98(void)
{
    REG_DISPCNT |= 0x1F00;
    sub_080064AC();
    return sub_08075AE4(4);
}
u16 sub_08006ABC(void)
{
    sub_080064AC();
    if (sub_08075A6C(4)) {
        REG_DISPCNT &= 0xE0FF;
        return 1;
    }
    return 0;
}
/* Card Detail main step: A/B closes, timer auto-closes, Up/Down scroll the text. */
u16 sub_08006AE8(void)
{
    sub_080064AC();
    if (gMain.newKeys & (A_BUTTON | B_BUTTON)) {
        sub_08077AEC(2);
        return 1;
    }
    if (gCardDetail.timer != 0) {
        if (--gCardDetail.timer == 0)
            return 1;
    }
    if ((gMain.newKeys & DPAD_UP) && gCardDetail.scrollTarget > 0)
        gCardDetail.scrollTarget = 0;
    if ((gMain.newKeys & DPAD_DOWN) && gCardDetail.scrollTarget < gCardDetail.scrollMax - 0x70)
        gCardDetail.scrollTarget = gCardDetail.scrollMax - 0x70;
    if (gCardDetail.scrollPos != gCardDetail.scrollTarget) {
        if (gCardDetail.scrollPos < gCardDetail.scrollTarget)
            gCardDetail.scrollPos += 2;
        else
            gCardDetail.scrollPos -= 2;
    }
    gMain.bgVofs[3] = gCardDetail.scrollPos;
    return 0;
}
/* Card frame graphics for a card: by type for Magic/Trap/Ritual, else by subtype. */
static inline const void *GetCardFrameGfx(u16 id)
{
    switch ((u8)CARD_TYPE(gCardStats[id & 0x7FF])) {
    case 21:
        return gUnk_08631558;
    case 22:
        return gUnk_0862EEC0;
    case 23:
        return gUnk_08633BF0;
    default:
        switch (GetCardSubtype(id)) {
        case 1:
            return gUnk_08627AF8;
        case 2:
            return gUnk_0862A190;
        case 3:
            return gUnk_0862C828;
        default:
            return gUnk_08625460;
        }
    }
}

/* Draw the card: frame graphics (none for tokens), then the card itself. */
void sub_08006B80(void)
{
    u32 pal = gCardDetail.useHBlank * 9;

    sub_08005A70(gCardDetail.cardId);
    if (!IS_TOKEN(gCardDetail.cardId))
        sub_08072EB0(pal | 0x440, 0x20, 0x10, GetCardFrameGfx(gCardDetail.cardId));
    sub_08072D28(1, pal + 0xC2, gCardDetail.cardId, 0x130, 0x80);
}
/* Card Detail callback: setup, fade/mosaic-in (HBlank variant), main, fade out. */
u16 sub_08006D08(void)
{
    switch (gCardDetail.state) {
    case 0:
        if (sub_0800696C()) {
            sub_08006B80();
            gCardDetail.state++;
        }
        return 0;
    case 1:
        if (!gCardDetail.useHBlank) {
            if (sub_08006A98())
                gCardDetail.state++;
        } else {
            REG_DISPCNT |= 0x1F00;
            if (gMain.hblankY < 0xA0) {
                s32 y = gMain.hblankY + 0x20;
                s32 level = 15 - y / 12;

                REG_BLDY = 0x18 - y / 8;
                REG_MOSAIC = ((level & 0xF) << 4) | (level & 0xF);
                sub_08075294(gMain.hblankScroll, gUnk_08198A50 + ((gMain.hblankY + 0x20) / 8) * 0x20, 0x20);
                gMain.hblankY += 0x10;
            } else {
                REG_BG0CNT &= 0xFFBF;
                REG_MOSAIC = 0;
                sub_080757F4();
                sub_080757AC();
                REG_IME = 0;
                REG_IE &= 0xFFFD;
                REG_IME = 1;
                REG_IME = 0;
                REG_IE &= 0xFFFD;
                IntrTable[1] = NULL;
                REG_IME = 1;
                gCardDetail.state++;
            }
        }
        return 0;
    case 2:
        if (sub_08006AE8())
            gCardDetail.state++;
        return 0;
    case 3:
        if (sub_08006ABC())
            gCardDetail.state++;
        return 0;
    }
    return 1;
}
/* Auto Detail step 0 (hypothesis): show card 800, then run the viewer steps. */
u16 sub_08006E94(void)
{
    switch (gMain.seqState1) {
    case 0:
        sub_08075278(&gCardDetail, sizeof(gCardDetail));
        gCardDetail.cardId = gCardDetail.unk40 = 800;
        gCardDetail.atk = GetCardAtk(800);
        gCardDetail.def = GetCardDef(gCardDetail.unk40);
        gMain.seqState1++;
        break;
    case 1:
        if (sub_0800696C()) {
            sub_08006B80();
            gMain.seqState1++;
        }
        break;
    default:
        return sub_08006A98();
    }
    return 0;
}
/* Interactive card browser: Left/Right step the card ID by 1, L/R (held) by 10, within 1..0x334. */
u16 sub_08006FAC(void)
{
    u32 changed;

    sub_080064AC();
    switch (gMain.seqState1) {
    case 0:
        changed = 0;
        if ((gMain.newKeys & DPAD_RIGHT) && gCardDetail.unk40 < 0x334) {
            gCardDetail.unk40++;
            changed = 1;
        }
        if (gMain.heldKeys & R_BUTTON) {
            if (gCardDetail.unk40 < 0x32A)
                gCardDetail.unk40 += 10;
            else
                gCardDetail.unk40 = 0x334;
            changed = 1;
        }
        if ((gMain.newKeys & DPAD_LEFT) && gCardDetail.unk40 > 1) {
            gCardDetail.unk40--;
            changed = 1;
        }
        if (gMain.heldKeys & L_BUTTON) {
            if (gCardDetail.unk40 > 10)
                gCardDetail.unk40 -= 10;
            else
                gCardDetail.unk40 = 1;
            changed = 1;
        }
        if (sub_08006AE8())
            gMain.seqState1 = 10;
        else if (changed)
            gMain.seqState1 = 1;
        return 0;
    case 1:
    case 10:
        if (sub_08006ABC())
            gMain.seqState1++;
        return 0;
    case 2:
        gCardDetail.cardId = gCardDetail.unk40;
        gCardDetail.atk = GetCardAtk(gCardDetail.unk40);
        gCardDetail.def = GetCardDef(gCardDetail.unk40);
        sub_08006B80();
        gMain.seqState1++;
        return 0;
    case 3:
        if (sub_08006A98())
            gMain.seqState1 = 0;
        return 0;
    }
    return 1;
}
/* Card Detail slideshow (hypothesis): show every card ID 1..0x334 in turn. */
u16 sub_080071F8(void)
{
    sub_080064AC();
    switch (gMain.seqState1) {
    case 0:
        gCardDetail.unk40 = 1;
        gMain.seqState1++;
        return 0;
    case 4:
        if (gCardDetail.unk40 < 0x334) {
            gCardDetail.unk40++;
            gMain.seqState1 = 1;
        } else {
            gMain.seqState1++;
        }
        return 0;
    case 1:
    case 5:
        if (sub_08006ABC())
            gMain.seqState1++;
        return 0;
    case 2:
        gCardDetail.cardId = gCardDetail.unk40;
        gCardDetail.atk = GetCardAtk(gCardDetail.unk40);
        gCardDetail.def = GetCardDef(gCardDetail.unk40);
        sub_08006B80();
        gMain.seqState1++;
        return 0;
    case 3:
        if (sub_08006A98())
            gMain.seqState1++;
        return 0;
    }
    return 1;
}
/* CB "Card Detail" (debug menu): step runner over gUnk_08198D50, index gMain.seqIndex1. */
u16 sub_0800736C(void)
{
    if (gUnk_08198D50[gMain.seqIndex1] != NULL) {
        if (gUnk_08198D50[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return 1;
}
/* CB "Auto Detail" (debug menu): like sub_0800736C over gUnk_08198D5C, with seqState2 = 1. */
u16 sub_080073BC(void)
{
    gMain.seqState2 = 1;
    if (gUnk_08198D5C[gMain.seqIndex1] != NULL) {
        if (gUnk_08198D5C[gMain.seqIndex1]()) {
            gMain.seqIndex1++;
            gMain.seqState1 = 0;
        }
        return 0;
    }
    gMain.seqState2 = 0;
    return 1;
}
/* Subtract `amount` from a player's life points, clamped at 0. */
void sub_08007418(struct DuelPlayer *players, u32 player, s32 amount)
{
    struct DuelPlayer *p = &players[player & 1];
    if (p->lifePoints > amount)
        p->lifePoints -= amount;
    else
        p->lifePoints = 0;
}
/* Clear a set of per-slot status flag bits. */
void sub_0800743C(struct DuelSlotFlags *slot)
{
    slot->flag1_6 = 0;
    slot->flag1_7 = 0;
    slot->flag2_0 = 0;
    slot->flag2_1 = 0;
    slot->flag2_2 = 0;
    slot->flag2_4 = 0;
    slot->flag2_5 = 0;
    slot->flag2_7 = 0;
    slot->flag3_0 = 0;
    slot->flag3_4 = 0;
}
void sub_0800747C(u32 player, u32 slot)
{
    struct DuelZonesPlayer *p = &gUnk_0201930C[player & 1];
    sub_0800743C((struct DuelSlotFlags *)&p->zones[slot]);
}
/* Nonzero if two card IDs are the same card: same card number (alternate art, +2000,
 * folded), or one of three pairs of card numbers treated as equivalent. */
u32 sub_080074A0(u32 id1, u32 id2)
{
    u16 no1 = gCardIdToNumber[id1 & 0x7FF];
    u16 no2 = gCardIdToNumber[id2 & 0x7FF];

    if (no1 > 1999)
        no1 -= 2000;
    if (no2 > 1999)
        no2 -= 2000;
    if (no1 == no2)
        return 1;
    switch (no1) {
    case 1003:
    case 1034:
        if (no2 == 1003 || no2 == 1034)
            return 1;
        break;
    case 34:
    case 1210:
        if (no2 == 34 || no2 == 1210)
            return 1;
        break;
    case 61:
    case 1249:
        if (no2 == 61 || no2 == 1249)
            return 1;
        break;
    }
    return 0;
}
void sub_08007558(u32 *dst, u32 *src)
{
    *dst = *src;
}
void sub_08007560(u32 *a, u32 *b)
{
    u32 tmp = *a;
    *a = *b;
    *b = tmp;
}
u32 sub_0800756C(u16 cardNo)
{
    switch (cardNo) {
    case 726:
    case 727:
    case 728:
    case 766:
        return 1;
    }
    return 0;
}
/* Card-number property test (hypothesis): true for a fixed list of card numbers; card 640
 * only when `flag` is 0, cards 735 and 1170 only when it is set. */
/* The flag arrives as a word; callers include 0x08622AB4. Decode the
 * low half here, matching the original entry shifts and return values. */
u32 sub_08007590(u16 cardNo, int flags)
{
    u16 flag = flags;
    switch (cardNo) {
    case 39:
    case 82:
    case 83:
    case 101:
    case 170:
    case 223:
    case 265:
    case 427:
    case 461:
    case 468:
    case 489:
    case 500:
    case 539:
    case 540:
    case 561:
    case 582:
    case 585:
    case 590:
    case 601:
    case 610:
    case 615:
    case 729:
    case 731:
    case 762:
    case 1048:
    case 1106:
    case 1116:
    case 1163:
    case 1254:
    case 1255:
    case 1256:
    case 1307:
    case 1330:
    case 1337:
    case 1338:
    case 1435:
    case 1436:
    case 1512:
    case 1521:
    case 1524:
        return 1;
    case 640:
        return flag == 0;
    case 735:
    case 1170:
        return flag;
    }
    return 0;
}
u32 sub_08007730(u16 id)
{
    u32 result = 0;

    if (CARD_TYPE(gCardStats[id & 0x7FF]) > 20)
        return 0;
    if (GetCardSubtype(id) == 1)
        result = 1;
    switch (CARD_NUMBER(id)) {
    case 0x2DA:
    case 0x32C:
    case 0x4D9:
    case 0x536:
    case 0x5F6:
        result = 1;
    }
    return result;
}
/* Monster card test (hypothesis: e.g. "can be targeted by X"): subtype 2/3, or one of a
 * list of card numbers; never for subtype 0 or non-monsters. */
u32 sub_08007834(u16 id)
{
    int subtype;

    if (CARD_TYPE(gCardStats[id & 0x7FF]) > 20)
        return 0;
    subtype = GetCardSubtype(id);
    /* This empty compiler barrier keeps every subtype path entering the
     * common switch head, as in the ROM. It emits no instructions. */
    asm volatile ("" : : "r"(subtype));
    switch (subtype) {
    case 0:
        return 0;
    case 2:
    case 3:
        goto yes;
    }
    switch (CARD_NUMBER(id)) {
    case 55:
    case 56:
    case 62:
    case 66:
    case 368:
    case 373:
    case 391:
    case 726:
    case 727:
    case 728:
    case 741:
    case 766:
    case 845:
    case 1202:
    case 1257:
    case 1514:
    case 1515:
    case 1516:
    case 1517:
    case 1518:
    case 1519:
    yes:
        return 1;
    }
    return 0;
}
