#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel card-effect step handlers, part 2, with the signature int f(struct EffCtx *ctx).
 * Same conventions as code_08032CB0 (see wiki/functions/code-08033dac.md).
 *
 * DuelCard/DuelZone/DuelZonesPlayer/DuelPlayer/DuelScreen and the globals
 * gUnk_020192E0/gUnk_020192E4/gUnk_0201930C/gUnk_0201CFB0 come from duel.h and duel_ui.h.
 * The unit keeps struct EffCtx (not in any shared header) and the local gUnk_02017A40
 * (ActBlk) / gUnk_0201D810 (SelBlk) / gUnk_0201AE60 views, which have no shared header.
 */

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Effect context (argument of every handler). */
struct EffCtx {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 */
    u8 flags4;          /* +0x04: bit 2 = skip */
    u8 unk5;
    u16 pos6;           /* +0x06: player | zone << 8 of a card (CardRef view) */
    u8 filler8[2];
    u8 phaseA : 3;      /* +0x0A bits 0-2: phase / count of targets */
    u8 unkA_3 : 5;
    u8 fillerB;
    u16 pos;            /* +0x0C: player | zone << 8 of a target */
    u16 unkE;           /* +0x0E */
    u16 unk10;          /* +0x10 */
    u8 filler12[2];
};

/* Player bit read as a raw byte (the bitfield form gives lsl/lsr). */
#define PLAYER_RAW(c) (1 & ((u8 *)(c))[2])

struct ActBlk {
    u8 filler[0x3E0];
    u8 step;            /* +0x3E0 */
    u8 sub;             /* +0x3E1 */
};
extern struct ActBlk gUnk_02017A40;
/*
 * Local view: duel.h models gUnk_020192E0 as struct DuelState, whose u32 at +0x1ACC
 * (unk1ACC_0:15) has no byte-sized field at +0x1ACD. sub_08033DAC tests bit 6 of the byte at
 * +0x1ACD (u32 bit 14) and only matches with a u8 load, so keep a unit-local byte view.
 */
struct DuelStateByteView {
    u8 filler[0x1ACD];
    u8 unk1ACD;
};
extern struct DuelStateByteView gDuelStateBytes asm("gUnk_020192E0");

void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
int sub_0802E784(struct EffCtx *ctx, int a, int b);
int sub_0802BE70(struct EffCtx *ctx, u16 pos);
int sub_08008A44(int player);
void sub_08019078(int player, u16 pos, int c);
int sub_08044224(int player, int number, int b);
void sub_0802AF34(int player, int a, u16 number, int b);
void sub_080199E0(int player, int a);
int sub_080074A0(u16 a, u16 b);
void sub_080193D4(int player, int index, int a, int b);
void sub_08019E0C(int player, u16 number, int a);
void sub_08018544(int player, int zone, int a);
void sub_08030028(int player, int zone);
void sub_08046CB0(int a, int player, int zone);
int sub_080090C8(int player, u16 number);
int sub_08008A1C(int player);
int sub_08009C08(int player, u16 id, u32 *out);
void sub_08056094(int player, u32 *card, int a, int b);
struct CardWord {
    u32 id : 12;
    u32 flag12 : 1;
    u32 rest : 19;
};
struct SelBlk {
    u8 filler0[5];
    u8 sel : 2;             /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 base;               /* +6 */
    u8 filler8[4];
    u32 cards[64];          /* +0xC: card words */
};
extern struct SelBlk gUnk_0201D810;
extern struct { u8 filler[0x14]; u16 unk14; } gUnk_0201AE60;
extern const char gUnk_08082CA8[];
extern const char gUnk_08082CE0[];
void sub_080602A4(u16 a, u16 b, int c, const char *text);
void sub_08060308(int a, int b, int c);
int sub_08052F38(u32 mask);
void sub_08019788(int player, u16 id);
void sub_08017AB4(int a, int b, int c, int d);
int sub_0802B1B8(u16 id, int player, int zone);
int sub_0800C894(int player, int zone);
int sub_0800C8A8(int player, int zone);
int sub_0802B28C(int player, int zone);
void sub_080197E0(int player, u16 id);
extern const char *const gUnk_0819D1C4[];
extern const char gUnk_08082D24[];
int sub_08054398(int player, u16 id);
int sub_08007834(u16 id);
int sub_08008A6C(u32 a, u32 b);
void sub_08077AEC(u16 id);
void sub_08055D3C(int player, u16 a, u16 b, u16 c);
#define CARD_LEVEL(id) ((CARD_STATS(id) & 0x1E000000) >> 25)
void sub_0802272C(int player, int a, int b, int c);

int sub_08033DAC(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        sub_0801EC58(PLAYER_RAW(ctx) ? 0x801D : 0x1D, !((gDuelStateBytes.unk1ACD >> 6) & 1), 0, 0);
    return 0;
}
int sub_08033DF4(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gUnk_02017A40.step) {
        case 0x80:
            sub_080199E0(ctx->player, 3);
            return 0x7F;
        case 0x7F:
            sub_0802272C(ctx->player, 2, 0, 0);
            return 0x7E;
        }
    }
    return 0;
}
int sub_08033E44(struct EffCtx *ctx)
{
    int player = ((u8 *)&ctx->pos6)[0];
    int zone = ctx->pos6 >> 8;
    int p = 1 & player;
    struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);
    u16 id = CARD_ID(CARD_WORD(z->card));
    u32 zero = (u8)(ctx->flags4 & 4);

    if (zero == 0) {
        switch (gUnk_02017A40.step) {
        case 0x80:
            if (id == 0)
                return 0;
            gUnk_02017A40.sub = zero;
            gUnk_02017A40.step--;
            /* fall through */
        case 0x7F:
            while (gUnk_02017A40.sub < gUnk_020192E4[1 & player].handCount) {
                if (sub_080074A0(CARD_ID(CARD_WORD(gUnk_020192E4[1 & player].hand[gUnk_02017A40.sub])), id) != 0) {
                    sub_080193D4(player, gUnk_02017A40.sub, 1, 1);
                    return 0x7F;
                }
                gUnk_02017A40.sub++;
            }
            return 0x7E;
        case 0x7E:
            sub_08019E0C(player, CARD_NUMBER(id), 1);
            return 0x7D;
        case 0x7D:
            sub_0801EC58(player ? 0x8060 : 0x60, 0, 0, 0);
            return 0x78;
        }
    }
    return 0;
}
int sub_08033F7C(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        sub_0801EC58(PLAYER_RAW(ctx) == 0 ? 0x8048 : 0x48, 1, 0, 0);
    return 0;
}
int sub_08033FB0(struct EffCtx *ctx, int a)
{
    if (!(ctx->flags4 & 4) && sub_0802E784(ctx, a, 0) != 0) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1 && sub_0802BE70(ctx, ctx->pos) != 0) {
            int v = sub_08008A44(ctx->player);

            ctx->unkE = ctx->player | ((u8)v << 8);
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x80A2 : 0xA2, ctx->pos, 1, 0);
            sub_08019078(ctx->player, ctx->pos, ctx->unkE);
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x8038 : 0x38, ctx->unkE, 1, 0);
        }
    }
    return 0;
}
int sub_08034044(struct EffCtx *ctx)
{
    int best = 99999;
    int idx = -1;

    if (!(ctx->flags4 & 4)) {
        int i;

        for (i = 0; i <= 4; i++) {
            int p = (1 - ctx->player) & 1;
            struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

            /* Keep the player shift and zone product in the ROM scratch registers. */
            __asm__("" : : : "r1");
            if (CARD_WORD(z->card) << 20 != 0) {
                int p2 = (1 - ctx->player) & 1;
                struct DuelZone *z2 = (struct DuelZone *)(i * 0x94 + p2 * 0xD64 + (u32)gUnk_0201930C);

                if (z2->flag6_1) {
                    int a = sub_0800C894(1 - ctx->player, i);

                    if (a < best) {
                        best = a;
                        idx = i;
                    }
                }
            }
        }
        if (idx != -1) {
            if (sub_0802B28C(1 - ctx->player, idx) != 0) {
                sub_08030028(1 - ctx->player, idx);
                sub_08046CB0(ctx->player, 1 - ctx->player, idx);
            } else {
                int p3 = (1 - ctx->player) & 1;
                struct DuelZone *z3 = (struct DuelZone *)(idx * 0x94 + p3 * 0xD64 + (u32)gUnk_0201930C);

                if (CARD_WORD(z3->card) << 20 != 0) {
                    sub_080197E0(ctx->player, CARD_ID(CARD_WORD(((struct DuelZone *)(((1 - ctx->player) & 1) * 0xD64 + idx * 0x94 + (u32)gUnk_0201930C))->card)));
                }
            }
        }
    }
    return 0;
}
static inline int Le500(int value)
{
    int r = 0;

    if (value <= 500)
        r = 1;
    return r;
}

static inline int Gt999(int value)
{
    int r = 0;

    if (value > 999)
        r = 1;
    return r;
}

int sub_0803415C(struct EffCtx *ctx)
{
    int player = ((u8 *)&ctx->pos6)[0];
    int zone = ctx->pos6 >> 8;
    int ok = 1;

    /* FAKEMATCH: preserve the initialized default result separately
     * from the later player mask, matching the ROM's two constants. */
    __asm__("" : "+r"(ok));
    if (!(ctx->flags4 & 4) && (ctx->kind == 5 || ctx->kind == 6)) {
        int p = 1 & player;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

        if (CARD_WORD(z->card) << 20 != 0 && sub_0802B1B8(ctx->id, player, zone) != 0) {
            switch (CARD_NUMBER(ctx->id)) {
            case 0x3EA:
                ok = Gt999(sub_0800C894(player, zone));
                break;
            case 0x2A8:
                ok = Le500(sub_0800C8A8(player, zone));
                break;
            case 0x2A9:
                ok = Le500(sub_0800C894(player, zone));
                break;
            }
            if (ok != 0) {
                sub_08030028(player, zone);
                sub_08046CB0(ctx->player, player, zone);
            }
        }
    }
    /* FAKEMATCH: reserve r5 at the return so ctx/zone use r5/r4. */
    __asm__("" : : : "r5");
    return 0;
}
int sub_08034240(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1) {
            int player = ((u8 *)&ctx->pos)[0];
            int zone = ctx->pos >> 8;
            int p = phase & player;
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);
            u32 id = CARD_ID(CARD_WORD(z->card));

            if (id != 0 && CARD_TYPE(id) == 0x15 && (z->flag6_1))
                sub_08018544(player, zone, 1);
        }
    }
    return 0;
}
int sub_080342C0(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (7 & ((u8 *)ctx)[0xA]) == 3) {
        int i;

        for (i = 0; i < ctx->phaseA; i++) {
            register int off __asm__("r1") = i * 2;
            register u8 *loopbase __asm__("r0") = (u8 *)ctx;
            u16 *e;
            int zone, p;
            struct DuelZone *z;

            /* FAKEMATCH: initialized index/base constraints retain the ROM's
             * per-iteration address setup rather than a walking pointer.
             * The second input keeps +0xC on the base. No code emitted. */
            asm("" : "+r"(off), "+r"(loopbase));
            loopbase += 0xC;
            asm("" : : "r"(loopbase));
            e = (u16 *)(loopbase + off);
            zone = *e >> 8;
            p = 1 & ((u8 *)e)[0];
            z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

            if (!CARD_ID(CARD_WORD(z->card)))
                return 0;
        }
        for (i = 0; i < ctx->phaseA; i++) {
            register int off __asm__("r1") = i * 2;
            register u8 *loopbase __asm__("r0") = (u8 *)ctx;
            u16 *e;
            int player, zone;

            /* FAKEMATCH: same initialized address setup as the check loop. */
            asm("" : "+r"(off), "+r"(loopbase));
            loopbase += 0xC;
            asm("" : : "r"(loopbase));
            e = (u16 *)(loopbase + off);
            player = ((u8 *)e)[0];
            zone = *e >> 8;

            sub_08030028(player, zone);
            sub_08046CB0(ctx->player, player, zone);
        }
    }
    return 0;
}
int sub_0803435C(struct EffCtx *ctx)
{
    u32 out;
    struct CardWord card;

    if (!(ctx->flags4 & 4)) {
        if (CARD_NUMBER(ctx->id) == 0x3F0) {
            if (sub_080090C8(0, 0x402) > 0 || sub_080090C8(1, 0x402) > 0)
                return 0;
        }
        if ((7 & ((u8 *)ctx)[0xA]) == 2 && sub_08008A1C(ctx->player) > 0) {
            *(u32 *)&card = ctx->unkE << 16 | ctx->pos;
            if (sub_08009C08(card.flag12, card.id, &out) != 0) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, ctx->pos, ctx->unkE, 0);
                sub_08056094(ctx->player, (u32 *)&card, 1, 0x30);
            }
        }
    }
    return 0;
}
int sub_08034414(struct EffCtx *ctx)
{
    int n = 0;

    if (!(ctx->flags4 & 4)) {
        switch (CARD_NUMBER(ctx->id)) {
        case 0x21B:
        case 0x5A7:
            n = 1;
            break;
        case 0x3F2:
            n = 2;
            break;
        }
        if (n > 0)
            sub_080199E0(ctx->player, n);
    }
    return 0;
}
int sub_0803447C(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gUnk_02017A40.step) {
        case 0x80:
            switch (CARD_NUMBER(ctx->id)) {
            case 0x3F3:
                gUnk_02017A40.sub = 2;
                break;
            case 0x400:
                gUnk_02017A40.sub = 5;
                break;
            }
            gUnk_02017A40.step--;
            /* fall through */
        case 0x7F:
            if (sub_08044224(ctx->player, CARD_NUMBER(ctx->id), 0) <= 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082CA8);
            return 0x7E;
        case 0x7E:
            sub_0802AF34(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7D;
        case 0x7D: {
            u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.sel + gUnk_0201D810.base];

            sub_0801EC58((int)(gUnk_0201D810.cards[gUnk_0201D810.sel + gUnk_0201D810.base] << 19) < 0 ? 0x80D4 : 0xD4,
                         ((u16 *)card)[0], ((u16 *)card)[1], 0);
            gUnk_02017A40.sub--;
            if (gUnk_02017A40.sub == 0)
                return 0;
            return 0x7C;
        }
        case 0x7C:
            if (sub_08044224(ctx->player, CARD_NUMBER(ctx->id), 0) == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082CE0);
            sub_08060308(1, 0, 0);
            return 0x7B;
        case 0x7B:
            if (gUnk_0201AE60.unk14 == 0)
                return 0;
            return 0x7F;
        }
    }
    return 0;
}
int sub_08034644(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        if (gUnk_020192E4[1 & ctx->player].handCount != 0) {
            if (sub_08052F38(0x10000) != 0) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x8008 : 0x8, (u16)gUnk_0201CFB0.player,
                             *(u8 *)&gUnk_0201CFB0.zone | (*(u8 *)&gUnk_0201CFB0.cursor << 8), 0);
                sub_08019788(ctx->player, CARD_ID(CARD_WORD(gUnk_020192E4[(1 - ctx->player) & 1].hand[gUnk_0201CFB0.cursor])));
            } else
                goto r80;
        }
    }
    return 0;
r80:
    return 0x80;
}
int sub_08034708(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int phase = 7 & ((u8 *)ctx)[0xA];

        if (phase == 1) {
            u16 pos = ctx->pos;
            int zone = pos >> 8;
            int p = phase & ((u8 *)&ctx->pos)[0];
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

            if ((z->flag6_1) && CARD_ID(CARD_WORD(z->card)))
                sub_08017AB4(ctx->player, ctx->id, pos, 3);
        }
    }
    return 0;
}
int sub_08034768(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gUnk_02017A40.step) {
        case 0x80: {
            int i;

            for (i = 0; i < gUnk_020192E4[1 & ctx->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ctx->player].hand[i]));

                if (sub_08054398(ctx->player, id) != 0 && sub_08007834(id) == 0) {
                    sub_080602A4(0x206, 0x712, 0xB, gUnk_08082D24);
                    return 0x7F;
                }
            }
            break;
        }
        case 0x7F:
            if (sub_08052F38(1) != 0) {
                u32 cursor = gUnk_0201CFB0.cursor;
                u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[ctx->player].hand[cursor]));

                if (sub_08054398(ctx->player, id) != 0 && sub_08007834(id) == 0) {
                    int v;

                    ctx->pos = cursor;
                    switch ((int)CARD_TYPE(id)) {
                    case 0x15:
                    case 0x16:
                    case 0x17:
                        v = 0;
                        break;
                    case 0x18:
                        v = 10;
                        break;
                    default:
                        v = CARD_LEVEL(id);
                        break;
                    }
                    switch (v) {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        ctx->unkE = sub_08008A44(ctx->player);
                        ctx->unk10 = 0;
                        return 0x64;
                    case 5:
                    case 6:
                        return 0x6E;
                    default:
                        return 0x78;
                    }
                }
                sub_08077AEC(3);
            }
            return 0x7F;
        case 0x78:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[1]);
            sub_08060308(1, 0, 0);
            return 0x77;
        case 0x77:
            if (gUnk_0201AE60.unk14 == 0)
                return 0x80;
            sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[2]);
            return 0x76;
        case 0x76:
            if (sub_08052F38(0xF0) != 0) {
                if (sub_08008A6C(gUnk_0201CFB0.player, gUnk_0201CFB0.cursor) != 0) {
                    sub_08077AEC(1);
                    sub_0801EC58(8, (u16)gUnk_0201CFB0.player, *(u8 *)&gUnk_0201CFB0.zone | (*(u8 *)&gUnk_0201CFB0.cursor << 8), 0);
                    ctx->unkE = gUnk_0201CFB0.cursor;
                    ctx->unk10 = (u8)((int)gUnk_0201CFB0.cursor | 0x80) << 8;
                    return 0x75;
                }
                sub_08077AEC(3);
            }
            return 0x76;
        case 0x75:
            sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[3]);
            return 0x74;
        case 0x74:
            if (sub_08052F38(0xF0) != 0) {
                if (sub_08008A6C(gUnk_0201CFB0.player, gUnk_0201CFB0.cursor) != 0 && gUnk_0201CFB0.cursor != ctx->unkE) {
                    sub_08077AEC(1);
                    sub_0801EC58(8, (u16)gUnk_0201CFB0.player, *(u8 *)&gUnk_0201CFB0.zone | (*(u8 *)&gUnk_0201CFB0.cursor << 8), 0);
                    ctx->unk10 = (u8)((int)gUnk_0201CFB0.cursor | 0x80) | ctx->unk10;
                    return 0x64;
                }
                sub_08077AEC(3);
            }
            return 0x74;
        case 0x6E:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[0]);
            sub_08060308(1, 0, 0);
            return 0x6D;
        case 0x6D:
            if (gUnk_0201AE60.unk14 == 0)
                return 0x80;
            sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[4]);
            return 0x6C;
        case 0x6C:
            if (sub_08052F38(0xF0) != 0) {
                if (sub_08008A6C(gUnk_0201CFB0.player, gUnk_0201CFB0.cursor) != 0) {
                    sub_08077AEC(1);
                    sub_0801EC58(8, (u16)gUnk_0201CFB0.player, *(u8 *)&gUnk_0201CFB0.zone | (*(u8 *)&gUnk_0201CFB0.cursor << 8), 0);
                    ctx->unkE = gUnk_0201CFB0.cursor;
                    ctx->unk10 = (u8)((int)gUnk_0201CFB0.cursor | 0x80);
                    return 0x64;
                }
                sub_08077AEC(3);
            }
            return 0x6C;
        case 0x64:
            sub_08055D3C(ctx->player, ctx->pos, ctx->unkE, ctx->unk10);
            return 0xA;
        }
    }
    return 0;
}
int sub_08034BA8(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        sub_08044224(ctx->player, CARD_NUMBER(ctx->id), 0);
        sub_0802AF34(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
    }
    return 0;
}
int sub_08007590(u16 number, int flag);

int sub_08034BFC(struct EffCtx *ctx, struct EffCtx *other)
{
    int i;

    if (!(ctx->flags4 & 4) && other != NULL) {
        switch (CARD_NUMBER(ctx->id)) {
        case 0x3FB:
            if (CARD_NUMBER(other->id) == 0x14F && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 0; i <= 4; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        sub_08030028(1 - ctx->player, i);
                        sub_08046CB0(ctx->player, 1 - ctx->player, i);
                    }
                }
            }
            return 0;
        case 0x3FD:
            if (CARD_NUMBER(other->id) == 0x3F0 && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            break;
        case 0x3FE:
            if (CARD_NUMBER(other->id) == 0x150 && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 0; i <= 4; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        sub_08030028(1 - ctx->player, i);
                        sub_08046CB0(ctx->player, 1 - ctx->player, i);
                    }
                }
            }
            return 0;
        case 0x405:
            if (CARD_TYPE(other->id) == 0x16) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            return 0;
        case 0x406:
            if (CARD_TYPE(other->id) == 0x15) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
            }
            return 0;
        case 0x426:
            if (CARD_NUMBER(other->id) == 0x29F && PLAYER_RAW(other) != PLAYER_RAW(ctx)) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                for (i = 5; i <= 10; i++) {
                    int p = (1 - ctx->player) & 1;
                    struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

                    if (CARD_WORD(z->card) << 20 != 0) {
                        sub_0801EC58(PLAYER_RAW(ctx) == 0 ? 0x808B : 0x8B, i, 1, 0);
                        sub_08018544(1 - ctx->player, i, 1);
                    }
                }
            }
            return 0;
        case 0x5FA:
            if (CARD_TYPE(other->id) <= 0x14
                && (sub_08007590(CARD_NUMBER(other->id), 1) != 0 || sub_08007590(CARD_NUMBER(other->id), 0) != 0))
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
            break;
        case 0x5FB:
            switch (CARD_NUMBER(other->id)) {
            case 0x12C ... 0x13C:
            case 0x13E ... 0x147:
            case 0x28B:
            case 0x28D:
            case 0x290:
            case 0x3C2:
            case 0x3F0:
            case 0x3F4:
            case 0x3F5:
            case 0x3FF:
            case 0x403:
            case 0x412:
            case 0x413:
            case 0x416:
            case 0x417:
            case 0x422:
            case 0x424:
            case 0x42C:
            case 0x430:
            case 0x433:
            case 0x434:
            case 0x485:
            case 0x488:
            case 0x49E:
            case 0x4BB:
            case 0x4C4:
            case 0x521:
            case 0x58B:
            case 0x58C:
            case 0x58E:
            case 0x5A8 ... 0x5AB:
            case 0x604:
            case 0x60A:
            case 0x60C:
            case 0x60E:
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80B0 : 0xB0, 1, 0, 0);
                sub_0801EC58(PLAYER_RAW(other) ? 0x80B1 : 0xB1, other->zone, 1, 0);
                break;
            }
            break;
        }
    }
    return 0;
}
