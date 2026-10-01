	thumb_func_start sub_0802FD90
sub_0802FD90: @ 0x0802FD90
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	str r0, [sp, #0]
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _0802FDAA
	b _0802FE52
_0802FDA6:
	mov r0, #1
	b _0802FE54
_0802FDAA:
	mov r0, #0
	str r0, [sp, #4]
_0802FDAE:
	mov r1, #0
	mov sl, r1
	ldr r2, [sp, #4]
	add r0, r2, #0
	mov r1, #1
	and r0, r1
	str r0, [sp, #8]
_0802FDBC:
	mov r0, #0x94
	mov r1, sl
	mul r1, r0
	ldr r0, _0802FE64 @ =0x00000D64
	ldr r2, [sp, #8]
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802FE68 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802FE3E
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802FE3E
	mov r6, #0
	ldr r1, [sp, #4]
	lsl r0, r1, #0x18
	mov r2, sl
	lsl r1, r2, #0x18
	lsr r7, r0, #8
	orr r7, r1
_0802FDEE:
	mov r5, #5
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	mov r9, r0
	lsr r0, r7, #0x10
	mov r8, r0
_0802FDFA:
	lsl r1, r5, #0x18
	lsr r1, r1, #0x10
	mov r2, r9
	orr r1, r2
	ldr r0, [sp, #0]
	bl sub_0802C080
	neg r1, r0
	orr r1, r0
	lsr r4, r1, #0x1F
	add r0, r6, #0
	add r1, r5, #0
	ldr r2, [sp, #4]
	mov r3, sl
	bl sub_0800CCCC
	cmp r0, #0
	bne _0802FE20
	mov r4, #0
_0802FE20:
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0800CD68
	cmp r0, r8
	bne _0802FE2E
	mov r4, #0
_0802FE2E:
	cmp r4, #0
	bne _0802FDA6
	add r5, #1
	cmp r5, #9
	ble _0802FDFA
	add r6, #1
	cmp r6, #1
	ble _0802FDEE
_0802FE3E:
	mov r0, #1
	add sl, r0
	mov r1, sl
	cmp r1, #4
	ble _0802FDBC
	ldr r2, [sp, #4]
	add r2, #1
	str r2, [sp, #4]
	cmp r2, #1
	ble _0802FDAE
_0802FE52:
	mov r0, #0
_0802FE54:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0802FE64: .4byte 0x00000D64
_0802FE68: .4byte 0x0201930C
	thumb_func_end sub_0802FD90

