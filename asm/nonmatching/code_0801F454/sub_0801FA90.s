	thumb_func_start sub_0801FA90
sub_0801FA90: @ 0x0801FA90
	push {r4, r5, r6, r7, lr}
	sub sp, #0xC
	add r5, r1, #0
	add r6, r2, #0
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	ldr r0, _0801FAD0 @ =0x0000FFFF
	and r0, r5
	cmp r0, #0
	bne _0801FAA6
	b _0801FBC4
_0801FAA6:
	ldr r1, _0801FAD4 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0801FB04
	ldr r1, _0801FAD8 @ =0x020192E0
	ldr r0, _0801FADC @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801FB04
	mov r0, #0x80
	lsl r0, r0, #0x18
	cmp r5, #0
	bge _0801FAE0
	sub r0, #1
	and r5, r0
	b _0801FAE2
_0801FAD0: .4byte 0x0000FFFF
_0801FAD4: .4byte 0x02015EE8
_0801FAD8: .4byte 0x020192E0
_0801FADC: .4byte 0x00001B12
_0801FAE0:
	orr r5, r0
_0801FAE2:
	mov r0, sp
	strh r7, [r0]
	strh r5, [r0, #2]
	mov r1, sp
	lsr r0, r5, #0x10
	strh r0, [r1, #4]
	mov r0, sp
	strh r6, [r0, #6]
	lsr r0, r6, #0x10
	strh r0, [r1, #8]
	ldr r0, _0801FB00 @ =0x0000F072
	mov r2, #0xA
	bl sub_080229BC
	b _0801FBC4
_0801FB00: .4byte 0x0000F072
_0801FB04:
	cmp r7, #0
	beq _0801FB28
	ldr r2, _0801FB24 @ =0x02017A40
	mov r3, #0xF0
	lsl r3, r3, #2
	add r1, r2, r3
	ldrh r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	mov r3, #0xA0
	lsl r3, r3, #2
	add r1, r2, r3
	add r4, r0, r1
	b _0801FB3A
	.align 2, 0
_0801FB24: .4byte 0x02017A40
_0801FB28:
	ldr r2, _0801FBB0 @ =0x02017A40
	mov r0, #0xF1
	lsl r0, r0, #2
	add r1, r2, r0
	ldrh r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r4, r0, r2
_0801FB3A:
	mov ip, r2
	strh r5, [r4]
	lsr r0, r5, #0x1F
	mov r3, #2
	neg r3, r3
	add r1, r3, #0
	ldrb r2, [r4, #2]
	and r1, r2
	orr r1, r0
	mov r0, #0xF0
	lsl r0, r0, #0x11
	and r0, r5
	lsr r0, r0, #0x15
	mov r2, #7
	and r0, r2
	lsl r0, r0, #1
	mov r2, #0xF
	neg r2, r2
	and r1, r2
	orr r1, r0
	strb r1, [r4, #2]
	mov r1, #0xF8
	lsl r1, r1, #0xD
	and r1, r5
	lsr r1, r1, #0xC
	ldr r0, _0801FBB4 @ =0xFFFFFC0F
	ldrh r2, [r4, #2]
	and r0, r2
	orr r0, r1
	strh r0, [r4, #2]
	ldrb r0, [r4, #4]
	and r3, r0
	mov r0, #3
	neg r0, r0
	and r3, r0
	sub r0, #2
	and r3, r0
	sub r0, #4
	and r3, r0
	sub r0, #8
	and r3, r0
	strb r3, [r4, #4]
	mov r1, #0xFC
	lsl r1, r1, #0x17
	and r1, r5
	lsr r1, r1, #0x17
	mov r0, #3
	ldrb r2, [r4, #3]
	and r0, r2
	orr r0, r1
	strb r0, [r4, #3]
	strh r6, [r4, #6]
	lsr r0, r6, #0x10
	strh r0, [r4, #8]
	cmp r7, #0
	beq _0801FBB8
	mov r1, #0xF0
	lsl r1, r1, #2
	b _0801FBBC
_0801FBB0: .4byte 0x02017A40
_0801FBB4: .4byte 0xFFFFFC0F
_0801FBB8:
	mov r1, #0xF1
	lsl r1, r1, #2
_0801FBBC:
	add r1, ip
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
_0801FBC4:
	add sp, #0xC
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0801FA90

