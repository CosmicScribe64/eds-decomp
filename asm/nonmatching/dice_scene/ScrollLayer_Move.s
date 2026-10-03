	thumb_func_start ScrollLayer_Move
ScrollLayer_Move: @ 0x08026D84
	push {r4, r5, lr}
	add r2, r0, #0
	ldrb r3, [r2]
	mov r0, #7
	and r0, r3
	cmp r0, #0
	beq _08026DC0
	ldrh r0, [r2, #2]
	ldrh r4, [r2, #6]
	add r1, r0, r4
	strh r1, [r2, #6]
	lsl r0, r0, #0x10
	cmp r0, #0
	bge _08026DAC
	lsl r1, r1, #0x10
	ldrh r4, [r2, #4]
	lsl r0, r4, #0x10
	cmp r1, r0
	bgt _08026DC0
	b _08026DB6
_08026DAC:
	lsl r1, r1, #0x10
	ldrh r4, [r2, #4]
	lsl r0, r4, #0x10
	cmp r1, r0
	blt _08026DC0
_08026DB6:
	mov r0, #8
	neg r0, r0
	and r0, r3
	strb r0, [r2]
	strh r4, [r2, #6]
_08026DC0:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end ScrollLayer_Move
	.align 2, 0

