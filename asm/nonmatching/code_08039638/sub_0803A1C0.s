	thumb_func_start sub_0803A1C0
sub_0803A1C0: @ 0x0803A1C0
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803A1D0
	b _0803A2DC
_0803A1D0:
	ldr r0, _0803A208 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803A22C
	cmp r0, #0x80
	beq _0803A1E4
	b _0803A2DC
_0803A1E4:
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0
	bne _0803A218
	ldr r0, _0803A20C @ =0x00000206
	ldr r1, _0803A210 @ =0x00000613
	ldr r3, _0803A214 @ =0x08083558
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	b _0803A224
	.align 2, 0
_0803A208: .4byte 0x02017A40
_0803A20C: .4byte 0x00000206
_0803A210: .4byte 0x00000613
_0803A214: .4byte gUnk_08083558
_0803A218:
	bl sub_08076F9C
	ldr r2, _0803A228 @ =0x0201AE60
	mov r1, #1
	and r0, r1
	strh r0, [r2, #0x14]
_0803A224:
	mov r0, #0x7F
	b _0803A2DE
_0803A228: .4byte 0x0201AE60
_0803A22C:
	bl sub_08076F9C
	add r4, r0, #0
	mov r6, #1
	and r4, r6
	add r0, r6, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r3, #0xE0
	cmp r0, #0
	beq _0803A244
	ldr r3, _0803A2A4 @ =0x000080E0
_0803A244:
	ldr r7, _0803A2A8 @ =0x0201AE60
	ldrh r1, [r7, #0x14]
	add r2, r4, #0
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r6, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r1, #0x12
	cmp r0, #0
	beq _0803A260
	ldr r1, _0803A2AC @ =0x00008012
_0803A260:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrh r7, [r7, #0x14]
	cmp r4, r7
	bne _0803A2B8
	ldr r4, _0803A2B0 @ =0x020192E4
	ldrb r5, [r5, #2]
	lsl r2, r5, #0x1F
	lsr r1, r2, #0x1F
	add r0, r6, #0
	and r0, r1
	ldr r3, _0803A2B4 @ =0x00000D64
	mul r0, r3
	add r0, r0, r4
	ldrb r0, [r0, #2]
	cmp r0, #4
	bhi _0803A2D2
	add r0, r1, #0
	add r2, r0, #0
	add r1, r6, #0
	and r1, r2
	add r2, r1, #0
	mul r2, r3
	add r2, r2, r4
	mov r1, #5
	ldrb r2, [r2, #2]
	sub r1, r1, r2
	bl sub_080199E0
	b _0803A2D2
_0803A2A4: .4byte 0x000080E0
_0803A2A8: .4byte 0x0201AE60
_0803A2AC: .4byte 0x00008012
_0803A2B0: .4byte 0x020192E4
_0803A2B4: .4byte 0x00000D64
_0803A2B8:
	add r0, r6, #0
	ldrb r5, [r5, #2]
	and r0, r5
	mov r1, #0x46
	cmp r0, #0
	beq _0803A2C6
	ldr r1, _0803A2D8 @ =0x00008046
_0803A2C6:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0803A2D2:
	mov r0, #0xA
	b _0803A2DE
	.align 2, 0
_0803A2D8: .4byte 0x00008046
_0803A2DC:
	mov r0, #0
_0803A2DE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0803A1C0

