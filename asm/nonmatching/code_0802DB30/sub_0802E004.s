	thumb_func_start sub_0802E004
sub_0802E004: @ 0x0802E004
	push {r4, lr}
	add r4, r0, #0
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _0802E020 @ =0x000005E7
	bl sub_08008524
	cmp r0, #0
	ble _0802E024
	mov r0, #0
	b _0802E048
_0802E020: .4byte 0x000005E7
_0802E024:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E050 @ =0x000007FF
	ldrh r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _0802E054 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	mov r1, #0
	cmp r0, #0
	ble _0802E046
	mov r1, #1
_0802E046:
	add r0, r1, #0
_0802E048:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802E050: .4byte 0x000007FF
_0802E054: .4byte gUnk_08622AB4
	thumb_func_end sub_0802E004

