#include "global.h"
#include "gba.h"

struct CardWord {
    u32 id:12;
    u32 player:1;
    u32 rest:19;
};

struct LinkState {
    u16 message, command, arg1, arg2, arg3;
    u8 padA[0x304 - 0xA];
    u32 flags304:21;
    u32 resultReceived:1;
    u32 flagsRemaining:10;
    u8 pad308[0x484 - 0x308];
    u16 remoteCommand, remoteArg1, remoteArg2, remoteArg3;
};
extern struct LinkState gUnk_02017FB0;

void sub_08022D5C(void)
{
    u16 command = gUnk_02017FB0.command;
    u16 arg1 = gUnk_02017FB0.arg1;
    u16 arg2 = gUnk_02017FB0.arg2;
    u16 arg3 = gUnk_02017FB0.arg3;
    struct CardWord card1, card2;

    if (command & 0x8000)
        command &= 0x7FFF;
    else
        command |= 0x8000;
    switch (command & 0xFFF) {
    case 8:
        arg1 = 1 - arg1;
        break;
    case 0x82: case 0x83: case 0x84:
        arg1 = (u8)(1 - arg1) | ((arg1 >> 8) << 8);
        arg2 = (u8)(1 - arg2) | ((arg2 >> 8) << 8);
        break;
    case 0x85: case 0x86:
        arg2 = (u8)(1 - arg2) | ((arg2 >> 8) << 8);
        switch ((u8)arg3) {
        case 1: case 2: case 5:
            arg1 = (u8)(1 - arg1) | ((arg1 >> 8) << 8);
        }
        break;
    case 0x30:
        arg1 = gUnk_02017FB0.arg2;
        arg2 = gUnk_02017FB0.arg1;
        break;
    case 0x31:
        arg1 = gUnk_02017FB0.arg2;
        arg2 = gUnk_02017FB0.arg1;
        arg3 = (gUnk_02017FB0.arg3 >> 8) | ((u8)gUnk_02017FB0.arg3 << 8);
        break;
    case 0x64: case 0x65: case 0x66: case 0x67: case 0x68:
    case 0x6A: case 0x7C: case 0x7D: case 0xC2: case 0xCB:
    case 0xD2: case 0xD3: case 0xD4: case 0xD7:
    case 0xDB: case 0xDC: case 0xDD: case 0xDE:
        *(u32 *)&card1 = arg1 | (arg2 << 16);
        { struct CardWord *card = &card1; card->player = 1 - card->player; }
        arg1 = *(u32 *)&card1;
        arg2 = *(u32 *)&card1 >> 16;
        break;
    case 0x77: case 0xA8:
        *(u32 *)&card2 = arg2 | (arg3 << 16);
        { struct CardWord *card = &card2; card->player = 1 - card->player; }
        arg2 = *(u32 *)&card2;
        arg3 = *(u32 *)&card2 >> 16;
        break;
    case 0xA2:
        arg1 = (u8)(1 - arg1) | ((arg1 >> 8) << 8);
        break;
    case 4:
        if (gUnk_02017FB0.arg1 <= 1)
            arg1 = 1 - gUnk_02017FB0.arg1;
        gUnk_02017FB0.resultReceived = 1;
        break;
    }
    gUnk_02017FB0.remoteCommand = command;
    gUnk_02017FB0.remoteArg1 = arg1;
    gUnk_02017FB0.remoteArg2 = arg2;
    gUnk_02017FB0.remoteArg3 = arg3;
}
/* Byte and halfword views retain the original access widths for packet fields. */
struct LinkReceive {
    u16 w0, w2, w4, w6, w8, w0xA;
    u8 padC[0x202 - 0xC];
    u16 w0x202;
    u8 pad204[0x304 - 0x204];
    u32 f304_0:1, f304_1:1, f304_2:1, f304_rest:5;
    u32 f305_0:1, f305_1:1, f305_2:1, f305_3:1;
    u32 f305_4:1, f305_5:1, f305_6:1, f305_7:1;
    u32 f306_0:1, f306_1:1, f306_2:1, f306_3:1;
    u32 f306_4:1, f306_5:1, f306_6:1, f306_7:1;
    u32 f307_0:1, f307_1:1, f307_2:1, f307_3:1;
    u32 f307_4:1, f307_5:1, f307_6:1, f307_7:1;
    u32 f308_0:1, f308_1:1, f308_2:1, f308_3:1;
    u32 f308_4:1, f308_5:1, f308_6:1, f308_7:1;
    u32 f308_rest:24;
    u8 pad30C[0x44C - 0x30C];
    u16 w0x44C;
    u8 pad44E[2];
    u32 f450_0:1, f450_1:1, f450_rest:14;
    u16 w0x452;
    u16 w0x454, w0x456, w0x458, w0x45A;
    u8 pad45C[2];
    u32 player45E:1, rest45E:7;
    u8 pad45F[0x472 - 0x45F];
    u32 player472:1, rest472:7;
    u8 pad473[0x48C - 0x473];
    u8 b0x48C, pad48D, b0x48E;
};
#define LINK_RX (*(struct LinkReceive *)&gUnk_02017FB0)
#define LB(off) (LINK_RX.b##off)
#define LW(off) (LINK_RX.w##off)
#define LP(off) ((u8 *)&gUnk_02017FB0 + (off))
#define AB(off) (gUnk_02017A40[(off)])
struct ActionState {
    u8 pad0[0x491];
    u32 rest491:7, appended:1;
    u32 preserved492:1, clear492:7;
    u32 fromPair:1, rest493:7;
    u8 pad494[0x4C2 - 0x494];
    u16 w0x4C2, w0x4C4;
};
#define ACTION_STATE (*(struct ActionState *)gUnk_02017A40)
#define AW(off) (ACTION_STATE.w##off)
struct DuelReceiveFlags {
    u8 pad0[0x1B12];
    u32 unused12:5, linkError:1, result:2;
    u32 unused13:8;
    u32 unused14:1, surrender:1, rest14:14;
    u8 pad16[0x1B50 - 0x1B16];
    u32 messageSent:1, rest50:31;
};
extern struct DuelReceiveFlags gUnk_020192E0;
extern u8 gUnk_02017A40[], gUnk_0201CFB0[];
struct DuelUiReceive { u32 fast:1, rest:31; u8 pad4[4]; };
#define DUEL_UI_RX (*(struct DuelUiReceive *)gUnk_0201CFB0)
extern u8 gUnk_0201CF90[], gUnk_0201AE44[], gUnk_0201840C[];
struct SioState { u8 pad0[0x522]; u16 error; };
extern struct SioState gUnk_030049D0;
struct ReceivePlayer {
    u16 life;
    u8 handCount, deckCount, graveCount, fusionCount, banishCount;
    u8 pad7[0x684 - 7];
    struct CardWord hand[80], deck[80], grave[80], fusion[80], banish[80];
    u8 padCC4[0xD64 - 0xCC4];
};
extern struct ReceivePlayer gUnk_020192E4[2];
extern struct CardWord gUnk_02019968[], gUnk_02019AA8[], gUnk_02019BE8[];
extern struct CardWord gUnk_02019D28[], gUnk_02019E68[], gUnk_02017FB4[];

struct Action {
    u16 card;
    u16 player:1;
    u16 bits2:9;
    u16 kind:6;
    u8 flags4, byte5;
    u16 from, to;
    u8 rest[10];
};
struct PairReceive {
    struct Action first, second;
    u8 pad28[0x31 - 0x28];
    u8 step31, unused32, step33, selector34;
};
struct IndexedReceiveAction {
    u8 pad0[0x30E];
    u32 player:1, rest:7;
};
#define PAIR_RX (*(struct PairReceive *)gUnk_0201840C)
extern struct Action gUnk_02017EFC, gUnk_02017F10;
#define gUnk_02017EFC (*(struct Action *)&gUnk_02017A40[0x4BC])
#define gUnk_02017F10 (*(struct Action *)&gUnk_02017A40[0x4D0])
#define gUnk_0201840C LP(0x45C)
int sub_080722B0(void *dest);
void sub_08077BCC(void);
void sub_080617F4(void);
void sub_08024134(int player, int zone, int index);
void sub_08075294(void *dest, const void *src, u32 size);
void sub_08055AB4(void);
void sub_08042AB0(int player, int kind, u32 arg);
void sub_0801FBF4(int toB, const void *src);
void sub_0801FBCC(u32 arg);
void sub_080226CC(int player, u16 kind, const void *src, int size);
void sub_0801FA90(u16 toB, u32 a, u32 b);
void sub_0801A7B4(void *state, int player);
void sub_080229EC(int player);
void sub_08022A9C(int player);
void sub_08022B4C(int player);
void sub_08022BFC(int player);
void sub_08022CAC(int player);
void sub_08007558(struct CardWord *dest, const struct CardWord *src);
void sub_080611AC(void);
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);

u16 sub_08023228(void)
{
    int received;
    int player, count, i;
    u16 sio = REG_SIOCNT;
    {
        u32 message;
        /* FAKEMATCH: schedule the shared receive-buffer address before the seed. */
        asm("" : : "r"(&gUnk_02017FB0));
        message = 0xF000;
        /* FAKEMATCH: rematerialize the switch constant after the receive call. */
        asm("" : "+r"(message));
        LW(0) = message;
    }
    received = sub_080722B0(&gUnk_02017FB0);
    if (gUnk_030049D0.error) {
        /* FAKEMATCH: duplicate the error tail to retain the target register allocation. */
        sub_0802297C(0xEE00, 0, 0, 0);
        sub_08077BCC();
        gUnk_020192E0.linkError = 1;
        return 0;
    }
    if (!received)
        return 0;
    switch (LW(0)) {
    case 0xF001: LINK_RX.f304_2 = 1; return 1;
    case 0xEE01: LINK_RX.f304_0 = 1; return 1;
    case 0xEE02: LINK_RX.f304_1 = 1; return 1;
    case 0xEE03: DUEL_UI_RX.fast = 1; return 1;
    case 0xF000: return 1;
    case 0xF002: LINK_RX.f306_2 = 1; return 1;
    case 0xF003:
        LINK_RX.f306_3 = 1;
        gUnk_020192E0.result = (u8)LW(2);
        return 1;
    case 0xF004: LINK_RX.f306_6 = 1; return 1;
    case 0xF005:
        gUnk_020192E0.surrender = 1;
        goto selectSurrender;
    case 0xF006:
        gUnk_020192E0.surrender = 0;
        LINK_RX.f306_6 = 0;
        sub_080617F4();
    selectSurrender:
        sub_08024134(0, 5, 0);
        return 1;
    case 0xF05A:
        sub_08075294(gUnk_0201CF90, LP(2), 0x14);
        sub_08055AB4();
        return 1;
    case 0xF05B: LINK_RX.f306_7 = 1; return 1;
    case 0xF057:
        LINK_RX.f450_0 = 1;
        {
            /* FAKEMATCH: schedule the destination before rematerializing zero. */
            register u16 *dest asm("r1") = &LW(0x452);
            register u32 zero asm("r0") = 0;
            asm("" : "+r"(dest), "+r"(zero));
            *dest = zero;
        }
        LW(0x454) = LW(2);
        LW(0x456) = LW(4);
        LW(0x458) = LW(6);
        return 1;
    case 0xF058:
        LINK_RX.f450_1 = 1;
        LW(0x45A) = LW(2);
        return 1;
    case 0xF051:
        sub_08075294(&gUnk_02017EFC, LP(2), 0x14);
        gUnk_02017EFC.player = 1 - gUnk_02017EFC.player;
        switch (gUnk_02017EFC.kind) {
        case 5: case 6: case 7: case 8: case 16: case 17:
        case 20: case 21: case 25: case 26: case 27: case 29: case 30:
            AW(0x4C2) = (u8)(1 - AW(0x4C2)) | ((AW(0x4C2) >> 8) << 8);
            AW(0x4C4) = (u8)(1 - AW(0x4C4)) | ((AW(0x4C4) >> 8) << 8);
            break;
        case 15: AW(0x4C4) = 1 - AW(0x4C4); break;
        case 13: case 14:
            AW(0x4C4) = (AW(0x4C4) >> 8) | ((u8)AW(0x4C4) << 8);
            break;
        case 19:
            {
                /* FAKEMATCH: retain the original no-op reload/stores and registers. */
                register u16 from asm("r3") = AW(0x4C2);
                register u16 to asm("r1") = AW(0x4C4);
                asm("" : "+r"(from), "+r"(to));
                AW(0x4C2) = from;
                AW(0x4C4) = to;
            }
            break;
        }
        ACTION_STATE.clear492 = 0;
        ACTION_STATE.fromPair = 0;
        LINK_RX.f307_2 = 1;
        return 1;
    case 0xF052: LINK_RX.f307_3 = 1; return 1;
    case 0xF059:
        {
            /* FAKEMATCH: preserve the action-dispatch register allocation. */
            register u16 who asm("r4") = LW(2);
            u16 kind = LW(4), from = LW(6), to = LW(8);
            switch (kind) {
            case 5: case 6: case 7: case 8: case 16: case 17:
            case 25: case 26: case 27: case 29: case 30:
                from = (u8)(1 - from) | ((from >> 8) << 8);
                to = (u8)(1 - to) | ((to >> 8) << 8);
                break;
            case 15: to = 1 - to; break;
            case 13: case 14: to = (to >> 8) | ((u8)to << 8); break;
            case 19: from = LW(8); to = LW(6); break;
            }
            {
                u32 packed = from | (to << 16);
                sub_08042AB0(who, kind, packed);
            }
        }
        return 1;
    case 0xF054:
        sub_08075294(&gUnk_02017F10, LP(2), 0x14);
        gUnk_02017F10.player = 1 - gUnk_02017F10.player;
        gUnk_02017F10.from = (u8)(1 - gUnk_02017F10.from) | ((gUnk_02017F10.from >> 8) << 8);
        gUnk_02017F10.to = (u8)(1 - gUnk_02017F10.to) | ((gUnk_02017F10.to >> 8) << 8);
        sub_08075294(&gUnk_02017EFC, &gUnk_02017F10, 0x14);
        ACTION_STATE.clear492 = 0;
        ACTION_STATE.fromPair = 1;
        LINK_RX.f307_2 = 1;
        return 1;
    case 0xF055: LINK_RX.f307_3 = 1; return 1;
    case 0xF053:
        sub_0801FBF4(0, LP(2));
        ACTION_STATE.appended = 1;
        LINK_RX.f307_3 = 1;
        return 1;
    case 0xF056:
        sub_0801FBF4(1, LP(2));
        ACTION_STATE.appended = 1;
        LINK_RX.f307_3 = 1;
        return 1;
    case 0xF0B1:
        {
            /* FAKEMATCH: retain the source and packed-argument registers. */
            register struct LinkReceive *rx asm("r0") = &LINK_RX;
            register u32 packed asm("r1") = rx->w2 | (rx->w4 << 16);
            sub_0801FBCC(packed);
        }
        return 1;
    case 0xF0A1:
        sub_080226CC(0, LW(2), LP(4), 8);
        gUnk_020192E0.messageSent = 1;
        return 1;
    case 0xF0A2:
        sub_08075294(gUnk_0201AE44, LP(2), 0x10);
        LINK_RX.f307_7 = 1;
        LINK_RX.f306_6 = 0;
        return 1;
    case 0xF091:
        sub_08075294(gUnk_0201840C, LP(2), 0x28);
        PAIR_RX.first.player = 0;
        PAIR_RX.second.player = 1 - PAIR_RX.second.player;
        PAIR_RX.step31 = 0;
        LINK_RX.f308_2 = 1;
        return 1;
    case 0xF092:
        LINK_RX.f308_3 = 1;
        LINK_RX.f306_6 = 0;
        sub_08075294(&gUnk_02017A40[0x280 + AB(0x3D1) * 0x14], LP(2), 0x14);
        return 1;
    case 0xF081:
        sub_08075294(LP(0x45C), LP(2), 0x28);
        LINK_RX.player45E = 0;
        LINK_RX.player472 = 1 - LINK_RX.player472;
        LB(0x48E) = 0;
        LINK_RX.f308_0 = 1;
        return 1;
    case 0xF082:
        LINK_RX.f308_1 = 1;
        LINK_RX.f306_6 = 0;
        sub_08075294(&gUnk_02017A40[0x280 + AB(0x3D1) * 0x14], LP(2), 0x14);
        return 1;
    case 0xF071:
        sub_08075294(gUnk_0201840C, LP(6), 0x28);
        {
            struct LinkReceive *rx = &LINK_RX;
            u8 *dest = &PAIR_RX.selector34;
            u32 low = (u8)rx->w4 & 1;
            *dest = ((u8)rx->w2 << 1) | low;
        }
        PAIR_RX.step33 = 0;
        LINK_RX.f308_4 = 1;
        return 1;
    case 0xF072:
        sub_0801FA90(LW(2), LW(4) | (LW(6) << 16), LW(8) | (LW(0xA) << 16));
        return 1;
    case 0xF073: LINK_RX.f308_5 = 1; return 1;
    case 0xF061: LW(0x44C) = LW(2); return 1;
    case 0xF062:
        sub_08075294(LP(0x30C + LW(2) * 0x14), LP(4), 0x14);
        { struct IndexedReceiveAction *a = (struct IndexedReceiveAction *)LP(LW(2) * 0x14); a->player = 1 - a->player; }
        return 1;
    case 0xF063:
        sub_0801A7B4(LP(0x30C), 0);
        LINK_RX.f308_6 = 1;
        return 1;
    case 0xF064:
        sub_0801A7B4(LP(0x30C), 1);
        LINK_RX.f308_6 = 1;
        return 1;
    case 0xF065: LINK_RX.f308_7 = 1; return 1;
    case 0xF011: sub_080229EC(0); return 1;
    case 0xF012: sub_08022A9C(0); return 1;
    case 0xF013: sub_08022B4C(0); return 1;
    case 0xF014: sub_08022BFC(0); return 1;
    case 0xF015: sub_08022CAC(0); return 1;
    case 0xF021:
        player = LW(2) >> 8; count = (u8)LW(2);
        player = 1 - player;
        i = 0;
        if (i < count) do {
            const struct CardWord *src = &gUnk_02017FB4[i];
            struct CardWord *card = &gUnk_020192E4[player & 1].hand[i];
            sub_08007558(&gUnk_020192E4[player & 1].hand[i], src);
            card->player = 1 - card->player;
        } while (++i < count);
        gUnk_020192E4[player & 1].handCount = count;
        sub_0802297C(0xF031, 0, 0, 0);
        LINK_RX.f305_5 = 1;
        return 1;
    case 0xF022:
        player = LW(2) >> 8; count = (u8)LW(2);
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct CardWord *src = &gUnk_02017FB4[i];
            struct CardWord *card = &gUnk_020192E4[player & 1].deck[i];
            sub_08007558(card, src);
            card->player = 1 - card->player;
        }
        gUnk_020192E4[player & 1].deckCount = count;
        sub_0802297C(0xF032, 0, 0, 0);
        LINK_RX.f305_6 = 1;
        return 1;
    case 0xF023:
        player = LW(2) >> 8; count = (u8)LW(2);
        player = 1 - player;
        i = 0;
        if (i < count) do {
            const struct CardWord *src = &gUnk_02017FB4[i];
            struct CardWord *card = &gUnk_020192E4[player & 1].grave[i];
            sub_08007558(&gUnk_020192E4[player & 1].grave[i], src);
            card->player = 1 - card->player;
        } while (++i < count);
        gUnk_020192E4[player & 1].graveCount = count;
        sub_080611AC();
        sub_0802297C(0xF033, 0, 0, 0);
        LINK_RX.f305_7 = 1;
        return 1;
    case 0xF024:
        player = LW(2) >> 8; count = (u8)LW(2);
        player = 1 - player;
        i = 0;
        if (i < count) do {
            const struct CardWord *src = &gUnk_02017FB4[i];
            struct CardWord *card = &gUnk_020192E4[player & 1].fusion[i];
            sub_08007558(&gUnk_020192E4[player & 1].fusion[i], src);
            card->player = 1 - card->player;
        } while (++i < count);
        gUnk_020192E4[player & 1].fusionCount = count;
        sub_080611AC();
        sub_0802297C(0xF034, 0, 0, 0);
        LINK_RX.f306_0 = 1;
        return 1;
    case 0xF025:
        player = LW(2) >> 8; count = (u8)LW(2);
        player = 1 - player;
        i = 0;
        if (i < count) do {
            const struct CardWord *src = &gUnk_02017FB4[i];
            struct CardWord *card = &gUnk_020192E4[player & 1].banish[i];
            sub_08007558(&gUnk_020192E4[player & 1].banish[i], src);
            card->player = 1 - card->player;
        } while (++i < count);
        /* The ROM does not update the banished-list count here. */
        sub_080611AC();
        sub_0802297C(0xF035, 0, 0, 0);
        LINK_RX.f306_0 = 1;
        return 1;
    case 0xF031: LINK_RX.f305_0 = 1; return 1;
    case 0xF032: LINK_RX.f305_1 = 1; return 1;
    case 0xF033: LINK_RX.f305_2 = 1; return 1;
    case 0xF034: LINK_RX.f305_3 = 1; return 1;
    case 0xF035: LINK_RX.f305_4 = 1; return 1;
    case 0xF041:
        sub_08022D5C();
        LINK_RX.f307_0 = 1;
        LB(0x48C) = 0;
        return 1;
    case 0xF042: LW(0x202) = 0; return 1;
    case 0xF043: LINK_RX.f307_1 = 1; return 1;
    case 0xEE00:
        sub_08077BCC();
        gUnk_020192E0.linkError = 1;
        goto idle;
    default:
    error:
        sub_0802297C(0xEE00, 0, 0, 0);
        sub_08077BCC();
        gUnk_020192E0.linkError = 1;
        goto idle;
    }
    return 1;
idle:
    return 0;
}
