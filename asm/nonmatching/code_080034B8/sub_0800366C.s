	thumb_func_start sub_0800366C
sub_0800366C: @ 0x0800366C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	add r5, r2, #0
	cmp r5, #0x63
	ble _0800367E
	mov r5, #0x63
_0800367E:
	cmp r5, #0
	bge _08003684
	mov r5, #0
_08003684:
	add r4, r7, #0
	add r4, #8
	lsl r6, r1, #0x10
	orr r4, r6
	add r0, r5, #0
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	mov r0, #0xA0
	lsl r0, r0, #2
	mov r9, r0
	add r2, r9
	mov r0, #0xC0
	lsl r0, r0, #6
	mov r8, r0
	mov r0, r8
	orr r2, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0
	bl sub_080761F0
	add r0, r5, #0
	mov r1, #0xA
	bl __divsi3
	add r5, r0, #0
	cmp r5, #0
	ble _080036E0
	orr r6, r7
	mov r1, #0xA
	bl __modsi3
	add r2, r0, #0
	add r2, r9
	mov r0, r8
	orr r2, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r6, #0
	mov r1, #0
	bl sub_080761F0
	b _080036EC
_080036E0:
	orr r6, r7
	ldr r2, _080036F8 @ =0x000032A0
	add r0, r6, #0
	mov r1, #0
	bl sub_080761F0
_080036EC:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080036F8: .4byte 0x000032A0
	thumb_func_end sub_0800366C

