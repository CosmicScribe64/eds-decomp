---
title: Unit code_0803DD7C (duel target selectors)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit code_0803DD7C

`0x0803DD7C`-`0x0803EDC3`, Thumb, `old_agbcc -O2`. Source: `src/code_0803DD7C.c`. Follows [[code-0803b670]]. The functions here are not effect executors. They are the **target pickers** that the executors ([[code-08039638]], [[code-0803a654]]) call while resolving an effect. `int pick(struct CardRef *ref)` returns 1 when the targets are filled in and 0 while it is still waiting. `CardRef` is the one from [[code-08030b88]] with **three** target slots (`+0xC`, `+0xE`, `+0x10`), where a target is `zone << 8 | player` (see `AddEffectTarget`).

Unit status: `unit bytes MATCH`, **14/14 functions in C** since workflow wave 2 (2026-10-01), when the last fallback `EffectCardListTargetChainB` matched. Verified with `tools/check.py code_0803DD7C`.

## Shared headers (migrated 2026-09-30)
The unit includes `main.h` and `duel.h` and does not define its own copies of the shared layouts:
- `gMain` is `struct Main`; the cancel-key test reads `gMain.newKeys` (+0x06), replacing the local `struct MainView`/`h6`.
- `gDuelPlayers` is `struct DuelPlayer[2]` (`lifePoints` +0x00), replacing the local `struct PlayerLP` (`#define LP` still aliases it).
- `gDuelZones` is `struct DuelZonesPlayer[2]`, whose zones are the header's `struct DuelZone`; the face-down flag reads `flag6_1` (+0x06 bit 1) instead of the local byte `flags6 & 2`.

Five local struct definitions (`PlayerLP`, `MainView`, `DuelCard`, `DuelZone`, `DuelZonesPlayer`) and three local externs were removed. No local views were needed, because the canonical tags, fields and bitfields reproduce the ROM bytes exactly for every function. `struct CardRef`, `struct AE60`, `struct DuelScreenView` and `struct ListView` are unit-local (not in the shared headers) and stay.

## Shape shared by the pickers
- `if (1 & ((u8 *)ref)[2])` (player 1 = the AI): pick a target by itself (`AiFindStrongestMonster`/`AiFindWeakestMonster`, zone scans) and `return 1`.
- Player 0 uses a **step byte at `0x02017A40 + 0x3E5`** (not `+0x3E0`, which belongs to the executors). Step 0 clears `ref->numTargets` and shows a prompt (`TextBoxOpen(0x206, 0x712, 0xB, text)`). Step 1 waits. If `gMain.newKeys & 2` (`0x03000040+6`, cancel, hypothesis), it resets the step. Otherwise, if `DuelCursor_PickTarget(mask)` is true, it reads the cursor from the duel screen (`0x0201CFB0`: `+0x824` player, `+0x828 + 0x82C` zone, hypothesis) and calls `TryAddEffectTarget(ref, player, zone)`. `DuelCursor_PickTarget` is a key test, and the mask differs per picker (0xF0, 0xB0<<16, 0xA000A, 0xE000E0, 0x60006, 0xF000F0).

## Functions

| Address | Size | Status | Purpose (hypotheses about role, verified about logic) |
|---|---|---|---|
| `0x0803DD7C` | 0x30 | matching | `AddTarget(ref, v)`: `if (ref) ref->targets[ref->numTargets++] = v` |
| `0x0803DDAC` | 0x94 | matching | `AddTargetChecked(ref, player, zone) -> u16`: if `CanCardTargetZone(ref->id, player, zone)` accepts: `PlaySE(1)` unless player 1, message 8 (`\| 0x8000` for player 1) with the zone split into (`0/5/10` group, index) and `AddTarget(ref, zone << 8 \| player)`; returns 1, else 0 |
| `0x0803DE40` | 0x78 | matching | same as above without the acceptance test. `do { hi = 5; lo = z - 5; } while (0);` (permuter) fixes the ROM's hi r7 / lo r4 register order |
| `0x0803DEB8` | 0x12C | **matching** (wave 2, 2026-10-01) | list-viewer picker (`0x0201D810`): AI adds the first entry (`AiPickCardListEntry`); player 0 opens the viewer (`CollectEffectTargets`, `CardListView_Open`), then takes the chosen card word (id + two halves as two targets). The draft matches except that the AI arm scales `i<<2` into r4 instead of r1+base, and the default arm parks the list base in r5 (ROM r4) so `base+0xC` isn't reused after the call. `CARD_NUMBER` replaced by the constant-address form fixed the case 0/1 arms |
| `0x0803DFE4` | 0x17C | matching | spell/trap zones 5-9: AI takes the first face-down (flag 2) card of type 0x15, else the first occupied unflagged one; player 0: prompt `gStrDesignateTrapToDestroy`, keys 0xA000A |
| `0x0803E160` | 0xCC | matching | AI: `AiPickEffectTribute(-1)`; player 0: prompt `gStrDesignateOwnTribute`, keys 0xF0 |
| `0x0803E22C` | 0x198 | matching | AI: `CountMonsters(0) > 0` then `AiFindStrongestMonster(0, -1, 1, 1)`; player 0: per-card prompt text (card numbers 0x5E / 0x77, 0x2E6, 0x2DA, 0x536 select `gStrDesignateOpponentMonsterToReturn/D50/D90/DCC`), keys 0xF0 << 16 |
| `0x0803E3C4` | 0x1F4 | matching | AI: choose the side (the opponent for cards 0x416/0x417/0x58B/0x60C, or when the opponent has more life points for 0x290), then the monster zone with the best `GetZoneCardAtk` among those `EffectEquipTargetCheck` accepts; player 0: prompt `gStrDesignateMonsterToEquip`, keys 0xE000E0. Returns 1 at once for `(byte2 & 0xE) == 6` |
| `0x0803E5B8` | 0xA0 | matching | prompt `gStrDesignateMonsterForAttackPosition`, keys 0xB0 << 16, `TryAddEffectTarget` |
| `0x0803E658` | 0x84 | matching | AI: `AddTarget(ref, 0)`; player 0: prompt `gStrAskWhoseLpToRecover` + `TextBoxSetMenu(2, 0, 0)`, then adds `0x0201AE60+0x14` (a saved value, hypothesis) |
| `0x0803E6DC` | 0x21C | matching | 5-step machine (kind 2): first pick via `EffectBlastJugglerPrepare` / `EffectBlastJugglerCheck` (zone check) + `TryAddEffectTarget`, then a second pick that must differ from `targets[0]`; `CountMonsters(0) + CountMonsters(1) == 1` finishes at once; `PlaySE(3)` is played on a pick (a select sound, hypothesis) |
| `0x0803E8F8` | 0x1A4 | matching | twin of `0x0803DFE4` for zones 5-10 and type 0x16; player 0: keys 0x60006, prompt `gStrDesignateMagicToDestroy`, needs `CountSpellTrapsFiltered` for either side |
| `0x0803EA9C` | 0x114 | matching | first field position accepted by `EffectDragonSeekerCheck`: AI adds it, player 0 gets a prompt built with `FormatStr` into a stack buffer (`gStrDesignateTypeMonsterToDestroyFmt` + `gStrDragonType`) |
| `0x0803EBB0` | 0x214 | matching | `(ref, arg)`: AI with `CanActivateEffect(ref, arg, 0)`: per side `AiFindStrongestMonster` / `AiFindWeakestMonster`, else first occupied monster zone; player 0: per-card prompt (numbers 0x1F4/0x3FF, 0x21C), keys 0xF000F0, `CanCardTargetZone` + `TryAddEffectTarget` |

## Matching tricks
- **Step byte through a local base pointer.** `u8 *es = gChain; u8 *st = es + 0x3E5;` declared *after* the first `if` gives the ROM's two literals (`ldr =sym; ldr =0x3E5; adds`). `&gChain[0x3E5]` folds to one literal and is wrong. `es` must not be live across calls, or it lands in a callee-saved register.
- **`u8 z = 0; *st = z; return z;`** for a reset that ends `mov r0,#0; strb r0,[rX]; b epilogue`: the plain `*st = 0; return 0;` gets tail-merged with the shared `mov r0,#0`. (`0x0803E5B8`, `0x0803E6DC`, `0x0803EA9C`.)
- **`if (f(...) != 0) return 1; return 1;`** reproduces `bl f; lsls r0,#16` with the result unused, when `f` returns `u16` (`TryAddEffectTarget`). Give `TryAddEffectTarget` the return type `u16`, that is what the callers' `lsl/cmp` come from.
- **Same `sw` in the jump table with `goto`**: two arms that both end `q = base + 0x3E5; *q = 0` share one tail in the ROM; write `q = e2 + 0x3E5; goto reset;` in the first and `q = ...; reset: {..}` in the second (`0x0803E6DC`).
- **`u8 *base = (u8 *)&gDuelScreen; *(u32 *)(base + 0x824)` etc.** yields the ROM's unfolded `ldr =0x824; add; add r3,#4` chain for the cursor words; `gDuelScreen.w824` folded offsets give a different order (`0x0803E3C4`, `0x0803EA9C`). `int z = *(base+0x828) + *(base+0x82C); int p = *pa;` in that order.
- **`u32 id = (*(u32 *)z << 20) >> 20; CARD_TYPE((u16)id)`** (not `u16 id`) puts the zone pointer in r1 and the id in r2 as the ROM does while keeping the `ldr r3,=0x7FF; adds r0,r3,#0` form (`0x0803DFE4`, `0x0803E8F8`).
- **`pl = 1 & ((u8 *)ref)[2]; one = 1;`** (mem form, constant 1 in a variable afterwards) gives `adds r2,r4,#0; ands r2,r1` (constant register first); assigning `one` first or via a `b` local copies the byte register instead (`0x0803E22C`, `0x0803E3C4`). Read `ref->player` (bitfield) for later uses so the earlier `ldrb r1` is reused across the store to `ref+0xA`.
- **Card-number tables via constant address**: `((const u16 *)0x08622AB4)[0x7FF & ref->id]` keeps `ldr =0x7FF` before the table literal (with the symbol `gCardIdToNumber` the table literal is hoisted).
- **`switch` arm order = source order**: `case 0x290` before the `0x416/0x417/...` group, `0x1F4/0x3FF` before `0x21C`.
- **`do { a; b; } while (0);`** can be the difference in register allocation: in `0x0803DE40` wrapping `hi = 5; lo = z - 5;` in a `do{}while(0)` puts `hi` in r7 (`/* FAKEMATCH */`, a permuter result). A bare block does *not* work there. This same macro/func trick is what fixed `0x0803DEB8` case 0/1: the `CARD_NUMBER` macro over `gCardIdToNumber` hoists the table literal; the constant address `((const u16 *)0x08622AB4)[...]` does not.
- **`for (...; i++, side = 1 - side)`** (increment first, then flip) and `j = 0; sb = (u8)side; for (; j <= 4; ...)` (constant, then the shared byte) reproduce the AI loop of `0x0803E3C4`.

## Open problems

The last `INCLUDE_ASM` function, `EffectCardListTargetChainB`, matched in workflow wave 2 (see below); the unit has no assembly fallback left.

## Verified C conversions (2026-09-30)

- `EffectDragonSeekerChainB` (Thumb, `0x114` bytes) is matching C. It is the field-card selector. The AI takes an accepted position, and the human path builds a prompt and waits for input. The match computes the cursor sum before loading the player cursor, with an empty read-write asm on the sum, to retain r4/r5 and the ROM load order.

The compiler hint emits no instructions. The conversion passed a whole-unit byte comparison with the baserom, and the remaining assembly function keeps its original bytes. Earlier notes describing this function as an allocation near miss are resolved by the matching C above.

## List-viewer picker matched (wave 2, 2026-10-01)

`EffectCardListTargetChainB` (0x12C, start score 48) matches in ordinary C. Working notes: `build/wf/EffectCardListTargetChainB/NOTES.md`.

1. AI arm: the ROM computes `lsl r1,i,#2; add r4,r1,base`, but the draft tied the shift into r4. Fix: declare **one** `u16 *c` at function scope and use it in both the AI arm and the default arm. Because `c` spans several basic blocks, global-alloc allocates it (r4), and local-alloc gives the shift/sum temporary its own register, so the temporary is no longer tied to `c`.
2. Default arm: the ROM keeps the list base in r4 and `base+0xC` in r5 across the call, and reuses `base+0xC`. Fix: access `gCardListView.cards[gCardListView.top + gCardListView.row]` directly instead of through a `struct ListView *lv` local. The player then comes before the base literal, and CSE shares `base+0xC` across the call.
