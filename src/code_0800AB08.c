#include "global.h"
#include "duel.h"

int sub_0800A78C(u32 player, u32 slot, u16 number);
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
#define ZONE_BASE(p, s) ((u8 *)gUnk_0201930C + (s) * 0x94 + (p) * 0xD64)
#define ZONE_WORD(p, s) (((struct DuelCard *)ZONE_BASE(p, s))->id)
#define ZONE_FLAGS(p, s) (((struct ZoneFlags *)ZONE_BASE(p, s))->flags)

int sub_0800AB08(u32 player, u16 number)
{
    int count = 0;
    int slot;
    for (slot = 0; slot <= 4; slot++) {
        if (ZONE_WORD(player & 1, slot) &&
            (ZONE_FLAGS(player & 1, slot) & 2) &&
            sub_0800A78C(player, slot, number))
            count++;
    }
    return count;
}
int sub_0800AB6C(u32 player, u16 number)
{
    int slot;
    for (slot = 0; slot <= 4; slot++) {
        if (ZONE_WORD(player & 1, slot) &&
            (ZONE_FLAGS(player & 1, slot) & 2) &&
            sub_0800A78C(player, slot, number))
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
extern u8 gUnk_020195F0[];
extern u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
extern const s16 gUnk_080815A8[][24];
extern const s16 gUnk_080816C8[][8];
int sub_080094E4(void);
int sub_08008524(int player, u16 number);
int sub_080085B0(int player, u16 type);
int sub_08008860(int player);
int sub_080086CC(int player, u16 number);
int sub_080087EC(int player, u16 number, u16 flag);
int sub_080088A4(int player, u16 faceUp, u16 bit0Clear);
int sub_08008B70(int player, u16 faceUp, u16 faceDown, u16 includeField);
int sub_0800A3F4(int player, int slot);
s32 sub_0807548C(s32 value); /* round(value / 2), including signed division */
#define ZB(p, s) ((struct DuelZone *)ZONE_BASE(p, s))
#define CARD_STATS(id) (((u32 *)0x08621DE0)[(id) & 0x7FF])
/* A 16-bit card-ID boundary avoids folding repeated ID/table loads into
 * one expression in this reconstruction. */
static inline int GetCardType(u16 id) { return (CARD_STATS(id) & 0x1F00000) >> 20; }
#define CARD_TYPE(id) GetCardType(id)
static inline u16 GetCardNumber(int id) { return ((const u16 *)0x08622AB4)[id & 0x7FF]; }
static inline u16 GetCardNumberS(int id) { return *(gUnk_08622AB4 + (id & 0x7FF)); }
#define CARD_NUMBER(id) GetCardNumber(id)
#define ZONE_DISABLED(p, s) (((struct ZoneDisabled *)ZB(p, s))->flags & 8)
#define ZONE_VALUE(p, s) (((struct ZoneAuxBits *)((u8 *)ZB(p, s) + 0x90))->value)
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

static inline struct DuelZone * GetFieldTarget(int player, int slot) { return (struct DuelZone *)((u8 *)&gUnk_020192E0 + 0x2C + (slot * 0x94 + (player & 1) * 0xD64)); }

/* Kept inactive between matching passes so this unit remains usable. */
#if 1 /* NONMATCHING: the frontier is 0x1CD0 versus 0x1CCC, with the target 0x50-byte frame
       * and 660 normalized +/- diff lines. Shared tails, field-table accesses and register
       * allocation still differ. See wiki/functions/code-0800ab08.md for experiment details. */
#if 0 /* NONMATCHING (score 482): pointer-plus symbol lookup in equip case; first rd diff +0xAB2 */
void sub_0800ABC8(int player, int slot, struct ZoneCardInfo *out)
{
    int i, p;
    int atkHalves = 0, atkDoubles = 0, defHalves = 0, defDoubles = 0;
    int equipAtk = 0, equipDef = 0, equipCount = 0;
    int otherAtk = 0, otherDef = 0, addAtk = 0, addDef = 0;
    int pp = player & 1;
    u32 poff = pp * 0xD64;
    /* FAKEMATCH: retain the equip replacement-stat output-pointer lifetime. */
    struct ZoneCardInfo *replacementOut;
    u8 *base = (u8 *)gUnk_0201930C + poff;
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
        {
            /* FAKEMATCH: keep the first type mask independent of the attribute merge. */
            int mask = 31;
            asm("" : "+r"(mask));
            type &= mask;
            ((u8 *)out)[2] = type;
        }
        asm("" : "+r"(type));
        {
            int attr = GetCardAttribute(out->id) << 5;
            asm("" : "+r"(attr)); /* FAKEMATCH: shift the attribute before remasking type. */
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
        if (sub_080094E4() == 0x14D) immune = 1;
        break;
    }

    /* The newest qualifying type-changing card wins this preliminary pass. */
    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = (struct DuelZone *)(i*0x94+(player&1)*0xD64+(u32)gUnk_0201930C);
        if (((struct DuelCard *)z)->id && CARD_NUMBER(((struct DuelCard *)z)->id) == 0x2FA &&
            ((struct ZoneCardBits *)z)->flag17 && (((u8 *)z)[6]&2) && z->serial > (u32)newest) {
            newest = z->serial;
            out->type = 10;
        }
        for (p = 0; p <= 1; p++) {
            struct DuelZone *z2 = ((struct DuelZone *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gUnk_020195F0)));
            if (((struct DuelCard *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gUnk_020195F0)))->id && CARD_NUMBER(((struct DuelCard *)((u8 *)((p&1)*0xD64+i*0x94+(u32)gUnk_020195F0)))->id) == 0x479 &&
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
                /* FAKEMATCH: preserve the separate first +700 link tail. */
                asm("" : : "r"(otherAtk));
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
                for (i = 0; i < gUnk_020192E4[player & 1].graveCount; i++)
                    if ((u32)CARD_TYPE(((struct DuelCard *)((u8 *)gUnk_020192E4 + (player & 1) * 0xD64 + 0x904))[i].id) <= 20) otherAtk += 100;
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
            if (!ZONE_DISABLED(lp & 1, ls) && !sub_08008524(0, 0x601) &&
                !sub_08008524(1, 0x601) && !(*((u8 *)gUnk_0201930C + 0x1AA1) & 3) &&
                (!immune || CARD_TYPE(linkedId) != 22)) {
                switch (GetCardNumberS(linkedId)) {
                case 0x47:
                    if (GetCardNumberS(out->id) == 0x115) { out->atk = 0; out->def = 2000; }
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
                case 309: if (out->type == 13) goto equip_300_type16; break;
                case 310: equipAtk += 500; break;
                case 311: if (out->type == 17) { equipAtk += 300; equipDef += 300; } break;
                case 312: equipDef += 800; break;
                case 313: equipAtk += 700; equipDef += 700; break;
                case 314: if (out->type == 1) { equipAtk += 300; equipDef += 300; } break;
                case 315: if (out->type == 19) { equipAtk += 300; equipDef += 300; } break;
                case 316:
                    if ((u16)(GetCardNumberS(out->id) - 61) <= 1 || GetCardNumberS(out->id) == 0x4E1) equipAtk += 500;
                    break;
                case 318: if (out->type == 12) goto equip_300_type16; break;
                case 320: equipAtk += 700; break;
                case 321: if (out->type == 2) { equipAtk += 300; equipDef += 300; } break;
                case 322: if (out->type == 18) { equipAtk += 300; equipDef += 300; } break;
                case 323: if (out->attr == 5) { equipAtk += 400; equipDef -= 200; } break;
                case 324: if (out->type == 7) { equipAtk += 300; equipDef += 300; } break;
                case 325: if (out->type == 9) { equipAtk += 300; equipDef += 300; } break;
                case 326:
                    if (out->type == 16) {
                        /* FAKEMATCH: share this conditional +300 equip tail. */
                    equip_300_type16:
                        equipAtk += 300; equipDef += 300;
                    }
                    break;
                case 327: if (out->type == 14) { equipAtk += 300; equipDef += 300; } break;
                case 650: equipAtk += 500; break;
                case 653: if (out->attr == 4) equipAtk += 700; break;
                case 656:
                    if (((struct DuelPlayer *)((u8 *)gUnk_0201930C - 0x28))[lp & 1].lifePoints < ((struct DuelPlayer *)((u8 *)gUnk_0201930C - 0x28))[(1 - lp) & 1].lifePoints) { replacementOut = out; replacementOut->atk = BaseAttack(out->id) * 2; }
                    if (gUnk_020192E4[lp & 1].lifePoints > gUnk_020192E4[(1 - lp) & 1].lifePoints) out->atk = sub_0807548C(BaseAttack(out->id));
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
                case 1448: out->attr = ZONE_VALUE(lp & 1, ls); break;
                case 1449:
                    equipAtk += sub_080088A4(lp, 1, 0) * 800;
                    equipDef += sub_080088A4(lp, 1, 0) * 800;
                    break;
                case 1450:
                    equipAtk += sub_08008B70(lp, 0, 0, 0) * 500;
                    equipDef += sub_08008B70(lp, 0, 0, 0) * 500;
                    break;
                case 1540: if (GetCardNumberS(out->id) == 0x53B) equipAtk += 300; break;
                case 1550:
                    if (out->type == 15) { out->type = 1; equipAtk += 500; equipDef += 500; }
                    if (ZB(lp & 1, ls)->serial > (u32)newest) out->type = 1;
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
        case 8: addAtk = BaseAttack(link) + addAtk; addDef = BaseDefense(link) + addDef; break;
        case 9: addAtk -= (value + 1) * 500; addDef -= (value + 1) * 500; break;
        case 10: equipAtk += 200; break;
        case 11: equipAtk += value * 300; break;
        case 12: addAtk -= value * 200; break;
        }
    }
    /* Intrinsic monster effects are applied after all links. */
    switch (CARD_NUMBER(out->id)) {
    case 11: addAtk += sub_08008524(player, 0x229) * 500; break;
    case 0x16F:
        for (i = 0; i < gUnk_020192E4[player & 1].graveCount; i++)
            if ((u32)CARD_TYPE(((struct DuelCard *)((u8 *)gUnk_020192E4 + (player & 1) * 0xD64 + 0x904))[i].id) <= 20) addAtk += 100;
        break;
    case 0xEA:
        addAtk += sub_080085B0(0, 13) * 100;
        addAtk += sub_080085B0(1, 13) * 100;
        break;
    case 0x181: {
        int a = sub_08008524(0, 61), b = sub_08008524(1, 61);
        int c = sub_08008524(0, 0x4E1), d = sub_08008524(1, 0x4E1);
        addAtk += (a + b + c + d) * 300; addDef += (a + b + c + d) * 300;
        break;
    }
    case 0x196:
        addAtk += sub_080085B0(0, 7) * 100;
        addAtk += sub_080085B0(1, 7) * 100;
        break;
    case 0x1EC: addAtk += equipCount * 500; break;
    case 0x203:
        addAtk += gUnk_020192E4[player & 1].handCount * 300;
        addDef += gUnk_020192E4[player & 1].handCount * 300;
        break;
    case 0x229: addAtk += sub_08008524(player, 11) * 500; break;
    case 0x267:
        if (ZB(player & 1, slot)->counter6 <= 1) { atkHalves++; defHalves++; }
        else { atkDoubles++; defDoubles++; }
        break;
    case 0x2F7:
        for (i = 0; i <= 1; i++)
            for (p = 0; p < gUnk_020192E4[i & 1].graveCount; p++) {
                int n = CARD_NUMBER(((struct DuelCard *)((u8 *)gUnk_020192E4 + (i & 1) * 0xD64 + 0x904))[p].id);
                switch (n) {
                case 0x22: case 0x2D1: case 0x4BA: case 0x7F2:
                    addAtk += 300;
                    break;
                }
            }
        break;
    case 0x2F9:
        addAtk += sub_080085B0(0, 10) * 200;
        addAtk += sub_080085B0(1, 10) * 200;
        break;
    case 0x328:
        for (i = 0; i < gUnk_020192E4[(1 - player) & 1].graveCount; i++)
            if (CARD_TYPE(((struct DuelCard *)((u8 *)gUnk_020192E4 + ((1 - player) & 1) * 0xD64 + 0x904))[i].id) == 1) addAtk += 500;
        addAtk += sub_080085B0(1 - player, 1) * 500;
        break;
    case 0x457:
        addAtk -= gUnk_020192E4[player & 1].handCount * 400;
        addDef -= gUnk_020192E4[player & 1].handCount * 400;
        break;
    case 0x45E: if (sub_08008860(1 - player) > 0) addAtk -= 1000; break;
    case 0x585: addAtk -= sub_08008860(1 - player) * 200; break;
    case 0x5EC:
        if (player == gUnk_020192E0.linkSkip && gUnk_020192E0.phase1B12 == 3) addAtk += 300;
        break;
    case 0x5EE:
        if (player != gUnk_020192E0.linkSkip && gUnk_020192E0.phase1B12 == 3) addAtk += 300;
        break;
    }
    if (out->type == 13) {
        addAtk += sub_080087EC(player, 0x4E4, 1) * 500;
        addDef += sub_080087EC(player, 0x4E4, 1) * 500;
    }
    /* Opposing attribute auras affect ATK only, across both players. */
    switch (out->attr) {
    case 1:
        addAtk += sub_08008524(0, 0x1EB) * 500;
        addAtk += sub_08008524(1, 0x1EB) * 500;
        addAtk -= sub_08008524(0, 0x273) * 400;
        addAtk -= sub_08008524(1, 0x273) * 400;
        break;
    case 2:
        addAtk -= sub_08008524(0, 0x1EB) * 400;
        addAtk -= sub_08008524(1, 0x1EB) * 400;
        addAtk += sub_08008524(0, 0x273) * 500;
        addAtk += sub_08008524(1, 0x273) * 500;
        break;
    case 3:
        addAtk += sub_08008524(0, 0x20B) * 500;
        addAtk += sub_08008524(1, 0x20B) * 500;
        addAtk -= sub_08008524(0, 0x255) * 400;
        addAtk -= sub_08008524(1, 0x255) * 400;
        addAtk -= sub_08008524(0, 0x58E) * 500;
        addAtk -= sub_08008524(1, 0x58E) * 500;
        break;
    case 4:
        addAtk -= sub_08008524(0, 0x20B) * 400;
        addAtk -= sub_08008524(1, 0x20B) * 400;
        addAtk += sub_08008524(0, 0x255) * 500;
        addAtk += sub_08008524(1, 0x255) * 500;
        break;
    case 5:
        addAtk += sub_08008524(0, 0x20E) * 500;
        addAtk += sub_08008524(1, 0x20E) * 500;
        addAtk -= sub_08008524(0, 0x260) * 400;
        addAtk -= sub_08008524(1, 0x260) * 400;
        break;
    case 6:
        addAtk -= sub_08008524(0, 0x20E) * 400;
        addAtk -= sub_08008524(1, 0x20E) * 400;
        addAtk += sub_08008524(0, 0x260) * 500;
        addAtk += sub_08008524(1, 0x260) * 500;
        break;
    }
    /* Field-zone effects use type and attribute modifier tables. */
    if (!(((struct StatDuelFlagBytes *)&gUnk_020192E0)->flags & 4) && !immune) {
        for (i = 0; i <= 1; i++) {
            struct DuelZone *field = &gUnk_020192E0.players[i & 1].zones[10];
            if (((struct DuelCard *)((u8 *)&gUnk_020192E0 + 0x2C + 0x5C8 + (i & 1) * 0xD64))->id && !(((u8 *)field)[0x91] & 8) && (((u8 *)field)[6]&2)) {
                int n = CARD_NUMBER(((struct DuelCard *)((u8 *)&gUnk_020192E0 + 0x2C + 0x5C8 + (i & 1) * 0xD64))->id);
                switch (n) {
                case 329: case 330: case 331: case 332: case 333: case 334:
                    addAtk += gUnk_080815A8[n - 329][out->type];
                    addDef += gUnk_080815A8[n - 329][out->type];
                    break;
                case 1125: case 1126: case 1127: case 1128: case 1129: case 1130:
                    addAtk += gUnk_080816C8[n - 1125][out->attr] * 500;
                    addDef -= gUnk_080816C8[n - 1125][out->attr] * 400;
                    break;
                case 1069: if (((struct ZoneFlags *)GetFieldTarget(player, slot))->flags & 1) addDef += 500; break;
                }
            }
        }
    }
    if (player == gUnk_020192E0.linkSkip &&
        gUnk_020192E0.phase1B12 == 3 && sub_080086CC(1 - player, 0x5EB)) addAtk -= 300;
    addAtk += sub_0800A3F4(player, slot) * 500;
    addDef += sub_0800A3F4(player, slot) * 500;
    /* Reverse additive modifiers, clamp, scale, then optionally swap stats. */
    if ((((struct StatDuelFlags *)&gUnk_020192E0)->flags & 0x2080) == 0x2000) {
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
        for (i = 0; i < atkHalves - atkDoubles; i++) out->atk = sub_0807548C(out->atk);
    if (defDoubles > defHalves)
        for (i = 0; i < defDoubles - defHalves; i++) out->def *= 2;
    if (defDoubles < defHalves)
        for (i = 0; i < defHalves - defDoubles; i++) out->def = sub_0807548C(out->def);
    if ((((struct StatDuelFlags *)&gUnk_020192E0)->flags & 0x4040) == 0x4000 && !immune) {
        int temp = out->def, attack = out->atk; out->atk = temp; out->def = attack;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800AB08", sub_0800ABC8); /* 0x0800ABC8 size 0x1CCC */

#endif

