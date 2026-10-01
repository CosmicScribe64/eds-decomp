	thumb_func_start sub_0800ECB0
sub_0800ECB0: @ 0x0800ECB0
	push {r4, lr}
	ldr r4, _0800ED08 @ =0x020185C0
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	mov r0, #0x94
	ldrh r3, [r4, #2]
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0800ED0C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0800ED10 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800ECF2
	mov r0, #0x3D
	neg r0, r0
	ldrb r2, [r1, #6]
	and r0, r2
	strb r0, [r1, #6]
	add r3, r1, #0
	add r3, #0x90
	mov r1, #0x1F
	ldrh r0, [r4, #4]
	and r1, r0
	lsl r1, r1, #0xD
	ldr r0, [r3]
	ldr r2, _0800ED14 @ =0xFFFC1FFF
	and r0, r2
	orr r0, r1
	str r0, [r3]
_0800ECF2:
	ldr r2, _0800ED18 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800ED08: .4byte 0x020185C0
_0800ED0C: .4byte 0x00000D64
_0800ED10: .4byte 0x0201930C
_0800ED14: .4byte 0xFFFC1FFF
_0800ED18: .4byte 0x0000080D
	thumb_func_end sub_0800ECB0

