#ifndef GUARD_CONSTANTS_SOUND_H
#define GUARD_CONSTANTS_SOUND_H

/*
 * Sound ids passed to the game-side sound API (include/sound.h).
 *
 * Sound effects: ids for PlaySE / SoundRequestSE, an index into gSeTable (struct SoundSeDef). Only the ids
 * the menu and duel code use are named; the meanings come from where each id is played (wiki
 * game/sound-engine.md, functions/sound-api.md) and are not checked by ear.
 *
 * Songs (PlayBGM / SoundRequestBGM ids, 58 entries in gSongTable) have no names yet.
 */

/* Sound effect ids (PlaySE). Values are the numbers the code passes today. */
enum SoundEffect {
    SE_CURSOR = 0,              /* cursor moves */
    SE_CONFIRM = 1,             /* "decide": also played for B on some screens (opponent select) */
    SE_CANCEL = 2,              /* "close": also played for A when leaving the Calendar */
    SE_ERROR = 3,               /* buzzer: action refused */
    SE_CARD_FLIP = 6,           /* a booster pack card turns over */
    SE_DESTROY = 8,             /* a card is destroyed (low confidence) */
    SE_BANISH = 17,             /* a card is removed from play (low confidence) */
    SE_PASSWORD_CURSOR = 37,    /* Password screen: digit cursor moves */
    SE_PASSWORD_PRESS = 38,     /* Password screen: key pressed */
    SE_PASSWORD_ROLL = 39,      /* Password screen: digit rolls */
    SE_PASSWORD_REJECT = 40,    /* Password screen: wrong or used password */
    SE_PASSWORD_GET_CARD = 41,  /* Password screen: card received */
};

#endif /* GUARD_CONSTANTS_SOUND_H */
