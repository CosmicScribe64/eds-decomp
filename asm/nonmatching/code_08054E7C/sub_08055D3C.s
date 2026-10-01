	thumb_func_start sub_08055D3C
sub_08055D3C: @ 0x08055D3C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r8, r1
	lsl r3, r3, #0x10
	lsr r6, r3, #0x10
	ldr r4, _08055E14 @ =0x0201CF90
	mov r0, #1
	mov r1, r9
	and r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	orr r0, r1
	mov r1, #0x1F
	and r2, r1
	lsl r2, r2, #1
	mov r1, #0x3F
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r4]
	mov r0, #0xFF
	mov r1, r8
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _08055E18 @ =0xFFFFC03F
	ldrh r7, [r4]
	and r0, r7
	orr r0, r1
	strh r0, [r4]
	mov r0, #0x41
	neg r0, r0
	ldrb r1, [r4, #1]
	and r0, r1
	strb r0, [r4, #1]
	ldr r5, _08055E1C @ =0x0000047F
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08055DA4
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	beq _08055DAC
_08055DA4:
	mov r0, #0x40
	ldrb r2, [r4, #1]
	orr r0, r2
	strb r0, [r4, #1]
_08055DAC:
	cmp r6, #0
	beq _08055E20
	lsl r3, r6, #0x18
	lsr r2, r6, #8
	lsl r4, r2, #0x18
	ldr r6, _08055E14 @ =0x0201CF90
	mov r5, #7
	lsr r1, r3, #0x18
	and r1, r5
	mov r0, #8
	neg r0, r0
	ldrb r7, [r6, #2]
	and r0, r7
	orr r0, r1
	and r2, r5
	lsl r2, r2, #3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r6, #2]
	lsr r1, r3, #0x1F
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	ldrb r2, [r6, #3]
	and r0, r2
	orr r0, r1
	mov r5, #1
	lsr r1, r4, #0x1F
	lsl r1, r1, #2
	mov r2, #5
	neg r2, r2
	and r0, r2
	orr r0, r1
	lsr r3, r3, #0x1C
	and r3, r5
	and r3, r5
	lsl r3, r3, #4
	mov r1, #0x11
	neg r1, r1
	and r0, r1
	orr r0, r3
	lsr r4, r4, #0x1C
	and r4, r5
	and r4, r5
	lsl r4, r4, #5
	sub r1, #0x10
	and r0, r1
	orr r0, r4
	strb r0, [r6, #3]
	b _08055E42
_08055E14: .4byte 0x0201CF90
_08055E18: .4byte 0xFFFFC03F
_08055E1C: .4byte 0x0000047F
_08055E20:
	ldr r2, _08055EA0 @ =0x0201CF90
	mov r0, #8
	neg r0, r0
	ldrb r3, [r2, #2]
	and r0, r3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r2, #2]
	mov r0, #3
	neg r0, r0
	ldrb r7, [r2, #3]
	and r0, r7
	add r1, #0x34
	and r0, r1
	strb r0, [r2, #3]
	add r6, r2, #0
_08055E42:
	mov r3, #1
	mov r0, r9
	and r0, r3
	mov r2, r8
	lsl r1, r2, #2
	ldr r2, _08055EA4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08055EA8 @ =0x02019968
	add r1, r1, r0
	ldr r2, [r1]
	lsl r2, r2, #0x14
	lsr r1, r2, #0x14
	and r1, r3
	lsl r1, r1, #7
	mov r0, #0x7F
	ldrb r3, [r6, #3]
	and r0, r3
	orr r0, r1
	strb r0, [r6, #3]
	lsr r2, r2, #0x15
	ldr r0, _08055EAC @ =0xFFFF8000
	ldrh r7, [r6, #4]
	and r0, r7
	orr r0, r2
	strh r0, [r6, #4]
	mov r0, #0x1D
	neg r0, r0
	ldrb r1, [r6, #0xE]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r6, #0xE]
	mov r0, #3
	strh r0, [r6, #0xC]
	mov r0, r9
	bl sub_08046A74
	bl sub_08055A00
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08055EA0: .4byte 0x0201CF90
_08055EA4: .4byte 0x00000D64
_08055EA8: .4byte 0x02019968
_08055EAC: .4byte 0xFFFF8000
	thumb_func_end sub_08055D3C

