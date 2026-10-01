#!/usr/bin/env python3
# usage: cmpobj.py <obj.o> <.sec>=<addr> ... [sym=addr[:a|:d] ...]
# Links obj with each section placed at addr; extra symbols become stub thumb funcs (:a = ARM func, :d = data)
# at the given absolute address; then compares each placed section with the ROM byte-for-byte.
import sys, subprocess, os
from elftools.elf.elffile import ELFFile
rom=open('/work/baserom.gba','rb').read()
obj=sys.argv[1]; secs={}; syms={}
for a in sys.argv[2:]:
    k,v=a.split('=')
    if k.startswith('.'): secs[k]=int(v,16)
    else:
        kind='t'
        if ':' in v: v,kind=v.split(':')
        syms[k]=(int(v,16),kind)
base=obj[:-2]; elf=base+'.elf'
cmd=['arm-none-eabi-ld','-o',elf,'-e','0','--no-warn-rwx-segments']
objs=[obj]
if syms:
    st=[]
    for i,(k,(v,kind)) in enumerate(syms.items()):
        sec='.stub%d'%i
        if kind=='t': st.append('.section %s,"ax"\n.thumb\n.global %s\n.thumb_func\n%s: bx lr\n'%(sec,k,k))
        elif kind=='a': st.append('.section %s,"ax"\n.arm\n.global %s\n.type %s,%%function\n%s: bx lr\n'%(sec,k,k,k))
        else: st.append('.section %s,"a"\n.global %s\n%s: .word 0\n'%(sec,k,k))
        cmd+=['--section-start=%s=0x%x'%(sec,v)]
    open(base+'_stub.s','w').write(''.join(st))
    subprocess.check_call(['arm-none-eabi-as','-mcpu=arm7tdmi','-mthumb-interwork',base+'_stub.s','-o',base+'_stub.o'])
    objs.append(base+'_stub.o')
for k,v in secs.items(): cmd+=['--section-start=%s=0x%x'%(k,v)]
cmd+=objs
subprocess.check_call(cmd)
ok=True
with open(obj,'rb') as f:
    insz={s.name:s['sh_size'] for s in ELFFile(f).iter_sections()}
with open(elf,'rb') as f:
    e=ELFFile(f)
    st=e.get_section_by_name('.symtab')
    for s in e.iter_sections():
        if s.name not in secs: continue
        d=s.data()[:insz.get(s.name,s['sh_size'])]; a=s['sh_addr']; o=a-0x08000000
        r=rom[o:o+len(d)]
        fsyms=sorted(set([(x['st_value']&~1,x.name) for x in st.iter_symbols() if x['st_shndx']!='SHN_UNDEF' and x.name and not x.name.startswith('$') and not x.name.startswith('.') and x['st_info']['type'] in ('STT_FUNC','STT_OBJECT','STT_NOTYPE') and a<=x['st_value']<a+len(d)]))
        diffs=[i for i in range(len(d)) if d[i]!=r[i]]
        print('%s @0x%08x size 0x%x: %s'%(s.name,a,len(d),'MATCH' if not diffs else '%d diff bytes'%len(diffs)))
        for (sa,n),nxt in zip(fsyms,fsyms[1:]+[(a+len(d),None)]):
            dd=[i for i in diffs if sa-a<=i<nxt[0]-a]
            print('   %-28s 0x%08x size 0x%-4x %s'%(n,sa,nxt[0]-sa,'ok' if not dd else 'DIFF at +0x%x'%(dd[0]-(sa-a))))
        if diffs: ok=False
sys.exit(0 if ok else 1)
