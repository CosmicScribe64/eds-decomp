#include "global.h"

struct DuelCard { u32 id:12; u32 flag12:1; u32 rest:19; };
struct ActRec {
    u32 player:1, zone5:5, zone8:8, f14:1, f15:1;
    u32 f16:3, f19:3, f22:3, f25:1, f26:1, f27:1, f28:1, f29:1, f30:1;
    u32 cardId:16, pad47:17;
    struct DuelCard card;
    u16 h0C;
    u32 active:1, ready:1, kind:3, step:7, phase:4;
    u32 aux0:3, aux3:8, aux11:8, pad19:13;
};
extern struct ActRec gUnk_0201CF90;
struct DuelPlayer { u8 pad0[8]; u8 f8lo:4, handAction:1, queued:1, f8hi:2; u8 rest[0xD64-9]; };
extern struct DuelPlayer gUnk_020192E4[];
extern u8 gUnk_02015EE8[];
struct DuelWork { u8 pad[0x306]; u8 flags; };
extern struct DuelWork gUnk_02017FB0;
void sub_080229BC(int command, void *data, int size);
void sub_0801EC58(int command, int a, int b, int c);
void sub_08024134(int player, int zone, int index);
void sub_0801FBCC(u32 action, u32 target);
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
void sub_08017FF4(int player, int zone);
struct Menu { u8 pad[0x14]; u16 selection; };
extern struct Menu gUnk_0201AE60;
int sub_080576BC(int id, int flag);
void sub_080602A4(int a, int b, int c, const void *text);
void sub_08060308(int kind, void (*draw)(void), int (*keys)(void));
void sub_08054770(void);
int sub_0805487C(void);
extern const u8 gUnk_08086370[];
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_08018544(int player, int zone, int flag);
int sub_08008524(int player, u16 number);
void sub_08046A74(int player);
extern u32 gUnk_02019968[];
void sub_08055A00(void);
int sub_0800756C(int number);
u16 sub_08054900(void), sub_08054B60(void), sub_08054E7C(void), sub_080555B0(void);
void sub_080199E0(int player, int zone);
void sub_08046738(int player, int zone);
void sub_080469DC(void), sub_08046AD0(void);
void sub_08046B54(int player, int zone);
void sub_08046808(int player);
void sub_08042AB0(int player, int kind, int target);
void sub_0802297C(int command, int a, int b, int c);
struct Zone { u32 card; u8 pad[2]; u8 flag0:1, flag1:1, rest:6; u8 tail[0x94-7]; };
extern u8 gUnk_0201930C[];
#define ZONE(p,z) ((struct Zone *)((z)*0x94 + (p)*0xD64 + (u32)gUnk_0201930C))
extern const u16 gUnk_08623DF4[];
static inline int NumberId(int number)
{
    /* FAKEMATCH: the original table lookup keeps its initialized address in r0. */
    register u32 address __asm__("r0") = number * 2;
    address += (u32)gUnk_08623DF4;
    return *(const u16 *)address;
}
#define NUMBER_ID(n) NumberId(n)

int sub_0802CFA0(int player, u16 id, u16 x);
u32 sub_08007590(u16 number, u16 flag);
void sub_080197E0(int player, u16 id);
void sub_080467B0(int player);
void sub_08018ED8(int player, int zone, u16 a, u16 b);
/* Zone address terms are staged in the same order as the record loads. */
static inline struct Zone *ActionZone(int player, int zone)
{
    int off = zone * 0x94;
    off += player * 0xD64;
    off += (u32)gUnk_0201930C;
    return (struct Zone *)off;
}

static inline struct Zone *RecordZone(struct ActRec *r)
{
    int player = r->player & 1;
    int zone = r->zone5;
    return ActionZone(player, zone);
}
#define ACTION_STATS(id) (((const u32 *)0x08621DE0)[(id)&0x7FF])
static inline u16 ActionNumberId(int number)
{
    /* FAKEMATCH: retain the initialized doubled table index in r1. */
    register u32 off asm("r1") = number * 2;
    off += (u32)gUnk_08623DF4;
    return *(const u16 *)off;
}

static inline u32 ActionCardLevel(u32 id)
{
    switch ((int)((ACTION_STATS(id) & 0x1F00000) >> 20)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (ACTION_STATS(id) & 0x1E000000) >> 25;
    }
}

u16 sub_08054E7C(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    switch (r->step) {
    case 0: {
        int p = r->player;
        int side = p & 1;
        int z = r->zone5;
        if (ActionZone(side, z)->flag0) {
            u32 id;
            sub_0801EC58(p ? 0x807E : 0x7E, z, 1, 0);
            id = ((RecordZone(r)->card << 20) >> 20);
            if (CARD_NUMBER(id) == 0x5E && sub_0802CFA0(r->player, id, 0)) {
                int player = r->player & 1;
                u32 action = (u32)player << 31;
                int zone = r->zone5;
                u32 destination = zone << 16;
                destination |= 0x14400000;
                action |= destination;
                action |= ((ActionZone(player, zone)->card << 20) >> 20);
                sub_0801FBCC(action, 0);
            }
        } else {
            sub_0801EC58(p ? 0x807F : 0x7F, z, 1, 0);
        }
        gUnk_0201CF90.step++;
        return 0;
    }
    case 1:
        sub_08024134(r->player, 0, r->zone5);
        sub_0801EC58(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2:
        if (ActionCardLevel(r->cardId) <= 2) {
            int number = 0x2AE;
            if (sub_08008524(0, number) > 0 || sub_08008524(1, number) > 0) {
                sub_080197E0(gUnk_0201CF90.player, ActionNumberId(number));
                sub_08018544(gUnk_0201CF90.player, gUnk_0201CF90.zone5, 1);
                return 1;
            }
        }
        gUnk_0201CF90.step++;
        return 0;
    case 3:
        sub_0801EC58(r->player ? 0x8090 : 0x90, r->zone5, r->h0C, 0);
        {
            /* FAKEMATCH: preserve the initialized record byte in its original scratch. */
            register u32 byte asm("r3") = *(u8 *)r;
            asm("" : : "r"(byte));
            sub_080467B0((byte << 31) >> 31);
        }
        switch (CARD_NUMBER(r->cardId)) {
        case 0x1F3:
        case 0x455:
        case 0x462:
        case 0x4D8:
        case 0x4DE:
        case 0x534:
            if (sub_0802CFA0(gUnk_0201CF90.player, ((RecordZone(&gUnk_0201CF90)->card << 20) >> 20), 0)) {
                int player = gUnk_0201CF90.player;
                u32 action = (u32)(player & 1) << 31;
                int zone = gUnk_0201CF90.zone5;
                u32 destination = zone << 16;
                destination |= 0xC400000;
                action |= destination;
                action |= gUnk_0201CF90.cardId;
                sub_0801FBCC(action, player | zone << 8);
            }
            break;
        case 0x31C:
            sub_08018ED8(r->player, r->zone5, 0, 0);
            break;
        }
        if (sub_08007590(CARD_NUMBER(gUnk_0201CF90.cardId), 0)) {
            if (sub_0802CFA0(gUnk_0201CF90.player, ((RecordZone(&gUnk_0201CF90)->card << 20) >> 20), 0) &&
                !sub_08008524(0, 0x5FA) && !sub_08008524(1, 0x5FA)) {
                int player = gUnk_0201CF90.player;
                u32 action = (u32)(player & 1) << 31;
                int zone = gUnk_0201CF90.zone5;
                u32 destination = zone << 16;
                destination |= 0xC400000;
                action |= destination;
                action |= gUnk_0201CF90.cardId;
                sub_0801FBCC(action, player | zone << 8);
            }
        }
        gUnk_0201CF90.step++;
        return 0;
    default:
        return 1;
    }
}

u16 sub_08055298(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    switch (r->step) {
    case 0: {
        int command = r->player ? 0x8077 : 0x77;
        int zone = r->zone5;
        int flag = r->f14;
        int flags = r->f15 << 1;
        flags |= flag;
        sub_0801EC58(command, zone | (flags << 8),
                    *(u16 *)&r->card, *((u16 *)&r->card + 1));
        r->step++;
        return 0;
    }
    case 1:
        sub_08024134(r->player, 0, r->zone5);
        sub_0801EC58(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2: {
        int command = r->player ? 0x8090 : 0x90;
        int id;
        sub_0801EC58(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        if (CARD_NUMBER(id) == 0x4DE) {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xC400000;
            action |= destination;
            action |= id;
            sub_0801FBCC(action, player | (zone << 8));
        }
        r->step++;
        return 0;
    }
    default:
        return 1;
    }
}
u16 sub_080553B0(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    struct DuelCard copy;
    switch (r->step) {
    case 0:
        if (r->player) {
            gUnk_0201AE60.selection = sub_080576BC(r->cardId, r->f14);
        } else {
            sub_080602A4(0x207, 0x30F, 11, gUnk_08086370);
            sub_08060308(5, sub_08054770, sub_0805487C);
        }
        r->step++;
        return 0;
    case 1: {
        int command, zone, flag, flags;
        u32 word;
        r->f15 = gUnk_0201AE60.selection;
        if (!r->f15) r->f14 = 1;
        sub_08007558(&copy, &r->card);
        command = r->player ? 0x8077 : 0x77;
        zone = r->zone5;
        flag = r->f14;
        flags = r->f15 << 1;
        flags |= flag;
        sub_0801EC58(command, zone | (flags << 8), (u16)*(u32 *)&copy, *(u32 *)&copy >> 16);
        r->step++;
        return 0;
    }
    case 2:
        sub_08024134(r->player, 0, r->zone5);
        sub_0801EC58(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 3: {
        int command = r->player ? 0x8090 : 0x90;
        int id, number;
        sub_0801EC58(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        number = CARD_NUMBER(id);
        switch (number) {
        case 0x4DE: {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xC400000;
            action |= destination;
            action |= id;
            sub_0801FBCC(action, player | (zone << 8));
        }
        break;
        case 0x5F6: {
            int i;
            for (i = 0; i < 5; i++) {
                if (i != gUnk_0201CF90.zone5)
                    sub_08018544(gUnk_0201CF90.player, i, 1);
            }
        }
        break;
        }
        gUnk_0201CF90.step++;
        return 0;
    }
    default:
        return 1;
    }
}
u16 sub_080555B0(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    switch (r->step) {
    case 0: {
        u16 msg;
        int id;
        if (r->f25) sub_08017FF4(r->player, r->f16);
        if (r->f26) sub_08017FF4(r->player, r->f19);
        if (r->f27) sub_08017FF4(r->player, r->f22);
        msg = r->player ? 0x80C4 : 0xC4;
        id = r->cardId;
        {
            /* FAKEMATCH: the initialized shifted field stays in r0 until masked. */
            register u32 shifted asm("r0") = *(u16 *)r >> 6;
            u32 mask = 15;
            u32 packed = mask;
            packed &= shifted;
            packed <<= 4;
            mask &= r->zone5;
            packed |= mask;
            packed |= (r->f14 | r->f15 << 1) << 8;
            sub_0801EC58(msg,id,packed,0);
        }
        r->step++;
        return 0;
    }
    case 1:
        sub_08024134(r->player, 0, r->zone5);
        sub_0801EC58(0x71, r->cardId, 1, 0);
        r->step++;
        return 0;
    case 2: {
        int command = r->player ? 0x8090 : 0x90;
        int id;
        sub_0801EC58(command, r->zone5, r->h0C, 0);
        id = r->cardId;
        if (CARD_NUMBER(id) == 0x4DE) {
            int player = r->player;
            u32 action = player << 31;
            int zone = r->zone5;
            u32 destination = zone << 16;
            destination |= 0xA400000;
            action |= destination;
            action |= id;
            sub_0801FBCC(action, player | (zone << 8));
        }
        r->step++;
        return 0;
    }
    default:
        return 1;
    }
}


/* FAKEMATCH: the initialized u16 copy below preserves the original r8-to-r2
 * transfer. Its input is a 12-bit card id, so narrowing is lossless. */
u16 sub_08055728(void)
{
    struct RawAct { u8 pad[14]; u8 flags; };
    int flags = ((struct RawAct *)&gUnk_0201CF90)->flags;
    if (flags & 1) {
        if (!(2 & flags) || !gUnk_0201CF90.player) {
            switch (((u32)flags << 27) >> 29) {
            case 1: if (!sub_08054900()) return 1; break;
            case 2: if (!sub_08054B60()) return 1; break;
            case 3: if (!sub_08054E7C()) return 1; break;
            case 4: if (!sub_08055298()) return 1; break;
            case 5: if (!sub_080553B0()) return 1; break;
            case 6: if (!sub_080555B0()) return 1; break;
            }
        } else {
            if (!(gUnk_02017FB0.flags >> 7)) return 1;
        }
        {
            struct ActRec *r = &gUnk_0201CF90;
            struct Zone *zone;
            int card;
            r->active = 0;
            {
                int player = r->player;
                int index = r->zone5;
                zone = ZONE(player, index);
            }
            card = (zone->card << 20) >> 20;
            if (card != 0) {
                int id = card;
                int wasFlagged = zone->flag1;
                sub_08024134(r->player, 0, r->zone5);
                if (r->h0C & 0x20) {
                    int player = r->player;
                    int number = 0x595;
                    int index = sub_08008524(player, number);
                    if (index > 0) {
                        sub_0801EC58(r->player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                        sub_080199E0(r->player, index);
                    }
                }
                if (wasFlagged) {
                    int number = 0x610;
                    if (sub_08008524(0, number) || sub_08008524(1, number)) {
                        sub_0801EC58(gUnk_0201CF90.player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                        sub_0801EC58(gUnk_0201CF90.player ? 0x8096 : 0x96, gUnk_0201CF90.zone5, 1, 0);
                    }
                }
                {
                    u32 index = 0x7FF;
                    register u16 savedId __asm__("r2") = id;
                    int number;
                    index &= savedId;
                    number = ((const u16 *)0x08622AB4)[index];
                    switch (number) {
                    case 0x62:
                        sub_08046738(gUnk_0201CF90.player, gUnk_0201CF90.zone5);
                        break;
                    case 0x2EF:
                        if (wasFlagged) {
                            sub_0801EC58(gUnk_0201CF90.player ? 0x8073 : 0x73, NUMBER_ID(number), 1, 0);
                            sub_080469DC();
                        }
                        break;
                    case 0x464:
                        if (wasFlagged) sub_08046AD0();
                        break;
                    }
                }
                if (wasFlagged)
                    sub_08046B54(gUnk_0201CF90.player, gUnk_0201CF90.zone5);
                {
                    u32 byte = *(u8 *)&gUnk_0201CF90;
                    sub_08046808((byte << 31) >> 31);
                }
                {
                    int kind;
                    u32 kindFlags = ((struct RawAct *)&gUnk_0201CF90)->flags;
                    switch ((int)((kindFlags << 27) >> 29)) {
                    case 1: case 2: { int choice = gUnk_0201CF90.f14 ? 5 : 8; kind = choice; break; }
                    case 3: kind = 6; break;
                    default: kind = 7; break;
                    }
                    sub_08042AB0(1 - gUnk_0201CF90.player, kind,
                                 gUnk_0201CF90.player | (gUnk_0201CF90.zone5 << 8));
                }
            }
        }
        if ((((struct RawAct *)&gUnk_0201CF90)->flags & 2) && !gUnk_0201CF90.player)
            sub_0802297C(0xF05B, 0, 0, 0);
    }
    return gUnk_0201CF90.active;
}

void sub_08055A00(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    r->step = 0;
    r->phase = 0;
    r->aux0 = 0;
    r->aux3 = 0;
    r->aux11 = 0;
    r->active = 1;
    r->ready = 0;
    gUnk_020192E4[r->player & 1].queued = 1;
    if (r->player && (gUnk_02015EE8[1] & 1)) {
        r->ready = 1;
        sub_080229BC(0xF05A, r, 0x14);
        gUnk_02017FB0.flags &= 0x7F;
    }
}
void sub_08055AB4(void)
{
    struct ActRec *r = &gUnk_0201CF90;
    r->player = 0;
    r->step = 0;
    r->phase = 0;
    r->aux0 = 0;
    r->aux3 = 0;
    r->aux11 = 0;
    r->active = 1;
    r->ready = 1;
    {
        struct DuelCard *card = &r->card;
        card->flag12 = 1 - card->flag12;
    }
}
/* Word-valued callers are decoded by the original two halfword shifts. */
/* Queue a hand summon/set and decode the two packed tribute bytes.
 * FAKEMATCH: five initialized register bindings and two empty constraints
 * preserve the original packed-record loads and stores. Caller-saved bindings
 * die before calls. The card ID spans byte 3 and the low 15 bits of halfword 4;
 * stage that split explicitly to retain the original cached low bit. */
struct QueueBytes {
    u8 byte0, byte1;
};
void sub_08055B28(int player, int zone, int target, int tributeWord, int faceWord)
{
    u16 tribute = tributeWord;
    u16 faceUp = faceWord;
    register struct ActRec *r asm("r9");
    u32 mask;
    register u32 idmask asm("r8");
    const u16 *table;
    gUnk_0201CF90.player = player;
    gUnk_0201CF90.zone5 = target;
    gUnk_0201CF90.zone8 = zone;
    if (faceUp != 0) {
        gUnk_0201CF90.f14 = 1;
        gUnk_0201CF90.f15 = 0;
    }
    else {
        gUnk_0201CF90.f14 = 0;
        gUnk_0201CF90.f15 = 1;
    }
    if (sub_08008524(0, 0x47F) || sub_08008524(1, 0x47F)) gUnk_0201CF90.f14 = 1;
    if (tribute != 0) {
        u8 lo = tribute;
        u8 hi = tribute >> 8;
        gUnk_0201CF90.f16 = lo & 7;
        gUnk_0201CF90.f19 = hi & 7;
        gUnk_0201CF90.f25 = lo >> 7;
        gUnk_0201CF90.f26 = hi >> 7;
        gUnk_0201CF90.f28 = (lo >> 4) & 1;
        gUnk_0201CF90.f29 = (hi >> 4) & 1;
        r = &gUnk_0201CF90;
    }
    else {
        gUnk_0201CF90.f16 = 0;
        gUnk_0201CF90.f19 = 0;
        gUnk_0201CF90.f25 = 0;
        gUnk_0201CF90.f26 = 0;
        r = &gUnk_0201CF90;
    }
    {
        struct ActRec *first = r;
        u32 offset;
        u32 packed;
        {
            u32 side = player;
            u32 address;
            side &= 1;
            address = (u32)zone * 4;
            offset = side * 0xD64;
            address += offset;
            address += (u32)gUnk_02019968;
            address = *(u32 *)address;
            address <<= 20;
            {
                u32 low = address >> 20;
                low &= 1;
                low <<= 7;
                packed = *((u8 *)first + 3) & 0x7F;
                packed |= low;
                *((u8 *)first + 3) = packed;
            }
            mask = 0x7FFF;
            address >>= 21;
            {
                u32 clear = 0xFFFF8000;
                *(u16 *)((u8 *)first + 4) = (*(u16 *)((u8 *)first + 4) & clear) | address;
            }
        }
        first->kind = 1;
        first->h0C = 3;
        if (!sub_0800756C(({
            u32 value;
            packed >>= 7;
            value = mask;
            value &= *(u16 *)((u8 *)first + 4);
            value <<= 1;
            value |= packed;
            idmask = 0x7FF;
            value &= idmask;
            value <<= 1;
            table = (const u16 *)0x08622AB4;
            *(const u16 *)((u32)table + value);
        }
        ))) {
            first->h0C = 2;
            {
                u32 base = (u32)gUnk_02019968 - 0x684;
                base = offset + base;
                {
                    u32 flags = *((u8 *)base + 8);
                    *((u8 *)base + 8) = flags | 0x10;
                }
            }
        }
    }
    if (sub_0800756C(({
        struct ActRec *rp = r;
        u32 low = ((u8 *)rp)[3];
        u32 bit = low >> 7;
        register struct ActRec *hp asm("r2") = r;
        u32 high = ((u16 *)hp)[2];
        u32 value;
        mask &= high;
        value = (mask << 1) | bit;
        {
            register u32 m asm("r3") = idmask;
            asm("" : : "r"(m));
            value &= m;
        }
        table[value];
    }
    ))) {
        u32 flags = 0x40;
        register struct QueueBytes *readp asm("r6") = (struct QueueBytes *)r;
        flags |= readp->byte1;
        {
            struct QueueBytes *dst = (struct QueueBytes *)r;
            /* Keep the store address in an ordinary low-register pseudo.
             * A direct r7 binding makes old_agbcc emit a separate ADD.
             * These scratch clobbers are dead here and emit no instructions. */
            asm("" : "+l"(dst) : "r"(flags) : "r1", "r2", "r3", "r4", "r5", "r6");
            dst->byte1 = flags;
        }
    }
    sub_08046A74(player);
    sub_08055A00();
}

void sub_08055D3C(int player, int zone, int target, u16 tribute)
{
    struct ActRec *r;
    gUnk_0201CF90.player = player;
    gUnk_0201CF90.zone5 = target;
    gUnk_0201CF90.zone8 = zone;
    gUnk_0201CF90.f14 = 0;
    if (sub_08008524(0, 0x47F) || sub_08008524(1, 0x47F))
        gUnk_0201CF90.f14 = 1;
    if (tribute != 0) {
        u8 lo = tribute;
        u8 hi = tribute >> 8;
        r = &gUnk_0201CF90;
        r->f16 = lo & 7;
        r->f19 = hi & 7;
        r->f25 = lo >> 7;
        r->f26 = hi >> 7;
        r->f28 = (lo >> 4) & 1;
        r->f29 = (hi >> 4) & 1;
    } else {
        gUnk_0201CF90.f16 = 0;
        gUnk_0201CF90.f19 = 0;
        gUnk_0201CF90.f25 = 0;
        gUnk_0201CF90.f26 = 0;
        r = &gUnk_0201CF90;
    }
    r->cardId = ((*(u32 *)((player & 1) * 0xD64 + zone * 4 + (u32)gUnk_02019968)) << 20) >> 20;
    r->kind = 2;
    r->h0C = 3;
    sub_08046A74(player);
    sub_08055A00();
}
