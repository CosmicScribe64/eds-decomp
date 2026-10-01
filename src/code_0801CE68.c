#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

#define gMain gUnk_03000040

/* Save image (0x02011C20). */
struct SaveData {
    u8 filler0[0x2150];
    u16 unk2150;                    /* 0x2150: incremented at the end of every Campaign match */
};
extern struct SaveData gUnk_02011C20;
#define gSaveData gUnk_02011C20

/* Selection widget at 0x020192E0+0x1B2C = 0x0201AE0C (hypothesis: row of up to 13 choices). */
struct SelMask {
    u16 flag0:1;                    /* bit 0 */
    u16 active:1;                   /* bit 1: a choice was confirmed with A */
    u16 cursor:4;                   /* bits 2-5: selected entry */
    u16 rows:4;                     /* bits 6-9: slide-in / animation counter (0-8) */
    u32 mask:16;                    /* bits 10-25: entry i present if bit i set */
    u32 state:8;                    /* bits 26-33 (straddles 0x1B2F/0x1B30): sub_0801DC04 state */
    u32 unk34:8;                    /* bits 34-41 */
    u32 unk42:8;                    /* bits 42-49 */
    u16 timer:7;                    /* bits 50-56: animation counter (u16 container matters in sub_0801D840) */
    u16 player:1;                   /* bit 57: copy of 0x0201CFB0+0x824 bit 0 */
    u32 zone:7;                     /* bits 58-64 (straddles 0x1B33/0x1B34): copy of 0x0201CFB0+0x828 */
    u32 unk65:8;                    /* bits 65-72: copy of 0x0201CFB0+0x82C */
    u32 unk73:23;
};
extern struct SelMask gUnk_0201AE0C;
/* sub_0801D960 addresses the widget directly as 0x0201AE0C; the other functions go through gUnk_020192E0. */
#define gSelMask gUnk_0201AE0C

/* DuelZone/DuelPlayer/DuelState and gUnk_020192E0/020192E4/0201930C come from duel.h. */
#define ZONE_AT(p, s) (&gUnk_020192E4[p].zones[s])

/* duel.h covers DuelState only up to +0x1B20; selCard (+0x1B28, u16) and sel (+0x1B2C) follow. The sel
   accesses must stay relative to gUnk_020192E0 (base + 0x1B2C), not a direct 0x0201AE0C literal, to match. */
struct DuelStateTail {
    u8 filler0[0x1B28];
    u16 selCard;                    /* +0x1B28: card ID from sub_0805ECFC, shown with sub_0800688C */
    u16 unk1B2A;                    /* +0x1B2A */
    struct SelMask sel;             /* +0x1B2C */
};
#define gDuelState ((struct DuelStateTail *)&gUnk_020192E0)

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
extern struct Battle gUnk_02018450;
#define gBattle gUnk_02018450
#define ATK_SIDE (gBattle.side[attacker])
#define DEF_SIDE (gBattle.side[1 - attacker])

extern const u16 gUnk_08622AB4[];   /* card ID to card number */
/* Integer-address indexing keeps the table load after the masked card ID. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

void sub_08075294(void *dst, const void *src, u32 size);    /* MemCopy16 */
u32 sub_0800C894(u32 player, u32 slot);     /* ATK of the card in a zone */
u32 sub_0800C8A8(u32 player, u32 slot);     /* DEF of the card in a zone */
int sub_0800849C(int player, u16 cardNo, int skipZone);
int sub_0800C8BC(int player, int zone);
int sub_0800CAF0(int player, int zone);
int sub_0800A8CC(int player, int zone, u16 number);
int sub_0807548C(int);
void sub_08017AB4(int player, u16 cardId, u16 pos, u16 a);

/* Zone s of player p. The ROM computes s*0x94 + p*0xD64 on top of the zones base (0x0201930C). */
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (s) * 0x94 + (p) * 0xD64))

extern const u16 gUnk_081A4424[];
extern const u16 gUnk_081A4444[];   /* affine per sel.rows */
u16 sub_0805ECFC(void);
int sub_08062140(u16);
void sub_08060FD0(s32, s32);
int sub_080623AC(s32, s32, s32);
int sub_080623EC(s32, s32, s32);   /* 16-frame affine table for the selected entry */
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

/* gUnk_0201CFB0 (struct DuelScreen) comes from duel_ui.h. */

void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);    /* AddSprite */

typedef u16 (*StepFunc)(void);
extern StepFunc gUnk_08198EAC[];    /* Campaign steps */

/* Unpacked date (see code_080044E4). */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
    u32 weekday:3;
};
void sub_08004914(struct Date *out);    /* today's date (hypothesis) */
u32 sub_080044E4(u32 year, u32 month, s32 day);  /* calendar event flags for a date */
s32 sub_08076F9C(void);                 /* Random */
void sub_08077B24(u16 bgm);
u32 sub_08063B48(u16 id);

void sub_08001C10(u16 textId);
u32 sub_08001AE4(void);
u16 sub_08075A6C(u16 speed);        /* FadeToBlack */

/* Campaign step before the match (hypothesis: calendar-event tournaments). */
u16 sub_0801CE68(void)
{
    struct Date d;

    switch (gMain.step488A) {
    case 0:
        sub_08004914(&d);
        gMain.events = sub_080044E4(d.year, d.month, d.day);
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
        sub_08077B24(0x1F);
        sub_08001C10(0x323);
        sub_08004914(&d);
        gUnk_020192E0.result = sub_08076F9C() & 3;
        if (d.year == 2001 && d.month == 1 && d.day == 9) {
            sub_08001C10(0x322);
            gUnk_020192E0.result = 0;
        }
        gMain.step488A++;
    case 2:
    case 5:
        if (sub_08001AE4()) {
            gMain.seqIndex1 = 0;
            gMain.seqState1 = 0;
            gMain.seqState2 = 0;
            gMain.step488A++;
        }
        return 0;
    case 3:
        if (gUnk_020192E0.result == 0) {
            if (sub_08063B48(0x321)) {
                gMain.seqIndex1 = 0;
                gMain.seqState1 = 0;
                gMain.seqState2 = 0;
                gMain.step488A++;
            }
        } else {
            if (sub_08063B48(0x385)) {
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
        sub_08077B24(0x1F);
        sub_08001C10(0x321);
        gMain.step488A++;
        sub_08004914(&d);
        if (d.year == 2001 && d.month == 1)
            sub_08001C10(0x320);
        return 0;
    case 6:
        return sub_08063B48(0x322);
    }
    return 1;
}

/* Last Campaign step: bump the match counter. */
u16 sub_0801D140(void)
{
    gSaveData.unk2150++;
    return 1;
}

u16 sub_0801D158(void)
{
    switch (gMain.step488A) {
    case 0:
        sub_08001C10(0x191);
        gMain.step488A++;
        break;
    case 1:
        if (sub_08001AE4()) {
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

/* CB_Campaign: step runner over gUnk_08198EAC, index gMain.seqIndexCampaign. */
u16 sub_0801D1E8(void)
{
    StepFunc step = gUnk_08198EAC[gMain.seqIndexCampaign];
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
    return sub_08075A6C(8);
}

/* Battle calculation between gBattle.atkSlot of `attacker` and gBattle.defSlot of the other player. */
void sub_0801D264(int attacker, u16 noAtk)
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
        sub_08075294(copy, ZONE_AT(i & 1, slot), 0x94);
        s->slot = slot;
        s->destroyed = 0;
        s->defending = ZONE_AT(i & 1, slot)->flag6_0;
        s->cardId = ((struct DuelCard *)ZONE_AT(i & 1, slot))->id;
        s->atk = sub_0800C894(i, slot);
        s->def = sub_0800C8A8(i, slot);
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
        if (DEF_SIDE.damage && sub_0800849C(1 - attacker, 0x58F, -1) > 0)
            DEF_SIDE.damage = 0;
        return;
    }
    switch (CARD_NUMBER(ATK_SIDE.cardId)) {
    case 0x1DD:
        if (sub_0800CAF0(1 - attacker, gBattle.defSlot) == 6)
            ATK_SIDE.atk += 1000;
        break;
    case 0x4E5:
        if (sub_0800C8BC(1 - attacker, gBattle.defSlot) == 15) {
            ATK_SIDE.atk += 2000;
            ATK_SIDE.def += 2000;
        }
        break;
    }
    switch (CARD_NUMBER(DEF_SIDE.cardId)) {
    case 0x11F:
        if (sub_0800CAF0(attacker, gBattle.atkSlot) == 1)
            DEF_SIDE.def /= 2;
        break;
    case 0x4E5:
        if (sub_0800C8BC(attacker, gBattle.atkSlot) == 15) {
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
        int n = sub_0800A8CC(attacker, gBattle.atkSlot, 0x291);
        ATK_SIDE.value += sub_0807548C(DEF_SIDE.atk) * n;
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
        if (sub_0800A8CC(attacker, gBattle.atkSlot, 0x521))
            pierce = 1;
        if (CARD_NUMBER(ATK_SIDE.cardId) == 0x53D)
            pierce = 1;
        if (sub_0800A8CC(attacker, gBattle.atkSlot, 0x604))
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
        sub_08017AB4(attacker, ATK_SIDE.cardId, (u8)(1 - attacker) | (gBattle.defSlot << 8), 3);
        break;
    case 0x4E3:
        if (DEF_SIDE.atk > 0x76B)
            ATK_SIDE.destroyed = 0;
        break;
    case 0x199:
        if (sub_0800CAF0(1 - attacker, gBattle.defSlot) == 2)
            DEF_SIDE.destroyed = 1;
        break;
    }
    if (sub_0800A8CC(attacker, gBattle.atkSlot, 0x49E) && sub_0800C8BC(1 - attacker, gBattle.defSlot) == 1)
        DEF_SIDE.flag6 = 1;
    if (sub_0800A8CC(1 - attacker, gBattle.defSlot, 0x49E) && sub_0800C8BC(attacker, gBattle.atkSlot) == 1)
        ATK_SIDE.flag6 = 1;
    if (CARD_NUMBER(DEF_SIDE.cardId) == 0x4E3 && ATK_SIDE.atk > 0x76B)
        DEF_SIDE.destroyed = 0;
    /* Test player flag bit 1 with the original signed-shift form. */
    if ((s32)((u32)gUnk_020192E4[(1 - attacker) & 1].unk8 << 30) < 0) {
        DEF_SIDE.destroyed = 0;
        DEF_SIDE.damage = 0;
    }
    if (DEF_SIDE.damage && sub_0800849C(1 - attacker, 0x58F, -1) > 0)
        DEF_SIDE.damage = 0;
    if (ATK_SIDE.damage && sub_0800849C(attacker, 0x58F, -1) > 0)
        ATK_SIDE.damage = 0;
    for (i = 0; i <= 1; i++)
        gBattle.side[i].destroyedCopy = gBattle.side[i].destroyed;
}

void sub_0801D840(void)
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
    switch (gUnk_0201CFB0.zone) {
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
                sub_08076714((y << 16) | x, 0x40, tile, gUnk_081A4424[(gDuelState->sel.timer >> 1) & 0xF] << 16);
            else
                sub_080761F0((y << 16) | x, 0x40, tile);
            x += 16;
        }
        tile += 4;
    }
}

void sub_0801D960(void)
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
    switch (gUnk_0201CFB0.zone) {
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
                sub_080761F0((0x60 << 16) | x, 0x4080, tile);
            x += 16;
        }
        tile += 8;
    }
}
void sub_0801D9F8(void)
{
    u16 tile;
    int x0, y0;
    int x, y;
    int player;

    /* duel_ui.h declares DuelScreen.zone as u32; this unit switches on it as s32 (signed compares). */
    switch ((s32)gUnk_0201CFB0.zone) {
    case 12:
    case 13:
        return;
    }
    tile = sub_08062140(sub_0805ECFC()) + 0x1400;
    if (gDuelState->sel.rows == 1 || gDuelState->sel.rows == 7) {
        switch ((s32)gUnk_0201CFB0.zone) {
        case 0:
        case 5:
            sub_08060FD0(gUnk_0201CFB0.player, gUnk_0201CFB0.zone + gUnk_0201CFB0.cursor);
            break;
        case 10:
            sub_08060FD0(gUnk_0201CFB0.player, 10);
            break;
        }
    }
    if ((u32)gDuelState->sel.rows < 8) {
        x0 = sub_080623AC(gUnk_0201CFB0.player, gUnk_0201CFB0.zone, gUnk_0201CFB0.cursor);
        y0 = sub_080623EC(gUnk_0201CFB0.player, gUnk_0201CFB0.zone, gUnk_0201CFB0.cursor);
        x = (0x68 - x0) * gDuelState->sel.rows / 8;
        y = (0x20 - y0) * gDuelState->sel.rows / 8;
        x += x0;
        y += y0;
        switch ((s32)gUnk_0201CFB0.zone) {
        case 0:
        case 5:
        case 10:
            player = gUnk_0201CFB0.player & 1;
            if (!((struct DuelZone *)((u8 *)gUnk_020192E0.players[0].zones + (gUnk_0201CFB0.zone + gUnk_0201CFB0.cursor) * 0x94 + player * 0xD64))->flag6_1) {
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
            sub_08076714(x | (y << 16), 0x80, tile, gUnk_081A4444[gDuelState->sel.rows] << 16);
    } else {
        sub_08076714(0x00200068, 0x80, tile, 0x800000);
    }
}
void sub_08077AEC(u16 se);          /* PlaySE */
void sub_0802AF34(u32 player, u32 a, u32 b, u32 c);
void sub_08060ECC(s32, s32);
u32 sub_08060B4C(void);
u32 sub_08060B2C(void);
void sub_0800688C(u16 card, u16 b, u16 c);
u16 sub_08006D08(void);
void sub_080609C4(void);
void sub_0805F96C(void);

#define SEL gDuelState->sel
#define SEL_HAS(n) ((s32)SEL.mask >> (n) & 1)

void sub_0801DC04(void)
{
    int i;

    switch (SEL.state) {
    case 0:
        gUnk_0201CFB0.busy = 0;
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
        sub_0801D9F8();
        SEL.rows++;
        sub_0801D840();
        if (SEL.rows > 7)
            SEL.state++;
        break;
    case 2:
        sub_0801D9F8();
        sub_0801D840();
        sub_0801D960();
        if (gMain.newKeys & 0x20) {
            for (i = 0; i <= 12; i++) {
                if (SEL.cursor)
                    SEL.cursor--;
                else
                    SEL.cursor = 12;
                if (SEL_HAS(SEL.cursor))
                    break;
            }
            sub_08077AEC(0);
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
            sub_08077AEC(0);
        }
        if (gMain.newKeys & 2) {
            sub_08077AEC(2);
            SEL.active = 0;
            SEL.state++;
        } else if (gMain.newKeys & 1) {
            SEL.active = 1;
            SEL.unk34 = 0;
            gDuelState->selCard = sub_0805ECFC();
            SEL.player = gUnk_0201CFB0.player;
            SEL.zone = (u16)gUnk_0201CFB0.zone;
            SEL.unk65 = gUnk_0201CFB0.cursor;
            if (!SEL.cursor) {
                if (SEL.zone == 12) {
                    sub_0802AF34(SEL.player, 12, 0, 0);
                    sub_08077AEC(1);
                    SEL.state = 2;
                } else {
                    sub_08077AEC(1);
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
        sub_0801D9F8();
        sub_0801D840();
        if (!SEL.rows) {
            switch ((s32)gUnk_0201CFB0.zone) {
            case 0:
            case 5:
                sub_08060ECC(gUnk_0201CFB0.player, gUnk_0201CFB0.zone + gUnk_0201CFB0.cursor);
                break;
            case 10:
                sub_08060ECC(gUnk_0201CFB0.player, 10);
                break;
            }
            SEL.state++;
        }
        break;
    case 10:
        if (sub_08060B4C()) {
            gUnk_0201CFB0.flag0_1 = 0;
            gUnk_0201CFB0.flag0_2 = 0;
            sub_0800688C(gDuelState->selCard, 0, 0);
            SEL.state++;
        }
        break;
    case 11:
        if (sub_08006D08()) {
            sub_080609C4();
            sub_0805F96C();
            switch ((s32)gUnk_0201CFB0.zone) {
            case 0:
            case 5:
                sub_08060FD0(gUnk_0201CFB0.player, gUnk_0201CFB0.zone + gUnk_0201CFB0.cursor);
                break;
            case 10:
                sub_08060FD0(gUnk_0201CFB0.player, 10);
                break;
            }
            SEL.state++;
        }
        break;
    case 12:
        sub_0801D9F8();
        sub_0801D840();
        sub_0801D960();
        if (sub_08060B2C())
            SEL.state = 2;
        break;
    default:
        SEL.flag0 = 0;
        SEL.state = 0;
        break;
    }
}
