---
title: Cross-reference database (tools/xref.py)
type: tool
status: solid
confidence: high
sources: [rom-analysis]
updated: 2026-10-02
---
# Cross-reference database (`tools/xref.py`)

`tools/xref.py` answers two questions for every function in the ROM, C or assembly: what does this function
touch, and who touches this global? It reads the linked ELF, so the answers describe the shipped machine code
rather than a reading of the C. Use it before naming a function, a global or a struct field. It is the static
half of the readability pass, and the [[emulator]] is the dynamic half. Added in commit `c441483`. The author's
full notes are in `build/xref/README.md`, which exists only locally because `build/` is gitignored.

## Setup
```
tools/dr python3 tools/xref.py build      # build/eds.elf -> build/xref/xref.json, about 10 s
python3 tools/xref.py selftest            # 16 checks against facts documented in the wiki
```
- The build needs capstone and pycparser from the `eds-decomp` image, so run it through `tools/dr`. It reads
  `build/eds.elf` and `build/eds.map`. If they are missing, run `tools/dr make -j8 compare` first.
- It never reads `baserom.gba`, because the ELF already holds every ROM byte.
- `xref.json` contains decoded game text (card names, dialogue). It stays in `build/`, which git ignores:
  never commit it, upload it, or paste it in bulk. The file is written atomically, so a query never reads a
  half-written database.
- Queries use only the Python standard library. They run on the host (`python3 tools/xref.py ...`) or
  through `tools/dr`, in about 0.1 s each.

**When to rebuild.** Rebuild after a code change (a new ELF), after new C struct declarations (if you want
their field names), and after changes to the wiki, `config/names.txt` or `config/assets.tsv`. Renames in
`config/functions.tsv` need no rebuild, because queries read the current names at run time. A query prints a
note on stderr when an input is newer than the database.

## Commands
Every command except `selftest` takes `--json`. Text output truncates long lists; add `--all` to see
everything. A function can be given by its current name, an address inside it, a proposed name from
`config/names.txt`, or a unique substring of either.

| Command | Answers |
|---|---|
| `func <name\|addr>` | A summary card: size, mode, unit, parameters, return use, proposed name, wiki pages, callers, callees with constant arguments, globals, pointer and parameter fields, ROM data and strings. |
| `global <name\|addr> [--offset 0x40C] [--exact]` | Every function that reads, writes, indexes, dereferences or takes the address of a global, grouped by address with field names. `--offset` narrows the list to one field. |
| `strings <regex>` | Functions that use matching text: C strings, string tables, inline text records, card names, and dialogue lines started with a constant event ID. |
| `graph <name> [--depth N] [--up] [--refs] [--hide-hubs N]` | A call tree of callees, or of callers with `--up`. Indirect edges are tagged `[table]`, `[slot]` or `[ptr]`. `--refs` also follows functions whose address is only taken. |
| `unit <unit>` | One screen per unit: each function's size, caller and callee counts, proposed name, wiki page and first string, plus external edges and the globals and assets the unit uses. |
| `subsystems [--resolution R] [--min N] [-v]` | Louvain clusters over calls, shared globals and address adjacency, two levels deep, each with a suggested label and its evidence. |
| `what <addr>` | What lives at an address: a function and offset, a global field, an IO register, an asset record, a card or effect record, the pointer stored there, and the functions that use it. |
| `selftest` | Checks the database against 16 facts documented in the wiki. Run it after every rebuild. |

One example of each:
```
python3 tools/xref.py func MainLoop                     # AgbMain's main loop (proposed name)
python3 tools/xref.py global gMain --offset 0x4859      # who uses the sequencer byte seqIndex1
python3 tools/xref.py strings 'rare card'               # who starts this dialogue line or prints this string
python3 tools/xref.py graph LZSSDecompress --up --depth 3 # how the LZSS decoder is reached
python3 tools/xref.py unit card_detail
python3 tools/xref.py subsystems -v | less
python3 tools/xref.py what 0x020192E4                   # players[0].lifePoints and its 226 users
python3 tools/xref.py selftest
```

## Reading a `func` card
- **`sub_X (Name?)`**: a proposed name from `config/names.txt`, which is not applied yet.
- **parameters**: the argument registers the function reads before writing them, plus stack arguments. An
  argument passed unchanged to a callee that reads it counts as used.
- **returns**: "a value in r0 that callers use" means at least one caller reads r0 after the call.
- **Callee tags.** `[ptr]` is a constant call target. `[slot]` is a call through a RAM function pointer,
  and its candidates are every function ever stored there, including through a setter such as
  `SetMainCallback` ([[set-main-callback]]). `[table]` is a call through a ROM table, and its candidates are the
  table's entries at that element offset ([[function-pointer-tables]]). `address taken by` lists functions
  that load this function's address without calling it.
- **Call arguments** (`r0=...`) are constant register values at the call site, cut to the callee's parameter
  count. They are decoded where possible: `&gMain+0x...` is an address, `arg2` is the caller's own third
  argument, and a quoted string is a dialogue event ID.
- **globals**: one row per address, with the access (`r`, `w`, `rw`), the size, and the field name from the
  merged C declarations.
  - `(= gUnk_X)` is another symbol at the same address.
  - `[i]` means the address uses an index computed at run time, and `(offset varies)` means the offset could
    not be pinned down.
- **through pointers** shows `(*gX)->+0x10` or `table[i]->+4`. **parameter fields** shows `arg0+0x14`. The
  parameter fields are the best clue to a parameter's struct type.
- **takes the address of** lists data passed to calls or stored, such as DMA sources. **also loads these
  addresses** lists literals whose use the analysis lost, so nothing is silently dropped.
- **ROM data / strings / stored in ROM data tables** lists referenced data with its `config/assets.tsv` path,
  decoded where the layout is known: card records, dialogue, and card-effect records ([[cards]]). The tables
  that hold a pointer to the function show how card-effect handlers and step functions reach their
  dispatchers.

## How it works
1. **Symbols.** Function ranges come from ELF FUNC symbols, with names and units from `config/functions.tsv`.
   Data symbols are ranked: hand-made names first, then `gUnk_X`, then aliases. Library units come from
   `build/eds.map`, and `INCLUDE_ASM` functions are flagged `[asm]`.
2. **Disassembly** uses capstone, in Thumb or ARM. The ELF's `$t`, `$a` and `$d` mapping symbols separate
   code from literal pools. agbcc's intra-function "far jump" `bl` is treated as a branch.
3. **Value propagation.** A forward analysis over each control-flow graph tracks register and stack values to
   a fixpoint. The values are constants, a pointer plus an unknown index, arguments, pointers loaded from
   globals, table words and call results. Jump tables are resolved. Values that disagree at a merge are
   widened.
4. **Recording.** The tool records every load and store whose address is known, every call with its
   arguments, function-pointer stores, arguments stored into globals, and parameter use.
5. **Cross-function resolution.** This covers slot and table candidates, and function pointers found in the
   `rodata/`, `tables/`, `text/`, `gfx/scene_sets` and sound-table assets ([[assets]]). Parameter counts and
   return use are resolved across call sites.
6. **Names and text.** Struct fields are merged from every C unit, with the layout rules of
   `tools/structmap.py`. IO register names come from `include/gba.h`. Text is decoded with the
   `tools/assets.py` helpers ([[text-system]]).

## Verification (2026-10-02)
- **`selftest`: 16 of 16 pass.** The checks cover:
  - the 3 callers and 10 call sites of the LZSS decoder ([[lzss-decompress]]);
  - MainLoop and GameInit ([[program-flow]], [[agb-main]]);
  - SetMainCallback's argument-to-callback store;
  - DuelMainStep's phase-table dispatch;
  - IntrMain's IntrTable dispatch ([[interrupt-handlers]]);
  - the password and card-name lookups ([[card-data-functions]]);
  - an effect-table record;
  - StartDialogue's event-ID argument;
  - the booster pack names.

  Re-run by the scribe on 2026-10-02: 16 PASS.
- **Globals against the C sources:** xref finds every `gUnk_` or address-suffixed global named in a matched C
  function body (2940 of 2940 references).
- **Parameter counts against C signatures:** 1936 of 1960 are equal. The 23 functions with fewer parameters
  are handlers that ignore an argument of a shared signature. The one with more is `TextDrawNumber`, which
  forwards r3 to callees that read it.
- **Reachability:** 137 code functions have no caller, reference or data reference, and their addresses
  appear nowhere in the image. They are dead code (the [[debug-menu]], `SaveAndResetSceneState`) or are
  reached through computed addresses.

## Limitations
- The analysis is static and path-insensitive. Values that differ between paths are widened, so some rows show
  `[i] (offset varies)` or appear under "also loads".
- Slot candidates include every function ever stored in the slot, even ones that never reach a given call
  site. Table candidates without a C element size include every function pointer in the label.
- Field names are what the C units call those bytes today. Where units disagree, the JSON keeps the
  alternates (`decls.<addr>.leaves[*][8]`).
- Text reached through scene scripts is not attributed to a function.
- `subsystems` is a heuristic. Use it to find candidates, then confirm with `func`, `graph` or `global`
  before naming anything.

Related: [[emulator]], [[agent-tooling]], [[decomp-workflow]], [[ram-map]].
