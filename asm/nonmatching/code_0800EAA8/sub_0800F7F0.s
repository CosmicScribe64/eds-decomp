	thumb_func_start sub_0800F7F0
sub_0800F7F0: @ 0x0800F7F0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0xC
	ldr r5, _0800F830 @ =0x020185C0
	ldrh r0, [r5]
	lsr r6, r0, #0xF
	ldrh r1, [r5, #4]
	lsl r0, r1, #0x10
	ldrh r2, [r5, #2]
	orr r0, r2
	str r0, [sp, #0]
	ldr r0, _0800F834 @ =0x0000080A
	add r7, r5, r0
	ldrb r1, [r7]
	lsl r0, r1, #0x19
	lsr r4, r0, #0x19
	cmp r4, #0
	beq _0800F83C
	cmp r4, #1
	beq _0800F846
	ldr r2, _0800F838 @ =0x00000814
	add r0, r5, r2
	bl sub_080096F4
	add r0, r6, #0
	mov r1, #0xE
	mov r2, #0
	bl sub_08024134
	b _0800F8D4
_0800F830: .4byte 0x020185C0
_0800F834: .4byte 0x0000080A
_0800F838: .4byte 0x00000814
_0800F83C:
	add r0, r6, #0
	mov r1, #0xB
	bl sub_080240A8
	b _0800F8AE
_0800F846:
	add r0, r6, #0
	mov r1, sp
	bl sub_08007F48
	cmp r0, #0
	beq _0800F8D4
	ldr r0, _0800F8C8 @ =0x00000814
	add r0, r0, r5
	mov r9, r0
	mov r1, sp
	bl sub_08007558
	and r6, r4
	mov r8, r6
	mov r6, #2
	neg r6, r6
	ldr r0, [sp, #4]
	and r0, r6
	mov r1, r8
	orr r0, r1
	mov r5, #0x1F
	neg r5, r5
	and r0, r5
	mov r1, #0x1A
	orr r0, r1
	ldr r4, _0800F8CC @ =0xFFFFC01F
	and r0, r4
	ldr r3, _0800F8D0 @ =0xFFFFBFFF
	and r0, r3
	mov r2, #0x80
	lsl r2, r2, #8
	orr r0, r2
	str r0, [sp, #4]
	ldr r0, [sp, #8]
	and r0, r6
	mov r1, r8
	orr r0, r1
	and r0, r5
	mov r1, #0x1C
	orr r0, r1
	and r0, r4
	and r0, r3
	orr r0, r2
	str r0, [sp, #8]
	mov r2, r9
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, sp, #4
	add r2, sp, #8
	bl sub_080242C4
_0800F8AE:
	ldrb r2, [r7]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r7]
	b _0800F8E6
	.align 2, 0
_0800F8C8: .4byte 0x00000814
_0800F8CC: .4byte 0xFFFFC01F
_0800F8D0: .4byte 0xFFFFBFFF
_0800F8D4:
	bl sub_080611AC
	ldr r0, _0800F8F4 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0800F8E6:
	add sp, #0xC
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800F8F4: .4byte 0x0000080D
	thumb_func_end sub_0800F7F0

