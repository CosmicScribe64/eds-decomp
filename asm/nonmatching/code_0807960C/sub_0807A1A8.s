	thumb_func_start sub_0807A1A8
sub_0807A1A8: @ 0x0807A1A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r4, r0, #0
	add r5, r1, #0
	add r6, r2, #0
	mov r0, #0
	str r0, [sp, #0]
	mov sl, r0
	ldr r1, _0807A21C @ =0x02030000
	mov r9, r1
	ldr r7, _0807A220 @ =0x00000FEE
	mov r2, #0
	mov r3, #0
	mov r8, r3
	ldr r1, _0807A224 @ =0x00000FED
_0807A1CE:
	mov r3, r9
	add r0, r3, r2
	mov r3, r8
	strb r3, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, r1
	bls _0807A1CE
_0807A1E0:
	mov r1, sl
	lsl r0, r1, #0x19
	lsr r0, r0, #0x18
	mov sl, r0
	cmp r0, #0
	bne _0807A1F8
	ldrb r3, [r4]
	str r3, [sp, #0]
	add r4, #1
	sub r6, #1
	mov r0, #1
	mov sl, r0
_0807A1F8:
	ldr r0, [sp, #0]
	mov r1, sl
	and r0, r1
	cmp r0, #0
	beq _0807A22C
	ldrb r1, [r4]
	strb r1, [r5]
	add r5, #1
	add r0, r7, #0
	add r7, r0, #1
	add r0, r9
	strb r1, [r0]
	add r4, #1
	ldr r3, _0807A228 @ =0x00000FFF
	and r7, r3
	sub r6, #1
	b _0807A27A
	.align 2, 0
_0807A21C: .4byte 0x02030000
_0807A220: .4byte 0x00000FEE
_0807A224: .4byte 0x00000FED
_0807A228: .4byte 0x00000FFF
_0807A22C:
	ldrb r3, [r4]
	add r4, #1
	ldrb r1, [r4]
	add r4, #1
	mov r0, #0xF0
	and r0, r1
	lsl r0, r0, #4
	orr r3, r0
	ldr r0, _0807A290 @ =0x0000FF0F
	and r1, r0
	sub r6, #2
	mov r2, #0
	add r1, #3
	mov ip, r1
	cmp r2, ip
	bge _0807A27A
	ldr r1, _0807A294 @ =0x00000FFF
	mov r8, r1
_0807A250:
	mov r1, r9
	add r0, r1, r3
	ldrb r0, [r0]
	strb r0, [r5]
	add r5, #1
	add r1, r7, #0
	add r7, r1, #1
	add r1, r9
	add r0, r3, #0
	add r3, r0, #1
	add r0, r9
	ldrb r0, [r0]
	strb r0, [r1]
	mov r0, r8
	and r7, r0
	and r3, r0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, ip
	blt _0807A250
_0807A27A:
	cmp r6, #0
	bne _0807A1E0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807A290: .4byte 0x0000FF0F
_0807A294: .4byte 0x00000FFF
	thumb_func_end sub_0807A1A8

