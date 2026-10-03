#!/usr/bin/env python3
"""Inventory assembly fallbacks and group them into reproducible unit batches.

This uses the same source-status rule as progress.py. A parked C body is only
a draft; its presence says nothing about compilation, behavior, or matching.
Run in Docker: python3 tools/decomp_queue.py --out build/decomp-queue
"""
import argparse
import collections
import json
from pathlib import Path
import re
from match_drafts import BLOCK


def source_state(unit):
    path = Path('src') / (unit + '.c')
    if path.exists():
        source = path.read_text()
        included = set(re.findall(r'INCLUDE_ASM\([^,]+,\s*(\w+)\)', source))
        notes = {}
        for match in BLOCK.finditer(source):
            note = match['header'].lstrip()
            if note.startswith('/*'):
                note = (note + '\n' + match['body']).split('*/',1)[0][2:]
            notes[match['name']] = ' '.join(note.split())
        return included, notes
    if (Path('src') / (unit + '.s')).exists():
        return set(), {}
    return None, {}


def wiki_path(unit):
    name = 'sound-mixer' if unit == 'sound_mixer_arm' else unit.replace('_','-').lower()
    path = Path('wiki/functions') / (name+'.md')
    return str(path) if path.exists() else None


PATTERNS = {
    'register-allocation':r'regist|regalloc|allocation|\br\d+\b|swap',
    'hoisting-and-reloads':r'hoist|CSE|reload|rematerial|invariant',
    'branches-and-shared-tails':r'branch|cross.jump|goto|layout|join|tail',
    'access-width-and-narrowing':r'narrow|signed|bitfield|width|ldsh|ldrh|ldrb',
    'stack-and-spills':r'spill|stack|frame',
    'dma-sequences':r'\bDMA\b|busy.wait',
}


def owner(address, unit):
    if unit in ['sound_driver', 'sound_mixer_arm']:
        return 'sound'
    if 0x08027580 <= address < 0x08041F9C:
        return 'middle'
    if 0x080431E4 <= address < 0x0807D3D0 and unit != 'effect_target_collect':
        return 'later'
    return 'coordinator'


def inventory():
    states, records = {}, []
    total = done = 0
    names = set()
    for line in Path('config/functions.tsv').read_text().splitlines():
        if not line or line.startswith('#'):
            continue
        fields = line.split('\t')
        address, mode, size, name, unit = fields[:5]
        assert name not in names, 'Duplicate function: ' + name
        names.add(name)
        total += 1
        if unit not in states:
            states[unit] = source_state(unit)
        included, notes = states[unit]
        if included is not None and name not in included:
            done += 1
            continue
        asm_path = Path('asm/nonmatching') / unit / (name + '.s')
        asm = asm_path.read_text() if asm_path.exists() else ''
        calls = sorted(set(re.findall(r'\bbl\s+(sub_[A-Za-z0-9_]+|[A-Za-z][A-Za-z0-9_]+)', asm)))
        records.append({
            'address':address, 'mode':mode, 'size':int(size,16), 'name':name,
            'unit':unit, 'owner':owner(int(address,16),unit),
            'draft':'parked-c' if name in notes else 'no-parked-c',
            'draft_note':notes.get(name,''),
            'patterns':[key for key,pattern in PATTERNS.items() if re.search(pattern,notes.get(name,''),re.I)],
            'm2c_available':(Path('build/m2c')/unit/(name+'.c')).exists(),
            'callees':calls,
        })
    assert done + len(records) == total
    return total, done, records


def batches(records):
    units = collections.defaultdict(list)
    for record in records:
        units[record['unit']].append(record)
    result = []
    # Few remaining functions per unit first makes settled complete-unit checks
    # cheap. Large routines keep their own batch instead of stalling small ones.
    for unit, functions in sorted(units.items(), key=lambda item:(len(item[1]), sum(f['size'] for f in item[1]))):
        functions.sort(key=lambda f:(-max((s['similarity'] for s in f.get('matched_siblings',[])),default=0),
                                     f['draft'] != 'parked-c', f['size'], f['address']))
        for start in range(0,len(functions),8):
            part = functions[start:start+8]
            calls = collections.Counter(c for f in part for c in f['callees'])
            result.append({
                'id':unit+'-'+str(start//8+1), 'unit':unit, 'owner':part[0]['owner'],
                'functions':[f['name'] for f in part], 'bytes':sum(f['size'] for f in part),
                'parked':sum(f['draft']=='parked-c' for f in part),
                'shared_callees':[c for c,n in calls.most_common(6) if n>1],
                'patterns':sorted(set(p for f in part for p in f['patterns'])),
                'best_sibling_similarity':max((s['similarity'] for f in part for s in f.get('matched_siblings',[])),default=0),
            })
    for batch in result:
        pages = [wiki_path(batch['unit'])]
        for record in units[batch['unit']]:
            for sibling in record.get('matched_siblings',[]):
                page = sibling.get('wiki')
                if page and page not in pages and len(pages)<4:
                    pages.append(page)
        callset = set(batch['shared_callees'])
        related = sorted((b for b in result if b['unit']!=batch['unit']),
                         key=lambda b:len(callset.intersection(b['shared_callees'])), reverse=True)
        for other in related:
            if len(pages)>=4 or not callset.intersection(other['shared_callees']):
                break
            page = wiki_path(other['unit'])
            if page and page not in pages:
                pages.append(page)
        batch['wiki_pages'] = [p for p in pages if p]
        batch['wiki_pages'] += ['wiki/concepts/matching-tricks.md',
                                'wiki/concepts/decomp-workflow.md',
                                'wiki/tools/decomp-permuter.md']
    result.sort(key=lambda b:(b['best_sibling_similarity']<0.8, -b['best_sibling_similarity'],
                              len(b['functions']),b['bytes']))
    assert collections.Counter(f for b in result for f in b['functions']) == collections.Counter(f['name'] for f in records)
    return result


def add_siblings(records):
    path = Path('build/similar.tsv')
    if not path.exists():
        return
    units = {}
    for line in Path('config/functions.tsv').read_text().splitlines():
        if line and not line.startswith('#'):
            f=line.split('\t')
            units[f[3]]=f[4]
    targets = {f['name']:f for f in records}
    for line in path.read_text().splitlines():
        fields = line.split('\t')
        if fields[0] not in targets:
            continue
        siblings=[]
        for token in fields[2:]:
            name, score = token.rsplit(':',1)
            unit=units.get(name)
            if not unit:
                continue
            included,_=source_state(unit)
            if included is None or name in included:
                continue
            siblings.append({'name':name,'similarity':float(score),
                             'source':'src/'+unit+'.c','wiki':wiki_path(unit)})
        targets[fields[0]]['matched_siblings']=siblings


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--out', type=Path, default=Path('build/decomp-queue'))
    args = parser.parse_args()
    total, done, records = inventory()
    add_siblings(records)
    grouped = batches(records)
    args.out.mkdir(parents=True,exist_ok=True)
    data = {'total':total, 'matching_source':done, 'remaining':len(records),
            'functions':records, 'batches':grouped}
    (args.out/'queue.json').write_text(json.dumps(data,indent=2)+'\n')
    parked = sum(f['draft']=='parked-c' for f in records)
    lines = ['# Decomp batch queue', '',
             f'Snapshot: {done}/{total} matching source; {len(records)} fallbacks, '
             f'{parked} with parked C and {len(records)-parked} without parked C.', '',
             'Draft presence is not a compilation, behavioral, or matching claim. '
             'Regenerate while source writers are held for a settled snapshot.', '',
             'Each batch owns one source unit. High-similarity matched siblings are prioritized; '
             'scores ignore registers/constants and are only a source-reuse heuristic. Related callees guide reuse. '
             'Check isolated candidates during refinement; accept only complete-unit '
             'ROM matches, then run combined ROM verification at settled checkpoints.', '',
             'Read wiki/concepts/matching-tricks.md, the listed unit/sibling pages, '
             'and prior failed attempts before each batch. '
             'Discrepancy categories come from old draft notes and may be stale; '
             'verify them against the current object before choosing a recorded trick.', '',
             '| Batch | Owner | Functions | Parked C | Bytes | Shared callees | Unit wiki |',
             '|---|---|---:|---:|---:|---|---|']
    for b in grouped:
        lines.append(f"| {b['id']} | {b['owner']} | {len(b['functions'])} | {b['parked']} | "
                     f"0x{b['bytes']:X} | {', '.join(b['shared_callees'])} | {wiki_path(b['unit']) or ''} |")
    (args.out/'queue.md').write_text('\n'.join(lines)+'\n')
    print(f'{done}/{total} matching; {len(records)} remaining: {parked} parked C, '
          f'{len(records)-parked} without parked C; {len(grouped)} unit batches')
    missing = sum(f['draft']=='no-parked-c' and not f['m2c_available'] for f in records)
    print(f'Untranslated functions without an existing raw m2c draft: {missing}')
    for group, n in sorted(collections.Counter(f['owner'] for f in records).items()):
        print(group+': '+str(n)+' functions')


if __name__ == '__main__':
    main()
