	thumb_func_start sub_0805FD28
sub_0805FD28: @ 0x0805FD28
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r2, #0
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	mov r2, #1
	mov r8, r2
	mov r2, #0
	mov r9, r2
	mov r6, #0
	ldr r5, _0805FDBC @ =0x0201AE60
	lsl r2, r0, #8
	lsr r2, r2, #0x18
	strh r2, [r5, #8]
	lsr r0, r0, #0x18
	strh r0, [r5, #0xA]
	lsl r0, r1, #8
	lsr r0, r0, #0x18
	strh r0, [r5, #0xC]
	lsr r1, r1, #0x18
	strh r1, [r5, #0xE]
	cmp r0, #0x18
	bls _0805FD5E
	mov r0, #0x18
	strh r0, [r5, #0xC]
_0805FD5E:
	ldrh r0, [r5, #0xE]
	cmp r0, #0xB
	bls _0805FD68
	mov r0, #0xB
	strh r0, [r5, #0xE]
_0805FD68:
	ldrh r1, [r5, #0xC]
	cmp r1, #4
	bhi _0805FD72
	mov r0, #5
	strh r0, [r5, #0xC]
_0805FD72:
	ldrh r2, [r5, #0xE]
	cmp r2, #1
	bhi _0805FD7C
	mov r0, #2
	strh r0, [r5, #0xE]
_0805FD7C:
	bl sub_0805FCF4
	ldrh r0, [r5, #0xC]
	ldrh r1, [r5, #0xE]
	bl sub_08074B08
	ldrb r0, [r4]
	cmp r0, #0
	beq _0805FE80
	ldr r7, _0805FDC0 @ =0x081A4214
	mov r0, #0xA0
	lsl r0, r0, #4
	add r5, r0, #0
_0805FD96:
	ldrb r2, [r4]
	cmp r2, #0xA
	beq _0805FDC4
	cmp r2, #0x40
	bne _0805FDCC
	ldrb r0, [r4, #1]
	sub r0, #0x30
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #9
	bhi _0805FE78
	ldrb r1, [r4, #1]
	sub r1, #0x30
	mov r8, r1
	cmp r1, #0
	bne _0805FE22
	mov r2, #1
	mov r8, r2
	b _0805FE22
_0805FDBC: .4byte 0x0201AE60
_0805FDC0: .4byte gUnk_081A4214
_0805FDC4:
	mov r0, #0
	mov r9, r0
	add r6, #0xC
	b _0805FE78
_0805FDCC:
	ldr r1, _0805FE28 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _0805FE30
	mov r0, r9
	add r0, #0xA
	ldr r1, _0805FE2C @ =0x0201AE60
	ldrh r1, [r1, #0xC]
	lsl r1, r1, #3
	cmp r0, r1
	blt _0805FDEC
	mov r1, #0
	mov r9, r1
	add r6, #0xC
_0805FDEC:
	lsl r0, r2, #8
	ldrb r2, [r4, #1]
	orr r0, r2
	mov r1, r9
	add r1, #1
	add r2, r6, #1
	ldr r3, [r7, #0x24]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	orr r3, r5
	bl sub_08074C80
	ldrb r1, [r4]
	lsl r0, r1, #8
	ldrb r2, [r4, #1]
	orr r0, r2
	mov r2, r8
	lsl r1, r2, #2
	add r1, r1, r7
	ldrb r3, [r1]
	orr r3, r5
	mov r1, r9
	add r2, r6, #0
	bl sub_08074C80
	mov r0, #0xA
	add r9, r0
_0805FE22:
	add r4, #1
	b _0805FE78
	.align 2, 0
_0805FE28: .4byte 0x02011C20
_0805FE2C: .4byte 0x0201AE60
_0805FE30:
	add r0, r4, #0
	bl sub_08074AB4
	lsl r1, r0, #2
	add r1, r1, r0
	add r1, r9
	ldr r0, _0805FE98 @ =0x0201AE60
	ldrh r0, [r0, #0xC]
	lsl r0, r0, #3
	cmp r1, r0
	ble _0805FE4C
	mov r1, #0
	mov r9, r1
	add r6, #0xC
_0805FE4C:
	ldrb r0, [r4]
	mov r1, r9
	add r1, #1
	add r2, r6, #1
	ldr r3, [r7, #0x24]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	orr r3, r5
	bl sub_08074D48
	ldrb r0, [r4]
	mov r2, r8
	lsl r1, r2, #2
	add r1, r1, r7
	ldrb r3, [r1]
	orr r3, r5
	mov r1, r9
	add r2, r6, #0
	bl sub_08074D48
	mov r0, #5
	add r9, r0
_0805FE78:
	add r4, #1
	ldrb r0, [r4]
	cmp r0, #0
	bne _0805FD96
_0805FE80:
	ldr r0, _0805FE9C @ =0x0201AE84
	ldr r1, _0805FEA0 @ =0x081A4214
	ldrh r1, [r1]
	bl sub_08075114
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805FE98: .4byte 0x0201AE60
_0805FE9C: .4byte 0x0201AE84
_0805FEA0: .4byte gUnk_081A4214
	thumb_func_end sub_0805FD28

