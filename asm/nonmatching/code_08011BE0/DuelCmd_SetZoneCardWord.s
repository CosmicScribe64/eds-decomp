	thumb_func_start DuelCmd_SetZoneCardWord
DuelCmd_SetZoneCardWord: @ 0x08012AE0
	push {r4, lr}
	sub sp, #4
	ldr r4, _08012B24 @ =0x020185C0
	ldrh r0, [r4]
	lsr r1, r0, #0xF
	ldrb r2, [r4, #2]
	ldrh r3, [r4, #6]
	lsl r0, r3, #0x10
	ldrh r3, [r4, #4]
	orr r0, r3
	str r0, [sp, #0]
	ldr r0, _08012B28 @ =0x00000D64
	mul r0, r1
	ldr r1, _08012B2C @ =0x0201930C
	add r0, r0, r1
	mov r1, #0x94
	mul r1, r2
	add r0, r0, r1
	mov r1, sp
	bl CopyDuelCard
	bl DrawAllAreaTiles
	ldr r0, _08012B30 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
_08012B24: .4byte 0x020185C0
_08012B28: .4byte 0x00000D64
_08012B2C: .4byte 0x0201930C
_08012B30: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetZoneCardWord

