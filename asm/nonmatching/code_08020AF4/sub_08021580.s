	thumb_func_start sub_08021580
sub_08021580: @ 0x08021580
	push {r4, lr}
	add r4, r0, #0
	mov r1, #0x10
	bl sub_0800A2A8
	cmp r0, #0
	beq _080215C2
	add r0, r4, #0
	mov r1, #0x11
	bl sub_0800A2A8
	cmp r0, #0
	beq _080215C2
	add r0, r4, #0
	mov r1, #0x12
	bl sub_0800A2A8
	cmp r0, #0
	beq _080215C2
	add r0, r4, #0
	mov r1, #0x13
	bl sub_0800A2A8
	cmp r0, #0
	beq _080215C2
	add r0, r4, #0
	mov r1, #0x14
	bl sub_0800A2A8
	cmp r0, #0
	beq _080215C2
	mov r0, #1
	b _080215C4
_080215C2:
	mov r0, #0
_080215C4:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08021580
	.align 2, 0

