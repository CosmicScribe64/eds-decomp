	thumb_func_start sub_08003C78
sub_08003C78: @ 0x08003C78
	push {r4, r5, r6, r7, lr}
	bl sub_0800406C
	ldr r4, _08003CF4 @ =0x0201F814
	mov r0, #6
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	bne _08003C8C
	b _08003DB4
_08003C8C:
	ldrh r2, [r4]
	ldr r0, _08003CF8 @ =0x00001FF8
	and r0, r2
	cmp r0, #0
	beq _08003D14
	ldr r0, [r4]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x16
	sub r0, #1
	ldr r3, _08003CFC @ =0x000003FF
	add r1, r3, #0
	and r0, r1
	lsl r0, r0, #3
	ldr r1, _08003D00 @ =0xFFFFE007
	and r1, r2
	orr r1, r0
	strh r1, [r4]
	ldr r5, _08003D04 @ =0x03000040
	ldr r6, _08003D08 @ =0x08198508
	ldr r4, [r4]
	lsl r2, r4, #0x13
	lsr r1, r2, #0x16
	lsl r1, r1, #1
	lsl r3, r4, #0x1D
	lsr r0, r3, #0x1E
	sub r0, #1
	lsl r0, r0, #5
	add r1, r1, r0
	lsl r4, r4, #0x1F
	lsr r0, r4, #0x1F
	lsl r0, r0, #6
	add r1, r1, r0
	add r1, r1, r6
	ldrh r1, [r1]
	ldr r7, _08003D0C @ =0x0000442A
	add r0, r5, r7
	strh r1, [r0]
	lsr r2, r2, #0x16
	lsl r2, r2, #1
	lsr r3, r3, #0x1E
	sub r3, #1
	lsl r3, r3, #5
	add r2, r2, r3
	lsr r4, r4, #0x1F
	lsl r4, r4, #6
	add r2, r2, r4
	add r2, r2, r6
	ldrh r0, [r2]
	ldr r1, _08003D10 @ =0x0000442C
	add r5, r5, r1
	strh r0, [r5]
	b _08003E5A
_08003CF4: .4byte 0x0201F814
_08003CF8: .4byte 0x00001FF8
_08003CFC: .4byte 0x000003FF
_08003D00: .4byte 0xFFFFE007
_08003D04: .4byte 0x03000040
_08003D08: .4byte gUnk_08198508
_08003D0C: .4byte 0x0000442A
_08003D10: .4byte 0x0000442C
_08003D14:
	ldr r1, [r4]
	lsl r0, r1, #0x1D
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _08003D24
	cmp r0, #2
	beq _08003D34
	b _08003D46
_08003D24:
	lsl r0, r1, #0x10
	lsr r0, r0, #0x1D
	add r0, #1
	lsl r0, r0, #5
	mov r1, #0x1F
	ldrb r2, [r4, #1]
	and r1, r2
	b _08003D42
_08003D34:
	lsl r0, r1, #0x10
	lsr r0, r0, #0x1D
	sub r0, #1
	lsl r0, r0, #5
	mov r1, #0x1F
	ldrb r3, [r4, #1]
	and r1, r3
_08003D42:
	orr r1, r0
	strb r1, [r4, #1]
_08003D46:
	ldr r3, _08003D88 @ =0x0201F814
	mov r2, #7
	neg r2, r2
	ldrb r7, [r3]
	and r2, r7
	strb r2, [r3]
	ldr r0, [r3]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	mov r2, #1
	and r2, r0
	cmp r2, #0
	beq _08003D98
	ldr r0, _08003D8C @ =0x03000040
	ldr r2, _08003D90 @ =0x0000442A
	add r1, r0, r2
	mov r2, #0x80
	lsl r2, r2, #1
	strh r2, [r1]
	ldr r3, _08003D94 @ =0x0000442C
	add r0, r0, r3
	strh r2, [r0]
	b _08003E5A
	.align 2, 0
_08003D88: .4byte 0x0201F814
_08003D8C: .4byte 0x03000040
_08003D90: .4byte 0x0000442A
_08003D94: .4byte 0x0000442C
_08003D98:
	ldr r0, _08003DA8 @ =0x03000040
	ldr r7, _08003DAC @ =0x0000442A
	add r1, r0, r7
	strh r2, [r1]
	ldr r1, _08003DB0 @ =0x0000442C
	add r0, r0, r1
	strh r2, [r0]
	b _08003E5A
_08003DA8: .4byte 0x03000040
_08003DAC: .4byte 0x0000442A
_08003DB0: .4byte 0x0000442C
_08003DB4:
	ldr r1, _08003DEC @ =0x03000040
	mov r0, #0x88
	lsl r0, r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08003DF6
	bl sub_08003EEC
	cmp r0, #0
	beq _08003DF0
	ldr r2, [r4]
	ldrb r3, [r4, #1]
	lsr r0, r3, #5
	add r0, #1
	lsl r2, r2, #0x1F
	lsr r2, r2, #0x1F
	mov r1, #1
	sub r1, r1, r2
	bl sub_080040E4
	mov r0, #7
	neg r0, r0
	ldrb r7, [r4]
	and r0, r7
	mov r1, #2
	b _08003E2C
	.align 2, 0
_08003DEC: .4byte 0x03000040
_08003DF0:
	mov r0, #3
	bl sub_08077AEC
_08003DF6:
	ldr r1, _08003E48 @ =0x03000040
	mov r0, #0x88
	lsl r0, r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08003E5A
	bl sub_08003ED4
	cmp r0, #0
	beq _08003E54
	ldr r4, _08003E4C @ =0x0201F814
	ldr r2, [r4]
	ldrb r3, [r4, #1]
	lsr r0, r3, #5
	sub r0, #1
	lsl r2, r2, #0x1F
	lsr r2, r2, #0x1F
	mov r1, #1
	sub r1, r1, r2
	bl sub_080040E4
	mov r0, #7
	neg r0, r0
	ldrb r7, [r4]
	and r0, r7
	mov r1, #4
_08003E2C:
	orr r0, r1
	strb r0, [r4]
	ldr r0, _08003E50 @ =0xFFFFE007
	ldrh r1, [r4]
	and r0, r1
	mov r1, #0x80
	orr r0, r1
	strh r0, [r4]
	mov r0, #0
	bl sub_08077AEC
	mov r0, #0
	b _08003E78
	.align 2, 0
_08003E48: .4byte 0x03000040
_08003E4C: .4byte 0x0201F814
_08003E50: .4byte 0xFFFFE007
_08003E54:
	mov r0, #3
	bl sub_08077AEC
_08003E5A:
	ldr r1, _08003E6C @ =0x03000040
	mov r0, #3
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08003E70
	mov r0, #0
	b _08003E78
	.align 2, 0
_08003E6C: .4byte 0x03000040
_08003E70:
	mov r0, #2
	bl sub_08077AEC
	mov r0, #1
_08003E78:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08003C78
	.align 2, 0

