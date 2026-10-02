#include "global.h"

struct Ui {
    u8 u0[8];
    u16 x;      /* +8 */
    u16 y;      /* +0xA */
    u8 uC[2];
    u16 h;      /* +0xE */
    u8 u10[4];
    u16 sel;    /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;   /* +0x22 */
    u8 timer;   /* +0x23 */
};
extern struct Ui gUnk_0201AE60;
struct SelMask {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u32 unk65 : 8;
    u32 unk73 : 23;
};
struct DuelGlobal {
    u8 unk0[0x1B12];
    u8 b1B12;
    u8 unk1B13;
    u32 f1B14_0 : 9;
    u32 stage : 8;
    u32 f1B14_17 : 15;
    u16 f1B18;
    u8 unk1B1A[0x1B20 - 0x1B1A];
    u8 step;    /* +0x1B20 */
    u8 unk1B21[0x1B26 - 0x1B21];
    u8 f1B26_0 : 1;
    u8 f1B26_1 : 7;
    u8 unk1B27[0x1B2C - 0x1B27];
    struct SelMask sel;
};
extern struct DuelGlobal gUnk_020192E0;
struct PS {
    u8 u0[8];
    u8 b8_0 : 4;
    u8 b8_4 : 1;
    u8 b8_5 : 1;
    u8 b8_6 : 2;
    u8 b9_0 : 3;
    u8 b9_3 : 1;
    u8 b9_4 : 1;
    u8 b9_5 : 1;
    u8 b9_6 : 2;
    u8 uA;
    u8 bB_0 : 3;
    u8 bB_3 : 1;
    u8 bB_4 : 4;
    u8 bC_0 : 5;
    u8 bC_5 : 1;
    u8 bC_6 : 2;
    u8 rest[0xD64 - 0xD];
};
extern struct PS gUnk_020192E4[];
struct Ui2 { u8 u0[1]; u8 b1; u8 pad[0x20]; };
extern struct Ui2 gUnk_02015EE8;
extern u8 gUnk_02015EF0[];
void sub_080240A8(int player, int a);
void sub_0802297C(u16 msg, u16 a, u16 b, u16 c);
void sub_0804EFF0(int player);
void sub_08046738(int player, int zone);
void sub_08017AB4(int player, u16 cardId, u16 pos, u16 a);
int sub_0800C8BC(int player, int zone);
extern const u8 gUnk_08085C28[], gUnk_08085C8C[];
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, void (*b)(void), int (*c)(void));
int sub_0804A92C(int player);
int sub_0804A1C8(void);
int sub_0804E31C(int a);
void sub_0804F310(void);
int sub_0804F384(void);
struct Main { u8 u0[6]; u16 keys; };
extern struct Main gUnk_03000040;
struct Zone {
    u32 card;   /* +0: 12-bit card id */
    u16 w4;
    u8 f6;      /* +6: bit 1 = face-down */
    u8 f7;
    u8 pad[0x8C - 8];
    u8 b8C_0 : 4;
    u8 b8C_4 : 1;
    u8 b8C_5 : 3;
    u8 pad2[0x94 - 0x8D];
};
extern struct Zone gUnk_0201930C[];
#define ZB(p, z) ((struct Zone *)((z) * 0x94 + (p) * 0xD64 + (u32)&gUnk_0201930C[0]))
extern const u16 gUnk_08622AB4[];
#define TBL(id) (*(u16 *)((u8 *)gUnk_08622AB4 + (((id) & 0x7FF) << 1)))
int sub_0802CFD0(int player, int zone, u16 kind);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
void sub_08077AEC(u16 se);
int sub_0802B9EC(int a, int b);

#define ZB2(p, z) ((struct Zone *)((p) * 0xD64 + (z) * 0x94 + (u32)&gUnk_0201930C[0]))
void sub_0804EFF0(int player)
{
    int i;
    for (i = 0; i < 5; i++) {
        if ((ZB2(player & 1, i)->card << 20) != 0 && (ZB2(player & 1, i)->f6 & 2)) {
            int found = 0;
            if (ZB2(player & 1, i)->b8C_4)
                ZB2(player & 1, i)->b8C_4 = 0;
            switch (((const u16 *)0x08622AB4)[(ZB2(player & 1, i)->card << 20 >> 20) & 0x7FF]) {
            case 0x52:
                if (!(ZB2(player & 1, i)->f7 & 0x20))
                    found = 1;
                break;
            case 0x62:
                if (((u32)(ZB2(player & 1, i)->f6 << 26) >> 28) <= 3)
                    sub_08046738(player, i);
                break;
            }
            if (found != 0 && ((u32)(ZB2(player & 1, i)->f6 << 26) >> 28) <= 5) {
                int p, j;
                for (p = 0; p < 2; p++) {
                    for (j = 0; j < 5; j++) {
                        if ((ZB2(p & 1, j)->card << 20) != 0 && (ZB2(p & 1, j)->f6 & 2) && sub_0800C8BC(p, j) == 2
                            && ZB2(p & 1, j)->w4 <= ZB2(player & 1, i)->w4)
                            sub_08017AB4(player, (u8)player | (u8)i << 8, (u8)p | (u8)j << 8, 2);
                    }
                }
            }
        }
    }
}

/* Byte +0x1B12 of the duel global: bit 1 is the current player. Padded past 4 bytes so it is read with ldrb. */
struct F168Flags {
    u8 b0 : 1;
    u8 cur : 1;
    u8 rest : 6;
    u8 pad[4];
};
/* gUnk_020192E0 with the player array at +4 (the unit's struct DuelGlobal has no players member). */
struct F168Duel {
    u32 u0;
    struct PS players[2];
    u8 pad[0x1B12 - 4 - 2 * 0xD64];
    u8 flags;   /* +0x1B12 */
    u8 pad2[0x1B20 - 0x1B13];
    u8 step;    /* +0x1B20 */
};
#define F168_CUR(p) (((struct F168Flags *)(p))->cur)

/* Step machine on gUnk_020192E0.step; returns 1 when finished. */
int sub_0804F168(void)
{
    struct F168Duel *e = (struct F168Duel *)&gUnk_020192E0;
    switch (e->step) {
    case 0:
        sub_080240A8(F168_CUR(&e->flags), 0xB);
        e->step++;
        return 0;
    case 1: {
        struct PS *ps = e->players;
        /* bit 3 of byte +9, tested as the sign of the byte shifted left by 4 (the ROM's lsl #28; bge) */
        if ((s8)(((u8 *)&ps[F168_CUR(&e->flags) & 1])[9] << 4) < 0) {
            ps[F168_CUR(&e->flags) & 1].b9_3 = 0;
            if (!(gUnk_02015EE8.b1 & 1) && !(e->flags & 2)) {
                gUnk_02015EF0[0] = 0;
                gUnk_02015EF0[1] = 0;
            }
            if (gUnk_02015EE8.b1 & 1)
                sub_0802297C(0xF002, 0, 0, 0);
            gUnk_02015EE8.u0[0] += 5;
            return 1;
        }
        sub_0801EC58((e->flags & 2) ? 0x8001 : 1, 0, 0, 0);
        e->step++;
        return 0;
    }
    case 2:
        sub_0804EFF0(F168_CUR(&e->flags));
        sub_0804EFF0(1 - F168_CUR(&e->flags));
        e->step++;
        return 0;
    default:
        /* the same flag byte addressed from the players symbol (0x020192E4 + 0x1B0E) */
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].b9_4 = 0;
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].b9_5 = 0;
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].b8_4 = 0;
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].b8_5 = 0;
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].bB_3 = 0;
        gUnk_020192E4[F168_CUR((u8 *)gUnk_020192E4 + 0x1B0E)].bC_5 = 0;
        return 1;
    }
}

/* Draws the menu cursor sprite (blinks while confirming). */
void sub_0804F310(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) * 8;
    int y = (u->y + 2) * 8 + u->sel * 12 - 2;
    y -= (u->h + u->y - u->b21 + 2) * 8;
    if (u->state == 1) {
        if (u->timer & 2)
            sub_080761F0((y << 16) | x, 0, 0x431F);
    } else {
        sub_080761F0((y << 16) | x, 0, 0x431F);
    }
}

/* 3-choice menu (A = confirm, B = choose last, up/down move); returns 1 when finished. */
int sub_0804F384(void)
{
    u32 st = gUnk_0201AE60.state;
    switch ((u8)st) {
    case 1:
        if (gUnk_0201AE60.timer <= 0x3B)
            gUnk_0201AE60.timer++;
        else
            gUnk_0201AE60.state = st + 1;
        break;
    case 2:
        return 1;
    default:
        if (gUnk_03000040.keys & 0x80) {
            sub_08077AEC(0);
            if (gUnk_0201AE60.sel <= 1)
                gUnk_0201AE60.sel++;
            else
                gUnk_0201AE60.sel = 0;
        }
        if (gUnk_03000040.keys & 0x40) {
            sub_08077AEC(0);
            if (gUnk_0201AE60.sel != 0)
                gUnk_0201AE60.sel--;
            else
                gUnk_0201AE60.sel = 2;
        }
        if (gUnk_03000040.keys & 1) {
            sub_08077AEC(1);
            gUnk_0201AE60.state = 1;
            gUnk_0201AE60.timer = 0;
        }
        if (gUnk_03000040.keys & 2) {
            sub_08077AEC(2);
            gUnk_0201AE60.sel = 2;
            gUnk_0201AE60.state = 1;
            gUnk_0201AE60.timer = 0;
        }
        break;
    }
    return 0;
}
/* Confirmation-menu step machine on gUnk_020192E0.step. */
int sub_0804F460(void)
{
    switch (gUnk_020192E0.step) {
    case 0:
        sub_0801EC58(0x52, 0, 0, 0);
        gUnk_020192E0.sel.flag0 = 0;
        gUnk_020192E0.sel.active = 0;
        gUnk_020192E0.step++;
        return 0;
    case 10:
        if (sub_0804A92C(0)) {
            sub_080602A4(0x205, 0x615, 0xB, gUnk_08085C28);
            sub_08060308(5, sub_0804F310, sub_0804F384);
            gUnk_020192E0.step = 0x14;
        } else {
            sub_080602A4(0x206, 0x412, 0xB, gUnk_08085C8C);
            sub_08060308(1, 0, 0);
            gUnk_020192E0.step = gUnk_020192E0.step + 1;
        }
        return 0;
    case 11:
        if (gUnk_0201AE60.sel != 0)
            return 1;
        gUnk_020192E0.step = 1;
        return 0;
    case 20:
        if (gUnk_0201AE60.sel != 0) {
            if (gUnk_0201AE60.sel == 1)
                return 1;
            gUnk_020192E0.step = 1;
        } else {
            gUnk_020192E0.f1B26_0 = 0;
            gUnk_020192E0.stage = 0;
            gUnk_020192E0.step = gUnk_020192E0.step + 1;
            /* FAKEMATCH: keep this completed store separate from the
             * step=1 tail shared by other cases. Emits no instructions. */
            __asm__ volatile("" ::: "memory");
        }
        return 0;
    case 21:
        if (sub_0804E31C(0)) {
            if (gUnk_020192E0.f1B26_0)
                return 1;
            gUnk_020192E0.step = 1;
        }
        return 0;
    default:
        if (sub_0804A1C8() == 0 && (gUnk_03000040.keys & 2))
            gUnk_020192E0.step = 10;
        return 0;
    }
    return 0;
}

int sub_0804F654(u16 id)
{
    switch (id) {
    case 0x44B:
        return 2000;
    case 0x482:
        return 700;
    case 0x58C:
        return 1000;
    case 0x3BA:
    case 0x590:
        return 500;
    default:
        return 0;
    }
}
 /* 0x0804F654 size 0x54 */
int sub_0804F6A8(int a, int b)
{
    int n = 0;
    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            if (i == a && j == b)
                continue;
            if (sub_0802B9EC(i, j) != 0)
                n++;
        }
    }
    return n;
}
 /* 0x0804F6A8 size 0x44 */
int sub_0804F6EC(int player)
{
    int i = 0;
    /* FAKEMATCH: retain this initialized player offset in the ROM's
     * callee-saved register; player itself stays live for the predicate. */
    register u32 poff __asm__("r6") = (player & 1) * 0xD64;
    for (; i < 10; i++) {
        struct Zone *z = (struct Zone *)(i * 0x94 + poff + (u32)gUnk_0201930C);
        u32 id = z->card << 20 >> 20;
        if (id != 0) {
            switch (((const u16 *)gUnk_08622AB4)[(u16)id & 0x7FF]) {
            case 0x243:
            case 0x1A0:
            case 0x2DB:
                if (i > 4)
                    continue;
                if (!(((struct Zone *)(i * 0x94 + poff + (u32)gUnk_0201930C))->f6 & 2))
                    continue;
                break;
            case 0x428:
                if ((z->f6 & 2) || i <= 4)
                    continue;
                break;
            default:
                continue;
            }
            if (sub_0802CFD0(player, i, 2))
                return 1;
        }
    }
    return 0;
}
void sub_080197E0(int player, u16 id);
void sub_08019860(int player, int amount);
void sub_08019980(int player, int amount);
void sub_08018ED8(int player, int zone, int a, int b);
void sub_08018544(int player, int zone, int a);
void sub_08046E8C(int player);
int sub_08008860(int player);
int sub_0800A8CC(int player, int zone, u16 number);
int sub_0800A78C(int player, int zone, u16 number);
int sub_0800C894(int player, int zone);
int sub_0800A1C4(int player);
int sub_08009CAC(int player, u16 number);
int sub_0807548C(int amount);
/* Player block view (0xD64 bytes): an array-of-struct declaration is needed so that
 * gEndPS_020192E4[x].field goes through get_inner_reference (constant base loaded first,
 * and the 0x46B case's index `player > 99` scaled separately from the base). */
struct EndPS {
    u16 life;           /* +0 */
    u8 pad2_[9];
    u8 bB;              /* +0xB: bits 4-7 = low 4 bits of a 5-bit zone mask */
    u8 bC;              /* +0xC: bit 0 = high bit of that mask */
    u8 pad[0x28 - 0xD];
    struct Zone zones[22]; /* +0x28 */
    u8 pad2[0x84];
};
extern struct EndPS gEndPS_020192E4[];
#define EZB(p, z) ((struct Zone *)((z) * 0x94 + (p) * 0xD64 + (u32)((u8 *)gEndPS_020192E4 + 0x28)))
#define END_ID(z) (((z)->card << 20) >> 20)
#define END_NUMBER(id) (((const u16 *)0x08622AB4)[(u16)(id) & 0x7FF])
void sub_0804F7A4(int player)
{
    int i;
    u32 id1 = 0x527;
    for (i=0; i<=4; i++) {
        int lo = gEndPS_020192E4[player&1].bB >> 4;
        if ((((int)(((gEndPS_020192E4[player&1].bC&1)<<4)|lo) >> i)&1) &&
            (gEndPS_020192E4[player&1].zones[i].card << 20) == 0) {
            sub_080197E0(player,((const u16 *)0x08623DF4)[id1]);
            sub_0801EC58(player ? 0x80AA : 0xAA,(u16)i,1,0);
        }
    }
    for (i=0; i<=4; i++) {
        struct Zone *z=EZB(player&1,i);
        u16 id=END_ID(z);
        u32 face=((u32)z->f6<<30)>>31;
        u32 position=((u32)z->f6<<31)>>31;
        if (id && face) {
            switch (END_NUMBER(id)) {
            case 0x228:
                sub_080197E0(player,id);
                sub_08019860(player,300);
                break;
            case 0x459: {
                int count=sub_08008860(player);
                if (count==1) {
                    sub_080197E0(player,id);
                    if (!(z->f6 & count)) sub_08018ED8(player,i,0,0);
                    z->f7 |= 4;
                }
                break;
            }
            case 0x59D:
                if (position) break;
                goto gain1000;
            case 0x59E:
                if (!position) break;
            gain1000:
                sub_080197E0(player,id);
                sub_08019980(player,1000);
                break;
            case 0x5A1:
                sub_080197E0(player,id);
                sub_08019980(player,800);
                break;
            }
            {
            int number = 0x58B;
            if (sub_0800A8CC(player,i,number)) {
                sub_080197E0(player,((const u16 *)0x08623DF4)[number]);
                sub_08019860(player,sub_0800A8CC(player,i,number)*500);
            }
            }
            {
            int number = 0x2DF;
            if (sub_0800A78C(player,i,number)) {
                int amount=sub_0800C894(player,i);
                sub_080197E0(player,((const u16 *)0x08623DF4)[number]);
                sub_08018544(player,i,1);
                sub_08019860(player,amount);
            }
            }
            {
            int number = 0x492;
            if (sub_0800A78C(player,i,number)) {
                int amount=sub_0800C894(player,i);
                sub_080197E0(player,((const u16 *)0x08623DF4)[number]);
                amount=sub_0807548C(amount);
                sub_08019980(1-player,amount*sub_0800A78C(player,i,number));
            }
            }
        }
    }
    sub_08046E8C(player);
    for (i=5; i<=10; i++) {
        int p = player & 1;
        struct Zone *z;
        u16 id;
        u32 face;
        u32 disabled;
        /* FAKEMATCH: the do/while(0) wrapper puts loop notes after the zone reads, so the
         * first CSE pass starts a new block there and 0xD64/base stay in the loop; the
         * post-loop CSE then reuses them for the 0x46B case as in the ROM. */
        do {
        z=EZB(p,i);
        id=END_ID(z);
        face=((u32)z->f6<<30)>>31;
        disabled=((u32)((u8 *)z)[0x91]<<28)>>31;
        } while (0);
        if (id && face && !disabled) {
            int count;
            int number = 0x589;
            count=sub_0800A8CC(player,i,number);
            if (count>0) {
                sub_080197E0(player,((const u16 *)0x08623DF4)[number]);
                sub_08019860(player,count*500);
            }
            switch (END_NUMBER(id)) {
            case 0x46B:
                /* This compares player itself to 99 in the ROM. */
                if (gEndPS_020192E4[player > 99].life) {
                    sub_080197E0(player,id);
                    sub_0801EC58(player ? 0x8043 : 0x43,100,1,0);
                }
                break;
            case 0x51F:
                sub_080197E0(player,id);
                sub_08019860(player,500);
                break;
            case 0x527:
                sub_080197E0(player,id);
                break;
            }
        }
    }
    for (i=5; i<=9; i++) {
        int opponent=1-player;
        struct Zone *z=EZB(opponent&1,i);
        u16 id=END_ID(z);
        u32 face=((u32)z->f6<<30)>>31;
        u32 disabled=((u32)((u8 *)z)[0x91]<<28)>>31;
        if (id && face && !disabled) {
            switch (END_NUMBER(id)) {
            case 0x445:
                if (sub_0800A1C4(player)==-1) break;
            case 0x42C:
                sub_080197E0(opponent,id);
                sub_08019980(player,1000);
                break;
            case 0x516:
                sub_080197E0(opponent,id);
                sub_08019860(player,500);
                break;
            case 0x51F:
                sub_080197E0(player,id);
                sub_08019860(player,500);
                break;
            }
        }
    }
    {
    int number = 0x5A6;
    if (sub_08009CAC(player,number)>0) {
        sub_080197E0(player,((const u16 *)0x08623DF4)[number]);
        sub_08019980(player,200*sub_08009CAC(player,number));
    }
    }
}
struct FcPlayer {
    u16 life; u8 handCount; u8 pad3[3]; u8 listCount; u8 pad7;
    u8 flags8; u8 flags9; u8 padA; u8 flagsB; u8 flagsC;
    u8 padD[0xCC4-0xD]; u16 cardList[(0xD64-0xCC4)/2];
};
typedef char FcPlayerStride[(sizeof(struct FcPlayer)==0xD64)?1:-1];
extern const u16 gUnk_08623DF4[], gUnk_0862457C[], gUnk_086249F4[];
extern const u32 gUnk_08621DE0[];
extern const char gUnk_08085C9C[], gUnk_08085CB4[], gUnk_08085D08[], gUnk_08085D70[];
extern const char gUnk_0822C720[];
extern u8 gUnk_0201CFB0[];
void sub_0804F7A4(int player);
int sub_0804691C(int player);
int sub_08008A1C(int player);
int sub_08008A44(int player);
int sub_080195D0(int player, u16 number, u32 *card);
void sub_080197E0(int player, u16 id);
void sub_080197C0(int player, u16 id);
void sub_08055F70(int player, u32 *card, u16 a, u16 b, u16 c);
int sub_08008524(int player, u16 number);
int sub_080086CC(int player, u16 number);
int sub_0800842C(int player, u16 number);
int sub_080088A4(int player, int a, int b);
void sub_08022678(int player, int a, u16 number, u16 b);
void sub_0801FBCC(u32 event, int a);
void sub_080241C4(void);
int sub_0804F6EC(int player);
int sub_08076F9C(void);
void sub_08018544(int player, int zone, int a);
int sub_08008AF8(int player, int exclude);
int sub_0804F654(u16 number);
int sub_08056E04(int player, u16 number);
void sub_080753F4(char *dst, const char *format, const char *arg);
void sub_08075434(char *dst, const char *format, int arg);
u32 sub_08052F38(u32 a);
int sub_08017FF4(int a, int b);
struct FcState {
    u32 header; struct FcPlayer players[2]; u8 pad1ACC[0x1B12-0x1ACC]; u8 flags; u8 pad13[0x1B20-0x1B13];
    u8 step, zone, cursor, subcursor; u8 pad24[0x1B64-0x1B24]; u16 choice;
};
/* The ROM also addresses the same step byte from 0x020192E4 + 0x1B1C. */
struct FcPlayerStateView {
    struct FcPlayer players[2];
    u8 pad1AC8[0x1B1C-0x1AC8];
    u8 step;
};
#define FC_PLAYER_STEP (((struct FcPlayerStateView *)gUnk_020192E4)->step)
struct FcFlags { u8 pad0[9]; u8 bit0:1; u8 bit1:1; u8 rest:6; };
#define FC_E ((u8 *)&gUnk_020192E0)
#define FC_STEP (((struct FcState *)&gUnk_020192E0)->step)
#define FC_ZONE (((struct FcState *)&gUnk_020192E0)->zone)
#define FC_CURSOR (((struct FcState *)&gUnk_020192E0)->cursor)
#define FC_SUBCURSOR (((struct FcState *)&gUnk_020192E0)->subcursor)
#define FC_CHOICE (((struct FcState *)&gUnk_020192E0)->choice)
#define FC_PS(p) ((u8 *)gUnk_020192E4+(p)*0xD64)
#define FC_PLAYER(p) (((struct FcState *)&gUnk_020192E0)->players[p])
#define FC_LIFE(p) (((struct FcPlayer *)gUnk_020192E4)[p].life)
#define FC_ID(z) (((z)->card<<20)>>20)
#define FC_NUMBER(id) ((const u16 *)0x08622AB4)[(u16)(id)&0x7FF]
static inline int EndTurnLevel(u16 id)
{
    int type=(((const u32 *)0x08621DE0)[id&0x7FF]&0x1F00000)>>20;
    switch (type) {
    case 21: case 22: case 23: return 0;
    case 24: return 10;
    default: return (((const u32 *)0x08621DE0)[id&0x7FF]&0x1E000000)>>25;
    }
}
static inline u16 EndTurnCardId(u16 number)
{
    if (number==0xFFFF) return 0;
    if (number<=0x7CF) return ((const u16 *)0x08623DF4)[number&0x7FF];
    return ((const u16 *)0x08623DF4)[(number-0x7D0)&0x7FF]+1;
}
#if 0 /* NONMATCHING (score 954): per-case return 0 tails; prologue and cases 0-3,5-8,11,21,100,121,122 match;
       * remaining: loop-invariant hoisting and regalloc in cases 4,9,20,101,102,110,111,120, case 10 tail register; size
       * +18; see build/wf/sub_0804FC4C/NOTES.md */
struct FcFlagsS { u8 pad0[9]; u8 bit0:1; s8 bit1:1; u8 rest:6; };
static inline int FcNum(u32 id) { return ((const u16 *)0x08622AB4)[(u16)id&0x7FF]; }
struct FcCfb0 { u8 pad0[0x824]; int a824; u8 pad828[4]; int a82C; };
int sub_0804FC4C(void)
{
    u32 card; /* Written by sub_080195D0 on success before it is consumed. */
    char text[128];
    char format[128];
    u16 player=((u32)((struct FcState *)&gUnk_020192E0)->flags<<30)>>31;
    u8 *base=FC_E; u8 *b4=base+4; struct FcFlagsS *ps=(struct FcFlagsS *)(b4+player*0xD64);
    if (ps->bit1<0) {
        ps->bit1=0;
        goto done;
    }
    switch (FC_STEP) {
    case 0: {
        u16 msg=0x51;
        if (player) msg=0x8051;
        sub_0801EC58(msg,0,0,0);
        FC_STEP=20;
        return 0;
    }
    case 1:
        sub_0804F7A4(player);
        FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
        FC_STEP++;
        return 0;
    case 2:
        if (sub_0804691C(player)) {
            FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
            FC_STEP++;
        }
        return 0;
    case 3: {
        struct FcPlayer *pl=(struct FcPlayer *)gUnk_020192E4;
        if (((u32)(pl[player].flagsC)<<28)>>29) {
            u16 msg=0x4B;
            if (player) msg=0x804B;
            sub_0801EC58(msg,0,0,0);
            if (sub_08008A1C(player)>0 && sub_080195D0(player,0x57D,&card)) {
                sub_080197E0(player,EndTurnCardId(0x57D));
                sub_08055F70(player,&card,1,1,0x20);
                return 0;
            }
        }
        FC_STEP++;
        FC_CURSOR=0;
        return 0;
    }
    case 4:
        for (;FC_CURSOR<FC_PLAYER(player).listCount;FC_CURSOR++) {
            u32 c=FC_PLAYER(player).cardList[FC_CURSOR]; if (c==0x402) {
                u16 msg;
                sub_080197E0(player,gUnk_0862457C[0]);
                msg=0xCF;
                if (player) msg=0x80CF;
                sub_0801EC58(msg,FC_CURSOR,1,0);
                return 0;
            }
        }
        FC_CURSOR=0; FC_SUBCURSOR=0; FC_ZONE=0;
        FC_STEP++;
        return 0;
    case 5: {
        int other=1-player;
        struct FcPlayer *pl;
        if (sub_08008524(other,0x489) && (pl=(struct FcPlayer *)gUnk_020192E4, pl[other&1].life>499) && pl[player&1].handCount) {
            sub_08022678(other,15,0x489,0);
            FC_PLAYER_STEP++;
        } else FC_STEP=7;
        return 0;
    }
    case 6:
        if (FC_CHOICE) {
            int other, index;
            u32 kind=0x6200000;
            u16 msg=0x43;
            if (player!=1) msg=0x8043;
            sub_0801EC58(msg,500,0,0);
            other=1-player;
            index=sub_0800842C(other,0x489);
            sub_0801FBCC(((u32)(other&1)<<31)|(((index&31)<<16)|kind)|EndTurnCardId(0x489),0);
            FC_STEP=5;
        } else FC_STEP=7;
        return 0;
    case 7: {
        int other=1-player;
        if (sub_080086CC(other,0x5ED) && sub_080088A4(player,1,0)>0) {
            sub_08022678(other,15,0x5ED,0);
            FC_STEP++;
        } else {
            FC_ZONE=5;
            FC_STEP=9;
        }
        return 0;
    }
    case 8:
        if (FC_CHOICE) {
            int other=1-player;
            u32 kind;
            int index=sub_0800842C(other,0x5ED);
            sub_0801FBCC(((u32)(other&1)<<31)|(((index&31)<<16)|(kind=0x6400000))|EndTurnCardId(0x5ED),0);
        }
        FC_ZONE=5;
        FC_STEP=9;
        return 0;
    case 9:
        for (;FC_ZONE<=9;FC_ZONE++) {
            struct Zone *z=(struct Zone *)(FC_ZONE*0x94+player*0xD64+FC_E+0x2C);
            u32 id=FC_ID(z);
            if (id && (z->f6&2) && !(((u8 *)z)[0x91]&8) && FcNum(id)==0x592 && sub_08008A1C(player)>0) {
                u16 msg;
                sub_080197E0(player,FC_ID((struct Zone *)(FC_ZONE*0x94+player*0xD64+FC_E+0x2C)));
                msg=0xA3;
                if (player) msg=0x80A3;
                sub_0801EC58(msg,(u8)sub_08008A44(player)|(FC_ZONE<<8),3,0);
                FC_ZONE++;
                return 0;
            }
        }
        sub_080241C4();
        FC_STEP++;
        return 0;
    case 10:
        if ((u16)sub_0804F6EC(player)==0) {
            sub_080241C4();
            FC_STEP=100;
        } else if (sub_0804A1C8()==0 && (gUnk_03000040.keys&2)) {
            sub_080602A4(0x206,0x713,11,gUnk_08085C9C);
            sub_08060308(1,0,0);
            FC_STEP++;
        }
        return 0;
    case 11:
        switch (gUnk_0201AE60.sel) {
        case 0: FC_STEP=10; break;
        case 1: FC_STEP=100; break;
        }
        return 0;
    case 20: {
        u16 count=sub_08008524(player,0x600);
        for (;count>0;count--) {
            int die=sub_08076F9C()%6+1;
            int p, i;
            u16 msg;
            sub_080197E0(player,gUnk_086249F4[0]);
            msg=0xE4;
            if (player) msg=0x80E4;
            sub_0801EC58(msg,die,0,0);
            msg=0x12;
            if (player) msg=0x8012;
            sub_0801EC58(msg,0,0,0);
            for (p=0;p<=1;p++) {
                for (i=0;i<=4;i++) {
                    struct Zone *z=ZB(p&1,i);
                    u32 id=FC_ID(z);
                    if (id && (z->f6&2)) {
                        int level=EndTurnLevel(id);
                        int destroy=0;
                        if (level==die) destroy=1;
                        if (level>5 && die==6) destroy=1;
                        if (destroy) sub_08018544(p,i,1);
                    }
                }
            }
        }
        FC_STEP=1;
        return 0;
    }
    case 21: FC_STEP=20; break;
    case 100:
        FC_ZONE=0;
        FC_STEP++;
        /* fall through */
    case 101:
        for (;FC_ZONE<=9;FC_ZONE++) {
            struct Zone *z=(struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+player*0xD64);
            u32 id=FC_ID(z);
            if (id && (z->f6&2)) {
                int destroy=0;
                switch (FC_NUMBER(id)) {
                case 0x47A:
                    if (sub_08008AF8(player,-1)>0) { FC_STEP=120; return 0; }
                    destroy=1;
                    break;
                case 0x597:
                    if (sub_08008AF8(player,FC_ZONE)>0) { FC_STEP=120; return 0; }
                    destroy=1;
                    break;
                case 0x3BA: case 0x44B: case 0x482: case 0x58C: case 0x590:
                    if (FC_LIFE(player)<sub_0804F654(FC_NUMBER(id))) destroy=1;
                    else { FC_PLAYER_STEP=110; return 0; }
                    break;
                }
                if (destroy) {
                    u16 msg=0x74;
                    if (player) msg=0x8074;
                    sub_0801EC58(msg,id,1,0);
                    sub_08018544(player,FC_ZONE,1);
                }
            }
        }
        FC_STEP++;
        return 0;
    case 102: {
        int i;
        for (i=5;i<=9;i++) {
            struct Zone *z=ZB(player,i);
            u32 id=FC_ID(z);
            if (id && (z->f6&2)) {
                u16 number=FC_NUMBER(id);
                if (number==0x416 || number==0x424) {
                    u16 msg=0xB4;
                    if (player) msg=0x80B4;
                    sub_0801EC58(msg,i,1,0);
                }
            }
        }
        FC_STEP++;
        return 0;
    }
    case 110: {
        u32 id=FC_ID((struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+(player&1)*0xD64));
        if (player) {
            switch (FC_NUMBER(id)) {
            case 0x3BA: {
                int found=0;
                if (sub_08056E04(1,0x2D6)) found=1;
                if (sub_08056E04(1,0x2D7)) found=1;
                if (sub_08056E04(1,0x2D8)) found=1;
                if (sub_08056E04(1,0x2FE)) found=1;
                if (sub_080086CC(1,0x2D6)) found=1;
                if (sub_080086CC(1,0x2D7)) found=1;
                if (sub_080086CC(1,0x2D8)) found=1;
                if (sub_080086CC(1,0x2FE)) found=1;
                gUnk_0201AE60.sel=0;
                if (found && (((struct FcState *)&gUnk_020192E0)->players[1].life)>1000) gUnk_0201AE60.sel=1;
                break;
            }
            case 0x44B:
                if ((((struct FcState *)&gUnk_020192E0)->players[1].life)>7000) gUnk_0201AE60.sel=1;
                else gUnk_0201AE60.sel=0;
                break;
            default:
                if ((((struct FcState *)&gUnk_020192E0)->players[1].life)>sub_0804F654(FC_NUMBER(id))+1000) gUnk_0201AE60.sel=1;
                else gUnk_0201AE60.sel=0;
                break;
            }
        } else {
            sub_080753F4(format,gUnk_08085CB4,gUnk_0822C720+(id<<6));
            sub_08075434(text,format,sub_0804F654(FC_NUMBER(id)));
            sub_080602A4(0x206,0x613,11,text);
            sub_08060308(1,0,0);
        }
        sub_080197C0(player,id);
        FC_STEP++;
        return 0;
    }
    case 111:
        if (gUnk_0201AE60.sel) {
            u32 id=FC_ID((struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+player*0xD64));
            u16 msg=0x43;
            if (player) msg=0x8043;
            sub_0801EC58(msg,sub_0804F654(FC_NUMBER(id)),1,0);
            if (FC_NUMBER(id)==0x3BA) {
                msg=0xB3;
                if (player) msg=0x80B3;
                sub_0801EC58(msg,FC_ZONE,500,0);
            }
        } else sub_08018544(player,FC_ZONE,1);
        FC_ZONE++;
        FC_STEP=101;
        return 0;
    case 120:
        sub_080753F4(text,gUnk_08085D08,gUnk_0822C720+(FC_ID((struct Zone *)(FC_E+0x2C+FC_ZONE*0x94+player*0xD64))<<6));
        sub_080602A4(0x206,0x813,11,text);
        sub_08060308(1,0,0);
        FC_STEP++;
        return 0;
    case 121:
        if (gUnk_0201AE60.sel) {
            sub_080602A4(0x206,0x712,11,gUnk_08085D70);
            FC_STEP++;
            return 0;
        } else {
            sub_08018544(player,FC_ZONE,1);
            FC_ZONE++;
            FC_STEP=101;
        }
        return 0;
    case 122:
        if (sub_08052F38(0xF0)) {
            sub_08017FF4(((struct FcCfb0 *)gUnk_0201CFB0)->a824,((struct FcCfb0 *)gUnk_0201CFB0)->a82C);
            FC_ZONE++;
            FC_STEP=101;
        }
        return 0;
    default:
        goto done;
    }
    return 0;
done:
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804EFF0", sub_0804FC4C); /* 0x0804FC4C size 0xE24 */


