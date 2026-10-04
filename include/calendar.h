#ifndef GUARD_CALENDAR_H
#define GUARD_CALENDAR_H

/*
 * Calendar: in-game date arithmetic, Japanese holidays and calendar events, and the Calendar screen
 * (main-menu slot 4).
 *
 * The game counts days from 2001-01-01 (day 0) in gSaveData.days; the Campaign moves it on by one day per
 * duel (Campaign_AdvanceDay). DayCountToDate unpacks a day count into a struct Date, and
 * GetCalendarEvents returns the CalendarEvent bits of a date, which decide the special duels, magazine
 * deliveries and rewards of the Campaign.
 *
 * Code: bustup_runner.c (screen drawing helpers), campaign_select.c (screen steps), main_menu.c (date
 * math, holidays), title_screen.c (events, day count conversion). Prototypes are the definitions as
 * compiled; a unit that calls a function through a different local prototype keeps that view (see
 * build/readability/proto_mismatches.txt).
 */

#include "global.h"

/*
 * Event bits of a date, as returned by GetCalendarEvents and stored in gMain.events. Bits 0-14 come from
 * GetHolidayFlags (the same bits as enum HolidayFlag); the names are those of the debug name table at
 * 0x08087720. The Calendar screen draws the names of the bits in 0x3F700000 (gCalendarEvents).
 */
enum CalendarEvent {
    CAL_NEW_YEARS_DAY = 0x1,              /* Jan 1 */
    CAL_FOUNDATION_DAY = 0x2,             /* Feb 11 */
    CAL_GREENERY_DAY = 0x4,               /* Apr 29 */
    CAL_CONSTITUTION_DAY = 0x8,           /* May 3 */
    CAL_CITIZENS_HOLIDAY = 0x10,          /* May 4 */
    CAL_CHILDRENS_DAY = 0x20,             /* May 5 */
    CAL_MARINE_DAY = 0x40,                /* Jul 20 */
    CAL_RESPECT_FOR_AGED_DAY = 0x80,      /* Sep 15 */
    CAL_CULTURE_DAY = 0x100,              /* Nov 3 */
    CAL_LABOR_THANKSGIVING_DAY = 0x200,   /* Nov 23 */
    CAL_EMPERORS_BIRTHDAY = 0x400,        /* Dec 23 */
    CAL_COMING_OF_AGE_DAY = 0x800,        /* 2nd Monday of January */
    CAL_SPORTS_DAY = 0x1000,              /* 2nd Monday of October */
    CAL_SPRING_DAY = 0x2000,              /* debug-table name; the date tested is Feb 24 (HOLIDAY_FEB_24) */
    CAL_AUTUMN_DAY = 0x4000,              /* debug-table name; the date tested is Jul 7 (HOLIDAY_JUL_7) */
    CAL_MURAN_BIRTHDAY = 0x8000,          /* Jun 28 */
    CAL_HALLOWEEN = 0x10000,              /* Oct 31 */
    CAL_CHRISTMAS_EVE = 0x20000,          /* Dec 24 */
    CAL_VALENTINES_DAY = 0x40000,         /* Feb 14 */
    CAL_WHITE_DAY = 0x80000,              /* Mar 14 */
    CAL_WEEKLY_JUMP = 0x100000,           /* Weekly magazine release: Tuesdays, earlier before days off */
    CAL_V_JUMP = 0x200000,                /* Monthly magazine release: the 21st, earlier before days off */
    CAL_DUEL_CEREMONY = 0x400000,         /* 2nd and 4th Saturday of each month */
    CAL_RARE_HUNTER = 0x800000,           /* not from GetCalendarEvents: Campaign_StartDay sets it every 60th
                                           * day when the player owns 5+ cards of gRareCardNumbers */
    CAL_TOURNAMENT_ROUND1 = 0x1000000,    /* 1st Sunday of November */
    CAL_TOURNAMENT_ROUND2 = 0x2000000,    /* 2nd Sunday of November, if gSaveData.tournamentRound >= 1 */
    CAL_TOURNAMENT_SEMIFINAL = 0x4000000, /* 3rd Sunday of November, if tournamentRound >= 2 */
    CAL_TOURNAMENT_FINAL = 0x8000000,     /* 4th Sunday of November, if tournamentRound >= 3 */
    CAL_SUGOROKU_PRELIM = 0x10000000,     /* 1st Saturday of June */
    CAL_SUGOROKU_MATCH = 0x20000000,      /* the next day, if gSaveData.sugorokuQualified */
};

/*
 * Holiday bits returned by GetHolidayFlags (Japanese public holidays of 2000-2002, without the equinox
 * days, plus two extra dates). IsDayOff tests the whole HOLIDAY_MASK. Same bits as CAL_* 0x1-0x4000.
 */
enum HolidayFlag {
    HOLIDAY_NEW_YEARS_DAY = 0x1,
    HOLIDAY_FOUNDATION_DAY = 0x2,
    HOLIDAY_GREENERY_DAY = 0x4,
    HOLIDAY_CONSTITUTION_DAY = 0x8,
    HOLIDAY_NATIONAL_HOLIDAY_MAY4 = 0x10,
    HOLIDAY_CHILDRENS_DAY = 0x20,
    HOLIDAY_MARINE_DAY = 0x40,
    HOLIDAY_RESPECT_FOR_AGED_DAY = 0x80,
    HOLIDAY_CULTURE_DAY = 0x100,
    HOLIDAY_LABOR_THANKSGIVING_DAY = 0x200,
    HOLIDAY_EMPERORS_BIRTHDAY = 0x400,
    HOLIDAY_COMING_OF_AGE_DAY = 0x800,
    HOLIDAY_SPORTS_DAY = 0x1000,
    HOLIDAY_FEB_24 = 0x2000,              /* purpose unknown; still a day off */
    HOLIDAY_JUL_7 = 0x4000,               /* purpose unknown; still a day off */
    HOLIDAY_MASK = 0x7FFF,                /* every holiday bit (IsDayOff) */
};

/* Day of the week: GetDayOfWeek result and struct Date.weekday. */
enum Weekday {
    WEEKDAY_SUNDAY = 0,
    WEEKDAY_MONDAY = 1,
    WEEKDAY_TUESDAY = 2,
    WEEKDAY_WEDNESDAY = 3,
    WEEKDAY_THURSDAY = 4,
    WEEKDAY_FRIDAY = 5,
    WEEKDAY_SATURDAY = 6,
};

/* A date unpacked from a day count by DayCountToDate (4 bytes; also used as a local by the debug menu). */
struct Date {
    u32 year:12;     /* bits 0-11: full year, 2001 and later */
    u32 month:4;     /* bits 12-15: 1-12 */
    u32 day:5;       /* bits 16-20: 1-31 */
    u32 weekday:3;   /* bits 21-23: enum Weekday */
};

/* gCalendar.monthNameState once the month-name tiles are loaded. */
#define MONTH_NAME_LOADED   3

/* gCalendar (0x0201F7D0, 0xC bytes): state of the Calendar screen, cleared by Calendar_Init. */
struct Calendar {
    u16 today;               /* +0x00 today's day count (gSaveData.days); that cell gets the today marker */
    u16 viewDate;            /* +0x02 a day count in the month on screen (R: 1st of the next month,
                              *       L: last day of the previous one) */
    u32 shownEvents;         /* +0x04 CalendarEvent bits (within 0x3F700000) of the cursor cell whose names
                              *       are drawn; redrawn only when the cell's bits differ */
    u16 monthNameState:2;    /* +0x08 bits 0-1: 0-2 = month-name tiles loading (copied at 1 by
                              *       Calendar_DrawCursorAndHeader), MONTH_NAME_LOADED; L/R wait for it */
    u16 secondHalf:1;        /* bit 2: showing rows 3-5 of the month (cells 21+) instead of rows 0-2 */
    u16 displayFrame:1;      /* bit 3: Mode-4 frame on screen (DISPCNT_FRAME_SELECT); drawing goes to the
                              *       other */
    u16 cursorCol:3;         /* bits 4-6: cursor column = enum Weekday */
    u16 cursorRow:3;         /* bits 7-9: cursor row 0-2 within the shown half */
};

/* One row of gCalendarEvents (0x081980D4): the name the Calendar shows for an event bit. */
struct CalendarEventEntry {
    u32 flag;        /* +0x00 CalendarEvent bit(s) this name is shown for */
    char name[0x40]; /* +0x04 display name */
};

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char calendar_h_check_date[sizeof(struct Date) == 0x4 ? 1 : -1];
typedef char calendar_h_check_cal[sizeof(struct Calendar) == 0xC ? 1 : -1];
typedef char calendar_h_check_cal_events[(u32)&((struct Calendar *)0)->shownEvents == 0x4 ? 1 : -1];
typedef char calendar_h_check_entry[sizeof(struct CalendarEventEntry) == 0x44 ? 1 : -1];
typedef char calendar_h_check_entry_name[(u32)&((struct CalendarEventEntry *)0)->name == 0x4 ? 1 : -1];

/* Calendar screen state (0x0201F7D0). Written by the Calendar steps and drawing helpers. */
extern struct Calendar gCalendar;

/* 0x08198628: days per month, January first (February 28; GetDaysInMonth adds the leap day). */
extern const u8 gDaysPerMonth[];

/* --- Date arithmetic (main_menu.c, title_screen.c) --- */

/* 1 if year is a Gregorian leap year, else 0. */
u32 IsLeapYear(u32 year);
/* Number of days in month (1-12) of year, from gDaysPerMonth plus the leap day. */
u32 GetDaysInMonth(u32 year, u32 month);
/* Weekday (enum Weekday) of a date, counted from 2000-01-01 (a Saturday) in 1461-day cycles. */
s32 GetDayOfWeek(u32 year, u32 month, u32 day);
/* 1-based week of the month, (day - 1) / 7 + 1. Unreferenced out-of-line copy of an inline helper. */
u32 GetWeekOfMonth(u32 year, u32 month, u32 day);
/* Unpacks a day count (day 0 = 2001-01-01) into *date. From day 36524 (2101-01-01) the count is moved on by
 * one: the 1461-day cycles would give 2100, which is not a leap year, a 366th day. */
void DayCountToDate(struct Date *date, u16 days);
/* Today's in-game date: DayCountToDate(date, gSaveData.days). */
void GetCurrentDate(struct Date *date);

/* --- Holidays and calendar events --- */

/* HolidayFlag bits of a date (fixed dates, plus Coming-of-Age and Sports Day on the 2nd Monday). */
u32 GetHolidayFlags(u32 year, u32 month, u32 day);
/* 1 for a Sunday, a holiday, or a Monday after a Sunday holiday (substitute holiday), else 0. */
u32 IsDayOff(u32 year, u32 month, u32 day);
/* CalendarEvent bits of a date: holidays, seasonal days, tournaments (gated by save progress), Duel
 * Ceremonies and magazine release days. */
u32 GetCalendarEvents(u32 year, u32 month, u32 day);

/* --- Calendar screen steps (gCalendarSteps, run by CB_Calendar) --- */

/* Scene callback (main-menu slot 4): runs gCalendarSteps[gMain.seqState2]; returns 1 at the table's end. */
u16 CB_Calendar(void);
/* Step 0: clears gCalendar, sets today = viewDate = gSaveData.days, sets up Mode 4 and loads the graphics. */
u16 Calendar_Init(void);
/* Step 1: draws the month and fades in; returns 1 when the fade is done. */
u16 Calendar_FadeIn(void);
/* Step 2: R/L change the month, the D-pad moves the cursor (UP/DOWN at the edge switch halves); A or B
 * leave. */
u16 Calendar_HandleInput(void);
/* Step 3: fades to black; returns 1 when done (the table then ends and the main menu returns). */
u16 Calendar_FadeOut(void);

/* --- Calendar screen drawing --- */

/* Per-frame sprites: frame, cursor and headers, the year, and the day cells of the shown half-month with
 * their event icons and the today marker. */
void Calendar_DrawMonth(void);
/* Fixed sprites: day cursor, half-month arrow, weekday headers and the month name (loading its tiles). */
void Calendar_DrawCursorAndHeader(void);
/* Puts the cursor on day count date: column = weekday, row = week of the month (rows 3+ in the second
 * half). */
void Calendar_SetCursorDate(u16 date);
/* Redraws the event names when the cursor cell's event bits differ from gCalendar.shownEvents. */
void Calendar_UpdateEventNames(void);
/* Shows the other Mode-4 frame: toggles gCalendar.displayFrame and DISPCNT bit 4. */
void Calendar_FlipPage(void);
/* Restores the event panel (rows 120-159) of the background into the hidden Mode-4 frame. */
void Calendar_ClearEventPanel(void);
/* Draws the names of up to two events whose bit is in eventMask at (32,128) and (32,140) of the hidden
 * frame. */
void Calendar_DrawEventNames(u32 eventMask);
/* Draws str (ASCII or 2-byte Shift-JIS) at (x, y) of the hidden Mode-4 frame with a drop shadow (colour
 * 0xFF at +1,+1, then 0xF7). */
void Calendar_DrawStringShadow(u32 x, u32 y, const u8 *str);

#endif /* GUARD_CALENDAR_H */
