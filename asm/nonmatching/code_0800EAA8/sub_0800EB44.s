	thumb_func_start sub_0800EB44
sub_0800EB44: @ 0x0800EB44
	push {r4, lr}
	ldr r4, _0800EB8C @ =0x020185C0
	ldrh r0, [r4]
	lsr r1, r0, #0xF
	mov r0, #0x94
	ldrb r2, [r4, #2]
	add r3, r2, #0
	mul r3, r0
	ldr r0, _0800EB90 @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _0800EB94 @ =0x0201930C
	add r3, r3, r0
	add r3, #0x90
	mov r1, #0x1F
	ldrh r0, [r4, #4]
	and r1, r0
	lsl r1, r1, #0xD
	ldr r0, [r3]
	ldr r2, _0800EB98 @ =0xFFFC1FFF
	and r0, r2
	orr r0, r1
	str r0, [r3]
	bl sub_080611AC
	ldr r1, _0800EB9C @ =0x0000080D
	add r4, r4, r1
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800EB8C: .4byte 0x020185C0
_0800EB90: .4byte 0x00000D64
_0800EB94: .4byte 0x0201930C
_0800EB98: .4byte 0xFFFC1FFF
_0800EB9C: .4byte 0x0000080D
	thumb_func_end sub_0800EB44

