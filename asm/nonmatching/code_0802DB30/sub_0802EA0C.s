	thumb_func_start sub_0802EA0C
sub_0802EA0C: @ 0x0802EA0C
	push {r4, lr}
	add r4, r0, #0
	add r3, r1, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802EA2C
	cmp r3, #0
	beq _0802EA2C
	mov r0, #1
	ldrb r2, [r4, #2]
	add r1, r0, #0
	ldrb r4, [r3, #2]
	and r1, r4
	and r0, r2
	cmp r1, r0
	bne _0802EA30
_0802EA2C:
	mov r0, #0
	b _0802EA46
_0802EA30:
	ldr r0, _0802EA4C @ =0x00000431
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	add r2, r3, #0
	bl sub_0802F808
	mov r1, #0
	cmp r0, #0
	ble _0802EA44
	mov r1, #1
_0802EA44:
	add r0, r1, #0
_0802EA46:
	pop {r4}
	pop {r1}
	bx r1
_0802EA4C: .4byte 0x00000431
	thumb_func_end sub_0802EA0C

