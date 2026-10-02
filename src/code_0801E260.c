#include "global.h"
#include "gba.h"
#include "main.h"       /* struct Main */
#include "duel_ui.h"    /* struct DuelCmd/DuelScreen (pulls in duel.h) */

/* gMain (0x03000040): canonical layout in main.h. */
#define gMain gUnk_03000040

typedef u16 (*StepFunc)(void);

/* Message box request block at 0x02017A30. */
struct DuelMsg {
    StepFunc func;                  /* 0x0: handler from gUnk_08198EF8[id] */
    u16 id:15;                      /* 0x4 bits 0-14 */
    u16 flag:1;                     /* 0x4 bit 15 */
    u16 arg;                        /* 0x6 */
    u8 filler8[2];
    u8 stepIndex;                   /* 0xA: index into gUnk_08198F14 */
    u8 state;                       /* 0xB */
    u8 unkC;                        /* 0xC */
    u8 unkD;                        /* 0xD */
};
extern struct DuelMsg gUnk_02017A30;
#define gMsg gUnk_02017A30

/* Duel command queue at 0x020185C0: canonical layout in duel_ui.h. */

/* Local view: canonical struct DuelCard puts the owner bit at +0x1 bit 4 and
   flag20 at bit 20; sub_0801E260 reads the zone word's flag at bit 18 instead,
   so keep a unit-specific view. */
struct ZoneWord {
    u32 cardId:12;
    u32 unk0_12:6;
    u32 flag0_18:1;                 /* bit 18 */
    u32 unk0_19:13;
};

/* Card command menu at 0x020192E0+0x1B2C (see code_0801CE68). Canonical duel.h
   stops at DuelState.phaseStep +0x1B20, so this is a unit-specific view used only
   by sub_0801E260. */
struct SelMask {
    u16 flag0:1;
    u16 active:1;
    u16 cursor:4;
    u16 rows:4;
    u32 mask:16;
    u32 state:8;
    u32 subState:8;                 /* bits 34-41: step counter of the chosen command */
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;                   /* bit 57 */
    u32 zone:7;                     /* bits 58-64 */
    u32 column:8;                   /* bits 65-72 */
    u32 unk73:23;
};

/* Same bits as struct SelMask with a u16 column. sub_0801E260's case 7 reads the
   column through it so the atkSlot bit-field store gets a HImode value and regmove
   ties the AND to the constant (`movs r1, #7; ands r1, r0`); FAKEMATCH: the u32
   column gives `ands r1, r0` with the column as destination. */
struct SelMaskCol {
    u16 flag0:1;
    u16 active:1;
    u16 cursor:4;
    u16 rows:4;
    u32 mask:16;
    u32 state:8;
    u32 subState:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;
    u32 zone:7;
    u16 column:8;                   /* bits 65-72 */
    u32 unk73:23;
};

/* Unit-specific view: canonical struct DuelPlayer names +0x08/+0x09 unk8/unk9,
   but this unit's draft reads flags at +0x08 bit 4 and +0x09 bit 5. */
struct DuelPlayerView {
    u8 unk0[8];
    u8 unk8_0:4;
    u8 flag8_4:1;                   /* 0x08 bit 4 */
    u8 unk8_5:3;
    u8 unk9_0:5;
    u8 flag9_5:1;                   /* 0x09 bit 5 */
    u8 unk9_6:2;
    u8 unkA[0x28 - 0xA];
    struct DuelZone zones[11];      /* 0x28 */
    u8 filler684[0xD64 - 0x684];
};

/* Unit-specific view: canonical struct DuelState has no +0x1B16 bitfield, no
   selCard +0x1B28 / sel +0x1B2C, names +0x1B12 bits 2..4 phase1B12 (here
   unk1B12_2) and ends at +0x1B20 phaseStep (here unk1B20), with no +0x1B21. */
struct DuelStateView {
    u32 unk0;
    struct DuelPlayerView players[2];   /* 0x004 */
    u8 filler1ACC[0x1B12 - 0x1ACC];
    u8 flag1B12_0:1;                /* 0x1B12 bit 0 */
    u8 unk1B12_1:1;
    u32 unk1B12_2:3;                /* 0x1B12 bits 2-4 (u32: see sub_0801E260) */
    u8 unk1B12_5:1;
    u8 result:2;                    /* 0x1B12 bits 6-7 */
    u8 filler1B13[0x1B16 - 0x1B13];
    u16 unk1B16_0:1;
    u16 unk1B16_1:8;                /* 0x1B16 bits 1-8 */
    u16 unk1B16_9:7;
    u8 filler1B18[0x1B20 - 0x1B18];
    u8 unk1B20;                     /* 0x1B20 (canonical phaseStep) */
    u8 unk1B21;                     /* 0x1B21 (not in canonical struct) */
    u8 filler1B22[0x1B28 - 0x1B22];
    u16 selCard;                    /* 0x1B28 */
    u16 unk1B2A;
    union {
        struct SelMask x;
        struct SelMaskCol col;
    } sel;                          /* 0x1B2C */
};
extern struct DuelStateView gUnk_020192E0View asm("gUnk_020192E0");
#define SEL gUnk_020192E0View.sel.x

struct Battle {
    u16 unk0_0:6;
    u16 atkSlot:3;                  /* bits 6-8 */
    u16 unk0_9:7;
};
extern struct Battle gUnk_02018450;

struct Unk0201AE60 {
    u8 filler0[0x14];
    u16 unk14;
};
extern struct Unk0201AE60 gUnk_0201AE60;

extern const u32 gUnk_08621DE0[];   /* card stats */
extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */
extern const u16 gUnk_0862467A;
extern const u8 gUnk_08081CA4[];
#define CARD_TYPE(id) ((*(gUnk_08621DE0 + ((id) & 0x7FF)) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (*(gUnk_08622AB4 + ((id) & 0x7FF)))

void sub_080493D0(u32 cmd);
void sub_08048FE0(void);
void sub_080471E8(u32 a, u32 b);
void sub_08049048(u32 a, u32 b, u32 c);
void sub_08049450(void);
void sub_080193D4(int player, int idx, int a, int b);
void sub_0801FBCC(u32 card, u32 b);
void sub_080197E0(int player, u16 id);
void sub_08019860(int player, int lp);
int sub_08017FF4(int player, int column);
u16 sub_08007FEC(int player, u16 cardNo, void *out);
void sub_08056094(int player, void *card, u32 a, u32 b);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, u32 b, u32 c);
void sub_0801EC58(u16 cmd, u16 arg2, int arg4Word, int arg6Word);

/* Duel screen state (0x0201CFB0): canonical layout in duel_ui.h. */

struct Unk02015EE8 {
    u8 phase;
    u8 link:1;                      /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct Unk02015EE8 gUnk_02015EE8;

/* Step runner state at 0x02015EF0. */
struct Unk02015EF0 {
    u8 index;
    u8 state;
};
extern struct Unk02015EF0 gUnk_02015EF0;

struct OpponentBgm {
    u16 opponent;
    u16 bgm;
};

extern StepFunc gUnk_08198EDC[];
extern StepFunc gUnk_08198EF8[];    /* message handlers by id */
extern StepFunc gUnk_08198F14[];
extern const struct OpponentBgm gUnk_08198F20[0x18];

void sub_08077BA0(void);
void sub_080757AC(void);            /* ResetBgScroll */
u32 sub_08060B4C(void);
void sub_080609C4(void);
u16 sub_08075AE4(u16 speed);        /* FadeFromBlack */
void sub_08077BCC(void);
void sub_08077B24(u16 bgm);

/* Execute the command chosen in the card command menu (SEL.cursor = 1..12). */
/* Zones and players are reached through casts so the field offsets (+6, +8/+9)
   stay in the ldrb/strb as in the ROM instead of folding into the base constant. */
struct ZoneFlags1E260 {
    u32 card;
    u16 serial;
    u8 flag6_0:1;
    u8 flag6_1:1;                   /* +0x06 bit 1 */
    u8 unk6_2:6;
};

struct PlayerFlags1E260 {
    u8 unk0[8];
    u8 unk8_0:4;
    u8 flag8_4:1;                   /* 0x08 bit 4 */
    u8 unk8_5:3;
    u8 unk9_0:5;
    u8 flag9_5:1;                   /* 0x09 bit 5 */
    u8 unk9_6:2;
};

#define ZONE_FLAGS(p, i) ((struct ZoneFlags1E260 *)&gUnk_020192E0View.players[p].zones[i])
#define ZONE_WORD(p, i) ((struct ZoneWord *)&gUnk_020192E0View.players[p].zones[i])
#define PLAYER_FLAGS(pl, p) ((struct PlayerFlags1E260 *)&(pl)[p])

/* Execute the command chosen in the card command menu (SEL.cursor = 1..12).
   FAKEMATCH notes: `ev = (zone << 16 | kind)` inside the sub_0801FBCC packings stops
   fold from floating the kind constant out of the OR chain; per-site `pl` locals keep
   the constant-1 pseudo from inheriting an r4 preference; `unk1B12_2 > 1u` on the u32
   view (struct DuelStateView) drops two pre-combine extension insns so the reloaded base wins r5 over
   &column; sub_08007FEC is called as int-returning (the ROM tests r0 unextended). */
void sub_0801E260(void)
{
    u8 buf[4];
    s16 zone;
    u32 ev;
    int idx;
    int p;

    switch (SEL.cursor) {
    case 1:
    case 2:
        if (SEL.zone != 0)
            break;
        sub_080493D0(SEL.cursor);
        return;
    case 3:
        if (SEL.zone != 0)
            break;
        sub_08048FE0();
        return;
    case 4:
        if (SEL.zone != 11)
            break;
        if (((((const u32 *)0x08621DE0)[gUnk_020192E0View.selCard & 0x7FF] & 0x1F00000) >> 20) <= 20)
            sub_080471E8(0, 0);
        else
            sub_08049048(0, 0, 0);
        return;
    case 5:
        sub_080471E8(1, 0);
        return;
    case 11:
        sub_080471E8(1, 1);
        return;
    case 12:
        sub_080471E8(0, 1);
        return;
    case 10:
        sub_08049450();
        return;
    case 6:
        zone = SEL.zone;
        switch (zone) {
        case 11:
            if (CARD_TYPE(gUnk_020192E0View.selCard) > 20) {
                sub_08049048(1, 0, 0);
                return;
            }
            switch (((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF]) {
            case 0x47:
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag8_4 = 1;
                }
                sub_08049048(1, 0, 0);
                return;
            case 0x1A8:
                sub_080193D4(SEL.player, SEL.column, 0, 1);
                sub_0801FBCC(((SEL.player & 1) << 31) | (ev = 0x600000 | gUnk_020192E0View.selCard), 0);
                break;
            }
            break;
        case 10:
            if (!ZONE_FLAGS(SEL.player & 1, 10)->flag6_1)
                sub_0801EC58(SEL.player ? 0x807F : 0x7F, 10, 0, 0);
            sub_0801FBCC(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x200000) | gUnk_020192E0View.selCard, 0);
            if (gUnk_020192E0View.unk1B12_2 > 1u) {
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag9_5 = 1;
                }
            }
            break;
        case 5:
            p = SEL.player & 1;
            idx = SEL.column;
            idx += 5;
            if (!ZONE_FLAGS(p, idx)->flag6_1)
                sub_0801EC58(SEL.player ? 0x807F : 0x7F, SEL.column + SEL.zone, 0, 0);
            if (ZONE_WORD(SEL.player & 1, SEL.column)->flag0_18) {
                sub_080197E0(SEL.player, gUnk_0862467A);
                sub_08019860(SEL.player, 2000);
            }
            sub_0801FBCC(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x200000) | gUnk_020192E0View.selCard, 0);
            if (gUnk_020192E0View.unk1B12_2 > 1u) {
                {
                    struct DuelPlayerView *pl = gUnk_020192E0View.players;
                    PLAYER_FLAGS(pl, SEL.player & 1)->flag9_5 = 1;
                }
            }
            break;
        case 0:
            switch (((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF]) {
            case 0x51:
            case 0x186:
                switch (SEL.subState) {
                case 0:
                    if (sub_08017FF4(SEL.player, SEL.column)) {
                        SEL.subState++;
                        return;
                    }
                    SEL.active = 0;
                    SEL.subState = 0;
                    return;
                case 1:
                    if (((int (*)(int, u16, void *))sub_08007FEC)(SEL.player, ((const u16 *)0x08622AB4)[gUnk_020192E0View.selCard & 0x7FF] == 0x51 ? 0x2E5 : 0x187, buf)) {
                        sub_08056094(SEL.player, buf, 1, 1);
                        SEL.subState++;
                        return;
                    }
                }
                SEL.active = 0;
                SEL.subState = 0;
                return;
            case 0x1A0:
            case 0x243:
            case 0x2DB:
                sub_0801FBCC(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x4400000) | gUnk_020192E0View.selCard, 0);
                break;
            default:
                sub_0801FBCC(((SEL.player & 1) << 31) | (ev = (((SEL.column + SEL.zone) & 0x1F) << 16) | 0x400000) | gUnk_020192E0View.selCard, 0);
                break;
            }
            break;
        }
        break;
    case 7:
        gUnk_02018450.atkSlot = gUnk_020192E0View.sel.col.column;
        gUnk_020192E0View.unk1B16_1 = 2;
        break;
    case 8:
        gUnk_020192E0View.unk1B20++;
        SEL.active = 0;
        SEL.subState = 0;
        return;
    case 9:
        switch (SEL.subState) {
        case 0:
            sub_080602A4(0x206, 0x412, 0xB, gUnk_08081CA4);
            sub_08060308(1, 0, 0);
            SEL.subState++;
            break;
        case 1:
            if (gUnk_0201AE60.unk14)
                sub_0801EC58(0x40, 1, 0, 0);
            SEL.active = 0;
            SEL.subState = 0;
            break;
        }
        return;
    }
    SEL.active = 0;
    SEL.subState = 0;
}

u16 sub_0801E944(void)
{
    StepFunc step = gUnk_08198EDC[gUnk_02015EF0.index];
    if (step != NULL) {
        if (step()) {
            gUnk_020192E0View.unk1B20 = 0;
            gUnk_020192E0View.unk1B21 = 0;
            gUnk_02015EF0.state = 0;
            gUnk_02015EF0.index++;
        }
        return 0;
    }
    return 1;
}

/* Start message `id` (handler gUnk_08198EF8[id]). */
void sub_0801E998(u16 id, u32 flag)
{
    gMsg.stepIndex = 0;
    gMsg.state = 0;
    gMsg.unkC = 0;
    gMsg.unkD = 0;
    gMsg.flag = flag;
    gMsg.id = id;
    gMsg.func = gUnk_08198EF8[id];
    switch (gMsg.id) {
    case 1:
    case 5:
        sub_08077BA0();
        gUnk_020192E0.flag1B12_0 = 0;
        break;
    }
    gUnk_0201CFB0.flag0_1 = 0;
}

u16 sub_0801EA1C(void)
{
    if (gMsg.state == 0) {
        gMain.vblankCallback = NULL;
        sub_080757AC();
        gUnk_0201CFB0.busy = 0;
        gUnk_0201CFB0.flag0_1 = 0;
        gUnk_0201CFB0.flag0_2 = 0;
        gMsg.state++;
    }
    return sub_08060B4C();
}

u16 sub_0801EA7C(void)
{
    StepFunc func = gMsg.func;
    if (func != NULL)
        return func();
    return 0;}

u16 sub_0801EA9C(void)
{
    struct DuelMsg *msg = &gMsg;

    switch (msg->state) {
    case 0:
        REG_DISPCNT = 0;
        break;
    case 1:
        sub_080609C4();
        break;
    default:
        return sub_08075AE4(4);
    }
    msg->state++;
    return 0;
}

u16 sub_0801EAD8(void)
{
    StepFunc step = gUnk_08198F14[gMsg.stepIndex];
    if (step != NULL) {
        if (step()) {
            gMsg.stepIndex++;
            gMsg.state = 0;
            gMsg.unkC = 0;
            gMsg.unkD = 0;
        }
        return 0;
    }
    gMsg.stepIndex = 0;
    return 1;
}

/* HBlank: wavy BG0/BG1/BG3 HOFS from the per-line table. */
void sub_0801EB1C(void)
{
    u16 hofs = gUnk_020185C0.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];
    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* HBlank: same for all four BGs. */
void sub_0801EB60(void)
{
    u16 hofs = gUnk_020185C0.hofsTable[(REG_VCOUNT + gMain.frameCounter) & 0xF];
    REG_BG0HOFS = hofs;
    REG_BG1HOFS = hofs;
    REG_BG2HOFS = hofs;
    REG_BG3HOFS = hofs;
}

/* Start the duel BGM for the current opponent (or event / link BGM). */
void sub_0801EBA8(void)
{
    u16 bgm = 0xFFFF;
    u32 i;

    i = 0;
    do {
        if (gUnk_08198F20[i].opponent == gMain.opponent) {
            bgm = gUnk_08198F20[i].bgm;
            break;
        }
    } while (++i <= 0x17);
    if (gMain.events & 0xF000000)
        bgm = 0x13;
    if (gUnk_02015EE8.link)
        bgm = 5;
    if (!gUnk_020192E0.flag1B12_0)
        bgm = 0xFFFF;
    if (bgm == 0xFFFF)
        sub_08077BCC();
    else
        sub_08077B24(bgm);
}

/* Append a command to the duel command queue (max 256).
 * The original r2/r3 entry shifts explicitly decode both final words as u16. */
void sub_0801EC58(u16 cmd, u16 arg2, int arg4Word, int arg6Word)
{
    u16 arg4 = arg4Word;
    u16 arg6 = arg6Word;
    if (gUnk_020185C0.queueCount < 0x100) {
        gUnk_020185C0.queue[gUnk_020185C0.queueCount].cmd = cmd;
        gUnk_020185C0.queue[gUnk_020185C0.queueCount].arg2 = arg2;
        gUnk_020185C0.queue[gUnk_020185C0.queueCount].arg4 = arg4;
        gUnk_020185C0.queue[gUnk_020185C0.queueCount].arg6 = arg6;
        gUnk_020185C0.queueCount++;
    }
}

void sub_0800CE28(void);
void sub_0800D234(void);
void sub_0800D398(void);
void sub_0800D43C(void);
void sub_0800D468(void);
void sub_0800D498(void);
void sub_0800D4C8(void);
void sub_0800D524(void);
void sub_0800D594(void);
void sub_0800D634(void);
void sub_0800D6D4(void);
void sub_0800D6FC(void);
void sub_0800D784(void);
void sub_0800D7D4(void);
void sub_0800D824(void);
void sub_0800D864(void);
void sub_0800D8A4(void);
void sub_0800D990(void);
void sub_0800DA84(void);
void sub_0800DD04(void);
void sub_0800DF94(void);
void sub_0800E1E0(void);
void sub_0800E438(void);
void sub_0800E630(void);
void sub_0800E874(void);
void sub_0800EAA8(void);
void sub_0800EADC(void);
void sub_0800EB10(void);
void sub_0800EB44(void);
void sub_0800EBA0(void);
void sub_0800EBF4(void);
void sub_0800EC54(void);
void sub_0800ECB0(void);
void sub_0800ED1C(void);
void sub_0800ED58(void);
void sub_0800EDCC(void);
void sub_0800EE50(void);
void sub_0800EF38(void);
void sub_0800F0F8(void);
void sub_0800F294(void);
void sub_0800F3D0(void);
void sub_0800F544(void);
void sub_0800F678(void);
void sub_0800F6B0(void);
void sub_0800F7F0(void);
void sub_0800F8F8(void);
void sub_0800FA04(void);
void sub_0800FB10(void);
void sub_0800FB48(void);
void sub_0800FB80(void);
void sub_0800FBC8(void);
void sub_0800FC00(void);
void sub_0800FDD0(void);
void sub_0800FF04(void);
void sub_08010038(void);
void sub_08010124(void);
void sub_08010160(void);
void sub_080102B0(void);
void sub_080103DC(void);
void sub_080104B8(void);
void sub_080104EC(void);
void sub_08010538(void);
void sub_080106BC(void);
void sub_08010708(void);
void sub_08010794(void);
void sub_080108FC(void);
void sub_08010A5C(void);
void sub_08010BDC(void);
void sub_08010C14(void);
void sub_08010D94(void);
void sub_08010F84(void);
void sub_08010FAC(void);
void sub_08010FE4(void);
void sub_08011148(void);
void sub_08011278(void);
void sub_08011498(void);
void sub_08011610(void);
void sub_08011C18(void);
void sub_08011C98(void);
void sub_08011CB4(void);
void sub_08011F38(void);
void sub_08011FA0(void);
void sub_08011FFC(void);
void sub_080120E0(void);
void sub_08012264(void);
void sub_080122B4(void);
void sub_080124E8(void);
void sub_08012670(void);
void sub_080126F0(void);
void sub_0801270C(void);
void sub_08012728(void);
void sub_08012744(void);
void sub_08012760(void);
void sub_0801277C(void);
void sub_08012798(void);
void sub_080127E0(void);
void sub_08012834(void);
void sub_08012888(void);
void sub_080128A4(void);
void sub_080128F8(void);
void sub_0801296C(void);
void sub_08012AE0(void);
void sub_08012B34(void);
void sub_08012C4C(void);
void sub_08012D7C(void);
void sub_08012ED4(void);
void sub_08012FA4(void);
void sub_08013104(void);
void sub_08013154(void);
void sub_08013190(void);
void sub_08013390(void);
void sub_080134EC(void);
void sub_08013BB0(void);
void sub_08013CDC(void);
void sub_0801401C(void);
void sub_08014710(void);
void sub_08014804(void);
void sub_08014844(void);
void sub_08014884(void);
void sub_080148C4(void);
void sub_08014914(void);
void sub_08014964(void);
void sub_080149D8(void);
void sub_08014A24(void);
void sub_08014A64(void);
void sub_08014AA8(void);
void sub_08014B5C(void);
void sub_08014BAC(void);
void sub_08014C30(void);
void sub_080150DC(void);
void sub_080153D4(void);
void sub_08015720(void);
void sub_08015A2C(void);
void sub_08015E40(void);
void sub_080162C4(void);
void sub_08016424(void);
void sub_08016460(void);
void sub_08016488(void);
void sub_080164D0(void);
void sub_08016A18(void);
void sub_08016D24(void);
void sub_08016E74(void);
void sub_08016EB8(void);
void sub_08017024(void);
void sub_08017084(void);
void sub_080170E4(void);
void sub_0801715C(void);
void sub_080171D0(void);
void sub_0801723C(void);
void sub_080172A8(void);
void sub_08013888(u32);
void sub_08016848(u32);

/* Run the current duel command (id = bits 0-11 of gUnk_020185C0.cmd). */
void sub_0801ECA8(void)
{
    gUnk_0201CFB0.busy = 0;
    switch (gUnk_020185C0.cmd & 0xFFF) {
    case 0x01:
        sub_08013CDC();
        break;
    case 0x02:
        sub_0801401C();
        break;
    case 0x03:
        sub_08014710();
        break;
    case 0x04:
        sub_08013190();
        break;
    case 0x05:
        sub_08017024();
        break;
    case 0x06:
        sub_08017084();
        break;
    case 0x07:
        sub_08016A18();
        break;
    case 0x08:
        sub_08016D24();
        break;
    case 0x09:
        sub_08016E74();
        break;
    case 0x10:
        sub_080162C4();
        break;
    case 0x11:
        sub_08016488();
        break;
    case 0x12:
        sub_08016424();
        break;
    case 0x13:
        sub_08016460();
        break;
    case 0x14:
        sub_08016EB8();
        break;
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
        sub_08013BB0();
        break;
    case 0x1C:
        sub_08014A24();
        break;
    case 0x1D:
        sub_08014A64();
        break;
    case 0x30:
        sub_0800D468();
        break;
    case 0x31:
        sub_0800D498();
        break;
    case 0x32:
        sub_0800D398();
        break;
    case 0x33:
        sub_0800CE28();
        break;
    case 0x34:
        sub_0800D234();
        break;
    case 0x35:
        sub_0800D43C();
        break;
    case 0x36:
        sub_0800D4C8();
        break;
    case 0x37:
        sub_0800D524();
        break;
    case 0x38:
        sub_0800D594();
        break;
    case 0x39:
        sub_0800D634();
        break;
    case 0x3A:
        sub_0800D6D4();
        break;
    case 0x3B:
        sub_0800D6FC();
        break;
    case 0x40:
        sub_08013390();
        break;
    case 0x41:
        sub_080134EC();
        break;
    case 0x42:
        sub_08013888(1);
        break;
    case 0x43:
        sub_08013888(0);
        break;
    case 0x44:
        sub_08014804();
        break;
    case 0x45:
        sub_08014844();
        break;
    case 0x46:
        sub_08014884();
        break;
    case 0x47:
        sub_080148C4();
        break;
    case 0x48:
        sub_08014914();
        break;
    case 0x49:
        sub_08014964();
        break;
    case 0x4A:
        sub_080149D8();
        break;
    case 0x4B:
        sub_08014AA8();
        break;
    case 0x4C:
        sub_08014B5C();
        break;
    case 0x50:
        sub_08016848(0);
        break;
    case 0x51:
        sub_08016848(1);
        break;
    case 0x52:
        sub_08016848(2);
        break;
    case 0x53:
        sub_080164D0();
        break;
    case 0x54:
        sub_08016848(4);
        break;
    case 0x55:
        sub_08016848(5);
        break;
    case 0x61:
        sub_0800F0F8();
        break;
    case 0x62:
        sub_0800F294();
        break;
    case 0x63:
        sub_0800F3D0();
        break;
    case 0x60:
        sub_0800EF38();
        break;
    case 0x64:
        sub_0800F544();
        break;
    case 0x65:
        sub_0800F678();
        break;
    case 0x66:
        sub_0800F6B0();
        break;
    case 0x67:
        sub_0800F7F0();
        break;
    case 0x68:
        sub_0800F8F8();
        break;
    case 0x69:
        sub_0800FB80();
        break;
    case 0x6A:
        sub_0800FB10();
        break;
    case 0x6B:
        sub_0800FB48();
        break;
    case 0x70:
        sub_08014BAC();
        break;
    case 0x71:
        sub_08014C30();
        break;
    case 0x72:
        sub_080150DC();
        break;
    case 0x73:
        sub_080153D4();
        break;
    case 0x74:
        sub_08015720();
        break;
    case 0x75:
        sub_08015A2C();
        break;
    case 0x76:
        sub_08015E40();
        break;
    case 0x77:
        sub_0800D784();
        break;
    case 0x78:
        sub_0800D7D4();
        break;
    case 0x7C:
        sub_0800D824();
        break;
    case 0x7D:
        sub_0800D864();
        break;
    case 0x7E:
        sub_0800D8A4();
        break;
    case 0x7F:
        sub_0800D990();
        break;
    case 0x79:
        sub_0800DA84();
        break;
    case 0x7A:
        sub_0800DD04();
        break;
    case 0x7B:
        sub_0800DF94();
        break;
    case 0x80:
        sub_0800E1E0();
        break;
    case 0x81:
        sub_0800E438();
        break;
    case 0x82:
        sub_0800E630();
        break;
    case 0x83:
        sub_0800EAA8();
        break;
    case 0x84:
        sub_0800E874();
        break;
    case 0x85:
        sub_0800EADC();
        break;
    case 0x86:
        sub_0800EB10();
        break;
    case 0x87:
        sub_0800EB44();
        break;
    case 0x88:
        sub_0800ECB0();
        break;
    case 0x89:
        sub_0800EC54();
        break;
    case 0x8A:
        sub_0800EBF4();
        break;
    case 0x8B:
        sub_0800EBA0();
        break;
    case 0x8C:
        sub_0800ED1C();
        break;
    case 0x8D:
        sub_0800ED58();
        break;
    case 0x8E:
        sub_0800EDCC();
        break;
    case 0x8F:
        sub_0800EE50();
        break;
    case 0x90:
        sub_08011FFC();
        break;
    case 0x91:
        sub_080120E0();
        break;
    case 0x92:
        sub_08012264();
        break;
    case 0x93:
        sub_080122B4();
        break;
    case 0x94:
        sub_080124E8();
        break;
    case 0x95:
        sub_08012670();
        break;
    case 0x96:
        sub_08012834();
        break;
    case 0x97:
        sub_080127E0();
        break;
    case 0x98:
        sub_08012798();
        break;
    case 0x99:
        sub_080126F0();
        break;
    case 0x9A:
        sub_0801270C();
        break;
    case 0x9B:
        sub_08012728();
        break;
    case 0x9C:
        sub_08012744();
        break;
    case 0x9D:
        sub_08012760();
        break;
    case 0x9E:
        sub_0801277C();
        break;
    case 0x9F:
        sub_08012888();
        break;
    case 0xA0:
        sub_08013154();
        break;
    case 0xA1:
        sub_080128A4();
        break;
    case 0xA2:
        sub_080128F8();
        break;
    case 0xA3:
        sub_0801296C();
        break;
    case 0xA4:
        sub_08012AE0();
        break;
    case 0xA5:
        sub_08012FA4();
        break;
    case 0xA6:
        sub_08013104();
        break;
    case 0xA7:
        sub_08012B34();
        break;
    case 0xA8:
        sub_08012C4C();
        break;
    case 0xA9:
        sub_08012D7C();
        break;
    case 0xAA:
        sub_08012ED4();
        break;
    case 0xB0:
        sub_08011C18();
        break;
    case 0xB2:
        sub_08011C98();
        break;
    case 0xB1:
        sub_08011CB4();
        break;
    case 0xB3:
        sub_08011F38();
        break;
    case 0xB4:
        sub_08011FA0();
        break;
    case 0xC0:
        sub_08010794();
        break;
    case 0xC1:
        sub_080108FC();
        break;
    case 0xC3:
        sub_08010A5C();
        break;
    case 0xC2:
        sub_08010BDC();
        break;
    case 0xC4:
        sub_08010C14();
        break;
    case 0xC5:
        sub_08010D94();
        break;
    case 0xC7:
        sub_08011278();
        break;
    case 0xCB:
        sub_08010FAC();
        break;
    case 0xCA:
        sub_08010F84();
        break;
    case 0xCC:
        sub_08011498();
        break;
    case 0xCD:
        sub_08011610();
        break;
    case 0xCE:
        sub_08010FE4();
        break;
    case 0xCF:
        sub_08011148();
        break;
    case 0xD0:
        sub_0800FDD0();
        break;
    case 0xD1:
        sub_0800FF04();
        break;
    case 0xD2:
        sub_0800FC00();
        break;
    case 0xD4:
        sub_08010038();
        break;
    case 0xD3:
        sub_08010124();
        break;
    case 0xD5:
        sub_080102B0();
        break;
    case 0xD6:
        sub_08010160();
        break;
    case 0xD7:
        sub_080104B8();
        break;
    case 0xD8:
        sub_080104EC();
        break;
    case 0xD9:
        sub_08010538();
        break;
    case 0xDA:
        sub_080106BC();
        break;
    case 0xDB:
        sub_08010708();
        break;
    case 0xDC:
        sub_0800FBC8();
        break;
    case 0xDD:
        sub_0800FA04();
        break;
    case 0xDE:
        sub_080103DC();
        break;
    case 0xE0:
        sub_080170E4();
        break;
    case 0xE1:
        sub_0801715C();
        break;
    case 0xE2:
        sub_080171D0();
        break;
    case 0xE4:
        sub_080172A8();
        break;
    case 0xE3:
    case 0xE5:
        sub_0801723C();
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
