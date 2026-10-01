	thumb_func_start sub_08032058
sub_08032058: @ 0x08032058
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _080320B4
	ldrb r1, [r6, #0xA]
	lsl r0, r1, #0x1D
	mov r4, #0
	cmp r0, #0
	beq _080320B4
_08032070:
	lsl r1, r4, #1
	add r0, r6, #0
	add r0, #0xC
	add r0, r0, r1
	ldrb r5, [r0]
	ldrh r0, [r0]
	lsr r3, r0, #8
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _080320BC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080320C0 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080320A4
	add r0, r5, #0
	add r1, r3, #0
	mov r2, #0
	bl sub_08018AE8
_080320A4:
	add r4, #1
	ldrb r1, [r6, #0xA]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1D
	cmp r4, r0
	bge _080320B4
	cmp r4, #1
	ble _08032070
_080320B4:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_080320BC: .4byte 0x00000D64
_080320C0: .4byte 0x0201930C
	thumb_func_end sub_08032058

