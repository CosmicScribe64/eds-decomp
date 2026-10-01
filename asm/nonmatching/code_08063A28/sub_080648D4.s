	thumb_func_start sub_080648D4
sub_080648D4: @ 0x080648D4
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r2, #0
	ldr r0, _080648EC @ =0x080865DC
	add r1, r0, #4
_080648E0:
	ldrh r4, [r0]
	cmp r4, r3
	bne _080648F0
	ldr r0, [r1]
	b _080648FC
	.align 2, 0
_080648EC: .4byte gUnk_080865DC
_080648F0:
	add r1, #0x48
	add r0, #0x48
	add r2, #1
	cmp r2, #0x16
	bls _080648E0
	mov r0, #0
_080648FC:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080648D4
	.align 2, 0

