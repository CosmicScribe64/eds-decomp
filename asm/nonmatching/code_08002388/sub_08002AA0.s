	thumb_func_start sub_08002AA0
sub_08002AA0: @ 0x08002AA0
	push {r4, r5, r6, lr}
	ldr r4, _08002AFC @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #0
	bl sub_080034B8
	mov r0, #0
	bl sub_08003174
	ldr r3, _08002B00 @ =0x0819834C
	ldrb r2, [r4]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1D
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	bl sub_080036FC
	ldr r1, _08002B04 @ =0x03000040
	mov r0, #0x20
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08002B2E
	ldrb r2, [r4]
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1D
	cmp r0, #3
	bhi _08002B08
	add r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #3
	mov r1, #0x39
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _08002B10
_08002AFC: .4byte 0x0201F7E0
_08002B00: .4byte gUnk_0819834C
_08002B04: .4byte 0x03000040
_08002B08:
	mov r0, #0x39
	neg r0, r0
	and r0, r2
	strb r0, [r4]
_08002B10:
	ldr r2, _08002B60 @ =0x0201F7E0
	ldrb r1, [r2]
	mov r0, #0x3F
	and r0, r1
	cmp r0, #4
	bne _08002B28
	mov r0, #0x39
	neg r0, r0
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r2]
_08002B28:
	mov r0, #0
	bl sub_08077AEC
_08002B2E:
	ldr r1, _08002B64 @ =0x03000040
	mov r0, #0x10
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08002B90
	ldr r1, _08002B60 @ =0x0201F7E0
	ldrb r2, [r1]
	mov r0, #0x38
	and r0, r2
	add r3, r1, #0
	cmp r0, #0
	beq _08002B68
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1D
	sub r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #3
	mov r1, #0x39
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r3]
	b _08002B74
_08002B60: .4byte 0x0201F7E0
_08002B64: .4byte 0x03000040
_08002B68:
	mov r0, #0x39
	neg r0, r0
	and r0, r2
	mov r1, #0x20
	orr r0, r1
	strb r0, [r3]
_08002B74:
	ldrb r1, [r3]
	mov r0, #0x3F
	and r0, r1
	cmp r0, #4
	bne _08002B8A
	mov r0, #0x39
	neg r0, r0
	and r0, r1
	mov r1, #0x20
	orr r0, r1
	strb r0, [r3]
_08002B8A:
	mov r0, #0
	bl sub_08077AEC
_08002B90:
	ldr r4, _08002BB8 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _08002BCA
	ldr r1, _08002BBC @ =0x0201F7E0
	mov r0, #7
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08002BC4
	mov r0, #0
	bl sub_08077AEC
	ldr r0, _08002BC0 @ =0x00004859
	add r1, r4, r0
	mov r0, #5
	b _08002CD6
_08002BB8: .4byte 0x03000040
_08002BBC: .4byte 0x0201F7E0
_08002BC0: .4byte 0x00004859
_08002BC4:
	mov r0, #3
	bl sub_08077AEC
_08002BCA:
	ldr r1, _08002BF0 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08002C42
	mov r1, #0
	ldr r0, _08002BF4 @ =0x0201F7E0
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1D
	cmp r0, #1
	beq _08002C08
	cmp r0, #1
	bgt _08002BF8
	cmp r0, #0
	beq _08002C02
	b _08002C1C
_08002BF0: .4byte 0x03000040
_08002BF4: .4byte 0x0201F7E0
_08002BF8:
	cmp r0, #2
	beq _08002C0E
	cmp r0, #3
	beq _08002C14
	b _08002C1C
_08002C02:
	bl sub_08063BAC
	b _08002C18
_08002C08:
	bl sub_08063C14
	b _08002C18
_08002C0E:
	bl sub_08063C7C
	b _08002C18
_08002C14:
	bl sub_08063CE4
_08002C18:
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08002C1C:
	cmp r1, #0
	beq _08002C3C
	mov r0, #0
	bl sub_08077AEC
	ldr r0, _08002C34 @ =0x03000040
	ldr r1, _08002C38 @ =0x00004859
	add r0, r0, r1
	mov r1, #6
	strb r1, [r0]
	b _08002CD8
	.align 2, 0
_08002C34: .4byte 0x03000040
_08002C38: .4byte 0x00004859
_08002C3C:
	mov r0, #3
	bl sub_08077AEC
_08002C42:
	ldr r6, _08002CA8 @ =0x03000040
	mov r0, #1
	ldrh r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	beq _08002CBE
	ldr r5, _08002CAC @ =0x0819834C
	ldr r4, _08002CB0 @ =0x0201F7E0
	ldrb r2, [r4]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1D
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	bl sub_08063DAC
	cmp r0, #0
	beq _08002CB8
	ldrb r2, [r4]
	lsl r0, r2, #0x1D
	lsr r0, r0, #0x1D
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1D
	add r1, r1, r2
	lsl r1, r1, #1
	add r1, r1, r5
	ldr r0, _08002CB4 @ =0x00004870
	add r3, r6, r0
	mov r2, #0x1F
	ldrb r1, [r1]
	and r2, r1
	lsl r2, r2, #1
	mov r0, #0x3F
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	orr r0, r2
	strb r0, [r3]
	mov r0, #1
	bl sub_08077AEC
	mov r0, #1
	b _08002CDA
	.align 2, 0
_08002CA8: .4byte 0x03000040
_08002CAC: .4byte gUnk_0819834C
_08002CB0: .4byte 0x0201F7E0
_08002CB4: .4byte 0x00004870
_08002CB8:
	mov r0, #3
	bl sub_08077AEC
_08002CBE:
	ldr r4, _08002CE0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _08002CD8
	mov r0, #1
	bl sub_08077AEC
	ldr r0, _08002CE4 @ =0x00004859
	add r1, r4, r0
	mov r0, #8
_08002CD6:
	strb r0, [r1]
_08002CD8:
	mov r0, #0
_08002CDA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08002CE0: .4byte 0x03000040
_08002CE4: .4byte 0x00004859
	thumb_func_end sub_08002AA0

