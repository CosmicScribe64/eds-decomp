#include "global.h"
#include "main.h"
#include "duel.h"

/*
 * Duel field-target checks (can a card in (player, zone) be selected / affected?)
 * and the duel overlay object at 0x0201D810.  See wiki/functions/code-0802aac0.md.
 */

/* struct DuelCard, struct DuelZone and struct DuelZonesPlayer come from include/duel.h. */

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); p is player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

/* Card reference (0x14 bytes, see code_08009A68 / code_0800C894). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 unk2_10 : 6;
    u8 filler4[0x14 - 0x4];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)

/* Card list viewer at 0x0201D810 (0x310 bytes, cleared by the duel setup). */
struct ListView {
    u8 active : 1;      /* +0x000 bit 0 */
    u8 player : 1;      /* +0x000 bit 1 (hypothesis: owner of the listed cards) */
    u8 clearVram : 1;   /* +0x000 bit 2: clear 0x06004200 on the next update */
    u8 unk0_3 : 1;
    u8 drawList : 1;    /* +0x000 bit 4 */
    u8 mode : 3;        /* +0x000 bits 5-7 */
    u8 step;            /* +0x001: index into gUnk_0819A7B8 */
    u8 state;           /* +0x002 */
    u8 unk3;
    u8 unk4;
    u8 row : 2;         /* +0x005 bits 0-1: cursor row on screen */
    u8 scrollTimer : 3; /* +0x005 bits 2-4: scroll animation frames left */
    u8 scrollDir : 2;   /* +0x005 bits 5-6: 1 up, 2 down */
    u8 unk5_7 : 1;
    u16 top;            /* +0x006: first visible entry */
    u8 button : 2;      /* +0x008 bits 0-1: selected button */
    u8 buttonMask : 4;  /* +0x008 bits 2-5: enabled buttons */
    u8 unk8_6 : 2;
    u8 filler9[3];
    u32 cards[0xC0];    /* +0x00C: card words */
    u16 count;          /* +0x30C */
};
extern struct ListView gUnk_0201D810;
#define gListView gUnk_0201D810

/* gUnk_03000040 (struct Main) comes from include/main.h; +0x4422 is bgVofs[1]. */
#define gMain gUnk_03000040

struct ScrollStep { u16 y; u16 unk2; };
extern const struct ScrollStep gUnk_0819A788[][4];   /* scroll offsets [dir][timer] */
void sub_08075114(void *dest, u16 value);
void sub_0802A45C(void);
void sub_0802A4A4(void);
void sub_0802A47C(void);
void sub_0802A658(int button, int mask);
void sub_0802A4CC(u32 *card);

void sub_08077AEC(u16 se);  /* PlaySE */
void sub_0800688C(u16 cardId, u16 timer, u16 c);  /* Card Detail view (hypothesis) */
u16 sub_08006D08(void);
u16 sub_0802A6DC(void);

/* struct DuelPlayer and gUnk_020192E4 come from include/duel.h (+0x904 = graveyard). */

void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_08044224(int player, int a, int b);

typedef u16 (*StepFunc)(void);
extern const StepFunc gUnk_0819A7B8[];

u16 sub_08075A6C(u32 a);
void sub_080609C4(void);
void sub_0805F96C(void);
u16 sub_08060B2C(void);
void sub_0802AB0C(void);

int sub_0800C8BC(int player, int zone);
int sub_080086CC(int player, u16 cardNo);
int sub_080094E4(void);
int sub_0800C894(int player, int zone);
int sub_08008524(int player, u16 number);
int sub_0800CAF0(int player, int zone);
int sub_0800A78C(int player, int zone, u16 number);

u16 sub_0802AAC0(void)
{
    struct ListView *v = &gListView;

    switch (v->state) {
    case 0:
        if (sub_08075A6C(4)) {
            v->drawList = 0;
            v->state++;
        }
        break;
    case 1:
        sub_080609C4();
        sub_0805F96C();
        v->state++;
        break;
    default:
        return sub_08060B2C();
    }
    return 0;
}
/* Per-frame list viewer update: VRAM clear, scroll animation, redraw. */
void sub_0802AB0C(void)
{
    int idle = 1;
    struct ListView *v = &gListView;

    if (v->clearVram) {
        v->clearVram = 0;
        sub_08075114((void *)0x06004200, 0);
    }
    if (v->scrollDir) {
        if (v->scrollTimer) {
            v->scrollTimer--;
            gMain.bgVofs[1] = gUnk_0819A788[v->scrollDir][v->scrollTimer].y - (v->row << 4);
            idle = 0;
        } else {
            switch (v->scrollDir) {
            case 1:
                v->row--;
                break;
            case 2:
                v->row++;
                break;
            }
            gListView.scrollDir = 0;
            gMain.bgVofs[1] = -(gListView.row << 4);
            sub_0802A45C();
            sub_0802A4A4();
        }
    }
    if (gListView.drawList) {
        sub_0802A658(gListView.button, gListView.buttonMask);
        if (idle && gListView.count)
            sub_0802A4CC(&gListView.cards[gListView.top + gListView.row]);
    }
}
/* List viewer input step: Up/Down scroll, Right cycles the buttons, A activates, B closes. */
u16 sub_0802AC2C(void)
{
    int idx = gListView.top + gListView.row;
    u32 id = CARD_ID(gListView.cards[idx]);

    switch (gListView.state) {
    case 1:
        if (sub_08075A6C(4)) {
            gListView.drawList = 0;
            gListView.state++;
        }
        break;
    case 2:
        sub_0800688C(id, 0, 0);
        gListView.state++;
        break;
    case 3:
        if (sub_08006D08()) {
            gListView.unk3 = 0;
            gListView.state++;
        }
        break;
    case 4:
        if (sub_0802A6DC()) {
            gListView.unk3 = 0;
            gListView.state++;
        }
        break;
    default:
        if (gListView.scrollDir)
            break;
        if (gMain.newKeys & 0x40) {
            if (idx > 0) {
                if (gListView.row) {
                    gListView.scrollDir = 1;
                    gListView.scrollTimer = 4;
                } else {
                    gListView.top--;
                    sub_0802A47C();
                    sub_0802A45C();
                    sub_0802A4A4();
                }
                sub_08077AEC(0);
            } else {
                sub_08077AEC(3);
            }
        }
        if (gMain.newKeys & 0x80) {
            if (idx < gListView.count - 1) {
                if (gListView.row <= 2) {
                    gListView.scrollDir = 2;
                    gListView.scrollTimer = 4;
                } else {
                    gListView.top++;
                    sub_0802A47C();
                    sub_0802A45C();
                    sub_0802A4A4();
                }
                sub_08077AEC(0);
            } else {
                sub_08077AEC(3);
            }
        }
        if (gMain.newKeys & 0x10) {
            sub_08077AEC(0);
            do {
                gListView.button++;
            } while (!((gListView.buttonMask >> gListView.button) & 1));
        }
        if (gMain.newKeys & 0x20) {
            sub_08077AEC(0);
            do {
                gListView.button--;
            } while (!((gListView.buttonMask >> gListView.button) & 1));
        }
        if ((gMain.newKeys & 2) && (gListView.buttonMask & 2)) {
            sub_08077AEC(2);
            return 1;
        }
        if (gMain.newKeys & 1) {
            switch (gListView.button) {
            case 0:
                if (gListView.mode == 3) {
                    int player = gListView.player;
                    int i = gListView.top + gListView.row;
                    if ((u8)gUnk_020192E4[player & 1].arrCC4[i] == 2 && player) {
                        sub_08077AEC(3);
                        break;
                    }
                }
                sub_08077AEC(1);
                gListView.state = 1;
                break;
            case 1:
                sub_08077AEC(2);
                return 1;
            case 2:
                sub_08077AEC(1);
                break;
            case 3:
                sub_08077AEC(1);
                return 1;
            }
        }
        break;
    }
    return 0;
}
/* Run the current step of the list viewer; returns 1 while active. */
u16 sub_0802AED8(void)
{
    struct ListView *v = &gListView;

    if (v->active) {
        if (gUnk_0819A7B8[v->step] != NULL) {
            sub_0802AB0C();
            if (gUnk_0819A7B8[v->step]()) {
                v->state = 0;
                v->unk3 = 0;
                v->unk4 = 0;
                v->step++;
            }
            return 1;
        }
        v->active = 0;
    }
    return 0;
}
/* Open the list viewer on one of player's card lists (area 12-15, 13 = deck) or, for area -1, sub_08044224. */
void sub_0802AF34(int player, int area, int a2, int a3)
{
    struct DuelCard *src;
    int copy = 0;
    int i;

    sub_08075278(gListView.cards, 0x200);
    gListView.player = player & 1;
    gListView.count = 0;
    gListView.buttonMask = 0;
    gListView.button = 0;
    switch (area) {
    case 14:
        gListView.mode = player;
        gListView.buttonMask = 2;
        gListView.count = gUnk_020192E4[player & 1].graveCount;
        src = gUnk_020192E4[player & 1].graveyard;
        copy = 1;
        break;
    case 12:
        gListView.mode = 2;
        gListView.buttonMask = 2;
        gListView.count = gUnk_020192E4[player & 1].fusionCount;
        src = gUnk_020192E4[player & 1].fusionDeck;
        copy = 1;
        break;
    case 15:
        gListView.mode = 3;
        gListView.buttonMask = 2;
        gListView.count = gUnk_020192E4[player & 1].countB84;
        src = gUnk_020192E4[player & 1].listB84;
        copy = 1;
        break;
    case 13:
        gListView.mode = 5;
        gListView.buttonMask = 2;
        gListView.count = gUnk_020192E4[player & 1].deckCount;
        src = gUnk_020192E4[player & 1].deck;
        copy = 1;
        break;
    case -1:
        gListView.mode = 4;
        gListView.buttonMask = 8;
        sub_08044224(player, a2, a3);
        break;
    default:
        gListView.active = 0;
        return;
    }
    if (copy) {
        struct DuelCard *dst = (struct DuelCard *)gListView.cards;
        for (i = 0; i < gListView.count; i++)
            sub_08007558(dst++, src++);
    }
    gListView.row = 0;
    gListView.scrollTimer = 0;
    gListView.scrollDir = 0;
    gListView.top = 0;
    gListView.active = 1;
    gListView.step = 0;
}

/* Can the card `id` target the card in (player, zone)?  0 if the zone is empty. */
u16 sub_0802B1B8(u16 id, int player, int zone)
{
    u16 ok = 1;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    u32 zid = CARD_ID(CARD_WORD(z->card));

    if (zid == 0)
        return 0;
    if (!(ZFLAGS(z) & 2))
        return 1;
    if (sub_0800C8BC(player, zone) == 1) {
        if (sub_080086CC(0, 0x2E4) > 0)
            ok = 0;
        if (sub_080086CC(1, 0x2E4) > 0)
            ok = 0;
    }
    if ((CARD_NUMBER(zid) == 0x52E || CARD_NUMBER(zid) == 0x531) && sub_080094E4() == 0x14D
        && CARD_TYPE(id) == 0x16 && CARD_SUBTYPE(id) != 3)
        ok = 0;
    return ok;
}

int sub_0802B28C(int player, int zone)
{
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    u32 id = CARD_ID(CARD_WORD(z->card));

    if (id == 0)
        return 0;
    if ((CARD_NUMBER(id) == 0x52E || CARD_NUMBER(id) == 0x531) && sub_080094E4() == 0x14D
        && (ZFLAGS(z) & 2))
        return 0;
    return 1;
}
/* Card-specific target check: ref (card 0x37/0x38/0x42/0x170) on its own monster (player, zone) that has
 * card number `needNo`; true when a kind-1 link of that zone holds card `wantNo` with counter > limit. */
#if 0 /* NONMATCHING: loop register allocation differs (zone ends up in r8, p kept live on the stack;
          the ROM recomputes player & 1 before the loop and keeps zone in r6) */
int sub_0802B2FC(struct CardRef *ref, u16 pos)
{
    u16 player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    u32 id = CARD_ID(CARD_WORD(ZB(p, zone)->card));
    u16 wantNo = 0x47;
    u8 needNo = 0x115;
    int limit;
    int i;

    if (id == 0 || zone > 4 || player != ref->player)
        return 0;
    switch (CARD_NUMBER(ref->id)) {
    case 0x37:
        limit = 1;
        break;
    case 0x38:
        limit = 3;
        break;
    case 0x42:
        limit = 5;
        break;
    case 0x170:
        limit = -1;
        wantNo = 0x28B;
        needNo = 0x16D;
        break;
    default:
        return 0;
    }
    if (CARD_NUMBER(id) != needNo)
        return 0;
    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0)
        return 0;
    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        s8 link = ZB(player & 1, zone)->links[i];

        if ((u8)ZB(player & 1, zone)->linkKinds[i] == 1) {
            u8 lp = (u8)link;
            int lz = link >> 8;
            struct DuelZone *l = ZB(lp & 1, lz);

            if (CARD_NUMBER(CARD_ID(CARD_WORD(l->card))) == wantNo && l->counter6 > limit)
                return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatching/code_0802AAC0", sub_0802B2FC); /* 0x0802B2FC size 0x190 */
#endif

int sub_0802B48C(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    u16 copy;
    int t;

    copy = id;
    if (id == 0 || (u32)(zone - 5) > 5)
        return 0;
    if ((ZFLAGS(z) & 2)) {
        t = CARD_TYPE(copy);
        return t == 21;
    }
    return 1;
}

int sub_0802B500(struct CardRef *ref, u16 pos)
{
    int zone;
    int player;

    player = (u8)pos;
    zone = pos >> 8;

    if (ref->player != player && zone <= 4 && sub_0802B1B8(ref->id, player, zone)) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);
        if (CARD_ID(CARD_WORD(z->card)))
            return 1;
    }
    return 0;
}

/* Main target check for a face-up monster in (player, zone): per card number of ref, compares
 * sub_0800C8BC (a) or sub_0800CAF0 (b) of the target with a constant, or tests the target's card. */
int sub_0802B558(struct CardRef *ref, u16 pos)
{
    u16 refId = ref->id;
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZonesPlayer *pz = &gUnk_0201930C[player & 1];
    struct DuelZone *first = (struct DuelZone *)pz;
    u32 zid;
    u16 a;
    u16 b;
    struct DuelZone *z;

    first = (struct DuelZone *)((u32)first + zone * 0x94);
    zid = CARD_ID(CARD_WORD(first->card));
    a = sub_0800C8BC(player, zone);
    b = sub_0800CAF0(player, zone);

    if (zone > 4)
        return 0;
    z = ZB(player & 1, zone);
    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2) || !sub_0802B1B8(ref->id, player, zone))
        return 0;
    switch (CARD_NUMBER(refId)) {
    case 0x416:
    case 0x417:
        return a != 7;
    case 0x28A:
    case 0x422:
        return ref->player == player;
    case 0x47: {
        int result;

        if (CARD_NUMBER(zid) != 0x115)
            return 0;
        if (ref->player != player)
            return 0;
        result = sub_0800A78C(player, zone, 0x47);
        if (result != 0)
            return 0;
        result = 1;
        return result;
    }
    case 0x13C:
        switch (CARD_NUMBER(zid)) {
        case 0x3D:
        case 0x3E:
        case 0x4E1:
            return 1;
        }
        return 0;
    case 0x28B:
        if (CARD_NUMBER(zid) == 0x16D)
            return 1;
        return 0;
    case 0x604:
        if (CARD_NUMBER(zid) == 0x53B)
        return_true:
            return 1;
        return 0;
    case 0x130:
    case 0x131:
        return a == 10;
    case 0x144:
    case 0x3C2:
    case 0x522:
        return a == 7;
    case 0x137:
        return a == 0x11;
    case 0x145:
        return a == 9;
    case 0x147:
        return a == 0xE;
    case 0x13B:
        return a == 0x13;
    case 0x12C:
    case 0x49E:
    case 0x58E:
    case 0x60E:
        return a == 0xF;
    case 0x142:
        return a == 0x12;
    case 0x13A:
        return a == 1;
    case 0x146:
        return a == 0x10;
    case 0x135:
        return a == 0xD;
    case 0x13E:
        return a == 0xC;
    case 0x141:
        return a == 2;
    case 0x133:
        return a == 0xB;
    case 0x12E:
        return a == 3;
    case 0x143:
        return b == 5;
    case 0x134:
        return b == 3;
    case 0x28D:
    case 0x3F4:
        return b == 4;
    case 0x132:
    case 0x29B:
        return b == 1;
    case 0x3F5:
        return b == 6;
    case 0x12D:
        return b == 2;
    case 0x12F: case 0x136: case 0x138: case 0x139: case 0x140: case 0x290: case 0x291:
    case 0x412: case 0x424: case 0x4EA: case 0x521: case 0x58B: case 0x58C:
    case 0x5A8: case 0x5A9: case 0x5AA: case 0x60C:
        goto return_true;
    }
    return 0;
}

int sub_0802B98C(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZone *z;

    if (zone <= 4 && player != ref->player) {
        int p = player & 1;
        z = ZB(p, zone);
        if (CARD_ID(CARD_WORD(z->card)) && sub_0802B1B8(ref->id, player, zone))
            return ZFLAGS(z) & 1;
    }
    return 0;
}

int sub_0802B9EC(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);

    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2) || sub_0800C894(player, zone) > 1000
        || !sub_0802B1B8(ref->id, player, zone)
        || (player == ref->player && zone == ref->zone))
        return 0;
    return 1;
}

int sub_0802BA68(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    u16 copy;
    int t;

    copy = id;
    if (id == 0 || (u32)(zone - 5) > 5)
        return 0;
    if ((ZFLAGS(z) & 2) && (t = CARD_TYPE(copy)) == 0x15)
        return 0;
    return 1;
}
