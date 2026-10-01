	thumb_func_start sub_08073498
sub_08073498: @ 0x08073498
	push {r4, r5, lr}
	ldr r5, _080734CC @ =0x0300045C
	mov r4, #7
_0807349E:
	add r0, r5, #0
	mov r1, #0x80
	lsl r1, r1, #4
	bl sub_08075278
	mov r0, #0x80
	lsl r0, r0, #4
	add r5, r5, r0
	sub r4, #1
	cmp r4, #0
	bge _0807349E
	ldr r4, _080734D0 @ =0x02010014
	mov r1, #0xE0
	lsl r1, r1, #5
	add r0, r4, #0
	bl sub_08075278
	sub r4, #4
	mov r0, #0
	strh r0, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
_080734CC: .4byte 0x0300045C
_080734D0: .4byte 0x02010014
	thumb_func_end sub_08073498

