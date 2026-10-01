import sys, capstone
rom=open('/work/baserom.gba','rb').read()
mode=sys.argv[1]; a=int(sys.argv[2],16); n=int(sys.argv[3],16)
md=capstone.Cs(capstone.CS_ARCH_ARM, capstone.CS_MODE_THUMB if mode=='t' else capstone.CS_MODE_ARM)
off=a-0x08000000
for i in md.disasm(rom[off:off+n], a):
    print(f"{i.address:08x}: {i.bytes.hex():10s} {i.mnemonic} {i.op_str}")
