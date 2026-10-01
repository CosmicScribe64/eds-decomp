	thumb_func_start sub_0807A568
sub_0807A568: @ 0x0807A568
	push {r4, r5, r6, r7, lr}
	ldr r4, [sp, #0x14]
	ldr r5, [sp, #0x18]
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r5, r5, #0x18
	lsr r7, r5, #0x18
	lsr r1, r1, #0xD
	lsr r2, r2, #0x17
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r2, r2, r0
	add r1, r1, r2
	lsr r3, r3, #0x12
	add r1, r1, r3
	mov r3, #0
	cmp r3, r7
	bcs _0807A5CC
	mov r0, #0x20
	sub r0, r0, r4
	lsl r0, r0, #1
	mov ip, r0
_0807A59E:
	mov r2, #0
	add r3, #1
	cmp r2, r4
	bcs _0807A5C2
	mov r5, #0x80
	lsl r5, r5, #4
_0807A5AA:
	cmp r4, #0x1F
	bls _0807A5B4
	add r0, r1, r5
	strh r6, [r0]
	b _0807A5B6
_0807A5B4:
	strh r6, [r1]
_0807A5B6:
	add r1, #2
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, r4
	bcc _0807A5AA
_0807A5C2:
	add r1, ip
	lsl r0, r3, #0x18
	lsr r3, r0, #0x18
	cmp r3, r7
	bcc _0807A59E
_0807A5CC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807A568
	.align 2, 0

