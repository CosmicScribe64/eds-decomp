	thumb_func_start sub_0800EC54
sub_0800EC54: @ 0x0800EC54
	push {r4, r5, lr}
	ldr r5, _0800ECA0 @ =0x020185C0
	ldrh r3, [r5, #4]
	ldrh r0, [r5]
	lsr r2, r0, #0xF
	mov r0, #0x94
	ldrh r4, [r5, #2]
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0800ECA4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0800ECA8 @ =0x0201930C
	add r2, r1, r0
	ldrb r4, [r2, #6]
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800EC8A
	mov r0, #0xF
	and r3, r0
	lsl r1, r3, #2
	mov r0, #0x3D
	neg r0, r0
	and r0, r4
	orr r0, r1
	strb r0, [r2, #6]
_0800EC8A:
	ldr r0, _0800ECAC @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0800ECA0: .4byte 0x020185C0
_0800ECA4: .4byte 0x00000D64
_0800ECA8: .4byte 0x0201930C
_0800ECAC: .4byte 0x0000080D
	thumb_func_end sub_0800EC54

