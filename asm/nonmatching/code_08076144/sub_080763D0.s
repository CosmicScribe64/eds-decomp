	thumb_func_start sub_080763D0
sub_080763D0: @ 0x080763D0
	push {r4, r5, r6, r7, lr}
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov ip, r2
	lsl r2, r0, #0x10
	lsr r6, r2, #0x10
	lsr r2, r0, #0x10
	mov r3, #0xFF
	lsl r3, r3, #8
	and r3, r1
	lsl r1, r1, #8
	ldr r4, _08076434 @ =0xFFFFFE00
	add r0, r4, #0
	and r1, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	ldr r0, _08076438 @ =0x03000040
	ldr r7, _0807643C @ =0x00004830
	add r4, r0, r7
	ldrb r1, [r4]
	cmp r1, #0x80
	beq _0807642C
	lsl r1, r1, #3
	ldr r7, _08076440 @ =0x00004430
	add r0, r0, r7
	add r1, r1, r0
	mov r0, #0xFF
	and r2, r0
	orr r3, r2
	mov r2, #0x90
	lsl r2, r2, #6
	add r0, r2, #0
	orr r3, r0
	strh r3, [r1]
	ldr r0, _08076444 @ =0x000001FF
	and r0, r6
	orr r0, r5
	strh r0, [r1, #2]
	mov r7, ip
	lsl r0, r7, #1
	strh r0, [r1, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0807642C:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076434: .4byte 0xFFFFFE00
_08076438: .4byte 0x03000040
_0807643C: .4byte 0x00004830
_08076440: .4byte 0x00004430
_08076444: .4byte 0x000001FF
	thumb_func_end sub_080763D0

