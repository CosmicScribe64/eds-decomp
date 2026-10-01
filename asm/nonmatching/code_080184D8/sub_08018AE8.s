	thumb_func_start sub_08018AE8
sub_08018AE8: @ 0x08018AE8
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08018B34 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08018B38 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r6, #0
	bne _08018B10
	b _08018C0C
_08018B10:
	ldr r0, _08018B3C @ =0x000007FF
	and r0, r6
	lsl r0, r0, #1
	ldr r1, _08018B40 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _08018B44 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bhi _08018B48
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl sub_08018544
	b _08018B5C
_08018B34: .4byte 0x00000D64
_08018B38: .4byte 0x0201930C
_08018B3C: .4byte 0x000007FF
_08018B40: .4byte gUnk_08622AB4
_08018B44: .4byte 0xFFFFF880
_08018B48:
	mov r0, #0x80
	cmp r4, #0
	beq _08018B50
	ldr r0, _08018C14 @ =0x00008080
_08018B50:
	lsl r1, r5, #0x10
	lsr r1, r1, #0x10
	add r2, r3, #0
	mov r3, #0
	bl sub_0801EC58
_08018B5C:
	cmp r5, #4
	bgt _08018B8A
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl sub_08017DE0
	ldr r0, _08018C18 @ =0x020192E0
	ldr r1, _08018C1C @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r1, r0, #0x1E
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	lsl r2, r4, #0x18
	lsl r1, r5, #0x18
	lsr r2, r2, #8
	orr r2, r1
	lsr r2, r2, #0x10
	mov r1, #0x1B
	bl sub_08042AB0
_08018B8A:
	cmp r5, #0xA
	bne _08018BBC
	mov r0, #1
	and r0, r4
	ldr r1, _08018C20 @ =0x00000D64
	mul r1, r0
	mov r0, #0xB9
	lsl r0, r0, #3
	add r1, r1, r0
	ldr r0, _08018C24 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08018BBC
	mov r0, #0x11
	cmp r4, #0
	beq _08018BB2
	ldr r0, _08018C28 @ =0x00008011
_08018BB2:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08018BBC:
	ldr r0, _08018C2C @ =0x000007FF
	and r6, r0
	lsl r0, r6, #1
	ldr r1, _08018C30 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08018C34 @ =0x00000447
	ldrh r0, [r0]
	cmp r0, r1
	bne _08018C0C
	add r0, r4, #0
	add r1, r5, #0
	bl sub_0800CD68
	lsl r6, r0, #0x10
	lsr r3, r6, #0x10
	ldr r0, _08018C38 @ =0x0000FFFF
	cmp r3, r0
	beq _08018C0C
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08018C20 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08018C24 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08018C0C
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	lsr r1, r6, #0x18
	mov r2, #1
	bl sub_08018544
_08018C0C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08018C14: .4byte 0x00008080
_08018C18: .4byte 0x020192E0
_08018C1C: .4byte 0x00001B12
_08018C20: .4byte 0x00000D64
_08018C24: .4byte 0x0201930C
_08018C28: .4byte 0x00008011
_08018C2C: .4byte 0x000007FF
_08018C30: .4byte gUnk_08622AB4
_08018C34: .4byte 0x00000447
_08018C38: .4byte 0x0000FFFF
	thumb_func_end sub_08018AE8

