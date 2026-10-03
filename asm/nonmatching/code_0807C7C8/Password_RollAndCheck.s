	thumb_func_start Password_RollAndCheck
Password_RollAndCheck: @ 0x0807C7C8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _0807C884 @ =0x0201F7B0
	mov r0, #0x40
	neg r0, r0
	mov r8, r0
	ldrb r1, [r4, #0x11]
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r4, #0x11]
	ldr r0, [r4, #0x10]
	ldr r7, _0807C888 @ =0xFFF03FFF
	and r0, r7
	mov r1, #0x80
	lsl r1, r1, #0xC
	orr r0, r1
	str r0, [r4, #0x10]
	bl Password_DrawDigits
	bl Password_DrawKeyCursor
	ldr r6, _0807C88C @ =0x03000040
	mov r0, #1
	ldrh r2, [r6, #6]
	and r0, r2
	cmp r0, #0
	beq _0807C812
	mov r0, #0xF
	ldrh r1, [r4, #0x12]
	and r0, r1
	mov r2, #0xB4
	lsl r2, r2, #4
	add r1, r2, #0
	orr r0, r1
	strh r0, [r4, #0x12]
_0807C812:
	ldrh r2, [r4, #0x12]
	lsl r1, r2, #0x10
	lsr r0, r1, #0x14
	add r0, #1
	lsl r0, r0, #4
	mov r5, #0xF
	add r3, r5, #0
	and r3, r2
	orr r3, r0
	strh r3, [r4, #0x12]
	lsr r1, r1, #0x14
	cmp r1, #0xB3
	bhi _0807C890
	mov r5, #0
	add r6, r4, #0
_0807C830:
	bl Random
	add r4, r5, r6
	mov r1, #0xA
	bl __modsi3
	strb r0, [r4]
	add r5, #1
	cmp r5, #7
	ble _0807C830
	bl Random
	ldr r5, _0807C884 @ =0x0201F7B0
	mov r1, #0xA
	bl __modsi3
	lsl r0, r0, #4
	mov r4, #0xF
	add r1, r4, #0
	ldrb r2, [r5, #0x10]
	and r1, r2
	orr r1, r0
	strb r1, [r5, #0x10]
	bl Random
	mov r1, #7
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r2, [r5, #0x10]
	and r1, r2
	orr r1, r0
	strb r1, [r5, #0x10]
	ldrh r5, [r5, #0x12]
	lsr r0, r5, #4
	and r4, r0
	cmp r4, #8
	bne _0807C912
	mov r0, #0x27
	bl PlaySE
	b _0807C912
_0807C884: .4byte 0x0201F7B0
_0807C888: .4byte 0xFFF03FFF
_0807C88C: .4byte 0x03000040
_0807C890:
	and r3, r5
	strh r3, [r4, #0x12]
	mov r0, r8
	ldrb r1, [r4, #0x11]
	and r0, r1
	strb r0, [r4, #0x11]
	ldr r0, [r4, #0x10]
	and r0, r7
	str r0, [r4, #0x10]
	ldrh r0, [r4, #0x14]
	cmp r0, #0
	bne _0807C8C0
	add r0, r4, #0
	mov r1, #8
	bl MemClear16
	ldr r2, _0807C8BC @ =0x00004859
	add r1, r6, r2
	ldrb r0, [r1]
	add r0, #3
	b _0807C910
	.align 2, 0
_0807C8BC: .4byte 0x00004859
_0807C8C0:
	ldr r0, _0807C8F4 @ =0x02011C20
	ldrh r2, [r4, #0x14]
	lsl r1, r2, #2
	add r1, r1, r0
	ldrb r1, [r1, #0xA]
	lsl r0, r1, #0x1E
	lsr r5, r0, #0x1F
	cmp r5, #0
	bne _0807C900
	add r1, r4, #0
	add r1, #8
	add r0, r4, #0
	mov r2, #8
	bl MemCopy16
	ldrh r0, [r4, #0x14]
	bl Password_DrawCard
	ldr r1, _0807C8F8 @ =0x0000442C
	add r0, r6, r1
	strh r5, [r0]
	ldr r0, _0807C8FC @ =0x04000018
	strh r5, [r0]
	mov r0, #1
	b _0807C914
	.align 2, 0
_0807C8F4: .4byte 0x02011C20
_0807C8F8: .4byte 0x0000442C
_0807C8FC: .4byte 0x04000018
_0807C900:
	add r0, r4, #0
	mov r1, #8
	bl MemClear16
	ldr r2, _0807C920 @ =0x00004859
	add r1, r6, r2
	ldrb r0, [r1]
	add r0, #6
_0807C910:
	strb r0, [r1]
_0807C912:
	mov r0, #0
_0807C914:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0807C920: .4byte 0x00004859
	thumb_func_end Password_RollAndCheck

