	thumb_func_start AddSprite8bppFlip
AddSprite8bppFlip: @ 0x08076348
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	lsl r2, r0, #0x10
	lsr r6, r2, #0x10
	lsr r3, r0, #0x10
	mov r2, #0xFF
	lsl r2, r2, #8
	and r2, r1
	lsl r1, r1, #8
	ldr r4, _080763BC @ =0xFFFFFE00
	add r0, r4, #0
	and r1, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	ldr r0, _080763C0 @ =0x03000040
	ldr r7, _080763C4 @ =0x00004830
	add r4, r0, r7
	ldrb r1, [r4]
	cmp r1, #0x80
	beq _080763B2
	lsl r1, r1, #3
	ldr r7, _080763C8 @ =0x00004430
	add r0, r0, r7
	add r1, r1, r0
	mov r0, #0xFF
	and r3, r0
	orr r2, r3
	mov r3, #0x80
	lsl r3, r3, #6
	add r0, r3, #0
	orr r2, r0
	strh r2, [r1]
	ldr r0, _080763CC @ =0x000001FF
	and r0, r6
	orr r0, r5
	mov r7, r8
	orr r0, r7
	strh r0, [r1, #2]
	mov r2, ip
	lsl r0, r2, #1
	strh r0, [r1, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080763B2:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080763BC: .4byte 0xFFFFFE00
_080763C0: .4byte 0x03000040
_080763C4: .4byte 0x00004830
_080763C8: .4byte 0x00004430
_080763CC: .4byte 0x000001FF
	thumb_func_end AddSprite8bppFlip

