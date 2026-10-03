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
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

/* Card reference (0x14 bytes, see duel_piles / duel_stat_queries). */
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
    u8 step;            /* +0x001: index into gCardListViewSteps */
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
extern struct ListView gCardListView;
#define gListView gCardListView

/* gMain (struct Main) comes from include/main.h; +0x4422 is bgVofs[1]. */
#define gMain gMain

struct ScrollStep { u16 y; u16 unk2; };
extern const struct ScrollStep gCardListViewCursorSlide[][4];   /* scroll offsets [dir][timer] */
void TextCanvasToTiles(void *dest, u16 value);
void CardListView_DrawSelectedInfo(void);
void CardListView_DrawSelectedCursorFrame(void);
void CardListView_DrawPage(void);
void CardListView_DrawButtons(int button, int mask);
void CardListView_DrawCardStatus(u32 *card);

void PlaySE(u16 se);  /* PlaySE */
void CardDetail_Init(u16 cardId, u16 timer, u16 c);  /* Card Detail view (hypothesis) */
u16 CardDetail_Run(void);
u16 CardListView_InitScreen(void);

/* struct DuelPlayer and gDuelPlayers come from include/duel.h (+0x904 = graveyard). */

void MemClear16(void *dst, u32 size); /* MemClear16 */
void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void CollectEffectTargets(int player, int a, int b);

typedef u16 (*StepFunc)(void);
extern const StepFunc gCardListViewSteps[];

u16 FadeToBlack(u32 a);
void DuelScreen_Init(void);
void DuelScreen_DrawCursorInfo(void);
u16 DuelScreen_FadeInStep(void);
void CardListView_Update(void);

int GetZoneCardType(int player, int zone);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int GetFaceUpFieldMagicNumber(void);
int GetZoneCardAtk(int player, int zone);
int CountActiveCardsOnField(int player, u16 number);
int GetZoneCardAttribute(int player, int zone);
int CountZoneLinksFromCard(int player, int zone, u16 number);

u16 CardListView_Exit(void)
{
    struct ListView *v = &gListView;

    switch (v->state) {
    case 0:
        if (FadeToBlack(4)) {
            v->drawList = 0;
            v->state++;
        }
        break;
    case 1:
        DuelScreen_Init();
        DuelScreen_DrawCursorInfo();
        v->state++;
        break;
    default:
        return DuelScreen_FadeInStep();
    }
    return 0;
}
/* Per-frame list viewer update: VRAM clear, scroll animation, redraw. */
void CardListView_Update(void)
{
    int idle = 1;
    struct ListView *v = &gListView;

    if (v->clearVram) {
        v->clearVram = 0;
        TextCanvasToTiles((void *)0x06004200, 0);
    }
    if (v->scrollDir) {
        if (v->scrollTimer) {
            v->scrollTimer--;
            gMain.bgVofs[1] = gCardListViewCursorSlide[v->scrollDir][v->scrollTimer].y - (v->row << 4);
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
            CardListView_DrawSelectedInfo();
            CardListView_DrawSelectedCursorFrame();
        }
    }
    if (gListView.drawList) {
        CardListView_DrawButtons(gListView.button, gListView.buttonMask);
        if (idle && gListView.count)
            CardListView_DrawCardStatus(&gListView.cards[gListView.top + gListView.row]);
    }
}
/* List viewer input step: Up/Down scroll, Right cycles the buttons, A activates, B closes. */
u16 CardListView_HandleInput(void)
{
    int idx = gListView.top + gListView.row;
    u32 id = CARD_ID(gListView.cards[idx]);

    switch (gListView.state) {
    case 1:
        if (FadeToBlack(4)) {
            gListView.drawList = 0;
            gListView.state++;
        }
        break;
    case 2:
        CardDetail_Init(id, 0, 0);
        gListView.state++;
        break;
    case 3:
        if (CardDetail_Run()) {
            gListView.unk3 = 0;
            gListView.state++;
        }
        break;
    case 4:
        if (CardListView_InitScreen()) {
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
                    CardListView_DrawPage();
                    CardListView_DrawSelectedInfo();
                    CardListView_DrawSelectedCursorFrame();
                }
                PlaySE(0);
            } else {
                PlaySE(3);
            }
        }
        if (gMain.newKeys & 0x80) {
            if (idx < gListView.count - 1) {
                if (gListView.row <= 2) {
                    gListView.scrollDir = 2;
                    gListView.scrollTimer = 4;
                } else {
                    gListView.top++;
                    CardListView_DrawPage();
                    CardListView_DrawSelectedInfo();
                    CardListView_DrawSelectedCursorFrame();
                }
                PlaySE(0);
            } else {
                PlaySE(3);
            }
        }
        if (gMain.newKeys & 0x10) {
            PlaySE(0);
            do {
                gListView.button++;
            } while (!((gListView.buttonMask >> gListView.button) & 1));
        }
        if (gMain.newKeys & 0x20) {
            PlaySE(0);
            do {
                gListView.button--;
            } while (!((gListView.buttonMask >> gListView.button) & 1));
        }
        if ((gMain.newKeys & 2) && (gListView.buttonMask & 2)) {
            PlaySE(2);
            return 1;
        }
        if (gMain.newKeys & 1) {
            switch (gListView.button) {
            case 0:
                if (gListView.mode == 3) {
                    int player = gListView.player;
                    int i = gListView.top + gListView.row;
                    if ((u8)gDuelPlayers[player & 1].arrCC4[i] == 2 && player) {
                        PlaySE(3);
                        break;
                    }
                }
                PlaySE(1);
                gListView.state = 1;
                break;
            case 1:
                PlaySE(2);
                return 1;
            case 2:
                PlaySE(1);
                break;
            case 3:
                PlaySE(1);
                return 1;
            }
        }
        break;
    }
    return 0;
}
/* Run the current step of the list viewer; returns 1 while active. */
u16 CardListView_Run(void)
{
    struct ListView *v = &gListView;

    if (v->active) {
        if (gCardListViewSteps[v->step] != NULL) {
            CardListView_Update();
            if (gCardListViewSteps[v->step]()) {
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
/* Open the list viewer on one of player's card lists (area 12-15, 13 = deck) or, for area -1, CollectEffectTargets. */
void CardListView_Open(int player, int area, int a2, int a3)
{
    struct DuelCard *src;
    int copy = 0;
    int i;

    MemClear16(gListView.cards, 0x200);
    gListView.player = player & 1;
    gListView.count = 0;
    gListView.buttonMask = 0;
    gListView.button = 0;
    switch (area) {
    case 14:
        gListView.mode = player;
        gListView.buttonMask = 2;
        gListView.count = gDuelPlayers[player & 1].graveCount;
        src = gDuelPlayers[player & 1].graveyard;
        copy = 1;
        break;
    case 12:
        gListView.mode = 2;
        gListView.buttonMask = 2;
        gListView.count = gDuelPlayers[player & 1].fusionCount;
        src = gDuelPlayers[player & 1].fusionDeck;
        copy = 1;
        break;
    case 15:
        gListView.mode = 3;
        gListView.buttonMask = 2;
        gListView.count = gDuelPlayers[player & 1].countB84;
        src = gDuelPlayers[player & 1].listB84;
        copy = 1;
        break;
    case 13:
        gListView.mode = 5;
        gListView.buttonMask = 2;
        gListView.count = gDuelPlayers[player & 1].deckCount;
        src = gDuelPlayers[player & 1].deck;
        copy = 1;
        break;
    case -1:
        gListView.mode = 4;
        gListView.buttonMask = 8;
        CollectEffectTargets(player, a2, a3);
        break;
    default:
        gListView.active = 0;
        return;
    }
    if (copy) {
        struct DuelCard *dst = (struct DuelCard *)gListView.cards;
        for (i = 0; i < gListView.count; i++)
            CopyDuelCard(dst++, src++);
    }
    gListView.row = 0;
    gListView.scrollTimer = 0;
    gListView.scrollDir = 0;
    gListView.top = 0;
    gListView.active = 1;
    gListView.step = 0;
}

/* Can the card `id` target the card in (player, zone)?  0 if the zone is empty. */
u16 CanCardTargetZone(u16 id, int player, int zone)
{
    u16 ok = 1;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    u32 zid = CARD_ID(CARD_WORD(z->card));

    if (zid == 0)
        return 0;
    if (!(ZFLAGS(z) & 2))
        return 1;
    if (GetZoneCardType(player, zone) == 1) {
        if (CountFaceUpMonstersByNumber(0, 0x2E4) > 0)
            ok = 0;
        if (CountFaceUpMonstersByNumber(1, 0x2E4) > 0)
            ok = 0;
    }
    if ((CARD_NUMBER(zid) == 0x52E || CARD_NUMBER(zid) == 0x531) && GetFaceUpFieldMagicNumber() == 0x14D
        && CARD_TYPE(id) == 0x16 && CARD_SUBTYPE(id) != 3)
        ok = 0;
    return ok;
}

int IsZoneTargetable(int player, int zone)
{
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    u32 id = CARD_ID(CARD_WORD(z->card));

    if (id == 0)
        return 0;
    if ((CARD_NUMBER(id) == 0x52E || CARD_NUMBER(id) == 0x531) && GetFaceUpFieldMagicNumber() == 0x14D
        && (ZFLAGS(z) & 2))
        return 0;
    return 1;
}
/* Card-specific target check: ref (card 0x37/0x38/0x42/0x170) on its own monster (player, zone) that has
 * card number `needNo`; true when a kind-1 link of that zone holds card `wantNo` with counter > limit. */
/* Zone pointer with the player term written first: agbcc then emits the zone multiply first (the ROM's order). */
#define ZR(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))

/* The u16 cid temporary adds pre-combine insns after the last use of the 0xD64 constant, which lengthens
 * the zone*0x94 invariant's life so the constant gets r7 and zone*0x94 gets ip, as in the ROM. */
int EffectEquippedTributeCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    u32 id = CARD_ID(CARD_WORD(ZR(player & 1, zone)->card));
    u16 wantNo = 0x47;
    u16 needNo = 0x115;
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
    if (CountActiveCardsOnField(0, 0x58A) > 0 || CountActiveCardsOnField(1, 0x58A) > 0)
        return 0;
    for (i = 0; i < ZR(player & 1, zone)->numLinks; i++) {
        u16 link = ZR(player & 1, zone)->links[i];

        if ((u8)ZR(player & 1, zone)->linkKinds[i] == 1) {
            int lp = (u8)link;
            int lz = link >> 8;
            int pp = lp & 1;
            struct DuelZone *l = ZB(pp, lz);
            u16 cid = CARD_ID(CARD_WORD(l->card));

            if (CARD_NUMBER(cid) == wantNo && l->counter6 > limit)
                return 1;
        }
    }
    return 0;
}

int EffectTrapTargetCheck(struct CardRef *ref, u16 pos)
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

int EffectOpponentMonsterCheck(struct CardRef *ref, u16 pos)
{
    int zone;
    int player;

    player = (u8)pos;
    zone = pos >> 8;

    if (ref->player != player && zone <= 4 && CanCardTargetZone(ref->id, player, zone)) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);
        if (CARD_ID(CARD_WORD(z->card)))
            return 1;
    }
    return 0;
}

/* Main target check for a face-up monster in (player, zone): per card number of ref, compares
 * GetZoneCardType (a) or GetZoneCardAttribute (b) of the target with a constant, or tests the target's card. */
int EffectEquipTargetCheck(struct CardRef *ref, u16 pos)
{
    u16 refId = ref->id;
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZonesPlayer *pz = &gDuelZones[player & 1];
    struct DuelZone *first = (struct DuelZone *)pz;
    u32 zid;
    u16 a;
    u16 b;
    struct DuelZone *z;

    first = (struct DuelZone *)((u32)first + zone * 0x94);
    zid = CARD_ID(CARD_WORD(first->card));
    a = GetZoneCardType(player, zone);
    b = GetZoneCardAttribute(player, zone);

    if (zone > 4)
        return 0;
    z = ZB(player & 1, zone);
    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2) || !CanCardTargetZone(ref->id, player, zone))
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
        result = CountZoneLinksFromCard(player, zone, 0x47);
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

int EffectStopDefenseCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    struct DuelZone *z;

    if (zone <= 4 && player != ref->player) {
        int p = player & 1;
        z = ZB(p, zone);
        if (CARD_ID(CARD_WORD(z->card)) && CanCardTargetZone(ref->id, player, zone))
            return ZFLAGS(z) & 1;
    }
    return 0;
}

int EffectBlastJugglerCheck(struct CardRef *ref, u16 pos)
{
    int player = (u8)pos;
    int zone = pos >> 8;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);

    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2) || GetZoneCardAtk(player, zone) > 1000
        || !CanCardTargetZone(ref->id, player, zone)
        || (player == ref->player && zone == ref->zone))
        return 0;
    return 1;
}

int EffectMagicTargetCheck(struct CardRef *ref, u16 pos)
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
