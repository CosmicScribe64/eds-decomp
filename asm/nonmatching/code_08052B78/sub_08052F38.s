	thumb_func_start sub_08052F38
sub_08052F38: @ 0x08052F38
	push {r4, r5, r6, lr}
	sub sp, #0x10
	add r5, r0, #0
	ldr r0, _08052F78 @ =0x03000040
	ldrh r4, [r0, #6]
	ldr r1, _08052F7C @ =0x0201CFB0
	ldr r2, _08052F80 @ =0x00000824
	add r0, r1, r2
	ldr r3, [r0]
	str r3, [sp, #4]
	ldr r6, _08052F84 @ =0x00000828
	add r0, r1, r6
	ldr r2, [r0]
	str r2, [sp, #8]
	add r6, #4
	add r0, r1, r6
	ldr r0, [r0]
	str r0, [sp, #0xC]
	ldr r0, _08052F88 @ =0x00000808
	add r1, r1, r0
	mov r0, #8
	ldrb r6, [r1]
	orr r0, r6
	strb r0, [r1]
	cmp r2, #0xF
	bgt _08052F8C
	cmp r2, #0xC
	blt _08052F8C
	add r0, r3, #0
	mov r1, #0
	mov r2, #0
	b _0805302E
_08052F78: .4byte 0x03000040
_08052F7C: .4byte 0x0201CFB0
_08052F80: .4byte 0x00000824
_08052F84: .4byte 0x00000828
_08052F88: .4byte 0x00000808
_08052F8C:
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	ldr r2, [sp, #0xC]
	add r3, r5, #0
	bl sub_08052908
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08052FB2
	add r2, sp, #8
	add r3, sp, #0xC
	str r5, [sp, #0]
	mov r0, #1
	add r1, sp, #4
	bl sub_08052CE8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08053028
_08052FB2:
	mov r0, #0x40
	and r0, r4
	cmp r0, #0
	beq _08052FCE
	add r2, sp, #8
	add r3, sp, #0xC
	str r5, [sp, #0]
	mov r0, #1
	add r1, sp, #4
	bl sub_08052CE8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08053022
_08052FCE:
	mov r0, #0x80
	and r0, r4
	cmp r0, #0
	beq _08052FEA
	add r2, sp, #8
	add r3, sp, #0xC
	str r5, [sp, #0]
	mov r0, #2
	add r1, sp, #4
	bl sub_08052CE8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08053022
_08052FEA:
	mov r0, #0x20
	and r0, r4
	cmp r0, #0
	beq _08053006
	add r2, sp, #8
	add r3, sp, #0xC
	str r5, [sp, #0]
	mov r0, #8
	add r1, sp, #4
	bl sub_08052CE8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08053022
_08053006:
	mov r0, #0x10
	and r0, r4
	cmp r0, #0
	beq _08053036
	add r2, sp, #8
	add r3, sp, #0xC
	str r5, [sp, #0]
	mov r0, #4
	add r1, sp, #4
	bl sub_08052CE8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08053036
_08053022:
	mov r0, #0
	bl sub_08077AEC
_08053028:
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	ldr r2, [sp, #0xC]
_0805302E:
	bl sub_08024134
	mov r0, #0
	b _08053044
_08053036:
	mov r0, #1
	and r4, r0
	cmp r4, #0
	bne _08053042
	mov r0, #0
	b _08053044
_08053042:
	mov r0, #1
_08053044:
	add sp, #0x10
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08052F38

