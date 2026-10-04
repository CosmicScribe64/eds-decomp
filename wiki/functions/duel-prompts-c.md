---
title: Unit duel_prompts (duel messages / link send, prompts)
type: function
status: draft
confidence: medium
sources: [rom-analysis]
updated: 2026-10-02
---
# Unit duel_prompts

`0x08021CC8`–`0x08022D5B`, Thumb, `old_agbcc -O2`. Source: `src/duel_prompts.c`.
Duel "message" helpers: a small message record inside the duel state (`0x020192E0+0x1B50`) that is forwarded to the link partner (`LinkQueueMessage`, link send), senders for whole card lists (hand/deck/graveyard/fusion/banished) over link, and step-based text prompts. Context: [[program-flow]] (the duel step `0x08021A48`), [[duel-card-lists-c]] (per-player lists), [[duel-cmd-turn-c]].

Unit status: **28/29 functions in C**; see the table (verified with `tools/check.py duel_prompts`, `unit bytes MATCH`).

> [!warning] Contradiction: the unit is now 29/29
> The count above (28/29) predates later matches. `src/duel_prompts.c` has no `INCLUDE_ASM` left (checked 2026-10-02), so all
> 29 functions are in matching C. The decompilation reached 100% at commit `d77fcef` ([[overview]]). Resolved in favour of
> the source. Text below that calls a function nonmatching, parked or `INCLUDE_ASM` is history. Some of the later matches
> are recorded only in git (`git log`) and not yet written up here.

## Functions

| Address | Size | Status | Purpose | Proposed name |
|---|---|---|---|---|
| `0x08021CC8` | 0x4 | matching | `return 0` | |
| `0x08021CCC` | 0x20 | matching | clear `+0x1B43`, `+0x1B44` | |
| `0x08021CEC` | 0xBC | matching | two OBJs at (0x50,0x40) and (0xA0,0x40) (`AddAffineSprite`), tile `GetCardIconObjTile(a) \| 0x1000` (the second one only if `b`, else tile 0x40), pulsing by `gPulseScaleCurve[(frameCounter & 0x1E) >> 1]` on the one selected by `pulse` | |
| `0x08021DA8` | 0x120 | matching | message kind 15 handler: step 0 `ShowActivatedCard(player, CardNumberToId(number))`; step 1 text `0x08081D34` for card number 0x489, `0x08081D70` for 0x5ED/0x5EF, then `TextBoxSetMenu(1,0,0)`; afterwards copy the answer to `result` | |
| `0x08021EC8` | 0x20C | asm (`#if 0` attempt) | kind 17 handler: look for a type-22 card in the hand without bit 18. The CPU (`player != 0`) picks the first type-22 card of player 1's hand (`DiscardHandCard(1, j, 1, 1)`); the human gets text `0x08081DB8`/`0x08081DF0` and a hand cursor (`DuelCursor_PickTarget(1)`, index `0x0201CFB0+0x82C`); B cancels. `result` = 1 when a card was chosen | |
| `0x080220D4` | 0x88 | matching | prompt: step 0 = `sprintf(buf, 0x08081E34, cardName[card])`, text box `TextBoxOpen(0x206, 0x713, 11, buf)`, `TextBoxSetMenu(1,0,0)` (yes/no?); later steps copy the answer `0x0201AE60+0x14` to `result`. `card == 0` returns 1 at once | |
| `0x0802215C` | 0x88 | matching | same with the card `*(u16*)0x086249D4` and format `0x08081E6C`. **Bug in the original:** it formats into `buf` but passes the format string, not `buf`, to the text box | |
| `0x080221E4` | 0x88 | matching | prompt `0x08081EE0`, then `DuelCursor_PickTarget(0xF0)`; if `0x0201CFB0+0x82C != value` stores it in `result`; SE 3 (`PlaySE`) | |
| `0x0802226C` | 0x8C | matching | same with `0x08081F28` and `DuelCursor_PickTarget(0xF00000)` | |
| `0x080222F8` | 0x2E0 | matching | runs the pending message: if forwarded from the partner (`msgSent && msgPlayer`) wait for `0x02017FB0+0x307` bit 7, else switch on `msgKind` 1..20 to its handler (`DuelPrompt_Discard`, `0x0805194C`, `0x08051A9C`, `0x08051BBC`, `0x08051CD8`, `0x08052810`, `0x08051DF4`, `0x0805232C`, `0x08052560`, `0x080525C4`, `0x08052714`, `0x08053F98`, `0x08051ED0`, `0x08052018`, then this unit's prompts `0x08021DA8`, `0x0802226C`, `0x08021EC8`, `0x080220D4`, `0x0802215C`, `0x080221E4`). When done: our own forwarded message sends `result`+14 bytes as link message `0xF0A2`, then clears `msgPending`. Returns 1 while running | `DuelMsg_Run` |
| `0x080225D8` | 0xA0 | matching | post the message: `pending = 1`, clear `step`, bits 0/3; if `msgPlayer` is set, it is a link duel (`0x02015EE8` byte 1 bit 0) and `kind != 3`, send `{kind, arg..}` as link message `0xF0A1` (18 bytes) and set `msgSent` | `DuelMsg_Post` |
| `0x08022678` | 0x54 | matching | `DuelMsg_Set(player, kind, arg, value)` then post | `DuelMsg_Set` |
| `0x080226CC` | 0x60 | matching | like the above, but copies up to 8 halfwords of payload (`MemCopy16`) | `DuelMsg_SetData` |
| `0x0802272C` | 0x2C | matching | kind 1, value = `(x != 0) \| (y ? 2 : 0)` | |
| `0x08022758` | 0x2C | matching | kind 2, same flags | |
| `0x08022784` | 0x48 | matching | kind 3: if a kind-3 message is already pending, `value += n`, else post a new one (accumulates) | |
| `0x080227CC` | 0x48 | matching | kind 4, accumulating like kind 3 | |
| `0x08022814` | 0x10 | matching | kind 5, value 1 | |
| `0x08022824` | 0x10 | matching | kind 7 | |
| `0x08022834` | 0xE0 | matching | if the player's hand has a card for which `CanSummonFromHand(player, id)` holds, not `IsSpecialSummonOnly(id)`, with level ≤ 4, post kind 13 and return 1 | |
| `0x08022914` | 0x48 | matching | if `0x02017FB0+0x306` bit 6 is clear and `+0x1B14` bits 2–8 are 0, set the bit | |
| `0x0802295C` | 0x20 | matching | clear `+0x1B14` bits 2–8, return 0 | |
| `0x0802297C` | 0x40 | matching | link-send an 8-byte message `{a, b, c, d}` (u16 each) | `LinkSend4` |
| `0x080229BC` | 0x30 | matching | link-send `{head, payload[size]}` via a 0x100-byte stack buffer | `LinkSendMsg` |
| `0x080229EC` | 0xB0 | matching | send the hand list: id `0xF021`, `(player<<8) \| count`, the cards, then clear dirty bit 0 of `0x02017FB0+0x305` | `LinkSendHand` |
| `0x08022A9C` | 0xB0 | matching | same for the deck (`0xF022`, bit 1) | `LinkSendDeck` |
| `0x08022B4C` | 0xB0 | matching | graveyard (`0xF023`, bit 2) | `LinkSendGrave` |
| `0x08022BFC` | 0xB0 | matching | fusion deck (`0xF024`, bit 3) | `LinkSendFusion` |
| `0x08022CAC` | 0xB0 | matching | banished (`0xF025`, bit 4) | `LinkSendBanished` |

## Data

- `0x020192E0` duel state: `+0x1B14` bits 2–8 (u16 container); `+0x1B43`, `+0x1B44` bytes; `+0x1B50` message: bit 0 `msgSent` (hypothesis), bit 1 `msgPending`, bit 2 `msgPlayer` (u8 container), bits 4–9 `msgKind` (u16 container); `+0x1B52` `msgArg`, `+0x1B54` `msgValue`, `+0x1B56` 6 more halfwords; `+0x1B62` `step` of the prompt handlers, `+0x1B64` u16 `result`.
- `0x02017FB0` link state: `+0x204` u16 message id (`0xF0xx`), `+0x206` u16 arg, `+0x208` payload (`0x020181B8`); `+0x305` bits 0–4 "list must be resent" flags (hypothesis), `+0x306` bit 6, `+0x307` bit 7.
- `0x020192E4` players (0xD64): counts at `+0x2..+0x6`, lists at `+0x684/+0x7C4/+0x904/+0xA44/+0xB84` (see [[duel-card-lists-c]]).
- Link message ids seen: `0xF021`–`0xF025` (card lists), `0xF0A1` (duel message).

## Matching tricks

- **Bitfields at `+0x304` of `0x02017FB0` must be `u32` containers.** `!f` then compiles to `ldrb; lsl #25; cmp; blt` (sign test after the shift), as in the ROM. A `u8` container gives `and #0x40`.
- **Struct padding:** a sub-struct made of u16 fields gets its size rounded up to 4 by ARM GCC. The message record at `+0x1B50` is 0x12 bytes, so it had to be flattened into the parent struct, or every later field shifts by 2.
- **`u16 flags = x != 0; if (y) flags |= 2;`** (`0x0802272C`): with `int flags` the argument register is saved instead.
- **`0x080225D8`:** load the kind into a `u16` local and `return` if it is 3. Putting `kind != 3` in the `&&` chain tests it with a mask first.
- **`0x080222F8`:** the tail must be `if (done) { ...; return 0; } return 1;` so that the first `return 0` stays the shared exit, as in the ROM.
- **`0x08021CEC`:** `gPulseScaleCurve[(frameCounter & 0x1E) >> 1]` (the ROM's `and #0x1E` then add as a byte offset). `[(x >> 1) & 0xF]` does not fold.
- **`0x08021DA8`:** the number-to-id table must go through a constant address (`(const u16 *)0x08623DF4`), or the literal registers swap.
- **`0x0802215C`:** the early-out form `if (step != 0) { ...; return 1; }` gives the ROM's block order. The if/else form puts the else block first.
- **`0x08022834`:** the hand card must be read as `*(u32 *)((u8 *)gDuelHands + ((p & 1) * 0xD64 + i * 4))`, the card id kept in a `u16` local, and the level computed by a `static inline u32 GetCardLevel(u16 id)` (switch on `int type`). Only this combination makes loop.c hoist the `0x7FF` mask into `r8` and avoids an extra copy of `id`. The card table goes through a constant address (`(const u32 *)0x08621DE0`).
- **Card-list senders (`0x080229EC`–`0x08022CAC`): write the loop as `i = 0; if (i < n) do { ... } while (++i < n);`.** The ROM does not strength-reduce `&buf[i]` (it recomputes `0x020181B8 + i*4` every pass). A `for` loop with `int i` gets reduced (needs `r9`). `u8 i` gets the body right but narrows the increment. A goto loop loses invariant hoisting. The header must be `count | ((u8)player << 8)` (count first).
- **Unsolved `0x08021EC8`:** agbcc hoists the card-table address and the `0x7FF` mask, or strength-reduces the hand pointer. The ROM reloads the table every pass and folds the mask into `lsl #21; lsr #19`. The do/while form did not help here.
