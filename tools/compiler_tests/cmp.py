#!/usr/bin/env python3
# Compile a one-function C file with agbcc and compare its .text against the ROM.
# usage: cmp.py src.c ADDR [--cc agbcc|old_agbcc] [--flags "..."] [-q] [--asm]
# Compiles one-function C with agbcc (cpp -> cc1 -> as, pret-style ".align 2, 0" trailer),
# resolves relocations from _XXXXXXXX name suffixes / libgcc table, compares .text to baserom.
import sys, subprocess, re, argparse, capstone, os, tempfile
from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection
ap=argparse.ArgumentParser()
ap.add_argument('src'); ap.add_argument('addr')
ap.add_argument('--cc',default='agbcc')
ap.add_argument('--flags',default='-mthumb-interwork -O2 -fhex-asm')
ap.add_argument('-q',action='store_true')
ap.add_argument('--asm',action='store_true')
ap.add_argument('--arm',action='store_true')
a=ap.parse_args()
rom=open('/work/baserom.gba','rb').read()
addr=int(a.addr,16)
td=tempfile.mkdtemp()
s=os.path.join(td,'o.s'); o=os.path.join(td,'o.o')
cpp=subprocess.run(['cpp','-nostdinc','-I/opt/agbcc/include','-undef',a.src],capture_output=True,text=True)
if cpp.returncode: print(cpp.stderr); sys.exit(2)
cc=subprocess.run([a.cc]+a.flags.split()+['-o',s],input=cpp.stdout,capture_output=True,text=True)
if cc.returncode or not os.path.exists(s): print('CC FAIL',cc.stderr); sys.exit(2)
if cc.stderr.strip() and not a.q: print(cc.stderr.strip())
open(s,'a').write('\n\t.text\n\t.align\t2, 0\n')
if a.asm: print(open(s).read())
r=subprocess.run(['arm-none-eabi-as','-mcpu=arm7tdmi','-o',o,s],capture_output=True,text=True)
if r.returncode: print('AS FAIL',r.stderr); sys.exit(2)
e=ELFFile(open(o,'rb'))
text=e.get_section_by_name('.text'); data=bytearray(text.data())
symtab=e.get_section_by_name('.symtab')
mask=bytearray(b'\x01'*len(data)); notes=[]
rel=e.get_section_by_name('.rel.text')
LIB={'_call_via_r0':0x0807EE98,'_call_via_r1':0x0807EE9C,'_call_via_r2':0x0807EEA0,'_call_via_r3':0x0807EEA4,
 '__divsi3':0x0807EED4,'__modsi3':0x0807EF6C,'__muldi3':0x0807F03C,'__udivsi3':0x0807F0AC,'__umodsi3':0x0807F124,
 'memcpy':0x08080918,'memset':0x08080978,'strcpy':0x080809CC}
for k in range(4,8): LIB[f'_call_via_r{k}']=0x0807EE98+4*k
def symaddr(name):
    if name in LIB: return LIB[name]
    m=re.search(r'_(0[0-9A-Fa-f]{7})$',name)
    return int(m.group(1),16) if m else None
if rel:
    for r_ in rel.iter_relocations():
        off=r_['r_offset']; sym=symtab.get_symbol(r_['r_info_sym']); t=r_['r_info_type']
        sa=symaddr(sym.name)
        if t==2: # ABS32
            add=int.from_bytes(data[off:off+4],'little')
            if sa is not None:
                if sym.name.startswith('sub_') or sym['st_info']['type']=='STT_FUNC': sa|=1
                data[off:off+4]=((sa+add)&0xffffffff).to_bytes(4,'little')
            else:
                for k in range(4): mask[off+k]=0
                notes.append(f'{off:#x}: abs32 {sym.name} (masked)')
        elif t in (10,): # THM_CALL
            if sa is not None:
                pc=addr+off+4; d=(sa-pc)>>1
                hi=0xF000|((d>>11)&0x7ff); lo=0xF800|(d&0x7ff)
                data[off:off+2]=hi.to_bytes(2,'little'); data[off+2:off+4]=lo.to_bytes(2,'little')
            else:
                for k in range(4): mask[off+k]=0
                notes.append(f'{off:#x}: call {sym.name} (masked)')
        else:
            for k in range(4): mask[off+k]=0
            notes.append(f'{off:#x}: reloc type {t} {sym.name} (masked)')
n=len(data)
target=rom[addr-0x08000000:addr-0x08000000+n]
diff=[i for i in range(n) if mask[i] and data[i]!=target[i]]
# trailing padding tolerance: if our output ends with 2 zero bytes of alignment and rom differs there, note it
status='MATCH' if not diff else f'DIFF ({len(diff)} bytes, first at +{diff[0]:#x})'
print(f'{os.path.basename(a.src)} @ {addr:#010x} size {n:#x}: {status}  [{a.cc} {a.flags}]')
for nt in notes:
    if not a.q: print('  ',nt)
if diff and not a.q:
    md=capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_ARM if a.arm else capstone.CS_MODE_THUMB)
    step=4 if a.arm else 2
    i=0
    while i<n:
        mine=bytes(data[i:i+step]); theirs=target[i:i+step]
        def dis(b,ad):
            x=list(md.disasm(b,ad))
            return f'{x[0].mnemonic} {x[0].op_str}' if x else b.hex()
        flag='  ' if all((not mask[i+k]) or data[i+k]==target[i+k] for k in range(len(mine))) else '!!'
        print(f'{flag} {addr+i:08x}  {mine.hex():8s} {dis(mine,addr+i):28s} | {theirs.hex():8s} {dis(theirs,addr+i)}')
        i+=step
sys.exit(0 if not diff else 1)
