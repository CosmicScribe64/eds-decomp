	thumb_func_start sub_0801FD68
sub_0801FD68: @ 0x0801FD68
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	add r4, r1, #0
	mov r7, #1
	cmp r2, #5
	beq _0801FD96
	cmp r2, #5
	bgt _0801FD82
	cmp r2, #0
	beq _0801FDA6
	b _0801FE28
_0801FD82:
	cmp r2, #0xB
	bne _0801FE28
	add r0, r5, #0
	add r1, r4, #0
	add r2, r3, #0
	bl sub_0802D25C
	cmp r0, #0
	beq _0801FE28
	b _0801FE26
_0801FD96:
	add r2, r3, #5
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0802D0E0
	cmp r0, #0
	beq _0801FE28
	b _0801FE26
_0801FDA6:
	and r4, r7
	mov r0, #0x94
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0801FE34 @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	ldr r0, _0801FE38 @ =0x0201930C
	add r2, r1, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _0801FE28
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0801FE28
	ldr r6, _0801FE3C @ =0x000007FF
	and r1, r6
	lsl r0, r1, #1
	ldr r1, _0801FE40 @ =0x08622AB4
	mov r8, r1
	add r0, r8
	ldr r1, _0801FE44 @ =0x000005F5
	ldrh r0, [r0]
	cmp r0, r1
	bne _0801FE28
	add r0, r6, #0
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0801FE48 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0801FE28
	ldr r4, _0801FE4C @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _0801FE28
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _0801FE28
	add r0, r6, #0
	ldrh r5, [r5]
	and r0, r5
	lsl r0, r0, #1
	add r0, r8
	ldr r1, _0801FE50 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	beq _0801FE28
_0801FE26:
	mov r7, #0x41
_0801FE28:
	add r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0801FE34: .4byte 0x00000D64
_0801FE38: .4byte 0x0201930C
_0801FE3C: .4byte 0x000007FF
_0801FE40: .4byte gUnk_08622AB4
_0801FE44: .4byte 0x000005F5
_0801FE48: .4byte gUnk_08621DE0
_0801FE4C: .4byte 0x0000058A
_0801FE50: .4byte 0x00000603
	thumb_func_end sub_0801FD68

