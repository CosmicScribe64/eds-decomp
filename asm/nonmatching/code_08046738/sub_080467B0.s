	thumb_func_start sub_080467B0
sub_080467B0: @ 0x080467B0
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r1, #0xA5
	bl sub_080090C8
	add r4, r0, #0
	mov r0, #1
	sub r7, r0, r6
	add r0, r7, #0
	mov r1, #0xA5
	bl sub_080090C8
	add r5, r0, #0
	cmp r4, #0
	bgt _080467D2
	cmp r5, #0
	ble _080467FC
_080467D2:
	ldr r0, _08046804 @ =0x08623F3E
	ldrh r1, [r0]
	add r0, r6, #0
	bl sub_080197E0
	lsl r1, r4, #5
	sub r1, r1, r4
	lsl r1, r1, #2
	add r1, r1, r4
	lsl r1, r1, #2
	add r0, r6, #0
	bl sub_08019980
	lsl r1, r5, #5
	sub r1, r1, r5
	lsl r1, r1, #2
	add r1, r1, r5
	lsl r1, r1, #2
	add r0, r7, #0
	bl sub_08019980
_080467FC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08046804: .4byte gUnk_08623F3E
	thumb_func_end sub_080467B0

