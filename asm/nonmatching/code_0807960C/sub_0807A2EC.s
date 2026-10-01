	thumb_func_start sub_0807A2EC
sub_0807A2EC: @ 0x0807A2EC
	push {r4, lr}
	add r3, r0, #0
	mov r2, #0
	mov r4, #0xFF
_0807A2F4:
	add r1, r3, r2
	ldrb r0, [r1]
	orr r0, r4
	strb r0, [r1]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0x13
	bls _0807A2F4
	ldr r0, _0807A31C @ =0x00000614
	add r1, r3, r0
	mov r0, #0x80
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0807A31C: .4byte 0x00000614
	thumb_func_end sub_0807A2EC

