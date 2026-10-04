/*
 * Duel command handlers (wiki/functions/duel-cmd-turn-c.md): the start-of-turn and end-of-turn bookkeeping,
 * the end-of-turn hand gesture, the per-player and duel-wide rule flags that card effects request, the Card
 * Detail viewer and the "card tiles fly together" presentation of a played card.
 *
 * DuelCmd_Dispatch calls a handler once per frame while gDuelCmd.running is set. The handler reads its
 * operands from gDuelCmd (arg2/arg4; the acting player is bit 15 of cmd), steps through gDuelCmd.step and
 * clears gDuelCmd.running when it is done.
 */
#include "global.h"
#include "legacy/gba.h"                    /* REG_BLDCNT, REG_BLDALPHA, REG_BLDY, B_BUTTON, BLDCNT_* */
#include "legacy/main.h"                   /* gMain.heldKeys */
#include "legacy/sound.h"                  /* PlaySE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, CARD_STATS_TYPE_* */
#include "constants/duel.h"         /* DUEL_AREA_*, ZONE_*, ZONE_LINK_*, BANISH_*, DUEL_LOC */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_NUMBER_ALT_ART */
#include "sprite.h"                 /* AddAffineSprite, SPRITE_SHAPE_* */
#include "card_detail.h"            /* CardDetail_Init, CardDetail_Run */
#include "duel_actions.h"           /* QueueRemoveZoneLink, DestroyFieldCard, ShowCardEffect */
#include "duel_flow.h"              /* gDuelCtrl */

#ifdef DISPCNT_MODE_4
#include "legacy/duel.h"                   /* struct DuelCard / DuelZone / DuelPlayer / DuelState, gDuel, ... */
#include "duel_cmd.h"               /* gDuelCmd, DuelCmd_Push, gScatterScaleCurve */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_*, GetAreaX/Y, the card-image loaders */
#else
/* ---- BEGIN pre-H0 subset ---- */
/*
 * Before H0 (build/readability/HEADERS.md) include/gba.h, main.h, sound.h and duel.h still hold the legacy
 * headers, whose duel structs have other field names, and duel_cmd.h / duel_screen.h need the new duel.h.
 * Until then this block repeats the part of the new gba.h, duel.h, duel_cmd.h, duel_screen.h and sound.h
 * that the unit uses: the same tags, field names, types and bitfield containers (truncated structs end after
 * the last field used here), and the same prototypes. With the new headers installed the block is skipped;
 * then delete it (build/readability/issues/duel_cmd_turn.md).
 */
#define BLDCNT_TGT1_OBJ         0x0010
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_EFFECT_LIGHTEN   0x0080
#define BLDCNT_TGT2_BG0         0x0100
#define BLDCNT_TGT2_BG1         0x0200
#define BLDCNT_TGT2_BG2         0x0400
#define BLDCNT_TGT2_BG3         0x0800
#define BLDCNT_TGT2_OBJ         0x1000

struct DuelCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;
    u32 specialSummoned:1;
    u32 planted:1;
    u32 graverobbed:1;
    u32 unk19:1;
    u32 isFusionMaterial:1;
    u32 destroyedInBattle:1;
    u32 destroyedByOpponent:1;
    u32 flag23:1;
    u32 pendingEquip:1;
    u32 equipZone:3;
    u32 pendingOpponentSummon:1;
    u32 unk29:3;
};
struct DuelZone {
    struct DuelCard card;
    u16 serial;
    u8 isDefense:1;
    u8 isFaceUp:1;
    u8 turnCounter:4;
    u16 destroyCountdown:4;
    u8 positionLocked:1;
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;
    u8 revivedByMonsterReborn:1;
    u8 summonedFromGraveyard:1;
    u8 levelCheckDone:1;
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];
    u16 linkKinds[32];
    u16 numLinks;
    u8 unk8C_0:1;
    u8 destroyAfterBattle:1;
    u32 returnAfterBattle:1;
    u8 cannotAttackNextTurn:1;
    u8 cannotAttack:1;
    u8 atkHalved:1;
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;
    u32 unk90_6:4;
    u32 canActivate:1;
    u8 isDisabled:1;
    u32 unk91_4:1;
    u32 declaredValue:5;
    u32 unk92_2:14;
};
struct DuelPlayer {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 graveCount;
    u8 fusionCount;
    u8 banishedCount;
    u8 deckOut:1;
    u8 exodiaWin:1;
    u8 destinyBoardWin:1;
    u8 noNormalSummon:1;
    u8 noSpecialSummon:1;
    u8 positionChangeLocked:1;
    u8 magicTrapLockTurns:2;
    u8 noBattleDamage:1;
    u8 battleProtected:1;
    u8 unk8_2:1;
    u8 insectQueenWonBattle:1;
    u8 normalSummonUsed:1;
    u8 summonedThisTurn:1;
    u8 extraBattlePhase:1;
    u8 unk8_7:1;
    u8 handRevealed:1;
    u8 skipStandbyPhase:1;
    u8 skipDrawPhase:1;
    u8 skipTurn:1;
    u8 battlePhaseDone:1;
    u8 magicTrapActivatedThisTurn:1;
    u32 lockedZones:10;
    u8 crushCardTurns:3;
    u8 monsterSentToGraveThisTurn:1;
    u32 removedMask:5;
    u8 delayedSummonCount:3;
    u8 destroyedTriggerPending:1;
    u8 banishCostFromField:1;
    u8 unkC_6:2;
    u8 unkD;
    u16 lpPaid[11];
    u16 attackableMask;
    u16 attackedMask;
    struct DuelZone zones[11];
    struct DuelCard hand[80];
    struct DuelCard deck[80];
    struct DuelCard graveyard[80];
    struct DuelCard fusionDeck[80];
    struct DuelCard banished[80];
    u16 banishedInfo[80];
};
struct DuelState {
    u16 serial;
    u16 unk2;
    struct DuelPlayer players[2];
    u8 fieldBackground:4;
    u8 duelOver:1;
    u8 unk1ACC_5:1;
    u8 magicNegated:1;
    u8 trapsNegated:1;
    u8 equipMagicNegated:1;
    u8 equipMagicNegatedThisTurn:1;
    u8 fieldMagicNegatedThisTurn:1;
    u8 contMagicNegatedThisTurn:1;
    u8 contTrapNegatedThisTurn:1;
    u8 statChangesReversed:1;
    u8 atkDefSwapped:1;
    u32 prohibitionCount:4;
    u32 unk1ACE_3:13;
    u16 prohibitionZones[16];
    u16 prohibitedCards[16];
    u16 turnCount;
    u8 bgmOn:1;
    u8 turnPlayer:1;
    u8 phase:3;
    u8 linkError:1;
    u8 result:2;
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};
extern struct DuelState gDuel;
extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);

struct DuelCmdEntry {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
};
struct DuelCmd {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
    struct DuelCmdEntry queue[256];
    u16 queueCount;
    u16 step:7;
    u16 counter:7;
    u16 unk80A_14:2;
    u32 unk80C_0:5;
    u32 timer:7;
    u32 unk80C_12:1;
    u32 running:1;
    u32 unk80C_14:18;
};
extern struct DuelCmd gDuelCmd;
extern const s32 gScatterScaleCurve[];
void DuelCmd_Push(u16 cmd, u16 arg2, int arg4, int arg6);

struct DuelScreen {
    u8 fast:1;
    u8 uiGfxLoaded:1;
    u8 active:1;
    u8 unk0_3:5;
};
extern struct DuelScreen gDuelScreen;
u32 DuelScreen_FadeOutStep(void);
void LoadDuelUiGfx(void);
void UnloadDuelUiGfx(void);
void DuelScreen_ScrollToZone(u32 player, u32 area);
s32 GetAreaX(int player, int area, int index);
s32 GetAreaY(u32 player, int area, int index);
void TextCellsClear(void);
void DuelInfo_DrawCardNameCentered(u16 cardId);
void LoadCardFrame(u16 cardId);
void LoadCardPicture(u16 cardId);
void DrawCardInfo(u16 cardId);

void PlaySE(u32 seId);
/* ---- END pre-H0 subset ---- */
#endif

/*
 * Local views of the sprite emitters (sprite.h declares the shape and tile as u16). This unit calls them with
 * u32 parameters: the u16 ones add narrowing at the call sites, and the ROM has none.
 */
void AddSprite8bppU32(u32 yx, u32 shape, u32 tile) asm("AddSprite8bpp");
void AddSprite8bppAlphaU32(u32 yx, u32 shape, u32 tile) asm("AddSprite8bppAlpha");

/*
 * Local view of ShowCardEffect (duel_actions.h: int player, u16 cardId). DuelCmd_TurnEnd passes the card ID
 * as an int; with the u16 parameter the argument registers are set up in another order.
 */
void ShowCardEffectInt(int player, int cardId) asm("ShowCardEffect");

/*
 * Local view of the player bytes +0x06/+0x07 as one u16 bitfield container: DuelCmd_SetSummonLocks writes
 * noNormalSummon and noSpecialSummon (+0x07 bits 3 and 4) with halfword accesses, and only matches with this
 * container (duel.h declares them as u8 bitfields). Same symbol and stride as gDuelPlayers.
 */
struct DuelPlayerSummonLocks {
    u8 unk0[6];
    u16 banishedCount:8;            /* +0x06 */
    u16 unk7_0:3;                   /* +0x07 bits 0-2: deckOut, exodiaWin, destinyBoardWin */
    u16 noNormalSummon:1;           /* +0x07 bit 3 */
    u16 noSpecialSummon:1;          /* +0x07 bit 4 */
    u16 unk7_5:3;                   /* +0x07 bits 5-7: positionChangeLocked, magicTrapLockTurns */
    u8 rest[0xD64 - 8];
};
extern struct DuelPlayerSummonLocks gDuelPlayersSummonLocks[2] asm("gDuelPlayers");

/* ROM data used only by this unit. */
extern u16 gEndOfTurnEffectCards[12];       /* card numbers whose ZONE_LINK_CARD_EFFECT links end with the
                                             * turn ("during this turn" effects: Reinforcements, Castle Walls,
                                             * Rush Recklessly, Graceful Dice, ...) */
extern const u16 gTurnEndHandFrames[];      /* [32] OBJ tile offset per frame of the end-of-turn hand */
/* Alias symbol = gCardNumberToId[CARD_INSECT_MONSTER_TOKEN] (the token's card ID); the matched code loads
 * this address from its own literal. */
extern const u16 gCardNumberToId_InsectMonsterToken[];

/* The acting player of the current command (cmd bit 15), in the two forms the handlers use. */
#define CMD_PLAYER()        (gDuelCmd.cmd >> 15)
#define CMD_PLAYER_BIT()    ((gDuelCmd.cmd & DUEL_CMD_PLAYER) != 0)

/* Fast-forward: B held, or the duel's fast mode. */
#define FAST_FORWARD()      ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)

/* Sound effects (constants/sound.h has no names for these yet; the names say where they are played). */
#define SE_CARD_SHOWN       0x2C    /* the presented card is complete (also DuelCmd_ShowCardUnrollSideways) */
#define SE_END_TURN_HAND    15      /* the end-of-turn hand appears */

/*
 * Effect keys without an EDS card (CARD_<n>) used here: CARD_1334 and CARD_1513 refresh like the
 * once-per-turn monsters, CARD_1344 locks the monster's position once, a link from CARD_1314 or CARD_1512
 * destroys the monster at the end of the turn, and CARD_1255 gives control back like Change of Heart (only on
 * the acting player's side).
 */

/*
 * Command 0x01 (DUEL_CMD_TURN_START), queued by the duel's turn-start step; finishes in one call.
 * - acting player: Crush Card's turn counter counts down;
 * - both players: the Insect Queen battle flag is cleared and the Magic/Trap lock turns count down (when
 *   either player's reaches 0, both are cleared);
 * - acting player's monsters: the position lock is lifted; a face-up monster's turn counter counts up (to
 *   15). Castle of Dark Illusions counts one more once its effect is in use (effectUnused clear; up to 6),
 *   Hourglass of Courage one more (up to 4). The once-per-turn monsters (Time Wizard, Goddess of Whim,
 *   Patrol Robo, Relinquished, Barrel Dragon, Karate Man and keys 1334/1513) get their effect back and lose
 *   any ZONE_LINK_ATK_300_PER_VALUE link of their own card ID; key 1344, once its effect was used, gets it
 *   back together with a position lock;
 * - acting player's spell/trap zones: a face-up Cocoon of Evolution counts its turns (to 12); a face-down
 *   Magic or Trap card may now be activated;
 * - cards banished face down (BANISH_FACE_DOWN) count their turns in the high byte of banishedInfo (to 5).
 */
void DuelCmd_TurnStart(void)
{
    u32 player = gDuelCmd.cmd >> 15;
    int i;
    int zone;
    u32 stride;
    u16 monsterId, spellId;
    struct DuelZone *monster, *spell;

    if (gDuelPlayers[player & 1].crushCardTurns)
        gDuelPlayers[player & 1].crushCardTurns--;
    gDuelPlayers[(1 - player) & 1].insectQueenWonBattle = 0;
    gDuelPlayers[player & 1].insectQueenWonBattle = 0;

    for (i = 0; i <= 1; i++) {
        if (gDuelPlayers[i & 1].magicTrapLockTurns)
            gDuelPlayers[i & 1].magicTrapLockTurns--;
    }
    if (!gDuelPlayers[0].magicTrapLockTurns || !gDuelPlayers[1].magicTrapLockTurns) {
        gDuelPlayers[0].magicTrapLockTurns = 0;
        gDuelPlayers[1].magicTrapLockTurns = 0;
    }

    /* A stride variable set before the loop: its pseudo is spilled and rematerialised each
     * iteration (ldr r4, =0xD64), and player * stride stays in the loop as in the ROM. */
    stride = 0xD64;
    for (zone = 0; zone <= 4; zone++) {
        struct DuelZonesPlayer *base = gDuelZones;
        struct DuelZonesPlayer *own = (struct DuelZonesPlayer *)((u8 *)base + player * stride);

        monster = &own->zones[zone];
        spell = &own->zones[zone + ZONE_SPELL_0];
        /* Byte offsets (zone first) give the ROM's zone*0x94 + player*0xD64 sum, shared by both reads;
         * 0x2E4 = ZONE_SPELL_0 * 0x94. */
        monsterId = ((struct DuelCard *)((u8 *)gDuelZones + zone * 0x94 + player * stride))->id;
        spellId = ((struct DuelCard *)((u8 *)gDuelZones + 0x2E4 + zone * 0x94 + player * stride))->id;

        if (monsterId) {
            monster->positionLocked = 0;
            if (monster->isFaceUp) {
                if (monster->turnCounter < 15)
                    monster->turnCounter++;
                switch (gCardIdToNumber[monsterId & CARD_ID_MASK]) {
                case CARD_CASTLE_OF_DARK_ILLUSIONS:
                    if (!monster->effectUnused)
                        monster->turnCounter++;
                    if (monster->turnCounter > 6)
                        monster->turnCounter = 6;
                    break;
                case CARD_HOURGLASS_OF_COURAGE:
                    monster->turnCounter++;
                    if (monster->turnCounter > 4)
                        monster->turnCounter = 4;
                    break;
                case CARD_TIME_WIZARD:
                case CARD_GODDESS_OF_WHIM:
                case CARD_PATROL_ROBO:
                case CARD_RELINQUISHED:
                case CARD_BARREL_DRAGON:
                case CARD_KARATE_MAN:
                case CARD_1334:
                case CARD_1513:
                    monster->effectUnused = 1;
                    QueueRemoveZoneLink(player, monsterId, DUEL_LOC(player, (u8)zone),
                                        ZONE_LINK_ATK_300_PER_VALUE);
                    break;
                case CARD_1344:
                    if (!monster->effectUnused) {
                        monster->positionLocked = 1;
                        monster->effectUnused = 1;
                    }
                    break;
                }
            }
        }
        if (spellId) {
            if (spell->isFaceUp) {
                if (gCardIdToNumber[spellId & CARD_ID_MASK] == CARD_COCOON_OF_EVOLUTION && spell->turnCounter < 12)
                    spell->turnCounter++;
            } else if ((*(const u32 *)(0x08621DE0 + (spellId & CARD_ID_MASK) * 4) & CARD_STATS_TYPE_MASK)
                           >> CARD_STATS_TYPE_SHIFT > CARD_TYPE_REPTILE) {
                /* a face-down Magic or Trap card (0x08621DE0 = gCardStats, integer form kept) */
                spell->canActivate = 1;
            }
        }
    }

    /* FAKEMATCH: the ROM multiplies player by 0xD64 twice around the banished loop (once for the
     * count test, once more in the preheader for the entry pointer). The count test reads
     * through an asm-opaque copy of player so cse2 cannot share the body's product; the
     * statement-expression stride loads 0xD64 before that copy, as in the ROM.
     * Byte 0xA of gDuel + player * 0xD64 is players[player].banishedCount. */
    {
        struct DuelState *duel;
        u32 player2;

        i = 0;
        duel = &gDuel;
        if (i < ((u8 *)duel + ({ u32 c = 0xD64; c; })
                              * ({ player2 = player; asm("" : "+r"(player2)); player2; }))[0xA]) {
            for (; i < duel->players[player2].banishedCount; i++) {
                u16 info = gDuel.players[player].banishedInfo[i];
                /* low byte: enum BanishKind; high byte: turns spent banished face down */
                if ((u8)gDuel.players[player].banishedInfo[i] == BANISH_FACE_DOWN && (u8)(info >> 8) <= 4)
                    gDuel.players[player].banishedInfo[i] = ((u8)((info >> 8) + 1) << 8) | BANISH_FACE_DOWN;
            }
        }
    }
    gDuelCmd.running = 0;
}

/*
 * Zones of gDuel addressed as players[0].zones + byte offsets (through the gDuel symbol). The two association
 * orders compile differently, and each loop of DuelCmd_TurnEnd keeps the one it matches with.
 */
#define TURN_END_ZONE(player, zone) \
    ((struct DuelZone *)((u8 *)gDuel.players[0].zones + ((zone) * 0x94 + (player) * 0xD64)))
#define TURN_END_ZONE_PZ(player, zone) \
    ((struct DuelZone *)((u8 *)gDuel.players[0].zones + ((player) * 0xD64 + (zone) * 0x94)))

/* Command word of command id for player (DUEL_CMD_PLAYER set for player 1), for DuelCmd_Push. */
#define CMD_OF(player, id) ((player) ? (DUEL_CMD_PLAYER | (id)) : (id))

/* DUEL_LOC(player, zone) of byte-sized values, ORed player first (the order DuelCmd_TurnEnd matches with) */
#define ZONE_LOC(player, zone) ((u8)(player) | ((u8)(zone) << 8))

/* gDuel.players seen through a pointer to the array: the pile loops of DuelCmd_TurnEnd match with this form */
#define DUEL_PLAYERS (*(struct DuelPlayer (*)[2])gDuel.players)

/* gCardNumberToId, for an alternate-art number as well (2000 + n gives the ID of n, plus 1); 0xFFFF gives 0.
 * 0x08623DF4 = gCardNumberToId (integer form kept). */
static inline u16 CardNumberToId(u16 cardNo)
{
    if (cardNo == 0xFFFF)
        return 0;
    if (cardNo <= CARD_NUMBER_ALT_ART - 1)
        return ((const u16 *)0x08623DF4)[cardNo & CARD_ID_MASK];
    return ((const u16 *)0x08623DF4)[(cardNo - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/*
 * End of the turn for a monster taken with card cardNo (Change of Heart): if (player, zone) has a link from
 * that card, control goes back. The monster moves to the other player's first free monster zone and the
 * card's ZONE_LINK_CARD_EFFECT links at both places are removed; with no free zone it is destroyed.
 */
static inline void ReturnControlAtTurnEnd(int player, int zone, u16 cardNo)
{
    if (CountZoneLinksFromCard(player, zone, cardNo)) {
        int target = FindFreeMonsterZone(1 - player);

        if (target < 0) {
            DestroyFieldCard(player, zone, 1);
        } else {
            u16 cmd = CMD_OF(player, DUEL_CMD_MOVE_TO_ZONE);
            u16 from = ZONE_LOC(player, zone);
            u16 to = DUEL_LOC((u8)(1 - player), (u8)target);

            DuelCmd_Push(cmd, from, to, 0);
            QueueRemoveZoneLink(player, ((u16 *)0x08623DF4)[cardNo], from, ZONE_LINK_CARD_EFFECT);
            QueueRemoveZoneLink(player, ((u16 *)0x08623DF4)[cardNo], to, ZONE_LINK_CARD_EFFECT);
        }
    }
}

/*
 * Command 0x02 (DUEL_CMD_TURN_END), queued by the duel's turn-end step. Step 0 scrolls to the acting player's
 * monster row; the next call does all of this and finishes:
 * 1. acting player's monsters: destroy countdowns (Zone Eater, Steel Scorpion) count down; a monster whose
 *    countdown reaches 0 is destroyed (in a link duel only when gDuel.turnPlayer is 0, the local player);
 * 2. acting player: the one-turn position and summon locks and the battle damage guards end;
 * 3. the one-turn negations, Reverse Trap and Shield & Sword end;
 * 4. both players' hand, deck, fusion deck, graveyard and banished cards lose the destroyedInBattle and
 *    destroyedByOpponent marks;
 * 5. both players' face-down Magic/Trap cards may be activated;
 * 6. every monster's ATK halving (Riryoku) ends;
 * 7. unless the link partner owns this turn, for every monster: a Karate Man whose effect was used and a
 *    monster linked to key 1314 / 1512 are destroyed; ATK bonus links (kind 4) and the links of
 *    gEndOfTurnEffectCards end; Change of Heart (and key 1255 on the acting player's side) gives control
 *    back; an Insect Queen that won a battle this turn makes an Insect Monster Token in a free zone.
 */
void DuelCmd_TurnEnd(void)
{
    int player = gDuelCmd.cmd >> 15;
    int i;
    int j;

    if ((s8)gDuelCmd.step == 0) {      /* FAKEMATCH: the (s8) cast is needed to match */
        DuelScreen_ScrollToZone(player, DUEL_AREA_MONSTER);
        gDuelCmd.step++;
        return;
    }

    /* 1. destroy countdowns */
    for (i = 0; i <= 4; i++) {
        struct DuelZone *zone = TURN_END_ZONE(player & 1, i);

        if (zone->destroyCountdown) {
            zone->destroyCountdown--;
            if ((!gDuelCtrl.isLinkDuel || !gDuel.turnPlayer) && zone->destroyCountdown == 0)
                DestroyFieldCard(player, i, 1);
        }
    }

    /* 2. the acting player's one-turn locks (players base + byte offset: &gDuel.players[player & 1] gives other
     * code) */
    {
        u32 players = (u32)gDuel.players;
        struct DuelPlayer *self = (struct DuelPlayer *)((player & 1) * 0xD64 + players);

        self->positionChangeLocked = 0;
        self->noNormalSummon = 0;
        self->noSpecialSummon = 0;
        self->noBattleDamage = 0;
        self->battleProtected = 0;
        self->unk8_2 = 0;
    }

    /* 3. duel-wide one-turn flags */
    gDuel.equipMagicNegatedThisTurn = 0;
    gDuel.fieldMagicNegatedThisTurn = 0;
    gDuel.contMagicNegatedThisTurn = 0;
    gDuel.contTrapNegatedThisTurn = 0;
    gDuel.statChangesReversed = 0;
    gDuel.atkDefSwapped = 0;

    /* 4. battle marks on the cards in every pile */
    for (i = 0; i <= 1; i++) {
        /* FAKEMATCH: the statement expression loads the players base before the 0xD64 stride in the
         * first count test, so loop.c hoists the base copy first and the copies land in ip/sl as in the ROM. */
        for (j = 0; j < ((struct DuelPlayer *)({ u32 b_ = (u32)gDuel.players; b_; }))[i & 1].handCount; j++) {
            int offset = (i & 1) * 0xD64;
            u32 pile = (u32)gDuel.players[0].hand;

            ((struct DuelCard *)(offset + pile))[j].destroyedInBattle = 0;
            ((struct DuelCard *)(offset + pile))[j].destroyedByOpponent = 0;
        }
        for (j = 0; j < DUEL_PLAYERS[i & 1].deckCount; j++) {
            int offset = (i & 1) * 0xD64;
            u32 pile = (u32)gDuel.players[0].deck;

            ((struct DuelCard *)(offset + pile))[j].destroyedInBattle = 0;
            ((struct DuelCard *)(offset + pile))[j].destroyedByOpponent = 0;
        }
        for (j = 0; j < DUEL_PLAYERS[i & 1].fusionCount; j++) {
            int offset = (i & 1) * 0xD64;
            u32 pile = (u32)gDuel.players[0].fusionDeck;

            ((struct DuelCard *)(offset + pile))[j].destroyedInBattle = 0;
            ((struct DuelCard *)(offset + pile))[j].destroyedByOpponent = 0;
        }
        for (j = 0; j < DUEL_PLAYERS[i & 1].graveCount; j++) {
            int offset = (i & 1) * 0xD64;
            u32 pile = (u32)gDuel.players[0].graveyard;

            ((struct DuelCard *)(offset + pile))[j].destroyedInBattle = 0;
            ((struct DuelCard *)(offset + pile))[j].destroyedByOpponent = 0;
        }
        for (j = 0; j < gDuel.players[i].banishedCount; j++) {
            ((struct DuelCard *)((u8 *)gDuel.players[0].banished + (i & 1) * 0xD64))[j].destroyedInBattle = 0;
            ((struct DuelCard *)((u8 *)gDuel.players[0].banished + (i & 1) * 0xD64))[j].destroyedByOpponent = 0;
        }
    }

    /* 5. face-down Magic/Trap cards (0x08621DE0 = gCardStats, integer form kept) */
    for (j = 0; j <= 1; j++) {
        for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
            struct DuelZonesPlayer *own = &(*(struct DuelZonesPlayer (*)[2])gDuel.players[0].zones)[j & 1];
            struct DuelZone *zone = &own->zones[i];
            /* FAKEMATCH: zone->card.id read as the whole word with shifts; the member read compiles to other
             * code (also below) */
            u16 id = (*(u32 *)zone << 20) >> 20;

            if (id && ((*((const u32 *)0x08621DE0 + (id & CARD_ID_MASK)) & CARD_STATS_TYPE_MASK)
                           >> CARD_STATS_TYPE_SHIFT) > CARD_TYPE_REPTILE && !zone->isFaceUp)
                zone->canActivate = 1;
        }
    }

    /* 6. ATK halving ends */
    for (player = 0; player <= 1; player++) {
        for (i = 0; i <= 4; i++)
            TURN_END_ZONE_PZ(player & 1, i)->atkHalved = 0;
    }

    /* 7. in a link duel the partner's console queues these commands on its own turn */
    if (gDuelCtrl.isLinkDuel && gDuel.turnPlayer)
        goto end;

    for (player = 0; player <= 1; player++) {
        for (i = 0; i <= 4; i++) {
            int destroy = 0;
            u32 id = (*(u32 *)TURN_END_ZONE_PZ(player & 1, i) << 20) >> 20;     /* card.id, see step 5 */
            int hasAtkBonus;

            if (id == 0)
                continue;
            /* 0x08622AB4 = gCardIdToNumber (integer form kept) */
            if (((const u16 *)0x08622AB4)[(u16)id & CARD_ID_MASK] == CARD_KARATE_MAN
                && TURN_END_ZONE_PZ(player & 1, i)->isFaceUp && !TURN_END_ZONE_PZ(player & 1, i)->effectUnused)
                destroy = 1;
            if (CountZoneLinksFromCard(player, i, CARD_1314))
                destroy = 1;
            if (CountZoneLinksFromCard(player, i, CARD_1512))
                destroy = 1;
            if (destroy) {
                DestroyFieldCard(player, i, 1);
                continue;
            }

            hasAtkBonus = 0;
            for (j = 0; j < TURN_END_ZONE_PZ(player & 1, i)->numLinks; j++) {
                if ((u8)TURN_END_ZONE_PZ(player & 1, i)->linkKinds[j] == ZONE_LINK_ATK_BONUS)
                    hasAtkBonus = 1;
            }
            if (hasAtkBonus)
                QueueRemoveZoneLink(player, 0, ZONE_LOC(player, i), ZONE_LINK_ATK_BONUS);

            for (j = 0; j < ARRAY_COUNT(gEndOfTurnEffectCards); j++) {
                if (CountZoneLinksFromCard(player, i, gEndOfTurnEffectCards[j]))
                    QueueRemoveZoneLink(player, CardNumberToId(gEndOfTurnEffectCards[j]),
                                        ZONE_LOC(player, i), ZONE_LINK_CARD_EFFECT);
            }

            /* FAKEMATCH: the card numbers go through u16 locals; a constant argument is propagated into the
             * inline function and gives other code. */
            { u16 cardNo = CARD_CHANGE_OF_HEART; ReturnControlAtTurnEnd(player, i, cardNo); }
            if (player == gDuelCmd.cmd >> 15) {
                u16 cardNo = CARD_1255;
                ReturnControlAtTurnEnd(player, i, cardNo);
            }

            /* Insect Queen that won a battle this turn: an Insect Monster Token in the first free zone */
            if (((const u16 *)0x08622AB4)[(u16)id & CARD_ID_MASK] == CARD_INSECT_QUEEN
                && gDuelPlayers[player & 1].insectQueenWonBattle && CountFreeMonsterZones(player) > 0) {
                int target = FindFreeMonsterZone(player);

                ShowCardEffectInt(player, id);
                DuelCmd_Push(CMD_OF(player, DUEL_CMD_SHOW_CARD_ASSEMBLE), gCardNumberToId_InsectMonsterToken[0], 1, 0);
                /* arg2: the token's zone (the high byte, Insect Queen's zone, is not read) */
                DuelCmd_Push(CMD_OF(player, DUEL_CMD_SUMMON_TOKEN), (u8)target | ((u8)i << 8),
                             TOKEN_KIND_INSECT_MONSTER, 0);
            }
        }
    }
end:
    gDuelCmd.running = 0;
}

/*
 * Command 0x03 (DUEL_CMD_SHOW_END_TURN_HAND), queued after DUEL_CMD_TURN_END: a white glove over the acting
 * player's centre monster zone points and then waves (32 frames of gTurnEndHandFrames). The sprite is affine
 * (double size); player 1's is turned half a turn (angle 0x40 of 128).
 */
void DuelCmd_ShowEndTurnHand(void)
{
    u32 player = gDuelCmd.cmd >> 15;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_MONSTER);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        PlaySE(SE_END_TURN_HAND);
        break;
    case 1:
        if (gDuelCmd.timer < 32) {
            u32 x = GetAreaX(player, DUEL_AREA_MONSTER, ZONE_MONSTER_2);
            u32 yx = (GetAreaY(player, DUEL_AREA_MONSTER, ZONE_MONSTER_2) << 16) | x;

            /* attr2 0x400: priority 1; scaleAngle: scale 0x100, angle 0 or 0x40 */
            AddAffineSprite(yx, SPRITE_SHAPE_32x32, gTurnEndHandFrames[gDuelCmd.timer] | 0x400,
                            player ? 0x1000040 : 0x1000000);
            gDuelCmd.timer++;
            break;
        }
        /* fall through */
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* Command 0x44 (DUEL_CMD_SKIP_NEXT_DRAW_PHASE): the acting player skips the Draw Phase of the next turn
 * (Time Seal, aimed at the opponent). */
void DuelCmd_SkipNextDrawPhase(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].skipDrawPhase = 1;
    gDuelCmd.running = 0;
}

/* Command 0x45 (DUEL_CMD_SKIP_NEXT_STANDBY_PHASE): the acting player skips the next Standby Phase
 * (Solomon's Lawbook). */
void DuelCmd_SkipNextStandbyPhase(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].skipStandbyPhase = 1;
    gDuelCmd.running = 0;
}

/* Command 0x46 (DUEL_CMD_SKIP_NEXT_TURN): the acting player's next turn goes straight to its end. */
void DuelCmd_SkipNextTurn(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].skipTurn = 1;
    gDuelCmd.running = 0;
}

/* Command 0x47 (DUEL_CMD_SET_EXTRA_BATTLE_PHASE): a second Battle Phase allowed for the acting player = arg2
 * (Weather Report). */
void DuelCmd_SetExtraBattlePhase(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].extraBattlePhase = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/* Command 0x48 (DUEL_CMD_SET_POSITION_CHANGE_LOCK): the acting player may not change positions = arg2
 * (Mesmeric Control, Curse of Fiend); DuelCmd_TurnEnd clears it. */
void DuelCmd_SetPositionChangeLock(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].positionChangeLocked = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/* Command 0x49 (DUEL_CMD_SET_SUMMON_LOCKS): for the acting player, no Normal Summon = arg2 and no Special
 * Summon = arg4; DuelCmd_TurnEnd clears both. */
void DuelCmd_SetSummonLocks(void)
{
    gDuelPlayersSummonLocks[CMD_PLAYER_BIT()].noNormalSummon = gDuelCmd.arg2;
    gDuelPlayersSummonLocks[CMD_PLAYER_BIT()].noSpecialSummon = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* Command 0x4A (DUEL_CMD_SET_MAGIC_TRAP_LOCK_TURNS): turns in which the acting player may not play Magic or
 * Trap cards = arg2 (counted down by DuelCmd_TurnStart). */
void DuelCmd_SetMagicTrapLockTurns(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].magicTrapLockTurns = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/* Command 0x1C (DUEL_CMD_SET_STAT_CHANGES_REVERSED): stat modifiers are subtracted = arg2 (Reverse Trap);
 * DuelCmd_TurnEnd clears it. */
void DuelCmd_SetStatChangesReversed(void)
{
    gDuel.statChangesReversed = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/* Command 0x1D (DUEL_CMD_SET_ATK_DEF_SWAPPED): ATK and DEF are swapped = arg2 (Shield & Sword);
 * DuelCmd_TurnEnd clears it. */
void DuelCmd_SetAtkDefSwapped(void)
{
    gDuel.atkDefSwapped = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/* Command 0x4B (DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT): the acting player's count of delayed Special Summons
 * (key 1405) goes up by 1 when arg2 != 0, else down by 1 (not below 0). */
void DuelCmd_AdjustDelayedSummonCount(void)
{
    if (gDuelCmd.arg2)
        gDuelPlayers[CMD_PLAYER_BIT()].delayedSummonCount++;
    else if (gDuelPlayers[CMD_PLAYER_BIT()].delayedSummonCount)
        gDuelPlayers[CMD_PLAYER_BIT()].delayedSummonCount--;
    gDuelCmd.running = 0;
}

/* Command 0x4C (DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING): destroyedTriggerPending of the acting player = arg2
 * (key 1514 was destroyed; its effect runs in the End Phase). */
void DuelCmd_SetDestroyedTriggerPending(void)
{
    gDuelPlayers[CMD_PLAYER_BIT()].destroyedTriggerPending = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/*
 * Command 0x70 (DUEL_CMD_SHOW_CARD_DETAIL), arg2 = card ID: the full-screen Card Detail page. Step 0 waits
 * for the duel screen to fade out, then opens the viewer with a 300-frame auto-close timer; step 1 runs it
 * until it has closed.
 */
void DuelCmd_ShowCardDetail(void)
{
    switch (gDuelCmd.step) {
    case 0:
        if (DuelScreen_FadeOutStep()) {
            gDuelScreen.active = 0;
            CardDetail_Init(gDuelCmd.arg2, 300, 0);
            gDuelCmd.step++;
        }
        break;
    case 1:
        if (CardDetail_Run())
            gDuelCmd.step++;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* The card grid of the presentations (see duel_cmd_presentation.c): 4 x 5 sprites of 32x32, top-left at
 * (0x44, 2), centred on (CARD_CENTER_X, CARD_CENTER_Y). */
#define CARD_GRID_X(col)            ((col) * 32 + 0x44)
#define CARD_GRID_Y(row)            ((row) * 32 + 2)
#define CARD_GRID_TILE(row, col)    ((u16)((col) * 4) + ((u16)((row) * 2 + 1) << 5))
#define CARD_CENTER_X               0x68
#define CARD_CENTER_Y               0x40

#define BLEND_CARD_OVER_BG  (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 \
                             | BLDCNT_TGT2_BG3)                         /* 0x0F40: card alpha-blended over BG0-3 */
#define BRIGHTEN_SPRITES    (BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_OBJ)     /* 0x1090 */

/* BLDALPHA value: weight eva of the card, evb of the background. Same value as gba.h's BLDALPHA_BLEND, but
 * ORed in the ROM's operand order (the other order changes the code). */
#define BLEND_WEIGHTS(eva, evb) ((eva) | ((evb) << 8))

/*
 * Command 0x71 (DUEL_CMD_SHOW_CARD_ASSEMBLE), arg2 = card ID: the presentation of a played or summoned card.
 * Steps 0-4 scroll to player 0's monster row, free the sprite VRAM and build the card image. Step 5: the 4x5
 * cells fly together from a 2.8x spread (gScatterScaleCurve read backwards, about 1/3 of its range) over 0x18
 * frames while the card fades in; at frame 0x20 the card name is shown and SE_CARD_SHOWN plays. Step 6
 * flashes the card white for 32 frames, step 7 holds it and fades it out over frames 0x30-0x40. Then the duel
 * sprites are reloaded.
 */
void DuelCmd_ShowCardAssemble(void)
{
    int row, col;
    int x, y, dy, scale;
    int tFly, tBlend, tFlash, tFade, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, DUEL_AREA_MONSTER);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if ((tFly = gDuelCmd.timer) < 0x18) {
                    /* pos = centre + (pos - centre) * scale / 128, scale from 0x16C down to 0x80 */
                    x -= CARD_CENTER_X;
                    dy = y - CARD_CENTER_Y;
                    scale = (gScatterScaleCurve[0x17 - tFly] - 0x100) / 3 + 0x80;
                    x *= scale;
                    dy *= scale;
                    x /= 128;
                    dy /= 128;
                    x += CARD_CENTER_X;
                    y = dy + CARD_CENTER_Y;
                }
                AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tBlend = gDuelCmd.timer;
        if (tBlend < 16) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS(tBlend, (u8)(16 - tBlend));
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x17)
            gDuelCmd.timer += 7;
        if (gDuelCmd.timer >= 0x20) {
            TextCellsClear();
            DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
            PlaySE(SE_CARD_SHOWN);
        }
        break;
    case 6:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++)
                AddSprite8bppU32(CARD_GRID_X(col) | (CARD_GRID_Y(row) << 16), SPRITE_SHAPE_32x32,
                                 CARD_GRID_TILE(row, col));
        }
        /* white flash on the card: BLDY 0 -> 15 over 16 frames, then back to 0 */
        tFlash = gDuelCmd.timer;
        if (tFlash < 0x20) {
            if (tFlash < 16) {
                REG_BLDY = tFlash;
                REG_BLDCNT = BRIGHTEN_SPRITES;
            } else {
                REG_BLDY = 0x1F - tFlash;
                REG_BLDCNT = BRIGHTEN_SPRITES;
            }
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x17)
            gDuelCmd.timer += 7;
        if (gDuelCmd.timer >= 0x20) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 7:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++)
                AddSprite8bppAlphaU32(CARD_GRID_X(col) | (CARD_GRID_Y(row) << 16), SPRITE_SHAPE_32x32,
                                      CARD_GRID_TILE(row, col));
        }
        tFade = gDuelCmd.timer;
        if (tFade > 0x30) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x40 - tFade), (u8)(tFade - 0x30));
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x40) {
            if (FAST_FORWARD() && tEnd <= 0x37)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 8:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}
