#ifndef GUARD_EFFECT_HANDLERS_H
#define GUARD_EFFECT_HANDLERS_H

/*
 * The card effect handlers: every function that gCardEffects (include/effect.h, struct CardEffect) points to.
 * Nothing calls them by name; the chain code calls them through the table. These declarations document the
 * table and give each handler's exact signature as defined today. The signatures differ from the table's
 * slot types (int or u16 returns, unused trailing parameters, a u16 * for a link); the table stores them as
 * plain function pointers.
 *
 * Slots and parameters (see struct CardEffect):
 *   Prepare(card, chainLink, fromHand)  activation condition; 1 = may activate
 *   Check(card, pos)                    target filter for pos = player | zone << 8; nonzero = valid target
 *   ChainA(link, prevLink)              activation cost when the link joins the chain; 1 = done, 0 = call again
 *   ChainB(link, prevLink)              target selection (CPU at once, human through prompts and the cursor);
 *                                       1 = done (possibly without a target), 0 = call again
 *   Resolve(link, chainedTo)            the effect; returns the next enum EffectStep, 0 when finished
 * chainLink / prevLink / chainedTo is the link this one answers (NULL for none). Handlers that do not use the
 * trailing parameters are defined with fewer of them; a link parameter defined as int or u16 still receives
 * the pointer (those definitions only pass it on or ignore it, and match that way).
 *
 * Order: first the handlers shared by several unrelated cards, by slot; then the handlers of each card in
 * card-number order (Prepare, Check, ChainA, ChainB, Resolve). A handler used by two related cards is listed
 * under the first one. Numbers 1211-1552 are effect keys without an EDS card ("key N"); "(hypothesis: OCG X)"
 * names the OCG card a key behaves like.
 *
 * Code: src/effect_*.c, card_list_viewer.c, duel_ritual.c (EffectRitualSummon*), effect_hooks.c.
 */

#include "global.h"

struct ChainEntry;

/* --- Shared handlers ------------------------------------------------------------------------------------ */

/* Prepare (activation conditions) */
/* On the field with its once-per-turn effect unused (Goddess of Whim, Barrel Dragon, key 1112). */
int EffectOncePerTurnPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Ritual spells: can Special Summon, the ritual monster is in the hand and hand plus field cover its
 * level. */
int EffectRitualSummonPrepare(struct ChainEntry *card);
/* Trap Hole, House of Adhesive Tape, Eatgaboon: the opponent summoned a face-up monster that fails the
 * card's ATK/DEF test. */
int EffectTrapHolePrepare(struct ChainEntry *card);
/* Attack-declaration traps (Widespread Ruin, Mirror Force, Negate Attack, Magical Hats, ...): the opponent
 * declared an attack, plus a per-card test. */
int EffectAttackResponsePrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* The opponent has a hand card (Lightforce Sword, The Inexperienced Spy, Confiscation, ...). */
int EffectOpponentHasHandPrepare(struct ChainEntry *card);
/* White Hole, Call of the Grave, Anti Raigeki, Gryphon Wing, keys 1247/1531: answering the one opponent
 * Magic card (or set of cards) the trap counters. */
int EffectSpellResponsePrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Event-triggered traps (Gust, Driving Snow, Armored Glass, World Suppression, Appropriate, Major Riot,
 * ...): the event the trap answers, per card number. */
int EffectEventResponsePrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Keys 1430/1433/1444: tributes allowed and a graveyard monster destroyed in battle this turn. */
int EffectTributeRecoverGraveMonsterPrepare(struct ChainEntry *card);
/* Keys 1541-1544: always 0; they are only placed by PlaceNextSpiritMessage. */
int EffectSpiritMessagePrepare(void);

/* Check (target filters) */
/* Larvae Moth, Great Moth, Perfectly Ultimate Great Moth, Wall Shadow: own Petit Moth (Labyrinth Wall)
 * under Cocoon of Evolution (Magical Labyrinth) for enough turns. */
int EffectEquippedTributeCheck(struct ChainEntry *card, u16 pos);
/* Equip spells: a face-up targetable monster that suits the card (own side only, a monster type or
 * attribute, or one specific monster) (53 rows). */
int EffectEquipTargetCheck(struct ChainEntry *card, u16 pos);
/* Crass Clown, Dream Clown, Spellbinding Circle, Change of Heart, ...: an opponent's monster the card may
 * target. */
int EffectOpponentMonsterCheck(struct ChainEntry *card, u16 pos);
/* Any monster, face up or down, the card may target. */
int EffectTargetableMonsterCheck(struct ChainEntry *card, u16 pos);
/* Warrior Elimination, Eternal Rest, Stain Storm, ...: a face-up monster of the type the card destroys
 * (Eternal Rest: an equipped monster). */
int EffectDestroyByTypeCheck(struct ChainEntry *card, u16 pos);
/* Face-up monster the card may target. */
int EffectTargetableFaceUpMonsterCheck(struct ChainEntry *card, u16 pos);
/* Opponent's face-up monster (no targeting test): Fissure, key 1177, key 1244. */
int EffectOpponentFaceUpMonsterCheck(struct ChainEntry *card, u16 pos);
/* Any card in a spell/trap or field zone (Heavy Storm, Mystical Space Typhoon, Giant Trunade, ...). */
int EffectAnySpellTrapCheck(struct ChainEntry *card, u16 pos);

/* ChainA (activation costs) */
/* Activation cost: pay the card's LP cost (500 to 5000, half for Solemn Judgment). Always returns 1. */
int EffectPayLifePointsChainA(struct ChainEntry *link);
/* Activation cost: tribute the activating monster itself. */
int EffectTributeSelfChainA(struct ChainEntry *link, u16 unused);
/* Activation cost: discard 1 (Tribute to The Doomed, Magic Jammer), 2 (Darkness Approaches) or 5 (Final
 * Destiny) cards. Always returns 1. */
int EffectDiscardCostChainA(struct ChainEntry *link);

/* ChainB (target selection) */
/* Equip spells: pick the monster to equip (CPU: the strongest one it accepts, harmful equips on the
 * opponent). No target for the sent-to-graveyard trigger (49 rows). */
int EffectEquipTargetChainB(struct ChainEntry *link);
/* Catapult Turtle, Cannon Soldier, The Little Swordsman of Aile: pick one of your monsters to tribute. */
int EffectTributeTargetChainB(struct ChainEntry *link);
/* Crass Clown, Dream Clown, Spellbinding Circle, Barrel Dragon, ...: pick an opponent's monster (CPU: its
 * strongest). */
int EffectOpponentMonsterChainB(struct ChainEntry *link);
/* Monster Reborn, Mask of Darkness, Graverobber, Premature Burial, ...: pick the target card in the
 * card-list viewer; stores the card word in targets[0..1]. */
int EffectCardListTargetChainB(struct ChainEntry *link);
/* Mooyan Curry, Goblin's Secret Remedy, Soul of the Pure, Blue Medicine: ask whose LP to recover (enum
 * ChosenPlayer in targets[0]; the CPU picks itself). */
int EffectGainLpChosenPlayerChainB(struct ChainEntry *link);
/* Man-Eater Bug, Hane-Hane, Tribute to The Doomed, ...: pick any monster (CPU: the human's strongest or
 * its own weakest). */
int EffectTargetableMonsterChainB(struct ChainEntry *link, int prevLink);
/* Invader of the Throne, Change of Heart, Snatch Steal, keys 1244/1514: pick the opponent's monster to
 * take (CPU: its strongest). */
int EffectTakeControlChainB(struct ChainEntry *link);
/* Bell of Destruction, keys 1330/1451/1527: pick a face-up monster (CPU: the human's strongest). */
int EffectTargetableFaceUpMonsterChainB(struct ChainEntry *link);
/* Magical Hats, keys 1316/1319: pick one of your monsters (no CPU branch). */
int EffectOwnMonsterTargetChainB(struct ChainEntry *link);
/* Reinforcements, Castle Walls, Rush Recklessly, The Reliable Guardian, Snake Fang, key 1534: pick a
 * face-up monster (no CPU branch). */
int EffectStatModifierTargetChainB(struct ChainEntry *link);
/* Mystical Space Typhoon, Gust, Driving Snow: pick a Magic/Trap card to destroy (CPU: the human's face-up
 * Magic first). */
int EffectSpellTrapTargetChainB(struct ChainEntry *link);

/* Resolve (effects) */
/* Resolve of continuous and passive cards whose effect lives in hooks elsewhere: returns EFFECT_STEP_DONE
 * at once (38 rows). */
int EffectNopResolve(void);
/* Equip spells: equip the card to targets[0]; 7 Completed also stores the chosen stat (45 rows). */
u16 EffectEquipResolve(struct ChainEntry *link);
/* Dream Clown, Man-Eater Bug, Mystical Space Typhoon, Gust, ...: destroy the single target (Magic cannot
 * hit Magic-immune monsters; a set Big Shield Gardna is only flipped). */
int EffectDestroyTargetResolve(struct ChainEntry *link);
/* Field Magic (14 rows): set the field background and post the field-change event. */
int EffectFieldMagicResolve(struct ChainEntry *link);
/* Recover LP for the chosen player: Mooyan Curry 200, Goblin's Secret Remedy 600, Soul of the Pure 800,
 * Blue Medicine 400. */
int EffectGainLpChosenPlayerResolve(struct ChainEntry *link);
/* Recover LP: Red Medicine 500, Dian Keto the Cure Master 1000, key 1315 1000 for both players. */
int EffectGainLpResolve(struct ChainEntry *link);
/* Burn the opponent: Sparks 200, Hinotama 500, Final Flame 600, Ookazi 800, Tremendous Fire 1000 (and 500
 * to its owner), Raimei 300, Restructer Revolution 200 per hand card. */
int EffectDamageOpponentResolve(struct ChainEntry *link);
/* Draw cards: Pot of Greed 2, Skelengel and key 1447 1. */
int EffectDrawCardsResolve(struct ChainEntry *link);
/* Warrior Elimination, Eternal Rest, Stain Storm, ...: destroy every monster the check accepts, the
 * opponent's first. */
int EffectDestroyAllByTypeResolve(struct ChainEntry *link);
/* Ritual spells: pay the tributes (human: text-box menu; CPU: highest level first) and Special Summon the
 * ritual monster (enum RitualStep). */
int EffectRitualSummonResolve(struct ChainEntry *link);
/* Trap Hole, House of Adhesive Tape, Eatgaboon: destroy the summoned monster (loc0) when it passes the
 * card's ATK/DEF test. */
int EffectTrapHoleResolve(struct ChainEntry *link);
/* Mirror Force, Enchanted Javelin, Widespread Ruin, keys 1214/1415: answer the attack of loc0 (Widespread
 * Ruin: the human breaks ATK ties). */
int EffectAttackResponseResolve(struct ChainEntry *link);
/* Jinzo, Royal Decree (Traps), Imperial Order (Magic), key 1537 (Equip Magic): disable the matching
 * face-up cards, then set the global negation flag. */
int EffectNegateTrapsOrMagicResolve(struct ChainEntry *link);
/* Reinforcements, Castle Walls, Rush Recklessly, The Reliable Guardian, Snake Fang, key 1534: link the
 * card to the target for its ATK/DEF change this turn (+500/+700 ATK or DEF, -500 DEF). */
int EffectAddStatModifierResolve(struct ChainEntry *link);
/* Counter traps (White Hole, Magic Jammer, Seven Tools of the Bandit, Gryphon Wing, ...): negate and
 * destroy chainedTo; some turn its effect back on its user. */
int EffectNegateChainedCardResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);
/* Giant Rat, UFO Turtle, Shining Fairy, Mother Grizzly, Flying Kamakiri #1, Mystic Tomato: Special Summon
 * a deck monster with ATK 1500 or less. */
int EffectSpecialSummonFromDeckResolve(struct ChainEntry *link);
/* Giant Germ, Nimble Momonga, key 1307: burn or heal, then summon more copies from the deck, one prompt
 * each (the CPU always accepts). */
int EffectSummonSameNameFromDeckResolve(struct ChainEntry *link);
/* Armored Glass, World Suppression, Mystic Probe, Metal Detector: negate a kind of card this turn, then
 * refresh the spell/trap negation. */
int EffectNegateCardsThisTurnResolve(struct ChainEntry *link);
/* Keys 1421/1430/1433/1444: pick a graveyard monster destroyed in battle; it goes to the deck top (1430),
 * deck bottom (1433) or hand (1444), or is marked (1421). */
int EffectRecoverGraveMonsterResolve(struct ChainEntry *link);

/* --- Handlers of one card (by card number) -------------------------------------------------------------- */

/* 15 Time Wizard */
/* On the field, once per turn, and the opponent has a monster. */
int EffectTimeWizardPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Coin toss: a right call destroys the opponent's monsters (then offers the Dark Sage summon); a wrong one
 * destroys your own and costs half their ATK. */
int EffectTimeWizardResolve(struct ChainEntry *link);

/* 39 Dragon Piper */
/* Destroy every face-up Dragon Capture Jar, then switch face-up defense-position Dragons to attack
 * position. */
int EffectDragonPiperResolve(struct ChainEntry *link);

/* 47 Sangan */
/* Add a candidate monster from the deck to the hand (also Witch of the Black Forest). */
int EffectSanganResolve(struct ChainEntry *link);

/* 71 Cocoon of Evolution */
/* From the hand only while the Normal Summon is unused. */
int EffectCocoonOfEvolutionPrepare(struct ChainEntry *card, int unused, u16 fromHand);

/* 82 Castle of Dark Illusions */
/* Link a continuous boost to every face-up Zombie on the field. */
int EffectCastleOfDarkIllusionsResolve(struct ChainEntry *link);

/* 83 Reaper of the Cards */
/* Spell/trap or field zone with a set card or a face-up Trap (also Trap Master). */
int EffectTrapTargetCheck(struct ChainEntry *card, u16 pos);
/* Pick a set card or face-up Trap to destroy; the CPU looks at the human's side first (also Trap Master). */
int EffectTrapTargetChainB(struct ChainEntry *link);
/* Destroy the target if it is a Trap; a set card is flipped and shown first, and flipped back when it is
 * not a Trap (also Trap Master). */
int EffectReaperOfTheCardsResolve(struct ChainEntry *link);

/* 88 Catapult Turtle */
/* Tribute the target and burn the opponent: half its ATK (Catapult Turtle) or 500 (also Cannon Soldier). */
int EffectCatapultTurtleResolve(struct ChainEntry *link);

/* 94 Crass Clown */
/* Return the target to its owner's hand (also Hane-Hane). */
int EffectReturnTargetToHandResolve(struct ChainEntry *link);

/* 101 Mask of Darkness */
/* Return the chosen graveyard card to the hand (also Magician of Faith). */
int EffectMaskOfDarknessResolve(struct ChainEntry *link);

/* 161 Tainted Wisdom */
/* Shuffle the player's deck. */
int EffectTaintedWisdomResolve(struct ChainEntry *link);

/* 170 Big Eye */
/* Rearrange the top 5 deck cards (a link duel sends the new order to the partner). */
int EffectBigEyeResolve(struct ChainEntry *link);

/* 198 Penguin Knight */
/* Return the graveyard to the deck and shuffle it. */
int EffectPenguinKnightResolve(struct ChainEntry *link);

/* 255 Dimensional Warrior */
/* Banish both battling monsters. */
int EffectDimensionalWarriorResolve(struct ChainEntry *link);

/* 261 The Little Swordsman of Aile */
/* Tribute the target for +700 ATK this turn. */
int EffectTheLittleSwordsmanOfAileResolve(struct ChainEntry *link);

/* 265 Princess of Tsurugi */
/* The opponent loses 500 LP per card in its spell/trap zones. */
int EffectPrincessOfTsurugiResolve(struct ChainEntry *link);

/* 303 Axe of Despair */
/* EffectEquipResolve; sent to the graveyard: tribute a monster to put it on top of the deck (human only). */
u16 EffectAxeOfDespairResolve(struct ChainEntry *link);

/* 310 Black Pendant */
/* EffectEquipResolve; sent to the graveyard: the opponent loses 500 LP. */
u16 EffectBlackPendantResolve(struct ChainEntry *link);

/* 312 Horn of Light */
/* EffectEquipResolve; sent to the graveyard: pay 500 LP to put it on top of the deck (also Malevolent
 * Nuzzler). */
u16 EffectHornOfLightResolve(struct ChainEntry *link);

/* 313 Horn of the Unicorn */
/* EffectEquipResolve; sent to the graveyard: it goes on top of the deck. */
u16 EffectHornOfTheUnicornResolve(struct ChainEntry *link);

/* 317 Elegant Egotist */
/* Can Special Summon, has a free zone, Harpie Lady (or key 1249) is on the field and there is a candidate. */
int EffectElegantEgotistPrepare(struct ChainEntry *card);
/* Special Summon a Harpie Lady from the hand or deck (the CPU prefers Harpie Lady Sisters). */
int EffectElegantEgotistResolve(struct ChainEntry *link);

/* 319 Stop Defense */
/* Opponent's targetable monster; accepted only in Defense Position. */
int EffectStopDefenseCheck(struct ChainEntry *card, u16 pos);
/* Pick the opponent's defense-position monster (no CPU branch). */
int EffectStopDefenseChainB(struct ChainEntry *link);
/* Switch the target to face-up Attack Position (a set Big Shield Gardna is only flipped up). */
int EffectStopDefenseResolve(struct ChainEntry *link);

/* 328 Dragon Capture Jar */
/* Switch every face-up attack-position Dragon to Defense Position. */
int EffectDragonCaptureJarResolve(struct ChainEntry *link);

/* 335 Dark Hole */
/* Either player has a monster. */
int EffectDarkHolePrepare(struct ChainEntry *card);
/* Destroy every monster, one per call, the opponent's first (also key 1425). */
int EffectDarkHoleResolve(struct ChainEntry *link);

/* 336 Raigeki */
/* The opponent has a monster. */
int EffectRaigekiPrepare(struct ChainEntry *card);
/* Destroy the opponent's monsters, one per call. */
int EffectRaigekiResolve(struct ChainEntry *link);

/* 347 Swords of Revealing Light */
/* Flip the opponent's face-down monsters face up, then run the Kotodama check (the three-turn attack lock
 * is set elsewhere). */
int EffectSwordsOfRevealingLightResolve(struct ChainEntry *link);

/* 348 Spellbinding Circle */
/* Link the target monster to the Circle, which locks it (also key 1244). */
int EffectSpellbindingCircleResolve(struct ChainEntry *link);

/* 349 Dark-Piercing Light */
/* The opponent has a face-down monster. */
int EffectDarkPiercingLightPrepare(struct ChainEntry *card);
/* Flip the opponent's face-down monsters face up, with their flip effects. */
int EffectDarkPiercingLightResolve(struct ChainEntry *link);

/* 401 Monster Eye */
/* 1000 LP for the cost and a Polymerization in the graveyard. */
int EffectMonsterEyePrepare(struct ChainEntry *card);
/* Return Polymerization from the graveyard to the hand. */
int EffectMonsterEyeResolve(struct ChainEntry *link);

/* 416 Blast Juggler */
/* On the field in the own Standby Phase, with two monsters its check accepts. */
int EffectBlastJugglerPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Face-up card with ATK 1000 or less, other than Blast Juggler. */
int EffectBlastJugglerCheck(struct ChainEntry *card, u16 pos);
/* Pick up to two monsters with ATK 1000 or less (no CPU branch). */
int EffectBlastJugglerChainB(struct ChainEntry *link);
/* Send Blast Juggler to the graveyard and destroy the targets that still qualify. */
int EffectBlastJugglerResolve(struct ChainEntry *link);

/* 419 Cyber-Stein */
/* Cyber-Stein: 5000 LP, a Fusion Deck card and a free zone; Gale Dogra: 3000 LP and a Fusion Deck card
 * (also Gale Dogra). */
int EffectCyberSteinPrepare(struct ChainEntry *card);
/* Pick a Fusion Deck monster: Cyber-Stein Special Summons it, Gale Dogra sends it to the graveyard (also
 * Gale Dogra). */
int EffectCyberSteinResolve(struct ChainEntry *link);

/* 424 Thunder Dragon */
/* From the hand, with a Thunder Dragon in the deck. */
int EffectThunderDragonPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Add up to two Thunder Dragons from the deck to the hand, one prompt each. */
int EffectThunderDragonResolve(struct ChainEntry *link);

/* 428 Goddess of Whim */
/* Coin toss; the result is kept as a card-effect link on its zone (1 = right call). */
int EffectGoddessOfWhimResolve(struct ChainEntry *link);

/* 461 The Immortal of Thunder */
/* Gain 3000 LP and arm the 5000 LP loss for when it leaves the field. */
int EffectTheImmortalOfThunderResolve(struct ChainEntry *link);

/* 468 Armed Ninja */
/* Spell/trap or field zone with a set card or a face-up Magic card (also De-Spell). */
int EffectMagicTargetCheck(struct ChainEntry *card, u16 pos);
/* Pick a set card or face-up Magic card to destroy (also De-Spell). */
int EffectMagicTargetChainB(struct ChainEntry *link);
/* Destroy the target unless it is a Trap; a set card is flipped and shown first, and a Trap flipped back
 * (also De-Spell). */
int EffectDestroyMagicTargetResolve(struct ChainEntry *link);

/* 489 Needle Ball */
/* Pay 2000 LP to deal 1000 damage (the CPU only when the human has 999 LP or less). */
int EffectNeedleBallResolve(struct ChainEntry *link);

/* 496 Yado Karu */
/* Human only: return hand cards to the bottom of the deck, one prompt each. */
int EffectYadoKaruResolve(struct ChainEntry *link);

/* 499 Dragon Seeker */
/* Face-up targetable Dragon. */
int EffectDragonSeekerCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-up Dragon to destroy (CPU: the first one). */
int EffectDragonSeekerChainB(struct ChainEntry *link);
/* Destroy the target if it is still a face-up Dragon. */
int EffectDragonSeekerResolve(struct ChainEntry *link);

/* 561 Needle Worm */
/* Send the top 5 cards of the opponent's deck to the graveyard. */
int EffectNeedleWormResolve(struct ChainEntry *link);

/* 579 Patrol Robo */
/* On the field, once per turn. */
int EffectPatrolRoboPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Opponent's face-down card (no targeting test). */
int EffectPatrolRoboCheck(struct ChainEntry *card, u16 pos);
/* Pick one of the opponent's face-down cards (no CPU branch). */
int EffectPatrolRoboChainB(struct ChainEntry *link);
/* Look at the target (flip, show, flip back) and use up the once-per-turn effect. */
int EffectPatrolRoboResolve(struct ChainEntry *link);

/* 582 Weather Report */
/* Destroy the opponent's face-up Swords of Revealing Light; if any, an extra Battle Phase. */
int EffectWeatherReportResolve(struct ChainEntry *link);

/* 585 Greenkappa */
/* At least two set Magic/Trap cards on the field. */
int EffectGreenkappaPrepare(struct ChainEntry *card);
/* Pick two set Magic/Trap cards to destroy; chainLink is passed to the prepare handler. */
int EffectGreenkappaChainB(struct ChainEntry *link, int prevLink);
/* Destroy the two targets that are still face down. */
int EffectGreenkappaResolve(struct ChainEntry *link);

/* 590 Morphing Jar */
/* Both players discard their hands (one card per call), then draw 5. */
int EffectMorphingJarResolve(struct ChainEntry *link);

/* 601 Penguin Soldier */
/* Pick up to two monsters to return to the hand (CPU: the human's strongest; its own Exodia pieces with
 * AI_FLAG_EXODIA). */
int EffectPenguinSoldierChainB(struct ChainEntry *link, int prevLink);
/* Return the targets to their owners' hands. */
int EffectPenguinSoldierResolve(struct ChainEntry *link);

/* 610 Hiro's Shadow Scout */
/* The opponent draws 3 cards; the Magic cards among them are shown and discarded. */
int EffectHirosShadowScoutResolve(struct ChainEntry *link);

/* 640 Invader of the Throne */
/* Swap control of Invader of the Throne and the target. */
int EffectInvaderOfTheThroneResolve(struct ChainEntry *link);

/* 650 Kunai with Chain */
/* On the field: answering an opponent's attack, or with a face-up monster to equip. */
int EffectKunaiWithChainPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Pick your face-up monster to equip (no CPU branch). */
int EffectKunaiWithChainChainB(struct ChainEntry *link);
/* Switch the attacker to defense (attack response), then equip the target or destroy itself. */
int EffectKunaiWithChainResolve(struct ChainEntry *link);

/* 660 Crush Card */
/* Tributes allowed (no key 1418) and a DARK monster with ATK 1000 or less to tribute. */
int EffectCrushCardPrepare(struct ChainEntry *card);
/* Activation cost: the human picks the DARK monster with ATK 1000 or less to tribute. */
int EffectCrushCardChainA(struct ChainEntry *link);
/* Destroy the opponent's monsters and hand monsters with ATK 1500 or more (set cards are shown and flipped
 * back otherwise). */
int EffectCrushCardResolve(struct ChainEntry *link);

/* 671 Harpie's Feather Duster */
/* The opponent has a Magic/Trap card. */
int EffectHarpiesFeatherDusterPrepare(struct ChainEntry *card);
/* Destroy every card in the opponent's spell/trap and field zones. */
int EffectHarpiesFeatherDusterResolve(struct ChainEntry *link);

/* 684 Acid Trap Hole */
/* Face-down defense-position monster, tributes allowed (no targeting test). */
int EffectAcidTrapHoleCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-down defense-position monster (no CPU branch). */
int EffectAcidTrapHoleChainB(struct ChainEntry *link);
/* Flip the target up; destroy it if its DEF is 2000 or less, else flip it back down. */
int EffectAcidTrapHoleResolve(struct ChainEntry *link);

/* 688 Reverse Trap */
/* Toggle the reversal of ATK/DEF modifiers. */
int EffectReverseTrapResolve(struct ChainEntry *link);

/* 689 Fake Trap */
/* On the field, answering an opponent's link that would destroy one of your Traps. */
int EffectFakeTrapPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Reveal the threatened Traps, let the other cards go, and negate chainedTo. */
int EffectFakeTrapResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* 729 Dark-Eyes Illusionist */
/* Link the target to the Illusionist so it cannot attack (also key 1332). */
int EffectDarkEyesIllusionistResolve(struct ChainEntry *link);

/* 730 Relinquished */
/* On the field, once per turn, nothing absorbed yet and a free spell/trap zone. */
int EffectRelinquishedPrepare(struct ChainEntry *card, int unused, u16 fromHand);
/* Absorb the target into a spell/trap zone and link it as ZONE_LINK_ABSORBED (also key 1334). */
int EffectRelinquishedResolve(struct ChainEntry *link);

/* 731 Jigen Bakudan */
/* Own Standby Phase, at least 2 monsters, the effect armed by the flip (zone effectUnused clear) and
 * tributes allowed. */
int EffectJigenBakudanPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Activation cost (Standby Phase activation): tribute Jigen Bakudan itself. */
int EffectJigenBakudanChainA(struct ChainEntry *link, u16 unused);
/* Standby Phase: tribute all your monsters and burn the opponent for half their ATK; on the flip: arm that
 * effect. */
int EffectJigenBakudanResolve(struct ChainEntry *link);

/* 742 Barrel Dragon */
/* Toss three coins; with two heads or more, destroy the target. Uses up the once-per-turn effect. */
int EffectBarrelDragonResolve(struct ChainEntry *link);

/* 762 Parasite Paracide */
/* Plant Parasite Paracide in the opponent's deck and shuffle it. */
int EffectParasiteParacideResolve(struct ChainEntry *link);

/* 845 Valkyrion the Magna Warrior */
/* On the field, 2 free monster zones, tributes allowed, and Alpha, Beta and Gamma The Magnet Warrior in
 * the graveyard. */
int EffectValkyrionTheMagnaWarriorPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Special Summon Alpha, Beta and Gamma The Magnet Warrior from the graveyard. */
int EffectValkyrionTheMagnaWarriorResolve(struct ChainEntry *link);

/* 939 Bell of Destruction */
/* Destroy the target and both players lose its ATK (key 1451: skip the next Draw Phase instead) (also key
 * 1451). */
int EffectBellOfDestructionResolve(struct ChainEntry *link);

/* 945 Magical Hats */
/* Hide the attacked monster among two deck cards set as hats and shuffle the three. */
int EffectMagicalHatsResolve(struct ChainEntry *link);

/* 950 Time Machine */
/* On the field, answering a monster destroyed in battle. */
int EffectTimeMachinePrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Return the monsters destroyed in battle to the field in their position, one side per call. */
u16 EffectTimeMachineResolve(struct ChainEntry *link);

/* 954 Toon World */
/* At least 1000 LP for the cost. */
int EffectToonWorldPrepare(struct ChainEntry *card, int chainLink);
/* Activation cost: pay 1000 LP. */
int EffectToonWorldChainA(struct ChainEntry *link, u16 unused);

/* 960 Negate Attack */
/* End the Battle Phase (also The Unhappy Maiden). */
int EffectEndBattlePhaseResolve(struct ChainEntry *link);

/* 962 7 Completed */
/* Pick the monster to equip, then ATK or DEF (enum SevenCompletedStat in targets[1]). */
int EffectSevenCompletedChainB(struct ChainEntry *link);

/* 964 Lightforce Sword */
/* Banish a random card of the opponent's hand face down. */
int EffectLightforceSwordResolve(struct ChainEntry *link);

/* 966 The Flute of Summoning Dragon */
/* A free zone, Special Summons allowed, Lord of D. face up and a Dragon in the hand. */
int EffectTheFluteOfSummoningDragonPrepare(struct ChainEntry *card);
/* Human: Special Summon up to two Dragons from the hand. */
int EffectTheFluteOfSummoningDragonResolve(struct ChainEntry *link);

/* 967 Shield & Sword */
/* Toggle the global swap of original ATK and DEF. */
int EffectShieldAndSwordResolve(struct ChainEntry *link);

/* 968 Graceful Charity */
/* Draw 3 cards, then discard 2. */
int EffectGracefulCharityResolve(struct ChainEntry *link);

/* 969 Chain Destruction */
/* A face-up monster with ATK 2000 or less was just summoned. */
int EffectChainDestructionPrepare(struct ChainEntry *card);
/* Send the summoner's hand and deck copies of the summoned monster to the graveyard. */
int EffectChainDestructionResolve(struct ChainEntry *link);

/* 970 Mesmeric Control */
/* The opponent cannot change battle positions. */
int EffectMesmericControlResolve(struct ChainEntry *link);

/* 990 Magic-Arm Shield */
/* Opponent's face-up targetable monster other than the attacker. */
int EffectMagicArmShieldCheck(struct ChainEntry *card, u16 pos);
/* Pick the opponent's monster that becomes the attack target (CPU: the first one). */
int EffectMagicArmShieldChainB(struct ChainEntry *link);
/* Take control of the target until the battle ends and make it the attack target. */
int EffectMagicArmShieldResolve(struct ChainEntry *link, int chainedTo);

/* 1001 Fissure */
/* Destroy the opponent's face-up monster with the lowest ATK. */
int EffectFissureResolve(struct ChainEntry *link);

/* 1003 Polymerization */
/* Special Summons allowed and materials for some Fusion Deck monster (also 1034 Polymerization). */
int EffectPolymerizationPrepare(struct ChainEntry *card);
/* Pick a Fusion monster and its materials, send them to the graveyard (key 1547: banish) and Special
 * Summon it (also 1034 Polymerization). */
int EffectPolymerizationResolve(struct ChainEntry *link, int chainedTo);

/* 1004 Remove Trap */
/* Face-up Trap in a spell/trap or field zone. */
int EffectRemoveTrapCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-up Trap (no CPU branch). */
int EffectRemoveTrapChainB(struct ChainEntry *link);
/* Destroy the target if it is still a face-up Trap. */
int EffectRemoveTrapResolve(struct ChainEntry *link);

/* 1005 Two-Pronged Attack */
/* You have 2 monsters and the opponent has 1. */
int EffectTwoProngedAttackPrepare(struct ChainEntry *card);
/* Pick two of your monsters and one of the opponent's (no CPU branch). */
int EffectTwoProngedAttackChainB(struct ChainEntry *link);
/* Destroy the three targets if all are still there. */
int EffectTwoProngedAttackResolve(struct ChainEntry *link);

/* 1008 Monster Reborn */
/* A free zone, Special Summons allowed, no Call of the Dark and a graveyard monster. The definition names
 * its two unused parameters action and flags (also key 1241). */
int EffectMonsterRebornPrepare(struct ChainEntry *card, int chainLink, int fromHand);
/* Special Summon the chosen graveyard monster to your field (also key 1241). */
int EffectMonsterRebornResolve(struct ChainEntry *link);

/* 1011 Gravedigger Ghoul */
/* No key 1511 on the opponent's field and a graveyard monster to banish (also key 1513). */
int EffectGravediggerGhoulPrepare(struct ChainEntry *card);
/* Banish graveyard cards one by one: up to 2 for Gravedigger Ghoul, 5 for Soul Release (also Soul
 * Release). */
int EffectBanishGraveyardCardsResolve(struct ChainEntry *link);

/* 1014 The Inexperienced Spy */
/* Look at one card of the opponent's hand (human pick). */
int EffectTheInexperiencedSpyResolve(struct ChainEntry *link);

/* 1017 Ultimate Offering */
/* On the field, 500 LP, Normal Summons allowed and a summonable hand monster. */
int EffectUltimateOfferingPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* An extra Normal Summon from the hand, with the tribute prompts for level 5 and up. */
int EffectUltimateOfferingResolve(struct ChainEntry *link);

/* 1018 Ancient Telescope */
/* Show the cards collected for Ancient Telescope in the card-list viewer. */
int EffectAncientTelescopeResolve(struct ChainEntry *link);

/* 1023 Tribute to The Doomed */
/* The hand holds a card to discard. */
int EffectTributeToTheDoomedPrepare(struct ChainEntry *card);

/* 1024 Soul Release */
/* No key 1511 on the opponent's field and a card in either graveyard. */
int EffectSoulReleasePrepare(struct ChainEntry *card);

/* 1025 The Cheerful Coffin */
/* The hand holds a Monster card. */
int EffectTheCheerfulCoffinPrepare(struct ChainEntry *card);
/* Discard up to 3 Monster cards from the hand, one prompt each. */
int EffectTheCheerfulCoffinResolve(struct ChainEntry *link, int chainedTo);

/* 1026 Call of the Dark */
/* Destroy every monster revived by Monster Reborn. */
int EffectCallOfTheDarkResolve(struct ChainEntry *link);

/* 1027 Change of Heart */
/* The opponent has a monster and you have a free monster zone. */
int EffectChangeOfHeartPrepare(struct ChainEntry *card);
/* Take control of the target for the turn (a set Big Shield Gardna is only flipped up). */
int EffectChangeOfHeartResolve(struct ChainEntry *link);

/* 1028 Solemn Judgment */
/* On the field, answering a summon or a Magic/Trap activation (not key 1539). */
int EffectSolemnJudgmentPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Negate and destroy the chained Magic/Trap, or destroy the summoned monster (loc0). */
int EffectSolemnJudgmentResolve(struct ChainEntry *link, u16 *chainedTo);

/* 1029 Magic Jammer */
/* On the field, answering a Magic card (not key 1539), with a card to discard. */
int EffectMagicJammerPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);

/* 1030 Seven Tools of the Bandit */
/* On the field, answering a Trap card, with 1000 LP. */
int EffectSevenToolsOfTheBanditPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);

/* 1031 Horn of Heaven */
/* On the field, answering a summon, with a monster to tribute. */
int EffectHornOfHeavenPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Activation cost: the human picks a monster to tribute (also Share the Pain). */
int EffectTributeChosenMonsterChainA(struct ChainEntry *link);
/* Negate the summon and destroy the summoned monster (loc0). */
int EffectHornOfHeavenResolve(struct ChainEntry *link);

/* 1032 Just Desserts */
/* The opponent loses 500 LP per monster it controls. */
int EffectJustDessertsResolve(struct ChainEntry *link);

/* 1039 Restructer Revolution */
/* The opponent has a hand card (a byte-identical copy of EffectOpponentHasHandPrepare). */
int EffectRestructerRevolutionPrepare(struct ChainEntry *card);

/* 1040 Fusion Sage */
/* A Polymerization in the deck. */
int EffectFusionSagePrepare(struct ChainEntry *card);
/* Add Polymerization from the deck to the hand and shuffle. */
int EffectFusionSageResolve(struct ChainEntry *link);

/* 1043 Block Attack */
/* Opponent's targetable monster in Attack Position. */
int EffectBlockAttackCheck(struct ChainEntry *card, u16 pos);
/* Pick the opponent's attack-position monster (no CPU branch). */
int EffectBlockAttackChainB(struct ChainEntry *link);
/* Switch the target to Defense Position. */
int EffectBlockAttackResolve(struct ChainEntry *link);

/* 1048 The Stern Mystic */
/* Look at every face-down card on the field (flip, show, flip back). */
int EffectTheSternMysticResolve(struct ChainEntry *link);

/* 1049 Wall of Illusion */
/* Return the attacking monster (loc0) to the hand. */
int EffectWallOfIllusionResolve(struct ChainEntry *link);

/* 1054 Last Will */
/* A monster went to your graveyard this turn, a free zone and a deck monster with ATK 1500 or less. */
int EffectLastWillPrepare(struct ChainEntry *card);
/* Special Summon a deck monster with ATK 1500 or less and shuffle. */
int EffectLastWillResolve(struct ChainEntry *link, int chainedTo);

/* 1055 Waboku */
/* On the field during the opponent's turn. */
int EffectWabokuPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* No battle damage to you this turn. */
int EffectWabokuResolve(struct ChainEntry *link);

/* 1059 Share the Pain */
/* Both players have a monster to tribute. */
int EffectShareThePainPrepare(struct ChainEntry *card);
/* The opponent tributes a monster too (the player's tribute is the chainA cost). */
int EffectShareThePainResolve(struct ChainEntry *link);

/* 1061 Heavy Storm */
/* Destroy every Magic/Trap card, one per call, the opponent's first. */
int EffectHeavyStormResolve(struct ChainEntry *link);

/* 1064 Curse of Fiend */
/* Standby Phase and either player has a monster. */
int EffectCurseOfFiendPrepare(struct ChainEntry *card);
/* Change the position of every monster, then lock position changes. */
int EffectCurseOfFiendResolve(struct ChainEntry *link);

/* 1065 Upstart Goblin */
/* Draw 1 card; the opponent gains 1000 LP. */
int EffectUpstartGoblinResolve(struct ChainEntry *link);

/* 1067 Final Destiny */
/* Five other hand cards to discard and a card on the field. */
int EffectFinalDestinyPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Destroy every card on the field. */
int EffectFinalDestinyResolve(struct ChainEntry *link);

/* 1068 Snatch Steal */
/* Opponent's face-up targetable monster that may change control (not key 1351) (also key 1514). */
int EffectSnatchStealCheck(struct ChainEntry *card, u16 pos);
/* Equip Snatch Steal to the target and take control of it. */
int EffectSnatchStealResolve(struct ChainEntry *link);

/* 1070 Confiscation */
/* Look at the opponent's hand and discard the card you pick. */
int EffectConfiscationResolve(struct ChainEntry *link);

/* 1071 Delinquent Duo */
/* The opponent discards one random card, then one of its choice. */
int EffectDelinquentDuoResolve(struct ChainEntry *link);

/* 1072 Darkness Approaches */
/* Two other hand cards to discard. */
int EffectDarknessApproachesPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Face-up targetable monster that is not a token. */
int EffectDarknessApproachesCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-up monster that is not a token (no CPU branch). */
int EffectDarknessApproachesChainB(struct ChainEntry *link);
/* Turn the target face down without changing its position. */
int EffectDarknessApproachesResolve(struct ChainEntry *link);

/* 1073 Fairy's Hand Mirror */
/* On the field, answering an opponent's Magic card that can be moved to another target. */
int EffectFairysHandMirrorPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Pick the new target for chainedTo's effect (key 1317 against an attack: a replacement monster) (also key
 * 1317). */
int EffectRedirectTargetChainB(struct ChainEntry *link, struct ChainEntry *prevLink);
/* Run the chained Magic card's resolve on the new target (gChain.proxyLink), then negate the original. */
int EffectFairysHandMirrorResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* 1074 Tailor of the Fickle */
/* Face-up equip card that could move to another monster. */
int EffectTailorOfTheFickleCheck(struct ChainEntry *card, u16 pos);
/* Pick the equip card, then its new monster (no CPU branch). */
int EffectTailorOfTheFickleChainB(struct ChainEntry *link);
/* Move the equip card (targets[0]) to the new monster (targets[1]). */
int EffectTailorOfTheFickleResolve(struct ChainEntry *link);

/* 1077 The Forceful Sentry */
/* Look at the opponent's hand and return the card you pick to its deck. */
int EffectTheForcefulSentryResolve(struct ChainEntry *link);

/* 1080 Giant Trunade */
/* Return every Magic/Trap card on the field to the hand. */
int EffectGiantTrunadeResolve(struct ChainEntry *link);

/* 1081 Painful Choice */
/* At least 5 cards in the deck. */
int EffectPainfulChoicePrepare(struct ChainEntry *card);
/* Pick 5 deck cards; the opponent chooses one for your hand, the rest go to the graveyard. */
int EffectPainfulChoiceResolve(struct ChainEntry *link, int chainedTo);

/* 1090 Time Seal */
/* The opponent skips its next Draw Phase. */
int EffectTimeSealResolve(struct ChainEntry *link);

/* 1091 Graverobber */
/* A Magic card in the opponent's graveyard. */
int EffectGraverobberPrepare(struct ChainEntry *card);
/* Take the chosen Magic card from the opponent's graveyard. */
int EffectGraverobberResolve(struct ChainEntry *link);

/* 1092 Gift of The Mystical Elf */
/* Gain 300 LP per face-up monster on the field. */
int EffectGiftOfTheMysticalElfResolve(struct ChainEntry *link);

/* 1094 Dust Tornado */
/* Any card in the opponent's spell/trap or field zones. */
int EffectDustTornadoCheck(struct ChainEntry *card, u16 pos);
/* Pick the opponent's Magic/Trap card (no CPU branch). */
int EffectDustTornadoChainB(struct ChainEntry *link);
/* Destroy the target, then offer to set a Magic/Trap card from the hand. */
int EffectDustTornadoResolve(struct ChainEntry *link);

/* 1095 Call Of The Haunted */
/* Special Summons allowed, a free zone and a graveyard monster. */
int EffectCallOfTheHauntedPrepare(struct ChainEntry *card);
/* Special Summon the chosen graveyard monster and link it to the card (Call of the Haunted: zone link,
 * Premature Burial: equip) (also Premature Burial). */
int EffectReviveFromGraveyardResolve(struct ChainEntry *link);

/* 1096 Solomon's Lawbook */
/* Skip your next Standby Phase. */
int EffectSolomonsLawbookResolve(struct ChainEntry *link);

/* 1097 Earthshaker */
/* The opponent has a face-up monster. */
int EffectEarthshakerPrepare(struct ChainEntry *card);
/* Declare two attributes; the opponent picks one (targets[0] = attribute). */
int EffectEarthshakerChainB(struct ChainEntry *link);
/* Destroy every face-up monster of the picked attribute. */
int EffectEarthshakerResolve(struct ChainEntry *link);

/* 1106 Cyber Jar */
/* Destroy all monsters; each player shows the top 5 deck cards, summons the level 4 or lower monsters and
 * adds the rest to the hand. */
int EffectCyberJarResolve(struct ChainEntry *link);

/* 1109 Senju of the Thousand Hands */
/* Add a Ritual Monster (Senju) or Ritual Magic card (Sonic Bird) from the deck to the hand (also Sonic
 * Bird). */
int EffectAddRitualCardToHandResolve(struct ChainEntry *link);

/* 1112 Karate Man */
/* Use up the once-per-turn effect, which doubles the base ATK in GetZoneCardStats (also key 1254). */
int EffectKarateManResolve(struct ChainEntry *link);

/* 1116 Spear Cretin */
/* Each player, turn player first, Special Summons a monster from its graveyard (The Shallow Grave: face
 * down) (also The Shallow Grave). */
int EffectEachPlayerReviveResolve(struct ChainEntry *link);

/* 1140 Numinous Healer */
/* Gain 1000 LP + 500 per Numinous Healer in your graveyard (key 1304: burn 700 + 300 per copy) (also key
 * 1304). */
int EffectNuminousHealerResolve(struct ChainEntry *link);

/* 1145 DNA Surgery */
/* Declare a monster type (targets[0] = type + 1) (also The Regulation of Tribe). */
int EffectDeclareTypeChainB(struct ChainEntry *link);
/* Store the declared type on the card's zone; DNA Surgery then destroys equips that no longer fit (also
 * The Regulation of Tribe). */
int EffectDeclareTypeResolve(struct ChainEntry *link);

/* 1147 Backup Soldier */
/* At least 5 monsters in your graveyard. */
int EffectBackupSoldierPrepare(struct ChainEntry *card);
/* Add up to three non-effect monsters with ATK 1500 or less from the graveyard to the hand. */
int EffectBackupSoldierResolve(struct ChainEntry *link);

/* 1149 Major Riot */
/* Return every monster to the hand; each player then sets as many level 4 or lower monsters. */
int EffectMajorRiotResolve(struct ChainEntry *link);

/* 1150 Ceasefire */
/* Either player has a face-down monster. */
int EffectCeasefirePrepare(struct ChainEntry *card);
/* Flip all face-down defense monsters up (no flip effects); the opponent loses 500 LP per effect monster. */
int EffectCeasefireResolve(struct ChainEntry *link);

/* 1157 Nobleman of Crossout */
/* Face-down monster (no targeting test). */
int EffectNoblemanOfCrossoutCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-down monster (no CPU branch). */
int EffectNoblemanOfCrossoutChainB(struct ChainEntry *link);
/* Flip the target without its effect and banish it; a flip-effect monster (Crossout) or a Trap
 * (Extermination) also loses its copies in both decks (also Nobleman of Extermination). */
int EffectNoblemanResolve(struct ChainEntry *link);

/* 1158 Nobleman of Extermination */
/* Set card in a spell/trap zone (5-9). */
int EffectNoblemanOfExterminationCheck(struct ChainEntry *card, u16 pos);
/* Pick a set Magic/Trap card (no CPU branch). */
int EffectNoblemanOfExterminationChainB(struct ChainEntry *link);

/* 1159 The Shallow Grave */
/* Special Summons allowed, a free zone and a graveyard monster. */
int EffectTheShallowGravePrepare(struct ChainEntry *card);

/* 1160 Premature Burial */
/* 800 LP for the cost, then EffectCallOfTheHauntedPrepare. */
int EffectPrematureBurialPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);

/* 1161 Inspection */
/* Opponent's Standby Phase: show a random card of its hand. */
int EffectInspectionResolve(struct ChainEntry *link);

/* 1162 Prohibition */
/* Declare any card through the Deck Edit card list (leaves and rebuilds the duel screen). */
int EffectProhibitionChainB(struct ChainEntry *link);
/* Show the declared card and add it to the prohibited cards. */
int EffectProhibitionResolve(struct ChainEntry *link);

/* 1163 Morphing Jar #2 */
/* Return all monsters to the decks, then each side digs for as many monsters, setting level 4 or lower
 * ones. */
int EffectMorphingJar2Resolve(struct ChainEntry *link);

/* 1177 Windstorm of Etaqua */
/* Change the position of the opponent's face-up monsters (Dragons under Dragon Capture Jar stay). */
int EffectWindstormOfEtaquaResolve(struct ChainEntry *link);

/* 1179 Sebek's Blessing */
/* Your monster dealt battle damage. */
int EffectSebeksBlessingPrepare(struct ChainEntry *card);
/* Gain LP equal to the battle damage (loc0). */
int EffectSebeksBlessingResolve(struct ChainEntry *link);

/* 1181 Riryoku */
/* At least two face-up monsters on the field. */
int EffectRiryokuPrepare(void);
/* Pick the monster to halve, then the one to strengthen (no CPU branch). */
int EffectRiryokuChainB(struct ChainEntry *link);
/* Halve the first target's ATK and add that amount to the second. */
int EffectRiryokuResolve(struct ChainEntry *link);

/* 1183 Seal of the Ancients */
/* 1000 LP for the cost and an opponent's face-down card. */
int EffectSealOfTheAncientsPrepare(struct ChainEntry *card);
/* Look at every face-down card of the opponent. */
int EffectSealOfTheAncientsResolve(struct ChainEntry *link);

/* 1203 Graceful Dice */
/* You have a face-up monster. */
int EffectGracefulDicePrepare(struct ChainEntry *card);
/* Roll a die and link the result to every face-up monster of one side: Graceful Dice yours, Skull Dice the
 * opponent's (also Skull Dice). */
int EffectDiceResolve(struct ChainEntry *link);

/* 1204 Skull Dice */
/* The opponent has a face-up monster. */
int EffectSkullDicePrepare(struct ChainEntry *card);

/* 1205 Exchange */
/* Both players have another hand card. */
int EffectExchangePrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Each player takes one card of the other's hand. */
int EffectExchangeResolve(struct ChainEntry *link);

/* 1211 (key, no EDS card) */
/* The opponent has a monster and you have an active Dark Magician (hypothesis: OCG Thousand Knives). */
int EffectDarkMagicianOnFieldPrepare(struct ChainEntry *card);

/* 1212 (key, no EDS card) */
/* A free zone, Special Summons allowed, Normal Summon unused and a Dark Magician in the deck (hypothesis:
 * OCG Dark Magic Curtain). */
int EffectDarkMagicianInDeckPrepare(struct ChainEntry *card);
/* Activation cost: pay half your LP (rounded up). */
int EffectPayHalfLifePointsChainA(struct ChainEntry *link);
/* Special Summon Dark Magician from the deck, then lock summons for the turn. */
int EffectSummonDarkMagicianFromDeckResolve(struct ChainEntry *link);

/* 1213 (key, no EDS card) */
/* Both players have a monster (hypothesis: OCG Mystic Box). */
int EffectBothPlayersHaveMonsterPrepare(struct ChainEntry *card);
/* Pick the opponent's monster to destroy, then your monster to give away. */
int EffectDestroyAndGiveMonsterChainB(struct ChainEntry *link);
/* Destroy the opponent's target and move your monster into its zone (hypothesis: OCG Mystic Box). */
int EffectDestroyAndGiveMonsterResolve(struct ChainEntry *link);

/* 1220 (key, no EDS card) */
/* Main Phase 1, tributes allowed, and a use for the tribute: a face-up Catapult Turtle / Little Swordsman
 * of Aile / Cannon Soldier or a tribute-summonable hand monster (hypothesis: OCG Soul Exchange). */
int EffectCanTributeOpponentMonsterPrepare(struct ChainEntry *card);
/* Activation cost: give up this turn's Battle Phase (also key 1248). */
int EffectSkipBattlePhaseChainA(struct ChainEntry *link);
/* Pick the opponent's monster to use as a tribute. */
int EffectTributeOpponentMonsterChainB(struct ChainEntry *link);
/* Use the target as a tribute: for a Tribute Summon or for the effect of Catapult Turtle, The Little
 * Swordsman of Aile or Cannon Soldier. ROM bug: the summon path tributes the wrong zone. */
int EffectTributeOpponentMonsterResolve(struct ChainEntry *link, int chainedTo);

/* 1221 (key, no EDS card) */
/* Key 1221: both players discard their hands and draw as many (hypothesis: OCG Card Destruction). */
int EffectDiscardHandsAndRedrawResolve(struct ChainEntry *link);

/* 1232 (key, no EDS card) */
/* Tributes allowed and a face-up Kuriboh (hypothesis: OCG Multiply). */
int EffectFaceUpKuribohPrepare(struct ChainEntry *card);
/* Activation cost: pick a face-up Kuriboh to tribute (the prompt shows a raw %s, ROM bug). */
int EffectTributeKuribohChainA(struct ChainEntry *link);
/* Summon a token into every free monster zone. */
int EffectSummonTokensInFreeZonesResolve(struct ChainEntry *link);

/* 1240 (key, no EDS card) */
/* Key 1240: add Gazelle the King of Mythical Beasts from the deck to the hand (hypothesis: OCG Berfomet). */
int EffectAddGazelleFromDeckResolve(struct ChainEntry *link);

/* 1242 (key, no EDS card) */
/* Key 1242, sent to the graveyard: offer to equip it to a face-up monster. */
int EffectEquipSelfFromGraveyardResolve(struct ChainEntry *link);

/* 1245 (key, no EDS card) */
/* No summon this turn and 4 free monster zones (hypothesis: OCG Scapegoat). */
int EffectNoSummonFourFreeZonesPrepare(struct ChainEntry *card);
/* Summon four defense-position tokens. */
int EffectSummonFourTokensResolve(struct ChainEntry *link);

/* 1246 (key, no EDS card) */
/* Key 1246, on summon: link an opponent monster's base ATK/DEF to this monster. */
int EffectGainOpponentMonsterStatsResolve(struct ChainEntry *link);

/* 1247 (key, no EDS card) */
/* Key 1247: run the opponent's Magic card for yourself (gChain.proxyLink), then negate the original. */
int EffectReflectPlayerMagicResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* 1248 (key, no EDS card) */
/* Key 1248: Main Phase 1 only. */
int EffectMainPhase1Prepare(void);
/* Your face-up Summoned Skull or Thunder monster. */
int EffectOwnSkullOrThunderCheck(struct ChainEntry *card, u16 pos);
/* Pick your face-up Summoned Skull or Thunder monster. */
int EffectOwnSkullOrThunderChainB(struct ChainEntry *link);
/* Destroy the opponent's face-up monsters whose DEF is below the target's ATK. */
int EffectDestroyWeakerDefenseResolve(struct ChainEntry *link);

/* 1255 (key, no EDS card) */
/* Key 1255: pick the opponent's face-up Machine. */
int EffectTakeControlOfMachineChainB(struct ChainEntry *link);
/* Take control of the target Machine until the end of the turn. */
int EffectTakeControlOfMachineResolve(struct ChainEntry *link);

/* 1256 (key, no EDS card) */
/* Key 1256, flip: each player discards one card. */
int EffectEachPlayerDiscardsResolve(struct ChainEntry *link, int unused);

/* 1257 (key, no EDS card) */
/* Key 1257, sent to the graveyard: equip key 1258 from the deck to a monster and switch that monster's
 * control. */
int EffectEquipFromDeckAndSwitchControlResolve(struct ChainEntry *link);

/* 1258 (key, no EDS card) */
/* Key 1258: return the card to the deck and shuffle. */
int EffectReturnSelfToDeckResolve(struct ChainEntry *link);

/* 1302 (key, no EDS card) */
/* Key 1302: the opponent has 3000 LP or less (continuous: 500 damage per turn). */
int EffectOpponentLifeAtMost3000Prepare(struct ChainEntry *card);

/* 1303 (key, no EDS card) */
/* Key 1303: the opponent holds 6 cards or more and you 2 or fewer. */
int EffectHandDisadvantagePrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Coin toss: a right call draws up to 5 hand cards, a wrong one skips the next turn (hypothesis: OCG
 * Gamble). */
int EffectCoinTossDrawToFiveResolve(struct ChainEntry *link);

/* 1311 (key, no EDS card) */
/* Destroy both field zones' cards (hypothesis: OCG Burning Land). */
int EffectDestroyFieldMagicsResolve(struct ChainEntry *link);

/* 1312 (key, no EDS card) */
/* Start of Main Phase 1: no Magic/Trap activated and no Normal Summon yet (hypothesis: OCG Cold Wave). */
int EffectStartOfMainPhase1Prepare(struct ChainEntry *card);
/* Neither player may play Magic/Trap cards for 2 turns. */
int EffectLockMagicTrapResolve(struct ChainEntry *link);

/* 1314 (key, no EDS card) */
/* Link a boost to each of your face-up Machines (hypothesis: OCG Limiter Removal). */
int EffectDoubleMachineAtkResolve(struct ChainEntry *link);

/* 1316 (key, no EDS card) */
/* You have a monster and another hand card. */
int EffectOwnMonsterAndHandCardPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Your monster, face up or down, that the card may target (also key 1319). */
int EffectTargetableOwnMonsterCheck(struct ChainEntry *card, u16 pos);
/* Return the target and your whole hand to the deck, shuffle and draw as many cards as the hand had. */
int EffectReturnMonsterAndRedrawResolve(struct ChainEntry *link);

/* 1317 (key, no EDS card) */
/* Answering an opponent's attack with 2 monsters, or an opponent's link whose target can move (hypothesis:
 * OCG Shift). */
int EffectCanRedirectTargetPrepare(struct ChainEntry *card, struct ChainEntry *chainLink);
/* Redirect the attack to targets[0], or move chainedTo's effect to the new target through
 * EffectFairysHandMirrorResolve. */
u16 EffectRedirectTargetResolve(struct ChainEntry *link, int chainedTo);

/* 1318 (key, no EDS card) */
/* Your monster whose level + 1 matches a summonable Insect in the deck. */
int EffectTributeForInsectCheck(struct ChainEntry *card, u16 pos);
/* Tribute your monster and keep its level + 1 in targets[0]. ROM bug: passes the packed position as the
 * zone, so only zone 0 of player 0 works. */
int EffectTributeForLevelChainB(struct ChainEntry *link);
/* Special Summon a deck Insect of level targets[0] (hypothesis: OCG Insect Imitation). */
int EffectSummonInsectFromDeckResolve(struct ChainEntry *link);

/* 1319 (key, no EDS card) */
/* Banish the target until the End Phase (hypothesis: OCG Interdimensional Matter Transporter). */
int EffectBanishOwnMonsterUntilEndPhaseResolve(struct ChainEntry *link);

/* 1320 (key, no EDS card) */
/* At least 2 free monster zones on the field (hypothesis: OCG Ground Collapse). */
int EffectTwoFreeMonsterZonesPrepare(void);
/* Pick two empty monster zones (no CPU branch). */
int EffectBlockMonsterZonesChainB(struct ChainEntry *link);
/* Link the two empty zones to the card so that nothing can be placed there. */
int EffectBlockTwoMonsterZonesResolve(struct ChainEntry *link);

/* 1321 (key, no EDS card) */
/* On the field, answering a Magic card (not key 1539) (hypothesis: OCG Magic Drain). */
int EffectChainedToMagicPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Negate chainedTo unless the opponent discards a Magic card. */
int EffectNegateMagicUnlessDiscardResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* 1324 (key, no EDS card) */
/* On the field, with a Magic card in the hand. */
int EffectMagicCardInHandPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Activation cost: the human discards a Magic card from the hand. */
int EffectDiscardMagicCardChainA(struct ChainEntry *link);
/* The opponent loses 500 LP. */
int EffectDiscardMagicDamageResolve(struct ChainEntry *link);

/* 1325 (key, no EDS card) */
/* On the field, answering the opponent's Set of a monster that is still face down. */
int EffectOpponentSetMonsterPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Turn the Set monster (loc0) face-up in Attack Position without its flip effect. */
int EffectFlipSetMonsterToAttackResolve(struct ChainEntry *link);

/* 1328 (key, no EDS card) */
/* Offer to put Parasite Paracide from the deck on top of it (the CPU always does). */
int EffectPlaceParasiteParacideOnDeckResolve(struct ChainEntry *link);

/* 1330 (key, no EDS card) */
/* Change the target's battle position. */
int EffectChangeTargetPositionResolve(struct ChainEntry *link);

/* 1337 (key, no EDS card) */
/* Pick the opponent's face-down defense monster (no CPU branch). */
int EffectFlipOpponentSetMonsterChainB(struct ChainEntry *link);
/* Flip the target up; destroy it if it is an effect monster, else flip it back. */
int EffectDestroySetEffectMonsterResolve(struct ChainEntry *link);

/* 1338 (key, no EDS card) */
/* Destroy the opponent's face-up level 4 monsters. */
int EffectDestroyOpponentLevel4Resolve(struct ChainEntry *link);

/* 1405 (key, no EDS card) */
/* Offer to pay 1000 LP to Special Summon the card in the next Standby Phase (hypothesis: OCG Revival Jam). */
int EffectPayToReviveNextStandbyResolve(struct ChainEntry *link);

/* 1414 (key, no EDS card) */
/* A free zone, a summonable hand monster and two other hand cards. */
int EffectHandMonsterAndTwoCardsPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Pick a hand monster and two Magic/Trap cards; a roulette picks one, the others are discarded, and the
 * monster is summoned if it won. */
int EffectHandRouletteSummonResolve(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* 1417 (key, no EDS card) */
/* Face-up Magic card in a spell/trap or field zone. */
int EffectFaceUpMagicCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-up Magic card other than this one (no CPU branch). */
int EffectFaceUpMagicTargetChainB(struct ChainEntry *link);
/* Link the target Magic card to this card (hypothesis: OCG Mask of Dispel; the damage is dealt in the
 * Standby Phase). */
int EffectCurseFaceUpMagicResolve(struct ChainEntry *link);

/* 1421 (key, no EDS card) */
/* A hand monster and a graveyard monster destroyed in battle this turn. */
int EffectHandMonsterAndGraveTargetPrepare(struct ChainEntry *card);
/* Post the discard prompt (1 card); returns 1 at once. */
int EffectDiscardHandCardChainB(struct ChainEntry *link);

/* 1423 (key, no EDS card) */
/* Umi is the face-up field Magic (hypothesis: OCG Tornado Wall). */
int EffectUmiOnFieldPrepare(void);

/* 1424 (key, no EDS card) */
/* Coin toss: a right call makes the attacker's ATK 0 (hypothesis: OCG Fairy Box). */
int EffectCoinTossZeroAttackerResolve(struct ChainEntry *link);

/* 1425 (key, no EDS card) */
/* On the field, answering a summon, with a monster on the field. */
int EffectDestroyAllOnSummonPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);

/* 1428 (key, no EDS card) */
/* Answering an attack on your monster, with a face-up key 1405 to take the attack. */
int EffectRedirectAttackPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Pick your face-up key 1405 (no targeting test). */
int EffectRedirectAttackChainB(struct ChainEntry *link);
/* Make the face-up key 1405 the attack target (hypothesis: OCG Jam Defender). */
int EffectRedirectAttackResolve(struct ChainEntry *link);

/* 1432 (key, no EDS card) */
/* At least two monsters to tribute (also key 1442). */
int EffectTributeTwoMonstersPrepare(struct ChainEntry *card);
/* Pick two of your monsters to tribute (also key 1442). */
int EffectTributeTwoMonstersChainB(struct ChainEntry *link);
/* Tribute both targets: key 1432 burns the opponent for 1200, key 1442 gains 1000 LP (also key 1442). */
int EffectTributeTwoMonstersResolve(struct ChainEntry *link);

/* 1435 (key, no EDS card) */
/* Banish your top 3 deck cards and burn the opponent for 800. */
int EffectBanishDeckTopDamageResolve(struct ChainEntry *link);

/* 1436 (key, no EDS card) */
/* At least two hand cards. */
int EffectHasTwoHandCardsPrepare(struct ChainEntry *card);
/* Activation cost: banish 2 random hand cards. */
int EffectBanishRandomHandCardsChainA(struct ChainEntry *link);
/* The opponent loses 800 LP (hypothesis: OCG Fire Sorcerer). */
int EffectDamageOpponent800Resolve(struct ChainEntry *link);

/* 1439 (key, no EDS card) */
/* Tributes allowed and a graveyard Magic card destroyed by the opponent. */
int EffectTributeRecoverGraveMagicPrepare(struct ChainEntry *card);
/* Put the chosen graveyard Magic card on the bottom of the deck. */
int EffectTributeRecoverGraveMagicResolve(struct ChainEntry *link);

/* 1448 (key, no EDS card) */
/* Declare an attribute (targets[0]), then pick the monster to equip. */
int EffectDeclareAttributeEquipChainB(struct ChainEntry *link);
/* Store the declared attribute on the card's zone and equip it to targets[1]. */
int EffectEquipSetAttributeResolve(struct ChainEntry *link);

/* 1510 (key, no EDS card) */
/* On the field, with a hand card. */
int EffectHasHandCardPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Face-up monster that was Special Summoned. */
int EffectSpecialSummonedMonsterCheck(struct ChainEntry *card, u16 pos);
/* Activation cost: discard a random hand card. */
int EffectDiscardRandomHandCardChainA(struct ChainEntry *link);
/* Destroy every face-up Special Summoned monster (one per call), then forbid Special Summons this turn. */
int EffectDestroySpecialSummonedResolve(struct ChainEntry *link);

/* 1511 (key, no EDS card) */
/* Banish up to 2 monsters from the opponent's graveyard (hypothesis: OCG Kycoo the Ghost Destroyer). */
int EffectBanishOpponentGraveMonstersResolve(struct ChainEntry *link);

/* 1512 (key, no EDS card) */
/* Human: tribute another monster to Special Summon a Fusion Deck monster, destroyed at the end of the
 * turn. */
int EffectTributeToSummonFusionResolve(struct ChainEntry *link);

/* 1513 (key, no EDS card) */
/* Banish up to 3 graveyard monsters for +300 ATK each, once per turn (hypothesis: OCG Bazoo the
 * Soul-Eater). */
int EffectBanishGraveForAtkResolve(struct ChainEntry *link);

/* 1514 (key, no EDS card) */
/* From the graveyard, equip itself to the target and take control of it (hypothesis: OCG Dark Necrofear). */
int EffectEquipFromGraveTakeControlResolve(struct ChainEntry *link);

/* 1517 (key, no EDS card) */
/* Pick an opponent's face-up monster, change its position and lock it (also key 1519). */
int EffectChangeOpponentPositionResolve(struct ChainEntry *link);

/* 1520 (key, no EDS card) */
/* The opponent may Special Summon a monster from its graveyard. */
int EffectOpponentGraveSummonResolve(struct ChainEntry *link);

/* 1521 (key, no EDS card) */
/* At least two cards in the spell/trap and field zones. */
int EffectTwoSpellTrapsOnFieldPrepare(void);
/* Pick two Magic/Trap cards (no CPU branch). */
int EffectReturnTwoSpellTrapsChainB(struct ChainEntry *link, int prevLink);
/* Return both targets to their owners' hands. */
int EffectReturnTwoCardsToHandResolve(struct ChainEntry *link);

/* 1524 (key, no EDS card) */
/* Add two graveyard monsters that were Fusion materials to the hand. */
int EffectReturnFusionMaterialsToHandResolve(struct ChainEntry *link);

/* 1525 (key, no EDS card) */
/* On the field, tributes allowed, answering a Magic card (not key 1539). */
int EffectTributeNegateMagicPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Tribute this monster to negate and destroy the chained Magic card. */
int EffectTributeNegateMagicResolve(struct ChainEntry *link, u16 *chainedTo);

/* 1527 (key, no EDS card) */
/* On the field, with an equip card that could move to another face-up monster. */
int EffectMoveEquipPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Move every equip card that may go to the target; destroy the others. */
int EffectMoveAllEquipsResolve(struct ChainEntry *link);

/* 1529 (key, no EDS card) */
/* Answering an opponent's attack, the opponent has 2 face-up monsters. */
int EffectSwitchAttackerPrepare(struct ChainEntry *card);
/* Pick the opponent's monster that attacks instead (no CPU branch). */
int EffectSwitchAttackerChainB(struct ChainEntry *link);
/* The target attacks in place of the current attacker (switched to attack position). */
int EffectSwitchAttackerResolve(struct ChainEntry *link);

/* 1532 (key, no EDS card) */
/* Graveyard monsters and a face-up monster whose level is at most their number. */
int EffectBanishGraveToDestroyPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand);
/* Pick a face-up monster, then banish as many graveyard monsters as its level. */
int EffectBanishGraveToDestroyChainB(struct ChainEntry *link);
/* Destroy the target if its level still equals the number banished. */
int EffectBanishGraveToDestroyResolve(struct ChainEntry *link);

/* 1535 (key, no EDS card) */
/* On the field, after an opponent's monster attacked your defense-position monster and its controller took
 * battle damage. */
int EffectRepelledAttackerPrepare(struct ChainEntry *card, int chainLink, u16 fromHand);
/* Destroy that attacker. */
int EffectDestroyRepelledAttackerResolve(struct ChainEntry *link);

/* 1539 (key, no EDS card) */
/* Any card in the opponent's spell/trap or field zones. */
int EffectOpponentSpellTrapCheck(struct ChainEntry *card, u16 pos);
/* Pick the opponent's Magic/Trap card (no CPU branch). */
int EffectReturnOpponentSpellTrapChainB(struct ChainEntry *link);
/* Return the target to the opponent's hand. */
int EffectReturnOpponentSpellTrapResolve(struct ChainEntry *link);

/* 1545 (key, no EDS card) */
/* Set card in a spell/trap zone (5-9). */
int EffectFaceDownSpellTrapCheck(struct ChainEntry *card, u16 pos);
/* Pick a set card (no CPU branch). */
int EffectForceActivateTrapChainB(struct ChainEntry *link);
/* Reveal the target; force a Trap to activate (destroy it if it cannot), then shuffle this card into the
 * deck (hypothesis: OCG Bait Doll). */
int EffectForceActivateTrapResolve(struct ChainEntry *link);

/* 1546 (key, no EDS card) */
/* Face-up targetable Fusion monster. */
int EffectFaceUpFusionMonsterCheck(struct ChainEntry *card, u16 pos);
/* Pick a face-up Fusion monster (no CPU branch). */
int EffectReturnFusionToDeckChainB(struct ChainEntry *link);
/* Return the Fusion monster to the Fusion Deck, then optionally Special Summon its materials from the
 * graveyard (hypothesis: OCG De-Fusion). */
int EffectSplitFusionResolve(struct ChainEntry *link);

/* 1549 (key, no EDS card) */
/* No face-up Banisher of the Light and at least 5 banished monsters. */
int EffectReturnBanishedToGravePrepare(struct ChainEntry *card);
/* Return up to 3 banished monsters to the graveyard. */
int EffectReturnBanishedToGraveResolve(struct ChainEntry *link, int chainedTo);

/* 1551 (key, no EDS card) */
/* For the rest of the turn, your "banish graveyard monsters" summon costs are paid with monsters on the
 * field. */
int EffectBanishCostFromFieldResolve(struct ChainEntry *link);

#endif /* GUARD_EFFECT_HANDLERS_H */
