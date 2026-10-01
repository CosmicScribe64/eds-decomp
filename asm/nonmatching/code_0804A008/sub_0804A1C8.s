	thumb_func_start sub_0804A1C8
sub_0804A1C8: @ 0x0804A1C8
	push {r4, r5, r6, lr}
	ldr r0, _0804A1E4 @ =0x020192E0
	ldr r1, _0804A1E8 @ =0x00001B2C
	add r0, r0, r1
	ldrb r1, [r0]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _0804A1EC
	bl sub_0801DC04
	mov r0, #1
	b _0804A394
	.align 2, 0
_0804A1E4: .4byte 0x020192E0
_0804A1E8: .4byte 0x00001B2C
_0804A1EC:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0804A1FC
	bl sub_0801E260
	mov r0, #1
	b _0804A394
_0804A1FC:
	bl sub_0805304C
	cmp r0, #0
	bne _0804A206
	b _0804A392
_0804A206:
	ldr r0, _0804A22C @ =0x0201CFB0
	ldr r2, _0804A230 @ =0x00000824
	add r1, r0, r2
	ldr r5, [r1]
	ldr r3, _0804A234 @ =0x00000828
	add r0, r0, r3
	ldr r4, [r0]
	bl sub_0805ECFC
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r4, #0xF
	bls _0804A222
	b _0804A392
_0804A222:
	lsl r0, r4, #2
	ldr r1, _0804A238 @ =0x0804A23C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0804A22C: .4byte 0x0201CFB0
_0804A230: .4byte 0x00000824
_0804A234: .4byte 0x00000828
_0804A238: .4byte 0x0804A23C
_0804A23C:
	.4byte _0804A27C
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A27C
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A392
	.4byte _0804A27C
	.4byte _0804A2EC
	.4byte _0804A32C
	.4byte _0804A32C
	.4byte _0804A380
	.4byte _0804A380
_0804A27C:
	cmp r6, #0
	beq _0804A2E8
	ldr r3, _0804A2C8 @ =0x0201CFB0
	ldr r4, _0804A2CC @ =0x00000824
	add r0, r3, r4
	ldr r2, [r0]
	cmp r2, #0
	beq _0804A2B6
	mov r0, #1
	and r2, r0
	ldr r1, _0804A2D0 @ =0x00000828
	add r0, r3, r1
	ldr r1, [r0]
	add r4, #8
	add r0, r3, r4
	ldr r0, [r0]
	add r1, r1, r0
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0804A2D4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0804A2D8 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804A2E8
_0804A2B6:
	ldr r1, _0804A2DC @ =0x020192E0
	ldr r0, _0804A2E0 @ =0x00001B2C
	add r4, r1, r0
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r3, _0804A2E4 @ =0x00001B2F
	b _0804A340
_0804A2C8: .4byte 0x0201CFB0
_0804A2CC: .4byte 0x00000824
_0804A2D0: .4byte 0x00000828
_0804A2D4: .4byte 0x00000D64
_0804A2D8: .4byte 0x0201930C
_0804A2DC: .4byte 0x020192E0
_0804A2E0: .4byte 0x00001B2C
_0804A2E4: .4byte 0x00001B2F
_0804A2E8:
	mov r0, #3
	b _0804A38E
_0804A2EC:
	ldr r0, _0804A318 @ =0x0201CFB0
	ldr r3, _0804A31C @ =0x00000824
	add r0, r0, r3
	ldr r0, [r0]
	cmp r0, #0
	beq _0804A300
	bl sub_0800A368
	cmp r0, #0
	beq _0804A2E8
_0804A300:
	cmp r6, #0
	beq _0804A2E8
	ldr r1, _0804A320 @ =0x020192E0
	ldr r0, _0804A324 @ =0x00001B2C
	add r4, r1, r0
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r3, _0804A328 @ =0x00001B2F
	b _0804A340
	.align 2, 0
_0804A318: .4byte 0x0201CFB0
_0804A31C: .4byte 0x00000824
_0804A320: .4byte 0x020192E0
_0804A324: .4byte 0x00001B2C
_0804A328: .4byte 0x00001B2F
_0804A32C:
	cmp r5, #0
	bne _0804A2E8
	ldr r1, _0804A370 @ =0x020192E0
	ldr r3, _0804A374 @ =0x00001B2C
	add r4, r1, r3
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	add r3, #3
_0804A340:
	add r2, r1, r3
	mov r0, #3
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _0804A378 @ =0x00001B30
	add r1, r1, r0
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bl sub_0804A008
	lsl r0, r0, #0x10
	lsr r0, r0, #6
	ldr r1, [r4]
	ldr r2, _0804A37C @ =0xFC0003FF
	and r1, r2
	orr r1, r0
	str r1, [r4]
	mov r0, #1
	b _0804A394
	.align 2, 0
_0804A370: .4byte 0x020192E0
_0804A374: .4byte 0x00001B2C
_0804A378: .4byte 0x00001B30
_0804A37C: .4byte 0xFC0003FF
_0804A380:
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_0802AF34
	mov r0, #1
_0804A38E:
	bl sub_08077AEC
_0804A392:
	mov r0, #0
_0804A394:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0804A1C8
	.align 2, 0

