#include "global.h"
#include "duel.h"

int CountZoneLinksFromCard(u32 player, u32 slot, u16 number);
/* Unit-specific views of bytes the shared headers split differently:
 * - ZoneCardBits / ZoneAuxBits: zone +0x00 bit 17 and zone +0x90 bits 13..17 have no
 *   canonical field in struct DuelCard / struct DuelZone.
 * - ZoneFlags: struct DuelZone splits +6 into bitfields, but this unit reads it as one
 *   byte and tests bit 1, so keep a byte view to preserve codegen. */
struct ZoneCardBits { u32 id:12; u32 pad:5; u32 flag17:1; u32 rest:14; };
struct ZoneAuxBits { u32 pad:13; u32 value:5; u32 rest:14; };
struct ZoneFlags { u8 pad[6]; u8 flags; u8 tail; };
struct ZoneDisabled { u8 pad[0x91]; u8 flags; };
struct ZoneAux { u8 pad[0x90]; u32 lo:13; u32 value:5; u32 hi:14; };
struct StatDuelFlags { u8 pad[0x1ACC]; u16 flags; };
struct StatDuelFlagBytes { u8 pad[0x1ACD]; u8 flags; };
#define ZONE_BASE(p, s) ((u8 *)gDuelZones + (s) * 0x94 + (p) * 0xD64)
#define ZONE_WORD(p, s) (((struct DuelCard *)ZONE_BASE(p, s))->id)
#define ZONE_FLAGS(p, s) (((struct ZoneFlags *)ZONE_BASE(p, s))->flags)

int CountMonstersAffectedByCard(u32 player, u16 number)
{
    int count = 0;
    int slot;
    for (slot = 0; slot <= 4; slot++) {
        if (ZONE_WORD(player & 1, slot) &&
            (ZONE_FLAGS(player & 1, slot) & 2) &&
            CountZoneLinksFromCard(player, slot, number))
            count++;
    }
    return count;
}
int FindMonsterAffectedByCard(u32 player, u16 number)
{
    int slot;
    for (slot = 0; slot <= 4; slot++) {
        if (ZONE_WORD(player & 1, slot) &&
            (ZONE_FLAGS(player & 1, slot) & 2) &&
            CountZoneLinksFromCard(player, slot, number))
            return slot;
    }
    return -1;
}
struct ZoneCardInfo {
    u16 id;
    u8 type:5;
    u8 attr:3;
    u8 pad3;
    s32 atk, def;
};
typedef char zone_card_info_size[sizeof(struct ZoneCardInfo) == 12 ? 1 : -1];
typedef char zone_card_info_attack_offset[(u32)&((struct ZoneCardInfo *)0)->atk == 4 ? 1 : -1];
typedef char zone_card_info_defense_offset[(u32)&((struct ZoneCardInfo *)0)->def == 8 ? 1 : -1];
extern u8 gDuelSpellTrapZones[];
extern u32 gCardStats[];
extern const u16 gCardIdToNumber[];
extern const s16 gFieldTypeBonuses[][24];
extern const s16 gFieldAttributeBonuses[][8];
int GetFaceUpFieldMagicNumber(void);
int CountActiveCardsOnField(int player, u16 number);
int CountFaceUpMonstersOfType(int player, u16 type);
int CountMonsters(int player);
int CountFaceUpMonstersByNumber(int player, u16 number);
int CountFaceUpMonstersByNumberInPosition(int player, u16 number, u16 flag);
int CountMonstersFiltered(int player, u16 faceUp, u16 bit0Clear);
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);
int CountAquaChorusBoosts(int player, int slot);
s32 HalveRoundUp(s32 value); /* round(value / 2), including signed division */
#define ZB(p, s) ((struct DuelZone *)ZONE_BASE(p, s))
#define CARD_STATS(id) (((u32 *)0x08621DE0)[(id) & 0x7FF])
/* A 16-bit card-ID boundary avoids folding repeated ID/table loads into
 * one expression in this reconstruction. */
static inline int GetCardType(u16 id) { return (CARD_STATS(id) & 0x1F00000) >> 20; }
#define CARD_TYPE(id) GetCardType(id)
static inline u16 GetCardNumber(int id) { return ((const u16 *)0x08622AB4)[id & 0x7FF]; }
/* The equip switch reads the number table through its symbol, so one register holds the
 * table address across the case bodies; elsewhere the ROM reloads a cast address. */
static inline u16 GetCardNumberSym(int id) { return *(gCardIdToNumber + (id & 0x7FF)); }
#define CARD_NUMBER(id) GetCardNumber(id)
#define ZONE_DISABLED(p, s) (((struct ZoneDisabled *)ZB(p, s))->flags & 8)
static inline u16 GetZoneAuxValue(struct DuelZone *z) { return ((struct ZoneAuxBits *)((u8 *)z + 0x90))->value; }
/* FAKEMATCH: keep the full-word read from the immutable ROM stats table. The ROM
 * reads a whole stats word here. Ordinary nonvolatile expressions narrow it to the
 * final byte. volatile preserves the observed read, but the original qualifier is unknown. */
static inline int GetCardAttribute(u16 id) { return ((const volatile u32 *)0x08621DE0)[id & 0x7FF] >> 29; }
static inline int BaseAttack(u16 id)
{
    switch (CARD_TYPE(id)) {
    case 21: case 22: case 23: return 0;
    case 24: return 4000;
    default: return ((CARD_STATS(id) >> 9) & 0x1FF) * 10;
    }
}
static inline int BaseDefense(u16 id)
{
    switch (CARD_TYPE(id)) {
    case 21: case 22: case 23: return 0;
    case 24: return 4000;
    default: return (CARD_STATS(id) & 0x1FF) * 10;
    }
}

void GetZoneCardStats(int player, int slot, struct ZoneCardInfo *out)
{
    int i, p;
    int atkHalves = 0, atkDoubles = 0, defHalves = 0, defDoubles = 0;
    int equipAtk = 0, equipDef = 0, equipCount = 0;
    int otherAtk = 0, otherDef = 0, addAtk = 0, addDef = 0;
    int pp = player & 1;
    u32 poff = pp * 0xD64;
    u8 *base = (u8 *)gDuelZones + poff;
    u32 zoff = slot * 0x94;
    struct DuelZone *zone = (struct DuelZone *)(base + zoff);
    u16 newest = 0, immune = 0;
    out->type = 0;
    out->attr = 0;
    out->atk = 0;
    out->def = 0;
    out->id = ZONE_WORD(player & 1, slot);
    if (!out->id) return;
    {
        int type = CARD_TYPE(out->id);
        type &= 31;
        ((u8 *)out)[2] = type;
        {
            int attr = GetCardAttribute(out->id) << 5;
            ((u8 *)out)[2] = (type & 31) | attr;
        }
    }
    out->atk = BaseAttack(out->id);
    out->def = BaseDefense(out->id);
    /* FAKEMATCH: retain the card-ID reload after base-stat initialization. */
    asm("" : "+m"(out->id));
    if (slot > 4 || !(ZONE_FLAGS(player & 1, slot) & 2)) return;
    switch (CARD_NUMBER(out->id)) {
    case 0x458:
        if (!(ZB(player & 1, slot)->unk7 & 0x20)) out->atk *= 2;
        break;
    case 0x4E6:
        if (!(ZB(player & 1, slot)->unk7 & 0x20)) { addAtk += 500; addDef += 500; }
        break;
    case 0x52E: case 0x531:
        if (GetFaceUpFieldMagicNumber() == 0x14D) immune = 1;
        break;
    }

    /* The newest qualifying type-changing card wins this preliminary pass. */
    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = (struct DuelZone *)(i*0x94+(player&1)*0xD64+(u32)gDuelZones);
        if (((struct DuelCard *)z)->id && CARD_NUMBER(((struct DuelCard *)z)->id) == 0x2FA &&
            ((struct ZoneCardBits *)z)->flag17 && (((u8 *)z)[6]&2) && z->serial > (u32)newest) {
            newest = z->serial;
            out->type = 10;
        }
        for (p = 0; p <= 1; p++) {
            struct DuelZone *z2 = ((struct DuelZone *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gDuelSpellTrapZones)));
            if (((struct DuelCard *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gDuelSpellTrapZones)))->id && CARD_NUMBER(((struct DuelCard *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gDuelSpellTrapZones)))->id) == 0x479 &&
                (((u8 *)z2)[6]&2) && !(((u8 *)z2)[0x91]&8) && z2->serial > (u32)newest) {
                newest = z2->serial;
                out->type = GetZoneAuxValue(z2);
            }
        }
    }
    /* Low kind byte identifies the modifier; high byte carries its value. */
    for (i = 0; i < ZB(player & 1, slot)->numLinks; i++) {
        u16 link = ZB(player & 1, slot)->links[i];
        u8 kind = ZB(player & 1, slot)->linkKinds[i];
        int value = ZB(player & 1, slot)->linkKinds[i] >> 8;
        u8 lp = link;
        int ls = link >> 8;
        u16 linkedId = ZONE_WORD(lp & 1, ls);
        switch (kind) {
        case 3:
            if (immune && CARD_TYPE(link) == 22) break;
            switch (CARD_NUMBER(link)) {
            case 0x105:
                otherAtk += (value + 1) * 700;
                break;
            case 0x1AC:
                if (ZB(player & 1, slot)->linkKinds[i] >> 8) atkDoubles++;
                else atkHalves++;
                break;
            case 0x3F7: otherAtk += (value + 1) * 500; break;
            case 0x3F8: otherDef += (value + 1) * 500; break;
            case 0x433: otherAtk += (value + 1) * 700; break;
            case 0x434: otherDef += (value + 1) * 700; break;
            case 0x43A: otherDef -= (value + 1) * 500; break;
            case 0x4B3:
                otherAtk += (ZB(player & 1, slot)->linkKinds[i] >> 8) * 100;
                otherDef += (ZB(player & 1, slot)->linkKinds[i] >> 8) * 100;
                break;
            case 0x4B4:
                otherAtk -= (ZB(player & 1, slot)->linkKinds[i] >> 8) * 100;
                otherDef -= (ZB(player & 1, slot)->linkKinds[i] >> 8) * 100;
                break;
            case 0x4CF: otherAtk -= (value + 1) * 500; break;
            case 0x522: atkDoubles++; break;
            case 0x587: otherAtk -= (value + 1) * 700; break;
            case 0x5FE:
                /* The ROM reuses the link-loop index for this list scan. */
                for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++)
                    if ((u32)CARD_TYPE(((struct DuelCard *)((u8 *)gDuelPlayers + (player & 1) * 0xD64 + 0x904))[i].id) <= 20) otherAtk += 100;
                break;
            }
            break;
        case 2:
            if (!linkedId || ZONE_DISABLED(lp & 1, ls)) break;
            switch (CARD_NUMBER(linkedId)) {
            case 0x52:
                addAtk += ((ZB(player & 1, slot)->linkKinds[i] >> 8) + 1) * 200;
                addDef += ((ZB(player & 1, slot)->linkKinds[i] >> 8) + 1) * 200;
                break;
            case 0x4DC: addAtk -= 700; break;
            case 0x44B: if (lp != player) atkHalves++; break;
            }
            break;
        case 1: {
            if (!linkedId) break;
            if (!ZONE_DISABLED(lp & 1, ls) && !CountActiveCardsOnField(0, 0x601) &&
                !CountActiveCardsOnField(1, 0x601) && !(*((u8 *)gDuelZones + 0x1AA1) & 3) &&
                (!immune || CARD_TYPE(linkedId) != 22)) {
                switch (GetCardNumberSym(linkedId)) {
                case 0x47:
                    if (GetCardNumberSym(out->id) == 0x115) { out->atk = 0; out->def = 2000; }
                    break;
                case 300: if (out->type == 15) { equipAtk += 300; equipDef += 300; } break;
                case 301: if (out->attr == 2) { equipAtk += 400; equipDef -= 200; } break;
                case 302: if (out->type == 3) { equipAtk += 300; equipDef += 300; } break;
                case 303: equipAtk += 1000; break;
                case 304: if (out->type == 10) { equipAtk += 300; equipDef += 300; } break;
                case 305: if (out->type == 10) equipAtk += 700; break;
                case 306: if (out->attr == 1) { equipAtk += 400; equipDef -= 200; } break;
                case 307: if (out->type == 11) { equipAtk += 300; equipDef += 300; } break;
                case 308: if (out->attr == 3) { equipAtk += 400; equipDef -= 200; } break;
                case 309: if (out->type == 13) { equipAtk += 300; equipDef += 300; } break;
                case 310: equipAtk += 500; break;
                case 311: if (out->type == 17) { equipAtk += 300; equipDef += 300; } break;
                case 312: equipDef += 800; break;
                case 313: equipAtk += 700; equipDef += 700; break;
                case 314: if (out->type == 1) { equipAtk += 300; equipDef += 300; } break;
                case 315: if (out->type == 19) { equipAtk += 300; equipDef += 300; } break;
                case 316:
                    if ((u16)(GetCardNumberSym(out->id) - 61) <= 1 || GetCardNumberSym(out->id) == 0x4E1) equipAtk += 500;
                    break;
                case 318: if (out->type == 12) { equipAtk += 300; equipDef += 300; } break;
                case 320: equipAtk += 700; break;
                case 321: if (out->type == 2) { equipAtk += 300; equipDef += 300; } break;
                case 322: if (out->type == 18) { equipAtk += 300; equipDef += 300; } break;
                case 323: if (out->attr == 5) { equipAtk += 400; equipDef -= 200; } break;
                case 324: if (out->type == 7) { equipAtk += 300; equipDef += 300; } break;
                case 325: if (out->type == 9) { equipAtk += 300; equipDef += 300; } break;
                case 326: if (out->type == 16) { equipAtk += 300; equipDef += 300; } break;
                case 327: if (out->type == 14) { equipAtk += 300; equipDef += 300; } break;
                case 650: equipAtk += 500; break;
                case 653: if (out->attr == 4) equipAtk += 700; break;
                case 656:
                    if (((struct DuelPlayer *)((u8 *)gDuelZones - 0x28))[lp & 1].lifePoints < ((struct DuelPlayer *)((u8 *)gDuelZones - 0x28))[(1 - lp) & 1].lifePoints) out->atk = BaseAttack(out->id) * 2;
                    if (gDuelPlayers[lp & 1].lifePoints > gDuelPlayers[(1 - lp) & 1].lifePoints) out->atk = HalveRoundUp(BaseAttack(out->id));
                    addAtk = 0;
                    otherAtk = 0;
                    break;
                case 657: equipAtk += 300; equipDef += 300; break;
                case 667: if (out->attr == 1) equipAtk += 700; break;
                case 962:
                    if (out->type == 7) {
                        switch (((struct ZoneAux *)ZB(lp & 1, ls))->value) {
                        case 1: equipAtk += 700; break;
                        case 2: equipDef += 700; break;
                        }
                    }
                    break;
                case 1012: if (out->attr == 4) { equipAtk += 400; equipDef -= 200; } break;
                case 1013: if (out->attr == 6) { equipAtk += 400; equipDef -= 200; } break;
                case 1042: equipAtk += 500; equipDef += 500; break;
                case 1046: if (out->type != 7) equipAtk -= ZB(lp & 1, ls)->counter6 * 300; break;
                case 1058: equipAtk -= 500; equipDef -= 500; break;
                case 1060: equipAtk += 700 - ZB(lp & 1, ls)->counter6 * 200; break;
                case 1182: if (out->type == 15) equipAtk += 700; break;
                case 1242: equipAtk -= 500; break;
                case 1420: equipAtk += 1000; equipDef -= 1000; break;
                case 1422: if (out->type == 15) equipAtk += 800; break;
                case 1448: out->attr = ((struct ZoneAux *)ZB(lp & 1, ls))->value; break;
                case 1449:
                    equipAtk += CountMonstersFiltered(lp, 1, 0) * 800;
                    equipDef += CountMonstersFiltered(lp, 1, 0) * 800;
                    break;
                case 1450:
                    equipAtk += CountSpellTrapsFiltered(lp, 0, 0, 0) * 500;
                    equipDef += CountSpellTrapsFiltered(lp, 0, 0, 0) * 500;
                    break;
                case 1540: if (GetCardNumberSym(out->id) == 0x53B) equipAtk += 300; break;
                case 1550:
                    if (out->type == 15) { out->type = 1; equipAtk += 500; equipDef += 500; }
                    /* lp % 2, not lp & 1: the byte-wide AND would share its constant 1 with the store below. */
                    if (ZB(lp % 2, ls)->serial > (u32)newest) out->type = 1;
                    break;
                }
            }
            if (linkedId) equipCount++;
            break;
        }
        case 13: addAtk += value * 100; addDef += value * 100; break;
        case 5:
            if (CARD_NUMBER(out->id) == 0x2DA || CARD_NUMBER(out->id) == 0x536) {
                out->atk = 0;
                out->def = 0;
                if (ZONE_FLAGS(lp & 1, ls)&2) { out->atk = BaseAttack(linkedId); out->def = BaseDefense(linkedId); }
            }
            break;
        case 4: if (!immune) out->atk += link; break;
        case 8:
            /* The sums go through a temporary: the ROM adds base stat + total, in that order. */
            { int t = BaseAttack(link) + addAtk; addAtk = t; }
            { int t = BaseDefense(link) + addDef; addDef = t; }
            break;
        case 9: addAtk -= (value + 1) * 500; addDef -= (value + 1) * 500; break;
        case 10: equipAtk += 200; break;
        case 11: equipAtk += value * 300; break;
        case 12: addAtk -= value * 200; break;
        }
    }
    /* Intrinsic monster effects are applied after all links. */
    switch (CARD_NUMBER(out->id)) {
    case 11: addAtk += CountActiveCardsOnField(player, 0x229) * 500; break;
    case 0x16F:
        for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++)
            if ((u32)CARD_TYPE(((struct DuelCard *)((u8 *)gDuelPlayers + (player & 1) * 0xD64 + 0x904))[i].id) <= 20) addAtk += 100;
        break;
    case 0xEA:
        addAtk += CountFaceUpMonstersOfType(0, 13) * 100;
        addAtk += CountFaceUpMonstersOfType(1, 13) * 100;
        break;
    case 0x181: {
        int a = CountActiveCardsOnField(0, 61), b = CountActiveCardsOnField(1, 61);
        int c = CountActiveCardsOnField(0, 0x4E1), d = CountActiveCardsOnField(1, 0x4E1);
        addAtk += (a + b + c + d) * 300; addDef += (a + b + c + d) * 300;
        break;
    }
    case 0x196:
        addAtk += CountFaceUpMonstersOfType(0, 7) * 100;
        addAtk += CountFaceUpMonstersOfType(1, 7) * 100;
        break;
    case 0x1EC: addAtk += equipCount * 500; break;
    case 0x203:
        addAtk += gDuelPlayers[player & 1].handCount * 300;
        addDef += gDuelPlayers[player & 1].handCount * 300;
        break;
    case 0x229: addAtk += CountActiveCardsOnField(player, 11) * 500; break;
    case 0x267:
        if (ZB(player & 1, slot)->counter6 <= 1) { atkHalves++; defHalves++; }
        else { atkDoubles++; defDoubles++; }
        break;
    case 0x2F7:
        for (i = 0; i <= 1; i++)
            for (p = 0; p < gDuelPlayers[i & 1].graveCount; p++) {
                int n = CARD_NUMBER(((struct DuelCard *)((u8 *)gDuelPlayers + (i & 1) * 0xD64 + 0x904))[p].id);
                switch (n) {
                case 0x22: case 0x2D1: case 0x4BA: case 0x7F2:
                    addAtk += 300;
                    break;
                }
            }
        break;
    case 0x2F9:
        addAtk += CountFaceUpMonstersOfType(0, 10) * 200;
        addAtk += CountFaceUpMonstersOfType(1, 10) * 200;
        break;
    case 0x328:
        for (i = 0; i < gDuelPlayers[(1 - player) & 1].graveCount; i++)
            if (CARD_TYPE(((struct DuelCard *)((u8 *)gDuelPlayers + ((1 - player) & 1) * 0xD64 + 0x904))[i].id) == 1) addAtk += 500;
        addAtk += CountFaceUpMonstersOfType(1 - player, 1) * 500;
        break;
    case 0x457:
        addAtk -= gDuelPlayers[player & 1].handCount * 400;
        addDef -= gDuelPlayers[player & 1].handCount * 400;
        break;
    case 0x45E: if (CountMonsters(1 - player) > 0) addAtk -= 1000; break;
    case 0x585: addAtk -= CountMonsters(1 - player) * 200; break;
    case 0x5EC:
        if (player == gDuel.linkSkip && gDuel.phase1B12 == 3) addAtk += 300;
        break;
    case 0x5EE:
        if (player != gDuel.linkSkip && gDuel.phase1B12 == 3) addAtk += 300;
        break;
    }
    switch (out->type) {
    case 13:
        addAtk += CountFaceUpMonstersByNumberInPosition(player, 0x4E4, 1) * 500;
        addDef += CountFaceUpMonstersByNumberInPosition(player, 0x4E4, 1) * 500;
        break;
    }
    /* Opposing attribute auras affect ATK only, across both players. */
    switch (out->attr) {
    case 1:
        addAtk += CountActiveCardsOnField(0, 0x1EB) * 500;
        addAtk += CountActiveCardsOnField(1, 0x1EB) * 500;
        addAtk -= CountActiveCardsOnField(0, 0x273) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x273) * 400;
        break;
    case 2:
        addAtk -= CountActiveCardsOnField(0, 0x1EB) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x1EB) * 400;
        addAtk += CountActiveCardsOnField(0, 0x273) * 500;
        addAtk += CountActiveCardsOnField(1, 0x273) * 500;
        break;
    case 3:
        addAtk += CountActiveCardsOnField(0, 0x20B) * 500;
        addAtk += CountActiveCardsOnField(1, 0x20B) * 500;
        addAtk -= CountActiveCardsOnField(0, 0x255) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x255) * 400;
        addAtk -= CountActiveCardsOnField(0, 0x58E) * 500;
        addAtk -= CountActiveCardsOnField(1, 0x58E) * 500;
        break;
    case 4:
        addAtk -= CountActiveCardsOnField(0, 0x20B) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x20B) * 400;
        addAtk += CountActiveCardsOnField(0, 0x255) * 500;
        addAtk += CountActiveCardsOnField(1, 0x255) * 500;
        break;
    case 5:
        addAtk += CountActiveCardsOnField(0, 0x20E) * 500;
        addAtk += CountActiveCardsOnField(1, 0x20E) * 500;
        addAtk -= CountActiveCardsOnField(0, 0x260) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x260) * 400;
        break;
    case 6:
        addAtk -= CountActiveCardsOnField(0, 0x20E) * 400;
        addAtk -= CountActiveCardsOnField(1, 0x20E) * 400;
        addAtk += CountActiveCardsOnField(0, 0x260) * 500;
        addAtk += CountActiveCardsOnField(1, 0x260) * 500;
        break;
    }
    /* Field-zone effects use type and attribute modifier tables. */
    if (!(((struct StatDuelFlagBytes *)&gDuel)->flags & 4) && !immune) {
        for (i = 0; i <= 1; i++) {
            struct DuelZone *field = &gDuel.players[i & 1].zones[10];
            if (((struct DuelCard *)((u8 *)&gDuel + 0x2C + 0x5C8 + (i & 1) * 0xD64))->id && !(((u8 *)field)[0x91] & 8) && (((u8 *)field)[6]&2)) {
                int n = CARD_NUMBER(((struct DuelCard *)((u8 *)&gDuel + 0x2C + 0x5C8 + (i & 1) * 0xD64))->id);
                switch (n) {
                case 329: case 330: case 331: case 332: case 333: case 334:
                    addAtk += gFieldTypeBonuses[n - 329][out->type];
                    addDef += gFieldTypeBonuses[n - 329][out->type];
                    break;
                case 1125: case 1126: case 1127: case 1128: case 1129: case 1130:
                    addAtk += gFieldAttributeBonuses[n - 1125][out->attr] * 500;
                    addDef -= gFieldAttributeBonuses[n - 1125][out->attr] * 400;
                    break;
                case 1069: if (((struct ZoneFlags *)&gDuel.players[player & 1].zones[slot])->flags & 1) addDef += 500; break;
                }
            }
        }
    }
    if (player == gDuel.linkSkip &&
        gDuel.phase1B12 == 3 && CountFaceUpMonstersByNumber(1 - player, 0x5EB)) addAtk -= 300;
    addAtk += CountAquaChorusBoosts(player, slot) * 500;
    addDef += CountAquaChorusBoosts(player, slot) * 500;
    /* Reverse additive modifiers, clamp, scale, then optionally swap stats. */
    if ((((struct StatDuelFlags *)&gDuel)->flags & 0x2080) == 0x2000) {
        out->atk -= addAtk + equipAtk + otherAtk;
        out->def -= addDef + equipDef + otherDef;
    } else {
        out->atk += addAtk + equipAtk + otherAtk;
        out->def += addDef + equipDef + otherDef;
    }
    if (out->atk < 0) out->atk = 0;
    if (out->def < 0) out->def = 0;
    if (((u8 *)zone)[0x8C] & 0x20) atkHalves++;
    if (atkDoubles > atkHalves)
        for (i = 0; i < atkDoubles - atkHalves; i++) out->atk *= 2;
    if (atkDoubles < atkHalves)
        for (i = 0; i < atkHalves - atkDoubles; i++) out->atk = HalveRoundUp(out->atk);
    if (defDoubles > defHalves)
        for (i = 0; i < defDoubles - defHalves; i++) out->def *= 2;
    if (defDoubles < defHalves)
        for (i = 0; i < defHalves - defDoubles; i++) out->def = HalveRoundUp(out->def);
    if ((((struct StatDuelFlags *)&gDuel)->flags & 0x4040) == 0x4000 && !immune) {
        int temp = out->def, attack = out->atk; out->atk = temp; out->def = attack;
    }
}

