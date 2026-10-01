	thumb_func_start sub_080240A8
sub_080240A8: @ 0x080240A8
	push {lr}
	ldr r2, _080240CC @ =0x081A43A4
	lsl r1, r1, #1
	lsl r0, r0, #5
	add r1, r1, r0
	add r1, r1, r2
	ldr r0, _080240D0 @ =0x0201CFB0
	ldrh r2, [r1]
	ldrb r0, [r0, #6]
	ldrb r1, [r1]
	cmp r0, r1
	beq _080240C6
	add r0, r2, #0
	bl sub_0802408C
_080240C6:
	pop {r0}
	bx r0
	.align 2, 0
_080240CC: .4byte gUnk_081A43A4
_080240D0: .4byte 0x0201CFB0
	thumb_func_end sub_080240A8

