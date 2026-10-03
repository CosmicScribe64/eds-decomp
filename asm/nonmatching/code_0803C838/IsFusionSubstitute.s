	thumb_func_start IsFusionSubstitute
IsFusionSubstitute: @ 0x0803CB28
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r2, r0, #0
	ldr r1, _0803CB40 @ =0x00000101
	cmp r0, r1
	beq _0803CB52
	cmp r0, r1
	bgt _0803CB44
	cmp r0, #0x6C
	beq _0803CB52
	b _0803CB5C
	.align 2, 0
_0803CB40: .4byte 0x00000101
_0803CB44:
	mov r0, #0x86
	lsl r0, r0, #1
	cmp r2, r0
	beq _0803CB52
	ldr r0, _0803CB58 @ =0x00000281
	cmp r2, r0
	bne _0803CB5C
_0803CB52:
	mov r0, #1
	b _0803CB5E
	.align 2, 0
_0803CB58: .4byte 0x00000281
_0803CB5C:
	mov r0, #0
_0803CB5E:
	bx lr
	thumb_func_end IsFusionSubstitute

