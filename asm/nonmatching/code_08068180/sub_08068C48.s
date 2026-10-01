	thumb_func_start sub_08068C48
sub_08068C48: @ 0x08068C48
	push {r4, r5, r6, r7, lr}
	sub sp, #0x20
	ldr r1, _08068C84 @ =0x0201DB20
	ldr r2, _08068C88 @ =0x00001C1C
	add r0, r1, r2
	ldrb r2, [r0]
	ldr r3, _08068C8C @ =0x00001C42
	add r0, r1, r3
	add r0, r2, r0
	ldrb r0, [r0]
	add r6, r1, #0
	cmp r0, #0
	bne _08068CFA
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r6, r1
	add r0, r2, r0
	ldrb r3, [r0]
	lsl r1, r3, #1
	add r1, r1, r3
	add r1, r1, r2
	lsl r1, r1, #1
	ldr r2, _08068C90 @ =0x00001494
	add r0, r6, r2
	add r1, r1, r0
	ldrh r1, [r1]
	cmp r1, #3
	bhi _08068C94
	mov r7, #0x30
	b _08068CA6
_08068C84: .4byte 0x0201DB20
_08068C88: .4byte 0x00001C1C
_08068C8C: .4byte 0x00001C42
_08068C90: .4byte 0x00001494
_08068C94:
	ldr r3, _08068D04 @ =0x00001BB2
	add r1, r6, r3
	ldr r2, _08068D08 @ =0x00001BB0
	add r0, r6, r2
	ldrh r0, [r0]
	lsr r0, r0, #1
	ldrh r1, [r1]
	add r0, r1, r0
	lsr r7, r0, #8
_08068CA6:
	ldr r3, _08068D0C @ =0x00001C58
	add r1, r6, r3
	ldrh r2, [r1]
	mov r3, #0
	ldsh r0, [r1, r3]
	cmp r0, #0
	beq _08068CFA
	sub r0, r2, #1
	mov r4, #0
	strh r0, [r1]
	ldr r0, _08068D10 @ =0x081A70F4
	ldr r2, _08068D14 @ =0x000001FD
	add r1, r7, r2
	str r1, [sp, #0]
	mov r5, #4
	str r5, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r6, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	mov r3, #0xD8
	bl sub_08077EF4
	ldr r0, _08068D18 @ =0x080874A0
	add r1, r7, #1
	str r1, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	str r6, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	mov r3, #0xDB
	bl sub_08077EF4
_08068CFA:
	add sp, #0x20
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08068D04: .4byte 0x00001BB2
_08068D08: .4byte 0x00001BB0
_08068D0C: .4byte 0x00001C58
_08068D10: .4byte gUnk_081A70F4
_08068D14: .4byte 0x000001FD
_08068D18: .4byte gUnk_080874A0
	thumb_func_end sub_08068C48

