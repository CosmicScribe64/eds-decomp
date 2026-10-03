	thumb_func_start PutMapTileRun
PutMapTileRun: @ 0x080792A0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	ldr r1, [sp, #0x20]
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r2, r2, #0x18
	lsr r0, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov ip, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r9, r1
	mov r6, #0
	mov r7, #0
	cmp r7, r9
	bcs _08079330
	mov r1, #0
	mov sl, r1
	lsl r4, r0, #0xC
_080792D0:
	mov r0, ip
	cmp r0, #0
	beq _080792DC
	cmp r0, #1
	beq _080792F2
	b _08079320
_080792DC:
	add r0, r6, #0
	add r1, r0, #1
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	add r0, sl
	lsl r0, r0, #1
	add r0, r8
	add r1, r5, #0
	orr r1, r4
	strh r1, [r0]
	b _08079320
_080792F2:
	mov r0, sl
	add r1, r0, r6
	lsl r1, r1, #1
	add r1, r8
	lsl r2, r5, #2
	add r0, r4, #0
	orr r0, r2
	strh r0, [r1]
	add r0, r2, #1
	orr r0, r4
	strh r0, [r1, #2]
	add r3, r1, #0
	add r3, #0x40
	add r0, r2, #2
	orr r0, r4
	strh r0, [r3]
	add r1, #0x42
	add r2, #3
	orr r2, r4
	strh r2, [r1]
	add r0, r6, #2
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
_08079320:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, r9
	bcc _080792D0
_08079330:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end PutMapTileRun
	.align 2, 0

