/*
 * effect_targets3 (0x0803FE70-0x08040EBB): card effect target selection, part 3
 * (wiki/functions/effect-targets3-c.md; parts 1, 2 and 4 are effect_targets1.c, effect_targets2.c and
 * effect_targets4.c).
 *
 * Seventeen ChainB handlers of gCardEffects (struct CardEffect, include/effect.h): the stat cards
 * Reinforcements, Castle Walls, Rush Recklessly, The Reliable Guardian and Snake Fang, Block Attack, Darkness
 * Approaches, Fairy's Hand Mirror, Tailor of the Fickle, Mystical Space Typhoon (and Gust, Driving Snow), Dust
 * Tornado, Earthshaker, DNA Surgery, the two Noblemen, Prohibition, Riryoku, and effect keys 1213-1255 that
 * no EDS card uses. Chain_Build calls a link's ChainB handler every frame until it returns 1; then
 * link->targets[0..numTargets-1] holds the chosen targets (possibly none: the effect then does nothing), and
 * 0 means "call me again next frame".
 *
 * A target is a board position player | zone << 8 (DUEL_LOC; zones 0-4 monsters, 5-9 spells and traps, 10
 * the Field Magic) or a declared value (a type, an attribute, a card ID). The steps are counted in
 * gChain.targetStep, which Chain_Build clears before the first call: step 0 opens the prompt text box and
 * clears numTargets; the next step waits for the field cursor, which only stops on positions that match a
 * FieldPickMask (DuelCursor_PickTarget returns 1 when A is pressed; the position is gDuelScreen.selPlayer and
 * selArea + selIndex). A refused pick plays SE_ERROR. In most handlers B goes back to step 0. Declarations
 * use a duel prompt (DuelPrompt_Post) whose answer arrives in gDuel.promptResult. Only
 * EffectSpellTrapTargetChainB has a branch for the CPU (player 1).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_NUMBER_TOKEN_*, gCardNames */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, DuelPromptKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* DUEL_CMD_POINT_AT_CARD, DUEL_CMD_PLAYER */
#include "duel.h"                  /* struct DuelPlayer, gDuel, ... */
#include "sound.h"                 /* PlaySE */
#include "constants/sound.h"        /* enum SoundEffect */
#include "gba.h"              /* B_BUTTON */
#include "main.h"             /* gMain.newKeys */

#include "bg.h"                     /* ResetVideo */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "deck_edit.h"              /* ProhibitCardSelect_Run */
#include "duel_actions.h"           /* ShowPickedCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget, DuelScreen_Init / Fade*Step */
#include "effect.h"                 /* AddEffectTarget, TryAddEffectTarget, shared prompt texts */
#include "effect_handlers.h"        /* the handlers defined here and the Check handlers they call */
#include "text_box.h"               /* TextBoxOpen */
#include "util.h"                   /* FormatStr */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: these are defined with u16 returns; this unit tests the results as int (cmp r0, #0 with no
 * lsl #16), as its callers did. */
extern int CanCardTargetZoneInt(u16 cardId, int player, int zone) asm("CanCardTargetZone");
extern int CanRedirectEffectToZoneInt(u16 effectNumber, struct ChainEntry *link, int player, int zone)
    asm("CanRedirectEffectToZone");
extern int IsValidEquipTargetInt(int equipPlayer, int equipSlot, int targetPlayer, int targetSlot)
    asm("IsValidEquipTarget");
extern int FindMonsterLinkedToCardInt(int player, int slot) asm("FindMonsterLinkedToCard");
extern int ProhibitCardSelect_RunInt(void) asm("ProhibitCardSelect_Run");
extern int DuelScreen_FadeInStepInt(void) asm("DuelScreen_FadeInStep");
/* Matching: EffectRiryokuPrepare is defined with no parameters (it ignores them), but the ROM calls it like
 * a Prepare handler, with (link, 0, 0) in r0-r2. */
extern int EffectRiryokuPrepare3(struct ChainEntry *link, int chainLink, int fromHand) asm("EffectRiryokuPrepare");

/* gChain as a byte array (the same symbol). Matching: the ROM forms the address of a step byte as gChain +
 * 0x3E5 from two literals (ldr =gChain; ldr =0x3E5; adds); the member gChain.targetStep folds into one. */
extern u8 gChainBytes[] asm("gChain");
#define TARGET_STEP     OFFSET_OF(struct ChainState, targetStep)    /* 0x3E5 */
#define TARGET_WORK     OFFSET_OF(struct ChainState, targetWork)    /* 0x3E6 */
#define TARGET_WORK2    OFFSET_OF(struct ChainState, targetWork2)   /* 0x3E7 */

/* gDuel.promptResult[1]: the second answer of a two-value prompt (+0x1B66; duel.h only names [0]). */
#define PROMPT_RESULT2  ((&gDuel.promptResult)[1])

/* gMain.pickedCardId (+0x4872): the card ID chosen in the Deck Edit list popup. The legacy main.h has no name
 * for it (unk4871[3]); the staged main.h does. */
#define MAIN_PICKED_CARD_ID 0x4872

/* &gCardNumberToId[CARD_SUMMONED_SKULL] (0x08623DF4 + 2 * 21). Matching: the ROM loads this element address
 * from its own literal. */
extern const u16 gCardNumberToId_SummonedSkull[];

/* Prompts (ROM; only this unit uses them). */
extern const u8 gStrDesignateMonsterToIncreaseAtk[];        /* 0x08084520: stat cards, Riryoku */
extern const u8 gStrDesignateMonsterToIncreaseDef[];        /* 0x08084558: Castle Walls, The Reliable Guardian */
extern const u8 gStrDesignateMonsterToDecreaseDef[];        /* 0x08084590: Snake Fang */
extern const u8 gStrDesignateMonsterForDefensePosition[];   /* 0x080845C8: Block Attack */
extern const u8 gStrDesignateMonsterToSetFaceDown[];        /* 0x08084620: Darkness Approaches */
extern const char gStrDesignateNewTargetFmt[];              /* 0x08084658: '... as target for '%s'.' */
extern const u8 gStrDesignateEquipToSwitch[];               /* 0x08084694: Tailor of the Fickle */
extern const char gStrDesignateMonsterToSwitchEquipFmt[];   /* 0x080846CC: '... switch over %s.' */
extern const u8 gStrDesignateSpellTrapToDestroy[];          /* 0x08084704: Mystical Space Typhoon */
extern const u8 gStrDesignateFaceDownMonsterToBanish[];     /* 0x08084788: Nobleman of Crossout */
extern const u8 gStrDesignateFaceDownSpellTrapToBanish[];   /* 0x080847C8: Nobleman of Extermination */
extern const u8 gStrDesignateCardToProhibit[];              /* 0x08084814: Prohibition */
extern const u8 gStrDesignateMonsterToHalveAtk[];           /* 0x0808484C: Riryoku */
extern const u8 gStrDesignateMonsterToGiveControl[];        /* 0x08084888: key 1213 */
extern const u8 gStrDesignateOpponentMonsterToTribute[];    /* 0x080848E4: key 1220 */

/* Text box of the target prompts: cell (6, 2), 18 x 7 cells. */
#define TARGET_PROMPT_POS 0x206
#define TARGET_PROMPT_SIZE 0x712

/* Pick masks: the same positions on both sides; a face-up monster in either position (0xE0); any Magic or
 * Trap card, face up or set (0xE). */
#define PICK_BOTH_SIDES(mask) ((mask) | PICK_PLAYER1(mask))
#define PICK_FACE_UP_MONSTER_ANY (PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)
#define PICK_ANY_SPELL_TRAP (PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP)

/* ChainEntry.player read as the raw byte (ldrb; and #1). Matching: a bitfield read gives lsl/lsr. */
#define ENTRY_PLAYER_BYTE(entry) (1 & ((u8 *)(entry))[2])
/* ChainEntry.event tested in place: byte 3 holds event << 2 in its top six bits. */
#define ENTRY_EVENT_BITS(entry) (((u8 *)(entry))[3] & 0xFC)

/* &gDuelZones[player].zones[zone] by byte arithmetic; player must be 0 or 1. Matching: the two forms add
 * the zone and player terms in the orders the ROM uses (ZONE_AT: zone term first, ZONE_AT_PZ: player term
 * first; array indexing gives yet another order). */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelZonesPlayer) + (u32)gDuelZones))
#define ZONE_AT_PZ(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelZonesPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))
/* The card word of a zone as one u32 (ldr), and its card ID: 12 bits (lsl #20; lsr #20), or the 11 bits a
 * card table index keeps (lsl #21; lsr #21). Matching: a bitfield read of .id loads a halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
#define CARD_ID11(word) (((word) << 21) >> 21)

/* Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the symbol forms (card_data.h) load other literals. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */
#define ID_TO_NUMBER ((const u16 *)0x08622AB4)              /* gCardIdToNumber */

/*
 * ChainB of the until-end-of-turn stat cards: Reinforcements, Rush Recklessly and key 1534 (ATK up), Castle
 * Walls and The Reliable Guardian (DEF up), Snake Fang (DEF down).
 *   Step 0: the card's prompt; any other card number returns 1 with no target.
 *   Then: a pick among the face-up monsters of both players. No B-cancel.
 */
int EffectStatModifierTargetChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;

    if (chain[TARGET_STEP] == 0) {
        link->numTargets = 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_RUSH_RECKLESSLY:
        case CARD_REINFORCEMENTS:
        case CARD_1534:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToIncreaseAtk);
            break;
        case CARD_CASTLE_WALLS:
        case CARD_THE_RELIABLE_GUARDIAN:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToIncreaseDef);
            break;
        case CARD_SNAKE_FANG:
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                        gStrDesignateMonsterToDecreaseDef);
            break;
        default:
            return 1;
        }
        gChainBytes[TARGET_STEP]++;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        if (TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex) != 0)
            return 1;
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* ChainB of Block Attack: prompt, then a pick among the opponent's attack-position monsters that
 * CanCardTargetZone allows (else SE_ERROR). B goes back to step 0. */
int EffectBlockAttackChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: the B branch stores and returns this shared zero (movs r0, #0; strb; b to the end). */
    int zero = 0;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateMonsterForDefensePosition);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_DOWN_MONSTER | PICK_FACE_UP_MONSTER
                                                  | PICK_ATTACK_POSITION)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (CanCardTargetZoneInt(link->card, player, zone) != 0) {
            TryAddEffectTarget(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* ChainB of Darkness Approaches: prompt, then a pick among the face-up monsters of both players; a token
 * (card numbers 1920-1999) or a pick TryAddEffectTarget refuses plays SE_ERROR. No B-cancel. */
int EffectDarknessApproachesChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToSetFaceDown);
        (*step)++;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        int side = 1 & player;
        u16 number = ID_TO_NUMBER[CARD_ID11(CARD_WORD(ZONE_AT_PZ(side, zone)->card))];
        if ((u16)(number - CARD_NUMBER_TOKEN_FIRST) > CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST - 1) {
            if (TryAddEffectTarget(link, player, zone) != 0)
                return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/*
 * ChainB of Fairy's Hand Mirror and key 1317: move the single-target effect of prevLink (the link this one
 * answers) onto another monster.
 *   Key 1317 answering an attack (event RESPONSE_ATTACK_DECLARED): step 0 asks the player with the duel
 *     prompt PROMPT_SELECT_OWN_REPLACEMENT_TARGET (argument: the attacked monster's zone, loc1 >> 8); the
 *     next call adds the chosen zone of the player's own side and returns 1.
 *   Otherwise: step 0 shows 'Designate a monster to switch places as target for <prevLink's card>.'; then a
 *     pick (any monster of both players for Fairy's Hand Mirror, your own for key 1317) that
 *     CanRedirectEffectToZone accepts and that is not (1 - player, zone), compared with prevLink's
 *     targets[0]; else SE_ERROR. No B-cancel.
 */
int EffectRedirectTargetChainB(struct ChainEntry *link, struct ChainEntry *prevLink)
{
    char prompt[0x80];

    if (CARD_NUMBER(link->card) == CARD_1317 && ENTRY_EVENT_BITS(link) == RESPONSE_ATTACK_DECLARED << 2) {
        u8 *chain = gChainBytes;
        u8 *step = chain + TARGET_STEP;
        if (*step == 0) {
            link->numTargets = 0;
            DuelPrompt_Post(link->player, PROMPT_SELECT_OWN_REPLACEMENT_TARGET, link->loc1 >> 8, 0);
            (*step)++;
            return 0;
        }
        TryAddEffectTarget(link, link->player, gDuel.promptResult);
        return 1;
    } else {
        u8 *chain = gChainBytes;
        u8 *step = chain + TARGET_STEP;
        if (*step == 0) {
            FormatStr(prompt, gStrDesignateNewTargetFmt, (const char *)gCardNames + prevLink->card * CARD_NAME_SIZE);
            TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
            {
                /* link->numTargets = 0, as a read-modify-write of the byte at +0xA. FAKEMATCH: the named r1
                 * byte and the empty asm use keep the ROM's scratch register (the bitfield store uses
                 * another one). */
                int mask = ~7;
                register u8 fields __asm__("r1");

                fields = ((u8 *)link)[0xA];
                __asm__("" : : "r"(fields));
                ((u8 *)link)[0xA] = mask & fields;
            }
            (*step)++;
            return 0;
        } else {
            /* Only Fairy's Hand Mirror and key 1317 have this handler (gCardEffects rows 216 and 327), so the
             * mask is set for those two numbers only. Matching: without a default, any other number passes
             * whatever r2 holds, as in the ROM. */
            int pickMask;
            switch (CARD_NUMBER(link->card)) {
            case CARD_FAIRYS_HAND_MIRROR:
                pickMask = PICK_BOTH_SIDES(PICK_ANY_MONSTER);
                break;
            case CARD_1317:
                pickMask = PICK_ANY_MONSTER;
                break;
            }
            if (DuelCursor_PickTarget(pickMask) != 0) {
                u32 player = gDuelScreen.selPlayer;
                int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
                if (CanRedirectEffectToZoneInt(CARD_NUMBER(link->card), prevLink, player, zone) != 0) {
                    /* (u8)(1 - player) | (u8)zone << 8, built from the high bytes as in the ROM. */
                    u32 sideBits = (1 - player) << 24;
                    u32 zoneBits = (u32)zone << 24;
                    if (prevLink->targets[0] != (u16)((sideBits >> 8 | zoneBits) >> 16)) {
                        TryAddEffectTarget(link, player, zone);
                        return 1;
                    }
                }
                PlaySE(SE_ERROR);
            }
        }
    }
    return 0;
}

/*
 * ChainB of Tailor of the Fickle: move an Equip Card to another monster. No B-cancel.
 *   Step 0: prompt for the Equip Card.
 *   Step 1: a pick among face-up Magic and Trap cards of both players that EffectTailorOfTheFickleCheck
 *           accepts: confirm sound, the pointer at the card (DUEL_CMD_POINT_AT_CARD) and targets[0]; else
 *           SE_ERROR.
 *   Step 2: prompt 'Designate the monster that you wish to switch over <equip name>.'
 *   Step 3: a pick among face-up monsters that IsValidEquipTarget allows for the equip and that is not the
 *           monster it equips now (FindMonsterLinkedToCard): TryAddEffectTarget (its result is not checked);
 *           else SE_ERROR.
 *   Step 4: return 1.
 */
int EffectTailorOfTheFickleChainB(struct ChainEntry *link)
{
    char prompt[0x80];
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    int state = *step;

    switch (state) {
    case 0:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateEquipToSwitch);
        link->numTargets = 0;
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP)) == 0)
            return 0;
        {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            u16 pos = (u8)player | (u8)zone << 8;
            if (EffectTailorOfTheFickleCheck(link, pos) != 0) {
                u16 cmd;
                PlaySE(SE_CONFIRM);
                /* state is 1 here: state & byte 2 is the link's player bit. */
                cmd = (state & ((u8 *)link)[2]) ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD;
                DuelCmd_Push(cmd, gDuelScreen.selPlayer, (u8)gDuelScreen.selArea | (u8)gDuelScreen.selIndex << 8, 0);
                AddEffectTarget(link, pos);
                (*step)++;
                return 0;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    case 2: {
        const char *fmt = gStrDesignateMonsterToSwitchEquipFmt;
        int equipSide = 1 & link->targets[0];
        struct DuelZone *equip = ZONE_AT(equipSide, link->targets[0] >> 8);
        u32 nameOffset = (CARD_WORD(equip->card) << 20) >> 14;     /* card ID * CARD_NAME_SIZE */
        /* FAKEMATCH: the ROM loads the name-table address into r3 for the add. */
        register const char *names asm("r3") = (const char *)gCardNames;
        FormatStr(prompt, fmt, (const char *)(nameOffset + (u32)names));
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
        {
            /* FAKEMATCH: keep r2/r3 busy across the increment so reload picks r4 for the step pointer, as in
             * the ROM; this also sets the reload order that case 3 depends on. Emits no code. */
            register int k2 asm("r2");
            register int k3 asm("r3");
            (*step)++;
            asm("" : : "r"(k2), "r"(k3));
        }
        return 0;
    }
    case 3:
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) != 0) {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            u8 equipPlayer = link->targets[0];
            int equipZone = link->targets[0] >> 8;
            if (IsValidEquipTargetInt(equipPlayer, equipZone, player, zone) != 0) {
                int equipped = FindMonsterLinkedToCardInt(equipPlayer, equipZone);
                /* (u8)player | (u8)zone << 8, built from the high bytes as in the ROM. */
                u32 sideBits = player << 24;
                u32 zoneBits = (u32)zone << 24;
                if (equipped != (u16)((sideBits >> 8 | zoneBits) >> 16)) {
                    TryAddEffectTarget(link, player, zone);
                    (*step)++;
                    /* Matching: skip the SE_ERROR call through a label before case 3's own return, so that
                     * case 3's tail merges into case 0's (not the reverse). */
                    goto done;
                }
            }
            PlaySE(SE_ERROR);
        }
    done:
        return 0;
    default:
        return 1;
    }
}

/*
 * ChainB of Mystical Space Typhoon, Gust and Driving Snow: pick a Magic or Trap card to destroy.
 *   CPU: for player 0, then 1, zones 5-10: the first face-up Magic card, else the first face-down card; it
 *        returns 1 also when it found none.
 *   Human: prompt, then a pick among the Magic and Trap cards of both players; returns 1 after the pick even
 *          if TryAddEffectTarget refused it. B goes back to step 0.
 */
int EffectSpellTrapTargetChainB(struct ChainEntry *link)
{
    int isCpu = ENTRY_PLAYER_BYTE(link);
    u8 *chain;
    u8 *step;

    if (isCpu) {
        int player;
        link->numTargets = 0;
        for (player = 0; player <= 1; player++) {
            int zone;
            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(1 & player, zone);
                u16 id = CARD_ID(CARD_WORD(z->card));
                /* FAKEMATCH: the empty use raises z's allocation priority above id's, so z gets r1 and id r3
                 * as in the ROM */
                asm("" : : "r"(z));
                if (id != 0 && z->isFaceUp && CARD_TYPE(id) == CARD_TYPE_MAGIC) {
                    TryAddEffectTarget(link, player, zone);
                    return 1;
                }
            }
            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT_PZ(1 & player, zone);
                if ((CARD_WORD(z->card) << 20) != 0 && !z->isFaceUp) {
                    TryAddEffectTarget(link, player, zone);
                    return 1;
                }
            }
        }
        return 1;
    }
    chain = gChainBytes;
    step = chain + TARGET_STEP;
    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateSpellTrapToDestroy);
        link->numTargets = 0;
        (*step)++;
        return 0;
    } else if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: isCpu is 0 here; storing it reproduces the ROM's register use. */
        *step = isCpu;
        return 0;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_ANY_SPELL_TRAP)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/* ChainB of Dust Tornado: prompt, then a pick among the opponent's Magic and Trap cards; returns 1 after
 * the pick even if TryAddEffectTarget refused it. B goes back to step 0. */
int EffectDustTornadoChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: one shared zero for the step-0 return and the B branch (see EffectBlockAttackChainB). */
    int zero = 0;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentSpellTrapToDestroy);
        link->numTargets = 0;
        (*step)++;
        return zero;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_SPELL_TRAP)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/*
 * ChainB of Earthshaker: the player declares two attributes, the opponent picks one of them.
 *   Step 0: duel prompt PROMPT_SELECT_TWO_ATTRIBUTES for the player.
 *   Steps 1-4: only advance.
 *   Step 5: PROMPT_PICK_ONE_OF_TWO_ATTRIBUTES for the opponent, with the two declared attributes.
 *   Then: targets[0] = the picked attribute + 1 (enum CardAttribute); return 1.
 */
int EffectEarthshakerChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    u16 value; /* FAKEMATCH: shared narrow temporary matches the return paths. */

    switch (value = *step) {
    case 0:
        link->numTargets = 0;
        DuelPrompt_Post(link->player, PROMPT_SELECT_TWO_ATTRIBUTES, 0, 0);
        (*step)++;
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
        (*step)++;
        return 0;
    case 5:
        DuelPrompt_Post(1 - link->player, PROMPT_PICK_ONE_OF_TWO_ATTRIBUTES, gDuel.promptResult, PROMPT_RESULT2);
        (*step)++;
        return 0;
    default:
        AddEffectTarget(link, (value = gDuel.promptResult) + 1);
        return 1;
    }
}

/* ChainB of DNA Surgery and The Regulation of Tribe: step 0 asks for a monster type (PROMPT_SELECT_TYPE),
 * step 1 stores it + 1 (enum CardType) as targets[0], step 2 returns 1. */
int EffectDeclareTypeChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;

    switch (*step) {
    case 0:
        link->numTargets = 0;
        DuelPrompt_Post(link->player, PROMPT_SELECT_TYPE, 0, 0);
        break;
    case 1:
        AddEffectTarget(link, gDuel.promptResult + 1);
        break;
    default:
        return 1;
    }
    (*step)++;
    return 0;
}

/* ChainB of Nobleman of Crossout: prompt, then a pick among the face-down monsters of both players; returns
 * 1 after the pick even if TryAddEffectTarget refused it. B goes back to step 0. */
int EffectNoblemanOfCrossoutChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: one shared zero for the step-0 return and the B branch (see EffectBlockAttackChainB). */
    int zero = 0;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateFaceDownMonsterToBanish);
        link->numTargets = 0;
        (*step)++;
        return zero;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_MONSTER | PICK_ATTACK_POSITION
                                                     | PICK_DEFENSE_POSITION)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/* ChainB of Nobleman of Extermination: prompt, then a pick among the set Magic and Trap cards of both
 * players; returns 1 after the pick even if TryAddEffectTarget refused it. B goes back to step 0. */
int EffectNoblemanOfExterminationChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: one shared zero for the step-0 return and the B branch (see EffectBlockAttackChainB). */
    int zero = 0;

    if (*step == 0) {
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateFaceDownSpellTrapToBanish);
        link->numTargets = 0;
        (*step)++;
        return zero;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_DOWN_SPELL_TRAP)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/*
 * ChainB of Prohibition: declare any card by name. The duel screen is left for the Deck Edit card-list popup
 * and rebuilt afterwards.
 *   Step 0: prompt.
 *   Step 1: fade out the duel screen, then ResetVideo and clear targetWork / targetWork2 (the popup keeps its
 *           own step there).
 *   Step 2: run the popup (ProhibitCardSelect_Run) until done, then DuelScreen_Init.
 *   Step 3: fade the duel screen in.
 *   Step 4: show the chosen card (ShowPickedCard) and store its card ID (gMain.pickedCardId) as targets[0];
 *           return 1.
 */
int EffectProhibitionChainB(struct ChainEntry *link)
{
    switch (gChainBytes[TARGET_STEP]) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateCardToProhibit);
        gChainBytes[TARGET_STEP]++;
        return 0;
    case 1:
        if (DuelScreen_FadeOutStep() != 0) {
            ResetVideo();
            gChainBytes[TARGET_WORK] = 0;
            gChainBytes[TARGET_WORK2] = 0;
            gChainBytes[TARGET_STEP]++;
        }
        return 0;
    case 2:
        if (ProhibitCardSelect_RunInt() != 0) {
            DuelScreen_Init();
            gChainBytes[TARGET_STEP]++;
        }
        return 0;
    case 3:
        if (DuelScreen_FadeInStepInt() != 0) {
            gChainBytes[TARGET_STEP]++;
        }
        return 0;
    case 4: {
        u8 *main;
        u16 *pickedCardId;
        int player;
        link->numTargets = 0;
        player = link->player;
        /* Matching: the pointer is formed late, from a byte pointer and the offset. */
        main = (u8 *)&gMain;
        pickedCardId = (u16 *)(main + MAIN_PICKED_CARD_ID);
        ShowPickedCard(player, *pickedCardId);
        AddEffectTarget(link, *pickedCardId);
        gChainBytes[TARGET_STEP]++;
        return 1;
    }
    default:
        return 1;
    }
}

/*
 * ChainB of Riryoku: halve one monster's ATK and add it to another's. B in a pick goes back to step 0.
 *   Step 0: no target (return 1) unless EffectRiryokuPrepare allows it, else the 'reduce ATK by half' prompt.
 *   Step 1: a pick among face-up monsters; TryAddEffectTarget, then always advance.
 *   Step 2: the 'increase ATK' prompt.
 *   Step 3: a second pick; the first target again plays SE_ERROR, else TryAddEffectTarget and return 1
 *           (also when it refused: the resolve re-checks the targets).
 */
int EffectRiryokuChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;

    switch (*step) {
    case 0:
        link->numTargets = 0;
        if (EffectRiryokuPrepare3(link, 0, 0) == 0)
            return 1;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToHalveAtk);
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: the zero in a variable gives the ROM's movs r0, #0; strb; b end tail. */
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) == 0)
            return 0;
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        (*step)++;
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToIncreaseAtk);
        (*step)++;
        return 0;
    case 3:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: as in step 1. */
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_BOTH_SIDES(PICK_FACE_UP_MONSTER_ANY)) == 0)
            return 0;
        {
            u32 player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            u16 pos = (u8)player | (u8)zone << 8;
            if (link->targets[0] == pos) {
                PlaySE(SE_ERROR);
                return 0;
            }
            TryAddEffectTarget(link, player, zone);
        }
        return 1;
    default:
        return 1;
    }
}

/*
 * ChainB of key 1213 (no EDS card; OCG Mystic Box by behaviour): targets[0] is the opponent's monster to
 * destroy, targets[1] your monster to give away. B in a pick goes back to step 0.
 *   Step 0: prompt for the opponent's monster.  Step 1: a pick among the opponent's monsters.
 *   Step 2: prompt for your monster.            Step 3: a pick among your monsters.
 *   Steps 1 and 3 advance whether or not TryAddEffectTarget accepted the pick; step 4 returns 1.
 */
int EffectDestroyAndGiveMonsterChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;

    switch (*step) {
    case 0:
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentMonsterToDestroy);
        (*step)++;
        return 0;
    case 1:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: see EffectRiryokuChainB. */
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER)) == 0)
            return 0;
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        (*step)++;
        return 0;
    case 2:
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrDesignateMonsterToGiveControl);
        (*step)++;
        return 0;
    case 3:
        if (gMain.newKeys & B_BUTTON) {
            /* FAKEMATCH: see EffectRiryokuChainB. */
            int zero = 0;
            *step = zero;
            return zero;
        }
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) == 0)
            return 0;
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        (*step)++;
        return 0;
    default:
        return 1;
    }
}

/* ChainB of key 1220 (no EDS card): prompt, then a pick among the opponent's monsters to tribute; returns 1
 * after the pick even if TryAddEffectTarget refused it. B goes back to step 0. */
int EffectTributeOpponentMonsterChainB(struct ChainEntry *link)
{
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: one shared zero for the step-0 return and the B branch (see EffectBlockAttackChainB). */
    int zero = 0;

    if (*step == 0) {
        link->numTargets = 0;
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT,
                    gStrDesignateOpponentMonsterToTribute);
        (*step)++;
        return zero;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER)) != 0) {
        TryAddEffectTarget(link, gDuelScreen.selPlayer, gDuelScreen.selArea + gDuelScreen.selIndex);
        return 1;
    }
    return 0;
}

/* ChainB of key 1248 (no EDS card): prompt 'Designate one face-up Summoned Skull or Thunder monster on your
 * Field', then a pick among your face-up monsters that EffectOwnSkullOrThunderCheck accepts (else
 * SE_ERROR). B goes back to step 0. */
int EffectOwnSkullOrThunderChainB(struct ChainEntry *link)
{
    char prompt[0x100];
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;
    /* FAKEMATCH: the B branch stores and returns this shared zero (movs r0, #0; strb; b to the end). */
    int zero = 0;

    if (*step == 0) {
        {
            /* FormatStr(prompt, fmt, name of Summoned Skull). FAKEMATCH: the format, the name offset and the
             * name table are pinned to r1, r2 and r3, and the offset is added first, as in the ROM. */
            register const char *fmt __asm__("r1") = gStrDesignateFaceUpMonsterOfTwoFmt;
            u16 id = gCardNumberToId_SummonedSkull[0];
            register u32 nameOffset __asm__("r2");
            register const char *names __asm__("r3");

            nameOffset = id * CARD_NAME_SIZE;
            names = (const char *)gCardNames;
            FormatStr(prompt, fmt, (const char *)(nameOffset + (u32)names));
        }
        /* FAKEMATCH: a no-op on selArea that the ROM's code depends on. */
        gDuelScreen.selArea += 0;
        FormatStr(prompt, prompt, gStrThunderType);
        TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
        link->numTargets = 0;
        (*step)++;
    } else if (gMain.newKeys & B_BUTTON) {
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER_ANY) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (EffectOwnSkullOrThunderCheck(link, (u8)player | (u8)zone << 8) != 0) {
            TryAddEffectTarget(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* &gDuelZones[1 - link->player].zones[] + offset, adding the offset first. */
static inline struct DuelZone *ZoneFromOpponentOffset(struct ChainEntry *link, int offset)
{
    int product = ((1 - link->player) & 1) * sizeof(struct DuelZonesPlayer);

    /* FAKEMATCH: preserve the offset-first address sum without emitting code. */
    __asm__("" : : "r"(product));
    return (struct DuelZone *)(offset + product + (u32)gDuelZones);
}

/*
 * ChainB of key 1255 (no EDS card): take control of an opponent's Machine.
 *   Step 0: no target (return 1) unless one of the opponent's monster zones holds a face-up card whose
 *           GetZoneCardType is Machine; else the prompt 'Select an opponent's Machine monster ...'.
 *   Then: a pick among the opponent's face-up monsters; a non-Machine plays SE_ERROR. B goes back to step 0.
 */
int EffectTakeControlOfMachineChainB(struct ChainEntry *link)
{
    char prompt[0x100];
    u8 *chain = gChainBytes;
    u8 *step = chain + TARGET_STEP;

    if (*step == 0) {
        int zone;
        link->numTargets = 0;
        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            u32 shifted = (u32)((u8 *)link)[2] << 31;
            int offset;

            /* FAKEMATCH: load the player bit before multiplying the zone index. */
            __asm__("" : : "r"(shifted));
            offset = zone * sizeof(struct DuelZone);
            if (ZoneFromOpponentOffset(link, offset)->isFaceUp
                && (CARD_WORD(ZoneFromOpponentOffset(link, offset)->card) << 20) != 0
                && GetZoneCardType(1 - link->player, zone) == CARD_TYPE_MACHINE) {
                FormatStr(prompt, gStrSelectOpponentMonsterToControlFmt, gStrMachineType);
                TextBoxOpen(TARGET_PROMPT_POS, TARGET_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)prompt);
                gChainBytes[TARGET_STEP]++;
                return 0;
            }
        }
        return 1;
    } else if (gMain.newKeys & B_BUTTON) {
        /* FAKEMATCH: see EffectBlockAttackChainB. */
        int zero = 0;
        *step = zero;
        return zero;
    } else if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTER_ANY)) != 0) {
        u32 player = gDuelScreen.selPlayer;
        int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
        if (GetZoneCardType(player, zone) == CARD_TYPE_MACHINE) {
            TryAddEffectTarget(link, player, zone);
            return 1;
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}
