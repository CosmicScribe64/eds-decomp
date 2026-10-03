"""Sound sequences of the Konami driver (wiki/game/sound-engine.md): the SE and BGM tables, their track
bytecode, and the driver's lookup tables, as text and JSON that build back to the exact ROM bytes.

  sound_seq_se_table     SE table, 28-byte entries {u32 track[6]; u8 priority; u8 slotMask; u16 lock} -> JSON
                         (tracks=DIR names the se_tracks directory, for the annotations only)
  sound_seq_se_tracks    SE bytecode (absolute pointers) -> <path>/index.txt + one .txt per track entry
                         (table=ADDR count=N: the SE table, read at extraction to find the entry points)
  sound_seq_song_table   BGM song table, 24-byte entries {u32 data; u16 trackOffset[10]} -> JSON
  sound_seq_song_tracks  BGM bytecode (offsets relative to the song) -> <path>/index.txt + one .txt per song
                         (table=ADDR count=N: the song table)
  sound_seq_lookup       driver lookup tables (volume scale, PCM pitch, PSG frequency) -> JSON

The command sets are those of the matched decoders in src/sound_driver.c: SoundSeTrackTick decodes SE tracks and
SoundSequencerTick decodes BGM tracks. Text files have one event per line, `<ticks> <command> [args]`, where <ticks>
is the number of frames (VBlanks) until the next event; `;` starts a comment. The ROM layout stays fixed:
every song and SE track is assembled at the address its `.song`/`.org` line gives, and the gaps (the ROM has
none apart from the trailing padding) are zero-filled.

SE track commands (SoundSeTrackTick). Every event is `<ticks> <command>`; in the ROM the tick byte comes first.
  freq F vol=V        0xB0: set the channel to raw frequency register value F (0x000-0xFFF), volume V
  note N vol=V        0xE0: PSG-table pitch N (1/32 semitones from C2, 0x000-0xFFF), volume V
  freq_rel delta=D vol_delta=W   0xA0: raw frequency base+D (-1024..1023), volume current+W (-16..15)
  note_rel delta=D vol_delta=W   0xC0: same on the PSG-table pitch
  note_add delta=D vol_delta=W [x=N]   0xD0: base pitch + D (s16)
     the five tone commands also take env=0-3 (low bits of the envelope byte), temp (do not make this the
     new base pitch) and silent (only update the base, send nothing to the channel)
  noise vol=V nr43=B  0x90: noise channel: volume V, SOUND4CNT_H frequency byte B (restarts the channel)
  setenv vol=V env=B  0x80: store volume V and envelope byte B for the channel without sending them
  pcm sample=S vol=V link=L [pitch=P] [wait] [x=N]   0x60: start PCM bank-1 sample S (sound/pcm_bank1_*) at
                      volume V and pitch P (PCM pitch-table index, default 0); the next L SE tracks are held
                      silent until this one ends; wait: with ticks 0, wait for the sample to end, otherwise
                      key it off when the ticks run out
  pitch P             0x30: base pitch = P (12 bits) and send it
  vol V / vol_add D [temp]   0x50 / 0x40: set / add to the volume (vol_add clamps to 0..63; temp: send only)
  break_loop track=T  0x70: set SE track T's loop counter to 1
  call L / jump L / loop N L   0xFA / 0xFB / 0xFC: subroutine call (returns at `end`), jump, and a loop that
                      jumps back to L N more times (N=0: jump unless the SE is being stopped)
  waitpcm (0xF9)  silence (0xFD, note off)  keyoff (0xFE, PCM key-off)  end (0xFF: return, or end the track)
  nop [x=N] (0xF0-0xF8)   raw 0xNN [bytes] (any other opcode, emitted as given: 0x00-0x2F hang the driver)

BGM track commands (SoundSequencerTick). Every event is `<ticks> <command>`; in the ROM the tick byte follows the
command. A track starts with `<ticks> wait` (just the tick byte) and ends with end, loop or stop (no ticks).
  note N vol=V        0xD0: pitch = note N (a name such as C#4 on PSG tone tracks, else 0-255); restarts the
                      note when V differs from the current volume
  vol V               0xC0: set volume V (PSG: restarts the note; songs use it as a volume envelope)
  play inst=I vol=V [note=S]   0xA0/0xB0: start instrument I (PCM: bank-0 sample I) at volume V, S semitones
                      from its base pitch (signed; without note= the 0xA0 form, base pitch)
  off [x=N]           0xE0: note off (volume 0)
  vibrato D [alias=0xF4-0xFC]   0xF1: vibrato depth D >> 1 (0 stops it)
  loopstart           0xF3: loop point of this track; `loop` restarts every track from its loop point
  bend D              0xF2: pitch = current pitch + D/32 semitones, without restarting
  pan B               0xF0: PSG tracks: NR51 output bits; PCM tracks: volume B & 15, next voice's volume B >> 4
  wave N              0x80: PSG instrument / wave N (N > 3 loads wave-RAM bank N-4)
  call 0xPPPP count=C [wave=N]   0x90: play C events from position P of this track, then return
  rest [0xNN]         0x00-0x7F: nothing (wait)
  end (0xFD: this track stops)  loop (0xFE: all tracks jump to their loopstart)  stop (0xFF: the song stops)
"""
import json
import re
import struct

BASE = 0x08000000

SE_SLOTS = ['sq2', 'noise', 'pcm5', 'pcm4', 'pcm3', 'pcm2']     # SE track k plays on this channel (variant 0)
SE_SLOT_DESC = {'sq2': 'PSG square 2', 'noise': 'PSG noise', 'pcm5': 'PCM voice 5', 'pcm4': 'PCM voice 4',
                'pcm3': 'PCM voice 3', 'pcm2': 'PCM voice 2'}
BGM_TRACKS = ['sq1', 'sq2', 'wave', 'noise', 'pcm0', 'pcm1', 'pcm2', 'pcm3', 'pcm4', 'pcm5']
BGM_TRACK_DESC = {'sq1': 'PSG square 1', 'sq2': 'PSG square 2', 'wave': 'PSG wave', 'noise': 'PSG noise',
                  **{f'pcm{i}': f'PCM voice {i}' for i in range(6)}}
# Names from game code (wiki/functions/sound-api.md); hypotheses unless the wiki says otherwise.
SONG_NAMES = {0: 'title screen', 1: 'New Game intro script', 3: 'main menu', 0x1B: 'Campaign pre-duel'}
SE_NAMES = {0: 'cursor move (hypothesis)', 1: 'confirm (hypothesis)', 2: 'cancel / back (hypothesis)',
            3: 'error buzz (hypothesis)'}
NOTE_NAMES = ['C', 'C#', 'D', 'D#', 'E', 'F', 'F#', 'G', 'G#', 'A', 'A#', 'B']
PSG_NOTES = 84            # the PSG frequency table covers C2..B8


class AsmError(ValueError):
    pass


# ---------------------------------------------------------------------------------------------- helpers
def _num(tok, where, lo=None, hi=None):
    """Decimal (optionally signed) or 0x-hex integer, range-checked."""
    t = tok.strip()
    m = re.fullmatch(r'([+-]?)(0[xX][0-9A-Fa-f]+|\d+)', t)
    if not m:
        raise AsmError(f'{where}: expected a number, got {tok!r}')
    v = int(m.group(2), 16) if m.group(2)[:2] in ('0x', '0X') else int(m.group(2))
    v = -v if m.group(1) == '-' else v
    if (lo is not None and v < lo) or (hi is not None and v > hi):
        raise AsmError(f'{where}: {tok} is out of range {lo}..{hi}')
    return v


def note_name(n):
    m = 36 + n                                   # PSG table entry 0 is C2 (MIDI 36, 65.41 Hz)
    return f'{NOTE_NAMES[m % 12]}{m // 12 - 1}'


def note_value(tok, where):
    m = re.fullmatch(r'([A-G])(#?)(-?\d+)', tok)
    if m:
        n = (int(m.group(3)) + 1) * 12 + NOTE_NAMES.index(m.group(1) + m.group(2)) - 36
        if not 0 <= n <= 255:
            raise AsmError(f'{where}: note {tok} is outside C2..{note_name(255)}')
        return n
    return _num(tok, where, 0, 255)


class Args:
    """Arguments after the mnemonic: positional words and key=value pairs."""

    def __init__(self, toks, where):
        self.pos, self.kw, self.where = [], {}, where
        for t in toks:
            if '=' in t:
                k, v = t.split('=', 1)
                if k in self.kw:
                    raise AsmError(f'{where}: {k}= given twice')
                self.kw[k] = v
            else:
                self.pos.append(t)

    def flag(self, name):
        if name in self.pos:
            self.pos.remove(name)
            return True
        return False

    def take(self, what):
        if not self.pos:
            raise AsmError(f'{self.where}: missing {what}')
        return self.pos.pop(0)

    def num(self, what, lo, hi):
        return _num(self.take(what), self.where, lo, hi)

    def key(self, k, lo, hi, default=None):
        if k not in self.kw:
            if default is None:
                raise AsmError(f'{self.where}: missing {k}=')
            return default
        return _num(self.kw.pop(k), self.where, lo, hi)

    def done(self):
        if self.pos or self.kw:
            extra = self.pos + [f'{k}={v}' for k, v in self.kw.items()]
            raise AsmError(f'{self.where}: unexpected {" ".join(extra)}')


def _lines(text, fname):
    """(where, tokens) for each non-empty line, comments (;) removed."""
    for i, line in enumerate(text.splitlines(), 1):
        toks = line.split(';', 1)[0].split()
        if toks:
            yield f'{fname}:{i}', toks


def _ticks(tok, where):
    """Event length column: N, or NL for the (non-canonical) two-byte form of a value below 0xF0."""
    long = tok.endswith('L')
    n = _num(tok[:-1] if long else tok, where, 0, 0xFFF)
    return n, long


def enc_delay(n, long=False):
    if n >= 0xF0 or long:
        return bytes([0xF0 | n >> 8, n & 0xFF])
    return bytes([n])


def dec_delay(b, pos):
    """-> (ticks, long, new pos). Raises IndexError past the end."""
    d = b[pos]
    if d >= 0xF0:
        n = (d & 0xF) << 8 | b[pos + 1]
        return n, n < 0xF0, pos + 2
    return d, False, pos + 1


def _ticks_text(n, long):
    return f'{n}L' if long and n < 0xF0 else str(n)


def _bytes_line(bs):
    return '.byte ' + ' '.join(f'0x{x:02X}' for x in bs)


def _place(size, base, chunks, what):
    """Lay out (address, bytes, origin) chunks in a zero-filled image of the asset."""
    out = bytearray(size)
    used = []
    for addr, bs, origin in sorted(chunks, key=lambda c: c[0]):
        if not bs:
            continue
        if addr < base or addr + len(bs) > base + size:
            raise AsmError(f'{origin}: 0x{addr:08X}-0x{addr + len(bs):08X} is outside {what} '
                           f'(0x{base:08X}-0x{base + size:08X})')
        if used and used[-1][0] > addr:
            raise AsmError(f'{origin}: starts at 0x{addr:08X}, but {used[-1][1]} runs to 0x{used[-1][0]:08X}. '
                           f'The layout is fixed: shorten a track or move it (and its table pointer)')
        out[addr - base:addr - base + len(bs)] = bs
        used.append((addr + len(bs), origin))
    return bytes(out)


def _index(files, header):
    return ('\n'.join(header + files) + '\n').encode()


def _read_index(read, path):
    names = []
    for _, toks in _lines(read(f'{path}/index.txt').decode(), f'{path}/index.txt'):
        names.extend(toks)
    return names


# ------------------------------------------------------------------------------------------- SE bytecode
def se_oplen(c):
    if c < 0x30:
        return 0          # 0x20-0x2F peek at the next byte without consuming it; 0x00-0x1F never return
    if c < 0x50:
        return 1
    if c < 0x60:
        return 0
    if c < 0x70:
        return 5 if c & 4 else 3
    if c < 0x80:
        return 0
    if c < 0xA0:
        return 1
    if c < 0xD0:
        return 2
    if c < 0xE0:
        return 3
    if c < 0xF0:
        return 2
    if c in (0xFA, 0xFB):
        return 4
    if c == 0xFC:
        return 5
    return 0


def _se_tone_flags(c):
    s = ''
    if c & 3:
        s += f' env={c & 3}'
    if c & 4:
        s += ' temp'
    if c & 8:
        s += ' silent'
    return s


def se_text(c, ops, label):
    """One SE command (opcode + operands) -> text. label(addr) names a branch target."""
    hi = c & 0xF0
    if c < 0x30:
        return f'raw 0x{c:02X}'
    if hi == 0x30:
        return f'pitch 0x{(c & 0xF) << 8 | ops[0]:03X}'
    if hi == 0x40:
        d = ops[0] - 256 if ops[0] > 127 else ops[0]
        return f'vol_add {d:+d}' + (' temp' if c & 1 else '') + (f' x={c >> 1 & 7}' if c & 0xE else '')
    if hi == 0x50:
        return f'vol {c & 0xF}'
    if hi == 0x60:
        s = f'pcm sample={ops[0] | ops[1] << 8} vol={ops[2] & 0xF} link={ops[2] >> 4}'
        if c & 4:
            s += f' pitch={ops[3] | ops[4] << 8}'
        if c & 8:
            s += ' wait'
        if c & 3:
            s += f' x={c & 3}'
        return s
    if hi == 0x70:
        return f'break_loop track={c & 0xF}'
    if hi == 0x80:
        return f'setenv vol={c & 0xF} env=0x{ops[0]:02X}'
    if hi == 0x90:
        return f'noise vol={c & 0xF} nr43=0x{ops[0]:02X}'
    if hi in (0xA0, 0xC0):
        v = ops[0] | ops[1] << 8
        return (f'{"freq_rel" if hi == 0xA0 else "note_rel"} delta={(v >> 5) - 0x400:+d} '
                f'vol_delta={(v & 0x1F) - 0x10:+d}' + _se_tone_flags(c))
    if hi in (0xB0, 0xE0):
        v = ops[0] | ops[1] << 8
        return f'{"freq" if hi == 0xB0 else "note"} 0x{v >> 4:03X} vol={v & 0xF}' + _se_tone_flags(c)
    if hi == 0xD0:
        d = struct.unpack('<h', ops[:2])[0]
        return (f'note_add delta={d:+d} vol_delta={(ops[2] & 0x1F) - 0x10:+d}'
                + (f' x={ops[2] >> 5}' if ops[2] >> 5 else '') + _se_tone_flags(c))
    if c < 0xF9:
        return 'nop' + (f' x={c & 0xF}' if c & 0xF else '')
    if c == 0xF9:
        return 'waitpcm'
    if c in (0xFA, 0xFB):
        return f'{"call" if c == 0xFA else "jump"} {label(struct.unpack("<I", ops)[0])}'
    if c == 0xFC:
        return f'loop {ops[0]} {label(struct.unpack("<I", ops[1:])[0])}'
    return {0xFD: 'silence', 0xFE: 'keyoff', 0xFF: 'end'}[c]


def _se_tone(a, op):
    c = op | a.key('env', 0, 3, 0) | (4 if a.flag('temp') else 0) | (8 if a.flag('silent') else 0)
    return c


def se_encode(mn, a, resolve):
    """mnemonic + Args -> opcode and operand bytes. resolve(token) -> address of a label or number."""
    if mn == 'raw':
        bs = [a.num('byte', 0, 255)]
        while a.pos:
            bs.append(a.num('byte', 0, 255))
        return bytes(bs)
    if mn == 'pitch':
        v = a.num('pitch', 0, 0xFFF)
        return bytes([0x30 | v >> 8, v & 0xFF])
    if mn == 'vol_add':
        d = a.num('delta', -128, 127)
        return bytes([0x40 | (1 if a.flag('temp') else 0) | a.key('x', 0, 7, 0) << 1, d & 0xFF])
    if mn == 'vol':
        return bytes([0x50 | a.num('volume', 0, 15)])
    if mn == 'pcm':
        sample, vol, link = a.key('sample', 0, 0xFFFF), a.key('vol', 0, 15), a.key('link', 0, 15)
        pitch = a.key('pitch', 0, 0xFFFF, -1)
        c = 0x60 | a.key('x', 0, 3, 0) | (4 if pitch >= 0 else 0) | (8 if a.flag('wait') else 0)
        bs = bytes([c]) + struct.pack('<HB', sample, link << 4 | vol)
        return bs + (struct.pack('<H', pitch) if pitch >= 0 else b'')
    if mn == 'break_loop':
        return bytes([0x70 | a.key('track', 0, 15)])
    if mn in ('setenv', 'noise'):
        vol = a.key('vol', 0, 15)
        return bytes([(0x80 if mn == 'setenv' else 0x90) | vol, a.key('env' if mn == 'setenv' else 'nr43', 0, 255)])
    if mn in ('freq_rel', 'note_rel'):
        d, vd = a.key('delta', -0x400, 0x3FF), a.key('vol_delta', -16, 15)
        c = _se_tone(a, 0xA0 if mn == 'freq_rel' else 0xC0)
        return bytes([c]) + struct.pack('<H', (d + 0x400) << 5 | (vd + 16))
    if mn in ('freq', 'note'):
        v = a.num('value', 0, 0xFFF)
        vol = a.key('vol', 0, 15)
        return bytes([_se_tone(a, 0xB0 if mn == 'freq' else 0xE0)]) + struct.pack('<H', v << 4 | vol)
    if mn == 'note_add':
        d, vd, x = a.key('delta', -0x8000, 0x7FFF), a.key('vol_delta', -16, 15), a.key('x', 0, 7, 0)
        return bytes([_se_tone(a, 0xD0)]) + struct.pack('<hB', d, x << 5 | (vd + 16))
    if mn == 'nop':
        return bytes([0xF0 | a.key('x', 0, 8, 0)])
    if mn in ('call', 'jump'):
        return bytes([0xFA if mn == 'call' else 0xFB]) + struct.pack('<I', resolve(a.take('target')))
    if mn == 'loop':
        n = a.num('count', 0, 255)
        return bytes([0xFC, n]) + struct.pack('<I', resolve(a.take('target')))
    simple = {'waitpcm': 0xF9, 'silence': 0xFD, 'keyoff': 0xFE, 'end': 0xFF}
    if mn in simple:
        return bytes([simple[mn]])
    raise AsmError(f'{a.where}: unknown SE command {mn!r}')


def se_disasm(blob, base, start, end, label, last):
    """SE events of blob[start:end] (offsets) -> text lines. After an `end`, all-zero bytes up to the end of
    the last chunk are padding and are left out (the build zero-fills)."""
    lines, pos = [], start
    while pos < end:
        if last and blob[pos:end].strip(b'\0') == b'' and lines and lines[-1].split()[-1] == 'end':
            break
        try:
            n, long, p2 = dec_delay(blob, pos)
            c = blob[p2]
            k = se_oplen(c)
            if p2 + 1 + k > end:
                raise IndexError
        except IndexError:
            lines.append(f'    {_bytes_line(blob[pos:end])}')
            break
        ops = blob[p2 + 1:p2 + 1 + k]
        ll = label(base + pos, inline=True)
        if ll:
            lines.append(f'{ll}:')
        lines.append(f'{_ticks_text(n, long):>5}  {se_text(c, ops, label)}')
        pos = p2 + 1 + k
    return lines


def _se_targets(blob, base, start, end):
    """Branch targets (call targets, jump/loop targets) of a chunk, by linear parse."""
    calls, jumps, starts, pos = set(), set(), set(), start
    while pos < end:
        try:
            starts.add(base + pos)
            _, _, p2 = dec_delay(blob, pos)
            c = blob[p2]
            k = se_oplen(c)
            if p2 + 1 + k > end:
                break
        except IndexError:
            break
        ops = blob[p2 + 1:p2 + 1 + k]
        if c in (0xFA, 0xFB):
            (calls if c == 0xFA else jumps).add(struct.unpack('<I', ops)[0])
        elif c == 0xFC:
            jumps.add(struct.unpack('<I', ops[1:])[0])
        pos = p2 + 1 + k
    return calls, jumps, starts


def _se_entries(table, count):
    """SE table bytes -> {track address: [(se id, slot index), ...]} in table order."""
    ent = {}
    for i in range(count):
        for k, ptr in enumerate(struct.unpack_from('<6I', table, i * 28)):
            if ptr:
                ent.setdefault(ptr, []).append((i, k))
    return ent


def _se_label(users):
    i, k = users[0]
    return f'se{i:02d}_{SE_SLOTS[k]}'


def x_se_tracks(data, p, rom):
    tbl, count = int(p['table'], 16), int(p['count'])
    lo, hi = p['start'], p['end']
    entries = {a: u for a, u in _se_entries(rom[tbl - BASE:tbl - BASE + count * 28], count).items() if lo <= a < hi}
    bounds = set(entries) | {lo}
    while True:                                   # call targets start their own file
        cuts = sorted(bounds) + [hi]
        new = set()
        for s, e in zip(cuts, cuts[1:]):
            calls, _, _ = _se_targets(data, lo, s - lo, e - lo)
            new |= {t for t in calls if lo <= t < hi} - bounds
        if not new:
            break
        bounds |= new
    cuts = sorted(bounds) + [hi]
    names, starts, jumps = {}, set(), set()
    for s, e in zip(cuts, cuts[1:]):
        _, j, st = _se_targets(data, lo, s - lo, e - lo)
        jumps |= j
        starts |= st
    for a in sorted(bounds):
        names[a] = _se_label(entries[a]) if a in entries else f'sub_{a:08X}'
    for t in sorted(jumps):
        if lo <= t < hi and t not in names and t in starts:
            names[t] = f'loc_{t:08X}'

    def label(addr, inline=False):
        if inline:
            return names.get(addr) if addr not in bounds else None
        return names.get(addr, f'0x{addr:08X}')

    files, index = {}, []
    for n, (s, e) in enumerate(zip(cuts, cuts[1:])):
        name = names[s]
        head = [f'; {name}: Konami SE track. Format: tools/assetfmt/sound_seq.py, wiki/game/sound-engine.md.',
                '; One event per line: <ticks> <command> [args]; <ticks> = frames until the next event.']
        if s in entries:
            uses = ', '.join(f'SE {i} track {k} ({SE_SLOTS[k]}, {SE_SLOT_DESC[SE_SLOTS[k]]})'
                             for i, k in entries[s])
            head.append(f'; Played by: {uses}')
        else:
            head.append('; Not in the SE table: reached through call/jump only.')
        head += [f'.org 0x{s:08X}', f'{name}:']
        body = se_disasm(data, lo, s - lo, e - lo, label, n == len(cuts) - 2)
        files[f"{p['path']}/{name}.txt"] = ('\n'.join(head + body) + '\n').encode()
        index.append(f'{name}.txt')
    files[f"{p['path']}/index.txt"] = _index(index, [
        '; SE track files, assembled in this order. Each starts with `.org ADDRESS`, its fixed ROM address,',
        '; which the SE table (se_table.json) points to. Labels are shared by all files.'])
    return files


def _se_assemble(text, fname, labels, pass2):
    """-> list of (address, bytes, origin). Pass 1 (pass2=False) fills `labels`."""
    chunks, cur, addr0, origin, org = [], None, None, fname, fname

    def resolve(tok):
        if tok in labels:
            return labels[tok]
        if re.fullmatch(r'0[xX][0-9A-Fa-f]+', tok):
            return int(tok, 16)
        if pass2:
            raise AsmError(f'{origin}: unknown label {tok!r}')
        return 0

    for where, toks in _lines(text, fname):
        origin = where
        if toks[0] == '.org':
            if cur is not None:
                chunks.append((addr0, bytes(cur), org))
            addr0, cur, org = _num(toks[1], where), bytearray(), f'{where} (.org)'
            continue
        if cur is None:
            raise AsmError(f'{where}: the file must start with .org ADDRESS')
        if len(toks) == 1 and toks[0].endswith(':'):
            name = toks[0][:-1]
            if not pass2:
                if name in labels:
                    raise AsmError(f'{where}: label {name} is defined twice')
                labels[name] = addr0 + len(cur)
            continue
        if toks[0] == '.byte':
            cur += bytes(_num(t, where, 0, 255) for t in toks[1:])
            continue
        n, long = _ticks(toks[0], where)
        if len(toks) < 2:
            raise AsmError(f'{where}: missing command after the tick count')
        a = Args(toks[2:], where)
        bs = se_encode(toks[1], a, resolve)
        a.done()
        cur += enc_delay(n, long) + bs
    if cur is not None:
        chunks.append((addr0, bytes(cur), org))
    return chunks


def b_se_tracks(read, p):
    names = _read_index(read, p['path'])
    texts = [(n, read(f"{p['path']}/{n}").decode()) for n in names]
    labels = {}
    for n, t in texts:
        _se_assemble(t, f"{p['path']}/{n}", labels, False)
    chunks = []
    for n, t in texts:
        chunks += _se_assemble(t, f"{p['path']}/{n}", labels, True)
    return _place(p['end'] - p['start'], p['start'], chunks, p['path'])


# --------------------------------------------------------------------------------------------- SE table
def x_se_table(data, p, rom):
    count = len(data) // 28
    if len(data) % 28:
        raise ValueError('SE table size is not a multiple of 28')
    entries = _se_entries(data, count)
    tdir = p.get('tracks', 'sound/se_tracks')
    rows = []
    for i in range(count):
        e = struct.unpack_from('<6IBBH', data, i * 28)
        row = {'id': i}
        if i in SE_NAMES:
            row['_name'] = SE_NAMES[i]
        row.update(priority=e[6], slot_mask=f'0x{e[7]:02X}', lock_ticks=e[8],
                   tracks={SE_SLOTS[k]: f'0x{v:08X}' for k, v in enumerate(e[:6]) if v})
        row['_files'] = {SE_SLOTS[k]: f'{tdir}/{_se_label(entries[v])}.txt' for k, v in enumerate(e[:6]) if v}
        rows.append(row)
    doc = {
        '_format': 'Konami driver SE table (sound/se_table). Each effect has up to six tracks, one per SE '
                   'channel; tracks maps channel -> ROM address of its bytecode (se_tracks/*.txt). '
                   'priority: a request cannot take a channel whose playing SE has a higher priority. '
                   'slot_mask: channels claimed (0x20 sq2, 0x10 noise, 0x08 pcm5, 0x04 pcm4, 0x02 pcm3, '
                   '0x01 pcm2; SoundRequestSE variants 1-3 rotate the PCM bits). lock_ticks: starting this SE sets the '
                   'driver\'s SE lock counter to this value (it counts down once per frame while SEs play); '
                   'an SE whose lock_ticks is nonzero is refused while the counter is nonzero. '
                   'Keys starting with _ are notes and are ignored.',
        '_channels': {k: SE_SLOT_DESC[k] for k in SE_SLOTS},
        'effects': rows,
    }
    return {p['path']: _dumps(doc)}


def b_se_table(read, p):
    doc = json.loads(read(p['path']))
    out = bytearray()
    for i, e in enumerate(doc['effects']):
        where = f"{p['path']}: effect {i}"
        if e.get('id', i) != i:
            raise AsmError(f'{where}: ids must run 0, 1, 2, ... (got {e.get("id")})')
        bad = set(e.get('tracks', {})) - set(SE_SLOTS)
        if bad:
            raise AsmError(f'{where}: unknown channel(s) {sorted(bad)}; use {SE_SLOTS}')
        ptrs = [_ptr(e.get('tracks', {}).get(k), where) for k in SE_SLOTS]
        out += struct.pack('<6IBBH', *ptrs, _int(e['priority'], where, 0, 255), _int(e['slot_mask'], where, 0, 255),
                           _int(e['lock_ticks'], where, 0, 0xFFFF))
    return bytes(out)


def _ptr(v, where):
    if v is None:
        return 0
    return _int(v, where, 0, 0xFFFFFFFF)


def _int(v, where, lo, hi):
    if isinstance(v, bool) or not isinstance(v, (int, str)):
        raise AsmError(f'{where}: expected a number, got {v!r}')
    return _num(str(v).split()[0], where, lo, hi) if isinstance(v, str) else _num(str(v), where, lo, hi)


def _dumps(doc):
    """Top-level keys on their own lines; lists of records one record per line."""
    out = []
    for k, v in doc.items():
        if isinstance(v, list):
            out.append(f'  {json.dumps(k)}: [\n' + ',\n'.join('    ' + json.dumps(x) for x in v) + '\n  ]')
        else:
            out.append(f'  {json.dumps(k)}: {json.dumps(v)}')
    return ('{\n' + ',\n'.join(out) + '\n}\n').encode()


# ------------------------------------------------------------------------------------------ BGM bytecode
def bgm_oplen(c):
    if c < 0x90:
        return 0
    if c < 0xA0:
        return 3
    if c < 0xB0:
        return 1
    if c < 0xC0:
        return 2
    if c < 0xD0:
        return 0
    if c < 0xE0:
        return 1
    if c < 0xF0:
        return 0
    if c == 0xF3 or c >= 0xFD:
        return 0
    return 1


def bgm_text(c, ops, names):
    hi = c & 0xF0
    if c < 0x80:
        return 'rest' + (f' 0x{c:02X}' if c else '')
    if hi == 0x80:
        return f'wave {c & 0xF}'
    if hi == 0x90:
        return (f'call 0x{ops[0] | ops[1] << 8:04X} count={ops[2]}'
                + (f' wave={c & 0xF}' if c & 0xF != 0xF else ''))
    if hi == 0xA0:
        return f'play inst={ops[0]} vol={c & 0xF}'
    if hi == 0xB0:
        s = ops[1] - 256 if ops[1] > 127 else ops[1]
        return f'play inst={ops[0]} note={s:+d} vol={c & 0xF}'
    if hi == 0xC0:
        return f'vol {c & 0xF}'
    if hi == 0xD0:
        n = note_name(ops[0]) if names and ops[0] < PSG_NOTES else str(ops[0])
        return f'note {n} vol={c & 0xF}'
    if hi == 0xE0:
        return 'off' + (f' x={c & 0xF}' if c & 0xF else '')
    if c == 0xF0:
        return f'pan 0x{ops[0]:02X}'
    if c == 0xF2:
        return f'bend {ops[0] - 0x40:+d}'
    if c == 0xF3:
        return 'loopstart'
    if c < 0xFD:
        return f'vibrato {ops[0]}' + (f' alias=0x{c:02X}' if c != 0xF1 else '')
    return {0xFD: 'end', 0xFE: 'loop', 0xFF: 'stop'}[c]


BGM_TERMINATORS = {'end': 0xFD, 'loop': 0xFE, 'stop': 0xFF}


def bgm_encode(mn, a):
    if mn == 'rest':
        return bytes([a.num('byte', 0, 0x7F) if a.pos else 0])
    if mn == 'wave':
        return bytes([0x80 | a.num('wave', 0, 15)])
    if mn == 'call':
        pos = a.num('position', 0, 0xFFFF)
        return bytes([0x90 | a.key('wave', 0, 14, 15), pos & 0xFF, pos >> 8, a.key('count', 0, 255)])
    if mn == 'play':
        inst, vol = a.key('inst', 0, 255), a.key('vol', 0, 15)
        if 'note' in a.kw:
            return bytes([0xB0 | vol, inst, a.key('note', -128, 127) & 0xFF])
        return bytes([0xA0 | vol, inst])
    if mn == 'vol':
        return bytes([0xC0 | a.num('volume', 0, 15)])
    if mn == 'note':
        n = note_value(a.take('note'), a.where)
        return bytes([0xD0 | a.key('vol', 0, 15), n])
    if mn == 'off':
        return bytes([0xE0 | a.key('x', 0, 15, 0)])
    if mn == 'pan':
        return bytes([0xF0, a.num('value', 0, 255)])
    if mn == 'bend':
        return bytes([0xF2, a.num('offset', -0x40, 0xBF) + 0x40])
    if mn == 'loopstart':
        return bytes([0xF3])
    if mn == 'vibrato':
        op = a.key('alias', 0xF1, 0xFC, 0xF1)
        if op in (0xF2, 0xF3):
            raise AsmError(f'{a.where}: alias must be 0xF1 or 0xF4-0xFC')
        return bytes([op, a.num('depth', 0, 255)])
    raise AsmError(f'{a.where}: unknown BGM command {mn!r}')


def bgm_disasm(blob, pos, end, names):
    """One track region -> lines. Bytes after the terminator that are all zero are left to the zero fill."""
    lines = []
    try:
        n, long, q = dec_delay(blob, pos)
        if q > end:
            raise IndexError
    except IndexError:
        return [f'    {_bytes_line(blob[pos:end])}']
    lines.append(f'{_ticks_text(n, long):>5}  wait')
    pos = q
    while pos < end:
        c = blob[pos]
        k = bgm_oplen(c)
        ops = blob[pos + 1:pos + 1 + k]
        if c >= 0xFD:
            lines.append(f'       {bgm_text(c, ops, names)}')
            pos += 1
            rest = blob[pos:end]
            if rest.strip(b'\0'):
                lines.append(f'    {_bytes_line(rest)}')
            return lines
        try:
            n, long, q = dec_delay(blob, pos + 1 + k)
            if q > end:
                raise IndexError
        except IndexError:
            lines.append(f'    {_bytes_line(blob[pos:end])}')
            return lines
        lines.append(f'{_ticks_text(n, long):>5}  {bgm_text(c, ops, names)}')
        pos = q
    return lines


def bgm_length(rom, addr):
    """Walk one BGM track in the ROM -> (ticks until its terminator, tick of its loopstart or None, terminator)."""
    try:
        t, _, pos = dec_delay(rom, addr - BASE)
        ls = None
        while True:
            c = rom[pos]
            if c >= 0xFD:
                return t, ls, {0xFD: 'end', 0xFE: 'loop', 0xFF: 'stop'}[c]
            if c == 0xF3:
                ls = t
            n, _, pos = dec_delay(rom, pos + 1 + bgm_oplen(c))
            t += n
    except IndexError:
        return None


FPS = 16777216 / 280896      # VBlank rate: the sequencer advances one tick per frame


def _songs(rom, tbl, count):
    return [struct.unpack_from('<I10H', rom, tbl - BASE + i * 24) for i in range(count)]


def x_song_tracks(data, p, rom):
    tbl, count = int(p['table'], 16), int(p['count'])
    lo, hi = p['start'], p['end']
    songs = _songs(rom, tbl, count)
    bases = sorted({s[0] for s in songs})
    if bases[0] != lo or any(not lo <= b < hi for b in bases) or len(bases) != count:
        raise ValueError('songs must be distinct and start at the asset start')
    files, index = {}, []
    for i, s in enumerate(songs):
        base = s[0]
        send = next((b for b in bases if b > base), hi)
        users = {}
        for t, off in enumerate(s[1:]):
            users.setdefault(off, []).append(t)
        offs = sorted(users)
        if offs[-1] >= send - base:
            raise ValueError(f'song {i}: a track lies outside the song')
        name = SONG_NAMES.get(i)
        head = [f'; Song {i} (BGM id 0x{i:02X}){": " + name if name else ""}. Konami BGM tracks;',
                '; format: tools/assetfmt/sound_seq.py. One event per line: <ticks> <command> [args], where',
                '; <ticks> = frames until the next event. Each .track starts with `<ticks> wait` and ends with',
                '; end / loop / stop. .song is the song\'s fixed address and .track its offset, both as in',
                '; song_table.json. Tracks: ' + ', '.join(f'{BGM_TRACKS[t]}=0x{o:04X}' for t, o in enumerate(s[1:])),
                f'.song 0x{base:08X}']
        body = []
        prev_end = 0
        for j, off in enumerate(offs):
            rend = offs[j + 1] if j + 1 < len(offs) else send - base
            if off != prev_end and data[base - lo + prev_end:base - lo + off].strip(b'\0'):
                raise ValueError(f'song {i}: non-zero bytes between tracks')
            ts = users[off]
            psg_tone = all(t < 3 for t in ts)
            desc = ', '.join(f'{BGM_TRACKS[t]} ({BGM_TRACK_DESC[BGM_TRACKS[t]]})' for t in ts) \
                if len(ts) == 1 else ', '.join(BGM_TRACKS[t] for t in ts)
            lines = bgm_disasm(data, base - lo + off, base - lo + rend, psg_tone)
            if [x.split() for x in lines] == [['0', 'wait'], ['end']]:
                desc += ' (unused: empty track)'
            body += ['', f'.track 0x{off:04X}    ; {desc}'] + lines
            prev_end = rend
        fn = f'song_{i:02d}.txt'
        files[f"{p['path']}/{fn}"] = ('\n'.join(head + body) + '\n').encode()
        index.append(fn)
    files[f"{p['path']}/index.txt"] = _index(index, [
        '; Song files, one per BGM id. Each is assembled at its fixed `.song` address.'])
    return files


def _bgm_assemble(text, fname):
    chunks, base, cur, addr0, state, org = [], None, None, None, None, fname
    for where, toks in _lines(text, fname):
        d = toks[0]
        if d == '.song':
            base = _num(toks[1], where)
            continue
        if d == '.track':
            if base is None:
                raise AsmError(f'{where}: .track before .song')
            if cur is not None:
                chunks.append((addr0, bytes(cur), org))
            addr0, cur, state = base + _num(toks[1], where, 0, 0xFFFF), bytearray(), 'wait'
            org = f'{where} (.track {toks[1]})'
            continue
        if cur is None:
            raise AsmError(f'{where}: events must follow a .track line')
        if d == '.byte':
            cur += bytes(_num(t, where, 0, 255) for t in toks[1:])
            continue
        if d in BGM_TERMINATORS:
            if len(toks) > 1:
                raise AsmError(f'{where}: {d} takes no arguments and no tick count')
            if state != 'events':
                raise AsmError(f'{where}: ' + ('the track has already ended' if state == 'ended'
                                               else 'a track starts with `<ticks> wait`'))
            cur.append(BGM_TERMINATORS[d])
            state = 'ended'
            continue
        n, long = _ticks(d, where)
        if len(toks) < 2:
            raise AsmError(f'{where}: missing command after the tick count')
        mn = toks[1]
        if state == 'ended':
            raise AsmError(f'{where}: event after the end of the track (start a new .track)')
        if mn == 'wait':
            if state != 'wait' or len(toks) > 2:
                raise AsmError(f'{where}: `wait` is only the first line of a track (use `rest` later)')
            cur += enc_delay(n, long)
            state = 'events'
            continue
        if state == 'wait':
            raise AsmError(f'{where}: a track starts with `<ticks> wait`')
        if mn in BGM_TERMINATORS:
            raise AsmError(f'{where}: {mn} takes no tick count')
        a = Args(toks[2:], where)
        bs = bgm_encode(mn, a)
        a.done()
        cur += bs + enc_delay(n, long)
    if cur is not None:
        chunks.append((addr0, bytes(cur), org))
    return chunks


def b_song_tracks(read, p):
    chunks = []
    for n in _read_index(read, p['path']):
        chunks += _bgm_assemble(read(f"{p['path']}/{n}").decode(), f"{p['path']}/{n}")
    return _place(p['end'] - p['start'], p['start'], chunks, p['path'])


# ------------------------------------------------------------------------------------------- song table
def x_song_table(data, p, rom):
    if len(data) % 24:
        raise ValueError('song table size is not a multiple of 24')
    tdir = p.get('tracks', 'sound/song_tracks')
    rows = []
    for i in range(len(data) // 24):
        e = struct.unpack_from('<I10H', data, i * 24)
        row = {'id': i}
        if i in SONG_NAMES:
            row['_name'] = SONG_NAMES[i]
        row.update(data=f'0x{e[0]:08X}', _file=f'{tdir}/song_{i:02d}.txt')
        ln = bgm_length(rom, e[0] + e[1]) if rom is not None else None
        if ln:
            t, ls, term = ln
            row['_length'] = (f'{t / FPS:.1f} s, then stops' if term != 'loop' or ls is None else
                              f'{ls / FPS:.1f} s intro + {(t - ls) / FPS:.1f} s loop')
        row['tracks'] = {BGM_TRACKS[t]: f'0x{o:04X}' for t, o in enumerate(e[1:])}
        rows.append(row)
    doc = {
        '_format': 'Konami driver BGM song table (sound/song_table). data: ROM address of the song '
                   '(the .song line of song_tracks/song_NN.txt); tracks: offset of each of the ten tracks '
                   'from data (its .track line). Unused channels share one empty track. A song is requested '
                   'by its index (id). Keys starting with _ are notes and are ignored.',
        '_tracks': {k: BGM_TRACK_DESC[k] for k in BGM_TRACKS},
        'songs': rows,
    }
    return {p['path']: _dumps(doc)}


def b_song_table(read, p):
    doc = json.loads(read(p['path']))
    out = bytearray()
    for i, s in enumerate(doc['songs']):
        where = f"{p['path']}: song {i}"
        if s.get('id', i) != i:
            raise AsmError(f'{where}: ids must run 0, 1, 2, ... (got {s.get("id")})')
        tr = s['tracks']
        if set(tr) != set(BGM_TRACKS):
            raise AsmError(f'{where}: tracks needs exactly the keys {BGM_TRACKS}')
        out += struct.pack('<I10H', _int(s['data'], where, 0, 0xFFFFFFFF),
                           *(_int(tr[k], where, 0, 0xFFFF) for k in BGM_TRACKS))
    return bytes(out)


# ---------------------------------------------------------------------------------------- lookup tables
# (name, offset, element size, rows, items per row, row label, value style, meaning, formula, used by)
def _pitch_label(r):
    return f'{r - 48:+d}'


def _psg_label(r):
    return note_name(r)


LOOKUP = [
    ('volume_nibble_scale', 0x0000, 1, 16 * 16, 16,
     lambda r: f'vol {r // 16:2d} hi {r % 16:X}', 'hex',
     '16 tables of 256 bytes. Table k maps a byte of two 4-bit samples to the same byte with both nibbles '
     'scaled by k/15 (rounded down). Rows here: "vol k hi h" lists the results for the 16 bytes 0xh0..0xhF '
     'of table k. Hypothesis: a volume table for 4-bit wave-RAM data; no code reads it (the wave patterns '
     'in sound/wave_ram_patterns are stored pre-scaled).',
     'table[k][x] = (((x >> 4) * k // 15) << 4) | ((x & 15) * k // 15)', 'none found'),
    ('pcm_pitch', 0x1000, 2, 96, 32, _pitch_label, 'dec',
     'PCM pitch multipliers, 4.12 fixed point (4096 = the sample\'s own rate), one per 1/32 semitone from '
     '-48 to +47.97 semitones. Rows are labelled by semitone offset. The driver indexes it from its middle '
     '(gSoundPitchTable = row "+0", value 4096) with the signed pitch of a PCM voice (note << 5 in BGM tracks): '
     'step = pitch[p] * sample.rate >> 12.',
     'value[i] = round(4096 * 2 ** ((i - 1536) / 384)), i = 0..3071', 'SoundPcmStart (SoundPcmStart), '
     'SoundSequencerTick (pitch changes)'),
    ('psg_frequency', 0x2800, 2, 84, 32, _psg_label, 'hex',
     'PSG square/wave frequency register values (SOUNDxCNT_X), one per 1/32 semitone from C2 (65.41 Hz on '
     'the square channels) to B8; bit 15 is the restart (trigger) bit. Rows are labelled by note; the BGM '
     '`note` command selects row n (C2 + n semitones). The wave channel sounds an octave lower for the same '
     'value. Indexed by gPsgFreqTable[pitch].',
     'value[i] = 0x8000 | int(2048 - 131072 / (440 * 2 ** ((i / 32 - 33) / 12))), i = 0..2687',
     'SoundSequencerTick (PSG channels 1-3)'),
    ('psg_vibrato_steps', 0x3D00, 2, 84, 8, _psg_label, 'hex',
     'For each semitone of psg_frequency (rows by note): its value, the value lowered by 1, 2 and 3 steps, '
     'the value again, and raised by 1, 2 and 3 steps. A step is 1/32 semitone at low notes and one register '
     'unit at high notes (where 1/32 semitone is less than a unit); the top rows saturate. Hypothesis: a '
     'vibrato/detune table; no code reads it (BGM vibrato uses the sine table at 0x081ABC4C), but '
     'psg_frequency[pitch] runs into it for pitches past B8.',
     'approximately P(s*32) +/- max(k, |P(s*32 +/- k) - P(s*32)|) for k = 1..3, P = psg_frequency formula '
     '(matches 622 of 672 values; the rows hold the exact data)', 'none found'),
]


def x_lookup(data, p, rom):
    if len(data) != 0x4240:
        raise ValueError('unexpected lookup table size')
    out = ['{', '  "_format": ' + json.dumps(
        'Konami sound driver lookup tables (sound/lookup_tables). Each table: address, element (u8/u16), '
        'meaning, formula (how the values were generated, verified against the ROM) and rows (label -> '
        'values in order; hex strings or decimal numbers). Only address/element/rows are read back; edit '
        'the values in rows.') + ',', '  "tables": [']
    tabs = []
    for name, off, esz, nrows, per, lab, style, meaning, formula, used in LOOKUP:
        t = [f'    {{', f'      "name": {json.dumps(name)},', f'      "address": "0x{p["start"] + off:08X}",',
             f'      "element": "{"u8" if esz == 1 else "u16"}",', f'      "meaning": {json.dumps(meaning)},',
             f'      "formula": {json.dumps(formula)},', f'      "used_by": {json.dumps(used)},',
             '      "rows": {']
        rows = []
        for r in range(nrows):
            a = off + r * per * esz
            vals = list(data[a:a + per]) if esz == 1 else list(struct.unpack_from(f'<{per}H', data, a))
            if style == 'hex':
                v = json.dumps(' '.join(f'{x:0{2 * esz}X}' for x in vals))
            else:
                v = json.dumps(vals)
            rows.append(f'        {json.dumps(lab(r))}: {v}')
        t.append(',\n'.join(rows))
        t.append('      }')
        t.append('    }')
        tabs.append('\n'.join(t))
    out.append(',\n'.join(tabs))
    out += ['  ]', '}']
    return {p['path']: ('\n'.join(out) + '\n').encode()}


def b_lookup(read, p):
    doc = json.loads(read(p['path']))
    out = bytearray()
    for t in doc['tables']:
        where = f"{p['path']}: {t.get('name', '?')}"
        addr = _int(t['address'], where, 0, 0xFFFFFFFF)
        if addr != p['start'] + len(out):
            raise AsmError(f'{where}: address 0x{addr:08X}, but the previous tables end at '
                           f'0x{p["start"] + len(out):08X} (tables are contiguous and fixed in size)')
        esz = {'u8': 1, 'u16': 2}[t['element']]
        for label, row in t['rows'].items():
            items = row if isinstance(row, list) else [row]
            for it in items:
                vals = [int(x, 16) for x in it.split()] if isinstance(it, str) else [_int(it, where, 0, 0xFFFF)]
                for v in vals:
                    if not 0 <= v < 1 << 8 * esz:
                        raise AsmError(f'{where}: row {label}: {v} does not fit in {t["element"]}')
                    out += v.to_bytes(esz, 'little')
    return bytes(out)


def register(A):
    return {
        'sound_seq_se_table': (x_se_table, b_se_table),
        'sound_seq_se_tracks': (x_se_tracks, b_se_tracks),
        'sound_seq_song_table': (x_song_table, b_song_table),
        'sound_seq_song_tracks': (x_song_tracks, b_song_tracks),
        'sound_seq_lookup': (x_lookup, b_lookup),
    }
