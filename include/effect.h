#ifndef GUARD_EFFECT_H
#define GUARD_EFFECT_H

/*
 * Card effects: the effect table and the rules around it.
 *
 * Every card with an effect has a row in gCardEffects (struct CardEffect), 426 rows sorted by card number.
 * Numbers 1211-1552 are effect keys with no EDS card (gCardNumberToId[n] == 0): engine leftovers that some
 * EDS cards reuse, named after their behaviour. A row holds up to five handlers, which the chain code
 * (include/chain.h) calls with struct ChainEntry links:
 *
 *   prepare  activation condition: may the card be activated now (on the field or from the hand)?
 *   check    target filter, called for every board position pos = player | zone << 8; an effect with a
 *            check handler can only be activated when at least one position passes (CanActivateEffect)
 *   chainA   first step when the link is added to the chain: pay the cost (tribute, discard, LP, ...)
 *   chainB   second step: choose the targets (link->targets[], link->numTargets)
 *   resolve  apply the effect when the chain resolves; multi-step handlers return the next enum EffectStep
 *            value (kept in gChain.effectStep) until they return EFFECT_STEP_DONE
 *
 * chainA and chainB return 1 when done and 0 to be called again next frame. A NULL slot is skipped (a row
 * without prepare and check is always activatable). The handlers are declared in include/effect_handlers.h.
 *
 * This header also has the shared helpers of the effect units: spell speed and chain-response rules,
 * target selection, Fusion and Ritual recipes, the global negation flags (Jinzo, Royal Decree, Imperial
 * Order) and the passive card hooks that the duel flow calls directly.
 *
 * Positions: "pos" and the target words are player | zone << 8 (zones 0-4 monsters, 5-9 spell/trap,
 * 10 field). Card numbers ("number") are the game's card-list numbers; card ids ("cardId") are the
 * alphabetical ids stored in card words (gCardIdToNumber converts an id to its number).
 *
 * Code: src/effect_*.c, card_list_viewer.c, duel_ritual.c. Wiki: functions/effect-*-c.md (CollectEffectTargets:
 * functions/effect-target-collect-c.md), functions/card-list-viewer-c.md, functions/duel-ritual-c.md.
 */

#include "global.h"

struct ChainEntry;

/* --- Effect table --------------------------------------------------------------------------------------- */

/* One row of gCardEffects (0x0819A9D4, 426 rows, sorted by number for FindCardEffect's binary search).
 * Units view the rows through partial local structs today (TargetRule, EffEntry, EffEnt, Ent42BE0) whose
 * handler pointer types differ; a migrated unit keeps its view where the call's codegen needs it. */
struct CardEffect {
    u16 number;     /* +0x00: card number, or an effect key (1211-1552: no EDS card) */
    u16 flags;      /* +0x02: 0 in every row */
    /* +0x04: apply the effect (resolve step); the second link is the one this link answers.
     * EffectFairysHandMirrorResolve calls another card's resolve through gChain.proxyResolve. */
    int (*resolve)(struct ChainEntry *link, struct ChainEntry *chainedTo);
    /* +0x08: may the effect touch the card at pos = player | zone << 8? */
    u16 (*check)(struct ChainEntry *card, u16 pos);
    /* +0x0C: activation condition (chainLink: the link being answered or NULL; fromHand: 1 when played
     * from the hand, 0 on the field) */
    int (*prepare)(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
    /* +0x10: first step when the link is added to the chain (Chain_Build): pay the activation cost.
     * Returns 1 when done; its step byte is gChain.costStep. */
    int (*chainA)(struct ChainEntry *link, struct ChainEntry *prevLink);
    /* +0x14: second step: choose the targets. Returns 1 when done; its step byte is gChain.targetStep. */
    int (*chainB)(struct ChainEntry *link, struct ChainEntry *prevLink);
};

typedef char effect_h_check_card_effect_size[sizeof(struct CardEffect) == 0x18 ? 1 : -1];
typedef char effect_h_check_card_effect_check[(u32)&((struct CardEffect *)0)->check == 0x08 ? 1 : -1];
typedef char effect_h_check_card_effect_chain_b[(u32)&((struct CardEffect *)0)->chainB == 0x14 ? 1 : -1];

/* The effect table. Declared as const u32[] or as partial row structs in the units today; a migrated
 * unit that needs its old view for codegen keeps a commented local alias. */
extern const struct CardEffect gCardEffects[];

/* Return value of a multi-step resolve handler, kept in gChain.effectStep: Chain_Resolve calls the
 * handler with EFFECT_STEP_START first, then once per frame with the value it returned last, until it
 * returns EFFECT_STEP_DONE. The handlers count down from 0x80; a value a handler has no case for (0x78 or
 * 0x0A in some) ends the link through its default case on the next call. Some units number 0x7F as
 * STEP_1; this header uses one scheme for all. */
enum EffectStep {
    EFFECT_STEP_DONE = 0,       /* the link is finished */
    EFFECT_STEP_END = 100,      /* 0x64: the last step of many handlers (show the result, clean up) */
    EFFECT_STEP_10 = 119,       /* 0x77 */
    EFFECT_STEP_9 = 120,        /* 0x78 */
    EFFECT_STEP_8 = 121,        /* 0x79 */
    EFFECT_STEP_7 = 122,        /* 0x7A */
    EFFECT_STEP_6 = 123,        /* 0x7B */
    EFFECT_STEP_5 = 124,        /* 0x7C */
    EFFECT_STEP_4 = 125,        /* 0x7D */
    EFFECT_STEP_3 = 126,        /* 0x7E */
    EFFECT_STEP_2 = 127,        /* 0x7F */
    EFFECT_STEP_START = 128,    /* 0x80: first call (set by Chain_Resolve) */
};

/* gChain.effectStep values of EffectRitualSummonResolve (src/duel_ritual.c); it starts with
 * EFFECT_STEP_START like every resolve handler. */
enum RitualStep {
    RITUAL_STEP_END = 98,           /* 0x62 */
    RITUAL_STEP_SUMMON = 99,        /* 0x63: Special Summon the ritual monster */
    RITUAL_STEP_TAKE_MONSTER = 100, /* 0x64: take the ritual monster out of the hand */
    RITUAL_STEP_CPU_TRIBUTE = 120,  /* 0x78: the CPU picks its tributes */
};

/* Spell speed, returned by GetCardSpellSpeed. A link can only be answered by a card of equal or higher
 * speed, and speed 1 cannot answer at all (CanActivateEffect, CanChainFieldCard, CanChainHandCard). */
enum SpellSpeed {
    SPELL_SPEED_NONE = 0,   /* no effect (Normal monsters, ...) */
    SPELL_SPEED_1 = 1,      /* Magic, effect monsters */
    SPELL_SPEED_2 = 2,      /* Traps, Quick-Play Magic, flip-effect monsters */
    SPELL_SPEED_3 = 3,      /* Counter Traps */
};

/* --- Values the effect handlers store in link->targets[] ------------------------------------------------ */

/* Coin-toss call and result: gTextBox.sel of the 'Coin-toss Selection' menu (Heads listed first) and
 * the DUEL_CMD_TOSS_COIN result (Random() & 1). */
enum CoinFace {
    COIN_FACE_HEADS = 0,
    COIN_FACE_TAILS = 1,
};

/* targets[0] of EffectGainLpChosenPlayerChainB: the answer of the 'whose LP' menu (gTextBox.result),
 * read by EffectGainLpChosenPlayerResolve. Relative to the activating player. */
enum ChosenPlayer {
    CHOSEN_PLAYER_SELF = 0,
    CHOSEN_PLAYER_OPPONENT = 1,
};

/* targets[1] of EffectSevenCompletedChainB (gTextBox.result + 1): the stat 7 Completed raises; stored
 * as the zone's declaredValue by EffectEquipResolve. */
enum SevenCompletedStat {
    SEVEN_COMPLETED_ATK = 1,
    SEVEN_COMPLETED_DEF = 2,
};

/* --- Fusion and Ritual recipes -------------------------------------------------------------------------- */

/* One recipe of gFusionRecipes2 (0x0819A7C8, 2 materials, materials[2] = 0) or gFusionRecipes3
 * (0x0819A970, 3 materials). Both tables end with result 0x3E7; the terminator test reads result and
 * materials[0] together as one u32 (0x03E703E7). */
struct FusionRecipe {
    u16 result;         /* +0x0: card number of the Fusion monster */
    u16 materials[3];   /* +0x2: card numbers of the materials */
};

typedef char effect_h_check_fusion_recipe_size[sizeof(struct FusionRecipe) == 0x8 ? 1 : -1];

/* One row of gRitualRecipes (src/duel_ritual.c); the table ends at ritualSpell == 0. */
struct RitualRecipe {
    u32 monster:13;     /* bits 0-12: card number of the ritual monster */
    u32 ritualSpell:13; /* bits 13-25: card number of its ritual spell */
    u32 level:6;        /* bits 26-31: level total the tributes must reach */
};

typedef char effect_h_check_ritual_recipe_size[sizeof(struct RitualRecipe) == 0x4 ? 1 : -1];

/* Fusion slot words (FindFusionMaterialSlot, GetFusionSlotCardId, FindFusionMaterials' slots[],
 * gChain.fusionMaterialSlots): a flag ORed with the index of the material (hand index or monster zone). */
enum FusionSlot {
    FUSION_SLOT_FIELD = 0x4000,     /* | monster zone (0-4) */
    FUSION_SLOT_HAND = 0x8000,      /* | hand index */
    FUSION_SLOT_NONE = 0xFFFF,      /* no card found */
};


/* --- Effect table lookup -------------------------------------------------------------------------------- */

/* Index of the gCardEffects row of a card id (binary search on its card number), or -1. */
int FindCardEffect(u32 cardId);
/* Does the Check handler of card's effect accept (player, zone)? 1 when the card has no row or no Check
 * handler, 0 for card == NULL. card is a struct ChainEntry (the definition takes u16 *). */
u16 CanEffectTargetZone(u16 *card, int player, int zone);

/* --- Activation and chain-response rules ---------------------------------------------------------------- */

/* Spell speed of a card (enum SpellSpeed): Counter Trap 3; Trap, Quick-Play Magic and flip-effect monsters
 * 2; other Magic and effect monsters 1; anything else 0. */
int GetCardSpellSpeed(u16 cardId);
/* 1 if the effect of card may be activated now, in answer to chainLink (NULL: no link). Checks
 * prohibition, spell speed, the damage-step whitelist, then the row's prepare and check handlers. */
int CanActivateEffect(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* CanActivateEffect for a card id with no link. Only card and player of the temporary entry are set: zone
 * and event are stack garbage that some prepare handlers read. */
int CanActivateEffectOfCard(int player, u16 cardId, u16 fromHand);
/* CanActivateEffect for the card in (player, zone) answering event; 0 for an empty zone. */
u16 CanActivateEffectInZone(int player, int zone, u16 event);
/* 1 if the chain or the pending list already holds an entry from (player, zone). */
u16 IsZoneInActionLists(int player, int zone);
/* Can the set Magic/Trap in (player, zone) be activated in answer to chainLink? Speed, the zone's
 * canActivate/isDisabled bits and Jinzo, then CanActivateEffect. */
u16 CanChainFieldCard(struct ChainEntry *chainLink, int player, int zone);
/* Can the Quick-Play Magic hand[handIdx] be activated in answer to chainLink? */
u16 CanChainHandCard(struct ChainEntry *chainLink, int player, int handIdx);
/* 1 if player has any answer to chainLink: a set card in zones 5-9, a Quick-Play Magic in the hand (turn
 * player only), or key 1525 on the field against a Magic card. */
int CanPlayerChain(struct ChainEntry *chainLink, int player);

/* --- Targets -------------------------------------------------------------------------------------------- */

/* May the effect of card cardId target the card in (player, zone)? 0 for an empty zone, 1 for a face-down
 * card; Lord of D. protects face-up Dragons, Umi protects keys 1326/1329 from Magic. */
u16 CanCardTargetZone(u16 cardId, int player, int zone);
/* Card-independent CanCardTargetZone: 1 if (player, zone) holds a card that Umi does not protect (keys
 * 1326/1329). */
int IsZoneTargetable(int player, int zone);
/* Append a target word (player | zone << 8, or half of a card word) to card->targets. No bounds check:
 * callers add at most 3. */
void AddEffectTarget(struct ChainEntry *card, u16 target);
/* If CanCardTargetZone allows it: confirm sound for the human, point at the card (DUEL_CMD_POINT_AT_CARD)
 * and AddEffectTarget; 1 if added. Callers depend on the u16 return. */
u16 TryAddEffectTarget(struct ChainEntry *card, int player, int zone);
/* TryAddEffectTarget without the CanCardTargetZone test, for effects that check the zone themselves. */
void AddEffectTargetUnchecked(struct ChainEntry *card, int player, int zone);
/* Fill gCardListView (cards, sources, count) with the candidates of cardNumber's effect for player; param
 * is a per-card extra key (a level, a player, ...). Returns the count. */
u16 CollectEffectTargets(int player, u16 cardNumber, int param);
/* Fairy's Hand Mirror (1073) and key 1317: may the single-target effect of link move onto (player, zone)?
 * The new position must hold a card, differ from targets[0] and pass link's own Check handler. */
u16 CanRedirectEffectToZone(u16 effectNumber, struct ChainEntry *link, int player, int zone);
/* Number of positions CanRedirectEffectToZone accepts (1073: both players' monster zones; 1317: player's
 * own; other numbers 0). */
int CountRedirectTargets(u16 effectNumber, int player, struct ChainEntry *link);

/* --- Destruction by card effects ------------------------------------------------------------------------ */

/* Destroy the card in (player, zone) by a card effect. Sangan, Witch of the Black Forest and keys
 * 1241/1257 are flagged first so that their graveyard trigger runs. */
void DestroyFieldCardByEffect(int player, int zone);
/* Called after an effect of srcPlayer destroyed (player, zone): arms the delayed trigger of key 1514 (not
 * in EDS) when it was destroyed by the opponent. */
void OnCardDestroyedByEffect(int srcPlayer, int player, int zone);

/* --- Fusion --------------------------------------------------------------------------------------------- */

/* 1 for the four cards that may stand in for any one Fusion material (Goddess with the Third Eye,
 * Beastking of the Swamps, Versago the Destroyer, Mystical Sheep #1). */
u16 IsFusionSubstitute(u16 cardNumber);
/* 1 if matA + matB (either order, at most one substitute) form a gFusionRecipes2 recipe for resultId. */
int CheckFusionRecipe2(int resultId, int matA, int matB);
/* 1 if matA, matB, matC (any order, at most one substitute) form a gFusionRecipes3 recipe for resultId.
 * Gives up at the first recipe with that result. */
int CheckFusionRecipe3(u16 resultId, u16 matA, u16 matB, u16 matC);
/* CheckFusionRecipe3, or CheckFusionRecipe2 when matC is 0. Unused (no callers). */
int CheckFusionRecipe(u16 resultId, u16 matA, u16 matB, u16 matC);
/* 1 if materialId is a material of some recipe for fusionId, or a Fusion substitute (alternate-art numbers
 * 2000+ count as the original). */
int IsMaterialOfFusion(int fusionId, int materialId);
/* Fusion slot (enum FusionSlot | index) of a card with that number on player's field, else in the hand,
 * skipping excludeA/excludeB; substitutes in a second pass. FUSION_SLOT_NONE if none. */
u16 FindFusionMaterialSlot(int player, u16 cardNumber, u16 excludeA, u16 excludeB);
/* Card id at a fusion slot word (hand index or monster zone), 0 for anything else. */
u16 GetFusionSlotCardId(int player, u16 slot);
/* 1 if player holds the materials of the Fusion monster fusionId; slots[] receives their fusion slot words
 * (last material first). */
int FindFusionMaterials(int player, u16 fusionId, u16 *slots);
/* 1 if cardId's card number is one of the materials still to pick (gChain.fusionMaterials). */
int IsPendingFusionMaterial(u16 cardId);
/* Clear cardId's entry in gChain.fusionMaterials; a substitute clears the first entry that is itself a
 * substitute. */
void RemovePendingFusionMaterial(u16 cardId);

/* --- Ritual summons ------------------------------------------------------------------------------------- */

/* Index of the gRitualRecipes row whose ritual spell is card id cardId, or -1. */
int FindRitualRecipe(u16 cardId);
/* 1 if player's hand holds the ritual monster of gRitualRecipes[recipeIdx]. */
int HandHasRitualMonster(int player, int recipeIdx);
/* Total level of player's hand, skipping the first card with id excludeId (Magic, Trap and Ticket count 0,
 * Divine-Beasts 10). */
int SumHandLevelsExcept(int player, u16 excludeId);
/* Total level of player's monsters that IsTributableMonster accepts. */
int SumTributableMonsterLevels(int player);
/* Text-box draw callback of the ritual tribute prompt: one star per required level (gChain.effectCount),
 * the unpaid ones in another palette. */
void DrawRitualStarGauge(void);
/* Text-box step callback of the human's ritual tribute pick (monsters or hand cards); 1 once the tributes
 * cover the required level. */
int RitualTributeSelectStep(void);
/* 1 if graveyard[graveIdx] may be Special Summoned from the graveyard: Normal monsters always, Effect
 * monsters unless special-summon-only, Fusion and Ritual monsters only after a proper summon (card-word
 * bit 14, hypothesis). */
int CanReviveGraveyardCard(int player, int graveIdx);

/* --- Global negation (Jinzo, Royal Decree, Imperial Order) ---------------------------------------------- */

/* 1 if Jinzo (751) or Royal Decree (1033) is active on either field: Traps are negated. */
int IsJinzoOrRoyalDecreeActive(void);
/* 1 if Imperial Order (1154) is active on either field: Magic is negated. */
int IsImperialOrderActive(void);
/* Recompute gDuel's trap, magic and equip-magic negation flags, then queue
 * DUEL_CMD_SET_SPELL_TRAP_DISABLED for every face-up spell/trap whose isDisabled bit must change. */
void UpdateSpellTrapNegation(void);
/* Queue DUEL_CMD_SET_SPELL_TRAP_DISABLED for every face-up Trap in both players' spell/trap zones. */
void DisableFaceUpTraps(void);

/* --- Passive card hooks (called by the duel flow, not through gCardEffects) ----------------------------- */

/* Pumpking the King of Ghosts in (player, zone): with Castle of Dark Illusions active, link a
 * ZONE_LINK_STATS_UP_100 boost to it. */
void ApplyPumpkingBoost(int player, int zone);
/* Each active Mysterious Puppeteer (165) gives its controller 500 LP. */
void TriggerMysteriousPuppeteer(int player);
/* Dragon Capture Jar active: switch every face-up attack-position Dragon to Defense Position. */
void ApplyDragonCaptureJar(int player);
/* Standby Phase: offer to return Sinister Serpent from the graveyard to the hand (the CPU always accepts).
 * 0 while running, 1 when done. */
int SinisterSerpentStandbyStep(int player);
/* player pays 500 LP per active Chain Energy (1078) on the field. */
void PayChainEnergyCost(int player);
/* Kotodama face up: destroy every face-up monster that has a face-up same-name duplicate. */
void ApplyKotodama(void);
/* Kotodama check for one monster: destroy (player, zone) if it has a face-up same-name duplicate. */
void ApplyKotodamaToZone(int player, int zone);
/* player draws 2 cards per active Appropriate (1141). */
void TriggerAppropriate(int player);
/* The opponent of player discards `discarded` cards per active Forced Requisition (1142) of player. */
void TriggerForcedRequisition(int player, int discarded);
/* Key 1306 (not in EDS) active on either field: player loses 300 LP per card sent to the graveyard
 * (count). */
void LoseLpOnSendToGraveyard(int player, int count);
/* Answer to the "equip it to a monster?" prompt for graveyard[graveIdx]: equip != 0 equips it to the
 * opponent's monster, 0 clears the pending equip. */
void ResolvePendingGraveyardEquip(int player, int graveIdx, u16 equip);
/* Place the next of keys 1541-1544 (chosen by the turn counter of the card in boardZone) from the deck,
 * else the hand (hypothesis: Destiny Board's Spirit Messages). */
void PlaceNextSpiritMessage(int player, int boardZone);
/* Key 1533 (not in EDS) active for player: the opponent loses 100 LP per monster in its banished pile. */
void DamageOpponentPerBanishedMonster(int player);
/* Key 1536 (not in EDS): roll a die and destroy the face-up monsters of that level. Unused: the Standby
 * Phase handler has an inlined copy. */
void RollDieDestroyMonstersByLevel(int player);

/* --- Texts shared by several effect units (ROM) --------------------------------------------------------- */

/* Prompts of the effect handlers; @2/@3 switch the text colour, @0 restores it. Texts used by one unit
 * stay local externs of that unit. */
/* 0x08082988 Elegant Egotist: pick a monster to Special Summon from the deck or hand */
extern const u8 gStrElegantEgotistSelectPrompt[];
/* 0x080829F0 Cyber-Stein: pick a monster to Special Summon from the Fusion Deck */
extern const u8 gStrCyberSteinSelectPrompt[];
/* 0x08082A4C Gale Dogra: pick a Fusion Deck monster to send to the graveyard */
extern const u8 gStrGaleDograSelectPrompt[];
/* 0x08082AB4 Thunder Dragon: "add %s to your hand from your Deck?" (%s = card name) */
extern const u8 gStrThunderDragonAddPromptFmt[];
/* 0x08082AF0 Needle Ball: "pay 2000LP?" */
extern const u8 gStrNeedleBallPayLpPrompt[];
/* 0x08082B10 Yado Karu: return a hand card to the bottom of the deck? (Yes/No) */
extern const u8 gStrYadoKaruReturnPrompt[];
/* 0x08082B58 Yado Karu: pick the hand card to return to the deck */
extern const u8 gStrYadoKaruSelectPrompt[];
/* 0x08082D64 Cheerful Coffin: discard a Monster card from the hand? (Yes/No) */
extern const u8 gStrCheerfulCoffinDiscardPrompt[];
/* 0x08082DB4 Cheerful Coffin: pick the Monster card to discard */
extern const u8 gStrCheerfulCoffinSelectMonster[];
/* 0x08082DE8 Widespread Ruin: several attack-position monsters share the top ATK (%d), pick one */
extern const u8 gStrWidespreadRuinTiePrompt[];
/* 0x08082E7C Painful Choice: "Select 5 cards from your Deck." */
extern const u8 gStrPainfulChoiceSelect5[];
/* 0x08082E9C Painful Choice: "%d cards remaining" (another copy at 0x08085758) */
extern const u8 gStrPainfulChoiceCardsRemaining[];
/* 0x08082EC0 Dust Tornado: set a Magic/Trap card from the hand? (Yes/No) */
extern const u8 gStrDustTornadoSetPrompt[];
/* 0x08082EF4 Dust Tornado: pick the Magic/Trap cards to set */
extern const u8 gStrDustTornadoSelectCards[];
/* 0x08082F34 "There are no cards to be added from your Deck." (Sangan has its own copy) */
extern const u8 gStrRecruiterNoCardsInDeck[];
/* 0x08082F64 recruiters: Special Summon a monster to the field? (Yes/No) */
extern const u8 gStrRecruiterSummonPrompt[];
/* 0x08082FA4 recruiters: pick the deck monster to Special Summon */
extern const u8 gStrRecruiterSelectMonster[];
/* 0x08083020 Senju: add a Ritual Monster from the deck to the hand? (Yes/No) */
extern const u8 gStrSenjuAddRitualMonsterPrompt[];
/* 0x0808306C Sonic Bird: add a Ritual Magic card from the deck to the hand? (Yes/No) */
extern const u8 gStrSonicBirdAddRitualMagicPrompt[];
/* 0x080830B4 Senju / Sonic Bird: pick the deck card to add to the hand */
extern const u8 gStrRitualSearchSelectCard[];
/* 0x08083104 Giant Germ: Special Summon %s to the field? (%s = card name) */
extern const u8 gStrGiantGermSummonPrompt[];
/* 0x0808313C Nimble Momonga and key 1307: set %s on the field? (%s = card name) */
extern const u8 gStrSameNameSetPrompt[];
/* 0x080831E4 Time Wizard, Goddess of Whim: "Coin-toss Selection" menu (Heads, Tails) */
extern const char gStrCoinTossSelection[];
/* 0x08083300 key 1220: use the tribute to Summon a monster or to activate the effect? (menu) */
extern const char gStrPromptTributeUse[];
/* 0x08083350 pick a high-level monster to Summon from the hand */
extern const char gStrSelectHighLevelMonster[];
/* 0x0808339C pick one more monster as the second tribute */
extern const char gStrSelectSecondTribute[];
/* 0x080833EC pick the Effect Monster whose effect to activate */
extern const char gStrSelectEffectMonster[];
/* 0x08083430 EffectBerfometResolve: "add %s from the Deck to your hand?" */
extern const u8 gStrAddFromDeckToHandPrompt[];
/* 0x08083468 "%s has been sent to the Graveyard", equip it to a monster? (Yes/No) */
extern const u8 gStrEquipFromGraveyardPrompt[];
/* 0x080834CC keys 1242/1257: pick the monster to equip with %s */
extern const u8 gStrSelectEquipTarget[];
/* 0x08083508 EffectGainOpponentMonsterStatsResolve: pick the opponent monster to copy ATK/DEF from */
extern const u8 gStrSelectStatsSourceMonster[];
/* 0x08083558 EffectGambleResolve, EffectFairyBoxResolve: "Coin-toss Selection" menu (Heads, Tails) */
extern const u8 gStrCoinTossMenu[];
/* 0x08083588 EffectInsectImitationResolve: pick the deck monster to Special Summon */
extern const u8 gStrSelectDeckMonsterToSummon[];
/* 0x080837D4 key 1511: banish a monster from the opponent's graveyard? (first pick) */
extern const char gStrBanishOpponentGraveMonsterQuestion[];
/* 0x08083828 key 1511: banish another one? (later picks) */
extern const char gStrBanishAnotherOpponentGraveMonsterQuestion[];
/* 0x08083880 key 1511: pick the monster in the opponent's graveyard to banish */
extern const char gStrSelectOpponentGraveMonsterToBanish[];
/* 0x080838D4 key 1512: designate one of your monsters as Tribute */
extern const char gStrDesignateOwnMonsterToTribute[];
/* 0x08083904 key 1512: pick a Fusion Deck monster to Special Summon (separate copy of the Cyber-Stein
 * text) */
extern const char gStrSelectFusionToSummonForTribute[];
/* 0x08083960 key 1513: banish another graveyard card? (later picks) */
extern const char gStrBanishAnotherGraveCardQuestion[];
/* 0x080839A4 key 1513: banish a card from the graveyard? (first pick) */
extern const char gStrBanishGraveCardQuestion[];
/* 0x080839DC key 1513: pick the graveyard card to banish */
extern const char gStrSelectGraveCardToBanishForAtk[];
/* 0x08083A14 keys 1517/1519: pick the opponent's face-up monster whose position changes */
extern const char gStrSelectOpponentMonsterToChangePosition[];
/* 0x08083A6C key 1524: pick the Fusion material monster to add to the hand */
extern const char gStrSelectFusionMaterialToAddToHand[];
/* 0x08083DCC default prompt of EffectOpponentMonsterChainB / EffectTakeControlChainB */
extern const u8 gStrDesignateOpponentMonsterTarget[];
/* 0x08083E14 EffectEquipTargetChainB, EffectSevenCompletedChainB, key 1320: pick the monster to equip */
extern const u8 gStrDesignateMonsterToEquip[];
/* 0x08083FD0 pick a monster to destroy (also key 1532) */
extern const u8 gStrDesignateMonsterToDestroy[];
/* 0x08084044 default of EffectTargetableMonsterChainB / EffectTargetableFaceUpMonsterChainB */
extern const u8 gStrDesignateOneMonster[];
/* 0x080844E4 Two-Pronged Attack, key 1213: pick the opponent's monster to destroy */
extern const u8 gStrDesignateOpponentMonsterToDestroy[];
/* 0x08084740 Dust Tornado: pick the opponent's Magic/Trap card to destroy */
extern const u8 gStrDesignateOpponentSpellTrapToDestroy[];
/* 0x08084930 key 1248: format "one face-up %s or %s monster" (Summoned Skull, gStrThunderType) */
extern const char gStrDesignateFaceUpMonsterOfTwoFmt[];
/* 0x08084968 "Thunder", the second %s of key 1248's prompt */
extern const char gStrThunderType[];
/* 0x08084970 key 1255: format "pick an opponent's %s monster to control" */
extern const char gStrSelectOpponentMonsterToControlFmt[];
/* 0x080849AC "Machine", the %s of key 1255's prompt */
extern const char gStrMachineType[];
/* 0x0819D1C4 the five tribute-summon prompts (requires 1 / 2 Tributes, select the 1st / 2nd tribute, ...) */
extern const char * const gTributeSummonPrompts[];

#endif /* GUARD_EFFECT_H */
