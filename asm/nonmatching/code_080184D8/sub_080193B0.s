	thumb_func_start sub_080193B0
sub_080193B0: @ 0x080193B0
	push {lr}
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r3, #0xC3
	cmp r0, #0
	beq _080193BE
	ldr r3, _080193D0 @ =0x000080C3
_080193BE:
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	pop {r0}
	bx r0
	.align 2, 0
_080193D0: .4byte 0x000080C3
	thumb_func_end sub_080193B0

