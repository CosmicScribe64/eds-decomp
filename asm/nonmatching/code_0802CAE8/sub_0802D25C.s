	thumb_func_start sub_0802D25C
sub_0802D25C: @ 0x0802D25C
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r0, #0
	add r6, r1, #0
	mov r0, #1
	and r0, r6
	lsl r2, r2, #2
	ldr r1, _0802D2F0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802D2F4 @ =0x02019968
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	ldr r4, _0802D2F8 @ =0x000007FF
	and r4, r5
	mov r0, sp
	add r1, r7, #0
	mov r2, #0x14
	bl sub_08075294
	mov r3, sp
	mov r0, #1
	add r1, r6, #0
	and r1, r0
	ldrb r2, [r3, #2]
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, sp
	strh r5, [r0]
	lsl r4, r4, #2
	ldr r0, _0802D2FC @ =0x08621DE0
	add r4, r4, r0
	ldr r4, [r4]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802D300
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r4, r0
	lsr r0, r4, #0x11
	cmp r0, #5
	bne _0802D300
	add r0, r5, #0
	bl sub_0802CD28
	add r4, r0, #0
	ldrh r0, [r7]
	bl sub_0802CD28
	cmp r4, r0
	blt _0802D300
	add r0, r6, #0
	add r1, r5, #0
	bl sub_08008C94
	cmp r0, #0
	beq _0802D300
	mov r0, sp
	add r1, r7, #0
	mov r2, #0
	bl sub_0802CE38
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802D302
	.align 2, 0
_0802D2F0: .4byte 0x00000D64
_0802D2F4: .4byte 0x02019968
_0802D2F8: .4byte 0x000007FF
_0802D2FC: .4byte gUnk_08621DE0
_0802D300:
	mov r0, #0
_0802D302:
	add sp, #0x14
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D25C
	.align 2, 0

