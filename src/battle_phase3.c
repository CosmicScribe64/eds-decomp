/*
 * battle_phase3 (0x0804DB6C-0x0804EFEF): the last Battle Phase stages, the Draw Phase and the End Phase
 * dispatcher (wiki/functions/battle-phase3-c.md).
 *
 * BattlePhase_Run runs the Battle Phase stages through gBattleStageHandlers; this unit holds the last
 * three: BattleStage_EndAttack (stage 11, Kotodama and the key 1243 control change), BattleStage_EndBattlePhase
 * (stage 12, the graveyard pending-equip / pending-summon bookkeeping) and BattleStage_Cleanup (stage 13,
 * Magical Hats decoys and borrowed monsters). DuelPhase_Draw (duel step 3) and DuelPhase_End (duel step 6,
 * enum EndPhaseStep) are the Draw and End Phase handlers; DuelPhase_End announces the phase, runs the
 * three end-of-phase scans (EndPhase_ReturnWickedWormBeast, EndPhase_TransferMushroomMan2,
 * EndPhase_DestroyLowLevelMonsters), resolves the zone-link and graveyard effects and enforces the
 * six-card hand limit. BattlePhase_UnusedNop is an empty stub; DuelScreen_DrawPulseIconOverlay queues the pulsing sprite of the
 * selected card.
 *
 * Card numbers written CARD_12xx to CARD_15xx are effect-table keys of cards the EDS ROM does not
 * contain; their meanings come from what the code does with them.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, gCardNames */
#include "constants/card_stats.h"   /* CARD_STATS_* masks and shifts, enum CardType */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelArea, DuelPromptKind, ChainEntryKind, ResponseEventKind, ZoneStatusFlag */
#include "constants/duel_cmds.h"    /* DUEL_CMD_*, DUEL_CMD_PLAYER */
#include "duel.h"                 /* gDuel, gDuelPlayers, gDuelZones, duel structs */
#include "gba.h"                  /* R_BUTTON */
#include "main.h"                 /* struct Main gMain (newKeys, frameCounter) */

#include "battle.h"                 /* gBattle (struct Battle), the functions defined here */
#include "chain.h"                  /* Chain_AddPending */
#include "duel_actions.h"           /* DestroyFieldCard, ReturnFieldCardToHand, ChangeBattlePosition, MoveFieldCard, DrawCards, ShowCardEffect */
#include "duel_cmd.h"               /* DuelCmd_Push (through the U16 alias below) */
#include "duel_flow.h"              /* gDuelCtrl, Duel_CheckWin, gPulseScaleCurve, enum EndPhaseStep, DuelPhase_Draw, DuelPhase_End */
#include "duel_prompt.h"            /* DuelPrompt_Post, DuelPrompt_PostDiscard */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_HandleInput, DuelCursor_Select */
#include "effect.h"                 /* ApplyKotodamaToZone, OnCardDestroyedByEffect, ResolvePendingGraveyardEquip, PlaceNextSpiritMessage */
#include "sprite.h"                 /* AddAffineSprite */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr */

/* ---- ROM data used only here ---- */

extern const char gStrAskTransferControlFmt[];  /* 0x08085BD8: "Do you wish to pay 2500LP and transfer control
                                                 * of %s to your opponent?" (Mushroom Man #2) */
/* Card IDs read through their alias symbols (= &gCardNumberToId[CARD_x], include/card_data.h); indexing
 * gCardNumberToId instead changes the code. */
extern const u16 gCardNumberToId_MushroomMan2[];               /* CARD_MUSHROOM_MAN_2 */
extern const u16 gCardNumberToId_SwordOfDragonsSoul[];               /* CARD_SWORD_OF_DRAGONS_SOUL */
extern const u16 gCardNumberToId_1322[];               /* CARD_1322 (0 in EDS) */
extern const u16 gCardNumberToId_1340[];               /* CARD_1340 (0 in EDS) */
extern const u16 gCardNumberToId_1514[];               /* CARD_1514 (0 in EDS) */
extern const u16 gCardNumberToId_1538[];               /* CARD_1538 (0 in EDS) */
extern const u16 gCardNumberToId_1548[];               /* CARD_1548 (0 in EDS) */
/* 0x0819D1D8: the Battle Phase stage handlers, indexed by gDuel.battleStage (enum BattleStage). */
extern int (*const volatile gBattleStageHandlers[])(int);

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/* gDuel as bytes: this unit's step machines work on the raw step bytes and pointers into the state, and
 * read the state through the views below, so the global keeps its byte-array declaration. */
extern u8 gDuelBytes[] asm("gDuel");            /* 0x020192E0: struct DuelState */
/* The graveyards as raw card words (gDuel + 0x908 = gDuelPlayers[0].graveyard). */
extern u32 gDuelGraveyardWords[] asm("gDuelGraveyards"); /* 0x02019BE8 */

/* Matching: Duel_CheckWin's result is tested as an int here (duel_flow.h declares u16). */
extern int Duel_CheckWinInt(void) asm("Duel_CheckWin");
/* Matching: byte view of gDuelCtrl; the ROM tests byte +1 (isLinkDuel) through the array, and casting
 * &gDuelCtrl would fold the +1 into the literal pool entry. */
extern u8 gDuelCtrlBytes[] asm("gDuelCtrl");
/* Matching: this unit calls DuelCmd_Push through u16 parameters (the operands are narrowed at the call
 * sites); duel_cmd.h declares the int form. */
extern void DuelCmd_PushU16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
/* Matching: the equip flag is passed as int here (effect.h declares u16). */
extern void ResolvePendingGraveyardEquipInt(int player, int graveIdx, int equip) asm("ResolvePendingGraveyardEquip");
/* Matching: this unit calls FindFaceUpCardOnField2 with two arguments (duel.h declares a third, skipZone). */
extern int FindFaceUpCardOnField2Two(int player, u16 cardNo) asm("FindFaceUpCardOnField2");
/* Matching: the destination is a view of the state here (duel.h declares struct DuelCard pointers). */
extern void CopyDuelCardRaw(void *dst, const void *src) asm("CopyDuelCard");
/* Matching: the zone is passed as u8 here (effect.h declares int). */
extern void PlaceNextSpiritMessageU8(int player, u8 zone) asm("PlaceNextSpiritMessage");

/* The card tables of card_data.h through their constant addresses: the ROM loads the table address as a
 * literal and indexes it directly instead of going through the symbol. */
#define CARD_NUMBER_OF(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_ID_OF(number)  (((const u16 *)0x08623DF4)[(number)])              /* gCardNumberToId */
#define CARD_STATS_OF(id)   (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats */

/* Card ID of a field zone in any of the zone views below (the card word's low 12 bits). */
#define ZONE_ID(z)          (((z)->card << 20) >> 20)

/* The battle stage and step fields of gDuel. Adjacent duel steps: the word at +0x1B14 holds the stage in
 * bits 9-16, then the halfword at +0x1B16 holds the step in bits 1-8. The latter is bits 17-24 relative
 * to +0x1B14, not an alias. */
struct BattleStageWord {
    u32 unk0:9;
    u32 stage:8;                    /* battleStage (enum BattleStage) */
    u32 unk17:15;
};
struct BattleStepHalf {
    u16 unk0:1;
    u16 step:8;                     /* battleStep */
    u16 unk9:7;
    u8 pad[6];
};

/* Field zone as the end-of-battle scans read it: the card word and the +0x06 flags byte (bit 1 = face
 * down in these scans; see include/duel.h on the flag naming). */
struct ZoneFlagsView {
    u32 card;                       /* +0x00: struct DuelCard word */
    u8 unk4;
    u8 unk5;
    u8 flags6;                      /* +0x06 */
    u8 unk7[0x94 - 7];
};

/* Stage 11, after the attack: Kotodama on the defender's zone, then the key 1243 control change: if the
 * defender zone (1 - player) holds a card linked by key 1243 and the player has a free monster zone, the
 * card moves over (MoveFieldCard). If key 1538 is present on either side it is announced and the stage
 * ends (return 1); otherwise the stage resets for the next attack (return 0). */
int BattleStage_EndAttack(int player)
{
    int found = 0;
    int opponent, opponentSide;
    u8 *e;
    struct ZoneFlagsView *zn;
    if (Duel_CheckWinInt() != 0)
        return 1;
    opponent = 1 - player;
    ApplyKotodamaToZone(opponent, gBattle.defSlot);
    opponentSide = opponent & 1;
    zn = (struct ZoneFlagsView *)(gBattle.defSlot * 0x94 + opponentSide * 0xD64 + 0x0201930C);
    if ((zn->card << 20) != 0 && CountFreeMonsterZones(player) > 0 && CountZoneLinksFromCard(opponent, gBattle.defSlot, CARD_1243) != 0) {
        int t = opponent;
        u32 pos;
        /* FAKEMATCH: preserve the initialized player copy before narrowing;
         * opponent remains live as the first call argument. Emits no instructions. */
        __asm__("" : "+r"(t));
        pos = (u8)t | gBattle.defSlot << 8;
        MoveFieldCard(opponent, pos, (u8)player | (u8)FindFreeMonsterZone(player) << 8);
    }
    if (CountActiveCardsOnField(player, CARD_1538) != 0 || CountActiveCardsOnField(1 - player, CARD_1538) != 0)
        found = 1;
    if (found == 0) {
        e = gDuelBytes;
        ((struct BattleStageWord *)(e + 0x1B14))->stage = BATTLE_STAGE_SELECT_ATTACKER;
        ((struct BattleStepHalf *)(e + 0x1B16))->step = 0;
        return 0;
    }
    ShowCardEffect(player, gCardNumberToId_1538[0]);
    return 1;
}

/* Views of gDuel for BattleStage_EndBattlePhase: player records (0xD64 bytes) at +4 with the graveyard
 * count at +4 and the graveyard list at +0x904, zones (0x94 bytes) at +0x2C, and the u16 bitfields at
 * +0x1B14 where the battle stage / step / args straddle the halfwords, as agbcc lays out straddling
 * bitfields (byte-split access). */
struct BattleEndZone {
    u32 card;                       /* +0x00: struct DuelCard word */
    u8 unk4[2];
    u16 flags6;                     /* +0x06: isDefense/isFaceUp halfword (byte +0x07 bit 5 = effectUnused) */
    u8 unk8[0x94 - 8];
};
struct BattleEndPlayer {
    u8 unk0[4];
    u8 graveCount;                  /* +0x004 */
    u8 unk5[3];
    u32 unk8_0:6;
    u32 extraBattlePhase:1;         /* +0x008 bit 6 */
    u32 unk8_7:5;
    u32 battlePhaseDone:1;          /* +0x009 bit 4 */
    u32 unk9_5:19;
    u8 unkC[0xD64 - 0xC];
};
struct BattleEndState {
    u8 unk0[0x1B14];
    u16 unk1B14:9;
    u16 battleStage:8;              /* bits 9-16 */
    u16 battleStep:8;               /* bits 17-24: the switch below */
    u16 battleArg0:8;               /* player of the pending graveyard summon */
    u16 battleArg1:8;               /* graveyard index of the pending graveyard summon */
    u16 unk1B19:7;
    u8 unk1B1A[2];
    struct DuelCard battleCard;     /* +0x1B1C: the card to summon */
    u8 unk1B20[0x1B64 - 0x1B20];
    u16 promptResult;               /* +0x1B64: scratch flag of steps 6-7 */
};
#define BATTLE_END_STATE    ((struct BattleEndState *)gDuelBytes)
#define BATTLE_END_PLAYER(p) ((struct BattleEndPlayer *)(gDuelBytes + 4) + ((p) & 1))
#define BATTLE_END_ZONES    (gDuelBytes + 0x2C)
#define BATTLE_END_GRAVE    (gDuelBytes + 0x908)

/* Stage 12, end-of-Battle-Phase effects on the step byte at +0x1B16 (bits 1-8, battleStep): step 0 handles
 * key 1344 in the player's zones and CARD_SWORD_OF_DRAGONS_SOUL for both players, 1 resolves graveyard
 * entries with pendingEquip (card bit 24; the linked zone, key 1327), 2 selects an entry with
 * pendingOpponentSummon (bit 28) and jumps to 5-8 (DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON /
 * DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD, DuelPrompt_Post, CopyDuelCard, QueueSpecialSummonChoosePosition);
 * the default step announces DUEL_CMD_SET_EXTRA_BATTLE_PHASE and resets, or marks the player's
 * battlePhaseDone. Matching notes: the duel state is read through the global (GCSE keeps one copy in r8);
 * the outer loops use i and the inner ones j in every case. */
int BattleStage_EndBattlePhase(int player)
{
    int i, j;
    /* FAKEMATCH: card is first assigned in the zone check (through n, so the compare stays int-wide)
     * and reassigned CARD_SWORD_OF_DRAGONS_SOUL in the inner loop; the earlier first use stops loop.c
     * from folding the constant into the call and its priority puts it in r8 ahead of player. */
    u16 card;
    int n;
    switch (BATTLE_END_STATE->battleStep) {
    case 0:
        for (i = 0; i <= 4; i++) {
            struct BattleEndZone *z = (struct BattleEndZone *)(i * 0x94 + (player & 1) * 0xD64 + BATTLE_END_ZONES);
            u16 id = ZONE_ID(z);
            if (id != 0 && (card = n = CARD_NUMBER_OF((u16)id), n == CARD_1344) && (z->flags6 & 0x2003) == 2) {
                ShowCardEffect(player, id);
                ChangeBattlePosition(player, i, 0, 0);
            }
            for (j = 0; j <= 1; j++) {
                /* FAKEMATCH: the table pointer is set at the top of the body so loop.c hoists it. */
                const u16 *t;
                card = CARD_SWORD_OF_DRAGONS_SOUL;
                t = gCardNumberToId_SwordOfDragonsSoul;
                if (HasZoneCardEffectLink(j, i, card)) {
                    ShowCardEffect(j, t[0]);
                    DestroyFieldCard(j, i, 1);
                }
            }
        }
        BATTLE_END_STATE->battleStep++;
        return 0;
    case 1:
        for (i = 0; i <= 1; i++) {
            for (j = 0; j < BATTLE_END_PLAYER(i)->graveCount; j++) {
                u32 *g = (u32 *)((i & 1) * 0xD64 + BATTLE_END_GRAVE);
                /* FAKEMATCH: the g local keeps (p & 1) * 0xD64 + list in the loop (no strength
                 * reduction) and the int sum puts g first in the add. */
                u32 card = *(u32 *)((int)g + (j << 2));
                if ((s32)(card << 7) < 0) {
                    int q = 1 - i;
                    int qs = q & 1; /* computed before the slot field, as in the ROM */
                    struct BattleEndZone *z = (struct BattleEndZone *)(((card << 4) >> 29) * 0x94 + qs * 0xD64 + BATTLE_END_ZONES);
                    u32 id = ZONE_ID(z);
                    int special = 0;
                    if ((((u8 *)z)[6] & 2) && id && FindFreeSpellTrapZone(q) >= 0 && CARD_NUMBER_OF((u16)id) == CARD_1327)
                        special = 1;
                    ResolvePendingGraveyardEquipInt(i, j, special);
                    return 0;
                }
            }
        }
        BATTLE_END_STATE->battleStep++;
        return 0;
    case 2:
        for (i = 0; i <= 1; i++) {
            /* FAKEMATCH: the comma expression loads the player base first in the loop test, so loop.c
             * hoists it in pass 1 and the inner test keeps its own copy (G - 0x904). */
            u8 *y;
            for (j = 0; j < ((struct BattleEndPlayer *)(y = gDuelBytes + 4, y + (i & 1) * 0xD64))->graveCount; j++) {
                u32 *g = (u32 *)((i & 1) * 0xD64 + BATTLE_END_GRAVE);
                u32 card = g[j];
                if ((s32)(card << 3) < 0) {
                    BATTLE_END_STATE->battleArg0 = i;
                    BATTLE_END_STATE->battleArg1 = j;
                    BATTLE_END_STATE->battleStep = 5;
                    return 0;
                }
            }
        }
        BATTLE_END_STATE->battleStep++;
        return 0;
    case 5:
        if (FindFreeMonsterZone(1 - BATTLE_END_STATE->battleArg0) == -1) {
            int pl = BATTLE_END_STATE->battleArg0;
            u16 msg = DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON;
            if (pl)
                msg = DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, BATTLE_END_STATE->battleArg1, 1, 0);
            BATTLE_END_STATE->battleStep = 2;
            return 0;
        }
        ShowCardEffect(player, gCardNumberToId_1340[0]);
        BATTLE_END_STATE->battleStep++;
        return 0;
    case 6:
        if (BATTLE_END_STATE->battleArg0 && !(gDuelCtrlBytes[1] & 1)) {
            BATTLE_END_STATE->promptResult = 1;
            BATTLE_END_STATE->battleStep = 8;
            return 0;
        }
        DuelPrompt_Post(1 - BATTLE_END_STATE->battleArg0, PROMPT_CONFIRM_SPECIAL_SUMMON, ((struct BattleEndZone *)((BATTLE_END_STATE->battleArg0 & 1) * 0xD64 + (u8 *)BATTLE_END_STATE + 0x908 + BATTLE_END_STATE->battleArg1 * 4))->card << 20 >> 20, 0);
        BATTLE_END_STATE->battleStep++;
        return 0;
    case 7:
        if (BATTLE_END_STATE->promptResult) {
            u16 *card = (u16 *)((BATTLE_END_STATE->battleArg0 & 1) * 0xD64 + (u8 *)BATTLE_END_STATE + 0x908 + BATTLE_END_STATE->battleArg1 * 4);
            int msg = DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD;
            if (BATTLE_END_STATE->battleArg0)
                msg = DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, card[0], card[1], 0);
            CopyDuelCardRaw(&BATTLE_END_STATE->battleCard, card);
            BATTLE_END_STATE->battleStep++;
        } else {
            int pl = BATTLE_END_STATE->battleArg0;
            u16 msg = DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON;
            if (pl)
                msg = DUEL_CMD_CLEAR_PENDING_OPPONENT_SUMMON | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, BATTLE_END_STATE->battleArg1, 1, 0);
            BATTLE_END_STATE->battleStep = 2;
        }
        return 0;
    case 8:
        BATTLE_END_STATE->battleCard.pendingOpponentSummon = 0;
        QueueSpecialSummonChoosePosition(1 - BATTLE_END_STATE->battleArg0, &BATTLE_END_STATE->battleCard, 1, ZONE_STATUS_FROM_GRAVEYARD);
        BATTLE_END_STATE->battleStep = 2;
        return 0;
    default:
        {
            u8 *y = gDuelBytes + 4;
            struct BattleEndPlayer *ps = (struct BattleEndPlayer *)(y + (player & 1) * 0xD64);
            if (ps->extraBattlePhase) {
                u16 msg = DUEL_CMD_SET_EXTRA_BATTLE_PHASE;
                if (player)
                    msg = DUEL_CMD_SET_EXTRA_BATTLE_PHASE | DUEL_CMD_PLAYER;
                DuelCmd_PushU16(msg, 0, 0, 0);
                BATTLE_END_STATE->battleStage = 0;
                BATTLE_END_STATE->battleStep = 0;
                return 0;
            }
            ps->battlePhaseDone = 1;
            return 1;
        }
    }
}

/* Zone view of BattleStage_Cleanup: the card word and the +0x8C flags byte. */
struct CleanupZoneView {
    u32 card;                       /* +0x00: struct DuelCard word */
    u8 unk4[0x8C - 4];
    u8 flags8C;                     /* +0x8C */
    u8 unk8D[0x94 - 0x8D];
};
/* The +0x8C flags byte (struct DuelZone destroy/return-after-battle bits). */
struct ZoneAfterBattleFlags {
    u8 unk8C_0:1;
    u8 destroyAfterBattle:1;        /* bit 1 */
    u8 returnAfterBattle:1;         /* bit 2 */
    u8 unk8C_3:5;
};

/* Stage 13: find the first occupied zone with flag +0x8C bit 1 (destroy it, Magical Hats decoys) or
 * bit 2 (announce DUEL_CMD_SET_RETURN_AFTER_BATTLE and move it to a free zone of the other player, or
 * destroy it when there is none); 1 when no zone is left. */
int BattleStage_Cleanup(int player)
{
    int i = 0, j;
    u8 *zb = (u8 *)0x0201930C;
    for (; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            int side = i & 1;
            struct CleanupZoneView *zn = (struct CleanupZoneView *)(j * 0x94 + side * 0xD64 + (int)zb);
            u8 opp = 1 - i;
            if ((zn->card << 20) != 0) {
                if (((struct ZoneAfterBattleFlags *)&zn->flags8C)->destroyAfterBattle) {
                    DestroyFieldCard(i, j, 1);
                found:
                    return 0;
                }
                if (((struct ZoneAfterBattleFlags *)&zn->flags8C)->returnAfterBattle) {
                    int r = FindFreeMonsterZone(1 - i);
                    u16 msg = DUEL_CMD_SET_RETURN_AFTER_BATTLE;
                    u32 pos;
                    if (player != 0)
                        msg = DUEL_CMD_SET_RETURN_AFTER_BATTLE | DUEL_CMD_PLAYER;
                    pos = (u8)i | (u8)j << 8;
                    DuelCmd_PushU16(msg, pos, 0, 0);
                    if (r >= 0) {
                        MoveFieldCard(player, pos, (u8)r << 8 | opp);
                        goto found;
                    }
                    DestroyFieldCard(i, j, 1);
                    goto found;
                }
            }
        }
    }
    return 1;
}

/* Run one stage handler per call (gBattleStageHandlers[gDuel.battleStage]); 1 after the last stage,
 * when the Main Phase 2 banner is pushed. */
int BattlePhase_Run(int player)
{
    int (*const volatile *table)(int) = gBattleStageHandlers;
    u8 *e = gDuelBytes;
    struct BattleStageWord *stageWord = (struct BattleStageWord *)(e + 0x1B14);
    u32 packed = *(u32 *)stageWord << 15;
    if (table[packed >> 24] != 0) {
        /* FAKEMATCH: keep the initialized shifted stage live across the null
         * check, so the callback index is extracted again. No instructions. */
        __asm__("" : "+r"(packed));
        if ((u16)table[packed >> 24](player) != 0) {
            ((struct BattleStepHalf *)(e + 0x1B16))->step = 0;
            stageWord->stage++;
        }
        return 0;
    } else {
        int flag = e[0x1B12] & 2;
        int msg = DUEL_CMD_MAIN2_PHASE;
        if (flag)
            msg = DUEL_CMD_MAIN2_PHASE | DUEL_CMD_PLAYER;
        DuelCmd_PushU16(msg, 0, 0, 0);
        return 1;
    }
}

void BattlePhase_UnusedNop(void)
{
}

/* Queue a sprite draw of the selected card: the packed first argument depends on how far the duel
 * screen cursor has travelled (DuelScreen +0x810 - +0x04), the scale comes from gPulseScaleCurve
 * indexed by bits 1-4 of the gMain frame counter (+0x485E). */
void DuelScreen_DrawPulseIconOverlay(void)
{
    u8 *e = (u8 *)&gDuelScreen;
    u8 *m;
    const u16 *t;
    int d = *(int *)(e + 0x810) - e[4];
    u32 v = 0x002800A0;
    if (d <= 0x47)
        v = 0x007000A0;
    AddAffineSprite(v, 0x40C0, 0xF364, (t = gPulseScaleCurve, m = (u8 *)&gMain, t[(*(u16 *)(m + 0x485E) >> 1) & 0xF] << 16));
}

/* Duel screen as DuelPhase_Draw reads it (struct DuelScreen in duel_screen.h keeps the same offsets):
 * the +0x808 bit 3 flag and the word at +0x85C. */
struct DuelScreenView {
    u8 unk0[0x808];
    u8 unk808_0:3;
    u8 showCursor:1;                /* +0x808 bit 3 */
    u8 unk808_4:4;
    u8 unk809[0x85C - 0x809];
    u32 overlayCallback;            /* +0x85C: DuelScreen.overlayCallback, cleared as a word */
};

/* The duel flags byte at gDuel +0x1B12 as DuelPhase_Draw reads it, reached from the player base. */
struct DuelFlagsByte {
    u8 bgmOn:1;                     /* bit 0 */
    u8 turnPlayer:1;                /* bit 1: player whose turn it is */
    u8 unk2:6;
    u8 pad[7];
};

/* Duel step 3, the Draw Phase (enum DrawPhaseStep), on the step byte at 0x020192E4+0x1B1C
 * (0x0201AE00 = gDuel.phaseStep), keyed on the turn player (bit 1 of the duel flags byte at
 * 0x020192E4+0x1B0E). If the turn player's skipDrawPhase flag is set, it is cleared and the routine ends
 * (returns 1). Otherwise step 0 sends DUEL_CMD_DRAW_PHASE, step 1 finishes with DrawCards(1, 1) for the
 * CPU or runs DuelCursor_Select(who, DUEL_AREA_DECK, 0) for the human, step 2 sets the screen showCursor
 * bit, step 4 clears the screen word at +0x85C and finishes with DrawCards(0, 1). Other steps advance
 * on R_BUTTON when DuelScreen_HandleInput() is 0. Both player indexes are written `who & 1`: CSE shares
 * the constant 1, and combine folds the AND only in the first block, as in the ROM. The early return in
 * the default case keeps the per-case step increments from being cross-jumped. */
int DuelPhase_Draw(void)
{
    u8 *e4 = (u8 *)gDuelPlayers;
    struct DuelFlagsByte *sb = (struct DuelFlagsByte *)(e4 + 0x1B0E);
    u8 *st;
    int z = gDuelPlayers[sb->turnPlayer & 1].skipDrawPhase;
    if (z != 0) {
        gDuelPlayers[sb->turnPlayer & 1].skipDrawPhase = 0;
        return 1;
    }
    st = e4 + 0x1B1C;
    switch (*st) {
    case 0:
        DuelCmd_PushU16((*(u8 *)sb & 2) ? DUEL_CMD_DRAW_PHASE | DUEL_CMD_PLAYER : DUEL_CMD_DRAW_PHASE, 0, 0, 0);
        (*st)++;
        return 0;
    case 1:
        if ((*(u8 *)sb & 2) != 0) {
            DrawCards(1, 1);
            return 1;
        }
        DuelCursor_Select(sb->turnPlayer, DUEL_AREA_DECK, 0);
        (*st)++;
        return 0;
    case 2:
        ((struct DuelScreenView *)&gDuelScreen)->showCursor = 1;
        (*st)++;
        return 0;
    case 4:
        ((struct DuelScreenView *)&gDuelScreen)->overlayCallback = 0;
        DrawCards(0, 1);
        return 1;
    default:
        if (DuelScreen_HandleInput() != 0 || (gMain.newKeys & R_BUTTON) == 0)
            return 0;
        gDuelBytes[0x1B20]++;
        return 0;
    }
}

/* For every monster zone (0-4) of `player` holding a face-down card (flags6 bit 1) whose card number is
 * CARD_THE_WICKED_WORM_BEAST: send DUEL_CMD_SHOW_CARD_EFFECT and run ReturnFieldCardToHand on it. */
void EndPhase_ReturnWickedWormBeast(int player)
{
    int i;
    for (i = 0; i <= 4; i++) {
        int s1 = i * 0x94 + (player & 1) * 0xD64;
        struct ZoneFlagsView *zn = (struct ZoneFlagsView *)(s1 + 0x0201930C);
        u32 id = ZONE_ID(zn);
        u32 n = id;
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if (CARD_NUMBER_OF((u16)id) == CARD_THE_WICKED_WORM_BEAST) {
                u32 msg = DUEL_CMD_SHOW_CARD_EFFECT;
                if (player != 0)
                    msg = DUEL_CMD_SHOW_CARD_EFFECT | DUEL_CMD_PLAYER;
                DuelCmd_PushU16(msg, n, 1, 0);
                ReturnFieldCardToHand(player, i, 0);
            }
        }
    }
}

/* Mushroom Man #2 control transfer, a three-step routine on the step byte 0x020192E0+0x1B22 with a zone
 * counter at +0x1B23. Step 0: needs the player's life points (players +0x000) > 0x1F3 and a free monster
 * zone for the opponent, then scans zones counter..4 for a face-down Mushroom Man #2 (advancing the
 * step, twice for player 1 which also arms the text box answer); step 1 prints the card-name prompt;
 * step 2 sends DUEL_CMD_LOSE_LP (500 LP) and MoveFieldCard for the found zone, then advances the counter
 * and finally the step.
 * The loop reads the duel state through the global (not the local e) and recomputes player & 1 at the top
 * of its body: loop.c then hoists the constant 1 first (it later serves the result store) and CSE keeps
 * the step value across the gTextBox.result store. */
int EndPhase_TransferMushroomMan2(int player)
{
    char buf[0x80];
    u8 *e = gDuelBytes;
    u8 *st = e + 0x1B22;
    switch (*st) {
    case 0: {
        u8 *pe = e + 4;
        s16 base = (player & 1) * 0xD64;
        if (*(u16 *)(base + (int)pe) <= 0x1F3)
            return 1;
        if (CountFreeMonsterZones(1 - player) == 0)
            return 1;
        for (; gDuelBytes[0x1B23] <= 4; gDuelBytes[0x1B23]++) {
            int side = player & 1;
            int s1 = gDuelBytes[0x1B23] * 0x94 + side * 0xD64;
            struct ZoneFlagsView *zn = (struct ZoneFlagsView *)(s1 + (int)(gDuelBytes + 0x2C));
            u16 id = ZONE_ID(zn);
            if (id != 0 && (zn->flags6 & 2) != 0 && CARD_NUMBER_OF(id) == CARD_MUSHROOM_MAN_2) {
                gDuelBytes[0x1B22]++;
                if (player != 0) {
                    gTextBox.result = 1;
                    gDuelBytes[0x1B22]++;
                }
                return 0;
            }
        }
        return 1;
    }
    case 1:
        FormatStr(buf, gStrAskTransferControlFmt, gCardNames + (gCardNumberToId_MushroomMan2[0] << 6));
        TextBoxOpen(0x206, 0x712, TEXTBOX_FLAGS_DEFAULT, buf);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
        (*st)++;
        return 0;
    case 2:
        if (gTextBox.result != 0) {
            u32 msg = DUEL_CMD_LOSE_LP;
            if (player != 0)
                msg = DUEL_CMD_LOSE_LP | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, 500, 1, 0);
            MoveFieldCard(player, (u8)player | e[0x1B23] << 8, (u8)(1 - player) | (u8)FindFreeMonsterZone(1 - player) << 8);
        }
        {
            u8 *g = gDuelBytes;
            (*(g + 0x1B23))++;
            if (*(g + 0x1B23) <= 4)
                *(g + 0x1B22) = 0;
            else
                (*(g + 0x1B22))++;
        }
        return 0;
    default:
        return 1;
    }
}

/* The level of a card from its gCardStats word: Trap/Magic/Ticket count as 0, Divine as 10. */
static inline int CardLevelOf(u16 id)
{
    switch ((int)((CARD_STATS_OF(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return (CARD_STATS_OF(id) & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT;
    }
}

/* Zone view of EndPhase_DestroyLowLevelMonsters: the card word, the +0x06 flags byte and the +0x08
 * byte (bit 0 = levelCheckDone). */
struct ZoneLevelView {
    u32 card;                       /* +0x00: struct DuelCard word */
    u8 unk4[2];
    u8 flags6;                      /* +0x06 */
    u8 unk7;
    u8 flags8;                      /* +0x08 */
    u8 unk9[0x94 - 9];
};

/* Two passes over the face-down monster zones of `player`: the first sets `found` when a monster of
 * level <= 3 with the +0x08 level-checked bit clear exists (cleared again unless CARD_1322 is present
 * on either side; then DUEL_CMD_SHOW_CARD_EFFECT is sent), the second destroys such monsters
 * (DestroyFieldCard) when found, and sends DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG for each other face-down
 * zone.
 * Zone addresses go through the symbol gDuelZones so GCSE keeps one copy of it in sl. Loop 1 computes
 * the zone address twice, player term first: loop.c hoists the 0x94 only in its second pass, so the
 * zone pointer is strength-reduced while i still counts up (a hand-stepped pointer gets a reversed
 * counter). */
void EndPhase_DestroyLowLevelMonsters(int player)
{
    int found = 0;
    int i;
    for (i = 0; i <= 4; i++) {
        struct ZoneLevelView *zn = (struct ZoneLevelView *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones);
        u32 id = ZONE_ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)CardLevelOf(id) <= 3 && (((struct ZoneLevelView *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones))->flags8 & 1) == 0)
                found = 1;
        }
    }
    if (CountActiveCardsOnField(0, CARD_1322) == 0 && CountActiveCardsOnField(1, CARD_1322) == 0)
        found = 0;
    if (found != 0) {
        u16 msg = DUEL_CMD_SHOW_CARD_EFFECT;
        if (player != 0)
            msg = DUEL_CMD_SHOW_CARD_EFFECT | DUEL_CMD_PLAYER;
        DuelCmd_PushU16(msg, gCardNumberToId_1322[0], 1, 0);
    }
    for (i = 0; i <= 4; i++) {
        struct ZoneLevelView *zn = (struct ZoneLevelView *)(i * 0x94 + (player & 1) * 0xD64 + (int)gDuelZones);
        u16 id = ZONE_ID(zn);
        if (id != 0 && (zn->flags6 & 2) != 0) {
            if ((u32)CardLevelOf(id) <= 3 && (((struct ZoneLevelView *)((player & 1) * 0xD64 + i * 0x94 + (int)gDuelZones))->flags8 & 1) == 0 && found != 0) {
                DestroyFieldCard(player, i, 1);
            } else {
                u32 msg = DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG;
                if (player != 0)
                    msg = DUEL_CMD_SET_ZONE_LEVEL_CHECK_FLAG | DUEL_CMD_PLAYER;
                DuelCmd_PushU16(msg, i, 1, 0);
            }
        }
    }
}

/* Views of the field and the players as DuelPhase_End reads them. The zone view flattens the links
 * area (+0x0A..+0x90) into one array; the player view reads the graveyard as raw card words and keeps
 * the +0x0C bit 4 flag in a signed container (the code tests it with < 0). */
struct EndPhaseZone {
    u32 card;                       /* +0x00: struct DuelCard word */
    u8 unk4;
    u8 unk5;
    u8 flags6;                      /* +0x06 */
    u8 unk7;
    u16 unk8;
    u16 links[0x43];                /* +0x0A: links[32], linkKinds[32] and numLinks as one array */
    u8 unk90;
    u8 flags91;                     /* +0x91: bit 3 = isDisabled */
    u8 unk92[2];
};
struct EndPhasePlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 unk3;
    u8 graveCount;                  /* +0x004 */
    u8 unk5[7];
    u8 unkC_0:4;
    s8 destroyedTriggerPending:1;   /* +0x00C bit 4, signed: tested with < 0 */
    u8 unkC_5:3;
    u8 unkD[0x904 - 0xD];
    u32 graveyard[(0xD64 - 0x904) / 4]; /* +0x904: struct DuelCard words */
};
struct EndPhaseState {
    u32 unk0;
    struct EndPhasePlayer players[2];
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 flags1B12;                   /* +0x1B12: bit 1 = turnPlayer */
    u8 unk1B13[0x1B20 - 0x1B13];
    u8 phaseStep;                   /* +0x1B20: enum EndPhaseStep */
    u8 phaseCounter;                /* +0x1B21: zone cursor */
    u8 phaseSubStep;                /* +0x1B22 */
    u8 phaseSubCounter;             /* +0x1B23 */
    u8 unk1B24[0x1B64 - 0x1B24];
    u16 promptResult;               /* +0x1B64 */
};
#define END_STATE           ((struct EndPhaseState *)gDuelBytes)
#define END_STEP            (END_STATE->phaseStep)
#define END_ZONE            (END_STATE->phaseCounter)
/* Matching: gDuelPlayers indexed through a struct that holds the array, so its base is loaded before
 * the index is computed (ROM order; a plain array index loads the base last). */
struct EndPhasePlayerPair {
    struct EndPhasePlayer players[2];
};
#define END_PLAYERS         (((struct EndPhasePlayerPair *)gDuelPlayers)->players)

/* Card number -> card ID (gCardNumberToId through its constant address); alternate-art numbers
 * (CARD_NUMBER_ALT_ART + n) map to the ID of n + 1. */
static inline u16 CardIdFromNumber(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= CARD_NUMBER_ALT_ART - 1) return CARD_ID_OF(number & CARD_ID_MASK);
    return CARD_ID_OF((number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK) + 1;
}

/* Duel step 6, the End Phase dispatcher (enum EndPhaseStep) for the player in flags bit 1 (turnPlayer):
 * announces and resolves end-of-phase card effects (linked-zone destruction for CARD_1548, the
 * opponent's CARD_SWORDS_OF_REVEALING_LIGHT / CARD_1230 / CARD_1528 zones, the CARD_1519 prompt,
 * graveyard pendingEquip notices, the destroyedTriggerPending events for both players), then trims the
 * hand to six (returns 1 when done).
 * Case 10 uses three FAKEMATCH forms, explained at each site: an opaque zone base `zb` from
 * which step++ is formed, a dead product that sets loop.c's insn count (so it hoists
 * player & 1 in its first pass and 0x94, 0xD64 and the side product in its second, as in
 * the ROM), and the table pointer at the top of the loop body (hoisted, left without a hard
 * register and reloaded, which keeps the ROM's reload-register rotation). */
int DuelPhase_End(void)
{
    u32 player = ((u32)gDuelBytes[0x1B12] << 30) >> 31;
    u32 kind;
    switch (gDuelBytes[0x1B20]) {
    case END_STEP_ENTER: {
        DuelCmd_PushU16(player ? DUEL_CMD_END_PHASE | DUEL_CMD_PLAYER : DUEL_CMD_END_PHASE, 0, 0, 0);
        gDuelBytes[0x1B22] = 0;
        gDuelBytes[0x1B23] = 0;
        gDuelBytes[0x1B20]++;
    }
    case END_STEP_MUSHROOM_MAN_2:
        if ((u16)EndPhase_TransferMushroomMan2(player)) {
            u16 msg = DUEL_CMD_SET_EXTRA_BATTLE_PHASE;
            if (player) msg = DUEL_CMD_SET_EXTRA_BATTLE_PHASE | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, 0, 0, 0);
            EndPhase_ReturnWickedWormBeast(player);
            END_STEP++;
        }
        return 0;
    case END_STEP_LOW_LEVEL_CHECK:
        EndPhase_DestroyLowLevelMonsters(player);
        END_STEP++;
        return 0;
    case END_STEP_OPPONENT_LINKS: {
        int i;
        for (i = 0; i <= 4; i++) {
            int other = 1 - player;
            struct EndPhaseZone *z = (struct EndPhaseZone *)(i * 0x94 + (other & 1) * 0xD64 + (u32)gDuelZones);
            if (ZONE_ID(z) && (z->flags6 & 2)) {
                int idx = FindZoneLinkFromCard(player, i, CARD_1548);
                if (idx >= 0) {
                    u16 link = ((struct EndPhaseZone *)((player & 1) * 0xD64 + i * 0x94 + (u32)gDuelZones))->links[idx];
                    u8 who = link;
                    u32 slot = link >> 8;
                    struct EndPhaseZone *row = (struct EndPhaseZone *)((who & 1) * 0xD64 + (u32)gDuelZones);
                    struct EndPhaseZone *lz = &row[slot];
                    if ((lz->flags6 & 0x3C) == 8 && !(lz->flags91 & 8)) {
                        ReturnFieldCardToHand(who, slot, 0);
                        DestroyFieldCard(other, i, 1);
                        OnCardDestroyedByEffect(player, other, i);
                    }
                }
            }
        }
        END_STEP++;
        END_ZONE = 5;
        return 0;
    }
    case END_STEP_SPELL_TRAP_COUNTERS:
        for (; END_ZONE <= 9; END_ZONE++) {
            int other = 1 - player;
            int side = other & 1;
            struct EndPhaseZone *z = (struct EndPhaseZone *)(END_ZONE * 0x94 + side * 0xD64 + gDuelBytes + 0x2C);
            u32 id = ZONE_ID(z);
            u8 f;
            if (id && ((f = z->flags6) & 2)) {
                switch (CARD_NUMBER_OF(id)) {
                case CARD_SWORDS_OF_REVEALING_LIGHT:
                    if (((u32)f << 26) >> 28 <= 1) {
                        u16 msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER;
                        if (player != 1) msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER | DUEL_CMD_PLAYER;
                        DuelCmd_PushU16(msg, END_ZONE, 1, 0);
                    } else
                        DestroyFieldCard(other, END_ZONE, 1);
                    END_ZONE++;
                    return 0;
                case CARD_1230:
                    if (!(f & 0x3C)) {
                        u16 msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER;
                        /* FAKEMATCH: f outlives the AND, so regmove gives its result the
                         * 0x3C register (`ands r0, r3`) instead of f's. */
                        asm("" :: "r"(f));
                        if (player != 1) msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER | DUEL_CMD_PLAYER;
                        DuelCmd_PushU16(msg, END_ZONE, 1, 0);
                    } else
                        DestroyFieldCard(other, END_ZONE, 1);
                    END_ZONE++;
                    return 0;
                case CARD_1528:
                    if (FindFreeSpellTrapZone(other) >= 0
                        && !(((struct EndPhaseZone *)(side * 0xD64 + END_ZONE * 0x94 + gDuelBytes + 0x2C))->flags91 & 8)) {
                        u16 msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER;
                        if (player != 1) msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER | DUEL_CMD_PLAYER;
                        DuelCmd_PushU16(msg, END_ZONE, 1, 0);
                        PlaceNextSpiritMessageU8(other, END_ZONE);
                        END_ZONE++;
                        return 0;
                    }
                    break;
                }
            }
        }
        END_STEP++;
        return 0;
    case END_STEP_KEY1519_ASK: {
        int other = 1 - player;
        if (CountFaceUpMonstersByNumber(other, CARD_1519) && CountMonstersFiltered(player, 1, 0) > 0) {
            DuelPrompt_Post(other, PROMPT_CONFIRM_CARD_EFFECT, CARD_1519, 0);
            END_STEP++;
        } else
            END_STEP = END_STEP_OWN_LINKS;
        return 0;
    }
    case END_STEP_KEY1519_ANSWER:
        if (END_STATE->promptResult) {
            int other = 1 - player;
            u32 kind;
            int index = FindFaceUpCardOnField2Two(other, CARD_1519);
            Chain_AddPending(((u32)(other & 1) << 31) | (((index & 31) << 16) | (kind = 0x6400000)) | CardIdFromNumber(CARD_1519), 0);
        }
        END_STEP = END_STEP_OWN_LINKS;
        return 0;
    case END_STEP_OWN_LINKS:
        END_ZONE = 0;
        do {
            /* Declared at the top of the body: loop.c hoists the table address, it gets no
             * hard register and is reloaded at the call (keeps the reload rotation). */
            const u16 *t = gCardNumberToId_1548;
            if (CountZoneLinksFromCard(player, END_ZONE, CARD_1548)) {
                u16 idx = FindZoneLinkFromCard(player, END_ZONE, CARD_1548);
                int side = player & 1;
                u32 off = END_ZONE * 0x94 + side * 0xD64;
                u8 *zb = gDuelBytes + 0x2C;
                u16 link;
                u32 who, slot, wb;
                struct EndPhaseZone *lz;
                /* FAKEMATCH: zb is opaque, so step++ below is formed as (base+0x2C)+0x1AF4
                 * and stays in the loop; otherwise CSE relates it to the loop-top symbol
                 * and loop.c hoists it. */
                asm("" : "+r"(zb));
                link = ((struct EndPhaseZone *)(off + (u32)zb))->links[idx];
                who = (u8)link;
                slot = link >> 8;
                wb = who & 1;
                lz = (struct EndPhaseZone *)(slot * 0x94 + wb * 0xD64 + (u32)zb);
                if (!(lz->flags91 & 8)) {
                    if (!(lz->flags6 & 0x3C)) {
                        /* FAKEMATCH: dead product, deleted by flow before register allocation;
                         * it only raises loop.c's insn count to the ROM's (127 insns). */
                        u16 msg = who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who*who;
                        msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER;
                        if (who) msg = DUEL_CMD_ADD_ZONE_TURN_COUNTER | DUEL_CMD_PLAYER;
                        DuelCmd_PushU16(msg, slot, 1, 0);
                    } else {
                        ShowCardEffect(player, t[0]);
                        ReturnFieldCardToHand(who, slot, 0);
                        ((struct EndPhaseState *)(zb - 0x2C))->phaseStep++;
                        return 0;
                    }
                }
            }
            END_ZONE++;
        } while (END_ZONE <= 4);
        END_STEP = END_STEP_GRAVEYARD_RETURNS;
        return 0;
    case END_STEP_DESTROY_LINKED:
        DestroyFieldCard(player, END_ZONE, 1);
        END_ZONE++;
        END_STEP = END_STEP_OWN_LINKS;
        return 0;
    case END_STEP_GRAVEYARD_RETURNS: {
        int i;
        for (i = 0; i < END_PLAYERS[player & 1].graveCount; i++) {
            u32 *row = (u32 *)((player & 1) * 0xD64 + (u32)gDuelGraveyardWords);
            u32 *p = row + i;
            u32 card = *p;
            if ((s32)(card << 8) < 0) {
                u16 msg = DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND;
                if (player) msg = DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND | DUEL_CMD_PLAYER;
                DuelCmd_PushU16(msg, card, card >> 16, 0);
            }
        }
        END_STEP++;
        return 0;
    }
    case END_STEP_DESTROYED_TRIGGER_SELF:
        if (END_PLAYERS[player & 1].destroyedTriggerPending < 0) {
            u16 msg = DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING;
            if (player) msg = DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, 0, 0, 0);
            if (CountMonstersFiltered(1 - player, 1, 0) > 0) {
                const u16 *t = gCardNumberToId_1514;
                Chain_AddPending(((player & 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        END_STEP++;
        return 0;
    case END_STEP_DESTROYED_TRIGGER_OPPONENT:
        if (END_PLAYERS[player ^ 1].destroyedTriggerPending < 0) {
            u16 msg = DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING;
            if (player != 1) msg = DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING | DUEL_CMD_PLAYER;
            DuelCmd_PushU16(msg, 0, 0, 0);
            if (CountMonstersFiltered(player, 1, 0) > 0) {
                const u16 *t = gCardNumberToId_1514;
                Chain_AddPending(((player ^ 1) << 31) | (kind = 0x26600000 | t[0]), 0);
            }
        }
        END_STEP++;
        return 0;
    default:
        if (CountActiveCardsOnField(0, CARD_1427) <= 0 && CountActiveCardsOnField(1, CARD_1427) <= 0) {
            if (END_PLAYERS[player].handCount > 6)
                DuelPrompt_PostDiscard(player, END_PLAYERS[player].handCount - 6, 0, 0);
        }
        return 1;
    }
}
