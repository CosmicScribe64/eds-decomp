	thumb_func_start AddSpriteFlip
AddSpriteFlip: @ 0x08076624
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
	lsr r2, r0, #0x10
	mov r3, #0xFF
	lsl r3, r3, #8
	and r3, r1
	lsl r1, r1, #8
	ldr r4, _08076690 @ =0xFFFFFE00
	add r0, r4, #0
	and r1, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	ldr r0, _08076694 @ =0x03000040
	ldr r7, _08076698 @ =0x00004830
	add r4, r0, r7
	ldrb r1, [r4]
	cmp r1, #0x80
	beq _08076684
	lsl r1, r1, #3
	ldr r7, _0807669C @ =0x00004430
	add r0, r0, r7
	add r1, r1, r0
	mov r0, #0xFF
	and r2, r0
	orr r3, r2
	strh r3, [r1]
	ldr r0, _080766A0 @ =0x000001FF
	and r0, r6
	orr r0, r5
	mov r2, r8
	orr r0, r2
	strh r0, [r1, #2]
	mov r7, ip
	strh r7, [r1, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08076684:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076690: .4byte 0xFFFFFE00
_08076694: .4byte 0x03000040
_08076698: .4byte 0x00004830
_0807669C: .4byte 0x00004430
_080766A0: .4byte 0x000001FF
	thumb_func_end AddSpriteFlip

