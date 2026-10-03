#include "global.h"

/* Menu block 0x0201AE60 (see code_0804EFF0) */
struct Ui {
    u8 u0[8];
    u16 x;          /* +8 */
    u8 uA[4];
    u16 h;          /* +0xE */
    u8 u10[4];
    u16 sel;        /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;       /* +0x22 */
    u8 timer;       /* +0x23 */
};
extern struct Ui gTextBox;
struct MainKeys { u8 u0[4]; u16 keysHeld; u16 keys; };
extern struct MainKeys gMain;
/* Duel global 0x020192E0 */
struct Duel {
    u8 u0[0x1B50];
    u8 b1B50;
    u8 pad;
    u16 h1B52[2];       /* +0x1B52 candidate card ids */
    u8 u1B56[0x1B62 - 0x1B56];
    u8 step;            /* +0x1B62 */
    u8 pad2;
    u16 sel;            /* +0x1B64 */
    u16 sel2;           /* +0x1B66 */
};
extern struct Duel gDuel;
extern u16 gUnk_0201AE44;
extern const u8 gStrNewline[], gStrMenuIndent[];
extern const char *const gAttributeNames[];
void StrCopy(char *dst, const void *src);
void StrCat(char *dst, const void *src);
int Random(void);
void TextBoxDrawChoiceCursor(void);
int TextBoxHandleChoiceInputCpu(void);
struct Player { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
struct DuelP { u8 pad[4]; struct Player players[2]; };
struct Q1 { u8 pad[9]; u8 f0 : 1; u8 rest : 7; };
struct DuelQ { u8 pad[0xD68]; struct Q1 q1; };
extern const u32 gCardStats[];
#define CARD_STATS(id) (*(gCardStats + ((id) & 0x7FF)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }
int DuelCursor_PickTarget(u32 mask);
int AiPickOpponentHandCard(void);
int CountTributableMonsters(int player, int a);
int AiPickTributeMonster(int a, int b);
void TributeMonster(int a, int b);
int CanSummonFromHand(int player, int id);
int IsSpecialSummonOnly(int id);
int FindFreeMonsterZone(int player);
void PlaySE(u16 se);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
void MemCopy16(u32 dst, const void *src, u32 n);
void TextCanvasInit(u32 a, u32 b);
void TextDrawString(u32 a, u32 b, u32 c, const char *s);
void TextCanvasToTiles(u32 a, u32 b);
extern const u8 gSystemFontPal[];
extern const char *const gMonsterTypeNames[];
extern struct Player gDuelPlayers[];
extern u8 gUnk_0201AE42;
struct ActBlk { u8 u0[0x4FC]; u8 b4FC; u8 count; };
extern struct ActBlk gChain;
struct DuelScreen { u8 unk0[0x808]; u8 b808; u8 pad[0x824 - 0x809]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreen gDuelScreen;
void DuelScreen_ScrollToZone(int player, int a);
void DuelCursor_Select(u32 player, u32 a, u32 b);
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
void DiscardHandCard(int player, int idx, int a, int b);
extern const u8 gStrPromptSelectOpponentHandCard[];
extern const u8 gStrPromptSelectType[], gStrPromptSelectAttribute[], gStrPromptSelectAnotherAttribute[], gStrPromptSelectMonsterToSet[], gStrPromptSelectTribute[];
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, void (*b)(void), int (*c)(void));
int TypeMenu_HandleInput(void);
void AttributeMenu_Draw(void);
void TypeMenu_Draw(void);
int AttributeMenu_HandleInput(void);

extern struct { u8 pad[0x82C]; u8 b82C; } gScr1A9C asm("gDuelScreen");
struct Scr1A9C {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1A9Cb (*(struct Scr1A9C *)&gDuelScreen)
int DuelPrompt_DiscardRandom(int player, u16 x, int count)
{
    u8 *step;
    if (gDuelPlayers[player & 1].handCount != 0) {
        step = (u8 *)gDuelPlayers + 0x1B5E;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, 0xB);
            gChain.b4FC = 0;
            gChain.count = count;
            (*step)++;
            return 0;
        }
        if (gChain.count != 0) {
            u8 *bp = &gChain.b4FC;
            if (*bp <= 9) {
                gScr1A9Cb.busy = 1;
                DuelCursor_Select(player, 0xB, Random() % gDuelPlayers[player & 1].handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                DuelCmd_Push(msg, (u16)player, ((u8)gScr1A9Cb.cursor << 8) | 0xB, 0);
                DiscardHandCard(player, gScr1A9Cb.cursor, x, 1);
                gChain.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
struct Scr1BBC {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1BBC (*(struct Scr1BBC *)&gDuelScreen)
#define MSG1BBC ((void (*)(u16, u16, int, int))DuelCmd_Push)
int DuelPrompt_BanishRandom(int player, int unused, int count)
{
    u8 *base = (u8 *)gDuelPlayers;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, 0xB);
            gChain.b4FC = 0;
            gChain.count = count;
            (*step)++;
            return 0;
        }
        if (gChain.count != 0) {
            u8 *bp = &gChain.b4FC;
            if (*bp <= 9) {
                gScr1BBC.busy = 1;
                DuelCursor_Select(player, 0xB, Random() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                MSG1BBC(msg, (u16)player, ((u8)gScr1BBC.cursor << 8) | 0xB, 0);
                MSG1BBC(player != 0 ? 0x80C1 : 0xC1, (u16)gScr1BBC.cursor, 1, 0);
                gChain.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
struct Scr1CD8 {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1CD8 (*(struct Scr1CD8 *)&gDuelScreen)
#define MSG1CD8 ((void (*)(u16, u16, int, int))DuelCmd_Push)
int DuelPrompt_BanishRandomFaceDown(int player, int unused, int count)
{
    u8 *base = (u8 *)gDuelPlayers;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, 0xB);
            gChain.b4FC = 0;
            gChain.count = count;
            (*step)++;
            return 0;
        }
        if (gChain.count != 0) {
            u8 *bp = &gChain.b4FC;
            if (*bp <= 9) {
                gScr1CD8.busy = 1;
                DuelCursor_Select(player, 0xB, Random() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                MSG1CD8(msg, (u16)player, ((u8)gScr1CD8.cursor << 8) | 0xB, 0);
                MSG1CD8(player != 0 ? 0x80CE : 0xCE, (u16)gScr1CD8.cursor, 1, 0);
                gChain.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
int DuelPrompt_Tribute(int player)
{
    struct Duel *d = &gDuel;
    u8 *step = &d->step;
    if (*step == 0) {
        int m = -1;
        if (CountTributableMonsters(player, m) == 0)
            return 1;
        if (player != 0) {
            int r = AiPickTributeMonster(m, 1);
            if (r > m)
                TributeMonster(player, r);
            return 1;
        }
        TextBoxOpen(0x206, 0x712, 0xB, gStrPromptSelectTribute);
        (*step)++;
        return 0;
    }
    if (DuelCursor_PickTarget(0xF0)) {
        u32 a = gDuelScreen.w824;
        u32 b = gDuelScreen.w828 + gDuelScreen.w82C;
        PlaySE(1);
        DuelCmd_Push(player != 0 ? 0x8008 : 8, (u16)gDuelScreen.w824, (u8)gDuelScreen.w828 | (((u8)gDuelScreen.w82C) << 8), 0);
        TributeMonster(a, b);
        return 1;
    }
    return 0;
}
int DuelPrompt_SetMonsterFromHand(int player)
{
    struct Duel *d = &gDuel;
    u8 *step = &d->step;
    u8 st = *step;
    switch (st) {
    case 0:
        TextBoxOpen(0x206, 0x712, 0xB, gStrPromptSelectMonsterToSet);
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(1)) {
            int p = 1 & player;
            u16 id = CARD_ID(((struct DuelP *)d)->players[p].hand[gDuelScreen.w82C]);
            if (CanSummonFromHand(player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                u32 lvl;
                switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
                case 0x15:
                case 0x16:
                case 0x17:
                    lvl = 0;
                    break;
                case 0x18:
                    lvl = 10;
                    break;
                default:
                    lvl = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
                    break;
                }
                if (lvl <= 4) {
                    DuelCmd_Push(player != 0 ? 0x80C4 : 0xC4, id,
                                 ((gDuelScreen.w82C & 0xF) << 4) | (FindFreeMonsterZone(player) & 0xF) | 0x200, 0);
                    gDuel.step++;
                    return 0;
                }
            }
            PlaySE(3);
        }
        return 0;
    default:
        return 1;
    }
}
struct PickCard { u32 id : 12; u32 flag12 : 1; u32 rest : 19; };
struct PickCursor { u8 pad0[5]; u8 sub : 2; u8 flags : 6; u16 index; u8 pad8[4]; struct PickCard cards[80]; };
extern struct PickCursor gCardListView;
extern struct PickCard gCardListViewCards[];
extern const u8 gStrPromptGraveMonsterToPlay[], gStrPromptGraveMonsterToSet[], gStrPromptGraveMonsterToSpecialSummon[];
int AiPickCardListEntry(u16 id);
void CardListView_Open(s32 player, s32 skip, u16 number, u16 mode);
int DuelPrompt_SelectGraveyardMonster(s32 player, u16 id, u16 mode)
{
    struct Duel *d;
    struct PickCard *chosen;
    u8 st;
    /* Reading the step before taking d gives the ROM's `ldr r0; ...; ldrb; adds r7,r0,#0` copy. */
    st = gDuel.step;
    d = &gDuel;
    switch (st) {
    case 0:
        if (player != 0) {
            chosen = &gCardListViewCards[AiPickCardListEntry(id)];
            break;
        }
        switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
        case 0x45C: TextBoxOpen(0x206, 0x712, 0xB, gStrPromptGraveMonsterToPlay); break;
        case 0x487: TextBoxOpen(0x206, 0x712, 0xB, gStrPromptGraveMonsterToSet); break;
        case 0x5F0: TextBoxOpen(0x206, 0x712, 0xB, gStrPromptGraveMonsterToSpecialSummon); break;
        default: return 1;
        }
        gDuel.step++;
        return 0;
    case 1:
        CardListView_Open(player, -1, ((const u16 *)0x08622AB4)[id & 0x7FF], mode);
        gDuel.step++;
        return 0;
    default:
        chosen = &gCardListView.cards[gCardListView.sub + gCardListView.index];
        if (d->u0[0x1B12] & 2)
            /* FAKEMATCH: `(&...)[0]` keeps the byte store relative to the cards base ([r2,#1])
               instead of folding +0xC+1 into one offset off the cursor ([r2,#13]). */
            (&gCardListView.cards[gCardListView.sub + gCardListView.index])[0].flag12 =
                1 - ((*(u32 *)&gCardListView.cards[gCardListView.sub + gCardListView.index] << 19) >> 31);
        break;
    }
    d->sel = *(u32 *)chosen;
    d->sel2 = ((u16 *)chosen)[1];
    return 1;
}
void TypeMenu_Draw(void)
{
    struct Ui *u = &gTextBox;
    u32 x = (u->x + 1) << 3;
    u32 y = (u->b21 - u->h) << 3;
    u8 *t;
    u32 attr;
    asm("" : "+r"(y)); /* FAKEMATCH: keep combine from folding y<<16 into D<<19 */
    t = &u->timer;
    if (*t == 0) {
        MemCopy16(0x050003E0, gSystemFontPal, 0x20);
        TextCanvasInit(0xC, 2);
        TextDrawString(1, 1, 0xA0F, gMonsterTypeNames[u->sel]);
        TextDrawString(0, 0, 0xA02, gMonsterTypeNames[u->sel]);
        TextCanvasToTiles(0x06016C80, 0);
        (*t)++;
    }
    attr = 0x364;
    AddSprite(x | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    AddSprite((x + 0x20) | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    AddSprite((y << 16) | (x + 0x40), 0x4040, attr | 0xF000);
    attr += 4;
    AddSprite(x | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    AddSprite((x + 0x20) | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    AddSprite((x + 0x40) | ((y + 8) << 16), 0x4040, attr | 0xF000);
}
int TypeMenu_HandleInput(void)
{
    struct Ui *u = &gTextBox;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    /* gMain.keys must be read in every test (no local): a local makes the
     * final `& 1` compile with the and's result in the key register. */
    if (gMain.keys & 0x20) {
        u->sel += 0x13;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gMain.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gMain.keys & 1)
        return 1;
    return 0;
}
int DuelPrompt_SelectType(void)
{
    struct Duel *d = &gDuel;
    u8 *s = &d->step;
    if (*s == 0) {
        TextBoxOpen(0x206, 0x40F, 0xB, gStrPromptSelectType);
        TextBoxSetMenu(5, (void (*)(void))TypeMenu_Draw, TypeMenu_HandleInput);
        (*s)++;
        return 0;
    }
    d->sel = gTextBox.sel;
    return 1;
}
void AttributeMenu_Draw(void)
{
    struct Ui *u = &gTextBox;
    u32 x = (u->x + 1) << 3;
    int y = (u->b21 - u->h) << 3;
    u8 *t = &u->timer;
    if (*t == 0) {
        MemCopy16(0x050003E0, gSystemFontPal, 0x20);
        TextCanvasInit(8, 4);
        TextDrawString(4, 0x12, 0xC0F, gAttributeNames[u->sel]);
        TextDrawString(3, 0x11, 0xC01, gAttributeNames[u->sel]);
        TextCanvasToTiles(0x06016C80, 0);
        (*t)++;
    }
    AddSprite(x | ((y - 0x10) << 16), 0x40C0, 0xF364);
}
int AttributeMenu_HandleInput(void)
{
    struct Ui *u = &gTextBox;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    if (gMain.keys & 0x20) {
        u->sel += 5;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gMain.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gMain.keys & 1)
        return 1;
    return 0;
}
int AttributeMenu_HandleInputExcludeFirst(void)
{
    u16 prev;
    if (gTextBox.timer == 0)
        return 0;
    if (gTextBox.state == 0) {
        gTextBox.state++;
        return 0;
    }
    if (gMain.keys & 0x20) {
        struct Ui *p = &gTextBox;
        prev = gUnk_0201AE44;
        do {
            p->sel += 5;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gTextBox.timer = 0;
        return 0;
    }
    if (gMain.keys & 0x10) {
        struct Ui *p = &gTextBox;
        prev = gUnk_0201AE44;
        do {
            p->sel += 1;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gTextBox.timer = 0;
        return 0;
    }
    if (gMain.keys & 1) {
        if (gTextBox.sel != gDuel.sel)
            return 1;
    }
    return 0;
}
int DuelPrompt_SelectAttribute(void)
{
    struct Duel *d = &gDuel;
    u8 *s = &d->step;
    if (*s == 0) {
        TextBoxOpen(0x206, 0x40F, 0xB, gStrPromptSelectAttribute);
        TextBoxSetMenu(5, (void (*)(void))AttributeMenu_Draw, AttributeMenu_HandleInput);
        (*s)++;
        return 0;
    }
    d->sel = gTextBox.sel;
    return 1;
}
int DuelPrompt_SelectTwoAttributes(void)
{
    struct Duel *d = &gDuel;
    u8 *step = &d->step;
    /* A switch rather than an if/else chain keeps both step blocks out of line. */
    switch (*step) {
    case 0:
        TextBoxOpen(0x206, 0x40F, 0xB, gStrPromptSelectAttribute);
        TextBoxSetMenu(5, (void (*)(void))AttributeMenu_Draw, AttributeMenu_HandleInput);
        (*step)++;
        return 0;
    case 1:
        d->sel = gTextBox.sel;
        TextBoxOpen(0x206, 0x411, 0xB, gStrPromptSelectAnotherAttribute);
        TextBoxSetMenu(5, (void (*)(void))AttributeMenu_Draw, AttributeMenu_HandleInputExcludeFirst);
        (*step)++;
        return 0;
    }
    d->sel2 = gTextBox.sel;
    return 1;
}
int TextBoxHandleChoiceInputCpu(void)
{
    struct Ui *u = &gTextBox;
    u8 *s = &u->state;
    int st = *s; /* the (u8) switch index is a separate pseudo, so CSE does not fold st == 1 into the case 1 increment */
    switch ((u8)st) {
    case 0:
        if (u->timer <= 0x3F)
            u->sel = (u->timer >> 2) & 1;
        if (u->timer == 0x40)
            u->sel = Random() & 1;
        if (u->timer > 0xC0) {
            u->timer = 0;
            (*s)++;
        } else
            u->timer++;
        break;
    case 1: {
        u8 v = u->timer;
        if (v <= 0x3B) {
            u->timer = v + 1;
            if ((gMain.keysHeld & 2) || (*(u8 *)&gDuelScreen & 1)) {
                if (u->timer <= 0x33)
                    u->timer = v + 8;
            }
        } else
            (*s)++;
        break;
    }
    case 2:
        return 1;
    }
    return 0;
}
int DuelPrompt_PickOneOfTwoAttributes(void)
{
    char buf[0x80];
    if (gDuel.step == 0) {
        const char *const *tbl;
        u16 *p;
        int i;
        StrCopy(buf, gStrPromptSelectAttribute);
        StrCat(buf, gStrNewline);
        tbl = gAttributeNames;
        p = gDuel.h1B52;
        for (i = 1; i >= 0; i--) {
            StrCat(buf, gStrMenuIndent);
            StrCat(buf, tbl[*p]);
            StrCat(buf, gStrNewline);
            p++;
        }
        TextBoxOpen(0x206, 0x50F, 0xB, buf);
        if (gDuel.b1B50 & 4) {
            TextBoxSetMenu(5, TextBoxDrawChoiceCursor, TextBoxHandleChoiceInputCpu);
        } else {
            TextBoxSetMenu(2, 0, 0);
            gDuel.step++;
        }
        gDuel.step++;
        return 0;
    }
    gDuel.sel = gDuel.h1B52[gTextBox.sel];
    return 1;
}
int DuelPrompt_PickOpponentHandCard(int a)
{
    if (a != 0) {
        int r = AiPickOpponentHandCard();
        gDuel.sel = r;
        DuelCmd_Push(0x8008, 0, (u8)r << 8 | 0xB, 0);
        return 1;
    } else {
        struct Duel *d = &gDuel;
        u8 *step = &d->step;
        switch (*step) {
        case 0:
            TextBoxOpen(0x206, 0x712, 0xB, gStrPromptSelectOpponentHandCard);
            (*step)++;
            return 0;
        case 1:
            {
                struct Q1 *q = &((struct DuelQ *)d)->q1;
                q->f0 = 1;
            }
            if (DuelCursor_PickTarget(0x10000)) {
                PlaySE(1);
                DuelCmd_Push(8, (u16)gDuelScreen.w824, (u8)gDuelScreen.w828 | (((u8)gDuelScreen.w82C) << 8), 0);
                (*step)++;
            }
            return 0;
        default:
            {
                struct Q1 *q = &((struct DuelQ *)d)->q1;
                q->f0 = 0;
            }
            d->sel = gDuelScreen.w82C;
            return 1;
        }
    }
}
struct Z908Zone {
    u32 card;
    u8 pad4[2];
    u8 flags;
    u8 pad7[3];
    u16 links[32];
    u16 kinds[32];
    u16 count;
    u8 pad8C[8];
};
struct Z908Player {
    u8 pad0[2];
    u8 handCount;
    u8 pad3[0x25];
    struct Z908Zone zones[11];
    u8 rest[0xD64 - 0x28 - 11 * 0x94];
};
/* The duel players at 0x020192E4 (declared above as struct Player) in this layout. */
#define gZ908Players ((struct Z908Player *)gDuelPlayers)
/* Card ID test on a card word (lsl #20, compared with 0). */
#define Z908_ID(w) (((w) << 20) >> 20)
/* Zone pointer as base + zone * 0x94 + player * 0xD64 (the link scan's address order). */
#define Z908_ZB(p, z) ((struct Z908Zone *)((u8 *)gZ908Players[0].zones + (z) * 0x94 + (p) * 0xD64))

/*
 * Is (player, kind + slot) a valid pick for the 16-bit-per-player key mask?
 * kind 11 = hand (slot < handCount), 0 = monster zones (face/position bits),
 * 5 = magic/trap zones (types 0x15/0x16, else a kind-1/5/6 link from a
 * face-up monster zone of either player), 10 = field zone.
 */
int DuelCursor_IsValidTarget(int player, int kind, int slot, u32 mask)
{
    struct Z908Zone *zone = &gZ908Players[player & 1].zones[kind + slot];
    /* The signed shift keeps the id extraction apart from the later `id != 0`
     * tests, so only zone->card << 20 is shared (CSE), as in the ROM. */
    int type = (((const u32 *)0x08621DE0)[((s32)(zone->card << 20) >> 20) & 0x7FF] & 0x1F00000) >> 20;
    int p, i, k;

    switch (kind) {
    case 11:
        if (!((1 << (player << 4)) & mask)) return 0;
        if (slot < gZ908Players[player & 1].handCount) return 1;
        return 0;
    case 0: {
        int a, b;
        if (!((0xF0 << (player << 4)) & mask)) return 0;
        a = 0;
        b = 0;
        if (Z908_ID(zone->card) == 0) return 0;
        if (((0x20 << (player << 4)) & mask) && (zone->flags & 2)) a = 1;
        if (((0x10 << (player << 4)) & mask) && !(zone->flags & 2)) a = 1;
        if (((0x80 << (player << 4)) & mask) && (zone->flags & 1)) b = 1;
        if (((0x40 << (player << 4)) & mask) && !(zone->flags & 1)) b = 1;
        if (b && a) return 1;
        return 0;
    }
    case 5: {
        u32 sel = (0xE << (player << 4)) & mask;
        if (!sel) return 0;
        if (Z908_ID(zone->card) == 0) return 0;
        if (!(zone->flags & 2)) {
            if ((2 << (player << 4)) & mask) return 1;
            return 0;
        }
        if (sel == (2 << (player << 4))) return 0;
        switch (type) {
        case 0x16:
            if ((4 << (player << 4)) & mask) return 1;
            return 0;
        case 0x15:
            if ((8 << (player << 4)) & mask) return 1;
            return 0;
        }
        for (p = 0; p <= 1; p++) {
            for (i = 0; i <= 4; i++) {
                if (Z908_ID(Z908_ZB(p & 1, i)->card) && (Z908_ZB(p & 1, i)->flags & 2)) {
                    for (k = 0; k < Z908_ZB(p & 1, i)->count; k++) {
                        u16 link = Z908_ZB(p & 1, i)->links[k];
                        switch (Z908_ZB(p & 1, i)->kinds[k]) {
                        case 1:
                        case 5:
                        case 6:
                            if (link == (u16)((u8)player | ((u8)(slot + 5) << 8))) return 1;
                            break;
                        }
                    }
                }
            }
        }
        return 0;
    }
    case 10: {
        int ret;
        struct Z908Zone *f = Z908_ZB(player & 1, 10);
        if (!Z908_ID(f->card)) return 0;
        ret = 0;
        if (f->flags & 2) {
            if ((4 << (player << 4)) & mask)
                ret = 1;
        } else if ((2 << (player << 4)) & mask)
            ret = 1;
        if (ret) return 1;
        return 0;
    }
    }
    return 0;
}
