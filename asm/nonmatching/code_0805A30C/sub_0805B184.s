	thumb_func_start sub_0805B184
sub_0805B184: @ 0x0805B184
	push {r4, r5, r6, r7, lr}
	ldr r4, _0805B19C @ =0x02015EF0
	ldrb r0, [r4, #6]
	add r1, r4, #0
	cmp r0, #1
	beq _0805B1B2
	cmp r0, #1
	bgt _0805B1A0
	cmp r0, #0
	beq _0805B1A8
	b _0805B3EA
	.align 2, 0
_0805B19C: .4byte 0x02015EF0
_0805B1A0:
	cmp r0, #2
	bne _0805B1A6
	b _0805B3D4
_0805B1A6:
	b _0805B3EA
_0805B1A8:
	strb r0, [r4, #4]
	strb r0, [r4, #5]
	ldrb r0, [r4, #6]
	add r0, #1
	strb r0, [r4, #6]
_0805B1B2:
	add r5, r1, #0
	ldrb r0, [r5, #5]
	cmp r0, #4
	bls _0805B1BC
	b _0805B3EA
_0805B1BC:
	ldrb r3, [r5, #5]
	mov r7, #0x94
	add r0, r3, #0
	mul r0, r7
	ldr r6, _0805B1D8 @ =0x0201A070
	add r2, r0, r6
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	bne _0805B1DC
	add r0, r3, #1
	b _0805B204
	.align 2, 0
_0805B1D8: .4byte 0x0201A070
_0805B1DC:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0805B1EA
	add r0, r3, #1
	b _0805B204
_0805B1EA:
	ldr r4, _0805B20C @ =0x000007FF
	and r1, r4
	lsl r0, r1, #1
	ldr r1, _0805B210 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	bne _0805B214
	ldrb r0, [r5, #5]
	add r0, #1
_0805B204:
	strb r0, [r5, #5]
	mov r0, #0
	b _0805B3EC
	.align 2, 0
_0805B20C: .4byte 0x000007FF
_0805B210: .4byte gUnk_08622AB4
_0805B214:
	ldrb r2, [r5, #5]
	add r0, r2, #0
	mul r0, r7
	add r0, r0, r6
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0805B254 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805B258 @ =0x00000231
	cmp r1, r0
	beq _0805B2F8
	cmp r1, r0
	bgt _0805B294
	ldr r0, _0805B25C @ =0x00000109
	cmp r1, r0
	bne _0805B23E
	b _0805B374
_0805B23E:
	cmp r1, r0
	bgt _0805B26E
	cmp r1, #0x53
	bne _0805B248
	b _0805B380
_0805B248:
	cmp r1, #0x53
	bgt _0805B260
	cmp r1, #0x27
	bne _0805B252
	b _0805B358
_0805B252:
	b _0805B3C4
_0805B254: .4byte gUnk_08622AB4
_0805B258: .4byte 0x00000231
_0805B25C: .4byte 0x00000109
_0805B260:
	cmp r1, #0x65
	bne _0805B266
	b _0805B3AE
_0805B266:
	cmp r1, #0xDF
	bne _0805B26C
	b _0805B380
_0805B26C:
	b _0805B3C4
_0805B26E:
	mov r0, #0xFA
	lsl r0, r0, #1
	cmp r1, r0
	beq _0805B324
	cmp r1, r0
	bgt _0805B284
	sub r0, #0x49
	cmp r1, r0
	bne _0805B282
	b _0805B3A8
_0805B282:
	b _0805B3C4
_0805B284:
	ldr r0, _0805B290 @ =0x0000021B
	cmp r1, r0
	beq _0805B304
	add r0, #1
	b _0805B2C0
	.align 2, 0
_0805B290: .4byte 0x0000021B
_0805B294:
	ldr r0, _0805B2B4 @ =0x00000262
	cmp r1, r0
	beq _0805B38A
	cmp r1, r0
	bgt _0805B2CC
	sub r0, #0x19
	cmp r1, r0
	bne _0805B2A6
	b _0805B39C
_0805B2A6:
	cmp r1, r0
	bgt _0805B2B8
	sub r0, #3
	cmp r1, r0
	beq _0805B364
	b _0805B3C4
	.align 2, 0
_0805B2B4: .4byte 0x00000262
_0805B2B8:
	ldr r0, _0805B2C8 @ =0x0000024E
	cmp r1, r0
	beq _0805B304
	add r0, #0xB
_0805B2C0:
	cmp r1, r0
	beq _0805B324
	b _0805B3C4
	.align 2, 0
_0805B2C8: .4byte 0x0000024E
_0805B2CC:
	ldr r0, _0805B2E0 @ =0x000002FA
	cmp r1, r0
	beq _0805B38A
	cmp r1, r0
	bgt _0805B2E4
	sub r0, #0x7A
	cmp r1, r0
	beq _0805B342
	b _0805B3C4
	.align 2, 0
_0805B2E0: .4byte 0x000002FA
_0805B2E4:
	ldr r0, _0805B2F4 @ =0x00000452
	cmp r1, r0
	beq _0805B32C
	add r0, #0x39
	cmp r1, r0
	beq _0805B32C
	b _0805B3C4
	.align 2, 0
_0805B2F4: .4byte 0x00000452
_0805B2F8:
	mov r0, #0
	bl sub_08008860
	cmp r0, #0
	bne _0805B3C4
	b _0805B3BA
_0805B304:
	ldr r1, _0805B31C @ =0x020192E4
	ldr r2, _0805B320 @ =0x00000D66
	add r0, r1, r2
	ldrb r2, [r0]
	cmp r2, #2
	bls _0805B38A
	add r0, r2, #0
	add r0, #2
	ldrb r1, [r1, #2]
	cmp r1, r0
	ble _0805B3C4
	b _0805B38A
_0805B31C: .4byte 0x020192E4
_0805B320: .4byte 0x00000D66
_0805B324:
	mov r0, #0
	bl sub_08008860
	b _0805B386
_0805B32C:
	mov r0, #1
	bl sub_08008860
	cmp r0, #1
	bne _0805B3C4
	mov r0, #0
	bl sub_08008860
	cmp r0, #1
	ble _0805B3C4
	b _0805B38A
_0805B342:
	mov r0, #0
	bl sub_08008860
	add r4, r0, #0
	mov r0, #1
	bl sub_08008860
	add r0, #1
	cmp r4, r0
	ble _0805B3C4
	b _0805B3BA
_0805B358:
	mov r1, #0xA4
	lsl r1, r1, #1
	mov r0, #0
	bl sub_080090C8
	b _0805B3B6
_0805B364:
	ldr r1, _0805B370 @ =0x0000015B
	mov r0, #0
	bl sub_080090C8
	b _0805B3B6
	.align 2, 0
_0805B370: .4byte 0x0000015B
_0805B374:
	mov r0, #0
	bl sub_080091B4
	cmp r0, #1
	ble _0805B3C4
	b _0805B3BA
_0805B380:
	mov r0, #0
	bl sub_08009280
_0805B386:
	cmp r0, #0
	ble _0805B3C4
_0805B38A:
	ldr r1, _0805B398 @ =0x02015EF0
	ldrb r0, [r1, #6]
	add r0, #1
	strb r0, [r1, #6]
	mov r0, #0
	b _0805B3EC
	.align 2, 0
_0805B398: .4byte 0x02015EF0
_0805B39C:
	mov r0, #0
	bl sub_08009280
	cmp r0, #1
	ble _0805B3C4
	b _0805B3BA
_0805B3A8:
	mov r0, #1
	mov r1, #0x16
	b _0805B3B2
_0805B3AE:
	mov r0, #1
	mov r1, #0x15
_0805B3B2:
	bl sub_08009150
_0805B3B6:
	cmp r0, #0
	ble _0805B3C4
_0805B3BA:
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
	mov r0, #0
	b _0805B3EC
_0805B3C4:
	ldr r1, _0805B3D0 @ =0x02015EF0
	ldrb r0, [r1, #5]
	add r0, #1
	strb r0, [r1, #5]
	mov r0, #0
	b _0805B3EC
_0805B3D0: .4byte 0x02015EF0
_0805B3D4:
	ldrb r1, [r4, #5]
	mov r0, #1
	bl sub_08055EB0
	ldrb r0, [r4, #5]
	add r0, #1
	strb r0, [r4, #5]
	mov r0, #1
	strb r0, [r4, #6]
	mov r0, #0
	b _0805B3EC
_0805B3EA:
	mov r0, #1
_0805B3EC:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0805B184
	.align 2, 0

