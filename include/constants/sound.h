#ifndef GUARD_CONSTANTS_SOUND_H
#define GUARD_CONSTANTS_SOUND_H

/*
 * Sound ids passed to the game-side sound API (include/sound.h).
 *
 * Sound effects: ids for PlaySE / SoundRequestSE, an index into gSeTable (struct SoundSeDef, 48 entries). The ids
 * the C code passes are named here; the meanings come from where each id is played (wiki game/sound-engine.md,
 * functions/sound-api.md and the callers named below) and are not checked by ear.
 *
 * Songs: ids for PlayBGM / PlayBGMNoTrack / PlayJingle (58 entries in gSongTable). Only the ids the code passes
 * as literals are named, after where they are played. The duel music comes from tables (gOpponentDuelBGM,
 * gOpponentDialogueBGM, gDuelResultBanners[].jingle) and has no names here.
 */

/* Sound effect ids (PlaySE). Values are the numbers the code passes. */
enum SoundEffect {
    SE_CURSOR = 0,              /* cursor moves */
    SE_CONFIRM = 1,             /* "decide": also played for B on some screens (opponent select) */
    SE_CANCEL = 2,              /* "close": also played for A when leaving the Calendar */
    SE_ERROR = 3,               /* buzzer: action refused */
    SE_CARD_FLIP = 6,           /* a card turns over: booster pack cards, the duel flip and position-change
                                 * animations; also L/R in Big Eye's top-of-deck reorder */
    SE_CARD_MOVE = 7,           /* a card slides to another place (duel move and swap animations); also every
                                 * loop of the deck shuffle (DuelCmd_ShuffleDeck) */
    SE_DESTROY = 8,             /* a card is destroyed (low confidence) */
    SE_BATTLE_HIT = 9,          /* battle scene: the values have been shown, the hit side starts to shake */
    SE_DUEL_START_BANNER = 11,  /* the Start Duel banner (DuelCmd_StartDuelBanner) */
    SE_LP_GAIN = 12,            /* life points counting up (DuelCmd_ChangeLifePoints) */
    SE_LP_LOSS = 13,            /* life points counting down */
    SE_CARD_DRAW = 14,          /* a card is drawn from the deck to the hand */
    SE_END_TURN_HAND = 15,      /* the end-of-turn glove appears (DuelCmd_ShowEndTurnHand) */
    SE_ZONE_EFFECT = 16,        /* with a zone sprite effect (DuelAnim_PlayZoneEffect): the negate animation,
                                 * the tribute whirlwind, the smoke puff (Magical Hats, a monster set face down) */
    SE_BANISH = 17,             /* a card is removed from play (low confidence) */
    SE_EXODIA_PIECE = 19,       /* Exodia win scene: a piece appears; the five assembled pieces flash */
    SE_EXODIA_LAUNCH = 20,      /* Exodia win scene: the pieces fly off */
    SE_EXODIA_GATHERED = 21,    /* Exodia win scene: the pieces meet at full white */
    SE_BANNER_SLIDE = 22,       /* a banner slides in: "Just a moment" (also when it stops), the Chain banner */
    SE_BATTLE_PHASE_BANNER = 29,    /* the Battle Phase banner, once its two halves meet */
    SE_COIN_TOSS = 30,          /* a coin is thrown (coin toss scene, CoinToss_Update) */
    SE_DIE_ROLL = 31,           /* die roll scene: the throw, each landing and every third rolling frame */
    SE_PASSWORD_CURSOR = 37,    /* Password screen: digit cursor moves */
    SE_PASSWORD_PRESS = 38,     /* Password screen: key pressed */
    SE_PASSWORD_ROLL = 39,      /* Password screen: digit rolls */
    SE_PASSWORD_REJECT = 40,    /* Password screen: wrong or used password */
    SE_PASSWORD_GET_CARD = 41,  /* Password screen: card received */
    SE_DUEL_LOGO_SWING = 42,    /* turn-order screen: the DUEL logo starts to swing */
    SE_CARD_SHOWN = 44,         /* a presented card has opened (the card presentations, DuelCmd_ShowCardAssemble) */
    SE_DESTINY_BOARD_HAND = 45, /* Destiny Board win scene: step 1 of the ghost-hand animation (hypothesis: the
                                 * hand appears) */
    SE_DESTINY_BOARD_LETTERS = 46,  /* Destiny Board win scene: step 11, after the hand sheet was replaced by the
                                     * letters (hypothesis: the letters appear) */
    SE_DESTINY_BOARD_LETTER_LAUNCH = 47,    /* Destiny Board win scene: one F-I-N-A-L letter flies off */
};

/* Song ids (PlayBGM) the code passes as literals, named after where they are played. */
enum Song {
    SONG_TITLE = 0,                     /* title screen (PlayBGMNoTrack); stops at the end */
    SONG_NEW_GAME = 1,                  /* the New Game introduction dialogue (title menu) */
    SONG_MAIN_MENU = 3,                 /* main menu (PlayBGMNoTrack) */
    SONG_DESTINY_BOARD_WIN = 20,        /* Destiny Board win scene; stops at the end */
    SONG_MATCH_CONTINUES = 21,          /* Campaign: result dialogue of a match duel when the match goes on */
    SONG_DUEL_WON = 24,                 /* Campaign: result dialogue after a won duel or match */
    SONG_DUEL_LOST = 25,                /* Campaign: result dialogue after a lost or drawn duel or match */
    SONG_RARE_HUNTER_APPEARS = 26,      /* Campaign day start: the Rare Hunter's announcement (text 900) */
    SONG_RARE_HUNTER_CHALLENGE = 27,    /* Campaign day start: after that dialogue, when his opponent is picked */
    SONG_RARE_HUNTER_WON = 28,          /* Campaign: result dialogue after losing to the Rare Hunter */
    SONG_MAGAZINE_DAY = 31,             /* Campaign: Weekly Jump / V Jump release-day dialogue */
    SONG_SPECIAL_DUEL = 48,             /* Campaign: holiday special duels and the Duel Ceremony */
    SONG_CHAMPIONSHIP = 49,             /* Campaign: Championship rounds 1-3 */
    SONG_CHAMPIONSHIP_FINAL = 50,       /* Campaign: Championship final */
    SONG_GRANDPA_CUP = 51,              /* Campaign: Grandpa Cup qualifier and final */
};

#endif /* GUARD_CONSTANTS_SOUND_H */
