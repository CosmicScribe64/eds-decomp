	thumb_func_start sub_0802CE38
sub_0802CE38: @ 0x0802CE38
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	add r6, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	ldrh r0, [r0]
	bl sub_08047058
	add r5, r0, #0
	mov r1, r8
	ldrh r0, [r1]
	bl sub_0800966C
	cmp r0, #0
	beq _0802CE60
	b _0802CF92
_0802CE60:
	cmp r6, #0
	beq _0802CE88
	mov r2, r8
	ldrh r0, [r2]
	bl sub_0802CD28
	add r4, r0, #0
	ldrh r0, [r6]
	bl sub_0802CD28
	cmp r4, r0
	bge _0802CE7A
	b _0802CF92
_0802CE7A:
	mov r1, r8
	ldrh r0, [r1]
	bl sub_0802CD28
	cmp r0, #1
	bne _0802CE88
	b _0802CF92
_0802CE88:
	mov r0, #0xFC
	mov r2, r8
	ldrb r2, [r2, #3]
	and r0, r2
	cmp r0, #0x44
	bne _0802CF26
	ldr r0, _0802CEC8 @ =0x000007FF
	mov r1, r8
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0802CECC @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0802CED0 @ =0x0000043A
	cmp r1, r0
	beq _0802CF26
	cmp r1, r0
	bgt _0802CEF0
	sub r0, #0x42
	cmp r1, r0
	bgt _0802CEDE
	sub r0, #1
	cmp r1, r0
	bge _0802CF26
	ldr r0, _0802CED4 @ =0x00000291
	cmp r1, r0
	beq _0802CF26
	cmp r1, r0
	bgt _0802CED8
	sub r0, #7
	b _0802CF22
_0802CEC8: .4byte 0x000007FF
_0802CECC: .4byte gUnk_08622AB4
_0802CED0: .4byte 0x0000043A
_0802CED4: .4byte 0x00000291
_0802CED8:
	mov r0, #0xAC
	lsl r0, r0, #2
	b _0802CF22
_0802CEDE:
	ldr r0, _0802CEEC @ =0x00000434
	cmp r1, r0
	bgt _0802CF92
	sub r0, #1
	cmp r1, r0
	blt _0802CF92
	b _0802CF26
_0802CEEC: .4byte 0x00000434
_0802CEF0:
	ldr r0, _0802CF08 @ =0x000004B4
	cmp r1, r0
	bgt _0802CF0C
	sub r0, #1
	cmp r1, r0
	bge _0802CF26
	sub r0, #0x68
	cmp r1, r0
	beq _0802CF26
	add r0, #0x4F
	b _0802CF22
	.align 2, 0
_0802CF08: .4byte 0x000004B4
_0802CF0C:
	ldr r0, _0802CF1C @ =0x00000587
	cmp r1, r0
	beq _0802CF26
	cmp r1, r0
	bgt _0802CF20
	sub r0, #0x65
	b _0802CF22
	.align 2, 0
_0802CF1C: .4byte 0x00000587
_0802CF20:
	ldr r0, _0802CF4C @ =0x000005FE
_0802CF22:
	cmp r1, r0
	bne _0802CF92
_0802CF26:
	cmp r5, #0
	blt _0802CF92
	ldr r2, _0802CF50 @ =0x0819A9D4
	lsl r1, r5, #1
	add r1, r1, r5
	lsl r1, r1, #3
	add r0, r2, #0
	add r0, #8
	add r0, r1, r0
	ldr r7, [r0]
	add r2, #0xC
	add r1, r1, r2
	ldr r3, [r1]
	cmp r7, #0
	bne _0802CF54
	cmp r3, #0
	bne _0802CF58
_0802CF48:
	mov r0, #1
	b _0802CF94
_0802CF4C: .4byte 0x000005FE
_0802CF50: .4byte gUnk_0819A9D4
_0802CF54:
	cmp r3, #0
	beq _0802CF68
_0802CF58:
	mov r0, r8
	add r1, r6, #0
	mov r2, r9
	bl _call_via_r3
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802CF92
_0802CF68:
	cmp r7, #0
	beq _0802CF48
	mov r6, #0
_0802CF6E:
	mov r4, #0
	lsl r0, r6, #0x18
	lsr r5, r0, #0x18
_0802CF74:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	orr r1, r5
	mov r0, r8
	bl _call_via_r7
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0802CF48
	add r4, #1
	cmp r4, #0xA
	ble _0802CF74
	add r6, #1
	cmp r6, #1
	ble _0802CF6E
_0802CF92:
	mov r0, #0
_0802CF94:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802CE38

