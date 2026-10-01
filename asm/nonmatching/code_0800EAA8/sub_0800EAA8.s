	thumb_func_start sub_0800EAA8
sub_0800EAA8: @ 0x0800EAA8
	push {r4, lr}
	ldr r4, _0800EAD4 @ =0x020185C0
	ldrh r0, [r4, #4]
	ldrh r1, [r4, #2]
	mov r2, #1
	bl sub_0800935C
	bl sub_0805F96C
	bl sub_080611AC
	ldr r0, _0800EAD8 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800EAD4: .4byte 0x020185C0
_0800EAD8: .4byte 0x0000080D
	thumb_func_end sub_0800EAA8

