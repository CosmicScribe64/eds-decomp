#ifndef GUARD_CARD_DATA_H
#define GUARD_CARD_DATA_H

/*
 * The read-only card database in ROM (wiki/data/card-table.md, card-id-map.md, card-art.md).
 *
 * Two number spaces:
 *   card ID      1..820, the alphabetical index of every per-card table (gCardStats, gCardNames, the art);
 *                slot 0 is empty. Readers mask the ID with CARD_ID_MASK.
 *   card number  the language-independent Konami number (enum CardNumber, include/constants/cards.h): deck
 *                lists, packs, the effect table and every "is this card X" test. gCardIdToNumber and
 *                gCardNumberToId convert between the two.
 *
 * Units read these tables both through the symbols below and through integer-constant addresses
 * (((const u32 *)0x08621DE0)[id]); the two forms generate different code, so each unit keeps the form it
 * matches with. The static inline helpers repeated across units (CardNumberToId, GetCardSubtype, ...) also
 * differ in matching-relevant details and stay in the units. Tables used by one unit only (gCardDescriptions
 * 0x082461A0, gCardPasswords 0x08623120, the type/attribute name tables) stay local externs there.
 */

#include "global.h"
#include "constants/card_stats.h"

/* Card IDs: 0..820 (0 is an empty slot) */
#define CARD_ID_COUNT               821
#define CARD_ID_MASK                0x7FF       /* applied by every table reader */

/* Card numbers */
#define CARD_NUMBER_COUNT           2048        /* entries of gCardNumberToId */
#define CARD_NUMBER_TOKEN_FIRST     1920        /* 1920-1999 are monster tokens */
#define CARD_NUMBER_TOKEN_END       2000
#define CARD_NUMBER_ALT_ART         2000        /* 2000 + n: alternate art of card n (its ID is n's ID + 1) */

/* Per-card record sizes */
#define CARD_NAME_SIZE              0x40        /* gCardNames record */
#define CARD_DESCRIPTION_SIZE       0x1E0       /* gCardDescriptions record */
#define CARD_ART_SIZE               0x10E0      /* gCardArtGfx record: 72x80 px, 6bpp packed */
#define CARD_ART_PALETTE_SIZE       0x80        /* gCardArtPalettes record: 64 BGR555 colours */

/*
 * Field extractors for a gCardStats word (a u32; layout in include/constants/card_stats.h). They spell out
 * the ROM's usual idioms for reference; units keep their own macros/inlines where the access form matters.
 */
#define CARD_STATS_DEF(stats)       ((stats) & CARD_STATS_DEF_MASK)
#define CARD_STATS_ATK(stats)       (((stats) << 14) >> 23)
#define CARD_STATS_SUBTYPE(stats)   (((stats) & CARD_STATS_SUBTYPE_MASK) >> CARD_STATS_SUBTYPE_SHIFT)
#define CARD_STATS_KIND(stats)      (((stats) & CARD_STATS_KIND_MASK) >> CARD_STATS_KIND_SHIFT)
#define CARD_STATS_TYPE(stats)      (((stats) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)
#define CARD_STATS_LEVEL(stats)     (((stats) & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT)
#define CARD_STATS_ATTR(stats)      ((stats) >> CARD_STATS_ATTR_SHIFT)

/* ---- Tables indexed by card ID ---- */

/* 0x08621DE0: packed stats word per card ID (u32[821]; layout in constants/card_stats.h). */
extern const u32 gCardStats[];

/* 0x0822C720: card names, one CARD_NAME_SIZE (0x40) byte record per card ID (units also view it as char[]
 * or [][0x40]). */
extern const u8 gCardNames[];

/* 0x08622AB4: card number per card ID (u16[821] + 2 bytes of padding; 0xFFFF = none). */
extern const u16 gCardIdToNumber[];

/* 0x082A6500: card art, 72x80 px 6bpp packed, CARD_ART_SIZE bytes per card ID. */
extern const u16 gCardArtGfx[];

/* 0x08608360: 64-colour BGR555 art palette per card ID (CARD_ART_PALETTE_SIZE bytes each);
 * UnpackCardArt8bpp copies entry cardId. */
extern const u8 gCardArtPalettes[];

/* ---- Table indexed by card number ---- */

/* 0x08623DF4: card ID per card number (u16[CARD_NUMBER_COUNT], 0 = no card). Entries 2000-2047 are 0: code
 * maps an alternate-art number (up to 2389) to the ID + 1 of number - 2000. DuelCmd_SummonToken looks up
 * 1920 + kind. */
extern const u16 gCardNumberToId[];

/* ---- Card icons and frames ---- */

/* 0x08198950: 16x16 icon palettes, 11 pointers: attribute 1-6, then 8 Magic, 9 Trap, 10 Divine (0 and 7
 * NULL). */
extern const u8 *const gCardIconPals[];

/* 0x0819897C: 16x16 4bpp icon tiles, 11 pointers, same indexing as gCardIconPals. */
extern const u8 *const gCardIconGfx[];

/* 0x081989A8: image packs (include/bg.h) [10]: 0 none, 1-6 Light..Wind, 7 Trap, 8 Magic, 9 Divine. */
extern const u16 *const gAttributeIconImages[];

/* 0x081989D0: image packs [7] indexed by enum SpellSubtype (0 none; 3 is the equip cross). */
extern const u16 *const gSpellSubtypeIconImages[];

/* 0x081989EC: image packs indexed by enum CardType 1-20 (1 = Dragon); 0 and 21-24 NULL. */
extern const u16 *const gMonsterTypeIconImages[];

/* 0x0863840C: OBJ palette of the small stat digits and ATK/DEF labels (the battle scene loads it to OBJ
 * palette 2). */
extern const u8 gCardStatDigitsPal[];

/* 0x0863842C: 4bpp OBJ tiles of the red 8x8 digits 0-9; Card Detail copies 0x200 bytes (16 tiles) to OBJ
 * tile 0x30 for its ATK/DEF sprites. */
extern const u8 gCardStatDigitsGfx[];

/* 0x0867795C: 16-colour palette of the 32x32 card icons, loaded to OBJ palette 1. */
extern const u8 gCardIconPal[];

/* 32x32 card icons by frame colour, each 4 poses of 32x32 4bpp (0x800 bytes). */
extern const u8 gCardIconNormalGfx[];   /* 0x0867897C: normal monster (yellow); also LoadDuelUiGfx */
extern const u8 gCardIconEffectGfx[];   /* 0x0867917C: effect monster (orange) */
extern const u8 gCardIconFusionGfx[];   /* 0x0867997C: fusion (purple) */
extern const u8 gCardIconRitualGfx[];   /* 0x0867A17C: ritual (blue); also Obelisk */
extern const u8 gCardIconTrapGfx[];     /* 0x0867A97C: Trap (pink) */
extern const u8 gCardIconMagicGfx[];    /* 0x0867B17C: Magic (green) */

/* Full-size card frames, 8bpp image packs (104x144), picked by type or enum CardKind. */
extern const u8 gCardFrameNormalGfx[];  /* 0x08625460: normal monster (yellow) */
extern const u8 gCardFrameEffectGfx[];  /* 0x08627AF8: effect monster; also Slifer and Ra */
extern const u8 gCardFrameFusionGfx[];  /* 0x0862A190: fusion */
extern const u8 gCardFrameRitualGfx[];  /* 0x0862C828: ritual (blue); also Obelisk */
extern const u8 gCardFrameMagicGfx[];   /* 0x0862EEC0: Magic (type 22) */
extern const u8 gCardFrameTrapGfx[];    /* 0x08631558: Trap (type 21) */
extern const u8 gCardFrameTicketGfx[];  /* 0x08633BF0: Ticket (type 23) */

/*
 * Address-suffixed aliases of single table entries. Some units read one entry through its own symbol
 * because that is how the ROM's code loads it (a literal-pool address of the element, not base + index).
 * They stay unit-local externs; this is what each one is:
 *
 *   gCardIdToNumber_821   gCardIdToNumber[821] (padding)      booster_get_pack
 *   gCardIdToNumber_1081   gCardIdToNumber[0x439] (out of range, inside gCardPasswords)  deck_edit_stats
 *   gCardNumberToId_FlameSwordsman   gCardNumberToId[CARD_FLAME_SWORDSMAN]               collection
 *   gCardNumberToId_SummonedSkull   gCardNumberToId[CARD_SUMMONED_SKULL]                effect_targets3
 *   gCardNumberToId_DarkMagician   gCardNumberToId[CARD_DARK_MAGICIAN]                 collection, effect_resolve8
 *   gCardNumberToId_GaiaTheFierceKnight   gCardNumberToId[CARD_GAIA_THE_FIERCE_KNIGHT]        collection
 *   gCardNumberToId_CelticGuardian   gCardNumberToId[CARD_CELTIC_GUARDIAN]               collection
 *   gCardNumberToId_Kuriboh   gCardNumberToId[CARD_KURIBOH]                       battle_phase2, duel_turn_end,
 *                                                                       effect_activation
 *   gCardNumberToId_HarpieLady   gCardNumberToId[CARD_HARPIE_LADY]                   collection
 *   gCardNumberToId_TigerAxe   gCardNumberToId[CARD_TIGER_AXE]                     collection
 *   gCardNumberToId_ThousandDragon   gCardNumberToId[CARD_THOUSAND_DRAGON]               collection
 *   gCardNumberToId_MysteriousPuppeteer   gCardNumberToId[CARD_MYSTERIOUS_PUPPETEER]          effect_hooks
 *   gCardNumberToId_PetitMoth   gCardNumberToId[CARD_PETIT_MOTH]                    effect_hooks
 *   gCardNumberToId_AxeOfDespair   gCardNumberToId[CARD_AXE_OF_DESPAIR]                effect_prepare3
 *   gCardNumberToId_DragonCaptureJar   gCardNumberToId[CARD_DRAGON_CAPTURE_JAR]            effect_hooks
 *   gCardNumberToId_LabyrinthWall   gCardNumberToId[CARD_LABYRINTH_WALL]                effect_hooks
 *   gCardNumberToId_PendulumMachine   gCardNumberToId[CARD_PENDULUM_MACHINE]              collection
 *   gCardNumberToId_LauncherSpider   gCardNumberToId[CARD_LAUNCHER_SPIDER]               collection
 *   gCardNumberToId_SinisterSerpent   gCardNumberToId[CARD_SINISTER_SERPENT]              effect_hooks
 *   gCardNumberToId_MushroomMan2   gCardNumberToId[CARD_MUSHROOM_MAN_2]                battle_phase3
 *   gCardNumberToId_GazelleTheKingOfMythicalBeasts   gCardNumberToId[CARD_GAZELLE_THE_KING_OF_MYTHICAL_BEASTS]  effect_resolve9, effect_resolve10
 *   gCardNumberToId_ParasiteParacide   gCardNumberToId[CARD_PARASITE_PARACIDE]             effect_resolve10
 *   gCardNumberToId_ValkyrionTheMagnaWarrior   gCardNumberToId[CARD_VALKYRION_THE_MAGNA_WARRIOR]   ai_strategy
 *   gCardNumberToId_ToonWorld   gCardNumberToId[CARD_TOON_WORLD]                    ai_steps, booster_get_pack
 *   gCardNumberToId_LightforceSword   gCardNumberToId[CARD_LIGHTFORCE_SWORD]              duel_phases
 *   gCardNumberToId_Polymerization   gCardNumberToId[CARD_POLYMERIZATION]                collection
 *   gCardNumberToId_Graverobber   gCardNumberToId[CARD_GRAVEROBBER]                   card_command_menu, duel_cmd_queue,
 *                                                                       duel_turn_end
 *   gCardNumberToId_Kotodama   gCardNumberToId[CARD_KOTODAMA]                      effect_hooks
 *   gCardNumberToId_SwordOfDragonsSoul   gCardNumberToId[CARD_SWORD_OF_DRAGONS_SOUL]         battle_phase2, battle_phase3
 *   gCardNumberToId_DarkSage   gCardNumberToId[CARD_DARK_SAGE]                     effect_resolve8
 *   gCardNumberToId_1210   gCardNumberToId[CARD_1210] (0 in EDS)               collection
 *   gCardNumberToId_1243   gCardNumberToId[CARD_1243] (0 in EDS)               battle_phase1
 *   gCardNumberToId_1249   gCardNumberToId[CARD_1249] (0 in EDS)               collection
 *   gCardNumberToId_1258   gCardNumberToId[CARD_1258] (0 in EDS)               effect_resolve9, effect_resolve10
 *   gCardNumberToId_1322   gCardNumberToId[CARD_1322] (0 in EDS)               battle_phase3
 *   gCardNumberToId_1340   gCardNumberToId[CARD_1340] (0 in EDS)               battle_phase3
 *   gCardNumberToId_1405   gCardNumberToId[CARD_1405] (0 in EDS)               effect_resolve10, effect_targets4
 *   gCardNumberToId_1514   gCardNumberToId[CARD_1514] (0 in EDS)               battle_phase3
 *   gCardNumberToId_1520   gCardNumberToId[CARD_1520] (0 in EDS)               duel_prompts
 *   gCardNumberToId_1533   gCardNumberToId[CARD_1533] (0 in EDS)               effect_hooks
 *   gCardNumberToId_1536   gCardNumberToId[CARD_1536] (0 in EDS)               duel_phases
 *   gCardNumberToId_1538   gCardNumberToId[CARD_1538] (0 in EDS)               battle_phase3
 *   gCardNumberToId_1547   gCardNumberToId[CARD_1547] (0 in EDS)               card_command_menu
 *   gCardNumberToId_1548   gCardNumberToId[CARD_1548] (0 in EDS)               battle_phase3
 *   gCardNumberToId_TheMonarchy   gCardNumberToId[CARD_THE_MONARCHY]                  campaign_steps
 *   gCardNumberToId_SetSailForTheKingdom   gCardNumberToId[CARD_SET_SAIL_FOR_THE_KINGDOM]      campaign_steps
 *   gCardNumberToId_GloryOfTheKingsHand   gCardNumberToId[CARD_GLORY_OF_THE_KINGS_HAND]       campaign_steps
 *   gCardNumberToId_InsectMonsterToken   gCardNumberToId[CARD_INSECT_MONSTER_TOKEN]          duel_cmd_turn
 */

#endif /* GUARD_CARD_DATA_H */
