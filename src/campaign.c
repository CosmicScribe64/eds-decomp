#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

#define gMain gMain

/* Save image (0x02011C20). */
struct SaveData {
    u8 filler0[0x2150];
    u16 unk2150;                    /* 0x2150: incremented at the end of every Campaign match */
};
extern struct SaveData gSaveData;
#define gSaveData gSaveData

/* Selection widget at 0x020192E0+0x1B2C = 0x0201AE0C (hypothesis: row of up to 13 choices). */
struct SelMask {
    u16 flag0:1;                    /* bit 0 */
    u16 active:1;                   /* bit 1: a choice was confirmed with A */
    u16 cursor:4;                   /* bits 2-5: selected entry */
    u16 rows:4;                     /* bits 6-9: slide-in / animation counter (0-8) */
    u32 mask:16;                    /* bits 10-25: entry i present if bit i set */
    u32 state:8;                    /* bits 26-33 (straddles 0x1B2F/0x1B30): CardMenu_Update state */
    u32 unk34:8;                    /* bits 34-41 */
    u32 unk42:8;                    /* bits 42-49 */
    u16 timer:7;                    /* bits 50-56: animation counter (u16 container matters in CardMenu_DrawIcons) */
    u16 player:1;                   /* bit 57: copy of 0x0201CFB0+0x824 bit 0 */
    u32 zone:7;                     /* bits 58-64 (straddles 0x1B33/0x1B34): copy of 0x0201CFB0+0x828 */
    u32 unk65:8;                    /* bits 65-72: copy of 0x0201CFB0+0x82C */
    u32 unk73:23;
};
extern struct SelMask gUnk_0201AE0C;
/* CardMenu_DrawLabel addresses the widget directly as 0x0201AE0C; the other functions go through gDuel. */
#define gSelMask gUnk_0201AE0C

/* DuelZone/DuelPlayer/DuelState and gDuel/020192E4/0201930C come from duel.h. */
#define ZONE_AT(p, s) (&gDuelPlayers[p].zones[s])

/* duel.h covers DuelState only up to +0x1B20; selCard (+0x1B28, u16) and sel (+0x1B2C) follow. The sel
   accesses must stay relative to gDuel (base + 0x1B2C), not a direct 0x0201AE0C literal, to match. */
struct DuelStateTail {
    u8 filler0[0x1B28];
    u16 selCard;                    /* +0x1B28: card ID from DuelCursor_GetCardId, shown with CardDetail_Init */
    u16 unk1B2A;                    /* +0x1B2A */
    struct SelMask sel;             /* +0x1B2C */
};
#define gDuelState ((struct DuelStateTail *)&gDuel)

/* One side of a battle (0xC bytes). Halfword bitfield containers preserve the flag-copy narrowing. */
struct BattleSide {
    u16 slot:3;          /* +0 bits 0-2: zone */
    u16 destroyed:1;     /* +0 bit 3 */
    u16 defending:1;     /* +0 bit 4: copied from zone +6 bit 0 (hypothesis: defence position) */
    u16 destroyedCopy:1; /* +0 bit 5: copy of bit 3 at the end */
    u16 flag6:1;         /* +0 bit 6 */
    u16 unk0_7:1;
    u8 unk1;
    u16 cardId;         /* +0x2 */
    u16 atk;            /* +0x4 */
    u16 def;            /* +0x6 */
    u16 value;          /* +0x8: value compared in the battle */
    u16 damage;         /* +0xA: life point damage to this side */
};

/* Battle state at 0x02018450. */
struct Battle {
    u16 attacker:1;     /* +0 bit 0 */
    u16 direct:1;       /* +0 bit 1: direct attack (hypothesis) */
    u16 unk0_2:3;
    u16 noAttack:1;     /* +0 bit 5: attacker's ATK counts as 0 */
    u16 atkSlot:3;      /* +0 bits 6-8 */
    u16 defSlot:3;      /* +0 bits 9-11 */
    u16 unk0_12:4;
    u16 unk2;
    u8 flag4_0:1;       /* +4 bit 0 */
    u8 unk4_1:7;
    u8 unk5[3];
    struct BattleSide side[2];      /* +0x8 */
    struct DuelZone zones[2];       /* +0x20: copies of both zones */
};
typedef char battle_side_size_check[sizeof(struct BattleSide) == 0xC ? 1 : -1];
extern struct Battle gBattle;
#define gBattle gBattle
#define ATK_SIDE (gBattle.side[attacker])
#define DEF_SIDE (gBattle.side[1 - attacker])

extern const u16 gCardIdToNumber[];   /* card ID to card number */
/* Integer-address indexing keeps the table load after the masked card ID. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

void MemCopy16(void *dst, const void *src, u32 size);    /* MemCopy16 */
u32 GetZoneCardAtk(u32 player, u32 slot);     /* ATK of the card in a zone */
u32 GetZoneCardDef(u32 player, u32 slot);     /* DEF of the card in a zone */
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int GetZoneCardType(int player, int zone);
int GetZoneCardAttribute(int player, int zone);
int CountActiveZoneLinksFromCard(int player, int zone, u16 number);
int HalveRoundUp(int);
void QueueAddZoneLink(int player, u16 cardId, u16 pos, u16 a);

/* Zone s of player p. The ROM computes s*0x94 + p*0xD64 on top of the zones base (0x0201930C). */
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (s) * 0x94 + (p) * 0xD64))

extern const u16 gPulseScaleCurve[];
extern const u16 gShrinkScaleSteps[];   /* affine per sel.rows */
u16 DuelCursor_GetCardId(void);
int GetCardIconObjTile(u16);
void ClearZoneTiles(s32, s32);
int GetAreaX(s32, s32, s32);
int GetAreaY(s32, s32, s32);   /* 16-frame affine table for the selected entry */
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

/* gDuelScreen (struct DuelScreen) comes from duel_ui.h. */

void AddSprite(u32 yx, u16 shapeSize, u16 attr2);    /* AddSprite */

typedef u16 (*StepFunc)(void);
extern StepFunc gCampaignSteps[];    /* Campaign steps */

/* Unpacked date (see title_screen). */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};
void GetCurrentDate(struct Date *out);    /* today's date (hypothesis) */
u32 GetCalendarEvents(u32 year, u32 month, s32 day);  /* calendar event flags for a date */
s32 Random(void);                 /* Random */
void PlayBGM(u16 bgm);
u32 GetRewardPack(u16 id);

void StartDialogue(u16 textId);
u32 CB_Bustup(void);
u16 FadeToBlack(u16 speed);        /* FadeToBlack */

/* Campaign step before the match (hypothesis: calendar-event tournaments). */
u16 Campaign_DeliverMagazines(void)
{
    struct Date d;

    switch (gMain.step488A) {
    case 0:
        GetCurrentDate(&d);
        gMain.events = GetCalendarEvents(d.year, d.month, d.day);
        if (!(gMain.events & 0x300000))
            return 1;
        gMain.step488A++;
    case 1:
        if (!(gMain.events & 0x100000)) {
            gMain.step488A++;
            gMain.step488A++;
            gMain.step488A++;
            return 0;
        }
        PlayBGM(0x1F);
        StartDialogue(0x323);
        GetCurrentDate(&d);
        gDuel.result = Random() & 3;
        if (d.year == 2001 && d.month == 1 && d.day == 9) {
            StartDialogue(0x322);
            gDuel.result = 0;
        }
        gMain.step488A++;
    case 2:
    case 5:
        if (CB_Bustup()) {
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
        }
        return 0;
    case 3:
        if (gDuel.result == 0) {
            if (GetRewardPack(0x321)) {
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
                gMain.step488A++;
            }
        } else {
            if (GetRewardPack(0x385)) {
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
                gMain.step488A++;
            }
        }
        return 0;
    case 4:
        if (!(gMain.events & 0x200000))
            return 1;
        PlayBGM(0x1F);
        StartDialogue(0x321);
        gMain.step488A++;
        GetCurrentDate(&d);
        if (d.year == 2001 && d.month == 1)
            StartDialogue(0x320);
        return 0;
    case 6:
        return GetRewardPack(0x322);
    }
    return 1;
}

/* Last Campaign step: bump the match counter. */
u16 Campaign_AdvanceDay(void)
{
    gSaveData.unk2150++;
    return 1;
}

u16 Campaign_DeckTooSmall(void)
{
    switch (gMain.step488A) {
    case 0:
        StartDialogue(0x191);
        gMain.step488A++;
        break;
    case 1:
        if (CB_Bustup()) {
            gMain.step488A++;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

/* CB_Campaign: step runner over gCampaignSteps, index gMain.seqIndexCampaign. */
u16 CB_Campaign(void)
{
    StepFunc step = gCampaignSteps[gMain.seqIndexCampaign];
    if (step != NULL) {
        if (step()) {
            gMain.seqIndexCampaign++;
            gMain.step488A = 0;
            gMain.seqState0 = 0;
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
        }
        return 0;
    }
    return FadeToBlack(8);
}

/* Battle calculation between gBattle.atkSlot of `attacker` and gBattle.defSlot of the other player. */
void CalcBattle(int attacker, u16 noAtk)
{
    int i;
    int slot;

    gBattle.attacker = attacker;
    gBattle.flag4_0 = 1;
    for (i = 0; i <= 1; i++) {
        struct BattleSide *s = &gBattle.side[i];
        struct DuelZone *copy = &gBattle.zones[i];
        if (i == attacker)
            slot = gBattle.atkSlot;
        else
            slot = gBattle.defSlot;
        MemCopy16(copy, ZONE_AT(i & 1, slot), 0x94);
        s->slot = slot;
        s->destroyed = 0;
        s->defending = ZONE_AT(i & 1, slot)->flag6_0;
        s->cardId = ((struct DuelCard *)ZONE_AT(i & 1, slot))->id;
        s->atk = GetZoneCardAtk(i, slot);
        s->def = GetZoneCardDef(i, slot);
        s->damage = 0;
    }
    if (i != attacker && gBattle.direct) {
        DEF_SIDE.cardId = 0;
        DEF_SIDE.atk = 0;
        DEF_SIDE.def = 0;
    }
    if (gBattle.noAttack)
        ATK_SIDE.atk = 0;
    ATK_SIDE.value = ATK_SIDE.atk;
    DEF_SIDE.value = DEF_SIDE.atk;
    ATK_SIDE.defending = 0;
    if (gBattle.direct) {
        DEF_SIDE.damage = ATK_SIDE.atk;
        DEF_SIDE.value = 0;
        if (DEF_SIDE.damage && CountActiveCardsOnFieldExcept(1 - attacker, 0x58F, -1) > 0)
            DEF_SIDE.damage = 0;
        return;
    }
    switch (CARD_NUMBER(ATK_SIDE.cardId)) {
    case 0x1DD:
        if (GetZoneCardAttribute(1 - attacker, gBattle.defSlot) == 6)
            ATK_SIDE.atk += 1000;
        break;
    case 0x4E5:
        if (GetZoneCardType(1 - attacker, gBattle.defSlot) == 15) {
            ATK_SIDE.atk += 2000;
            ATK_SIDE.def += 2000;
        }
        break;
    }
    switch (CARD_NUMBER(DEF_SIDE.cardId)) {
    case 0x11F:
        if (GetZoneCardAttribute(attacker, gBattle.atkSlot) == 1)
            DEF_SIDE.def /= 2;
        break;
    case 0x4E5:
        if (GetZoneCardType(attacker, gBattle.atkSlot) == 15) {
            DEF_SIDE.atk += 2000;
            DEF_SIDE.def += 2000;
        }
        break;
    }
    if (noAtk)
        ATK_SIDE.atk = 0;
    ATK_SIDE.value = ATK_SIDE.atk;
    DEF_SIDE.value = DEF_SIDE.atk;
    if (DEF_SIDE.defending)
        DEF_SIDE.value = DEF_SIDE.def;
    {
        int n = CountActiveZoneLinksFromCard(attacker, gBattle.atkSlot, 0x291);
        ATK_SIDE.value += HalveRoundUp(DEF_SIDE.atk) * n;
    }
    if (ATK_SIDE.value == DEF_SIDE.value) {
        if (!DEF_SIDE.defending && ATK_SIDE.value) {
            ATK_SIDE.destroyed = 1;
            DEF_SIDE.destroyed = 1;
        }
    } else if (ATK_SIDE.value > DEF_SIDE.value) {
        int pierce = 0;
        if (!DEF_SIDE.defending)
            pierce = 1;
        if (CountActiveZoneLinksFromCard(attacker, gBattle.atkSlot, 0x521))
            pierce = 1;
        if (CARD_NUMBER(ATK_SIDE.cardId) == 0x53D)
            pierce = 1;
        if (CountActiveZoneLinksFromCard(attacker, gBattle.atkSlot, 0x604))
            pierce = 1;
        if (pierce)
            DEF_SIDE.damage = ATK_SIDE.value - DEF_SIDE.value;
        DEF_SIDE.destroyed = 1;
    } else {
        ATK_SIDE.damage = DEF_SIDE.value - ATK_SIDE.value;
        if (!DEF_SIDE.defending)
            ATK_SIDE.destroyed = 1;
    }
    switch (CARD_NUMBER(ATK_SIDE.cardId)) {
    case 0x4CF:
        ATK_SIDE.destroyed = 0;
        ATK_SIDE.damage = 0;
        QueueAddZoneLink(attacker, ATK_SIDE.cardId, (u8)(1 - attacker) | (gBattle.defSlot << 8), 3);
        break;
    case 0x4E3:
        if (DEF_SIDE.atk > 0x76B)
            ATK_SIDE.destroyed = 0;
        break;
    case 0x199:
        if (GetZoneCardAttribute(1 - attacker, gBattle.defSlot) == 2)
            DEF_SIDE.destroyed = 1;
        break;
    }
    if (CountActiveZoneLinksFromCard(attacker, gBattle.atkSlot, 0x49E) && GetZoneCardType(1 - attacker, gBattle.defSlot) == 1)
        DEF_SIDE.flag6 = 1;
    if (CountActiveZoneLinksFromCard(1 - attacker, gBattle.defSlot, 0x49E) && GetZoneCardType(attacker, gBattle.atkSlot) == 1)
        ATK_SIDE.flag6 = 1;
    if (CARD_NUMBER(DEF_SIDE.cardId) == 0x4E3 && ATK_SIDE.atk > 0x76B)
        DEF_SIDE.destroyed = 0;
    /* Test player flag bit 1 with the original signed-shift form. */
    if ((s32)((u32)gDuelPlayers[(1 - attacker) & 1].unk8 << 30) < 0) {
        DEF_SIDE.destroyed = 0;
        DEF_SIDE.damage = 0;
    }
    if (DEF_SIDE.damage && CountActiveCardsOnFieldExcept(1 - attacker, 0x58F, -1) > 0)
        DEF_SIDE.damage = 0;
    if (ATK_SIDE.damage && CountActiveCardsOnFieldExcept(attacker, 0x58F, -1) > 0)
        ATK_SIDE.damage = 0;
    for (i = 0; i <= 1; i++)
        gBattle.side[i].destroyedCopy = gBattle.side[i].destroyed;
}

void CardMenu_DrawIcons(void)
{
    int i;
    int count = 0;
    u16 tile;
    int x, y;

    for (i = 0; i <= 12; i++) {
        s32 m = gDuelState->sel.mask;
        m >>= i;
        if (m & 1)
            count++;
    }
    tile = 0x2624;
    x = 0x78 - count * 8;
    y = 0xA0 - gDuelState->sel.rows * 10;
    switch (gDuelScreen.zone) {
    case 13:
        x = 0xD8 - count * 8;
        break;
    case 12:
        x = 0x28 - count * 8;
        break;
    }
    gDuelState->sel.timer++;
    for (i = 0; i <= 12; i++) {
        s32 m = gDuelState->sel.mask;
        m >>= i;
        if (m & 1) {
            if (gDuelState->sel.cursor == i)
                AddAffineSprite((y << 16) | x, 0x40, tile, gPulseScaleCurve[(gDuelState->sel.timer >> 1) & 0xF] << 16);
            else
                AddSprite((y << 16) | x, 0x40, tile);
            x += 16;
        }
        tile += 4;
    }
}

void CardMenu_DrawLabel(void)
{
    int i;
    int count = 0;
    u16 tile;
    int x;

    for (i = 0; i <= 12; i++) {
        s32 m = gSelMask.mask;
        m >>= i;    /* Separate shift, which keeps the lsr #16 inside the loop. */
        if (m & 1)
            count++;
    }
    tile = 0x3664;
    x = 0x70 - count * 8;
    switch (gDuelScreen.zone) {
    case 13:
        x = 0xD0 - count * 8;
        break;
    case 12:
        x = 0x20 - count * 8;
        break;
    }
    for (i = 0; i <= 12; i++) {
        if ((gSelMask.mask >> i) & 1) {
            if (gSelMask.cursor == i)
                AddSprite((0x60 << 16) | x, 0x4080, tile);
            x += 16;
        }
        tile += 8;
    }
}
void CardMenu_DrawCardPreview(void)
{
    u16 tile;
    int x0, y0;
    int x, y;
    int player;

    /* duel_ui.h declares DuelScreen.zone as u32; this unit switches on it as s32 (signed compares). */
    switch ((s32)gDuelScreen.zone) {
    case 12:
    case 13:
        return;
    }
    tile = GetCardIconObjTile(DuelCursor_GetCardId()) + 0x1400;
    if (gDuelState->sel.rows == 1 || gDuelState->sel.rows == 7) {
        switch ((s32)gDuelScreen.zone) {
        case 0:
        case 5:
            ClearZoneTiles(gDuelScreen.player, gDuelScreen.zone + gDuelScreen.cursor);
            break;
        case 10:
            ClearZoneTiles(gDuelScreen.player, 10);
            break;
        }
    }
    if ((u32)gDuelState->sel.rows < 8) {
        x0 = GetAreaX(gDuelScreen.player, gDuelScreen.zone, gDuelScreen.cursor);
        y0 = GetAreaY(gDuelScreen.player, gDuelScreen.zone, gDuelScreen.cursor);
        x = (0x68 - x0) * gDuelState->sel.rows / 8;
        y = (0x20 - y0) * gDuelState->sel.rows / 8;
        x += x0;
        y += y0;
        switch ((s32)gDuelScreen.zone) {
        case 0:
        case 5:
        case 10:
            player = gDuelScreen.player & 1;
            if (!((struct DuelZone *)((u8 *)gDuel.players[0].zones + (gDuelScreen.zone + gDuelScreen.cursor) * 0x94 + player * 0xD64))->flag6_1) {
                switch (gDuelState->sel.rows) {
                case 0:
                case 1:
                case 2:
                    tile = gDuelState->sel.rows * 16 + 0x440;
                    break;
                case 3:
                case 4:
                    tile += (5 - gDuelState->sel.rows) * 16;
                    break;
                }
            }
            break;
        }
        if ((u32)y < 0xA0)
            AddAffineSprite(x | (y << 16), 0x80, tile, gShrinkScaleSteps[gDuelState->sel.rows] << 16);
    } else {
        AddAffineSprite(0x00200068, 0x80, tile, 0x800000);
    }
}
void PlaySE(u16 se);          /* PlaySE */
void CardListView_Open(u32 player, u32 a, u32 b, u32 c);
void DrawZoneTiles(s32, s32);
u32 DuelScreen_FadeOutStep(void);
u32 DuelScreen_FadeInStep(void);
void CardDetail_Init(u16 card, u16 b, u16 c);
u16 CardDetail_Run(void);
void DuelScreen_Init(void);
void DuelScreen_DrawCursorInfo(void);

#define SEL gDuelState->sel
#define SEL_HAS(n) ((s32)SEL.mask >> (n) & 1)

void CardMenu_Update(void)
{
    int i;

    switch (SEL.state) {
    case 0:
        gDuelScreen.busy = 0;
        SEL.rows = 0;
        SEL.cursor = 0;
        for (i = 0; !SEL_HAS(SEL.cursor); ) {
            if (SEL.cursor)
                SEL.cursor--;
            else
                SEL.cursor = 12;
            if (++i > 12)
                break;
        }
        SEL.state++;
        break;
    case 1:
        CardMenu_DrawCardPreview();
        SEL.rows++;
        CardMenu_DrawIcons();
        if (SEL.rows > 7)
            SEL.state++;
        break;
    case 2:
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        CardMenu_DrawLabel();
        if (gMain.newKeys & 0x20) {
            for (i = 0; i <= 12; i++) {
                if (SEL.cursor)
                    SEL.cursor--;
                else
                    SEL.cursor = 12;
                if (SEL_HAS(SEL.cursor))
                    break;
            }
            PlaySE(0);
        }
        if (gMain.newKeys & 0x10) {
            for (i = 0; i <= 12; i++) {
                if (SEL.cursor < 12)
                    SEL.cursor++;
                else
                    SEL.cursor = 0;
                if (SEL_HAS(SEL.cursor))
                    break;
            }
            PlaySE(0);
        }
        if (gMain.newKeys & 2) {
            PlaySE(2);
            SEL.active = 0;
            SEL.state++;
        } else if (gMain.newKeys & 1) {
            SEL.active = 1;
            SEL.unk34 = 0;
            gDuelState->selCard = DuelCursor_GetCardId();
            SEL.player = gDuelScreen.player;
            SEL.zone = (u16)gDuelScreen.zone;
            SEL.unk65 = gDuelScreen.cursor;
            if (!SEL.cursor) {
                if (SEL.zone == 12) {
                    CardListView_Open(SEL.player, 12, 0, 0);
                    PlaySE(1);
                    SEL.state = 2;
                } else {
                    PlaySE(1);
                    SEL.active = 0;
                    SEL.state = 10;
                }
            } else {
                SEL.state++;
            }
        }
        break;
    case 3:
        SEL.rows--;
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        if (!SEL.rows) {
            switch ((s32)gDuelScreen.zone) {
            case 0:
            case 5:
                DrawZoneTiles(gDuelScreen.player, gDuelScreen.zone + gDuelScreen.cursor);
                break;
            case 10:
                DrawZoneTiles(gDuelScreen.player, 10);
                break;
            }
            SEL.state++;
        }
        break;
    case 10:
        if (DuelScreen_FadeOutStep()) {
            gDuelScreen.flag0_1 = 0;
            gDuelScreen.flag0_2 = 0;
            CardDetail_Init(gDuelState->selCard, 0, 0);
            SEL.state++;
        }
        break;
    case 11:
        if (CardDetail_Run()) {
            DuelScreen_Init();
            DuelScreen_DrawCursorInfo();
            switch ((s32)gDuelScreen.zone) {
            case 0:
            case 5:
                ClearZoneTiles(gDuelScreen.player, gDuelScreen.zone + gDuelScreen.cursor);
                break;
            case 10:
                ClearZoneTiles(gDuelScreen.player, 10);
                break;
            }
            SEL.state++;
        }
        break;
    case 12:
        CardMenu_DrawCardPreview();
        CardMenu_DrawIcons();
        CardMenu_DrawLabel();
        if (DuelScreen_FadeInStep())
            SEL.state = 2;
        break;
    default:
        SEL.flag0 = 0;
        SEL.state = 0;
        break;
    }
}
