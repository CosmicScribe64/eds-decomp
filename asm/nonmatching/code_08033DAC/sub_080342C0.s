	thumb_func_start sub_080342C0
sub_080342C0: @ 0x080342C0
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _0803434C
	ldrb r1, [r7, #0xA]
	mov r0, #7
	and r0, r1
	add r4, r1, #0
	cmp r0, #3
	bne _0803434C
	lsl r0, r4, #0x1D
	mov r6, #0
	cmp r0, #0
	beq _08034314
	add r3, r0, #0
_080342E4:
	lsl r1, r6, #1
	add r0, r7, #0
	add r0, #0xC
	add r0, r0, r1
	ldrh r2, [r0]
	lsr r1, r2, #8
	mov r2, #1
	ldrb r0, [r0]
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _08034354 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08034358 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803434C
	add r6, #1
	lsr r0, r3, #0x1D
	cmp r6, r0
	blt _080342E4
_08034314:
	lsl r0, r4, #0x1D
	mov r6, #0
	cmp r0, #0
	beq _0803434C
_0803431C:
	lsl r1, r6, #1
	add r0, r7, #0
	add r0, #0xC
	add r0, r0, r1
	ldrb r4, [r0]
	ldrh r0, [r0]
	lsr r5, r0, #8
	add r0, r4, #0
	add r1, r5, #0
	bl sub_08030028
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	add r2, r5, #0
	bl sub_08046CB0
	add r6, #1
	ldrb r2, [r7, #0xA]
	lsl r0, r2, #0x1D
	lsr r0, r0, #0x1D
	cmp r6, r0
	blt _0803431C
_0803434C:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08034354: .4byte 0x00000D64
_08034358: .4byte 0x0201930C
	thumb_func_end sub_080342C0

