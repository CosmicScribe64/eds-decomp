	thumb_func_start sub_08041D00
sub_08041D00: @ 0x08041D00
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r0, _08041D30 @ =0x02017A40
	ldr r1, _08041D34 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08041D44
	mov r0, #8
	neg r0, r0
	ldrb r3, [r4, #0xA]
	and r0, r3
	strb r0, [r4, #0xA]
	ldr r0, _08041D38 @ =0x00000206
	ldr r1, _08041D3C @ =0x00000712
	ldr r3, _08041D40 @ =0x08084D20
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _08041DBA
	.align 2, 0
_08041D30: .4byte 0x02017A40
_08041D34: .4byte 0x000003E5
_08041D38: .4byte 0x00000206
_08041D3C: .4byte 0x00000712
_08041D40: .4byte gUnk_08084D20
_08041D44:
	ldr r1, _08041D58 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041D5C
	mov r0, #0
	strb r0, [r5]
	b _08041DBC
	.align 2, 0
_08041D58: .4byte 0x03000040
_08041D5C:
	ldr r0, _08041DA4 @ =0x00F000F0
	bl sub_08052F38
	cmp r0, #0
	beq _08041DBA
	ldr r0, _08041DA8 @ =0x0201CFB0
	ldr r1, _08041DAC @ =0x00000824
	add r2, r0, r1
	ldr r3, _08041DB0 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	ldr r6, [r2]
	lsl r1, r5, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r4, #0
	bl sub_0802C674
	cmp r0, #0
	beq _08041DB4
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041DB4
	mov r0, #1
	b _08041DBC
	.align 2, 0
_08041DA4: .4byte 0x00F000F0
_08041DA8: .4byte 0x0201CFB0
_08041DAC: .4byte 0x00000824
_08041DB0: .4byte 0x00000828
_08041DB4:
	mov r0, #3
	bl sub_08077AEC
_08041DBA:
	mov r0, #0
_08041DBC:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08041D00
	.align 2, 0

