	thumb_func_start sub_08002D48
sub_08002D48: @ 0x08002D48
	push {r4, r5, r6, lr}
	ldr r5, _08002DB8 @ =0x03000040
	ldr r0, _08002DBC @ =0x0000485A
	add r6, r5, r0
	ldrb r0, [r6]
	cmp r0, #0
	beq _08002DD4
	cmp r0, #1
	beq _08002E46
	ldr r4, _08002DC0 @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #1
	bl sub_080034B8
	ldr r1, _08002DC4 @ =0x00004832
	add r0, r5, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	bl sub_08002D34
	ldr r3, _08002DC8 @ =0x0819834C
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
	mov r0, #3
	bl sub_08075AE4
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08002E60
	mov r0, #0
	bl sub_08002D34
	ldr r0, _08002DCC @ =0x00004859
	add r1, r5, r0
	mov r2, #0
	mov r0, #2
	strb r0, [r1]
	strb r2, [r6]
	ldr r1, _08002DD0 @ =0x0000485B
	add r0, r5, r1
	strb r2, [r0]
	b _08002E60
_08002DB8: .4byte 0x03000040
_08002DBC: .4byte 0x0000485A
_08002DC0: .4byte 0x0201F7E0
_08002DC4: .4byte 0x00004832
_08002DC8: .4byte gUnk_0819834C
_08002DCC: .4byte 0x00004859
_08002DD0: .4byte 0x0000485B
_08002DD4:
	ldr r4, _08002E30 @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #1
	bl sub_080034B8
	ldr r3, _08002E34 @ =0x0819834C
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
	ldr r0, _08002E38 @ =0x00004832
	add r4, r5, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1A
	neg r0, r0
	bl sub_08002D34
	mov r0, #3
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08002E3C
	ldrb r4, [r4]
	lsl r0, r4, #0x1A
	lsr r0, r0, #0x1A
	neg r0, r0
	bl sub_08003174
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _08002E70
	.align 2, 0
_08002E30: .4byte 0x0201F7E0
_08002E34: .4byte gUnk_0819834C
_08002E38: .4byte 0x00004832
_08002E3C:
	ldrb r4, [r4]
	lsl r0, r4, #0x1A
	lsr r0, r0, #0x1A
	neg r0, r0
	b _08002E6C
_08002E46:
	ldr r0, _08002E5C @ =0x0201F7E0
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1D
	sub r0, #1
	bl sub_08003298
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _08002E70
_08002E5C: .4byte 0x0201F7E0
_08002E60:
	ldr r0, _08002E78 @ =0x03000040
	ldr r1, _08002E7C @ =0x00004832
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
_08002E6C:
	bl sub_08003174
_08002E70:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08002E78: .4byte 0x03000040
_08002E7C: .4byte 0x00004832
	thumb_func_end sub_08002D48

