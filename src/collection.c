#include "global.h"
#include "gba.h"

/*
 * Game-side sound API, card-collection bookkeeping and OAM/copy helpers.
 * See wiki/functions/code-0807717c.md
 */


/* Per-card collection record, 4 bytes at save+8 + id*4. */
struct CardCount {
    u16 count : 10;                 /* copies owned (max 0x3FF) */
    u16 rest : 6;
    u16 unkA;
};
struct CardBits {                   /* byte view of +1 of the record */
    u8 unk0;
    u8 pad : 2;
    u8 n1 : 2;                      /* 0..3 counters, capped by GetCardCopyLimit(id) */
    u8 n2 : 2;
    u8 n3 : 2;
    u16 unkA;
};
union CardEntry {
    struct CardCount c;
    struct CardBits b;
};

/* Second per-card record (4 bytes at save+0x20D0 + id*4), three counters seen through different views. */
struct Card2B { u32 lo : 11; u32 b : 11; u32 c : 10; };
struct Card2C { u16 lo; u16 pad : 6; u16 c : 10; };
struct Card2A { u16 a : 11; u16 rest : 5; u16 hi; };
struct Card2Rec {
    u8 pad0[0x20D0];
    struct Card2A a;
};
struct Card2RecB {
    u8 pad0[0x20D0];
    struct Card2B b;
};
struct Card2RecC {
    u8 pad0[0x20D0];
    struct Card2C c;
};

struct Save {
    u8 pad0[8];
    union CardEntry cards[0x800];   /* +0x0008 */
    u16 list1[0x3C];                /* +0x2008 */
    u16 list2[0xF];                 /* +0x2080 */
    u16 list3[0x14];                /* +0x209E .. */
    u16 total;                      /* +0x20C6 */
    u16 n1;                         /* +0x20C8 */
    u16 n2;                         /* +0x20CA */
    u16 n3;                         /* +0x20CC */
    u8 pad20CE[2];
    u32 words[0x20];                /* +0x20D0 */
    u8 pad2150[2];
    u16 options;                    /* +0x2152 bit0 SE on, bit1 BGM on */
    u8 pad2154[4];
    u16 lastId;                     /* +0x2158 last id touched by 948/998/9E8 */
    u8 pad215A[8];
    u8 unk2162;
};
extern struct Save gSaveData;
/* Card record viewed from the save base: the record for `id` sits at base + id*4 + 8. */
struct CardRec {
    u8 pad0[8];
    union CardEntry e;
};
#define CARD_REC(id) ((struct CardRec *)((u8 *)&gSaveData + (id) * 4))


struct Main {
    u8 pad0[0x485C];
    u16 currentBgm;                 /* +0x485C */
    u16 frameCounter;               /* +0x485E */
    u8 pad4860[8];
    u16 lastSeFrame;                /* +0x4868 */
};
extern struct Main gMain;

struct RarityRow { u16 key; u16 value; };
extern const u16 gCardIdToNumber[];
extern const struct RarityRow gCardCopyLimits[];
extern u8 gStrErrorIdFmt[];
s32 GetCardCopyLimit(u32 id);
void RemoveCardFromSavedSideDeck(u16 id);
void AddCardToTrunk(u16 id);
void RemoveCardFromSavedDeck(u16 id);
extern const u16 gCardNumberToId[], gUnk_08623E10[], gUnk_08623E38[], gUnk_08623E3E[], gUnk_08623E44[];
extern const u16 gUnk_08623E6E[], gUnk_08623E72[], gUnk_08623E7C[], gUnk_086240FA[], gUnk_086240FE[];
extern const u16 gUnk_086245CA[], gUnk_08624768[], gUnk_086247B6[];
extern void DebugPrintf(const void *, u32);
extern void DebugPrintFlush(void);

u16 *OamListAlloc(u8 idx, void *work);
extern void SoundRequestSE(u32);
extern void SoundRequestBGM(u32);
extern void SoundFadeOutBGM(u32);
extern void SoundStopBGM(void);
extern void SoundStopAllSE(void);
u16 IsSeEnabled(void);
u16 IsBgmEnabled(void);

/* BEGIN GetCardCopyLimit */
/* Look up the card's key in gCardIdToNumber, then find its row in the 0x2F-entry table gCardCopyLimits. */
s32 GetCardCopyLimit(u32 id) {
    u16 key = *(const u16 *)((const u8 *)gCardIdToNumber + ((id << 21) >> 20));
    const struct RarityRow *p;
    u32 i;
    for (i = 0, p = gCardCopyLimits; i <= 0x2E; p++, i++) {
        if (key == p->key)
            return p->value;
    }
    return 3;
}
/* END GetCardCopyLimit */
/* BEGIN IsBelowCardCopyLimit */
/* Returns 1 if the player owns fewer copies of card `id` (plus its related cards) than the limit. */
#define CR(x) ((struct CardRec *)((u8 *)s + (x) * 4))
#define ADD_PAIR(x) do { sum += CR(x)->e.b.n1; sum += CR(x)->e.b.n2; } while (0)
#define ADD_N3(x) sum += CR(x)->e.b.n3
u32 IsBelowCardCopyLimit(u16 id) {
    struct Save *s = &gSaveData;
    struct CardRec *r = CR(id);
    s32 sum = r->e.b.n1 + r->e.b.n2 + r->e.b.n3;
    s32 limit = GetCardCopyLimit(id);
    u16 key = *(const u16 *)((const u8 *)gCardIdToNumber + ((id & 0x7FF) << 1));
    switch (key) {
    case 0x3EB:
        ADD_PAIR(*(const u16 *)(((key + 0x1F) << 1) + (u32)((const u16 *)0x08623DF4)));
        break;
    case 0x40A:
        ADD_PAIR(gUnk_086245CA[0]);
        break;
    case 0x22:
        ADD_PAIR(gUnk_08624768[0]);
        ADD_PAIR((u16)(gUnk_08623E38[0] + 1));
        break;
    case 0x4BA:
        ADD_PAIR(gUnk_08623E38[0]);
        ADD_PAIR(*(const u16 *)((key << 1) + (u32)((const u16 *)0x08623DF4)));
        break;
    case 0x7F2: {
        /* FAKEMATCH: keep the initialized related ID separate from the cached count byte. */
        register u16 relatedId asm("r1") = gUnk_08623E38[0];
        u8 flags = *((u8 *)s + relatedId * 4 + 9);
        sum += ((u32)flags << 28) >> 30;
        sum += ((u32)flags << 26) >> 30;
        ADD_PAIR((u16)(relatedId + 1));
        break;
    }
    case 0x3D:
        ADD_PAIR(gUnk_086247B6[0]);
        break;
    case 0x4E1:
        ADD_PAIR(gUnk_08623E6E[0]);
        break;
    case 0:
        ADD_PAIR((u16)(((const u16 *)0x08623DF4)[0] + 1));
        break;
    case 0xE:
        ADD_N3((u16)(gUnk_08623E10[0] + 1));
        break;
    case 0x25:
        ADD_PAIR((u16)(gUnk_08623E3E[0] + 1));
        break;
    case 0x28:
        ADD_PAIR((u16)(gUnk_08623E44[0] + 1));
        break;
    case 0x3F:
        ADD_PAIR((u16)(gUnk_08623E72[0] + 1));
        break;
    case 0x44:
        ADD_N3((u16)(gUnk_08623E7C[0] + 1));
        break;
    case 0x183:
        ADD_PAIR((u16)(gUnk_086240FA[0] + 1));
        break;
    case 0x185:
        ADD_PAIR((u16)(gUnk_086240FE[0] + 1));
        break;
    case 0x7D0:
        ADD_PAIR(((const u16 *)0x08623DF4)[0]);
        break;
    case 0x7DE:
        ADD_N3(gUnk_08623E10[0]);
        break;
    case 0x7F5:
        ADD_PAIR(gUnk_08623E3E[0]);
        break;
    case 0x7F8:
        ADD_PAIR(gUnk_08623E44[0]);
        break;
    case 0x80F:
        ADD_PAIR(gUnk_08623E72[0]);
        break;
    case 0x814:
        ADD_N3(gUnk_08623E7C[0]);
        break;
    case 0x953:
        ADD_PAIR(gUnk_086240FA[0]);
        break;
    case 0x955:
        ADD_PAIR(gUnk_086240FE[0]);
        break;
    }
    return sum < limit;
}

/* END IsBelowCardCopyLimit */
/* BEGIN DebugCheckCardId */
/* Debug assert: prints gStrErrorIdFmt (format string) with the id, then halts, when id is out of range. */
void DebugCheckCardId(u16 id) {
    if ((u16)(id - 0x76D) <= 0x62 || id == 0) {
        DebugPrintf(gStrErrorIdFmt, id);
        DebugPrintFlush();
    }
}
/* END DebugCheckCardId */
/* BEGIN AddCardToTrunk */
void AddCardToTrunk(u16 id) {
    struct Save *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.c.count < 0x3FF) {
        r->e.c.count++;
        s->total++;
    }
}
/* END AddCardToTrunk */
/* BEGIN AddCardToSavedDeck */
void AddCardToSavedDeck(u16 id) {
    struct Save *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.n1 < GetCardCopyLimit(id)) {
        if (s->n1 < 0x3C) {
            r->e.b.n1++;
            s->list1[s->n1++] = id;
        }
    }
}
/* END AddCardToSavedDeck */
/* BEGIN AddCardToSavedSideDeck */
void AddCardToSavedSideDeck(u16 id) {
    struct Save *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.n2 < GetCardCopyLimit(id)) {
        if (s->n2 < 0xF) {
            r->e.b.n2++;
            s->list2[s->n2++] = id;
        }
    }
}
/* END AddCardToSavedSideDeck */
/* BEGIN AddCardToSavedFusionDeck */
void AddCardToSavedFusionDeck(u16 id) {
    struct Save *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.n3 < GetCardCopyLimit(id)) {
        if (s->n3 < 0x14) {
            r->e.b.n3++;
            s->list3[s->n3++] = id;
        }
    }
}
/* END AddCardToSavedFusionDeck */
/* BEGIN RemoveCardFromTrunk */
void RemoveCardFromTrunk(u16 id) {
    struct Save *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if ((r->e.c.count << 22) != 0) {
        r->e.c.count--;
        s->total--;
    }
}
/* END RemoveCardFromTrunk */
/* BEGIN RemoveCardFromSavedDeck */
/* Removes one n1/list1 copy and compacts its list.
 * FAKEMATCH: initialized register constraints reproduce byte caching and
 * pointer copies. The bits/flags input with r1 clobber prevents deriving the
 * negative byte mask from the earlier 3. No instruction is emitted. */
void RemoveCardFromSavedDeck(u16 id)
{
    struct Save *s;
    register struct CardRec *r __asm__("r5");
    register u32 off __asm__("r0");
    register u32 flags __asm__("r2");
    register u32 stage __asm__("r1");
    u32 n;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 28;
    n = stage >> 30;
    if (n != 0) {
        u32 bits = ((n - 1) & 3) << 2;
        s32 mask;
        int i;
        u32 countOff;
        register u16 *p __asm__("r0");
        __asm__ volatile("" : : "r"(bits), "r"(flags) : "r1");
        mask = ~12;
        *((u8 *)r + 9) = (mask & flags) | bits;
        i = 0;
        countOff = 0x20C8;
        p = (u16 *)((u32)s + countOff);
        if (i < *p) {
            u32 listOff = 0x2008;
            register u16 *list __asm__("r5") = (u16 *)((u32)s + listOff);
            u16 *pn = p;
            register u16 *savedPn __asm__("r6") = pn;
            u32 base = (u32)s;
            register u32 byteOff __asm__("r3");
            byteOff = 0;
        scan:
            if (*(u16 *)(byteOff + (u32)list) == id) {
                (*pn)--;
                if (i < *pn) {
                    register u16 *innerPn __asm__("r4") = savedPn;
                    register u32 listStart __asm__("r0") = byteOff + 0x2008;
                    register u16 *q __asm__("r1") = (u16 *)(listStart + base);
                    do {
                        q[0] = q[1];
                        q++;
                        i++;
                    } while (i < *innerPn);
                }
                goto out;
            }
            byteOff += 2;
            i++;
            if (i < *pn)
                goto scan;
        }
    }
out:
    ;
}

/* END RemoveCardFromSavedDeck */
/* Removes one n2/list2 copy and compacts its list.
 * FAKEMATCH: initialized register constraints reproduce byte caching and
 * pointer copies. The bits/flags input with r1 clobber prevents deriving the
 * negative byte mask from the earlier 3. No instruction is emitted. */
void RemoveCardFromSavedSideDeck(u16 id)
{
    struct Save *s;
    register struct CardRec *r __asm__("r5");
    register u32 off __asm__("r0");
    register u32 flags __asm__("r2");
    register u32 stage __asm__("r1");
    u32 n;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 26;
    n = stage >> 30;
    if (n != 0) {
        u32 bits = ((n - 1) & 3) << 4;
        s32 mask;
        int i;
        u32 countOff;
        register u16 *p __asm__("r0");
        __asm__ volatile("" : : "r"(bits), "r"(flags) : "r1");
        mask = ~48;
        *((u8 *)r + 9) = (mask & flags) | bits;
        i = 0;
        countOff = 0x20CA;
        p = (u16 *)((u32)s + countOff);
        if (i < *p) {
            u32 listOff = 0x2080;
            register u16 *list __asm__("r5") = (u16 *)((u32)s + listOff);
            u16 *pn = p;
            register u16 *savedPn __asm__("r6") = pn;
            u32 base = (u32)s;
            register u32 byteOff __asm__("r3");
            byteOff = 0;
        scan:
            if (*(u16 *)(byteOff + (u32)list) == id) {
                (*pn)--;
                if (i < *pn) {
                    register u16 *innerPn __asm__("r4") = savedPn;
                    register u32 listStart __asm__("r0") = byteOff + 0x2080;
                    register u16 *q __asm__("r1") = (u16 *)(listStart + base);
                    do {
                        q[0] = q[1];
                        q++;
                        i++;
                    } while (i < *innerPn);
                }
                goto out;
            }
            byteOff += 2;
            i++;
            if (i < *pn)
                goto scan;
        }
    }
out:
    ;
}

/* Removes one n3/list3 copy and compacts its list.
 * FAKEMATCH: initialized constraints retain the staged extraction, separate
 * count copy before decrement, and save-base/count-pointer allocation.
 * Ordinary allocation preserves the copied count pointer in r7. */
void RemoveCardFromSavedFusionDeck(u16 id)
{
    struct Save *s;
    struct CardRec *r;
    u32 off;
    u32 flags;
    register u32 stage __asm__("r1");
    u32 n;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 24;
    __asm__ volatile("" : "+r"(stage));
    n = stage >> 30;
    if (n != 0) {
        register u32 bits __asm__("r1");
        register u32 kept __asm__("r0");
        int i;
        u32 countOff;
        register u16 *p __asm__("r0");
        bits = n;
        __asm__ volatile("" : "+r"(bits) : "r"(n));
        bits--;
        bits <<= 6;
        kept = 0x3F & flags;
        *((u8 *)r + 9) = kept | bits;
        i = 0;
        countOff = 0x20CC;
        p = (u16 *)((u32)s + countOff);
        if (i < *p) {
            u32 listOff = 0x209E;
            register u16 *list __asm__("r6") = (u16 *)((u32)s + listOff);
            register u16 *pn __asm__("r1") = p;
            u16 *savedPn = pn;
            register u32 base __asm__("r5") = (u32)s;
            u32 byteOff;
            byteOff = 0;
        scan:
            if (*(u16 *)(byteOff + (u32)list) == id) {
                (*pn)--;
                if (i < *pn) {
                    register u16 *innerPn __asm__("r4") = savedPn;
                    register u32 listStart __asm__("r0") = byteOff + 0x209E;
                    u16 *q = (u16 *)(listStart + base);
                    do {
                        q[0] = q[1];
                        q++;
                        i++;
                    } while (i < *innerPn);
                }
                goto out;
            }
            byteOff += 2;
            i++;
            if (i < *pn)
                goto scan;
        }
    }
out:
    ;
}

/* BEGIN TrimSavedDecksToCopyLimits */
/* Trim list1 and list2 copy counts to the card-specific limit.
 * FAKEMATCH: initialized address/copy bindings, a read-only stack-pointer
 * input, and staged narrowing retain the ROM's original register lifetimes.
 * The next index stays full-width until the inner removal loop finishes. */
void TrimSavedDecksToCopyLimits(void)
{
    u32 i;
    struct Save *s;
    u32 saveBase;
    u32 countOff;
    u16 *initialCount;
    u16 *saved;
    i = 0;
    saveBase = (u32)&gSaveData;
    countOff = 0x20C8;
    initialCount = (u16 *)(saveBase + countOff);
    s = (struct Save *)saveBase;
    {
        u32 initialLength = *initialCount;
        __asm__ volatile("" : : "r"(initialLength));
        if (i != initialLength) {
            u32 base = (u32)s;
            do {
                register u32 byteOff __asm__("r1") = i * 2;
                register u32 listBase __asm__("r0") = base + 0x2008;
                {
                    register u16 *p __asm__("r3") = (u16 *)(byteOff + listBase);
                    u16 id = *p;
                    struct CardRec *r = (struct CardRec *)((u32)id * 4 + base);
                    u32 flags = *((u8 *)r + 9);
                    u8 n1 = (flags << 28) >> 30;
                    u8 n2 = (flags << 26) >> 30;
                    u8 limit;
                    u32 next;
                    int sum;
                    saved = p;
                    limit = GetCardCopyLimit(id);
                    sum = n1 + n2;
                    next = i + 1;
                    __asm__ volatile("" : : "m"(saved));
                    {
                        register u16 *loaded __asm__("r3") = saved;
                        if (sum > limit) {
                            register u16 *current __asm__("r6") = loaded;
                            do {
                                if (n2 != 0) {
                                    RemoveCardFromSavedSideDeck(*current);
                                    AddCardToTrunk(*current);
                                    n2--;
                                } else if (n1 != 0) {
                                    RemoveCardFromSavedDeck(*current);
                                    AddCardToTrunk(*current);
                                    n1--;
                                }
                            } while (n1 + n2 > limit);
                        }
                        {
                            register u32 nextCopy __asm__("r1") = next;
                            u32 stage = nextCopy << 16;
                            __asm__ volatile("" : "+r"(stage) : "r"(nextCopy));
                            i = stage >> 16;
                        }
                    }
                    {
                        register u16 *count1 __asm__("r2") = &gSaveData.n1;
                        if (i == *count1)
                        break;
                    }
                }
            } while (1);
        }
    }
    i = 0;
    if (i != s->n2) {
        u32 base = (u32)s;
        u16 *count = &s->n2;
        do {
            register u32 byteOff __asm__("r1") = i * 2;
            register u32 listBase __asm__("r0") = base + 0x2080;
            {
                register u16 *p __asm__("r3") = (u16 *)(byteOff + listBase);
                u16 id = *p;
                struct CardRec *r = (struct CardRec *)((u32)id * 4 + base);
                u32 flags = *((u8 *)r + 9);
                u8 n1 = (flags << 28) >> 30;
                u8 n2 = (flags << 26) >> 30;
                u8 limit;
                u32 next;
                int sum;
                saved = p;
                limit = GetCardCopyLimit(id);
                sum = n1 + n2;
                next = i + 1;
                __asm__ volatile("" : : "m"(saved));
                {
                    register u16 *loaded __asm__("r3") = saved;
                    if (sum > limit) {
                        u16 *current = loaded;
                        do {
                            if (n2 != 0) {
                                RemoveCardFromSavedSideDeck(*current);
                                AddCardToTrunk(*current);
                                n2--;
                            } else if (n1 != 0) {
                                RemoveCardFromSavedDeck(*current);
                                AddCardToTrunk(*current);
                                n1--;
                            }
                        } while (n1 + n2 > limit);
                    }
                    {
                        u32 nextCopy = next;
                        u32 stage = nextCopy << 16;
                        __asm__ volatile("" : "+r"(stage) : "r"(nextCopy));
                        i = stage >> 16;
                    }
                }
            }
            {
                u16 *lastCount = count;
                if (i == *lastCount)
                break;
            }
        } while (1);
    }
}

/* END TrimSavedDecksToCopyLimits */
/* BEGIN RecordDuelWin */
void RecordDuelWin(u32 id) {
    struct Save *s = &gSaveData;
    struct Card2Rec *w = (struct Card2Rec *)((u8 *)s + id * 4);
    s32 a = w->a.a;
    if (a <= 0x7FE)
        w->a.a = a + 1;
    s->lastId = id;
}
/* END RecordDuelWin */
/* BEGIN RecordDuelLoss */
void RecordDuelLoss(u32 id) {
    struct Save *s = &gSaveData;
    struct Card2RecB *w = (struct Card2RecB *)((u8 *)s + id * 4);
    s32 b = w->b.b;
    if (b <= 0x7FE)
        w->b.b = b + 1;
    s->lastId = id;
}
/* END RecordDuelLoss */
/* BEGIN RecordDuelDraw */
void RecordDuelDraw(u32 id) {
    struct Save *s = &gSaveData;
    struct Card2RecC *w = (struct Card2RecC *)((u8 *)s + id * 4);
    s32 c = w->c.c;
    if (c <= 0x3FE)
        w->c.c = c + 1;
    s->lastId = id;
}
/* END RecordDuelDraw */
/* BEGIN IncrementChampionshipWins */
void IncrementChampionshipWins(void) {
    if (gSaveData.unk2162 < 0xFF)
        gSaveData.unk2162++;
}
/* END IncrementChampionshipWins */
/* BEGIN IsSeEnabled */
u16 IsSeEnabled(void) {
    return gSaveData.options & 1;
}
/* END IsSeEnabled */
/* BEGIN IsBgmEnabled */
u16 IsBgmEnabled(void) {
    return (gSaveData.options >> 1) & 1;
}
/* END IsBgmEnabled */
/* BEGIN SetSeEnabled */
void SetSeEnabled(u16 on) {
    if (on)
        gSaveData.options |= 1;
    else
        gSaveData.options &= ~1;
}
/* END SetSeEnabled */
/* BEGIN SetBgmEnabled */
void SetBgmEnabled(u16 on) {
    if (on)
        gSaveData.options |= 2;
    else
        gSaveData.options &= ~2;
}
/* END SetBgmEnabled */
/* BEGIN PlaySE */
void PlaySE(u32 id) {
    if (IsSeEnabled()) {
        struct Main *m = &gMain;
        if (m->lastSeFrame != m->frameCounter) {
            m->lastSeFrame = m->frameCounter;
            SoundRequestSE(id);
        }
    }
}
/* END PlaySE */
/* BEGIN PlayBGM */
void PlayBGM(u32 id) {
    if (IsBgmEnabled()) {
        struct Main *m = &gMain;
        if (m->currentBgm != id) {
            SoundRequestBGM(id);
            m->currentBgm = id;
        }
    }
}
/* END PlayBGM */
/* BEGIN PlayBGMNoTrack */
void PlayBGMNoTrack(u32 id) {
    if (IsBgmEnabled())
        SoundRequestBGM(id);
}
/* END PlayBGMNoTrack */
/* BEGIN PlayJingle */
void PlayJingle(u32 id) {
    if (IsSeEnabled()) {
        struct Main *m = &gMain;
        if (m->currentBgm != id) {
            SoundRequestBGM(id);
            m->currentBgm = id;
        }
    }
}
/* END PlayJingle */
/* BEGIN StopBGM */
void StopBGM(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundStopBGM();
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
/* END StopBGM */
/* BEGIN FadeOutBGM */
void FadeOutBGM(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundFadeOutBGM(0x10);
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
/* END FadeOutBGM */
/* BEGIN FadeOutBGMAtSpeed */
void FadeOutBGMAtSpeed(u32 arg) {
    struct Main *m;
    SoundFadeOutBGM(arg);
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
/* END FadeOutBGMAtSpeed */
/* BEGIN StopAllSound */
void StopAllSound(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundStopBGM();
    if (IsSeEnabled())
        SoundStopAllSE();
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
/* END StopAllSound */
/* BEGIN OffsetNonZeroPixelsAndCopy */
/* Add `add` to every non-zero byte of the buffer (colour-index brighten), then CpuFastSet it to dst. */
void OffsetNonZeroPixelsAndCopy(u8 *src, u8 *dst, u16 n, u8 add) {
    u16 i;
    u8 j;
    u32 fill = (add << 24) | (add << 16) | (add << 8) | add;
    u16 cnt = n >> 2;
    for (i = 0; i < cnt; i++) {
        u32 *p = (u32 *)(i * 4 + (u32)src);
        u32 w = *p;
        if (*(u8 *)p != 0 && (w & 0xFF00) != 0 && (w & 0xFF0000) != 0 && (w & 0xFF000000) != 0) {
            *p = w + fill;
        } else {
            for (j = 0; j < 4; j++) {
                u8 *q = (u8 *)(i * 4 + (u32)src) + j;
                if (*q != 0)
                    *q += add;
            }
        }
    }
    CpuFastSet(src, dst, n >> 2);
}
/* END OffsetNonZeroPixelsAndCopy */
/* BEGIN CopyTileSheetTo2D */
/* Copy a tile image: mode 0x100 is one flat block, mode 0x10 copies 16 rows of
 * 0x200 bytes into a 0x400-byte stride. */
void CopyTileSheetTo2D(u8 *srcArg, u8 *dstArg, u16 mode) {
    u16 i;
    u8 *src = srcArg;
    u8 *dst = dstArg;
    if (mode != 0x10) {
        if (mode == 0x100)
            CpuSet(src, dst, 0x2000);
    } else {
        for (i = 0; i < 0x10; i++) {
            CpuSet(src, dst, 0x100);
            src += 0x200;
            dst += 0x400;
        }
    }
}
/* END CopyTileSheetTo2D */
/* BEGIN CopyTileSheetRowsTo2D */
void CopyTileSheetRowsTo2D(u8 *srcArg, u8 *dstArg, u16 mode, u8 rows) {
    u16 i;
    u8 *src = srcArg;
    u8 *dst = dstArg;
    if (mode != 0x10) {
        if (mode == 0x100)
            CpuSet(src, dst, rows << 9);
    } else {
        for (i = 0; i < rows; i++) {
            CpuSet(src, dst, 0x100);
            src += 0x200;
            dst += 0x400;
        }
    }
}
/* END CopyTileSheetRowsTo2D */
/* BEGIN CopyTileRectTo2D */
void CopyTileRectTo2D(u8 *srcArg, u8 *dstArg, u16 a, u16 b, u16 c, u16 mode) {
    u16 i;
    u8 *src = srcArg;
    u8 *dst = dstArg;
    if (mode != 0x10) {
        if (mode == 0x100) {
            for (i = 0; i < (u16)(c >> 3); i++) {
                CpuSet(src, dst + (a << 5), (b << 2) & 0x1FFFFF);
                src += b << 3;
                a += 0x20;
            }
        }
    } else {
        for (i = 0; i < (u16)(c >> 3); i++) {
            CpuSet(src, dst + (a << 5), (b << 1) & 0x1FFFFF);
            src += b << 2;
            a += 0x20;
        }
    }
}
/* END CopyTileRectTo2D */
/* BEGIN OamListAddTemplateAt */
/* Copy OAM entry `src` into slot idx with attr0.y = y and attr1.x = x replaced. */
u16 *OamListAddTemplateAt(u16 *src, u8 idx, s16 x, s16 y, u32 u4, u32 u5, u32 u6, void *work) {
    u16 *oam = OamListAlloc(idx, work);
    oam[0] = (src[0] & 0xFF00) | (y & 0xFF);
    oam[1] = (src[1] & 0xFE00) | (((u32)x << 23) >> 23);
    return oam;
}
/* END OamListAddTemplateAt */
/* BEGIN OamListAddTemplateOffset */
/* Copy OAM entry `src` into slot idx, offsetting y and x. */
u16 *OamListAddTemplateOffset(u16 *src, u8 idx, s16 x, s16 y, u32 u4, u32 u5, u32 u6, void *work) {
    u16 *oam = OamListAlloc(idx, work);
    u16 a = src[0];
    u16 b;
    oam[0] = (src[0] & 0xFF00) | ((a + y) & 0xFF);
    b = src[1];
    oam[1] = (src[1] & 0xFE00) | ((b + x) & 0x1FF);
    return oam;
}
/* END OamListAddTemplateOffset */
/* BEGIN OamListAddTemplate */
/* Copy a 3-halfword OAM entry (6 bytes) into slot idx of the OAM buffer `work`. */
u16 *OamListAddTemplate(u16 *src, u8 idx, u32 u2, u32 u3, void *work) {
    u16 *oam = OamListAlloc(idx, work);
    *(u32 *)oam = *(u32 *)src;
    oam[2] = src[2];
    return oam;
}
/* END OamListAddTemplate */
/* BEGIN OamListAddSpriteGroup */
/* Emit count eight-byte sprite templates. format 1 accepts modes 0..2;
 * format 0 also accepts modes 3, 4 and 8. Return the last allocated entry.
 * As in the ROM, callers may use the return value only when count is nonzero
 * and the format/mode is supported. No entry is emitted otherwise. */
u16 *OamListAddSpriteGroup(u16 *src, u8 idx, u8 count, u16 x, u16 y,
    u8 mode, u8 priority, u8 tileOffset, u8 palette, u8 format, u16 flags, void *work) {
    u16 *out;
    u8 i;
    priority &= 3;
    switch (format) {
    case 1:
        switch (mode) {
        case 0:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplate(src, idx, tileOffset, palette, work);
                out[2] = (src[2] + (tileOffset << 4)) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        case 1:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplateAt(src, idx, (s16)x, (s16)y, priority, tileOffset, palette, work);
                out[2] = (src[2] + (tileOffset << 4)) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        case 2:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplateOffset(src, idx, (s16)x, (s16)y, priority, tileOffset, palette, work);
                out[2] = (src[2] + (tileOffset << 4)) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        }
        break;
    case 0:
        switch (mode) {
        case 0:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplate(src, idx, tileOffset, palette, work);
                out[2] = ((src[2] & 0xFF0F) + (tileOffset << 4)) | ((src[2] & 0xF0) << 1) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        case 1:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplateAt(src, idx, (s16)x, (s16)y, priority, tileOffset, palette, work);
                out[2] = ((src[2] & 0xFF0F) + (tileOffset << 4)) | ((src[2] & 0xF0) << 1) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        case 2:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplateOffset(src, idx, (s16)x, (s16)y, priority, tileOffset, palette, work);
                out[2] = ((src[2] & 0xFF0F) + (tileOffset << 4)) | ((src[2] & 0xF0) << 1) | (palette << 9) | (priority << 10);
                out[0] |= flags;
                src += 4;
            }
            break;
        case 3:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplate(src, idx, tileOffset, palette, work);
                switch (src[2] & 0x700) {
                case 0:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x100:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x200:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x300:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x400:
                    out[2] = (src[2] & 0xF80F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                }
                out[0] |= flags;
                src += 4;
            }
            break;
        case 4:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplate(src, idx, tileOffset, palette, work);
                switch (src[2] & 0x700) {
                case 0:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x100:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x200:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x300:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x400:
                    out[2] = (src[2] & 0xF80F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                }
                out[0] = (src[0] & 0xFF00) | (y & 0xFF) | flags;
                out[1] = (src[1] & 0xFE00) | (x & 0x1FF);
                src += 4;
            }
            break;
        case 8:
            for (i = 0; i < count; i++) {
                out = OamListAddTemplate(src, idx, tileOffset, palette, work);
                switch (src[2] & 0x700) {
                case 0:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x100:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                case 0x200:
                    out[2] = (src[2] & 0xFC0F) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x300:
                    out[2] = ((src[2] & 0xFC0F) + 0x10) | ((src[2] & 0xF0) << 1) | 0x200 | (priority << 10);
                    break;
                case 0x400:
                    out[2] = (src[2] & 0xF80F) | ((src[2] & 0xF0) << 1) | (priority << 10);
                    break;
                }
                out[0] = (src[0] & 0xFF00) | ((y + (src[0] & 0xFF)) & 0xFF) | flags;
                out[1] = (src[1] & 0xFE00) | ((x + (src[1] & 0x1FF)) & 0x1FF);
                src += 4;
            }
            break;
        }
        break;
    }
    return out;
}
/* END OamListAddSpriteGroup */
