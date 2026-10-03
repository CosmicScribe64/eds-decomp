#ifndef GUARD_CONSTANTS_GAME_H
#define GUARD_CONSTANTS_GAME_H

/*
 * Game-wide catalogs: duelists, booster packs and text languages.
 */

/* Duelist (character) id: IsOpponentUnlocked argument, gSaveData.duelRecords index, gMain.opponent.
 * The same ids select names in gDuelists (include/bustup.h); see wiki/data/duelist-table.md. */
enum DuelistId {
    DUELIST_YUGI = 1,
    DUELIST_TEA = 2,
    DUELIST_JOEY = 3,
    DUELIST_TRISTAN = 4,
    DUELIST_BAKURA = 5,
    DUELIST_REX = 6,
    DUELIST_ESPA_ROBA = 7,
    DUELIST_WEEVIL = 8,
    DUELIST_MAKO = 9,
    DUELIST_MAI = 10,
    DUELIST_RARE_HUNTER = 11,
    DUELIST_ARKANA = 12,
    DUELIST_STRINGS = 13,
    DUELIST_UMBRA_LUMIS = 14,
    DUELIST_MARIK = 15,
    DUELIST_KAIBA = 16,
    DUELIST_ISHIZU = 17,
    DUELIST_SHADI = 18,
    DUELIST_YAMI_BAKURA = 19,
    DUELIST_YAMI_YUGI = 20,
    DUELIST_DUEL_COMPUTER = 21,
    DUELIST_SIMON = 22,
    DUELIST_PEGASUS = 23,
    DUELIST_GRANDPA = 24,
};

/* Booster pack id: IsPackUnlocked / PackList_AddPack / GetRewardPack argument and gPackInfo.id.
 * Names follow the PackInfo strings (typos corrected); 801/802/901/902 are the magazine and prize sets
 * given by Campaign_DeliverMagazines / Campaign_GiveRewards (wiki/data/booster-packs.md). */
enum BoosterPackId {
    PACK_VOL_1 = 1,
    PACK_VOL_2 = 2,
    PACK_VOL_3 = 3,
    PACK_VOL_4 = 4,
    PACK_VOL_5 = 5,
    PACK_VOL_6 = 6,
    PACK_VOL_7 = 7,
    PACK_LOB_EWD = 11,
    PACK_PHANTOM_OF_G = 12,
    PACK_MAGIC_RULER = 21,
    PACK_PHARAOHS_SERVANT = 22,
    PACK_CURSE_OF_ANUBIS = 23,
    PACK_PREMIUM_3 = 33,
    PACK_CELEMONY = 41,
    PACK_RANDOM_TRAP = 102,         /* unused id: GeneratePackCards draws 5 distinct random Trap cards */
    PACK_RANDOM_MAGIC = 103,        /* unused id: 5 distinct random Magic cards */
    PACK_RANDOM_ANY = 110,          /* unused id: 5 distinct random cards (tokens excluded) */
    PACK_EXPERT_1 = 501,
    PACK_EXPERT_2 = 502,
    PACK_EXPERT_3 = 503,
    PACK_DUELIST_PACK = 504,
    PACK_THE_FINAL_DUELIST = 505,
    PACK_RARE_SELECTIONS = 506,
    PACK_EXPERT_4 = 507,
    PACK_EXPERT_5 = 508,
    PACK_LIMITED_COLLECTION = 509,
    PACK_WEEKLY_YUGIOH = 801,
    PACK_YUGIOH_MAGAZINE = 802,
    PACK_WEEKLY_YUGIOH_B = 901,
    PACK_GRANDPA_CUP_PRIZE = 902,
};

/* Text language: gSaveData.language (bits 0-6 of save byte +4), labels of DebugMenu_DrawLanguage.
 * The USA game sets ENGLISH; all of its text is English. */
enum Language {
    LANGUAGE_JAPANESE = 0,
    LANGUAGE_ENGLISH = 1,
    LANGUAGE_GERMAN = 2,
    LANGUAGE_FRENCH = 3,
    LANGUAGE_ITALIAN = 4,
};

#endif /* GUARD_CONSTANTS_GAME_H */
