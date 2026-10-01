	thumb_func_start sub_08064370
sub_08064370: @ 0x08064370
	push {r4, r5, lr}
	ldr r0, _080643BC @ =0x02020310
	ldr r2, [r0, #0xC]
	ldr r1, [r0, #8]
	add r3, r0, #0
	cmp r2, r1
	beq _08064388
	ldr r0, [r3, #0x10]
	cmp r0, #0
	bne _08064388
	mov r0, #4
	str r0, [r3, #0x10]
_08064388:
	ldr r4, [r3, #0x10]
	cmp r4, #0
	ble _080643C0
	ldr r0, [r3, #0xC]
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #4
	ldr r2, [r3, #8]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #4
	add r5, r0, #0
	add r5, #0x20
	sub r0, r0, r1
	mul r0, r4
	cmp r0, #0
	bge _080643AC
	add r0, #3
_080643AC:
	asr r0, r0, #2
	sub r1, r5, r0
	sub r0, r4, #1
	str r0, [r3, #0x10]
	cmp r0, #0
	bne _080643CC
	str r2, [r3, #0xC]
	b _080643CC
_080643BC: .4byte 0x02020310
_080643C0:
	ldr r1, [r3, #8]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #4
	add r1, r0, #0
	add r1, #0x20
_080643CC:
	mov r0, #0xB0
	lsl r0, r0, #0xF
	orr r1, r0
	mov r2, #0x80
	lsl r2, r2, #1
	add r0, r1, #0
	mov r1, #0x80
	bl sub_080761F0
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08064370

