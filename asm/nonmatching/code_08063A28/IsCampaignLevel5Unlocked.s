	thumb_func_start IsCampaignLevel5Unlocked
IsCampaignLevel5Unlocked: @ 0x08063CE4
	ldr r1, _08063D30 @ =0x02011C20
	ldr r2, _08063D34 @ =0x00002110
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	ble _08063D48
	ldr r2, _08063D38 @ =0x00002114
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	ble _08063D48
	ldr r2, _08063D3C @ =0x00002118
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	ble _08063D48
	ldr r2, _08063D40 @ =0x0000211C
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	ble _08063D48
	ldr r2, _08063D44 @ =0x00002120
	add r0, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	ble _08063D48
	mov r0, #1
	b _08063D4A
_08063D30: .4byte 0x02011C20
_08063D34: .4byte 0x00002110
_08063D38: .4byte 0x00002114
_08063D3C: .4byte 0x00002118
_08063D40: .4byte 0x0000211C
_08063D44: .4byte 0x00002120
_08063D48:
	mov r0, #0
_08063D4A:
	bx lr
	thumb_func_end IsCampaignLevel5Unlocked

