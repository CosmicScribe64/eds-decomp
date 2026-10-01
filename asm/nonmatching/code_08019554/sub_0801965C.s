	thumb_func_start sub_0801965C
sub_0801965C: @ 0x0801965C
	push {lr}
	add r2, r1, #0
	mov r3, #0xD4
	cmp r0, #0
	beq _08019668
	ldr r3, _08019678 @ =0x000080D4
_08019668:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	pop {r0}
	bx r0
_08019678: .4byte 0x000080D4
	thumb_func_end sub_0801965C

