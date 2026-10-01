	thumb_func_start sub_08021A48
sub_08021A48: @ 0x08021A48
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r0, _08021AD4 @ =0x08198F80
	ldr r5, _08021AD8 @ =0x02015EE8
	ldrb r2, [r5]
	lsl r1, r2, #2
	add r1, r1, r0
	ldr r0, [r1]
	cmp r0, #0
	bne _08021A62
	b _08021C88
_08021A62:
	bl sub_08024440
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0
	bne _08021AA6
	mov r0, #1
	ldrb r5, [r5, #1]
	and r0, r5
	cmp r0, #0
	beq _08021A8E
	ldr r0, _08021ADC @ =0x02017FB0
	ldr r3, _08021AE0 @ =0x00000307
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	cmp r0, #0
	beq _08021A8E
	bl sub_0801F628
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_08021A8E:
	cmp r4, #0
	bne _08021AA6
	bl sub_0801F454
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0
	bne _08021AA6
	bl sub_0802AED8
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
_08021AA6:
	ldr r1, _08021AE4 @ =0x0201CFB0
	mov r0, #6
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #6
	bne _08021AF0
	ldr r1, _08021AE8 @ =0x0201AE60
	mov r0, #1
	ldrb r5, [r1]
	and r0, r5
	cmp r0, #0
	beq _08021AF0
	add r0, r1, #0
	add r0, #0x20
	ldrb r0, [r0]
	cmp r0, #2
	bhi _08021AF0
	ldr r0, [r1, #0x18]
	cmp r0, #0
	beq _08021AEC
	bl _call_via_r0
	b _08021AF0
_08021AD4: .4byte gUnk_08198F80
_08021AD8: .4byte 0x02015EE8
_08021ADC: .4byte 0x02017FB0
_08021AE0: .4byte 0x00000307
_08021AE4: .4byte 0x0201CFB0
_08021AE8: .4byte 0x0201AE60
_08021AEC:
	bl sub_08060160
_08021AF0:
	cmp r4, #0
	beq _08021AF6
	b _08021C72
_08021AF6:
	bl sub_08060344
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021B02
	b _08021C72
_08021B02:
	bl sub_080222F8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021B0E
	b _08021C72
_08021B0E:
	bl sub_08055728
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021B1A
	b _08021C72
_08021B1A:
	bl sub_080213C0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021B26
	b _08021C72
_08021B26:
	mov r0, #1
	bl sub_0804325C
	bl sub_08042B60
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021B38
	b _08021C72
_08021B38:
	ldr r0, _08021B5C @ =0x02015EE8
	ldrb r0, [r0]
	cmp r0, #6
	bgt _08021C1A
	cmp r0, #3
	blt _08021C1A
	ldr r2, _08021B60 @ =0x020192E0
	ldr r0, _08021B64 @ =0x00001B14
	add r1, r2, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	add r7, r2, #0
	cmp r0, #0
	beq _08021B68
	bl sub_080617F8
	b _08021C72
_08021B5C: .4byte 0x02015EE8
_08021B60: .4byte 0x020192E0
_08021B64: .4byte 0x00001B14
_08021B68:
	ldr r0, _08021BC4 @ =0x02017FB0
	ldr r1, _08021BC8 @ =0x00000306
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	cmp r0, #0
	bge _08021C1A
	mov r6, #1
	ldr r2, _08021BCC @ =0x00001B2C
	add r2, r2, r7
	mov r8, r2
	ldrb r4, [r2]
	add r0, r6, #0
	and r0, r4
	cmp r0, #0
	beq _08021BDA
	ldr r3, _08021BD0 @ =0x00001B2F
	add r3, r3, r7
	mov ip, r3
	ldrb r3, [r3]
	lsr r1, r3, #2
	ldr r5, _08021BD4 @ =0x00001B30
	add r2, r7, r5
	mov r0, #3
	mov r9, r0
	ldrb r5, [r2]
	and r0, r5
	lsl r0, r0, #6
	orr r0, r1
	cmp r0, #2
	bne _08021BD8
	mov r0, #2
	neg r0, r0
	and r0, r4
	mov r1, r8
	strb r0, [r1]
	mov r0, r9
	and r0, r3
	mov r3, ip
	strb r0, [r3]
	mov r0, #4
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	strb r0, [r2]
	b _08021BDA
_08021BC4: .4byte 0x02017FB0
_08021BC8: .4byte 0x00000306
_08021BCC: .4byte 0x00001B2C
_08021BD0: .4byte 0x00001B2F
_08021BD4: .4byte 0x00001B30
_08021BD8:
	mov r6, #0
_08021BDA:
	ldr r0, _08021C40 @ =0x00001B2C
	add r1, r7, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08021BEA
	mov r6, #0
_08021BEA:
	cmp r6, #0
	beq _08021C12
	ldr r0, _08021C44 @ =0x00008041
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _08021C48 @ =0x0000F005
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r2, _08021C4C @ =0x00001B14
	add r1, r7, r2
	mov r0, #2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
_08021C12:
	bl sub_080611AC
	bl sub_080617F0
_08021C1A:
	ldr r1, _08021C50 @ =0x08198F80
	ldr r5, _08021C54 @ =0x02015EE8
	ldrb r2, [r5]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	bl _call_via_r0
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	bl sub_08021628
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #0
	beq _08021C58
	mov r2, #0
	mov r0, #9
	b _08021C60
_08021C40: .4byte 0x00001B2C
_08021C44: .4byte 0x00008041
_08021C48: .4byte 0x0000F005
_08021C4C: .4byte 0x00001B14
_08021C50: .4byte gUnk_08198F80
_08021C54: .4byte 0x02015EE8
_08021C58:
	cmp r4, #0
	beq _08021C72
	ldrb r0, [r5]
	add r0, #1
_08021C60:
	strb r0, [r5]
	ldr r0, _08021C80 @ =0x020192E0
	mov r3, #0xD9
	lsl r3, r3, #5
	add r1, r0, r3
	strb r2, [r1]
	ldr r5, _08021C84 @ =0x00001B21
	add r0, r0, r5
	strb r2, [r0]
_08021C72:
	bl sub_08061580
	bl sub_080616D0
	mov r0, #0
	b _08021CAA
	.align 2, 0
_08021C80: .4byte 0x020192E0
_08021C84: .4byte 0x00001B21
_08021C88:
	ldr r2, _08021CB8 @ =0x03000040
	ldr r0, _08021CBC @ =0x020192E0
	ldr r1, _08021CC0 @ =0x00001B12
	add r0, r0, r1
	ldr r3, _08021CC4 @ =0x00004870
	add r2, r2, r3
	ldrb r0, [r0]
	lsr r1, r0, #6
	lsl r1, r1, #6
	mov r0, #0x3F
	ldrb r5, [r2]
	and r0, r5
	orr r0, r1
	strb r0, [r2]
	bl sub_080754BC
	mov r0, #1
_08021CAA:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08021CB8: .4byte 0x03000040
_08021CBC: .4byte 0x020192E0
_08021CC0: .4byte 0x00001B12
_08021CC4: .4byte 0x00004870
	thumb_func_end sub_08021A48

