#include "global.h"

extern u8 gDuel[];
extern u8 gChain[];
extern const u16 gUnk_08624A0A[];
void QueueFlipSummon(int player, int zone);
void ChangeBattlePosition(int a, int b, int c, int d);
extern const u32 gCardStats[];
extern u8 gDuelZones[];
int CanActivateEffectOfCard(int player, u16 id, u16 x);
int CanActivateEffectInZone(int player, int zone, u16 kind);
int CountTributableMonsters(int player, int exclude);
u16 FindAbsorbedMonsterLink(int player, int zone);
int FindFreeSpellTrapZone(int player);
u16 CanCardTargetZone(u16 id, int player, int zone);
int CountActiveCardsOnField(int player, u16 number);
int CountZoneLinksFromCard(int player, int zone, u16 number);
int CanPlaceSpellTrapCard(int player, u16 id);
u32 IsSpecialSummonOnly(u16 id);
int CanSummonFromHand(int player, u16 id);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
u16 CountMonsters(int player);
int CountFreeMonsterZones(int player);
int CanSpecialSummon(int player);
int CanNormalSummon(int player);
void DuelCursor_Select(int player, int a, int zone);
void DestroyFieldCard(int player, int a, int b);
void ShowCardEffect(int player, u16 id);
void LoseLifePoints(int player, int lp);
void Chain_AddLink(u32 event, u32 value);
void Chain_AddPending(u32 event, int value);
void DuelCmd_Push(u16 msg, u16 card, u16 packed, u16 zero);
void PayChainEnergyCost(int player);
extern u8 gDuelFieldZone[];
extern u16 gUnk_0862467A;
/* Field zone (0x94 bytes) with the fields used by the usability test. */
struct ZoneF { u8 pad0[6]; u8 b6; u8 pad7[0x91 - 7]; u8 b91; u8 pad92[0x94 - 0x92]; };
struct EB { u8 pad[0x310]; u8 zb[1]; };
struct ZoneG { u8 pad0[6]; u8 b6; u8 pad7[0x94 - 7]; };
struct ZonesF { struct ZoneF z[11]; u8 filler[0xD64 - 11 * 0x94]; };
struct PlB7 { u8 pad[7]; u8 b7; u8 rest[0xD64 - 8]; };
extern struct PlB7 gDuelPlayers[];

/* Bit flags at 0x020192E0 + 0x1B2C..0x1B34 describing a pending zone request (hypothesis). */
struct ReqFlags { u8 b0 : 1; u8 b1 : 1; u8 rest : 6; u8 pad[7]; };           /* +0x1B2C */
struct ReqStep { u16 lo : 2; u16 cnt : 8; u16 hi : 6; u8 pad[6]; };          /* +0x1B30 */
struct ReqPlayer { u8 lo : 1; u8 player : 1; u8 hi : 6; u8 pad[6]; };  /* +0x1B33 */
struct ReqZone { u16 lo : 1; u16 zone : 8; u16 hi : 7; u8 pad[6]; };  /* +0x1B34 */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];
};
int EffectPolymerizationResolve(struct CardRef *ref, int a);

struct ZoneB7 { u8 pad[7]; u8 b7; u8 rest[0x94 - 8]; };
struct PlZones { struct ZoneB7 z[11]; u8 filler[0xD64 - 11 * 0x94]; };
#define REQ_PLAYER(e) (((struct ReqPlayer *)((e) + 0x1B33))->player)
#define REQ_ZONE(e) (((struct ReqZone *)((e) + 0x1B34))->zone)
#define ZONE_BYTE7(e, p, z) (*(u8 *)((z) * 0x94 + (p) * 0xD64 + ((e) + 0x2C) + 7))

/* --- CanActivateMonsterEffect support --- */
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_STATS_N(n) (((const u32 *)0x08621DE0)[(n)])
#define CARD_NUM_N(n) (((const u16 *)0x08622AB4)[(n)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define PLAYER_BASE(p) ((u8 *)gDuelPlayers + ((p) & 1) * 0xD64)
#define DECK_COUNT(p) (*(u8 *)(PLAYER_BASE(p) + 3))
#define DECK_WORD(p, i) (((u32 *)PLAYER_BASE(p))[0x7C4 / 4 + (i)])
#define LIST_COUNT(p) (*(u8 *)(PLAYER_BASE(p) + 4))
#define LIST_WORD(p, i) (((u32 *)PLAYER_BASE(p))[0x904 / 4 + (i)])
#define ZONE7(p, z) ((((u8 *)gDuelZones)[(z) * 0x94 + ((p) & 1) * 0xD64 + 7] >> 5) & 1)
/* Second request zone field: bits 9-16 of the word at 0x020192E0+0x1B34 (CardMenu_PlaySpellTrapFromHand). */
#define REQ_ZONE2(e) (((*(u32 *)((e) + 0x1B34)) << 15) >> 24)

/* Resolve the pending request: run QueueFlipSummon on its zone, mark the zone (byte +7 bit 2) and clear the request flag. */
void CardMenu_FlipSummon(void)
{
    u8 *e = gDuel;
    struct ZoneB7 *z;
    u32 p, zi;
    int s1;
    u8 *zb;
    QueueFlipSummon(REQ_PLAYER(e), REQ_ZONE(e));
    p = REQ_PLAYER(e);
    zi = REQ_ZONE(e);
    s1 = zi * 0x94 + p * 0xD64;
    zb = e + 0x2C;
    z = (struct ZoneB7 *)(s1 + (int)zb);
    z->b7 |= 4;
    ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
}

/* One field zone (0x94 bytes); the first word is the card: bits 0-11 id, bit 12 owner, bit 18 flag. */
struct Zone49 {
    u32 card;
    u8 rest[0x94 - 4];
};
/* Per-player duel state (0xD64 bytes, two of them at 0x020192E4). */
struct Player49 {
    u8 pad0[9];
    u8 flags9_lo : 5;
    u8 flags9_5 : 1;  /* +0x09 bit 5 */
    u8 flags9_hi : 2;
    u8 pad0A[0x28 - 0xA];
    struct Zone49 zones[11]; /* +0x28 */
    u8 filler[0xD64 - 0x28 - 11 * 0x94];
};
/* The duel state at 0x020192E0, seen as one struct (players, phase byte, pending request). */
struct Duel49 {
    u32 pad0;
    struct Player49 players[2]; /* +0x004 */
    u8 pad1ACC[0x1B12 - 0x1ACC];
    u8 f1B12_lo : 2; /* +0x1B12 */
    u8 phase : 3;
    u8 f1B12_hi : 3;
    u8 pad1B13[0x1B28 - 0x1B13];
    u16 card; /* +0x1B28: requested card */
    u16 pad1B2A;
    u8 f2C_0 : 1; /* +0x1B2C */
    u8 reqFlag : 1;
    u8 f2C_hi : 6;
    u8 pad1B2D[3];
    union {
        struct { u16 lo : 2; u16 step : 8; u16 hi : 6; } s;            /* step counter */
        struct { u32 lo : 25; u32 player : 1; u32 hi : 6; } w;         /* player, word view */
        struct { u8 pad[3]; u8 lo : 1; u8 player : 1; u8 hi : 6; } b; /* player, byte view */
        struct { u8 pad[3]; u8 v; } v;
    } r30; /* +0x1B30 */
    union {
        struct { u16 lo : 1; u16 zone : 8; u16 hi : 7; } h;
        struct { u32 lo : 9; u32 zone2 : 8; u32 hi : 15; } w;
    } z; /* +0x1B34 */
};
#define D49 (*(struct Duel49 *)gDuel)
struct Zone2W49 { u32 lo : 9; u32 zone2 : 8; u32 hi : 15; };
/* FAKEMATCH: the ROM extracts the second zone through r2 before masking it into r1. */
#define ZONE2_R2_49 ({ register u32 z_ asm("r2") = D49.z.w.zone2; asm("" : "+r"(z_)); z_; })

static inline int Low4_49(u8 x) { return x & 0xF; }
static inline int Hi4_49(u8 x) { return (x & 0xF) << 4; }
static inline void Lose49(u8 p) { ShowCardEffect(p, gUnk_0862467A); }
static inline u32 ZoneCard49(u8 p, u8 z) { return D49.players[p].zones[z].card; }

/*
 * Request step driver on the step counter at 0x020192E0+0x1B30 (bits 2-9). Step 0 picks a free
 * spell/trap zone (FindFreeSpellTrapZone) into the second zone field (0x1B34 bits 9-16); for trap subtype 2
 * it runs DestroyFieldCard(p, 10, 0) on both players whose zone 10 holds a card and forces zone 10;
 * then it emits message 0xC5 (0x80C5 for player 1) and advances. Step 1 calls
 * DuelCursor_Select(player, 0, zone) and advances. Later steps (when arg0 is set) handle a flagged
 * card owned by the other side (ShowCardEffect / LoseLifePoints), feed the request to
 * Chain_AddLink / Chain_AddPending, mark the player during phases 2+ and clear the request flag.
 */
void CardMenu_PlaySpellTrapFromHand(u16 arg0, u16 arg1, struct CardRef *arg2)
{
    switch (D49.r30.s.step) {
    case 0:
        ((struct Zone2W49 *)((u8 *)&D49 + 0x1B34))->zone2 = (u16)FindFreeSpellTrapZone(D49.r30.b.player);
        {
            u32 st = CARD_STATS(D49.card);

            if (((st & 0x1F00000) >> 20) == 0x16 && ((st & 0xE0000) >> 17) == 2) {
                int i;

                for (i = 0; i <= 1; i++) {
                    int pp = i ? 1 - D49.r30.b.player : D49.r30.b.player;

                    if ((*(u32 *)((u8 *)&D49 + 0x5F4 + (pp & 1) * 0xD64) << 20) != 0)
                        DestroyFieldCard(pp, 10, 0);
                }
                ((struct Zone2W49 *)((u8 *)&D49 + 0x1B34))->zone2 = 10;
            }
        }
        DuelCmd_Push((2 & D49.r30.v.v) ? 0x80C5 : 0xC5, D49.card,
                     Hi4_49(D49.z.h.zone) | Low4_49(D49.z.w.zone2) | (1 & arg0) << 8, 0);
        PayChainEnergyCost(D49.r30.b.player);
        D49.r30.s.step++;
        break;
    case 1:
        DuelCursor_Select(D49.r30.b.player, 0, D49.z.w.zone2);
        D49.r30.s.step++;
        break;
    default:
        if (arg0) {
            if ((int)(ZoneCard49(1 & D49.r30.b.player, D49.z.w.zone2) << 13) < 0) {
                if (((ZoneCard49(1 & D49.r30.b.player, D49.z.w.zone2) << 19) >> 31) != D49.r30.w.player) {
                    /* FAKEMATCH: the loop note ends the cse block, so the next argument is reloaded. */
                    do {
                        Lose49(D49.r30.w.player);
                    } while (0);
                    LoseLifePoints(D49.r30.b.player, 2000);
                }
            }
            if (arg1) {
                Chain_AddLink(D49.r30.b.player << 31 | arg2->kind << 25 | (0x1F & ZONE2_R2_49) << 16 | D49.card,
                             arg2->unk8 << 16 | arg2->pos);
            } else if (arg2 == NULL) {
                Chain_AddPending(D49.r30.b.player << 31 | (0x1F & ZONE2_R2_49) << 16 | D49.card, 0);
            } else {
                Chain_AddPending(D49.r30.b.player << 31 | arg2->kind << 25 | (0x1F & ZONE2_R2_49) << 16 | D49.card,
                             arg2->unk8 << 16 | arg2->pos);
            }
        }
        if (D49.phase > 1) {
            struct Player49 *pl = (struct Player49 *)((u8 *)&D49 + 4);

            pl[D49.r30.b.player].flags9_5 = 1;
        }
        D49.reqFlag = 0;
        break;
    }
}
/* Like CardMenu_FlipSummon but with a step argument: for step 1 or 2 run ChangeBattlePosition on the request zone first. */
void CardMenu_ChangePosition(u16 step)
{
    u8 *e;
    struct ZoneB7 *z;
    u32 p, zi;
    int s1;
    u8 *zb;
    switch (step) {
    case 1:
    case 2: {
        u8 *e1 = gDuel;
        ChangeBattlePosition(REQ_PLAYER(e1), REQ_ZONE(e1), 0, 0);
    }
    }
    e = gDuel;
    p = REQ_PLAYER(e);
    zi = REQ_ZONE(e);
    s1 = zi * 0x94 + p * 0xD64;
    zb = e + 0x2C;
    z = (struct ZoneB7 *)(s1 + (int)zb);
    z->b7 |= 4;
    ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
}
/* Two-step request driver on the step counter at 0x020192E0+0x1B30 (bits 2-9): step 0 sets 02017A40[0x3E0] = 0x80 and advances,
 * step 1 runs the picker EffectPolymerizationResolve on a new CardRef and advances when it returns 0; other steps clear the request flag. */
void CardMenu_FusionSummon(void)
{
    struct CardRef ref;
    u8 *e = gDuel;

    switch (((struct ReqStep *)(e + 0x1B30))->cnt) {
    case 0:
        gChain[0x3E0] = 0x80;
        ((struct ReqStep *)(e + 0x1B30))->cnt++;
        /* fall through */
    case 1: {
        u16 id = gUnk_08624A0A[0];

        ref.id = id;
        ref.player = 0;
        ref.skip4 = 0;
        gChain[0x3E0] = EffectPolymerizationResolve(&ref, 0);
        if (gChain[0x3E0] == 0) {
            u8 *e2 = gDuel; /* a fresh base: reusing e keeps it live across the call */

            ((struct ReqStep *)(e2 + 0x1B30))->cnt++;
        }
        break;
    }
    default:
        ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
        break;
    }
}
struct Flags514 { u8 f0 : 1; u8 bit1 : 1; u8 phase : 3; u8 rest : 3; };
/* CountMonsters returns int (see src/duel_card_lists.c); this unit declares it u16. */
#define CN8860(p) (((int (*)(int))CountMonsters)(p))
/*
 * Usability flags for card `id` held by `player`. Only when the duel phase (0x020192E0+0x1B12
 * bits 2-4) is 2 or 4: traps (type 0x16) get 0x10 (+0x40 when CanActivateEffectOfCard allows it), with
 * card numbers 0x520 and 0x605-0x608 special-cased; magic (0x15) gets 0x10; other cards go
 * through CanSummonFromHand / IsSpecialSummonOnly and the CanSpecialSummon / CanNormalSummon masks, then card
 * numbers 0x47 / 0x1A8 add 0x40. The shared tail adds 0x40 for trap subtype 5, drops it when
 * either side has card 0x49C, and clears 0x10/0x40 for spells and traps when player byte +7
 * bits 6-7 are set.
 *
 * The explicit `(u16)(flags | C)` forms and the one plain `flags |= 0x1800` are not
 * interchangeable here: the ROM keeps the lsl/lsr truncation after every OR except that one.
 */
int CardMenu_GetHandCardCommands(u16 id, int player)
{
    u16 flags;
    u8 *e;
    struct CardRef ref; /* FAKEMATCH: unused, but the ROM reserves its 0x14-byte stack slot */
    u32 n;
    u8 *pb;
    int t;
    u8 *e2;

    flags = 0;
    e = gDuel;
    switch (((u32)e[0x1B12] << 27) >> 29) {
    case 2:
    case 4:
        n = 0x7FF & id;
        switch ((((const u32 *)0x08621DE0)[n] & 0x1F00000) >> 20) {
        case 0x16:
            if (CanPlaceSpellTrapCard(player, id) == 0)
                break;
            if (CanActivateEffectOfCard(player, id, 1) != 0)
                flags = 0x40;
            flags = (u16)(flags | 0x10);
            switch (((const u16 *)0x08622AB4)[n]) {
            case 0x605:
            case 0x606:
            case 0x607:
            case 0x608:
                flags &= 0xFFEF;
                break;
            case 0x520:
                {
                    /* e + 4 first, as a separate term (also fixes later reload registers) */
                    u8 *zb = e + 4;
                    int s1 = (player & 1) * 0xD64;
                    pb = (u8 *)(s1 + (int)zb);
                }
                if ((pb[9] << 26) < 0 || (pb[8] << 27) < 0)
                    flags &= 0xFFBF;
                break;
            }
            break;
        case 0x15:
            if (CanPlaceSpellTrapCard(player, id) != 0)
                flags = 0x10;
            break;
        default:
            if (CanSummonFromHand(player, id) != 0) {
                if (IsSpecialSummonOnly(id) != 0) {
                    flags |= 0x1800;
                    if ((u16)CanSpecialSummon(0) == 0)
                        flags = 0;
                } else {
                    if ((e[(player & 1) * 0xD64 + 0xC] << 27) >= 0)
                        flags = 0x30;
                    switch (((const u16 *)0x08622AB4)[n]) {
                    case 0x17A:
                        if (CountFaceUpMonstersByNumber(player, 0x4DB) <= 0)
                            break;
                    case 0x5F0:
                        flags = (u16)(flags | 0x1800);
                        break;
                    case 0x546:
                        if (CN8860(0) + 1 < CN8860(1) && CountFreeMonsterZones(0) > 0)
                            flags = (u16)(flags | 0x1800);
                        if (CountTributableMonsters(player, -1) == 0)
                            flags &= 0xFFCF;
                        break;
                    }
                }
                if ((u16)CanSpecialSummon(0) == 0)
                    flags &= 0xE7FF;
                if ((u16)CanNormalSummon(0) == 0)
                    flags &= 0xFFDF;
            }
            switch (((const u16 *)0x08622AB4)[0x7FF & id]) {
            case 0x47:
                if (CanPlaceSpellTrapCard(player, id) != 0 && CanActivateEffectOfCard(player, id, 1) != 0 &&
                    CountFreeMonsterZones(player) > 0 &&
                    (gDuelPlayers[player & 1].rest[0] << 27) >= 0)
                    flags = (u16)(flags | 0x40);
                break;
            case 0x1A8:
                if (CanActivateEffectOfCard(player, id, 1) != 0)
                    flags = (u16)(flags | 0x40);
                break;
            }
            break;
        }
        break;
    }
    e2 = gDuel; /* a fresh base: the ROM reloads 0x020192E0 and adds 0x1B12 here */
    if (((struct Flags514 *)(e2 + 0x1B12))->bit1 == 0) {
        u32 st = ((const u32 *)0x08621DE0)[0x7FF & id];
        if (((st & 0x1F00000) >> 20) == 0x16 && ((st & 0xE0000) >> 17) == 5 &&
            CanPlaceSpellTrapCard(player, id) != 0 && CanActivateEffectOfCard(player, id, 1) != 0)
            flags = (u16)(flags | 0x40);
    }
    if (((((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20) == 0x16 && (flags & 0x40) != 0) {
        if (CountActiveCardsOnField(0, 0x49C) > 0)
            flags &= 0xFFBF;
        if (CountActiveCardsOnField(1, 0x49C) > 0)
            flags &= 0xFFBF;
    }
    t = (((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20;
    switch (t) {
    case 0x15:
    case 0x16:
        if (gDuelPlayers[player & 1].b7 >> 6 != 0) {
            flags &= 0xFFEF;
            flags &= 0xFFBF;
        }
        break;
    }
    return flags;
}
/*
 * Usability lookup for the spell/trap command menu: given a card id, a player and a spell/trap
 * zone, return a flag (or a CanActivateEffectInZone / CountTributableMonsters result) depending on the card's number.
 * Only the listed card numbers reach a special case; everything else returns 0.
 */
struct Z880 { u8 pad[7]; u8 lo7 : 5; u8 flag : 1; u8 hi7 : 2; u8 rest[0x94 - 8]; };
static inline int GetCardType880(u16 id) { return (((u32 *)0x08621DE0)[(id) & 0x7FF] & 0x1F00000) >> 20; }
#define CTYPE880(id) GetCardType880(id)

int CanActivateMonsterEffect(u16 id, int player, int zone)
{
    struct CardRef ref;
    u32 flag;
    int i;
    int target;

    flag = ((struct Z880 *)((player & 1) * 0xD64 + zone * 0x94 + (int)((u8 *)gDuelPlayers + 0x28)))->flag;
    ref.player = player;
    ref.id = id;

    switch (CARD_NUM(id)) {
    case 0x58:
    case 0x1FF:
        return CountTributableMonsters(player, -1) > 0;
    case 0x105:
        return CountTributableMonsters(player, zone) > 0;

    case 0xF:
    case 0x191:
    case 0x1A3:
    case 0x1AC:
    case 0x1F9:
    case 0x2E6:
    case 0x34D:
    case 0x458:
    case 0x596:
    case 0x598:
    case 0x599:
    case 0x59F:
    case 0x5A2:
    case 0x5A4:
    case 0x5E6:
        return ((u16 (*)(int, int, u16))CanActivateEffectInZone)(player, zone, 0);

    case 0x1A0:
    case 0x243:
    case 0x2DB:
        if ((gDuel[0x1B12] & 0x1C) != 4)
            return 0;
        return ((u16 (*)(int, int, u16))CanActivateEffectInZone)(player, zone, 2);

    case 0x51:
    case 0x186:
        if (CountZoneLinksFromCard(player, zone, 0x291) == 0)
            return 0;
        target = 0;
        switch (CARD_NUM(id)) {
        case 0x51:
            target = 0x2E5;
            break;
        case 0x186:
            target = 0x187;
            break;
        }
        if (target <= 0)
            return 0;
        for (i = 0; i < gDuelPlayers[player & 1].pad[3]; i++) {
            if (CARD_NUM((((u32 *)((u8 *)gDuelPlayers + 0x7C4 + (player & 1) * 0xD64))[i] << 20) >> 20) == target) {
                if (CountActiveCardsOnField(0, 0x58A) > 0)
                    return 0;
                if (CountActiveCardsOnField(1, 0x58A) > 0)
                    return 0;
                return 1;
            }
        }
        return 0;

    case 0x2DA:
    case 0x536:
        if (((int (*)(int, int))FindAbsorbedMonsterLink)(player, zone) != 0xFFFF)
            return 0;
        if (FindFreeSpellTrapZone(player) == -1)
            return 0;
        for (i = 0; i <= 4; i++) {
            if (((int (*)(u16, int, int))CanCardTargetZone)(id, 1 - player, i) != 0)
                return flag;
        }
        return 0;

    case 0x5E9:
        if (flag == 0)
            return 0;
        for (i = 0; i < gDuelPlayers[player & 1].pad[4]; i++) {
            if ((u32)CTYPE880((((u32 *)((u8 *)gDuelPlayers + 0x904 + (player & 1) * 0xD64))[i] << 20) >> 20) <= 0x14)
                return 1;
        }
        return 0;

    default:
        return 0;
    }
}
/* Usability flag builder for a card in spell/trap zone `arg2` of `arg1` (only player 0 is handled). */
int CountActiveCardsOnField(int player, u16 number);
int GetZoneCardType(int player, int zone);
int CountZoneLinksFromCard(int player, int zone, u16 number);
int CanNormalSummon(int player);
int CanActivateMonsterEffect(u16 id, int player, int zone);
int CanMonsterAttack(int player, int zone, int a);
struct DZone { u32 w0; u8 unk4; u8 unk5; u8 b6; u8 b7; u8 rest[0x94 - 8]; };
struct DZones { struct DZone z[11]; u8 filler[0xD64 - 11 * 0x94]; };
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define ZONE_ADDR(p, z) ((struct DZone *)(0xD64 * ((p) & 1) + (u32)gDuelZones + 0x94 * (z)))
#define ZONE_R(p, z) ((struct DZone *)(0x94 * (z) + 0xD64 * ((p) & 1) + (u32)gDuelZones))
struct WfPl49B74 { u8 pad[0x2A]; u16 f2A; u8 rest[0xD64 - 0x2C]; };
struct WfDuel49B74 { struct WfPl49B74 p[2]; };
u16 CardMenu_GetMonsterCommands(u16 arg0, int arg1, int arg2)
{
    u16 id = arg0;
    int s1 = 0xD64 * (arg1 & 1);
    u32 base = (u32)gDuelZones;
    struct DZone *z = (struct DZone *)(s1 + base + 0x94 * arg2);
    u16 flags = 0;
    int found = 0;
    u8 *e;
    if (arg1 != 0)
        return 0;
    if ((ZONE_R(0, arg2)->b6 & 2) != 0 && GetZoneCardType(0, arg2) == 1 &&
        (CountActiveCardsOnField(0, 0x148) != 0 || CountActiveCardsOnField(1, 0x148) != 0))
        found = 1;
    e = gDuel;
    switch ((int)((u32)(e[0x1B12] << 27) >> 29)) {
    case 1: {
        int t = arg1 & 1;
        int s1 = arg2 * 0x94 + t * 0xD64;
        u8 *zb = e + 0x2C;
        struct DZone *zn = (struct DZone *)(s1 + (int)zb);
        if ((zn->b6 & 2) != 0) {
            u16 cid = CARD_NUM(((u32)zn->w0 << 21) >> 21);
            switch (cid) {
            case 0x1A0:
            case 0x243:
            case 0x2DB:
                if (CanActivateEffectInZone(arg1, arg2, 2) != 0)
                    flags |= 0x40;
                break;
            }
        }
        break;
    }
    case 2:
    case 4:
        if ((z->b7 & 4) == 0 && (s32)(gDuelPlayers[arg1 & 1].b7 << 26) >= 0 &&
            CountZoneLinksFromCard(arg1, arg2, 0x15C) == 0 &&
            CountZoneLinksFromCard(arg1, arg2, 0x4DC) == 0 && found == 0) {
            if ((z->b6 & 2) != 0) {
                if ((z->b6 & 1) != 0)
                    flags = (u16)(flags | 4);
                else
                    flags = (u16)(flags | 2);
            } else {
                flags = (u16)(flags | 8);
                if ((z->b6 & 1) == 0)
                    flags = (u16)(flags | 2);
                if ((u16)CanNormalSummon(0) == 0)
                    flags &= 0xFFF7;
            }
            if (CountActiveCardsOnField(0, 0x536) > 0 || CountActiveCardsOnField(1, 0x536) > 0) {
                int t = arg1 & 1;
                int s2 = arg2 * 0x94 + t * 0xD64;
                if (CARD_NUM(((u32)((struct DZone *)(s2 + (u32)gDuelZones))->w0 << 21) >> 21) != 0x536)
                    flags &= 0xFFF1;
            }
        }
        {
            int t = arg1 & 1;
            int s2 = arg2 * 0x94 + t * 0xD64;
            if ((((struct DZone *)(s2 + (u32)gDuelZones))->b6 & 2) != 0 && (u16)CanActivateMonsterEffect(id, arg1, arg2) != 0)
                flags = (u16)(flags | 0x40);
        }
        break;
    case 3:
        asm("" : "+r"(e)); /* FAKEMATCH: hide e's constant value from CSE so `e + t*0xD64` keeps its operand order */
        if (CanMonsterAttack(arg1, arg2, 1) != 0 &&
            !(((((struct WfDuel49B74 *)e)->p[arg1 & 1].f2A) >> arg2) & 1))
            flags = (u16)(flags | 0x80);
        break;
    }
    return flags;
}
/* Which usability flags (bit 6 = can be activated) does the card `id` have when set in spell/trap zone `zone` + 5 of `player`?
 * Only player 0 is ever evaluated (hypothesis: "is a face-down trap/spell of the current player usable"). */
/* Duel flags byte at gDuel+0x1B12 (DuelState.linkSkip / phase1B12 in duel.h). */
struct Flags1B12 { u8 f0 : 1; u8 bit1 : 1; u8 phase : 3; u8 rest : 3; };
int CardMenu_GetSpellTrapCommands(u16 id, int player, int zone)
{
    u8 *e;
    u32 flags;
    u32 n;
    u32 st;
    int t;
    struct ZonesF *pz = &((struct ZonesF *)gDuelZones)[player & 1];
    struct ZoneF *zn = &pz->z[zone + 5];
    flags = 0;
    if (player != 0)
        return 0;
    n = 0x7FF & id;
    st = ((const u32 *)0x08621DE0)[n];
    switch ((st & 0x1F00000) >> 20) {
    case 0x16:
        if ((zn->b91 & 4) != 0) {
            if ((zn->b6 << 30) >= 0) {
                int f = 0;
                if (((st & 0xE0000) >> 17) == 5)
                    f = 1;
                {
                    struct Flags1B12 *fl = (struct Flags1B12 *)(gDuelZones + 0x1AE6);
                    if ((fl->phase == 2 || fl->phase == 4) && fl->bit1 == 0)
                        f = 1;
                }
                if (f != 0 && CanActivateEffectOfCard(player, id, 0) != 0)
                    flags |= 0x40;
            }
        }
        e = gDuel;
        {
            struct Flags1B12 *fl = (struct Flags1B12 *)(e + 0x1B12);
            if (fl->phase == 1) {
                if (fl->bit1) {
                    if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x489) {
                        if (CanActivateEffectInZone(1 - player, zone + 5, 3) != 0)
                            flags = (u16)(flags | 0x40);
                    }
                } else {
                    if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x428) {
                        int s1 = (player & 1) * 0xD64 + zone * 0x94;
                        u8 *zb = e + 0x310;
                        if ((((struct ZoneG *)(s1 + (int)zb))->b6 & 2) == 0) {
                            if (CanActivateEffectInZone(player, zone + 5, 2) != 0)
                                flags = (u16)(flags | 0x40);
                        }
                    }
                }
            }
        }
        break;
    case 0x15:
        if ((zn->b91 & 4) != 0) {
            int face = (u32)(zn->b6 << 30) >> 31;
            switch (((const u16 *)0x08622AB4)[n]) {
            case 0x52C:
            case 0x3F9:
            case 0x594:
            case 0x5FC:
                face = 0;
                break;
            }
            if (face == 0) {
                if (CanActivateEffectInZone(player, zone + 5, 0) != 0)
                    flags = (u16)(flags | 0x40);
            }
        }
        break;
    default:
        break;
    }
    t = (((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20;
    switch (t) {
    case 0x15:
    case 0x16:
        if (gDuelPlayers[player & 1].b7 >> 6 != 0)
            flags &= 0xFFBF;
        break;
    }
    return flags;
}

