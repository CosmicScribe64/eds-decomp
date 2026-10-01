	thumb_func_start sub_080766A4
sub_080766A4: @ 0x080766A4
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov ip, r3
	mov r3, #0xFF
	lsl r3, r3, #8
	and r3, r2
	lsl r2, r2, #8
	ldr r4, _08076704 @ =0xFFFFFE00
	add r0, r4, #0
	and r2, r0
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	ldr r0, _08076708 @ =0x03000040
	ldr r7, _0807670C @ =0x00004830
	add r4, r0, r7
	ldrb r2, [r4]
	cmp r2, #0x80
	beq _080766FC
	lsl r2, r2, #3
	ldr r7, _08076710 @ =0x00004430
	add r0, r0, r7
	add r2, r2, r0
	lsl r0, r1, #0x10
	asr r0, r0, #0x10
	mov r1, #0xFF
	and r0, r1
	orr r3, r0
	strh r3, [r2]
	lsl r0, r6, #0x17
	lsr r0, r0, #0x17
	orr r0, r5
	strh r0, [r2, #2]
	mov r0, ip
	strh r0, [r2, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080766FC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076704: .4byte 0xFFFFFE00
_08076708: .4byte 0x03000040
_0807670C: .4byte 0x00004830
_08076710: .4byte 0x00004430
	thumb_func_end sub_080766A4

