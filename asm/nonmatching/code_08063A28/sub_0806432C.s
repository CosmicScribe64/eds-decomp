	thumb_func_start sub_0806432C
sub_0806432C: @ 0x0806432C
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r6, r1, #0
	lsl r5, r2, #0x10
	lsr r2, r5, #0x10
	mov r0, #0xC8
	lsl r0, r0, #4
	mov r1, #0
	bl sub_08072EB0
	cmp r4, r6
	bge _08064366
	lsr r2, r5, #0x11
	ldr r5, _0806436C @ =0x03001C5C
_08064348:
	mov r1, #0
	lsl r0, r4, #0x10
	add r4, #1
	lsr r3, r0, #0xB
_08064350:
	lsl r0, r1, #0x10
	lsr r0, r0, #0x10
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r0, r5
	strh r2, [r0]
	add r1, #1
	cmp r1, #0x1F
	ble _08064350
	cmp r4, r6
	blt _08064348
_08064366:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0806436C: .4byte 0x03001C5C
	thumb_func_end sub_0806432C

