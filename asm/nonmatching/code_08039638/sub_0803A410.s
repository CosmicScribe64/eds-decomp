	thumb_func_start sub_0803A410
sub_0803A410: @ 0x0803A410
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _0803A4A4
	mov r4, #7
	ldrb r2, [r5, #0xA]
	and r4, r2
	cmp r4, #1
	bne _0803A4A4
	ldrb r3, [r5, #0xC]
	ldrh r0, [r5, #0xC]
	lsr r2, r0, #8
	add r1, r3, #0
	and r1, r4
	mov r0, #0x94
	mul r0, r2
	ldr r7, _0803A4AC @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r6, _0803A4B0 @ =0x0201930C
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A4A4
	add r0, r3, #0
	add r1, r2, #0
	bl sub_08018C3C
	add r0, r6, #0
	sub r0, #0x28
	ldrb r2, [r5, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	and r4, r1
	add r1, r4, #0
	mul r1, r7
	add r1, r1, r0
	ldrb r6, [r1, #2]
	cmp r6, #0
	beq _0803A47E
	add r4, r6, #0
_0803A46A:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0
	mov r2, #1
	bl sub_080193B0
	sub r4, #1
	cmp r4, #0
	bne _0803A46A
_0803A47E:
	mov r0, #1
	ldrb r2, [r5, #2]
	and r0, r2
	mov r1, #0x60
	cmp r0, #0
	beq _0803A48C
	ldr r1, _0803A4B4 @ =0x00008060
_0803A48C:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	bl sub_080199E0
_0803A4A4:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803A4AC: .4byte 0x00000D64
_0803A4B0: .4byte 0x0201930C
_0803A4B4: .4byte 0x00008060
	thumb_func_end sub_0803A410

