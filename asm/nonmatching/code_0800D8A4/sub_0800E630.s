	thumb_func_start sub_0800E630
sub_0800E630: @ 0x0800E630
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	ldr r0, _0800E6E8 @ =0x020185C0
	mov r9, r0
	ldrb r7, [r0, #2]
	ldrh r1, [r0, #2]
	lsr r1, r1, #8
	mov sl, r1
	ldrb r2, [r0, #4]
	str r2, [sp, #8]
	ldrh r3, [r0, #4]
	lsr r3, r3, #8
	str r3, [sp, #0xC]
	ldr r4, _0800E6EC @ =0x02018DCA
	ldrb r4, [r4]
	lsl r0, r4, #0x19
	lsr r6, r0, #0x19
	cmp r6, #0
	beq _0800E700
	cmp r6, #1
	bne _0800E664
	b _0800E76C
_0800E664:
	mov r2, #1
	ldr r0, [sp, #8]
	and r0, r2
	ldr r1, _0800E6F0 @ =0x00000D64
	add r3, r0, #0
	mul r3, r1
	str r3, [sp, #0x14]
	ldr r4, _0800E6F4 @ =0x0201930C
	add r4, r3, r4
	str r4, [sp, #0x18]
	mov r0, #0x94
	ldr r4, [sp, #0xC]
	add r3, r4, #0
	mul r3, r0
	mov r8, r3
	ldr r5, [sp, #0x18]
	add r5, r8
	and r7, r2
	add r4, r7, #0
	mul r4, r1
	ldr r1, _0800E6F4 @ =0x0201930C
	add r4, r4, r1
	mov r2, sl
	mul r2, r0
	add r0, r2, #0
	add r4, r4, r0
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0x94
	bl sub_08075294
	ldr r1, _0800E6F8 @ =0x00000814
	add r1, r9
	add r0, r5, #0
	bl sub_08007558
	add r0, r4, #0
	mov r1, #0x94
	bl sub_08075278
	ldr r3, [sp, #0x14]
	add r3, r8
	ldr r4, _0800E6F4 @ =0x0201930C
	add r4, r3, r4
	str r4, [sp, #0x20]
	mov r0, #5
	neg r0, r0
	ldrb r1, [r4, #7]
	and r0, r1
	strb r0, [r4, #7]
	ldr r0, [sp, #8]
	mov r1, #0
	ldr r2, [sp, #0xC]
	bl sub_08024134
	bl sub_080611AC
	ldr r1, _0800E6FC @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	b _0800E846
	.align 2, 0
_0800E6E8: .4byte 0x020185C0
_0800E6EC: .4byte 0x02018DCA
_0800E6F0: .4byte 0x00000D64
_0800E6F4: .4byte 0x0201930C
_0800E6F8: .4byte 0x00000814
_0800E6FC: .4byte 0x0000080D
_0800E700:
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mov r3, sl
	mul r3, r0
	add r0, r3, #0
	ldr r1, _0800E730 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0800E734 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0800E73C
	ldr r1, _0800E738 @ =0x0000080D
	add r1, r9
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	b _0800E846
	.align 2, 0
_0800E730: .4byte 0x00000D64
_0800E734: .4byte 0x0201930C
_0800E738: .4byte 0x0000080D
_0800E73C:
	mov r0, sl
	bl sub_08062354
	add r1, r0, #0
	add r0, r7, #0
	bl sub_080240A8
	ldr r0, _0800E768 @ =0x02018DCA
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
	ldr r1, _0800E768 @ =0x02018DCA
	strb r0, [r1]
	b _0800E846
	.align 2, 0
_0800E768: .4byte 0x02018DCA
_0800E76C:
	add r0, r7, #0
	and r0, r6
	ldr r1, _0800E858 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	str r2, [sp, #0x10]
	ldr r3, _0800E85C @ =0x0201930C
	add r1, r2, r3
	mov r0, #0x94
	mov r4, sl
	mul r4, r0
	add r1, r1, r4
	ldr r0, _0800E860 @ =0x02018DD4
	bl sub_08007558
	add r0, r7, #0
	mov r1, sl
	bl sub_08060FD0
	and r7, r6
	ldr r1, [sp, #0]
	mov r0, #2
	neg r0, r0
	and r1, r0
	orr r1, r7
	mov r2, #0x1F
	neg r2, r2
	mov ip, r2
	and r1, r2
	mov r3, sl
	lsl r0, r3, #5
	ldr r7, _0800E864 @ =0xFFFFC01F
	and r1, r7
	orr r1, r0
	str r1, [sp, #0]
	ldr r0, [sp, #0x10]
	add r4, r4, r0
	ldr r2, _0800E85C @ =0x0201930C
	add r4, r4, r2
	ldrb r3, [r4, #6]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	ldr r5, _0800E868 @ =0xFFFFBFFF
	add r2, r5, #0
	and r2, r1
	orr r2, r0
	str r2, [sp, #0]
	ldrb r4, [r4, #6]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xF
	ldr r3, _0800E86C @ =0xFFFF7FFF
	add r4, r3, #0
	and r4, r2
	orr r4, r0
	str r4, [sp, #0]
	ldr r0, [sp, #8]
	and r0, r6
	ldr r1, [sp, #4]
	mov r2, #2
	neg r2, r2
	and r1, r2
	orr r1, r0
	mov r0, ip
	and r1, r0
	ldr r2, [sp, #0xC]
	lsl r0, r2, #5
	and r1, r7
	orr r1, r0
	lsl r0, r4, #0x11
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xE
	and r1, r5
	orr r1, r0
	lsl r0, r4, #0x10
	lsr r0, r0, #0x1F
	and r0, r6
	lsl r0, r0, #0xF
	and r1, r3
	orr r1, r0
	str r1, [sp, #4]
	cmp r2, #4
	ble _0800E81E
	and r4, r5
	str r4, [sp, #0]
_0800E81E:
	ldr r3, _0800E860 @ =0x02018DD4
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r2, sp, #4
	mov r1, sp
	bl sub_080242C4
	ldr r4, _0800E870 @ =0x02018DCA
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
_0800E846:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0800E858: .4byte 0x00000D64
_0800E85C: .4byte 0x0201930C
_0800E860: .4byte 0x02018DD4
_0800E864: .4byte 0xFFFFC01F
_0800E868: .4byte 0xFFFFBFFF
_0800E86C: .4byte 0xFFFF7FFF
_0800E870: .4byte 0x02018DCA
	thumb_func_end sub_0800E630

