"""Sound samples of the Konami driver (wiki/game/sound-engine.md, src/sound_driver.c): PCM sample banks,
their pointer tables, the PSG channel-3 wave-RAM patterns and the PSG channel-4 (noise) table.

  sound_samples_table    PCM pointer table -> JSON          (path: <name>.json; bank=0|1 samples=ASSET_PATH)
  sound_samples_pcm      PCM sample bank -> <path>/NN.wav + <path>/index.json   (table=ADDR entries=N bank=0|1)
  sound_samples_waveram  16-byte wave-RAM images -> JSON, one 32-nibble string per pattern
  sound_samples_noise    u16 SOUND4CNT_H values -> JSON with the register fields

A sample is {s32 rate; u32 length; s32 loopStart (-1 = no loop); s8 data[length]}, 16-byte aligned. The
mixer runs at 2^24/839 Hz (Timer0 reload 0xFCB9) and advances a voice by pitch[note] * rate >> 12 in 20.12
fixed point; pitch[0] = 0x1000, so a sample plays at rate * 4096 / 839 Hz at the table's first pitch. The WAV
files are written at that rate (rate 2048 -> 9998 Hz), as 8-bit PCM: WAV stores 8-bit samples unsigned, so a
WAV byte is the ROM's signed byte + 128 (XOR 0x80).
"""
import json
import struct

BASE = 0x08000000
MIX_CYCLES = 839            # Timer0 period in CPU cycles (TM0CNT reload 0xFCB9)
PCM_ALIGN = 16


def _hz(rate):
    """Header rate -> WAV sample rate (playback rate at pitch 0x1000)."""
    return (rate * 4096 + MIX_CYCLES // 2) // MIX_CYCLES


def _rate(hz):
    """WAV sample rate -> header rate; the inverse of _hz for every rate (one rate unit is ~4.9 Hz)."""
    return (hz * MIX_CYCLES + 2048) // 4096


def _ptr_str(v):
    return None if v == 0 else f'0x{v:08X}'


def _ptr_val(s):
    if s is None:
        return 0
    return int(s, 16) if isinstance(s, str) else int(s)


def _dumps(doc):
    """{key: value or list}: lists of records get one record per line, which reads like a table."""
    out = []
    for k, v in doc.items():
        if isinstance(v, list):
            rows = [' ' * 2 + json.dumps(x, ensure_ascii=False) for x in v]
            out.append(f' {json.dumps(k)}: [\n' + ',\n'.join(rows) + '\n ]')
        else:
            out.append(f' {json.dumps(k)}: {json.dumps(v, ensure_ascii=False)}')
    return ('{\n' + ',\n'.join(out) + '\n}\n').encode()


def _sample_id(bank, index):
    return f'0x{(0x8000 if bank else 0) | index:04X}'


def _first_index(ptrs):
    """Distinct non-NULL pointer -> the first table index that uses it (that index names the WAV file)."""
    first = {}
    for i, v in enumerate(ptrs):
        if v and v not in first:
            first[v] = i
    return first


# ----------------------------------------------------------------------------------------------- WAV
def wav_write(hz, s8_data):
    pcm = bytes(b ^ 0x80 for b in s8_data)
    fmt = struct.pack('<HHIIHH', 1, 1, hz, hz, 1, 8)
    data = pcm + (b'\0' if len(pcm) & 1 else b'')
    body = b'WAVE' + b'fmt ' + struct.pack('<I', len(fmt)) + fmt + b'data' + struct.pack('<I', len(pcm)) + data
    return b'RIFF' + struct.pack('<I', len(body)) + body


def wav_read(blob, name):
    """Mono 8-bit or 16-bit PCM WAV -> (sample rate, signed 8-bit bytes). 16-bit input is rounded to 8 bits."""
    if blob[:4] != b'RIFF' or blob[8:12] != b'WAVE':
        raise ValueError(f'{name}: not a RIFF/WAVE file')
    pos, fmt, data = 12, None, None
    while pos + 8 <= len(blob):
        cid, n = blob[pos:pos + 4], struct.unpack_from('<I', blob, pos + 4)[0]
        body = blob[pos + 8:pos + 8 + n]
        if cid == b'fmt ':
            fmt = body
        elif cid == b'data':
            data = body
        pos += 8 + n + (n & 1)
    if fmt is None or data is None:
        raise ValueError(f'{name}: missing fmt or data chunk')
    tag, ch, hz, _, _, bits = struct.unpack_from('<HHIIHH', fmt)
    if tag == 0xFFFE and len(fmt) >= 26:          # WAVE_FORMAT_EXTENSIBLE: the subformat GUID starts at +24
        tag = struct.unpack_from('<H', fmt, 24)[0]
    if tag != 1 or ch != 1 or bits not in (8, 16):
        raise ValueError(f'{name}: must be mono 8-bit or 16-bit integer PCM '
                         f'(found format {tag}, {ch} channel(s), {bits} bits)')
    if bits == 8:
        return hz, bytes(b ^ 0x80 for b in data)
    vals = struct.unpack(f'<{len(data) // 2}h', data[:len(data) // 2 * 2])
    return hz, bytes((min(127, (v + 128) >> 8)) & 0xFF for v in vals)


# --------------------------------------------------------------------------------------- PCM tables
def x_table(data, p, rom):
    """Pointer table -> JSON. "ptr" is the only field the build reads; the others are notes."""
    bank, samples = int(p.get('bank', 0)), p.get('samples', '')
    ptrs = list(struct.unpack(f'<{len(data) // 4}I', data))
    first = _first_index(ptrs)
    entries = []
    for i, v in enumerate(ptrs):
        e = {'index': i, 'sample_id': _sample_id(bank, i), 'ptr': _ptr_str(v)}
        if v == 0:
            e['points_to'] = 'NULL: no sample'
        elif rom is not None and samples:
            hdr = rom[v - BASE:v - BASE + 12] if BASE <= v < BASE + len(rom) - 12 else b''
            e['points_to'] = f'{samples}/{first[v]:02d}.wav'
            if len(hdr) == 12:
                rate, length, loop = struct.unpack('<iIi', hdr)
                e['points_to'] += f' ({length} samples, ' + ('one-shot)' if loop < 0 else f'loops from {loop})')
        entries.append(e)
    doc = {
        'format': 'Konami PCM sample table, bank %d: sample id %s+n -> pointer to a sample header '
                  '{s32 rate; u32 length; s32 loopStart; s8 data[]}. Only "ptr" is read when building '
                  '(null = NULL); the ROM layout is fixed, so a pointer must name the "address" of a sample '
                  'in %s/index.json.' % (bank, '0x8000' if bank else '0x0000', samples or 'the sample bank'),
        'entries': entries,
    }
    return {p['path']: _dumps(doc)}


def b_table(read, p):
    doc = json.loads(read(p['path']))
    vals = [_ptr_val(e['ptr']) for e in doc['entries']]
    return struct.pack(f'<{len(vals)}I', *vals)


# ------------------------------------------------------------------------------------------ PCM banks
def x_pcm(data, p, rom):
    """Sample bank -> one WAV per sample plus <path>/index.json. The samples are found through the pointer
    table at table=ADDR (entries=N); each keeps its fixed address so the table stays valid."""
    start, end = p['start'], p['end']
    bank = int(p.get('bank', 0))
    tab = int(p['table'], 16) - BASE
    ptrs = list(struct.unpack_from(f'<{int(p["entries"])}I', rom, tab))
    first = _first_index(ptrs)
    addrs = sorted(v for v in first if start <= v < end)
    if not addrs:
        raise ValueError('the table points to no sample in this range')
    files, samples = {}, []
    for k, a in enumerate(addrs):
        off = a - start
        nxt = (addrs[k + 1] if k + 1 < len(addrs) else end) - start
        rate, length, loop = struct.unpack_from('<iIi', data, off)
        if off + 12 + length > nxt:
            raise ValueError(f'sample at 0x{a:08X} overlaps the next one')
        if rate <= 0 or _rate(_hz(rate)) != rate:
            raise ValueError(f'sample at 0x{a:08X}: rate {rate} has no WAV equivalent')
        if loop != -1 and not 0 <= loop < length:
            raise ValueError(f'sample at 0x{a:08X}: loop start {loop} outside 0..{length - 1}')
        name = f'{first[a]:02d}.wav'
        files[f"{p['path']}/{name}"] = wav_write(_hz(rate), data[off + 12:off + 12 + length])
        e = {'file': name, 'address': f'0x{a:08X}',
             'sample_ids': [_sample_id(bank, i) for i, v in enumerate(ptrs) if v == a],
             'rate': rate, 'loop_start': None if loop == -1 else loop}
        pad = data[off + 12 + length:nxt]
        if pad.strip(b'\0'):
            e['pad_hex'] = pad.hex()
        samples.append(e)
    doc = {
        'format': 'Konami PCM sample bank. Each WAV is one sample (mono 8-bit, length = the header length). '
                  '"rate" is the header pitch factor: the mixer plays rate * 4096 / 839 samples per second at '
                  'pitch 0x1000 (2048 = 9998 Hz), and the WAV is written at that rate; set "rate" to null to '
                  'take it from the WAV instead. "loop_start" is the sample index playback jumps back to at the '
                  'end (null = one-shot). "address" is fixed: the sample (12-byte header + data) must fit before '
                  'the next one. "sample_ids" are notes (see the table).',
        'samples': samples,
    }
    if addrs[0] > start:
        doc['lead_hex'] = data[:addrs[0] - start].hex()
    files[f"{p['path']}/index.json"] = _dumps(doc)
    return files


def b_pcm(read, p):
    start, end = p['start'], p['end']
    doc = json.loads(read(f"{p['path']}/index.json"))
    out = bytearray(end - start)
    lead = bytes.fromhex(doc.get('lead_hex', ''))
    out[:len(lead)] = lead
    samples = sorted(doc['samples'], key=lambda e: _ptr_val(e['address']))
    pos = start + len(lead)       # first free address
    for k, e in enumerate(samples):
        a = _ptr_val(e['address'])
        limit = (_ptr_val(samples[k + 1]['address']) if k + 1 < len(samples) else end)
        name = f"{p['path']}/{e['file']}"
        if a < pos or a % PCM_ALIGN or a >= end:
            raise ValueError(f'{name}: address 0x{a:08X} overlaps the previous sample, lies outside '
                             f'0x{start:08X}-0x{end:08X} or is not 16-byte aligned')
        hz, pcm = wav_read(read(name), name)
        rate = e.get('rate')
        if rate is None:
            rate = _rate(hz)
        elif hz != _hz(rate):
            raise ValueError(f'{name} is {hz} Hz, but rate {rate} plays at {_hz(rate)} Hz: resample the WAV, '
                             f'set "rate" to {_rate(hz)}, or set it to null to take the rate from the WAV')
        loop = e.get('loop_start')
        if loop is None:
            loop = -1
        elif not 0 <= loop < len(pcm):
            raise ValueError(f'{name}: loop_start {loop} must be below the length {len(pcm)}')
        if a + 12 + len(pcm) > limit:
            raise ValueError(f'{name}: {len(pcm)} samples need 0x{12 + len(pcm):X} bytes, but only '
                             f'0x{limit - a:X} fit at 0x{a:08X}; shorten it, or move the following samples '
                             f'(their "address" here and the pointers in the table)')
        chunk = struct.pack('<iIi', rate, len(pcm), loop) + pcm + bytes.fromhex(e.get('pad_hex', ''))
        chunk = chunk[:limit - a]
        out[a - start:a - start + len(chunk)] = chunk
        pos = a + 12 + len(pcm)
    return bytes(out)


# ------------------------------------------------------------------------------------------ wave RAM
def _nibbles(b):
    h = b.hex().upper()
    return ' '.join(h[i:i + 8] for i in range(0, len(h), 8))


def x_waveram(data, p, rom):
    """Wave-RAM images for PSG channel 3: 32 4-bit samples each, as a hex string in play order."""
    if len(data) % 16:
        raise ValueError('not a whole number of 16-byte patterns')
    pats = [{'wave': i // 16, 'level': i % 16, 'samples': _nibbles(data[i * 16:i * 16 + 16])}
            for i in range(len(data) // 16)]
    doc = {
        'format': 'PSG channel 3 wave-RAM patterns. SoundLoadWaveRam (SoundLoadWaveRam) loads pattern wave*16 + '
                  'level, where wave is the channel-3 track\'s instrument number and level its volume 1-15 '
                  '(volume 0 silences the channel instead, so level-0 patterns are never loaded). "samples" '
                  'are the 32 4-bit samples in play order, one hex digit each (0 = lowest, F = highest), '
                  'grouped as the four words written to WAVE_RAM0-3; spaces are ignored. The build places '
                  'each pattern by its wave and level, and every slot must be present once.',
        'patterns': pats,
    }
    return {p['path']: _dumps(doc)}


def b_waveram(read, p):
    n = (p['end'] - p['start']) // 16
    slots = [None] * n
    for e in json.loads(read(p['path']))['patterns']:
        i = int(e['wave']) * 16 + int(e['level'])
        h = ''.join(e['samples'].split())
        if not 0 <= int(e['level']) < 16 or not 0 <= i < n:
            raise ValueError(f'wave {e["wave"]} level {e["level"]}: no such slot ({n} patterns)')
        if len(h) != 32:
            raise ValueError(f'wave {e["wave"]} level {e["level"]}: need 32 hex digits, got {e["samples"]!r}')
        if slots[i] is not None:
            raise ValueError(f'wave {e["wave"]} level {e["level"]} is listed twice')
        slots[i] = bytes.fromhex(h)
    if None in slots:
        i = slots.index(None)
        raise ValueError(f'wave {i // 16} level {i % 16} is missing')
    return b''.join(slots)


# ------------------------------------------------------------------------------------ noise (PSG 4)
NOISE_FIELDS = [('divide_ratio', 0, 3), ('counter_7bit', 3, 1), ('shift_clock', 4, 4), ('stop_at_length', 14, 1),
                ('restart', 15, 1)]


def x_noise(data, p, rom):
    vals = struct.unpack(f'<{len(data) // 2}H', data)
    entries = []
    for i, v in enumerate(vals):
        e = {'index': i}
        for f, s, n in NOISE_FIELDS:
            x = (v >> s) & ((1 << n) - 1)
            e[f] = bool(x) if n == 1 else x
        if v & 0x3F00:
            e['unused_bits'] = f'0x{v & 0x3F00:04X}'
        entries.append(e)
    doc = {
        'format': 'PSG channel 4 (noise) presets. The sequencer (SoundSequencerTick) writes entry n to SOUND4CNT_H '
                  '(0x0400007C) when the noise channel\'s pitch value is n, unless bit 1 of the channel\'s dirty '
                  'or command byte is set (then it writes the pitch value itself). Fields: divide_ratio r '
                  '(0-7) and shift_clock s (0-15) give the LFSR clock 524288 / r / 2^(s+1) Hz (r = 0 counts '
                  'as 0.5); counter_7bit '
                  'selects the 7-bit LFSR (metallic) instead of 15-bit; stop_at_length enables the length '
                  'counter; restart (bit 15) retriggers the channel.',
        'entries': entries,
    }
    return {p['path']: _dumps(doc)}


def b_noise(read, p):
    out = []
    for e in json.loads(read(p['path']))['entries']:
        v = int(e.get('unused_bits', '0x0'), 16)
        for f, s, n in NOISE_FIELDS:
            x = int(e[f])
            if not 0 <= x < 1 << n:
                raise ValueError(f'noise entry {e.get("index")}: {f}={x} does not fit in {n} bits')
            v |= x << s
        out.append(v)
    return struct.pack(f'<{len(out)}H', *out)


def register(A):
    return {
        'sound_samples_table': (x_table, b_table),
        'sound_samples_pcm': (x_pcm, b_pcm),
        'sound_samples_waveram': (x_waveram, b_waveram),
        'sound_samples_noise': (x_noise, b_noise),
    }
