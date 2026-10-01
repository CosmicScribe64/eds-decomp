	thumb_func_start sub_08064538
sub_08064538: @ 0x08064538
	push {r4, lr}
	bl sub_08064370
	ldr r4, _0806455C @ =0x02020310
	ldr r0, [r4, #0x10]
	cmp r0, #0
	bne _0806459E
	ldr r0, _08064560 @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08064564
	mov r0, #1
	bl sub_08077AEC
	mov r0, #1
	b _080645A0
_0806455C: .4byte 0x02020310
_08064560: .4byte 0x03000040
_08064564:
	mov r0, #0x20
	and r0, r1
	cmp r0, #0
	beq _0806457E
	ldr r0, [r4, #8]
	cmp r0, #0
	ble _0806457E
	mov r0, #0
	bl sub_08077AEC
	ldr r0, [r4, #8]
	sub r0, #1
	str r0, [r4, #8]
_0806457E:
	ldr r1, _080645A8 @ =0x03000040
	mov r0, #0x10
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0806459E
	ldr r4, _080645AC @ =0x02020310
	ldr r0, [r4, #8]
	cmp r0, #1
	bgt _0806459E
	mov r0, #0
	bl sub_08077AEC
	ldr r0, [r4, #8]
	add r0, #1
	str r0, [r4, #8]
_0806459E:
	mov r0, #0
_080645A0:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_080645A8: .4byte 0x03000040
_080645AC: .4byte 0x02020310
	thumb_func_end sub_08064538

