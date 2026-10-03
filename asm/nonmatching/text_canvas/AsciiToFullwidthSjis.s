	thumb_func_start AsciiToFullwidthSjis
AsciiToFullwidthSjis: @ 0x08074A90
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r1, r0, #0
	sub r1, #0x20
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x5E
	bls _08074AA4
	mov r0, #0
	b _08074AAC
_08074AA4:
	ldr r0, _08074AB0 @ =0x081A76A0
	lsl r1, r1, #1
	add r1, r1, r0
	ldrh r0, [r1]
_08074AAC:
	bx lr
	.align 2, 0
_08074AB0: .4byte gAsciiToSjisTable
	thumb_func_end AsciiToFullwidthSjis

