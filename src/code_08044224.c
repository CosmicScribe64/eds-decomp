#include "global.h"

/* Local raw-word view: the search copies complete card words, including flags. */
struct TargetPlayer {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 otherCount;
    u8 unk7[0x684 - 7];
    u32 hand[80];
    u32 deck[80];
    u32 graveyard[80];
    u32 fusionDeck[80];
    u32 otherCards[80];
    u16 otherKinds[80];
};

/* The list viewer's +0x20C region is interpreted as 128 halfword area tags here. */
struct TargetList {
    u8 header[0xC];
    u32 cards[128];
    u16 areas[128];
    u16 count;
};

struct TargetPlayerList {
    u32 cards[80];
    u8 rest[0xD64 - 80 * 4];
};

extern struct TargetPlayer gUnk_020192E4[2];
extern struct TargetPlayerList gUnk_02019AA8[2];
extern struct TargetPlayerList gUnk_02019BE8[2];
extern struct TargetList gUnk_0201D810;
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];

void sub_08007558(u32 *dst, u32 *src);
u32 sub_0800756C(u16 number);
u32 sub_08007730(u16 id);
u32 sub_08007834(u16 id);
u16 sub_08008668(int player);
u32 sub_0800966C(u16 id);
int sub_0803CDEC(u16 firstId, u16 secondId);
int sub_0803D0E8(int player, u16 id, u16 *materials);
int sub_0804412C(int player, int index);
u16 sub_08044224(int player, u16 number, int arg);

typedef char target_player_size[sizeof(struct TargetPlayer) == 0xD64 ? 1 : -1];
typedef char target_list_count[(u32)&((struct TargetList *)0)->count == 0x30C ? 1 : -1];
typedef char target_list_areas[(u32)&((struct TargetList *)0)->areas == 0x20C ? 1 : -1];

#define TARGET_ID(word) (((u32)(word) << 20) >> 20)
#define TARGET_STATS(id) (((volatile const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline int TargetType(u16 id)
{
    return (TARGET_STATS(id) & 0x1F00000) >> 20;
}
#define TARGET_TYPE(id) ((u32)TargetType(id))
static inline u16 TargetNumber(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define TARGET_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define PP ((struct TargetPlayer *)(b + off))
#define PPL (&gUnk_020192E4[player & 1])
#define CASE_LOCALS struct TargetPlayer *p; u32 *word; u32 id; int cardNo; u32 type; u32 allowed; u32 attribute; int pidx; u32 off; u8 *b; u32 *g;
#define ADD_TARGET(word, area) do { \
    gUnk_0201D810.cards[gUnk_0201D810.count] = (word); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
} while (0)

static inline u16 TargetAttack(u32 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((TARGET_STATS(id) << 14) >> 23) * 10;
}

static inline u32 TargetDefense(u16 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return (TARGET_STATS(id) & 0x1FF) * 10;
}

static inline s8 TargetLevel(u16 id)
{
    u32 type = ((TARGET_STATS(id) & 0x1F00000) >> 20);
    switch ((s32)type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    }
    return (TARGET_STATS(id) & 0x1E000000) >> 25;
}

static inline s8 TargetKind(u16 id)
{
    u16 number = TARGET_NUMBER(id);
    switch (number) {
    case 0x776: return 3;
    case 0x777:
    case 0x778: return 1;
    }
    switch (((TARGET_STATS(id) & 0x1F00000) >> 20)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    }
    return (TARGET_STATS(id) & 0xC0000) >> 18;
}

static inline u32 ReadTargetDeckWord(int player, int index)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(index * 4 + off + (u32)gUnk_02019AA8);
}
static inline u32 CopyTargetDeckWord(int player, int index)
{
    u32 off = (player & 1) * 0xD64;
    return *(u32 *)(index * 4 + off + (u32)gUnk_020192E4 + 0x7C4);
}
/* Populate the list-view overlay with targets for a card/effect number. */
#if 0 /* NONMATCHING (score 5939): unpinned i + one empty r4-r7 clobber (FAKEMATCH) puts i in r8; bitfield
       * TargetCard view; GCSE/loop/regalloc still differ; see build/wf/sub_08044224/NOTES.md */
struct TargetCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:4;
    u32 flag17:1;
    u32 unk18:2;
    s32 flag20:1;
    u32 flag21:1;
    u32 flag22:1;
    u32 unk23:9;
};
#define CARDP(p) ((struct TargetCard *)(p))
struct TargetPlayerS {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 otherCount;
    u8 unk7[0x684 - 7];
    struct TargetCard hand[80];
    struct TargetCard deck[80];
    struct TargetCard graveyard[80];
    struct TargetCard fusionDeck[80];
    struct TargetCard otherCards[80];
    u16 otherKinds[80];
};
#define PS ((struct TargetPlayerS *)(b + off))
struct TargetPlayerListS {
    struct TargetCard cards[80];
    u8 rest[0xD64 - 80 * 4];
};
#define GRAVE2(pl) ((struct TargetPlayerListS *)gUnk_02019BE8)[(pl) & 1].cards
#define PLS(pl) ((struct TargetPlayerS *)gUnk_020192E4)[(pl) & 1]
#define ADD_TARGET_S(card, area) do { \
    ((struct TargetCard *)gUnk_0201D810.cards)[gUnk_0201D810.count] = (card); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
} while (0)
struct TargetCards {
    u32 cards[128];
    u16 areas[128];
    u16 count;
};
extern struct TargetCards gUnk_0201D81C;
u16 sub_08044224(int player, u16 number, int arg)
{
    u16 materials[4];
    int forceFilter = 0;
    int skipFilter = 0;
    int flag;
    int i;
    int j;
    int pidx;
    struct TargetPlayer *p;
    u32 *word;
    u32 id;
    int cardNo;
    u32 type;
    u32 allowed;
    u32 attribute;
    u16 fieldCount;

    gUnk_0201D810.count = 0;
    switch (number) {
    case 0x2F:
    {
        CASE_LOCALS
        u32 w;
        i = 0;
        asm volatile("" ::: "r4", "r5", "r6", "r7");
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        p = (struct TargetPlayer *)(b + off);
        for (; i < p->deckCount; off += 4, i++) {
            w = *(u32 *)((u8 *)gUnk_02019AA8 + off);
            if (TARGET_TYPE(TARGET_ID(w)) <= 20 && TargetAttack(TARGET_ID(w)) <= 1500)
                ADD_TARGET(*(u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + off), 2);
        }
        break;
    }
    case 0x65:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < ((struct TargetPlayer *)(b + off))->graveCount) {
            g = (u32 *)(b + 0x904);
            do {
                u32 *t = (u32 *)((u8 *)g + off);
                word = t + i;
                if (TARGET_TYPE(TARGET_ID(*word)) == 21)
                    ADD_TARGET(*word, 4);
                i++;
            } while (i < ((struct TargetPlayer *)(b + off))->graveCount);
        }
        break;
    }
    case 0x13D:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->handCount) {
            g = (u32 *)(b + 0x684);
            do {
                word = (u32 *)((u8 *)g + (off + i * 4));
                cardNo = TARGET_NUMBER(TARGET_ID(*word));
                switch (cardNo) {
                case 0x3D:
                case 0x3E:
                case 0x4E1:
                    ADD_TARGET(*word, 1);
                    break;
                }
                i++;
            } while (i < PPL->handCount);
        }
        i = 0;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            do {
                word = (u32 *)((u8 *)gUnk_02019AA8 + (off + i * 4));
                cardNo = TARGET_NUMBER(TARGET_ID(*word));
                switch (cardNo) {
                case 0x3D:
                case 0x3E:
                case 0x4E1:
                    ADD_TARGET(*word, 2);
                    break;
                }
                i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0xF:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->handCount) {
            g = (u32 *)(b + 0x684);
            word = (u32 *)((u8 *)g + off);
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x4B2)
                ADD_TARGET(*word, 1);
        
                word++; i++;
            } while (i < PPL->handCount);
        }
        i = 0;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x4B2)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x191:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < ((struct TargetPlayer *)(b + off))->graveCount) {
            do {
                word = (u32 *)((u8 *)gUnk_02019BE8 + (off + i * 4));
                cardNo = TARGET_NUMBER(TARGET_ID(*word));
                if (cardNo == 0x3EB || cardNo == 0x40A)
                    ADD_TARGET(*word, 4);
                i++;
            } while (i < gUnk_020192E4[player & 1].graveCount);
        }
        break;
    }
    case 0x1A3:
    case 0x1F9:
    case 0x5E8:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < ((struct TargetPlayer *)(b + off))->fusionCount) {
            g = (u32 *)(b + 0xA44);
            do {
                gUnk_0201D810.cards[gUnk_0201D810.count] = *(u32 *)((u8 *)g + off);
                gUnk_0201D810.count++;
                off += 4;
                i++;
            } while (i < gUnk_020192E4[player & 1].fusionCount);
        }
        break;
    }
    case 0x1A8:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == number)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x1AB:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x23D:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetDefense(id) <= 1500)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    }
    case 0x3B1:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) > 20)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x3C6:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        /* The tested card is in hand, but the copied word is the deck slot. */
        for (; i < PP->handCount; i++) {
            id = TARGET_ID(PP->hand[i]);
            if (id != 0) {
                type = TARGET_TYPE(id);
                if (type == 1)
                    ADD_TARGET(CopyTargetDeckWord(player, i), type);
            }
        }
        break;
    }
    case 0x3EB:
    case 0x40A:
    case 0x60B:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->fusionCount) {
            g = (u32 *)(b + 0xA44);
            word = (u32 *)((u8 *)g + off);
            do {
            if (sub_0803D0E8(player, TARGET_ID(*word), materials) != 0)
                ADD_TARGET(*word, 8);
        
                word++; i++;
            } while (i < PPL->fusionCount);
        }
        break;
    }
    case 0x3F0:
    {
        CASE_LOCALS
        pidx = 0;
        do {
            i = 0;
            for (; i < gUnk_020192E4[pidx & 1].graveCount; i++) {
                word = &gUnk_020192E4[pidx & 1].graveyard[i];
                if (TARGET_TYPE(*word & 0x7FF) <= 20
                    && (u16)sub_0804412C(pidx, i) != 0)
                    ADD_TARGET(*word, 4);
            }
            pidx++;
        } while (pidx <= 1);
        break;
    }
    case 0x3F3:
    {
        CASE_LOCALS
        skipFilter = 1;
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = ((1 - player) & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PP->graveCount);
        }
        break;
    }
    case 0x3FA:
    {
        CASE_LOCALS
        skipFilter = 1;
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = ((1 - player) & 1) * 0xD64;
        for (; i < PP->deckCount && i <= 4; i++)
            ADD_TARGET(CopyTargetDeckWord(1-player, i), 2);
        break;
    }
    case 0x400:
    {
        CASE_LOCALS
        skipFilter = 1;
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            do {
                ADD_TARGET(*(u32 *)((u8 *)g + off), 4);
                off += 4;
                i++;
            } while (i < PPL->graveCount);
        }
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = ((1 - player) & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            do {
                ADD_TARGET(*(u32 *)((u8 *)g + off), 4);
                off += 4;
                i++;
            } while (i < gUnk_020192E4[(1 - player) & 1].graveCount);
        }
        break;
    }
    case 0x410:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x3EB || cardNo == 0x40A)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x41E:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetAttack(id) <= 1500
                && sub_08007834(id) == 0)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        forceFilter = 1;
        break;
    }
    case 0x439:
    {
        CASE_LOCALS
        skipFilter = 1;
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++)
            ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        break;
    }
    case 0x443:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = ((1 - player) & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PP->graveCount);
        }
        break;
    }
    case 0x454:
    case 0x456:
    case 0x45D:
    case 0x45F:
    case 0x460:
    case 0x463:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) > 20 || TargetAttack(id) > 1500)
                continue;
            allowed = 0;
            switch (number) {
            case 0x454:
                allowed = (TARGET_STATS(id) >> 29) == 5;
                break;
            case 0x456:
                allowed = (TARGET_STATS(id) >> 29) == 4;
                break;
            case 0x45D:
                allowed = (TARGET_STATS(id) >> 29) == 1;
                break;
            case 0x45F:
                allowed = (TARGET_STATS(id) >> 29) == 3;
                break;
            case 0x460:
                allowed = (TARGET_STATS(id) >> 29) == 6;
                break;
            case 0x463:
                allowed = (TARGET_STATS(id) >> 29) == 2;
                break;
            }
            if (TargetKind(id) == 3 || TargetKind(id) == 2)
                allowed = 0;
            switch (TARGET_NUMBER(id)) {
            case 0x37:
            case 0x38:
            case 0x3E:
            case 0x42:
            case 0x170:
            case 0x175:
            case 0x187:
            case 0x2E5:
            case 0x34D:
                allowed = 0;
                break;
            }
            if (allowed != 0)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    }
    case 0x455:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetKind(id) == 3)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    }
    case 0x45A:
    case 0x45B:
    case 0x51B:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == number)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x462:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            id = TARGET_ID(*word);
            if (TARGET_TYPE(id) == 22) {
                type = TARGET_TYPE(id);
                if ((s32)type <= 22 && (s32)type >= 21)
                    allowed = (TARGET_STATS(id) & 0xE0000) >> 17;
                else
                    allowed = 0;
                if (allowed == 6)
                    ADD_TARGET(*word, 2);
            }
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x447:
    case 0x487:
    case 0x488:
    case 0x5F0:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->graveCount; i++) {
            word = &PP->graveyard[i];
            if (TARGET_TYPE(*word & 0x7FF) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGET(*word, 4);
        }
        break;
    }
    case 0x45C:
    {
        CASE_LOCALS
        for (i = 0; i < gUnk_020192E4[player & 1].graveCount; i++) {
            word = &gUnk_020192E4[player & 1].graveyard[i];
            if (TARGET_TYPE(*word & 0x7FF) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGET(*word, 4);
        }
        if (player == arg) {
            flag = 1;
            i = 0;
            if (i < gUnk_0201D810.count) {
                do {
                    if (TARGET_NUMBER(TARGET_ID(gUnk_0201D81C.cards[i])) == 0x45C) {
                        flag = 0;
                        gUnk_0201D810.count--;
                        for (j = i; j < gUnk_0201D810.count; j++) {
                            sub_08007558(&gUnk_0201D81C.cards[j], &gUnk_0201D81C.cards[j + 1]);
                            gUnk_0201D81C.areas[j] = gUnk_0201D81C.areas[j + 1];
                        }
                    }
                    i++;
                } while (i < gUnk_0201D81C.count && flag);
            }
        }
        break;
    }
    case 0x47B:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (i = 0; i < gUnk_020192E4[player & 1].graveCount; i++) {
            id = TARGET_ID(gUnk_02019BE8[player & 1].cards[i]);
            if (TARGET_TYPE(id) <= 20 && TargetAttack(id) <= 1500
                && sub_08007730(id) == 0)
                ADD_TARGET(gUnk_020192E4[player & 1].graveyard[i], 4);
        }
        break;
    }
    case 0x4B2:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x4BC:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            switch (TARGET_NUMBER(TARGET_ID(*word))) {
            case 0x22:
            case 0x4BA:
            case 0x7F2:
                ADD_TARGET(*word, 2);
                break;
            }
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x4D8:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->deckCount) {
            g = (u32 *)(b + 0x7C4);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x2EA)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < PPL->deckCount);
        }
        break;
    }
    case 0x4D9:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x2EA || cardNo == 0x4D8)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x526:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) == 10 && TargetLevel(id) == arg
                && sub_08007834(id) == 0)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    }
    case 0x58D:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20 && (*word & (1 << 21)))
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x59F:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22 && (*word & (1 << 22)))
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x5E7:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = ((1 - player) & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PP->graveCount);
        }
        break;
    }
    case 0x5E9:
    {
        CASE_LOCALS
        skipFilter = 1;
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x5EA:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 3)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x5EB:
    case 0x5EC:
    case 0x5ED:
    case 0x5EE:
    case 0x5EF:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->graveCount; i++) {
            attribute = 0;
            switch (number) {
            case 0x5EB:
                attribute = 1;
                break;
            case 0x5EC:
                attribute = 4;
                break;
            case 0x5ED:
                attribute = 3;
                break;
            case 0x5EE:
                attribute = 5;
                break;
            case 0x5EF:
                attribute = 6;
                break;
            }
            id = TARGET_ID(gUnk_02019BE8[player & 1].cards[i]);
            if (TARGET_TYPE(id) <= 20 && (TARGET_STATS(id) >> 29) == attribute)
                ADD_TARGET(PP->graveyard[i], 4);
        }
        break;
    }
    case 0x5F4:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (; i < PP->graveCount; i++) {
            if (PS->graveyard[i].flag20)
                ADD_TARGET(gUnk_020192E4[player & 1].graveyard[i], 4);
        }
        break;
    }
    case 0x5FC:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->graveCount) {
            g = (u32 *)(b + 0x904);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < PPL->graveCount);
        }
        break;
    }
    case 0x60A:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        for (i = 0; i < PLS(player).graveCount; i++) {
            if (sub_0803CDEC((u16)arg, GRAVE2(player)[i].id) != 0
                && PLS(player).graveyard[i].flag20)
                ADD_TARGET_S(GRAVE2(player)[i], 4);
        }
        break;
    }
    case 0x60D:
    {
        CASE_LOCALS
        i = 0;
        b = (u8 *)gUnk_020192E4;
        off = (player & 1) * 0xD64;
        if (i < PP->otherCount) {
            g = (u32 *)(b + 0xB84);
            word = (u32 *)((u8 *)g + off);
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20
                && *(u8 *)&PP->otherKinds[i] != 2)
                ADD_TARGET(*word, 0x10);
        
                word++; i++;
            } while (i < PPL->otherCount);
        }
        break;
    }
    }
    if (gUnk_0201D810.count == 0)
        return 0;

    fieldCount = sub_08008668(player);
    if ((forceFilter || fieldCount == 0) && !skipFilter) {
        for (i = 0; i < gUnk_0201D810.count;) {
            if (sub_0800756C(TARGET_NUMBER(TARGET_ID(gUnk_0201D810.cards[i]))) != 0) {
                /* Retain the ROM's final one-past-count copy and untouched area tags. */
                for (j = i; j < gUnk_0201D810.count; j++)
                    gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
                gUnk_0201D810.count--;
            } else {
                i++;
            }
        }
    }
    for (i = 0; i < gUnk_0201D810.count;) {
        if ((s32)(gUnk_0201D810.cards[i] << 14) < 0) {
            for (j = i; j < gUnk_0201D810.count; j++)
                gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
            gUnk_0201D810.count--;
        } else {
            i++;
        }
    }
    for (i = 0; i < gUnk_0201D810.count;) {
        if (sub_0800966C(TARGET_ID(gUnk_0201D810.cards[i])) != 0) {
            for (j = i; j < gUnk_0201D810.count; j++)
                gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
            gUnk_0201D810.count--;
        } else {
            i++;
        }
    }
    return gUnk_0201D810.count;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08044224", sub_08044224); /* 0x08044224 size 0x2514 */

