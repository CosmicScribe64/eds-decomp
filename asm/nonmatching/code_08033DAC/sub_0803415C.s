	thumb_func_start sub_0803415C
sub_0803415C: @ 0x0803415C
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldrb r6, [r5, #6]
	ldrh r0, [r5, #6]
	lsr r4, r0, #8
	mov r7, #1
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _08034236
	mov r0, #0xFC
	ldrb r1, [r5, #3]
	and r0, r1
	cmp r0, #0x14
	beq _08034180
	cmp r0, #0x18
	bne _08034236
_08034180:
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mul r0, r4
	ldr r1, _080341C8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080341CC @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08034236
	ldrh r0, [r5]
	add r1, r6, #0
	add r2, r4, #0
	bl sub_0802B1B8
	cmp r0, #0
	beq _08034236
	ldr r0, _080341D0 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080341D4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080341D8 @ =0x000002A9
	cmp r1, r0
	beq _08034206
	cmp r1, r0
	bgt _080341DC
	sub r0, #1
	cmp r1, r0
	beq _080341FC
	b _0803421C
_080341C8: .4byte 0x00000D64
_080341CC: .4byte 0x0201930C
_080341D0: .4byte 0x000007FF
_080341D4: .4byte gUnk_08622AB4
_080341D8: .4byte 0x000002A9
_080341DC:
	ldr r0, _080341F4 @ =0x000003EA
	cmp r1, r0
	bne _0803421C
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C894
	mov r2, #0
	ldr r1, _080341F8 @ =0x000003E7
	cmp r0, r1
	ble _0803421A
	b _08034218
_080341F4: .4byte 0x000003EA
_080341F8: .4byte 0x000003E7
_080341FC:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C8A8
	b _0803420E
_08034206:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C894
_0803420E:
	mov r2, #0
	mov r1, #0xFA
	lsl r1, r1, #1
	cmp r0, r1
	bgt _0803421A
_08034218:
	mov r2, #1
_0803421A:
	add r7, r2, #0
_0803421C:
	cmp r7, #0
	beq _08034236
	add r0, r6, #0
	add r1, r4, #0
	bl sub_08030028
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	add r2, r4, #0
	bl sub_08046CB0
_08034236:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803415C
	.align 2, 0

