#!/usr/bin/env python3
"""Generate include/constants/cards.h: enum CardNumber with one CARD_<NAME> per card number.

  python3 tools/gencards.py                       # writes include/constants/cards.h
  python3 tools/gencards.py --out build/x.h       # somewhere else (preview)
  python3 tools/gencards.py --check               # exit 1 if the header is out of date

Needs the extracted assets (`make setup`): assets/cards/names.json (card names by card ID),
assets/cards/id_to_number.json and number_to_id.json (gCardIdToNumber / gCardNumberToId), and
assets/tables/card_effect_handlers.json (effect keys). build/names/types.json (enum CardNumber, the CARDNO_*
names the naming pass used) is optional: it adds comments, the extra numeric constants it lists, and the alias
map build/readability/cards_aliases.json (CARDNO_X -> CARD_Y) for the units that migrate.

Naming: the card name in upper snake case ("Alligator's Sword" -> CARD_ALLIGATORS_SWORD, "Mystical Sheep #1" ->
CARD_MYSTICAL_SHEEP_1, "Master & Expert" -> CARD_MASTER_AND_EXPERT). A name used by several card numbers (the
alternate-art copies at 2000 + n, the second Polymerization) gets _ALT, _ALT2... on the higher numbers. Effect
keys and other numbers the code uses that have no EDS card keep a numeric CARD_<number>. Card IDs (the
alphabetical index of gCardStats/gCardNames) are a different number space and are not in this enum.
"""
import argparse
import collections
import json
import os
import re
import sys

OUT = 'include/constants/cards.h'
ALIASES = 'build/readability/cards_aliases.json'


def ident(name):
    s = name.upper().replace('&', ' AND ').replace("'", '').replace('.', ' ').replace('#', ' ')
    s = re.sub(r'[^A-Z0-9]+', '_', s).strip('_')
    return 'CARD_' + s


def load(path):
    with open(path) as f:
        return json.load(f)


def build():
    names = load('assets/cards/names.json')
    i2n = load('assets/cards/id_to_number.json')
    n2i = load('assets/cards/number_to_id.json')
    effects = load('assets/tables/card_effect_handlers.json')
    tj = load('build/names/types.json') if os.path.exists('build/names/types.json') else None

    # number -> (id, name) for every card ID 1..len(names)-1
    by_number = {}
    for cid in range(1, len(names)):
        num = i2n[cid]
        if num == 0xFFFF:
            continue
        if num in by_number:
            sys.exit(f'card number {num} has two IDs ({by_number[num][0]}, {cid})')
        by_number[num] = (cid, names[cid])

    # identifiers; duplicate names: the lowest number keeps the plain name
    groups = collections.defaultdict(list)
    for num, (cid, nm) in by_number.items():
        groups[ident(nm)].append(num)
    entries = {}
    for base, nums in groups.items():
        for k, num in enumerate(sorted(nums)):
            cid, nm = by_number[num]
            sym = base if k == 0 else base + ('_ALT' if k == 1 else f'_ALT{k}')
            note = f'id {cid}'
            if k:
                note += f', {"alternate art of" if num >= 2000 else "second number for"} {base}'
            if num < len(n2i) and n2i[num] != cid:
                note += f' (gCardNumberToId[{num}] = {n2i[num]})'
            entries[num] = (sym, note)

    # numbers without an EDS card that the code uses: effect keys and the types.json CardNumber values
    tj_vals = {}
    if tj:
        for e in tj['enums']:
            if e['name'] == 'CardNumber':
                for v in e['values']:
                    tj_vals[int(str(v['value']), 0)] = v
    handlers = collections.defaultdict(list)
    for e in effects:
        for slot in ('prepare', 'check', 'chain_a', 'chain_b', 'resolve'):
            if e.get(slot):
                handlers[e['number']].append(e[slot])
    for num in sorted(set(handlers) | set(tj_vals)):
        if num in entries:
            continue
        v = tj_vals.get(num)
        note = 'no EDS card'
        if num in handlers:
            note += ': effect key'
        if v and v.get('comment'):
            note += f" ({v['comment'][:100]})"
        entries[num] = (f'CARD_{num}', note)

    # collisions with other enum values of types.json
    others = set()
    if tj:
        for e in tj['enums']:
            if e['name'] != 'CardNumber':
                others |= {v['name'] for v in e['values']}
    syms = collections.Counter(s for s, _ in entries.values())
    bad = [s for s, k in syms.items() if k > 1] + [s for s, _ in entries.values() if s in others]
    if bad:
        sys.exit(f'identifier collisions: {sorted(set(bad))}')

    # CARDNO_* -> CARD_* aliases
    aliases, renamed = {}, []
    for num, v in sorted(tj_vals.items()):
        new = entries[num][0]
        aliases[v['name']] = new
        if v['name'].replace('CARDNO_', 'CARD_') != new:
            renamed.append((v['name'], new))
    return entries, aliases, renamed


def render(entries):
    w = max(len(s) for s, _ in entries.values()) + len(' = 2047,')
    lines = [
        '#ifndef GUARD_CONSTANTS_CARDS_H',
        '#define GUARD_CONSTANTS_CARDS_H',
        '',
        '/*',
        ' * GENERATED by tools/gencards.py from the extracted card tables (assets/cards/names.json,',
        ' * id_to_number.json, number_to_id.json) and the effect table (assets/tables/card_effect_handlers.json).',
        ' * Do not edit by hand; rerun the tool.',
        ' *',
        ' * enum CardNumber is the card NUMBER: the index of gCardNumberToId, the effect table key and the value',
        ' * the duel code compares. Card IDs (the alphabetical index of gCardStats and gCardNames) are a different',
        ' * number space (gCardIdToNumber maps ID -> number). Numbers with no EDS card keep a numeric name.',
        ' */',
        '',
        'enum CardNumber {',
    ]
    for num in sorted(entries):
        sym, note = entries[num]
        lines.append(f'    {sym + " = " + str(num) + ",":<{w}} /* {note} */')
    lines += ['};', '', '#endif /* GUARD_CONSTANTS_CARDS_H */', '']
    return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('--out', default=OUT)
    ap.add_argument('--aliases', default=ALIASES)
    ap.add_argument('--check', action='store_true')
    a = ap.parse_args()
    entries, aliases, renamed = build()
    text = render(entries)
    if a.check:
        cur = open(a.out).read() if os.path.exists(a.out) else ''
        if cur != text:
            sys.exit(f'{a.out} is out of date: run python3 tools/gencards.py')
        print(f'{a.out}: up to date')
        return
    os.makedirs(os.path.dirname(a.out) or '.', exist_ok=True)
    open(a.out, 'w').write(text)
    if aliases:
        os.makedirs(os.path.dirname(a.aliases), exist_ok=True)
        json.dump({'note': 'types.json CardNumber name -> generated constant (tools/gencards.py)',
                   'aliases': aliases, 'renamed': renamed}, open(a.aliases, 'w'), indent=1)
    n_cards = sum(1 for s, n in entries.values() if not n.startswith('no EDS'))
    print(f'wrote {a.out}: {len(entries)} constants ({n_cards} cards, {len(entries) - n_cards} numeric)'
          + (f'; {a.aliases}: {len(aliases)} CARDNO_ aliases, {len(renamed)} renamed' if aliases else ''))


if __name__ == '__main__':
    main()
