	thumb_func_start sub_08063C7C
sub_08063C7C: @ 0x08063C7C
	ldr r1, _08063CCC @ =0x02011C20
	ldr r2, _08063CD0 @ =0x000020FC
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #3
	ble _08063CE0
	mov r2, #0x84
	lsl r2, r2, #6
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #3
	ble _08063CE0
	ldr r2, _08063CD4 @ =0x00002104
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #3
	ble _08063CE0
	ldr r2, _08063CD8 @ =0x00002108
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #3
	ble _08063CE0
	ldr r2, _08063CDC @ =0x0000210C
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #3
	ble _08063CE0
	mov r0, #1
	b _08063CE2
	.align 2, 0
_08063CCC: .4byte 0x02011C20
_08063CD0: .4byte 0x000020FC
_08063CD4: .4byte 0x00002104
_08063CD8: .4byte 0x00002108
_08063CDC: .4byte 0x0000210C
_08063CE0:
	mov r0, #0
_08063CE2:
	bx lr
	thumb_func_end sub_08063C7C

