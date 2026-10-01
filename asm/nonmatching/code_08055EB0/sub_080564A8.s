	thumb_func_start sub_080564A8
sub_080564A8: @ 0x080564A8
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _080564D0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _080564D4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080564E0
	cmp r0, #0x17
	ble _080564D8
	cmp r0, #0x18
	beq _080564DC
	b _080564E0
_080564D0: .4byte 0x000007FF
_080564D4: .4byte gUnk_08621DE0
_080564D8:
	mov r0, #0
	b _080564F4
_080564DC:
	mov r0, #0xA
	b _080564F4
_080564E0:
	ldr r0, _08056514 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08056518 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080564F4:
	cmp r0, #0
	blt _0805651C
	cmp r0, #4
	ble _08056538
	cmp r0, #6
	bgt _0805651C
	mov r4, #1
	neg r4, r4
	add r0, r4, #0
	mov r1, #0
	bl sub_080563B8
	cmp r0, r4
	bne _08056538
	b _0805653C
	.align 2, 0
_08056514: .4byte 0x000007FF
_08056518: .4byte gUnk_08621DE0
_0805651C:
	mov r5, #1
	neg r5, r5
	add r0, r5, #0
	mov r1, #0
	bl sub_080563B8
	add r4, r0, #0
	mov r1, #0
	bl sub_080563B8
	cmp r4, r5
	beq _0805653C
	cmp r0, r5
	beq _0805653C
_08056538:
	mov r0, #1
	b _0805653E
_0805653C:
	mov r0, #0
_0805653E:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_080564A8

