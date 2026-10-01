	thumb_func_start sub_080645B0
sub_080645B0: @ 0x080645B0
	push {r4, lr}
	ldr r4, _080645D4 @ =0x02020310
	ldr r0, [r4]
	cmp r0, #0x3B
	bgt _080645D8
	asr r0, r0, #2
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _080645C8
	bl sub_08064370
_080645C8:
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	mov r0, #0
	b _080645FA
	.align 2, 0
_080645D4: .4byte 0x02020310
_080645D8:
	bl sub_08064370
	mov r0, #2
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080645EC
	mov r0, #0
	b _080645FA
_080645EC:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08064600 @ =0x0000EEFF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_080645FA:
	pop {r4}
	pop {r1}
	bx r1
_08064600: .4byte 0x0000EEFF
	thumb_func_end sub_080645B0

