	thumb_func_start sub_08017C48
sub_08017C48: @ 0x08017C48
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	str r0, [sp, #0]
	str r1, [sp, #4]
	mov r0, #0
	str r0, [sp, #8]
	ldr r1, [sp, #0]
	lsl r0, r1, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x14]
_08017C64:
	mov r2, #0
	ldr r4, [sp, #8]
	add r4, #1
	str r4, [sp, #0x18]
	ldr r0, [sp, #8]
	mov sl, r0
	mov r1, sl
	mov r4, #1
	and r1, r4
	mov sl, r1
_08017C78:
	mov r0, #0x94
	add r3, r2, #0
	mul r3, r0
	ldr r1, _08017D30 @ =0x00000D64
	mov r9, r1
	mov r0, sl
	mul r0, r1
	add r0, r3, r0
	ldr r4, _08017D34 @ =0x0201930C
	mov ip, r4
	add r1, r0, r4
	ldr r0, [r1]
	lsl r0, r0, #0x14
	add r2, #1
	str r2, [sp, #0x1C]
	cmp r0, #0
	beq _08017D10
	mov r0, #0
	mov r8, r0
	add r0, r1, #0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r8, r0
	bge _08017D10
	mov r0, #1
	ldr r6, [sp, #0]
	and r6, r0
	ldr r2, [sp, #4]
	mov r4, #0x94
	add r1, r2, #0
	mul r1, r4
	str r1, [sp, #0x10]
	ldr r5, [sp, #8]
	and r5, r0
	str r3, [sp, #0xC]
	ldr r0, [sp, #8]
	lsl r7, r0, #1
	lsl r0, r2, #0x18
	ldr r1, [sp, #0x14]
	lsl r4, r1, #0x10
	orr r4, r0
_08017CCA:
	mov r1, r9
	mul r1, r6
	ldr r2, [sp, #0x10]
	add r1, r2, r1
	add r1, ip
	add r0, r1, #0
	add r0, #0xA
	add r0, r0, r7
	ldrh r2, [r0]
	add r1, #0x4A
	add r1, r1, r7
	ldrb r3, [r1]
	cmp r3, #2
	bgt _08017CF2
	cmp r3, #1
	blt _08017CF2
	ldr r0, [sp, #0]
	lsr r1, r4, #0x10
	bl sub_08017ADC
_08017CF2:
	mov r0, #1
	add r8, r0
	ldr r1, _08017D30 @ =0x00000D64
	mov r9, r1
	mov r0, r9
	mul r0, r5
	ldr r2, [sp, #0xC]
	add r0, r2, r0
	ldr r1, _08017D34 @ =0x0201930C
	mov ip, r1
	add r0, ip
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r8, r0
	blt _08017CCA
_08017D10:
	ldr r2, [sp, #0x1C]
	cmp r2, #0xA
	ble _08017C78
	ldr r2, [sp, #0x18]
	str r2, [sp, #8]
	cmp r2, #1
	ble _08017C64
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08017D30: .4byte 0x00000D64
_08017D34: .4byte 0x0201930C
	thumb_func_end sub_08017C48

