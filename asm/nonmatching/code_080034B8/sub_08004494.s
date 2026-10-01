	thumb_func_start sub_08004494
sub_08004494: @ 0x08004494
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	add r6, r1, #0
	add r4, r2, #0
	bl sub_080042D8
	cmp r0, #0
	beq _080044D4
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_08004358
	ldr r7, _080044D8 @ =0x00007FFF
	and r0, r7
	cmp r0, #0
	bne _080044D4
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_080042D8
	cmp r0, #1
	bne _080044DC
	sub r2, r4, #1
	add r0, r5, #0
	add r1, r6, #0
	bl sub_08004358
	and r0, r7
	cmp r0, #0
	beq _080044DC
_080044D4:
	mov r0, #1
	b _080044DE
_080044D8: .4byte 0x00007FFF
_080044DC:
	mov r0, #0
_080044DE:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08004494

