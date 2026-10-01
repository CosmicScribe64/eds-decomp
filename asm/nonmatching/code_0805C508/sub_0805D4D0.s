	thumb_func_start sub_0805D4D0
sub_0805D4D0: @ 0x0805D4D0
	push {lr}
	mov r2, #1
	ldr r0, _0805D4EC @ =0x02015F00
	ldr r1, _0805D4F0 @ =0x00001B24
	add r0, r0, r1
	ldrb r0, [r0]
	lsr r0, r0, #1
	cmp r0, #8
	bhi _0805D554
	lsl r0, r0, #2
	ldr r1, _0805D4F4 @ =0x0805D4F8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0805D4EC: .4byte 0x02015F00
_0805D4F0: .4byte 0x00001B24
_0805D4F4: .4byte 0x0805D4F8
_0805D4F8:
	.4byte _0805D51C
	.4byte _0805D522
	.4byte _0805D528
	.4byte _0805D52E
	.4byte _0805D534
	.4byte _0805D53A
	.4byte _0805D540
	.4byte _0805D546
	.4byte _0805D54C
_0805D51C:
	bl sub_0805C0A0
	b _0805D550
_0805D522:
	bl sub_0805C508
	b _0805D550
_0805D528:
	bl sub_0805C938
	b _0805D550
_0805D52E:
	bl sub_0805CB2C
	b _0805D550
_0805D534:
	bl sub_0805CDA4
	b _0805D550
_0805D53A:
	bl sub_0805D4B4
	b _0805D550
_0805D540:
	bl sub_0805CEAC
	b _0805D550
_0805D546:
	bl sub_0805D080
	b _0805D550
_0805D54C:
	bl sub_0805D254
_0805D550:
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_0805D554:
	ldr r0, _0805D56C @ =0x02015F00
	ldr r1, _0805D570 @ =0x00001B24
	add r0, r0, r1
	mov r3, #1
	add r1, r3, #0
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _0805D574
	add r0, r2, #0
	b _0805D582
	.align 2, 0
_0805D56C: .4byte 0x02015F00
_0805D570: .4byte 0x00001B24
_0805D574:
	ldr r0, _0805D588 @ =0x02015EF0
	strb r3, [r0, #1]
	strb r1, [r0, #2]
	strb r1, [r0, #3]
	strb r1, [r0, #4]
	strb r1, [r0, #5]
	mov r0, #0
_0805D582:
	pop {r1}
	bx r1
	.align 2, 0
_0805D588: .4byte 0x02015EF0
	thumb_func_end sub_0805D4D0

