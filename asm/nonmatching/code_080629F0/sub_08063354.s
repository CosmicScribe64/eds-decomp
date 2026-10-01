	thumb_func_start sub_08063354
sub_08063354: @ 0x08063354
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r0, #0
	mov r8, r0
	bl sub_0806245C
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08063372
	mov r0, #0
	b _08063466
_08063372:
	bl sub_080624A4
	mov r4, #0
	ldr r6, _080633AC @ =0x02015160
	mov r1, #0x86
	lsl r1, r1, #1
	add r7, r6, r1
	add r5, r7, #0
_08063382:
	ldrb r0, [r5]
	cmp r0, #0x17
	bhi _080633DA
	cmp r0, #2
	bne _08063392
	mov r0, #6
	bl sub_08077AEC
_08063392:
	ldr r1, _080633B0 @ =0x03000040
	mov r0, #3
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080633B4
	ldrb r2, [r5]
	cmp r2, #0x16
	bhi _080633B4
	mov r0, #0x17
	strb r0, [r5]
	b _080633DE
	.align 2, 0
_080633AC: .4byte 0x02015160
_080633B0: .4byte 0x03000040
_080633B4:
	cmp r4, #0
	beq _080633D2
	sub r0, r4, #1
	mov r2, #0x86
	lsl r2, r2, #1
	add r1, r6, r2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0xC
	bls _080633DE
	add r1, r4, r1
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _080633DE
_080633D2:
	ldrb r0, [r7]
	add r0, #1
	strb r0, [r7]
	b _080633DE
_080633DA:
	mov r0, #1
	add r8, r0
_080633DE:
	add r5, #1
	add r4, #1
	cmp r4, #4
	ble _08063382
	mov r4, #0
	ldr r5, _0806340C @ =0x02015160
	mov r1, #0x81
	lsl r1, r1, #1
	add r6, r5, r1
	ldr r7, _08063410 @ =0x0000FFFF
_080633F2:
	mov r2, #0x86
	lsl r2, r2, #1
	add r0, r5, r2
	add r0, r4, r0
	ldrb r0, [r0]
	cmp r0, #0x17
	bne _08063454
	ldrh r1, [r6]
	add r2, r1, #0
	cmp r1, r7
	bne _08063414
	mov r0, #0
	b _0806344A
_0806340C: .4byte 0x02015160
_08063410: .4byte 0x0000FFFF
_08063414:
	ldr r0, _0806342C @ =0x000007CF
	cmp r1, r0
	bhi _08063438
	ldr r2, _08063430 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08063434 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0806344A
	.align 2, 0
_0806342C: .4byte 0x000007CF
_08063430: .4byte 0x000007FF
_08063434: .4byte gUnk_08623DF4
_08063438:
	ldr r1, _08063470 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08063474 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08063478 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_0806344A:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	add r0, r4, #0
	bl sub_08062604
_08063454:
	add r6, #2
	add r4, #1
	cmp r4, #4
	ble _080633F2
	mov r0, #0
	mov r1, r8
	cmp r1, #4
	ble _08063466
	mov r0, #1
_08063466:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08063470: .4byte 0xFFFFF830
_08063474: .4byte 0x000007FF
_08063478: .4byte gUnk_08623DF4
	thumb_func_end sub_08063354

