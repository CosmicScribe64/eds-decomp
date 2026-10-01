	thumb_func_start sub_08063C14
sub_08063C14: @ 0x08063C14
	ldr r1, _08063C60 @ =0x02011C20
	ldr r2, _08063C64 @ =0x000020E8
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #2
	ble _08063C78
	ldr r2, _08063C68 @ =0x000020EC
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #2
	ble _08063C78
	ldr r2, _08063C6C @ =0x000020F0
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #2
	ble _08063C78
	ldr r2, _08063C70 @ =0x000020F4
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #2
	ble _08063C78
	ldr r2, _08063C74 @ =0x000020F8
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #2
	ble _08063C78
	mov r0, #1
	b _08063C7A
_08063C60: .4byte 0x02011C20
_08063C64: .4byte 0x000020E8
_08063C68: .4byte 0x000020EC
_08063C6C: .4byte 0x000020F0
_08063C70: .4byte 0x000020F4
_08063C74: .4byte 0x000020F8
_08063C78:
	mov r0, #0
_08063C7A:
	bx lr
	thumb_func_end sub_08063C14

