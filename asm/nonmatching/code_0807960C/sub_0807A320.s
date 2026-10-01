	thumb_func_start sub_0807A320
sub_0807A320: @ 0x0807A320
	push {r4, r5, lr}
	add r5, r1, #0
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	ldr r0, _0807A370 @ =0x00000614
	add r4, r5, r0
	ldrb r0, [r4]
	lsl r2, r0, #0x19
	lsr r0, r2, #0x19
	cmp r0, #0x7F
	bhi _0807A374
	add r2, r0, #0
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r5, r0
	add r3, r5, r1
	ldrb r1, [r3]
	strb r1, [r0, #0x1C]
	strb r2, [r3]
	ldrb r2, [r4]
	lsl r0, r2, #0x19
	lsr r0, r0, #0x19
	add r0, #1
	mov r1, #0x7F
	and r0, r1
	mov r1, #0x80
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	lsl r1, r1, #0x19
	lsr r1, r1, #0x19
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, #8
	add r0, r5, r0
	b _0807A376
	.align 2, 0
_0807A370: .4byte 0x00000614
_0807A374:
	mov r0, #0
_0807A376:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0807A320

