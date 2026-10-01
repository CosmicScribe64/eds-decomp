	thumb_func_start sub_0802D0E0
sub_0802D0E0: @ 0x0802D0E0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	mov r9, r0
	add r7, r1, #0
	mov r8, r2
	mov r1, #1
	and r1, r7
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	ldr r0, _0802D190 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802D194 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	ldrb r2, [r2, #6]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1F
	mov sl, r0
	mov r0, sp
	mov r1, r9
	mov r2, #0x14
	bl sub_08075294
	mov r3, sp
	mov r0, #1
	add r1, r7, #0
	and r1, r0
	ldrb r2, [r3, #2]
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, sp
	strh r5, [r0]
	cmp r5, #0
	beq _0802D21E
	ldr r6, _0802D198 @ =0x000007FF
	and r6, r5
	lsl r0, r6, #2
	ldr r1, _0802D19C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0802D21E
	add r0, r5, #0
	bl sub_0802CD28
	add r4, r0, #0
	mov r1, r9
	ldrh r0, [r1]
	bl sub_0802CD28
	cmp r4, r0
	blt _0802D21E
	add r0, r7, #0
	mov r1, r8
	bl sub_0802D058
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802D21E
	lsl r0, r6, #1
	ldr r1, _0802D1A0 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802D1A4 @ =0x0000052C
	cmp r1, r0
	beq _0802D1B8
	cmp r1, r0
	bgt _0802D1AC
	ldr r0, _0802D1A8 @ =0x000003F9
	cmp r1, r0
	beq _0802D1B8
	b _0802D1BC
	.align 2, 0
_0802D190: .4byte 0x00000D64
_0802D194: .4byte 0x0201930C
_0802D198: .4byte 0x000007FF
_0802D19C: .4byte gUnk_08621DE0
_0802D1A0: .4byte gUnk_08622AB4
_0802D1A4: .4byte 0x0000052C
_0802D1A8: .4byte 0x000003F9
_0802D1AC:
	ldr r0, _0802D224 @ =0x00000594
	cmp r1, r0
	beq _0802D1B8
	add r0, #0x68
	cmp r1, r0
	bne _0802D1BC
_0802D1B8:
	mov r0, #0
	mov sl, r0
_0802D1BC:
	mov r1, sl
	cmp r1, #0
	bne _0802D21E
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	ldr r1, _0802D228 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802D22C @ =0x0201930C
	add r0, r0, r1
	add r0, #0x91
	ldrb r1, [r0]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0802D21E
	mov r0, #8
	and r0, r1
	cmp r0, #0
	bne _0802D21E
	ldr r0, _0802D230 @ =0x000007FF
	and r5, r0
	lsl r0, r5, #2
	ldr r1, _0802D234 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0802D23C
	ldr r4, _0802D238 @ =0x000002EF
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _0802D21E
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _0802D23C
_0802D21E:
	mov r0, #0
	b _0802D24A
	.align 2, 0
_0802D224: .4byte 0x00000594
_0802D228: .4byte 0x00000D64
_0802D22C: .4byte 0x0201930C
_0802D230: .4byte 0x000007FF
_0802D234: .4byte gUnk_08621DE0
_0802D238: .4byte 0x000002EF
_0802D23C:
	mov r0, sp
	mov r1, r9
	mov r2, #0
	bl sub_0802CE38
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0802D24A:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802D0E0
	.align 2, 0

