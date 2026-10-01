import sys, glob, os
from elftools.elf.elffile import ELFFile
rom = open('/work/baserom.gba','rb').read()
def text_and_mask(path):
    with open(path,'rb') as f:
        e = ELFFile(f)
        out = []
        for si,sec in enumerate(e.iter_sections()):
            if sec.name.startswith('.text') and sec['sh_size']>0:
                data = bytearray(sec.data())
                mask = bytearray(b'\xff'*len(data))
                rel = e.get_section_by_name('.rel'+sec.name)
                if rel:
                    for r in rel.iter_relocations():
                        o=r['r_offset']
                        for k in range(4):
                            if o+k < len(mask): mask[o+k]=0
                syms = []
                st = e.get_section_by_name('.symtab')
                for s in st.iter_symbols():
                    if s["st_shndx"]==si and s['st_info']['type'] in ('STT_FUNC','STT_NOTYPE') and s.name and not s.name.startswith('$') and not s.name.startswith('.'):
                        syms.append((s['st_value'],s.name))
                out.append((sec.name,bytes(data),bytes(mask),sorted(syms)))
        return out
def find(data,mask):
    # anchor on longest unmasked prefix
    n=len(data)
    first = data[:8]
    i=0; hits=[]
    while True:
        i = rom.find(first, i)
        if i<0: break
        if all((rom[i+k]&mask[k])==(data[k]&mask[k]) for k in range(n)): hits.append(i)
        i+=1
    return hits
for p in sorted(glob.glob(sys.argv[1]+'/*.o')):
    for name,data,mask,syms in text_and_mask(p):
        if len(data)<8: continue
        h = find(data,mask)
        print(os.path.basename(p), name, hex(len(data)), [hex(0x08000000+x) for x in h], syms[:6])
