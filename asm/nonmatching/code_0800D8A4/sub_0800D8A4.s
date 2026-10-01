	thumb_func_start sub_0800D8A4
sub_0800D8A4: @ 0x0800D8A4
	push {r4, r5, r6, r7, lr}
	ldr r2, _0800D8D0 @ =0x020185C0
	ldrh r0, [r2]
	lsr r4, r0, #0xF
	ldrh r5, [r2, #2]
	ldrh r7, [r2, #4]
	ldr r0, _0800D8D4 @ =0x00000D64
	add r1, r4, #0
	mul r1, r0
	ldr r0, _0800D8D8 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x94
	mul r0, r5
	add r3, r1, r0
	ldr r0, [r3]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800D8E0
	ldr r0, _0800D8DC @ =0x0000080D
	add r1, r2, r0
	b _0800D978
	.align 2, 0
_0800D8D0: .4byte 0x020185C0
_0800D8D4: .4byte 0x00000D64
_0800D8D8: .4byte 0x0201930C
_0800D8DC: .4byte 0x0000080D
_0800D8E0:
	ldr r0, _0800D918 @ =0x0000080A
	add r6, r2, r0
	ldrb r1, [r6]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _0800D91C
	cmp r0, #1
	beq _0800D92C
	ldrb r2, [r3, #6]
	mov r1, #1
	add r0, r2, #0
	eor r0, r1
	and r0, r1
	mov r1, #2
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3, #6]
	cmp r7, #0
	beq _0800D964
	add r2, r1, #0
	mov r0, #2
	and r1, r0
	cmp r1, #0
	bne _0800D964
	b _0800D95E
	.align 2, 0
_0800D918: .4byte 0x0000080A
_0800D91C:
	add r0, r5, #0
	bl sub_08062354
	add r1, r0, #0
	add r0, r4, #0
	bl sub_080240A8
	b _0800D946
_0800D92C:
	lsl r1, r5, #0x18
	lsr r1, r1, #0x10
	orr r1, r4
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r7, #0x18
	lsl r0, r0, #0x10
	orr r0, r2
	orr r1, r0
	mov r0, #1
	bl sub_08024288
_0800D946:
	ldrb r2, [r6]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	b _0800D982
_0800D95E:
	mov r0, #2
	orr r0, r2
	strb r0, [r3, #6]
_0800D964:
	bl sub_080611AC
	add r0, r4, #0
	mov r1, #0
	add r2, r5, #0
	bl sub_08024134
	ldr r1, _0800D988 @ =0x020185C0
	ldr r2, _0800D98C @ =0x0000080D
	add r1, r1, r2
_0800D978:
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800D982:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800D988: .4byte 0x020185C0
_0800D98C: .4byte 0x0000080D
	thumb_func_end sub_0800D8A4

