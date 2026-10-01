---
title: Debug menu (unused) CB_DebugMenu `sub_08074A34`
type: function
status: draft
confidence: high
sources: [rom-analysis]
updated: 2026-10-01
---
# Debug menu (dead code)

| Function | Address | Size | Proposed name |
|---|---|---|---|
| runner | `0x08074A34` | 0x4C | `CB_DebugMenu` |
| steps | `0x08074554`, `0x08074794`, `0x08074868`, `0x080749E8` | | table `0x081A768C`, index `gMain+0x4857` |

Mode: Thumb; unit: `asm/code_080740BC.s`; match: nonmatching.

## Findings (verified)
- **Nothing references it.** A search of the whole ROM for the pointer `0x08074A35` and for `bl` to `0x08074A34` finds nothing, so it is a leftover developer menu.
- The last step (`0x080749E8`) sets `DISPCNT = 0` and calls `SetMainCallback(sDebugMenuItems[gMain.byte4859].callback)`. It then clears `gMain+0x4857` and `0x02017A30+0xB`.
- The item table **`sDebugMenuItems` @ `0x081A73A0`** is an array of `struct { char name[0x40]; u16 (*callback)(void); }` (stride 0x44):

| # | Name | Callback | What it is |
|---|---|---|---|
| 0 | `Menu` | `0x08003AA5` | main menu ([[main-menu]]) |
| 1 | `Card Detail` | `0x0800736D` | card detail viewer (runner, table `0x08198D50`) |
| 2 | `Auto Detail` | `0x080073BD` | auto-cycling card detail |
| 3 | `Bustup` | `0x08001AE5` | dialogue text-box runner with a portrait (step table `0x0813ADD4`, see [[text-system]]) |
| 4 | `Auto Bustup` | `0x08001B35` | auto-advancing script runner |
| 5 | `Get all card` | `0x08074481` | cheat: give every card |
| 6 | `Get a pack` | `0x08063AF9` | booster pack opening (runner, table `0x081A572C`; see [[booster-packs]]) |
| 7 | `Next Level` | `0x0807448D` | cheat: advance level |
| 8 | `License` | `0x08004EAD` | boot sequence ([[license-sequence]]) |
| 9 | `TITLE` | `0x080057BD` | title ([[title-screen]]) |

- Other functions in the same area are referenced from nearby tables (`0x081A7300` holds `0x0806EE35`, `0x081A7328` holds `0x08001AE5`), so the debug scenes themselves may be reused by the real game (for example "Get a pack" by the Campaign reward step).
- Row labels are drawn by `0x080746F0`, which reads the names from the table (4 references to `0x081A73A0`).

This table is also the source of several scene names on [[program-flow]].

Related: [[program-flow]], [[set-main-callback]].
