/* card_menu_input (0x0804A008-0x0804B63F)
 *
 * Card-menu availability and duel-screen input for the duel, plus the Battle Phase attack
 * declaration step machine: which commands a selected card offers (CardMenu_GetAvailableCommands),
 * opening the command menu from the duel screen (DuelScreen_HandleInput), and the stage handlers
 * that pick an attacker and a target (BattleStage_Start, BattleStage_SelectAttacker,
 * BattleStage_SelectTarget) with the replay check (Battle_CheckReplay) in between.
 * (wiki/functions/card-menu-input-c.md)
 */
#include "global.h"
#include "card_data.h"           /* gCardStats / gCardIdToNumber card database tables, CARD_ID_MASK */
#include "constants/card_stats.h" /* CARD_STATS_* stats-word bit fields, enum CardType, enum CardAttribute */
#include "constants/cards.h"     /* CARD_* card numbers */
#include "constants/duel.h"      /* enum DuelPhase, enum DuelArea, CARDMENU_MASK_*, enum BattleStage */
#include "constants/duel_cmds.h" /* DUEL_CMD_* duel command ids */
#include "constants/sound.h"     /* SE_* sound effect ids */
#include "legacy/gba.h"                 /* B_BUTTON */
#include "legacy/main.h"                /* struct Main, gMain */

/* ---- BEGIN duel.h stand-in (pre-H0) --------------------------------------
 * include/duel.h cannot be included yet: several of its prototypes conflict with the
 * width-sensitive local declarations this unit has to keep (see
 * build/readability/issues/card_menu_input.md). Until milestone H0 makes the shared
 * headers authoritative, this block carries the part of duel.h the unit uses, with
 * field layouts copied from it.
 * ------------------------------------------------------------------------- */
#define GUARD_DUEL_H

/* struct DuelZone (duel.h): one duel zone. Kept as this unit's word/byte view: the card is
 * read as one packed word and the +0x06/+0x07 flag bytes and the +0x8C/+0x91 bytes are
 * reached directly, where the canonical struct reads struct DuelCard and named bitfields
 * inside halfwords. */
struct DuelZone {
    u32 card;                       /* +0x00: packed card word (struct DuelCard in duel.h) */
    u8 serialLo;                    /* +0x04: serial low byte */
    u8 serialHi;                    /* +0x05: serial high byte */
    u8 isFaceDown : 1;              /* +0x06 bit 0: face-down (this unit's use; duel.h notes the
                                     * meaning is not settled across units) */
    u8 unk6_1 : 7;                  /* +0x06 bits 1-7: bit 1 is the face-up mark */
    u8 flags7;                      /* +0x07: bit 2 = attacked this turn (MarkMonsterAttacked),
                                     * bit 4 blocks attacking, bit 5 = replay flag */
    u8 unk8[0x8C - 8];
    u8 unk8C;                       /* +0x8C: bits 3-4 block attacking */
    u8 unk8D[0x91 - 0x8D];
    u8 unk91_0 : 1;                 /* +0x91 */
    u8 unk91_1 : 1;
    u8 unk91_2 : 1;
    u8 isDisabled : 1;              /* +0x91 bit 3: cannot attack while set */
    u8 unk91_4 : 4;
    u8 unk92[2];
};

/* struct ZoneCardStats (duel.h): stats of the card in a zone, filled by GetZoneCardStats. */
struct ZoneCardStats {
    u16 id;                         /* +0x00 */
    u8 type : 5;                    /* +0x02 bits 0-4: enum CardType */
    u8 attribute : 3;               /* +0x02 bits 5-7: enum CardAttribute */
    u8 unk3;                        /* +0x03 */
    s32 atk;                        /* +0x04 */
    s32 def;                        /* +0x08 */
};

/* Field pick-mask bits (duel.h, enum FieldPickMask): which zones DuelCursor_PickTarget accepts. */
#define PICK_FACE_UP_MONSTER    0x20 /* face-up monster */
#define PICK_ATTACK_POSITION    0x40 /* attack position */
#define PICK_DEFENSE_POSITION   0x80 /* defense position */
#define PICK_ANY_MONSTER        0xF0 /* any monster (all four bits) */
/* Place a mask in the addressed player's bits (player 0's zones are the low fields). */
#define PICK_PLAYER1(mask)      ((mask) << 16)              /* player 1 */

/* Sound effect player (sound.h has the canonical u32 form; this unit calls it with narrowed
 * u16 ids, the form its call sites match with). */
void PlaySE(u16 se);
/* ---- END duel.h stand-in ---- */

/* One player's duel state and zones as the fused 0xD64-stride block at gDuelPlayers
 * (duel.h keeps the head as struct DuelPlayer and the zones in gDuelZones; the ROM block
 * holds both, which is the form this unit matches with). */
struct DuelPlayerZones {
    u16 lifePoints;                 /* +0x00 */
    u8 unk2[6];
    u8 unk8;                        /* +0x08 */
    u8 unk9;                        /* +0x09 */
    u8 unkA[0x24 - 10];
    u16 attackableMask;             /* +0x24: zones that may attack this turn */
    u16 attackedMask;               /* +0x26: zones that have attacked this turn */
    struct DuelZone zones[11];
    u8 filler[0xD64 - 0x28 - 11 * 0x94];
};
extern struct DuelPlayerZones gDuelPlayers[2]; /* 0x020192E4 */
/* Duel screen cursor state (duel_screen.h struct DuelScreen). This unit keeps u32
 * selection fields where the canonical struct has s32, the form its call sites match with. */
struct DuelScreenView { u8 unk0[0x824]; u32 selPlayer; u32 selArea; u32 selIndex; };
extern struct DuelScreenView gDuelScreen; /* 0x0201CFB0 */
/* Scratch block handed to EffectPolymerizationPrepare (its entry argument); only the
 * bit at +0x02 is cleared before the call. */
struct PolymerizationScratch { u8 unk0[2]; u8 unk2_0 : 1; u8 rest : 7; u8 unk3[0x11]; };
/* ---- Cross-unit declarations ----
 * Local declaration forms kept because the shared headers declare several of these with
 * different argument widths (see build/readability/issues/card_menu_input.md). */
u16 DuelCursor_GetCardId(void);
int IsCardProhibited(u16 id);
int GetFaceUpFieldMagicNumber(void);
int GetCardSpellSpeed(u16 id);
int EffectPolymerizationPrepare(struct PolymerizationScratch *entry, int chainLink, int fromHand);
int CardMenu_GetHandCardCommands(u16 id, int a, int zone);
int CardMenu_GetMonsterCommands(u16 id, int a, int zone);
int CardMenu_GetSpellTrapCommands(u16 id, int a, int zone);
int DuelScreen_HandleInput(void);
void CardMenu_Update(void);
void CardMenu_Execute(void);
u32 DuelCursor_PickAny(void);
u16 CardMenu_GetAvailableCommands(void);
int IsHandRevealed(int player);
void CardListView_Open(int player, int row, int a, int b);
/* gBattle (0x02018450), the current battle (battle.h struct Battle). The ROM code here
 * stores the flags through byte-wide accesses and reads the slot fields as one halfword,
 * so the unit keeps a byte view and a halfword view instead of the canonical bitfields.
 * The trailing pads keep the access code on the load/store widths the ROM uses. */
struct BattleB {
    u8 attacker : 1;                /* bit 0 */
    u8 direct : 1;                  /* bit 1: direct attack */
    u8 flipEffectPending : 1;       /* bit 2 */
    u8 attackDeclared : 1;          /* bit 3 */
    u8 attackCostsPaid : 1;         /* bit 4 */
    u8 zeroAttackerAtk : 1;         /* bit 5 */
    u8 atkSlotLow : 2;              /* bits 6-7 (atkSlot bits 0-1) */
    u8 atkSlotHigh : 1;             /* bit 8 (atkSlot bit 2) */
    u8 defSlot : 3;                 /* bits 9-11 */
    u8 unk1_4 : 4;
    u8 pad[0x20];
};
struct BattleH {
    u16 unk0_0 : 6;
    u16 atkSlot : 3;                /* bits 6-8: attacking monster zone */
    u16 defSlot : 3;                /* bits 9-11: defending monster zone */
    u16 unk0_12 : 4;
    u8 pad[0x20];
};
extern struct BattleH gBattle; /* 0x02018450 */
#define BTB (*(struct BattleB *)&gBattle)
#define BTH (*(struct BattleH *)&gBattle)
/* Halfword fields past the flag word: +0x148 monsterCountSnapshot, +0x14C zoneSerialSnapshot. */
#define BT_U16(off) (*(u16 *)((u8 *)&gBattle + (off)))
/* The gDuel word at +0x1B14: bits 9-16 hold the battle stage. */
struct BattleStageWord {
    u32 unk0 : 9;
    u32 battleStage : 8;            /* enum BattleStage */
    u32 unk17 : 15;
};
#define gStageWord (*(struct BattleStageWord *)((u8 *)&gDuel + 0x1B14))
/* Card stats word by id. Matching: the literal table address is reloaded at every use in
 * the ROM; indexing the gCardStats symbol instead would change the code. */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE_OF(id) ((CARD_STATS_WORD(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)
/* Level-like value into v: 0 for Trap/Magic/Ticket, 10 for Divine, else the level bits. */
#define CARD_LEVEL(v, id)                                                 \
    switch ((int)CARD_TYPE_OF(id)) {                                      \
    case CARD_TYPE_TRAP:                                                  \
    case CARD_TYPE_MAGIC:                                                 \
    case CARD_TYPE_TICKET:                                                \
        v = 0;                                                            \
        break;                                                            \
    case CARD_TYPE_DIVINE:                                                \
        v = 10;                                                           \
        break;                                                            \
    default:                                                              \
        v = (CARD_STATS_WORD(id) & CARD_STATS_LEVEL_MASK) >> CARD_STATS_LEVEL_SHIFT; \
        break;                                                            \
    }
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);
int CountZoneLinksFromCard(int player, int zone, u16 number);
int CountActiveZoneLinksFromCard(int player, int zone, u16 number);
int CountTributableMonsters(int player, int zone);
int CountMonsters(int player);
u16 IsTypeForbiddenToAttack(u16 type);
/* The card-menu widget at gDuel +0x1B2C (duel.h struct CardMenu). Field names follow the
 * canonical struct; summonSeq here spans the canonical summonSeq and tributeSources
 * bitfields, which this unit never splits. */
struct CardMenuView {
    u16 open : 1;
    u16 confirmed : 1;
    u16 command : 4;
    u16 slide : 4;
    u32 available : 16;
    u32 state : 8;
    u32 step : 8;
    u32 summonSeq : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 area : 7;
    u32 index : 8;
    u32 placeZone : 23;
};
/* This unit's view of gDuel, the duel global state (duel.h struct DuelState). */
struct DuelStateView {
    u8 unk0[0xC];
    u8 unk0C;                       /* +0x0C (player 0's +0x08 byte): bit 6 picks the Extra
                                     * end-of-Battle-Phase menu text */
    u8 unkD[0x1B10 - 0xD];
    u16 unk1B10;                    /* +0x1B10 */
    u8 flags1B12;                   /* +0x1B12: bit 1 = turnPlayer, bits 2-4 = phase
                                     * (enum DuelPhase) */
    u8 unk1B13[3];
    u16 unk1B16_0 : 1;              /* +0x1B16 */
    u16 battleStep : 8;             /* +0x1B16 bits 1-8: step inside the battle stage */
    u16 unk1B16_9 : 7;
    u8 unk1B18[0x1B26 - 0x1B18];
    u8 flag1B26_0 : 1;              /* +0x1B26 bit 0: end-Battle-Phase answer flag */
    u8 f1B26_1 : 7;
    u8 unk1B27[0x1B2C - 0x1B27];
    struct CardMenuView cardMenu;   /* +0x1B2C */
};
/* Duel command queue push (duel_cmd.h declares (u16, u16, int, int); this unit's commands
 * are pushed through the all-u16 form its call sites match with). */
void DuelCmd_Push(u16 msg, u16 a, u16 b, u16 c);
/* The battle-step halfword at gDuel +0x1B16. Matching: the pad keeps accesses on the
 * halfword form the ROM uses. */
struct BattleStepHalf { u16 unk0 : 1; u16 step : 8; u16 unk9 : 7; u8 pad[0x20]; };
void DuelCursor_Select(int player, int a, int b);
/* Text box (text_box.h declares u16-wide arguments; these u32 forms are the ones this
 * unit's calls match with). The flags argument is always 0xB (TEXTBOX_FLAGS_DEFAULT). */
void TextBoxOpen(u32 a, u32 b, u32 c, const void *d);
void TextBoxSetMenu(u32 a, u32 b, u32 c);
int AiPlanAttack(int a);
void PhaseMenu_DrawCursor(void);
void PhaseMenu_HandleInput(void);
extern const u8 gStrEndBattlePhaseMenuExtra[]; /* 0x08085878 */
extern const u8 gStrEndBattlePhaseMenu[];      /* 0x080858E4 */
/* AI workspace (ai.h struct AiWork). Byte view of bestAttack (+0xC, struct AttackPlan):
 * attackerZone is the planned attacking monster zone. The halfword view of the same plan
 * is struct AttackPlanView below. */
struct AiWorkView { u8 unk0[0xC]; u8 unkC_0 : 4; u8 attackerZone : 3; u8 unkC_7 : 1; u8 pad[0x20]; };
extern struct AiWorkView gAiWork; /* 0x02015F00 */
/* Text box state (text_box.h struct TextBox): result is the menu answer at +0x14.
 * Matching: the pad keeps the read on the halfword form. */
struct TextBoxView { u8 unk0[0x14]; u16 result; u8 pad[0x20]; };
extern struct TextBoxView gTextBox; /* 0x0201AE60 */
extern struct DuelStateView gDuel; /* 0x020192E0 */
int CountActiveCardsOnField(int player, int id);
u16 CanMonsterAttack(int player, int zone, u16 flag);
void BuildAttackableMask(struct DuelPlayerZones *ps, int player, int a, u16 flag);
int HasFaceUpToonMonster(int player);
u16 HasNoFaceUpLightDarkWindMonster(int player);
/* Zone pointers through gDuelPlayers. Matching: the two operand orders are both
 * load-bearing; each call site uses the order its ROM instructions use. */
#define ZONE_AT(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)&gDuelPlayers[0].zones[0]))
#define ZONE_AT_PZ(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)&gDuelPlayers[0].zones[0]))

u16 CardMenu_GetAvailableCommands(void)
{
    struct PolymerizationScratch ref;
    u16 flags = 1;
    u16 id = DuelCursor_GetCardId();
    if (IsCardProhibited(id))
        return 1;
    if (gDuel.flags1B12 & 2) {
        struct DuelScreenView *cur = &gDuelScreen;
        if (cur->selArea == DUEL_AREA_SPELL_TRAP && GetCardSpellSpeed(id) > 1 && cur->selPlayer == 0)
            flags |= CardMenu_GetSpellTrapCommands(id, 0, cur->selIndex);
    } else {
        switch (gDuelScreen.selArea) {
        case DUEL_AREA_HAND:
            if (id == 0)
                return 0;
            if (gDuelScreen.selPlayer == 0)
                flags |= CardMenu_GetHandCardCommands(id, 0, gDuelScreen.selIndex);
            break;
        case DUEL_AREA_MONSTER:
            if (id == 0)
                return 0;
            if (gDuelScreen.selPlayer == 0)
                flags |= CardMenu_GetMonsterCommands(id, 0, gDuelScreen.selIndex);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
            break;
        case DUEL_AREA_SPELL_TRAP:
            if (id == 0)
                return 0;
            if (gDuelScreen.selPlayer == 0)
                flags |= CardMenu_GetSpellTrapCommands(id, 0, gDuelScreen.selIndex);
            break;
        case DUEL_AREA_FIELD:
            if (id == 0)
                return 0;
            if (gDuelScreen.selPlayer == 0)
                flags |= CardMenu_GetSpellTrapCommands(id, 0, 5);
            break;
        case DUEL_AREA_DECK:
            if (((u32)(gDuel.flags1B12 << 27) >> 29) == PHASE_DRAW)
                flags = CARDMENU_MASK_DRAW;
            else
                flags = CARDMENU_MASK_SURRENDER;
            break;
        case DUEL_AREA_FUSION_DECK:
            if (GetFaceUpFieldMagicNumber() == CARD_1547) {
                ref.unk2_0 = 0;
                if (EffectPolymerizationPrepare(&ref, 0, 0))
                    flags |= CARDMENU_MASK_FUSION;
            }
            break;
        }
    }
    return flags;
}
/* Byte view of a zone's +0x06 flag byte (bit 1 = face-up), for the opponent-field check. */
struct ZoneFlagsView { u8 unk0[6]; u8 flags6; u8 unk7[0x94 - 7]; };
int DuelScreen_HandleInput(void)
{
    u32 player, area;
    u16 id;

    if (gDuel.cardMenu.open) {
        CardMenu_Update();
        return 1;
    }
    if (gDuel.cardMenu.confirmed) {
        CardMenu_Execute();
        return 1;
    }
    if (DuelCursor_PickAny() == 0)
        return 0;
    player = gDuelScreen.selPlayer;
    area = gDuelScreen.selArea;
    id = DuelCursor_GetCardId();
    switch (area) {
    case DUEL_AREA_MONSTER:
    case DUEL_AREA_SPELL_TRAP:
    case DUEL_AREA_FIELD:
        if (id != 0 && (gDuelScreen.selPlayer == 0 || (((struct ZoneFlagsView *)((gDuelScreen.selPlayer & 1) * 0xD64 + (gDuelScreen.selArea + gDuelScreen.selIndex) * 0x94 + (u32)gDuelPlayers[0].zones))->flags6 & 2))) {
            gDuel.cardMenu.open = 1;
            gDuel.cardMenu.state = 0;
            gDuel.cardMenu.available = CardMenu_GetAvailableCommands();
            return 1;
        }
        PlaySE(SE_ERROR);
        return 0;
    case DUEL_AREA_HAND:
        if ((gDuelScreen.selPlayer == 0 || IsHandRevealed(gDuelScreen.selPlayer) != 0) && id != 0) {
            gDuel.cardMenu.open = 1;
            gDuel.cardMenu.state = 0;
            gDuel.cardMenu.available = CardMenu_GetAvailableCommands();
            return 1;
        }
        PlaySE(SE_ERROR);
        return 0;
    case DUEL_AREA_FUSION_DECK:
    case DUEL_AREA_DECK:
        if (player == 0) {
            gDuel.cardMenu.open = 1;
            gDuel.cardMenu.state = 0;
            gDuel.cardMenu.available = CardMenu_GetAvailableCommands();
            return 1;
        }
        PlaySE(SE_ERROR);
        return 0;
    case DUEL_AREA_GRAVEYARD:
    case DUEL_AREA_BANISHED:
        CardListView_Open(player, area, 0, 0);
        PlaySE(SE_CONFIRM);
        return 0;
    }
    return 0;
}

void MarkMonsterAttacked(int player, int zone)
{
    u32 pl = player & 1;
    struct DuelZone *z = ZONE_AT(pl, zone);
    z->flags7 |= 4;
    gDuelPlayers[pl].attackedMask |= 1 << zone;
}

int CanAttackDirectly(int player, int zone)
{
    int one = 1; /* Matching (hypothesis): masking through this local keeps the original shape. */
    int pl = player & one;
    int number = *(u16 *)((u8 *)gCardIdToNumber + ((ZONE_AT_PZ(pl, zone)->card << 21) >> 20));
    switch (number) {
    case CARD_MYSTIC_LAMP:
    case CARD_LEGHUL:
    case CARD_OOGUCHI:
    case CARD_JINZO_7:
    case CARD_RAINBOW_FLOWER:
    case CARD_QUEENS_DOUBLE:
        return 1;
    case CARD_MANGA_RYU_RAN:
    case CARD_TOON_MERMAID:
    case CARD_TOON_SUMMONED_SKULL:
    case CARD_BLUE_EYES_TOON_DRAGON:
        if (HasFaceUpToonMonster(1 - player) == 0)
            return 1;
        return 0;
    case CARD_ALLIGATORS_SWORD_DRAGON:
        return HasNoFaceUpLightDarkWindMonster(one - player);
    default:
        return 0;
    }
}
u16 IsTypeForbiddenToAttack(u16 type)
{
    int p, z;
    for (p = 0; p < 2; p++) {
        for (z = 5; z <= 9; z++) {
            struct DuelZone *zn = ZONE_AT(p & 1, z);
            u32 id = zn->card << 20 >> 20;
            if (id != 0 && (((u8 *)zn)[6] & 2) && !zn->isDisabled
                && *(u16 *)((u8 *)gCardIdToNumber + ((id & CARD_ID_MASK) << 1)) == CARD_THE_REGULATION_OF_TRIBE
                && (*(u32 *)((u8 *)zn + 0x90) << 14 >> 27) == type)
                return 1;
        }
    }
    return 0;
}

/* The narrow local ID and helper argument preserve the original lookup shape. */
static inline u16 DuelCardNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & CARD_ID_MASK];
}

u16 CanMonsterAttack(int player, int zone, u16 flag)
{
    struct DuelZone *zn = &gDuelPlayers[player & 1].zones[zone];
    struct ZoneCardStats info;
    u32 fd = zn->isFaceDown;
    u16 id = zn->card << 20 >> 20;
    u32 type, atkVal, lv;
    if (id == 0)
        return 0;
    if (!(((u8 *)zn)[6] & 2))
        return 0;
    if ((gDuelPlayers[player & 1].attackedMask >> zone) & 1)
        return 0;
    GetZoneCardStats(player, zone, &info);
    type = info.type;
    atkVal = (u16)info.atk;
    if (CountZoneLinksFromCard(player, zone, CARD_SPELLBINDING_CIRCLE))
        return 0;
    if (CountZoneLinksFromCard(player, zone, CARD_1244))
        return 0;
    if (CountActiveZoneLinksFromCard(player, zone, CARD_1548))
        return 0;
    if (CountActiveZoneLinksFromCard(player, zone, CARD_PARALYZING_POTION) && type != CARD_TYPE_MACHINE)
        return 0;
    if (IsTypeForbiddenToAttack(type))
        return 0;
    if (CountZoneLinksFromCard(player, zone, CARD_DARK_EYES_ILLUSIONIST))
        return 0;
    if (CountZoneLinksFromCard(player, zone, CARD_1332))
        return 0;
    if (CountActiveCardsOnField(0, CARD_MESSENGER_OF_PEACE) > 0 || CountActiveCardsOnField(1, CARD_MESSENGER_OF_PEACE) > 0) {
        if (atkVal > 0x5DB) /* 1499 ATK */
            return 0;
    }
    if (CountActiveCardsOnField(0, CARD_1323) > 0 || CountActiveCardsOnField(1, CARD_1323) > 0) {
        CARD_LEVEL(lv, id);
        if (lv > 3)
            return 0;
    }
    if (CountActiveCardsOnField(1 - player, CARD_1352) > 0 && type == CARD_TYPE_INSECT)
        return 0;
    if (CountZoneLinksFromCard(player, zone, CARD_1419))
        return 0;
    if (DuelCardNumber(id) == CARD_1523 && CountMonsters(1 - player) == 0)
        return 0;
    if (CountActiveCardsOnField(0, CARD_1334) > 0 || CountActiveCardsOnField(1, CARD_1334) > 0) {
        if (DuelCardNumber(id) != CARD_1334)
            return 0;
    }
    if (flag != 0) {
        switch (DuelCardNumber(id)) {
        case CARD_DARK_ELF:
            if (gDuelPlayers[player & 1].lifePoints <= 0x3E7 /* 999 */)
                return 0;
            break;
        case CARD_MANGA_RYU_RAN:
        case CARD_TOON_MERMAID:
        case CARD_TOON_SUMMONED_SKULL:
        case CARD_BLUE_EYES_TOON_DRAGON:
            if (gDuelPlayers[player & 1].lifePoints <= 0x1F3 /* 499 */)
                return 0;
            break;
        case CARD_PANTHER_WARRIOR:
        case CARD_INSECT_QUEEN:
            if (CountTributableMonsters(player, zone) == 0)
                return 0;
            break;
        }
    }
    if (DuelCardNumber(id) == CARD_TOTAL_DEFENSE_SHOGUN) {
        id = 0;
        fd = id;
    }
    if (fd != 0)
        return 0;
    if (zn->flags7 & 0x10)
        return 0;
    if (zn->unk8C & 0x18)
        return 0;
    return 1;
}

void BuildAttackableMask(struct DuelPlayerZones *ps, int player, int a, u16 flag)
{
    int i, x, zero, lp;
    struct DuelPlayerZones *me = &ps[player & 1];
    me->attackableMask = 0;
    if (CountActiveCardsOnField(1 - player, CARD_SWORDS_OF_REVEALING_LIGHT) != 0)
        return;
    if (CountActiveCardsOnField(0, CARD_1230) != 0)
        return;
    zero = CountActiveCardsOnField(1, CARD_1230);
    if (zero != 0)
        return;
    lp = gDuelPlayers[player & 1].lifePoints;
    x = CountActiveCardsOnField(0, CARD_TOLL);
    x += CountActiveCardsOnField(1, CARD_TOLL);
    if (lp < x * 500)
        return;
    if (flag != 0)
        me->attackedMask = zero;
    for (i = 0; i <= 4; i++) {
        struct DuelPlayerZones *m = &ps[player & 1];
        struct DuelPlayerZones *g = &gDuelPlayers[player & 1];
        if (CanMonsterAttack(player, i, 1) != 0) {
            if (flag != 0 || !((g->attackedMask >> i) & 1))
                m->attackableMask |= 1 << i;
        }
    }
}

int CanEnterBattlePhase(int player)
{
    struct DuelStateView *e = &gDuel;
    struct DuelPlayerZones *ps, *ps0;
    if (e->unk1B10 == 0)
        return 0;
    ps0 = (struct DuelPlayerZones *)((u8 *)e + 4);
    ps = ps0 + (player & 1);
    if ((ps->unk9 << 27) < 0 && (ps->unk8 << 25) >= 0)
        return 0;
    BuildAttackableMask(gDuelPlayers, player, 0, 1);
    return gDuelPlayers[player & 1].attackableMask != 0;
}
int IsBattleEffectBlocked(void)
{
    return 0;
}
/* Zone view for the replay check, read through gDuelZones: card word, serial and the
 * +0x07 flag byte with its replay bit. */
struct DuelZoneReplayView {
    u32 card;
    u16 serial;                     /* +0x04 */
    u8 flags6;                      /* +0x06 */
    u8 unk7_0 : 5;                  /* +0x07 */
    u8 flag7_5 : 1;                 /* +0x07 bit 5: consumed by CARD_1336's extra attack */
    u8 unk7_6 : 2;
    u8 pad[0x94 - 8];
};
extern struct DuelZoneReplayView gDuelZones[]; /* 0x0201930C */
#define ZONE_SNAP(p, z) (*(struct DuelZoneReplayView *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
/* Battle-step check for `player`'s attack. Returns 1 and resets the step (gBattle direct,
 * battleStep, stage back to SELECT_ATTACKER) when the attacker can no longer attack
 * (CARD_1336 then consumes its zone flag bit 5) or the defender's state changed;
 * otherwise returns 0. The cast-constant card table is a reload, which the ROM's later
 * reload registers need. */
int Battle_CheckReplay(int player)
{
    int ok = 0;
    if (CanMonsterAttack(player, BTH.atkSlot, 0) == 0) {
        int done = 0;
        int pl = player & 1;
        if (((const u16 *)0x08622AB4)[(ZONE_SNAP(pl, BTH.atkSlot).card << 21) >> 21] == CARD_1336) {
            if (ZONE_SNAP(pl, BTH.atkSlot).flag7_5) {
                ZONE_SNAP(pl, BTH.atkSlot).flag7_5 = 0;
                done = 1;
            }
        }
        if (done == 0)
            MarkMonsterAttacked(player, BTH.atkSlot);
        BTB.direct = 0;
        gDuel.battleStep = 0;
        gStageWord.battleStage = BATTLE_STAGE_SELECT_ATTACKER;
    } else {
        int opp = 1 - player;
        if (CountMonsters(opp) != BT_U16(0x148 + opp * 2))
            ok = 1;
        if (!BTB.direct) {
            if ((ZONE_SNAP(opp & 1, BTB.defSlot).card << 20) == 0)
                ok = 1;
            if ((ZONE_SNAP(opp & 1, BTB.defSlot).card << 20) != 0 && BT_U16(0x14C + opp * 2) != ZONE_SNAP(opp & 1, BTB.defSlot).serial)
                ok = 1;
        }
        if (BTB.direct) {
            /* CanAttackDirectly is defined above returning int; the caller narrows its result as u16. */
            if (CountMonsters(1 - player) > 0 && ((u16 (*)(int, int))CanAttackDirectly)(player, BTH.atkSlot) == 0)
                ok = 1;
        }
        if (ok == 0)
            return 0;
        BTB.direct = 0;
        gDuel.battleStep = 0;
        gStageWord.battleStage = BATTLE_STAGE_SELECT_ATTACKER;
        BTB.attackDeclared = 1;
        BTB.attackCostsPaid = 1;
    }
    return 1;
}

int BattleStage_Start(int player)
{
    struct DuelStateView *e = &gDuel;
    u32 c = e->battleStep;
    if (c == 0) {
        DuelCmd_Push((player ? DUEL_CMD_PLAYER : 0) | DUEL_CMD_PREPARE_BATTLE_PHASE, 1, 0, 0);
        e->battleStep++;
        return 0;
    }
    BuildAttackableMask((struct DuelPlayerZones *)((u8 *)e + 4), player, 0, 1);
    DuelCmd_Push(((e->flags1B12 & 2) ? DUEL_CMD_PLAYER : 0) | DUEL_CMD_BATTLE_PHASE, 0, 0, 0);
    return 1;
}
int BattleStage_SelectAttacker(int player)
{
    struct DuelStateView *e = &gDuel;
    struct BattleStepHalf *c = (struct BattleStepHalf *)((u8 *)e + 0x1B16);
    struct BattleH *bh;
    u16 msg, a1;
    switch (c->step) {
    case 0:
        BuildAttackableMask((struct DuelPlayerZones *)((u8 *)e + 4), player, 0, 0);
        BTB.direct = 0;
        BTB.flipEffectPending = 0;
        BTB.zeroAttackerAtk = 0;
        DuelCursor_Select(player, 0, 0);
        c->step++;
        return 0;
    case 1:
        if (player == 0) {
            if (DuelScreen_HandleInput() != 0)
                return 0;
            if (gMain.newKeys & B_BUTTON) {
                if ((e->unk0C << 25) < 0)
                    TextBoxOpen(0x204, 0x616, 0xB, gStrEndBattlePhaseMenuExtra);
                else
                    TextBoxOpen(0x204, 0x616, 0xB, gStrEndBattlePhaseMenu);
                TextBoxSetMenu(5 /* TEXTBOX_MENU_CUSTOM */, (u32)PhaseMenu_DrawCursor, (u32)PhaseMenu_HandleInput);
                {
                    struct DuelStateView *e2 = &gDuel;
                    e2->battleStep = 10;
                }
            }
            return 0;
        }
        if (AiPlanAttack(0)) {
            bh = &BTH;
            bh->atkSlot = gAiWork.attackerZone;
            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD, gDuelScreen.selPlayer, bh->atkSlot << 8, 0);
            ((struct BattleB *)bh)->attackCostsPaid = 0;
            ((struct BattleB *)bh)->attackDeclared = 0;
            return 1;
        }
        ((struct BattleStageWord *)((u8 *)e + 0x1B14))->battleStage = BATTLE_STAGE_END;
        c->step = 0;
        return 0;
    case 10:
        switch (gTextBox.result) {
        case 0:
            ((struct BattleStageWord *)((u8 *)e + 0x1B14))->battleStage = BATTLE_STAGE_END;
            c->step = 0;
            e->flag1B26_0 = 0;
            return 0;
        case 1:
            ((struct BattleStageWord *)((u8 *)e + 0x1B14))->battleStage = BATTLE_STAGE_END;
            c->step = 0;
            e->flag1B26_0 = 1;
            return 0;
        case 2:
            c->step = 1;
            return 0;
        }
        return 0;
    default:
        msg = player ? (DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD) : DUEL_CMD_POINT_AT_CARD;
        a1 = player;
        bh = &BTH;
        DuelCmd_Push(msg, a1, bh->atkSlot << 8, 0);
        ((struct BattleB *)bh)->attackCostsPaid = 0;
        ((struct BattleB *)bh)->attackDeclared = 0;
        return 1;
    }
}
/* Zone view for target selection, read through gDuelZones: card word and the +0x06 flag
 * byte (bit 1 = face-up). */
struct DuelZoneTargetView {
    u32 card;
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 flags7;
    u8 pad[0x94 - 8];
};
/* Halfword view of the AI's attack plan (gAiWork +0xC, ai.h struct AttackPlan): isDirect
 * makes the battle step attack directly, targetZone is the planned defending zone. */
struct AttackPlanView {
    u8 unk0[0xC];
    u16 unk0_0 : 1;
    u16 isDirect : 1;
    u16 unk0_2 : 5;
    u16 targetZone : 3;
    u16 rest : 6;
};
#define gAiPlan (*(struct AttackPlanView *)&gAiWork)
#define ZONE_PTR(p, z) ((struct DuelZoneTargetView *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))
#define ZONE_PTR_P1(z) ((struct DuelZoneTargetView *)((u8 *)gDuelZones + 0xD64) + (z))
/* The battle-step halfword, addressed from gDuelZones (+0x1AEA is gDuel +0x1B16). */
#define gBattleStep (*(struct BattleStepHalf *)((u8 *)gDuelZones + 0x1AEA))
/* The gDuel word at +0x1B14 again, viewed from the gDuel base (cf. struct BattleStageWord). */
struct DuelStageView {
    u8 prefix[0x1B14];
    u32 lo : 9;
    u16 battleStage : 8;            /* enum BattleStage */
    u32 hi : 15;
    u8 pad[8];
};
/* Byte view of the duel screen's selected zone index (gDuelScreen +0x82C). */
struct DuelScreenSelView { u8 pad[0x82C]; u8 selIndex; };
#define gScreenSel (*(struct DuelScreenSelView *)&gDuelScreen)
#define gDuelStage (*(struct DuelStageView *)&gDuel)
int CountMonstersAffectedByCard(int player, int number);
int FindMonsterAffectedByCard(int player, int number);
int GetZoneCardAtk(int player, int zone);
int GetZoneCardDef(int player, int zone);
void ShowCardEffect(int player, u16 id);
int DuelCursor_PickTarget(u32 keys);
int IsToonMonster(u16 number);
extern const u8 gStrSelectAttackTarget[]; /* 0x08085958 */
extern const u8 gStrAskDirectAttack[];    /* 0x0808598C */

/* Attack step machine switched on the battle step (gDuel +0x1B16 bits 1-8):
 * CARD_RING_OF_MAGNETISM on the defender's side redirects the attack (steps 0, 20),
 * then target selection (2, 3, 10, 11, 30) and messages.
 * Returns 1 when the step hands over to a message, else 0. */
int BattleStage_SelectTarget(int player)
{
    u32 pl = player & 1;
    u32 id = ZONE_PTR(pl, BTH.atkSlot)->card << 20 >> 20;
    if (CountMonsters(1 - player) == 0)
        BTB.direct = 1;
    if (BTB.direct) {
        DuelCmd_Push((player ? DUEL_CMD_PLAYER : 0) | DUEL_CMD_DIRECT_ATTACK, BTH.atkSlot, 1, 0);
        BTB.defSlot = 5;
        return 1;
    }
    switch (gBattleStep.step) {
    case 0: {
        int opp = 1 - player;
        u32 num = CARD_RING_OF_MAGNETISM;
        if (CountMonstersAffectedByCard(opp, num) > 0) {
            /* FAKEMATCH: the ROM keeps copies of opp (r6) and num (sl) beside the originals (r8, r4).
             * The empty asms stop CSE from merging them; the sl pin fixes the allocation order. */
            register int c asm("sl");
            int p, n, zone, a, b, mine;
            ShowCardEffect(opp, ((const u16 *)0x08623DF4)[num] /* gCardNumberToId */);
            p = opp;
            c = num;
            asm("" : "+r"(opp));
            asm("" : "+r"(c));
            n = CountMonstersAffectedByCard(p, c);
            if (n == 1) {
                if (player != 0) {
                    zone = FindMonsterAffectedByCard(p, c);
                    a = GetZoneCardAtk(p, zone);
                    b = GetZoneCardDef(p, zone);
                    mine = GetZoneCardAtk(player, BTH.atkSlot);
                    if (ZONE_PTR(p & 1, zone)->flags6 & 1) {
                        if (mine > b) {
                            BTB.defSlot = FindMonsterAffectedByCard(p, c);
                            gBattleStep.step = 3;
                            return 0;
                        }
                    } else {
                        if (mine > a) {
                            BTB.defSlot = FindMonsterAffectedByCard(opp, c);
                            gBattleStep.step = 3;
                            return 0;
                        }
                    }
                    MarkMonsterAttacked(player, BTH.atkSlot);
                    gDuel.battleStep = 0;
                    gDuelStage.battleStage = BATTLE_STAGE_SELECT_ATTACKER;
                    return 0;
                }
                BTB.defSlot = FindMonsterAffectedByCard(1, num);
                gDuel.battleStep = 4;
                return 0;
            }
            if (player != 0) {
                MarkMonsterAttacked(player, BTH.atkSlot);
                gDuel.battleStep = 0;
                gDuelStage.battleStage = BATTLE_STAGE_SELECT_ATTACKER;
                return 0;
            }
            gDuel.battleStep = 20;
            return 0;
        }
        gDuel.battleStep++;
    }
    case 1:
        if (IsToonMonster(((const u16 *)0x08622AB4)[id & CARD_ID_MASK]) && HasFaceUpToonMonster(1 - player)) {
            gDuel.battleStep = 30;
            return 0;
        }
        if ((u16)CanAttackDirectly(player, BTH.atkSlot)) {
            gDuel.battleStep = 10;
            return 0;
        }
        gDuel.battleStep++;
    case 2:
        if (player != 0) {
            if (AiPlanAttack(0)) {
                if (gAiPlan.isDirect) {
                    BTB.direct = 1;
                    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_DIRECT_ATTACK, BTH.atkSlot, 1, 0);
                    BTB.defSlot = 5;
                    return 1;
                }
                BTB.defSlot = gAiPlan.targetZone;
                DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_ATTACK, BTH.atkSlot, BTB.defSlot, 0);
                return 1;
            }
            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_MARK_ATTACKED, BTH.atkSlot, 1, 0);
            gDuelStage.battleStage = BATTLE_STAGE_SELECT_ATTACKER;
            gDuel.battleStep = 0;
            return 0;
        }
        TextBoxOpen(0x206, 0x511, 0xB, gStrSelectAttackTarget);
        gDuel.battleStep++;
        return 0;
    case 3:
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER))) {
            struct DuelZoneTargetView *z = ZONE_PTR_P1(gDuelScreen.selIndex);
            int cid;
            if ((z->flags6 & 2) && (cid = z->card << 20 >> 20) > 0 && ((const u16 *)0x08622AB4)[cid & CARD_ID_MASK] == CARD_1326 && GetFaceUpFieldMagicNumber() == CARD_UMI) {
                PlaySE(SE_ERROR);
                return 0;
            }
            BTB.defSlot = gScreenSel.selIndex;
            gDuel.battleStep++;
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            DuelCursor_Select(player, 0, BTH.atkSlot);
            gDuel.battleStep = 0;
            gDuelStage.battleStage--;
            return 0;
        }
        return 0;
    case 4:
        DuelCmd_Push((player ? DUEL_CMD_PLAYER : 0) | DUEL_CMD_ATTACK, BTH.atkSlot, BTB.defSlot, 0);
        return 1;
    case 10:
        if (player != 0) {
            BTB.direct = 1;
            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_DIRECT_ATTACK, BTH.atkSlot, 1, 0);
            return 1;
        }
        TextBoxOpen(0x204, 0x715, 0xB, gStrAskDirectAttack);
        TextBoxSetMenu(1 /* TEXTBOX_MENU_YES_NO */, 0, 0);
        gDuel.battleStep++;
        return 0;
    case 11:
        if (gTextBox.result) {
            BTB.direct = 1;
            DuelCmd_Push((player ? DUEL_CMD_PLAYER : 0) | DUEL_CMD_DIRECT_ATTACK, BTH.atkSlot, 1, 0);
            return 1;
        }
        gDuel.battleStep = 2;
        return 0;
    case 20:
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION))) {
            if (CountZoneLinksFromCard(1 - player, gDuelScreen.selIndex, CARD_RING_OF_MAGNETISM)) {
                BTB.defSlot = gScreenSel.selIndex;
                gDuel.battleStep = 4;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            gDuel.battleStep = 0;
            gDuelStage.battleStage--;
            return 0;
        }
        return 0;
    case 30:
        if (player != 0)
            return 0;
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER)) && IsToonMonster(((const u16 *)0x08622AB4)[(ZONE_PTR_P1(gDuelScreen.selIndex)->card << 20 >> 20) & CARD_ID_MASK])) {
            BTB.defSlot = gScreenSel.selIndex;
            gBattleStep.step = 3;
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            gDuel.battleStep = 0;
            gDuelStage.battleStage--;
            return 0;
        }
        return 0;
    }
    return 1;
}
