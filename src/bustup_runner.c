/*
 * Bust-up dialogue runner and Calendar screen drawing helpers.
 *
 * First half: CB_Bustup / CB_AutoBustup and the steps of gBustupSteps (enum BustupStep), the gDialogueTable
 * lookups and the speaker -> scene set mapping. The rest of the dialogue module (scene loading, the text
 * printer, sprites) is in bustup_scene.c.
 * Second half: the Calendar screen's event-name panel, drawn with a 10-px font straight into the hidden
 * Mode-4 frame, and its cursor/header sprites. The Calendar steps are in campaign_select.c.
 */
#include "global.h"
#include "legacy/gba.h"                /* REG_DISPCNT, REG16, VRAM, OBJ_VRAM0, A_BUTTON, B_BUTTON */
#include "legacy/main.h"               /* gMain */
#include "constants/game.h"     /* enum DuelistId */
#include "constants/sound.h"    /* enum SoundEffect */
#include "util.h"               /* MemCopy16, struct Line, LineStep */
#include "palette.h"            /* struct Fade, FadeStart, FadeTick */
#include "sprite.h"             /* struct OamList, struct AnimState, AddSprite*, AnimStateTick, AnimBlockDraw */
#include "text.h"               /* gFontKanji10x10, gFontLatin8x10, SjisToGlyphIndex */
#include "save.h"               /* gSaveData */
#include "legacy/sound.h"              /* (the new sound.h declares PlaySE; see below) */
#include "debug.h"              /* DebugPrintFlush */
#include "calendar.h"           /* gCalendar, struct Date, the date functions, Calendar_* */
#include "bustup.h"             /* gBustup and its aliases, enum BustupStep, GetSceneSet, Bustup_* */

/* ---- Names the legacy headers lack (until H0 installs the new gba.h and sound.h) ---- */

/* Values as in the new gba.h; inert once it is installed. */
#ifndef DISPCNT_MODE_4
#define DISPCNT_MODE_4      0x0004
#define DISPCNT_BG_ALL_ON   0x0F00
#define DISPCNT_OBJ_ON      0x1000
#endif
/* Not in the new gba.h either (build/readability/issues/bustup_runner.md). */
#ifndef DISPCNT_FRAME_SELECT
#define DISPCNT_FRAME_SELECT 0x0010     /* Mode 4/5: display frame 1 (VRAM + 0xA000) */
#endif
#ifndef REG_BG2X_L
/* The ROM writes the 28-bit BG2 reference point registers (REG_BG2X / REG_BG2Y) as two halfwords. */
#define REG_BG2X_L REG16(0x028)
#define REG_BG2X_H REG16(0x02A)
#define REG_BG2Y_L REG16(0x02C)
#define REG_BG2Y_H REG16(0x02E)
#endif

/* The legacy sound.h does not declare PlaySE (the new one does, with this prototype). */
void PlaySE(u32 seId);

/* ---- ROM data used only here ---- */

/* 0x0813ADD4: the steps of CB_Bustup, indexed by gMain.seqIndex1 (enum BustupStep); NULL ends the run.
 * The steps return u16 or s32; the runner reads the u16. */
typedef u16 (*BustupStepFunc)(void);
extern BustupStepFunc const gBustupSteps[];
/* 0x0813ADF4: DIALOGUE_COUNT records plus a terminator (event and speaker 0xFFFF, empty text). */
extern const struct DialogueEntry gDialogueTable[];
/* 0x081976A0: the 31 scene sets (wiki/data/scene-sets.md). */
extern const struct SceneSet gSceneSets[];
/* 0x08080AB6: first OBJ tile of the 128x16 banner of each preview slot (0-4). */
extern const u16 gBustupBannerTiles[];
/* 0x08080AE0: duelist id (enum DuelistId) of [page * 5 + slot] in the unused opponent preview; 0xFF = empty. */
extern const u8 gBustupOpponentIds[];
/* 0x08080AD4: screen positions of the wins, losses and draws counters: (69,40), (104,40), (139,40). */
struct BustupRecordPos {
    u16 x;
    u16 y;
};
extern const struct BustupRecordPos gBustupRecordPos[3];
/* 0x08080A64 / 0x08080A84: "DM5:Script=%3d(%5d) / BG = %d\n" and "Move to :Script=%3d(%5d) / BG = %d\n". */
extern const char gStrDebugDM5Script[];
extern const char gStrDebugMoveToScript[];

/* 0x081980D4: the 9 event names of the Calendar panel. */
extern const struct CalendarEventEntry gCalendarEvents[];
/* 0x087F1798: rows 120-159 (the event panel) of the Calendar background bitmap (0x087EA718 + 120 * 240). */
extern const u8 gCalendarBgEventPanel[];
/* 0x087F5DF8: month-name label tiles, 4 blocks of 0x800 (one per season, three 80-px labels each). */
extern const u8 gCalendarMonthNameTiles[];

/* ---- Local views (matching choices, see build/readability/HEADERS.md) ---- */

/* Bustup_Init passes an argument (0) that Bustup_InitState ignores. */
extern void Bustup_InitStateArg(u32 unused) asm("Bustup_InitState");
/* The unused preview step passes two arguments (the fade level and 1) that Bustup_DrawCursorTrail ignores. */
extern void Bustup_DrawCursorTrailArgs(u16 level, s32 one) asm("Bustup_DrawCursorTrail");
/* Bustup_Update uses the event id as a full register, without the u16 re-extension of the real prototype. */
extern u32 GetDialogueEventIdU32(u32 index) asm("GetDialogueEventId");
/* Bustup_UnusedOpponentPreview and Bustup_Update clear gMain.intrCheck bit 0 through a volatile struct member
 * (two ldrh for `&=`, base and offset formed separately); the legacy main.h declares it u16. With the new
 * main.h (vu16 intrCheck), `gMain.intrCheck &= ~1` compiles to the same code: drop this view after H0. */
struct MainIntrCheckView {
    u8 pad[0x40C];
    vu16 intrCheck;     /* +0x40C = gMain.intrCheck */
};
#define gMainIntrCheck ((*(struct MainIntrCheckView *)&gMain).intrCheck)

/*
 * Several steps address gBustup members from one of its alias symbols (bustup.h: gBustupFade = &gBustup.fade,
 * gBustupSprites = &gBustup.sprites, gBustupCursor = &gBustup.textBox.cursor), so the ROM builds those
 * addresses from the alias. BUSTUP_VIA(alias, aliasMember, type, member) is gBustup.member (read as `type`)
 * addressed from `alias`, which is &gBustup.aliasMember.
 */
#define BUSTUP_VIA(alias, aliasMember, type, member)                                                         \
    (*(type *)((u8 *)&(alias) + ((s32)OFFSET_OF(struct BustupState, member)                                  \
                                 - (s32)OFFSET_OF(struct BustupState, aliasMember))))
#define VIA_FADE(type, member)    BUSTUP_VIA(gBustupFade, fade, type, member)
#define VIA_SPRITES(type, member) BUSTUP_VIA(gBustupSprites, sprites, type, member)
#define VIA_CURSOR(type, member)  BUSTUP_VIA(gBustupCursor, textBox.cursor, type, member)

/* ---- Bust-up runner steps ---- */

/* BUSTUP_STEP_INIT: clears the bust-up state. Returns 1 (next step). */
u16 Bustup_Init(void)
{
    Bustup_InitStateArg(0);
    return 1;
}

/* Draws the wins, losses and draws against the opponent in preview slot gBustup.selectSlot of page
 * gBustup.selectPage as 3-digit sprites (palette 2); if drawBanner (only its low 16 bits are tested), first
 * that slot's banner at (67, 4). Only called by the unreachable preview step. */
void Bustup_DrawOpponentRecord(s32 drawBanner)
{
    if ((u16)drawBanner != 0)
        Bustup_DrawLabel(gBustupBannerTiles[gBustup.selectSlot], 67, 4, 1);
    Bustup_DrawNumber(gSaveData.duelRecords[gBustupOpponentIds[gBustup.selectSlot + gBustup.selectPage * 5]].wins,
                      gBustupRecordPos[0].x, gBustupRecordPos[0].y, 3, 2);
    Bustup_DrawNumber(gSaveData.duelRecords[gBustupOpponentIds[gBustup.selectSlot + gBustup.selectPage * 5]].losses,
                      gBustupRecordPos[1].x, gBustupRecordPos[1].y, 3, 2);
    Bustup_DrawNumber(gSaveData.duelRecords[gBustupOpponentIds[gBustup.selectSlot + gBustup.selectPage * 5]].draws,
                      gBustupRecordPos[2].x, gBustupRecordPos[2].y, 3, 2);
}

/*
 * BUSTUP_STEP_UNUSED_OPPONENT (unreachable: nothing sets seqIndex1 to 4). A leftover opponent preview: shows
 * only the animation track of the selected slot, moves the cursor sparkle three steps along its line and
 * draws it, draws the opponent's record while the fade is idle, and slides BG2 by the fade level. When the
 * fade has gone out it records whether the last page is selected and goes to BUSTUP_STEP_LOAD_SCENE.
 * Returns 0.
 */
s32 Bustup_UnusedOpponentPreview(void)
{
    u8 i;
    u16 offset;

    for (i = 0; i < gBustup.animCount; i++) {
        AnimStateTick(&gBustup.anims[i]);
        gBustup.anims[i].active = ANIM_HIDDEN;
    }
    gBustup.anims[gBustup.selectSlot].active = ANIM_FINISHED;   /* drawn, not ticked */
    /* AnimBlockDraw is given one state as the block, as in Bustup_Update. */
    for (i = 0; i < gBustup.animCount; i++) {
        if ((s8)gBustup.anims[i].active != (s8)ANIM_HIDDEN)
            AnimBlockDraw((u8 *)&gBustup.anims[i], 0, 0, 0, 1, 0, 0, 0, 0, (u32)&gBustup.sprites);
    }
    LineStep(&gBustupCursor);
    LineStep(&gBustupCursor);
    LineStep(&gBustupCursor);
    Bustup_DrawCursorTrailArgs(VIA_CURSOR(u16, fade.level), 1);
    if (VIA_CURSOR(u8, fade.state) == FADE_STATE_IDLE)
        Bustup_DrawOpponentRecord(TRUE);

    /* Slide BG2 by level / 128 px (0-32; the reference point has 8 fraction bits): BG2X +offset for
     * slideDir 1, -offset for 2, else BG2Y +offset. */
    offset = VIA_CURSOR(u16, fade.level);
    switch (VIA_CURSOR(s8, slideDir)) {
    default:
        REG_BG2Y_L = offset * 2;
        REG_BG2Y_H = ((offset * 2) >> 16) & 0xFFF;
        break;
    case 1:
        REG_BG2X_L = offset * 2;
        REG_BG2X_H = ((offset * 2) >> 16) & 0xFFF;
        break;
    case 2:
        REG_BG2X_L = -offset * 2;
        REG_BG2X_H = ((-offset * 2) >> 16) & 0xFFF;
        break;
    }
    /* Then BG2X again, overwriting the write above: +offset for slideDir 1, else -offset. */
    offset = gBustup.fade.level;
    if (gBustup.slideDir == 1) {
        REG_BG2X_L = offset * 2;
        REG_BG2X_H = ((offset * 2) >> 16) & 0xFFF;
    } else {
        REG_BG2X_L = -offset * 2;
        REG_BG2X_H = ((-offset * 2) >> 16) & 0xFFF;
    }

    OamListFlush(&gBustupSprites);
    OamListClear((u8 *)&gBustupSprites);
    gMainIntrCheck &= ~1;
    FadeTick(&VIA_SPRITES(struct Fade, fade));
    if (VIA_SPRITES(u8, fade.state) == FADE_STATE_FADED_OUT) {
        /* unk12F9 = the last page (row 4 of gBustupOpponentIds) is selected. */
        if (VIA_SPRITES(s8, selectPage) == 4)
            VIA_SPRITES(u8, unk12F9) = 1;
        else
            VIA_SPRITES(u8, unk12F9) = 0;
        gMain.seqIndex1 = BUSTUP_STEP_LOAD_SCENE;
        REG_BG2X_L = 0;
        REG_BG2X_H = 0;
        REG_BG2Y_L = 0;
        REG_BG2Y_H = 0;
    }
    return 0;
}

/*
 * BUSTUP_STEP_LOAD_SCENE: starts the fade-in from black and loads the speaker's scene set with the current
 * record's text: speakers 32-35 have the 240x80 scene sets (0, 1, 3, 4) drawn under the header strip, the
 * others a 240x96 set. Then resets the blink timer, initialises the text box and turns on Mode 4 with
 * BG0-3 and OBJ. Returns 1.
 */
s32 Bustup_LoadScene(void)
{
    u8 box = 1;     /* gDialogueBoxGfx[1]; as a variable it sits in a callee-saved register, as in the ROM */
    s32 speaker;

    FadeStart(FADE_BLACK, -0x180, 0, &gBustupFade);
    speaker = VIA_FADE(u8, speaker);
    /* A switch, not two compares: the ROM tests the range as `>= 0x20` and `<= 0x23`. */
    switch (speaker) {
    case 32: case 33: case 34: case 35:
        Bustup_LoadSceneSetWithHeader(GetSceneSet(speaker),
                                      gDialogueTable[VIA_FADE(u16, dialogueIndex)].text,
                                      &VIA_FADE(struct BustupTextBox, textBox),
                                      &VIA_FADE(struct AnimState, anims), box);
        break;
    default:
        Bustup_LoadSceneSet(GetSceneSet(gBustup.speaker), gDialogueTable[gBustup.dialogueIndex].text,
                            &gBustup.textBox, gBustup.anims, box);
        break;
    }
    Bustup_ResetBlink();
    Bustup_InitTextBox(&gBustup.textBox);
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return 1;
}

/*
 * BUSTUP_STEP_CHANGE_SPEAKER: the `$b` text code set gBustup.speaker and moved the runner from
 * BUSTUP_STEP_UPDATE to here. Fades in the new speaker's scene set (keeping the box and the text position)
 * and goes back to BUSTUP_STEP_UPDATE. Returns 0.
 */
s32 Bustup_ChangeSpeaker(void)
{
    u8 box = 1;     /* as in Bustup_LoadScene */

    FadeStart(FADE_BLACK, -0x180, 0, &gBustupFade);
    Bustup_ChangeSceneSet(GetSceneSet(VIA_FADE(u8, speaker)),
                          gDialogueTable[VIA_FADE(u16, dialogueIndex)].text,
                          &VIA_FADE(struct BustupTextBox, textBox),
                          &VIA_FADE(struct AnimState, anims), box);
    Bustup_ResetBlink();
    gMain.seqIndex1 -= BUSTUP_STEP_CHANGE_SPEAKER - BUSTUP_STEP_UPDATE;
    return 0;
}

/*
 * BUSTUP_STEP_UPDATE, every frame. When the fade has gone out, adds fade.param to the step (1: to
 * BUSTUP_STEP_END). While autoNext is clear, A acts on the text printer (print faster, continue after `$c`,
 * fade out at the end of the text, next page) and B skips the rest of the dialogue. autoNext is set when
 * that fade-out starts, or by the text printer when a record ends in auto mode; in auto mode the next
 * gDialogueTable record is then loaded, and the run ends after the terminator. Then ticks the blink and the
 * portrait animation, draws the sprites, shows a finished page and runs the text printer.
 * Returns 1 when the dialogue is over (B, or the end of the table in auto mode), else 0.
 */
s32 Bustup_Update(void)
{
    u8 i;
    struct BustupTextBox *tb;
    u16 *boxDirty;

    FadeTick(&gBustup.fade);
    if (gBustup.fade.state == FADE_STATE_FADED_OUT)
        gMain.seqIndex1 += gBustup.fade.param;
    if (!gBustup.autoNext) {
        if (gMain.newKeys & A_BUTTON) {
            switch (gBustup.textBox.state) {
            case TEXT_STATE_PRINT:
                gBustup.textBox.state = TEXT_STATE_FAST;
                break;
            case TEXT_STATE_WAIT_BUTTON:
                gBustup.textBox.state = TEXT_STATE_PRINT;
                if (gBustup.autoAdvance)
                    gBustup.textBox.state = TEXT_STATE_FAST;
                PlaySE(SE_CONFIRM);
                break;
            case TEXT_STATE_END:
                if (!gBustup.autoAdvance) {
                    /* Once the fade-in is over: fade out, then BUSTUP_STEP_END (param 1). */
                    if (gBustup.fade.step == 0) {
                        FadeStart(FADE_BLACK, 0x180, 1, &gBustup.fade);
                        PlaySE(SE_CONFIRM);
                        gBustup.autoNext = TRUE;
                    }
                } else {
                    /* Auto mode: back to BUSTUP_STEP_LOAD_SCENE with the next record, same speaker. */
                    gMain.seqIndex1--;
                    gBustup.dialogueIndex++;
                    gBustup.textBox.page = 0;
                    gMain.speaker = gBustup.speaker;
                    gMain.dialogueIndex = gBustup.dialogueIndex;
                    DebugPrintf(gStrDebugDM5Script, gMain.dialogueIndex,
                                GetDialogueEventIdU32(gMain.dialogueIndex), gMain.speaker);
                    DebugPrintFlush();
                }
                break;
            case TEXT_STATE_BOX_FULL:
                gBustup.textBox.state = TEXT_STATE_NEXT_PAGE;
                PlaySE(SE_CONFIRM);
                break;
            }
        }
        if (gMain.newKeys & B_BUTTON) {
            PlaySE(SE_CANCEL);
            return 1;
        }
    } else if (gBustup.autoAdvance) {
        /* Auto mode reached the end of the record: back to BUSTUP_STEP_LOAD_SCENE with the next record
         * and its own speaker. */
        gBustup.autoNext = FALSE;
        gMain.seqIndex1--;
        gBustup.dialogueIndex++;
        gBustup.speaker = GetDialogueSpeaker(gBustup.dialogueIndex);
        gBustup.textBox.page = 0;
        gMain.speaker = gBustup.speaker;
        gMain.dialogueIndex = gBustup.dialogueIndex;
        DebugPrintf(gStrDebugMoveToScript, gMain.dialogueIndex,
                    GetDialogueEventIdU32(gMain.dialogueIndex), gMain.speaker);
        DebugPrintFlush();
        /* GetDialogueEventId gives 0 only past the terminator record (index > DIALOGUE_COUNT; the terminator
         * itself, event 0xFFFF, still plays as an empty record): the run is over. */
        if (GetDialogueEventIdU32(gMain.dialogueIndex) == 0) {
            gMain.speaker = 0;
            gMain.dialogueIndex = 0;
            PlaySE(SE_CANCEL);
            return 1;
        }
    }
    Bustup_TickBlink(gBustup.anims);
    for (i = 0; i < gBustup.animCount; i++)
        AnimStateTick(&gBustup.anims[i]);
    /* Each state is passed as if it were the block. The block's count (+0x190) is gBustup.animsCount only
     * for i = 0, so that call draws every track; the others read the count from the padding after it. */
    for (i = 0; i < gBustup.animCount; i++)
        AnimBlockDraw((u8 *)&gBustup.anims[i], 0, 0, 0, 1, 0, 0, 0, 0, (u32)&gBustup.sprites);
    OamListFlush(&gBustupSprites);
    OamListClear((u8 *)&gBustupSprites);
    tb = &VIA_SPRITES(struct BustupTextBox, textBox);
    Bustup_ClearHiddenBox(tb);
    gMainIntrCheck &= ~1;
    boxDirty = &VIA_SPRITES(u16, textBox.boxDirty);
    if (*boxDirty == 1) {
        Bustup_ShowPage(tb);
        *boxDirty = 0;
    }
    Bustup_UpdateTextBox(tb);
    return 0;
}

/* ---- Runner ---- */

/* Scene callback: runs gBustupSteps[gMain.seqIndex1], moving to the next step when it returns non-zero.
 * At the NULL step it hides BG0-3 and OBJ and returns 1 (dialogue over); else 0. */
u16 CB_Bustup(void)
{
    BustupStepFunc step = gBustupSteps[gMain.seqIndex1];

    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    return 1;
}

/* CB_Bustup with gBustup.autoAdvance set every frame: fast text, and the records play back to back. */
u16 CB_AutoBustup(void)
{
    if (gBustupSteps[gMain.seqIndex1] != NULL) {
        if (gBustupSteps[gMain.seqIndex1]())
            gMain.seqIndex1++;
        gBustup.autoAdvance = TRUE;
        return 0;
    }
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    return 1;
}

/* ---- Dialogue table ---- */

/* Index of the first gDialogueTable record with this event id, or DIALOGUE_COUNT if there is none. Unused. */
u32 GetDialogueIndex(u16 eventId)
{
    u32 i;

    for (i = 0; i <= DIALOGUE_COUNT; i++) {
        if (gDialogueTable[i].eventId == eventId)
            return i;
    }
    return DIALOGUE_COUNT;
}

/* gDialogueTable[index].eventId, or 0 if index > DIALOGUE_COUNT. */
u16 GetDialogueEventId(u32 index)
{
    if (index <= DIALOGUE_COUNT)
        return gDialogueTable[index].eventId;
    return 0;
}

/* gDialogueTable[index].speakerId (the portrait character), or 0 if index > DIALOGUE_COUNT. */
u16 GetDialogueSpeaker(u32 index)
{
    if (index <= DIALOGUE_COUNT)
        return gDialogueTable[index].speakerId;
    return 0;
}

/* Selects the dialogue of an event for CB_Bustup: gMain.dialogueIndex, gMain.speaker and step 0. Does
 * nothing if no record has this event id. */
void StartDialogue(u16 eventId)
{
    u32 i;

    for (i = 0; i <= DIALOGUE_COUNT; i++) {
        if (gDialogueTable[i].eventId == eventId) {
            gMain.dialogueIndex = i;
            gMain.speaker = gDialogueTable[i].speakerId;
            gMain.seqIndex1 = BUSTUP_STEP_INIT;
            return;
        }
    }
}

/* The scene set of a speaker (character id: enum DuelistId for 1-24; 32-39 are scene-only ids). Unknown
 * ids and Yugi give set 6. */
const struct SceneSet *GetSceneSet(u32 charId)
{
    switch (charId) {
    case DUELIST_TEA:
        return &gSceneSets[5];
    case DUELIST_JOEY:
        return &gSceneSets[7];
    case DUELIST_TRISTAN:
        return &gSceneSets[9];
    case DUELIST_BAKURA:
        return &gSceneSets[8];
    case DUELIST_KAIBA:
        return &gSceneSets[11];
    case DUELIST_YAMI_BAKURA:
        return &gSceneSets[12];
    case DUELIST_YAMI_YUGI:
        return &gSceneSets[10];
    case 35:
        return &gSceneSets[1];
    case DUELIST_GRANDPA:
        return &gSceneSets[13];
    case DUELIST_REX:
        return &gSceneSets[16];
    case DUELIST_ESPA_ROBA:
        return &gSceneSets[17];
    case DUELIST_WEEVIL:
        return &gSceneSets[14];
    case DUELIST_MAKO:
        return &gSceneSets[18];
    case DUELIST_MAI:
        return &gSceneSets[15];
    case DUELIST_RARE_HUNTER:
        return &gSceneSets[19];
    case DUELIST_ARKANA:
        return &gSceneSets[20];
    case DUELIST_STRINGS:
        return &gSceneSets[22];
    case DUELIST_UMBRA_LUMIS:
        return &gSceneSets[28];
    case DUELIST_MARIK:
        return &gSceneSets[24];
    case DUELIST_ISHIZU:
        return &gSceneSets[21];
    case DUELIST_SHADI:
        return &gSceneSets[23];
    case DUELIST_DUEL_COMPUTER:
        return &gSceneSets[25];
    case DUELIST_SIMON:
        return &gSceneSets[26];
    case DUELIST_PEGASUS:
        return &gSceneSets[27];
    case 37:
        return &gSceneSets[2];
    case 38:    /* Umbra alone */
        return &gSceneSets[30];
    case 39:    /* Lumis alone */
        return &gSceneSets[29];
    case 32:
        return &gSceneSets[3];
    case 33:
        return &gSceneSets[4];
    case 34:    /* the duel arena */
        return &gSceneSets[0];
    case DUELIST_YUGI:
    default:
        return &gSceneSets[6];
    }
}

/* ---- 10-px font drawing into a Mode-4 bitmap (Calendar event panel) ---- */

/* Writes one 8bpp pixel through a 16-bit read-modify-write (VRAM ignores byte writes). */
void PlotPixel8bpp(u8 *dest, u32 color)
{
    if ((u32)dest & 1) {
        dest--;
        *(u16 *)dest = ((u8)color << 8) | *dest;
    } else {
        *(u16 *)dest = (u8)color | (*(u16 *)dest >> 8 << 8);
    }
}

/* Plots the set pixels of one 10-row 1bpp glyph into a 240-px-wide 8bpp bitmap at dest: 16-px rows of the
 * Shift-JIS font (big-endian u16 per row) in 2-byte text mode, else 8-px rows of the Latin font. */
void DrawGlyph8bpp(u8 *dest, u32 color, u16 glyph)
{
    s32 firstRow;
    /* FAKEMATCH: pinning the font offset to r0 keeps the index arithmetic before the font-base load. */
    register int fontOffset asm("r0");
    s32 row;
    s32 x;
    s32 nextRow;
    u16 kanjiBits;
    u32 latinMask;
    u32 kanjiMask;
    u8 *line;
    u8 *nextLine;
    u8 latinBits;

    line = dest;
    /* FAKEMATCH: this empty constraint (no instructions) keeps the ROM's entry copy of dest. */
    asm volatile ("" : "+r"(line));
    row = 0;
    firstRow = glyph * 10;
    do {
        if (gSaveData.sjisText) {
            /* 16 px from the big-endian u16 row, MSB first (read through a byte offset, as the ROM does). */
            fontOffset = (firstRow + row) * 2;
            kanjiBits = *(const u16 *)((const u8 *)gFontKanji10x10 + fontOffset);
            kanjiMask = 0x8000;
            kanjiBits = (kanjiBits >> 8) | ((u8)kanjiBits << 8);
            x = 0;
            nextLine = line + 240;
            nextRow = row + 1;
            do {
                if (kanjiBits & kanjiMask) {
                    PlotPixel8bpp(&line[x], color);
                }
                kanjiMask = kanjiMask >> 1;
                x += 1;
            } while (x <= 15);
        } else {
            /* 8 px from the byte row, MSB first. */
            fontOffset = firstRow + row;
            latinBits = gFontLatin8x10[fontOffset];
            latinMask = 0x80;
            x = 0;
            nextLine = line + 240;
            nextRow = row + 1;
            do {
                if (latinBits & latinMask) {
                    PlotPixel8bpp(&line[x], color);
                }
                latinMask = latinMask >> 1;
                x += 1;
            } while (x <= 7);
        }
        line = nextLine;
        row = nextRow;
    } while (row <= 9);
}

/* ---- Calendar screen ---- */

/* Draws str (1-byte characters, or 2-byte Shift-JIS in 2-byte text mode) at (x, y) of the hidden Mode-4
 * frame: each glyph in colour 0xFF one pixel down and right (the shadow), then in 0xF7 on top. */
void Calendar_DrawStringShadow(u32 x, u32 y, const u8 *str)
{
    u8 *dest;

    if (gCalendar.displayFrame)
        dest = (u8 *)VRAM;              /* frame 1 is shown: draw into frame 0 */
    else
        dest = (u8 *)VRAM + 0xA000;     /* frame 1 */
    dest += x;
    dest += y * 240;
    while (*str != 0) {
        if (gSaveData.sjisText) {
            DrawGlyph8bpp(dest + 240 + 1, 0xFF, SjisToGlyphIndex((str[0] << 8) | str[1]));
            DrawGlyph8bpp(dest, 0xF7, SjisToGlyphIndex((str[0] << 8) | str[1]));
            dest += 10;
            str += 2;
        } else {
            DrawGlyph8bpp(dest + 240 + 1, 0xFF, *str);
            DrawGlyph8bpp(dest, 0xF7, *str);
            dest += 5;
            str++;
        }
    }
}

/* Draws the names of the first two gCalendarEvents entries whose flag is in eventMask, at (32, 128) and
 * (32, 140) of the hidden frame. */
void Calendar_DrawEventNames(u32 eventMask)
{
    u32 x = 32;
    u32 y = 128;
    u32 count = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        if (gCalendarEvents[i].flag & eventMask) {
            Calendar_DrawStringShadow(x, y, (const u8 *)gCalendarEvents[i].name);
            y += 12;
            if (++count == 2)
                return;
        }
    }
}

/* Restores the event panel (rows 120-159) of the background into the hidden Mode-4 frame. */
void Calendar_ClearEventPanel(void)
{
    if (gCalendar.displayFrame)
        MemCopy16((void *)(VRAM + 120 * 240), gCalendarBgEventPanel, 40 * 240);
    else
        MemCopy16((void *)(VRAM + 0xA000 + 120 * 240), gCalendarBgEventPanel, 40 * 240);
}

/* Shows the other Mode-4 frame (the one just drawn into). */
void Calendar_FlipPage(void)
{
    gCalendar.displayFrame = 1 - gCalendar.displayFrame;
    if (gCalendar.displayFrame)
        REG_DISPCNT |= DISPCNT_FRAME_SELECT;
    else
        REG_DISPCNT &= ~DISPCNT_FRAME_SELECT;
}

/* The event bits whose names the panel shows (magazines, Duel Ceremony, tournaments, Sugoroku). */
#define CALENDAR_PANEL_EVENTS                                                                                \
    (CAL_WEEKLY_JUMP | CAL_V_JUMP | CAL_DUEL_CEREMONY | CAL_TOURNAMENT_ROUND1 | CAL_TOURNAMENT_ROUND2         \
     | CAL_TOURNAMENT_SEMIFINAL | CAL_TOURNAMENT_FINAL | CAL_SUGOROKU_PRELIM | CAL_SUGOROKU_MATCH)

/*
 * Finds the day under the cursor in the month on screen (cells 0-20 of the shown half; an empty cell
 * leaves the panel as it is). If its panel events differ from gCalendar.shownEvents, draws their names
 * into both Mode-4 frames.
 */
void Calendar_UpdateEventNames(void)
{
    struct Date date;
    s32 cell, col, row, day, daysInMonth;
    u32 events;

    DayCountToDate(&date, gCalendar.viewDate);
    day = 1;
    cell = GetDayOfWeek(date.year, date.month, 1);    /* the 1st is in row 0, column = its weekday */
    col = cell;
    daysInMonth = GetDaysInMonth(date.year, date.month);
    row = 0;
    if (gCalendar.secondHalf) {
        /* Skip the first three rows (cells 0-20). */
        while (cell <= 20) {
            cell++;
            day++;
            col++;
            col %= 7;
        }
        cell = 0;
    }
    while (day <= daysInMonth && cell <= 20) {
        if (col == gCalendar.cursorCol && row == gCalendar.cursorRow) {
            events = GetCalendarEvents(date.year, date.month, day) & CALENDAR_PANEL_EVENTS;
            if (gCalendar.shownEvents != events) {
                Calendar_ClearEventPanel();
                Calendar_DrawEventNames(events);
                Calendar_FlipPage();
                Calendar_DrawEventNames(events);
                gCalendar.shownEvents = events;
            }
            return;
        }
        col++;
        if (col > 6) {
            col = 0;
            row++;
        }
        day++;
        cell++;
    }
}

/* Puts the cursor on day count `date`: column = its weekday, row = its week of the month; rows 3 and
 * later are in the second half (rows - 2). */
void Calendar_SetCursorDate(u16 date)
{
    struct Date d;
    s32 firstWeekday;

    DayCountToDate(&d, date);
    gCalendar.cursorCol = GetDayOfWeek(d.year, d.month, d.day);
    firstWeekday = GetDayOfWeek(d.year, d.month, 1);
    gCalendar.cursorRow = (u32)(d.day - 1 + firstWeekday) / 7U;
    if (gCalendar.cursorRow > 2) {
        gCalendar.cursorRow -= 2;
        gCalendar.secondHalf = 1;
    } else {
        gCalendar.secondHalf = 0;
    }
}

/* AddSprite* position argument: y << 16 | x. */
#define SPRITE_YX(x, y) ((x) | ((y) << 16))

/*
 * Draws the fixed sprites: the day cursor (8bpp), the half-month arrow (8bpp; at the top in the second
 * half, else at the bottom), the seven weekday headers and the 80-px month-name label.
 * monthNameState: 1 copies the season's label tiles to OBJ tile 0x260; 0-2 count up; 3 draws the label.
 */
void Calendar_DrawCursorAndHeader(void)
{
    struct Date d;
    s32 i;
    u32 labelTile;
    s32 season;

    AddSprite8bpp(SPRITE_YX(gCalendar.cursorCol * 32 + 13, gCalendar.cursorRow * 24 + 37), SPRITE_SHAPE_32x32,
                  0x178);
    if (gCalendar.secondHalf)
        AddSprite8bpp(SPRITE_YX(104, 8), SPRITE_SHAPE_32x16, 0x190);
    else
        AddSprite8bpp(SPRITE_YX(104, 112), SPRITE_SHAPE_32x16, 0x194);
    for (i = 0; i <= 6; i++)
        AddSprite(SPRITE_YX(i * 32 + 16, 25), SPRITE_SHAPE_32x16, 0x82A0 + i * 4);    /* palette 8, tile 0x2A0 + 4i */

    DayCountToDate(&d, gCalendar.viewDate);
    labelTile = ((d.month - 1) % 3) * 10;     /* a season block holds three labels, 10 tiles wide each */
    switch (gCalendar.monthNameState) {
    case 1:
        DayCountToDate(&d, gCalendar.viewDate);
        season = (d.month - 1) / 3;
        MemCopy16((void *)(OBJ_VRAM0 + 0x260 * 32), gCalendarMonthNameTiles + season * 0x800, 0x800);
        /* fall through */
    case 0:
    case 2:
        gCalendar.monthNameState++;
        break;
    default:
        /* 32 + 32 + 16 px from OBJ tile 0x260 + labelTile, palette 7. */
        AddSprite(SPRITE_YX(16, 8), SPRITE_SHAPE_32x16, labelTile + 0x7260);
        AddSprite(SPRITE_YX(48, 8), SPRITE_SHAPE_32x16, labelTile + 0x7264);
        AddSprite(SPRITE_YX(80, 8), SPRITE_SHAPE_16x16, labelTile + 0x7268);
        break;
    }
}
