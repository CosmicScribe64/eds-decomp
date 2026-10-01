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
extern u32 gUnk_0201D81C[128];
extern u16 gUnk_0201DA1C[128];
extern u16 gUnk_0201DB1C;
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
#define TARGET_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
static inline int TargetType(u32 id)
{
    return (TARGET_STATS(id) & 0x1F00000) >> 20;
}
#define TARGET_TYPE(id) ((u32)TargetType(id))
static inline u16 TargetNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}
#define TARGET_NUMBER(id) TargetNumber(id)
#define ADD_TARGET(word, area) do { \
    gUnk_0201D810.cards[gUnk_0201D810.count] = (word); \
    gUnk_0201D810.areas[gUnk_0201D810.count] = (area); \
    gUnk_0201D810.count++; \
} while (0)

static inline u32 TargetAttack(u16 id)
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
#if 0 /* NONMATCHING: guard-typed32-wordfirst base; pointer formation, 0x7FF hoisting and global regalloc differ (see build/fable/sub_08044224/NOTES.md) */
u16 sub_08044224(int player, u16 number, int arg)
{
    u16 materials[4];
    int forceFilter = 0;
    int skipFilter = 0;
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
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetAttack(id) <= 1500)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    case 0x65:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 21)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x13D:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->handCount; i++) {
            cardNo = TARGET_NUMBER(TARGET_ID(p->hand[i]));
            if (cardNo >= 0x3D && (cardNo <= 0x3E || cardNo == 0x4E1))
                ADD_TARGET(p->hand[i], 1);
        }
        i = 0;
        for (; i < p->deckCount; i++) {
            cardNo = TARGET_NUMBER(TARGET_ID(p->deck[i]));
            if (cardNo >= 0x3D && (cardNo <= 0x3E || cardNo == 0x4E1))
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    case 0xF:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->handCount) {
            word = p->hand;
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x4B2)
                ADD_TARGET(*word, 1);
        
                word++; i++;
            } while (i < p->handCount);
        }
        i = 0;
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x4B2)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x191:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x3EB || cardNo == 0x40A)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x1A3:
    case 0x1F9:
    case 0x5E8:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        /* The ROM does not write area tags for this group. */
        for (; i < p->fusionCount; i++) {
            gUnk_0201D81C[gUnk_0201DB1C] = p->fusionDeck[i];
            gUnk_0201DB1C++;
        }
        break;
    case 0x1A8:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == number)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x1AB:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x23D:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetDefense(id) <= 1500)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    case 0x3B1:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) > 20)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x3C6:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        /* The tested card is in hand, but the copied word is the deck slot. */
        for (; i < p->handCount; i++) {
            id = TARGET_ID(p->hand[i]);
            if (id != 0) {
                type = TARGET_TYPE(id);
                if (type == 1)
                    ADD_TARGET(CopyTargetDeckWord(player, i), type);
            }
        }
        break;
    case 0x3EB:
    case 0x40A:
    case 0x60B:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->fusionCount) {
            word = p->fusionDeck;
            do {
            if (sub_0803D0E8(player, TARGET_ID(*word), materials) != 0)
                ADD_TARGET(*word, 8);
        
                word++; i++;
            } while (i < p->fusionCount);
        }
        break;
    case 0x3F0:
        pidx = 0;
        do {
            i = 0;
            p = &gUnk_020192E4[pidx & 1];
            for (; i < p->graveCount; i++) {
                word = &p->graveyard[i];
                if (TARGET_TYPE(*word & 0x7FF) <= 20
                    && (u16)sub_0804412C(pidx, i) != 0)
                    ADD_TARGET(*word, 4);
            }
            pidx++;
        } while (pidx <= 1);
        break;
    case 0x3F3:
        skipFilter = 1;
        i = 0;
        p = &gUnk_020192E4[(1 - player) & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x3FA:
        skipFilter = 1;
        i = 0;
        p = &gUnk_020192E4[(1 - player) & 1];
        for (; i < p->deckCount && i <= 4; i++)
            ADD_TARGET(CopyTargetDeckWord(1-player, i), 2);
        break;
    case 0x400:
        skipFilter = 1;
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++)
            ADD_TARGET(p->graveyard[i], 4);
        i = 0;
        p = &gUnk_020192E4[(1 - player) & 1];
        for (; i < p->graveCount; i++)
            ADD_TARGET(p->graveyard[i], 4);
        break;
    case 0x410:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x3EB || cardNo == 0x40A)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x41E:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetAttack(id) <= 1500
                && sub_08007834(id) == 0)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        forceFilter = 1;
        break;
    case 0x439:
        skipFilter = 1;
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++)
            ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        break;
    case 0x443:
        i = 0;
        p = &gUnk_020192E4[(1 - player) & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x454:
    case 0x456:
    case 0x45D:
    case 0x45F:
    case 0x460:
    case 0x463:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
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
    case 0x455:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) <= 20 && TargetKind(id) == 3)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    case 0x45A:
    case 0x45B:
    case 0x51B:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == number)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x462:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
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
            } while (i < p->deckCount);
        }
        break;
    case 0x447:
    case 0x487:
    case 0x488:
    case 0x5F0:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
            word = &p->graveyard[i];
            if (TARGET_TYPE(*word & 0x7FF) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGET(*word, 4);
        }
        break;
    case 0x45C:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
            word = &p->graveyard[i];
            if (TARGET_TYPE(*word & 0x7FF) <= 20
                && (u16)sub_0804412C(player, i) != 0)
                ADD_TARGET(*word, 4);
        }
        if (player == arg) {
            i = 0;
            while (i < gUnk_0201D810.count) {
                if (TARGET_NUMBER(gUnk_0201D81C[i]) == 0x45C) {
                    gUnk_0201D810.count--;
                    for (j = i; j < gUnk_0201D810.count; j++) {
                        sub_08007558(&gUnk_0201D81C[j], &gUnk_0201D81C[j + 1]);
                        gUnk_0201DA1C[j] = gUnk_0201DA1C[j + 1];
                    }
                    break;
                }
                i++;
            }
        }
        break;
    case 0x47B:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
            id = TARGET_ID(gUnk_02019BE8[player & 1].cards[i]);
            if (TARGET_TYPE(id) <= 20 && TargetAttack(id) <= 1500
                && sub_08007730(id) == 0)
                ADD_TARGET(p->graveyard[i], 4);
        }
        break;
    case 0x4B2:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x4BC:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            switch (TARGET_NUMBER(TARGET_ID(*word))) {
            case 0x22:
            case 0x4BA:
            case 0x7F2:
                ADD_TARGET(*word, 2);
                break;
            }
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x4D8:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->deckCount) {
            word = p->deck;
            do {
            if (TARGET_NUMBER(TARGET_ID(*word)) == 0x2EA)
                ADD_TARGET(*word, 2);
        
                word++; i++;
            } while (i < p->deckCount);
        }
        break;
    case 0x4D9:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            cardNo = TARGET_NUMBER(TARGET_ID(*word));
            if (cardNo == 0x2EA || cardNo == 0x4D8)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x526:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->deckCount; i++) {
            id = TARGET_ID(ReadTargetDeckWord(player, i));
            if (TARGET_TYPE(id) == 10 && TargetLevel(id) == arg
                && sub_08007834(id) == 0)
                ADD_TARGET(CopyTargetDeckWord(player, i), 2);
        }
        break;
    case 0x58D:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20 && (*word & (1 << 21)))
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x59F:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 22 && (*word & (1 << 22)))
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x5E7:
        i = 0;
        p = &gUnk_020192E4[(1 - player) & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x5E9:
        skipFilter = 1;
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x5EA:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) == 3)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x5EB:
    case 0x5EC:
    case 0x5ED:
    case 0x5EE:
    case 0x5EF:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
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
                ADD_TARGET(p->graveyard[i], 4);
        }
        break;
    case 0x5F4:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
            if (p->graveyard[i] & (1 << 20))
                ADD_TARGET(p->graveyard[i], 4);
        }
        break;
    case 0x5FC:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->graveCount) {
            word = p->graveyard;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20)
                ADD_TARGET(*word, 4);
        
                word++; i++;
            } while (i < p->graveCount);
        }
        break;
    case 0x60A:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        for (; i < p->graveCount; i++) {
            word = &p->graveyard[i];
            if (sub_0803CDEC((u16)arg, TARGET_ID(*word)) != 0
                && (*word & (1 << 20)))
                ADD_TARGET(*word, 4);
        }
        break;
    case 0x60D:
        i = 0;
        p = &gUnk_020192E4[player & 1];
        if (i < p->otherCount) {
            word = p->otherCards;
            do {
            if (TARGET_TYPE(TARGET_ID(*word)) <= 20
                && *(u8 *)&p->otherKinds[i] != 2)
                ADD_TARGET(*word, 0x10);
        
                word++; i++;
            } while (i < p->otherCount);
        }
        break;
    }
    if (gUnk_0201D810.count == 0)
        return 0;

    fieldCount = sub_08008668(player);
    if ((forceFilter || fieldCount == 0) && !skipFilter) {
        i = 0;
        while (i < gUnk_0201D810.count) {
            if (sub_0800756C(TARGET_NUMBER(gUnk_0201D810.cards[i])) != 0) {
                /* Retain the ROM's final one-past-count copy and untouched area tags. */
                for (j = i; j < gUnk_0201D810.count; j++)
                    gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
                gUnk_0201D810.count--;
            } else {
                i++;
            }
        }
    }
    i = 0;
    while (i < gUnk_0201D810.count) {
        if (gUnk_0201D810.cards[i] & (1 << 17)) {
            for (j = i; j < gUnk_0201D810.count; j++)
                gUnk_0201D810.cards[j] = gUnk_0201D810.cards[j + 1];
            gUnk_0201D810.count--;
        } else {
            i++;
        }
    }
    i = 0;
    while (i < gUnk_0201D810.count) {
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

