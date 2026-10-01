#!/usr/bin/env python3
"""Print, for each relocation in an object's .text, the value the baserom has at that spot.
Usage: relocvals.py file.o rom_address_of_text"""
import sys, struct
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
rom = open('/work/baserom.gba', 'rb').read()
base = int(sys.argv[2], 16) - 0x08000000
with open(sys.argv[1], 'rb') as f:
    e = ELFFile(f)
    symtab = e.get_section_by_name('.symtab')
    for sec in e.iter_sections():
        if isinstance(sec, RelocationSection) and sec.name == '.rel.text':
            for r in sec.iter_relocations():
                s = symtab.get_symbol(r['r_info_sym'])
                o = r['r_offset']
                print(f"{o:#06x} type={r['r_info_type']:3d} sym={s.name or '(sec '+str(s['st_shndx'])+')'} rom_word={struct.unpack_from('<I', rom, base+o)[0]:#010x}")
