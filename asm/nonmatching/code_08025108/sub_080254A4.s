	thumb_func_start sub_080254A4
sub_080254A4: @ 0x080254A4
	push {lr}
	add r1, r0, #0
	ldr r0, _080254C0 @ =0x0201F820
	ldr r2, _080254C4 @ =0x00000AEA
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #2
	beq _080254BA
	add r0, r1, #0
	bl sub_080786D0
_080254BA:
	pop {r0}
	bx r0
	.align 2, 0
_080254C0: .4byte 0x0201F820
_080254C4: .4byte 0x00000AEA
	thumb_func_end sub_080254A4

