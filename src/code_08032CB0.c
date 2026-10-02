#include "global.h"
#include "main.h"
#include "duel.h"

/*
 * Duel card-effect step handlers, gated on a u8 flag and the phase, with the signature
 * int f(struct EffCtx *ctx). See wiki/functions/code-08032cb0.md.
 *
 * Uses the shared layouts: struct Main (main.h) and the duel structs/globals (duel.h).
 */

#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)

/* Effect context (argument of every handler; 0x14+ bytes). */
struct EffCtx {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 */
    u8 flags4;          /* +0x04: bit 2 = skip */
    u8 filler5[5];
    u8 phaseA;          /* +0x0A: low 3 bits = phase */
    u8 fillerB;
    u16 pos;            /* +0x0C: player | zone << 8 of a target */
    u8 filler0E[6];
};

int sub_0800C894(int player, int zone);
void sub_08017FF4(int player, int zone);
int sub_0807548C(int a);
void sub_08019860(int player, int a);
void sub_08030028(int player, int zone);
void sub_08046CB0(int a, int player, int zone);
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
extern const u16 gUnk_08623DF4[];   /* card id to card index */
int sub_08008A1C(int player);
int sub_08009CAC(int player, u16 number);
int sub_08009C08(int player, u16 id, u32 *out);
void sub_08056094(int player, u32 *card, int a, int b);
struct Card2 {
    u16 id;
    u16 hi;
};
struct ActBlk {
    u8 filler[0x3E0];
    u8 step;            /* +0x3E0 */
    u8 sub;             /* +0x3E1 */
    u8 filler3E2[0x510 - 0x3E2];
    u8 count510;        /* +0x510 */
    u8 zone511;         /* +0x511 */
    u8 filler512[0x544 - 0x512];
    struct Card2 cards544[8];   /* +0x544: card words */
};
extern struct ActBlk gUnk_02017A40;

/* Card id (0..1999 and 2000+) to card index; 0xFFFF maps to 0. */
static inline u16 CardIdToIndex(u32 id)
{
    if (id == 0xFFFF)
        return 0;
    if (id <= 1999)
        return *(gUnk_08623DF4 + (id & 0x7FF));
    return *(gUnk_08623DF4 + ((id - 2000) & 0x7FF)) + 1;
}
int sub_08008C6C(int player);
void sub_08019078(int player, u16 pos, int c);
void sub_08017DE0(int player, int zone, int a);
#define CARD_ID11(w) (((w) << 21) >> 21)
/* 12-byte slots at 0x02018450, one per player. */
struct Slot {
    u8 filler0[8];
    u8 unk8_0 : 4;
    u8 flag4 : 1;       /* +8 bit 4 */
    u8 flag5 : 1;       /* +8 bit 5 */
    u8 unk8_6 : 2;
    u8 unk9;
    u16 id;             /* +0x0A card id */
};
extern struct Slot gUnk_02018450[];
struct CardBuf {
    u16 lo;
    u8 unk2_0 : 5;
    u8 flag21 : 1;
    u8 unk2_6 : 2;
    u8 unk3;
};
int sub_0800756C(u16 number);
int sub_08008668(int player);
int sub_08008A44(int player);
int sub_08009C64(int player, u32 *card);
void sub_08055F70(int player, u32 *card, int a, int b, int c);
extern struct { u8 filler[0x82C]; u32 cursor; } gUnk_0201CFB0;
extern struct { u8 filler[0x14]; u16 unk14; } gUnk_0201AE60;
struct HandCards {
    u32 cards[0xD64 / 4];
};
extern struct HandCards gUnk_02019968[2];
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const char gUnk_08082BFC[];
extern const char gUnk_08082C34[];
extern const char gUnk_08082C3C[];
extern const char gUnk_08082C70[];
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(u16 a, u16 b, int c, const char *text);
void sub_08060308(int a, int b, int c);
int sub_08052F38(u32 mask);
void sub_08077AEC(u16 id);
int sub_0802DCC4(struct EffCtx *ctx, int a, int b);
void sub_08022814(int player);
#define CARD_STATS2(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
struct SelBlk {
    u8 filler0[5];
    u8 sel : 2;             /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 base;               /* +6 */
    u8 filler8[4];
    u32 cards[64];          /* +0xC: card words */
};
extern struct SelBlk gUnk_0201D810;
#define A_02017F84 ((u32 *)0x02017F84)
#define A_02017F88 ((void *)0x02017F88)
#define A_02017F8C ((void *)0x02017F8C)
#define A_02017E2C ((void *)0x02017E2C)
#define A_02017E28 ((void *)0x02017E28)
extern const char gUnk_08082B9C[];
extern const char gUnk_08082BD0[];
int sub_08044224(int player, int number, int b);
int sub_0802E784(struct EffCtx *ctx, int a, int b);
void sub_0802AF34(int player, int a, u16 number, int b);
void sub_08007558(void *dst, const void *src);
void *sub_08075294(void *dst, const void *src, int n);
s32 sub_08076F9C(void);
int sub_08008524(int player, u16 number);
void sub_08017AB4(int a, int b, int c, int d);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
/* Player bit read as a raw byte (the bitfield form gives lsl/lsr). */
#define PLAYER_RAW(c) (1 & ((u8 *)(c))[2])
int sub_08032CB0(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (ctx->phaseA & 7) == 1) {
        int zone = ctx->pos >> 8;
        int player = ((u8 *)&ctx->pos)[0] & 1;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + player * 0xD64 + (u32)gUnk_0201930C);

        if (CARD_ID(CARD_WORD(z->card)))
            sub_08017AB4(ctx->player, ctx->player | (ctx->zone << 8), ctx->pos, 2);
    }
    return 0;
}
int sub_08032D10(struct EffCtx *ctx)
{
    int cnt = sub_08008C6C(ctx->player);

    if (!(ctx->flags4 & 4)) {
        sub_0801EC58(PLAYER_RAW(ctx) ? 0x8092 : 0x92, ctx->zone, 0, 0);
        if ((ctx->phaseA & 7) == 1 && cnt >= 0) {
            u16 player;
            register int zone asm("sl"); /* FAKEMATCH: ROM keeps zone in sl and the target id in r9 */
            struct DuelZone *tz;
            u32 tid;
            int p1;
            int p0;
            struct DuelZone *z1;

            player = ((u8 *)&ctx->pos)[0];
            zone = ctx->pos >> 8;
            p0 = player & 1;
            tz = (struct DuelZone *)(zone * 0x94 + p0 * 0xD64 + (u32)gUnk_0201930C);
            tid = CARD_ID(CARD_WORD(tz->card));
            p1 = 1 & ctx->player;
            z1 = (struct DuelZone *)(ctx->zone * 0x94 + p1 * 0xD64 + (u32)gUnk_0201930C);

            if (CARD_WORD(z1->card) << 20 != 0) {
                int p2 = 1 & ctx->player;
                struct DuelZone *z2 = (struct DuelZone *)(ctx->zone * 0x94 + p2 * 0xD64 + (u32)gUnk_0201930C);

                /* FAKEMATCH: the r7 clobber stops reload reusing the 0xD64 constant in r7, as the ROM reloads it */
                asm volatile("" ::: "r7");
                if (CARD_NUMBER(CARD_ID11(CARD_WORD(z2->card))) == 0x2DA) {
                    int p3 = 1 & ctx->player;
                    struct DuelZone *z3 = (struct DuelZone *)(ctx->zone * 0x94 + p3 * 0xD64 + (u32)gUnk_0201930C);

                    if ((z3->flag6_1) && tid != 0) {
                        sub_08019078(ctx->player, ctx->pos, ctx->player | (u8)cnt << 8);
                        sub_08017DE0(player, zone, 0);
                        sub_0801EC58(PLAYER_RAW(ctx) ? 0x808C : 0x8C, cnt, 1, 0);
                        sub_08017AB4(ctx->player, ctx->player | (u8)cnt << 8, ctx->player | (ctx->zone << 8), 5);
                    }
                }
            }
        }
    }
    return 0;
}
int sub_08032E7C(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        if (ctx->kind == 2) {
            int sum = 0;
            int i;

            for (i = 0; i <= 4; i++) {
                /* FAKEMATCH: preserve the shifted player bit separately from p.
                 * Both values are initialized; no extra instructions are emitted. */
                register u32 shifted asm("r3") = (u32)((u8 *)ctx)[2] << 31;
                register int p asm("r2") = 1 & (shifted >> 31);
                struct DuelZone *z = (struct DuelZone *)(i * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

                if (CARD_ID(CARD_WORD(z->card))) {
                    sum += sub_0800C894(p, i);
                    sub_08017FF4(ctx->player, i);
                }
            }
            sub_08019860(1 - ctx->player, sub_0807548C(sum));
        } else {
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x8092 : 0x92, ctx->zone, 0, 0);
        }
    }
    return 0;
}

static inline int TypeIs(u32 id, u32 t)
{
    u32 ty = CARD_TYPE(id);
    int r = 0;

    if (ty == t)
        r = 1;
    return r;
}

int sub_08032F20(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gUnk_02017A40.step) {
        case 0x80:
            gUnk_02017A40.count510 = 0;
            return 0x7F;
        case 0x7F:
            gUnk_02017A40.zone511 = 5;
            return 0x7E;
        case 0x7E: {
            int p;
            int p1;
            int zone;
            u32 id;
            struct DuelZone *z;

            if (gUnk_02017A40.count510 != 0)
                p = ctx->player;
            else
                p = 1 - ctx->player;
            zone = gUnk_02017A40.zone511;
            p1 = 1 & p;
            z = (struct DuelZone *)(zone * 0x94 + p1 * 0xD64 + (u32)gUnk_0201930C);
            id = CARD_ID(CARD_WORD(z->card));
            if (id != 0 && !(p == ctx->player && zone == ctx->zone) && (z->flag6_1)) {
                int flag = 0;

                switch (CARD_NUMBER(ctx->id)) {
                case 0x409:
                case 0x2EF:
                    flag = TypeIs(id, 0x15);
                    break;
                case 0x482:
                    flag = TypeIs(id, 0x16);
                    break;
                case 0x601:
                    if (CARD_TYPE(id) == 0x16 && ((CARD_STATS2(id) & 0xE0000) >> 17) == 3)
                        flag = 1;
                    break;
                }
                if (flag != 0) {
                    sub_0801EC58(PLAYER_RAW(ctx) ? 0x8008 : 0x8, p, zone << 8, 0);
                    {
                        int msg = PLAYER_RAW(ctx) ? 0x8074 : 0x74;

                        sub_0801EC58(msg, id, 1, 0);
                    }
                    sub_0801EC58(p != 0 ? 0x80B1 : 0xB1, zone, 1, 0);
                }
            }
            return 0x7D;
        }
        case 0x7D:
            gUnk_02017A40.zone511++;
            if (gUnk_02017A40.zone511 <= 9)
                return 0x7E;
            gUnk_02017A40.count510++;
            if (gUnk_02017A40.count510 <= 1)
                return 0x7F;
            return 0x78;
        case 0x78:
            switch (CARD_NUMBER(ctx->id)) {
            case 0x409:
            case 0x2EF:
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x8019 : 0x19, 1, 0, 0);
                break;
            case 0x482:
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x801A : 0x1A, 1, 0, 0);
                break;
            case 0x601:
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x801B : 0x1B, 1, 0, 0);
                break;
            }
            break;
        }
    }
    return 0;
}
int sub_08033218(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        sub_0801EC58(PLAYER_RAW(ctx) ? 0x8094 : 0x94, ctx->zone, 1 - ctx->player, 0);
        sub_0801EC58(PLAYER_RAW(ctx) == 0 ? 0x8060 : 0x60, 0, 0, 0);
    }
    return 0;
}
int sub_0803327C(struct EffCtx *ctx)
{
    u32 zero = (u8)(ctx->flags4 & 4);

    if (zero == 0) {
        u8 *base = (u8 *)&gUnk_02017A40;
        u8 *step = base + 0x3E0;
        u32 n;

        switch (*step) {
        case 0x80:
            if (sub_08008A1C(ctx->player) <= 1)
                return 0;
            if (sub_08009CAC(ctx->player, 0x2E1) == 0)
                return 0;
            if (sub_08009CAC(ctx->player, 0x2F4) == 0)
                return 0;
            if (sub_08009CAC(ctx->player, 0x320) == 0)
                return 0;
            *(base + 0x3E1) = zero;
            (*step)--;
            /* fall through */
        case 0x7F: {
            u32 card;

            switch (gUnk_02017A40.sub) {
            case 0:
                n = 0x2E1;
                break;
            case 1:
                n = 0x2F4;
                break;
            case 2:
                n = 0x320;
                break;
            }
            if (sub_08009C08(ctx->player, CardIdToIndex(n), &card) != 0) {
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, card, card >> 16, 0);
                sub_08056094(ctx->player, &card, 1, 0x20);
            }
            gUnk_02017A40.sub++;
            if (gUnk_02017A40.sub <= 2)
                return 0x7F;
            return 0x7E;
        }
        }
    }
    return 0;
}
int sub_08033404(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4) && (ctx->phaseA & 7) == 1) {
        int player = ((u8 *)&ctx->pos)[0];
        int zone = ctx->pos >> 8;
        int p = player & 1;
        struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

        if (CARD_ID(CARD_WORD(z->card)) && (z->flag6_1)) {
            int atk = sub_0800C894(player, zone);

            sub_08030028(player, zone);
            sub_08046CB0(ctx->player, player, zone);
            switch ((int)CARD_NUMBER(ctx->id)) {
            case 0x3AB:
                sub_08019860(1 - ctx->player, atk);
                sub_08019860(ctx->player, atk);
                break;
            case 0x5AB:
                sub_0801EC58(PLAYER_RAW(ctx) ? 0x8044 : 0x44, 0, 0, 0);
                break;
            }
        }
    }
    return 0;
}
int sub_080334E4(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        switch (gUnk_02017A40.step) {
        case 0x80: {
            int phase = 7 & ctx->phaseA;

            if (phase != 1)
                return 0;
            if (sub_08044224(ctx->player, CARD_NUMBER(ctx->id), 0) <= 1)
                return 0;
            {
                int player = ((u8 *)&ctx->pos)[0];
                int zone = ctx->pos >> 8;
                int p = phase & player;
                struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + p * 0xD64 + (u32)gUnk_0201930C);

                if (!CARD_ID(CARD_WORD(z->card)))
                    return 0;
                if (ctx->player != player)
                    return 0;
            }
            if (sub_0802E784(ctx, 0, 0) == 0)
                return 0;
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082B9C);
            return 0x7F;
        }
        case 0x7F:
            sub_0802AF34(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7E;
        case 0x7E: {
            u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.base + gUnk_0201D810.sel];

            sub_08007558(A_02017F84, card);
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082BD0);
            return 0x7D;
        }
        case 0x7D:
            sub_0802AF34(ctx->player, -1, CARD_NUMBER(ctx->id), 0);
            return 0x7C;
        case 0x7C: {
            u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.base + gUnk_0201D810.sel];
            u8 *b = (u8 *)0x02017F88;

            sub_08007558(b, card);
            /* FAKEMATCH: retain the initialized destination before the player mask. */
            __asm__("" : : "r"(b));
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            {
                u8 *dst = b + 4;
                struct DuelZonesPlayer *pp = &gUnk_0201930C[1 & ctx->pos];
                struct DuelZone *z = (struct DuelZone *)((u32)pp + (ctx->pos >> 8) * 0x94);

                sub_08007558(dst, z);
            }
            {
                u8 *dst = b - 0x15C;
                struct DuelZonesPlayer *pp = &gUnk_0201930C[1 & ctx->pos];
                struct DuelZone *z = (struct DuelZone *)((u32)pp + (ctx->pos >> 8) * 0x94);

                sub_08075294(dst, z, 0x94);
            }
            sub_0801EC58((u8)ctx->pos ? 0x8078 : 0x78, ctx->pos >> 8, 8, 0);
            sub_0801EC58((u8)ctx->pos ? 0x808D : 0x8D, ctx->pos >> 8, 0xA, 0);
            return 0x7B;
        }
        case 0x7B: {
            int i = 0;
            u8 *tmp = (u8 *)0x02017E28;
            u32 *arr = (u32 *)(tmp + 0x15C);

            do {
                int a;
                int b;

                do {
                    a = sub_08076F9C() % 3;
                    b = sub_08076F9C() % 3;
                } while (a == b);
                sub_08007558(tmp, &arr[a]);
                sub_08007558(&arr[a], &arr[b]);
                sub_08007558(&arr[b], tmp);
                i++;
            } while (i <= 15);
            gUnk_02017A40.sub = 0;
            return 0x7A;
        }
        case 0x7A: {
            int v = sub_08008A44(ctx->player);
            struct Card2 *card = &gUnk_02017A40.cards544[gUnk_02017A40.sub];
            int flag = 0;

            /* FAKEMATCH: keep the formed card pointer alive across the flag checks. */
            __asm__("" : : "r"(card));
            if (sub_08008524(0, 0x47F) != 0 || sub_08008524(1, 0x47F) != 0)
                flag = 1;
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x80A8 : 0xA8, (u8)v | ((flag | 2) << 8), card->id, card->hi);
            if (CARD_TYPE(CARD_ID11((u32)gUnk_02017A40.cards544[gUnk_02017A40.sub].id)) <= 0x14)
                sub_0801EC58((u8)ctx->pos ? 0x808D : 0x8D, 0xA, (u16)v, 0);
            gUnk_02017A40.sub++;
            if (gUnk_02017A40.sub <= 2)
                return 0x7A;
            return 0x79;
        }
        case 0x79:
            sub_0801EC58(PLAYER_RAW(ctx) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x78;
        }
    }
    return 0;
}
u16 sub_080338CC(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        int p;
        int flag;
        u32 card;
        struct Slot *s;
        struct Slot *slots;
        int step = gUnk_02017A40.step;

        switch (step) {
        case 0x7F:
        case 0x80:
            {
                switch (step) {
                case 0x80:
                    p = ctx->player;
                    break;
                case 0x7F:
                    p = 1 - ctx->player;
                    break;
                }
                flag = 1;
                slots = gUnk_02018450;
                s = slots + p;
                if (sub_0800756C(CARD_NUMBER(s->id)) != 0)
                {
                    flag = 0;
                    if (sub_08008668(p) != 0)
                        flag = 1;
                }
                if (((int)((u32)((u8 *)s)[8] << 26) < 0) && flag && sub_08008A44(p) >= 0) {
                    int q;

                    if (sub_08009C08(p, s->id, &card) != 0 && sub_08009C64(p, &card) != 0) {
                        sub_0801EC58(PLAYER_RAW(ctx) ? 0x80D3 : 0xD3, card, card >> 16, 0);
                        ((struct CardBuf *)&card)->flag21 = 0;
                        sub_08055F70(p, &card, 1, s->flag4, 0x20);
                    }
                    q = 1 - p;
                    if (sub_08009C08(q, gUnk_02018450[p].id, &card) != 0 && sub_08009C64(q, &card) != 0) {
                        sub_0801EC58(PLAYER_RAW(ctx) == 0 ? 0x80D3 : 0xD3, card, card >> 16, 0);
                        ((struct CardBuf *)&card)->flag21 = 0;
                        sub_08055F70(p, &card, 1, gUnk_02018450[p].flag4, 0x20);
                    }
                }
                gUnk_02018450[p].flag5 = 0;
                return gUnk_02017A40.step - 1;
            }
        default:
            return 0;
        }
    }
    return 0;
}
int sub_08033A78(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4))
        sub_0801EC58(PLAYER_RAW(ctx) ? 0x8037 : 0x37, 0, 0, 0);
    return 0;
}
int sub_08033AAC(struct EffCtx *ctx)
{
    if (!(ctx->flags4 & 4)) {
        struct DuelPlayer *pl = gUnk_020192E4;
        int p = 1 - ctx->player;

        if (pl[p & 1].handCount != 0)
            sub_08022814(1 - ctx->player);
    }
    return 0;
}
int sub_08033AEC(struct EffCtx *ctx)
{
    char bufA[0x100];
    char bufB[0x100];
    char bufC[0x100];

    if (ctx->flags4 & 4)
        return 0;
    switch (gUnk_02017A40.step) {
    case 0x80:
        gUnk_02017A40.sub = 0;
        gUnk_02017A40.step--;
        /* fall through */
    case 0x7F:
        if (sub_08008A1C(ctx->player) == 0)
            return 0;
        if (sub_0802DCC4(ctx, 0, 0) == 0)
            return 0;
        sub_080753F4(bufA, gUnk_08082BFC, gUnk_08082C34);
        sub_080602A4(0x205, 0x914, 0xB, bufA);
        sub_08060308(1, 0, 0);
        return 0x7C;
    case 0x7E:
        if (gUnk_03000040.newKeys & 2) {
            sub_080753F4(bufB, gUnk_08082BFC, gUnk_08082C34);
            sub_080602A4(0x205, 0x914, 0xB, bufB);
            sub_08060308(1, 0, 0);
            return 0x7C;
        }
        if (sub_08052F38(1) != 0) {
            int pl = ctx->player;
            u32 word = *(u32 *)(pl * 0xD64 + gUnk_0201CFB0.cursor * 4 + (u32)gUnk_02019968);
            u32 id = CARD_ID11(word);

            if (CARD_TYPE(id) == 1 && (sub_0800756C(CARD_NUMBER(id)) == 0 || sub_08008668(ctx->player) != 0)) {
                u32 *hand = (u32 *)((1 & ctx->player) * 0xD64 + (u32)gUnk_02019968);
                u32 *card = hand + gUnk_0201CFB0.cursor;
                /* FAKEMATCH: the signed byte/halfword flag adds RTL insns while the card pointer is live, so
                 * global alloc ranks the type (r4) above the card pointer (r5) as in the ROM. */
                s16 f = (s8)PLAYER_RAW(ctx);

                sub_0801EC58(f ? 0x80C2 : 0xC2, ((u16 *)card)[0], ((u16 *)card)[1], 0);
                sub_08056094(ctx->player, card, 1, 0);
                return 0x7D;
            }
            sub_08077AEC(3);
        }
        return 0x7E;
    case 0x7D:
        gUnk_02017A40.sub++;
        if (gUnk_02017A40.sub == 2)
            return 0;
        if (sub_08008A1C(ctx->player) != 0) {
            int i;

            for (i = 0; i < gUnk_020192E4[1 & ctx->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ctx->player].hand[i]));
                /* FAKEMATCH: a named type-minus-one keeps the base copy before the 0x7FF load (r7/r4 as in
                 * the ROM) and compares against the immediate 1. */
                int t = CARD_TYPE(id) - 1;

                if (t != 0)
                    continue;
                sub_080602A4(0x205, 0x914, 0xB, gUnk_08082C3C);
                sub_08060308(1, 0, 0);
                return 0x7C;
            }
        }
        return 0;
    case 0x7C:
        if (gUnk_0201AE60.unk14 == 0)
            return 0;
        sub_080753F4(bufC, gUnk_08082C70, gUnk_08082C34);
        sub_080602A4(0x205, 0x914, 0xB, bufC);
        return 0x7E;
    }
    return 0;
}

