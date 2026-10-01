	thumb_func_start sub_0806B190
sub_0806B190: @ 0x0806B190
	push {r4, lr}
	bl sub_08068434
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806B19E
	b _0806B2E8
_0806B19E:
	ldr r0, _0806B1BC @ =0x0201DB20
	ldr r2, _0806B1C0 @ =0x00001C5A
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1B
	lsr r1, r1, #0x1D
	add r4, r0, #0
	cmp r1, #4
	bls _0806B1B2
	b _0806B2E8
_0806B1B2:
	lsl r0, r1, #2
	ldr r1, _0806B1C4 @ =0x0806B1C8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0806B1BC: .4byte 0x0201DB20
_0806B1C0: .4byte 0x00001C5A
_0806B1C4: .4byte 0x0806B1C8
_0806B1C8:
	.4byte _0806B2E8
	.4byte _0806B1DC
	.4byte _0806B2E8
	.4byte _0806B2A0
	.4byte _0806B2C4
_0806B1DC:
	ldr r4, _0806B1F4 @ =0x0201DB20
	ldr r0, _0806B1F8 @ =0x00001C3C
	add r2, r4, r0
	ldr r1, [r2]
	lsl r0, r1, #0xE
	lsr r3, r0, #0x1D
	cmp r3, #2
	beq _0806B1FC
	cmp r3, #3
	beq _0806B23C
	b _0806B26E
	.align 2, 0
_0806B1F4: .4byte 0x0201DB20
_0806B1F8: .4byte 0x00001C3C
_0806B1FC:
	ldr r0, _0806B230 @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	ldr r1, _0806B234 @ =0x00001C3D
	add r2, r4, r1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _0806B238 @ =0x00001C1C
	add r4, r4, r2
	strb r3, [r4]
	mov r0, #2
	bl sub_080671E8
	ldrb r0, [r4]
	bl sub_080679A8
	b _0806B26E
	.align 2, 0
_0806B230: .4byte 0xFFFC7FFF
_0806B234: .4byte 0x00001C3D
_0806B238: .4byte 0x00001C1C
_0806B23C:
	ldr r0, _0806B28C @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	ldr r0, _0806B290 @ =0x00001C3D
	add r2, r4, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _0806B294 @ =0x00001C1C
	add r4, r4, r2
	mov r0, #1
	strb r0, [r4]
	mov r0, #2
	bl sub_080671E8
	ldrb r0, [r4]
	bl sub_080679A8
_0806B26E:
	ldr r2, _0806B298 @ =0x0201DB20
	ldr r0, _0806B29C @ =0x00001C5A
	add r2, r2, r0
	ldrb r3, [r2]
	lsl r1, r3, #0x1B
	lsr r1, r1, #0x1D
	add r1, #1
	mov r0, #7
	and r1, r0
	lsl r1, r1, #2
	mov r0, #0x1D
	neg r0, r0
	and r0, r3
	b _0806B2B6
	.align 2, 0
_0806B28C: .4byte 0xFFFC7FFF
_0806B290: .4byte 0x00001C3D
_0806B294: .4byte 0x00001C1C
_0806B298: .4byte 0x0201DB20
_0806B29C: .4byte 0x00001C5A
_0806B2A0:
	ldr r2, _0806B2BC @ =0x00001866
	add r1, r4, r2
	mov r0, #1
	strb r0, [r1]
	ldr r0, _0806B2C0 @ =0x00001C5A
	add r2, r4, r0
	mov r0, #0x1D
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0x10
_0806B2B6:
	orr r0, r1
	strb r0, [r2]
	b _0806B2E8
_0806B2BC: .4byte 0x00001866
_0806B2C0: .4byte 0x00001C5A
_0806B2C4:
	ldr r2, _0806B2F0 @ =0x00001866
	add r1, r4, r2
	ldrb r2, [r1]
	mov r0, #0
	ldsb r0, [r1, r0]
	cmp r0, #0
	bne _0806B2E8
	mov r0, #0xFF
	strb r0, [r1]
	bl sub_0806AFA4
	ldr r0, _0806B2F4 @ =0x00001C5A
	add r1, r4, r0
	mov r0, #0x1D
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0806B2E8:
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0806B2F0: .4byte 0x00001866
_0806B2F4: .4byte 0x00001C5A
	thumb_func_end sub_0806B190

