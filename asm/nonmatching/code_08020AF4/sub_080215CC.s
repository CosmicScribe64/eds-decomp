	thumb_func_start sub_080215CC
sub_080215CC: @ 0x080215CC
	push {r4, lr}
	add r4, r0, #0
	mov r1, #0xBF
	lsl r1, r1, #3
	bl sub_08008524
	cmp r0, #0
	beq _08021620
	ldr r1, _08021614 @ =0x00000605
	add r0, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _08021620
	ldr r1, _08021618 @ =0x00000606
	add r0, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _08021620
	ldr r1, _0802161C @ =0x00000607
	add r0, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _08021620
	mov r1, #0xC1
	lsl r1, r1, #3
	add r0, r4, #0
	bl sub_08008524
	cmp r0, #0
	beq _08021620
	mov r0, #1
	b _08021622
	.align 2, 0
_08021614: .4byte 0x00000605
_08021618: .4byte 0x00000606
_0802161C: .4byte 0x00000607
_08021620:
	mov r0, #0
_08021622:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080215CC

