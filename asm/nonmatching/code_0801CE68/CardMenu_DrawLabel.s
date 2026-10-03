	thumb_func_start CardMenu_DrawLabel
CardMenu_DrawLabel: @ 0x0801D960
	push {r4, r5, r6, r7, lr}
	mov r1, #0
	mov r4, #0
	ldr r6, _0801D99C @ =0x0201CFB0
	ldr r0, _0801D9A0 @ =0x0201AE0C
	ldr r0, [r0]
	lsl r2, r0, #6
	mov r3, #1
_0801D970:
	lsr r0, r2, #0x10
	asr r0, r4
	and r0, r3
	cmp r0, #0
	beq _0801D97C
	add r1, #1
_0801D97C:
	add r4, #1
	cmp r4, #0xC
	ble _0801D970
	ldr r7, _0801D9A4 @ =0x00003664
	lsl r1, r1, #3
	mov r0, #0x70
	sub r5, r0, r1
	ldr r2, _0801D9A8 @ =0x00000828
	add r0, r6, r2
	ldr r0, [r0]
	cmp r0, #0xC
	beq _0801D9AC
	cmp r0, #0xD
	bne _0801D9B0
	mov r0, #0xD0
	b _0801D9AE
_0801D99C: .4byte 0x0201CFB0
_0801D9A0: .4byte 0x0201AE0C
_0801D9A4: .4byte 0x00003664
_0801D9A8: .4byte 0x00000828
_0801D9AC:
	mov r0, #0x20
_0801D9AE:
	sub r5, r0, r1
_0801D9B0:
	mov r4, #0
	ldr r6, _0801D9F4 @ =0x0201AE0C
_0801D9B4:
	ldr r0, [r6]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	asr r0, r4
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0801D9E0
	ldrb r1, [r6]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, r4
	bne _0801D9DE
	mov r0, #0xC0
	lsl r0, r0, #0xF
	orr r0, r5
	mov r1, #0x81
	lsl r1, r1, #7
	add r2, r7, #0
	bl AddSprite
_0801D9DE:
	add r5, #0x10
_0801D9E0:
	add r0, r7, #0
	add r0, #8
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	add r4, #1
	cmp r4, #0xC
	ble _0801D9B4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0801D9F4: .4byte 0x0201AE0C
	thumb_func_end CardMenu_DrawLabel

