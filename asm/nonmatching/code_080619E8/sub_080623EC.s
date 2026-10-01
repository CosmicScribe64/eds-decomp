	thumb_func_start sub_080623EC
sub_080623EC: @ 0x080623EC
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	cmp r0, #0
	bne _080623FC
	add r0, r2, #0
	bl sub_08062354
_080623FC:
	ldr r2, _08062418 @ =0x081A42A4
	lsl r0, r0, #3
	lsl r1, r4, #7
	add r0, r0, r1
	add r2, #4
	add r0, r0, r2
	ldr r1, _0806241C @ =0x0201CFB0
	ldr r0, [r0]
	ldrb r1, [r1, #4]
	sub r0, r0, r1
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08062418: .4byte gUnk_081A42A4
_0806241C: .4byte 0x0201CFB0
	thumb_func_end sub_080623EC

