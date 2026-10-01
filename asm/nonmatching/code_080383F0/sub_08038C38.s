	thumb_func_start sub_08038C38
sub_08038C38: @ 0x08038C38
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08038D3A
	ldr r0, _08038C7C @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08038CA0
	cmp r0, #0x80
	bne _08038D1C
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	bne _08038C8C
	ldr r0, _08038C80 @ =0x00000206
	ldr r1, _08038C84 @ =0x00000613
	ldr r3, _08038C88 @ =0x080831E4
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	b _08038C98
	.align 2, 0
_08038C7C: .4byte 0x02017A40
_08038C80: .4byte 0x00000206
_08038C84: .4byte 0x00000613
_08038C88: .4byte gUnk_080831E4
_08038C8C:
	bl sub_08076F9C
	ldr r2, _08038C9C @ =0x0201AE60
	mov r1, #1
	and r0, r1
	strh r0, [r2, #0x14]
_08038C98:
	mov r0, #0x7F
	b _08038D3C
_08038C9C: .4byte 0x0201AE60
_08038CA0:
	bl sub_08076F9C
	add r5, r0, #0
	mov r0, #1
	and r5, r0
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0xE0
	cmp r0, #0
	beq _08038CBA
	ldr r2, _08038D0C @ =0x000080E0
_08038CBA:
	ldr r7, _08038D10 @ =0x0201AE60
	ldrh r1, [r7, #0x14]
	add r0, r2, #0
	add r2, r5, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r6, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r1, #0x12
	cmp r0, #0
	beq _08038CD6
	ldr r1, _08038D14 @ =0x00008012
_08038CD6:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r6, r0, #0x1F
	ldrh r1, [r4]
	add r2, r6, #0
	ldrh r4, [r4, #2]
	lsl r0, r4, #0x16
	lsr r0, r0, #0x1A
	lsl r0, r0, #8
	orr r2, r0
	mov r3, #3
	ldrh r7, [r7, #0x14]
	cmp r5, r7
	bne _08038D00
	ldr r3, _08038D18 @ =0x00000103
_08038D00:
	add r0, r6, #0
	bl sub_08017AB4
	mov r0, #0xA
	b _08038D3C
	.align 2, 0
_08038D0C: .4byte 0x000080E0
_08038D10: .4byte 0x0201AE60
_08038D14: .4byte 0x00008012
_08038D18: .4byte 0x00000103
_08038D1C:
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _08038D2A
	ldr r2, _08038D44 @ =0x00008092
_08038D2A:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08038D3A:
	mov r0, #0
_08038D3C:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08038D44: .4byte 0x00008092
	thumb_func_end sub_08038C38

