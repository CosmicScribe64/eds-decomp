#include "global.h"

int CountActiveCardsOnField(int player, u16 number);
int CountFaceUpMonstersByNumber(int player, u16 number);
int CountOtherFaceUpSameNameMonsters(int player, int zone);
void ShowCardEffect(int player, u16 x);
void DrawCards(int player, int n);
void LoseLifePoints(int player, int n);
void DestroyFieldCard(int player, int zone, int a);
void DuelPrompt_PostDiscard(int player, int n, int a, int b);
void DuelCmd_Push(u32 msg, u16 zone, int a, int b);
extern const u16 gCardNumberToId[];
struct DuelZone { u32 w0; u8 unk4; u8 unk5; u8 flags6; u8 unk7[0x94 - 7]; };
#define ID(z) (((z)->w0 << 20) >> 20)
struct ZoneBits { u8 pad[6]; u8 lo : 2; u8 kind : 4; u8 hi : 2; u8 rest[0x94 - 7]; };
struct DuelZonesPlayer { struct DuelZone z[11]; u8 filler[0xD64 - 11 * 0x94]; };
extern struct DuelZonesPlayer gDuelZones[];
extern u8 gDuel[];
struct AE60 { u8 unk0[0x14]; u16 h14; };
extern struct AE60 gTextBox;
extern const u32 gCardStats[];
extern const u16 gUnk_086246BC[];
extern const u16 gUnk_08623F3E[], gUnk_08624084[], gUnk_086241A8[];
extern const char gStrSinisterSerpentPrompt[];
extern const char gCardNames[];
int CountActiveCardsOnField2(int player, u16 number);
int GetZoneCardType(int player, int zone);
void QueueAddZoneLink(int player, u16 cardId, u16 pos, u16 a);
void sub_080197C0(int player, u16 id);
void GainLifePoints(int player, int lp);
void ChangeBattlePosition(int a, int b, int c, int d);
int CountGraveyardCardsByNumber(int player, u16 number);
void ReturnGraveyardCardToHand(int player, u16 id);
void FormatStr(char *dst, const char *fmt, const char *arg);
void TextBoxOpen(int a, int b, int c, char *s);
void TextBoxSetMenu(int a, int b, int c);
struct EffEnt { u16 num; u8 rest0[6]; u16 (*fn)(void *, u16); u8 rest[12]; };
struct DuelPlayerB {
    u8 unk0[2];
    u8 handCount;                       /* +0x02 */
    u8 pad3[3];
    u8 count;                           /* +0x06 */
    u8 flags7lo : 3; u8 flags7bit3 : 1; u8 flags7bit4 : 1; u8 flags7hi : 3;  /* +0x07 */
    u8 pad8[0x684 - 8];
    u32 hand[80];                       /* +0x684 */
    u8 pad4[0xB84 - 0x684 - 0x140];
    u32 list[80];                       /* +0xB84 */
    u8 pad2[0xD64 - 0xB84 - 0x140];
};
extern struct DuelPlayerB gDuelPlayers[];
extern const u16 gUnk_086249EE[];
int FindCardEffect(u32 id);
int Random(void);
int FindFreeSpellTrapZone(void);
int PlaceDeckCardOnField(int player, u16 number, int x);
int __modsi3(int, int);
extern struct EffEnt gCardEffects[];
extern const u16 gCardIdToNumber[];

/* For a face-down or valid card in the zone: if card 0x52 is present for either side, queues a request for it (hypothesis) */
void ApplyPumpkingBoost(int player, int zone)
{
    int pi = player & 1;
    int s1 = zone * 0x94 + pi * 0xD64;
    u8 *base = (u8 *)gDuelZones;
    struct DuelZone *z = (struct DuelZone *)(s1 + (int)base);
    if (((z)->w0 << 20) != 0 && (z->flags6 & 2) != 0) {
        if (CountActiveCardsOnField2(0, 0x52) > 0 || CountActiveCardsOnField2(1, 0x52) > 0) {
            sub_080197C0(player, ID(z));
            QueueAddZoneLink(player, ID(z), (u8)player | (u8)zone << 8, 0xD);
        }
    }
}
void TriggerMysteriousPuppeteer(int player)
{
    int a = CountActiveCardsOnField2(player, 0xA5);
    int other = 1 - player;
    int b = CountActiveCardsOnField2(other, 0xA5);
    if (a > 0 || b > 0) {
        ShowCardEffect(player, gUnk_08623F3E[0]);
        GainLifePoints(player, a * 500);
        GainLifePoints(other, b * 500);
    }
}
void ApplyDragonCaptureJar(int player)
{
    int found;
    int p, z;
    u32 id = 0x148;
    if (CountActiveCardsOnField(0, id) > 0 || CountActiveCardsOnField(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2;) {
            /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
            register int next asm("r6");
            z = 0;
            next = p + 1;
            for (; z < 5; z++) {
                int s1 = z * 0x94 + (p & 1) * 0xD64;
                u8 *base = (u8 *)gDuelZones;
                struct DuelZone *zn = (struct DuelZone *)(s1 + (int)base);
                if ((zn->w0 << 20) != 0 && (zn->flags6 & 3) == 2) {
                    if (GetZoneCardType(p, z) == 1)
                        found = 1;
                }
            }
            p = next;
        }
        if (found != 0) {
            u32 msg = 0x73;
            if (player != 0)
                msg = 0x8073;
            DuelCmd_Push(msg, gUnk_08624084[0], 1, 0);
            for (p = 0; p < 2;) {
                /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
                register int next asm("r6");
                z = 0;
                next = p + 1;
                for (; z < 5; z++) {
                    int s1 = z * 0x94 + (p & 1) * 0xD64;
                    u8 *base = (u8 *)gDuelZones;
                    struct DuelZone *zn = (struct DuelZone *)(s1 + (int)base);
                    if ((zn->w0 << 20) != 0 && (zn->flags6 & 3) == 2) {
                        if (GetZoneCardType(p, z) == 1)
                            ChangeBattlePosition(p, z, 0, 0);
                    }
                }
                p = next;
            }
        }
    }
}
int SinisterSerpentStandbyStep(int player)
{
    char buf[0x80];
    u8 *e = gDuel;
    u8 *step = e + 0x1B22;
    switch (*step) {
    case 0: {
        int id = 0x1DA;
        if (CountGraveyardCardsByNumber(player, id) == 0)
            return 1;
        if (player != 0) {
            /* FAKEMATCH: materialize the base in r0 before loading the flag value. */
            register struct AE60 *flag __asm__("r0") = &gTextBox;
            __asm__ __volatile__("" : : "r"(flag));
            flag->h14 = 1;
        } else {
            FormatStr(buf, gStrSinisterSerpentPrompt, gCardNames + ((const u16 *)0x08623DF4)[id] * 0x40);
            TextBoxOpen(0x206, 0x712, 0xB, buf);
            TextBoxSetMenu(1, 0, 0);
        }
        break;
    }
    case 1:
        if (gTextBox.h14 != 0) {
            int id = 0x1DA;
            ShowCardEffect(player, gUnk_086241A8[0]);
            ReturnGraveyardCardToHand(player, id);
        }
        break;
    default:
        return 1;
    }
    (*step)++;
    return 0;
}
/* Both the inline argument and the caller's card ID are narrow in the ROM. */
static inline u32 EffectCardType(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}

/* Queue a prompt for each face-down type-0x15 card in spell/trap zones. */
void DisableFaceUpTraps(void)
{
    int p, z;
    for (p = 0; p < 2; p++) {
        for (z = 5; z <= 9; z++) {
            struct DuelZone *zn = (struct DuelZone *)(z * 0x94 + (p & 1) * 0xD64 + 0x0201930C);
            u16 id = (zn->w0 << 20) >> 20;
            if (id != 0 && (zn->flags6 & 2) != 0) {
                if (EffectCardType(id) == 0x15) {
                    u32 msg = 0xB1;
                    if (p != 0)
                        msg = 0x80B1;
                    DuelCmd_Push(msg, z, 1, 0);
                }
            }
        }
    }
}
void PayChainEnergyCost(int player)
{
    u32 id = 0x436;
    int n = CountActiveCardsOnField(player, id);
    n += CountActiveCardsOnField(1 - player, id);
    if (n > 0) {
        u32 msg;
        ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
        msg = 0x43;
        if (player != 0)
            msg = 0x8043;
        DuelCmd_Push(msg, n * 500, 1, 0);
    }
}
void ApplyKotodama(void)
{
    int found;
    int p, z;
    u32 id = 0x464;
    if (CountFaceUpMonstersByNumber(0, id) > 0 || CountFaceUpMonstersByNumber(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2; p++) {
            for (z = 0; z < 5; z++) {
                if (CountOtherFaceUpSameNameMonsters(p, z) != 0)
                    found = 1;
            }
        }
        if (found != 0) {
            ShowCardEffect(0, gUnk_086246BC[0]);
            for (p = 0; p < 2; p++) {
                for (z = 0; z < 5; z++) {
                    if (CountOtherFaceUpSameNameMonsters(p, z) != 0)
                        DestroyFieldCard(p, z, 1);
                }
            }
        }
    }
}
void ApplyKotodamaToZone(int player, int zone)
{
    u32 id = 0x464;
    if (CountFaceUpMonstersByNumber(0, id) > 0 || CountFaceUpMonstersByNumber(1, id) > 0) {
        if (CountOtherFaceUpSameNameMonsters(player, zone) > 0) {
            ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
            DestroyFieldCard(player, zone, 1);
        }
    }
}
void TriggerAppropriate(int player)
{
    u32 id = 0x475;
    int n = CountActiveCardsOnField(player, id);
    if (n > 0) {
        ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
        DrawCards(player, n * 2);
    }
}
void TriggerForcedRequisition(int player, int mul)
{
    u32 id = 0x476;
    mul *= CountActiveCardsOnField(player, id);
    if (mul > 0) {
        ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
        DuelPrompt_PostDiscard(1 - player, mul, 0, 1);
    }
}
void LoseLpOnSendToGraveyard(int player, int idx)
{
    u32 id = 0x51A;
    int n = CountActiveCardsOnField(player, id);
    n += CountActiveCardsOnField(1 - player, id);
    if (n > 0) {
        ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
        LoseLifePoints(player, idx * 300);
    }
}
void ResolvePendingGraveyardEquip(int player, int zone, u16 flag)
{
    if (flag != 0)
        DuelCmd_Push(player ? 0x80D9 : 0xD9, zone, 1, 0);
    else
        DuelCmd_Push(player ? 0x80D8 : 0xD8, zone, 0, 0);
}
void OnCardDestroyedByEffect(int a, int b, int c)
{
    int pi = b & 1;
    int s1 = c * 0x94 + pi * 0xD64;
    u8 *zb = (u8 *)gDuelZones;
    struct DuelZone *z = (struct DuelZone *)(s1 + (int)zb);
    if (((const u16 *)0x08622AB4)[(z->w0 << 21) >> 21] == 0x5EA && a != b) {
        u32 id = 0x453;
        if (CountFaceUpMonstersByNumber(0, id) <= 0 && CountFaceUpMonstersByNumber(1, id) <= 0 && c <= 4) {
            u32 msg;
            sub_080197C0(b, ID(z));
            msg = 0x4C;
            if (b != 0)
                msg = 0x804C;
            DuelCmd_Push(msg, 1, 0, 0);
        }
    }
}
void PlaceNextSpiritMessage(int player, int zone)
{
    /* FAKEMATCH: keep the queried position in r8 across the selection branches. */
    register int x asm("r8") = FindFreeSpellTrapZone();
    int pi = player & 1;
    struct DuelZone *zn = (struct DuelZone *)(zone * 0x94 + pi * 0xD64 + 0x0201930C);
    u16 num;
    switch (((struct ZoneBits *)zn)->kind) {
    case 0:
        num = 0x605;
        break;
    case 1:
        num = 0x606;
        break;
    case 2:
        num = 0x607;
        break;
    case 3:
        num = 0x608;
        break;
    default:
        return;
    }
    if (PlaceDeckCardOnField(player, num, x) != 0) {
        /* FAKEMATCH: preserve the lookup result in r0 until the u16 call conversion. */
        register u32 id asm("r0");
        if (num == 0xFFFF) {
            id = 0;
        } else if (num <= 0x7CF) {
            /* FAKEMATCH: retain the table load after the byte-index calculation. */
            int index = num * 2;
            const u16 *table = (const u16 *)0x08623DF4;
            __asm__("" : "+r"(table));
            id = *(const u16 *)(index + (int)table);
        } else {
            num |= 0x30;
            {
                /* FAKEMATCH: use r4 for the table base after computing the byte index. */
                int index = num * 2;
                register const u16 *table asm("r4") = (const u16 *)0x08623DF4;
                __asm__("" : "+r"(table));
                id = *(const u16 *)(index + (int)table) + 1;
            }
        }
        ShowCardEffect(player, id);
    } else {
        int i = 0;
        u8 *base = (u8 *)gDuelPlayers;
        int offset = (player & 1) * 0xD64;
        struct DuelPlayerB *pl = (struct DuelPlayerB *)(offset + (int)base);
        if (i < pl->handCount) {
            u8 *handBase = base + 0x684;
            /* FAKEMATCH: keep the slot mask in r9 and preserve its reload for masking x. */
            register int mask asm("r9") = 0xF;
            __asm__("" : "+r"(mask));
            {
                /* FAKEMATCH: retain the masked position in ip through the hand scan. */
                register int lowX asm("r12") = x & mask;
                u32 *e = (u32 *)(offset + (int)handBase);
                int bound;
                do {
                    u32 cid = (*e << 20) >> 20;
                    if (((const u16 *)0x08622AB4)[cid & 0x7FF] == num) {
                        u32 msg = 0xC5;
                        if (player != 0)
                            msg = 0x80C5;
                        {
                            int packed = (i & mask) << 4;
                            /* FAKEMATCH: combine the saved position through r5. */
                            register int low asm("r5") = lowX;
                            __asm__("" : "+r"(low));
                            packed |= low;
                            {
                                /* FAKEMATCH: materialize the flag in r7, retaining its move to r0. */
                                register int flag asm("r7") = 0x100;
                                register int value asm("r0") = flag;
                                __asm__("" : "+r"(value));
                                packed |= value;
                            }
                            DuelCmd_Push(msg, cid, packed, 0);
                        }
                        ShowCardEffect(player, (*e << 20) >> 20);
                        return;
                    }
                    e++;
                    i++;
                    bound = pl->handCount;
                    /* FAKEMATCH: preserve the loop-bound reload in the original scratch register. */
                    __asm__("" : "+r"(bound));
                } while (i < bound);
            }
        }
    }
}
void DamageOpponentPerBanishedMonster(int player)
{
    if (CountActiveCardsOnField(player, 0x5FD) != 0) {
        /* FAKEMATCH: keep the qualifying-card count in the original r4. */
        register int n asm("r4") = 0;
        u8 *base = (u8 *)gDuelPlayers;
        int pi = (1 - player) & 1;
        /* FAKEMATCH: retain the offset in r2; integer address sums preserve ADD operand order. */
        register int offset asm("r2") = pi * 0xD64;
        int count = ((struct DuelPlayerB *)(offset + (int)base))->count;
        if (n < count) {
            int listOffset = 0xB84;
            /* FAKEMATCH: materialize the list base in r0 before adding the player offset. */
            register u8 *listBase asm("r0") = base + listOffset;
            u32 *cursor = (u32 *)(offset + (int)listBase);
            /* FAKEMATCH: keep the only hoisted lookup constant in r6. */
            register int mask asm("r6") = 0x7FF;
            /* FAKEMATCH: reuse r2 for the remaining-card counter. */
            register int remain asm("r2") = count;
            /* FAKEMATCH: this back edge leaves the table and type-mask loads in the loop. */
        loop:
            {
                u16 id = (*cursor << 20) >> 20;
                if (((((const u32 *)0x08621DE0)[id & mask] & 0x1F00000) >> 20) <= 0x14)
                    n++;
                cursor++;
            }
            if (--remain != 0)
                goto loop;
        }
        if (n > 0) {
            ShowCardEffect(player, gUnk_086249EE[0]);
            LoseLifePoints(1 - player, n * 100);
        }
    }
}
/* Dice effect. Rolls a d6 and destroys every monster whose level (0 for types 0x15-0x17, 10 for 0x18) equals the roll (hypothesis: card 0x600) */
void RollDieDestroyMonstersByLevel(int player)
{
    int z, p;
    u32 id = 0x600;
    if (CountActiveCardsOnField(player, id) != 0) {
        int roll = Random() % 6 + 1;
        /* FAKEMATCH: retain the roll in the ROM's register before announcing it. */
        __asm__ __volatile__("" : : "r"(roll));
        ShowCardEffect(player, ((const u16 *)0x08623DF4)[id]);
        {
            u32 msg = 0xE4;
            if (player != 0)
                msg = 0x80E4;
            DuelCmd_Push(msg, roll, 0, 0);
        }
        {
            u32 msg = 0x12;
            if (player != 0)
                msg = 0x8012;
            DuelCmd_Push(msg, 0, 0, 0);
        }
        for (p = 0; p < 2; p++) {
            for (z = 0; z <= 4; z++) {
                struct DuelZone *zn = (struct DuelZone *)(z * 0x94 + (p & 1) * 0xD64 + 0x0201930C);
                u16 cid = (zn->w0 << 20) >> 20;
                if (cid != 0 && (zn->flags6 & 2) != 0) {
                    int lv;
                    int hit;
                    int t = (((const u32 *)0x08621DE0)[cid & 0x7FF] & 0x1F00000) >> 20;
                    switch (t) {
                    case 0x15:
                    case 0x16:
                    case 0x17:
                        lv = 0;
                        break;
                    case 0x18:
                        lv = 10;
                        break;
                    default:
                        lv = (((const u32 *)0x08621DE0)[cid & 0x7FF] & 0x1E000000) >> 25;
                        break;
                    }
                    hit = 0;
                    if (lv == roll)
                        hit = 1;
                    if (lv > 5 && roll == 6)
                        hit = 1;
                    if (hit != 0)
                        DestroyFieldCard(p, z, 1);
                }
            }
        }
    }
}
int EffectNopResolve(void)
{
    return 0;
}
int EffectSpiritMessagePrepare(void)
{
    return 0;
}
/* Binary-search the effect table gCardEffects (6-byte entries, 0x1AA of them, sorted by card number) for a card; -1 if absent. */
int FindCardEffect(u32 id)
{
    int lo = 0;
    int hi = 0x1A9;
    u16 key = ((const u16 *)0x08622AB4)[(id << 21) >> 21];
    for (;;) {
        int mid = (lo + hi) / 2;
        u16 v = gCardEffects[mid].num;
        if (key == v)
            return mid;
        if (lo == hi)
            return -1;
        if (key > v)
            lo = mid;
        if (key < v)
            hi = mid;
        if ((lo + hi) / 2 == mid)
            lo = hi;
    }
}
/* Runs the per-card effect handler from gCardEffects for the card in ref. Returns 1 if there is none and 0 if ref is NULL. */
u16 CanEffectTargetZone(u16 *ref, int a, int b)
{
    if (ref != 0) {
        int idx = FindCardEffect(*ref);
        u16 (*fn)(void *, u16);
        if (idx < 0 || (fn = gCardEffects[idx].fn) == 0)
            return 1;
        return fn(ref, (u8)a | (u8)b << 8);
    }
    return 0;
}
int CanNormalSummon(int player)
{
    if (!gDuelPlayers[player & 1].flags7bit3 && CountActiveCardsOnField(player, 0x592) == 0
        && CountActiveCardsOnField(0, 0x5F6) == 0 && CountActiveCardsOnField(1, 0x5F6) == 0)
        return 1;
    return 0;
}
int CanSpecialSummon(int player)
{
    if (gDuelPlayers[player & 1].flags7bit4)
        return 0;
    if (CountActiveCardsOnField(player, 0x592) != 0)
        return 0;
    if (CountActiveCardsOnField(0, 0x5E6) != 0)
        return 0;
    if (CountActiveCardsOnField(1, 0x5E6) != 0)
        return 0;
    if (CountActiveCardsOnField(0, 0x5F6) != 0)
        return 0;
    if (CountActiveCardsOnField(1, 0x5F6) != 0)
        return 0;
    return 1;
}
/* Reconstruction of the summon/tribute selection state machine (a disabled draft).
 * State names remain hypotheses; byte accesses and callees are ROM-derived.
 * The sequence switches intentionally have no default assignment, as in the
 * ROM; invalid sequence values retain the live required-card register. State
 * 70 likewise has no default text assignment. The draft stays disabled until
 * the whole unit is byte-equal, including those paths. */
struct SummonDuel {
    u8 prefix[0x1B28];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
extern struct SummonDuel gSummonDuel asm("gDuel");
struct SummonCursor {
    u8 prefix[0x824];
    int player;
    int area;
    int zone;
};
extern struct SummonCursor gDuelScreen;
struct SummonList {
    u8 prefix[5];
    u8 row:2;
    u8 rest5:6;
    u16 scroll;
    u8 pad8[4];
    u32 cards[128];
};
extern struct SummonList gCardListView;
struct SummonCardRef {
    u16 id;
    u8 player:1;
    u8 rest2:7;
    u8 filler[0x14 - 3];
};
struct SummonMain { u8 prefix[6]; u16 keys; };
extern struct SummonMain gMain;
extern char *gTributeSummonPrompts[];
extern const char gStrSpecialSummonSelectTribute[], gStrTributeFromField[], gStrTributeFromFieldOrHand[];
extern const char gStrTributeFromHand[], gStrBanishFieldMonstersCount[], gStrFiend[];
extern const char gStrBanishGraveyardMonstersCount[], gStrCardsRemaining[], gStrLight[];
extern const char gStrBanishFieldMonster[], gStrFire[], gStrBanishGraveyardMonster[];
extern const char gStrWater[], gStrEarth[], gStrWind[];
extern const char gStrTributeEitherFromField[], gStrTributeOneFromField[];
extern const u16 gUnk_0862401E[], gUnk_086240CE[];
int CountMonstersByNumber(int player, u16 number);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int IsTributableMonster(int player, int zone);
int CountHandCardsByNumber(int player, u16 number);
int GetZoneCardAttribute(int player, int zone);
void TributeMonster(int player, int zone);
void BanishFieldCard(int player, int zone, int arg);
void DiscardHandCard(int player, int zone, int arg, int arg2);
void BanishGraveyardCard(int player, u32 *card);
void Chain_AddPending(u32 msg, int arg);
void CardListView_Open(int player, int arg, u16 number, int arg2);
int EffectEquippedTributeCheck(struct SummonCardRef *ref, u16 pos);
int CollectEffectTargets(int player, u16 number, int arg);
int DuelCursor_PickTarget(int mask);
void QueueNormalSummon(int player, int zone, int target, u16 tribute, u16 faceUp);
void QueueSpecialSummonFromHand(int player, int zone, int target, u16 tribute, u16 faceUp);
u16 DuelCursor_GetCardId(void);
void FormatInt(char *dst, const char *format, int arg);
void PlaySE(int sound);

struct SummonDuelFromPlayer {
    u8 prefix[0x1B24];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
struct SummonDuelFromZone {
    u8 prefix[0x1AFC];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
struct SummonPlayerHeader {
    u8 prefix[6];
    u8 count;
    u8 pad7[9];
    u8 flags10;
    u8 rest[0xD64 - 0x11];
};
/* FAKEMATCH: unsigned subtraction of the negated stride retains ADD operand order. */
#define SUMMON_HEADER(p) (*(struct SummonPlayerHeader *)((u32)&SD - (u32)(-((p) * 0xD64))))
#define SD gSummonDuel
#define SDP (*(struct SummonDuelFromPlayer *)gDuelPlayers)
#define SDZ (*(struct SummonDuelFromZone *)gDuelZones)
#define SC gDuelScreen
#define SUMMON_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define SUMMON_CARD_NAME(id) (gCardNames + (id) * 0x40)
#define SUMMON_HUMAN(p) ((s32)((u32)gDuelPlayers[(p) & 1].pad8[4] << 26) < 0)
#define SUMMON_POS ((u8)SC.player | ((u8)SC.zone << 8))
#define SUMMON_EVENT_POS ((u8)SC.area | ((u8)SC.zone << 8))
#define SUMMON_SELECTED_POS (((u8)SC.player << 8) | (u8)SC.zone)

static inline int SummonLevel(u16 id)
{
    int type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    switch (type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}

static inline u16 SummonCardId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= 0x7CF)
        return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
static inline u16 SummonCardIdSymbol(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= 0x7CF)
        return *(gCardNumberToId + (number & 0x7FF));
    return *(gCardNumberToId + ((number - 0x7D0) & 0x7FF)) + 1;
}

void CardMenu_SummonMonster(u16 faceUp, u16 special)
{
    char text[0x80];
    char format[0x80];
    struct SummonCardRef ref;
    u16 required; /* Assigned only for sequence values 0..2, as in the ROM. */
    switch (SD.step) {
    case 0:
        switch (SUMMON_NUMBER(SD.cardId)) {
        case 0x37:
        case 0x38:
        case 0x42:
            FormatStr(format, gStrSpecialSummonSelectTribute, SUMMON_CARD_NAME(SD.cardId));
            FormatStr(text, format, SUMMON_CARD_NAME(gUnk_0862401E[0]));
            TextBoxOpen(0x206, 0x712, 0xB, text);
            SD.step = 20;
            break;
        case 0x170:
            FormatStr(format, gStrSpecialSummonSelectTribute, SUMMON_CARD_NAME(SD.cardId));
            FormatStr(text, format, SUMMON_CARD_NAME(gUnk_086240CE[0]));
            TextBoxOpen(0x206, 0x712, 0xB, text);
            SD.step = 20;
            break;
        case 0x175:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 30;
            break;
        case 0x34D:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 40;
            break;
        case 0x4E2:
            if (SUMMON_HEADER(SD.player).count == 1) {
                QueueNormalSummon(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, 1);
                SD.active = 0;
            } else {
                TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[1]);
                TextBoxSetMenu(1, 0, 0);
                SD.selected = 0;
                SD.step++;
            }
            break;
        case 0x4E9:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 80;
            break;
        case 0x5EA:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 50;
            break;
        case 0x5EB:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 60;
            break;
        case 0x5EC:
        case 0x5ED:
        case 0x5EE:
        case 0x5EF:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 70;
            break;
        case 0x546:
            switch (SD.command) {
            case 11:
            case 12:
                QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, faceUp);
                SD.active = 0;
                break;
            default:
                goto normal_summon;
            }
            break;
        default:
        normal_summon:
            /* FAKEMATCH: consume the level so its constant arms still enter the switch head tests. */
            switch (({ int level = SummonLevel(SD.cardId); asm volatile("" : : "r"(level)); level; })) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                if (special)
                    QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, faceUp);
                else
                    QueueNormalSummon(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, faceUp);
                if (SUMMON_NUMBER(SD.cardId) == 0x5F0) {
                    switch (SD.command) {
                    case 11:
                    case 12:
                        if (CollectEffectTargets(1 - SD.player, 0x447, 0) != 0
                            && CountFreeMonsterZones(1 - SD.player) > 0
                            && (u16)CanSpecialSummon(1 - SD.player) != 0) {
                            u32 event;
                            u32 playerBit = (1u & SD.player) << 31;
                            /* FAKEMATCH: extract the event field through r2 before masking into r1. */
                            register u32 eventField asm("r2") = SD.eventZone;
                            asm("" : "+r"(eventField)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            event = ((eventField & 0x1F) << 16) | 0x0E400000;
                            /* FAKEMATCH: combine the event word before the player/card bits. */
                            asm("" : "+r"(event));
                            Chain_AddPending(playerBit | event | SD.cardId, 0);
                        }
                    }
                }
                SD.active = 0;
                break;
            case 5:
            case 6:
                TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[0]);
                TextBoxSetMenu(1, 0, 0);
                SD.selected = 0;
                SD.step = 10;
                break;
            default:
                TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[1]);
                TextBoxSetMenu(1, 0, 0);
                SD.selected = 0;
                SD.step++;
                break;
            }
            break;
        }
        break;
    case 1:
        if (gTextBox.h14 == 0) {
            SD.active = 0;
            break;
        }
        SD.step++;
    case 2:
        TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[2]);
        SD.step++;
        break;
    case 3:
        if (gMain.keys & 2) {
            TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[1]);
            TextBoxSetMenu(1, 0, 0);
            SD.selected = 0;
            SD.step = 1;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (IsTributableMonster(SC.player, SC.zone)) {
                SD.selected = SUMMON_SELECTED_POS;
                PlaySE(1);
                DuelCmd_Push(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 5:
        if (gMain.keys & 2) {
            TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[1]);
            TextBoxSetMenu(1, 0, 0);
            {
                struct StepBits { u16 lo:2; u16 step:8; u16 hi:6; u8 pad[4]; };
                register u8 *duel asm("r2") = (u8 *)&SD; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register int selectedOffset asm("r3") = 0x1B2A;
                register int stepOffset asm("r4"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register struct StepBits *step asm("r2");
                asm("" : : "r"(duel), "r"(selectedOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                {
                    register u16 *selected asm("r1") = (u16 *)((u32)duel + selectedOffset); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    asm("" : : "r"(selected));
                    *selected = 0;
                }
                stepOffset = 0x1B30;
                asm("" : : "r"(stepOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                step = (struct StepBits *)((u32)duel + stepOffset);
                {
                    register u32 mask asm("r0") = 0xFFFFFC03; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    register u16 value asm("r5");
                    asm("" : : "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    value = *(u16 *)step;
                    asm("" : : "r"(value)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    *(u16 *)step = (mask & value) | 4;
                }
            }
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (SD.selected != ((u8)SC.zone | ((u8)SC.player << 8)) && IsTributableMonster(SC.player, SC.zone)) {
                PlaySE(1);
                DuelCmd_Push(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = (SD.selected & 15) | (((u8)(SD.selected >> 8) & 15) << 4)
                    | (((SC.zone & 15) | ((SC.player & 15) << 4)) << 8);
                SD.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 6:
        if (special)
            QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x8080, faceUp);
        else
            QueueNormalSummon(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x8080, faceUp);
        SD.active = 0;
        break;
    case 10:
        if (!gTextBox.h14) {
            SD.active = 0;
            break;
        }
        TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[4]);
        SD.step++;
        break;
    case 11:
        if (gMain.keys & 2) {
            TextBoxOpen(0x206, 0x712, 0xB, gTributeSummonPrompts[0]);
            TextBoxSetMenu(1, 0, 0);
            SD.selected = 0;
            SD.step = 10;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (IsTributableMonster(SC.player, SC.zone)) {
                PlaySE(1);
                DuelCmd_Push(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = (SC.zone & 15) | ((SC.player & 15) << 4);
                SD.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 12:
        QueueNormalSummon(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x80, faceUp);
        SD.active = 0;
        break;
    case 20:
        if (gMain.keys & 2) {
            PlaySE(2);
            SD.active = 0;
        } else if (DuelCursor_PickTarget(0xF0)) {
            struct PlayerBits { u8 low:1; u8 player:1; u8 high:6; u8 pad[4]; };
            /* FAKEMATCH: retain the ROM duel-base and player-field lifetimes. */
            register struct SummonDuel *duel asm("r8");
            register struct PlayerBits *player asm("r6"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            ref.id = (duel = &SD)->cardId;
            ref.player = (player = (struct PlayerBits *)((u8 *)duel + 0x1B33))->player;
            if (EffectEquippedTributeCheck(&ref, SUMMON_POS)) {
                u32 message;
                PlaySE(1);
                message = player->player ? 0x8008 : 8;
                DuelCmd_Push(message, SC.player, SUMMON_EVENT_POS, 0);
                TributeMonster(player->player, SC.zone);
                QueueSpecialSummonFromHand(player->player, duel->sourceIndex, SC.zone, 0, faceUp);
                duel->active = 0;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 30: {
        FormatStr(text, gStrTributeFromField, (const char *)0x0822C720 + SummonCardId(SD.sequence + 0x172) * 0x40);
        TextBoxOpen(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    }
    case 31:
        if (DuelCursor_PickTarget(0xE0)) {
            {
                /* FAKEMATCH: keep the cursor in r8. */
                register struct SummonCursor *cursor asm("r8") = &SC;
                int player = cursor->player;
                {
                    int *zonePtr = &cursor->zone;
                    int zone = *zonePtr;
                    int pi = player & 1;
                    int offset = zone * 0x94 + pi * 0xD64;
                    u8 *zoneBase = (u8 *)gDuelZones;
                    struct DuelZone *card = (struct DuelZone *)(offset + (int)zoneBase);
                    u32 id = ID(card);
                    if (id != 0 && (card->flags6 & 2)) {
                        const u16 *numberPtr = &((const u16 *)0x08622AB4)[id & 0x7FF];
                        unsigned sequence = SDZ.sequence;
                        if (*numberPtr == sequence + 0x172) {
                            u32 message = 8;
                            if (player)
                                message = 0x8008;
                            {
                                u16 eventPlayer = player;
                                u8 *area = (u8 *)&cursor->area;
                                DuelCmd_Push(message, eventPlayer, ((u8)zone << 8) | *area, 0);
                            }
                            TributeMonster(player, zone);
                            SDZ.sequence++;
                            if (SDZ.sequence <= 2) {
                                SDZ.step--;
                            } else {
                                QueueSpecialSummonFromHand(SDZ.player, SDZ.sourceIndex, *zonePtr, 0, faceUp);
                                SDZ.active = 0;
                            }
                        } else {
                            PlaySE(3);
                        }
                    }
                }
            }
        }
        break;
    /* State 0 enters with sequence=0; state 41 repeats while sequence<=2. */
    case 40:
        switch (SD.sequence) {
        case 0: required = 0x2E1; break;
        case 1: required = 0x2F4; break;
        case 2: required = 0x320; break;
        }
        SD.choices = 0;
        if (CountActiveCardsOnField(0, 0x58A) == 0 && CountActiveCardsOnField(1, 0x58A) == 0
            && CountFaceUpMonstersByNumber(0, required) > 0)
            SD.choices |= 2;
        if (CountHandCardsByNumber(0, required) && (SD.sequence <= 1 || CountFreeMonsterZones(0) > 0))
            SD.choices |= 1;
        {
            register unsigned off asm("r1"); register unsigned raw asm("r0"); register unsigned shifted asm("r2"); register unsigned choices asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            raw = (u32)&SD; asm("" : "+r"(raw)); off = 0x1B30; asm("" : "+r"(off)); raw = *(u32 *)(raw + off);
            shifted = raw << 14;
            choices = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & 1) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (choices & 2) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gStrTributeFromFieldOrHand;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gCardNames;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        FormatStr(dst, fmt, (const char *)(offset + (u32)names));
                    }
                }
            }
        }
        {
            register unsigned off asm("r1"); register unsigned raw asm("r0"); register unsigned shifted asm("r2"); register unsigned choices asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            raw = (u32)&SD; asm("" : "+r"(raw)); off = 0x1B30; asm("" : "+r"(off)); raw = *(u32 *)(raw + off);
            shifted = raw << 14;
            choices = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & 1) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (!(choices & 2)) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gStrTributeFromHand;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gCardNames;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        FormatStr(dst, fmt, (const char *)(offset + (u32)names));
                    }
                }
            }
        }
        if (!(SD.choices & 1) && (SD.choices & 2))
            {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gStrTributeFromField;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gCardNumberToId;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gCardNames;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        FormatStr(dst, fmt, (const char *)(offset + (u32)names));
                    }
        {
        register char *t3 asm("r3"); /* FAKEMATCH: keep r3/r5 live across the test so reload picks the ROM spill registers. */
        register int t5 asm("r5");
        asm("" : "=r"(t3), "=r"(t5));
        if ((((struct { u8 prefix[0x1B30]; u32 w; } *)&SD)->w & 0x3C000) == 0) {
            SD.active = 0;
            break;
        }
        asm("" : : "r"(t3), "r"(t5));
        }
        TextBoxOpen(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 41: {
        int mask;
        switch (SD.sequence) {
        case 0: required = 0x2E1; break;
        case 1: required = 0x2F4; break;
        case 2: required = 0x320; break;
        }
        {
            register unsigned choiceBits asm("r1") = ((struct { u8 prefix[0x1B30]; u32 choices; } *)&SD)->choices << 14; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            register unsigned choices asm("r0") = choiceBits >> 28;
            mask = 1;
            mask &= choices;
            asm("" : : "r"(choices), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            choiceBits = choices;
            asm("" : : "r"(choiceBits), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            if (choiceBits & 2)
                mask |= 0xE0;
        }
        if (DuelCursor_PickTarget(mask)) {
            int player = SC.player;
            int zone = SC.zone;
            int id = DuelCursor_GetCardId();
            if (id) {
                if (SUMMON_NUMBER(id) == required) {
                    u32 message = SC.player ? 0x8008 : 8;
                    DuelCmd_Push(message, SC.player, SUMMON_EVENT_POS, 0);
                    switch (SC.area) {
                    case 11: DiscardHandCard(player, zone, 0, 0); break;
                    case 0: TributeMonster(player, zone); break;
                    }
                    SD.sequence++;
                    if (SD.sequence <= 2)
                        SD.step--;
                    else
                        SD.step++;
                } else {
                    PlaySE(3);
                }
            }
        }
        break;
    }
    case 42:
        QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, faceUp);
        SD.active = 0;
        break;
    case 50:
        if (SUMMON_HUMAN(SC.player))
            FormatStr(format, gStrBanishFieldMonstersCount, gStrFiend);
        else
            FormatStr(format, gStrBanishGraveyardMonstersCount, gStrFiend);
        FormatInt(text, format, 3);
        TextBoxOpen(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 51:
    case 54:
    case 57:
        if (!SUMMON_HUMAN(SC.player)) {
            CardListView_Open(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (GetZoneCardType(SC.player, SC.zone) == 3) {
                BanishFieldCard(SC.player, SC.zone, 0);
                SDP.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 52:
    case 55:
        if (!SUMMON_HUMAN(SC.player))
            BanishGraveyardCard(SC.player, &gCardListView.cards[gCardListView.scroll + gCardListView.row]);
        SDP.step++;
        break;
    case 53:
        FormatInt(text, gStrCardsRemaining, 2);
        TextBoxOpen(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 56:
        FormatInt(text, gStrCardsRemaining, 1);
        TextBoxOpen(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 58:
        if (!SUMMON_HUMAN(SC.player))
            BanishGraveyardCard(SC.player, &gCardListView.cards[gCardListView.scroll + gCardListView.row]);
        SDP.step = 73;
        break;
    case 60:
        if (SUMMON_HUMAN(SC.player))
            FormatStr(format, gStrBanishFieldMonstersCount, gStrLight);
        else
            FormatStr(format, gStrBanishGraveyardMonstersCount, gStrLight);
        FormatInt(text, format, 2);
        TextBoxOpen(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 61:
    case 64:
        if (!SUMMON_HUMAN(SC.player)) {
            CardListView_Open(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (GetZoneCardAttribute(SC.player, SC.zone) == 1) {
                BanishFieldCard(SC.player, SC.zone, 0);
                SDP.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 62:
        if (!SUMMON_HUMAN(SC.player))
            BanishGraveyardCard(SC.player, &gCardListView.cards[gCardListView.scroll + gCardListView.row]);
        SDP.step++;
        break;
    case 63:
        FormatInt(text, gStrCardsRemaining, 1);
        TextBoxOpen(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 65:
        if (!SUMMON_HUMAN(SC.player))
            BanishGraveyardCard(SC.player, &gCardListView.cards[gCardListView.scroll + gCardListView.row]);
        SDP.step = 73;
        break;
    case 70:
        switch (SUMMON_NUMBER(SD.cardId)) {
        case 0x5EC:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrFire);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrFire);
            break;
        case 0x5ED:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrWater);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrWater);
            break;
        case 0x5EE:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrEarth);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrEarth);
            break;
        case 0x5EF:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                FormatStr(text, gStrBanishFieldMonster, gStrWind);
            else
                FormatStr(text, gStrBanishGraveyardMonster, gStrWind);
            break;
        }
        TextBoxOpen(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 71:
        if (!SUMMON_HUMAN(SC.player)) {
            CardListView_Open(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (DuelCursor_PickTarget(0xF0)) {
            int kind = 0;
            switch (SUMMON_NUMBER(SDP.cardId)) {
            case 0x5EC: kind = 4; break;
            case 0x5ED: kind = 3; break;
            case 0x5EE: kind = 5; break;
            case 0x5EF: kind = 6; break;
            }
            if (GetZoneCardAttribute(SC.player, SC.zone) == kind) {
                BanishFieldCard(SC.player, SC.zone, 0);
                SD.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 72:
        if (!SUMMON_HUMAN(SC.player))
            BanishGraveyardCard(SC.player, &gCardListView.cards[gCardListView.scroll + gCardListView.row]);
        SDP.step++;
        break;
    case 73:
        QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, FindFreeMonsterZone(SD.player), 0, faceUp);
        SD.active = 0;
        break;
    case 80: {
        int player = SD.player;
        int number1 = 0x582;
        u16 first = CountMonstersByNumber(player, number1) != 0;
        int player2 = SD.player;
        int number2 = 0x584;
        u16 second = CountMonstersByNumber(player2, number2) != 0;
        if (first) {
            if (second) {
                FormatStr(format, gStrTributeEitherFromField, SUMMON_CARD_NAME(SummonCardId(number1)));
                /* A plain table read: SummonCardId's folded branches above end the CSE block, so the table
                 * constant is not kept in a register across the call (the ROM reloads it). */
                FormatStr(text, format, SUMMON_CARD_NAME(((const u16 *)0x08623DF4)[number2]));
                TextBoxOpen(0x206, 0x712, 0xB, text);
                SD.step++;
            } else {
                /* Integer name-table base: reload loads it here, as in the ROM. */
                FormatStr(text, gStrTributeOneFromField, (const char *)0x0822C720 + SummonCardId(number1) * 0x40);
                TextBoxOpen(0x206, 0x712, 0xB, text);
                SD.step++;
            }
        } else if (second) {
            /* Integer name-table base: reload loads it here, as in the ROM. */
            FormatStr(text, gStrTributeOneFromField, (const char *)0x0822C720 + SummonCardId(number2) * 0x40);
            TextBoxOpen(0x206, 0x712, 0xB, text);
            SD.step++;
        } else {
            SD.active = 0;
        }
        break;
    }
    case 81:
        if (gMain.keys & 2) {
            SD.step = 80;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if (IsTributableMonster(SC.player, SC.zone)) {
                /* The ROM uses cursor.area for this player term; preserve it. */
                int player = SC.area & 1;
                int zone = SC.zone;
                u32 word = *(u32 *)(player * 0xD64 + zone * 0x94 + (u32)gDuelZones);
                u16 number = *(u16 *)((u8 *)gCardIdToNumber + ((word << 21) >> 20));
                if (number == 0x582 || number == 0x584) {
                    SDZ.selected = SC.zone;
                    /* FAKEMATCH: retain the number through the selection store. */
                    asm("" : : "r"(number));
                    PlaySE(1);
                    DuelCmd_Push(8, SC.player, SUMMON_EVENT_POS, 0);
                    SDZ.step++;
                } else {
                    PlaySE(3);
                }
            } else {
                PlaySE(3);
            }
        }
        break;
    case 4:
    case 82:
        TextBoxOpen(0x206, 0x412, 0xB, gTributeSummonPrompts[3]);
        SD.step++;
        break;
    case 83:
        if (gMain.keys & 2) {
            SD.step = 80;
        } else if (DuelCursor_PickTarget(0xF0)) {
            if ((u8)SD.selected != (u8)SC.zone && IsTributableMonster(SC.player, SC.zone)) {
                PlaySE(1);
                DuelCmd_Push(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = ((u8)SC.zone << 8) | (u8)SD.selected;
                SD.step++;
            } else {
                PlaySE(3);
            }
        }
        break;
    case 84:
        TributeMonster(SD.player, (u8)SD.selected);
        TributeMonster(SD.player, SD.selected >> 8);
        QueueSpecialSummonFromHand(SD.player, SD.sourceIndex, (u8)SD.selected & 7, 0, faceUp);
        SD.active = 0;
        break;
    default:
        SD.active = 0;
        break;
    }
}

