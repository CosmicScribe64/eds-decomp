	thumb_func_start DrawCardImageIcon16
DrawCardImageIcon16: @ 0x0806196C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	mov r9, r1
	mov r8, r2
	add r7, r3, #0
	mov r0, r9
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r4, r5, #0x10
	lsr r4, r4, #0x10
	lsr r5, r5, #0x10
	lsl r6, r5, #0x10
	add r0, r4, #0
	orr r0, r6
	mov r1, r9
	bl DrawCardImageTile
	mov r0, #0x20
	add r8, r0
	mov r0, #8
	add r0, r0, r4
	mov sl, r0
	orr r6, r0
	add r0, r6, #0
	mov r1, r9
	mov r2, r8
	add r3, r7, #0
	bl DrawCardImageTile
	mov r0, #0x20
	add r8, r0
	add r5, #8
	lsl r5, r5, #0x10
	orr r4, r5
	add r0, r4, #0
	mov r1, r9
	mov r2, r8
	add r3, r7, #0
	bl DrawCardImageTile
	mov r0, #0x20
	add r8, r0
	mov r0, sl
	orr r0, r5
	mov sl, r0
	mov r1, r9
	mov r2, r8
	add r3, r7, #0
	bl DrawCardImageTile
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DrawCardImageIcon16

