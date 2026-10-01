	thumb_func_start sub_08032258
sub_08032258: @ 0x08032258
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #0xFC
	ldrb r1, [r4, #3]
	and r0, r1
	cmp r0, #0x40
	bne _080322AA
	ldrb r3, [r4, #6]
	ldrh r0, [r4, #6]
	lsr r5, r0, #8
	mov r6, #1
	add r1, r3, #0
	and r1, r6
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _080322FC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08032300 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080322AA
	add r0, r6, #0
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _080322AA
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r3
	beq _080322AA
	add r0, r3, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
_080322AA:
	mov r2, #7
	ldrb r3, [r4, #0xA]
	and r2, r3
	cmp r2, #1
	bne _08032314
	ldrb r3, [r4, #0xC]
	ldrh r0, [r4, #0xC]
	lsr r1, r0, #8
	and r2, r3
	mov r0, #0x94
	mul r1, r0
	ldr r0, _080322FC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08032300 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	ldrb r2, [r4, #2]
	cmp r0, #0
	beq _08032304
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08032304
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	cmp r0, r3
	bne _08032304
	add r1, r0, #0
	ldrh r3, [r4, #2]
	lsl r2, r3, #0x16
	lsr r2, r2, #0x1A
	lsl r2, r2, #8
	orr r1, r2
	ldrh r2, [r4, #0xC]
	bl sub_08017B04
	b _08032314
	.align 2, 0
_080322FC: .4byte 0x00000D64
_08032300: .4byte 0x0201930C
_08032304:
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	mov r2, #1
	bl sub_08018544
_08032314:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08032258

