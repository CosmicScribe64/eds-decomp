/*
 * collection (0x0807717C-0x080784E4): card-collection bookkeeping on gSaveData, the game-side
 * sound API and the tile/OAM copy helpers (wiki/functions/collection-c.md).
 *
 * The collection half counts copies of every card in the trunk and in the three saved decks
 * (Deck, Side Deck, Fusion Deck): GetCardCopyLimit reads the forbidden/limited list, the
 * Add/Remove function families move single copies between the trunk and a deck list, and
 * TrimSavedDecksToCopyLimits pushes over-limit copies back to the trunk. RecordDuelWin,
 * RecordDuelLoss and RecordDuelDraw keep the per-opponent duel records.
 * The sound half (IsSeEnabled to StopAllSound) is the game-side API over the Konami driver
 * in sound_driver.c: it honours the save's SE/BGM options and gMain's BGM/SE bookkeeping.
 * The rest are helpers: OffsetNonZeroPixelsAndCopy brightens a bitmap, the CopyTile*
 * functions copy tile images with CpuSet, and the OamListAdd* functions emit OAM entries.
 */
#include "global.h"
#include "card_data.h"          /* CARD_ID_MASK, gCardIdToNumber, gCardNumberToId */
#include "constants/cards.h"    /* enum CardNumber: CARD_POLYMERIZATION, CARD_DARK_MAGICIAN, the alt-art numbers */
#include "debug.h"              /* DebugPrintf, DebugPrintFlush */
#include "legacy/gba.h"                /* CpuSet, CpuFastSet */
#include "legacy/main.h"               /* struct Main gMain: currentBgm, frameCounter, lastSeFrame */
#include "save.h"               /* struct SaveData gSaveData, struct TrunkEntry, struct CardCopyLimit,
                                   DECK_MAX_CARDS, SIDE_DECK_MAX_CARDS, FUSION_DECK_MAX_CARDS,
                                   OPTION_SE_ON, OPTION_BGM_ON, the collection prototypes */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/* One trunk record (gSaveData + 8 + id*4, struct TrunkEntry in save.h) in the two forms this
 * unit's code was compiled from: the copy count through the low halfword, the three per-deck
 * counters through byte +1. save.h packs the counters as u16 bitfields of the low halfword;
 * the ROM loads the byte, so the byte view stays. */
struct TrunkCount {
    u16 count:10;                   /* bits 0-9: copies in the trunk (max 0x3FF) */
    u16 rest:6;
    u16 unk2;
};
struct TrunkCopies {                /* byte view of +1 of the record */
    u8 unk0;
    u8 pad:2;
    u8 deckCopies:2;                /* bits 2-3: copies in the saved Deck, capped by GetCardCopyLimit(id) */
    u8 sideCopies:2;                /* bits 4-5: copies in the saved Side Deck */
    u8 fusionCopies:2;              /* bits 6-7: copies in the saved Fusion Deck */
    u16 unk2;
};
union TrunkEntryView {
    struct TrunkCount c;
    struct TrunkCopies b;
};

/* Card record viewed from the save base: the record for `id` sits at base + id*4 + 8. */
struct CardRec {
    u8 pad0[8];
    union TrunkEntryView e;
};

/* The duel record for `id` (gSaveData + 0x20D0 + id*4, struct DuelRecord in save.h) in the
 * form each RecordDuel* function was compiled from: only its own counter is read and
 * rewritten, as a low-halfword field (wins), a whole-word field (losses) or a high-halfword
 * field (draws). */
struct DuelRecordWinView {
    u8 pad0[0x20D0];
    u16 wins:11;                    /* bits 0-10, max 0x7FF */
    u16 rest:5;
    u16 hi;
};
struct DuelRecordLossView {
    u8 pad0[0x20D0];
    u32 wins:11;                    /* bits 0-10 */
    u32 losses:11;                  /* bits 11-21, max 0x7FF */
    u32 draws:10;                   /* bits 22-31 */
};
struct DuelRecordDrawView {
    u8 pad0[0x20D0];
    u16 lo;
    u16 pad:6;
    u16 draws:10;                   /* bits 22-31 of the record, max 0x3FF */
};

/* ---- ROM tables ---- */

extern const struct CardCopyLimit gCardCopyLimits[];    /* 0x081A78B4: the 47-row forbidden/limited list */

/* Single entries of gCardNumberToId (0x08623DF4) read through their own literal-pool symbols;
 * card_data.h lists what each one is. The code below reads entry [0] of each. */
extern const u16 gUnk_08623E10[];   /* gCardNumberToId[CARD_FLAME_SWORDSMAN] */
extern const u16 gUnk_08623E38[];   /* gCardNumberToId[CARD_DARK_MAGICIAN] */
extern const u16 gUnk_08623E3E[];   /* gCardNumberToId[CARD_GAIA_THE_FIERCE_KNIGHT] */
extern const u16 gUnk_08623E44[];   /* gCardNumberToId[CARD_CELTIC_GUARDIAN] */
extern const u16 gUnk_08623E6E[];   /* gCardNumberToId[CARD_HARPIE_LADY] */
extern const u16 gUnk_08623E72[];   /* gCardNumberToId[CARD_TIGER_AXE] */
extern const u16 gUnk_08623E7C[];   /* gCardNumberToId[CARD_THOUSAND_DRAGON] */
extern const u16 gUnk_086240FA[];   /* gCardNumberToId[CARD_PENDULUM_MACHINE] */
extern const u16 gUnk_086240FE[];   /* gCardNumberToId[CARD_LAUNCHER_SPIDER] */
extern const u16 gUnk_086245CA[];   /* gCardNumberToId[CARD_POLYMERIZATION] */
extern const u16 gUnk_08624768[];   /* gCardNumberToId[CARD_1210] (0 in EDS) */
extern const u16 gUnk_086247B6[];   /* gCardNumberToId[1249] (0 in EDS; 1249 has no CARD_ name) */

extern u8 gStrErrorIdFmt[];         /* 0x08087B80: the format string DebugCheckCardId prints */

/* ---- Sound driver (src/sound_driver.c) ---- */
extern void SoundRequestSE(u32 seId);
extern void SoundRequestBGM(u32 bgmId);
extern void SoundFadeOutBGM(u32 speed);
extern void SoundStopBGM(void);
extern void SoundStopAllSE(void);

/* sprite.h declares it as struct OamListEntry *OamListAlloc(u8 layer, struct OamList *list);
 * the callers here only fill the entry's OAM halfwords, so the pointer is kept as u16. */
u16 *OamListAlloc(u8 idx, void *work);

/* Look up the card's key in gCardIdToNumber, then find its row in the 0x2F-entry table gCardCopyLimits. */
s32 GetCardCopyLimit(u32 id) {
    u16 key = *(const u16 *)((const u8 *)gCardIdToNumber + ((id << 21) >> 20));
    const struct CardCopyLimit *p;
    u32 i;
    for (i = 0, p = gCardCopyLimits; i <= 0x2E; p++, i++) {
        if (key == p->cardNumber)
            return p->limit;
    }
    return 3;
}
/* Returns 1 if the player owns fewer copies of card `id` (plus its related cards) than the
 * limit. Prints that share a limit (alt arts, the Dark Magician and Harpie Lady variants)
 * pool their Deck/Side Deck copies; the Fusion Deck copies pool only for the cases that use
 * ADD_N3. */
#define CR(x) ((struct CardRec *)((u8 *)s + (x) * 4))
#define ADD_PAIR(x) do { sum += CR(x)->e.b.deckCopies; sum += CR(x)->e.b.sideCopies; } while (0)
#define ADD_N3(x) sum += CR(x)->e.b.fusionCopies
u32 IsBelowCardCopyLimit(u16 id) {
    struct SaveData *s = &gSaveData;
    struct CardRec *r = CR(id);
    s32 sum = r->e.b.deckCopies + r->e.b.sideCopies + r->e.b.fusionCopies;
    s32 limit = GetCardCopyLimit(id);
    u16 key = *(const u16 *)((const u8 *)gCardIdToNumber + ((id & CARD_ID_MASK) << 1));
    switch (key) {
    case CARD_POLYMERIZATION:
        ADD_PAIR(*(const u16 *)(((key + 0x1F) << 1) + (u32)((const u16 *)0x08623DF4)));
        break;
    case CARD_POLYMERIZATION_ALT:
        ADD_PAIR(gUnk_086245CA[0]);
        break;
    case CARD_DARK_MAGICIAN:
        ADD_PAIR(gUnk_08624768[0]);
        ADD_PAIR((u16)(gUnk_08623E38[0] + 1));
        break;
    case CARD_1210:
        ADD_PAIR(gUnk_08623E38[0]);
        ADD_PAIR(*(const u16 *)((key << 1) + (u32)((const u16 *)0x08623DF4)));
        break;
    case CARD_DARK_MAGICIAN_ALT: {
        /* FAKEMATCH: keep the initialized related ID separate from the cached count byte. */
        register u16 relatedId asm("r1") = gUnk_08623E38[0];
        u8 flags = *((u8 *)s + relatedId * 4 + 9);
        sum += ((u32)flags << 28) >> 30;
        sum += ((u32)flags << 26) >> 30;
        ADD_PAIR((u16)(relatedId + 1));
        break;
    }
    case CARD_HARPIE_LADY:
        ADD_PAIR(gUnk_086247B6[0]);
        break;
    case 1249: /* no EDS card (no CARD_ name); pairs with CARD_HARPIE_LADY */
        ADD_PAIR(gUnk_08623E6E[0]);
        break;
    case CARD_BLUE_EYES_WHITE_DRAGON:
        ADD_PAIR((u16)(((const u16 *)0x08623DF4)[0] + 1));
        break;
    case CARD_FLAME_SWORDSMAN:
        ADD_N3((u16)(gUnk_08623E10[0] + 1));
        break;
    case CARD_GAIA_THE_FIERCE_KNIGHT:
        ADD_PAIR((u16)(gUnk_08623E3E[0] + 1));
        break;
    case CARD_CELTIC_GUARDIAN:
        ADD_PAIR((u16)(gUnk_08623E44[0] + 1));
        break;
    case CARD_TIGER_AXE:
        ADD_PAIR((u16)(gUnk_08623E72[0] + 1));
        break;
    case CARD_THOUSAND_DRAGON:
        ADD_N3((u16)(gUnk_08623E7C[0] + 1));
        break;
    case CARD_PENDULUM_MACHINE:
        ADD_PAIR((u16)(gUnk_086240FA[0] + 1));
        break;
    case CARD_LAUNCHER_SPIDER:
        ADD_PAIR((u16)(gUnk_086240FE[0] + 1));
        break;
    case CARD_BLUE_EYES_WHITE_DRAGON_ALT:
        ADD_PAIR(((const u16 *)0x08623DF4)[0]);
        break;
    case CARD_FLAME_SWORDSMAN_ALT:
        ADD_N3(gUnk_08623E10[0]);
        break;
    case CARD_GAIA_THE_FIERCE_KNIGHT_ALT:
        ADD_PAIR(gUnk_08623E3E[0]);
        break;
    case CARD_CELTIC_GUARDIAN_ALT:
        ADD_PAIR(gUnk_08623E44[0]);
        break;
    case CARD_TIGER_AXE_ALT:
        ADD_PAIR(gUnk_08623E72[0]);
        break;
    case CARD_THOUSAND_DRAGON_ALT:
        ADD_N3(gUnk_08623E7C[0]);
        break;
    case CARD_PENDULUM_MACHINE_ALT:
        ADD_PAIR(gUnk_086240FA[0]);
        break;
    case CARD_LAUNCHER_SPIDER_ALT:
        ADD_PAIR(gUnk_086240FE[0]);
        break;
    }
    return sum < limit;
}

/* Debug assert: prints gStrErrorIdFmt with the id, then flushes, when the id is 0 or in the
 * 1901..1999 token range used by the card-number token entries. */
void DebugCheckCardId(u16 id) {
    if ((u16)(id - 0x76D) <= 0x62 || id == 0) {
        DebugPrintf(gStrErrorIdFmt, id);
        DebugPrintFlush();
    }
}
void AddCardToTrunk(u16 id) {
    struct SaveData *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.c.count < 0x3FF) {
        r->e.c.count++;
        s->trunkSize++;
    }
}
void AddCardToSavedDeck(u16 id) {
    struct SaveData *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.deckCopies < GetCardCopyLimit(id)) {
        if (s->deckSize < DECK_MAX_CARDS) {
            r->e.b.deckCopies++;
            s->deck[s->deckSize++] = id;
        }
    }
}
void AddCardToSavedSideDeck(u16 id) {
    struct SaveData *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.sideCopies < GetCardCopyLimit(id)) {
        if (s->sideDeckSize < SIDE_DECK_MAX_CARDS) {
            r->e.b.sideCopies++;
            s->sideDeck[s->sideDeckSize++] = id;
        }
    }
}
void AddCardToSavedFusionDeck(u16 id) {
    struct SaveData *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if (r->e.b.fusionCopies < GetCardCopyLimit(id)) {
        if (s->fusionDeckSize < FUSION_DECK_MAX_CARDS) {
            r->e.b.fusionCopies++;
            s->fusionDeck[s->fusionDeckSize++] = id;
        }
    }
}
void RemoveCardFromTrunk(u16 id) {
    struct SaveData *s;
    struct CardRec *r;
    DebugCheckCardId(id);
    s = &gSaveData;
    r = (struct CardRec *)((u8 *)s + id * 4);
    if ((r->e.c.count << 22) != 0) {
        r->e.c.count--;
        s->trunkSize--;
    }
}
/* Removes one copy from the saved Deck and compacts the list.
 * FAKEMATCH: initialized register constraints reproduce byte caching and
 * pointer copies. The bits/flags input with r1 clobber prevents deriving the
 * negative byte mask from the earlier 3. No instruction is emitted. */
void RemoveCardFromSavedDeck(u16 id)
{
    struct SaveData *s;
    register struct CardRec *r __asm__("r5");
    register u32 off __asm__("r0");
    register u32 flags __asm__("r2");
    register u32 stage __asm__("r1");
    u32 copies;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 28;
    copies = stage >> 30;
    if (copies != 0) {
        u32 bits = ((copies - 1) & 3) << 2;
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

/* Removes one copy from the saved Side Deck and compacts the list.
 * FAKEMATCH: initialized register constraints reproduce byte caching and
 * pointer copies. The bits/flags input with r1 clobber prevents deriving the
 * negative byte mask from the earlier 3. No instruction is emitted. */
void RemoveCardFromSavedSideDeck(u16 id)
{
    struct SaveData *s;
    register struct CardRec *r __asm__("r5");
    register u32 off __asm__("r0");
    register u32 flags __asm__("r2");
    register u32 stage __asm__("r1");
    u32 copies;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 26;
    copies = stage >> 30;
    if (copies != 0) {
        u32 bits = ((copies - 1) & 3) << 4;
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

/* Removes one copy from the saved Fusion Deck and compacts the list.
 * FAKEMATCH: initialized constraints retain the staged extraction, separate
 * count copy before decrement, and save-base/count-pointer allocation.
 * Ordinary allocation preserves the copied count pointer in r7. */
void RemoveCardFromSavedFusionDeck(u16 id)
{
    struct SaveData *s;
    struct CardRec *r;
    u32 off;
    u32 flags;
    register u32 stage __asm__("r1");
    u32 copies;
    DebugCheckCardId(id);
    s = &gSaveData;
    off = (u32)id * 4;
    r = (struct CardRec *)(off + (u32)s);
    flags = *((u8 *)r + 9);
    stage = flags << 24;
    __asm__ volatile("" : "+r"(stage));
    copies = stage >> 30;
    if (copies != 0) {
        register u32 bits __asm__("r1");
        register u32 kept __asm__("r0");
        int i;
        u32 countOff;
        register u16 *p __asm__("r0");
        bits = copies;
        __asm__ volatile("" : "+r"(bits) : "r"(copies));
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

/* Trim the saved Deck and Side Deck copy counts to the card-specific limits.
 * FAKEMATCH: initialized address/copy bindings, a read-only stack-pointer
 * input, and staged narrowing retain the ROM's original register lifetimes.
 * The next index stays full-width until the inner removal loop finishes. */
void TrimSavedDecksToCopyLimits(void)
{
    u32 i;
    struct SaveData *s;
    u32 saveBase;
    u32 countOff;
    u16 *initialCount;
    u16 *saved;
    i = 0;
    saveBase = (u32)&gSaveData;
    countOff = 0x20C8;
    initialCount = (u16 *)(saveBase + countOff);
    s = (struct SaveData *)saveBase;
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
                    u8 deckCopies = (flags << 28) >> 30;
                    u8 sideCopies = (flags << 26) >> 30;
                    u8 limit;
                    u32 next;
                    int sum;
                    saved = p;
                    limit = GetCardCopyLimit(id);
                    sum = deckCopies + sideCopies;
                    next = i + 1;
                    __asm__ volatile("" : : "m"(saved));
                    {
                        register u16 *loaded __asm__("r3") = saved;
                        if (sum > limit) {
                            register u16 *current __asm__("r6") = loaded;
                            do {
                                if (sideCopies != 0) {
                                    RemoveCardFromSavedSideDeck(*current);
                                    AddCardToTrunk(*current);
                                    sideCopies--;
                                } else if (deckCopies != 0) {
                                    RemoveCardFromSavedDeck(*current);
                                    AddCardToTrunk(*current);
                                    deckCopies--;
                                }
                            } while (deckCopies + sideCopies > limit);
                        }
                        {
                            register u32 nextCopy __asm__("r1") = next;
                            u32 stage = nextCopy << 16;
                            __asm__ volatile("" : "+r"(stage) : "r"(nextCopy));
                            i = stage >> 16;
                        }
                    }
                    {
                        register u16 *count1 __asm__("r2") = &gSaveData.deckSize;
                        if (i == *count1)
                        break;
                    }
                }
            } while (1);
        }
    }
    i = 0;
    if (i != s->sideDeckSize) {
        u32 base = (u32)s;
        u16 *count = &s->sideDeckSize;
        do {
            register u32 byteOff __asm__("r1") = i * 2;
            register u32 listBase __asm__("r0") = base + 0x2080;
            {
                register u16 *p __asm__("r3") = (u16 *)(byteOff + listBase);
                u16 id = *p;
                struct CardRec *r = (struct CardRec *)((u32)id * 4 + base);
                u32 flags = *((u8 *)r + 9);
                u8 deckCopies = (flags << 28) >> 30;
                u8 sideCopies = (flags << 26) >> 30;
                u8 limit;
                u32 next;
                int sum;
                saved = p;
                limit = GetCardCopyLimit(id);
                sum = deckCopies + sideCopies;
                next = i + 1;
                __asm__ volatile("" : : "m"(saved));
                {
                    register u16 *loaded __asm__("r3") = saved;
                    if (sum > limit) {
                        u16 *current = loaded;
                        do {
                            if (sideCopies != 0) {
                                RemoveCardFromSavedSideDeck(*current);
                                AddCardToTrunk(*current);
                                sideCopies--;
                            } else if (deckCopies != 0) {
                                RemoveCardFromSavedDeck(*current);
                                AddCardToTrunk(*current);
                                deckCopies--;
                            }
                        } while (deckCopies + sideCopies > limit);
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

void RecordDuelWin(u32 id) {
    struct SaveData *s = &gSaveData;
    struct DuelRecordWinView *w = (struct DuelRecordWinView *)((u8 *)s + id * 4);
    s32 wins = w->wins;
    if (wins <= 0x7FE)
        w->wins = wins + 1;
    s->lastOpponent = id;
}
void RecordDuelLoss(u32 id) {
    struct SaveData *s = &gSaveData;
    struct DuelRecordLossView *w = (struct DuelRecordLossView *)((u8 *)s + id * 4);
    s32 losses = w->losses;
    if (losses <= 0x7FE)
        w->losses = losses + 1;
    s->lastOpponent = id;
}
void RecordDuelDraw(u32 id) {
    struct SaveData *s = &gSaveData;
    struct DuelRecordDrawView *w = (struct DuelRecordDrawView *)((u8 *)s + id * 4);
    s32 draws = w->draws;
    if (draws <= 0x3FE)
        w->draws = draws + 1;
    s->lastOpponent = id;
}
void IncrementChampionshipWins(void) {
    if (gSaveData.championshipWins < 0xFF)
        gSaveData.championshipWins++;
}
u16 IsSeEnabled(void) {
    return gSaveData.options & OPTION_SE_ON;
}
u16 IsBgmEnabled(void) {
    return (gSaveData.options >> 1) & 1;    /* OPTION_BGM_ON, shifted down */
}
void SetSeEnabled(u16 on) {
    if (on)
        gSaveData.options |= OPTION_SE_ON;
    else
        gSaveData.options &= ~OPTION_SE_ON;
}
void SetBgmEnabled(u16 on) {
    if (on)
        gSaveData.options |= OPTION_BGM_ON;
    else
        gSaveData.options &= ~OPTION_BGM_ON;
}
void PlaySE(u32 id) {
    if (IsSeEnabled()) {
        struct Main *m = &gMain;
        if (m->lastSeFrame != m->frameCounter) {
            m->lastSeFrame = m->frameCounter;
            SoundRequestSE(id);
        }
    }
}
void PlayBGM(u32 id) {
    if (IsBgmEnabled()) {
        struct Main *m = &gMain;
        if (m->currentBgm != id) {
            SoundRequestBGM(id);
            m->currentBgm = id;
        }
    }
}
void PlayBGMNoTrack(u32 id) {
    if (IsBgmEnabled())
        SoundRequestBGM(id);
}
void PlayJingle(u32 id) {
    if (IsSeEnabled()) {
        struct Main *m = &gMain;
        if (m->currentBgm != id) {
            SoundRequestBGM(id);
            m->currentBgm = id;
        }
    }
}
void StopBGM(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundStopBGM();
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
void FadeOutBGM(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundFadeOutBGM(0x10);
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
void FadeOutBGMAtSpeed(u32 arg) {
    struct Main *m;
    SoundFadeOutBGM(arg);
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
void StopAllSound(void) {
    struct Main *m;
    if (IsBgmEnabled())
        SoundStopBGM();
    if (IsSeEnabled())
        SoundStopAllSE();
    m = &gMain;
    m->currentBgm = 0xFFFF;
}
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
/* Copy a tile image: mode 0x100 is one flat block, mode 0x10 copies 16 rows of
 * 0x200 bytes into a 0x400-byte stride. */
void CopyTileSheetTo2D(const u8 *srcArg, u8 *dstArg, u16 mode) {
    u16 i;
    const u8 *src = srcArg;
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
void CopyTileSheetRowsTo2D(const u8 *srcArg, u8 *dstArg, u16 mode, u8 rows) {
    u16 i;
    const u8 *src = srcArg;
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
void CopyTileRectTo2D(const u8 *srcArg, u8 *dstArg, u16 a, u16 b, u16 c, u16 mode) {
    u16 i;
    const u8 *src = srcArg;
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
/* Copy OAM entry `src` into slot idx with attr0.y = y and attr1.x = x replaced. */
u16 *OamListAddTemplateAt(u16 *src, u8 idx, s16 x, s16 y, u32 u4, u32 u5, u32 u6, void *work) {
    u16 *oam = OamListAlloc(idx, work);
    oam[0] = (src[0] & 0xFF00) | (y & 0xFF);
    oam[1] = (src[1] & 0xFE00) | (((u32)x << 23) >> 23);
    return oam;
}
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
/* Copy a 3-halfword OAM entry (6 bytes) into slot idx of the OAM buffer `work`. */
u16 *OamListAddTemplate(u16 *src, u8 idx, u32 u2, u32 u3, void *work) {
    u16 *oam = OamListAlloc(idx, work);
    *(u32 *)oam = *(u32 *)src;
    oam[2] = src[2];
    return oam;
}
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
