	thumb_func_start AnimStateTick
AnimStateTick: @ 0x080786D0
	add r1, r0, #0
	ldrb r0, [r1, #0xE]
	cmp r0, #1
	bne _0807871A
	ldrb r0, [r1, #0xF]
	sub r0, #1
	strb r0, [r1, #0xF]
	mov r2, #0xFF
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	bne _0807871A
	ldrb r0, [r1, #0xD]
	add r0, #1
	strb r0, [r1, #0xD]
	and r0, r2
	ldr r2, [r1]
	lsl r0, r0, #3
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _08078700
	strb r0, [r1, #0xD]
	strb r0, [r1, #0xE]
_08078700:
	ldrb r3, [r1, #0xD]
	lsl r0, r3, #3
	add r0, r0, r2
	ldrb r0, [r0]
	strb r0, [r1, #0xF]
	lsl r0, r3, #3
	add r0, r0, r2
	ldr r0, [r0, #4]
	str r0, [r1, #4]
	lsl r0, r3, #3
	add r0, r0, r2
	ldrb r0, [r0, #1]
	strb r0, [r1, #0xC]
_0807871A:
	bx lr
	thumb_func_end AnimStateTick

