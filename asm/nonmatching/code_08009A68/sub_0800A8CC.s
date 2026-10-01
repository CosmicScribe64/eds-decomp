	thumb_func_start sub_0800A8CC
sub_0800A8CC: @ 0x0800A8CC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	mov r2, #0
	mov ip, r2
	mov r8, r2
	mov r2, #1
	and r2, r0
	mov r0, #0x94
	mul r1, r0
	ldr r6, _0800A964 @ =0x00000D64
	mul r2, r6
	add r0, r1, r2
	ldr r5, _0800A968 @ =0x0201930C
	add r3, r0, r5
	add r0, r3, #0
	add r0, #0x8A
	ldrh r4, [r0]
	cmp ip, r4
	bge _0800A9B0
	str r3, [sp, #4]
	add r0, r2, #0
	add r0, #0x4A
	add r0, r1, r0
	add r3, r5, #0
	add r7, r0, r3
	ldr r0, _0800A96C @ =0x000007FF
	mov sl, r0
	str r4, [sp, #0]
_0800A912:
	mov r2, r8
	lsl r1, r2, #1
	ldr r0, [sp, #4]
	add r0, #0xA
	add r0, r0, r1
	ldrh r0, [r0]
	add r2, r0, #0
	ldrb r4, [r7]
	add r6, r4, #0
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	lsr r1, r2, #8
	mov r3, #1
	and r0, r3
	mov r3, #0x94
	mul r1, r3
	ldr r3, _0800A964 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _0800A968 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	mov r5, #1
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0800A952
	mov r5, #0
_0800A952:
	cmp r5, #0
	beq _0800A9A4
	cmp r4, #3
	beq _0800A990
	cmp r4, #3
	bgt _0800A970
	cmp r4, #1
	blt _0800A9A4
	b _0800A974
_0800A964: .4byte 0x00000D64
_0800A968: .4byte 0x0201930C
_0800A96C: .4byte 0x000007FF
_0800A970:
	cmp r6, #0xA
	bne _0800A9A4
_0800A974:
	mov r1, sl
	and r3, r1
	lsl r0, r3, #1
	ldr r2, _0800A98C @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r9
	bne _0800A9A4
	mov r3, #1
	add ip, r3
	b _0800A9A4
	.align 2, 0
_0800A98C: .4byte gUnk_08622AB4
_0800A990:
	mov r0, sl
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0800A9C4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r9
	bne _0800A9A4
	mov r2, #1
	add ip, r2
_0800A9A4:
	add r7, #2
	mov r3, #1
	add r8, r3
	ldr r0, [sp, #0]
	cmp r8, r0
	blt _0800A912
_0800A9B0:
	mov r0, ip
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0800A9C4: .4byte gUnk_08622AB4
	thumb_func_end sub_0800A8CC

