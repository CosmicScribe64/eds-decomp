	thumb_func_start sub_080779E8
sub_080779E8: @ 0x080779E8
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r5, _08077A18 @ =0x02011C20
	lsl r0, r4, #2
	add r0, r0, r5
	ldr r1, _08077A1C @ =0x000020D2
	add r3, r0, r1
	ldrh r2, [r3]
	lsr r1, r2, #6
	ldr r0, _08077A20 @ =0x000003FE
	cmp r1, r0
	bgt _08077A0C
	add r1, #1
	lsl r1, r1, #6
	mov r0, #0x3F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_08077A0C:
	ldr r1, _08077A24 @ =0x00002158
	add r0, r5, r1
	strh r4, [r0]
	pop {r4, r5}
	pop {r0}
	bx r0
_08077A18: .4byte 0x02011C20
_08077A1C: .4byte 0x000020D2
_08077A20: .4byte 0x000003FE
_08077A24: .4byte 0x00002158
	thumb_func_end sub_080779E8

