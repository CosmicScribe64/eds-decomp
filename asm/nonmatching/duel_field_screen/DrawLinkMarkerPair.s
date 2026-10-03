	thumb_func_start DrawLinkMarkerPair
DrawLinkMarkerPair: @ 0x0806120C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r4, r0, #0
	add r6, r1, #0
	mov r8, r2
	lsl r4, r4, #0x10
	lsl r6, r6, #0x10
	lsr r0, r6, #0x10
	mov r9, r0
	mov r1, r8
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	lsl r5, r4, #8
	lsr r5, r5, #0x18
	lsr r4, r4, #0x18
	add r0, r5, #0
	mov r1, #0
	add r2, r4, #0
	bl GetAreaX
	mov sl, r0
	mov r0, #8
	add sl, r0
	add r0, r5, #0
	mov r1, #0
	add r2, r4, #0
	bl GetAreaY
	add r4, r0, #0
	add r4, #8
	mov r1, r9
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r9, r1
	lsr r6, r6, #0x18
	mov r0, r9
	mov r1, #0
	add r2, r6, #0
	bl GetAreaX
	add r7, r0, #0
	add r7, #8
	mov r0, r9
	mov r1, #0
	add r2, r6, #0
	bl GetAreaY
	add r5, r0, #0
	add r5, #8
	lsl r4, r4, #0x10
	mov r0, sl
	orr r0, r4
	mov sl, r0
	ldr r4, _080612E8 @ =0x03000040
	ldr r1, _080612EC @ =0x0000485E
	add r4, r4, r1
	ldrh r1, [r4]
	lsr r0, r1, #3
	mov r1, #7
	mov r9, r1
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080612F0 @ =0x081A427C
	add r0, r0, r1
	ldr r2, [r0]
	add r2, r8
	mov r0, #0xA8
	lsl r0, r0, #7
	add r6, r0, #0
	add r2, r2, r6
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r0, sl
	mov r1, #0x40
	bl AddSprite
	lsl r5, r5, #0x10
	orr r7, r5
	ldrh r4, [r4]
	lsr r0, r4, #3
	mov r1, r9
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080612F0 @ =0x081A427C
	add r0, r0, r1
	ldr r0, [r0]
	add r8, r0
	add r8, r6
	mov r0, r8
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	add r0, r7, #0
	mov r1, #0x40
	mov r2, r8
	bl AddSprite
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080612E8: .4byte 0x03000040
_080612EC: .4byte 0x0000485E
_080612F0: .4byte gZoneMarkerAnimTiles
	thumb_func_end DrawLinkMarkerPair

