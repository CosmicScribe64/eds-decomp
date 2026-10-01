	thumb_func_start sub_080227CC
sub_080227CC: @ 0x080227CC
	push {r4, lr}
	add r2, r0, #0
	add r3, r1, #0
	ldr r4, _080227F0 @ =0x020192E0
	ldr r0, _080227F4 @ =0x00001B50
	add r1, r4, r0
	ldr r0, _080227F8 @ =0x000003F2
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0x42
	bne _08022800
	ldr r2, _080227FC @ =0x00001B54
	add r1, r4, r2
	ldrh r2, [r1]
	add r0, r2, r3
	strh r0, [r1]
	b _0802280E
	.align 2, 0
_080227F0: .4byte 0x020192E0
_080227F4: .4byte 0x00001B50
_080227F8: .4byte 0x000003F2
_080227FC: .4byte 0x00001B54
_08022800:
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r2, #0
	mov r1, #4
	mov r2, #0
	bl sub_08022678
_0802280E:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_080227CC

