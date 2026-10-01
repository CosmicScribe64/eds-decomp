	thumb_func_start sub_0802F888
sub_0802F888: @ 0x0802F888
	push {r4, lr}
	add r3, r0, #0
	add r2, r1, #0
	mov r0, #0xFC
	ldrb r1, [r3, #3]
	and r0, r1
	cmp r0, #0x40
	bne _0802F8B0
	ldrb r4, [r3, #2]
	lsl r1, r4, #0x1F
	lsr r0, r1, #0x1F
	ldrb r4, [r3, #6]
	cmp r4, r0
	beq _0802F8B0
	bl sub_08008860
	mov r1, #0
	cmp r0, #1
	ble _0802F8DA
	b _0802F8D8
_0802F8B0:
	cmp r2, #0
	beq _0802F8C4
	mov r0, #1
	ldrb r3, [r3, #2]
	add r1, r0, #0
	ldrb r4, [r2, #2]
	and r1, r4
	and r0, r3
	cmp r1, r0
	bne _0802F8C8
_0802F8C4:
	mov r0, #0
	b _0802F8DC
_0802F8C8:
	ldr r0, _0802F8E4 @ =0x00000525
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	bl sub_0802F808
	mov r1, #0
	cmp r0, #0
	ble _0802F8DA
_0802F8D8:
	mov r1, #1
_0802F8DA:
	add r0, r1, #0
_0802F8DC:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802F8E4: .4byte 0x00000525
	thumb_func_end sub_0802F888

