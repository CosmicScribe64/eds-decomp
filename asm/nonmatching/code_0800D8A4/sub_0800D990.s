	thumb_func_start sub_0800D990
sub_0800D990: @ 0x0800D990
	push {r4, r5, r6, r7, lr}
	ldr r4, _0800D9BC @ =0x020185C0
	ldrh r0, [r4]
	lsr r5, r0, #0xF
	ldrh r6, [r4, #2]
	ldr r0, _0800D9C0 @ =0x00000D64
	add r2, r5, #0
	mul r2, r0
	ldr r7, _0800D9C4 @ =0x0201930C
	add r1, r2, r7
	mov r0, #0x94
	mul r0, r6
	add r3, r1, r0
	add r0, r0, r2
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800D9CC
	ldr r2, _0800D9C8 @ =0x0000080D
	add r1, r4, r2
	b _0800DA6A
_0800D9BC: .4byte 0x020185C0
_0800D9C0: .4byte 0x00000D64
_0800D9C4: .4byte 0x0201930C
_0800D9C8: .4byte 0x0000080D
_0800D9CC:
	ldr r0, _0800DA0C @ =0x0000080A
	add r4, r4, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _0800DA10
	cmp r0, #1
	beq _0800DA20
	ldrb r2, [r3, #6]
	lsr r1, r2, #1
	mov r0, #1
	eor r1, r0
	and r1, r0
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #6]
	mov r1, #2
	and r0, r1
	cmp r0, #0
	beq _0800DA56
	add r0, r7, #0
	sub r0, #0x2C
	ldrh r2, [r0]
	add r1, r2, #1
	strh r1, [r0]
	strh r2, [r3, #4]
	b _0800DA56
	.align 2, 0
_0800DA0C: .4byte 0x0000080A
_0800DA10:
	add r0, r6, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r5, #0
	bl sub_080240A8
	b _0800DA3E
_0800DA20:
	lsl r1, r6, #0x18
	lsr r1, r1, #0x10
	orr r1, r5
	ldrb r0, [r3, #6]
	lsl r2, r0, #0x1F
	lsr r2, r2, #0x1F
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	lsl r0, r0, #8
	orr r2, r0
	lsl r2, r2, #0x10
	orr r1, r2
	mov r0, #2
	bl sub_08024288
_0800DA3E:
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _0800DA74
_0800DA56:
	add r0, r5, #0
	mov r1, #0
	add r2, r6, #0
	bl sub_08024134
	bl sub_080611AC
	ldr r1, _0800DA7C @ =0x020185C0
	ldr r2, _0800DA80 @ =0x0000080D
	add r1, r1, r2
_0800DA6A:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800DA74:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800DA7C: .4byte 0x020185C0
_0800DA80: .4byte 0x0000080D
	thumb_func_end sub_0800D990

