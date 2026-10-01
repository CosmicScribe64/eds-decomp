	thumb_func_start sub_0800EBF4
sub_0800EBF4: @ 0x0800EBF4
	push {r4, r5, lr}
	ldr r4, _0800EC44 @ =0x020185C0
	ldrh r5, [r4, #4]
	ldrh r0, [r4]
	lsr r2, r0, #0xF
	mov r0, #0x94
	ldrh r3, [r4, #2]
	add r1, r3, #0
	mul r1, r0
	ldr r0, _0800EC48 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0800EC4C @ =0x0201930C
	add r3, r1, r0
	ldr r0, [r3]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800EC30
	ldrb r2, [r3, #6]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	add r1, r5, r1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x3D
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #6]
_0800EC30:
	ldr r0, _0800EC50 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
_0800EC44: .4byte 0x020185C0
_0800EC48: .4byte 0x00000D64
_0800EC4C: .4byte 0x0201930C
_0800EC50: .4byte 0x0000080D
	thumb_func_end sub_0800EBF4

