	thumb_func_start sub_0801A130
sub_0801A130: @ 0x0801A130
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	mov r2, #0
	ldr r5, _0801A174 @ =0x020192E4
	mov r0, #1
	and r0, r4
	ldr r1, _0801A178 @ =0x00000D64
	mul r1, r0
	add r3, r1, r5
	ldrb r0, [r3, #2]
	cmp r2, r0
	bge _0801A18E
	ldr r7, _0801A17C @ =0x00000684
	add r0, r5, r7
	add r1, r1, r0
_0801A152:
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r5, _0801A180 @ =0x08622AB4
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r6
	bne _0801A184
	add r0, r4, #0
	add r1, r2, #0
	mov r2, #0
	mov r3, #1
	bl sub_080193D4
	mov r0, #1
	b _0801A190
	.align 2, 0
_0801A174: .4byte 0x020192E4
_0801A178: .4byte 0x00000D64
_0801A17C: .4byte 0x00000684
_0801A180: .4byte gUnk_08622AB4
_0801A184:
	add r1, #4
	add r2, #1
	ldrb r7, [r3, #2]
	cmp r2, r7
	blt _0801A152
_0801A18E:
	mov r0, #0
_0801A190:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0801A130
	.align 2, 0

