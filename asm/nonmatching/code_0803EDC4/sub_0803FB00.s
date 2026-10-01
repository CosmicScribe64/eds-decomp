	thumb_func_start sub_0803FB00
sub_0803FB00: @ 0x0803FB00
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _0803FB50 @ =0x02017A40
	ldr r1, _0803FB54 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _0803FB78
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	mov r0, #1
	ldrb r1, [r5, #2]
	and r0, r1
	cmp r0, #0
	beq _0803FB58
	mov r4, #0
_0803FB26:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	lsl r1, r1, #0x18
	lsl r0, r4, #0x18
	lsr r1, r1, #8
	orr r1, r0
	lsr r1, r1, #0x10
	add r0, r5, #0
	bl sub_0802BE70
	cmp r0, #0
	bne _0803FBC8
	add r4, #1
	cmp r4, #4
	ble _0803FB26
	mov r0, #1
	b _0803FBE6
	.align 2, 0
_0803FB50: .4byte 0x02017A40
_0803FB54: .4byte 0x000003E5
_0803FB58:
	ldr r0, _0803FB6C @ =0x00000206
	ldr r1, _0803FB70 @ =0x00000712
	ldr r3, _0803FB74 @ =0x08084420
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0803FBE4
_0803FB6C: .4byte 0x00000206
_0803FB70: .4byte 0x00000712
_0803FB74: .4byte gUnk_08084420
_0803FB78:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0803FBE4
	ldr r0, _0803FBC0 @ =0x0201CFB0
	ldr r2, _0803FBC4 @ =0x00000824
	add r1, r0, r2
	ldr r6, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r2, [r0]
	add r4, r1, r2
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r1, r1, #8
	orr r1, r2
	lsr r1, r1, #0x10
	add r0, r5, #0
	bl sub_0802BE70
	cmp r0, #0
	beq _0803FBDE
	add r0, r5, #0
	add r1, r6, #0
	b _0803FBD4
	.align 2, 0
_0803FBC0: .4byte 0x0201CFB0
_0803FBC4: .4byte 0x00000824
_0803FBC8:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	add r0, r5, #0
_0803FBD4:
	add r2, r4, #0
	bl sub_0803DDAC
	mov r0, #1
	b _0803FBE6
_0803FBDE:
	mov r0, #3
	bl sub_08077AEC
_0803FBE4:
	mov r0, #0
_0803FBE6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0803FB00

