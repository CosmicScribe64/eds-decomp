#include "global.h"
#include "main.h"

#include "gba.h"

/* Dialogue text-box ("Bustup") module and Calendar scene helpers. */

/* One record of the dialogue table at 0x0813ADF4 (490 records + terminator). */
struct DialogueEntry {
    u16 eventId;
    u16 speakerId;
    char text[0x300];
};

#define DIALOGUE_COUNT 490

/* Script / text-box state at 0x02013DE0 (fields this unit touches). */
struct ScriptState {
    u8 filler0[0x9A4];
    u8 objCount:3;              /* +0x09A4 number of active animation objects */
    u8 filler9A5[0x9AC - 0x9A5];
    u8 textBox[0xAA8 - 0x9AC];  /* +0x09AC dialogue box / text state */
    u8 unkAA8[0x10C0 - 0xAA8];  /* +0x0AA8 sparkle line state */
    u8 objs[0x12E9 - 0x10C0];   /* +0x10C0 second animation-object array */
    u8 unk12E9;                 /* +0x12E9 index into gBustupBannerTiles (sparkle start x) */
    u8 unk12EA;                 /* +0x12EA index into gBustupOpponentIds */
    u8 speaker;                 /* +0x12EB current speaker character id */
    u8 filler12EC[0x12EE - 0x12EC];
    u16 unk12EE;                /* +0x12EE */
    u8 filler12F0[0x136C - 0x12F0];
    u16 dialogueIndex;          /* +0x136C current dialogue table index */
    u8 filler136E[0x136F - 0x136E];
    u8 unk136F;                 /* +0x136F */
    u8 filler1370[0x137C - 0x1370];
    u8 unk137C_0:1;             /* +0x137C bit 0 */
    u8 autoAdvance:1;           /* +0x137C bit 1: set every frame by "Auto Bustup" */
};

/* Calendar scene state at 0x0201F7D0. */
struct Calendar {
    u16 unk0;
    u16 date;               /* +0x02 packed date, see DayCountToDate */
    u32 shownEvents;        /* +0x04 event mask currently drawn */
    u16 blink:2;            /* +0x08 bits 0-1: setup/animation state (3 = steady) */
    u16 secondHalf:1;       /* +0x08 bit 2: showing weeks 3+ of the month */
    u16 page:1;             /* +0x08 bit 3: displayed Mode-4 frame (DISPCNT bit 4) */
    u16 weekday:3;          /* +0x08 bits 4-6: cursor column (day of week) */
    u16 week:3;             /* +0x08 bits 7-9: cursor row (week within the page) */
};

/* Unpacked date (DayCountToDate output). */
struct Date {
    u32 year:12;
    u32 month:4;
    u32 day:5;
};

/* Save image at 0x02011C20 (see wiki save-game). */
struct SaveData {
    u32 unk0;
    u8 flags4;              /* +0x04 bit 7: Shift-JIS text mode */
};

/* Calendar event names at 0x081980D4 (9 entries). */
struct CalendarEvent {
    u32 flags;              /* event bit(s) this entry is shown for */
    u8 name[0x40];
};

#define gMain gMain
extern struct ScriptState gBustup;
extern struct Calendar gCalendar;
extern struct SaveData gSaveData;
#define gSaveData gSaveData
extern const u16 gFontKanji10x10[]; /* 10px Shift-JIS font, 10 rows of u16 per glyph */
extern const u8 gFontLatin8x10[];  /* 10px ASCII font, 10 rows of u8 per glyph */
extern const struct CalendarEvent gCalendarEvents[];
extern const u8 gCalendarMonthNameTiles[]; /* 4 x 0x800 seasonal OBJ tiles */
#define gCalendar gCalendar

extern struct DialogueEntry gDialogueTable[];
typedef u16 (*StepFunc)(void);
extern StepFunc gBustupSteps[];
extern const u8 gCalendarBgEventPanel[];

extern const u8 gSceneSets[];
extern const u8 gUnk_081976B4[];
extern const u8 gUnk_081976C8[];
extern const u8 gUnk_081976DC[];
extern const u8 gUnk_081976F0[];
extern const u8 gUnk_08197704[];
extern const u8 gUnk_08197718[];
extern const u8 gUnk_0819772C[];
extern const u8 gUnk_08197740[];
extern const u8 gUnk_08197754[];
extern const u8 gUnk_08197768[];
extern const u8 gUnk_0819777C[];
extern const u8 gUnk_08197790[];
extern const u8 gUnk_081977A4[];
extern const u8 gUnk_081977B8[];
extern const u8 gUnk_081977CC[];
extern const u8 gUnk_081977E0[];
extern const u8 gUnk_081977F4[];
extern const u8 gUnk_08197808[];
extern const u8 gUnk_0819781C[];
extern const u8 gUnk_08197830[];
extern const u8 gUnk_08197844[];
extern const u8 gUnk_08197858[];
extern const u8 gUnk_0819786C[];
extern const u8 gUnk_08197880[];
extern const u8 gUnk_08197894[];
extern const u8 gUnk_081978A8[];
extern const u8 gUnk_081978BC[];
extern const u8 gUnk_081978D0[];
extern const u8 gUnk_081978E4[];
extern const u8 gUnk_081978F8[];

void Bustup_InitState(u32);
void MemCopy16(void *dest, const void *src, u32 size);
u16 SjisToGlyphIndex(u16 sjis);
void PlotPixel8bpp(u8 *dest, u32 color);
void DrawGlyph8bpp(u8 *dest, u32 color, u16 glyph);
void Calendar_DrawStringShadow(u32 x, u32 y, const u8 *str);
void Calendar_DrawEventNames(u32 mask);
void Calendar_ClearEventPanel(void);
void Calendar_FlipPage(void);
void DayCountToDate(struct Date *out, u16 date);
s32 GetDayOfWeek(u32 year, u32 month, s32 day);
s32 GetDaysInMonth(u32 year, u32 month);
u32 GetCalendarEvents(u32 year, u32 month, s32 day);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
void AddSprite8bpp(u32 yx, u16 shapeSize, u16 attr2);

/* Scene/script state at 0x020150CC (part of the 0x02013DE0 script area). */
struct Unk020150CC {
    u8 filler0[4];
    s16 unk4;               /* +0x04 scroll value */
    u8 unk6;                /* +0x06 box state (2 = settle) */
    u8 unk7;                /* +0x07 step delta */
    u8 filler8[0x80 - 0x08];
    u16 dialogueIndex;      /* +0x80 index into the dialogue table */
    u8 filler82[0x90 - 0x82];
    u8 unk90;               /* +0x90 flags (1 = started, 2 = advancing) */
};
extern struct Unk020150CC gBustupFade;
extern const u8 gUnk_0813ADF8[]; /* dialogue table text base (0x0813ADF4 + 4) */

void FadeStart(s32, s32, s32, void *);
void Bustup_ResetBlink(void);
void Bustup_ChangeSceneSet(const void *set, const u8 *text, void *tb, void *objs, u8 box);
const void *GetSceneSet(u32 charId);
void Bustup_LoadSceneSet(const void *set, const u8 *text, void *tb, void *objs, u8 box);
void Bustup_LoadSceneSetWithHeader(const void *set, const u8 *text, void *tb, void *objs, u8 box);
void Bustup_InitTextBox(void *tb);

/* Packed per-card state at gSaveData+0x20D0 (one u32 per entry; the entry
 * index comes from gBustupOpponentIds). */
struct CardState {
    u16 unk0;               /* +0x00: low 11 bits = first stat */
    u16 unk1;               /* +0x02: bits 6-15 = third stat */
};
extern const u16 gBustupBannerTiles[];
extern const u8 gBustupOpponentIds[];
struct Unk08080AD4 {
    u16 unk0, unk2, unk4, unk6, unk8, unkA;
};
extern const struct Unk08080AD4 gBustupRecordPos;
void Bustup_DrawLabel(u16 x, u16 y, u16 attr, u16 n);
void Bustup_DrawNumber(u16 value, u16 x, u16 y, u16 digits, u8 pal);
void Bustup_DrawOpponentRecord(s32 arg0);

/* Animation object (stride 0x14) used by the two script object arrays. */
struct AnimObj {
    u8 filler0[0xE];
    u8 unkE;                /* +0x0E active/redraw flag */
};

/* Text-box module state at 0x02014888. */
struct Unk02014888 {
    u8 filler0[0x842];
    s8 unk842;                          /* +0x842 */
    u8 filler843;
    u8 unk844[0x84A - 0x844];           /* +0x844 (== gBustupFade) */
    u8 unk84A;                          /* +0x84A */
    u8 filler84B[0x851 - 0x84B];
    u8 unk851;                          /* +0x851 */
};
extern struct Unk02014888 gBustupSprites;

/* Dialogue-box sparkle sub-state at 0x020147B4. */
struct Unk020147B4 {
    u8 filler0[0x91A];
    u16 unk91A;                         /* +0x91A */
    u8 filler91C[2];
    u8 unk91E;                          /* +0x91E */
    u8 filler91F[0x99B - 0x91F];
    s8 unk99B;                          /* +0x99B */
};
extern struct Unk020147B4 gBustupCursor;

extern const u8 gStrDebugDM5Script[];
extern const u8 gStrDebugMoveToScript[];

void AnimStateTick(void *obj);
void AnimBlockDraw(void *obj, s32, s32, s32, s32, s32, s32, s32, s32, void *);
void LineStep(void *);
void OamListFlush(void *);
void OamListClear(void *);
void Bustup_DrawCursorTrail(u16, s32);
void FadeTick(void *);
void PlaySE(s32);
void DebugPrintf(const u8 *, u32, u32, u32);
void DebugPrintFlush(void);
u16 GetDialogueEventId(u32);
u16 GetDialogueSpeaker(u32);
void Bustup_ShowPage(void *);
void Bustup_ClearHiddenBox(void *);
void Bustup_UpdateTextBox(void *);
void Bustup_TickBlink(void *);

/* Step 0 of the Bustup runner. */
u16 Bustup_Init(void)
{
    Bustup_InitState(0);
    return 1;
}

/* Draws the selected card's three stat counters (attack/defense/level) into
 * the dialogue box; optionally also draws a rank icon when arg0 is set. */
struct DuelRec08001374 {
    u32 wins : 11;
    u32 losses : 11;
    u32 draws : 10;
};
struct Save08001374 {
    u8 filler0[0x20D0];
    struct DuelRec08001374 records[32];
};
#define gSave08001374 (*(struct Save08001374 *)&gSaveData)

void Bustup_DrawOpponentRecord(s32 arg0)
{
    if ((u16)arg0 != 0)
        Bustup_DrawLabel(gBustupBannerTiles[gBustup.unk12E9], 0x43, 4, 1);
    Bustup_DrawNumber(gSave08001374.records[gBustupOpponentIds[gBustup.unk12E9 + gBustup.unk12EA * 5]].wins,
                 gBustupRecordPos.unk0, gBustupRecordPos.unk2, 3, 2);
    Bustup_DrawNumber(gSave08001374.records[gBustupOpponentIds[gBustup.unk12E9 + gBustup.unk12EA * 5]].losses,
                 gBustupRecordPos.unk4, gBustupRecordPos.unk6, 3, 2);
    Bustup_DrawNumber(gSave08001374.records[gBustupOpponentIds[gBustup.unk12E9 + gBustup.unk12EA * 5]].draws,
                 gBustupRecordPos.unk8, gBustupRecordPos.unkA, 3, 2);
}
/* Bustup step: re-initialises both script object arrays and the sparkle trail,
 * programs the horizontal-scroll registers from the dialogue state, and
 * finalises the dialogue box. */
struct Obj464 {
    u8 filler0[0xE];
    s8 unkE;
    u8 fillerF[0x14 - 0xF];
};
struct Scr464 {
    u8 filler0[0x9A4];
    u8 objCount:3;
    u8 filler9A5[0xAA8 - 0x9A5];
    u8 unkAA8[0x10C0 - 0xAA8];
    struct Obj464 objs[(0x12E9 - 0x10C0) / 0x14];
    u8 filler12DC[0x12E9 - 0x12DC];
    u8 unk12E9;
    u8 filler12EA[0x12EE - 0x12EA];
    u16 unk12EE;
    u8 filler12F0[0x136F - 0x12F0];
    u8 unk136F;
};
#define gScr464 (*(struct Scr464 *)&gBustup)
struct Main464 {
    u8 filler0[0x40C];
    vu16 intrCheck;
    u8 filler40E[0x4859 - 0x40E];
    u8 seqIndex1;
};
#define gMain464 (*(struct Main464 *)&gMain)

s32 Bustup_UnusedOpponentPreview(void)
{
    u8 i;
    u16 v;

    for (i = 0; i < gScr464.objCount; i++) {
        AnimStateTick(&gScr464.objs[i]);
        gScr464.objs[i].unkE = 0xFF;
    }
    gScr464.objs[gScr464.unk12E9].unkE = 0;
    for (i = 0; i < gScr464.objCount; i++) {
        if (gScr464.objs[i].unkE != -1)
            AnimBlockDraw(&gScr464.objs[i], 0, 0, 0, 1, 0, 0, 0, 0, gScr464.unkAA8);
    }
    LineStep(&gBustupCursor);
    LineStep(&gBustupCursor);
    LineStep(&gBustupCursor);
    Bustup_DrawCursorTrail(gBustupCursor.unk91A, 1);
    if (gBustupCursor.unk91E == 0)
        Bustup_DrawOpponentRecord(1);
    v = gBustupCursor.unk91A;
    switch (gBustupCursor.unk99B) {
    default:
        *(vu16 *)0x0400002C = v * 2;
        *(vu16 *)0x0400002E = ((v * 2) >> 16) & 0xFFF;
        break;
    case 1:
        *(vu16 *)0x04000028 = v * 2;
        *(vu16 *)0x0400002A = ((v * 2) >> 16) & 0xFFF;
        break;
    case 2:
        *(vu16 *)0x04000028 = -v * 2;
        *(vu16 *)0x0400002A = ((-v * 2) >> 16) & 0xFFF;
        break;
    }
    v = gScr464.unk12EE;
    if (gScr464.unk136F == 1) {
        *(vu16 *)0x04000028 = v * 2;
        *(vu16 *)0x0400002A = ((v * 2) >> 16) & 0xFFF;
    } else {
        *(vu16 *)0x04000028 = -v * 2;
        *(vu16 *)0x0400002A = ((-v * 2) >> 16) & 0xFFF;
    }
    OamListFlush(&gBustupSprites);
    OamListClear(&gBustupSprites);
    gMain464.intrCheck &= 0xFFFE;
    FadeTick(gBustupSprites.unk844);
    if (gBustupSprites.unk84A == 2) {
        if (gBustupSprites.unk842 == 4)
            gBustupSprites.unk851 = 1;
        else
            gBustupSprites.unk851 = 0;
        gMain464.seqIndex1 = 1;
        *(vu16 *)0x04000028 = 0;
        *(vu16 *)0x0400002A = 0;
        *(vu16 *)0x0400002C = 0;
        *(vu16 *)0x0400002E = 0;
    }
    return 0;
}
/* Bustup step: loads the current dialogue's scene (large box for a speaker at
 * 0x20-0x23, small box otherwise) and resets the dialogue state, then enables
 * forced-blank and Mode 4. */
s32 Bustup_LoadScene(void)
{
    u8 box = 1;
    s32 speaker;

    FadeStart(0, 0xFFFFFE80, 0, &gBustupFade);
    speaker = *((u8 *)&gBustupFade - 1);
    /* Adjacent cases preserve the ROM's signed lower/upper bound tests. */
    switch (speaker) {
    case 0x20: case 0x21: case 0x22: case 0x23:
        break;
    default:
        goto small;
    }
    Bustup_LoadSceneSetWithHeader(GetSceneSet(speaker),
                 gUnk_0813ADF8 + gBustupFade.dialogueIndex * 0x304,
                 (u8 *)&gBustupFade - 0x940,
                 (u8 *)&gBustupFade - 0x22C, box);
    goto done;
small:
    Bustup_LoadSceneSet(GetSceneSet(gBustup.speaker),
                 gUnk_0813ADF8 + gBustup.dialogueIndex * 0x304,
                 gBustup.textBox, gBustup.objs, box);
done:
    Bustup_ResetBlink();
    Bustup_InitTextBox(gBustup.textBox);
    *(vu16 *)0x04000000 = 0x1F04;
    return 1;
}
/* Bustup step: loads the selected dialogue as a 240x80 scene, then rewinds the
 * step runner by 4 (to re-run the setup). */
s32 Bustup_ChangeSpeaker(void)
{
    u8 box = 1;

    FadeStart(0, 0xFFFFFE80, 0, &gBustupFade);
    Bustup_ChangeSceneSet(GetSceneSet(*((u8 *)&gBustupFade - 1)),
                 gUnk_0813ADF8 + gBustupFade.dialogueIndex * 0x304,
                 (u8 *)&gBustupFade - 0x940,
                 (u8 *)&gBustupFade - 0x22C, box);
    Bustup_ResetBlink();
    gMain.seqIndex1 -= 4;
    return 0;
}
/* Bustup step: advances the dialogue (A button), handles the page/settle
 * states and text-box dirty flags, and rebuilds the object arrays. Returns 1
 * once the scene is finished (B / box fully read). */
/* TextBox (gBustup + 0x9AC) state byte (+0x19) and page byte (+0x20). */
#define TB_STATE_17E8 (*((s8 *)&gBustup + 0x9C5))
#define TB_PAGE_17E8 (*((u8 *)&gBustup + 0x9CC))
#define SPEAKER_17E8 (gBustup.speaker)
/* Scene/box state at gBustup + 0x12EC (== gBustupFade); +0x90 holds the
 * same bits as ScriptState unk137C_0 / autoAdvance. */
struct Anim17E8 {
    u8 filler0[4];
    s16 unk4;
    u8 unk6;
    u8 unk7;
    u8 filler8[0x80 - 0x08];
    u16 dialogueIndex;
    u8 filler82[0x90 - 0x82];
    u8 started:1;
    u8 autoAdvance:1;
};
#define gAnim17E8 (*(struct Anim17E8 *)((u8 *)&gBustup + 0x12EC))
/* FAKEMATCH: the ROM uses GetDialogueEventId's result without re-extending it, so it
 * is called through an int-returning type (the unit's prototype says u16). */
typedef u32 (*EventOfFunc17E8)(u32);
#define EventOf17E8 ((EventOfFunc17E8)GetDialogueEventId)
s32 Bustup_Update(void)
{
    u8 i;
    u8 *tb;
    u16 *boxDirty;
    u32 idx;

    FadeTick(&gAnim17E8);
    if (gAnim17E8.unk6 == 2)
        gMain.seqIndex1 += gAnim17E8.unk7;
    if (!(gAnim17E8.started)) {
        if (gMain.newKeys & 1) {
            switch (TB_STATE_17E8) {
            case 0:
                TB_STATE_17E8 = 1;
                break;
            case -1:
                TB_STATE_17E8 = 0;
                if (gAnim17E8.autoAdvance)
                    TB_STATE_17E8 = 1;
                PlaySE(1);
                break;
            case -2:
                if (!(gAnim17E8.autoAdvance)) {
                    if (gAnim17E8.unk4 == 0) {
                        FadeStart(0, 0x180, 1, &gAnim17E8);
                        PlaySE(1);
                        gAnim17E8.started = 1;
                    }
                } else {
                    gMain.seqIndex1--;
                    gAnim17E8.dialogueIndex++;
                    TB_PAGE_17E8 = 0;
                    gMain.speaker = SPEAKER_17E8;
                    /* FAKEMATCH: the int temporary makes the ROM reload the
                     * just-incremented index instead of reusing it. */
                    idx = gAnim17E8.dialogueIndex;
                    gMain.dialogueIndex = idx;
                    DebugPrintf(gStrDebugDM5Script, gMain.dialogueIndex,
                                 EventOf17E8(gMain.dialogueIndex), gMain.speaker);
                    DebugPrintFlush();
                }
                break;
            case -3:
                TB_STATE_17E8 = 3;
                PlaySE(1);
                break;
            }
        }
        if (gMain.newKeys & 2) {
            PlaySE(2);
            return 1;
        }
    } else if (gAnim17E8.autoAdvance) {
        gAnim17E8.started = 0;
        gMain.seqIndex1--;
        gAnim17E8.dialogueIndex++;
        SPEAKER_17E8 = GetDialogueSpeaker(gAnim17E8.dialogueIndex);
        TB_PAGE_17E8 = 0;
        gMain.speaker = SPEAKER_17E8;
        gMain.dialogueIndex = gAnim17E8.dialogueIndex;
        DebugPrintf(gStrDebugMoveToScript, gMain.dialogueIndex,
                     EventOf17E8(gMain.dialogueIndex), gMain.speaker);
        DebugPrintFlush();
        if (EventOf17E8(gMain.dialogueIndex) == 0) {
            gMain.speaker = 0;
            gMain.dialogueIndex = 0;
            PlaySE(2);
            return 1;
        }
    }
    Bustup_TickBlink(gBustup.objs);
    for (i = 0; i < gBustup.objCount; i++)
        AnimStateTick(gBustup.objs + i * 0x14);
    for (i = 0; i < gBustup.objCount; i++)
        AnimBlockDraw(gBustup.objs + i * 0x14, 0, 0, 0, 1, 0, 0, 0, 0,
                     gBustup.unkAA8);
    OamListFlush(&gBustupSprites);
    OamListClear(&gBustupSprites);
    tb = (u8 *)&gBustupSprites - 0xFC;
    Bustup_ClearHiddenBox(tb);
    gMain464.intrCheck &= 0xFFFE;
    boxDirty = (u16 *)((u8 *)&gBustupSprites - 0xDA);
    if (*boxDirty == 1) {
        Bustup_ShowPage(tb);
        *boxDirty = 0;
    }
    Bustup_UpdateTextBox(tb);
    return 0;
}

/* "Bustup" scene callback: runs the step table at 0x0813ADD4. */
u16 CB_Bustup(void)
{
    StepFunc step = gBustupSteps[gMain.seqIndex1];

    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    REG_DISPCNT &= 0xE0FF;
    return 1;
}

/* "Auto Bustup" scene callback: same, but forces auto-advance each frame. */
u16 CB_AutoBustup(void)
{
    if (gBustupSteps[gMain.seqIndex1] != NULL) {
        if (gBustupSteps[gMain.seqIndex1]())
            gMain.seqIndex1++;
        gBustup.autoAdvance = TRUE;
        return 0;
    }
    REG_DISPCNT &= 0xE0FF;
    return 1;
}

/* Maps a dialogue event ID to its table index (DIALOGUE_COUNT if not found). */
u32 GetDialogueIndex(u16 eventId)
{
    u32 i;

    for (i = 0; i <= DIALOGUE_COUNT; i++) {
        if (gDialogueTable[i].eventId == eventId)
            goto found;
    }
    i = DIALOGUE_COUNT;
found:
    return i;
}

/* Maps a dialogue table index to its event ID. */
u16 GetDialogueEventId(u32 index)
{
    if (index <= DIALOGUE_COUNT)
        return gDialogueTable[index].eventId;
    return 0;
}

/* Maps a dialogue table index to the speaker (portrait) character ID. */
u16 GetDialogueSpeaker(u32 index)
{
    if (index <= DIALOGUE_COUNT)
        return gDialogueTable[index].speakerId;
    return 0;
}

void StartDialogue(u16 eventId)
{
    u32 i;

    for (i = 0; i <= DIALOGUE_COUNT; i++) {
        if (gDialogueTable[i].eventId == eventId) {
            gMain.dialogueIndex = i;
            gMain.speaker = gDialogueTable[i].speakerId;
            gMain.seqIndex1 = 0;
            return;
        }
    }
}

/* Maps a character ID to its portrait ("bustup") descriptor. */
const void *GetSceneSet(u32 charId)
{
    switch (charId) {
    case 2:
        return gUnk_08197704;
    case 3:
        return gUnk_0819772C;
    case 4:
        return gUnk_08197754;
    case 5:
        return gUnk_08197740;
    case 16:
        return gUnk_0819777C;
    case 19:
        return gUnk_08197790;
    case 20:
        return gUnk_08197768;
    case 35:
        return gUnk_081976B4;
    case 24:
        return gUnk_081977A4;
    case 6:
        return gUnk_081977E0;
    case 7:
        return gUnk_081977F4;
    case 8:
        return gUnk_081977B8;
    case 9:
        return gUnk_08197808;
    case 10:
        return gUnk_081977CC;
    case 11:
        return gUnk_0819781C;
    case 12:
        return gUnk_08197830;
    case 13:
        return gUnk_08197858;
    case 14:
        return gUnk_081978D0;
    case 15:
        return gUnk_08197880;
    case 17:
        return gUnk_08197844;
    case 18:
        return gUnk_0819786C;
    case 21:
        return gUnk_08197894;
    case 22:
        return gUnk_081978A8;
    case 23:
        return gUnk_081978BC;
    case 37:
        return gUnk_081976C8;
    case 38:
        return gUnk_081978F8;
    case 39:
        return gUnk_081978E4;
    case 32:
        return gUnk_081976DC;
    case 33:
        return gUnk_081976F0;
    case 34:
        return gSceneSets;
    case 1:
    default:
        return gUnk_08197718;
    }
}

/* Write one 8bpp pixel into VRAM (which only takes 16-bit writes). */
void PlotPixel8bpp(u8 *dest, u32 color)
{
    if ((u32)dest & 1) {
        dest--;
        *(u16 *)dest = ((u8)color << 8) | *dest;
    } else {
        *(u16 *)dest = (u8)color | (*(u16 *)dest >> 8 << 8);
    }
}

/* Draw one glyph (10 rows of 1bpp pixels) into the text canvas `dest`, using
 * either the 8x10 ASCII font (1 byte per row) or the 8x10 Shift-JIS font (u16
 * per row, MSB first). */
void DrawGlyph8bpp(u8 *dest, u32 color, u16 glyph)
{
    s32 glyphOffset;
    register int fontOffset asm("r0");
    s32 scanline;
    s32 x;
    s32 nextScanline;
    u16 widePixels;
    u32 asciiMask;
    u32 wideMask;
    u8 *destLine;
    u8 *nextDestLine;
    u8 asciiPixels;
    destLine = dest;
    /* FAKEMATCH: preserve the entry copy and index-before-font scheduling.
     * These empty constraints emit no instructions and do not change values. */
    asm volatile ("" : "+r"(destLine));
    scanline = 0;
    glyphOffset = glyph * 0xA;
    do {
        if (0x80 & gSaveData.flags4) {
            fontOffset = (glyphOffset + scanline) * 2;
            asm volatile ("" : : "r"(fontOffset));
            widePixels = *(const u16 *)((const u8 *)gFontKanji10x10 + fontOffset);
            wideMask = 0x8000;
            widePixels = (widePixels >> 8) | ((u8)widePixels << 8);
            x = 0;
            nextDestLine = destLine + 0xF0;
            nextScanline = scanline + 1;
            do {
                if (widePixels & wideMask) {
                    PlotPixel8bpp(&destLine[x], color);
                }
                wideMask = wideMask >> 1;
                x += 1;
            } while (x <= 0xF);
        } else {
            fontOffset = glyphOffset + scanline;
            asm volatile ("" : : "r"(fontOffset));
            asciiPixels = gFontLatin8x10[fontOffset];
            asciiMask = 0x80;
            x = 0;
            nextDestLine = destLine + 0xF0;
            nextScanline = scanline + 1;
            do {
                if (asciiPixels & asciiMask) {
                    PlotPixel8bpp(&destLine[x], color);
                }
                asciiMask = asciiMask >> 1;
                x += 1;
            } while (x <= 7);
        }
        destLine = nextDestLine;
        scanline = nextScanline;
    } while (scanline <= 9);
}
void Calendar_DrawStringShadow(u32 x, u32 y, const u8 *str)
{
    u8 *dest;

    if (gCalendar.page)
        dest = (u8 *)VRAM;
    else
        dest = (u8 *)VRAM + 0xA000;
    dest += x;
    dest += y * 240;
    while (*str != 0) {
        if (gSaveData.flags4 & 0x80) {
            DrawGlyph8bpp(dest + 241, 0xFF, SjisToGlyphIndex((str[0] << 8) | str[1]));
            DrawGlyph8bpp(dest, 0xF7, SjisToGlyphIndex((str[0] << 8) | str[1]));
            dest += 10;
            str += 2;
        } else {
            DrawGlyph8bpp(dest + 241, 0xFF, *str);
            DrawGlyph8bpp(dest, 0xF7, *str);
            dest += 5;
            str++;
        }
    }
}
void Calendar_DrawEventNames(u32 mask)
{
    u32 x = 0x20;
    u32 y = 0x80;
    u32 count = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        if (gCalendarEvents[i].flags & mask) {
            Calendar_DrawStringShadow(x, y, gCalendarEvents[i].name);
            y += 12;
            if (++count == 2)
                return;
        }
    }
}

void Calendar_ClearEventPanel(void)
{
    if (gCalendar.page)
        MemCopy16((void *)0x06007080, gCalendarBgEventPanel, 0x2580);
    else
        MemCopy16((void *)0x06011080, gCalendarBgEventPanel, 0x2580);
}

void Calendar_FlipPage(void)
{
    gCalendar.page = 1 - gCalendar.page;
    if (gCalendar.page)
        REG_DISPCNT |= 0x10;
    else
        REG_DISPCNT &= ~0x10;
}

void Calendar_UpdateEventNames(void)
{
    struct Date d;
    s32 cell, col, row, day, daysInMonth;
    u32 events;

    DayCountToDate(&d, gCalendar.date);
    day = 1;
    cell = GetDayOfWeek(d.year, d.month, 1);
    col = cell;
    daysInMonth = GetDaysInMonth(d.year, d.month);
    row = 0;
    if (gCalendar.secondHalf) {
        while (cell <= 20) {
            cell++;
            day++;
            col++;
            col %= 7;
        }
        cell = 0;
    }
    while (day <= daysInMonth && cell <= 20) {
        if (col == gCalendar.weekday && row == gCalendar.week) {
            events = GetCalendarEvents(d.year, d.month, day) & 0x3F700000;
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
/* Sets the calendar cursor (weekday + week) to the day `arg0`, clamping to the
 * second half of the month. */
void Calendar_SetCursorDate(u16 arg0)
{
    struct Date d;
    s32 first;

    DayCountToDate(&d, arg0);
    gCalendar.weekday = GetDayOfWeek(d.year, d.month, d.day);
    first = GetDayOfWeek(d.year, d.month, 1);
    gCalendar.week = (u32)(d.day - 1 + first) / 7U;
    if (gCalendar.week > 2) {
        gCalendar.week -= 2;
        gCalendar.secondHalf = 1;
    } else {
        gCalendar.secondHalf = 0;
    }
}
void Calendar_DrawCursorAndHeader(void)
{
    struct Date d;
    s32 i;
    s32 x;
    u32 tileOffset;
    s32 season;

    AddSprite8bpp((gCalendar.weekday * 32 + 13) | ((gCalendar.week * 24 + 37) << 16), 0x80, 0x178);
    if (gCalendar.secondHalf)
        AddSprite8bpp(0x00080068, 0x4080, 0x190);
    else
        AddSprite8bpp(0x00700068, 0x4080, 0x194);
    for (i = 0; i <= 6; i++)
        AddSprite((25 << 16) | (i * 32 + 16), 0x4080, 0x82A0 + i * 4);

    DayCountToDate(&d, gCalendar.date);
    tileOffset = ((d.month - 1) % 3) * 10;
    switch (gCalendar.blink) {
    case 1:
        DayCountToDate(&d, gCalendar.date);
        season = (d.month - 1) / 3;
        MemCopy16((void *)0x06014C00, gCalendarMonthNameTiles + season * 0x800, 0x800);
    case 0:
    case 2:
        gCalendar.blink++;
        break;
    default:
        AddSprite(0x00080010, 0x4080, tileOffset + 0x7260);
        AddSprite(0x00080030, 0x4080, tileOffset + 0x7264);
        AddSprite(0x00080050, 0x40, tileOffset + 0x7268);
        break;
    }
}
