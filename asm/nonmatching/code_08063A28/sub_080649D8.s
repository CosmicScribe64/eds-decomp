	thumb_func_start sub_080649D8
sub_080649D8: @ 0x080649D8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r7, r0, #0
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08064ACC @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r2, _08064AD0 @ =0x03000C5C
	str r2, [r1, #4]
	ldr r0, _08064AD4 @ =0x81000800
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r3, #0x80
	lsl r3, r3, #0x18
	mov r5, #0xC0
	lsl r5, r5, #5
	add r4, r2, r5
	ldr r5, _08064AD8 @ =0xFFFFF3E4
	add r2, r2, r5
	cmp r0, #0
	bge _08064A18
_08064A10:
	ldr r0, [r1, #8]
	and r0, r3
	cmp r0, #0
	bne _08064A10
_08064A18:
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r0, _08064ACC @ =0x040000D4
	str r1, [r0]
	str r4, [r0, #4]
	ldr r1, _08064ADC @ =0x81000400
	str r1, [r0, #8]
	ldr r1, [r0, #8]
	add r3, r0, #0
	ldr r0, [r3, #8]
	mov r1, #0x80
	lsl r1, r1, #0x18
	cmp r0, #0
	bge _08064A3E
_08064A36:
	ldr r0, [r3, #8]
	and r0, r1
	cmp r0, #0
	bne _08064A36
_08064A3E:
	ldr r0, _08064AE0 @ =0x0000442A
	add r1, r2, r0
	mov r0, #0
	strh r0, [r1]
	mov r6, #0
	ldr r1, _08064AE4 @ =0x02020310
	mov r8, r1
	mov r2, #0xC4
	lsl r2, r2, #0xF
	mov r9, r2
_08064A52:
	lsl r0, r6, #0x10
	lsr r5, r0, #0x10
	lsl r1, r7, #1
	mov r0, #0x2C
	add r0, r8
	mov sl, r0
	add r1, sl
	ldrh r2, [r1]
	lsl r0, r2, #3
	add r0, r0, r2
	lsl r0, r0, #3
	ldr r1, _08064AE8 @ =0x080865DC
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r5, #0
	bl sub_08064928
	mov r2, r9
	lsr r4, r2, #0x10
	mov r0, #1
	add r1, r4, #0
	add r2, r5, #0
	bl sub_08064984
	mov r1, r8
	ldr r0, [r1, #0xC]
	cmp r0, r6
	bne _08064A94
	mov r0, #4
	add r1, r4, #0
	add r2, r5, #0
	bl sub_08064984
_08064A94:
	add r7, #1
	ldr r2, _08064AEC @ =0x0202037C
	ldrh r1, [r2]
	add r0, r7, #0
	bl __modsi3
	add r7, r0, #0
	mov r5, #0xA0
	lsl r5, r5, #0xC
	add r9, r5
	add r6, #1
	cmp r6, #2
	ble _08064A52
	mov r1, sl
	sub r1, #0x2C
	mov r0, #1
	ldrb r2, [r1, #0x18]
	orr r0, r2
	strb r0, [r1, #0x18]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08064ACC: .4byte 0x040000D4
_08064AD0: .4byte 0x03000C5C
_08064AD4: .4byte 0x81000800
_08064AD8: .4byte 0xFFFFF3E4
_08064ADC: .4byte 0x81000400
_08064AE0: .4byte 0x0000442A
_08064AE4: .4byte 0x02020310
_08064AE8: .4byte gUnk_080865DC
_08064AEC: .4byte 0x0202037C
	thumb_func_end sub_080649D8

