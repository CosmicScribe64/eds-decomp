---
title: Unit campaign_steps (Campaign steps: dialogue, deck setup, duel init, post-duel)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit campaign_steps

`0x0801BCFC`–`0x0801CE67`, Thumb, `old_agbcc -O2`. Source: `src/campaign_steps.c`.
Steps of the Campaign scene runner (see [[program-flow]], callback `0x0801D1E8`).

Unit status: `unit bytes MATCH`, 6/6 functions in C (verified with `tools/check.py campaign_steps`).

## Shared headers

This unit now uses the canonical shared layouts instead of its own copies:

- `#include "main.h"` provides `struct Main` / `gMain` (`gMain`). The canonical field names used are
  `step488A` (was local `step`), `counter4888` (was `counter`), `opponent` (was `unk4870_1`) and
  `events` (was `unk487C`); `seqIndexCampaign`, `seqState0`, `seqIndex1`, `seqState1`, `seqState2`,
  `rewardPack`, `rewardCard` and `score` kept their names.
- `#include "duel.h"` provides `struct DuelState` / `gDuel` (`result` at +0x1B12 bits 6-7 is unchanged).

Removed the unit's own `struct Main` and `struct DuelState` definitions (2 structs) plus their externs.

**Local view kept:** `struct MainFlags4888` (`#define gMainBits (*(struct MainFlags4888 *)&gMain)`),
because main.h splits `+0x4888` as `unk4888_0:4 / counter4888:2 / unk4888_6:2`, while this unit reads
bit 1 (`skipScript`) and bits 2-3 (`unk4888_2`) as separate bitfields. Using the canonical 4-bit
`unk4888_0` changes the generated bit extraction and stops `Campaign_SelectOpponent`/`Campaign_ShowDuelResult` matching, so a
unit-local view of that one byte is kept. `counter4888` (bits 4-5) is read through the canonical struct.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x0801BCFC` | 0x110 | matching | Campaign step: 3 sub-steps in `gMain+0x488A` bits 4-11. Sub-step 0 runs `OpponentSelect_Run` until done unless `gMain+0x4888` bit 1 is set; 1 calls `Campaign_StartPreDuelDialogue`; 2 waits for `CB_Bustup`. Clears `seqIndex1/seqState1/seqState2` on completion. Returns 1 once past sub-step 2 | `Campaign_Dialogue` (hypothesis) |
| `0x0801BE0C` | 0x4C | matching | Deck setup: `TurnOrder_RunRps()` if `gMain+0x4888 & 0x30` is 0, else `TurnOrder_RunPlayerChoice()` if duel result (`0x020192E0+0x1B12` bits 6-7) is 2, else `TurnOrder_RunCpuChoice()` | `Campaign_DeckSetup` (hypothesis) |
| `0x0801BE58` | 0x38 | matching | Duel init: `Duel_Setup`, clears `0x02015EE9` bit 0, `LoadPlayerDeckFromSave`, `ShuffleDeck(0,8)`, `LoadOpponentDeck(0)`, `ShuffleDeck(1,8)`; returns 1 | `Campaign_DuelInit` (hypothesis) |
| `0x0801BE90` | 0xF0 | matching | Counts how many of the 28 card IDs at `gPackDisplayOrder` pass `IsPackUnlocked` before/after calling `RecordDuelWin/77998/779E8(gMain+0x4870 bits 1-5)` per duel result 1/2/3 (win/loss/draw, hypothesis); sets `gSaveData+0x2164` bit 0 if `GetCampaignLevel()` grew, bit 1 if the count grew | |
| `0x0801BF80` | 0x9B8 | matching | Match rewards, 26 sub-steps (jump table). 0: `Campaign_RecordDuelResult`. 1: on a win, `switch (gMain+0x487C)` (one-hot, 30 values) either picks a booster pack for `rewardPack` (`gMain+0x4876`: 0x1FD, 5, 0x15, 0xB, 0x1F9, 0xC, 0x16, 0x17, 4, 0x1FA, 0x1F8, 0x386) and jumps to sub-step 0xF (`GetRewardPack(rewardPack)`), or for bits 24-27 shows text 0xC8/0xCA/0xCC/0xCE, bumps `gSaveData+0x215E`, and sets `rewardCard` (`gMain+0x4880`) to one of the three Ticket cards (`gUnk_08624CCE/D0/D2` = `&gCardNumberToId[1901..1903]`, see [[card-id-map]]) (sub-step 0xA: `AddCardToTrunk(card)`, then Card Detail `CardDetail_Init`); bit 27 resets `+0x215E`, bumps `+0x2162` and gives all three cards via `RemoveCardFromTrunk`. On a loss, for bit 23 (opponents 11-15), a random notable card `PickRandomOwnedRareCard()` is given and shown (sub-steps 0x16/0x17); other listed bits show text 0x12C. 2-3: if `gSaveData+0x2164` bit 1 (set by `Campaign_RecordDuelResult`) show text 0x15F. 4: `CB_GetPack()`. 0xB: `GetRewardPack(0x1FD)` | `Campaign_Rewards` (hypothesis) |
| `0x0801C938` | 0x530 | matching | Post-duel result text, 5 sub-steps. Skips opponents 0, 0x19, 0x1F. Sub-step 0: unless `gMain+0x4888` bits 2-3 == 1 (single duel?), updates a best-of-3 match: `score` (`gMain+0x4889`, s8) += 1 on win / -= 1 on loss, `counter` (bits 4-5) += 1; after duel 2 a score of ±2 decides, after duel 3 the sign decides (0 = draw). If the match is still open it shows `gOpponentResultTexts[opp].unk6/unk8` and skips two sub-steps with BGM 0x15. Otherwise picks the result text (`win`/`winAlt4`/`winAlt9` by `gSaveData+0x20D0[opp]` bits 0-10, `lose`, `draw`) and BGM 0x18/0x19/0x1C; if `gMain+0x487C == 0x800000` opponents 11-15 get fixed texts 0x2AF8.. / 0x2AFB... Sub-steps 2-4: set `skipScript`, wait `SideDeckSwap_Run`, show `gOpponentNextMatchDuelText[opp]`, then `seqIndexCampaign -= 3` (replay the match loop) | `Campaign_MatchResult` (hypothesis) |

## Data

- `gMain+0x4888` (u8) Campaign flags: bit 1 skips `OpponentSelect_Run`; bits 4-5 select the deck-setup path.
- `gMain+0x488A` bits 4-11: sub-step counter (u16 bitfield container).
- `gMain+0x4870` bits 1-5: argument to the post-duel reward helpers `RecordDuelWin` etc. (hypothesis: opponent index).
- `0x020192E0+0x1B12` bits 6-7: duel result, u8 bitfield: 1 win, 2 loss, 3 draw (hypothesis, from the score logic and BGM choice in `Campaign_ShowDuelResult`).
- `gMain+0x4889` (s8): match score; `gMain+0x4888` bits 4-5: duels played in the match; bits 2-3: match mode (1 = no best-of-3, hypothesis).
- `gMain+0x487C` (u32): today's calendar event flags (set from `GetCalendarEvents(date)` in [[campaign-c]] `Campaign_DeliverMagazines`); one-hot values switched on in `Campaign_GiveRewards` (bits 0-19 pick a booster pack, bit 23 is the special case for opponents 11-15, bits 24-27 give the three `gUnk_08624CCx` cards, and bits 28-29 give packs 0x1F8/0x386). Probably the current opponent/tournament selection (hypothesis).
- `gMain+0x4876` (u16) `rewardPack`, `gMain+0x4880` (u16) `rewardCard` (card ID, passed to the Card Detail view `CardDetail_Init` and to `RemoveCardFromTrunk`; hypothesis: `RemoveCardFromTrunk` adds the card to the trunk).
- `gSaveData+0x215E` (u16) counter of the bit-24..26 wins; `+0x2160` (u16) set to 1 on a bit-28 win; `+0x2162` (u8) bumped on a bit-27 win.
- `gUnk_08624CCE`, `gUnk_08624CD0`, `gUnk_08624CD2`: `gCardNumberToId[1901..1903]`, the IDs of the three Ticket cards ([[card-id-map]]); all three are given together on a bit-27 win.
- `gOpponentResultTexts`: per-opponent text IDs, 0x10 bytes (`win`, `lose`, `draw`, +6, +8, `winAlt4`, `winAlt9`). `gOpponentFirstMeetingText[opp]`, `gOpponentNextMatchDuelText[opp]`: u16 text IDs.
- `gSaveData+0x20D0`: 4-byte entry per opponent, bits 0-10 a state (4 and 9 select alternate win texts).
- `gSaveData+0x2164` (u16): bits 0/1 set in `Campaign_RecordDuelResult`.

## Matching tricks

- `Campaign_SelectOpponent`: put `return 0` inside every case and `return 1` after the switch; `default: return 1` + `break` puts the blocks in the wrong order.
- `Campaign_DecideTurnOrder`: `if (x == 2) return A(); else return B();` gives the ROM's `beq A` layout.
- `Campaign_ShowDuelResult`: a two-case `switch` compiled as a `cmp 2; beq; cmp 2; bgt; cmp 1` tree needs an empty `case 3: break;`. The early exit to the result block is `if (mode == 1) done = 1; else {...}` then `if (!done) {...}` (a `goto` hoists the base address differently). `if (result == 1)` compiles to `and #0xC0; cmp #0x40`; the ROM's `lsr #6; cmp #1` comes from `switch (result) { case 1: ... default: ... }`. Nested `switch (score) { case -2: case 2: }` for the ±2 test.
- `Campaign_GiveRewards`: a `switch` whose jump table ends in a `return 1` entry needs that case written out (`case 0x19: return 1;`), or the table is one entry short. Inside `if (x) { ...; step = N; }` add an explicit `return 0;` in the `if` body (cases 0xA/0xB): otherwise the `step = N` tail is not cross-jumped into the shared `orr; strh; b` block and registers shift. Declare callees whose result is tested without `lsl #16` as returning `u32` (`GetRewardPack`), and those with it as `u16` (`CardDetail_Run`). Reading `gMain.rewardCard` twice (no local) gives the ROM's `ldrh r4` + reuse.
- `Campaign_RecordDuelResult`: `for (i = 0, count = 0; ...)` orders the two `mov #0` like the ROM; initialising `count` in its declaration swaps them. Index the table as `tbl[i]` (not a walking pointer).
