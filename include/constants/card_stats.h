#ifndef GUARD_CONSTANTS_CARD_STATS_H
#define GUARD_CONSTANTS_CARD_STATS_H

/*
 * Card stat values and the bit layout of a gCardStats word (include/card_data.h).
 *
 * gCardStats (0x08621DE0) holds one packed u32 per card ID (wiki/data/card-table.md, verified against the
 * ROM with tools/extract_cards.py --verify):
 *
 *   bits  0-8   DEF / 10            (monsters)
 *   bits  9-17  ATK / 10            (monsters)
 *   bits 17-19  spell subtype       (Magic and Trap cards only; overlaps ATK's top bit and the kind)
 *   bits 18-19  kind                (monsters: normal, effect, fusion, ritual)
 *   bits 20-24  type                (all cards: monster type 1-20, or Trap/Magic/Ticket/Divine)
 *   bits 25-28  level (stars)       (monsters)
 *   bits 29-31  attribute           (monsters; Trap cards carry 1 and Divine cards 2 here)
 *
 * The code decodes a word with mask-then-shift for type, level, kind and subtype, (w << 14) >> 23 for ATK
 * and w >> 29 for the attribute. Units keep their own access forms (macros, inlines or constant-address
 * reads), because the exact form decides the generated code.
 */

/* Field positions inside a gCardStats word. */
#define CARD_STATS_DEF_SHIFT        0
#define CARD_STATS_DEF_MASK         0x1FF
#define CARD_STATS_ATK_SHIFT        9
#define CARD_STATS_ATK_MASK         0x3FE00
#define CARD_STATS_SUBTYPE_SHIFT    17
#define CARD_STATS_SUBTYPE_MASK     0xE0000
#define CARD_STATS_KIND_SHIFT       18
#define CARD_STATS_KIND_MASK        0xC0000
#define CARD_STATS_TYPE_SHIFT       20
#define CARD_STATS_TYPE_MASK        0x1F00000
#define CARD_STATS_LEVEL_SHIFT      25
#define CARD_STATS_LEVEL_MASK       0x1E000000
#define CARD_STATS_ATTR_SHIFT       29
#define CARD_STATS_ATTR_MASK        0xE0000000

/* ATK and DEF are stored divided by 10. */
#define CARD_STATS_POINTS_SCALE     10

/* Card type: gCardStats bits 20-24. 1-20 are monster types; the switches in CardDetail_DrawInfo and
 * CardDetail_DrawSprites use the same values. Several older wiki unit pages swap Magic (22) and Trap (21). */
enum CardType {
    CARD_TYPE_DRAGON = 1,
    CARD_TYPE_ZOMBIE = 2,
    CARD_TYPE_FIEND = 3,
    CARD_TYPE_PYRO = 4,
    CARD_TYPE_SEA_SERPENT = 5,
    CARD_TYPE_ROCK = 6,
    CARD_TYPE_MACHINE = 7,
    CARD_TYPE_FISH = 8,
    CARD_TYPE_DINOSAUR = 9,
    CARD_TYPE_INSECT = 10,
    CARD_TYPE_BEAST = 11,
    CARD_TYPE_BEAST_WARRIOR = 12,
    CARD_TYPE_PLANT = 13,
    CARD_TYPE_AQUA = 14,
    CARD_TYPE_WARRIOR = 15,
    CARD_TYPE_WINGED_BEAST = 16,
    CARD_TYPE_FAIRY = 17,
    CARD_TYPE_SPELLCASTER = 18,
    CARD_TYPE_THUNDER = 19,
    CARD_TYPE_REPTILE = 20,     /* last monster type */
    CARD_TYPE_TRAP = 21,
    CARD_TYPE_MAGIC = 22,
    CARD_TYPE_TICKET = 23,      /* the three Championship prize tickets (card numbers 1901-1903) */
    CARD_TYPE_DIVINE = 24,      /* the three Egyptian God cards */
};

/* Monster attribute: gCardStats bits 29-31 (0 = none). Also the index of gCardIconPals/gCardIconGfx, whose
 * slots 8-10 hold the Magic, Trap and Divine icons (those are icon slots, not stats values). */
enum CardAttribute {
    ATTRIBUTE_LIGHT = 1,
    ATTRIBUTE_DARK = 2,
    ATTRIBUTE_WATER = 3,
    ATTRIBUTE_FIRE = 4,
    ATTRIBUTE_EARTH = 5,
    ATTRIBUTE_WIND = 6,
};

/* Card kind, the result of the per-unit GetCardSubtype/GetCardKind inlines: stats bits 18-19 for monsters,
 * 7/8/9 for Magic/Trap/Ticket cards. It picks the card frame graphics. Obelisk (card number 1910) gives
 * RITUAL, Slifer and Ra (1911, 1912) give EFFECT. IsFusionMonster tests FUSION. */
enum CardKind {
    CARD_KIND_NORMAL = 0,
    CARD_KIND_EFFECT = 1,
    CARD_KIND_FUSION = 2,
    CARD_KIND_RITUAL = 3,
    CARD_KIND_MAGIC = 7,
    CARD_KIND_TRAP = 8,
    CARD_KIND_TICKET = 9,
};

/* Magic/Trap subtype: gCardStats bits 17-19 (CARD_SUBTYPE(id) in the units; tested by World Suppression,
 * Mystic Probe and Metal Detector). Also the index of the subtype icon and suffix tables. */
enum SpellSubtype {
    SPELL_NORMAL = 0,
    SPELL_COUNTER = 1,          /* Trap cards only */
    SPELL_FIELD = 2,
    SPELL_EQUIP = 3,
    SPELL_CONTINUOUS = 4,
    SPELL_QUICK_PLAY = 5,
    SPELL_RITUAL = 6,
};

/* Card frame graphics: gDeckEdit.cursorCardFrame and GetCardFrameIndex. Same order as the frame assets
 * (assets/gfx/card_frames) and the gCardIcon*Gfx tables. */
enum CardFrame {
    CARD_FRAME_NORMAL = 0,
    CARD_FRAME_EFFECT = 1,
    CARD_FRAME_FUSION = 2,
    CARD_FRAME_RITUAL = 3,
    CARD_FRAME_MAGIC = 4,
    CARD_FRAME_TRAP = 5,
};

#endif /* GUARD_CONSTANTS_CARD_STATS_H */
