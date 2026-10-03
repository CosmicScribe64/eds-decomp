	thumb_func_start GetZoneArea
GetZoneArea: @ 0x08062354
	add r2, r0, #0
	mov r1, #0
	sub r0, r2, #5
	cmp r0, #4
	bhi _08062360
	mov r1, #5
_08062360:
	cmp r2, #0xA
	bne _08062366
	mov r1, #0xA
_08062366:
	add r0, r1, #0
	bx lr
	thumb_func_end GetZoneArea
	.align 2, 0

