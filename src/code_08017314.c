#include "global.h"

/*
 * Duel effect helpers around zone links and the effect-message queue
 * (sub_0801EC58). See wiki/functions/code-08017314.md.
 */

struct DuelCard {
    u32 id : 12;        /* card ID (index into gUnk_08621DE0 / gUnk_08622AB4); 0 = none */
    u32 owner : 1;
    u32 unk13 : 5;
    u32 unk18 : 1;
    u32 unk19 : 13;
};
#define CARD(word) (*(struct DuelCard *)&(word))

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    u32 card;               /* +0x00: struct DuelCard */
    u8 filler4[2];
    u8 flags6;              /* +0x06: bit 1 = face up (hypothesis) */
    u8 flags7;              /* +0x07: bit 5 tested by sub_08018280 */
    u8 filler8[2];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkInfo[32];       /* +0x4A: low byte = link kind */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 unk91_0 : 3;         /* +0x91 */
    u8 unk91_3 : 1;
    u8 unk91_4 : 4;
    u8 filler92[2];
};

struct DuelPlayer {
    u8 unk0[0xB];
    u8 unkB;                        /* +0x00B: bit 3 set when a monster enters (hypothesis) */
    u8 unkC[0x1C];
    struct DuelZone zones[11];      /* +0x028 */
    u8 filler684[0xD64 - 0x684];
};
extern struct DuelPlayer gUnk_020192E4[2];

#define PLAYER(p) (gUnk_020192E4[(p) & 1])
#define ZONE(p, z) (gUnk_020192E4[(p) & 1].zones[z])
/* Zone address from a precomputed t = player & 1: base + (zone * 0x94 + t * 0xD64). */
#define ZONE_T(t, z) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + ((z) * 0x94 + (t) * 0xD64)))
#define ZONE_PTR_ZP(p, z) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + ((z) * 0x94 + ((p) & 1) * 0xD64)))
#define ZONE_PTR(p, z) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (((p) & 1) * 0xD64 + (z) * 0x94)))
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_08042AB0(int player, int kind, u32 arg);
u16 sub_0800CD68(int player, int zone);
void sub_08018544(int player, int zone, u16 arg);
int sub_08008A44(int player);
void sub_08009424(u16 loc, u16 a, u16 b);
void sub_08019078(int player, u16 loc, u16 other);
void sub_08017ADC(int player, u16 loc, u16 b, u16 c);
void sub_08017B04(int player, u16 loc, u16 b);
void sub_08017C48(int player, int zone);
void sub_08017DE0(int player, int zone, u16 arg);
int sub_080086CC(int player, u16 cardNo);
int sub_08008524(int player, u16 cardNo);
void sub_0801FBCC(u32 event, int arg);
void sub_08046C20(int player, int arg);
void sub_08019860(int player, int arg);
void sub_08017D38(int player, int zone, int otherPlayer, int otherZone);

void sub_08017314(int player, int zone, u16 link0)
{
    int i, j;
    int owner;
    int t = player & 1;
    struct DuelZone *z = ZONE_T(t, zone);

    if (CARD(z->card).id == 0)
        return;
    owner = player;
    for (i = z->numLinks; i > 0; i--) {
        u16 l;
        u8 kind;
        int lp, lz;
        struct DuelZone *zp = ZONE_T(t, zone);
        j = i - 1;
        l = zp->links[j];
        kind = zp->linkInfo[j];
        lp = (u8)zp->links[j];
        lz = l >> 8;
        if (kind == 1) {
            int lt = lp & 1;
            struct DuelZone *lzp = ZONE_T(lt, lz);
            u16 id = CARD(lzp->card).id;
            if (id != 0 && (lzp->flags6 & 2)) {
                switch (CARD_NUMBER(id)) {
                case 1068: {
                    int f = lzp->unk91_3;
                    if (l == link0)
                        f = 1;
                    owner = lp;
                    if (f)
                        owner = 1 - owner;
                    break;
                }
                case 1514:
                    owner = lp;
                    if (l == link0)
                        owner = 1 - owner;
                    break;
                }
            }
        }
    }
    if (owner != player) {
        int r = sub_08008A44(owner);
        if (r == -1) {
            sub_08009424((u8)player | ((u8)zone << 8), link0, 1);
            sub_08018544(player, zone, 1);
        } else {
            sub_08019078(player, (u8)player | ((u8)zone << 8), (u8)owner | ((u8)r << 8));
        }
    }
}

#if 0 /* NONMATCHING (score 12): Written from scratch from the asm; only the player+0x0B |= 8 block differs
       * (t*0xD64 product lands in r0 instead of r1). Keys: PLAYERS17460->p[player & 1] array view loads the 0x020192E4
       * base first; ZONE_PTR(player, zone) under zone==10 (CSE folds zone to 10); ZONE_PZ (block-local t) for the
       * 461/954/1095/1116 zone tests; u16 id2 in the loops; (owner<<31) | (cond ? A : B) | id; double read of
       * unkE[zone]. */
struct Unk02017A40_17460 { u8 pad[0x48A]; u16 w48A; };
extern struct Unk02017A40_17460 gUnk_02017A40;
struct DuelCard17460 {
    u32 id : 12;
    u32 owner : 1;
    u32 unk13 : 1;
    u32 unk14 : 3;
    u32 unk17 : 15;
};
struct DuelPlayer17460 {
    u8 unk0[0xB];
    u8 unkB;
    u8 unkC[2];
    u16 unkE[11];
    u8 unk24[0xD64 - 0x24];
};
struct DuelPlayers17460 { struct DuelPlayer17460 p[2]; };
#define PLAYERS17460 ((struct DuelPlayers17460 *)gUnk_020192E4)
void sub_080184D8(int player, u16 cardNo);
u16 sub_08009538(int player, int zone);
void sub_08017460(int player, int zone, u16 arg, u16 arg3);
int sub_0800849C(int player, u16 number, int zone);
int sub_0800756C(u16 number);
void sub_08019980(int player, int lp);

#define ZONE_PZ(p, z) ({ int _t = (p) & 1; ZONE_T(_t, z); })

void sub_08017460(int player, int zone, u16 arg, u16 arg3)
{
    int t = player & 1;
    struct DuelZone *z = ZONE_T(t, zone);
    u32 id = CARD(z->card).id;
    struct DuelCard17460 *card;
    int i;
    u16 r;
    u16 number;

    if (id == 0)
        return;
    card = (struct DuelCard17460 *)&ZONE(player, zone).card;
    if (arg3 != 0) {
        switch (CARD_NUMBER(id)) {
        case 333:
            if ((z->flags6 & 2) && zone == 10) {
                sub_080184D8(player, 0x58F);
                sub_080184D8(1 - player, 0x58F);
            }
            break;
        case 1162:
            sub_0801EC58(player ? 0x808F : 0x8F, zone, 0, 0);
            break;
        }
        if (zone == 10 && (ZONE_PTR(player, zone)->flags6 & 2))
            sub_0801EC58(player ? 0x8011 : 0x11, 0, 0, 0);
    }
    if (sub_080086CC(0, 1107) > 0 || sub_080086CC(1, 1107) > 0) {
        sub_0801EC58(player ? 0x807A : 0x7A, zone, arg, 0);
        sub_08017DE0(player, zone, 1);
        return;
    }
    if (CARD_TYPE(id) <= 20)
        PLAYERS17460->p[player & 1].unkB |= 8;
    if (CARD_NUMBER(id) == 478 || CARD_NUMBER(id) == 1042) {
        sub_0801EC58(player ? 0x8073 : 0x73, id, 1, 0);
        sub_0801EC58(player ? 0x8081 : 0x81, zone, arg, 0);
        if (CARD_TYPE(id) <= 20 && zone <= 4)
            sub_08017DE0(player, zone, 1);
        return;
    }
    sub_0801EC58(player ? 0x8079 : 0x79, zone, arg, 0);
    switch (CARD_TYPE(id)) {
    case 0x16:
        sub_08042AB0(1 - player, 0x14, (u16)((u8)player | ((u8)zone << 8)));
        break;
    case 0x15:
        sub_08042AB0(1 - player, 0x15, (u16)((u8)player | ((u8)zone << 8)));
        break;
    default:
        sub_08042AB0(1 - player, 0x1E, (u16)((u8)player | ((u8)zone << 8)));
        break;
    }
    switch (CARD_NUMBER(id)) {
    case 1160:
        r = sub_08009538(player, zone);
        if (r != 0xFFFF && arg3 != 0) {
            sub_08009424(r, (u8)player | ((u8)zone << 8), 1);
            sub_08017460((u8)r, r >> 8, 1, 1);
        }
        break;
    case 1095:
        r = sub_08009538(player, zone);
        if (r != 0xFFFF && arg3 != 0 && !ZONE_PZ(player, zone)->unk91_3) {
            sub_08009424(r, (u8)player | ((u8)zone << 8), 2);
            sub_08017460((u8)r, r >> 8, 1, 1);
        }
        break;
    case 1068:
    case 1514:
        r = sub_08009538(player, zone);
        if (r != 0xFFFF)
            sub_08017314((u8)r, r >> 8, (u8)player | ((u8)zone << 8));
        break;
    }
    sub_08046C20(player, 1);
    if (arg3 == 0)
        return;
    number = CARD_NUMBER(id);
    switch (number) {
    case 47:
    case 573:
    case 1241:
    case 1257:
        if ((*(u32 *)card & 0x1C000) || zone > 4)
            sub_0801FBCC((card->owner << 31) | 0x3C600000 | id, 0);
        break;
    case 1242:
        sub_0801FBCC((card->owner << 31) | 0x3C600000 | id, 0);
        break;
    case 303:
    case 310:
    case 312:
    case 313:
    case 320:
        sub_0801FBCC((card->owner << 31) | (gUnk_02017A40.w48A == 0x13 ? 0x26600000 : 0x28600000) | id, 0);
        break;
    case 461:
        if (!(ZONE_PZ(player, zone)->flags7 & 0x20)) {
            sub_0801EC58(player ? 0x8073 : 0x73, id, 1, 0);
            sub_08019860(player, 5000);
        }
        break;
    case 954:
        if ((ZONE_PZ(player, zone)->flags6 & 2) && !ZONE_PZ(player, zone)->unk91_3) {
            sub_0801EC58(player ? 0x8073 : 0x73, id, 1, 0);
            if (sub_0800849C(player, number, zone) == 0) {
                for (i = 0; i <= 4; i++) {
                    u16 id2 = CARD(ZONE_PTR(player, i)->card).id;
                    if (id2 != 0 && sub_0800756C(CARD_NUMBER(id2)))
                        sub_08017460(player, i, 1, 1);
                }
                for (i = 0; i <= 4; i++) {
                    u16 id2 = CARD(ZONE_PTR(1 - player, i)->card).id;
                    if (id2 != 0 && sub_0800756C(CARD_NUMBER(id2)))
                        sub_08017460(1 - player, i, 1, 1);
                }
            }
            if (PLAYERS17460->p[player & 1].unkE[zone] != 0) {
                sub_08019980(player, PLAYERS17460->p[player & 1].unkE[zone]);
                sub_0801EC58(player ? 0x80B3 : 0xB3, zone, 0, 0);
            }
        }
        break;
    case 1116:
        if (!(ZONE_PZ(player, zone)->flags7 & 0x20))
            sub_0801FBCC(((card->owner & 1) << 31) | 0x3C600000 | id, 0);
        break;
    case 1162:
        sub_0801EC58(player ? 0x808F : 0x8F, zone, 0, 0);
        break;
    }
    sub_08017DE0(player, zone, 1);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08017314", sub_08017460); /* 0x08017460 size 0x654 */

void sub_08017AB4(int player, u16 a, u16 b, u16 c)
{
    sub_0801EC58(player ? 0x8085 : 0x85, a, b, c);
}

void sub_08017ADC(int player, u16 a, u16 b, u16 c)
{
    sub_0801EC58(player ? 0x8086 : 0x86, a, b, c);
}

void sub_08017B04(int player, u16 from, u16 to)
{
    int fp = (u8)from;
    int fz = from >> 8;
    int tp = (u8)to;
    int tz = to >> 8;
    u16 id = CARD(ZONE_PTR(tp, tz)->card).id;

    sub_0801EC58(player ? 0x8083 : 0x83, from, to, 0);
    if (CARD_NUMBER(id) == 1351) {
        sub_0801EC58(tp ? 0x8073 : 0x73, id, 1, 0);
        sub_08018544(fp, fz, 1);
        if (CARD_NUMBER(CARD(ZONE_PTR(fp, fz)->card).id) == 1160)
            sub_08018544(tp, tz, 1);
    } else {
        sub_08042AB0(1 - player, 0x19, from | (to << 16));
    }
}


void sub_08017C0C(u16 loc, u16 arg)
{
    u8 player = loc;
    u16 x = sub_0800CD68(player, loc >> 8);
    sub_08017ADC(player, loc, x, 1);
    sub_08017B04(player, loc, arg);
}

void sub_08017C48(int player, int zone)
{
    int p, z, i;
    for (p = 0; p < 2; p++) {
        for (z = 0; z < 11; z++) {
            if (CARD(ZONE_PTR(p, z)->card).id != 0) {
                for (i = 0; i < ZONE_PTR(p, z)->numLinks; i++) {
                    u16 l = ZONE_PTR(player, zone)->links[p];
                    u8 kind = ZONE_PTR(player, zone)->linkInfo[p];
                    switch (kind) {
                    case 1:
                    case 2:
                        sub_08017ADC(player, (u8)player | ((u8)zone << 8), l, kind);
                        break;
                    }
                }
            }
        }
    }
}

void sub_08017D38(int player, int zone, int otherPlayer, int otherZone)
{
    /* Preserve the ROM's counter allocation; this is an initialized local. */
    register int i asm("r5") = 0;
    for (; i < ZONE_PTR(player, zone)->numLinks; i++) {
        int to = (player & 1) * 0xD64;
        struct DuelZone *z = (struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (zone * 0x94 + to));
        int offset = i * 2;
        u16 *link = (u16 *)((u8 *)z + 0xA);
        u8 *info;
        u16 l;
        int lp;
        int lz;
        int kind;
        link = (u16 *)((u8 *)link + offset);
        info = (u8 *)z + 0x4A;
        info += offset;
        l = *link;
        lp = *(u8 *)link;
        lz = l >> 8;
        kind = *info;
        if (kind == 2 && lp == otherPlayer && lz == otherZone)
            sub_08017ADC(otherPlayer, l, (u8)player | ((u8)zone << 8), 2);
    }
}

void sub_08017DE0(int player, int zone, u16 arg)
{
    int i;
    int z;

    for (i = 0; i < ZONE_PTR(player, zone)->numLinks; i++) {
        int t = player & 1;
        struct DuelZone *zp = ZONE_T(t, zone);
        int offset = i * 2;
        u16 *link = (u16 *)((u8 *)zp + 0xA);
        u8 *info;
        int lp, lz;
        u16 id;
        int f;
        link = (u16 *)((u8 *)link + offset);
        info = (u8 *)zp + 0x4A;
        info += offset;
        lp = (u8)*link;
        lz = *link >> 8;
        id = CARD(ZONE_PTR(lp, lz)->card).id;
        f = 0;

        switch (*info) {
        case 1:
        case 10:
            if (id != 0)
                f = 1;
            break;
        case 2:
            switch (CARD_NUMBER(id)) {
            case 348:
            case 1095:
            case 1417:
                if (!ZONE_PTR(lp, lz)->unk91_3)
                    f = arg;
                break;
            case 1244:
                if (!ZONE_PTR(lp, lz)->unk91_3)
                    f = 1;
                break;
            }
            break;
        case 5:
        case 6:
            f = 1;
            break;
        }
        if (f)
            sub_08018544(lp, lz, arg != 0);
    }
    if (zone <= 4) {
        for (i = 0; i <= 1; i++)
            for (z = 0; z <= 4; z++)
                sub_08017D38(i, z, player, zone);
    }
}


/* For every kind-5 link of the zone, call sub_08018544(linkPlayer, linkZone, 1). */
void sub_08017F98(int player, int zone)
{
    int i = 0;
    if (i < ZONE_PTR(player, zone)->numLinks) {
        struct DuelZone *z = ZONE_PTR(player, zone);
        u16 *count = &z->numLinks;
        do {
            int offset = i * 2;
            u16 *link = (u16 *)((u8 *)z + 0xA);
            u8 *info;
            int lp;
            int lz;
            int kind;
            link = (u16 *)((u8 *)link + offset);
            info = (u8 *)z + 0x4A;
            info += offset;
            lp = *(u8 *)link;
            lz = *link >> 8;
            kind = *info;
            if (kind == 5)
                sub_08018544(lp, lz, 1);
            i++;
        } while (i < *count);
    }
}

int sub_08017FF4(int player, int zone)
{
    int t = player & 1;
    int zo = zone * 0x94;
    int to = t * 0xD64;
    u32 id = CARD(*(struct DuelCard *)((u8 *)gUnk_020192E4[0].zones + to + zo)).id;

    if (zone > 4)
        return 0;
    if (sub_08008524(0, 1418) > 0 || sub_08008524(1, 1418) > 0)
        return 0;
    if (id != 0) {
        struct DuelZone *z = &gUnk_020192E4[player & 1].zones[zone];
        if (sub_080086CC(0, 1107) > 0 || sub_080086CC(1, 1107) > 0) {
            sub_0801EC58(player ? 0x807A : 0x7A, zone, 1, 0);
            sub_08017DE0(player, zone, 1);
            return;
        }
        sub_0801EC58(player ? 0x8093 : 0x93, zone, 0, 0);
        if (CARD_TYPE(id) <= 20)
            { u8 *pl = (u8 *)&PLAYER(player); pl[0xB] |= 8; }
        sub_08046C20(player, 1);
        switch (CARD_NUMBER(id)) {
        case 47:
        case 573:
        case 1242:
        case 1257:
            sub_0801FBCC((CARD(z->card).owner << 31) | 0x3C600000 | id, 0);
            break;
        case 303:
        case 310:
        case 312:
        case 313:
        case 320:
            sub_0801FBCC((CARD(z->card).owner << 31) | 0x28600000 | id, 0);
            break;
        case 461:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20)) {
                sub_0801EC58(player ? 0x8073 : 0x73, id, 1, 0);
                sub_08019860(player, 5000);
            }
            break;
        case 1116:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20))
                sub_0801FBCC(((CARD(z->card).owner & 1) << 31) | 0x3C600000 | id, 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case 729:
        case 1332:
            sub_08017C48(player, zone);
        }
        sub_08017DE0(player, zone, 0);
        sub_08042AB0(1 - player, 0x1E, (u16)((u8)player | ((u8)zone << 8)));
        return 1;
    }
    return 0;
}
/* A monster was placed in (player, zone): queue the "summoned" event, set the player's +0x0B bit 3
 * for monsters, fire the card's on-summon effects, then re-evaluate the zone's links.
 * Returns 1 if handled, 0 for non-monster zones / empty zones. */
int sub_08018280(int player, int zone)
{
    int t = player & 1;     /* must be a separate local for the ROM's evaluation order */
    struct DuelZone *z = ZONE_T(t, zone);
    u32 id = CARD(z->card).id;

    if (zone > 4)
        return 0;
    if (id != 0) {
        struct DuelCard *card = (struct DuelCard *)&ZONE(player, zone).card;
        if (sub_080086CC(0, 1107) > 0 || sub_080086CC(1, 1107) > 0) {
            sub_0801EC58(player ? 0x807A : 0x7A, zone, 1, 0);
            sub_08017DE0(player, zone, 1);
            return;
        }
        sub_0801EC58(player ? 0x80A5 : 0xA5, zone, 0, 0);
        if (CARD_TYPE(id) <= 20) {
            u8 *pl = (u8 *)&PLAYER(player);
            pl[0xB] |= 8;
        }
        sub_08046C20(player, 1);
        switch (CARD_NUMBER(id)) {
        case 47:
        case 573:
        case 1242:
        case 1257:
            sub_0801FBCC((card->owner << 31) | 0x3C600000 | id, 0);
            /* fall through */
        case 303:
        case 310:
        case 312:
        case 313:
        case 320:
            sub_0801FBCC((card->owner << 31) | 0x28600000 | id, 0);
            break;
        case 461:
            if (!(ZONE_PTR(player, zone)->flags7 & 0x20)) {
                sub_0801EC58(player ? 0x8073 : 0x73, id, 1, 0);
                sub_08019860(player, 5000);
            }
            break;
        case 1116:
            if (!(z->flags7 & 0x20))
                sub_0801FBCC(((card->owner & 1) << 31) | 0x3C600000 | id, 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case 729:
        case 1332:
            sub_08017C48(player, zone);
        }
        sub_08017DE0(player, zone, 0);
        return 1;
    }
    return 0;
}
