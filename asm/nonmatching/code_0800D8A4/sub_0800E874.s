	thumb_func_start sub_0800E874
sub_0800E874: @ 0x0800E874
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xA0
	ldr r0, _0800E8FC @ =0x020185C0
	mov r9, r0
	ldrb r7, [r0, #2]
	ldrh r1, [r0, #2]
	lsr r1, r1, #8
	mov r8, r1
	ldrb r2, [r0, #4]
	mov sl, r2
	ldrh r1, [r0, #4]
	lsr r0, r1, #8
	str r0, [sp, #0x9C]
	ldr r2, _0800E900 @ =0x02018DCA
	ldrb r2, [r2]
	lsl r0, r2, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _0800E90C
	cmp r6, #1
	beq _0800E98C
	mov r0, sl
	mov r1, #1
	and r0, r1
	mov sl, r0
	ldr r4, _0800E904 @ =0x00000D64
	mov r5, sl
	mul r5, r4
	ldr r2, _0800E908 @ =0x0201930C
	add r5, r5, r2
	mov r0, #0x94
	mov sl, r0
	ldr r1, [sp, #0x9C]
	mov r0, sl
	mul r0, r1
	add r5, r5, r0
	mov r0, sp
	add r1, r5, #0
	mov r2, #0x94
	bl sub_08075294
	mov r2, #1
	and r7, r2
	mul r4, r7
	ldr r0, _0800E908 @ =0x0201930C
	add r4, r4, r0
	mov r1, sl
	mov r0, r8
	mul r0, r1
	add r4, r4, r0
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0x94
	bl sub_08075294
	add r0, r4, #0
	mov r1, sp
	mov r2, #0x94
	bl sub_08075294
	bl sub_080611AC
	b _0800E946
	.align 2, 0
_0800E8FC: .4byte 0x020185C0
_0800E900: .4byte 0x02018DCA
_0800E904: .4byte 0x00000D64
_0800E908: .4byte 0x0201930C
_0800E90C:
	mov r4, #1
	and r7, r4
	mov r3, #0x94
	mov r0, r8
	mul r0, r3
	ldr r2, _0800E958 @ =0x00000D64
	add r1, r7, #0
	mul r1, r2
	add r0, r0, r1
	ldr r5, _0800E95C @ =0x0201930C
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0800E946
	mov r0, sl
	and r0, r4
	mov sl, r0
	ldr r1, [sp, #0x9C]
	add r0, r1, #0
	mul r0, r3
	mov r1, sl
	mul r1, r2
	add r0, r0, r1
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800E964
_0800E946:
	ldr r1, _0800E960 @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0800EA7C
	.align 2, 0
_0800E958: .4byte 0x00000D64
_0800E95C: .4byte 0x0201930C
_0800E960: .4byte 0x0000080D
_0800E964:
	mov r0, #0x50
	bl sub_0802408C
	ldr r0, _0800E988 @ =0x02018DCA
	ldrb r2, [r0]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	ldr r1, _0800E988 @ =0x02018DCA
	strb r0, [r1]
	b _0800EA7C
	.align 2, 0
_0800E988: .4byte 0x02018DCA
_0800E98C:
	ldr r0, _0800EA8C @ =0x00000814
	add r0, r9
	add r1, r7, #0
	and r1, r6
	ldr r2, _0800EA90 @ =0x00000D64
	add r5, r1, #0
	mul r5, r2
	ldr r2, _0800EA94 @ =0x0201930C
	add r1, r5, r2
	mov r2, #0x94
	mov r4, r8
	mul r4, r2
	add r1, r1, r4
	bl sub_08007558
	add r0, r7, #0
	mov r1, r8
	bl sub_08060FD0
	mov r0, sl
	ldr r1, [sp, #0x9C]
	bl sub_08060FD0
	and r7, r6
	add r3, sp, #0x94
	ldr r2, [r3]
	mov r0, #2
	neg r0, r0
	and r2, r0
	orr r2, r7
	mov r1, #0x1F
	neg r1, r1
	and r2, r1
	mov r1, r8
	lsl r0, r1, #5
	ldr r1, _0800EA98 @ =0xFFFFC01F
	mov ip, r1
	and r2, r1
	orr r2, r0
	str r2, [r3]
	add r4, r4, r5
	ldr r0, _0800EA94 @ =0x0201930C
	add r4, r4, r0
	ldrb r1, [r4, #6]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	ldr r5, _0800EA9C @ =0xFFFFBFFF
	add r1, r5, #0
	and r1, r2
	orr r1, r0
	str r1, [r3]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xF
	ldr r7, _0800EAA0 @ =0xFFFF7FFF
	and r1, r7
	orr r1, r0
	str r1, [r3]
	mov r2, sl
	and r2, r6
	mov sl, r2
	add r4, sp, #0x98
	ldr r2, [r4]
	mov r0, #2
	neg r0, r0
	and r2, r0
	mov r1, sl
	orr r2, r1
	sub r0, #0x1D
	and r2, r0
	ldr r1, [sp, #0x9C]
	lsl r0, r1, #5
	mov r1, ip
	and r2, r1
	orr r2, r0
	str r2, [r4]
	ldr r0, [r3]
	lsl r0, r0, #0x11
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	add r1, r5, #0
	and r1, r2
	orr r1, r0
	str r1, [r4]
	ldrb r2, [r3, #1]
	lsr r0, r2, #7
	and r6, r0
	lsl r0, r6, #0xF
	and r1, r7
	orr r1, r0
	str r1, [r4]
	add r1, r3, #0
	ldr r0, [sp, #0x9C]
	cmp r0, #4
	ble _0800EA5A
	ldr r0, [r1]
	and r0, r5
	str r0, [r1]
_0800EA5A:
	add r0, r3, #0
	add r1, r4, #0
	bl sub_0802432C
	ldr r1, _0800EAA4 @ =0x02018DCA
	ldrb r2, [r1]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	ldr r2, _0800EAA4 @ =0x02018DCA
	strb r0, [r2]
_0800EA7C:
	add sp, #0xA0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800EA8C: .4byte 0x00000814
_0800EA90: .4byte 0x00000D64
_0800EA94: .4byte 0x0201930C
_0800EA98: .4byte 0xFFFFC01F
_0800EA9C: .4byte 0xFFFFBFFF
_0800EAA0: .4byte 0xFFFF7FFF
_0800EAA4: .4byte 0x02018DCA
	thumb_func_end sub_0800E874

