	thumb_func_start AiCountExodiaOnField
AiCountExodiaOnField: @ 0x080578AC
	push {r4, r5, r6, lr}
	mov r2, #0
	mov r3, #0
	ldr r5, _080578E8 @ =0x0201A070
	ldr r4, _080578EC @ =0x000007FF
	mov r1, #0
_080578B8:
	add r0, r1, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _080578D8
	and r0, r4
	lsl r0, r0, #1
	ldr r6, _080578F0 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, #0x14
	bgt _080578D8
	cmp r0, #0x10
	blt _080578D8
	add r3, #1
_080578D8:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _080578B8
	add r0, r3, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_080578E8: .4byte 0x0201A070
_080578EC: .4byte 0x000007FF
_080578F0: .4byte gCardIdToNumber
	thumb_func_end AiCountExodiaOnField

