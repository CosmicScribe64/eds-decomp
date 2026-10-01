	thumb_func_start sub_08041C60
sub_08041C60: @ 0x08041C60
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08041C90 @ =0x02017A40
	ldr r1, _08041C94 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08041CA4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldr r0, _08041C98 @ =0x00000206
	ldr r1, _08041C9C @ =0x00000712
	ldr r3, _08041CA0 @ =0x08084CEC
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _08041CF8
	.align 2, 0
_08041C90: .4byte 0x02017A40
_08041C94: .4byte 0x000003E5
_08041C98: .4byte 0x00000206
_08041C9C: .4byte 0x00000712
_08041CA0: .4byte gUnk_08084CEC
_08041CA4:
	ldr r1, _08041CB8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041CBC
	mov r0, #0
	strb r0, [r5]
	b _08041CFA
	.align 2, 0
_08041CB8: .4byte 0x03000040
_08041CBC:
	ldr r0, _08041CEC @ =0x00020002
	bl sub_08052F38
	cmp r0, #0
	beq _08041CF8
	ldr r0, _08041CF0 @ =0x0201CFB0
	ldr r3, _08041CF4 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041CF8
	mov r0, #1
	b _08041CFA
_08041CEC: .4byte 0x00020002
_08041CF0: .4byte 0x0201CFB0
_08041CF4: .4byte 0x00000824
_08041CF8:
	mov r0, #0
_08041CFA:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08041C60

