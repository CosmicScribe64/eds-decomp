	thumb_func_start sub_0803033C
sub_0803033C: @ 0x0803033C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _080303E2
	mov r0, #1
	ldrb r3, [r6, #2]
	and r0, r3
	mov r2, #0x92
	cmp r0, #0
	beq _08030360
	ldr r2, _080303F4 @ =0x00008092
_08030360:
	ldrh r0, [r6, #2]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r7, #0
	mov r1, #1
	mov sl, r1
_08030376:
	mov r4, #0
	add r3, r7, #1
	mov r9, r3
	add r5, r7, #0
	mov r0, sl
	and r5, r0
	lsl r0, r7, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
_08030388:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _080303F8 @ =0x00000D64
	mul r0, r5
	add r1, r1, r0
	ldr r0, _080303FC @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080303D6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080303D6
	add r0, r7, #0
	add r1, r4, #0
	bl sub_0800C8BC
	cmp r0, #2
	bne _080303D6
	ldrb r3, [r6, #2]
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	ldrh r3, [r6, #2]
	lsl r2, r3, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	lsl r2, r4, #0x18
	lsr r2, r2, #0x10
	mov r3, r8
	orr r2, r3
	mov r3, #2
	bl sub_08017AB4
_080303D6:
	add r4, #1
	cmp r4, #4
	ble _08030388
	mov r7, r9
	cmp r7, #1
	ble _08030376
_080303E2:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080303F4: .4byte 0x00008092
_080303F8: .4byte 0x00000D64
_080303FC: .4byte 0x0201930C
	thumb_func_end sub_0803033C

