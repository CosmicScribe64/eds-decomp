	thumb_func_start sub_0807625C
sub_0807625C: @ 0x0807625C
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
	ldr r4, _080762BC @ =0xFFFFFE00
	add r0, r4, #0
	and r1, r0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	ldr r0, _080762C0 @ =0x03000040
	ldr r7, _080762C4 @ =0x00004830
	add r4, r0, r7
	ldrb r1, [r4]
	cmp r1, #0x80
	beq _080762B6
	lsl r1, r1, #3
	ldr r7, _080762C8 @ =0x00004430
	add r0, r0, r7
	add r1, r1, r0
	mov r0, #0xFF
	and r2, r0
	orr r3, r2
	mov r2, #0x80
	lsl r2, r2, #3
	add r0, r2, #0
	orr r3, r0
	strh r3, [r1]
	ldr r0, _080762CC @ =0x000001FF
	and r0, r6
	orr r0, r5
	strh r0, [r1, #2]
	mov r7, ip
	strh r7, [r1, #4]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080762B6:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080762BC: .4byte 0xFFFFFE00
_080762C0: .4byte 0x03000040
_080762C4: .4byte 0x00004830
_080762C8: .4byte 0x00004430
_080762CC: .4byte 0x000001FF
	thumb_func_end sub_0807625C

