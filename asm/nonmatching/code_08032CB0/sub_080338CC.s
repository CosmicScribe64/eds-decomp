	thumb_func_start sub_080338CC
sub_080338CC: @ 0x080338CC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	beq _080338E6
	b _08033A64
_080338E6:
	ldr r0, _0803390C @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x80
	ble _080338F6
	b _08033A64
_080338F6:
	cmp r0, #0x7F
	bge _080338FC
	b _08033A64
_080338FC:
	cmp r0, #0x7F
	beq _08033910
	cmp r0, #0x80
	bne _0803391A
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r5, r0, #0x1F
	b _0803391A
_0803390C: .4byte 0x02017A40
_08033910:
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r5, r1, r0
_0803391A:
	mov r0, #1
	mov r8, r0
	ldr r1, _08033A50 @ =0x02018450
	lsl r4, r5, #1
	add r0, r4, r5
	lsl r0, r0, #2
	add r6, r0, r1
	ldr r0, _08033A54 @ =0x000007FF
	ldrh r1, [r6, #0xA]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08033A58 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	bl sub_0800756C
	mov sl, r4
	cmp r0, #0
	beq _0803394E
	add r0, r5, #0
	bl sub_08008668
	neg r1, r0
	orr r1, r0
	lsr r1, r1, #0x1F
	mov r8, r1
_0803394E:
	ldrb r1, [r6, #8]
	lsl r0, r1, #0x1A
	cmp r0, #0
	bge _08033A2A
	mov r2, r8
	cmp r2, #0
	beq _08033A2A
	add r0, r5, #0
	bl sub_08008A44
	cmp r0, #0
	blt _08033A2A
	ldrh r1, [r6, #0xA]
	add r4, sp, #4
	add r0, r5, #0
	add r2, r4, #0
	bl sub_08009C08
	cmp r0, #0
	beq _080339BE
	add r0, r5, #0
	add r1, sp, #4
	bl sub_08009C64
	cmp r0, #0
	beq _080339BE
	mov r0, #1
	ldrb r1, [r7, #2]
	and r0, r1
	mov r3, #0xD3
	cmp r0, #0
	beq _08033990
	ldr r3, _08033A5C @ =0x000080D3
_08033990:
	ldr r2, [sp, #4]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r4, #2]
	mov r0, #0x21
	neg r0, r0
	and r0, r1
	strb r0, [r4, #2]
	ldrb r6, [r6, #8]
	lsl r3, r6, #0x1B
	lsr r3, r3, #0x1F
	mov r0, #0x20
	str r0, [sp, #0]
	add r0, r5, #0
	add r1, sp, #4
	mov r2, #1
	bl sub_08055F70
_080339BE:
	mov r2, #1
	mov r9, r2
	sub r4, r2, r5
	ldr r1, _08033A50 @ =0x02018450
	mov r2, sl
	add r0, r2, r5
	lsl r0, r0, #2
	add r0, r0, r1
	mov r8, r0
	ldrh r1, [r0, #0xA]
	add r6, sp, #4
	add r0, r4, #0
	add r2, r6, #0
	bl sub_08009C08
	cmp r0, #0
	beq _08033A2A
	add r0, r4, #0
	add r1, sp, #4
	bl sub_08009C64
	cmp r0, #0
	beq _08033A2A
	mov r0, r9
	ldrb r7, [r7, #2]
	and r0, r7
	mov r3, #0xD3
	cmp r0, #0
	bne _080339FA
	ldr r3, _08033A5C @ =0x000080D3
_080339FA:
	ldr r2, [sp, #4]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r6, #2]
	mov r0, #0x21
	neg r0, r0
	and r0, r1
	strb r0, [r6, #2]
	mov r0, r8
	ldrb r0, [r0, #8]
	lsl r3, r0, #0x1B
	lsr r3, r3, #0x1F
	mov r0, #0x20
	str r0, [sp, #0]
	add r0, r5, #0
	add r1, sp, #4
	mov r2, #1
	bl sub_08055F70
_08033A2A:
	ldr r0, _08033A50 @ =0x02018450
	mov r2, sl
	add r1, r2, r5
	lsl r1, r1, #2
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1, #8]
	and r0, r2
	strb r0, [r1, #8]
	ldr r0, _08033A60 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	sub r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08033A66
_08033A50: .4byte 0x02018450
_08033A54: .4byte 0x000007FF
_08033A58: .4byte gUnk_08622AB4
_08033A5C: .4byte 0x000080D3
_08033A60: .4byte 0x02017A40
_08033A64:
	mov r0, #0
_08033A66:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080338CC
	.align 2, 0

