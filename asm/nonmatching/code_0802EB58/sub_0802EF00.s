	thumb_func_start sub_0802EF00
sub_0802EF00: @ 0x0802EF00
	push {r4, r5, lr}
	add r5, r0, #0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08047170
	cmp r0, #0
	bne _0802EF18
	b _0802EF4A
_0802EF14:
	mov r0, #1
	b _0802EF4C
_0802EF18:
	mov r4, #0
_0802EF1A:
	add r0, r4, #0
	bl sub_08008A1C
	cmp r0, #0
	ble _0802EF44
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0802EF54 @ =0x000007FF
	add r1, r2, #0
	ldrh r2, [r5]
	and r1, r2
	lsl r1, r1, #1
	ldr r2, _0802EF58 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bne _0802EF14
_0802EF44:
	add r4, #1
	cmp r4, #1
	ble _0802EF1A
_0802EF4A:
	mov r0, #0
_0802EF4C:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0802EF54: .4byte 0x000007FF
_0802EF58: .4byte gUnk_08622AB4
	thumb_func_end sub_0802EF00

