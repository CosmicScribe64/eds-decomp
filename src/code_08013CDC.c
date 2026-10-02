#include "global.h"
#include "gba.h"
#include "main.h"
#include "duel.h"

/* Command block at 0x020185C0 (current duel command, see code_0800EAA8). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002 */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 unk80A_7:9;
    u32 unk80C_0:5;
    u32 timer:7;        /* 0x80C bits 5-11: frame counter inside a step (u32 container: signed compares) */
    u32 unk80C_12:1;
    u32 running:1;      /* 0x80D bit 5: command in progress */
    u32 unk80C_14:2;
    u32 unk80C_16:16;
};

/* struct DuelCard / DuelZone / DuelPlayer / DuelState come from duel.h. */

/*
 * Local views kept because the canonical declarations in duel.h differ from what
 * this unit needs to match (duel.h is shared, so it is not changed here):
 *
 * - struct DuelPlayer08013CDC: canonical struct DuelPlayer declares +0x06..+0x0D as
 *   u8 fields/bitfields (countB84, flag7_3..turns7_6, unk8, unk9, unkB_0, flagsC).
 *   Most of this unit's flag handlers still match those (sub_08014914 writes flag7_5,
 *   sub_080149D8 writes turns7_6), but sub_08014964 writes two +0x07 bits and only
 *   matches with this unit's original u16 container; the +0x08/+0x09/+0x0C handlers
 *   (sub_08014804/44/84/C4, sub_08014AA8/14B5C) need the finer bitfield split of the
 *   bytes duel.h declares as plain unk8/unk9/flagsC. The rest of the layout is
 *   identical, so [player & 1] still strides 0xD64.
 * - struct DuelFlags08013CDC: canonical struct DuelState folds +0x1ACC..+0x1ACE into
 *   u32 bitfields (unk1ACC_0 / queueCount / unk1ACC_19), so bit 5/6 of byte +0x1ACD
 *   written by sub_08014A24/sub_08014A64 have no canonical field name.
 */
struct DuelPlayer08013CDC {
    u8 unk0[6];
    u16 numLinks:8;     /* 0x06: number of entries in links[] */
    u16 unk6_8:3;
    u16 flag6_11:1;     /* 0x07 bit 3 */
    u16 flag6_12:1;     /* 0x07 bit 4 */
    u16 flag6_13:1;     /* 0x07 bit 5 */
    u16 turns6_14:2;    /* 0x07 bits 6-7: counts down each turn */
    u16 unk8_0:3;
    u16 flag8_3:1;      /* 0x08 bit 3: cleared every turn */
    u16 unk8_4:2;
    u16 flag8_6:1;      /* 0x08 bit 6 */
    u16 unk8_7:2;
    u16 flag8_9:1;      /* 0x09 bit 1 */
    u16 flag8_10:1;     /* 0x09 bit 2 */
    u16 flag8_11:1;     /* 0x09 bit 3 */
    u16 unk8_12:4;
    u8 unkA;
    u8 turnsB_0:3;      /* 0x0B bits 0-2: counts down each turn */
    u8 unkB_3:5;
    u16 unkC_0:1;
    u16 counterC:3;     /* 0x0C bits 1-3: 0..7 counter */
    u16 flagC_4:1;      /* 0x0C bit 4 */
    u16 unkC_5:11;
    u8 fillerE[0x28 - 0xE];
    struct DuelZone zones[11];  /* 0x028 */
    u8 filler684[0xCC4 - 0x684];
    u16 links[80];      /* 0xCC4: low byte kind, high byte value */
};
extern struct DuelPlayer08013CDC gUnk_020192E4_lp[2] asm("gUnk_020192E4");

struct DuelFlags08013CDC {
    u8 filler[0x1ACD];
    u8 unk1ACD_0:5;
    u8 flag1ACD_5:1;    /* 0x1ACD bit 5 */
    u8 flag1ACD_6:1;    /* 0x1ACD bit 6 */
    u8 unk1ACD_7:1;
};
extern struct DuelFlags08013CDC gUnk_020192E0_flags asm("gUnk_020192E0");

extern struct DuelCmd gUnk_020185C0;

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;          /* +0x000 bit 0: (hypothesis) fast-forward animations */
    u8 unk0_1:1;
    u8 unk0_2:1;        /* +0x000 bit 2 */
    u8 unk0_3:5;
};

extern struct DuelScreen gUnk_0201CFB0;

extern const u16 gUnk_08198D8C[];
extern const s32 gUnk_08081768[];   /* zoom curve, 0x18 entries (0x100 = 1.0) */

/* gMain (0x03000040) layout comes from main.h. */
#define gMain gUnk_03000040
#define FAST_FORWARD() ((gMain.heldKeys & 2) || gUnk_0201CFB0.fast)

u32 sub_08060B4C(void);
void sub_080240A8(u32 player, u32 a);
u32 sub_080623AC(u32 player, u32 a, u32 b);
u32 sub_080623EC(u32 player, u32 a, u32 b);
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, u32 affine);
void sub_08077AEC(u16 se);  /* PlaySE */
void sub_0800688C(u16 cardId, u16 timer, u16 c);  /* opens the Card Detail view (hypothesis) */
u16 sub_08006D08(void);
void sub_080619E8(void);
void sub_08061A1C(u16);
void sub_08061D24(u16);
void sub_08061E54(u16);
void sub_0805ED9C(void);
void sub_0805F00C(u16);
void sub_08060578(void);
void sub_080762D0(u32 yx, u32 shapeSize, u32 attr2);
void sub_080763D0(u32 yx, u32 shapeSize, u32 attr2);

extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */

/* Pointer-arithmetic form: `tbl[i]` loads the table address before the index instead. */
#define CARD_TYPE(id) ((*(gUnk_08621DE0 + ((id) & 0x7FF)) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (*(gUnk_08622AB4 + ((id) & 0x7FF)))
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (p) * 0xD64 + (s) * 0x94))
#define ZONE_SP(p, s) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (s) * 0x94 + (p) * 0xD64))

void sub_08017ADC(u32 player, u16 id, u16 pos, u16 a);

/* Acting player of the current command (bit 15 of the command word). */
#define CMD_PLAYER() ((gUnk_020185C0.cmd & 0x8000) != 0)
#define CUR_PLAYER() (gUnk_020192E4_lp[CMD_PLAYER()])

struct Zone08013CDC {
    u32 id:12;
    u32 unk0_12:20;
    u16 serial;
    u8 flag6_0:1;
    u8 flag6_1:1;
    u8 counter6:4;
    u8 unk6_6:2;
    u8 flag7_0:2;
    u8 flag7_2:1;
    u8 flag7_3:2;
    u8 flag7_5:1;
    u8 turns7_6:2;
    u8 unk8[0x91 - 8];
    u8 flag91_0:2;
    u8 flag91_2:1;
    u8 flag91_3:5;
    u8 unk92[2];
};
struct ZonesPlayer08013CDC {
    struct Zone08013CDC zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

void sub_08013CDC(void)
{
    u32 player = gUnk_020185C0.cmd >> 15;
    int i;
    int j;
    u32 sz;
    u16 id, stId;
    struct Zone08013CDC *mon, *st;

    if (gUnk_020192E4_lp[player & 1].turnsB_0)
        gUnk_020192E4_lp[player & 1].turnsB_0--;
    gUnk_020192E4_lp[(1 - player) & 1].flag8_3 = 0;
    gUnk_020192E4_lp[player & 1].flag8_3 = 0;

    for (i = 0; i <= 1; i++) {
        if (gUnk_020192E4_lp[i & 1].turns6_14)
            gUnk_020192E4_lp[i & 1].turns6_14--;
    }
    if (!gUnk_020192E4_lp[0].turns6_14 || !gUnk_020192E4_lp[1].turns6_14) {
        gUnk_020192E4_lp[0].turns6_14 = 0;
        gUnk_020192E4_lp[1].turns6_14 = 0;
    }

    /* A stride variable set before the loop: its pseudo is spilled and rematerialised each
     * iteration (ldr r4, =0xD64), and player * sz stays in the loop as in the ROM. */
    sz = 0xD64;
    for (j = 0; j <= 4; j++) {
        struct ZonesPlayer08013CDC *base = (struct ZonesPlayer08013CDC *)gUnk_0201930C;
        struct ZonesPlayer08013CDC *zp = (struct ZonesPlayer08013CDC *)((u8 *)base + player * sz);
        mon = &zp->zones[j];
        st = &zp->zones[j + 5];
        /* Byte offsets (zone first) give the ROM's j*0x94 + player*0xD64 sum, shared by both reads. */
        id = ((struct DuelCard *)((u8 *)gUnk_0201930C + j * 0x94 + player * sz))->id;
        stId = ((struct DuelCard *)((u8 *)gUnk_0201930C + 0x2E4 + j * 0x94 + player * sz))->id;

        if (id) {
            mon->flag7_2 = 0;
            if (mon->flag6_1) {
                if (mon->counter6 < 15)
                    mon->counter6++;
                switch (gUnk_08622AB4[id & 0x7FF]) {
                case 0x052:
                    if (!mon->flag7_5)
                        mon->counter6++;
                    if (mon->counter6 > 6)
                        mon->counter6 = 6;
                    break;
                case 0x267:
                    mon->counter6++;
                    if (mon->counter6 > 4)
                        mon->counter6 = 4;
                    break;
                case 0x00F:
                case 0x1AC:
                case 0x243:
                case 0x2DA:
                case 0x2E6:
                case 0x458:
                case 0x536:
                case 0x5E9:
                    mon->flag7_5 = 1;
                    sub_08017ADC(player, id, ((u8)j << 8) | player, 11);
                    break;
                case 0x540:
                    if (!mon->flag7_5) {
                        mon->flag7_2 = 1;
                        mon->flag7_5 = 1;
                    }
                    break;
                }
            }
        }
        if (stId) {
            if (st->flag6_1) {
                if (gUnk_08622AB4[stId & 0x7FF] == 0x47 && st->counter6 < 12)
                    st->counter6++;
            } else if ((*(const u32 *)(0x08621DE0 + (stId & 0x7FF) * 4) & 0x1F00000) >> 20 > 20) {
                st->flag91_2 = 1;
            }
        }
    }

    /* FAKEMATCH: the ROM multiplies player by 0xD64 twice around the link loop (once for the
     * count test, once more in the preheader for the link pointer). The count test reads
     * through an asm-opaque copy of player so cse2 cannot share the body's product; the
     * statement-expression stride loads 0xD64 before that copy, as in the ROM. */
    {
        struct DuelState *b;
        u32 p2;

        i = 0;
        b = &gUnk_020192E0;
        if (i < ((u8 *)b + ({ u32 c = 0xD64; c; }) * ({ p2 = player; asm("" : "+r"(p2)); p2; }))[0xA]) {
            for (; i < b->players[p2].countB84; i++) {
                u16 v = gUnk_020192E0.players[player].arrCC4[i];
                if ((u8)gUnk_020192E0.players[player].arrCC4[i] == 2 && (u8)(v >> 8) <= 4)
                    gUnk_020192E0.players[player].arrCC4[i] = ((u8)((v >> 8) + 1) << 8) | 2;
            }
        }
    }
    gUnk_020185C0.running = 0;
}
INCLUDE_ASM("asm/nonmatching/code_08013CDC", sub_0801401C); /* 0x0801401C size 0x6F4 */
void sub_08014710(void)
{
    u32 player = gUnk_020185C0.cmd >> 15;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 0);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        sub_08077AEC(15);
        break;
    case 1:
        if ((s16)gUnk_020185C0.timer < 32) {
            u32 x = sub_080623AC(player, 0, 2);
            u32 yx = (sub_080623EC(player, 0, 2) << 16) | x;
            sub_08076714(yx, 0x80, gUnk_08198D8C[gUnk_020185C0.timer] | 0x400, player ? 0x1000040 : 0x1000000);
            gUnk_020185C0.timer++;
            break;
        }
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08014804(void)
{
    CUR_PLAYER().flag8_10 = 1;
    gUnk_020185C0.running = 0;
}

void sub_08014844(void)
{
    CUR_PLAYER().flag8_9 = 1;
    gUnk_020185C0.running = 0;
}

void sub_08014884(void)
{
    CUR_PLAYER().flag8_11 = 1;
    gUnk_020185C0.running = 0;
}

void sub_080148C4(void)
{
    CUR_PLAYER().flag8_6 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014914(void)
{
    gUnk_020192E4[CMD_PLAYER()].flag7_5 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014964(void)
{
    CUR_PLAYER().flag6_11 = gUnk_020185C0.arg2;
    CUR_PLAYER().flag6_12 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

void sub_080149D8(void)
{
    gUnk_020192E4[CMD_PLAYER()].turns7_6 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014A24(void)
{
    gUnk_020192E0_flags.flag1ACD_5 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014A64(void)
{
    gUnk_020192E0_flags.flag1ACD_6 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014AA8(void)
{
    if (gUnk_020185C0.arg2)
        CUR_PLAYER().counterC++;
    else if (CUR_PLAYER().counterC)
        CUR_PLAYER().counterC--;
    gUnk_020185C0.running = 0;
}

void sub_08014B5C(void)
{
    CUR_PLAYER().flagC_4 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

void sub_08014BAC(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        if (sub_08060B4C()) {
            gUnk_0201CFB0.unk0_2 = 0;
            sub_0800688C(gUnk_020185C0.arg2, 300, 0);
            gUnk_020185C0.step++;
        }
        break;
    case 1:
        if (sub_08006D08())
            gUnk_020185C0.step++;
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Big multi-step effect over a 4x5 grid of 32x32 sprites (tiles (i*2+1)*32 + j*4):
 * steps 2-4 call sub_08061A1C / sub_08061D24 / sub_08061E54 with arg2, step 5 zooms
 * the grid in (gUnk_08081768) with an alpha fade, step 6 flashes it (BLDY), step 7 fades
 * it out; then sub_08060578.
 */
void sub_08014C30(void)
{
    int i, j;
    int x, y, dy, scale;
    int ta, tb, tc, td, te;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 5:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = gUnk_020185C0.timer) < 0x18) {
                    x -= 0x68;
                    dy = y - 0x40;
                    scale = (gUnk_08081768[0x17 - ta] - 0x100) / 3 + 0x80;
                    x *= scale;
                    dy *= scale;
                    x /= 128;
                    dy /= 128;
                    x += 0x68;
                    y = dy + 0x40;
                }
                sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gUnk_020185C0.timer;
        if (tb < 16) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = tb | ((u8)(16 - tb) << 8);
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x17)
            gUnk_020185C0.timer += 7;
        if (gUnk_020185C0.timer >= 0x20) {
            sub_0805ED9C();
            sub_0805F00C(gUnk_020185C0.arg2);
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
            sub_08077AEC(0x2C);
        }
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++)
                sub_080762D0((j * 32 + 0x44) | ((i * 32 + 2) << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
        }
        tc = gUnk_020185C0.timer;
        if (tc < 0x20) {
            if (tc < 16) {
                REG_BLDY = tc;
                REG_BLDCNT = 0x1090;
            } else {
                REG_BLDY = 0x1F - tc;
                REG_BLDCNT = 0x1090;
            }
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x17)
            gUnk_020185C0.timer += 7;
        if (gUnk_020185C0.timer >= 0x20) {
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
        }
        break;
    case 7:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++)
                sub_080763D0((j * 32 + 0x44) | ((i * 32 + 2) << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
        }
        td = gUnk_020185C0.timer;
        if (td > 0x30) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = (u8)(0x40 - td) | ((u8)(td - 0x30) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        te = gUnk_020185C0.timer;
        if (te < 0x40) {
            if (FAST_FORWARD() && te <= 0x37)
                gUnk_020185C0.timer = te + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    case 8:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}
