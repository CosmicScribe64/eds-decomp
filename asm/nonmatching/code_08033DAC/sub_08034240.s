	thumb_func_start sub_08034240
sub_08034240: @ 0x08034240
	push {r4, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _080342A6
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _080342A6
	ldrb r4, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r3, r1, #8
	and r2, r4
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _080342B0 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080342B4 @ =0x0201930C
	add r2, r1, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _080342A6
	ldr r0, _080342B8 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _080342BC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _080342A6
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _080342A6
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #1
	bl sub_08018544
_080342A6:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080342B0: .4byte 0x00000D64
_080342B4: .4byte 0x0201930C
_080342B8: .4byte 0x000007FF
_080342BC: .4byte gUnk_08621DE0
	thumb_func_end sub_08034240

