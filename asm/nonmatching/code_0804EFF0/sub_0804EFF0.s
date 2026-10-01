	thumb_func_start sub_0804EFF0
sub_0804EFF0: @ 0x0804EFF0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	str r0, [sp, #0]
	mov r0, #0
	mov r8, r0
	mov r0, #1
	ldr r1, [sp, #0]
	and r0, r1
	ldr r4, _0804F06C @ =0x00000D64
	add r2, r0, #0
	mul r2, r4
	str r2, [sp, #4]
_0804F010:
	mov r0, #0x94
	mov r5, r8
	mul r5, r0
	add r0, r5, #0
	ldr r1, [sp, #4]
	add r0, r0, r1
	ldr r1, _0804F070 @ =0x0201930C
	add r3, r0, r1
	ldr r0, [r3]
	lsl r0, r0, #0x14
	mov r7, r8
	add r7, #1
	cmp r0, #0
	bne _0804F02E
	b _0804F144
_0804F02E:
	mov r0, #2
	ldrb r2, [r3, #6]
	and r0, r2
	cmp r0, #0
	bne _0804F03A
	b _0804F144
_0804F03A:
	mov r4, #0
	add r2, r3, #0
	add r2, #0x8C
	ldrb r1, [r2]
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _0804F054
	mov r5, #0x11
	neg r5, r5
	add r0, r5, #0
	and r0, r1
	strb r0, [r2]
_0804F054:
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804F074 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0x52
	beq _0804F078
	cmp r0, #0x62
	beq _0804F084
	b _0804F096
	.align 2, 0
_0804F06C: .4byte 0x00000D64
_0804F070: .4byte 0x0201930C
_0804F074: .4byte gUnk_08622AB4
_0804F078:
	mov r0, #0x20
	ldrb r3, [r3, #7]
	and r0, r3
	cmp r0, #0
	bne _0804F096
	b _0804F09E
_0804F084:
	ldrb r3, [r3, #6]
	lsl r0, r3, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, #3
	bhi _0804F096
	ldr r0, [sp, #0]
	mov r1, r8
	bl sub_08046738
_0804F096:
	mov r7, r8
	add r7, #1
	cmp r4, #0
	beq _0804F144
_0804F09E:
	mov r0, #0x94
	mov r2, r8
	mul r2, r0
	ldr r4, [sp, #4]
	add r0, r2, r4
	ldr r3, _0804F160 @ =0x0201930C
	add r0, r0, r3
	ldrb r0, [r0, #6]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1C
	mov r7, r8
	add r7, #1
	cmp r0, #5
	bhi _0804F144
	mov r6, #0
	ldr r0, _0804F164 @ =0x00000D64
	ldr r1, [sp, #0]
	mov r5, #1
	and r1, r5
	mul r0, r1
	add r2, r2, r0
	str r2, [sp, #8]
_0804F0CA:
	mov r5, #0
	add r0, r6, #1
	str r0, [sp, #0xC]
	add r0, r6, #0
	mov r1, #1
	and r0, r1
	ldr r2, _0804F164 @ =0x00000D64
	add r1, r0, #0
	mul r1, r2
	ldr r4, [sp, #8]
	ldr r0, _0804F160 @ =0x0201930C
	add r4, r4, r0
	mov sl, r4
	ldr r2, [sp, #0]
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	mov r9, r0
	ldr r0, _0804F160 @ =0x0201930C
	add r4, r1, r0
_0804F0F0:
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804F136
	mov r0, #2
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _0804F136
	add r0, r6, #0
	add r1, r5, #0
	bl sub_0800C8BC
	cmp r0, #2
	bne _0804F136
	ldrh r0, [r4, #4]
	mov r2, sl
	ldrh r2, [r2, #4]
	cmp r0, r2
	bhi _0804F136
	mov r1, r8
	lsl r0, r1, #0x18
	mov r2, r9
	lsl r1, r2, #0x10
	orr r1, r0
	lsl r2, r6, #0x18
	lsl r0, r5, #0x18
	lsr r2, r2, #8
	orr r2, r0
	lsr r2, r2, #0x10
	ldr r0, [sp, #0]
	lsr r1, r1, #0x10
	mov r3, #2
	bl sub_08017AB4
_0804F136:
	add r4, #0x94
	add r5, #1
	cmp r5, #4
	ble _0804F0F0
	ldr r6, [sp, #0xC]
	cmp r6, #1
	ble _0804F0CA
_0804F144:
	mov r8, r7
	mov r4, r8
	cmp r4, #4
	bgt _0804F14E
	b _0804F010
_0804F14E:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0804F160: .4byte 0x0201930C
_0804F164: .4byte 0x00000D64
	thumb_func_end sub_0804EFF0

