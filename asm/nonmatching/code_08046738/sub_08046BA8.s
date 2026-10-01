	thumb_func_start sub_08046BA8
sub_08046BA8: @ 0x08046BA8
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r6, _08046BD8 @ =0x00000475
	add r1, r6, #0
	bl sub_08008524
	add r5, r0, #0
	cmp r5, #0
	ble _08046BD0
	lsl r0, r6, #1
	ldr r1, _08046BDC @ =0x08623DF4
	add r0, r0, r1
	ldrh r1, [r0]
	add r0, r4, #0
	bl sub_080197E0
	lsl r1, r5, #1
	add r0, r4, #0
	bl sub_080199E0
_08046BD0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08046BD8: .4byte 0x00000475
_08046BDC: .4byte gUnk_08623DF4
	thumb_func_end sub_08046BA8

