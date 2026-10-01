	thumb_func_start sub_0800ED58
sub_0800ED58: @ 0x0800ED58
	push {r4, r5, r6, lr}
	ldr r4, _0800EDBC @ =0x020185C0
	ldrh r0, [r4]
	lsr r1, r0, #0xF
	ldr r0, _0800EDC0 @ =0x00000D64
	mul r1, r0
	ldr r0, _0800EDC4 @ =0x0201930C
	add r1, r1, r0
	mov r2, #0x94
	ldrh r3, [r4, #2]
	add r0, r3, #0
	mul r0, r2
	add r5, r1, r0
	ldrh r3, [r4, #4]
	add r0, r3, #0
	mul r0, r2
	add r6, r1, r0
	add r0, r6, #0
	add r0, #0xA
	add r1, r5, #0
	add r1, #0xA
	mov r2, #0x40
	bl sub_08075294
	add r0, r6, #0
	add r0, #0x4A
	add r1, r5, #0
	add r1, #0x4A
	mov r2, #0x40
	bl sub_08075294
	add r3, r5, #0
	add r3, #0x8A
	ldrh r2, [r3]
	add r0, r6, #0
	add r0, #0x8A
	mov r1, #0
	strh r2, [r0]
	strh r1, [r3]
	ldr r0, _0800EDC8 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800EDBC: .4byte 0x020185C0
_0800EDC0: .4byte 0x00000D64
_0800EDC4: .4byte 0x0201930C
_0800EDC8: .4byte 0x0000080D
	thumb_func_end sub_0800ED58

