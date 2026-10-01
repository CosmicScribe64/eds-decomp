	thumb_func_start sub_0802E3EC
sub_0802E3EC: @ 0x0802E3EC
	push {lr}
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	ldr r1, _0802E418 @ =0x000005E7
	bl sub_08008524
	cmp r0, #0
	bgt _0802E414
	ldr r1, _0802E41C @ =0x020192E4
	ldrb r0, [r1, #4]
	cmp r0, #0
	bne _0802E424
	ldr r2, _0802E420 @ =0x00000D68
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0
	bne _0802E424
_0802E414:
	mov r0, #0
	b _0802E426
_0802E418: .4byte 0x000005E7
_0802E41C: .4byte 0x020192E4
_0802E420: .4byte 0x00000D68
_0802E424:
	mov r0, #1
_0802E426:
	pop {r1}
	bx r1
	thumb_func_end sub_0802E3EC
	.align 2, 0

