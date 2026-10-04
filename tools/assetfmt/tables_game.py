"""Game tables: opponent deck lists, card passwords, the sound sine table, the dialogue terminator record, and
the two .rodata table regions (0x0819D1C4 "pointer tables" and 0x0819DD64 "rodata2") split into named sub-tables.

Types (manifest column 3):
  tables_game_dialogue_end   the 0x304-byte terminator record after the dialogue table     <path> (JSON)
  tables_game_decks          card-number lists + the two {cards, count} header tables      <path> (JSON)
  tables_game_passwords      821 x 4-byte packed-BCD passwords, indexed by card ID          <path> (CSV)
  tables_game_sine           256 x s16 sine table (4.12 fixed point)                        <path> (JSON)
  tables_game_layout         a region split into the sub-tables of LAYOUTS[layout=NAME]    <path>/<name>.json

Conventions for every JSON file written here:
  - Keys starting with "_" and the header keys doc, address, size, labels, users and format are for reading
    only; the build ignores them.
  - Card numbers are authoritative; card names next to them ([number, "name"] pairs, "_name" keys) are read
    from the ROM's name table when extracting and are ignored when building.
  - Pointers are "0x08XXXXXX" strings (null = NULL), or ["0x08XXXXXX", "what it points to"] pairs.
  - Numbers may be written in decimal or as "0x.." strings.
"""
import csv
import io
import json
import os
import re
import struct

A = None  # the tools/assets.py module, set by register()
BASE = 0x08000000

# --------------------------------------------------------------------------------------------- layouts
# (address, name, kind, options, users, doc). Each sub-table ends where the next one starts; the last one ends
# at the region end. Kinds: u8/s8/u16/s16/u32/s32 (opts: shape, hex), cards, ptrs (opts: str), struct
# (opts: fields, shape), oam, packs (opts: table), colors, sjis, text, pad.
SPRITE_DESC = [('oam', 'ptr'), ('count', 'u8'), ('pad', 'pad:3')]
SCROLL_REG = [('reg', 'ptr'), ('mask', 'hex16'), ('pad', 'pad:2')]

LAYOUTS = {
    'pointer_tables': (0x0819D1C4, 0x0819D34C, [
        (0x0819D1C4, 'tribute_prompts', 'ptrs', {'str': True}, ['EffectUltimateOfferingResolve', 'CardMenu_SummonMonster'],
         'Duel prompt strings for tribute summoning (char *[5]).'),
        (0x0819D1D8, 'duel_step_handlers', 'ptrs', {}, ['BattlePhase_Run'],
         'Step handlers called by BattlePhase_Run, indexed by bits 9-16 of the step word at 0x020192E0+0x1B14; '
         'NULL-terminated.'),
        (0x0819D214, 'monster_type_names', 'ptrs', {'str': True}, ['TypeMenu_Draw'],
         'Monster type names (char *[20]) in card type order: entry i is type i + 1 (Dragon = 1, see wiki '
         'card-table).'),
        (0x0819D264, 'attribute_names', 'ptrs', {'str': True}, ['AttributeMenu_Draw', 'DuelPrompt_PickOneOfTwoAttributes'],
         'Attribute names (char *[6]): entry i is attribute i + 1 (LIGHT, DARK, WATER, FIRE, EARTH, WIND).'),
        (0x0819D27C, 'card_jump_arc', 'struct', {'fields': [('dx', 's32'), ('dy', 's32')]}, ['DeckReorder_DrawSwap'],
         '{s32 dx; s32 dy}[16] pixel offsets along an arc, indexed by an animation phase. The code addresses '
         'the dx column as gCardJumpArc and the dy column as gCardJumpArcDy.'),
        (0x0819D2FC, 'ai_power_cards', 'cards', {}, ['AiPickHandCard (AiPickCardListEntry)'],
         'AI "power card" list (13 card numbers); see wiki special-card-lists.'),
        (0x0819D316, 'ai_priority_cards', 'cards', {}, ['AiPickOpponentHandCard'],
         'AI hand-card priority list (26 card numbers); see wiki special-card-lists.'),
        (0x0819D34A, 'pad_0819D34A', 'pad', {}, [], 'Alignment padding.'),
    ]),
    'rodata2': (0x0819DD64, 0x081A7A0C, [
        (0x0819DD64, 'ai_scan_cards', 'cards', {}, ['AiActivateMonsterEffects'],
         'Four card numbers the AI looks for in its spell/trap zones (number 0x1FF needs AiTryPlaySpellTrap).'),
        (0x0819DD6C, 'ai_turn_steps', 'ptrs', {}, ['AiRunStep'],
         'CPU-turn step handlers, indexed by AiState.step (gAiState+1).'),
        (0x0819DD94, 'warp_bg2x', 's32', {'shape': [16, 160]}, ['BattleScene_HBlank'],
         'HBlank warp effect: BG2X/BG3X reference point (20.8 fixed point) for each of 160 scanlines, 16 '
         'frames (index gBattle+0x15D).'),
        (0x081A0594, 'warp_bg2y', 's32', {'shape': [16, 160]}, ['BattleScene_HBlank'],
         'HBlank warp effect: BG2Y/BG3Y per scanline, 16 frames.'),
        (0x081A2D94, 'warp_bg2pa', 's16', {'shape': [16, 160]}, ['BattleScene_HBlank'],
         'HBlank warp effect: BG2PA/BG3PA (8.8 horizontal scale) per scanline, 16 frames.'),
        (0x081A4194, 'shake_offset_regs', 'ptrs', {}, ['BattleScene_Update'],
         'BG2X, BG2Y, BG3X, BG3Y register addresses, written with random shake offsets.'),
        (0x081A41A4, 'attribute_icons', 'ptrs', {}, ['DrawCardDetail (battle_scene)'],
         'Attribute icon graphics (0xC8 bytes each in gfx/bank_a), indexed by attribute 1-20.'),
        (0x081A41F8, 'kind_icons', 'ptrs', {}, ['DrawCardDetail (battle_scene)'],
         'Card kind icon graphics, indexed by kind 1-6.'),
        (0x081A4214, 'duel_text_colors', 'u32', {}, ['TextBoxDrawText'],
         'Text colour (palette index) per colour code; [9] is the shadow colour, [0] the fill.'),
        (0x081A423C, 'duel_icon_anim_frames', 'u16', {}, ['TextBoxDrawSprites'],
         'Animation frame per 4-tick step (counter >> 2 & 31); tile = frame * 4 + 0x42E4.'),
        (0x081A427C, 'zone_cursor_anim_tiles', 'u32', {'hex': True}, ['DrawZonePairCursor (duel_field_screen)'],
         'Zone cursor tile offsets cycled by frameCounter >> 3.'),
        (0x081A429C, 'cursor_lerp_weights', 'u16', {}, ['DuelScreen_Update (duel_field_view)'],
         'Cursor/scroll interpolation weights (8.8), indexed by the steps left.'),
        (0x081A42A4, 'duel_zone_positions', 'struct', {'fields': [('x', 'u32'), ('y', 'u32')], 'shape': [2, 16]},
         ['duel_field_view', 'duel_field_screen', 'card_canvas'],
         'Pixel position of every zone, [player][zone].'),
        (0x081A43A4, 'duel_zone_scroll_targets', 'u16', {'shape': [2, 16]}, ['DuelScreen_ScrollToZone'],
         'Screen scroll target per zone, [player][zone].'),
        (0x081A43E4, 'bounce_scale_curve', 'u16', {}, ['duel_stat_queries', 'duel_cmd_moves', 'duel_card_anim'],
         'Sprite scale curve (0x100 = 1.0): shrinks to 0x80 and bounces back.'),
        (0x081A4424, 'pulse_scale_curve', 'u16', {}, ['duel_cmd_moves', 'duel_cmd_presentation', 'duel_cmd_screen', 'campaign',
                                                   'duel_prompts', 'battle_phase3', 'duel_cursor', 'summon_checks',
                                                   'duel_info_bar'],
         'Pulse curve (0x100 -> 0xC0 -> 0x100) for the selected item.'),
        (0x081A4444, 'shrink_scale_steps', 'u16', {}, ['duel_cmd_screen', 'campaign'],
         'Scale steps 0xF0 .. 0x80.'),
        (0x081A4454, 'card_move_lerp_weights', 'u16', {}, ['duel_card_anim'],
         'Ease-out weights (/256) for a card moving between zones over 16 steps.'),
        (0x081A4474, 'card_move_anim_frames', 'u16', {'hex': True, 'shape': [2, 24]}, ['CardMoveAnimTick'],
         'Card flip/move frames, [set][frame]; bit 12 adds the card graphic.'),
        (0x081A44D4, 'card_move_anim_scale', 'u16', {'shape': [2, 10]}, ['CardMoveAnimTick'],
         'Colour/scale attribute per frame, [flag][frame].'),
        (0x081A44FC, 'banner_slide_offsets', 'u16', {}, ['duel_cmd_moves', 'duel_cmd_screen'],
         'Slide-in offsets for the turn banner (16 steps).'),
        (0x081A451C, 'zone_border_cycle_colors', 'colors', {}, ['CycleBorderColor (GetPack_HBlank)'],
         'Colours cycled through BG palette 0 entry 15 by scanline (8 BGR555 colours).'),
        (0x081A452C, 'booster_packs', 'packs', {'table': 0x081A562C}, ['booster_pack'],
         'Booster pack contents: per pack 8 rarity slots of card numbers (slot 0 = rarest ... 7 = commons). '
         'In ROM: each pack\'s slot lists, then its PackSlots {cards, count}[8], then the 28-entry table '
         '{PackSlots *p; u16 id; u16 pad} at 0x081A562C. See wiki booster-packs.'),
        (0x081A570C, 'pack_rarity_thresholds', 's32', {}, ['RollPackRarity'],
         'Cumulative thresholds for the rarity roll (r = rand() % 180 or % 270).'),
        (0x081A572C, 'pack_opening_steps', 'ptrs', {}, ['CB_GetAPack (CB_GetPack)'],
         'Steps of the pack-opening screen (debug "Get a pack"); NULL-terminated.'),
        (0x081A5758, 'pack_unlock_ids', 'u16', {}, ['PackList_AddUnlockedPacks'],
         'The 27 pack IDs whose unlock condition is checked.'),
        (0x081A578E, 'pad_081A578E', 'pad', {}, [], 'Alignment padding.'),
        (0x081A5790, 'sprite_oam_a', 'oam', {}, ['anim scripts in rodata 1 (0x08086C54..)', 'filter_menu_sprites_*'],
         'OAM entry lists (sprite frames) used by animation scripts and sprite descriptors.'),
        (0x081A6118, 'card_list_menu_anims', 'ptrs', {}, ['ListFilter_Init'],
         'Animation scripts (in rodata 1) started together by AnimBlockInit; NULL-terminated.'),
        (0x081A6154, 'sprite_oam_b', 'oam', {}, ['anim scripts in rodata 1 (0x08086F74..)', 'deck edit screens'],
         'OAM entry lists (sprite frames) used by animation scripts, sprite descriptors and code.'),
        (0x081A70FC, 'deck_edit_anims', 'ptrs', {}, ['deck_edit_filter_steps', 'deck_edit_view', 'deck_edit',
                                                      'deck_edit_prohibit'],
         'Animation scripts (in rodata 1) started together by AnimBlockInit; NULL-terminated.'),
        (0x081A7144, 'slot_sprite_frames', 'ptrs', {}, ['DrawSlotSprites (deck_edit_widgets)'],
         'One-OAM sprite frames for the 6 slots.'),
        (0x081A715C, 'filter_menu_sprites_a', 'struct', {'fields': SPRITE_DESC}, ['ListFilter_DrawCursor'],
         'Sprite descriptors {OAM list; count} for the filter/sort menu.'),
        (0x081A71CC, 'filter_menu_sprites_b', 'struct', {'fields': SPRITE_DESC}, ['ListFilter_DrawCursorFlash'],
         'Sprite descriptors {OAM list; count} for the filter/sort menu.'),
        (0x081A723C, 'transfer_steps', 'ptrs', {}, ['DeckEdit_RunListFilter', 'ProhibitCardSelect_RunListFilter'], 'Scene steps; NULL-terminated.'),
        (0x081A724C, 'statistics_steps', 'ptrs', {}, ['DeckEdit_RunStatistics'], 'Scene steps; NULL-terminated.'),
        (0x081A725C, 'deck_edit_steps', 'ptrs', {}, ['CB_DeckEdit (Deck Edit callback)'],
         'Deck Edit scene steps; NULL entries end a group.'),
        (0x081A72A0, 'deck_edit_select_steps', 'ptrs', {}, ['SideDeckSwap_Run'], 'Deck Edit selection steps.'),
        (0x081A72E4, 'deck_edit_sub_steps', 'ptrs', {}, ['TradeCardSelect_Run'], 'Deck Edit sub-steps.'),
        (0x081A7330, 'deck_edit_popup_steps', 'ptrs', {}, ['ProhibitCardSelect_Run'], 'Deck Edit popup steps.'),
        (0x081A7374, 'link_ack_packet', 'u8', {'hex': True}, ['LinkRxPop', 'LinkTxQueue', 'LinkRecvHook'],
         'Default 12-byte link packet (type 0xF0, ack).'),
        (0x081A7380, 'pad_081A7380', 'pad', {}, [], 'Alignment padding.'),
        (0x081A7382, 'link_dup_ack_packet', 'u8', {'hex': True}, ['LinkRecvHook'],
         '12-byte link packet sent for a duplicate sequence number (type 0xD0).'),
        (0x081A738E, 'pad_081A738E', 'pad', {}, [], 'Alignment padding.'),
        (0x081A7390, 'link_nak_packet', 'u8', {'hex': True}, ['LinkRecvHook'],
         '12-byte link packet sent for an unknown packet type (type 0xE0).'),
        (0x081A739C, 'pad_081A739C', 'pad', {}, [], 'Alignment padding.'),
        (0x081A73A0, 'debug_menu_items', 'struct', {'fields': [('name', 'text:64'), ('callback', 'ptr')]},
         ['text_canvas'], 'Debug menu {char name[0x40]; callback} entries, ended by an empty entry.'),
        (0x081A768C, 'debug_menu_steps', 'ptrs', {}, ['CB_DebugMenu (CB_DebugMenu)'], 'Debug menu steps.'),
        (0x081A76A0, 'ascii_to_sjis', 'sjis', {}, ['AsciiToFullwidthSjis'],
         'Full-width Shift-JIS code for ASCII 0x20..0x7F (entry 0x7F unused).'),
        (0x081A7760, 'map_fill_tile', 'u16', {'hex': True}, ['FillMapRect (FillMapRect)'],
         'Tile used by FillMapRect ([0]); [1] is unused.'),
        (0x081A7764, 'bg_hofs_regs', 'struct', {'fields': SCROLL_REG}, ['FrameSyncUpdate'],
         'BG0-3 HOFS register address and dirty-mask bit.'),
        (0x081A7784, 'bg_vofs_regs', 'struct', {'fields': SCROLL_REG}, ['FrameSyncUpdate'],
         'BG0-3 VOFS register address and dirty-mask bit.'),
        (0x081A77A4, 'pad_081A77A4', 'pad', {}, [], 'Padding.'),
        (0x081A77A8, 'sine_table_128', 's16', {}, ['SetOamAffineRotScale', 'SetOamMatrixPacked'],
         'Sine, 128 steps per turn, 0x100 = 1.0 (cosine = index + 0x20); the values are int(256*sin(2*pi*i/128)), '
         'truncated toward zero.'),
        (0x081A78A8, 'save_signature', 'text', {}, ['IsSaveSignatureValid', 'WriteSaveSignature'],
         'Save file signature (8 characters, stored in a 12-byte field).'),
        (0x081A78B4, 'card_copy_limits', 'struct', {'fields': [('card', 'card'), ('limit', 'u16')]},
         ['GetCardCopyLimit (GetCardCopyLimit)'],
         'Forbidden/limited list: {card number; max copies per deck}; cards not listed allow 3.'),
        (0x081A7970, 'password_steps', 'ptrs', {}, ['CB_Password'], 'Password screen steps.'),
        (0x081A79A4, 'card_trading_steps', 'ptrs', {}, ['CB_CardTrading'], 'Card Trading screen steps.'),
        (0x081A79E8, 'sound_channel_map', 'u8', {}, ['SoundSeTrackTick'],
         'Channel remap used when a track has flag 1.'),
        (0x081A79F4, 'se_variant_channel_order', 'u8', {'shape': [4, 6]}, ['SoundStartPendingSE'],
         'Channel order for each SE variant 0-3; the code reads it through gSeVariantTrackMap = &row[variant][5].'),
    ]),
}
NUM = {'u8': '<B', 's8': '<b', 'u16': '<H', 's16': '<h', 'u32': '<I', 's32': '<i'}
IO_REGS = {0x04000010: 'BG0HOFS', 0x04000012: 'BG0VOFS', 0x04000014: 'BG1HOFS', 0x04000016: 'BG1VOFS',
           0x04000018: 'BG2HOFS', 0x0400001A: 'BG2VOFS', 0x0400001C: 'BG3HOFS', 0x0400001E: 'BG3VOFS',
           0x04000020: 'BG2PA', 0x04000028: 'BG2X', 0x0400002C: 'BG2Y', 0x04000030: 'BG3PA',
           0x04000038: 'BG3X', 0x0400003C: 'BG3Y'}
OAM_SIZES = {(0, 0): '8x8', (0, 1): '16x16', (0, 2): '32x32', (0, 3): '64x64',
             (1, 0): '16x8', (1, 1): '32x8', (1, 2): '32x16', (1, 3): '64x32',
             (2, 0): '8x16', (2, 1): '8x32', (2, 2): '16x32', (2, 3): '32x64'}
OAM_SIZE_BITS = {v: k for k, v in OAM_SIZES.items()}

# ROM tables used for annotations only (names next to card numbers etc.); none of them is needed to build.
NAMES_AT, NUMBER_TO_ID_AT, ID_TO_NUMBER_AT = 0x0822C720, 0x08623DF4, 0x08622AB4
DUELISTS_AT, DUELISTS_END = 0x08139F64, 0x0813ADD4
PACK_INFO_AT, PACK_INFO_COUNT = 0x080865DC, 23
CODE_END, RODATA1 = 0x08080A20, (0x08080A20, 0x08087FB4)


def register(a):
    global A
    A = a
    return {
        'tables_game_dialogue_end': (x_dialogue_end, _checked(b_dialogue_end)),
        'tables_game_decks': (x_decks, _checked(b_decks)),
        'tables_game_passwords': (x_passwords, _checked(b_passwords)),
        'tables_game_sine': (x_sine, _checked(b_sine)),
        'tables_game_layout': (x_layout, _checked(b_layout)),
    }


def _checked(build):
    """Report a malformed file as ValueError (tools/assets.py prints those as build errors)."""
    def b(read, p):
        try:
            return build(read, p)
        except (TypeError, IndexError, AttributeError, struct.error, json.JSONDecodeError) as e:
            raise ValueError(f"{p['path']}: malformed file ({e!r})")
    return b


# ------------------------------------------------------------------------------------------- helpers
def _dumps(o, ind=0, width=112):
    """JSON with short containers on one line and long scalar lists wrapped, so tables stay readable."""
    flat = json.dumps(o, ensure_ascii=False, separators=(', ', ': '))
    if not isinstance(o, (list, dict)) or len(flat) + ind <= width and _depth(o) <= 2:
        return flat
    pad = ' ' * (ind + 1)
    if isinstance(o, dict):
        body = [f'{pad}{json.dumps(k)}: {_dumps(v, ind + 1, width)}' for k, v in o.items()]
        return '{\n' + ',\n'.join(body) + '\n' + ' ' * ind + '}'
    if all(not isinstance(x, (list, dict)) for x in o):
        lines, cur = [], []
        for x in o:
            s = json.dumps(x, ensure_ascii=False)
            if cur and len(pad) + sum(len(c) + 2 for c in cur) + len(s) > width:
                lines.append(', '.join(cur))
                cur = []
            cur.append(s)
        lines.append(', '.join(cur))
        return '[\n' + ',\n'.join(pad + ln for ln in lines) + '\n' + ' ' * ind + ']'
    return '[\n' + ',\n'.join(pad + _dumps(x, ind + 1, width) for x in o) + '\n' + ' ' * ind + ']'


def _depth(o):
    if isinstance(o, dict):
        return 1 + max((_depth(v) for v in o.values()), default=0)
    if isinstance(o, list):
        return 1 + max((_depth(v) for v in o), default=0)
    return 0


def _json(o):
    return (_dumps(o) + '\n').encode()


def _int(v):
    """A number from JSON: int, or a "0x.."/decimal string, or the value of a [value, note] pair."""
    if isinstance(v, list):
        v = v[0]
    if v is None:
        return 0
    if isinstance(v, str):
        return int(v.split()[0], 0)
    if isinstance(v, bool) or not isinstance(v, int):
        raise ValueError(f'expected a number, got {v!r}')
    return v


def _pack(fmt, v, what):
    try:
        return struct.pack(fmt, v)
    except struct.error:
        raise ValueError(f'{what}: {v} does not fit ({fmt[1:]})')


def _hex(v, digits):
    return f'0x{v:0{digits}X}'


def _optional_json(read, rel):
    try:
        return json.loads(read(rel))
    except (KeyError, FileNotFoundError):
        return None


class Ctx:
    """Everything the annotations need, read from the ROM (and the repo's data labels and manifest)."""

    def __init__(self, rom):
        self.rom = rom
        self.n2id = struct.unpack_from('<2048H', rom, NUMBER_TO_ID_AT - BASE)
        self.id2n = struct.unpack_from('<821H', rom, ID_TO_NUMBER_AT - BASE)
        self.duelists = {}
        for a in range(DUELISTS_AT, DUELISTS_END, 0x84):
            i = struct.unpack_from('<I', rom, a - BASE)[0]
            self.duelists.setdefault(i, self.cstr(a + 4, 0x40))
        self.packs = {}
        for i in range(PACK_INFO_COUNT):
            a = PACK_INFO_AT + i * 0x48
            self.packs[struct.unpack_from('<I', rom, a - BASE)[0]] = self.cstr(a + 8, 0x40)
        self.labels = {}
        for f in ('data/rodata_08080A20.s', 'data/rodata_08087FD0.s'):
            if os.path.exists(f):
                for name, addr in A.labels_of(f):
                    self.labels.setdefault(addr, name)
        self.assets = []
        if os.path.exists(A.MANIFEST):
            for line in open(A.MANIFEST):
                f = line.split('#')[0].split()
                try:
                    self.assets.append((int(f[0], 16), int(f[1], 16), f[3]))
                except (IndexError, ValueError):
                    pass
        self.tables = []   # (start, end, name, elem size) of the sub-tables of LAYOUTS, for pointer notes
        self.refs = {}     # target address -> [source addresses], see refs_in()
        for lname, (_, _, subs) in LAYOUTS.items():
            for (at, name, kind, opts, _, _), end in zip(subs, [s[0] for s in subs[1:]] + [LAYOUTS[lname][1]]):
                self.tables.append((at, end, name, _elem_size(kind, opts)))

    def cstr(self, a, n):
        return A.dec(self.rom[a - BASE:a - BASE + n].split(b'\0')[0])

    def card_name(self, no):
        if no == 0xFFFF:
            return None
        cid = self.n2id[no & 0x7FF] if no <= 1999 else (self.n2id[(no - 2000) & 0x7FF] or -1) + 1
        if cid <= 0 or cid > 820:
            return None
        return self.cstr(NAMES_AT + cid * 0x40, 0x40)

    def card_pair(self, no):
        name = self.card_name(no)
        return [no, name] if name is not None else no

    def asset_of(self, v):
        for s, e, path in self.assets:
            if s <= v < e:
                return f'{path}+0x{v - s:X}'
        return None

    def note(self, v, want_str=False):
        """What the pointer v points to, for reading."""
        if v == 0:
            return None
        if v in IO_REGS:
            return f'REG_{IO_REGS[v]}'
        if BASE <= v < CODE_END:
            return f'sub_{v & ~1:08X}' + ('' if v & 1 else ' (ARM)')
        for s, e, name, size in self.tables:
            if s <= v < e:
                off = v - s
                return f'{name}[{off // size}]' if size and off % size == 0 else f'{name}+0x{off:X}'
        parts = []
        if v in self.labels:
            parts.append(self.labels[v])
        if want_str:
            parts.append(json.dumps(self.cstr(v, 0x200), ensure_ascii=False))
        else:
            a = self.asset_of(v)
            if a:
                parts.append(a)
        return ' '.join(parts) or None

    def ptr(self, v, want_str=False):
        if v == 0:
            return None
        n = self.note(v, want_str)
        return [_hex(v, 8), n] if n else _hex(v, 8)

    def refs_in(self, lo, hi):
        """Aligned words in code, rodata 1 and the pointer-holding sub-tables of LAYOUTS that point into
        [lo, hi): {target: [sources]}."""
        if not self.refs:
            srcs = [(BASE, RODATA1[1])]
            for lname, (_, _, subs) in LAYOUTS.items():
                ends = [s[0] for s in subs[1:]] + [LAYOUTS[lname][1]]
                srcs += [(at, end) for (at, _, kind, _, _, _), end in zip(subs, ends) if kind in ('ptrs', 'struct')]
            lo_all, hi_all = 0x0819D1C4, 0x081A7A0C
            for s, e in srcs:
                for a in range((s + 3) & ~3, e - 3, 4):
                    v = struct.unpack_from('<I', self.rom, a - BASE)[0]
                    if lo_all <= v < hi_all:
                        self.refs.setdefault(v, []).append(a)
        return {t: s for t, s in self.refs.items() if lo <= t < hi}


_CTX = {}


def ctx_for(rom):
    if rom is None:
        return None
    if id(rom) not in _CTX:
        _CTX.clear()
        _CTX[id(rom)] = Ctx(rom)
    return _CTX[id(rom)]


def _cards_out(ctx, nums):
    return [ctx.card_pair(n) if ctx else n for n in nums]


def _cards_in(items, what):
    out = bytearray()
    for v in items:
        out += _pack('<H', _int(v), what)
    return bytes(out)


# ------------------------------------------------------------------------------------- dialogue end
def x_dialogue_end(data, p, rom):
    if len(data) != 0x304:
        raise ValueError('the terminator record is 0x304 bytes')
    ev, sp = struct.unpack_from('<HH', data)
    return {p['path']: _json({
        'doc': 'Terminator record after the dialogue table (text/dialogue.json): the same layout as a dialogue '
               'entry {u16 event; u16 speaker; char text[0x300]}, with event = speaker = 0xFFFF.',
        'event': ev, 'speaker': sp, 'text': A.text_field(data[4:])})}


def b_dialogue_end(read, p):
    d = json.loads(read(p['path']))
    return _pack('<H', _int(d['event']), 'event') + _pack('<H', _int(d['speaker']), 'speaker') + \
        A.field_bytes(d['text'], 0x300)


# --------------------------------------------------------------------------------------------- decks
DECK_TABLES = (('opponent_decks', 0x0819DC6C, 25), ('alternate_decks', 0x0819DD34, 6))
DECK_DOC = ('Opponent decks. "opponent_decks"[i] is the deck of duelist ID i (gOpponentDecks, 25 entries) and '
            '"alternate_decks"[i] the second deck of duelists 1-5 (gOpponentAltDecks, 6 entries); null = {NULL, 0}. '
            'Cards are card numbers (not IDs); the names next to them are for reading only. In ROM each table '
            'entry is {const u16 *cards; u16 count; u16 pad} and the lists are packed back to back from '
            '0x0819D34C in the order of "rom_order" (decks missing from it go last); unused space up to '
            '0x0819DC6C is zero-filled. Decks may change size as long as all lists fit in 0x920 bytes.')


def x_decks(data, p, rom):
    ctx = ctx_for(rom)
    start, end = p['start'], p['end']
    lists_end = DECK_TABLES[0][1]
    out = {'doc': DECK_DOC}
    placed = []
    for key, at, n in DECK_TABLES:
        decks = []
        for i in range(n):
            ptr, cnt, pad = struct.unpack_from('<IHH', data, at - start + i * 8)
            if ptr == 0 and cnt == 0 and pad == 0:
                decks.append(None)
                continue
            if not start <= ptr < lists_end or ptr + 2 * cnt > lists_end:
                raise ValueError(f'{key}[{i}] points outside the card lists')
            nums = struct.unpack_from(f'<{cnt}H', data, ptr - start)
            d = {}
            if ctx and i in ctx.duelists:
                d['_duelist'] = ctx.duelists[i]
            d['cards'] = _cards_out(ctx, nums)
            if pad:
                d['pad'] = pad
            decks.append(d)
            placed.append((ptr, f'{key}/{i}'))
        out[key] = decks
    out['rom_order'] = [k for _, k in sorted(placed)]
    return {p['path']: _json(out)}


def b_decks(read, p):
    d = json.loads(read(p['path']))
    start, end = p['start'], p['end']
    lists_end = DECK_TABLES[0][1]
    order = list(d.get('rom_order', []))
    for key, _, n in DECK_TABLES:
        decks = d[key]
        if len(decks) != n:
            raise ValueError(f'{key} must have {n} entries (index = duelist ID), not {len(decks)}')
        order += [f'{key}/{i}' for i, x in enumerate(decks) if x is not None and f'{key}/{i}' not in order]
    lists, ptrs, pos = bytearray(), {}, start
    for ref in order:
        key, i = ref.split('/')
        deck = d[key][int(i)]
        if deck is None:
            raise ValueError(f'rom_order lists {ref}, which is null')
        ptrs[ref] = pos
        b = _cards_in(deck['cards'], ref)
        lists += b
        pos += len(b)
    if pos > lists_end:
        raise ValueError(f'the deck lists take 0x{pos - start:X} bytes, only 0x{lists_end - start:X} fit')
    out = lists + bytes(lists_end - pos)
    for key, at, n in DECK_TABLES:
        assert len(out) == at - start
        for i, deck in enumerate(d[key]):
            if deck is None:
                out += bytes(8)
            else:
                out += struct.pack('<I', ptrs[f'{key}/{i}']) + _pack('<H', len(deck['cards']), key) + \
                    _pack('<H', _int(deck.get('pad', 0)), key)
    if len(out) != end - start:
        raise ValueError('deck tables do not fill the range')
    return bytes(out)


# ----------------------------------------------------------------------------------------- passwords
def x_passwords(data, p, rom):
    ctx = ctx_for(rom)
    buf = io.StringIO()
    w = csv.writer(buf, lineterminator='\n')
    w.writerow(['id', 'password', 'number', 'name'])
    for i in range(len(data) // 4):
        b = data[i * 4:i * 4 + 4]
        if b == b'\xff' * 4:
            pw = ''
        elif all(x >> 4 < 10 and x & 15 < 10 for x in b):
            pw = b.hex()
        else:
            pw = 'raw:' + b.hex()
        number = name = ''
        if ctx and i < len(ctx.id2n):
            number = ctx.id2n[i]
            name = ctx.cstr(NAMES_AT + i * 0x40, 0x40) if i else ''
        w.writerow([i, pw, number, name])
    return {p['path']: buf.getvalue().encode()}


def b_passwords(read, p):
    n = (p['end'] - p['start']) // 4
    out = [None] * n
    for row in csv.DictReader(io.StringIO(read(p['path']).decode())):
        i = int(row['id'])
        if not 0 <= i < n or out[i] is not None:
            raise ValueError(f'passwords: id {row["id"]} is out of range or repeated')
        pw = (row['password'] or '').strip()
        if pw == '':
            out[i] = b'\xff' * 4
        elif pw.startswith('raw:'):
            out[i] = bytes.fromhex(pw[4:])
        elif re.fullmatch(r'\d{8}', pw):
            out[i] = bytes.fromhex(pw)
        else:
            raise ValueError(f'passwords: card {i}: password must be 8 digits or empty, not {pw!r}')
    if any(x is None for x in out):
        raise ValueError(f'passwords: every card id 0-{n - 1} needs a row')
    return b''.join(out)


# ---------------------------------------------------------------------------------------------- sine
def x_sine(data, p, rom):
    vals = list(struct.unpack(f'<{len(data) // 2}h', data))
    return {p['path']: _json({
        'doc': 'Sine table: 256 steps per turn, s16 in 4.12 fixed point (4096 = 1.0, [64] = 0x1000); the '
               'values are 4096*sin(2*pi*i/256) truncated toward zero. Used by the sound driver for vibrato '
               '(gVibratoSineTable, sound_driver.c).',
        'values': vals})}


def b_sine(read, p):
    vals = [_int(v) for v in json.loads(read(p['path']))['values']]
    if len(vals) * 2 != p['end'] - p['start']:
        raise ValueError(f'the sine table needs {(p["end"] - p["start"]) // 2} values')
    return b''.join(_pack('<h', v, 'sine') for v in vals)


# ------------------------------------------------------------------------------------- layout kinds
def _elem_size(kind, opts):
    if kind in NUM:
        return struct.calcsize(NUM[kind])
    if kind in ('cards', 'colors', 'sjis'):
        return 2
    if kind == 'ptrs':
        return 4
    if kind == 'oam':
        return 8
    if kind == 'struct':
        return sum(_field_size(t) for _, t in opts['fields'])
    return 0


def _field_size(t):
    if t.startswith(('text:', 'pad:')):
        return int(t.split(':')[1])
    return {'u8': 1, 's8': 1, 'u16': 2, 's16': 2, 'hex16': 2, 'card': 2, 'u32': 4, 's32': 4, 'hex32': 4,
            'ptr': 4}[t]


def _shape(flat, shape):
    if not shape:
        return flat
    step = len(flat) // shape[0]
    return [_shape(flat[i * step:(i + 1) * step], shape[1:]) for i in range(shape[0])]


def _unshape(nested, shape):
    if not shape:
        return list(nested)
    if len(nested) != shape[0]:
        raise ValueError(f'expected {shape[0]} rows, got {len(nested)}')
    return [x for row in nested for x in _unshape(row, shape[1:])]


def _check_count(n, opts):
    shape = opts.get('shape')
    if shape:
        total = 1
        for s in shape:
            total *= s
        if total != n:
            raise ValueError(f'shape {shape} does not cover {n} elements')


# Each kind: x(data, at, opts, ctx) -> dict of JSON fields; b(dict, at, size, opts) -> bytes.
def kx_num(kind):
    def x(data, at, opts, ctx):
        fmt = NUM[kind]
        n = len(data) // struct.calcsize(fmt)
        _check_count(n, opts)
        vals = list(struct.unpack(f'<{n}{fmt[1]}', data))
        if opts.get('hex'):
            vals = [_hex(v, struct.calcsize(fmt) * 2) for v in vals]
        return {'values': _shape(vals, opts.get('shape', [])[:-1])}

    def b(d, at, size, opts):
        vals = _unshape(d['values'], opts.get('shape', [])[:-1])
        return b''.join(_pack(NUM[kind], _int(v), 'values') for v in vals)
    return x, b


def kx_cards(data, at, opts, ctx):
    nums = struct.unpack(f'<{len(data) // 2}H', data)
    return {'cards': _cards_out(ctx, nums)}


def kb_cards(d, at, size, opts):
    return _cards_in(d['cards'], 'cards')


def kx_ptrs(data, at, opts, ctx):
    vals = struct.unpack(f'<{len(data) // 4}I', data)
    return {'pointers': [ctx.ptr(v, opts.get('str')) if ctx else (_hex(v, 8) if v else None) for v in vals]}


def kb_ptrs(d, at, size, opts):
    return b''.join(_pack('<I', _int(v), 'pointers') for v in d['pointers'])


def kx_struct(data, at, opts, ctx):
    fields = opts['fields']
    size = sum(_field_size(t) for _, t in fields)
    n = len(data) // size
    _check_count(n, opts)
    rows = []
    for i in range(n):
        off, row = i * size, {}
        for name, t in fields:
            fs = _field_size(t)
            raw = data[off:off + fs]
            off += fs
            if t.startswith('pad:'):
                if raw.strip(b'\0'):
                    row[name] = raw.hex()
            elif t.startswith('text:'):
                row[name] = A.text_field(raw)
            elif t == 'ptr':
                v = struct.unpack('<I', raw)[0]
                row[name] = _hex(v, 8) if v else None
                note = ctx.note(v) if ctx and v else None
                if note:
                    row['_' + name] = note
            elif t == 'card':
                v = struct.unpack('<H', raw)[0]
                row[name] = v
                nm = ctx.card_name(v) if ctx else None
                if nm is not None:
                    row['_' + name] = nm
            elif t in ('hex16', 'hex32'):
                row[name] = _hex(int.from_bytes(raw, 'little'), fs * 2)
            else:
                row[name] = struct.unpack(NUM[t], raw)[0]
        rows.append(row)
    return {'entries': _shape(rows, opts.get('shape', [])[:-1])}


def kb_struct(d, at, size, opts):
    out = bytearray()
    for row in _unshape(d['entries'], opts.get('shape', [])[:-1]):
        for name, t in opts['fields']:
            fs = _field_size(t)
            if t.startswith('pad:'):
                raw = bytes.fromhex(row.get(name, '')) if row.get(name) else bytes(fs)
                if len(raw) != fs:
                    raise ValueError(f'{name}: {fs} bytes of hex expected')
                out += raw
            elif t.startswith('text:'):
                out += A.field_bytes(row[name], fs)
            elif t in ('ptr', 'u32', 'hex32'):
                out += _pack('<I', _int(row[name]), name)
            elif t in ('card', 'hex16'):
                out += _pack('<H', _int(row[name]), name)
            else:
                out += _pack(NUM[t], _int(row[name]), name)
    return bytes(out)


def _oam_decode(a0, a1, a2, a3):
    shape, sz = a0 >> 14, a1 >> 14
    e = {'x': a1 & 0x1FF, 'y': a0 & 0xFF}
    if (shape, sz) in OAM_SIZES:
        e['size'] = OAM_SIZES[shape, sz]
    else:
        e['shape'], e['size'] = shape, sz
    e['tile'] = a2 & 0x3FF
    for k, v in (('pal', a2 >> 12), ('prio', (a2 >> 10) & 3), ('affine', (a0 >> 8) & 3), ('mode', (a0 >> 10) & 3),
                 ('mosaic', (a0 >> 12) & 1), ('bpp8', (a0 >> 13) & 1), ('a1_bits9_11', (a1 >> 9) & 7),
                 ('hflip', (a1 >> 12) & 1), ('vflip', (a1 >> 13) & 1), ('attr3', a3)):
        if v:
            e[k] = v
    return e


def _oam_encode(e):
    def f(k, bits):
        v = _int(e.get(k, 0))
        if not 0 <= v < 1 << bits:
            raise ValueError(f'OAM {k}={v} does not fit in {bits} bits')
        return v
    if isinstance(e['size'], str):
        if e['size'] not in OAM_SIZE_BITS:
            raise ValueError(f'OAM size {e["size"]!r} is not one of {sorted(OAM_SIZE_BITS)}')
        shape, sz = OAM_SIZE_BITS[e['size']]
    else:
        shape, sz = f('shape', 2), f('size', 2)
    a0 = f('y', 8) | f('affine', 2) << 8 | f('mode', 2) << 10 | f('mosaic', 1) << 12 | f('bpp8', 1) << 13 | shape << 14
    a1 = f('x', 9) | f('a1_bits9_11', 3) << 9 | f('hflip', 1) << 12 | f('vflip', 1) << 13 | sz << 14
    a2 = f('tile', 10) | f('prio', 2) << 10 | f('pal', 4) << 12
    return struct.pack('<4H', a0, a1, a2, f('attr3', 16))


def kx_oam(data, at, opts, ctx):
    if len(data) % 8:
        raise ValueError('OAM lists are 8-byte entries')
    end = at + len(data)
    refs = ctx.refs_in(at, end) if ctx else {}
    cuts = sorted({at} | {t for t in refs if (t - at) % 8 == 0} | {a for a in (ctx.labels if ctx else {})
                                                                    if at < a < end and (a - at) % 8 == 0})
    frames = []
    for s, e in zip(cuts, cuts[1:] + [end]):
        fr = {'at': _hex(s, 8)}
        if ctx and s in ctx.labels:
            fr['_label'] = ctx.labels[s]
        if s in refs:
            fr['_refs'] = [_hex(r, 8) for r in refs[s]]
        fr['oam'] = [_oam_decode(*struct.unpack_from('<4H', data, o - at)) for o in range(s, e, 8)]
        frames.append(fr)
    return {'format': 'Frames of OAM entries {attr0, attr1, attr2, attr3}. Entry fields: x (0-511), y (0-255), size '
                      '("WxH" pixels), tile (0-1023), and when non-zero pal, prio, affine, mode, mosaic, bpp8, '
                      'a1_bits9_11, hflip, vflip, attr3. A frame starts at "at"; other data points there ("_refs" '
                      'are the addresses of those pointers), so entries can be edited but not added or removed.',
            'frames': frames}


def kb_oam(d, at, size, opts):
    out = bytearray()
    for fr in d['frames']:
        if _int(fr['at']) != at + len(out):
            raise ValueError(f'OAM frame {fr["at"]} would move to 0x{at + len(out):08X}: entries can be edited, '
                             f'but not added or removed (other data points at each frame)')
        for e in fr['oam']:
            out += _oam_encode(e)
    return bytes(out)


def kx_packs(data, at, opts, ctx):
    table = opts['table']
    end = at + len(data)
    packs, expect = [], at
    for i in range((end - table) // 8):
        ptr, pid, pad = struct.unpack_from('<IHH', data, table - at + i * 8)
        slots = []
        for s in range(8):
            cp, cnt = struct.unpack_from('<Ii', data, ptr - at + s * 8)
            if cnt == 0 and cp == 0:
                slots.append([])
                continue
            if cnt <= 0 or cp != expect:
                raise ValueError(f'pack {pid} slot {s} is not where the packed layout puts it')
            slots.append(_cards_out(ctx, struct.unpack_from(f'<{cnt}H', data, cp - at)))
            expect += 2 * cnt
        expect = (expect + 3) & ~3
        if ptr != expect:
            raise ValueError(f'pack {pid}: slot table not where the packed layout puts it')
        expect += 0x40
        e = {'id': pid}
        if ctx and pid in ctx.packs:
            e['_name'] = ctx.packs[pid]
        e['slots'] = slots
        if pad:
            e['pad'] = pad
        packs.append(e)
    return {'format': 'Each pack: "id" (shop/pack ID) and 8 "slots" of card numbers, slot 0 = rarest ... slot 7 = '
                      'commons; an empty slot is stored as {NULL, 0}. Lists may change size: the build re-packs '
                      'them (lists, 4-byte alignment, {cards, count}[8]) and zero-fills up to the table at '
                      '0x081A562C. The number of packs is fixed at 28.',
            'packs': packs}


def kb_packs(d, at, size, opts):
    table = opts['table']
    n = (at + size - table) // 8
    if len(d['packs']) != n:
        raise ValueError(f'booster_packs needs exactly {n} packs')
    out, entries = bytearray(), []
    for pk in d['packs']:
        if len(pk['slots']) != 8:
            raise ValueError(f'pack {pk["id"]}: 8 slots expected')
        slots = []
        for cards in pk['slots']:
            if cards:
                slots.append((at + len(out), len(cards)))
                out += _cards_in(cards, f'pack {pk["id"]}')
            else:
                slots.append((0, 0))
        out += bytes(-len(out) & 3)
        entries.append(struct.pack('<I', at + len(out)) + _pack('<H', _int(pk['id']), 'id') +
                       _pack('<H', _int(pk.get('pad', 0)), 'pad'))
        out += b''.join(struct.pack('<Ii', p, c) for p, c in slots)
    if len(out) > table - at:
        raise ValueError(f'pack lists take 0x{len(out):X} bytes, only 0x{table - at:X} fit before 0x{table:08X}')
    return bytes(out) + bytes(table - at - len(out)) + b''.join(entries)


def kx_colors(data, at, opts, ctx):
    cols = struct.unpack(f'<{len(data) // 2}H', data)
    d = {'colors': ['#%02X%02X%02X' % A.bgr555_to_rgb(c) for c in cols]}
    hi = [i for i, c in enumerate(cols) if c & 0x8000]
    if hi:
        d['bit15'] = hi
    return d


def kb_colors(d, at, size, opts):
    vals = []
    for s in d['colors']:
        s = s.lstrip('#')
        vals.append(A.rgb_to_bgr555((int(s[0:2], 16), int(s[2:4], 16), int(s[4:6], 16))))
    for i in d.get('bit15', []):
        vals[i] |= 0x8000
    return struct.pack(f'<{len(vals)}H', *vals)


def kx_sjis(data, at, opts, ctx):
    rows = []
    for i, (v,) in enumerate(struct.iter_unpack('<H', data)):
        r = {'ascii': chr(0x20 + i), 'sjis': _hex(v, 4)}
        try:
            if v:
                r['_glyph'] = bytes([v >> 8, v & 0xFF]).decode('shift_jis')
        except UnicodeDecodeError:
            pass
        rows.append(r)
    return {'map': rows}


def kb_sjis(d, at, size, opts):
    return b''.join(_pack('<H', _int(r['sjis']), 'sjis') for r in d['map'])


def kx_text(data, at, opts, ctx):
    return {'text': A.text_field(data)}


def kb_text(d, at, size, opts):
    return A.field_bytes(d['text'], size)


KINDS = {k: kx_num(k) for k in NUM}
KINDS.update({'cards': (kx_cards, kb_cards), 'ptrs': (kx_ptrs, kb_ptrs), 'struct': (kx_struct, kb_struct),
              'oam': (kx_oam, kb_oam), 'packs': (kx_packs, kb_packs), 'colors': (kx_colors, kb_colors),
              'sjis': (kx_sjis, kb_sjis), 'text': (kx_text, kb_text)})


def _subtables(p):
    name = p.get('layout')
    if name not in LAYOUTS:
        raise ValueError(f'layout={name} is not one of {sorted(LAYOUTS)}')
    start, end, subs = LAYOUTS[name]
    if (start, end) != (p['start'], p['end']):
        raise ValueError(f'layout {name} covers 0x{start:08X}-0x{end:08X}, the manifest row does not')
    ends = [s[0] for s in subs[1:]] + [end]
    return [(at, e, name, kind, opts, users, doc) for (at, name, kind, opts, users, doc), e in zip(subs, ends)]


def x_layout(data, p, rom):
    ctx = ctx_for(rom)
    files = {}
    for at, end, name, kind, opts, users, doc in _subtables(p):
        raw = data[at - p['start']:end - p['start']]
        rel = f"{p['path']}/{name}.json"
        if kind == 'pad':
            if raw.strip(b'\0'):
                files[rel] = _json({'doc': doc, 'address': _hex(at, 8), 'hex': raw.hex()})
            continue
        es = _elem_size(kind, opts)
        if es and len(raw) % es:
            raise ValueError(f'{name}: 0x{len(raw):X} bytes is not a whole number of elements')
        head = {'doc': doc, 'address': _hex(at, 8), 'size': _hex(len(raw), 0)}
        if ctx:
            labs = [f'{n} (+0x{a - at:X})' if a != at else n for a, n in sorted(ctx.labels.items())
                    if at <= a < end]
            if labs and kind != 'oam':
                head['labels'] = labs
        if users:
            head['users'] = users
        head.update(KINDS[kind][0](raw, at, opts, ctx))
        files[rel] = _json(head)
    return files


def b_layout(read, p):
    out = bytearray()
    for at, end, name, kind, opts, users, doc in _subtables(p):
        rel = f"{p['path']}/{name}.json"
        if kind == 'pad':
            d = _optional_json(read, rel)
            b = bytes.fromhex(d['hex']) if d else bytes(end - at)
        else:
            d = json.loads(read(rel))
            try:
                b = KINDS[kind][1](d, at, end - at, opts)
            except (KeyError, TypeError, IndexError, AttributeError) as e:
                raise ValueError(f'{rel}: missing or malformed field ({e!r})')
        if len(b) != end - at:
            raise ValueError(f'{rel}: built 0x{len(b):X} bytes, the table is 0x{end - at:X} '
                             f'(the ROM layout is fixed: keep the number of entries)')
        out += b
    return bytes(out)
