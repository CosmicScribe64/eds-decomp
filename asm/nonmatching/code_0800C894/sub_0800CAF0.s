	thumb_func_start sub_0800CAF0
sub_0800CAF0: @ 0x0800CAF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r3, r1, #0
	mov r1, #1
	and r1, r0
	ldr r5, _0800CB20 @ =0x00000D64
	mul r1, r5
	ldr r4, _0800CB24 @ =0x0201930C
	mov r0, #0x94
	mul r0, r3
	add r0, r0, r1
	add r2, r0, r4
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _0800CB28
	mov r0, #0
	b _0800CBEE
	.align 2, 0
_0800CB20: .4byte 0x00000D64
_0800CB24: .4byte 0x0201930C
_0800CB28:
	ldr r0, _0800CC00 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #2
	ldr r1, _0800CC04 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	lsr r7, r0, #0x1D
	cmp r3, #4
	bgt _0800CBEC
	mov r0, #2
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #0
	beq _0800CBEC
	mov r6, #0
	add r0, r2, #0
	add r0, #0x8A
	ldrh r1, [r0]
	cmp r6, r1
	bge _0800CBEC
	mov r9, r4
	mov r8, r2
	add r2, #0x4A
	str r2, [sp, #0]
	mov sl, r0
_0800CB5A:
	lsl r3, r6, #1
	mov r1, r8
	add r1, #0xA
	add r1, r1, r3
	ldr r4, [sp, #0]
	add r3, r4, r3
	ldrh r0, [r1]
	lsr r2, r0, #8
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	mov r4, #0x94
	add r1, r2, #0
	mul r1, r4
	ldr r2, _0800CC08 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	mov r4, r9
	add r5, r1, r4
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldrb r3, [r3]
	cmp r3, #1
	bne _0800CBE2
	cmp r4, #0
	beq _0800CBE2
	add r1, r5, #0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800CBE2
	mov r0, #0
	ldr r1, _0800CC0C @ =0x00000601
	bl sub_08008524
	cmp r0, #0
	bne _0800CBE2
	mov r0, #1
	ldr r1, _0800CC0C @ =0x00000601
	bl sub_08008524
	cmp r0, #0
	bne _0800CBE2
	mov r0, #3
	ldr r1, _0800CC10 @ =0x0201ADAD
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800CBE2
	ldr r2, _0800CC00 @ =0x000007FF
	add r0, r2, #0
	and r4, r0
	lsl r0, r4, #1
	ldr r4, _0800CC14 @ =0x08622AB4
	add r0, r0, r4
	mov r1, #0xB5
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _0800CBE2
	add r0, r5, #0
	add r0, #0x90
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r7, r0, #0x1B
_0800CBE2:
	add r6, #1
	mov r0, sl
	ldrh r0, [r0]
	cmp r6, r0
	blt _0800CB5A
_0800CBEC:
	add r0, r7, #0
_0800CBEE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800CC00: .4byte 0x000007FF
_0800CC04: .4byte gUnk_08621DE0
_0800CC08: .4byte 0x00000D64
_0800CC0C: .4byte 0x00000601
_0800CC10: .4byte 0x0201ADAD
_0800CC14: .4byte gUnk_08622AB4
	thumb_func_end sub_0800CAF0

