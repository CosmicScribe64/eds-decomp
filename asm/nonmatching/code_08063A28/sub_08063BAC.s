	thumb_func_start sub_08063BAC
sub_08063BAC: @ 0x08063BAC
	ldr r1, _08063BF8 @ =0x02011C20
	ldr r2, _08063BFC @ =0x000020D4
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #1
	ble _08063C10
	ldr r2, _08063C00 @ =0x000020D8
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #1
	ble _08063C10
	ldr r2, _08063C04 @ =0x000020DC
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #1
	ble _08063C10
	ldr r2, _08063C08 @ =0x000020E0
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #1
	ble _08063C10
	ldr r2, _08063C0C @ =0x000020E4
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #1
	ble _08063C10
	mov r0, #1
	b _08063C12
_08063BF8: .4byte 0x02011C20
_08063BFC: .4byte 0x000020D4
_08063C00: .4byte 0x000020D8
_08063C04: .4byte 0x000020DC
_08063C08: .4byte 0x000020E0
_08063C0C: .4byte 0x000020E4
_08063C10:
	mov r0, #0
_08063C12:
	bx lr
	thumb_func_end sub_08063BAC

