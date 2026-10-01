	thumb_func_start sub_08027CDC
sub_08027CDC: @ 0x08027CDC
	push {r4, r5, r6, lr}
	add r2, r0, #0
	mov r3, #0
	ldrb r4, [r2]
	ldr r6, _08027D18 @ =0x080826DC
	mov r5, #0
_08027CE8:
	add r0, r3, r6
	ldrb r1, [r0]
	cmp r4, r1
	blt _08027D06
	mov r0, #1
	ldsb r0, [r2, r0]
	cmp r0, #0
	bge _08027CFA
	neg r0, r0
_08027CFA:
	add r0, r1, r0
	cmp r4, r0
	bge _08027D06
	strb r5, [r2, #1]
	strb r1, [r2]
	strb r3, [r2, #2]
_08027D06:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #2
	bls _08027CE8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08027D18: .4byte gUnk_080826DC
	thumb_func_end sub_08027CDC

