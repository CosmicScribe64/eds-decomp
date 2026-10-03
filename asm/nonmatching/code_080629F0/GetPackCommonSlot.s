	thumb_func_start GetPackCommonSlot
GetPackCommonSlot: @ 0x080629F0
	mov r2, #7
	add r1, r0, #0
	add r1, #0x38
_080629F6:
	ldr r0, [r1, #4]
	cmp r0, #0
	ble _08062A00
	add r0, r2, #0
	b _08062A0A
_08062A00:
	sub r1, #8
	sub r2, #1
	cmp r2, #0
	bgt _080629F6
	mov r0, #0
_08062A0A:
	bx lr
	thumb_func_end GetPackCommonSlot

