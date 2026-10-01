	thumb_func_start sub_08051DF4
sub_08051DF4: @ 0x08051DF4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	ldr r0, _08051E20 @ =0x020192E0
	ldr r1, _08051E24 @ =0x00001B62
	add r6, r0, r1
	ldrb r0, [r6]
	cmp r0, #0
	bne _08051E64
	mov r4, #1
	neg r4, r4
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08008AF8
	cmp r0, #0
	bne _08051E28
_08051E1A:
	mov r0, #1
	b _08051EC2
	.align 2, 0
_08051E20: .4byte 0x020192E0
_08051E24: .4byte 0x00001B62
_08051E28:
	cmp r5, #0
	beq _08051E42
	add r0, r4, #0
	mov r1, #1
	bl sub_080563B8
	add r1, r0, #0
	cmp r1, r4
	ble _08051E1A
	add r0, r5, #0
	bl sub_08017FF4
	b _08051E1A
_08051E42:
	ldr r0, _08051E58 @ =0x00000206
	ldr r1, _08051E5C @ =0x00000712
	ldr r3, _08051E60 @ =0x08085FF4
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _08051EC0
	.align 2, 0
_08051E58: .4byte 0x00000206
_08051E5C: .4byte 0x00000712
_08051E60: .4byte gUnk_08085FF4
_08051E64:
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _08051EC0
	ldr r0, _08051EB0 @ =0x0201CFB0
	ldr r1, _08051EB4 @ =0x00000824
	add r4, r0, r1
	ldr r1, [r4]
	mov r9, r1
	ldr r1, _08051EB8 @ =0x00000828
	add r6, r0, r1
	add r1, #4
	add r7, r0, r1
	ldr r1, [r6]
	ldr r0, [r7]
	add r1, r1, r0
	mov r8, r1
	mov r0, #1
	bl sub_08077AEC
	mov r0, #8
	cmp r5, #0
	beq _08051E96
	ldr r0, _08051EBC @ =0x00008008
_08051E96:
	ldrh r1, [r4]
	ldrb r7, [r7]
	lsl r2, r7, #8
	ldrb r6, [r6]
	orr r2, r6
	mov r3, #0
	bl sub_0801EC58
	mov r0, r9
	mov r1, r8
	bl sub_08017FF4
	b _08051E1A
_08051EB0: .4byte 0x0201CFB0
_08051EB4: .4byte 0x00000824
_08051EB8: .4byte 0x00000828
_08051EBC: .4byte 0x00008008
_08051EC0:
	mov r0, #0
_08051EC2:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08051DF4
	.align 2, 0

