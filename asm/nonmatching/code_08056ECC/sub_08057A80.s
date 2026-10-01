	thumb_func_start sub_08057A80
sub_08057A80: @ 0x08057A80
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	add r7, r0, #0
	add r5, r1, #0
	mov r0, #1
	add r1, r7, #0
	bl sub_0804A3D8
	cmp r0, #0
	bne _08057A9E
	mov r0, #0
	bl sub_08008860
	cmp r0, #0
	bne _08057B00
_08057A9E:
	mov r0, #0x94
	mul r0, r7
	ldr r1, _08057AF4 @ =0x0201A070
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08057AF8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08057AFC @ =0x000005F3
	ldrh r0, [r0]
	cmp r0, r1
	beq _08057B00
	mov r0, #4
	ldrb r2, [r5]
	orr r0, r2
	mov r1, #2
	orr r0, r1
	strb r0, [r5]
	mov r0, #1
	add r1, r7, #0
	bl sub_0800C894
	strh r0, [r5, #4]
	mov r0, #7
	and r7, r0
	lsl r1, r7, #4
	mov r0, #0x71
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	mov r0, #5
	neg r0, r0
	ldrb r1, [r5, #1]
	and r0, r1
	mov r1, #9
	neg r1, r1
	and r0, r1
	strb r0, [r5, #1]
	mov r0, #1
	b _08057BA2
_08057AF4: .4byte 0x0201A070
_08057AF8: .4byte gUnk_08622AB4
_08057AFC: .4byte 0x000005F3
_08057B00:
	mov r0, #9
	neg r0, r0
	ldrb r2, [r5]
	and r0, r2
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r5]
	mov r6, #0
_08057B12:
	mov r0, #0x94
	mul r0, r6
	ldr r1, _08057B50 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08057B96
	mov r4, sp
	add r0, r7, #0
	add r1, r6, #0
	mov r2, sp
	bl sub_0805797C
	mov r1, sp
	mov r2, #8
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08057B96
	add r0, r2, #0
	ldrb r1, [r5]
	and r0, r1
	cmp r0, #0
	bne _08057B54
	add r0, r5, #0
	mov r1, sp
	bl sub_08075294
	b _08057B96
_08057B50: .4byte 0x0201930C
_08057B54:
	mov r2, #4
	ldsh r1, [r4, r2]
	mov r2, #4
	ldsh r0, [r5, r2]
	cmp r1, r0
	ble _08057B6C
	add r0, r5, #0
	mov r1, sp
	mov r2, #8
	bl sub_08075294
	b _08057B96
_08057B6C:
	mov r0, sp
	ldrh r0, [r0]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1D
	mov r0, #0
	bl sub_0800C8A8
	add r4, r0, #0
	ldrh r0, [r5]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x1D
	mov r0, #0
	bl sub_0800C8A8
	cmp r4, r0
	ble _08057B96
	add r0, r5, #0
	mov r1, sp
	mov r2, #8
	bl sub_08075294
_08057B96:
	add r6, #1
	cmp r6, #4
	ble _08057B12
	ldrb r5, [r5]
	lsl r0, r5, #0x1C
	lsr r0, r0, #0x1F
_08057BA2:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08057A80
	.align 2, 0

