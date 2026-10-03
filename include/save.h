#ifndef GUARD_SAVE_H
#define GUARD_SAVE_H

/*
 * Save data: the RAM save image gSaveData, its SRAM copy, the card collection (trunk and the three saved
 * decks), the forbidden/limited list and the duel records.
 *
 * gSaveData (0x02011C20, 0x2170 bytes) is the whole save. SaveGame writes it to SRAM (0x0E000000) through
 * the Nintendo AgbSram library and verifies it; GameInit reads it back. A new game starts from InitSaveData.
 * The checksum is the u16 sum of bytes 0..0x216B, stored negated at +0x216E.
 *
 * The AgbSram prototypes are repeated here instead of including agb_sram.h: that SDK header typedefs
 * u8/u16/u32 itself and cannot be included next to global.h.
 *
 * Layouts are checked against every unit's local view (tools/structmap.py gSaveData); see wiki
 * functions/save-game.md and functions/collection-c.md.
 */

#include "global.h"

/* Capacity of the saved deck lists (gSaveData.deck / sideDeck / fusionDeck). AddCardToSaved*Deck and the
 * Deck Edit add step refuse a card when the list is full. */
enum SavedDeckCapacity {
    SIDE_DECK_MAX_CARDS = 15,
    FUSION_DECK_MAX_CARDS = 20,
    DECK_MAX_CARDS = 60,
};

/* gSaveData.options bits (Options screen; read by IsSeEnabled / IsBgmEnabled). */
enum SaveOptionFlags {
    OPTION_SE_ON = 1 << 0,      /* sound effects on */
    OPTION_BGM_ON = 1 << 1,     /* music on */
};

/* One card of the collection, gSaveData.trunk[cardId] (4 bytes per card id).
 * The trunk is the pool of unused copies: `count` excludes the copies placed in the three saved decks,
 * which are counted separately in the 2-bit fields (at most 3 copies of a card per list). */
struct TrunkEntry {
    u16 count:10;               /* +0x0 bits 0-9: copies in the trunk, max 0x3FF (some units call it owned) */
    u16 deckCopies:2;           /* +0x1 bits 2-3: copies in the saved Deck */
    u16 sideCopies:2;           /* +0x1 bits 4-5: copies in the saved Side Deck */
    u16 fusionCopies:2;         /* +0x1 bits 6-7: copies in the saved Fusion Deck */
    u8 flag0:1;                 /* +0x2 bit 0: no reader found */
    u8 passwordUsed:1;          /* +0x2 bit 1: this card's password was redeemed (Password_RevealAndGiveCard) */
    u8 unk2_2:6;                /* +0x2 bits 2-7 */
    u8 unk3;                    /* +0x3 */
};

/* Win/loss/draw counts against one opponent, gSaveData.duelRecords[DuelistId]. Each counter saturates
 * (RecordDuelWin/Loss/Draw). */
struct DuelRecord {
    u32 wins:11;                /* bits 0-10, max 0x7FF */
    u32 losses:11;              /* bits 11-21, max 0x7FF */
    u32 draws:10;               /* bits 22-31, max 0x3FF */
};

/* One row of the forbidden/limited list gCardCopyLimits (0x081A78B4, 47 rows), read by GetCardCopyLimit.
 * Cards not in the list allow 3 copies. */
struct CardCopyLimit {
    u16 cardNumber;             /* card number (gCardIdToNumber), not the card id */
    u16 limit;                  /* copies allowed across the saved decks: 0 forbidden, 1 limited, 2 semi-limited */
};

/* The save image, gSaveData (0x02011C20). */
struct SaveData {
    u8 unk0[4];                             /* +0x0000 */
    u8 language:7;                          /* +0x0004 bits 0-6: enum Language (1 = English in the USA ROM) */
    u8 sjisText:1;                          /* +0x0004 bit 7: text uses the 2-byte Shift-JIS path; set
                                             *          together with language 0 (Japanese). Some units
                                             *          read the whole byte (flags4) */
    u8 unk5[3];                             /* +0x0005 */
    struct TrunkEntry trunk[0x800];         /* +0x0008: the collection, indexed by card id */
    u16 deck[60];                           /* +0x2008: saved Deck (card ids), DECK_MAX_CARDS */
    u16 sideDeck[15];                       /* +0x2080: saved Side Deck, SIDE_DECK_MAX_CARDS */
    u16 fusionDeck[20];                     /* +0x209E: saved Fusion Deck, FUSION_DECK_MAX_CARDS */
    u16 trunkSize;                          /* +0x20C6: cards in the trunk (sum of trunk[].count) */
    u16 deckSize;                           /* +0x20C8: entries in deck[] */
    u16 sideDeckSize;                       /* +0x20CA: entries in sideDeck[] */
    u16 fusionDeckSize;                     /* +0x20CC: entries in fusionDeck[] */
    u16 unk20CE;                            /* +0x20CE */
    struct DuelRecord duelRecords[32];      /* +0x20D0: indexed by enum DuelistId (1..24 used) */
    u16 days;                               /* +0x2150: in-game day count, +1 per Campaign duel
                                             *          (DayCountToDate converts it to a date) */
    u16 options;                            /* +0x2152: enum SaveOptionFlags */
    u16 lastPackId;                         /* +0x2154: pack of the last generated booster; buying the same
                                             *          pack again rolls % 270 instead of % 180 */
    u16 packPityCount;                      /* +0x2156: packs in a row that fell back to the commons slot
                                             *          (reset by a slot 0-4 card); above 5 (10 on a re-buy)
                                             *          the next roll is Random() % 12 */
    u16 lastOpponent;                       /* +0x2158: opponent of the last recorded duel (RecordDuel*);
                                             *          selects the rematch dialogue */
    u16 unk215A;                            /* +0x215A */
    u16 unk215C;                            /* +0x215C */
    u16 tournamentRound;                    /* +0x215E: November tournament rounds won this year (0..3);
                                             *          reset after the final and on Dec 31 */
    u16 sugorokuQualified;                  /* +0x2160: 1 after winning the June SUGOROKU prelim; enables
                                             *          the match the next day */
    u8 championshipWins;                    /* +0x2162: National Championship titles (saturates at 0xFF;
                                             *          pack 22 unlocks when > 1) */
    u8 unk2163;                             /* +0x2163 */
    u16 unlockNotices;                      /* +0x2164: bit 0 new opponents, bit 1 new packs; set by
                                             *          Campaign_RecordDuelResult, cleared once shown */
    char signature[8];                      /* +0x2166: "DMEX1INT" (WriteSaveSignature); its last two
                                             *          bytes (+0x216C) are outside the checksum */
    u16 checksum;                           /* +0x216E: -(sum of the u16 words at +0x0000..+0x216B) */
};

typedef char save_h_check_trunk_entry_size[sizeof(struct TrunkEntry) == 4 ? 1 : -1];
typedef char save_h_check_duel_record_size[sizeof(struct DuelRecord) == 4 ? 1 : -1];
typedef char save_h_check_copy_limit_size[sizeof(struct CardCopyLimit) == 4 ? 1 : -1];
typedef char save_h_check_size[sizeof(struct SaveData) == 0x2170 ? 1 : -1];
typedef char save_h_check_trunk[(u32)&((struct SaveData *)0)->trunk == 0x8 ? 1 : -1];
typedef char save_h_check_deck[(u32)&((struct SaveData *)0)->deck == 0x2008 ? 1 : -1];
typedef char save_h_check_side_deck[(u32)&((struct SaveData *)0)->sideDeck == 0x2080 ? 1 : -1];
typedef char save_h_check_fusion_deck[(u32)&((struct SaveData *)0)->fusionDeck == 0x209E ? 1 : -1];
typedef char save_h_check_trunk_size[(u32)&((struct SaveData *)0)->trunkSize == 0x20C6 ? 1 : -1];
typedef char save_h_check_fusion_size[(u32)&((struct SaveData *)0)->fusionDeckSize == 0x20CC ? 1 : -1];
typedef char save_h_check_records[(u32)&((struct SaveData *)0)->duelRecords == 0x20D0 ? 1 : -1];
typedef char save_h_check_days[(u32)&((struct SaveData *)0)->days == 0x2150 ? 1 : -1];
typedef char save_h_check_options[(u32)&((struct SaveData *)0)->options == 0x2152 ? 1 : -1];
typedef char save_h_check_last_opponent[(u32)&((struct SaveData *)0)->lastOpponent == 0x2158 ? 1 : -1];
typedef char save_h_check_round[(u32)&((struct SaveData *)0)->tournamentRound == 0x215E ? 1 : -1];
typedef char save_h_check_titles[(u32)&((struct SaveData *)0)->championshipWins == 0x2162 ? 1 : -1];
typedef char save_h_check_notices[(u32)&((struct SaveData *)0)->unlockNotices == 0x2164 ? 1 : -1];
typedef char save_h_check_signature[(u32)&((struct SaveData *)0)->signature == 0x2166 ? 1 : -1];
typedef char save_h_check_checksum[(u32)&((struct SaveData *)0)->checksum == 0x216E ? 1 : -1];

/* The save image. Units that match only with another view keep it locally (u8[], u32[], or a struct with
 * the fields they use); a few reach single fields through address aliases instead of gSaveData:
 *   gUnk_02013CE8 = &gSaveData.deckSize, gUnk_02013CEC = &gSaveData.fusionDeckSize (deck_edit_cards);
 *   collection.c uses the integer addresses 0x02013CE6 (trunkSize), 0x02013D72 (options),
 *   0x02013D78 (lastOpponent) and 0x02013D82 (championshipWins).
 * These forms are matching choices: keep them. */
extern struct SaveData gSaveData;

/* gSaveData.signature under its own linker symbol (0x02013D86), used by the signature check. */
extern char gSaveDataSignature[8];

/* --- Save image: SRAM copy, signature and checksum ------------------------------------------------- */

/* UpdateSaveChecksum, then write the 0x2170-byte gSaveData to SRAM and verify it, up to 32 tries. */
void SaveGame(void);
/* SaveGame, then the SetMainCallback reset without changing the callback (clears the VBlank callbacks
 * and gMain.seqState0..2); returns 1. No callers (dead code). */
u32 SaveAndResetSceneState(void);
/* Reset gSaveData to a new game: clear it, SE and BGM on, Latin text mode, write the signature. */
void InitSaveData(void);
/* Copy "DMEX1INT" into gSaveData.signature. */
void WriteSaveSignature(void);
/* 1 when gSaveData.signature is "DMEX1INT". No callers in the USA ROM. */
u32 IsSaveSignatureValid(void);
/* 1 when gSaveData.checksum matches the data (the title screen offers Continue only then). */
u32 IsSaveChecksumValid(void);
/* Recompute gSaveData.checksum = -(sum of the u16 words at +0x0000..+0x216B). */
void UpdateSaveChecksum(void);
/* 1 if any of the first n bytes of a and b differ, else 0. */
u32 MemDiffers(const u8 *a, const u8 *b, u8 n);

/* --- Nintendo AgbSram library (src/sdk/agb_sram.c; same prototypes as include/agb_sram.h) ------------ */

/* Copy size bytes from SRAM src to dst (runs a copy of its loop from the stack, as SRAM reads require). */
void ReadSram(u8 *src, u8 *dst, u32 size);
/* Copy size bytes from src to SRAM dst. */
void WriteSram(u8 *src, u8 *dst, u32 size);
/* Compare size bytes of src against SRAM tgt; returns the address of the first mismatch, or 0. */
u32 VerifySram(u8 *src, u8 *tgt, u32 size);
/* WriteSram + VerifySram, up to 3 attempts; returns the last mismatch address, or 0. */
u32 WriteSramEx(u8 *src, u8 *dst, u32 size);

/* --- Card collection: trunk and saved decks ---------------------------------------------------------- */

/* Copies of cardId allowed across the saved decks: the gCardCopyLimits entry for its card number, or 3. */
s32 GetCardCopyLimit(u32 cardId);
/* 1 if one more copy of cardId fits its copy limit, counting the Deck, Side Deck and Fusion Deck and the
 * card's other prints (alternate art, the second Polymerization). */
u32 IsBelowCardCopyLimit(u16 cardId);
/* trunk[cardId].count++ and trunkSize++ (unless already 0x3FF). */
void AddCardToTrunk(u16 cardId);
/* Append cardId to the saved Deck if the copy limit and DECK_MAX_CARDS allow it. */
void AddCardToSavedDeck(u16 cardId);
/* Append cardId to the saved Side Deck (max SIDE_DECK_MAX_CARDS). */
void AddCardToSavedSideDeck(u16 cardId);
/* Append cardId to the saved Fusion Deck (max FUSION_DECK_MAX_CARDS). */
void AddCardToSavedFusionDeck(u16 cardId);
/* trunk[cardId].count-- and trunkSize-- (unless already 0). */
void RemoveCardFromTrunk(u16 cardId);
/* Remove the first cardId from the saved Deck and close the gap (only if deckCopies is non-zero). */
void RemoveCardFromSavedDeck(u16 cardId);
/* As RemoveCardFromSavedDeck, for the Side Deck. */
void RemoveCardFromSavedSideDeck(u16 cardId);
/* As RemoveCardFromSavedDeck, for the Fusion Deck. */
void RemoveCardFromSavedFusionDeck(u16 cardId);
/* Move copies over the copy limit from the saved Deck and Side Deck back to the trunk. Dead code. */
void TrimSavedDecksToCopyLimits(void);
/* Load player 0's duel deck and fusion deck from the saved Deck and Fusion Deck. */
void LoadPlayerDeckFromSave(void);

/* --- Duel records --------------------------------------------------------------------------------------- */

/* duelRecords[opponent].wins++ (saturating) and lastOpponent = opponent. */
void RecordDuelWin(u32 opponent);
/* duelRecords[opponent].losses++ (saturating) and lastOpponent = opponent. */
void RecordDuelLoss(u32 opponent);
/* duelRecords[opponent].draws++ (saturating) and lastOpponent = opponent. */
void RecordDuelDraw(u32 opponent);
/* championshipWins++ unless it is 0xFF. */
void IncrementChampionshipWins(void);

#endif /* GUARD_SAVE_H */
