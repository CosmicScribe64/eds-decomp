---
title: Unit summon_action (duel action execution and hand action setup)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-01
---
# Unit summon_action

Thumb, `old_agbcc -O2`, `0x08054E7C`–`0x08055EAF`. 9/9 functions in matching C, verified with `unit bytes MATCH`. The entire 0x1034-byte unit is now enabled. Names and gameplay interpretations are hypotheses.

| Address | Size | Status | Proposed name | Purpose |
|---|---|---|---|---|
| 0x08054E7C | 0x41C | matching C | ExecuteZoneAction (hyp.) | Four-step zone action: face-state message, cursor/card presentation, low-level card cancellation, then effect dispatch by card number |
| 0x08055298 | 0x118 | matching C | ExecuteCardAction (hyp.) | Three steps: message 0x77 with the copied card, presentation 0x71, resolution 0x90; number 0x4DE queues a packed 0xC4 action |
| 0x080553B0 | 0x200 | matching C | ExecuteCardChoiceAction (hyp.) | AI or human choice determines f15; copied card message, presentation and resolution; number 0x5F6 applies a helper to the other four zones |
| 0x080555B0 | 0x178 | matching C | ExecuteTributeHandAction (hyp.) | Optional three zone removals, message 0xC4, presentation, resolution; number 0x4DE queues packed 0xA4 action |
| 0x08055728 | 0x2D8 | matching C | UpdateQueuedAction (hyp.) | Dispatches kinds 1–6 or waits on link completion; clears active and runs post-action effects and response prompt |
| 0x08055A00 | 0xB4 | matching C | StartQueuedAction (hyp.) | Clears step/phase/auxiliary fields, sets active and player flag bit 5; CPU link action sends 20-byte command 0xF05A |
| 0x08055AB4 | 0x74 | matching C | ResetQueuedAction (hyp.) | Forces player 0, resets progress, sets active/ready, toggles bit 12 of the copied card |
| 0x08055B28 | 0x214 | matching C, initialized hints | QueueHandChoiceAction (hyp.) | Fills kind 1 from a hand card, unpacks two tribute bytes, selects flags/h0C using `IsToonMonster`, then queues |
| 0x08055D3C | 0x174 | matching C | QueueHandAction2 (hyp.) | Fills kind 2 from a hand card and two packed tribute bytes, then queues |

## Record and globals

The action record at `0x0201CF90` is 0x14 bytes, extending the prefix documented in [[summon-builders-c]]. Bits 31–46 hold a 16-bit card ID across a word boundary. Copied card word is +8; +0xC is a u16 action parameter.

At +0xE: bit 0 active, bit 1 ready/link-controlled, bits 2–4 kind, bits 5–11 step (7 bits), bits 12–15 phase. The +0x10 word has fields at bits 0–2, 3–10 and 11–18, all cleared by initialization. Their meanings remain unknown. The packed tribute flags are bits 25/26/27 of the first word, referring to 3-bit zone indices at bits 16/19/22.

> [!warning] Record size clarification
> [[summon-builders-c]] calls this a 16-byte record. That describes the prefix used by its builders; these reset routines access +0x10, and `SummonAction_Start` transmits 0x14 bytes. The complete record is 20 bytes.

Player array `0x020192E4`, stride 0xD64: byte +8 bit 5 marks a queued action. `0x02015EE8+1` bit 0 enables the CPU link-send path. `0x02017FB0+0x306` bit 7 is link completion. `0x0201AE60+0x14` is the menu selection used by the AI/human card choice.

## Matching notes

- Extending the record with actual bitfields gives the reset's byte/halfword/word accesses exactly, including a 7-bit step crossing the +0xE/+0xF bytes.
- A standalone `struct DuelCard *card = &r->card` makes `card->flag12` read the full 4-byte word, as in the ROM. Access through the enclosing record instead reads a byte and saves an extra register.
- Player flag +8 must be a bitfield (`queued = 1`), and indexing uses `r->player & 1`, to reproduce `SummonAction_Start`'s multiplication order and shared mask register.
- Repeating `r->step++; return 0;` within every case reproduces the merged tail and literal-pool placement in action step functions.
- Compute packed action in sequence: player shifted to bit 31, zone shifted to bit 16, OR command constant, OR card ID. This creates the ROM's temporary pressure and saved register allocation.
- For message flags, read f14 first, then f15 shifted by 1, then OR into the latter local; the equivalent single expression swaps temporary registers.
- `switch (number)` for 0x4DE/0x5F6 gives both comparisons before either case body; an `if/else if` moves the second comparison after the first body.
- The zone-effect state machine and tribute hand action now match; see the zone action and tribute action section below. The hand-choice builder is now matched; see the hand action builder section below.
- In `QueueNormalSummonChoosePosition`, direct global stores during initial setup, followed by a record pointer chosen in the tribute/non-tribute arms, reproduce the pointer moves and high-register allocation. Keeping one pointer from entry does not.
- In the matched dispatcher, first extract the card into a temporary, test zero, then copy into a new inner-scope `id` local; this gives the ROM's `r8` card ID and `r7` zone flag. Directly retaining the outer local swaps them.

## Verified dispatcher conversion

`SummonAction_Update` (0x2D8) was enabled after a strict complete-unit check on 2026-09-30. Evidence: `build/manual_late_next/summon_action.accepted55728.log`; the check reports all nine functions and all 0x1034 unit bytes matching. This includes three original assembly fallbacks, so the actual source count is 6/9 C, with `SummonStep_Flip`, `SummonStep_SpecialFromHand`, and `QueueNormalSummon` still assembly.

- **Compiler-only bindings, explicitly marked FAKEMATCH:** `NumberId` initializes its `u32` address in `r0`; the dispatcher initializes a `u16 savedId` in `r2` before the card-number lookup. These are the only two retained bindings. Every empty assembly constraint and all other bindings were removable while keeping the whole unit exact.
- The `u16` copy is lossless: the value is extracted from the zone card word with unsigned shifts to 12 bits before reaching `savedId`. A word-sized copy changes allocation and misses the target; this is a compiler type/allocation hint rather than a new game rule.
- Read the player byte into an ordinary `u32` and extract bit 0 with `(byte << 31) >> 31`. Then read byte +0xE separately into an ordinary `u32` and extract kind bits 2–4 with `(kindFlags << 27) >> 29`. Cast the latter to `int` for the signed comparisons in the ROM switch. These expressions preserve the source bitfield values for every possible input byte and need no barriers.
- Keep the source's link wait, active-bit clear, step-function return normalization, ordered post-action calls, and final active-bit return. The enabled function retains the ROM push/pop sequence including saved `r8`, and no call signatures were changed.
- The original scouting attempt missed 14 bytes. An initialized address binding corrected all three number-to-card table lookups; the lossless narrow card copy corrected the card-number lookup; explicit byte extraction corrected the remaining player/kind scratch choices. Ordinary address types/pointer indexing or a plain signed/narrow card copy still miss 12 or 16 diff lines. These failed alternatives and the minimized exact candidate are saved under `build/manual_late_next/SummonAction_Update.*`; private scripts `build/manual_late_next_dispatch_minimize.py`, `build/manual_late_next_dispatch_minimize2.py`, and `build/manual_late_next_dispatch_plain.py` record the bounded comparisons. Do not infer behavior from a small diff count alone.

- Fresh sibling batch (2026-09-30): `0x080555B0` explicit byte extraction plus initialized message/mask constraints reached 32 private diff lines; expression temporaries changed the branch layout and remaining argument-pack registers. A union byte view was worse and was discarded. `0x08055B28` player/record constraints alone did not fix the complete unit allocation. No candidate was activated; private evidence is in `build/manual_late_next/SummonStep_SpecialFromHand.message.best.c` / `.message.diff` and the `QueueNormalSummon.action` files.

## Zone action and tribute action (2026-10-01)

`SummonStep_Flip` (0x41C) and `SummonStep_SpecialFromHand` (0x178) are enabled; the entire 0x1034-byte unit matches. This supersedes the historical six-of-nine and unattempted-zone-action notes above. Only `QueueNormalSummon` remains ASM. The full ROM check passes (`eds.gba: OK`, `build/lead-pass19/`).

- **Zone action:** stage the normalized player before reading the zone and pass both to a zone-address helper that forms the zone offset first, then adds the player stride and RAM base. For repeated record reads, a separate record helper preserves this order; direct macro arguments read the zone too early. The ordinary four-step switch retains the original effect dispatch, early completion at step 2, and common increment tail. The level helper distinguishes types 21..24 and uses the original packed stats table.
- Two initialized compiler bindings remain, marked FAKEMATCH: the doubled number-to-ID table index in r1, and the loaded player/zone byte in r3 just before `TriggerMysteriousPuppeteer`. One input-only constraint on that byte is necessary. The card-number helper, its experimental r3 table binding and the read/write form of the byte constraint were removed after exact whole-unit rechecks. All values are assigned and consumed before calls that could overwrite them.
- **Tribute action:** compute the shifted upper zone field first, initialize a nibble mask and its packed copy, then apply the same mask to the lower zone field. One initialized r0 binding on the shifted field remains; no empty constraints. This retains the extra mask copy, the record byte in r4 and message in r5. The message bits and all three optional zone-removal calls are unchanged.
- **Return ABI reviewed:** all existing declarations of `CanActivateEffectOfCard` in its other callers use an int result. The ROM callee explicitly zero-extends its result with LSL/LSR 16. Its definition now returns int with an explicit `(u16)` conversion, reproducing the same zero-extended full r0 and all bytes of `effect_activation`; no caller arguments or result bits change. A narrow declaration in this new caller adds a redundant LSL before each zero test, unlike the ROM.

Evidence: `build/bigguns-lead2/tribute_{pack,staging}.py`, `zone_action{,_shapes,_access,_tables,_number,_finish,_minimize}.py`, `SummonStep_Flip/solo-clean/`, `CanActivateEffectOfCard/solo-word-return-clean/`. The sibling hand-choice builder's base-address and initialized lifetime grids did not match; `queue_hand_{base,lifetimes}.py` preserves the failures. Do not repeat those grids unchanged.

## Main-phase interface reconciliation (2026-10-01)

The parked `QueueNormalSummon` draft now accepts word tribute/face arguments and explicitly converts each to u16 on entry, agreeing with the original two shifts and the enabled caller in [[ai-steps-c]]. This routine remains ASM; no coverage is claimed for it. The full ROM passes at `build/lead-pass28/`.

## Hand action builder matched (2026-10-01)

`QueueNormalSummon` is enabled: all 532 bytes match, completing 9/9 C functions and the 0x1034-byte unit. The full ROM passes (`eds.gba: OK`). Earlier ASM/near-match status notes are historical. No callee ABI changes were needed in this pass; the two word arguments retain the explicit u16 decoding described in the previous section.

- Stage the player stride, hand address and raw card shifts, then split the card ID explicitly across byte 3 and halfword 4. Preserve unrelated packed bits and retain the new low bit through the kind/h0C writes. The next predicate lookup reloads the record after the preceding call, including callee mutations.
- The record pointer lives in r9; the card-ID mask lives in r8 only after the original zone value is dead. The table and halfword mask use ordinary allocation. A fixed table binding caused old_agbcc to reuse that register for a live hand base and was rejected; removing the binding recovered the original lifetimes.
- The final byte store needs an ordinary pointer pseudo. A direct r7 binding caused old_agbcc to insert ADD #1 followed by a zero-offset store. One empty read/write low-register constraint, with dead r1–r6 scratch clobbers, selects the original store register without instructions or an uninitialized value.
- Two minimization passes removed 15 bindings and 6 constraints. Five initialized bindings and two empty constraints remain, documented as FAKEMATCH. All caller-saved bindings die before calls; callee-saved registers and SP are restored. Zone address multiplication is explicitly unsigned.

All 8,616 finite differential fixtures pass, with all 821 card IDs under eight profiles, both players plus masked noncanonical player words, hand indices through 79, all low/high tribute-byte values, nonzero upper argument halves, predicate short circuiting, and helper mutations of hand/packed-record values between reads. Complete EWRAM, live IWRAM, ordered calls, saved registers and SP agree; synthetic callees and finite fixtures supplement the exact byte comparison.

Evidence: `build/bigguns-lead2/queue_hand_{resume,staging,exact_order,word,header,record,rebuild,rebuild_refine,safe_lifetimes,final_shape,last_flag,flag_bitfield,store_constraint,store_allocation,minimize,clean,accept}.py`, `verify_queue_hand.py`, `QueueNormalSummon/solo-queue-clean/`, and `build/lead-pass30/`.

Related: [[duel-prompt-handlers-c]], [[summon-builders-c]], [[ai-picks-c]], [[decomp-workflow]].
