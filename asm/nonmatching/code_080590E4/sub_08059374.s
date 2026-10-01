	thumb_func_start sub_08059374
sub_08059374: @ 0x08059374
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r5, #5
	mov r4, sp
_08059380:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _080593E8 @ =0x0201A070
	add r3, r1, r0
	ldr r0, [r3]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080593F8
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r4]
	mov r0, #0x3F
	add r2, r5, #0
	and r2, r0
	lsl r2, r2, #4
	ldrh r0, [r4, #2]
	ldr r7, _080593EC @ =0xFFFFFC0F
	add r1, r7, #0
	and r0, r1
	orr r0, r2
	strh r0, [r4, #2]
	ldrb r1, [r4, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r4, #3]
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080593F0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _080593F8
	mov r0, sp
	mov r1, #1
	add r2, r5, #0
	bl sub_08041DC4
	cmp r0, #0
	beq _080593F8
	ldr r1, _080593F4 @ =0x02015EF0
	strb r5, [r1, #0xB]
	mov r0, #0xC8
	strb r0, [r1, #6]
	mov r0, #1
	b _08059400
_080593E8: .4byte 0x0201A070
_080593EC: .4byte 0xFFFFFC0F
_080593F0: .4byte gUnk_08622AB4
_080593F4: .4byte 0x02015EF0
_080593F8:
	add r5, #1
	cmp r5, #9
	ble _08059380
	mov r0, #0
_08059400:
	add sp, #0x14
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08059374

