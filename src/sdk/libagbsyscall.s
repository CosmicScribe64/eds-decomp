@ libagbsyscall -- Nintendo AGB SDK BIOS system-call stubs (Thumb).
@
@ Yu-Gi-Oh! The Eternal Duelist Soul (USA, AY5E)
@   ROM 0x0807ECF8 - 0x0807ED04 (0xC bytes, .text), directly after the ARM
@   sound mixer and directly before agb_sram.o (0x0807ED04).
@ Only the three stubs the game references were linked (one archive member
@ each in the SDK's libagbsyscall.a):
@   0x0807ECF8  CpuFastSet  swi 0x0C   (48 BL call sites)
@   0x0807ECFC  CpuSet      swi 0x0B   (153 BL call sites, e.g. 0x080002A6)
@   0x0807ED00  Div         swi 0x06   (2 BL call sites: 0x0807B50E, 0x0807B528)
@ The game makes no other SWI calls (no Thumb or ARM swi elsewhere in .text).
@ Order here is alphabetical, unlike the member order of the libagbsyscall.a
@ used by pokeemerald (which pulls Div before CpuFastSet/CpuSet).
@
@ Match status: MATCHING (assembled with arm-none-eabi-as -mcpu=arm7tdmi,
@ linked at 0x0807ECF8, compared byte-for-byte with baserom).

	.syntax unified
	.text

	.macro thumb_func_start name:req
	.align 2, 0
	.global \name
	.thumb
	.thumb_func
	.type \name, %function
	.endm

	.macro thumb_func_end name:req
	.size \name, .-\name
	.endm

	thumb_func_start CpuFastSet
CpuFastSet: @ 0x0807ECF8
	svc #0xC
	bx lr
	thumb_func_end CpuFastSet

	thumb_func_start CpuSet
CpuSet: @ 0x0807ECFC
	svc #0xB
	bx lr
	thumb_func_end CpuSet

	thumb_func_start Div
Div: @ 0x0807ED00
	svc #0x6
	bx lr
	thumb_func_end Div
